#include "path.h"

inherit "/std/room";

void setup() {
   set_short( "'add_item' room telling about simple items with many names" );
   set_light( 100 );

   set_long( "This is the room telling about making simple items with many "
            "names.  To help as examples it's been filled with several items "
            "including a small note, candles and some chairs.  Look at the "
            "note to learn more.  Go east to learn about adding verbs to the "
            "items, and west to go to a simpler example.\n" );
   add_item( "small note",
            "Ok things get more complicated here.\n"
            "First of all, the mudlib adds the plural forms of an "
            "item you add to a room.  Thus, \n"
            "  add_item( \"candle\", \"Very nice.\" );\n"
            "will take care of both \"look at candle\" and \"look at "
            "candles\".\n"
            "Secondly, the code:\n"
            "  add_item( \"small note\", \"Ok things ... \n"
            "will take care of both \"look at note\" and \"look at small "
            "note\"\n"
            "And finally, the code:\n"
            "  add_item( ({ \"corridor\", \"long hallway\" }), "
            "\"It is long.\" );\n"
            "will take care of \"look at corridor\", \"look at hallway\", "
            "\"look at long hallway\" and even \"look at long hallways\".\n"
            "If you look at the hallways you'll see the message you get is "
            "not very good as it refers to a single hallway.  To make a "
            "different description when you look at a single "
            "item and all of them, you can use the optional third parameter "
            "to tell the mudlib it shouldn't try to add the plural form. "
            "Thus:\n"
            "  add_item( \"chair\", \"This chair looks about ready to fall "
            "over.\", 1 );\n"
            "  add_item( \"chairs\", \"The chairs look ready to fall "
            "over.\", 1 );\n"
            "Oh, one last thing when listing things in this manner as a rule "
            "of thumb, put the best description first.\n" );
   add_item( "candle", "Very nice." );
   /* Warning, look at the note to see why this is not a good example */
   add_item( ({ "corridor", "long hallway" }), "It is long." );
   add_item( "chair", "This chair looks about ready to fall over", 1 );
   add_item( "chairs", "The chairs look ready to fall over", 1 );

   add_item( "item", "You don't see any since noone coded one yet!" );
   add_item( "reference", "Like one that you would find in a theusaures." );
   add_item( "room", "It is a big room." );

   add_exit( "east", PATH +"verbs", "path");
   add_exit( "west", PATH +"intro", "path");
   add_exit( "start", MAIN, "path" );
   add_exit( "exit", LEARNING +"search", "path" );
} /* setup() */
