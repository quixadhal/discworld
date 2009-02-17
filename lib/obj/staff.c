/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: staff.c,v 1.4 1999/05/13 20:57:18 rywfol Exp $
 * 
*/
#include <artifacts.h>
#include <tasks.h>

inherit "/obj/weapon";
inherit "/std/basic/artifact";

int query_magic_staff() { return 1; }

void create() {
  weapon::create();
  set_artifact_type( "staff" );
  set_no_limbs( 2 );
} /* create() */

void set_weight( int number ) {
  weapon::set_weight( number );
  new_weapon( 80 * number );
    add_attack( "thwack", 50, ({ weight / 3, 4, weight }), "blunt",
         "blunt", 0 );
    add_attack( "prod", 50, ({ weight / 2, 4, weight / 2 }), "blunt",
         "blunt", 0 );
} /* set_weight() */

string long( int word, int dark ) {
  return artifact::long( word, dark ) + weapon::long( word, dark );
} /* long() */

void init() {
  this_player()->add_command( "invoke", this_object() );
} /* init() */

int do_invoke() {
  int outcome;
  if ( query_wielded() != this_player() ) {
    this_player()->add_failed_mess( this_object(), "You must be holding "+
        "$D to $V it.\n", ({ }) );
    return 0;
  }

  outcome = (int)TASKER->perform_task(this_player(), STAFF_SKILL, level,
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
    write("%^YELLOW%^"+
          ({ "A piece of lore concerning staves becomes clearer for "+
             "you.\n", "You manage to grasp one of the principles of magic "+
             "staves more firmly.\n", "A sudden flash of insight allows you "+
             "to understand the staff a little better.\n" })[ random( 3 ) ] +
          "%^RESET%^");
    
  }
  charges--;
  if ( random( 100 ) < level / 10 ) {
    if ( !zapper->failed_zap( environment( this_player() ), this_player(),
        this_object() ) )
      this_player()->add_succeeded_mess( this_object(), "$N $V $D but "+
          "nothing happens!\n", ({ }) );
    return 1;
  }
  zapper->zap( environment( this_player() ), this_player(), this_object() );
  return 1;
} /* do_zap() */

mixed *stats() {
  return weapon::stats() + artifact::stats();
} /* stats() */

mapping query_dynamic_auto_load() {
  return ([
    "::" : weapon::query_dynamic_auto_load(),
    "artifact" : artifact::query_dynamic_auto_load(),
  ]);
} /* query_dynamic_auto_load() */

mapping query_static_auto_load() {
   if ( explode( file_name( this_object() ), "#" )[ 0 ] != "/obj/staff" )
      return ([ ]);
  return ([
    "::" : int_query_static_auto_load(),
    "artifact" : artifact::query_static_auto_load(),
  ]);
} /* query_static_auto_load() */

void init_dynamic_arg( mapping map ) {
  if ( map[ "::" ] )
    weapon::init_dynamic_arg( map[ "::" ] );
  if ( map[ "artifact" ] )
    artifact::init_dynamic_arg( map[ "artifact" ] );
} /* init_dynamic_arg() */

void init_static_arg( mapping map ) {
   if ( !mapp( map ) )
      return;
  if ( map[ "::" ] )
    weapon::init_static_arg( map[ "::" ] );
  if ( map[ "artifact" ] )
    artifact::init_static_arg( map[ "artifact" ] );
} /* init_static_arg() */
