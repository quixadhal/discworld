/*  -*- LPC -*-  */
/*
 * $Locker: ceres $
 * $Id: woodland.c,v 1.8 2003/04/16 23:12:26 ceres Exp ceres $
 */
#define CREATOR "Ceres"
/**
 * This is a standard woodland room.  It is a specialisation of /std/outside
 * providing various ready-made add_items for your woodland.
 * <p>
 * room_chats are also provided, if you don't wish to keep them simply use
 * room_chat to write your own or stop_room_chats to remove them.
 * <p>
 * The items and chats are seasonal adding richness to your woodland setting.
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
  add_zone("woodland");
  
  switch((string)WEATHER->query_season()) {
  case "spring":
    add_item("flower", ({"long", "Some pretty flowers.",
                         "get", ({this_object(), "do_get_flower"}),
                         "pick", ({this_object(), "do_get_flower"}),
                         "smell", "The flowers smell wonderful.\n"
                         }));
    add_item("tree", "There are trees everywhere here, you have "+
             "to pick your way between them.  They are budding with "+
             "new leaves at this time of year.");
    add_item(({"floor", "ground", "earth"}), "The ground is covered "+
             "by a beautiful carpet of flowers interspersed with small "+
             "shrubs.");
    add_item("shrub", "Dotted here and there among the carpet of flowers "+
             "are small shrubs, some thorny, all green.");

    room_chat(({120, 240, ({"Birds sing loudly announcing their presence.",
                            "Bees buzz busily from flower to flower.",
                            "A squirrel rushes up a nearby tree trunk.",
                            "A butterfly alights on a nearby flower.",
                            "A bird takes off from a tree with a furious "+
                              "beating of wings."})}));
    add_property("flower type", "bluebell");
    break;
  case "summer":
    add_item(({"floor", "ground", "earth"}), "You can barely see the "+
             "ground because of the waist-high ferns.  Dotted among the "+
             "ferns are small shrubs.");
    add_item("shrub", "Dotted here and there among the ferns "+
             "are small shrubs, some thorny, all green.");
    add_item("fern", "Everywhere you look there are ferns growing "+
                         "almost waist high between the trees.");
    add_item("tree", "The trees are in their full foliage right now.");
    room_chat(({120, 240, ({"The wind stirs the leaves of the trees making "+
                              "a sound like falling rain.",
                            "A squirrel rushes up a nearby tree trunk.",
                            "A butterfly alights on a nearby flower.",
                            "A bird takes off from a tree with a furious "+
                              "beating of wings."})}));
    break;
  case "autumn":
    add_item(({"floor", "ground", "earth"}), "The ground is littered "+
             "with dead leaves and the stems of dying ferns.");
    add_item("tree", "You are surrounded by trees, they are resplendent "+
             "with their autumn colours of red, gold and brown.");
    add_item("shrub", "Dotted here and there among the ferns "+
             "are small shrubs many with brightly coloured berries.");
    add_item(({"berry", "berries"}),
               ({"long", "There are a variety of berries hanging from the "+
                   "shrubs here.",
                 "get", ({this_object(), "do_get_berry"}),
                 "pick", ({this_object(), "do_get_berry"})}));
    add_item(({"leaf", "leaves"}),
             ({"long", "The ground is littered with dead leaves.",
               "get", ({this_object(), "do_get_leaf"})}));
    add_item("fern", "Here and there are the stems of ferns which have "+
             "died back and will survive the coming winter underground.");
    room_chat(({120, 240, ({"The wind stirs the leaves of the trees causing "+
                              "some to fall gently to earth.",
                            "A squirrel rushes up a nearby tree trunk.",
                            "A bird takes off from a tree with a furious "+
                              "beating of wings."})}));
    add_extra_look(this_object());
    break;
  case "winter":
    add_item(({"floor", "ground", "earth"}), "The ground is bare earth "+
             "apart from the occasional hardy shrub.");
    add_item("shrub", "Dotted here and there among the trees are small "+
             "shrubs trying to survive the winter.");
    add_item("tree", "There are bare trees everywhere here waiting for the "+
             "coming of spring.");
    room_chat(({120, 240, ({"The wind stirs the dry branches of the trees "+
                              "causing them to rustle eerily.",
                            "Somewhere a small animal forages in the "+
                              "undergrowth for winter food."})}));
    break;
  }
  add_item("rock", ({ "long", "A nice rock looking thingy.",
                      "get",  ({ this_object(), "do_get_rock" }) }) );
  add_property("track type", "woodland");
  if(!do_setup) {
    this_object()->setup();
    this_object()->reset();
  }
}

string query_terrain_map_colour() { return "%^GREEN%^"; }

string extra_look() {
  string extra;

  extra = ::extra_look();
  switch((string)WEATHER->query_season()) {
  case "spring":
    return extra + "Spring flowers are everywhere you look.\n";
  case "autumn":
    return extra + "The ground is littered with fallen leaves.\n";
    break;
  default:
    return extra;
  }
}

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

mixed do_get_flower( string verb, object *dest ) {
  string flower_type;
  object flower;
  if ( query_property( "flower object" ) )
    return (flower = clone_object((string)query_property("flower object")));
  if ((int)query_property("noflowers"))
    return 0;
  flower = clone_object("/std/plant");
  
  if(undefinedp(flower_type = (string)query_property("flower type")))
    flower_type = ({"forgetmenot", "violet"})[ random(2) ];
  flower->set_plant(flower_type);
  flower->set_plant_type(flower_type);
  flower->set_plant_desc("A beautiful "+flower_type+" picked before its "+
                         "time.\n");
  flower->set_main_plural(flower_type + "s");
  
  if (!(flower->move(this_player()))) return 1;
  flower->dest_me();
  return 0;
}

mixed do_get_berry( string verb, object *dest ) {
  object berry;
  string berry_type;
  if ((int)query_property("noflowers"))
    return 0;
  
  berry_type = ({"blackberry", "raspberry", "blueberry", "strawberry",
                 "blackcurrant"})[random(5)];
  
   berry = clone_object( "/obj/food" );
  berry->set_name(berry_type);
  berry->set_short(berry_type);
  berry->set_long("This is a lovely, plump "+berry_type+" it looks good "+
                  "enough to eat.\n");
  berry->add_alias("berry");
  
  if (!(berry->move(this_player()))) return 1;
  berry->dest_me();
  return 0;
}

mixed do_get_leaf( string verb, object *dest ) {
  object leaf;
  string leaf_type;
  if ((int)query_property("noflowers"))
    return 0;
  
  leaf_type = ({"maple", "oak", "sycamore", "ash", "silver birch"})[random(5)];
  
  leaf = clone_object("/std/object");
  leaf->set_name("leaf");
  leaf->set_short(leaf_type+" leaf");
  leaf->set_long("This is a dead "+leaf_type+" leaf.  Its natural green "+
                 "has turned to shades of gold, red and brown.\n");
  leaf->add_adjective(leaf_type);
  if (!(leaf->move(this_player()))) return 1;
  leaf->dest_me();
  return 0;
}


