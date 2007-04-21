// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {

    set_name("gong");
    set_short("silver gong");
    add_adjective( "silver" );
    set_long("This is a smallish silver gong, hanging from a silver "
        "frame.  Tiny goldfish swim around each other on the "
        "surface of the disc, nipping each others' tails as they "
        "go.  The gong can be hit to make a loud ringing "
        "sound and annoy everyone in the room.\n");
    set_weight( 40 );
    set_value( 22000 );
    set_material( "silver" );
    set_allowed_positions( ({"hanging"}) );
    set_allowed_room_verbs((["hanging" : "hangs",
                             "sitting" : "sits",
                             "dangling" : "dangles",
                             "standing" : "stands" ]));    
} /* setup() */

void init() {
    this_player()->add_command( "hit", this_object(), "<direct:object:me>" );
} /* init() */

int do_hit(){

    this_player()->add_succeeded_mess(this_object(), "$N $V $p "
        +this_object()->short()+" loudly, filling the room with a "
        "deafening ringing sound.\n", ({ }));    
    return 1;

} /* do_hit() */

