// Una
// Only for use in Cwc

inherit "/std/room/furniture/fireplace";

void setup() {

    set_name("oven");
    set_short("large wood oven");
    add_adjective( ({ "large", "wood" }) );
    set_long("A tastefully decorated door bears the name 'Wun Chi Puffin' "
        "and a temperature gauge monitors the large wood oven.  "
        "Resting on a smooth base, this range has a thirty-two inch cook "
        "surface and carries six burners and a water reservoir.  A top "
        "mounted nickel trimmed shelf on the back of the stove is "
        "complemented by dual warming shelves.  Subtle and sophisticated, "
        "this stovetop would compliment most tasteful homes.\n");
    set_allowed_room_verbs(([
                            "sitting" : "sits", 
                            "squatting" : "squats",
                            "crouching" : "crouches",
                            "resting" : "rests",
                            "lying" : "lies"
                          ]));
    set_weight(495);
    set_max_weight(100);
    set_value(120000);
  
} /* setup() */
