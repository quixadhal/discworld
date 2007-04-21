/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: taskmaster.c,v 2.13 2003/02/26 02:29:47 ceres Exp $
 */

/**
 * This handler gives access to the functions needed for the taskmaster
 * system.
 *
 * Rewritten by Shrike to include degrees of success.
 *
 * @author Deutha
 * @see help::tasks
 */
#include <tasks.h>

#define TIME_PER_USER 1800
#define SAVE_FILES "/save/tasks/"

//#define DEBUG "ceres"
//#define WATCH_PLAYER "serec"
//#define MIN_LEVEL 200
#define LOG_STATS 2
#undef LOGGING

mapping stats;
nosave int last_save;

nosave int last;
nosave string skill;
nosave mixed *control;
nosave int * critical_chances;

varargs mixed perform_task( object person, string skill, int difficulty,
                         int tm_type, int degree );
varargs mixed attempt_task( int difficulty, int bonus, int upper,
                            mixed extra, int degree );
varargs mixed attempt_task_e( int difficulty, int bonus, int upper,
                              int half, int degree );
int is_critical( int margin );

/** @ignore yes */
void create() {
   float a, b;

   seteuid( "Root" );

   // It's fairly processor intensive to calculate exponential values,
   // So we'll do them once and keep them in a lookup table. Equation
   // is y = a*e^(b*i), with constants chosen such that
   // y(1) = 1 and y(100) = 1000. (these numbers are chance out of 10000
   // that a critical will occur)
   critical_chances = allocate(100);
   a = 0.93260;
   b = 0.06978;
   for( int i = 0; i < 100; i++ ) {
      critical_chances[i] = a * exp( b * ( i + 1 ) );
   }
} /* create() */

/** @ignore yes */
mapping query_stats( string s_name ) {
  if ( skill != s_name ) {
    skill = s_name;
    if ( file_size( SAVE_FILES + skill +".o" ) > 0 )
      unguarded( (: restore_object, SAVE_FILES + skill :) );
    else
      stats = 0;
  }
  if ( !stats )
    stats = ([ ]);
  return copy( stats );
} /* query_stats() */

/** @ignore yes */
int query_last() { return last; }

/** @ignore yes */
mixed *query_control() { return control; }

/**
 * This function should only be used in the very rare situations
 * where the last skill checked with query_skill_bonus() wasn't the
 * one relevant for the task attempt.
 *
 * @param args an array consisting of ({ object player, string skill })
 */
void set_control( mixed *args ) { control = args; }

/** @ignore yes */
void reset_control() { control = 0; }

/** @ignore yes */
void award_made( string p_name, string o_name, string s_name, int level ) {
  user_event( "inform", p_name +" gains a level in "+ s_name +" from "+
              o_name + " at level " + level, "skill" );
#ifdef LOG_STATS
#if LOG_STATS == 1
  query_stats( s_name );
  if ( !stats[ level ] )
    stats[ level ] = ([ ]);
  stats[ level ][ explode( o_name, "#" )[ 0 ] ]++;
#else
  query_stats("summary");

  // these two just skew the stats so we don't record them.
  if(s_name[<7..] == ".points" || s_name[0..13] == "other.language")
    return;

  s_name = explode(s_name, ".")[0];
  if(!stats[s_name])
    stats[s_name] = ({ ({ 0 , 0 }),  ({ 0, 0 }),  ({ 0, 0 }) , ({0,0})});

  switch(level) {
  case 0..149:
    stats[s_name][0] = ({ stats[s_name][0][0], stats[s_name][0][1]+1 });
    break;
  case 150..299:
    stats[s_name][1] = ({ stats[s_name][1][0], stats[s_name][1][1]+1 });
    break;
  case 300..599:
    stats[s_name][2] = ({ stats[s_name][2][0], stats[s_name][2][1]+1 });
    break;
  default:
    stats[s_name][3] = ({ stats[s_name][3][0], stats[s_name][3][1]+1 });
    break;
  }

  if(last_save < time() - 300) {
    unguarded( (: save_object, SAVE_FILES + "summary" :) );
    last_save = time();
  }
#endif
#endif
} /* award_made() */

