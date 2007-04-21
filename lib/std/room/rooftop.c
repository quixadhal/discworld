/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: rooftop.c,v 1.10 2003/03/24 21:16:14 ceres Exp $
 */
#include <tasks.h>

#define TOO_SOON "too soon to proceed from rooftop"

inherit "/std/room/outside";

/**
 * This is a standard rooftop inheritable. It allows you to setup a rooftop
 * location including such things as slope, strength, jumping locations etc.
 *
 * @example
 * inherit "/std/rooftop"
 *
 * void setup() {
 *    set_fall_damage( "all", 500 );
 *    set_slope( 60, "ledge", PATH + "womble01" );
 *    set_weak_messages(
 *      "The ledge crumbles under your weight.",
 *      "$short$ falls from the crumbling ledge.",
 *      "$short$ comes crashing to the ground.",
 *      "The paving stones shift beneath your feet.",
 *    );
 *    set_weak_roof( 2000, PATH + "womble01" );
 *    set_jump( ({ "north", "n" }), PATH + "ledge01", PATH + "womble01", 6 );
 * }
 *
 * @see set_wall
 * @author shrike
 */
#define DEBUG

#define ROCK "other.movement.climbing.rock"
#define ROPE "other.movement.climbing.rope"

/* function prototypes */
void   set_death_reason( string );
void   set_fall_damage( string, int );
mixed  query_fall_damage( string );
int    calc_fall_damage( string );
void   set_weak_roof( int, string );
void   set_slope( int, string, string );
string process_string( string, mapping );
string process_mess( string, object, string );

/* object variables */
mapping  damages;          // damages for all the various types of falling.
string * damage_types;
int      roof_max_weight;  // in units (20 units == 1 kg)
int      gradient;         // percentage grade of the roof.
string   weak_roof_dest;   // where you fall if the roof collapses.
string   slope_dest;       // where you fall if you slip off the roof.
string   place;            // where you fall off of. (ie "edge of the roof
                           //  bordering Filigree Street" )
string   death_reason;     // used when you die.

/*
 * Message arrays. most are of the form
 * ({ player_mess, exit_mess, enter_mess })
 * Where player_mess is what the player who's jumping/falling sees.
 * exit mess is the message sent to the other objects in the starting room,
 * and enter_mess is what the objects in the target room see.
 * weak_messages has an additional 4th parameter, which is the message to
 * the room when the roof is about to collapse, and jump_tm_messages will
 * be randomly selected from if the player gets a TM while jumping.
 */
string * weak_messages;    // for when a roof collapses.
string * slope_messages;   // falling off
string * step_messages;    // stepping off when you should have jumped
string * jump_tm_messages;       // tm messages. it will pick a rnadom one
string * jump_success_messages;  // jumping successfully
string * jump_failure_messages;  // jumping unsuccessfully
string * ghost_fall_messages;    // a ghost drifting to the ground
string * item_slope_messages;    // a non-corpse item sliding off the edge
string * corpse_slope_messages;  // a corpse sliding off the edge

/*
 * Info for jumping from roof to roof. each element is of the form
 * <string:key>: ({             Name of the exit
 *    <string:destination>,     Filename of the destination room
 *    <string:fall_dest>,       filename of the room where you fall on failure
 *    <int:distance>            Distance. (streets in AM are ~5-20 wide
 * })
 */
mapping jump_info;

/*
 * Translates from direction aliases to jump_info keys. Each element looks
 * like <string:name> : <string: jump_info key>
 * In order to pull up the jump info for a particular location, you'd
 * use jump_info[ translations["name"] ].
 */
mapping translations;

/** @ignore yes */
void init() {
   ::init();
   add_command( "jump", "<word'direction'>",
     (: this_object()->do_roofjump( $4[0] ) :) );
} /* init() */

