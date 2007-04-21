#include "path.h"
#include <armoury.h>

inherit "/std/shops/storeroom";

nosave string *weapons = ({"dagger", "knife", "small knife", "short sword",
   "foil", "rapier", "cudgel", "crowbar", "knobbly mace","long sword",
   "chain", "morning star", "axe", "two-handed axe"});

nosave string *armours = ({"hard leather cap", "heavy cotton breeches",
   "ringmail", "leather breeches", "bone helm", 
   "brass breastplate", "leather breastplate", "bone mail",
   "bone shield", "medium wooden shield", "small metal shield",
   "hard leather boots", "metal clad boots", "leather gloves",
   "metal gauntlets", "metal helm", "iron breastplate"});

nosave string *clothes = ({"brown linen shirt", "white linen skirt",
   "white linen tunic", "black cotton dress", "white cotton robe",
   "black leather shoes", "soft leather shoes", "grey worsted trousers",
   "fawn cotton trousers", "discreet black trousers", "black wool socks",
   "socks", "black leather shoes", "soft leather shoes", "green cloak",
   "red-brown tunic", "nightshirt", "thermal underwear", "lacy underwear",
   "jester costume", "wide brimmed hat", "soft leather gloves"});

void setup() {
  set_short( "storeroom" );
  add_property( "determinate", "the " );
  set_long( "This is a storeroom.\n" );
  set_light( 100 );
} /* setup() */

void reset() { call_out( "restock", 0 ); }

void restock() {
  int i, size;
  object thing;
  object new_item;

  i = sizeof( match_objects_for_existence( "buckets", this_object() ) );
  for ( ; i < 3; i ++ ) {
    thing = clone_object( "/obj/vessel" );
    if (thing) {
      size = random( 3 );
      thing->set_leak_rate( 0 );
      thing->set_max_volume( 1000 + size * 2000 );
      thing->set_max_weight( 20 + size * 40 );
      thing->set_full_weight( 20 + size * 40 );
      thing->set_stuck( 1 );
      thing->set_name( "bucket" );
      thing->set_short( ({ "small", "medium", "large" })[ size ]
                        +" bucket" );
      thing->set_main_plural( ({ "small", "medium", "large" })[ size ]
                              +" buckets" );
      thing->add_adjective( ({ "small", "medium", "large" })[ size ] );
      thing->set_value( 60 + size * 25 );
      thing->set_long( "This is a metal thing.  It is not very shiny.\n" );
      thing->set_weight( 10 + size * 10 );
      thing->move( this_object() );
    }
  }

  i = sizeof( match_objects_for_existence( "torches", this_object() ) );
  for ( ; i < 6; i++ ) {
    new_item = ( ARMOURY->request_item("torch", 100) );
    if (new_item) new_item->move( this_object() );
  }
  
  
  for(i=0; i<30; i++) {
    new_item = ( ARMOURY->request_weapon( weapons[random(sizeof(weapons))],
                   95 ) );
    if (new_item) new_item->move( this_object() );
  }

  for(i=0; i<25; i++) {
    new_item = ( ARMOURY->request_armour( armours[random(sizeof(armours))],
                   95 ) );
    if (new_item) new_item->move( this_object() );
  }
  
  for(i=0; i<30; i++) {
    new_item = ( ARMOURY->request_clothing( clothes[random(sizeof(clothes))],
                   95 ) );
    if (new_item) new_item->move( this_object() );
  }     
} /* restock() */

