/*
Davyd
Item
For General Item Stores
*/

inherit "/std/object";

void setup(){
    set_name("brush");
    set_short("stiff hair brush");
    add_adjective("hair");
    add_adjective("stiff");
    set_long("This is a small brush with rather stiff tines "
             "sticking out from it.  It looks like just the thing to "
             "straighten out your hair after a sandstorm.\n");
    set_value(10);
    set_weight(20);
}