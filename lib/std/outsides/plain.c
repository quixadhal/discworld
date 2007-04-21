/*  -*- LPC -*-  */
/*
 * $Locker: ceres $
 * $Id: plain.c,v 1.7 2002/03/26 01:41:57 pinkfish Exp ceres $
 * 
 */
#define CREATOR "Ceres"
/**
 * This is a standard field room.  It is a specialisation of /std/outside
 * providing various ready-made add_items for your fields.
 * <p>
 * room_chats are also provided, if you don't wish to keep them simply use
 * room_chat to write your own or stop_room_chats to remove them.
 * <p>
 * The items and chats are seasonal adding richness to your field setting.
 * @author Ceres
 */
#include <weather.h>
#include <terrain_map.h>

inherit TERRAIN_MAP_OUTSIDE_BASE;

void create() {
  do_setup++;
  ::create();
  do_setup--;
  
  set_light(80);
  set_climate("Aw");
  add_zone("plain");
  
  switch((string)WEATHER->query_season()) {
  case "spring":
    add_item("flower", ({"long", "Some pretty flowers.",
                         "get", ({this_object(), "do_get_flower"}),
                         "pick", ({this_object(), "do_get_flower"}),
                         "smell", "The flowers smell wonderful."
                         }));
    add_item("tree", "There is the occasional tree here and there covered "
             "with new leaves at this time of year.");
    add_item(({"floor", "ground", "earth"}),
             "The ground is mostly grassy "
             "with the occasional clump of wild flowers.");
    
    room_chat(({120, 240, ({"Birds sing loudly announcing their presence.",
                              "Bees buzz busily from flower to flower.",
                              "A butterfly alights on a nearby flower.",
                              "A bird takes off from a tree and flies low "
                              "across the fields."})}));
    break;
  case "summer":
    add_item(({"floor", "ground", "earth"}), "The ground is mostly grassy.  "
               "The grass is long and sways gently in the breeze.  Here and "
               "there amongst the grass are wild flowers and other plants.");
    add_item("tree", "There are a few trees which are in their full "
             "foliage right now.");
    add_item(({"flower", "plant"}),
             ({"long", "Some pretty flowers.",
                 "get", ({this_object(), "do_get_flower"}),
                 "pick", ({this_object(), "do_get_flower"}),
                   "smell", "The flowers smell wonderful."
                     }));
    room_chat(({120, 240,
                  ({"The wind stirs the grass which sways reminding "
                      "you of waves on an ocean.",
                      "A small animal rustles in the long grass.",
                      "A butterfly alights on a nearby flower.",
                      "A bee buzzes around looking for nectar."})}));
    break;
  case "autumn":
    add_item(({"floor", "ground", "earth"}), "The ground is mostly grassy.");
    add_item("tree", "The occasional trees are gold and brown as their "
             "leaves die before winter.");
    
    room_chat(({120, 240, ({"A squirrel rushes up a nearby tree trunk.",
                              "A bird of prey soars high overhead looking "
                              "for something to eat."})}));
    break;
  case "winter":
    add_item(({"floor", "ground", "earth"}), "The ground is mostly grassy "+
               "apart from the occasional hardy shrub.");
    add_item("tree", "Dotted here and there are skeletal trees bare of "
             "leaves waiting for spring.");
    room_chat(({120, 240, ({"Somewhere a small animal forages in the "
                              "undergrowth for winter food."})}));
    break;
  }
  add_item("rock", ({ "long", "A nice rock looking thingy.",
                        "get",  ({ this_object(), "do_get_rock" }) }) );
  if (!do_setup) {
    this_object()->setup();
    this_object()->reset();
  }
}

/** @ignore yes */
mixed do_get_rock( string verb, object *dest ) {
  int rock_size;
  string rock_type;
  object rock;

  if ( query_property( "rock object" ) )
    return ( rock = clone_object( (string)query_property( "rock object" ) ) );
  if ( undefinedp( rock_size = (int)query_property( "rock size" ) ) )
    rock_size = 3;
  if ( !rock_size ) return 0;
  rock = clone_object( "/obj/mineral" );
  if ( undefinedp( rock_type = (string)query_property( "rock type" ) ) )
    rock_type = ({ "cinnabar", "coal", "rosy-quartz", "chalk", "flint",
                   "slate" })[ random( 6 ) ];
  rock->make_mineral( rock_type, 1 + random( rock_size ) );
  if ( !( rock->move( this_player() ) ) ) return 1;
  rock->dest_me();
  return 0;
}

/** @ignore yes */
mixed do_get_flower( string verb, object *dest ) {
  string flower_type;
  object flower;

  if ( query_property( "flower object" ) )
    return (flower = clone_object((string)query_property("flower object")));
  if ((int)query_property("noflowers"))
    return 0;
  flower = clone_object("/std/plant");
  
  if(undefinedp(flower_type = (string)query_property("flower type")))
    flower_type = ({"forgetmenot", "buttercup", "daisy"})[ random(2) ];
  flower->set_plant(flower_type);
  flower->set_plant_type(flower_type);
  flower->set_plant_desc("A beautiful "+flower_type+" picked before its "
                         "time.");
  flower->add_plural( pluralize( flower_type ) );
  
  if (!(flower->move(this_player()))) return 1;
  flower->dest_me();
  return 0;
}
