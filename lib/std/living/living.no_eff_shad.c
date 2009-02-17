/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: living.c,v 1.72 2003/03/07 03:24:37 ceres Exp $
 *
 */
/**
 * The main living inherit.  This inherits all the files
 * needed to be in a living object.
 * @author Pinkfish
 */
#include <dirs.h>
#include <drinks.h>
#include <living.h>
#include <move_failures.h>
#include <player.h>
#include <tune.h>
#include <position.h>
#include <obj_parser.h>

inherit "/std/container";
inherit "/std/living/armour";
inherit "/std/living/carrying";
inherit "/std/living/combat";
inherit "/std/living/force";
inherit "/std/living/gender";
inherit "/std/living/health";
inherit "/std/living/holding";
inherit "/std/living/money";
inherit "/std/living/skills";
inherit "/std/living/spells";
inherit "/std/living/crafts";
inherit "/std/living/stats";
inherit "/std/living/respond_cmd";
inherit "/std/living/nationality";

#define VERBOSE_TYPES ({"combat", "look", "score", "names", "htell", "finger"})

//The maximum amount of religious favour you can have.
#define MAX_FAVOUR 100

class living_data {
  int handicap;
  int burden;
  object* followers;
  class obj_match_context it_them;
  object* to_drop;
  int burden_call;
}

class messages {
   string msgout;
   string msgin;
   string mmsgout;
   string mmsgin;
}

private int alignment;
private class messages _messages;
private mixed *facing;
private mapping verbose;
private mapping abs_facing = ABS_FACING;
private mapping lengthen = LENGTHEN;

//Got religion?
private string deity;         //Your deity of choice.
private mapping deity_favour; //A mapping of deity ratings.

//private nosave object _dragging;
private nosave class living_data _liv_data;
//private nosave int _handicap;
//private nosave object *_followers;
//private nosave class obj_match_context _it_them;
//private nosave object* _to_drop;
//private nosave int _burden_call;

void return_to_default_position(int leaving);
void set_position(string name);
void set_position_on(mixed ob);
void set_position_multiple(int mult);
void set_position_type(string type);
object query_position_on();
protected mixed command(string);

/*
 * What position are they in?  Standing, sitting, lying, froging?
 */
private nosave string position;
private nosave mixed default_position;
private /* This will force an object to not use the position from the room. */
nosave int always_use_default_position;
/* in case they are standing on something or something... */
private nosave mixed position_on;

void create() {
   string t;

   _liv_data = new(class living_data);
   _messages = new(class messages);
   crafts::create();
   container::create();
   armour::create();
   combat::create();
   health::create();
   holding::create();
   skills::create();
   spells::create();
   respond_cmd::create();
   enable_commands();
   reset_can_export_inventory();
   reset_effects();
   _messages->msgin = "$N arrive$s from $F.";
   _messages->msgout = "$N leave$s $T.";
   _messages->mmsgin = "$N appear$s out of the ground.";
   _messages->mmsgout = "$N disappear$s in a puff of smoke.";
   _liv_data->followers = ({ });
   verbose = ([ ]);
   set_max_items(50);
   _liv_data->to_drop = ({ });
   foreach(t in VERBOSE_TYPES) {
      verbose[t] = 1;
   }

   facing = ({ 0, ({ "north", "northeast", "east", "southeast", "south",
         "southwest", "west", "northwest" }), ({ "up", "down" }) });
   position = STANDING;

   add_adjective("living");
} /* create() */

/** @ignore yes */
void heart_beat() {
  stats::heart_beat();
  combat::heart_beat();
}

/**
 * This method returns the pronoun string of the living object.
 * A pronoun is "he", "she", "it".
 * @return the pronoun string
 */
string query_pronoun() {
   return gender::query_pronoun();
} /* query_pronoun() */

/**
 * This method returns the prossessive string of the living object.
 * A possessive is "her", "his", "its".
 * @return the possessive string
 */
string query_possessive() {
   return gender::query_possessive();
} /* query_possessive() */

/**
 * This method returns the objective string of the living object.
 * An objective is "her", "him", "it".
 * @return the objective string
 */
string query_objective() {
   return gender::query_objective();
} /* query_objective() */

/** @ignore yes */
varargs int query_weight(int actual) {
  if(!actual && this_object()->query_property("dead"))
    return 0;
  return stats::query_weight();
}

/**
 * This method returns the current burden level of the living
 * object.  This is returned as a percentage of the maximum
 * weight that the living can carry.
 * @return the burden level (0-100)
 */
int query_burden() {
  return _liv_data->burden;
} /* query_burden() */

/**
 * This method returns the current handicap of the living
 * object.
 * @return the current handicap.
 * @see calc_burden()
 */
int query_handicap() { return _liv_data->handicap; }

/**
 * This method calculates the current handicap of the living
 * object.  The handicap is based on the burden of the
 * person, the more burdened the higher the handicap.  The
 * handicap is 1 point of dexterity for every 25% burdened.
 * @see query_handicap()
 * @see query_burden()
 */
