/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: water.c,v 1.12 2002/08/03 22:27:04 ceres Exp $
 * $Log: water.c,v $
 * Revision 1.12  2002/08/03 22:27:04  ceres
 * Fixed skill names
 *
 * Revision 1.11  2002/03/09 20:20:04  tannah
 * Made add_property recognise the third parameter.
 *
 * Revision 1.10  2002/03/01 23:28:25  tannah
 * Made it check for use_base_stats before returning modified bonus, and checked
 * for query_verb() == "skills", too.
 * Made it recalculate bonus whenever query_skill_bonus() is called.
 *
 * Revision 1.9  2002/02/21 14:03:50  taffyd
 * Forced it to recalculate the burdening.
 *
 * Revision 1.8  2002/02/21 13:21:17  presto
 *  Forcibly unlocked by taffyd
 *
 * Revision 1.7  2001/06/07 14:47:41  wobin
 * Added in cascading to do_soak for shadowed functions
 *
 * Revision 1.6  1999/05/25 23:39:50  pinkfish
 * Make it more error safe.
 *
 * Revision 1.5  1999/04/06 00:34:04  ceres
 * Modified to allow query_skill_bonus() to work for the swimming skill.
 *
 * Revision 1.4  1999/03/05 21:06:08  pinkfish
 * Fix up some errors that Mystic pointer out.
 *
 * Revision 1.3  1999/03/05 21:04:05  presto
 *  Forcibly unlocked by pinkfish
 *
 * Revision 1.2  1998/05/14 00:43:52  presto
 * Slave work for Jeremy. :)
 *
 * Revision 1.1  1998/01/06 04:39:04  ceres
 * Initial revision
 *
*/
#include <move_failures.h>

#define SWIMMING_SKILL "other.movement.swimming"
#define STAMINA_SKILL "other.health"
#define BUOYANT_PROP "buoyancy"
#define ANCHOR_PROP "anchor"
#define GILLS_PROP "gills"
#define TROLL_RACE "troll"

object swimmer;
int bonus, buoyancy, drown_stage, swimming = 0, recheck_delay = -1;
string sweep_dir = "";


void calc_swim_bonus();
int abs( int i );
void test_sweep();
void test_float();
void test_drown();
string sweep_string( mixed *dest_other_info, int pos );
varargs int exit_command( string word, mixed verb, object thing );

int abs( int i )  {  return i < 0 ? -i : i;  }

int start_floating();
int start_swimming();

void setup_shadow( object this_swimmer )  {
   shadow( this_swimmer, 1 );
   swimmer = this_swimmer;
   calc_swim_bonus();
}  /* setup_shadow() */


void event_enter( object ob, string message, object from )  {
   int old_bonus, old_buoy;

   if (swimmer) {
      swimmer->event_enter( ob, message, from );
   }
   old_bonus = bonus;
   old_buoy = buoyancy;
   calc_swim_bonus();
   if ( bonus != old_bonus )  test_sweep();
   if ( buoyancy != old_buoy )  {
      buoyancy += (int)ob->query_property( BUOYANT_PROP );
      test_float();
   }
   return;
}  /* event_enter() */


void event_exit( object ob, string message, object to )  {
   int old_bonus, old_buoy;

   if (swimmer) {
      swimmer->event_exit(ob, message, to);
   }
   old_bonus = bonus;
   old_buoy = buoyancy;
   calc_swim_bonus();
   if ( bonus != old_bonus )  test_sweep();
   if ( buoyancy != old_buoy )  {
      buoyancy -= (int)ob->query_property( BUOYANT_PROP );
      test_float();
   }
   return;
}  /* event_exit() */