/** @ignore yes */
void create() {
   do_setup++;
   ::create();
   do_setup--;

   weak_messages = ({
     "The roof collapses! This is going to be painful...\n",
     "$short$ crashes down through a weak spot in the roof.",
     "$short$ comes crashing to the ground, landing in a rain of debris.",
     "You hear an ominous creak.\n"
   });

   slope_messages = ({
     "The roof is too steep for you to stand on! It's time to make friends "
       "with the floor...\n",
     "$short$ gets in touch with gravity and slides over the edge of the "
       "roof.",
     "$short$ comes crashing to the ground, landing in a heap."
   });

   step_messages = ({
     "You step off the edge of the roof into midair.\n",
     "$short$ steps off the edge of the roof and plummets earthwards.",
     "$short$ comes crashing to the ground, landing in a heap."
   });

   jump_tm_messages = ({
     "You leap more gracefully through the air.",
     "You feel more able to leap tall buildings in a single bound.",
     "You jump like a mountain goat."
   });

   jump_success_messages = ({
     "You launch yourself off the edge of the roof and land gracefully on "
       "the other side.\n",
     "$short$ jumps gracefully across the gap to the $dir$.",
     "$short$ jumps in from across the gap to the $opp_dir$."
   });

   jump_failure_messages = ({
     "You launch yourself off the edge of the roof!\nUnfortunately you "
       "misjudge the distance and plummet earthwards. This is going to "
       "hurt...\n",
     "$short$ jumps off to the $dir$, but misjudges and plummets "
       "earthwards.",
     "$short$ plummets to the ground, landing in a heap."
   });

   ghost_fall_messages = ({
      "You find your consciousness drifting earthwards.\n",
      "$the_short$ drifts earthwards.",
      "$the_short$ drifts in from above, looking somewhat dazed."
   });

   item_slope_messages = ({
     "$the_short$ tumbles over the edge and plummets to the ground.\n",
     "Hearing a noise, you look up just as $a_short$ falls off the edge "
       "of the roof and hits the ground.\n"
   });

   corpse_slope_messages = ({
     "$the_short$ tumbles over the edge and plummets to the ground with "
       "a sickening thud.\n",
     "Hearing a noise, you look up just as $the_short$ tumbles over the "
       "edge of the roof and hits the ground with a sickening thud.\n"
   });

   damages = ([ ]);
   damage_types = ({ "weak", "slope", "step", "jump" });
   translations = ([ ]);
   jump_info = ([ ]);

   this_object()->setup();
} /* create() */

/* Customize the message sets */

/**
 * Set the messages for when a player falls through a weak roof.
 *
 * @param player the message shown to the player.
 * @param from the message shown in the room the player is leaving.
 * @param to the message shown in the room the player is entering.
 * @param warn the message shown to the room just before the roof collapses.
 *
 * @see set_weak_roof
 */
void set_weak_messages( string player, string from, string to, string warn ) {
   weak_messages = ({ player, from, to, warn });
} /* set_weak_messages() */

/**
 * Set the messages for when a player slips off a sloping roof.
 *
 * @param player the message shown to the player.
 * @param from the message shown in the room the player is leaving.
 * @param to the message shown in the room the player is entering.
 *
 * @see set_slope
 */
void set_slope_messages( string player, string from, string to ) {
   slope_messages = ({ player, from, to });
} /* set_slope_messages() */

/**
 * Set the messages for when a player falls off by moving in the wrong
 * direction.
 *
 * @param player the message shown to the player.
 * @param from the message shown in the room the player is leaving.
 * @param to the message shown in the room the player is entering.
 *
 * @see set_jump
 */
void set_step_messages( string player, string from, string to ) {
   step_messages = ({ player, from, to });
} /* set_step_messages() */

/**
 * Set the messages for when a player gets a TM by jumping. These messages
 * will be selected at random, and then coloured appropriately.
 *
 * @param messages the array of messages from which to choose.
 *
 * @see set_jump
 */
void set_jump_tm_messages( string * messages ) {
   jump_success_messages = messages;
} /* set_jump_tm_messages() */

/**
 * Set the messages for when a player successfully jumps a gap.
 *
 * @param player the message shown to the player.
 * @param from the message shown in the room the player is leaving.
 * @param to the message shown in the room the player is entering.
 *
 * @see set_jump
 */
