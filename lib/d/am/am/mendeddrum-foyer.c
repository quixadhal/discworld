inherit "std/room";
/* was room/pub.... but we dont have any pubs yet ;) */
#include "path.h"

#define HOSPITAL "d/am/hospital"

object detritus, stren, hrun;

void reset() {
  if(!detritus) {
    detritus = clone_object(CHAR+"detritus");
    detritus->move(this_object());
  }
  if(!stren) {
    stren = (object)HOSPITAL->get_monster("stren");
    stren->move(this_object());
  }
  if(!hrun) {
    hrun = (object)HOSPITAL->get_monster("hrun");
    hrun->move(this_object());
  }
}

void setup() {
  set_short("The Mended Drum");
  set_long(
"The room is covered in dirt soot and other less definable substances on the "+
"walls and anything else that looks good.  The floor itself is covered in "+
"rotting garbage of various kinds.  It is habited by the usual bunch of "+
"heroes, cut-throats, mercenaries, desperados and villains, and only "+
"microscopic analysis can tell them apart.  "+
"The streets of Ankh-Morpork are to the south.   Just to the north you can "+
"see the bar.\n");

  add_exit("south", ROOM+"outsidedrum", "door");
  add_exit("north", ROOM+"mendeddrum", "door");

  set_light(60);
  set_zone("pub");
  set_co_ord( ({ 0, 0, 0 }) );

  add_item("garbage",
           "Its trully amazing garbage.  Garbage of this kind does not "+
           "grow on trees, it requires carefull cultivation and nourishing "+
           "to reach the standard of here.  There is a define banana peel "+
           "look about some of it.\n");
  add_item(({ "hero", "cut throat", "mercenry", "desparado", "villan" }),
           "They are lots of them, all over the place.  They all look pretty "+
           "grubby, probably to tone into the furntiture.\n");
  add_item("table",
           "After some thought you think you can see the remains of a few "+
           "tables hidden in amongst the rest of the garbage.\n");
  add_item("chair",
           "Hmm,  yes you think that maybe that lump of garbage over there "+
           "is a chair.  No it moved.\n");
  add_item("floor",
           "You look at it for a while.  It is flat and very mucky, you "+
           "wonder briefly what diseases you will catch from this place.\n");
  add_item("wall",
           "They look like they were decorated by getting some muck and "+
           "carefully and meticously throwing it at them.  Very errr "+
           "well not at all pretty and sort of a sight you dont often "+
           "want to see.\n");
}
 
dest_me() {
  if (stren)
    stren->dest_me();
  if (detritus)
    detritus->dest_me();
  if (hrun)
    hrun->dest_me();
  ::dest_me();
}
void event_fight_in_progress() {
  int i;
  mixed *dest;

  dest = (mixed *)this_object()->query_dest_dir();
  for (i=0;i<sizeof(dest);i++)
    if (objectp(dest[i]))
      event(dest[i], "pub_brawl", this_object());
    else if (find_object(dest[i]))
      event(find_object(dest[i]), "pub_brawl", this_object());
}
