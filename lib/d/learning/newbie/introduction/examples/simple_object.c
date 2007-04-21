/*
    This is a basic object!
    Written by Drakkos.
    
    04/10/2000
*/  

inherit "/obj/clothing";

void setup() {
    set_name("dungarees");
    set_short("pair of waterproof dungarees");
    add_adjective(({"pair of", "waterproof"}));
    set_long("This is a pair of waterproof dungarees.  Luckily they "
        "are also ooze proof.\n");
    add_adjective(({"pair of", "waterproof"}));
    set_weight(45);
    set_value(4000);
    setup_clothing(50000);
    set_type("robe");
}