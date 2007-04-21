/****** This room is made my Mithal, learn and enjoy  ***/

#include "path.h"

inherit "/std/room";
object key;

void setup() {
   set_short( "'add_item' room telling about resolving item conflicts" );
   set_light( 100 );

   set_long( "This is the room telling about resolving item conflicts in "
            "items defined with add_item.\n"
            "This room still has some basic problems, and shouldn't be used "
            "as an example.\n"
            "Inside, add_item creates one "
            "object per room for its use and assigns an alias to each item "
            "pointing to this object.  Therefore it is likely that "
            "if you have multiple add_items with verbs that call functions "
            "you can run into problems.  In this room there is a fountain and "
            "a pool.  Both of these define 'toss'.  If the wrong function is "
            "called, then your function must return a 0 in order for the "
            "correct function to be called.  Otherwise you will have a bug!  "
            "Please look at this code and notice how each function checks to "
            "make sure that the appropriate pattern is used.  If this code "
            "were removed, then funny things would start to happen.:)\n" );
   
   add_item( "small fountain", 
            ({ "long",
               "You can see a few platinum coins in the water.",
               "toss", ({ this_object(), "do_throw_fount",
                  "<number> platinum [coins|coin] into <direct:object>" })
            }) );
   add_item( "small pool",
            ({ "long", "You can see some silver coins lying around in the "
                       "pool.",
               "toss", ({  this_object(),"do_throw_pool",
                  "<number> silver [coins|coin] into <direct:object>" })
            }) );
   
/*   add_exit( "east", PATH + "item_helper", "path" ); */
   add_exit( "west", PATH + "args_object", "path" );
   add_exit( "start", MAIN, "path" );
   add_exit( "exit", LEARNING +"search", "path" );
} /* setup() */

int do_success_thing( int pool, int silver, int no ) {
   /* This function generates the success message! */
   /* pool and silver are boolean */
   string coinstr, poolstr, nocoins;
   
   if( no < 0 ) return 0;
   
   if( silver ) coinstr = "silver";
   else coinstr = "platinum";
   
   if( pool ) poolstr = "pool";
   else poolstr = "fountain";
   
   nocoins = no+" "+coinstr+" coins";
   if( no == 1 ) nocoins = "one "+coinstr+" coin";
   this_player()->add_succeeded_mess( previous_object(),
           "$N $V "+ nocoins +" into a "+ poolstr +".\n", ({ }) ); 
   return 1;
} /* do_success_thing() */

int do_throw_pool( string command, object *indir, string dir_match, 
                  string indir_match, mixed *args, string pattern ) {
   string nocoins;
   int no;

   tell_object( this_player(), 
               sprintf( "$I$2=do_throw_pool called:\ncommand=%O\n"
                       "dir_match=%O\nargs=%O\n"
                       "pattern=%O$I$0=\n",
                       command, dir_match, 
                       args, pattern ) );
   no = args[ 0 ];
   if( pattern != "<number> silver [coins|coin] into <direct:object>" )
      return do_success_thing( 1, 0, no );

   if( !do_success_thing( 1, 1, no ) ) return 0;
   
   tell_object( this_player(), "You throw coins!\n");

   return 1;
} /* do_throw_pool() */

int do_throw_fount( string command, object *indir, string dir_match, 
                   string indir_match, mixed *args, string pattern ) {
   int no, stored, ran;
   
   tell_object( this_player(), 
               sprintf( "$I$2=do_throw_fount called:\ncommand=%O\n"
                       "dir_match=%O\nargs=%O\n"
                       "pattern=%O$I$0=\n",
                       command, dir_match, 
                       args, pattern ) );
   no = args[ 0 ];
   if( pattern != "<number> platinum [coins|coin] into <direct:object>" )
      return do_success_thing( 0, 1, no );
   if( !do_success_thing( 0, 0, no ) ) return 0; 
   
   tell_object( this_player(),  "*giggle*\n");
   
   return 1;                          
} /* do_throw_fount( )*/
