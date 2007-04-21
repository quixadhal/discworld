#include <effect.h>

#define SHADOW "/std/shadows/object/swim_fins"

string query_classification() { return "clothing.swim_fins"; }

string query_shadow_ob() { return SHADOW; }

void beginning(object player, int time ) { 
   player->submit_ee( "test_worn", 1, EE_ONCE );
   player->submit_ee( "test_worn", 20, EE_CONTINUOUS );
}

void restart( object player ) {
} /* restart() */

int merge_effect( object player, int old_state, int new_state ) {
   return old_state | new_state;
} /* merge_effect() */

int test_for_effect( object thing ) {
   if ( !thing )
      return 0;
   return member_array( file_name( this_object() ),
         (string *)thing->query_wear_effects() ) != -1;
} /* test_for_effect() */

void test_worn( object player ) {
   if ( player->still_auto_loading() )
      return;
   if ( !sizeof( filter_array( (object *)player->query_armours(),
         "test_for_effect", this_object() ) ) ) {
      player->submit_ee( 0, 0, EE_REMOVE );
      return;
   }
} /* test_worn() */

