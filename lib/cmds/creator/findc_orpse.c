/* -*- LPC -*- *
 * An attempt to make a command to find player corpses.
 * Goodness only knows how this will turn out.
 * From an exec constructed by young Wonko.
 * Started 19 May 1999
 * Allana
 */

inherit "/cmds/base";

int cmd( string arg ) {

   object *cs, c;
   string r;
  
   r = "player corpse objects\n";
  
   cs = filter( children( "/obj/corpse" ), (: $1 && $1->query_property(
      "player" ) :) );

   if( !arg ) {

      foreach( c in cs ) r += file_name( c ) + " - " + c->query_owner() + 
         " - " + ( environment( c ) ? file_name( environment( c ) ) : 
         "In void" ) + "\n";

   
      tell_object( this_player(), r );

      tell_object( this_player(), sizeof( cs ) + " player corpses.\n" );
 
   } else {

      cs = filter( cs, (: $1 && $1->query_ownership() == $( arg ) :) ); 

      if( sizeof( cs ) ) { 
         r = "Corpse objects of " + arg + ": \n"; 
         foreach( c in cs ) r += file_name( c ) + " in " + (
         environment( c ) ? file_name( environment( c ) ) : "void" ) + "\n";

         tell_object( this_player(), r ); 

      } else { 

         tell_object( this_player(), arg + " has no corpses loaded.\n" );    
      }
   }                
   return 1;
} 

void dest_me() {
  destruct(this_object());
}

void clean_up() {
  dest_me();
}

void reset() {
  dest_me();
}
