/*
Davyd
Item
For General Item Stores
*/

inherit "/obj/container";

void setup(){
    set_name("frame");
    set_short("picture frame");
    add_adjective("picture");
    set_long("A lovely gold painted picture frame.\n");
    set_value(20);
    set_weight(30);
    set_max_weight(1);
    set_open();
    set_stuck(1);
}