void set_jump_success_messages( string player, string from, string to ) {
   jump_success_messages = ({ player, from, to });
} /* set_jump_success_messages() */

/**
 * Set the messages for when a player falls off by failing to jump across
 * a gap.
 *
 * @param player the message shown to the player.
 * @param from the message shown in the room the player is leaving.
 * @param to the message shown in the room the player is entering.
 *
 * @see set_jump
 */
void set_jump_failure_messages( string player, string from, string to ) {
   jump_failure_messages = ({ player, from, to });
} /* set_jump_failure_messages() */

/**
 * Set the messages for when a ghost falls off a roof.
 *
 * @param player the message shown to the player.
 * @param from the message shown in the room the player is leaving.
 * @param to the message shown in the room the player is entering.
 *
 * @see set_slope
 */
void set_ghost_fall_messages( string player, string from, string to ) {
   ghost_fall_messages = ({ player, from, to });
} /* set_ghost_fall_messages() */

/**
 * Set the messages for when a corpse slides off a sloping roof.
 *
 * @param from the message shown in the room the corpse is leaving.
 * @param to the message shown in the room the corpse is entering.
 *
 * @see set_slope
 */
void set_corpse_slope_messages( string from, string to ) {
  corpse_slope_messages = ({ from, to });
} /* set_item_slope_messages() */

/**
 * Set the messages for when a non-corpse item slides off a sloping roof.
 *
 * @param from the message shown in the room the object is leaving.
 * @param to the message shown in the room the object is entering.
 *
 * @see set_slope
 */
void set_item_slope_messages( string from, string to ) {
   item_slope_messages = ({ from, to });
} /* set_item_slope_messages() */

/**
 * Set the amount of damage the player will take if they fall off this
 * roof.  This amount is somewhat randomised.
 *
 * @param type The type of fall (weak, slope, step or jump; all to set all damages the same)
 * @param damage The amount of damage.
 *
 * @see set_slope, set_jump, set_weak_roof
 */
void set_fall_damage( string type, int damage ) {
   if( damage < 0 )
      damage = -damage;

   if( type == "all" ) {
      foreach( type in damage_types )
         damages[type] = damage;
   } else if( member_array( type, damage_types ) >= 0 ) {
      damages[type] = damage;
   } else
      return;
} /* set_fall_famage() */

/**
 * Find out the damage for falling from this location.
 *
 * @param type The type ( weak, slope, step, jump, or all )
 * @return An int, or an integer array of the damage(s)
 *
 * @see set_fall_damage
 */
mixed query_fall_damage( string type ) {
   if( type == "all" )
      return damages;
   else
      return damages[type];
} /* query_fall_famage() */

/** @ignore yes */
int calc_fall_damage( string type ) {
   if( type == "all" )
      return 0;
   return query_fall_damage( type ) + random( query_fall_damage( type ) );
} /* calc_fall_famage() */

/**
 * Set the maximum weight this location can support.
 *
 * @param maxweight the maximum weight capacity of the roof.
 * @param dest the room you go to if the roof collapses.
 */
void set_weak_roof( int maxweight, string dest ) {
   roof_max_weight = maxweight;
   weak_roof_dest = dest;
} /* set_weak_roof() */

/**
 * Set the gradient of this location.
 *
 * @param angle the angle (in degrees).
 * @param loc a short description of the area to show to players.
 * @param dest the fall destination.
 */
void set_slope( int angle, string loc, string dest ) {
   gradient = ( angle * 100 ) / 90;
   place = loc;
   slope_dest = dest;
} /* set_slope() */

/**
 * Setup a jumping exit. The player will be able to "jump <dir>" and a skill
 * test will be performed and they'll succeed or fail.
 *
 * Streets in AM are usually 5-20 wide.
 * if you want to do something other than the standard fall function,
 * you'll need to mask do_fall() in the child room.
 *
 * @param dir direction names
 * @param dest the destination
 * @param fall_dest the destination if they fall
 * @param distance the distance of the jump
 * @return -1 if any of the keys in dir already exists, 0 if dir is
 * malformed (string and string * are the only acceptable types), or
 * 1 on success.
 *
 * @example
 * set_jump( ({ "north", "n" }), RUN + "run12", FILIGREE + "filigree10", 6 );
 */
