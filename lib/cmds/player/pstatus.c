/**
 * Score Command for Role-Players
 * @author Terano
 *
 */

broken
#include <config.h>
#include <library.h>
#include <quest_handler.h>
#ifndef __DISTRIBUTION_LIB__
#include <deity.h>
#endif
#include <top_ten_tables.h>
#include <tune.h>
#include <skills.h>

inherit "/cmds/base";

string find_hp_string( int current, int max );
string find_gp_string( int current, int max );
string find_xp_string( int current );
string find_wimpy_string( int wimpy );
string find_surrender_string( int surr );
string find_align_string( object tp );
string find_death_string( int remaining );
int calc_percent( float x, float y );
int calc_xp_cost( string skill );

int cmd() {
   object me;

   me = this_player();

   if ( !"/obj/handlers/playtesters"->query_playtester( me->query_name() ) &&
        !me->query_creator() ) {
      return 1;
   }

   if (this_player()->query_property("dead")) {
      write( "You are just a disembodied spirit.  What use does a wispy thing "
             "like you have for a collection of meaningless numbers?\n" );
      return 1;
   }

   switch( me->query_verbose( "score" ) ) {
   case 1:
      printf( "%s.\n%s and %s.\n%s.\n%s.\n%s and %s.\n%s.\n%s.\n", 
              "You are "+ me->query_cap_name() +" "+me->query_gtitle(),
              capitalize( find_hp_string( me->query_hp(), me->query_max_hp() ) ),
              find_gp_string( me->query_gp(), me->query_max_gp() ),
              capitalize( find_xp_string( me->query_xp() ) ),
              capitalize( find_align_string( me ) ), 
              capitalize( find_wimpy_string( me->query_wimpy() ) ),
              find_surrender_string( me->query_surrender() ),
              "You are "+ me->burden_string(),
              find_death_string( me->query_max_deaths() - me->query_deaths() ) );
      return 1;
   default: 
      printf( "%s and %s.\n%s.\n", 
              capitalize( find_hp_string( me->query_hp(), me->query_max_hp() ) ),
              find_gp_string( me->query_gp(), me->query_max_gp() ),
              capitalize( find_xp_string( me->query_xp() ) ) );
      return 1;
   }
} /* cmd() */

int cmd_stats() {
   object me;
   int stat;
   string ret;

   me = this_player();
   ret = "";

   if ( !"/obj/handlers/playtesters"->query_playtester( me->query_name() ) &&
        !me->query_creator() ) {
      return 1;
   }

   if ( stat = ( me->query_tmp_str() + me->query_bonus_str() ) ) {
      if ( stat > 0 ) {
         ret += "You feel stronger then usual.\n";
      } else {
         ret += "You feel weaker then usual.\n";
      }
   } 
   if ( stat = ( me->query_tmp_int() + me->query_bonus_int() ) ) {
      if ( stat > 0 ) {
         ret += "You feel smarter then normal.\n";
      } else {
         ret += "You feel less smart then normal.\n";
      }
   }
   if ( stat = ( me->query_tmp_con() + me->query_bonus_con() ) ) {
      if ( stat > 0 ) {
         ret += "You feel more healthy then usual.\n";
      } else {
         ret += "You feel a bit sickly.\n";
      }
   }
   if ( stat = ( me->query_tmp_dex() + me->query_bonus_dex() ) ) {
      if ( stat > 0 ) {
         ret += "You feel more nimble then usual.\n";
      } else {
         ret += "You feel more clumsy than you normally do.\n";
      }
   }
   if ( stat = ( me->query_tmp_wis() + me->query_bonus_wis() ) ) {
      if ( stat > 0 ) {
         ret += "You feel mentally attuned.\n";
      } else {
         ret += "You feel a bit vague.\n";
      }
   }
   if ( !strlen( ret ) ) {
      ret += "You feel fine.\n";
   }
   printf( "%s", ret );
   return 1;
} /* cmd_stats() */

string find_hp_string( int current, int max ) {
   if ( this_player()->query_property( "dead" ) ) {
      return "you are dead\n";
   }

   switch( calc_percent( current, max ) ) {
   case 90..100: 
      return "you are in perfect health";
   case 70..89: 
      return "you are slightly wounded";
   case 50..69: 
      return "you are moderately wounded";
   case 30..49: 
      return "you are seriously wounded";
   case 15..29: 
      return "you are critically wounded";
   case 6..14: 
      return "you are fatally wounded";
   case 0..5: 
      return "you are near death";
   default: 
      return "you are broken";
   }
} /* find_hp_string() */

