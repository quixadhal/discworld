/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: score.c,v 1.35 2003/06/05 01:22:45 pinkfish Exp $
 *
 *
 */

inherit "/cmds/base";

#include <config.h>
#include <library.h>
#include <quest_handler.h>
#ifndef __DISTRIBUTION_LIB__
#include <deity.h>
#endif
#include <top_ten_tables.h>
#include <player_handler.h>
#include <clubs.h>
#include <player.h>
#include <nomic_system.h>
#include <tune.h>
#include <skills.h>
#include <combat.h>

#define essify(x,y) (x > 1  ||  x == 0 ? " " y "s" : " " y)
#define LIMIT 150

int score_quests(object ob, int sorted) {
   string *quests;
   mixed *quest_info;
   
   if ( !this_player()->query_creator() && ob->query_property("dead")) {
      write( "You are just a disembodied spirit.  What use does a wispy thing "
             "like you have for a collection of meaningless numbers?\n" );
      return 1;
   }

   if(this_player()->query_role_playing()) {
     write("Sorry, you're roleplaying.\n");
     return 1;
   }
   quests = LIBRARY->query_quests( ob->query_name() );
   if ( !sizeof( quests ) ) {
      printf( "You have not completed any quests.\n" );
      return 1;
   }
  
   quest_info = map(quests, (: ({ QUEST_HANDLER->query_quest_title($1),
                                  QUEST_HANDLER->query_quest_story($1) }) :));
   if (sorted) { 
      quest_info = sort_array(quest_info, (: strcmp($1[0], $2[0]) :));
   }
   write( "$P$Quests$P$You have completed the following quests:\n"+
         implode( map_array( map_array( quest_info, (: "     \""+
         $1[0] +"\", in which you "+
         replace( $1[1],
         "was", "were" ) :) ), (: sprintf( $1[ 0 .. 24 ] +"%*-=s",
         $(ob)->query_cols() - 30, $1[ 25 .. ] ) :) ), ",\n" ) +".\n" );
   /* Wow.  That is impressive. --Presto */

   return 1;
} /* score_quests() */


int query_verbose(object ob, string verbose)  {
   if (!verbose) {
      return ob->query_verbose("score");
   }
   if (verbose == "verbose") {
      return 1;
   }
   return 0;
} /* query_verbose() */


