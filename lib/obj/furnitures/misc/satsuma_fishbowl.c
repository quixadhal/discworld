// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {

    set_name( "fishbowl" );
    set_short( "satsuma fishbowl" );
    add_adjective( ({ "satsuma" }) );
    add_alias("bowl");
    add_plural( ({ "bowls", "fishbowls" }) );
    set_long("Learning is regarded the highest virtue in Agatean "
        "culture.  The picture on this "
        "fishbowl depicts a group of scholars in a session of discussion.  "
        "This fishbowl is made of porcelain and hand-painted with dark-green "
        "lacquer.  On top of the lacquer gold threads are woven in "
        "decorative design.  One intriguing detail about this particular  "
        "fishbowl is that thousands of tiny dots are meticulously painted "
        "one by one over the entire 16-inch fishbowl.  The interior is an "
        "underwater scene of gold fish swimming in circular along the edge "
        "of the bowl.  Resting on a beautiful hand-polished mahogany base, "
        "the shapes of several goldfish can be seen swishing back and "
        "forth beneath the water it holds.\n" );
    set_weight( 110 );
    set_value( 56000 );
    set_allowed_positions(({"sitting"}));
    set_allowed_room_verbs(([ "sitting" : "sits",
                              "standing" : "stands",
                              "leaning" : "leans", 
                              "lying" : "lies",
                              "squatting" : "squats" ]));
} /* setup() */