void calc_swim_bonus()  {
   float pct;
   object *held;

   if ( living( swimmer )  &&
        ( held = (object *)swimmer->query_holding() ) )  {

      // Force a recalculation of their weight.
      swimmer->calc_burden();
      bonus = (int)swimmer->query_skill_bonus( SWIMMING_SKILL ) /
                            ( sizeof( held - (object *)({ 0 }) ) + 1 );
      buoyancy = bonus - (int)swimmer->query_loc_weight() +
                      (int)swimmer->query_property( BUOYANT_PROP );
      if ( (string)swimmer->query_race() == TROLL_RACE )  buoyancy -= 300;
      if ( buoyancy < 0  &&  bonus )  {
         pct = -buoyancy / bonus;
         if ( pct < 1.0 )  {
            bonus += buoyancy;
            buoyancy = 0;
         }
         else  {
            pct -= 1.0;
            buoyancy *= pct;
            bonus = 0;
         }
      }
   } else  {
      if (swimmer) {
         bonus = (int)swimmer->query_property( ANCHOR_PROP );
         buoyancy = (int)swimmer->query_property( BUOYANT_PROP ) -
                    (int)swimmer->query_weight();
      }
   }

   return;
}  /* calc_swim_bonus() */


void add_property( string prop, mixed val, int time )  {
   if (swimmer) {
      swimmer->add_property( prop, val, time );
   }
   if ( prop == GILLS_PROP  &&  val > 0 )  remove_call_out("do_drown");
   if ( prop == BUOYANT_PROP  &&  val )  {
      calc_swim_bonus();
      test_float();
   }
   return;
}  /* add_property() */


void remove_property( string prop )  {
   if (swimmer) {
     swimmer->remove_property( prop );
   }
   if ( prop == GILLS_PROP )  test_drown();
   else if ( prop == BUOYANT_PROP )  {
      calc_swim_bonus();
      test_float();
   }
   return;
}  /* remove_property() */


int add_skill_level( string skill, int lev )  {
   int lvl;

   if ( living(swimmer) )  {
      lvl = (int)swimmer->add_skill_level( skill, lev );
      if ( skill == SWIMMING_SKILL  &&  lev )  {
         calc_swim_bonus();
         test_sweep();
         test_float();
      }
      return lvl;
   }
   else return 0;
}  /* add_skill_level() */


/* Added to allow the use of query_skill_bonus directly and still
 * obtain the correct result. -- Ceres
 */
varargs int query_skill_bonus( string skill, int use_base_stats ) {
  if( ( skill == SWIMMING_SKILL ) &&
      ( !use_base_stats ) &&
      ( query_verb() != "skills" ) ) {
    calc_swim_bonus();
    return bonus;
  }
  return swimmer->query_skill_bonus(skill, use_base_stats);
}

int query_swim_bonus()  {  return bonus;  }

int query_buoyancy()  {  return buoyancy;  }


void test_sweep()  {
   mapping flows, possible;
   int total_flow, r, f, s, flow_rate;
   string *dirs, dir, *dest_dir_info;
   object room;

   s = find_call_out( "do_sweep" );
   room = environment( swimmer );
   flows = (mapping)room->query_flows();
   dest_dir_info = (string *)room->query_dest_dir();
   f = ( 300 - ( evaluate( flows[ sweep_dir ] ) - swimming * bonus ) ) / 30;
   if ( f < 0 )  f = 0;
   f += room->query_min_sweep_delay( sweep_dir );
   if ( s > f )  {
      remove_call_out( "do_sweep" );
      call_out( "do_sweep", f,
                dest_dir_info[ member_array( sweep_dir,
                                             dest_dir_info ) + 1 ] );
   }
   else if ( s == -1  ||
             (int)room->query_flow( sweep_dir ) - swimming * bonus <= 0 )  {
      remove_call_out( "do_sweep" );
      dirs = keys( flows );
      total_flow = 0;
      possible = ([ ]);
      foreach ( dir in dirs )  {
         flow_rate = evaluate( flows[ dir ] );
         if ( ( f = flow_rate - swimming * bonus ) > 0  &&  flow_rate > 0)  {
            total_flow += f;
            possible += ([ dir : f ]);
         }
      }

      r = random( total_flow );
      dirs = keys( possible );
      total_flow = 0;

      foreach ( dir in dirs )  {
         if ( r < total_flow + possible[ dir ] )  {
            sweep_dir = dir;
            f = ( 300 - possible[ dir ] ) / 30;
            if ( f < 0 )  f = 0;
            f += room->query_min_sweep_delay( dir );
//tell_object(swimmer, "Calling do sweep, delay = " + f + ", direction = " + dir + "\n" );
            call_out( "do_sweep", f,
                      dest_dir_info[ member_array( sweep_dir,
                                                   dest_dir_info ) + 1 ] );
            return;
         }
         else total_flow += possible[ dir ];
      }
   }
   return;
}  /* test_sweep() */


