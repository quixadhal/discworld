/****** This room is made my Mithal, learn and enjoy  ***/

#include "path.h"

inherit "/std/room";
object key;

void setup() {
   set_short( "'add_item' room telling about getting the object as argument" );
   set_light(100);
   add_property( "determinate", "the " );

   set_long( "This is the room telling about getting string arguments from a "
            "command defined with add_item.  Go east to see about getting "
            "string arguments, west to see about item conflicts.\n"
            "There is a large stone in the middle of the room.  It looks as "
            "if you can unlock it if you have the correct key.\n" );
   
   add_item( "large stone", ({
      "long","It can be unlocked.",
      "unlock", ({ this_object(), "do_unlock", 
         "<direct:object> [with] <indirect:object>" })
   }) );
   add_exit( "east", PATH +"item_conflicts", "path" );
   add_exit( "west", PATH +"args_string", "path" );
   add_exit( "start", MAIN, "path" );
   add_exit( "exit", LEARNING +"search", "path" );
} /* setup() */

int do_unlock( string command, object *indir, string dir_match, 
               string indir_match, mixed *args, string pattern ) {
   object ob;
   
   tell_object( this_player(), 
               sprintf( "$I$2=do_unlock called:\ncommand=%O\nindir=%O\n"
                       "dir_match=%O\nindir_match=%O\nargs=%O\n"
                       "pattern=%O$I$0=\n",
                       command, indir, dir_match, 
                       indir_match, args, pattern ) );
   this_player()->add_failed_mess( previous_object(),
                                  "You don't have the correct key.\n", ({ }) );
   /* Note that the pattern guarantees that there is an indir */
   ob = indir[ 0 ];
   if( (int) ob->query_property( "Mithal" ) != 1 ) { 
      write( "wrong key\n" ); 
      return 0; 
   }
   /* for every object you succeed to perform the verb with, you
    * must have these calls
    */
   this_player()->add_succeeded( ob );   
   this_player()->add_succeeded_mess( previous_object(),
                                     "$N $V the stone with $I.\n",
                                     ({ ob }));
   write( "You succeeded!\n" );
   return 1;
} /* do_unlock() */

void reset() {
   call_out( "check_key", 2 );
} /* reset() */

void check_key() {
   if( !key ) {
      key = clone_object( "std/object" );
      key->set_name( "key" );
      key->set_short( "Mithal's key" );
      key->add_property( "Mithal", 1 );
      key->move( this_object(), "Mithal sneaks through the room, "
                "dropping $N in the process." );
   }
} /* check_key() */