void calc_burden() {
  int new_handicap;
  int burden;
  object thing;
  mapping hands;
  
  _liv_data->burden_call = 0;
  
  update_loc_weight();
  burden = query_loc_weight();
  foreach(thing in query_wearing())
    burden -= (int)thing->query_complete_weight() / 2;

  hands = ([ ]);
  foreach(thing in query_holding())
    if(objectp(thing))
      hands[thing]++;

  foreach(thing in keys(hands))
    burden += (int)thing->query_complete_weight() / hands[thing];

  if(!query_max_weight()) {
    this_object()->reset_carry_cap();
    _liv_data->burden = 50;
  } else
    _liv_data->burden = (100 * burden) / query_max_weight();
  
  new_handicap = (_liv_data->burden / 25) - 1;
  if(new_handicap < 0)
    new_handicap = 0;
  
  if ( _liv_data->handicap != new_handicap ) {
    adjust_bonus_dex( _liv_data->handicap - new_handicap );
    _liv_data->handicap = new_handicap;
  }
}

/**
 * This method returns the string representation of the current
 * burden level.
 * @return the burden string
 * @see calc_burden()
 * @see query_burden()
 */
string burden_string() {
  switch (_liv_data->handicap) {
  case 0 :
    return "unburdened";
  case 1 :
    return "burdened";
  case 2 :
    return "heavily burdened";
  case 3 :
    return "very heavily burdened";
  default :
    return "incredibly heavily burdened";
  }
} /* burden_string() */

/**
 * This method adds any commands needed by the living inherit.
 */
void living_commands() {
#if efun_defined(add_action)
   add_action( "exit_command", "*", 1 );
#endif
} /* living_commands() */

/**
 * This method allows you to make the living object eat something.
 * @param food the food object to eat
 */
void eat_this( object food ) {
  this_object()->bypass_queue();
  command( "eat " + file_name( food ) );
} /* eat_this() */

/**
 * This method handles the cannot get flags.  This is placed
 * into the living object so that things which are marked as
 * being unable to be picked up can still be put into normal
 * containers.  The upwards checking of containers stops here.
 * This should make it so that objects marked as unable to be
 * picked up cannot be put into objects in the players
 * inventory.
 * @return 1 if the object can be added, 0 if not.
 */
int test_add(object ob, int flag) {
  if(sizeof(all_inventory(this_object())) > query_max_items())
    return 0;
  
  return !flag;
}

/**
 * This method handles the cannot drop flags.  This is placed
 * into the living object so that things which are marked as
 * being unable to be dropped can still be remove from normal
 * containers.  This does all sorts of other exciting checks
 * now.
 * @return 1 if the object can be added, 0 if not.
 */
int test_remove( object ob, int flag, mixed dest ) {
   object thing;

   if ( flag ) {
      return 0;
   }
   if ( !this_player() || ( this_player() == this_object() ) ) {
      return !flag;
   }
   if ( stringp( dest ) ) {
      thing = find_object( dest );
   }
   if ( objectp( dest ) ) {
      thing = dest;
      dest = file_name( dest );
   }
   if ( thing->query_corpse() || ( dest == "/room/rubbish" ) ) {
      return !flag;
   }
/*
 * True theft commands should return 1 to query_theft_command().
 *  Things like the ritual "fumble" and the command "disarm" should return -1.
 */
   if ( sizeof( filter_array( previous_object( -1 ),
         (: $1->query_theft_command() :) ) ) ) {
      return !flag;
   }
/* We should now only be considering things like "get", "take" and "palm". */
   if ( !query_property( PASSED_OUT ) ) {
      return 0;
   }
   if ( !pk_check( this_player(), this_object() ) ) {
      event( environment( this_player() ), "theft", this_player(),
            this_object(), ob );
      return !flag;
   }
   write( "An unseen force stays your hand.\n" );
   return 0;
} /* test_remove() */

/**
 * This method returns the current alignment of the living
 * object.
 * @return the current alignment
 * @see set_al()
 * @see adjust_al()
 * @see adjust_alignment()
 * @see align_string()
 */
int query_al() { return alignment; }

/**
 * This method sets the current alignment of the living
 * object.
 * @param number the new alignment for the object
 * @see query_al()
 * @see adjust_al()
 * @see adjust_alignment()
 * @see align_string()
 */
void set_al( int number ) { alignment = number; }

/**
 * This method adjusts the current alignment of the living
 * object.
 * @param number the amount to change the alignment by
 * @return the new alignment
 * @see query_al()
 * @see set_al()
 * @see adjust_alignment()
 * @see align_string()
 */
int adjust_al( int number ) {
   alignment += number;
   if ( alignment < -MAX_AL ) {
      alignment = -MAX_AL;
   }
   if ( alignment > MAX_AL ) {
      alignment = MAX_AL;
   }
   return alignment;
} /* adjust_al() */

/**
 * This method adjusts the current alignment of the living
 * object.  This is called when an object dies and handles the
 * mangling of the change value based on the current alignment and
 * the alignment of the thing dieing.
 * @param number the amount to change the alignment by
 * @return the new alignment
 * @see query_al()
 * @see adjust_al()
 * @see set_al()
 * @see align_string()
 */
int adjust_alignment( int number ) {
  int change;

  // To become more evil you must kill something that is at least 20%
  // good as you are evil and vice versa.
  change = - (number + alignment/5);

  // This stops the changes being too extreme. Increase this for smaller
  // changes, decrease it for larger ones.
  change /= 20;

  // Now wasn't that nice and easy? :-)
  return adjust_al( change );
} /* adjust_alignment() */

