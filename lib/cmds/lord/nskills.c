/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: skills.c,v 1.4 1998/08/19 10:19:00 pinkfish Exp $
 * $Log: skills.c,v $
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

inherit "/cmds/guild_base";

int new_skill_value(int sk) {
	float f = 1000.0;
	float k = 0.3;

	if(sk == 0)
		return 0;
	
	return to_int(f * log( 1.0 + sk/(f + sk * k) ) + 0.5);
}

string rec_list( mixed *args, string path, int all, int lvl,
                 int only_leaf, object pl ) {
   int i, sk, o_l;
   string str, tp, tmp;

   str = "";
   for ( i = 0 ; i < sizeof( args ) ; i += SKILL_ARR_SIZE ) {
      o_l = 0;
      tp = path +"."+ args[ i ];
      sk = (int)pl->query_skill( tp );
      if(tp[0..14] != ".other.language" && tp[0..13] != "other.language") {
        sk = new_skill_value(sk);
      }
			
      reset_eval_cost();
      if (lvl == 1 && SKILL_OB->query_only_leaf(tp))
        o_l = 1;
      if (!(only_leaf || o_l) || (!sizeof(args[i+SKILL_BIT]) &&
                                  (sk > 0 || all)))
        str += sprintf( "%*'| 's%*'.'-s %4d %4d\n", ( lvl-1 ) * 2, "",
              20 - ( ( lvl - 1 ) * 2 ), args[ i ], sk,
              (int)new_skill_value(pl->query_skill_bonus( tp )) );
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

int cmd( object *players, string word ) {
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
      return 1;
   }
   args = (mixed *)SKILL_OB->query_skills();
   result += sprintf( "%'='*-s\n", (int)this_player()->query_cols(),
         "=======SKILLS=======Level/Bonus");
   result += sprintf( "%#*-s\n", (int)this_player()->query_cols(),
         rec_list( args, "", ( member_array( "all", bits ) != -1 ), 1, 0, pl ) );
     result += "New guild level: " +
		   (pl->query_guild_ob())->query_new_level(pl) + "\n";

   this_player()->more_string( result, "Skills" );
   return 1;
} /* cmd() */

void dest_me() {
  destruct(this_object());
} /* dest_me() */

void clean_up() {
  dest_me();
} /* clean_up() */

void reset() {
  dest_me();
} /* reset() */

mixed *query_patterns() {
  return ({ "<indirect:any-living> <string>",
            (: cmd($1, $4[1]) :),
            "<indirect:any-living>",
            (: cmd($1, 0) :) });
} /* query_pattern() */
