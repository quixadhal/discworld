#include "path.h";
inherit "/std/room";

object red_demon;

void setup() {
   /* The following two lines define the description of the room when you 
    * glance at it, or enter it in brief mode.
    * It should be something that will make the sentence:
    * "This room is <determinate><short>." correct.
    * eg. here it would make: "This room is the room telling about adding
    * NPC's."
    * This is also a useful test to see if you've used capital letters
    * correctly.
    */
   set_short( "room telling about adding NPC's" );
   add_property( "determinate", "the " );
   /* The next command define the description you see when you look at it,
    * and when you enter it in verbose mode.
    * It should consist of complete sentences.  For normal rooms it should be
    * a description of the things the player can see.  
    * Remember to use two spaces beween sentences, to end the sentences with
    * "." and to put "\n" at the end.
    */
   set_long( "This is a room showing how to add a stationary NPC to a room.  "
            "If you try to \"update here\" you will see that it waits a few "
            "seconds after loading the room before adding the NPC and, that "
            "when it appears there are a message notifying the player about "
            "it.\n" );
   /* set_light() defines the amount of light in the room. 
    * Without it you wouldn't be able to see:)
    * See help light for examples of what the values mean.
    */
   set_light( 60 );
   
   add_exit( "exit", ENTRANCE, "path" );
   
   /* add_property is used to add a value to an object, that can later be
    * queried for with query_property.
    * The "commented functions" and "keywords" properties are specific for 
    * rooms in the learning domain and is used to make it possible to search
    * for subjects. 
    * See /d/learning/handlers/search.c for the way this is done.
    */
   add_property( "commented functions", 
                ({ "set_long", "set_short", "add_property", 
                   "set_light", "reset" }) );
   add_property( "keywords", ({ "npc", "clone", "permanent", "make" }) );
} /* setup() */

/* reset() is called when the room is created, and every 15-30 minutes after
 * that.
 * Note the use of call_out to make the npc appear a bit after reset is 
 * called.  This has two purposes since it both helps to prevent "Too deep
 * recursion" errors, and it makes for a more dynamic room since people 
 * actually see when the npc appear.  The number is
 * how many second to wait before calling the function.
 */
void reset() {
   if ( !red_demon ) {
      call_out( "make_demon", 5 );
   }
} /* reset() */

/* This function is an example of how to make a complete npc, without
 * having to make a new file for it.  This works well, as long as you don't
 * want it to be able to do anything more than hang around and be 
 * slaughtered:)
 */
void make_demon() {
   red_demon = clone_object( "/obj/monster" );
   red_demon->set_name( "imp" );
   red_demon->set_short( "small red imp" );
   red_demon->add_adjective( ({ "small", "red" }) );
   red_demon->set_long( "This is a small red imp who has agreed to be used "
                       "as an example.\n" );
   red_demon->set_base_weight( 10 );
   red_demon->set_height( 17 );
   red_demon->set_race( "imp" );
   red_demon->set_level( 1 );
   /* I use a second parameter to move here, so people in the room will be 
    * told that the demon entered.
    * Note the use of $s to make sure the verb is conjugated correctly if 
    * the same message is used for more than one thing entering at the same
    * time.
    */
   red_demon->move( this_object(), "$N materialize$s with a small *POP*." );
} /* make_demon() */

