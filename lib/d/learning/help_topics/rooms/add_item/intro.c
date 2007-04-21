#include "path.h"

inherit "/std/room";

void setup() {
   set_short( "'add_item' room telling about simple items" );
   set_light( 100 );
   add_property( "determinate", "the " );

   set_long( "This is the first room about add_item(), covering simple items.\n"
            "Go east to see how to make items that can be refered to in more "
            "complicated ways.\n"
            "In your course of describing a room you often mention quite "
            "a few nouns.  In this mud we try to give every noun mentioned a "
            "description a description.  We do this with the function "
            "add_item.  For more information read the note.\n" );
   
   add_item( "noun", "Nouns should have descriptions." );
   add_item( "description", "Descriptions should be witty wherever reasonable." );
   add_item( "discworld", "The mud you are learning to code for." );
   add_item( "information", "See the note." );
   add_sign( "This is a note with some information, perhaps you should read "
                "it.\n",
             "For simple items like 'room' the corresponding code "
               "to represent that item is relatively easy:\n\n"
               "   add_item( \"room\", \"It is a big room.\" );  \n\n"
               "Notice that there is no newline in the description.  "
               "The mudlib adds newline when necessary.  Also "
               "notice that this is not true with set_long where a \\n is "
               "necessary at the end of the description.",
             "note", "note" );
   add_item( "room", "It is a big room." );

   add_exit( "east", PATH +"many_names", "path" );
   add_exit( "start", MAIN, "path" );
   add_exit( "exit", LEARNING +"search", "path" );
} /* setup() */
