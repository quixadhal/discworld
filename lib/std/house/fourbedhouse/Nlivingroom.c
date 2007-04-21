#include "path.h";

/* MAIN ROOM -- Used by the housing handler to determine the main room. */

inherit PLAYER_HOUSE;

void setup() {
   set_light(50);
   set_theft_handler( HOSPITAL );
   set_save_file("%savedir%/%N%livingroom");

/* DESCRIPTIONS */
   set_orientation(%orientation%);
  set_quit_handler(%out%);
   set_short("livingroom");
   set_base_desc("a large, comfortable, and sunny livingroom");
   add_surface("floor", "finished pine board" );
   add_surface("ceiling", "plain plaster" );
   add_surface("%0% wall", "plain plaster");
   add_surface("%2% wall", "plain plaster");
   add_surface("%4% wall", "plain plaster");
   add_surface("%6% wall", "plain plaster");
   
   /* EXITS */
   add_exit("out", %out%, "door");   
   modify_exit("out", ({"door short", "front door"}));
   add_exit("%7%", PATH+"%N%diningroom", "corridor");
   add_exit("%0%", PATH + "%N%kitchen", "corridor");
   add_exit("up", PATH + "%N%landing1", "corridor");
   modify_exit("up", ({"upgrade", 9}));
   modify_exit("out", ({"message", "$N leaves the house.",
                          "enter",
                          ({ 1,
                             "$N enters from a house.",
                             "$N enter from a house." }),
                          "closed", 1,
                          "door long", "This is the front door to a house.  "
                          "It has the numerals '%N%' in brass lettering.\n",
                          }) );
}

