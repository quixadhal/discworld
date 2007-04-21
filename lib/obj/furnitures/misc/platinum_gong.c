// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {

    set_name("gong");
    set_short("platinum gong");
    add_adjective( "platinum" );
    set_long("This is a smallish platinum gong, hanging from a platinum "
        "frame.  Leaping lions writhe around each other on the "
        "surface of the disc, showing their teeth as they "
        "run across the etchings.  The gong can be hit to make a "
        "loud ringing sound and annoy everyone in the room.\n");
    set_weight( 30 );
    set_value( 28000 );
    set_material( "platinum" );
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

