#include "path.h"


inherit PATH + "inside";

object sign;

void setup() {
  set_zone( "Newbie" );
  set_short("Advancement Room" );
  set_light( 75 );
  add_property("no teleport", 1);

  set_long( "This is a rather cosy room designed to encourage visitors to "
            "linger a while and read a few of the notices pinned to the "
            "walls.  The air is thick and overly warm, and there is a "
            "pervasive odour of stale tea, mothballs, and elderly ladies who "
            "collect rather more cats and string than they can properly "
            "handle.  "
            "Several overstuffed sofas have been grouped in the centre "
            "of the room along with wobbly occasional tables.  There is "
            "a small cage holding an odd-looking creature in one corner, "
            "and an immense ball of yarn in the other.  A door to the south "
            "leads back to the foyer.\n");

  add_item(({ "floor", "ground" }), "The floor is scuffed, dusty wood.  It "
           "was probably nice at some point.");
  add_item( "wall", "The walls (what can be seen of them under the notices) "
            "have been covered with a gaudy floral paper.");
  add_item(({ "paper", "wallpaper", "floral", "flower" }), "The wallpaper has "
            "a dark, muddy background covered with enormous fantasy flowers "
            "in red, blue, and yellow.");
  add_item( "door", "The plain wooden door has been propped open in an effort "
            "to let some of the stale air out.");
  add_item( "cage", "The cage is a small, rickety bamboo affair with a tiny "
            "door tied closed with a strand of wire.  It contains a strange "
            "creature.");
  add_item( "creature", "The creature is a pale, translucent colour.  It has "
            "long, spindly legs, big teeth, and eyes that glow like lamps "
            "when it is awake.  Even though it is small, you sense it would "
            "not be wise to disturb it.  It is holding a small card.");
  add_item( "wire", "The wire holding the cage door shut is a hopelessly "
            "twisted mass of spun octiron, the magical metal of Discworld.");
  add_item( "card", "The card is stamped 'Demon Local 45, Holywood' in "
            "fading ink.");
  add_item(({"ball", "yarn"}), "A large ball of string.");
  add_item("notice",
           ({ "long", "There are many of these pinned to the walls of the "
                "room.  Several catch your eye including ones marked "
                "'statistics', 'guildpoints', 'hitpoints', "
                "'skills' and 'advancement'.\n",
                "read", "There are many of these pinned to the walls of the "
                "room.  Several catch your eye including ones marked "
                "'statistics', 'guildpoints', 'hitpoints', "
                "'skills' and 'advancement'.\n"}));
  add_item("sofa", ({"long", "Several well-stuffed and well-worn sofas with "
                       "rather dirty, brown upholstery.",
                       "position", "a sofa",
                       "position multiple", 1 }));
  add_item("table", "A number of wobbly occasional tables made from "
           "some indeterminable kind of wood have been placed near the "
           "sofas.");
  sign=add_sign("A handwritten notice marked 'Statistics and Rearrange'.\n",
                "On Discworld your skill bonus typically determines your "
                "ability to perform a task.  Skill bonus is calculated "
                "from your skill level and your statistics.  Different "
                "stats affect different skills.\n"
                "Once you enter Discworld, you may rearrange your stats.  "
                "This should be done to improve your combination of "
                "constitution, dexterity, intelligence, strength and wisdom "
                "(your stats), but you are advised to wait until you have "
                "chosen a guild and have asked other guild members for "
                "advice.  Use the command 'rearrange' to do this, but "
                "consider it well, as you can only rearrange your stats "
                "once.\n", 0, "statistics", "general" );
  sign->add_property("determinate","the ");
  sign->set_short("sign marked 'statistics and rearrange'");

  sign=add_sign("A handwritten notice marked 'Guildpoints'.\n",
           "Guildpoints are a measure of your capability to perform guild "
           "commands such as stealing, fighting, casting spells or rituals, "
           "etc.  When you have used all your guild points, you will have to "
           "rest to allow them to regenerate.  For more information, type "
           "in 'help guildpoints'.\n", 0, "guildpoints", "general");
  
  sign->add_property("determinate","the ");
  sign->set_short("sign marked 'guild points'");
  sign=add_sign("A handwritten notice marked 'Experience'.\n",
           "In addition to the direct skill advances provided by the "
           "taskmaster, quest completion, using your guild commands "
           "and by doing certain tasks, you are rewarded with "
           "experience points or XP.  These may be spent in advancing skills "
                "from your guild or by learning from other players.\n", 0,
                "experience", "general");
  sign->add_property("determinate","the ");
  sign->set_short("sign marked 'experience'");

  sign=add_sign("A handwritten notice marked 'Hitpoints'.\n",
           "Hitpoints, or HP determine how much damage you can take in "
           "combat before you die.  The number of hit points you have is "
           "dependent upon your stats (mostly Con) and on your levels of "
           "other.health.  Hitpoints regenerate gradually over time.  "
           "There are also a variety of other ways to be healed.  You "
           "can use 'monitor' to keep track of your hitpoints.  See also "
           "'help wimpy'.\n", 0, "hitpoints", "general");
  sign->add_property("determinate","the ");
  sign->set_short("sign marked 'hitpoints'");

  sign=add_sign("A handwritten notice marked 'Skills'.\n",
           "To check your skill levels, use the command 'skills', and "
           "to see various statistics about yourself, use 'score'.  Your "
           "stats can also be reviewed using 'score stats'.  The command "
           "'help' will give you a variety of other topics with useful "
           "information.", 0, "skills", "general");
  sign->add_property("determinate","the ");
  sign->set_short("sign marked 'skills'");

  sign=add_sign("A handwritten notice marked 'advancement'.\n\n",
           "Skills can be advanced in a variety of ways on Discworld.  "
           "The guild you join will allow you to advance your skills "
           "for a certain number of experience points and a small fee.  "
           "When you reach the proper skill level your guild will "
           "teach you the commands that you will need to carry out "
           "your guild duties.\n"
           "You may also have another player teach you skills if they have "
           "a higher skill level than you, or you may teach yourself.  Both "
           "of these methods cost experience points, and it is generally more "
           "expensive to teach yourself.\n"
           "Finally, many of your skills can be advanced simply by performing "
           "tasks that make use of those skills.  For example, your fighting "
           "skills will get better as you participate in more fights.  For "
           "more information on this system, type " 
           "'help taskmaster'.\n",0, "advancement", "general");
  sign->add_property("determinate","the ");
  sign->set_short("sign marked 'advancement'");

  /* Make it so you cannot godmother out of here */
  add_property("no godmother", 1);

  add_exit( "foyer", PATH + "foyer", "door" );
  add_alias( "south", "foyer");
  /*
  room_chat(({10, 40, ({"A voice whispers something that you can't quite "
                         "make out."
                              })}));
  */
} /* setup() */
