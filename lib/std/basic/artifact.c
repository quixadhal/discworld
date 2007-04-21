/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: artifact.c,v 1.1 1998/01/06 03:59:10 ceres Exp $
 * $Log: artifact.c,v $
 * Revision 1.1  1998/01/06 03:59:10  ceres
 * Initial revision
 * 
*/
#include <artifacts.h>

int charges, level, total_charges;
string type, zapper;

string query_artifact_type() { return type; }

void set_artifact_type( string word ) { type = word; }

int query_charges() { return charges; }

void set_charges( int number ) { charges = number; }

int adjust_charges( int number ) {
  charges += number;
  if ( number > 0 )
    total_charges += number;
  if ( charges < 0 )
    charges = 0;
  return charges;
} /* adjust_charges() */

int query_total_charges() { return total_charges; }

void set_total_charges( int number ) { total_charges = number; }

int query_level() { return level; }

void set_level( int number ) { level = number; }

string query_zapper() { return zapper; }

void set_zapper( string word ) {
   zapper = word;
   call_out( "zapper_init", 1, this_object() );
} /* set_zapper() */

void zapper_init( object thing ) {
   if ( !thing )
      return;
   zapper->init_artifact( thing );
} /* zapper_init() */

int break_on_charging() {
  int top, chance;
  top = 1000 / ( 32 + level );
  if ( total_charges > top )
    chance = 6 * ( total_charges - top ) * ( total_charges - top );
  top = 14 - level / 7;
  if ( top < 0 )
    top = 0;
  if ( charges > top )
    chance += 3 * ( charges - top ) * ( charges - top );
  chance += 1;
  if ( random( 100 ) < chance )
    return 1;
  return 0;
} /* break_on_charging() */

string long( int word, int dark ) {
  string a_name, start;
  a_name = (string)call_other( zapper, "query_"+ type +"_name" );
  if ( !ARTIFACT_HAND->query_known( (string)this_player()->query_name(),
      a_name ) )
    return "";
   start = "You recognise "+ this_object()->the_short() +" to be ";
  if ( a_name[ 0 .. 0 ] == lower_case( a_name[ 0 .. 0 ] ) )
    return start + add_a( a_name ) +".  ";
  return start + a_name +".  ";
} /* long() */

mixed *stats() {
  return ({
    ({ "charges", charges +" ("+ total_charges +")", }),
    ({ "level", level, }),
    ({ "zapper", zapper, }),
  });
} /* stats() */

mapping query_dynamic_auto_load() {
  return ([
    "charges" : charges,
    "total charges" : total_charges,
  ]);
} /* query_dynamic_auto_load() */

mapping query_static_auto_load() {
  return ([
    "level" : level,
    "zapper" : zapper,
  ]);
} /* query_static_auto_load() */

void init_dynamic_arg( mapping map ) {
  charges = map[ "charges" ];
  total_charges = map[ "total charges" ];
} /* init_dynamic_arg() */

void init_static_arg( mapping map ) {
  if ( map[ "level" ] )
    level = map[ "level" ];
  if ( map[ "zapper" ] )
    zapper = map[ "zapper" ];
} /* init_static_arg() */
