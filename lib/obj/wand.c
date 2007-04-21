/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: wand.c,v 1.6 2001/11/10 04:26:30 tannah Exp $
 * $Log: wand.c,v $
 * Revision 1.6  2001/11/10 04:26:30  tannah
 * Aquilo's new version wot deals with collective targets.
 *
 * Revision 1.5  2001/07/31 21:33:18  pinkfish
 * Add in some autodocs.
 *
 * Revision 1.4  2001/03/17 14:54:38  taffyd
 * Fixed add_command() patterns.
 *
 * Revision 1.3  1999/10/28 02:23:13  ceres
 * Can't remember
 *
 * Revision 1.1  1998/01/06 04:58:17  ceres
 * Initial revision
 * 
*/
/**
 * A magic wand object for use with all magic wands.
 */
#include <artifacts.h>
#include <tasks.h>

inherit "/obj/weapon";
inherit "/std/basic/artifact";

/**
 * This method will return true for all magic wands.
 * @return 1 for all wands
 */
int query_magic_wand() { return 1; }

void create() {
  weapon::create();
  set_artifact_type( "wand" );
} /* create() */

/** @ignore yes */
void set_weight( int number ) {
  weapon::set_weight( number );
  new_weapon( 80 * number );
   add_attack( "prod", 90, ({ 0, 5, weight }), "blunt", "blunt", 0 );
} /* set_weight() */

/** @ignore yes */
string long( int word, int dark ) {
  return artifact::long( word, dark ) + weapon::long( word, dark );
} /* long() */

void init() {
  this_player()->add_command( "zap", this_object(), "<direct:object> at <indirect:object>" );
  this_player()->add_command( "zap", this_object(), "<indirect:object> with <direct:object>" );
} /* init() */

/** @ignore yes */
int do_zap( object *things ) {
  int outcome;
  if ( query_wielded() != this_player() ) {
    this_player()->add_failed_mess( this_object(), "You must be holding "+
        "$D to $V it.\n", ({ }) );
    return 0;
  }
  if( ( sizeof( things ) > 1 ) || 
      ( sizeof( filter( things, (: $1->query_amount() > 1 :)) ) )
    ){
    this_player()->add_failed_mess( this_object(), "You can only zap $D "+
        "at one object at a time.\n", ({ }) );
    return 0;
  }
  if ( things[ 0 ] == this_object() ) {
    this_player()->add_failed_mess( this_object(), "You cannot zap $D at "+
        "itself!\n", ({ }) );
    return 0;
  }
/*
 * Knowing how to aim and fire the wand depends on the player's
 *     knowledge (rather than how well they can apply it), so the
 *     skill level determines ability to use it (rather than skill
 *     bonus).
 */
  outcome = (int)TASKER->perform_task(this_player(), WAND_SKILL, level,
                                      TM_FREE);
  if ( outcome == FAIL ) {
    this_player()->add_failed_mess( this_object(), "You failed "+
        "to $V $D.\n", ({ }) );
    return 0;
  }
  if ( !charges ) {
    this_player()->add_failed_mess( this_object(), "It appears that there "+
        "are no charges in $D.\n", ({ }) );
    return 0;
  }
   if ( outcome == AWARD ) {
      write( "%^YELLOW%^"+ ({ "A piece of lore concerning wands becomes "
            "clearer for you", "You manage to grasp one of the principles "
            "of magic wands more firmly", "A sudden flash of insight allows "
            "you to understand the wand a little better" })[ random( 3 ) ] +
            ".%^RESET%^\n" );
      
   }
  charges--;
  if ( random( 100 ) < level / 10 ) {
    if ( !zapper->failed_zap( things[ 0 ], this_player(), this_object() ) )
      this_player()->add_succeeded_mess( this_object(), "$N $V $D at $I but "+
          "nothing happens!\n", things );
    return 1;
  }
  zapper->zap( things[ 0 ], this_player(), this_object() );
  this_player()->add_succeeded( things[ 0 ] );
  return 1;
} /* do_zap() */

/** @ignore yes */
mixed *stats() {
  return weapon::stats() + artifact::stats();
} /* stats() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
  return ([
    "::" : weapon::query_dynamic_auto_load(),
    "artifact" : artifact::query_dynamic_auto_load(),
  ]);
} /* query_dynamic_auto_load() */

/** @ignore yes */
mapping query_static_auto_load() {
  if ( explode( file_name( this_object() ), "#" )[ 0 ] != "/obj/wand" )
    return ([ ]);
  return ([
    "::" : int_query_static_auto_load(),
    "artifact" : artifact::query_static_auto_load(),
  ]);
} /* query_static_auto_load() */

/** @ignore yes */
void init_dynamic_arg( mapping map ) {
  if ( map[ "::" ] )
    weapon::init_dynamic_arg( map[ "::" ] );
  if ( map[ "artifact" ] )
    artifact::init_dynamic_arg( map[ "artifact" ] );
} /* init_dynamic_arg() */

/** @ignore yes */
void init_static_arg( mapping map ) {
  if ( map[ "::" ] )
    weapon::init_static_arg( map[ "::" ] );
  if ( map[ "artifact" ] )
    artifact::init_static_arg( map[ "artifact" ] );
} /* init_static_arg() */