/**
 * This method returns the string associated with the current
 * alignment of the living object.
 * @return the string associated with the alignment
 * @see query_al()
 * @see adjust_al()
 * @see adjust_alignment()
 * @see set_al()
 */
string align_string() {
   switch ( alignment ) {
      case -MAX_AL .. -5001 : return "extremely good"; break;
      case -5000 .. -2501 : return "very good"; break;
      case -2500 .. -1251 : return "quite good"; break;
      case -1250 .. -601 : return "good"; break;
      case -600 .. -301 : return "barely good"; break;
      case -300 .. 300 : return "neutral"; break;
      case 301 .. 600 : return "barely evil"; break;
      case 601 .. 1250 : return "evil"; break;
      case 1251 .. 2500 : return "quite evil"; break;
      case 2501 .. 5000 : return "very evil"; break;
      default : return "extremely evil"; break;
   }
} /* align_string() */

/**
 * This method returns the current deity the living object is
 * worshipping.
 * @return the current deity
 * @see /obj/handlers/diety_handler
 * @see set_deity()
 */
string query_deity() { return deity; }
/**
 * This method sets the current deity the living object is
 * worshipping.
 * @param word the new deity
 * @see /obj/handlers/diety_handler
 * @see query_deity()
 */
void set_deity( string word ) { deity = word; }

/**
 * This is the method used to query the current message to use when
 * entering a room.   A $N in the string will be expanded to the
 * name and a $F will be expanded to the from direction.
 * @return the message to print when entering a room.
 * @see /obj/handlers/room_handler
 * @see query_msgout()
 * @see set_msgin()
 */
string query_msgin() { return _messages->msgin; }
/**
 * This is the method used to query the current message to use when
 * exiting a room.   A $N in the string will be expanded to the
 * name and a $T will be expanded to the to direction.
 * @return the message to print when entering a room.
 * @see /obj/handlers/room_handler
 * @see query_msgin()
 * @see set_msgout()
 */
string query_msgout() { return _messages->msgout; }

/**
 * This is the method used to set the current message to use when
 * entering a room.   A $N in the string will be expanded to the
 * name and a $F will be expanded to the from direction.
 * @param str the message to print when entering a room
 * @see /obj/handlers/room_handler
 * @see query_msgin()
 * @see set_msgout()
 */
int set_msgin(string str) {
   if (strsrch(str, "$N") == -1 || strsrch(str, "$F") == -1) {
      return 0;
   }
   _messages->msgin = str;
   return 1;
} /* set_msgin() */
/**
 * This is the method used to query the current message to use when
 * exiting a room.   A $N in the string will be expanded to the
 * name and a $T will be expanded to the to direction.
 * @return the message to print when entering a room.
 * @see /obj/handlers/room_handler
 * @see query_msgout()
 * @see set_msgin()
 */
int set_msgout(string str) {
   if (strsrch(str, "$N") == -1 || strsrch(str, "$T") == -1) {
      return 0;
   }
   _messages->msgout = str;
   return 1;
} /* set_msgout() */

/**
 * THis is the message to be used when the person is teleported.
 * @return the in message when they teleport
 */
string query_mmsgin() { return _messages->mmsgin; }
/**
 * THis is the message to be used when the person is teleported.
 * @return the out message when they teleport
 */
string query_mmsgout() { return _messages->mmsgout; }

/**
 * The teleport in message.  Sets the message to be seen when
 * a player telerpots into the room.
 * @param str the message to be seen
 */
int set_mmsgin(string str) {
   if (strsrch(str, "$N") == -1) {
      return 0;
   }
   _messages->mmsgin = str;
   return 1;
} /* set_mmsgin() */

/**
 * Sets the teleport out message.  If the player teleports out, this
 * is the message seen.
 * @param str the teleport message
 */
int set_mmsgout(string str) {
   if (strsrch(str, "$N") == -1) {
      return 0;
   }
   _messages->mmsgout = str;
   return 1;
} /* set_mmsgout() */

mixed *query_facing() { return copy( facing ); }

/**
 * The facing array is a list of directions and two integers which determine
 * which way we face.  The layout is:<br>
 * ({ facing, ({ dirs }), up_down_facing, ({ up_down_dirs }) })
 * @param args the facing arguments
 */
void set_facing( mixed *args ) { facing = args; }

/**
 * This method finds the relative direction from the passed in direction.
 * @param word the exit name ('east', 'west',...)
 * @param from the offset to find the exit from
 * @return the relative direction
 */
string find_rel( string word, int from ) {
  int i;

  i = member_array( word, facing[ 1 ] );
  if ( i != -1 ) {
    i = ( i + 8 - facing[ 0 ] ) % 8;
    return REL_DIRS[ 3 * i + from ];
  }
  i = member_array( word, facing[ 2 ] );
  if ( i != -1 ) {
    return ({ "up", "down" })[ i ];
  }
  return word;
} /* find_rel() */

/**
 * Finds the absolute direction from the input relative direction.
 * @param word the exit name ('left', 'right', ...)
 * @return the absolute direction
 */