#if LOG_STATS == 2
/** @ignore yes */
void skill_checked(string s_name, int level) {
  query_stats("summary");
  if(s_name[<7..] == ".points" || s_name[0..13] == "other.language")
    return;
  s_name = explode(s_name, ".")[0];
  if(!stats[s_name])
    stats[s_name] = ({ ({0,0}), ({0,0}), ({0,0}), ({0,0}), ({0,0}) });

  switch(level) {
  case 0..49:
    stats[s_name][0] = ({ stats[s_name][0][0]+1, stats[s_name][0][1] });
    break;
  case 50..149:
    stats[s_name][1] = ({ stats[s_name][1][0]+1, stats[s_name][1][1] });
    break;
  case 150..299:
    stats[s_name][2] = ({ stats[s_name][2][0]+1, stats[s_name][2][1] });
    break;
  case 300..599:
    stats[s_name][3] = ({ stats[s_name][3][0]+1, stats[s_name][3][1] });
    break;
  default:
    stats[s_name][4] = ({ stats[s_name][4][0]+1, stats[s_name][4][1] });
    break;
  }
  if(last_save < time() - 300) {
    unguarded( (: save_object, SAVE_FILES + "summary" :) );
    last_save = time();
  }
}
#endif

/** @ignore yes */
string *query_skill_files() {
  return unguarded( (: get_dir, SAVE_FILES +"*.o" :) );
} /* query_skill_files() */

/** @ignore yes */
void clear_skill_files() {
  string word;
  foreach ( word in unguarded( (: get_dir, SAVE_FILES +"*.o" :) ) )
    unguarded( (: rm, SAVE_FILES + word :) );
  skill = 0;
} /* clear_skill_files() */

/**
 * This function will compare the skills of two objects and return which one
 * won and if the winner got a TM.
 * With equal skills the chances are 50/50 as to who will win. As the balance
 * shifts so do the chances. Additionally a modifier can be applied to
 * account for conditions favouring one or the other. This should be
 * considered a percentage eg. -50 will add 50% to the defenders chances of
 * winning.
 * If
 *
 * @param offob The attacking object
 * @param offskill The name of the skill the attacker is using
 * @param deffob The defending object
 * @param deffskill The name of the skill the defender is using
 * @param modifier The percentage modifier
 * @param off_tm_type This should be one of the standard definitions in
 * /include/tasks.h and is for the attacker
 * @param def_tm_type This should be one of the standard definitions in
 * /include/tasks.h and is for the defender
 * @param degree Enable the degree-of-success code.  If this is used, the
 *  return value becomes a class instead of an int.
 *
 * @example
 * This is an example of the simpler, binary success/failure behaviour.
 *
 * switch(TASKER->compare_skills(attacker, "fighting.combat.melee.sharp",
 *                               defender, "fighting.combat.parry.unarmed",
 *                               25, TM_COMMAND, TM_FREE) {
 * case OFFAWARD:
 *   tell_object(attacker, "%^YELLOW%^You manage to grasp one of the "
 *                         "principles of slicing people up more firmly."
 *                         "%^RESET%^\n");
 * case OFFWIN:
 *   say(defender->one_short() + " loses an arm!\n");
 *   break;
 * case DEFAWARD:
 *   tell_object(defender, "%^YELLOW%^You feel better able to keep your arms "
 *                         "attached when parrying unarmed.%^RESET%^\n");
 * case DEFWIN:
 *   say(defender->one_short() + " keeps " + defender->query_possesive() +
 *       " arm attached.\n");
 *   break;
 * }
 *
 * @example
 * This is an example of the finer grained degree-of-success behaviour.
 *
 * class tasker_result tr;
 *
 * tr = (class tasker_result)TASKER->compare_skills(
 *   attacker, "fighting.combat.melee.sharp",
 *   defender, "fighting.combat.parry.unarmed",
 *   25, TM_COMMAND, TM_FREE, 1 );
 *
 * switch( tr->result ) {
 * case OFFAWARD:
 *   tell_object(attacker, "%^YELLOW%^You manage to grasp one of the "
 *                         "principles of slicing people up more firmly."
 *                         "%^RESET%^\n");
 * case OFFWIN:
 *   switch( tr->degree ) {
 *   case TASKER_MARGINAL:
 *     say( "You just barely scrape your opponent's hand.\n" );
 *     break;
 *   case TASKER_NORMAL:
 *     say( "You slice into your opponent's arm.\n" );
 *     break;
 *   case TASKER_EXCEPTIONAL:
 *     say( "You slice deeply into your opponent's muscle and sinew, and "
 *       "he screams in pain!\n" );
 *     break;
 *   case TASKER_CRITICAL:
 *     say( "You slice your opponent's arm clean off.  Blood spurts "
 *       "everywhere like a cherry slurpee machine gone insane!\n" );
 *     break;
 *   }
 *   break;
 * case DEFAWARD:
 *   tell_object(defender, "%^YELLOW%^You feel better able to keep your arms "
 *                         "attached when parrying unarmed.%^RESET%^\n");
 * case DEFWIN:
 *   switch( tr->degree ) {
 *      ...
 *      similar code in here...
 *      ...
 *   }
 *   break;
 * }
 *
 * @see perform_task()
 * @see tasker_result
 *
 */
