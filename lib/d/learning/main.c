#include "path.h"
inherit "/std/room/basic_room";

void setup() {
   /* The following two lines define the description of the room when you 
    * glance at it, or enter it in brief mode.
    * It should be something that will make the sentence:
    * "This room is <determinate>+<short>." correct.
    * eg. here it would make: "This room is the entrance to learning."
    * This is also a useful test to see if you've used capital letters
    * correctly.
    */
   set_short( "entrance to learning" );
   add_property( "determinate", "the " );
   /* The next command define the description you see when you look at it,
    * and when you enter it in verbose mode.
    * It should consist of complete sentences.  For normal rooms it should be
    * a description of the things the player can see.  
    * Remember to use two spaces beween sentences, to end the sentences with
    * "." and to put "\n" at the end.
    */
   set_long( "Welcome to the entrance room of the learning domain.  "
            "The purpose of this domain is to help beginning creators, by "
            "giving examples of the way to use the different functions that "
            "are available, and show ways to solve common problems.\n"
            "To see the code used in a room or an object you can use the "
            "commands \"more here\" and \"more <object name>\" "
            "respectively.  If you have something you'd like to see here, "
            "mail "+ CURRENT_LORD +" and suggest it.\n"
            "The search exit will take you to a special room with exits "
            "for each of the keywords defined in the different files in the "
            "domain, this will enable you to search for examples by "
            "selecting keywords, and is also an example of the use of "
            "virtual or cloned rooms.\n"
            "If you want to find examples of how to use specific functions, "
            "use the functions exit.\n"
            );
   /* set_light sets the amount of light the room has.  See help light
    * for a list of what the number means:)
    */
   set_light( 50 );
   /* set_co_ord Sets the current co-ordinates of the room.
    * they are ({ n-s, e-w, u-d })
    */
   set_co_ord( ({ 30, 0, 40 }) ); /* 30 feet south and 40 feet above the drum */
   /* add_exit is used to add exits (you'd never have guessed:)
    * Note the use of PROBLEMS, FUNCTIONS and ENTRANCE that are all defined
    * by path.h to make it easy to move the file without having to change it.
    * For a list of the different values you can use for the third parameter,
    * check "man add_exit"
    */
   add_exit( "search", PATH + "search", "door" );
   add_exit( "functions", PATH + "functions", "door" );
   add_exit( "common", "/w/common", "door" );
   /* this adds an exit back to this room from the common room
    */
   "/w/common"->add_exit( "learning", ENTRANCE, "door" );

   /* add_property is used to add a value to an object, that can later be
    * queried for with query_property.
    * The "commented functions" and "keywords" properties are specific for 
    * rooms in the learning domain and is used to make it possible to search
    * for subjects. 
    * See /d/learning/handlers/search.c for the way this is done.
    */
   add_property( "commented functions", 
                ({ "set_long", "set_short", "add_property", "set_light",
                   "add_exit" }) );
   add_property( "keywords", ({ "room" }) );
} /* setup() */