void test_float()  {
   object room;
   int s, t;

   room = environment( swimmer );
   if ( buoyancy < 0 )  {
      remove_call_out( "do_rise" );
      s = find_call_out( "do_sink" );
      if ( !( room->query_bottom() ) )  {
         t = (300 + buoyancy) / 30;
         if ( s > t )  {
            remove_call_out( "do_sink" );
            s = -1;
         }
         if ( s == -1 )  {
            call_out( "do_sink", t + room->query_min_sweep_delay( room->
                         query_down_dir() ) );
            swimmer->add_property( "there",
                                   (string)room->query_nonfloat_mess() );
         }
      }
      else  {
         swimmer->add_property( "there", (string)room->query_bottom_mess() );
         if ( s > -1 )  remove_call_out( "do_sink" );
      }
   }
   else if ( buoyancy > bonus )  {
      remove_call_out( "do_sink" );
      s = find_call_out( "do_rise" );
      if ( !( room->query_surface() ) )  {
         t = (300 - buoyancy) / 30;
         if ( s > t )  {
            remove_call_out( "do_rise" );
            s = -1;
         }
         if ( s == -1 )  {
            call_out( "do_rise", t + room->query_min_sweep_delay( room->
                         query_up_dir() ) );
            swimmer->add_property( "there",
                                   (string)room->query_float_mess() );
         }
      }
      else if ( s > -1 )  remove_call_out( "do_rise" );
   }
   else  {
      remove_call_out( "do_rise" );
      remove_call_out( "do_sink" );
      swimmer->add_property( "there", (string)room->query_float_mess() );
   }
   return;
}  /* test_float() */


void test_drown()  {
   int delay;

   if ( environment( swimmer )->query_surface() )
      remove_call_out("do_drown");
   else if ( find_call_out( "do_drown" ) == -1  &&
             !( swimmer->query_property( GILLS_PROP ) ) )  {
      delay = (int)swimmer->query_skill_bonus( STAMINA_SKILL ) / 20;
      if ( delay < 15 )  delay = 15;
      call_out( "do_drown", delay );
      drown_stage = 0;
   }
   return;
}  /* test_drown() */


void do_sweep( string dest )  {
   mixed *dest_other_info;
   object room;

   room = environment(swimmer);
   if ( environment( swimmer )->query_terrain() )
      environment( swimmer )->set_destination( sweep_dir );
   dest_other_info = (mixed *)room->query_dest_other();
   if ( !( swimmer->query_property("player") ) )
      swimmer->move( dest,
                     replace_string( (string)room->query_sweep_in_mess(),
                                     "$F",sweep_string( dest_other_info,
                                        member_array( sweep_dir,
                                          dest_other_info ) + 1 ) ),
                     replace_string( (string)room->query_sweep_out_mess(),
                                     "$T", sweep_dir ) );
   else  {
      tell_object( swimmer, "The current pulls you " + sweep_dir + ".\n" );
      swimmer->move_with_look( dest,
                               replace_string( (string)room->
                                               query_sweep_in_mess(), "$F",
                                               sweep_string( dest_other_info,
                                                 member_array( sweep_dir,
                                                   dest_other_info ) + 1 ) ),
                               replace_string( (string)room->
                                               query_sweep_out_mess(), "$T",
                                               sweep_dir ) );
   }
   return;
}  /* do_sweep() */


