//Una
//Only for use in Cwc

inherit "/std/room/furniture/storage";

void setup() {
    set_name("cabinet");
    set_short("rosewood display cabinet");
    add_adjective( ({ "rosewood", "display" }) );
    set_long("This is a tall rosewood display cabinet that has a glass "
        "front to ensure that its contents are well protected.  Supported "
        "by various delicate geometric shelves on the inside, the outside "
        "is inlaid with mother of pearl that has been shaped into "
        "blossoming cherry trees.\n");
    set_transparent();
    set_weight(1200);
    set_max_weight(200);
    set_value(590000);
    set_allowed_room_verbs((["squatting" : "squats" ]));
    
}


