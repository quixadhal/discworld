/* this is the most simple room imaginable -- bil */

/* >> paths <<
 * Always use a path.h file when making a group of rooms. The idea is
 * that the path.h file contains various definitions of paths. eg
 * the path.h refered to here contains:
   #define ROOM "/obj/examples/"
 * this is considered further in exits - below.
 */
#include "path.h"

/* >> inheriting the basic room <<
 * For rooms inside inherit "/std/room", for those outside inherit
 * "/std/outside" - this choice simply controls whether the room gets
 * weather messages, and whether its light (see below) is affected by the
 * time of day.
 */
inherit "/std/room";

/* >> setup <<
 * The setup function is called when a room is created. It should
 * contain all things to initialise the room.
 */
void setup()
{
/* >> Short description <<
 * The first thing to set is the short description for the room.
 * this is seen when a player is in brief mode, types 'glance' or when
 * you take things from the room.
 * As with all shorts it should be in lower case, except where proper names
 * are involved, and there shouldn't be a '.' or '\n' in it. It should
 * really only be a few words but should be vaguely descriptive.
 */
  set_short("first simple room");
  
/* >> Long description <<
 * Next thing is the long, which is seen when moving in verbose mode, or
 * with the 'look' command. This should be from 2 lines long and should
 * describe the location. It shouldn't be more than half a page (12 lines)
 * long except in exceptional circumstances. Here we must use all the
 * punctuation we want. Also, unlike old lpmud rooms there should only be
 * one newline ("\n") and that at the end. The game auto-magically wraps
 * text for you!
 */ 
  set_long("This is a simple room, have fun with it. To read the code " +
           "simply type 'more here'. For help with more, type 'h' in it.\n");
  
/* >> Light level <<
 * All rooms have to be lit. To see what values are reasonable for this
 * look at the help on 'light' - use this function even if you wish the
 * location to be pitch darkness (ie set_light(0)) so it is obvious
 * you have set it dark, rather than just forgotten to set it.
 */
  set_light(90); /* brightly lit room */

/* >> Exits <<
 * Exits are important if you want to be able to move about!
 * ROOM (see #include above) refers to a directory. The reason we
 * use this definition is so we can build rooms in our /w/<myname>
 * directories, before attaching them to the game (in your domain).
 * eg. path.h was first:
   #define ROOM "/w/bil/examples/"
 * when i was writing this in my own directory, the rooms all being held
 * in /w/bil/examples directory. Then when i moved them here I simply
 * changed the path.h file that was with them to:
   #define ROOM "/obj/examples/"
 * You should use a similar method for coding/testing in /w/fish/forest
 * before putting it into /d/sur/forest (for example *8-)* )
 * the first argument is the direction the exit leads, the second the
 * filename of the room you should arrive in if you leave that way and the
 * third the type of exit, here "door" means doorway, not an actual moveable
 * door. This will be discussed further later on.
 */
  add_exit("east", ROOM + "exa2", "door");

/* >> Zone <<
 * All locations are considered to be in a zone, this is used to control
 * the movement of wandering monsters (this will be considered again later)
 */
  set_zone("examples");
}
