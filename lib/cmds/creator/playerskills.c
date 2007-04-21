/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: playerskills.c,v 1.5 2001/06/07 06:15:15 shrike Exp $
 * $Log: playerskills.c,v $
 * Revision 1.5  2001/06/07 06:15:15  shrike
 * Changed it so it only prompts you for confirmation if there are players in the indirect_obs array.
 *
 * Revision 1.4  2000/07/27 05:35:08  taffyd
 * Added a confirmation prompt.
 *
 * Revision 1.3  2000/03/31 01:24:56  taffyd
 * Made to use wiz-present. :)
 *
 * Revision 1.2  1998/04/14 02:36:20  presto
 * Modified to always show creator's name, regardless of invisibility.
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 *
*/
#include <skills.h>

private string rec_list( mixed *args, string path, int all, int lvl,
                 int only_leaf, object pl ) {
   int i, sk, o_l;
   string str, tp, tmp;

   str = "";
   for ( i = 0 ; i < sizeof( args ) ; i += SKILL_ARR_SIZE ) {
      o_l = 0;
      tp = path +"."+ args[ i ];
      sk = (int)pl->query_skill( tp );
      reset_eval_cost();
      if (lvl == 1 && SKILL_OB->query_only_leaf(tp))
        o_l = 1;
      if (!(only_leaf || o_l) || (!sizeof(args[i+SKILL_BIT]) &&
                                  (sk > 0 || all)))
        str += sprintf( "%*'| 's%*'.'-s %4d %4d\n", ( lvl-1 ) * 2, "",
              20 - ( ( lvl - 1 ) * 2 ), args[ i ], sk,
              (int)pl->query_skill_bonus( tp ) );
      if ( sizeof( args[ i + SKILL_BIT ] ) && ( only_leaf || o_l || all
                                                || ( sk > 5 * lvl ) ) ) {
         tmp = rec_list( args[ i + SKILL_BIT ], path +"."+ args[i],
                         all, lvl + 1, only_leaf || o_l, pl );
         if ((only_leaf || o_l) && (tmp != "" || (!all && lvl == 1)))
           str += sprintf( "%*'| 's%*'.'-s    -    -\n", ( lvl-1 ) * 2, "",
                           20 - ( ( lvl - 1 ) * 2 ), args[ i ]) + tmp;
         else
           str += tmp;
      }
   }
   return str;
} /* rec_list() */

private int real_cmd( object *players, string word ) {
   int i;
   string result, *bits;
   object pl;
   mixed *args;

/* Only do the first player */
   pl = players[0];
   result = "";
   bits = ({ });
   if ( word ) {
      args = (mixed *)SKILL_OB->query_skills();
      if ( ( i = member_array( word, args ) ) == -1 ) {
         notify_fail( "Usage: "+ query_verb() +" <skill>\n" );
         return 0;
      }
      args = args[ i + SKILL_BIT ];
      result = sprintf( "%'='*-s\n", (int)this_player()->query_cols(),
            "=======SKILLS=======Level/Bonus" );
      result += sprintf( "%#*-s\n", (int)this_player()->query_cols(),
            rec_list( args, word, 1, 1, 0, pl ) );
      this_player()->more_string( result, "Skills" );
/*
      this_player()->add_succeeded(players[0]);
      this_player()->add_succeeded_mess(this_object(), ({ "",
                         this_player()->query_name() +
                         " delves skillfully into the internals of $I.\n" }),
                         players[0..0]);
*/

    tell_object( pl, this_player()->query_cap_name() + " is checking "
        "your skills.\n" );
      return 1;
   }
   args = (mixed *)SKILL_OB->query_skills();
   result += sprintf( "%'='*-s\n", (int)this_player()->query_cols(),
         "=======SKILLS=======Level/Bonus");
   result += sprintf( "%#*-s\n", (int)this_player()->query_cols(),
         rec_list( args, "", ( member_array( "all", bits ) != -1 ), 1, 0, pl ) );
   this_player()->more_string( result, "Skills" );
/*
   this_player()->add_succeeded(players[0]);
   this_player()->add_succeeded_mess(this_object(), ({ "",
                       this_player()->query_name() +
                       " delves skillfully into the internals of $I.\n" }),
                       players[0..0]);
*/
   tell_object( pl, this_player()->query_cap_name() + " is checking your "
       "skills.\n" );
   return 1;
} /* real_cmd() */


private void check_response( string txt, object *indirect_obs, string skill ) {

    int result;

    if ( upper_case( txt ) == "Y" ) {
        result = real_cmd( indirect_obs, skill );
        return;
    }

    tell_object( this_player(), "Not viewing skills.\n" );
} /* check_response() */

private int cmd( object *indirect_obs, string skill ) {
   if ( !sizeof( filter( indirect_obs, (: userp :) ) ) ) {
      check_response( "Y", indirect_obs, skill );
      return 1;
   }

   tell_object( this_player(), "Are you sure you wish to view " +
      query_multiple_short( indirect_obs ) + "'s skills?  You should ask "
      "$V$0=" + indirect_obs[0]->query_possessive() + ",their$V$ "
   "permission first.\n" );

   tell_object( this_player(), "Enter 'Y' to view " +
      query_multiple_short( indirect_obs ) + "'s skill tree.\n" );

   input_to( (: check_response :), 0, indirect_obs, skill );
   return 1;
} /* cmd() */

mixed *query_patterns() {
  return ({ "<indirect:wiz-present> <string>",
            (: cmd($1, $4[1]) :),
            "<indirect:wiz-present>",
            (: cmd($1, 0) :) });
} /* query_pattern() */