int set_jump( mixed dir, string dest, string fall_dest, int distance ) {
   string * dirs;
   mixed foo;
   string bar;

   if( stringp( dir ) ) {
      dirs = ({ dir });
   } else if( arrayp(dir) ) {
      foreach( foo in dir )
         if( !stringp( foo ) )
            return 0;
      dirs = sort_array( dir, 1 );
   }

   foreach( bar in dir )
      if( !undefinedp( translations[bar] ) )
         return -1;

   // setup the mappings
   jump_info[ dir[0] ] = ({ dest, fall_dest, distance });
   foreach( bar in dir )
      translations[bar] = dir[0];

   if( !query_exit( dir[0] ) )
      add_exit( dir[0], dest, "roof" );
   modify_exit( dir[0], ({
     "closed", 1,
     "function", (: this_object()->silly_move( $1, $2, $3,
       jump_info[ translations[$1] ][1], "step", step_messages ) :),
     "look", "You'll have to jump across to see what's on the other side."
   }) );

   return 1;
} /* set_jump() */

/**
 * @ignore yes
 * Internal function used if a player steps off the edge of a roof.
 */
int silly_move( string verb, object ob, string special, mixed dest,
  string dam_type, string * messages ) {
   this_object()->do_fall( ob, dest, dam_type, messages, 0 );
   return notify_fail( "" );
} /* silly_move() */

/**
 * @ignore yes
 * Internal function to handle falling. You can overload this if you so
 * desire.
 */
void do_fall( object obj, mixed dest, string dam_type, string * messages,
  string dir ) {
   int damage, i;
   object destob;

   // Remove the property that prevents them leaving the room..
   obj->remove_property( TOO_SOON );

   for( i = 0; i < sizeof( messages ); i++ )
      messages[i] = process_mess( messages[i], obj, dir );

   if( stringp( dest ) ) {
      if( !(destob = load_object( dest ) ) ) {
         tell_object( obj, "Cannot find " + dest + ".\n"
           "Moving you to the void - Please contact a creator.\n" );
         obj->move_with_look( "/room/void", "Poof. $N appears.\n",
           "$N plummets earthwards.\n" );
         return;
      }
   } else if( objectp( dest ) ) {
      destob = dest;
   } else {
      return;
   }

   tell_object( obj, messages[0] );
   obj->move_with_look( destob, messages[2], messages[1] );
   if( obj->query_property( "dead" ) )
      return;

   damage = calc_fall_damage( dam_type );
   if( damage >= obj->query_hp() ) {
      set_death_reason( "plummeting from the rooftops" );
      obj->attack_by( this_object() );
      obj->do_death();
   } else
      obj->adjust_hp( -damage );
} /* do_fall() */

/**
 * @ignore yes
 * Deals with falling through or off roofs. also takes care of dropped
 * items/corpses on sloped roofs.
 */
