#include <move_failures.h>
#define STATE_CHANGE "/obj/handlers/state_change"


inherit "/obj/container";

int test_add(object ob, int flag);
int do_grind( string what );
int do_empty(object *dest, string me, string him, string prep);
void set_max_grindable( int i );
int query_max_grindable();
string long_status();

private int _max_grind = 0;


void setup() {
  set_name( "grinder" );
  set_short( "food grinder" );
  add_adjective( "food" );
  set_main_plural( "food grinders" );
  set_long( "This is a mechanical contraption to grind food into small "
      "pieces.  There is a funnel-type piece at the top, a crank on one "
      "side and on the other a pipe where the ground food comes out and "
      "is caught in a small tray.\n" );
  set_value( 4000 );
  set_weight( 100 );
  set_max_weight( 100 );
} /* setup() */


void init() {
  this_player()->add_command( "grind", this_object(),
      "<string'object'> in <direct:object'" + query_short() + "'>",
      (: do_grind( $4[0] ) :) );
/*
  this_player()->add_command( "grind", this_object(),
      "<indirect:object:me> in <direct:object>",
      (: do_grind( $1[0] ) :) );
*/
  add_command( "empty", "<direct:object>" );
} /* init() */


int test_add(object ob, int flag) {
  object stuff = first_inventory();
  if (stuff) {
    add_failed_mess( "There is already some " + stuff->query_short()
      + " in the grinder.  You'll have to empty it first.\n");
    return 0;
  } else return 1;
} /* test_add() */


int do_grind( string what ) {
  object *things, thing;
  object ground;
  object contents;

  things = match_objects_for_existence( what, ({ this_object(),
                                                 this_player() }) );
  if( !sizeof( things ) ) {
    add_failed_mess( "You must be holding something or put it in $D in "
                     "order to $V it.\n" );
    return -1;
  }

  thing = things[0];

  /* If there's already something in the grinder, don't let more be ground
   * until it's removed. */

  if( ( contents = first_inventory() ) && contents != thing ) {
   // tell_creator( this_player(), "contents: %O\tthing: %O\n",
   //               contents, thing );
    add_failed_mess( "$D isn't empty.\n" );
    return -1;
  }

  /* If there's a limit on the amount that can be ground at once, make
   * sure that we haven't exceeded it.  Non-continuous things are checked
   * by weight. */

  if( query_max_grindable() &&
      ( thing->query_amount() > query_max_grindable() ||
        thing->query_weight() > query_max_grindable() ) ) {
    add_failed_mess( "You cannot grind that much with $D at once.\n" );
    return -1;
  }

  ground = STATE_CHANGE->transform( thing, "grind" );
  if ( !objectp( ground ) ) {
    add_failed_mess( "You cannot $V $I in $D.\n", ({ thing }) );
    return -1;
  }

  thing->move( "/room/rubbish" );

  if( ground->move( this_object() ) != MOVE_OK ) {
    // couldn't be moved into the grinder
    ground->move( environment( this_player() ) );
    add_succeeded_mess( "$N grind$s $I with $D, spilling it onto the " +
        ( environment( this_player() )->query_property( "location" ) ==
        "inside" ? "floor" : "ground" ) + ".\n" );
    return 1;
  }

  this_player()->add_succeeded_mess( this_object(), "$N grind$s $I "
    "in $D.\n", ({ thing }) );
  return 1;
} /* do_grind() */


int do_empty(object *dest, string me, string him, string prep) {
  /* There shouldn't ever be more than one thing in the grinder. */
  object ground = first_inventory( this_object() );

  if( !ground ) {
    add_failed_mess( "There is nothing in $D.\n" );
    return 0;
  }
  /* If the player can't hold it, empty it onto the floor */
  if( ground->move( this_player() ) != MOVE_OK ) {
    ground->move( environment( this_player() ) );
    add_succeeded_mess( "$N $V $D onto the " +
        ( environment( this_player() )->query_property( "location" ) ==
        "inside" ? "floor" : "ground" ) + ".\n" );
    return 1;
  }

  /* standard success msg */
  return 1;
} /* do_empty() */


/**
 * To set a maximum amount of an object that can be ground at once.
 * If left with the default value of 0, there is no maximum amount.
 */

void set_max_grindable( int i ) { _max_grind = i; }


/**
 * Returns the maximum amount grindable at once
 */

int query_max_grindable() { return _max_grind; }


/* This gets rid of the "It is open." message */
string long_status() { return ""; }
