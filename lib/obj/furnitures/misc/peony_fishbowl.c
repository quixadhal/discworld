// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {

    set_name( "fishbowl" );
    set_short( "gold-leafed peony fishbowl" );
    add_adjective( ({ "gold", "leafed", "leaf", "gold-leaf", "gold-leafed", 
        "peony" }) );
    add_plural( ({ "bowls", "fishbowls" }) );
    add_alias("bowl");
    set_long( "Richly designed with blooming peonies, the flower that "
        "symbolizes peace and prosperity in Agatean culture, this grand "
        "16-inch fishbowl is carefully hand-layered with gold leaf over "
        "porcelain and hand-painted in exuberant colors.  This fishbowl "
        "matches any kind of foliage or simply stands alone as an art "
        "object.  Resting on a beautiful hand-polished rosewood base, "
        "the shapes of several goldfish can be seen swishing back and "
        "forth beneath the water it holds.\n" );
  set_weight( 110 );
  set_value( 51000 );
  set_allowed_positions(({"sitting"}));
  set_allowed_room_verbs((["sitting" : "sits",
                           "standing" : "stands",
                           "leaning" : "leans", 
                           "lying" : "lies",
                           "squatting" : "squats" ]));
} /* setup() */
