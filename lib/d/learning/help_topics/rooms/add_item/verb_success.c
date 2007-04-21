/****** This room is made my Mithal, learn and enjoy  ***/

#include "path.h"

inherit "/std/room";

void setup() {
   set_short("add_item room #8, verb success messages ");
   set_light(100);

   set_long("add_item room #8, verb success messages\n"
	    "Let us continue with our rat.  Suppose now that we want the "
	    "player to succeed - but that the default message is really "
	    "inappropriate.  What we want to do is change the succeed "
	    "message!  Let's say when the player goes to touch the rat "
	    "it tells him - As you touch the rat, it snarls. - "
	    "Unfortunately succeed messages are much more complicated "
	    "beacuse your message is going to two parties, the player and "
	    "other players in the room. So you can't put the words \"You\" and "
	    "\"touch\" directly into code, because \"Fred\" and \"touches\" are the "
	    "appropriate substitutions to be used for the audience.  "
	    "Furthermore, if you are useing "
	    "multiple verbs, how do you know what verb the person used? "
	    "The solution is to use $V for verb and $N for player name.\n"
	    "A filthy rat.\nA note.\n");

   
   add_item("note", ({ "long", "It can be read.",
		       "read", "Here is the code for the do_touch:\n"
	     "int  do_touch()\n"
             "{\n"
             "      this_player()->add_succeeded_mess(previous_object(),\n"
	     "        \"As $N $V $D, it snarls.\\n\",  ({}));\n"
	     "    return 1;\n"
	     "}\n\n"
	     "The only difference between add_failed_mess and this one is "
	     "the message: When the message is processed for the player, "
	     "\"You\" is substituted for $N and \"touch\" for $V.  Likewise, when "
	     "the message is processed for others: \"Fred\" is substituted for $N "
	     "and \"touches\" is substituted for $V.  If you were using multiple "
	     "verbs, the appropriate verb would be used.  There might be "
	     "other $ stuff in the docs( namely $D and $I if you have an "
	     "indirect object list.)  There might be ones for pronouns and "
	     "possessives also.  At last "
	     "resort, you can always make this string a null string! And "
	     "insert tell_other and tell_room call's directly in your code."
	     "Of course add_failed_mess and this one can be used together in "
	     "the same code...\n" }));

   
   add_item("large filthy rat", ({
	        "long",
	                   "It looks like it wants to be touched.",
		"touch",
                           ({ this_object(), "do_touch" }) }));

   add_exit("west",PATH+"verb_failure","path");
   add_exit("east",PATH+"verb_patterns","path");
   add_exit( "start", MAIN, "path" );
   add_exit( "exit", LEARNING +"search", "path" );
} /* setup() */

int  do_touch()
{
  this_player()->add_succeeded_mess(previous_object(),
	            "As $N $V $D, it snarls.\n",  ({}));
    return 1;
} /* do_touch() */
