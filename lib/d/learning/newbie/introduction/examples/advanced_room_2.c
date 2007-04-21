/*
    This is a filler room!
    Written by Drakkos.
    
    15/10/2000
*/    

#include "path.h"

inherit "/std/outside";

void setup() {
    set_short("road to the market");
    add_property("determinate", "the ");
    set_day_long("This is a quiet road.  Absolutely nothing of interest is "
        "here..\n");
    set_night_long("This is a quiet road.  But at night!.\n");

    set_linker(({PATH + "advanced_room_1", PATH + "advanced_room_3"}),
        "onto", "on", "the newbie creator marketplace");
        
    set_light(80);
    add_zone("my rooms");

    add_item("road", "I *said*, there's nothing interesting at all here.");    
    
    add_exit("north", PATH + "advanced_room_1", "road");    

}