varargs mixed compare_skills( object offob, string offskill,
                              object defob, string defskill,
                              int modifier, int off_tm_type,
                              int def_tm_type, int degree )
{
  int offbonus, defbonus;
  int perc, chance;
  int success_margin, res, deg;

  // Make sure that we actually have two objects and the associated skills
  // to check against..
  if( !offob || !defob || !offskill || !defskill )
    return BARF;

  offbonus = offob->query_skill_bonus(offskill);
  defbonus = defob->query_skill_bonus(defskill);

  if( !defbonus ) {
    defbonus = 1;
  }
  if( !offbonus ) {
    offbonus = 1;
  }

  // This gives us a sliding scale where off == def is 50 and it climbs
  // from there hitting 0 at def = 2*off and 100 at off = 2*def.
  (offbonus > defbonus) ?
    perc = (50 * offbonus * offbonus) / (offbonus * defbonus) :
    perc = 100 - (50 * defbonus * defbonus) / (offbonus * defbonus);

  perc += modifier; // the difficulty may be weighted.

  // Always got a small chance :)
  if(perc > 99)
    perc = 99;
  if(perc < 1)
    perc = 1;

  chance = random(100);
  success_margin = perc - chance;
  if( success_margin > 0 ) {
    // winner is 1 now do a TM check.
    switch( perform_task( offob, offskill, defbonus-modifier,
                          off_tm_type, 0 ) )
    {
    case AWARD:
      res = OFFAWARD;
      break;
    default:
      res = OFFWIN;
    }

    if( degree ) {
      if( is_critical(success_margin) ) {
        deg = TASKER_CRITICAL;
      } else {
         if( success_margin < TASKER_MARGINAL_UPPER ) {
           deg = TASKER_MARGINAL;
         } else if( success_margin < TASKER_NORMAL_UPPER ) {
           deg = TASKER_NORMAL;
         } else {
           deg = TASKER_EXCEPTIONAL;
         }
      }

      return new( class tasker_result,
                  result : res,
                  degree : deg,
                  raw    : success_margin );
    } else {
      return res;
    }
  } else {
    // winner is 2
    switch( perform_task( defob, defskill, offbonus-modifier,
                          def_tm_type, 0 ) ) {
    case AWARD:
      res = DEFAWARD;
      break;
    default:
      res = DEFWIN;
    }

    if( degree ) {
      if( is_critical(success_margin) ) {
        deg = TASKER_CRITICAL;
      } else {
         if( -success_margin < TASKER_MARGINAL_UPPER ) {
           deg = TASKER_MARGINAL;
         } else if( -success_margin < TASKER_NORMAL_UPPER ) {
           deg = TASKER_NORMAL;
         } else {
           deg = TASKER_EXCEPTIONAL;
         }
      }

      return new( class tasker_result,
                  result : res,
                  degree : deg,
                  raw    : success_margin );
    } else {
      return res;
    }
  }
}

