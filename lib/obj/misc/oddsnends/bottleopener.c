/*
Davyd
Item
For General Item Stores
*/

inherit "/std/object";

void setup(){
    set_name("opener");
    set_short("bottle opener");
    add_adjective("bottle");
    set_main_plural("bottle openers");
    set_long("This is a multipurpose bottle opener that "
             "works for left- and right-handed people.\n");
    set_value(15);
    set_weight(5);
}