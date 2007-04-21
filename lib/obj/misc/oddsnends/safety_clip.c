/*
Davyd
Item
For General Item Stores
*/

inherit "/std/object";

void setup(){
    set_name("clip");
    set_short("safety clip");
    add_adjective("safety");
    set_long("This is a high technology item.  "
             "It is designed to be used as a buckle, and "
             "will safeguard you from much harm if used "
             "properly.\n");
    set_value(25);
    set_weight(2);
}