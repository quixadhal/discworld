// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {
    
    set_name( "screen" );
    set_short( "rice paper samurai screen" );
    add_adjective( ({ "rice", "paper", "samurai" }) );
    set_long("This is a simple screen that would fit in most traditional "
        "homes.  A stoic samurai in full traditional Agatean armour and "
        "dress storms across the panelling, hand-drawn in shades of orange, "
        "blue, brown, cream and black.  The frame is made "
        "of a strange combination of walnut and spruce woods, their "
        "differing colours blending well with the shades of the panels.\n");
    set_allowed_positions( ({ "standing", "sitting" }) );
    set_allowed_room_verbs((["standing" : "stands", "sitting" : "sits" ]) );
    set_weight(150);
    set_value(329000);

} /* setup() */
