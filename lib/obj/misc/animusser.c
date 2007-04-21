#include <skills.h>

inherit "/std/object";

string *skill_names;
mapping new_skills, old_skills;

void setup() {
   set_name( "lever" );
   set_short( "blue lever" );
   add_adjective( "blue" );
   set_long( "This blue lever is apparently an animus recalculation "+
         "device.  You're not totally sure how you know this, but "+
         "there you are.  You could pull it to have your animus "+
         "recalculated, I suppose.\n" );
   reset_get();
   skill_names = ({ });
   new_skills = ([ ]);
   old_skills = ([ ]);
} /* setup() */

void init() {
   this_player()->add_command( "pull", this_object() );
} /* init() */

int find_level( string skill ) {
   int i, sublevel, total;
   string *others;
   others = (string *)SKILL_OB->query_immediate_children( skill );
   if ( !sizeof( others ) )
      if ( undefinedp( old_skills[ skill ] ) )
         return (int)this_player()->query_skill( skill );
      else
         return old_skills[ skill ];
   for ( i = 0; i < sizeof( others ); i++ ) {
      sublevel = find_level( others[ i ] );
      total += sublevel;
      new_skills[ others[ i ] ] = sublevel;
      skill_names -= ({ others[ i ] });
   }
   return total / sizeof( others );
} /* find_level() */

int do_pull() {
   int i, adjust;
   new_skills = ([ ]);
   old_skills = (mapping)this_player()->query_skills();
   skill_names = m_indices( old_skills );
   while ( sizeof( skill_names ) ) {
      new_skills[ skill_names[ 0 ] ] = find_level( skill_names[ 0 ] );
      skill_names = delete( skill_names, 0, 1 );
   }
   reset_eval_cost();
   skill_names = m_indices( old_skills );
   for ( i = 0; i < sizeof( skill_names ); i++ ) {
      adjust = new_skills[ skill_names[ i ] ] - old_skills[ skill_names[ i ] ];
      if ( adjust )
         write( "Adjusting "+ skill_names[ i ] +" by "+ adjust +".\n" );
   }
   this_player()->set_skills( new_skills + ([ ]) );
   return 1;
} /* do_pull() */
