#include "path.h"
inherit "/std/room/basic_room";

string *keywords;

void setup() {
   mixed search_result;
   string word;
   
   set_short( "primary search room" );
   add_property( "determinate", "the " );
   set_long( "Welcome to the first room for doing keyword searches.  "
            "Take an exit to narrow the search to that keyword.  If less than "
            "five rooms or objects fit the keywords there will also be exits "
            "directly to them.\n" );
   /* set_light sets the amount of light the room has.  See help light
    * for a list of what the number means:)
    */
   set_light( 50 );

   search_result = SEARCH->get_keywords( ({ }) );
   keywords = search_result[ 0 ];
   foreach( word in keywords ) { 
      add_exit( word, "??", "path" );
   }
   add_exit( "exit", ENTRANCE, "door" );

   /* add_property is used to add a value to an object, that can later be
    * queried for with query_property.
    * The "commented functions" and "keywords" properties are specific for 
    * rooms in the learning domain and is used to make it possible to search
    * for subjects. 
    * See /d/learning/handlers/search.c for the way this is done.
    */
   add_property( "commented functions", 
                ({ "add_exit", "set_destination" }) );
   add_property( "keywords", ({ "virtual", "room" }) );
   /* Do not try to add this room to the map_handler
    */
   add_property( "no map", 1 );
} /* setup() */

/* set_destination is called whenever an exit is checked.  
 * The search handler will see if there's already a room defined by the 
 * keywords, and if not, it'll make one.
 */
void set_destination( string keyword ) {
   if( member_array( keyword, ({ "exit", "back", "a", "b", "c", "d", "e" }) ) != -1 )
      /* the exit is not to a keyword, and don't need recalculation */
      return; 
   modify_exit( keyword, ({ "dest", SEARCH->find_room( ({ keyword }) ) }) );
} /* set_destination() */