string find_gp_string( int current, int max ) {
   if ( this_player()->query_property( "dead" ) ) {
      return "you are dead\n";
   }
   
   switch( calc_percent( current, max ) ) {
   case 90..100: 
      return "you are full of energy";
   case 70..89: 
      return "you are enthusiastic";
   case 50..69: 
      return "you are not quite so full of beans";
   case 30..49: 
      return "you are weary";
   case 15..29: 
      return "you are tired";
   case 6..14: 
      return "you are exhausted";
   case 0..5: 
      return "you are nearly unconscious";
   default: 
      return "you are broken";
   }
} /* find_gp_string() */

string find_xp_string( int current ) {
   string *skills = ({ "magic", "faith", "fighting", "covert", "crafts" });
   int xptotal;
   
   foreach( string skill in skills ) {
      xptotal += calc_xp_cost( skill );
   }
   
   xptotal /= sizeof( skills );
   
   tell_creator( find_player( "terano" ), "%s: Current XP is: %d, Avg XP is %d, Ratio is: %d.\n", 
                 this_player()->query_name(), current, xptotal, calc_percent( current, xptotal ) );  
   
   switch( calc_percent( current, xptotal ) ) {
      //Less then average
   case 0..20: 
      return "you don't think you can learn much at the moment";
   case 21..59: 
      return "by the luck of a coin, you might be able to learn something new";
   case 60..100: 
      return "if you squeezed it, you could gain some insight";
      
      //1 level
   case 101..300: 
      return "you might be able to learn something new, if you found the right teacher";
      
      //3 levels of average
   case 301..400: 
      return "a small amount could be learned with training";
   case 401..800: 
      return "a bit of training might be in order";
   case 801..1200: 
      return "you could very well learn something new from training";
   case 1201..1500: 
      return "you could benefit from training";
   case 1501..2000: 
      return "you feel like training might help";
   case 2001..2500: 
      return "you should consider training your skills";
      
      //25 levels of average
   case 2501..4000: 
      return "your skills could be greatly improved with training";
   default: 
      return "certainly a hearty training session is in order!";
   }
}
 
string find_wimpy_string( int wimpy ) {
   if ( wimpy ) {
      return "you will flee when "+ find_hp_string( wimpy, 100 );
   }
   return "you will not flee";
} /* find_wimpy_string() */

string find_surrender_string( int surr ) {
   if ( surr ) {
      return "you will surrender when "+ find_hp_string( surr, 100 );
   }
   return "you will not surrender";
} /* find_surrender_string() */

string find_align_string( object tp ) {
   string ret = "";
   string word = tp->query_deity();
   
   ret = "";
   word = tp->query_deity();
   if ( !tp ) {
      return "";
   }
   ret += "you are ";
   ret += tp->align_string();
   if ( stringp( word ) ) {
      ret += " and in the service of ";
      ret += capitalize( word );
      ret += ", ";
#ifndef __DISTRIBUTION_LIB__
      ret += DEITY->query_title( word );
#endif
   }
   return ret;
} /* find_align_string() */

string find_death_string( int remaining ) {
   if ( !this_player()->query_deaths() ) {
      return "Death has never visited you in a professional capacity";
   }

   switch ( remaining ) {
   case 0..0: 
      return "Death has a special interest in your next appointment";
   case 1..3: 
      return "Death is starting to lose patience with you";
   case 4..7: 
      return "You and Death are on good terms";
   default: 
      return "You have an \"arrangement\" with Death";
   }
} /* find_death_string() */

int calc_xp_cost( string skill ) {
   int lvl;
   int my_lvl;
   int cost;
   int j;
   int total;
   int total2;
   int num = 1;

   my_lvl = this_player()->query_skill_bonus( skill, 1 );
   lvl = this_player()->query_skill(skill);
   
   cost = DEFAULT_COST;
   if( SKILL_OB->query_skill_cost(skill) > 0 ) {
      cost *= SKILL_OB->query_skill_cost(skill);
   }
   cost *= STD_COST/5;
   total2 = 0;
   total = 0;
 
   if ( !my_lvl ) {
      my_lvl = 1;
   }
   for ( j = 0; j < num; j++) {
      int k;
      
      k = 2 * this_player()->calc_bonus( lvl + j, skill, 1 ) / my_lvl + 1;
      total2 += cost * ( ( lvl + j ) / LEVEL_DIV + 1 ) * k * k / 2;
      /* This stops people taking advantage of two's complement. */
      if ( total > total2 ) {
         num = j;
         break;
      } else {
         total = total2;
      }
   }
   
   if (!total) {
      total = cost;
   }
   
   return total;  
} /* calc_xp_cost() */

int calc_percent( float x, float y ) {
   return to_int( ( to_float( x ) / to_float( y ) ) * 100 );
} /* calc_percent() */

mixed *query_patterns() { 
   return ({ "", (: cmd() :), "stats", (: cmd_stats() :) }); 
} /* query_patterns() */
