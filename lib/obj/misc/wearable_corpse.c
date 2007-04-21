/**
 * Wearable corpse for wearable pets!
 * 
 * To use this, you need to put a make_corpse function in your pet that
 * clones this object and sets it up as per a standard corpse.
 *
 * You should also call: set_type, set_max_cond and set_cond on it!
 *a
 * @author Terano
 * @started 24/11/02
 */

inherit "/obj/corpse";
inherit "/std/basic/wearable";

void create() {
  corpse::create();
  wearable::create();
}

string long( string word, int dark ) {
  return corpse::long( word, dark ) + wearable::long( word, dark );
}

void init_static_arg( mapping map ) {
   if ( map[ "corpse" ] )
      corpse::init_static_arg( map[ "corpse" ] );
   if ( map[ "wearable" ] )
      corpse::init_static_arg( map[ "wearable" ] );
}

void init_dynamic_arg( mapping map ) {
 if ( !undefinedp( map[ "corpse" ] ) ) 
      corpse::init_dynamic_arg( map[ "corpse" ] );

 if ( !undefinedp( map[ "wearable" ] ) ) 
      wearable::init_dynamic_arg( map[ "wearable" ] );
}

mapping query_static_auto_load() {
   return ([
      "corpse" : corpse::query_static_auto_load(),
      "wearable" : wearable::query_static_auto_load(),
   ]);
}

mapping query_dynamic_auto_load() {
   return ([
      "corpse" : corpse::query_dynamic_auto_load(),
      "wearable" : wearable::query_dynamic_auto_load()
   ]);
}

mixed *stats() {
	return corpse::stats() + wearable::stats();
}
