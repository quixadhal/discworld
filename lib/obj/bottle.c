/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: bottle.c,v 1.4 2003/07/09 11:26:29 carmine Exp $
 * $Log: bottle.c,v $
 * Revision 1.4  2003/07/09 11:26:29  carmine
 * Gave bottles vague sizes and a material
 *
 * Revision 1.3  2001/01/16 02:41:28  presto
 * Changed mass to be a float
 *
 * Revision 1.2  1999/10/25 23:05:21  pinkfish
 * Fix up a few elements of things.
 *
 * Revision 1.1  1998/01/06 04:58:17  ceres
 * Initial revision
 * 
*/
#define PHIAL 150
#define BOTTLE 1000
#define QUIRM 2500
#define DEMIJOHN 50000

inherit "/obj/vessel";

nosave string glass;

void make_bottle( string word, int number ) {
   float mass;
   string adjective, noun;
   glass = word;

   set_max_volume(number*4);
   mass = number / 100.0;
   if ( !mass ) mass = 1;
   set_leak_rate( 0 );
   set_value( to_int(mass * 25 + random( mass * 10 )) );
   set_weight( to_int(mass + random( mass * 10 ) / 10 ));
   set_max_weight( to_int(mass * 2 ));
   add_property( "fragile", to_int(mass * 20 + random( mass * 5 )) );
   switch( number ) {
      case 0..PHIAL:
         adjective = "small";
         noun = "phial";
         set_length( 2 );
         set_width( 1 );
         break;
      case PHIAL+1..BOTTLE:
         adjective = "medium sized";
         noun = "bottle";
         set_length( 12 );
         set_width( 6 );
         break;
      case BOTTLE+1..QUIRM:
         adjective = "large";
         noun = "Quirm";
         set_length( 24 );
         set_width( 12 );
         break;
      case QUIRM+1..DEMIJOHN:
         adjective = "very large";
         noun = "demijohn";
         set_length( 48 );
         set_width( 24 );
         break;
      default:
         adjective = "huge";
         noun = "tank";
         set_length( 88 );
         set_width( 48 );
   }
   set_name( lower_case( noun ) );
   set_short( adjective +" "+ glass +" "+ noun );
   set_long( "This is a "+ adjective +" "+ glass +" glass "+ noun +".\n" );
   add_alias( "bottle" );
   add_adjective( ({ adjective, glass }) );
   set_main_plural( adjective +" "+ glass +" "+ noun +"s" );
   add_plural( lower_case( noun ) +"s" );
   set_material( ({ "glass" }) );
} /* make_bottle() */

string query_glass() { return glass; }
int query_number() { return (int)query_property( "number" ); }

mixed *stats() {
   return ::stats() + ({
      ({ "glass", glass, }),
   });
} /* stats() */

mapping int_query_static_auto_load() {
   return ([
      "::" : ::int_query_static_auto_load(),
      "glass" : glass,
   ]);
} /* int_query_static_auto_load() */

void init_static_arg( mapping map ) {
   if ( map["::"] )
      ::init_static_arg( map["::"] );
   if ( !undefinedp( map["glass"] ) )
      glass = map["glass"];
} /* init_static_arg() */

mixed query_static_auto_load() {
   if ( explode( file_name( this_object() ), "#" )[ 0 ] == "/obj/bottle" )
      return int_query_static_auto_load();
   return ([ ]);
} /* query_static_auto_load() */
