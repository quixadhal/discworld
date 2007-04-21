#include "path.h"

inherit "/std/room";

void setup() {
   set_short( "primary room for \"add_item\"" );
   add_property( "determinate", "the " );
   set_light( 100 );
   set_long( "This is the start room for the examples for add_item.  "
            "Essentially boring, since it's only here to start you on your "
            "way:)\n" );   
   add_exit( "start", PATH +"intro", "path" );
   add_exit( "failure", PATH +"verb_failure", "path" );
   add_exit( "success", PATH +"verb_success", "path" );
   add_exit( "patterns", PATH +"verb_patterns", "path" );
   add_exit( "arguments", PATH +"func_arguments", "path" );
   add_exit( "args_int", PATH +"args_integer", "path" );
   add_exit( "args_string", PATH +"args_string", "path" );
   add_exit( "args_object", PATH +"args_object", "path" );
   add_exit( "helper", PATH +"item_helper", "path" );
   add_exit( "exit", LEARNING + "search", "path" );
   
   /* the normal add_property() calls for the learning domain
    */
   add_property( "commented functions", ({ "add_item" }) );
   add_property( "keywords", ({ "room", "add_item", "action", "command" }) );
} /* setup() */