string find_abs( string word ) {
  int i;

  i = member_array( word, REL_DIRS );
  if ( i != -1 ) {
    i = ( i / 3 + facing[ 0 ]) % 8;
    return facing[ 1 ][ i ];
  }
  i = member_array( word, ({ "up", "down" }) );
  if ( i != -1 ) {
    return facing[ 2 ][ i ];
  }
  return word;
} /* find_rel() */

/**
 * This method takes in a relative direction and reorients us in the correct
 * way to go in that direction.  This also updates our facing so we are
 * facing in the specified relative direction.
 * @param word the direction to look up
 * @return the real direction
 */
string reorient_rel( string word ) {
  int i;

  i = member_array( word, REL_DIRS );
  if ( i != -1 ) {
    i = ( i / 3 + facing[ 0 ] ) % 8;
    facing[ 0 ] = i;
    return facing[ 1 ][ i ];
  }
  i = member_array( word, ({ "up", "down" }) );
  if ( i != -1 ) {
    return facing[ 2 ][ i ];
  }
  return word;
} /* reorient_rel() */

/**
 * This method takes in a absolute direction and reorients us in the correct
 * way to go in that direction.
 * @param word the direction to look up
 */
void reorient_abs( string verb ) {
  if ((ABS_FACING)[verb]) {
    facing[0] = (ABS_FACING)[verb] % 8;
  }
} /* reorient_rel() */

/**
 * This method sets the object for us to drag when we try and leave.
 * @param thing the object to drag off
 */
void set_dragging( object thing ) { add_property("dragging", thing); }

/**
 * This method returns the object we are dragging.
 * @return the thing we are dragging
 */
object query_dragging() { return query_property("dragging"); }

/**
 * This method resets the object we are dragging.
 */
void reset_dragging() { add_property("dragging", 0); }

/**
 * This is used by the movement system to look in the room when we move.
 * It does checks for verbose and other checks before doing the look.
 */
int room_look() {
   if ( query_property( UNKNOWN_MOVE ) || !( interactive( this_object() ) ||
         this_object()->query_slave() ) )
      return 0;
   /* These need to be evaluated immediately, hence the bypass_queue() call. */
   if(!mapp(verbose))
     verbose = ([ ]);

   if(verbose && verbose["look"]) {
      this_object()->ignore_from_history( "look" );
      this_object()->bypass_queue();
      command( "look" );
   } else {
      this_object()->ignore_from_history( "glance" );
      this_object()->bypass_queue();
      command( "glance" );
   }
   return 1;
} /* room_look() */

/**
 * This method returns the current verbose mode setting of the player.
 * @param the type of verbosity, by default it will return the normal stuff.
 * @return the verbose mode of the player
 */
int query_verbose(string type) {
  if(!verbose || !mapp(verbose)) {
    verbose = ([ ]);
  }
  return verbose[type];
} /* query_verbose() */

/**
 * This method sets the verbosity for a given type.
 */
void set_verbose(string type, int val) {
  if(!verbose || !mapp(verbose)) {
    verbose = ([ ]);
  }

  if(member_array(type, VERBOSE_TYPES) != -1) {
    verbose[type] = val;
  }
}

/**
 * This method returns the current verbose/brief types.
 */
string *query_verbose_types() {
  return VERBOSE_TYPES;
}

varargs int move_with_look( mixed dest, string messin, string messout ) {
   return_to_default_position(1);
   if ( (int)this_object()->move( dest, messin, messout ) != MOVE_OK )
      return 0;
   room_look();
   return_to_default_position(1);
   return 1;
} /* move_with_look() */

varargs int exit_command( string word, mixed verb, object thing, int redirection ) {
   string special_mess, *bits, *exits, redirect;
   int temp;

   if ( !environment() ) {
      return 0;
   }

   if ( !verb ) {
      verb = word;
      bits = explode( word, " " );
      if ( sizeof( bits ) > 1 ) {
         word = implode( bits[ 1 .. ], " " );
      } else {
         word = "";
      }
   } else {
      if ( pointerp( verb ) ) {
         special_mess = verb[ 1 ];
         verb = verb[ 0 ];
      }
      bits = explode( verb, " " );
      if ( sizeof( bits ) > 1 ) {
         word = implode( bits[ 1 .. ], " " );
      } else {
         word = "";
      }
   }
   if ( lengthen[ verb ] ) {
      verb = lengthen[ verb ];
   }
   exits = (string *)environment()->query_exits();
   if ( !exits ) {
      return 0;
   }

   if ( member_array( verb, exits ) != -1 ) {
     if ( environment()->query_relative( verb ) ) {
       return 0;
     }

     if ((abs_facing)[verb]) {
       facing[0] = (abs_facing)[verb] % 8;
     }
   } else {
     if (member_array(find_abs(verb), exits ) == -1 ) {
       return 0;
     }
     verb = reorient_rel(verb);
   }
   if ( !thing ) {
      thing = this_object();
   }

    //If the player has redirection enabled!
    //Redirection returns a string which is an alternative exit to try.
    //It calls exit_command to do all of the proper checks before hand,
    //it blocks recursion by way of a flag which is passed as the fourth arg
    //in exit command.
    if ( !redirection ) {
        if ( stringp( redirect = thing->query_exit_redirection( word ) ) ) {
            if ( temp = this_object()->exit_command( verb, redirect, thing, 1 ) ) 
                return temp;
        }
    }

   return (int)"/obj/handlers/room_handler"->exit_move( verb, word,
         special_mess, thing );
} /* exit_command() */

