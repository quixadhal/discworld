inherit "/std/shop";

setup() {
  set_short("A nice little shop.\n");
  set_long("A small shop with lots of windows, It looks like it was \n"+
           "built on the side of a very steep hill.\n"+
           "Use \"buy\" to buy goods, \"sell\" to sell goods, \"list\""+
           "List the wares and \"browse\" to browse the items.\n");
  set_store_room("/w/pinkfish/store_room");
  set_light(100);
  add_exit("north", "/w/pinkfish/store_room", "door");
  add_exit("south", "/w/pinkfish/me_guild", "door");
}
