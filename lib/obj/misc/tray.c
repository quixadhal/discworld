/**
 * A small tray, to give to vendors!
 * This tray weighs 250 grams and can hold up to 2 kilos worth of items!
 * Lets hear it for.. small wooden tray!
 * @author Terano.
 */

#include <move_failures.h>

inherit "/std/surface";
inherit "/std/basic/holdable";
inherit "/std/basic/wearable";

void create() {
  wearable::create();
  holdable::create();
  surface::create();
}

void setup() {	
  set_name( "tray" );
  set_short( "wooden tray" );
  set_long( "This is a small wooden tray, similar in style to the ones used by "
    "many vendors and travelling salespeople. Only a small quantity of items will "
    "fit on top of it.\n" );

  add_adjective(({ "small", "wooden" }));
  set_material("wood");

  //Surface stuff.
  set_weight(5);
  set_max_weight(40);

  //Hold stuff.
  set_no_limbs(2);
  
  //Wear stuff.
  set_type("sash");
  set_cond(1000);
  set_max_cond(1000);
  add_wear_effect( "/std/effects/attached/single_shoulder" );
}

string long( string word, int dark ) {
  return surface::long() + wearable::long();
}

varargs int move( mixed dir, string messin, string messout ) {
  if ( holdable::move( dir, messin, messout ) == MOVE_OK ) {
	  return surface::move( dir, messin, messout );
  }
  return MOVE_NO_UNHOLD;
}

mapping query_static_auto_load() {
mapping map = ([ ]);
  
  map["holdable"] = holdable::query_static_auto_load();
  map["surface"] = surface::query_static_auto_load();
  map["wearable"] = wearable::query_static_auto_load();
  return map;
}

void init_static_arg( mapping map ) {
  if (!undefinedp( map["holdable"] ) )
	holdable::init_static_arg( map["holdable"] );

  if ( !undefinedp( map["surface"] ) && map["surface"] )
	surface::init_static_arg( map["surface"] );

  if ( !undefinedp( map["wearable"] ) && map["wearable"] )
	surface::init_static_arg( map["wearable"] );

}

void dest_me() {
  holdable::dest_me();
  surface::dest_me();
}

mapping query_dynamic_auto_load() {
mapping map = ([ ]);
  
  map["holdable"] = holdable::query_dynamic_auto_load();
  map["surface"] = surface::query_dynamic_auto_load();
  map["wearable"] = wearable::query_dynamic_auto_load();
  return map;
}

varargs void init_dynamic_arg( mapping map, mixed new_arg ) {
  if (!undefinedp( map["holdable"] ) )
	holdable::init_dynamic_arg( map["holdable"] );

  if ( !undefinedp( map["surface"] ) && map["surface"] )
	surface::init_dynamic_arg( map["surface"] );

  if ( !undefinedp( map["wearable"] ) && map["wearable"] )
	surface::init_dynamic_arg( map["wearable"] );
}

mixed stats() { 
	return surface::stats() + wearable::stats();
}