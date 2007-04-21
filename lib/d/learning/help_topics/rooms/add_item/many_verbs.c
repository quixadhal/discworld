/****** This room is made my Mithal, learn and enjoy  ***/

#include "path.h"

inherit "/std/room";

void setup() {
   set_short("add_item room #4, many verbs ");
   set_light(100);

   set_long( "add_item room #4, many verbs \n"
            "Sometimes you have two or more identical verbs - or at least "
            "identical in how you want to treat them.  Take our oak tree "
            "we can cut it, slice it, saw it, etc...  Now all of these have "
            "the same meaning. How do we do this?\n"
            "A large oak.\nA note.\n");
   
   add_item("note", ({ "long", "It can be read.",
      "read", "Here is the code for the tree:\n"
      "    add_item(\"large oak tree\", ({\n"
      "      \"long\",\n"
      "              \"It looks like it is very well protected.\"\n"
      "      ({\"cut\",\"slice\",\"saw\",\"destroy\",\"kill\" }),\n"
      "              \"It seems to have no effect.\\n\"       }});\n\n"
      "Pretty obvious hunh?  Note that kill probably does not work( I "
      "say probably cuz things might change)  That is beacuse user commands "
      "have precidence over object actions in rooms.  Also be aware that a "
      "add_action on an item that a person is holding will most likely stop "
      "your verbs from being used (unless it's coded right:).  Soul commands "
      "do not have precedence, however.  So verbs like \"pick\" which would "
      "normally say \"You pick your nose\" can be trapped appropriately.\n" 
   }) );
   
   add_item("large oak", ({
      "long", "It looks like it is very well protected.",
      ({ "cut", "slice", "saw", "destroy", "kill" }),
        "It seems to have no effect.\n" }) );

   add_exit( "east", PATH +"action_verbs", "path");
   add_exit( "west", PATH +"verbs", "path");
   add_exit( "start", MAIN, "path" );
   add_exit( "exit", LEARNING +"search", "path" );
}