void become_flummoxed() {
   int will;

   will = query_int() * query_wis();
   if ( will < random( WILL_POWER ) )
      this_object()->interrupt_ritual();
   if ( will < random( WILL_POWER ) )
      this_object()->interrupt_spell();
   if ( will < random( WILL_POWER ) )
      this_object()->stop_all_fight();
} /* become_flummoxed() */

int run_away() {
   int i;
   object old_env;
   mixed *direcs;
   
   direcs = (mixed *)environment()->query_dest_dir(this_object());
   old_env = environment();
   while ( sizeof( direcs ) ) {
      i = random( sizeof( direcs ) / 2 ) * 2;
      if ( exit_command( direcs[i] ) ) {
          event( old_env, "run_away", direcs[ i ], direcs[ i + 1 ] );
          return 1;
      }

      direcs = delete( direcs, i, 2 );
   }
   return 0;
} /* run_away() */

/* Now handled in test_remove().
int cannot_get_stuff() { return !query_property( PASSED_OUT ); }
*/

/** @ignore yes */
mixed *stats() {
   return container::stats() + stats::stats() + ({
           ({ "max_hp", max_hp }),
           ({ "hp",  hp }),
           ({ "max_gp", max_gp }),
           ({ "gp", gp }),
           ({ "alcohol", drink_info[ D_ALCOHOL ] }),
           ({ "food", drink_info[ D_FOOD ] }),
           ({ "drink", drink_info[ D_DRINK ] }),
           ({ "gender", query_gender_string() }),
           ({ "alignment", this_object()->query_al() }),
           ({ "deity", deity }),
           ({ "total money", query_value() }),
           ({ "xp", query_xp() }),
   }) + armour::stats() + combat::stats();
} /*  stats() */

/**
 * This method returns the current array used for calculating 'it' and
 * 'them' in the find_match code.
 * @return the array of objects matching them
 * @see /secure/simul_efun->find_match()
 * @see set_it_them()
 */
class obj_match_context query_it_them() { return _liv_data->it_them; }
/**
 * This method sets the current array used for calculating 'it' and
 * 'them' in the find_match code.
 * @param args the new array of objects
 * @see /secure/simul_efun->find_match()
 * @see query_it_them()
 */
void set_it_them( class obj_match_context args) { _liv_data->it_them = args; }

/**
 * This method adds a follower to the living object.  A follower will
 * happily follow around the person in front.  Used in the follow
 * command.
 * @param ob the object to follow us
 * @see remove_follower()
 * @see query_followers()
 * @return 1 on success, 0 on failure
 */
int add_follower(object ob) {
  if (ob == this_object()) {
    return 0;
  }
  if (member_array(ob, _liv_data->followers) == -1) {
    _liv_data->followers += ({ ob });
  }
  return 1;
} /* add_follower() */

/**
 * This method removes a follower from the living object.  A follower will
 * happily follow around the person in front.  Used in the unfollow
 * and lose commands.
 * @param ob the object to remove from the follow list
 * @see add_follower()
 * @see query_followers()
 * @return 1 on success, 0 on failure
 */
int remove_follower(object ob) {
  int i;

  i = member_array(ob, _liv_data->followers);
  if (i != -1) {
    _liv_data->followers = delete(_liv_data->followers, i, 1);
    return 1;
  }
  return 0;
} /* remove_follower() */

/**
 * This is a method to check to see if this object can actually follow
 * the person they are following.
 * @param thing the thing following us
 * @param verb the direction they are going to
 * @param special any special stuff
 * @return 1 if we are allowed to go there, 0 otherwise
 */
int check_doing_follow(object thing, string verb, string special) {
   return 1;
} /* check_doing_follow() */

/**
 * This method returns the current room of the object.  This was needed
 * previously to use in things like unique_array, before function
 * pointers came into existance.
 * @return the environment of the object
 */
object query_current_room() { return environment(); }

/**
 * This method returns the current list of followers to the living
 * object.
 * @see add_follower()
 * @see remove_follower()
 */
mixed *query_followers() { return copy(_liv_data->followers - ({ 0 })); }

/** @ignore yes */
varargs void adjust_money(mixed amt, string type) {
  return money::adjust_money(amt, type);
} /* adjust_money() */

/** @ignore yes */
mixed *query_money_array() {
  return money::query_money_array();
} /* query_money_array() */

/** @ignore yes */
int query_money(string type) {
  return money::query_money(type);
} /* query_money() */

/** @ignore yes */
int query_value() { return money::query_value(); }

void do_burden_call() {
  if(_liv_data->burden_call)
    remove_call_out(_liv_data->burden_call);
  
  _liv_data->burden_call = call_out("calc_burden", 1);
}

int query_burden_limit() {
  if (this_object()->query_creator()) { 
    return MAX_CREATOR_INVEN;
  } else {
    return MAX_INVEN;
  }
} /* query_burden_limit() */ 

/** @ignore yes */
void event_enter( object thing, string mess, object from ) {
  if(environment( thing ) == this_object()) {
    do_burden_call();
    
    if(sizeof(all_inventory()) > query_burden_limit() ) {
      _liv_data->to_drop += ({ thing });
      remove_call_out("test_number_of_items");
      call_out("test_number_of_items", 5 + random(5));
    }
  }
}