void do_sink()  {
   string dir;
   mixed *dest_other_info;
   int i;
   object room;

   room = environment(swimmer);
   dir = (string)room->query_down_dir();
   if ( environment( swimmer )->query_terrain() )
      environment( swimmer )->set_destination( dir );
   dest_other_info = (mixed *)room->query_dest_other();
   if ( ( i = member_array( dir, dest_other_info ) ) > -1 )  {
      if ( !( swimmer->query_property("player") ) )
         swimmer->move( dest_other_info[ i + 1 ][0],
                        replace_string( (string)room->query_sink_in_mess(),
                                        "$F", sweep_string( dest_other_info,
                                          i + 1 ) ),
                        replace_string( (string)room->query_sink_out_mess(),
                                        "$T", dir ) );
      else  {
         tell_object( swimmer, "You sink toward the bottom.\n" );
         swimmer->move_with_look( dest_other_info[ i + 1 ][0],
                                  replace_string( (string)room->
                                                          query_sink_in_mess(),
                                        "$F", sweep_string( dest_other_info,
                                          i + 1 ) ),
                                  replace_string( (string)room->
                                                     query_sink_out_mess(),
                                                  "$T", dir ) );
      }
   }
   return;
}  /* do_sink() */


void do_rise()  {
   string dir;
   mixed *dest_other_info;
   int i;
   object room;

   room = environment( swimmer );
   dir = (string)room->query_up_dir();
   if ( environment( swimmer )->query_terrain() )
      environment( swimmer )->set_destination( dir );
   dest_other_info = (mixed *)room->query_dest_other();
   if ( ( i = member_array( dir, dest_other_info ) ) > -1 )  {
      if ( !( swimmer->query_property("player") ) )
         swimmer->move( dest_other_info[ i + 1 ][0],
                        replace_string( (string)room->query_float_in_mess(),
                                        "$F", sweep_string( dest_other_info,
                                          i + 1 ) ),
                        replace_string( (string)room->query_float_out_mess(),
                                        "$T", dir ) );
      else  {
         tell_object( swimmer, "You drift toward the surface.\n" );
         swimmer->move_with_look( dest_other_info[ i + 1 ][0],
                                  replace_string( (string)room->
                                                     query_float_in_mess(),
                                     "$F", sweep_string( dest_other_info,
                                       i + 1 ) ),
                                  replace_string( (string)room->
                                                     query_float_out_mess(),
                                                  "$T", dir ) );
      }
   }
   return;
}  /* do_rise() */


