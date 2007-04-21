/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: ring.c,v 1.4 2001/06/01 21:14:59 ceres Exp $
 * $Log: ring.c,v $
 * Revision 1.4  2001/06/01 21:14:59  ceres
 * Can't remember
 *
 * Revision 1.3  1999/10/09 19:31:42  taffyd
 * Added jewellery alias.
 *
 * Revision 1.2  1999/04/06 00:28:22  ceres
 * Modified to use perform_task
 *
 * Revision 1.1  1998/01/06 04:58:17  ceres
 * Initial revision
 * 
*/
#include <artifacts.h>
#include <tasks.h>

inherit "/obj/armour";
inherit "/std/basic/artifact";

int query_magic_ring() { return 1; }

void create() {
   ::create();
   set_type( "ring" );
   set_damage_chance( 8 );
   set_artifact_type( "ring" );
   add_property( "shop type", "jewellers" );
   add_alias( "jewellery" );
   add_plural("jewellery");
} /* create() */

void set_weight( int number ) {
   armour::set_weight( number );
   setup_armour( 250 * number );
   add_ac( "blunt", "blunt", 5 * weight );
   add_ac( "sharp", "sharp", 2 * weight );
} /* set_weight() */

string long( int word, int dark ) {
  return artifact::long( word, dark ) + armour::long( word, dark );
} /* long() */

void init() {
  this_player()->add_command( "twist", this_object() );
} /* init() */

int do_twist() {
  int outcome;
  if ( query_worn_by() != this_player() ) {
    this_player()->add_failed_mess( this_object(), "You must be "+
        "wearing $D to $V it.\n", ({ }) );
    return 0;
  }

  outcome = (int)TASKER->perform_task(this_player(), RING_SKILL,
                                      level, TM_FREE);
  if ( outcome == FAIL ) {
    this_player()->add_failed_mess( this_object(), "You failed "+
        "to use $D.\n", ({ }) );
    return 0;
  }
  if ( !charges ) {
    this_player()->add_failed_mess( this_object(), "It appears that there "+
        "are no charges in $D.\n", ({ }) );
    return 0;
  }
  if ( outcome == AWARD ) {
    write( "%^YELLOW%^" +
          ({ "A piece of lore concerning rings becomes clearer for "+
        "you.\n", "You manage to grasp one of the principles of magic "+
        "rings more firmly.\n", "A sudden flash of insight allows you "+
        "to understand the ring a little better.\n" })[ random( 3 ) ] +
          "%^RESET%^");
  }
  charges--;
  if ( random( 100 ) < level / 10 ) {
    if ( !zapper->failed_zap( 0, this_player(), this_object() ) )
      this_player()->add_succeeded_mess( this_object(), "$N $V $D but "+
          "nothing happens!\n", ({ }) );
    return 1;
  }
  zapper->zap( 0, this_player(), this_object() );
  return 1;
} /* do_zap() */

mixed *stats() {
  return armour::stats() + artifact::stats();
} /* stats() */

mapping query_dynamic_auto_load() {
  return ([
    "::" : armour::query_dynamic_auto_load(),
    "artifact" : artifact::query_dynamic_auto_load(),
  ]);
} /* query_dynamic_auto_load() */

mapping query_static_auto_load() {
  if ( explode( file_name( this_object() ), "#" )[ 0 ] != "/obj/ring" )
    return ([ ]);
  return ([
    "::" : int_query_static_auto_load(),
    "artifact" : artifact::query_static_auto_load(),
  ]);
} /* query_static_auto_load() */

void init_dynamic_arg( mapping map ) {
  if ( map[ "::" ] )
    armour::init_dynamic_arg( map[ "::" ] );
  if ( map[ "artifact" ] )
    artifact::init_dynamic_arg( map[ "artifact" ] );
} /* init_dynamic_arg() */

void init_static_arg( mapping map ) {
  if ( map[ "::" ] )
    armour::init_static_arg( map[ "::" ] );
  if ( map[ "artifact" ] )
    artifact::init_static_arg( map[ "artifact" ] );
} /* init_static_arg() */
