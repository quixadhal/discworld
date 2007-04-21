#include <config.h>
#define DOM_TITLE "the domain of admin"
#define LORD "pinkfish"
#define DOMAIN "admin"
/* PLEASE re-describe me */
inherit "/std/dom/cmn_mas";
object board;

void setup() {
  set_dom(DOMAIN);
  set_light(100);
  add_exit("drum", CONFIG_START_ROOM, "door");
  modify_exit( "drum", ({ "one way", 1 }) );
  set_short("Common room of "+DOM_TITLE);
  set_long("Large relaxing chairs adorn the room.  The walls are covered "+
           "with strange motifs from different lands and the little shelf "+
           "above the fireplace is chocker block full of strange figurines "+
           "and bits of cloth.  It appears to be the common room of "+
           DOM_TITLE+" the large red letters on the wall were the " +
           "give away.\n");
  add_alias("chairs", "chair");
  add_alias("cloths", "cloth");
  add_alias("motifs", "motif");
  add_alias("figurines", "figurine");
  add_item("chair", "The chairs are lazing around the room relaxing it looks "+
                    "like they come here after a hard days working in "+
                    DOM_TITLE+".\n");
  add_item("fireplace", "A nice little fire place with a cheery fire burning "+
                        "in it keeping every one warm.\n");
  add_item("figurine", "Small figurines, they look strangely familiar "+
                         "until you realise they are the members of "+
                         DOM_TITLE+".\n");
  add_item("cloth", "Strange coloured bits of cloth strewn over the "+
                    "mantlepice for no readily apparent reason.\n");
  add_item("motif", "The motifs on close inspection look like stylised "+
                     "signatures of all the members of "+DOM_TITLE+
                     ".  Some of them are very strange, in fact there "+
                     "seem to be more than there are members of the house.  "+
                     "perhaps it is the members of the future.\n");
  add_item("shelf", "A nice normal sort of shelf thing.  It is like all "+
                    "those ones you see in houses all over the place, "+
                    "execpt... the way it has been burnt... hmm it does "+
                    "not look like the fire did it.\n");
  add_item("fire", "There is a fire happily burning away in the fireplace "+
                   "spluttering and crackling to itself.  The flames almost "+
                   "seem afraid of something.  Looking closer you notice a "+
                   "picture tacked to the side of the fire place.\n");
  add_item("picture", "Tacked to the inside wall of the fire thingy, you "+
                      "know the hole bit at the bottom, is a small picture "+
                      "it looks like it was taken with the best of demon "+
                      "photography.  It is a picture of a person holding "+
                      "a small staff, you think it might be "+LORD+
                      " but you are not sure as the fire light sparkles "+
                      "in your eyes.\n");
  board = clone_object("/obj/misc/board");
  board->set_datafile(DOMAIN);
  board->move(this_object());
}