/**
 * This function will perform a task.  It handles all the stuff about
 * looking up the skill, and giving the tm advance, but doesn't give
 * any messages to the player, you'll have to do that.
 * If you set the 'degree' parameter to 1, it will return a 3-element
 * class instead of the standard binary success/failure result.
 *
 * @param person the one attempting the task (could be any living thing)
 * @param skill the skill tested against
 * @param difficulty the lowest bonus where the attempt can succeed
 * @param tm_type This should use one of the standard definitions in 
 * /include/tasks.h. They are:
 *        TM_FIXED - for use where the difficulty is a fixed value
 *        TM_FREE  - for use when the tm attempt doesn't cost anything.
 *        TM_CONTINUOUS - for use in continuous actions eg. combat or sneak
 *        TM_COMMAND - for use with guild commands
 *        TM_RITUAL - when the action is a ritual
 *        TM_SPELL - when the action is a spell
 *        TM_NONE - when you want there to be no chance of a tm
 * @param degree Whether or not to use the degree of success code
 *
 * @return BARF if something screwed up, AWARD if the task succeeded, and
 * should give an advance, SUCCEED if it succeeded, FAIL if it failed. If
 * the degree of success code is used, it will pass out a 3-element class.
 *
 * @example
 * This is an example of the simpler, binary success/failure behaviour.
 *
 * switch(TASKER->perform_task( person, "covert.manipulation.stealing",
 *                              300, TM_COMMAND ) ) {
 *  case AWARD :
 *    tell_object( person, "%^YELLOW%^You manage to grasp the principles "
 *                "of stealing more firmly.%^RESET%^\n" );
 *    // Note, no break;
 *  case SUCCEED :
 *    // Whatever happens when it succeeds
 *    break;
 *  default :
 *    // Whatever happens when it fails
 * }
 *
 * @example
 * This is an example of the finer grained degree-of-success behaviour.
 *
 * class tasker_result tr;
 *
 * tr = (class tasker_result)TASKER->perform_task( person,
 *   "other.direction", 300, TM_FREE, 1 );
 * switch( tr->result ) {
 * case AWARD:
 *   tell_object(attacker, "%^YELLOW%^You feel very aligned!%^RESET%^\n" );
 * case SUCCEED:
 *    switch( tr->degree ) {
 *    case TASKER_MARGINAL:
 *       say( "You think he went thataway. Maybe.\n" );
 *       break;
 *    case TASKER_NORMAL:
 *       say( "You're pretty sure he went that way.\n" );
 *       break;
 *    case TASKER_EXCEPTIONAL:
 *       say( "He definitely went thataway!\n" );
 *       break;
 *    case TASKER_CRITICAL:
 *       say( "Your surity that he went thataway is so powerful that even "
 *         "if he didn't go thataway, he'll be in that direction.\n" );
 *       break;
 *   }
 *   break;
 * case FAIL:
 *   switch( tr->degree ) {
 *      ...
 *      Boy, it's a good thing I've already explained this, or I'd be
 *      pretty hard-pressed to think of an example of a critical failure
 *      for other.direction.  You get the idea.
 *      ...
 *   }
 *   break;
 * }
 *
 * @see compare_skills()
 * @see tasker_result
 *
 */