void do_drown()  {
   string *exits, up;
   int delay;

   delay = (int)swimmer->query_skill_bonus( STAMINA_SKILL ) / 5;
   if ( delay < 15 )  delay = 15;
   switch ( drown_stage )  {
      case 0:
         tell_object( swimmer, "Your lungs start to feel a bit heavy.\n" );
         tell_room( environment( swimmer ), (string)swimmer->query_short() +
                    " begins to look a bit uncomfortable.\n", swimmer );
         call_out( "do_drown", delay );
         break;
      case 1:
         tell_object( swimmer, "Your lungs are starting to burn.\n" );
         tell_room( environment( swimmer ), (string)swimmer->query_short() +
                    " starts to look slightly blue.\n", swimmer );
         call_out( "do_drown", delay );
         break;
      case 2:
         tell_object( swimmer, "Your lungs are fairly bursting.\n" );
         tell_room( environment( swimmer ), (string)swimmer->query_short() +
                    " begins to look panicky.\n", swimmer);
         call_out( "do_drown", delay );
         break;
      default:
         swimmer->adjust_hp( ( 2 - drown_stage ) * 5 *
                             ( 30 - (int)swimmer->query_con() ) );
         if ( swimmer->query_hp() > 0 )  {
            call_out( "do_drown", delay );
            exits = (string *)environment( swimmer )->query_dest_dir();
            if ( member_array( up = (string)environment( swimmer )->
                                            query_up_dir(),
                               exits ) > -1 )  {
               tell_object( swimmer, "You panic and try to flee for the "
                            "surface.\n" );
               tell_room( environment( swimmer ),
                          (string)swimmer->query_short() + " panics and madly "
                          "tries to flee for the surface.\n", swimmer );
               swimmer->exit_command( up );
            }
            else  {
               up = exits[ random( sizeof( exits ) ) / 2 ];
               tell_object( swimmer, "You panic and try to flee " + up +
                            ".\n" );
               tell_room( environment( swimmer ),
                          (string)swimmer->query_short() + " panics and tries "
                          "to flee " + up + ".\n", swimmer );
               swimmer->exit_command( up );
            }
         }
         else swimmer->attack_by( environment( swimmer ) );
         break;
   }
   ++drown_stage;
   return;
}  /* do_drown() */


// Changed slightly by Ceres to make it simpler, if youre swimming it
// makes you soaked immediately now.
void do_soak()  {
   swimmer->add_effect("/std/effects/other/wetness", swimmer->query_weight());
   swimmer->do_soak();
}  /*  do_soak()  */


void cancel_sweep()  {
   remove_call_out( "do_sweep" );
   sweep_dir = "";
   return;
}  /* cancel_sweep() */


void dest_water_shadow()  {
   remove_call_out( "do_sweep" );
   remove_call_out( "do_rise" );
   remove_call_out( "do_sink" );
   remove_call_out( "do_drown" );
   remove_call_out( "do_soak" );
   remove_call_out( "test_again" );
   if (swimmer) {
      swimmer->remove_property("there");
   }
   destruct( this_object() );
   return;
}  /* dest_water_shadow() */


object find_water_shadow()  {  return this_object();  }


int command_shadowed( string verb, string args )  {
   string my_mess, others_mess;

   my_mess = 0;
   if ( !( environment( swimmer )->query_surface() ) )  {
      if ( verb == "say"  ||  verb == "'" )  {
         my_mess = "You try to talk, but only generate some bubbles.";
         others_mess = (string)swimmer->query_short() + " emits a 'glub glub' "
                       "noise.";
         if ( !( swimmer->query_property( GILLS_PROP ) ) )  {
            my_mess += "  In the process you inhale some water.\n";
            others_mess += "  In the process, " +
                           (string)swimmer->query_pronoun() + " inhales "
                           "some water.\n";
            ++drown_stage;
         }
         else  {
            my_mess += "\n";
            others_mess += "\n";
         }
      }
      else if ( verb == "lsay" )  {
         my_mess = "You try to speak loudly, but can only produce a lot of "
                   "bubbles.";
         others_mess = (string)swimmer->query_short() + " produces a sort "
                       "of 'glooob gloob' sound.";
         if ( !( swimmer->query_property( GILLS_PROP ) ) )  {
            my_mess += "  You also inhale a fair amount of water.\n";
            others_mess += "  " + (string)swimmer->query_pronoun() + "also "
                           "inhales a fair amount of water.\n";
            drown_stage += 2;
         }
         else  {
            my_mess += "\n";
            others_mess += "\n";
         }
      }
      else if ( verb == "shout" )  {
         my_mess = "You try to shout, but your main effects are a muffled "
                   "'arrrble' and a lot of bubbles.";
         others_mess = (string)swimmer->query_short() + " open " +
                       (string)swimmer->query_possessive() + " mouth and "
                       "emits a muffled 'arrrble' noise.";
         if ( !( swimmer->query_property( GILLS_PROP ) ) )  {
            my_mess += "  You also inhale about a lungful of water.\n";
            others_mess += "  " + (string)swimmer->query_pronoun() + " sucks "
                           "in a large amount of water in the process.\n";
            drown_stage += 3;
         }
         else  {
            my_mess += "\n";
            others_mess += "\n";
         }
      }
   }
   if ( my_mess )  {
      tell_object( swimmer, my_mess );
      tell_object( environment( swimmer ), others_mess );
      return 1;
   }
   else return (int)swimmer->command_shadowed( verb, args );
}  /* command_shadowed() */


