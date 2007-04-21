/****** This room is made my Mithal, learn and enjoy  ***/

#include "path.h"

inherit "/std/room";

void setup() {
   set_short("add_item room #6, making verbs do stuff, better version ");
   set_light(100);

   set_long("add_item room #6, making verbs do stuff, better version\n"
	    +"There are two ways to call a function if a verb is used.  "
	    +"This room demonstrates the second one.  "
       	    +"Lets say you have a rat in a room and randomly ( one out of "
	    +"two times lets say ) want to let players touch it. "
	    +"This is how you could do it.\n"
	    +"A filthy rat and a can.\nA ratnote and a cannote.\n");

   
   add_item("ratnote", ({ "long", "It can be read.",
		       "read", "Here is the code for the rat:\n"
     +"   add_item(\"large filthy rat\", ({\n"
     +"        \"long\",\n"
     +"           \"It looks like it wants to be touched.\\n\"\n"
     +"         ,\"touch\",\n"
     +"           ({ this_object(), \"do_touch\" }) }) );  \n"
     +"When the rat is touched, do_touch() is called.  The do_touch() func "
     +"must return an integer - If the touch was successful it returns a 1, "
     +"if the touch failed the function should return a 0.  Note that a "
     +"success message is given to all in the room.  But, if the person fails "
     +"the add_item code looks for more matches - thinking that the func "
     +"do_touch is the wrong one.  If there is no other function found that "
     +"defines touching rats and that returns a 1, then the game returns the "
     +"message 'You fail to touch a rat'.\n"
     +"The this_object() refrence tells the add_item code what object is "
     +"the direct object of the verb.  Just trust me, and leave it in your "
     +"code without asking why for now.  It is one of those things that can "
     +"give you more flexibility later on.\n" }));
   

   add_item("dirty can", ({
	        "long",
	                   "It looks like it wants to be touched."
		,"touch",
                             ({ this_object(), "always_fail" })
                   }));

      
   add_item("cannote", ({ "long", "It can be read.",
		          "read", "Here is the code for the can:\n"
     +"   add_item(\"dirty can\", ({\n"
     +"        \"long\",\n"
     +"           \"It looks like it wants to be touched.\\n\"\n"
     +"         ,\"touch\",\n"
     +"           ({ this_object(), \"always_fail\" }) }) );  \n"
     +"When the can is touched, always_fail() is called.  This function "
     +"looks like this:\n int always_fail() { return 0; }\n"
     +"in this way if you want a person to be able to use a verb on an "
     +"object, but want to be lazy - and don't want the action to succeed "
     +"( such as a door that can't be broken down ) this is the way to do "
     +"it.\nPlease read ratnote also, if you havn't.\n" }));

   

   add_item("large filthy rat", ({
	        "long",
	                   "It looks like it wants to be touched."
		,"touch",
                             ({ this_object(), "do_touch" })
                   }));

   add_exit("east",ROOM+"verb_failure","door");
   add_exit("west",ROOM+"changing_descs","door");
   add_exit("north",MAIN,"door" );

 }

int  do_touch()
{
  if( random(2) ) {
      /* Code to infect this_player() */
      tell_object(this_player(),"\n ****  You feel deathly sick. ****\n  "
		              "(this is a tell_object(this_player(),...)) \n");
      tell_object(this_player(),
		  "do_touch is returning a 0, indicating failure!\n");
      return 0;
    }

    return 1;
}
    
   

