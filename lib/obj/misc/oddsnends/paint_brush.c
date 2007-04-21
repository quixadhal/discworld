/*
Davyd
Item
For General Item Stores
*/

inherit "/std/object";

void setup(){
    set_name("brush");
    set_short("camel hair brush");
    add_adjective("hair");
    add_adjective("camel");
    add_adjective("paint");
    set_long("This is a small paint brush with fine camel hairs sticking "
             "out.  It looks to be better suited to the finer arts than for"
             " painting wagons with.\n");
    set_value(15);
    set_weight(15);
}