varargs mixed perform_task( object person, string skill, int difficulty,
                            int tm_type, int degree ) {
  mixed result;
  int bonus, upper;
  
  // Make sure that we actually have an object and a skill to check against..
  if( !person || !skill )
    return BARF;
  
  bonus = person->query_skill_bonus( skill );
  
  switch(tm_type) {
  case TM_FIXED:
    result = attempt_task( difficulty, bonus, 100, 0, degree );
    break;
  case TM_FREE:
    result = attempt_task( difficulty, bonus, 25, 0, degree );
    break;
  case TM_CONTINUOUS:
    result = attempt_task( difficulty, bonus, 50, 0, degree );
    break;
  case TM_COMMAND:
    if( explode( skill, "." )[0] == "covert" )
      result = attempt_task_e 
  ( difficulty, bonus, 60, 40, degree );
    else
      result = attempt_task( difficulty, bonus, 100, 0, degree );
    break;
  case TM_RITUAL:
    result = attempt_task_e( difficulty, bonus, 50, 25, degree );
    break;
  case TM_SPELL:
    result = attempt_task_e( difficulty, bonus, 60, 40, degree );
    break;
  case TM_NONE:
    result = attempt_task_e(difficulty, bonus, 1, 0, degree);
    if(classp(result) && ( (class tasker_result)result )->result == AWARD) {
      result->result = SUCCEED;
    } else if( result == AWARD ) {
      result = SUCCEED;
    }
    break;
  default:                     // this is for backward compatibility
#ifdef LOGGING
    if(file_name(previous_object())[0..2] != "/w/")
      log_file("ATTEMPT_TASK",
               "%s Object %s using old perform_task [%d, %d]\n",
               ctime(time()), base_name(previous_object()), tm_type, degree );
#endif
    upper = tm_type;
    if ( !upper )
      upper = 100;
    result = attempt_task( difficulty, bonus, upper, 0, degree );   
  }
  
  
  if(classp(result) && ( (class tasker_result)result )->result == AWARD ||
     result == AWARD) {
    
    if(person->advancement_restriction() ||
       !person->add_skill_level(skill, 1, previous_object())) {
      if(classp(result))
        result->result = SUCCEED;
      else
        result = SUCCEED;
    }
  }
  return result;
} /* perform_task() */

/*
 * This function will attempt a task and return whether it succeedd.<br>
 * <br>
 * Chance of      /|\<br>
 * success  100% __|            ______..<br>
 *                 |           /<br>
 *                 |          /<br>
 *                 |         /<br>
 *            0% __| .._____/<br>
 *                 |_____________________\   Player's bonus<br>
 *                          |   |        /<br>
 *                 difficulty   difficulty + margin<br>
 * <br>
 * Chance of       /|\<br>
 * gaining  upper __|        .<br>
 *                  |        |\<br>
 *                  |        | \<br>
 *                  |        |  \<br>
 *             0% __| .._____|   \_____..<br>
 *                  |_____________________\  Player's bonus<br>
 *                           |   |        /   <br>
 *                  difficulty   difficulty + margin<br>
 * <br>
 * @param difficulty the lowest bonus where the attempt can succeed
 * @param bonus the bonus the player has in the relevant skill
 * @param upper the maximum chance of getting an advance
 * @param extra the margin control. If it is an int, the margin is set
 * to extra. It it's 0 the margin will be calculated automatically from
 * the difficulty as 3*sqrt(difficulty), if it's an array, it'll be
 * calculated as extra[0]+extra[1]*sqrt(difficulty)
 * @param degree set to one if we want to get a degree of success value
 * returned.
 * @return BARF if something screwed up, AWARD if the task succeeded, and
 * should give an advance, SUCCEED if it succeeded, FAIL if it failed. If
 * degree is one, return an array, where element 0 is one of the above
 * codes, and element 1 is the percentage by which the task was succeeded
 * or failed.
 */
/**
 * Do not use this function. Use perform_task instead.
 * @see perform_task()
 */
