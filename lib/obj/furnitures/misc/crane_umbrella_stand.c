//Una
//Only for use in Cwc

inherit "/std/room/furniture/storage";

void setup() {
    set_name("stand");
    set_short("lacquered umbrella stand");
    add_adjective( ({ "lacquered", "umbrella" }) );
    set_long("This is a conical umbrella stand that raises in an eight-sided "
        "cylinder.  Often placed by front doors to house the clothing "
        "accessories that are most-loathed, this particular stand is "
        "especially peculiar and eye-catching.  Lacquered with several "
        "shades of beige, green, and brown enamel, its faces boast an "
        "elegant crane taking off from a marshy landscape.\n");
    set_open();
    set_stuck(1);
    set_weight(90);
    set_max_weight(55);
    set_value(40000);
    set_allowed_room_verbs( ([ "squatting" : "squats",
                               "resting" : "rests",
                               "sitting" : "sits",
                               "standing" : "stands" ]) );
}


