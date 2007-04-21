/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: rpskills.c,v 1.1 2003/07/15 08:01:01 pinkfish Exp $
 * $Log: rpskills.c,v $
 * Revision 1.1  2003/07/15 08:01:01  pinkfish
 * Initial revision
 *
 * Revision 1.5  2000/11/21 02:39:55  ceres
 * Added syntax
 *
 * Revision 1.4  1998/08/19 10:19:00  pinkfish
 * Add in stuff to put in the command name.
 *
 * Revision 1.3  1998/02/25 19:33:25  gototh
 * Little fix on previous query_stupid_skills().
 *
 * Revision 1.1  1998/01/06 05:27:04  ceres
 * Initial revision
 * 
*/
#include <skills.h>

inherit "/cmds/base"; 

string bonus_to_string( int bonus ) {
    if ( bonus < 0 ) {
        return "incompetent";
    }

    switch( bonus ) {
        case 0..50:
            return "novice";
        case 51..100:
            return "apprentice";
        case 101..200:
            return "competent";
        case 201..300:
            return "proficient";
        case 301..350:
            return "skilled";
        case 351..400:
            return "adept";
        case 401..500:
            return "expert";
        default:
            return "master";
    }
}


string rec_list( mixed *args, string path, int all, int lvl,
                 int only_leaf ) {
   int i, sk, o_l, non_zero;
   int no_bonus;
   string str, tp, tmp;

   str = "";
   for ( i = 0 ; i < sizeof( args ) ; i += SKILL_ARR_SIZE ) {
      o_l = 0;
      tp = path +"."+ args[ i ];
      /* Get the level of the skill at this point */
      if (tp[0] == '.') {
         tp = tp[1..];
      }
      sk = (int)this_player()->query_skill( tp );
      non_zero = SKILL_OB->query_only_show_if_non_zero( tp );
      no_bonus = SKILL_OB->query_no_bonus( tp );
      reset_eval_cost();
      /* Is this part of the tree only a leaf heirachy? */
      if (lvl == 1 && SKILL_OB->query_only_leaf(tp))
        o_l = 1;
      /*
       * If it is not a leaf heirachy, we show the levels and bonuses.
       * Or, if it is a leaf the show the bonuses is the level is
       *    above 0 and the we are not force showing all.
       */
      if (!(only_leaf || o_l) ||
              (!sizeof(args[i+SKILL_BIT]) && (sk > 0 || (all && !non_zero))))
        str += sprintf( "%*'| 's%*'.'-s " + (no_bonus?"\n":"%4s\n"),
              ( lvl-1 ) * 2, "",
              20 - ( ( lvl - 1 ) * 2 ), args[ i ], 
              bonus_to_string( (int)this_player()->query_skill_bonus( tp ) ) );
      /* It is a leaf heirachy!  No bonuses, no level if this is not a leaf. */
      if ( sizeof( args[ i + SKILL_BIT ] ) && ( only_leaf || o_l || all 
                                                || ( sk > 5 * lvl ) ) ) {
         /* Generate the lower bits... */
         tmp = rec_list( args[ i + SKILL_BIT ], path +"."+ args[i],
                         all, lvl + 1, only_leaf || o_l );
         /* Only put ourselves in if the list thingy generated something */
         /* Don't generate the list if it is the language heirachy... */
         if ((only_leaf || o_l) && (tmp != ""))
// || (!all && lvl == 1)))
           str += sprintf( "%*'| 's%*'.'-s          \n", ( lvl-1 ) * 2, "",
                           20 - ( ( lvl - 1 ) * 2 ), args[ i ]) + tmp;
         else
           str += tmp;
      }
   }
   return str;
} /* rec_list() */

int cmd( string word ) {
   int i;
   string result, *bits;
   mixed *args;
   result = "";

   if ( !"/obj/handlers/playtesters"->query_tester( this_player() ) )
       return 0;


   // This is used to give ratty skills etc. during reincarnation.
   if(this_player()->query_stupid_skills() != 0) {
      write(this_player()->query_stupid_skills());
      return 1;
   }
   
   bits = ({ });
   if ( word ) {
      args = (mixed *)SKILL_OB->query_skills();
      if ( ( i = member_array( word, args ) ) == -1 ) {
         notify_fail( "Usage: "+ query_verb() +" <skill>\n" );
         return 0;
      }
      args = args[ i + SKILL_BIT ];
      result = sprintf( "%'='*-s\n", (int)this_player()->query_cols(),
            "=======SKILLS=======Proficiency" );
      result += sprintf( "%#*-s\n", (int)this_player()->query_cols(),
            rec_list( args, word, 1, 1, 0 ) );
      this_player()->more_string( result, "Skills" );
      return 1;
   }
   args = (mixed *)SKILL_OB->query_skills();
   result += sprintf( "%'='*-s\n", (int)this_player()->query_cols(),
         "=======SKILLS=======Proficiency");
   result += sprintf( "%#*-s\n", (int)this_player()->query_cols(),
         rec_list( args, "", ( member_array( "all", bits ) != -1 ), 1, 0 ) );
   this_player()->more_string( result, "Skills" );
   return 1;
} /* cmd() */

mixed *query_patterns() {
  return ({ "", (: cmd(0) :),
              "<word'skill'>", (: cmd($4[0]) :) });
}
