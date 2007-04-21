// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {

    set_name("gong");
    set_short("huge brass gong");
    add_adjective( ({ "huge", "brass" }) );
    set_long("This is a huge brass gong, which hangs from an equally "
        "huge frame.  Prancing peacocks strut around each other on the "
        "surface of the disc, ruffling their tails as they "
        "stretch.  The gong can be hit to make a loud ringing "
        "sound and annoy everyone in the room.\n");
    set_weight( 100 );
    set_value( 42000 );
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
        "deafening crashing ring which reverberates for a few "
        "moments before finally fading away.\n", ({ }));    return 1;

} /* do_hit() */

