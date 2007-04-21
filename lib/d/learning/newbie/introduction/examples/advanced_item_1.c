/*
    This is a container,
    For containing fings.
    
    Wrytten by Drakkos Thee Creator.
    20/10/2000
*/

inherit "/obj/baggage";

void setup() {
    set_name("bag");
    set_short("little green bag");
    set_long("This is a little green bag.\n");
    add_adjective(({"little", "green"}));
    set_weight(5);
    set_max_weight(20);
}

int test_add(object ob, int flag) {
    if(this_player()->query_name() != "drakkos") {
        return 0;
    } 
    else {
        return ::test_add(ob, flag);
    }
}

int test_remove(object ob, int flag, mixed dest) {
    if(this_player()->query_name() != "drakkos") {
        return 0;
    } 
    else {
        return ::test_remove(ob, flag, dest);
    }
}