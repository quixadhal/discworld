// Only for use within CWC - Una 07.02.02

inherit "/std/room/furniture/basic";

void setup() {
    
    set_name( "screen" );
    set_short( "latticed shoji screen" );
    add_adjective( ({ "latticed", "shoji" }) );
    set_long( "This double-sided shoji screen has a latticed "
        "wood pattern made of ramin hardwood.  A lovely honey "
        "oak colour, the panelling is broken up by rice paper "
        "dividers which checker the screen from the top all "
        "the way down to the solid kick panel which runs along "
        "the bottom.\n");
    set_allowed_positions( ({ "standing", "sitting" }) );
    set_allowed_room_verbs((["standing" : "stands", "sitting" : "sits" ]) );
    set_weight(20);
    set_value(400000);

} /* setup() */

