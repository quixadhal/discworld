// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {

    set_name("gong");
    set_short("large brass gong");
    add_adjective( ({ "large", "brass" }) );
    set_long("This is a large brass gong, which hangs from an equally "
        "large frame.  Tiny dragons writhe around each other on the "
        "surface of the disc, nipping each others' tails as they "
        "stretch.  The gong can be hit to make a loud ringing "
        "sound and annoy everyone in the room.\n");
    set_weight( 80 );
    set_value( 38000 );
    set_material( "brass" );
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
        "loud crashing ring.\n", ({ }));    return 1;

} /* do_hit() */

