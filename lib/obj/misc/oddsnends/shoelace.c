/*
Davyd
Item
For General Item Stores
*/

inherit "/std/object";

void setup(){
    set_name("lace");
    set_short("shoe lace");
    add_adjective("shoe");
    add_alias("shoelace");
    set_main_plural("shoe laces");
    add_plural("shoelaces");
    set_long("A bright yellow coloured shoelace, "
             "just the kind of thing a teenage kid would wear.\n");
    set_value(15);
    set_weight(2);
}