/** @ignore yes */
void event_exit( object thing, string mess, object to ) {
  if(environment(thing) == this_object()) {
    do_burden_call();
  }
}

/** @ignore yes */
void test_number_of_items() {
   int how_many;
   object thing, *things, *dropped;

   things = all_inventory() - query_armours() - query_holding();
   how_many = sizeof(things) - query_burden_limit();
   if ( how_many < 1 ) {
      return;
   }

   _liv_data->to_drop -= ({ 0 });

   dropped = ({ });
   while(how_many > 0 && sizeof(things)) {
      if (sizeof(_liv_data->to_drop)) {
         thing = _liv_data->to_drop[random(sizeof(_liv_data->to_drop))];
      } else {
         thing = things[random(sizeof(things))];
      }
      things -= ({ thing });
      _liv_data->to_drop -= ({ thing });

      if(!thing || !thing->short() || thing->drop() ||
         thing->query_property("cannot fumble") ||
         thing->id("coin") ||
         environment(thing) != this_object()) {
        continue;
      }

      if((int)thing->move(environment()) == MOVE_OK) {
        how_many--;
        dropped += ({ thing });
      }
   }

   _liv_data->to_drop = ({ });

   if(sizeof(dropped)) {
     tell_object( this_object(), "Whoops!  You tried to carry too many "
                  "things and fumbled "+
                  query_multiple_short( dropped ) +".\n" );
     this_object()->dest_hide_shadow();
     tell_room( environment(), capitalize( the_short() ) +" juggles "+
                "around "+ query_possessive() +" stuff and fumbles "+
                query_multiple_short( dropped ) +".\n", this_object() );
   }
}

/**
 * This forces a burden recalculation.  This should also be used to
 * force a recalcuation of the number of items someone can carry.
 */
void force_burden_recalculate() {
   do_burden_call();
   remove_call_out("test_number_of_items");
   call_out("test_number_of_items", 5 + random(5));
}

/** @ignore yes */
object *find_inv_match( string words, object looker ) {
   return sort_array( container::find_inv_match( words, looker ),
         (: ( member_array( $1, query_holding() ) != -1 ? -1 : 0 ) :) );
} /* find_inv_match() */

/** @ignore yes */
int attack_by(object ob) {
   return_to_default_position(0);
   return ::attack_by(ob);
} /* attack_by() */

/** @ignore yes */
int attack_ob(object ob) {
   return_to_default_position(0);
   return ::attack_ob(ob);
} /* attack_ob() */

/**
 * This method sets the always the flag to always use the default position.
 * If this is set then rooms cannot override the position message which is
 * displayed by the object.
 * @param flag if we should always use the default position
 * @see query_always_use_default_position()
 * @see set_default_position()
 * @see return_to_default_position()
 */
void set_always_use_default_position(int flag) {
   always_use_default_position = flag;
} /* set_always_use_default_position() */

/**
 * This method sets the always the flag to always use the default position.
 * If this is set then rooms cannot override the position message which is
 * displayed by the object.
 * @return the always use default position flag
 * @see set_always_use_default_position()
 * @see set_default_position()
 * @see return_to_default_position()
 */
int query_always_use_default_position() {
   return always_use_default_position;
} /* query_always_use_default_position() */

/**
 * This method sets the default position of the object.  This is used to
 * allow things to default to some other exciting off beat and froopy
 * default position.  The value returned by this is the command code
 * used to put the object back into the default position or an
 * array which contains three or one elements, the first is the string
 * to use as the position, the second and third (if they exist) are
 * the string to tell the person when changing and the string to tell
 * everyone else when changing position.
 * @return the default position
 * @see set_default_position()
 * @see return_to_default_position()
 * @see set_always_use_default_position()
 */
string query_default_position() {
   mixed pos;

   if (stringp(default_position) &&
              default_position->query_position_command()) {
      pos = default_position;
   } else if (pointerp(default_position) &&
              (sizeof(default_position) == POSITION_MESS_SIZE ||
               sizeof(default_position) == POSITION_ONLY_TYPE_SIZE)) {
      pos = default_position;
   } else if (functionp(default_position)) {
      pos = default_position;
   }

   if (!pos) {
      pos = STANDING_CMD;
   }

   return pos;
} /* query_default_position() */

/**
 * This sets the default position of the object.  This is used to
 * allow things to default to some other exciting off beat and froopy
 * default position.  The paramater to this is the command code
 * used to put the object back into the default position or an
 * array which contains three or one elements, the first is the string
 * to use as the position, the second and third (if they exist) are
 * the string to tell the person when changing and the string to tell
 * everyone else when changing position.  The paramer can also be
 * a function pointer, if it is then it will be evaluated and
 * have two parameters passed into the function.  The first is
 * the object returing to the position and the second is the leaving
 * flag.
 * <p>
 * Please note!  After setting the position you will need to
 * make the object return to the default position to use it.
 * <p>
 * A second note!  A room can also define a query_default_position()
 * function which will be called, if this returns a value (and the
 * override flag is not set) then that will be used for the default
 * position.
 * @param str the new default position
 * @see query_default_position()
 * @see set_always_use_default_position()
 * @see /obj/monster()->set_cannot_change_position()
 * @see return_to_default_position()
 * @example
 * set_default_position("/cmds/living/kneel");
 * @example
 * set_default_position(({ "running" }));
 * @example
 * set_default_position(({ "fishing",
 *                         "You start to fish.\n",
 *                         the_short() + " starts to fish.\n" }));
 * @example
 * npc->set_default_position(({ "running" }));
 * npc->return_to_default_position(1);
 */
