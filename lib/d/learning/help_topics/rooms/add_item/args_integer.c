#include "path.h"

inherit "/std/room";

void setup() {
   set_short( "'add_item' room telling about getting integer arguments" );
   set_light( 100 );
   add_property( "determinate", "the " );

   set_long( "This is the room telling about getting integer arguments from a "
            "command defined with add_item.  Go east to learn about getting "
            "string arguments, west to learn about the arguments passed "
            "to the function called when you do the command.\n"
            "This room is filled with an uncountable number of rats.  They "
            "all have numbers branded into them.  You can dissolve a rat if "
            "you know its number.\n" );
   
   add_item( "large filthy rat", ({
      "long", "The rats look vile.",
      "dissolve", ({ this_object(), "do_dissolve",
         "<direct:object> [no|number] <number>" })
   }) );
   add_exit( "east", PATH +"args_string", "path");
   add_exit( "west", PATH +"func_arguments", "path");
   add_exit( "start", MAIN, "path" );
   add_exit( "exit", LEARNING +"search", "path" );
} /* setup() */

int do_dissolve( string command, object *indir, string dir_match, 
                string indir_match, mixed *args, string pattern ) {
   string mess;
   
   tell_object( this_player(), 
               sprintf( "$I$2=do_dissolve called:\ncommand=%O\nindir=%O\n"
                       "dir_match=%O\nindir_match=%O\nargs=%O\n"
                       "pattern=%O$I$0=\n",
                       command, indir, dir_match, 
                       indir_match, args, pattern ) );
   
   mess =  "$N $V "+ dir_match +" number "+ args[ 1 ] +".\n";
   this_player()->add_succeeded_mess( previous_object(),
                                      mess, ({ }) );
   return 1;
} /* do_dissolve() */
