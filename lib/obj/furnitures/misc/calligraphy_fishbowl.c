// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {

    set_name( "fishbowl" );
    set_short( "calligraphised fishbowl" );
    add_adjective( ({ "calligraphised", "fish" }) );
    add_alias("bowl");
    add_plural( ({ "bowls", "fishbowls" }) );
    set_long("Calligraphy is the soul of Agatean culture.  On this 16-inch "
        "fishbowl a Zing Dynasty poem is strikingly carved in "
        "and then gold leafed against an equally striking black background.  "
        "The black lacquer is hand-painted over porcelain to create a matte "
        "effect.  A matching solid wood stand holds the bowl's weight, "
        "and several goldfish swim quietly beneath the water it holds.\n" );
    set_weight( 110 );
    set_value( 48000 );
    set_allowed_positions(({"sitting"}));
    set_allowed_room_verbs(([ "sitting" : "sits",
                              "standing" : "stands",
                              "leaning" : "leans", 
                              "lying" : "lies",
                              "squatting" : "squats" ]));
} /* setup() */