varargs mixed attempt_task( int difficulty, int bonus, int upper,
                            mixed extra, int degree )
{
   int margin, success_margin, deg, res;
   float tmp;

#ifdef LOGGING
   if( call_stack(2)[1] != "perform_task" &&
       base_name( previous_object() ) != "/std/effects/fighting/combat" &&
       file_name( previous_object() )[0..2] != "/w/" )
   {
     log_file("ATTEMPT_TASK", "%s Object %s is using old attempt_task "
              "[%O]\n",
              ctime( time() ), base_name( previous_object() ), call_stack(2) );
   }
#endif

#if LOG_STATS == 2
   if(pointerp(control) && control[1]) {
      skill_checked(control[1], control[0]->query_skill(control[1]));
   }
#endif

   /* If the bonus is below the difficulty, they fail. */
   if ( bonus < difficulty ) {
#ifdef DEBUG
      if(pointerp(control) && control[0]->query_name() == WATCH_PLAYER &&
         find_player(DEBUG))
      {
         tell_creator( DEBUG, "%^RED%^"
           + sprintf( "TM: %s Skill: %s [%d] [%d] Failed\n",
                      control[0]->query_name(), control[1], bonus, difficulty )
           + "%^RESET%^" );
      }
#endif
      if( degree ) {
         return new( class tasker_result,
                     result : FAIL,
                     degree : ( is_critical( -100 ) ?
                                TASKER_CRITICAL :
                                TASKER_EXCEPTIONAL ),
                     raw    : -100 );
      } else {
         return FAIL;
      }
   }

   /* Work out the margin between total failure and total success. */
   if ( !extra )
      margin = 3 * sqrt( difficulty );
   else {
      if ( intp( extra ) )
         margin = extra;
      if ( pointerp( extra ) )
         margin = extra[ 0 ] + extra[ 1 ] * sqrt( difficulty );
   }
   if ( !margin )
      return BARF;

   /* If the bonus is above the margin, they succeed. */
   if ( bonus > difficulty + margin ) {
#ifdef DEBUG
     if(pointerp(control) &&
        WATCH_PLAYER || control[0]->query_name() == WATCH_PLAYER &&
        find_player(DEBUG))
       tell_object(find_player(DEBUG), "%^RED%^" + sprintf("TM: %s Skill: %s [%d] [%d] [%d] Succeeded\n", control[0]->query_name(), control[1], bonus, difficulty, margin) + "%^RESET%^");
#endif
      if( degree ) {
         return new( class tasker_result,
                     result : SUCCEED,
                     degree : ( is_critical(100) ?
                                TASKER_CRITICAL :
                                TASKER_EXCEPTIONAL ),
                     raw    : 100 );
      } else {
         return SUCCEED;
      }
   }

   /* In the margin, they might fail. */
   if(!margin)
     margin = 1;

   success_margin = ( ( 100 * ( bonus - difficulty ) ) / margin ) - random(100);
   if ( success_margin <= 0 ) {
#ifdef DEBUG
     if(pointerp(control) &&
        control[0]->query_name() == WATCH_PLAYER &&
        find_player(DEBUG))
       tell_object(find_player(DEBUG), "%^RED%^" + sprintf("TM: %s Skill: %s [%d] [%d] Failed (2)\n", control[0]->query_name(), control[1], bonus, difficulty) + "%^RESET%^");
#endif

      if( degree ) {
         if( is_critical(success_margin) ) {
            deg = TASKER_CRITICAL;
         } else {
            if( -success_margin < TASKER_MARGINAL_UPPER ) {
               deg = TASKER_MARGINAL;
            } else if( -success_margin < TASKER_NORMAL_UPPER ) {
               deg = TASKER_NORMAL;
            } else {
               deg = TASKER_EXCEPTIONAL;
            }
         }

         return new( class tasker_result,
                     result : FAIL,
                     degree : deg,
                     raw    : success_margin );
      } else {
         return FAIL;
      }
   }

   /* If information is available, adjust the chance to award based on stats.*/
   if ( pointerp( control ) ) {
#ifdef DEBUG
     if(find_player(DEBUG) && bonus >= MIN_LEVEL)
       tell_object(find_player(DEBUG), "%^RED%^" +
                   sprintf("TM: %s Skill: %s [%d,%d] OU: %d ",
                           control[0]->query_name(),
                           control[1], bonus,
                           control[0]->query_skill(control[1]),
                           upper));
#endif

     // Reduce the upper dependaont on their stats.
     upper = (int)control[ 0 ]->stat_modify(upper, control[ 1 ] );

#ifdef DEBUG
     if(find_player(DEBUG) && bonus >= MIN_LEVEL)
       tell_object(find_player(DEBUG),
                   sprintf("SU: %d ", upper));
#endif

     // Reduce the upper dependant on their level.
     tmp = exp((control[0]->query_skill(control[1])-BASE) / DECAY);
     upper = to_int(upper / tmp) - MODIFIER;

#ifdef DEBUG
     if(find_player(DEBUG) && bonus >= MIN_LEVEL)
       tell_object(find_player(DEBUG), sprintf("LU: %d ", upper));
#endif

     // prevent upper going negative
     if(upper < 0)
       upper = 0;

#ifdef DEBUG
      if(find_player(DEBUG) && bonus >= MIN_LEVEL)
       tell_object(find_player(DEBUG), sprintf("FU: %d\n",
                                                 upper) + "%^RESET%^");
#endif
   }
#ifdef DEBUG
   else
     if(find_player(DEBUG))
       tell_object(find_player(DEBUG), sprintf("No control: %O\n",
                                                 previous_object(-1)));
#endif
   /* If they succeed, they might be awarded a level. */
     if(!margin)
        margin = 1;

   if( random(100) < ( upper * ( difficulty + margin - bonus ) ) / margin )
      res = AWARD;
   else
      res = SUCCEED;

   if( degree ) {
      if( is_critical(success_margin) ) {
         deg = TASKER_CRITICAL;
      } else {
         if( success_margin < TASKER_MARGINAL_UPPER ) {
            deg = TASKER_MARGINAL;
         } else if( success_margin < TASKER_NORMAL_UPPER ) {
            deg = TASKER_NORMAL;
         } else {
            deg = TASKER_EXCEPTIONAL;
         }
      }

      return new( class tasker_result,
                  result : res,
                  degree : deg,
                  raw    : success_margin );
   } else {
      return res;
   }
} /* attempt_task() */

