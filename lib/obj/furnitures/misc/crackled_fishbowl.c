// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {

    set_name( "fishbowl" );
    set_short( "fishbowl with crackled blue enamel" );
    add_adjective( ({ "with", "crackled", "blue", "enamel" }) );
    add_alias("bowl");
    add_plural( ({ "bowls", "fishbowls" }) );
    set_main_plural("fishbowls with cracked blue enamel");
    set_long("Peace and serenity, the foundation for a good life considered "
        "in Agatean philosophy, are reflected in this 16-inch blue and "
        "white porcelain fishbowl.  A special pottery heating technique is "
        "applied in this product.  Looking closely one can see the small "
        "crackles throughout the entire fishbowl, which is more interesting "
        "to the eye than just a plain surface.  The interior is an "
        "underwater scene of gold fish swimming in circular along the edge "
        "of the bowl.  Resting on a beautiful hand-polished cherry wood "
        "base, the shapes of several goldfish can be seen swishing back and "
        "forth beneath the water it holds.\n" );
    set_weight( 120 );
    set_value( 46000 );
    set_allowed_positions(({"sitting"}));
    set_allowed_room_verbs(([ "sitting" : "sits",
                              "standing" : "stands",
                              "leaning" : "leans", 
                              "lying" : "lies",
                              "squatting" : "squats" ]));
} /* setup() */
