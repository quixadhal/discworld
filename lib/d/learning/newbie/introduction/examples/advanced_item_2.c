/*
    Flourescent Flared Trousers
   (Pink!)
    Wrytten by Drakkos Thee Creator
    21/10/2000
*/

inherit "/obj/clothing";

void setup() {
    set_name("trousers");
    set_short("pair of flourescent pink flared trousers");
    add_adjective(({"pair of", "flourescent", "pink", "flared"}));
    set_long("This is a pair of extremely stylish, extremely "
    "tasteful, flourescent pink flared trousers.\n");
    set_weight(10);
    set_value(0);
    set_type("trousers");
    add_alias("flares");
    set_main_plural("pairs of flourescent pink flared trousers");
    add_plural("trousers");
    setup_clothing(10000);
    add_pocket("left", 20);
    add_pocket("right", 20);
    set_wear_remove_func(base_name(this_object()), "do_wear_stuff");
}

void do_wear_stuff(object ob) {
    if(this_player()->query_name() != "drakkos") {
        if(!ob) {
            tell_object(this_player(), "You heave a sigh of relief as "
                "you remove the ghastly garment.\n");
        }
        else {
            tell_object(this_player(), "Ye gods, are you really going "
            "to wear those hideous things?!\n");
        }
    }
    else {
        if(!ob) {
            tell_object(this_player(), "Awww, why are you taking off your "
                "super-cool slacks?\n");
        }
        else {
            tell_object(this_player(), "You feel Real Cool as you slip "
                "into your fabulous flares!\n");
        }
    }
}