int score_stats(object ob, string verbose)  {
   string word;
   int stat;
   string ret;
   
   if ( !this_player()->query_creator() && ob->query_property("dead")) {
     write( "You are just a disembodied spirit.  What use does a wispy thing "
            "like you have for a collection of meaningless numbers?\n" );
     return 1;
   }
   
   if(this_player()->query_role_playing()) {
     ret = "";
     if(stat = (this_player()->query_tmp_str() +
                this_player()->query_bonus_str())) {
       if ( stat > 0 ) {
         ret += "You feel stronger than usual.\n";
       } else {
         ret += "You feel weaker than usual.\n";
       }
     } 
     if ( stat = ( this_player()->query_tmp_int() +
                   this_player()->query_bonus_int() ) ) {
       if ( stat > 0 ) {
         ret += "You feel smarter than normal.\n";
       } else {
         ret += "You feel less smart than normal.\n";
       }
     }
     if ( stat = ( this_player()->query_tmp_con() +
                   this_player()->query_bonus_con() ) ) {
       if ( stat > 0 ) {
         ret += "You feel more healthy than usual.\n";
       } else {
         ret += "You feel a bit sickly.\n";
       }
     }
     if ( stat = ( this_player()->query_tmp_dex() +
                   this_player()->query_bonus_dex() ) ) {
       if ( stat > 0 ) {
         ret += "You feel more nimble than usual.\n";
       } else {
         ret += "You feel more clumsy than you normally do.\n";
       }
     }
     if ( stat = ( this_player()->query_tmp_wis() +
                   this_player()->query_bonus_wis() ) ) {
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
   } else {
     word = ( ob->query_weight() / 20 ) +"."+ ( ob->query_weight() % 10 );
     if (query_verbose(ob, verbose))  {
       printf( "%-#*s\n", ob->query_cols(),
               "Constitution ... "+ ob->query_con() +"\n"+
               "Dexterity ...... "+ ob->query_dex() +"\n"+
               "Intelligence ... "+ ob->query_int() +"\n"+
               "Strength ....... "+ ob->query_str() +"\n"+
               "Wisdom ......... "+ ob->query_wis() +"\n"+
               "Height ......... "+ ob->query_height() +" cm\n"+
               "Weight ......... "+ word +"kg\n" );
     } else  {
       printf( "%-#*s\n", ob->query_cols(),
               "Con: "+ ob->query_con() +"\nDex: "+ ob->query_dex() +"\n"+
               "Int: "+ ob->query_int() +"\nStr: "+ ob->query_str() +"\n"+
               "Wis: "+ ob->query_wis() +"\nHgt: "+ ob->query_height() +" cm\n"+
               "Wgt: "+ word +"kg\n" );
     }
   }
   return 1;
} /* score_stats() */

int calc_percent( float x, float y ) {
   return to_int( ( to_float( x ) / to_float( y ) ) * 100 );
} /* calc_percent() */

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

string find_xp_string( int current ) {
   string *skills = ({ "magic", "faith", "fighting", "covert", "crafts" });
   int xptotal;
   
   foreach( string skill in skills ) {
      xptotal += calc_xp_cost( skill );
   }
   
   xptotal /= sizeof( skills );

   //tell_creator( find_player( "terano" ), "%s: Current XP is: %d, Avg XP is %d, Ratio is: %d.\n", 
   //              this_player()->query_name(), current, xptotal, calc_percent( current, xptotal ) );  
   
   switch( calc_percent( current, xptotal ) ) {
      //Less than average
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
      return "certainly a hearty training session is in order";
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

int score_rp(object me, string verbose) {
  if(query_verbose(me, verbose)) {
    printf( "%s.\n%s and %s.\n%s.\n%s.\n%s.\n%s.\n%s.\n", 
            "You are "+ me->query_cap_name() +" "+me->query_gtitle(),
            capitalize( find_hp_string( me->query_hp(), me->query_max_hp() ) ),
            find_gp_string( me->query_gp(), me->query_max_gp() ),
            capitalize( find_xp_string( me->query_xp() ) ),
            capitalize( find_align_string( me ) ), 
            capitalize( find_wimpy_string( me->query_wimpy() ) ),
            //find_surrender_string( me->query_surrender() ),
            "You are "+ me->burden_string() + " and " +
               this_player()->query_temp_str(),
            find_death_string( me->query_max_deaths() - me->query_deaths() ) );
  } else {
    printf( "%s and %s.\n%s.\n", 
            capitalize( find_hp_string( me->query_hp(), me->query_max_hp() ) ),
            find_gp_string( me->query_gp(), me->query_max_gp() ),
            capitalize( find_xp_string( me->query_xp() ) ) );
  }
  return 1;
}

int score_burden(object me, string verbose) {
  write( sprintf("You are %s (%d%%).\n", me->burden_string(),
          me->query_burden()));
  return 1;
}

int score_normal(object ob, string verbose)  {
   string word;
   string guild_ob;
   string area;
   string* magistrates;
   string* citizens;
   int age, temp1, temp2, temp3, temp4;
   int no_title;

   if ( !this_player()->query_creator() && ob->query_property("dead")) {
      write( "You are just a disembodied spirit.  What use does a wispy thing "
             "like you have for a collection of meaningless numbers?\n" );
      return 1;
   }

   if(ob->query_role_playing())
     return score_rp(ob, verbose);
   
   if (query_verbose(ob, verbose))  {
      guild_ob = ob->query_guild_ob();
      if (!guild_ob) {
         guild_ob = "/std/guilds/warrior";
         no_title = 1;
      }
      temp1 = ob->query_hp();
      temp2 = ob->query_gp();
      temp3 = (int)LIBRARY->query_quest_points( ob->query_name() );
      temp4 = ob->query_sp();

      printf( "%-=*s", ob->query_cols(), "You have " +
            temp1 + " (" + ob->query_max_hp() + ") hit" +
            essify(temp1, "point") + ", " +
            temp2 + " (" + ob->query_max_gp() + ") guild" +
            essify(temp2, "point") + ", " +
            temp3 + " (" + (int)QUEST_HANDLER->query_total_qp() + ") quest" +
            essify(temp3, "point") + " and " +
            temp4 + " (" + ob->query_max_sp() + ") social" +
            essify( temp4, "point") + ".\n" );
      if ( guild_ob && find_object( guild_ob ) )  {
         printf( "%-=*s", ob->query_cols(), "Your current experience is " +
               ob->query_xp() + " and you are level " +
               (int)ob->query_level() + " in the " +
               (no_title?"Adventurers' Guild":guild_ob->query_short()) +
               ((ob->query_guild_ob() == "/std/guilds/thief" &&
               !(ob->query_guild_data() == 0)) ?
               ", specialised as a " + ob->query_guild_data() : "") +
               "; your overall rating is " + (int)TOP_TEN_HANDLER->
               calculate_rating( ob ) + ".\n" );
      }  else  {
         printf( "%-=*s", ob->query_cols(), "Your current experience is " +
               ob->query_xp() +".\n" );
      }
      temp1 = ob->query_deaths();
      temp2 = ob->query_max_deaths() - temp1;
      printf( "%-=*s", ob->query_cols(), "You have died " +
            temp1 + essify(temp1, "time") + " and can die " +
            temp2 + essify(temp2, "time") + " before you are completely "
            "dead.\n" );
      if ( ob->query_wimpy() )  {
         printf( "Your wimpy is set to %d%%.\n", ob->query_wimpy() );
      } else  {
         printf( "You are in brave mode.\n" );
      }
#ifdef USE_SURRENDER      
      if (ob->query_surrender())  {
         printf("You will surrender at %d%% of your maximum hit points.\n",
            ob->query_surrender());
      } else  {
         printf("You won't surrender.\n");
      }
#endif      
      printf( "You are %s (%d%%) and %s.\n", ob->burden_string(),
              ob->query_burden(), 
               this_player()->query_temp_str());
      word = ob->query_deity();
#ifndef __DISTRIBUTION_LIB__
      if ( stringp( word ) )  {
         printf( "You are %s, worshipping %s.\n", ob->align_string(),
               capitalize( word ) +", "+ (string)DEITY->query_title( word ) );
      } else  {
         printf( "You are %s, worshipping no god.\n", ob->align_string() );
      }
#endif

      //
      // Put any citizenship memberships in here.
      //
      citizens = ({ });
      magistrates = ({ });
      word = ob->query_name();
      foreach (area in NOMIC_HANDLER->query_citizenship_areas()) {
         if (NOMIC_HANDLER->is_magistrate_of(area, word)) {
            magistrates += ({ area });
         } else if (NOMIC_HANDLER->is_citizen_of(area, word)) {
            citizens += ({ area });
         }
      }
      if (sizeof(magistrates)) {
          printf("You are a magistrate of: " +
                 query_multiple_short(magistrates) + ".\n");
      }
      if (sizeof(citizens)) {
          printf("You are a citizen of: " +
                 query_multiple_short(citizens) + ".\n");
      }

      //
      // Player killer check.
      //
      if ( ob->query_property( PKER ) )  {
         printf( "You are registered as a player killer.\n" );
      }
      if ( m_sizeof( ob->query_hide_invis() ) )  {
         printf( "%-=*s\n", ob->query_cols(), "You are " +
               query_multiple_short( m_indices( ob->query_hide_invis() ) +
                                     ({ }) ) + "." );
      }
      printf( "%-=*s", ob->query_cols(), (string)ob->extra_score() );
      age = -ob->query_time_on();
      temp3 = ob->query_no_logins();
      word = sprintf("You are %s old and have logged in %d %s.\n",
                     query_time_string(age, -1),
                     temp3, (temp3 == 1 ? "time" : "times"));
      printf("%-=*s", ob->query_cols(), word);
   } else {
      printf( "%-=*s", ob->query_cols(), "Hp: " + ob->query_hp() + "(" +
            ob->query_max_hp() + ")  Gp: " + ob->query_gp() + "(" +
            ob->query_max_gp() + ")  Xp: "+ ob->query_xp() + "\n" );
   }
   return 1;
} /* score_normal() */

string create_relationship_text( string relationship, mapping data ) {
    string *relatives;
    int size;
    
    if ( !( relatives = data[ relationship ] ) ) 
        return "";
    
    if ( !size = sizeof( relatives ) )
        return "";
    
    if ( size == 1 ) {
        return capitalize( relatives[0] ) + " is your " +
            relationship;
    }
    
    return query_multiple_short( map( sort_array( relatives, 1 ),
        (: capitalize( $1 ) :) ) ) + " are your " + 
        pluralize( relationship );
} /* create_relationship_text() */


int get_gender( string name ) {
    int gender;
    
    gender = PLAYER_HANDLER->test_gender( name );
    
    if ( gender )
        gender -= 1;
    
    return gender;
} /* get_gender() */


int score_relationships( object ob, int verbose ) {
    string txt, family;
    string *sorted;
    class relationship_data *bing;
    mapping grouped;

    family = ob->query_family_name();

    if ( !family || !CLUB_HANDLER->is_family( family ) ) {
        add_failed_mess( "You're not in a family!\n" );
        return 0;
    }
   
    bing = CLUB_HANDLER->query_relationships( family, ob->query_name() );


    if ( !sizeof( bing ) ) {
        add_failed_mess( "You have no relatives.\n" );
        return 0;
    }

    grouped = unique_mapping( bing, 
        (: CLUB_HANDLER->query_relationship_gender( 
           CLUB_HANDLER->query_opposite_relationship( $1->relationship ),
           get_gender( $1->related_to ) )  :) );

    grouped = map( grouped, (: map( $2, (: $1->related_to :) ) :) );

    sorted = sort_array( keys( grouped ), 1 );

    txt = implode( map( sorted, 
        (: create_relationship_text( $1, $(grouped) ) :) ), ".\n" );

    tell_object( ob, "$P$Relationships$P$" + txt + ".\n" );

    return 1;
} /* score_relationships() */

int score_align( object ob, int verbose ) {
  int upper, lower, middle, align;
  string god, opinion;
  
  if (ob->query_property( "dead" )) {
     write( "You're dead.  Things such as 'good' and 'evil' are no longer of 
      your concern.\n" );
     return 1;
  }

   if( !ob->query_deity() ) {
    write( "You are " + ob->align_string() + ", and not worried about "
        "the approval of any deity at all.\n" );
    }
  else {
    god = ob->query_deity();
    align = ob->query_al();
#ifndef __DISTRIBUTION_LIB__
    lower = DEITY->query_al_lower( god );
    upper = DEITY->query_al_upper( god );
    middle = DEITY->query_al_middle( god );  
    opinion = "pleased with";
  
    if( align <= lower - LIMIT || align >= upper + LIMIT )
      opinion = "very angry with";
    if( ( align < lower && align > lower - LIMIT ) ||
        ( align > upper && align < upper + LIMIT ) )
      opinion = "quite annoyed with";
    if( ( align >= lower && align <= lower + LIMIT ) ||
        ( align <= upper && align >= upper - LIMIT ) )
      opinion = "a little concerned about";
    if( align > middle - ( LIMIT / 2 ) && align < middle + ( LIMIT / 2 ) )
      opinion = "very happy with";
      
    write( "You are " + ob->align_string() + ".  " + capitalize( god ) + 
      " is " + opinion + " you.\n" );
#else
    write( "You are " + ob->align_string() + ", and not worried about "
        "the approval of any deity at all.\n" );
#endif
  }
      
  return 1;
} /* score_align */

int score_fighting( object player, int verbose ) {
    object *attacker_list;
    object *attacker_list_here;
    string ret;
    class tactics tactics;
    
    attacker_list = player->query_attacker_list() - ({ 0 });
    
    ret = ""; 
    if ( attacker_list ) {
       attacker_list_here = filter( attacker_list, (: $1 && living( $1 ) && 
           environment( $1 ) == environment( $(player) ) :) );
   
       if ( !sizeof( attacker_list ) ) {
           ret += "You are not fighting anything!\n";
       } else {

          attacker_list -= attacker_list_here;
      
          if (sizeof(attacker_list_here)) { 
             ret += "$I$5=You are currently fighting " + 
                 query_multiple_short( attacker_list_here ) + " here.\n";
          }
          if (sizeof(attacker_list)) {
             ret += "$I$5=You are currently fighting " + 
                 query_multiple_short( attacker_list ) + " (these are "
                 "people you will auto attack on sight).\n" ;
          }
       }
    }
    tactics = (class tactics) this_player()->query_tactics();

    ret += "$I$0=Your current tactics are:\n"
      "   Attitude       :   "+ tactics->attitude +"\n"
      "   Response       :   "+ tactics->response +"\n"
#ifdef USE_SURRENDER
      "   Mercy          :   "+ (tactics->mercy?tactics->mercy:"none") + "\n"
#endif  
      "   Focus          :   "+
      (tactics->focus_zone?tactics->focus_zone:"none") + "\n"
#ifdef USE_DISTANCE
      "   Target distance:   "+
      (tactics->ideal_distance?tactics->ideal_distance:"none") + "\n"
#endif  
      "   Attack         :   "+ (tactics->attack == "both" ? "both" :
                     (tactics->attack?tactics->attack:"both")) + "\n"
      "   Parry          :   " + (tactics->parry == "both" ? "both" :
                      (tactics->parry?tactics->parry:"both")) + "\n"
      "   Unarmed Parry  :   " + (tactics->parry_unarmed ? "yes" : "no") + "\n";
    write(ret);
    return 1;
} /* score_fighting() */

int score_temperature( object player, int verbose ) {
   return "/cmds/player/temp_erature"->cmd();
} /* score_fighting() */

mixed *query_patterns() {
   return ({
             "quests", (: score_quests(this_player(), 0) :),
             "quests sorted", (: score_quests(this_player(), 1) :),
             "{brief|verbose} stats", (: score_stats(this_player(), $4[0]) :),
             "stats", (: score_stats(this_player(), 0) :),
             "{brief|verbose}", (: score_normal(this_player(), $4[0]) :),
             "relationships", (: score_relationships( this_player(), 0 ) :),
             "temperature", (: score_temperature( this_player(), 0 ) :),
             "burden", (: score_burden( this_player(), 0 ) :),
             "", (: score_normal(this_player(), 0) :),
             "{align|alignment}", (: score_align(this_player(), 0) :),
             "fighting", (: score_fighting( this_player(), 0 ) :) });
} /* query_patterns() */
