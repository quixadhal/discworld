/****** This room is made my Mithal, learn and enjoy  ***/

#include "path.h"

inherit "/std/room";

void setup() {
   set_short("add_item room #3, verbs ");
   set_light(100);

   set_long("add_item room #3, verbs \n"
            "As you know, things can be read, kicked, etc... in DiscWorld.  "
            "Therefore we need to make our items respond to actions.  The "
            "old way to do this is through add_action.  Unless you are "
            "experienced, you should not use this function.  add_item "
            "provides many facilities to do this which are much better to "
            "write, easier to understand, and have less chance of 'bugging' "
            "out on you.\nA large oak.\nA note.\n\n");
   
   
   add_item("note", ({ "long", "It can be read.",
      "read", "As you can see, this note can be read, "
      "this is our first introduction to verbs.  "
      "The code that does this:\n"
      "      add_item(\"note\", ({ \"long\", \"It can be read.\" \n"
      "                          \"read\", \"As you can see ... \" }) );\n"
      "This uses a mixed variable in the description part of the "
      "add_item.  The members of the mixed variables are strings which "
      "come in pairs.  Notice that \"long\" is not \"look\" this is just "
      "a simple way to specify a whole range of verbs that return this "
      "description, namely:  look, examine, ... \n"
      "After printing out the message the add_item program prints out to "
      "the person reading the note: \"You read a note.\"  More generally:  "
      "\"You <verb> <indefinate artical> <item>.\"  Similarly everyone "
      "else in the room sees:  \"Fred reads a note.\"\n\nA description of "
      "the tree can be found in treenote.\n" }));
   

   add_item("treenote", ({ "long", "Read it nim wit!",
      "read", "Here is the add_item for the oak:\n"
      " add_item( ({\"large oak tree\",\"large oak\"}) , ({  \n"
      "      \"long\",\"It looks like it can be climbed and cut.\"\n"
      "      \"climb\",\"You get half way up, and climb back down.\\n\"\n"
      "      \"cut\",\"You cannot hurt this tree!\\n\"  })); \n\n"
      "Oh joy.  Notice that it always prints out:\n"
      "   \"You <verb> a large oak.\" \n"
      "It picks the first description of the item from the mixed array.\n"
      "Some times you can interfere with item commands or soul commands "
      "by installing these actions. But, the interference would be more "
      "if you used add_action!.\n" }));
   
   add_item( ({ "large oak tree", "large oak" }) , ({
      "long", "It looks like it can be climbed and cut.",
      "climb", "You get half way up, and climb back down.\n",
      "cut", "You cannot hurt this tree!\n" }) );
   add_exit( "east", PATH +"many_verbs", "path" );
   add_exit( "west", PATH +"many_names", "path" );
   add_exit( "start", MAIN, "path" );
   add_exit( "exit", LEARNING +"search", "path" );
}

