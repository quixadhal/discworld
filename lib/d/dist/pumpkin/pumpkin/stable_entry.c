#include "path.h"
#include <armoury.h>

inherit "/std/room/stable";

void setup() {
   set_light(60);
   set_theft_handler( HOSPITAL );
   set_short( "stables" );
   add_property("determinate", "the ");
   set_long( "These are the Pumpkin Street stables, an ornate gate to the "+
      "north opening onto the street.  There are many stalls here, "+
           "and some of them even have horses in them. Against one wall "+
           "is a pile of hay and hanging on the walls are saddles and "+
           "other horse riding equipment.\n");
 
   add_exit( "east", PATH +"pumpkin4", "gate" );
 
   add_item(({"horse", "horses"}), "There are a few horses here, but "+
              "most of them seem to be in the carthorse mold, rather "+
              "than being fine chargers.\n");
   add_item(({"hay", "pile"}), "A big pile of hay (that's dead grass to you and me).\n");
   add_item(({"saddle", "saddles", "riding equipment", "equipment"}),
              "Hanging on one wall are saddles, bridles, bits and other "+
              "things that help you ride a horse.\n");
   add_item("stalls", "Boxes where horses live!\n");
   add_item("gate", "A large gate, made of wrought iron bent into intricate "+
                    "shapes. On the other side you can see Filigree Street.\n");
   add_item(({"shape", "shapes"}), "There are many shapes and patterns "+
              "to be found in the twists and turns of the iron gates, "+
              "but you can't find any meaning in them. Maybe it's "+
              "meant to be abstract?\n");
 
   room_chat( ({ 50, 50, ({
 		  "A horseman rides through the gates and dismounts. "+
             "A stable boy rushes forward and leads the horse to a stall "+
             "while the rider leaves.",
             "One of the horses rears up in its stall, but can't "+
             "force the door open."
   }) }) );
 
   set_zone("pumpkin stables");

   set_save_directory("/d/dist/pumpkin/save/stables/");
   add_stable_type("horse");
   add_property("place", "Pumpkin");
} /* setup() */