void set_default_position(mixed str) {
   if (stringp(str) && str->query_position_command() && str != STANDING_CMD) {
      default_position = str;
   } else if (!str || str == STANDING_CMD) {
      default_position = 0;
   } else if (pointerp(str) && (sizeof(str) == POSITION_ONLY_TYPE_SIZE ||
                                sizeof(str) == POSITION_MESS_SIZE)) {
      default_position = str;
   } else if (functionp(str)) {
      default_position = str;
   }
} /* set_default_position() */

/**
 * This method returns the living object to its default position.
 * @param leaving this is if we are leaving the room
 * @see set_default_position()
 * @see query_default_position()
 * @see set_always_use_default_position()
 */
void return_to_default_position(int leaving) {
   mixed pos;

   /* See if we are being forced to always use the set default position. */
   if (query_always_use_default_position()) {
      pos = query_default_position();
   } else {
      /* See if the room has a default position they wish to tell us about. */
      pos = 0;
      if (environment()) {
         pos = environment()->query_default_position(this_object());
      }
      if (!pos) {
         pos = query_default_position();
      }
   }

   if (functionp(pos)) {
      if (!evaluate(pos, this_object(), leaving)) {
         pos = 0;
         /* See if the environment has any special conditions */
         if (environment()) {
            pos = environment()->query_default_position(this_object());
         }
         if (!pos) {
            pos = STANDING_CMD;
         }
      }
   }

   if (stringp(pos)) {
      /* If we are not standing up...  Stand up... */
      if (position != pos->query_position_type() ||
          (leaving && query_position_on())) {
         if (leaving) {
            catch(pos->position_floor(this_object()));
         } else {
            /*
             * If they are not leaving, just get them to stand up or
             * whatever, this way people can fight on chairs and
             * stuff...
             */
            catch(pos->position(this_object()));
         }
      }
   } else if (pointerp(pos) && position != pos[POSITION_TYPE_INDEX]) {
      /* If it is a pointer, then we do something special... */
      if (sizeof(pos) > 1) {
         if (pos[POSITION_ME_MESS_INDEX]) {
            tell_object(this_object(), pos[POSITION_ME_MESS_INDEX]);
         }
         if (pos[POSITION_REST_MESS]) {
            tell_room(environment(), pos[POSITION_REST_MESS], this_object());
         }
      }
      set_position(pos[POSITION_TYPE_INDEX]);
      set_position_on(0);
      set_position_type(0);
      set_position_multiple(0);
   }
} /* return_to_default_position() */

/**
 * This sets the current position of the object.
 * @param name the string to use for the position
 * @see query_position()
 * @see query_position_on()
 * @see query_position_multiple()
 * @see query_position_type()
 * @see set_position_on()
 * @see set_position_multiple()
 * @see set_position_type()
 */
void set_position(string name) {
   position = name;
} /* set_position() */

/**
 * This queries the current position of the object.
 * @return the current position of the living
 * @see query_position_on()
 * @see query_position_multiple()
 * @see query_position_type()
 * @see set_position()
 * @see set_position_on()
 * @see set_position_multiple()
 * @see set_position_type()
 */
string query_position() {
   return position;
} /* query_position() */

/**
 * This sets the current object which is being referenced as being
 * 'on', 'beside' or 'at'.
 * @param ob the object being referenced
 * @see query_position()
 * @see query_position_on()
 * @see query_position_multiple()
 * @see query_position_type()
 * @see set_position()
 * @see set_position_multiple()
 * @see set_position_type()
 */
void set_position_on(mixed ob) {
   if (!position_on) {
      position_on = allocate(POSITION_ARRAY_SIZE);
   }
   position_on[POS_ON_OBJECT] = ob;
} /* set_position_on() */

/**
 * This sets fact that the object being referenced is one of many.  So
 * you get something more like 'xx is sitting on one of the couches'.
 * @param mult 0 if non-multiple, 1 if multiple
 * @see query_position()
 * @see query_position_on()
 * @see query_position_multiple()
 * @see query_position_type()
 * @see set_position()
 * @see set_position_multiple()
 * @see set_position_type()
 */
void set_position_multiple(int mult) {
   if (!position_on) {
      position_on = allocate(POSITION_ARRAY_SIZE);
   }
   position_on[POS_MULTIPLE] = mult;
} /* set_position_multiple() */

/**
 * This returns fact that the object being referenced is one of many.  So
 * you get something more like 'xx is sitting on one of the couches'.
 * @return 0 if non-multiple, 1 if multiple
 * @see query_position_on()
 * @see query_position_multiple()
 * @see query_position_type()
 * @see set_position()
 * @see set_position_on()
 * @see set_position_multiple()
 * @see set_position_type()
 */
int query_position_multiple() {
   if (!position_on) {
      return 0;
   }
   return position_on[POS_MULTIPLE];
} /* query_position_multiple() */

