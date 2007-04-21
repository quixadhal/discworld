/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: fighters.c,v 1.5 2003/02/19 06:54:24 ceres Exp $
 *
*/

#include "path.h"
inherit PATH + "inside";

void setup() {
   set_short( "Warriors' gazebo" );
   set_light( 75 );
   add_property("no teleport", 1);
   set_long( "As you enter this gazebo, you are overwhelmed with a smell of "
             "decaying limbs and dried blood.  A sign catches your eye and you "
             "notice it's the only non-blood covered item in the room.  Maybe "
             "if you 'read sign' you'll get some information about this "
             "deadly guild.\n"
             "A small notice pinned to the wall with a sword states that this "
             "is not the place where you actually join the guild.  The rest "
             "of the note is too torn to read.\n" );

   set_zone( "Newbie" );

   add_item( ({ "armour" }),
             "There are many items around the room, but their function is "
             "beyond you for the most part." );
   add_item( ({ "notice" }),
             "This notice was probably pinned to the wall during combat "
             "practice, and is therefore more or less unreadable after the "
             "battering it got during the session.\n" );

   add_sign( "This sign seems to cower in the corner of the room "
             "unsure of it's place here.\n", "In this deadly guild, "
             "the fine art of killing and berserking is taught.  Learn how "
             "to be deadly with accuracy and lots of pain!  To be a warrior "
             "you must endure lots of pain, practice as much as you can, have "
             "a strong stomach, and have the ability to kill without batting "
             "an eye.  Remember to always keep your guild motto with you: 'If "
             "It Moves Kill It.  If It Doesn't Move, Wait For It To Move, "
             "Then Kill It.'", 0, 0, "general" ); 

/* Make it so you cannot godmother out of here */
   add_property("no godmother", 1);

   add_exit( "garden", PATH +"guilds_foyer", "gate" );
} /* setup() */
