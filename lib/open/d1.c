#include "path.h"
inherit "/std/room";
 
object *mons;
int got_torch;
 
void add_monster();

void setup() {
  set_light(40);
/* 70 is a lot too bright for a dimly lit corridor. */
  set_short("dimly lit corridor");
  set_long("As you look around you realise that these dungeons are not the "+
          "nicest places in the world. The walls seem covered in a slick of "+
          "slimey fungus. If you wait around you will surely bump into some "+
          "of the notorious Patrician's Guards. There are torches on the "+
          "walls lighting your way.\n");
  add_exit("up", "/d/am/patrician/patrician3.c", "corridor");
  add_exit("south", ROOM+"d2", "corridor");
  add_item("torch","Funny that, it really looks like a torch.\n");
  add_item("fungus","Really slimey fungus.\n");
  mons = allocate(2);
}

void init() {
  ::init();
  add_action("get_fn", "get");
}

int get_fn(string str) {
  object tor;
  if (got_torch) return 0;
  if(str != "torch") return 0;
  if(str=="torch") {
    tor = clone_object("/w/elwood/misc/torch");
    if (tor->move(this_player())) {
/* Can they pick it up?  If it gets here they failed */
      return 0;
    }
    write("You get a torch from the wall. It feels quite heavy and it is "+
          "nearly all used up.\n");
/* Remove some light now the torch has gone. */
    adjust_light(-30);
    got_torch = 1;
    return 1;
  }
}

void reset() {
  int i;

  for (i=0;i<sizeof(mons);i++) {
/*
 * Ok.  We test each position in the array to see if there is already a monster
 * in it.  If a monster exists, we don't clone a new one.  If not
 * we clone a new one.
 *
 * Ok, it is a global variable and not a local one so we can keep track of
 * these monsters over resets.
 */
    if (!mons[i]) { /* Thats what this test does. */
      mons[i] = clone_object(MONS+"pguard");
      mons[i]->move(this_object());
    }
  }
  if (got_torch) {
/* Some one took a torch! */
/* Make the light correct */
    adjust_light(30);
    got_torch = 0;
  }
}
