/****** This room is made my Mithal, learn and enjoy  ***/

#include "path.h"

inherit "/std/room";

void setup() {
   set_short("add_item room #7, verb failure messages ");
   set_light(100);

   set_long("add_item room #7, verb failure messages\n"
            +"Let us continue with our rat.  Sometimes the general failure "
            +"message just dosn't cut it.  When the player fails to  "
            +"touch our rat, the mud should tell him/her nicely and not "
            +"let them think they messed up.  This rat does just that.  "
            +"Notice that, unlike success messages, failure messages go only "
            +"to the player - not to everyone in the room.  *smile* \n"
            +"A filthy rat.\nA note.\n");
   
   
   add_item("note", ({ "long", "It can be read.",
		       "read", "Here is the code for the do_touch:\n"
	     +"int  do_touch()\n"
             +"{\n"
	     +"   if(!random(2)) {\n"
	     +"       this_player()->add_failed_mess(previous_object(),\n"
	     +"                       \"You miss the rat!\\n\",\n"
	     +"                       ({})); \n"
	     +"        return 0;\n"
	     +"    }\n"
	     +"    return 1;\n"
	     +"}\n\n"
	     +"The add_failed_mess is a pretty complicated fella.  The first "
	     +"argument I beleve is the object that the verb failed on, ie "
	     +"the rat.  The second argument is the failure message. Since "
	     +"it is going only to the player, it is really simple to "
	     +"understand.  The third argument is the set of indirect "
	     +"that were used in the failing attempt.  I think you can "
	     +"reference there name in the message as %I.  But don't quote "
	     +"me on that.  Similarly %D can be used as a substution for the "
	     +"direct object, ie previous_object().  It is complicated.  "
	     +"If you stick to this form, you shouldn't have any problems.  "
	     +"If all else fails, read the help file on it. *giggle* \n" }));
   
   add_item( "large filthy rat", ({
      "long", "It looks like it wants to be touched.",
      "touch", ({ this_object(), "do_touch" }) }));
   
   add_exit( "east", PATH +"verb_success", "path" );
   add_exit( "west", PATH +"action_verbs", "path" );
   add_exit( "start", MAIN, "path" );
   add_exit( "exit", LEARNING +"search", "path" );
}

int do_touch() {
   if(!random(2)) {
      this_player()->add_failed_mess(previous_object(),
                                     "You miss the rat!\n",
                                     ({}));
      return 0;
   }
   return 1;
}
    
   

