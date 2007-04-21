/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: hints.c,v 1.11 2003/02/19 06:54:24 ceres Exp $
 *
 */
/*  Added search function to find wombles brooch.
    Talonia, Nov. '95
*/
/* Addded set_shorts to signs so they are still hidden, but you get a */
/* better message indicating what sign was read-Eron Mar. 21 1996     */
/* Added no teleport property- 26-06-98- Shalla */

#include "path.h"

inherit PATH+"inside";

object sign;

void setup() {
  set_zone( "Newbie" );
  set_short("Hints Room" );
  set_light( 75 );
  add_property("no teleport", 1);

  set_long( "You are in a small, dark room.  The only illumination seems to "
            "come from the crack under the door.  The walls are covered "
            "with handwritten notices, most of which you cannot read in this "
            "half-light, however a few catch your eye.  One is entitled "
            "'statistics', another 'navigation', another "
            "'advancement', a fourth 'assistance', yet another 'skills'"
            ", and a final one is marked 'dying'.  Something in the "
            "corner catches your eye.\n");

   add_item( ({ "something", "corner" }), 
             "There's something in the corner.  Maybe you could 'search' "
             "the corner and find out what's there." );
  add_item( "floor", 
            "You can barely make out the floor.  It appears to be made of "
            "stone");
  add_item( "wall", 
            "The walls (what you can see of them under the notices) are made "
            "of grey stone.");
  add_item(({"notice"}), "The walls are covered with handwritten notices.  In "
           "the half-light it's hard to pick them out, but you can make "
           "out a few of them.  One is entitled "
           "'statistics', another 'navigation', another "
           "'advancement', a fourth 'assistance', yet another 'skills'"
           ", and a final one is marked 'dying'.");
  
  sign=add_sign("A handwritten notice marked 'statistics'.\n",
                FOYER->format_sign(
           "Once you enter Discworld, you can rearrange your stats.  This "
           "should be done to improve your combination of constitution, "
           "dexterity, intelligence, strength and wisdom (your stats) once "
           "you have chosen a guild.  Use the command 'rearrange' to do "
           "this, but consider it well, as you can only rearrange your stats "
           "once.  You can view them using 'score stats'.  See 'help "
           "score'.\n"), 0, "statistics", "general");
  sign->set_short("sign marked 'statistics'");

  sign=add_sign("A handwritten notice marked 'combat'.\n",
                FOYER->format_sign(
           "Combat is a fairly important part of Discworld.  The combat "
           "system is quite advanced.  You should type 'help fighting' "
           "and 'help tactics' to find out more about how to modify "
           "the way in which you fight.\n"),
           0, "combat", "general");
  sign->set_short("sign marked 'combat'");

  sign=add_sign("A handwritten notice marked 'navigation'.\n",
                FOYER->format_sign(
           "Exploring a new area can be quite exciting.  There are lots of "
           "things to look at, and maps to make.  If you get lost, though, "
           "you might appreciate some maps.  In some areas, typing 'map' will "
           "show you a map of your immediate surroundings.  You may also find "
           "the maps some players have made useful - follow the 'Mud Atlas' "
           "link on our homepage.  See 'help maps'.\n"),
           0, "navigation", "general");
  sign->set_short("sign marked 'navigation'");

  sign=add_sign("A handwritten notice marked 'assistance'.\n",
      FOYER->format_sign(
      "Liaison creators are available for giving help and for "
      "answering questions; they cannot provide quest information, "
      "however.  You can find which liaisons are logged on at the moment "
      "using the 'liaisons' command.  "
      "To see if any creators are logged on, and to get a "
      "list of all players logged on, use the command 'finger':\n"
      " - creators are marked by having a C next to their name.\n"
      " - senior creators are marked by having an S next to their name\n"
      " - directors (domain leaders) are marked by having a D next to their "
      "name.\n"
      " - trustees (administrators) are marked by having a T next to their "
      "name.\n\n"
      "To report any bugs or typos that you may encounter, please "
      "make a report out for them.  To do this, please type "
      "'bug' and make out your report.  Please include the error, if "
      "any, that you get, the room that it is occuring in, and what "
      "you were doing to get the error.  Use the 'typo' command and "
      "the same guidelines when you report a typo.\n"), 0, "assistance", "general");
  sign->set_short("sign marked 'assistance'");

  sign=add_sign("A handwritten notice marked 'skills'.\n",
                FOYER->format_sign(
           "To check your skill levels, use the command 'skills', and "
           "to see various statistics about yourself, use 'score'.  Your "
           "stats can also be reviewed using 'score stats'.  The command "
           "'help' will give you a variety of other topics with useful "
           "information."), 0, "skills", "general");
  sign->set_short("sign marked 'skills'");

  sign=add_sign("A handwritten notice marked 'advancement'.\n",
                FOYER->format_sign(
           "Skills can be advanced in a variety of ways on Discworld.  "
           "The guild you join will allow you to advance your skills "
           "for a certain number of experience points and a small fee.  "
           "When you reach the proper skill level your guild will "
           "teach you the commands that you will need to carry out "
           "your guild duties.\n\n"
           "You may also have another player teach you skills if they have "
           "a higher skill level than you, or you may teach yourself.  Both "
           "of these methods cost experience points, and it is generally more "
           "expensive to teach yourself.\n\n"
           "Finally, many of your skills can be advanced simply by performing "
           "tasks that make use of those skills.  For example, your fighting "
           "skills will get better as you participate in more fights.  For "
           "more information on this system, type " 
           "'help taskmaster'.\n"),0, "advancement", "general");
  sign->set_short("sign marked 'advancement'");

  sign=add_sign("A handwritten notice marked 'dying'.\n",
                FOYER->format_sign(
           "At some point you will inevitably end up dying on Discworld, but "
           "fear not for you are given 7+1 lives.  When you die you need to "
           "go and visit Harry in the Temple of Small Gods who will raise "
           "you.\n"), 0, "dying", "general");
  sign->set_short("sign marked 'dying'");

/* Make it so you cannot godmother out of here */
  add_property("no godmother", 1);

  add_exit( "foyer", PATH + "foyer", "door" );
  add_alias( "northwest", "foyer");

  room_chat(({10, 40, ({"A voice whispers something that you can't quite "
                         "make out.",
                        "You think you hear someone whispering.",
                        "A voice whispers: Use 'chfn' to change your "+
                         "finger information.",
                        "A voice whispers: 'consider' will tell you how "
                         "tough an opponent is.",
                        "A voice whispers: You can re-execute your last "
                         "command by using '.' or the last command "+
                         "beginning xxx with '.xxx'.",
                        "A voice whispers: You can set up aliases for "
                         "sequences of commands that you use frequently.",
                        "A voice whispers: You can give your friends easier-"
                         "to-type nicknames.",
                        "A voice whispers: Use 'wimpy' to automatically "
                         "run away before you "
                         "die, or 'monitor' to keep a track of how hurt "
                         "you are getting.",
                        "A voice whispers: 'who' will provide a list of "
                         "the current players on Discworld.",
                        "If it's too noisy you can use 'earmuffs' to "
                         "quiet things down a bit.",
                        "A voice whispers: The street urchins can take you "
                         "to various places around Ankh-Morpork.  Just "
                         "say where you want to go and if they know it "
                         "they'll take you there."
                              })}));
} /* setup() */

int do_search()
{
  object ob;

  /* This is done so that brooches can be found at any time
     to an infinite number. Can't expect newbies to
     hang around waiting for resets so they can enter the game
     now can we? */

  tell_object( this_player(), "You examine the floor more "
              "closely and find a brooch lying in the corner.\n");
  tell_object(this_player(), "You pick up the brooch.\n");
  say(this_player()->one_short()+" searches the room "
      "and finds something in the corner.\n");
  ob = clone_object( PATH + "womble_brooch" );
  ob->move( this_player() );
  return 1;
}