void event_enter( object obj, object from ) {
   int totalweight, objectweight, i;
   float encum, diff;
   int fall;
   object * contents;
   object destination;

   if( !obj ) return;

   if( obj->query_property( "demon" )
     || obj->query_property( "floating" ) ) {
      return;
   }

   // This property will keep them from leaving until the skillchecks
   // have had a chance to process
   obj->add_property( TOO_SOON, 1, 5 );

   // deal with weak roofs.  Calculate the total weight of the objects
   // here, including carried items.
   if( roof_max_weight ) {
      contents = all_inventory( this_object() );

      for( i = 0; i < sizeof(contents); i++ ) {
         objectweight = contents[i]->query_weight();
         objectweight += contents[i]->query_loc_weight();
         totalweight += objectweight;
#ifdef DEBUG
         debug_printf( "event_enter: Total weight of %s is %d units.\n",
           obj->query_name(), objectweight );
#endif
      }
#ifdef DEBUG
      debug_printf( "event_enter: The current weight on this roof is %d "
        "units. Max weight is set to %d.\n", totalweight, roof_max_weight );
#endif

      // Have we exceeded the maxweight?
      if( totalweight > roof_max_weight ) {
         fall = 1;
         tell_room( environment( obj ), weak_messages[3] );
         if( !(destination = load_object( weak_roof_dest ) ) ) {
            tell_room( this_object(), "Error loading room "
              + weak_roof_dest + ", moving you to the void.\n"
              "Please contact a creator.\n" );
            for( i = 0; i < sizeof(contents); i++ )
               contents[i]->move_with_look( "/room/void" );
            return;
         }
         for( i = 0; i < sizeof(contents); i++ ) {
            call_out( "do_fall", 1, contents[i], destination, "weak",
              weak_messages, 0 );
         }
         return;
      }
   }

   if( gradient && !fall ) {
      if( living( obj ) ) {
         if( ( obj->query_property( "dead" ) )
           || !( obj->query_max_weight() ) ) {
            call_out( "do_fall", 1, obj, slope_dest, "step",
              ghost_fall_messages );
            return;
         }
         encum = ( 100 * to_float( obj->query_loc_weight() ) )
           / to_float( obj->query_max_weight() );
         diff = to_int( sin( ( 3.1415926536 / 180.0 ) * gradient )
           * ( encum * 10 ) );
         call_out( "gradient_check", 1, obj, slope_dest,
           to_int( diff + ( gradient * 2 ) ) );
      } else {
         if( member_array( obj->query_name(),
           ({ "death", "binky" }) ) >= 0 )
            return;
         if( gradient > 3 ) {
            if( obj->query_corpse() ) {
               obj->move( slope_dest,
                 process_mess( corpse_slope_messages[1], obj, 0 ),
                 process_mess( corpse_slope_messages[0], obj, 0 ) );
            } else {
               obj->move( slope_dest,
                 process_mess( item_slope_messages[1], obj, 0 ),
                 process_mess( item_slope_messages[0], obj, 0 ) );
            }
         }
      }
      return;
   }

   // Remove the property that prevents them leaving the room,
   // since there's no skill check forthcoming.
   obj->remove_property( TOO_SOON );
} /* event_enter() */

/** @ignore yes
 *  Internal function to perform the skillcheck for sloping roofs.
 */
void gradient_check( object obj, string destination, int diff ) {
   object destob;

#ifdef DEBUG
   debug_printf( "gradient_check( %s, \"%s\", %d )",
     obj->query_name(), destination, diff );
#endif

   // Remove the property that prevents them leaving the room..
   obj->remove_property( TOO_SOON );

   switch( TASKER->perform_task( obj, ROCK, diff + 1, TM_FIXED ) ) {
   case AWARD:
      tell_object( obj, "%^YELLOW%^" + ({
        "You balance more confidently on the " + place + ".",
        "Climbing becomes easier."
      })[ random(2) ] + "%^RESET%^\n" );
   case SUCCEED:
      tell_object( obj, "The " + place + " is steep, but you manage "
        "not to fall.\n" );
      break;
   case FAIL:
      if( !( destob = load_object( destination ) ) ) {
         tell_object( obj, "Error loading room " + destination +
           ", moving you to the void.\nPlease contact a creator.\n" );
         obj->move_with_look( "/room/void" );
      } else
         do_fall( obj, destob, "slope", slope_messages, 0 );
      break;
   default:
      write( "Gnaaaaaaaaaaaah! You should not be getting this message.  "
        "Please contact a creator.\n" );
      break;
   }
} /* gradient_check() */

