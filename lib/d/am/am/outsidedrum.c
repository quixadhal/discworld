#include "path.h"
inherit CITYROOM;
 
#define NUM 3

object *cabbages;
 
setup() { 
  set_light(100);
  set_short("Outside the Mended Drum");
  set_long("The Mended Drum is to the north.  Short Street goes off " +
           "to the south.  Filgree street heads east and west of here.\n");
  add_exit("south",SHORT+"short1", "corridor");
  add_exit("north",ROOM+"mendeddrum-foyer", "door");
  add_exit("east", ROOM+"filgree8", "corridor");
  add_exit("west", ROOM+"filgree7", "corridor");
    add_alias("mended drum", "pub");
    add_alias("drum", "pub");
  add_item("pub", "This is the Mended Drum.  It is the rebuilt 'Broken Drum' " +
            "that was destroyed in, and did in fact start, the Great fire " +
            "of Ankh Morpork.  It is world renowned for its bar brawls.\n");
  set_monster(NUM, "city");
  set_zone("anhk morpork");
}
void event_pub_brawl() {
  call_out("event_pub_brawl2", 0);
}

void event_pub_brawl2(object ob, object from) {
  int i;
  mixed *dest;

  dest = query_dest_dir();
  for (i=0;i<sizeof(dest);i++)
    if (objectp(dest[i]))
      event(dest[i], "pub_brawl", this_object());
    else if (dest[i] != ROOM+"mendeddrum-foyer" && find_object(dest[i]))
      event(find_object(dest[i]), "pub_brawl", this_object());
}

void init() {
  int i;
  object ob;

  ::init();
  if (this_player()->query_special_day("cabbage day")) {
    if (!cabbages)
      cabbages = allocate(10);
    for (i=0;i<sizeof(cabbages);i++)
      if (!cabbages[i]) {
        cabbages[i] = clone_object(CHAR+"wandering_cabbage");
        cabbages[i]->move(this_object());
      }
  }
} /* init() */
