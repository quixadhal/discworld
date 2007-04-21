#include "path.h"

inherit "/std/room";

void setup() {
   set_short( "'add_item' room telling about the arguments of the function" );
   set_light( 100 );
   add_property( "determinate", "the " );

   set_long( "This is a room telling about the arguments passed to the "
            "do_<verb>() function.  Go east for examples of using the "
            "arguments, west for something else:).\n"
            "Here goes:\n"
            "  int do_<verb>( string command, object *indir, string dir_match,\n" 
            "                 string indir_match, mixed *args, string pattern );\n"
            "$I$12=command     is a string, it is the name of the verb "
             "used.$I$0=\n"
            "$I$12=indir       is an array of objects.  If any indirect "
             "objects are requested for, then they will be in this array."
             "$I$0=\n"
            "$I$12=dir_match   is a string, it is the name of the direct "
             "object.$I$0=\n"
            "$I$12=indir_match is a string, it is the name of the indirect "
             "object.$I$0=\n"
            "$I$12=args        is a mixed array of strings and numbers.  For "
             "each <> thingie you have in the pattern, it has a member.  So "
             "if you have <direct:object>, it has the name of the direct "
             "object, if you have <number> then it will have a number also, "
             "if <indirect:object>, the indirect object's name, and so on.  "
             "They are in the same order as in the pattern.$I$0=\n"
            "$I$12=pattern     is a string containing the pattern. It is "
             "useful to know which pattern has been caught since multiple "
             "patterns are allowed per function.$I$0=\n\n"
            "All do_<verb> functions in the learning domain will write their "
            "arguments, to help you figure out how to use them.\n"
            "A pool you can jump into is here.\n" );
   
   add_item( "large pool", ({
      "long", "It looks like you can jump into it.",
      "jump", ({ this_object(), "do_jump", "[into|in] <direct:object>" })
   }) );
   add_exit( "east", PATH +"args_integer", "path" );
   add_exit( "west", PATH +"verb_patterns", "path" );
   add_exit( "start", MAIN, "path" );
   add_exit( "exit", LEARNING +"search", "path" );
} /* setup() */

int do_jump( string command, object *indir, string dir_match, 
            string indir_match, mixed *args, string pattern ) {
   tell_object( this_player(), 
               sprintf( "$I$2=do_jump called:\ncommand=%O\nindir=%O\n"
                       "dir_match=%O\nindir_match=%O\nargs=%O\n"
                       "pattern=%O$I$0=\n",
                       command, indir, dir_match, 
                       indir_match, args, pattern ) );
      
   this_player()->add_succeeded_mess( previous_object(),
                                     "$N $V into a pool!\n", ({ }) );
   return 1;
} /* do_jump() */