int *set_hold( object ob, int pos )  {
   int *other, old_bonus;

   old_bonus = bonus;
   other = (int *)swimmer->set_hold( ob, pos );
   calc_swim_bonus();
   if ( old_bonus != bonus )  {
      test_sweep();
      test_float();
   }
   return other;
}  /* set_hold() */


void do_death( object thing )  {
   if (swimmer) {
      swimmer->do_death( thing );
   }
   remove_call_out( "do_sweep" );
   remove_call_out( "do_rise" );
   remove_call_out( "do_sink" );
   remove_call_out( "do_drown" );
   remove_call_out( "do_soak" );
   swimmer->remove_property("there");
   return;
}  /* do_death() */


void remove_ghost()  {
  swimmer->remove_ghost();
  environment( swimmer )->event_enter( swimmer, "", 0 );
  return;
}  /* remove_ghost() */


int *set_unhold( object ob )  {
   int *other, old_bonus;

   old_bonus = bonus;
   other = (int *)swimmer->set_unhold( ob );
   calc_swim_bonus();
   if ( old_bonus != bonus )  {
      test_sweep();
      test_float();
   }
   return other;
}  /* set_unhold() */


int move( mixed dest, string messin, string messout )  {
   string where, *dest_dir_info;
   int pos;

   if ( !swimmer->query_property( "dead" )  &&  living( swimmer ) )  {
      if ( objectp( dest ) )  where = file_name( dest );
      else where = dest;
      dest_dir_info = (string *)environment( swimmer )->query_dest_dir();
      pos = member_array( where, dest_dir_info ) - 1;
      if ( pos > -1  &&
           !(int)environment( swimmer )->
                    attempt_exit( dest_dir_info[ pos ], swimmer ) )  {
         notify_fail( "" );
         return MOVE_NO_DROP;
      }
   }
   return swimmer->move( dest, messin, messout );
}  /* move() */



int do_float()  {
   if ( !swimming )  {
      tell_object( swimmer, "You are already drifting with the current.\n" );
   }
   else  {
      tell_object( swimmer, "You stop resisting the current.\n" );
      swimming = 0;
      test_sweep();
   }
   return 1;
}  /* do_float() */


int do_swim()  {
   if ( swimming )  {
      tell_object( swimmer, "You are already fighting the current.\n" );
   }
   else  {
      tell_object( swimmer, "You start to resist the current.\n" );
      swimming = 1;
      test_sweep();
   }
   return 1;
}  /* do_swim() */


void test_again()  {
   test_float();
   test_sweep();
   call_out( "test_again", recheck_delay );
}


void update_recheck( int time_out )  {
   int t;

//tell_object( swimmer, "Called update...\n" );
   recheck_delay = time_out;
   if ( time_out == -1 )  {
      remove_call_out( "test_again" );
   }
   else if ( ( t = find_call_out( "test_again" ) ) == -1 )  {
      call_out( "test_again", time_out );
   }
   else if ( time_out < t )  {
      remove_call_out( "test_again" );
      call_out( "test_again", time_out );
   }
   return;
}


string sweep_string( mixed *dest_other_info, int pos )  {
   if ( pointerp( dest_other_info[pos][5] ) )  {
      return dest_other_info[pos][5][1];
   }
   else  {
      return dest_other_info[pos][5];
   }
}
