/*
Davyd
Item
For General Item Stores
*/

#include <volumes.h>

inherit "/obj/vessel";

void setup(){
    set_name("glass");
    set_short("glass");
    set_long("This is a limited edition authentic "
             "scumble measure glass, as produced by the "
             "ancient scumble brewers.  It is very exclusive.\n");
    set_max_volume(VOLUME_HALFPINT);
    set_max_weight(10);
    set_leak_rate(0);
    set_value(30);
    set_weight(5);
    set_stuck(1);
}