/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: guilds_foyer.c,v 1.8 2003/02/19 06:54:24 ceres Exp $
 *
 */
/* Added no teleport property- 26-06-98- Shalla */

#include "path.h"


inherit PATH+"outside";

void setup() {
  set_zone( "Newbie" );
  set_short( "Garden of Guilds" );
  set_light( 100 );
  add_property("no teleport", 1);

  set_day_long( "This is a pleasant garden with trees, shrubs and flowers.  "
                "There are six gazebos in the garden each with a wrought "
                "iron gate with a coat of arms on it.  Each "
                "gazebo is dedicated to one of the Discworld guilds, which "
                "are "+ GUILDS +".  "
                "To look at the gates themselves - use "
                "'look wizard' or 'look witch' or to enter one of the "
                "gazebos, use 'wizard', 'witch' or whichever is appropriate.\n"
                "There is also a sign attached to a post in the centre of the "
                "garden.\n" );
  
  set_night_long("This is an octagonal garden with a fence around it.  "
                 "Someone has placed braziers around the garden to provide "
                 "light.  There "
                 "are six gazebos in the garden each with a wrought iron gate "
                 "with a coat of arms on it.  Each "
                 "gazebo is dedicated to one of the Discworld guilds, which "
                 "are "+ GUILDS +".  "
                 "To look at the gates themselves - use "
                 "'look wizard' or 'look witch' or to enter one of the "
                 "gazebos, use 'wizard', 'witch' or whichever is "
                 "appropriate.\n"
                 "There is also a sign attached to a post in the centre of "
                 "the garden.\n" );

  add_item("gazebo", "There are a number of gazebos here each dedicated to "
           "a different guild.");
  
  add_sign("This is a simple white sign attached to a post.\n",
      FOYER->format_sign("Each player will usually join a guild so that "
      "they can learn skills and commands and so advance their abilities.  "
      "Being in a guild also gives character to your character.  To find "
      "out more about the different guilds take each of the guild gates "
      "from this garden.  When you are in Ankh-Morpork you will find that "
      "each guild has a headquarters where you may join.\n\nTake care with "
      "your choice of guild since you will not be able to change it.\n")
      , 0, "sign", "general" );

/* Make it so you cannot godmother out of here */
  add_property("no godmother", 1);

  add_exit( "foyer", PATH + "foyer", "gate" );
#ifndef __DISTRIBUTION_LIB__
  add_exit( "witch", PATH +"witches", "gate" );
  add_exit( "wizard", PATH +"wizards", "gate" );
  add_exit( "thief", PATH +"thieves", "gate" );
  add_exit( "assassin", PATH + "assassins", "gate" );
  add_exit( "warrior", PATH +"fighters", "gate" );
  add_exit( "priest", PATH +"priests", "gate" );
  add_alias( "north", "communication");

  modify_exit("witch",
              ({ "look", "It's too dark to see inside the gazebo."}));
  modify_exit("wizard",
              ({ "look", "It's too dark to see inside the gazebo."}));
  modify_exit("thief",
              ({ "look", "It's too dark to see inside the gazebo."}));
  modify_exit("assassin",
              ({ "look", "It's too dark to see inside the gazebo."}));
  modify_exit("warrior",
              ({ "look", "It's too dark to see inside the gazebo."}));
  modify_exit("priest",
              ({ "look", "It's too dark to see inside the gazebo."}));

  modify_exit("witch", ({ "closed", 1}));
  modify_exit("wizard", ({ "closed", 1}));
  modify_exit("thief", ({ "closed", 1}));
  modify_exit("assassin", ({ "closed", 1}));
  modify_exit("warrior", ({ "closed", 1}));
  modify_exit("priest", ({ "closed", 1}));
  
  modify_exit( "witch", ({
      "door long", "Over the gate to the Witch room "
      "is the burned-in image of a broomstick with a cat clinging on "
      "to the bristles for dear life.  The gate itself has a little "
      "window in it, but all that you can see through it is darkness.  "
      "To enter here type 'witch'.\n"}) );
  
  modify_exit( "wizard", ({ "door long", "The gate into the Wizard room "
         "has the crest of Unseen University on it: this shows a large "
         "pointy hat with stars, above it an open book with the words "
         "'Nunc Id Vides, Nunc Ne Vides'. "
         "To enter here type 'wizard'.\n"}) );

  modify_exit( "thief", ({ "door long", "On the Thieves' gate you see "
      "their crest. It has a slashed purse with change falling from a "
        "hole in the bottom and the words ACVTVS ID VERBERAT below. "
          "To enter this gate type \"thief\".\n"}) );
   
  modify_exit( "assassin", ({ "door long", "On the Assassins' gate you see "
         "their crest.  A cloak and a dagger in the top left and two gold "
           "crosses in the bottom right. Their motto reads: \"Nil Mortifi "
             "Sine Lucre.\"  "
               "To enter this gate type \"assassin\".\n"}) );

  modify_exit( "warrior", ({ "door long", "On the Warriors' gate "
         "they have a strange crest.  A skull and scythe with blood "
         "dripping from it.  Their motto reads: \"If It Moves Kill It. "
         "If It Doesn't Move, Wait For It To Move, Then Kill It.\"  "
         "To enter here type \"warrior\".\n"}) );

  modify_exit( "priest", ({ "door long", "On the Priest's gate you "
         "see an odd crest.  Many unusual symbols, everything from a jar "
         "of fluff to a puddle of water, even an obscure breed of monkey. "
         "To enter here type \"priest\".\n"}) );
#endif

  add_night_item("brazier", "Basically this is a torch on a pole.\n");
  
  add_room_chats(({"An elderly man dressed in a blue robe and wearing "
                     "a large pointy hat enters from the wizards "
                     "gazebo and leaves south.\n",
                     "An elderly man dressed in a blue robe and wearing "
                     "a large pointy hat enters from the south "
                     "and leaves for the wizards gazebo.\n",
                     "A small woman dressed in a black cloak and wearing "
                     "a large pointy hat enters from the witches "
                     "gazebo and leaves south.\n",
                     "A small woman dressed in a black cloak and wearing "
                     "a large pointy hat enters from the south "
                     "and leaves for the witches gazebo.\n",
                     "A burly man dressed head to toe in armour and "
                     "carrying a large sword enters from the warriors "
                     "gazebo and leaves south.\n",
                     "A burly man dressed head to toe in armour and "
                     "carrying a large sword enters from the north "
                     "and leaves for the warriors gazebo.\n",
                     "A saintly looking woman walks in from the north, "
                     "smiles benevolently at you, and leaves for the "
                     "priests gazebo.\n",
                     "A saintly looking woman enters from the priests "
                     "gazebo, smiles benevolently at you, and leaves "
                     "south.\n",
                     "A shadowy figure enters from the assassins "
                     "gazebo and leaves for the foyer.\n",
                     "A shadowy figure enters from the foyer "
                     "and leaves for the assassins gazebo.\n",
                     "A weaselly, furtive man sneaks in from the foyer "
                     "and leaves for the thieves gazebo.\n",
                     "A weaselly, furtive man sneaks in from the thieves "
                     "gazebo and leaves for the foyer.\n"}));

} /* setup() */

// Dont ever let it get too dark here.
int query_light() {
  int light;

  light = ::query_light();
  if(light < 40)
    light = 40;

  return light;
}
