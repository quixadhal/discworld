// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup(){

    set_name( "fishbowl" );
    set_short( "gold-leafed garden fishbowl" );
    add_adjective( ({ "gold", "leafed", "leaf", "gold-leaf", "gold-leafed", 
        "garden" }) );
    add_alias("bowl");
    add_plural( ({ "bowls", "fishbowls" }) );
    set_long("Reading a story that is visualized on an art object is a "
        "particular kind of enjoyment.  A scene from the Agatean classic, "
        "'The Dream of Red Chamber' is hand-painted on this sixteen-inch "
        "fishbowl, in which the heroine expresses forbidden love in her "
        "poetry.  In this scene one also has a glimpse of traditional "
        "Agatean house garden.  The colorful scenery is painted over "
        "multi-layered gold leaf which has been carefully applied by hand.  "
        "Resting on a beautiful hand-polished rosewood base, "
        "the shapes of several goldfish can be seen swishing back and "
        "forth beneath the water it holds.\n" );
    set_weight( 110 );
    set_value( 52000 );
    set_allowed_positions(({"sitting"}));
    set_allowed_room_verbs(([ "sitting" : "sits",
                              "standing" : "stands",
                              "leaning" : "leans", 
                              "lying" : "lies",
                              "squatting" : "squats" ]));
} /* setup() */
