#define CREATOR "Rue"
// Rue, 5.2000 - lava lamp; special effects by Tannah

inherit "/std/room/furniture/basic";
#define MEDITATION "/std/effects/other/meditate_effect"

void setup() {
    set_name("lamp");
    set_short("lava lamp");
    set_light(25);
    add_adjective( ({ "lava", "red" }) );
    add_alias( ({ "lamp" }) );
    set_long( "This is an odd looking lamp, roughly the shape of an "
        "hourglass, with a brass base.  It is filled with murky pink "
        "liquid and large blobs of red goo that ooze this way and that.  "
        "The exact nature of the red goo is a mystery.  Some say it is "
        "a primitive type of troll that is still in its magma stage.  "
        "Others reckon it is something that got away from the Alchemists' "
        "Guild.  It gives off a faint light.\n" );
  
    item_chat( ({ 120, 360, ({
        "A rather disgusting wet popping noise comes from the lava lamp.",
        "A strange sucking noise seems to be coming from the lava lamp."
  
    }) }) );
  
    set_allowed_positions(({"sitting","standing"}));
    set_allowed_room_verbs((["sitting" : "sits", "standing" : "stands" ]));
    set_weight(82);
    set_value(20500);
    
} /* setup() */

void init() {
    ::init();
    add_command( "stare", "at <direct:object'lamp'>" );
} /* init() */

int do_stare() {
    call_out( (: $(this_player())->add_effect( MEDITATION ) :), 1 );
    add_succeeded_mess( "$N $V dreamily at $D and zone$s out.\n");
    return 1;
} /* do_stare() */