/*
 * This one acts like the other one except that the failure chance
 * starts at 100% at the difficulty and is halved every (half) bonus levels.
 *
 * @param difficulty the lowest bonus where the attempt can succeed
 * @param bonus the bonus the player has in the relevant skill
 * @param upper the maximum chance of getting an advance
 * @param half every time the bonus rise by half, the failure
 * change is halved
 *
 * @return BARF if something screwed up, AWARD if the task succeeded and
 * should give an advance, SUCCEED if it succeeded, FAIL if it failed.
 */
/**
 * Do not use this function. Use perform_task instead.
 * @see perform_task()
 */
varargs mixed attempt_task_e( int difficulty, int bonus, int upper,
                              int half, int degree )
{
   float fail_chance;
   float tmp;
   int success_margin, deg, res;

#ifdef LOGGING
   if(call_stack(2)[1] != "perform_task" &&
      base_name(previous_object()) != "/std/effects/fighting/combat" &&
      file_name(previous_object())[0..2] != "/w/") {
     log_file("ATTEMPT_TASK", "%s Object %s is using old attempt_task "
              "[%O]\n",
              ctime(time()), base_name(previous_object()), call_stack(2));
   }
#endif

#if LOG_STATS == 2
   if(pointerp(control) && control[1]) {
     skill_checked(control[1], control[0]->query_skill(control[1]));
   }
#endif

   /* If the bonus is below the difficulty, they fail. */
   if ( bonus < difficulty ) {
      if( degree ) {
         return new( class tasker_result,
                     result : FAIL,
                     degree : ( is_critical( -100 ) ?
                                TASKER_CRITICAL :
                                TASKER_EXCEPTIONAL ),
                     raw    : -100 );
      } else {
         return FAIL;
      }
   }

   if( !half ) /* Approximate old behaviour. */
      half = 6 * sqrt( difficulty );
   // We don't want to divide by 0...
   if( !half )
      half = 1;

   fail_chance = exp( ( -0.693 * ( bonus - difficulty ) ) / half );
   success_margin = ( random( 1000 ) - ( 1000 * fail_chance ) ) / 10;

   if ( success_margin < 0 ) {
      if( degree ) {
         if( is_critical(success_margin) ) {
            deg = TASKER_CRITICAL;
         } else {
            if( -success_margin < TASKER_MARGINAL_UPPER ) {
               deg = TASKER_MARGINAL;
            } else if( -success_margin < TASKER_NORMAL_UPPER ) {
               deg = TASKER_NORMAL;
            } else {
               deg = TASKER_EXCEPTIONAL;
            }
         }

         return new( class tasker_result,
                     result : FAIL,
                     degree : deg,
                     raw    : success_margin );
      } else {
         return FAIL;
      }
   }

   /* If information is available, adjust the chance to award based on stats.*/
   if ( pointerp( control ) ) {

#ifdef DEBUG
     if(find_player(DEBUG) && bonus >= MIN_LEVEL)
       tell_object(find_player(DEBUG), "%^RED%^" +
                   sprintf("TM: %s Skill: %s [%d,%d] OU: %d ",
                           control[0]->query_name(),
                           control[1], bonus,
                           control[0]->query_skill(control[1]),
                           upper));
#endif

     // Reduce the upper dependaont on their stats.
     upper = (int)control[ 0 ]->stat_modify( upper, control[ 1 ] );

#ifdef DEBUG
     if(find_player(DEBUG) && bonus >= MIN_LEVEL)
       tell_object(find_player(DEBUG),
                   sprintf("SU: %d ", upper));
#endif

     // Reduce the upper dependant on their level.
     tmp = exp((control[0]->query_skill(control[1])-BASE) / DECAY);
     upper = to_int(upper / tmp) - E_MODIFIER;

#ifdef DEBUG
     if(find_player(DEBUG) && bonus >= MIN_LEVEL)
       tell_object(find_player(DEBUG), sprintf("LU: %d ", upper));
#endif

     // prevent upper going negative and round down to zero from 10
     if(upper < 0)
       upper = 0;
#ifdef DEBUG
      if(find_player(DEBUG) && bonus >= MIN_LEVEL)
       tell_object(find_player(DEBUG), sprintf("FU: %d\n",
                                                 upper) + "%^RESET%^");
#endif

   }

   /* If they succeed, they might be awarded a level. */
   if ( random(1000) < ( upper * fail_chance * 10 )
    &&  bonus < difficulty + ( half * 5 ) ) {
      res = AWARD;
   } else {
      res = SUCCEED;
   }

   if( degree ) {
      if( is_critical(success_margin) ) {
         deg = TASKER_CRITICAL;
      } else {
         if( success_margin < TASKER_MARGINAL_UPPER ) {
            deg = TASKER_MARGINAL;
         } else if( success_margin < TASKER_NORMAL_UPPER ) {
            deg = TASKER_NORMAL;
         } else {
            deg = TASKER_EXCEPTIONAL;
         }
      }

      return new( class tasker_result,
                  result : res,
                  degree : deg,
                  raw    : success_margin );
   } else {
      return res;
   }
} /* attempt_task_e() */

/**
 * Figure out whether a given margin of success/failure will result
 * in a critical or not.  Takes an integer between -100 and 100,
 * representing the margin by which a skillcheck succeeded, and does
 * some logarithmic-type equations to figure out whether it should
 * result in a critical or not.
 * @param margin the margin of success.
 * @return 1 if the result is a critical, 0 otherwise.
*/
int is_critical( int margin ) {
   if ( margin < 0 ) {
      margin *= -1;
   }

   if ( margin > 100 ) {
      margin = 100;
   }

   if ( margin == 0 ) {
      return 0;
   }

   // critical chances are calculated above in create() so that the chance
   // at a margin of 1 is 1/10000 and the chance at a margin of 100 is 1/10.
   return random(10000) < critical_chances[ margin - 1 ];
} /* is_critical() */
