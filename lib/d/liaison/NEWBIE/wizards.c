/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: wizards.c,v 1.4 2003/02/19 06:54:24 ceres Exp $
 */
/* Added no teleport property- 26-06-98- Shalla */

#include "path.h"

inherit PATH + "inside";

void setup() {

   set_short( "Wizards' Gazebo" );
   set_light( 75 );
   add_property("no teleport", 1);

   set_long( "As you enter this gazebo, it is obvious that it is the room "
             "of a mage.  Strewn around are pointy hats, wands and other "
             "items of wizardly paraphernalia.  A small sign catches your "
             "eye.  Maybe if you 'read sign' you'll find something out "
             "about this magical guild.\n"
             "A small notice that, for some reason, is extremely fat, wearing "
             "a pointy hat and winking at you, states that this is not the "
             "place where you actually join the guild.\n" );

   set_zone( "Newbies" );

   add_item( ({ "pointy hat", "wand", "item", "paraphernalia" }), 
             "There are many items around the room, but their function is "
             "beyond you for the most part." );
   add_item( "notice",
             "This notice is extremely fat, wears a pointy hat and winks at "
             "you -- that's magic for you." );
 
   add_sign( "The sign is floating, apparently unsupported, in the "
         "centre of the room.\n", "In this magical guild, based at "
         "Unseen University in Ankh-Morpork, you can learn many "
         "wonderful and exciting spells.  Learn how to tickle an "
         "enemy with a feather, create cabbage pets for your friends "
         "and hold your own against muggers using a carrot "
         "and a torch, or even a human eye!  The Library at the Unseen "
         "University (U.U.) is extensive, with many books in which to "
         "search for that special spell.  You'll learn all this and more "
         "if you decide to follow the Higher Art.", 0, 0, "general" );

/* Make it so you cannot godmother out of here */
   add_property("no godmother", 1);

   add_exit( "garden", PATH +"guilds_foyer", "gate" );
} /* setup() */
