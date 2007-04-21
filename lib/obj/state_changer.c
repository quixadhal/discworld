/**
 * State Changer Inheritable!
 * You can use this to write a device which will change the states of
 * things.
 * @author Terano
 * @started 30/9/02
 */

#include <tasks.h>
#include <effect.h>
#define STATE_CHANGE "/obj/handlers/state_change"

inherit "/obj/vessel";

object *add_list = ({ }); //List of cloned objects.

int query_busy();
int do_action( object *obs );
string parse_message( string message );
string query_classification();

string command;
string process;
string start_mess;
string interim_mess;
string end_mess;

int duration;
int base_bonus;
string skill;
string fail_process;
string end_mess_fail;
string end_mess_abort;
string command_mess;

function start_func;
function end_func; 
function fail_func;
int gp_cost;
string gp_type = "other";

string running_inv_mess;

void create() {
    ::create();
    set_stuck( 1 );
    set_leak_rate( 0 );
    set_weight( 100 );
    set_max_weight( 5000 );
    set_max_volume( 5000 );
    set_hide_cond( 1 );
    this_object()->add_extra_look(this_object());
}

//Set command name! Required.
void set_command( string _command ) { command = _command; }

//Set process name! Required.
void set_process( string _process ) { process = _process; }

//Set fail process name! Optional.
void set_fail_process( string _process ) { fail_process = _process; }

//Start, Interim and End messages are required! Fail messages are optional.
void set_start_message( string message ) { start_mess = message; }
void set_interim_message( string message ) { interim_mess = message; }
void set_end_message( string message ) { end_mess = message; }
void set_fail_message( string message ) { end_mess_fail = message; }
void set_abort_message( string message ) { end_mess_abort = message; }

//Duration! Required.
void set_duration( int _duration ) { duration = _duration; }

//Set add_succeeded_mess for command. Optional!
void set_command_mess( string message ) { command_mess = message; }

//Called when we start, return 1 for 'ok to continue'. Optional!
void set_start_func( function f ) { start_func = f; }

//Called when we end, return 1 for 'ok to continue'. Optional!
void set_end_func( function f ) { start_func = f; }

//Called if we fail.
void set_fail_func( function f) { fail_func = f; }

//Set skill and bonus to use! Optional!
void set_skill( string _skill, int _base_bonus ) { 
    skill = _skill;
    base_bonus = _base_bonus;
}

void set_gp_cost( int gp ) { gp_cost = gp; }

void set_gp_type( string type ) { gp_type = type; }

//Set a message to be displayed as the inventory when it is running.
//This can use the tag $inventory$ for dynamicness.
void set_running_inv_mess( string mess ) { running_inv_mess = mess; }

void init() {
  ::init();
  if ( command ) {
    add_command( command, "all in <direct:object>", (: do_action( $1 ) :) );
  }
}

int do_action( object *obs ) {
int *tmp;

  if ( gp_cost ) {
      if ( this_player()->query_specific_gp( gp_type ) < gp_cost ) 
          return notify_fail( "You are too tired to "+ command +" with the "+ this_object()->the_short() + ".\n" );
      this_player()->adjust_gp( -gp_cost );
  }

  if ( sizeof( tmp = this_object()->effects_matching( query_classification() ) ) )
    return notify_fail( this_object()->the_short() + " is already being used by "+ 
      this_object()->arg_of( tmp[0] )->query_short() +"!\n" );

  if ( !sizeof( all_inventory( this_object() ) ) )
    return notify_fail( "There is nothing to "+ command + ".\n" );

  //If there is a 'start_func' which returns 0, then we stop. We are relying
  //on the start func to show a 'fail message' since we can't really get one.
  if ( functionp( start_func ) ) {
    if ( !evaluate( start_func ) ) {
        debug_printf( "Stopped at start_func\n" );
        return notify_fail( "" );
    
    }
  }

  //Start the process.
  this_object()->add_effect( file_name( this_object() ), this_player() );

  if ( stringp( start_mess ) )
      tell_room( environment(), parse_message(start_mess) );

  if ( stringp( command_mess ) ) {
      this_player()->add_succeeded_mess( this_object(), parse_message( command_mess ), 
          all_inventory( this_object() ) );
    debug_printf( "Setting asm to %s.\n", parse_message( command_mess ) );
  }
  else
      this_player()->add_succeeded_mess( this_object(), "$N "+ command +"$s $I "
        "in $D.\n", all_inventory( this_object() ) );
  return 1;
}

/**
 * Some effect stuff to save on call outs.
 */
object beginning(object apparatus, object tp, int id) {
    apparatus->submit_ee(0, duration, EE_REMOVE);
    apparatus->submit_ee("show_message", ({ 30, 30 }), EE_CONTINUOUS);
    return tp;
} /* beginning() */ 

object show_message(object apparatus, object tp, int id) {    
  if ( stringp( interim_mess ) )
      tell_room(environment(), parse_message(interim_mess) );

  //If we have run out of stuff, END IT NOW.
  if ( !sizeof( all_inventory() ) )
    submit_ee( 0, 0, EE_REMOVE );

  return tp;
} /* merge_effect() */

