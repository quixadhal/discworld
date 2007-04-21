// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {

    set_name("gong");
    set_short("brass gong");
    add_adjective( "brass" );
    set_long("This is a medium-sized brass gong, hanging from a brass "
        "frame.  Lazy pandas roll around the surface of the gong, "
        "the occasional one actually doing something other than "
        "eating or chewing on a stalk of bamboo.  The gong can be hit "
        "to make a loud ringing sound and annoy everyone in the room.\n");
    set_weight( 60 );
    set_value( 10000 );
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
        "deafening ringing sound.\n", ({ }));    
    return 1;

} /* do_hit() */

