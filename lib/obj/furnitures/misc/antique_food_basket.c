// Una
// For use in Cwc

inherit "/std/room/furniture/storage";

void setup(){

    set_name( "basket" );
    set_short( "antique food basket" );
    add_adjective( ({ "antique", "food", "basket" }) );
    set_long("Used for carrying food in the older times, now refurbished, "
        "this antique food basket is now a curious ornament to a "
        "contemporary eye.  Still usable for storage, if not for food, the "
        "handle is made of bamboo that has been bent into shape.  "
        "Retrieved from the Sum Dim countryside, no two of these baskets "
        "are alike.\n");
    set_weight( 60 );
    set_max_weight(38);
    set_value( 62000 );
    set_allowed_room_verbs(([ "sitting" : "sits",
                              "standing" : "stands",
                              "leaning" : "leans", 
                              "lying" : "lies",
                              "squatting" : "squats" ]));
} /* setup() */