object end(object apparatus, object tp, int id) {
 object ob;
 int temp;
 int l_bonus = base_bonus;

  //If there is a 'start_func' which returns 0, then we stop. We are relying
  //on the start func to show a 'fail message' since we can't really get one.
  if ( functionp( end_func ) ) {
    if ( !evaluate( end_func ) ) {
        debug_printf( "Stopped at start_func\n" );
        return tp;
    }
  }

  //If we run out of stuff to do, abort!
  if ( !sizeof( all_inventory() ) ) {
    debug_printf( "Aborted due to lack of inventory!\n" );
    if ( stringp( end_mess_abort ) )
      tell_room( environment(), parse_message(end_mess_abort) );
    return tp;
  }

  foreach( object i in all_inventory() ) {
      if ( temp = i->query_property( process + " modifier" ) ) {
          l_bonus += temp;
          debug_printf( "%s difficulty will be modified by %d (%O).\n", 
             process, temp, ob );
      }
  }

  if ( skill ) {
      switch( TASKER->perform_task( tp, skill, l_bonus, TM_COMMAND ) ) {
        case AWARD:
          tell_object( tp, "%^YELLOW%^You learn something new about "+ process +"ing!%^RESET%^\n" );
        case SUCCEED:
          tell_room( environment(), parse_message(end_mess) );
          foreach( object i in all_inventory() ) {
            if (!i)
              continue;
        
              ob = STATE_CHANGE->transform( i, process, tp );
              debug_printf( "Transforming: %O to %O.\n", i, ob );
              if ( ob ) {
                add_list += ({ ob });
                i->move( "/room/rubbish" );
                ob->move( this_object() );
              }
           }
           break;
        default:
            if ( stringp( end_mess ) )
                tell_room( environment(), parse_message(end_mess_fail) );

            if ( functionp( fail_func ) )
                evaluate( fail_func );

            foreach( object i in all_inventory() ) {
              if (!i)
                continue;
        
              ob = STATE_CHANGE->transform( i, fail_process, tp );
              debug_printf( "Transforming: %O to %O.\n", i, ob );
              if ( ob ) {
                add_list += ({ ob });
                i->move( "/room/rubbish" );
                ob->move( this_object() );
              }
           }
           break;  
      }
  }
  else {
    if ( stringp( end_mess ) )
        tell_room( environment(), parse_message(end_mess) );
    foreach( object i in all_inventory() ) {
      if (!i)
        continue;
    
      ob = STATE_CHANGE->transform( i, process, tp );
      debug_printf( "Transforming: %O to %O.\n", i, ob );
      if ( ob ) {
        add_list += ({ ob });
        i->move( "/room/rubbish" );
      }
      //Move the objects and clear the ob_list
      add_list->move( this_object() );
      add_list = ({ });
    }
  }

  return tp;
} /* beginning() */

string query_classification() { return "state.changing."+ process; }

/**
 * Something to mask!
 */
string extra_look( object ob ) {
  return "";
}

string parse_message( string message ) {
  message = replace( message, "$inventory$", query_multiple_short( all_inventory() ) );
  debug_printf( "message is: %s.\n" , message );
  return message;
}

int test_add(object ob, int flag) {

    debug_printf( "Testing add of: %O (%O) in state changer.\n", ob, environment( ob ) );

    //If the food has been cloned by this thingy.
    if ( member_array( ob, add_list ) != -1 ) {
       return ::test_add( ob, flag );
    }

    if ( query_busy() ) {
        tell_object( this_player(), "It might not be wise to add anything to "+ 
            this_object()->the_short() + " while it is in use!\n" );
        return 0;
    }
    return ::test_add( ob, flag );
}


int test_remove( object ob, int flag, mixed dest ) {

    if ( dest && base_name( dest ) == "/room/rubbish" )
        ::test_remove( ob, flag, dest );

    debug_printf( "Testing remove of: %O (%O) in state changer.\n", ob, environment( ob ) );

    if ( query_busy() ) {
        tell_object( this_player(), "It might not be wise to take anything from "+ 
            this_object()->the_short() + " while it is in use!\n" );
        return 0;
    }
    return ::test_remove( ob, flag, dest );
}


string long( string word, int dark ) {
  //If it's running!
  if ( query_busy() ) {
    if ( stringp( running_inv_mess ) )
      return query_long() + parse_message( running_inv_mess );
    else
      return ::long( word, dark );
  }
  return ::long( word, dark );
}

int do_empty(object *dest, string me, string him, string prep) {
  if ( query_busy() ) {
    add_failed_mess( "You can't empty "+ this_object()->short() +" while it is being used.\n" );
    return 0;
  }
  return ::do_empty( dest, me, him, prep );
}

int do_fill(object *to, mixed *args_b, mixed *args_a, mixed *args) {

  if ( query_busy() ) {
    add_failed_mess( "You can't fill anything from "+ this_object()->short() 
      +" while it is being used.\n" );
    return 0;
  }
  return  ::do_fill( to, args_b, args_a, args );
}


int query_busy() { return !!sizeof( this_object()->effects_matching( query_classification() ) ); }
