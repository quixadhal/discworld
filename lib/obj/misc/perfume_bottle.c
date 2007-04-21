#include <move_failures.h>

inherit "/std/object";
inherit "/std/basic/holdable";

int squirts;
string glass, scent, scent_plural;

void create() {
   object::create();
   holdable::create();
} /* create() */

void setup() {
   set_name( "bottle" );
   set_short( "perfume bottle" );
   add_adjective( "perfume" );
   set_value( 4000 );
   set_weight( 2 );
   squirts = 12;
} /* setup() */

void set_glass( string word ) {
   set_short( word +" perfume bottle" );
   add_adjective( word );
   set_main_plural( word +" perfume bottles" );
   glass = word;
} /* set_glass() */

void set_scent( string words ) { scent = words; }
void set_scent_plural( string words ) { scent_plural = words; }

string long( string word, int dark ) {
   string words;
   words = "This is a perfume bottle made out of "+ glass +" crystal.  "+
         "It is ";
   switch( squirts ) {
      case 0 :
         return words +"empty.\n";
      case 1 .. 3 :
         words += "about a quarter full";
         break;
      case 4 .. 6 :
         words += "about half full";
         break;
      case 7 .. 9 :
         words += "about three-quarters full";
         break;
      default :
         words += "almost full";
   }
   words += " of "+ scent +" perfume which can be sprayed onto "+
         "someone.\n";
   return words;
} /* long() */

void init() {
   this_player()->add_command( "spray", this_object(), "<direct:object> at <indirect:object>" );
} /* init() */

int do_spray( object *things ) {
  if ( query_wielded() != this_player() ) {
    this_player()->add_failed_mess( this_object(), "You must be holding "+
        "$D to use it.\n", ({ }) );
    return 0;
  }
  if ( sizeof( things ) > 1 ) {
    this_player()->add_failed_mess( this_object(), "You can only spray $D "+
        "at one person at a time.\n", ({ }) );
    return 0;
  }
  if ( things[ 0 ] == this_object() ) {
    this_player()->add_failed_mess( this_object(), "You cannot spray $D at "+
        "itself!\n", ({ }) );
    return 0;
  }
  if ( !living( things[ 0 ] ) ) {
    this_player()->add_failed_mess( this_object(), "SI would probably not "+
        "benefit from being sprayed.\n", ({ things[ 0 ] }) );
    return 0;
  }
  if ( !squirts ) {
    this_player()->add_failed_mess( this_object(), "There is no perfume in "+
        "$D to spray at $I.\n", ({ things[ 0 ] }) );
    return 0;
  }
  squirts--;
  things[ 0 ]->add_effect( "/std/effects/other/perfume", ({ 300, scent, 
      scent_plural }) );
  this_player()->add_succeeded( things[ 0 ] );
  return 1;
} /* do_spray() */

mapping query_dynamic_auto_load() {
  return ([
    "::" : ::query_dynamic_auto_load(),
    "squirts" : squirts
  ]);
} /* query_dynamic_auto_load() */

mapping query_static_auto_load() {
  if ( explode( file_name( this_object() ), "#" )[ 0 ] !=
      "/obj/misc/perfume_bottle" )
    return 0;
  return ([
    "::" : int_query_static_auto_load(),
    "glass" : glass,
    "scent" : scent,
    "scent_plural" : scent_plural
  ]);
} /* query_static_auto_load() */

void init_dynamic_arg( mapping map ) {
  if ( map[ "::" ] )
    ::init_dynamic_arg( map[ "::" ] );
  squirts = map[ "squirts" ];
} /* init_dynamic_arg() */

void init_static_arg( mapping map ) {
  if ( map[ "::" ] )
    ::init_static_arg( map[ "::" ] );
  if ( map[ "glass" ] )
    set_glass( map[ "glass" ] );
  if ( map[ "scent" ] )
    scent = map[ "scent" ];
  if ( map[ "scent_plural" ] )
    scent_plural = map[ "scent_plural" ];
} /* init_static_arg() */


void dest_me() {
   holdable::dest_me();
   object::dest_me();
} /* dest_me() */

varargs int move( mixed ob, string mess1, string mess2 ) {
   int ret;

   ret = holdable::move(ob);
   if (ret != MOVE_OK) {
      return ret;
   }
   return object::move( ob, mess1, mess2 );
}
