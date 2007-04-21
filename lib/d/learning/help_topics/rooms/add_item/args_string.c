#include "path.h"

inherit "/std/room";

void setup() {
   set_short( "'add_item' room telling about getting string arguments" );
   set_light( 100 );
   add_property( "determinate", "the " );
   
   set_long( "This is the room telling about getting string arguments from a "
            "command defined with add_item.  Go east to see about getting "
            "object arguments, west to see about integer arguments.\n"
            "There is a large stone in the middle of the room.  It looks as "
            "if you can engrave your name onto it.\n");
   add_item( "large stone", 
            ({ "long", "It can be engraved.",
               "engrave", ({ this_object(), "do_engrave", 
                  "[the text] <string> [into] [the] <direct:object>" })
            }) );
   add_exit( "east", PATH +"args_object", "path");
   add_exit( "west", PATH +"args_integer", "path");
   add_exit( "start", MAIN, "path" );
   add_exit( "exit", LEARNING +"search", "path" );
} /* setup() */

int do_engrave( string command, object *indir, string dir_match, 
               string indir_match, mixed *args, string pattern ) {
   string mess;
   
   tell_object( this_player(), 
               sprintf( "$I$2=do_engrave called:\ncommand=%O\nindir=%O\n"
                       "dir_match=%O\nindir_match=%O\nargs=%O\n"
                       "pattern=%O$I$0=\n",
                       command, indir, dir_match, 
                       indir_match, args, pattern ) );
   
   mess =  "$N $V \""+ args[ 0 ] +"\" into "+ dir_match +".\n";
   this_player()->add_succeeded_mess( previous_object(), mess, ({ }) );
   return 1;
} /* do_engrave() */
