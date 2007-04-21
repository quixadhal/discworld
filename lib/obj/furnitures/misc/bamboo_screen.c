// Only for use within CWC - Una 07.02.02

inherit "/std/room/furniture/basic";

void setup() {
    
    set_name( "screen" );
    set_short( "bamboo matchstick screen" );
    add_adjective( ({ "bamboo", "matchstick" }) );
    set_long( "This double-sided screen has been constructed by "
        "lashing long bamboo poles together so that "
        "four distinct interconnected panels are hinged "
        "together when it is standing.  A lovely tan colour, "
        "the texture is rough and the knots in the wood give it "
        "a rustic minimalist look.\n");
    set_allowed_positions( ({ "standing", "sitting" }) );
    set_allowed_room_verbs((["standing" : "stands", "sitting" : "sits" ]) );
    set_weight(20);
    set_value(400000);

} /* setup() */