/** @ignore yes */
int do_roofjump( string dir ){
   mixed * info;
   int distance;
   float weight, max_weight;
   object destination, fall_destination;
   string key;

   if( !( key = translations[dir] ) || !( info = jump_info[key] ) ) {
      add_failed_mess( "You can't jump there!\n" );
      return 0;
   }

   if( !(destination = load_object( info[0] ) ) ) {
      add_failed_mess( "Error! The file " + info[0] + " does not exist "
        "or does not load. Please contact a creator.\n" );
      return 0;
   }
   if( !(fall_destination = load_object( info[1] ) ) ) {
      add_failed_mess( "Error! The file " + info[1] + " does not exist "
        "or does not load. Please contact a creator.\n" );
      return 0;
   }
   distance = info[2];

   if( distance ) {
      weight = to_float( this_player()->query_loc_weight() );
      max_weight = to_float( this_player()->query_max_weight() );
      // at this rate, a 300-bonus, unencumbered
      // player can handle a 20-foot gap.
      distance *= to_float( ( to_float(weight) * 7) /
           to_float(max_weight) ) + 15;

      switch( TASKER->perform_task( this_player(), ROCK, distance, TM_FIXED ) ) {
         case AWARD:
            write( "%^YELLOW%^"
              + jump_tm_messages[ random( sizeof( jump_tm_messages ) ) ]
              + "%^RESET%^\n" );
         case SUCCEED:
            write( process_mess( jump_success_messages[0], this_player(), dir ) );
            this_player()->move_with_look( destination,
              process_mess( jump_success_messages[2], this_player(), dir ),
              process_mess( jump_success_messages[1], this_player(), dir ), );
            break;
         case FAIL:
            write( process_mess( jump_failure_messages[0], this_player(), dir ) );
            do_fall( this_player(), fall_destination, "jump",
              jump_failure_messages, dir );
            break;
         default:
            write( "Oh dear. Something is broken. Please inform a "
              "creator.\n" );
      }
      return 1;
   }
} /* do_roofjump() */

/** @ignore yes */
string query_death_reason() {
   call_out( (: death_reason = 0 :), 2 );
   if( death_reason )
      return "/global/player"->convert_message( death_reason );
   return "a rooftop (" + file_name( this_object() )
     + ") with an incorrectly set death message";
} /* query_death_reason() */

/** @ignore yes */
void set_death_reason( string str ) {
   death_reason = str;
} /* set_death_reason() */

/** @ignore yes */
string process_string( string str, mapping transforms ) {
   string foo, bar;

   foreach( foo, bar in transforms )
      if( bar ) str = replace_string( str, foo, bar );
   return "/global/player"->convert_message( str );
} /* process_string() */

/** @ignore yes */
string process_mess( string str, object ob, string direction ) {
   string opp_dir;
   string * directions = ({ "north", "northeast", "east", "southeast",
     "south", "southwest", "west", "northwest" });

   if( !str || ( str == "" ) )
      return "";

   if( stringp( direction ) ) {
      if( member_array( direction, directions ) < 0 ) {
         opp_dir = 0;
      } else {
         opp_dir =
           directions[ ( member_array( direction, directions ) + 4 ) % 8 ];
      }
   }

   str = process_string( str, ([
     "$dir$"       : direction,
     "$opp_dir$"   : opp_dir,
     "$short$"     : ob->query_short(),
     "$poss$"      : ob->query_possessive(),
     "$pronoun$"   : ob->query_pronoun(),
     "$obj$"       : ob->query_objective(),
     "$a_short$"   : ob->a_short(),
     "$the_short$" : ob->the_short(),
     "$one_short$" : ob->one_short()
   ]) );

   return implode( explode( str, ". " ),
     (: "$C$" + $1 + ". " + "$C$" + $2 :) );
} /* process_mess() */

int test_remove( object ob, int flag, mixed dest ) {
   if( !living(ob) ) {
      return ::test_remove( ob, flag, dest );
   }

   if( objectp(dest) )
      dest = file_name(dest);
   if( !stringp(dest) || dest == "/room/rubbish" )
      return 1;

   if( ob->query_property( TOO_SOON ) ) {
      tell_object( ob, "You haven't quite caught your balance yet.\n" );
      notify_fail( "" );
      return 0;
   }
   return ::test_remove( ob, flag, dest );
} /* test_remove() */