/**
 * This sets way the object is being referenced.  The 'on', 'at', 'beside'
 * or whatever strings.
 * @param type the new type string
 * @see query_position()
 * @see query_position_on()
 * @see query_position_multiple()
 * @see query_position_type()
 * @see set_position()
 * @see set_position_on()
 * @see set_position_multiple()
 */
void set_position_type(string type) {
   if (!position_on) {
      position_on = allocate(POSITION_ARRAY_SIZE);
   }
   position_on[POS_TYPE] = type;
} /* set_position_type() */

/**
 * This returns way the object is being referenced.
 * The 'on', 'at', 'beside'
 * or whatever strings.
 * @return the current type string
 * @see query_position()
 * @see query_position_on()
 * @see query_position_multiple()
 * @see set_position()
 * @see set_position_on()
 * @see set_position_multiple()
 * @see set_position_type()
 */
string query_position_type() {
   if (!position_on ||
       !position_on[POS_TYPE]) {
      return "on";
   }
   return position_on[POS_TYPE];
} /* query_position_type() */

/**
 * This queries the current object being referenced.  This can be an
 * object or a string.
 * @return the current object being referenced
 * @see query_position()
 * @see query_position_multiple()
 * @see query_position_type()
 * @see set_position()
 * @see set_position_on()
 * @see set_position_multiple()
 * @see set_position_type()
 */
object query_position_on() {
   if (!position_on) {
      return 0;
   }
   return position_on[POS_ON_OBJECT];
} /* query_position_on() */

/**
 * This method returns the short description of the object
 * we are referencing.
 * @return the short description of the object, "" if none
 * @see query_position_on()
 * @see set_position_on()
 * @see query_position_long()
 */
string query_position_on_short() {
   if (!position_on || !position_on[POS_ON_OBJECT]) {
      return "";
   }
   if (stringp(position_on[POS_ON_OBJECT])) {
      return position_on[POS_ON_OBJECT];
   }
   return position_on[POS_ON_OBJECT]->the_short();
} /* query_position_on_short() */

/**
 * This method returns the string used in the long description of the
 * living object.
 * @return the long description of the position
 * @see query_position_type()
 * @see query_position_on_short()
 */
string query_position_long() {
   if (position != STANDING || position_on) {
      if (position_on) {
         return query_pronoun() + " is " + query_position_type() + " "+
                query_position_on_short()+".\n";
      }
      return query_pronoun()+" is "+position+" on the floor.\n";
   }
   return "";
} /* query_position_long() */

/**
 * This method returns the description used in the inventory listing
 * code.
 * @return the string used in inventory listings
 * @see query_position_long()
 * @see query_position_on_short()
 * @see query_position_type()
 */
string query_position_short() {
   if (!position_on ||
       !position_on[POS_ON_OBJECT]) {
      return position;
   }
   return position + " " + query_position_type() + " " +
          query_position_on_short();
} /* query_position_short() */

/**
 * This method returns 1 if the creature is trapped, ie cannot walk.
 * By default, a creature is free to walk, hence the normal return
 * value of 0.  If you shadow this method, including a message about
 * why the player cannot move move is a good idea.
 * @param verb the exit direction that the player is trying to move.
 * @param dest_other the destination information for the exit.
 * @see /std/room->query_dest_other()
 * @return 0 means creature is free to move, 1 that it is trapped.
 */
varargs int cannot_walk( string verb, mixed *dest_other) {
   return 0;
}

/**
 * This method can be shadowed by all forms of magical and
 * religious shields so that the shields command will give
 * the player a nice description.
 * The first element in the array should contain the description
 * as shown to the player, the second element should contain the
 * description shown to others.
 * @example ({ "You have a nice shield.", "He has a nice shield." })
 * @return Array with description of shield.
 */
string *query_arcane_shields() { return ({ }); }

/** @ignore yes */
int can_find_match_reference_inside_object(object thing, object looker) {
   if (member_array(thing, query_wearing_hidden(looker, 0)) != -1) {
      return 0;
   }
   return 1;
} /* can_find_match_reference_inside_object() */

/**
 * If there is no mapping for deity favour, or if the God is not mentioned, 
 * return 0 as 'no favour'.
 * Otherwise return the favour amount.
 * @args God Name of God
 */
int query_deity_favour( string god ) {
  if ( !mapp( deity_favour ) ) 
    return 0;

  if ( !undefinedp( deity_favour[ god ] ) ) 
    return 0;

  return deity_favour[ god ];
}

/**
 * Adjust the favour rating for 'god' by amount.
 * A +ve amount is good favour, a negative is bad.
 */
void adjust_deity_favour( string god, int amount ) {

  if ( !mapp( deity_favour ) ) 
    deity_favour = ([ ]);
  
  if ( !undefinedp( deity_favour[ god ] ) ) 
    deity_favour[ god ] = amount;

  deity_favour[ god ] += amount;

  if ( deity_favour[ god ] > MAX_FAVOUR )
    deity_favour[ god ] = MAX_FAVOUR;

  if ( deity_favour[ god ] < -MAX_FAVOUR )
    deity_favour[ god ] = -MAX_FAVOUR;

  return;
}

/**
 * Return the deity favour mapping.
 */
mapping query_all_deity_favour() { return deity_favour; }
