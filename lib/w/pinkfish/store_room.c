inherit "/std/room";

setup() {
  set_short("Storeroom");
  set_long("A small store room that is stuffed full of completely\n"+
           "useless things, you think you would be lucky to find\n"+
           "anything you wnat in here.\n");
  set_light(100);
  add_exit("south","/w/pinkfish/shop", "door");
}
