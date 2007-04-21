/*
    This is a basic room!
    Written by Drakkos.
    
    29/09/2000
*/  

#include "path.h"

inherit "/std/room";

void setup() {
    set_short("blobby lair");
    set_long("This is where the grey blob lives.  All around lie "
        "frogs, and wombles, and strange oozy things.  It's a "
        "very nice lair, as lairs go.\n");
    add_property("determinate", "a ");
    set_light(50);

    add_item(({"frog", "pinkfish", "toad"}),({"long", "The frogs are very "
        "nice.  Very froggy.", "position", "one of the poor leetle "
        "frogs."}));
    add_item(({"womble", "uncle bulgaria"}),({"long", "It's Uncle "
        "Bulgaria!", "pet", "You pet Uncle Bulgaria.  He growls and "
        "chews the nails off your hand.\n", "snuggle", "Uncle Bulgaria gnaws "
        "on your teeth.\n"}));    
    add_item("strange oozy things", "Ewww!");
    
    add_zone("my rooms");
    
    add_sign("This is a nice sign.\n", "Do Notte Feed Thee Blob!",
        "nice sign", "sign", "common");
    
    room_chat(({120,240,({
        "A thick blob of goo oozes over one of the frogs.",
        "The womble bings quietly.",
        "The frogs ribbit in abstract contemplation.",
    })}));    

    add_exit("east", PATH + "advanced_room_1", "road");

}        

void reset() {
    call_out("after_reset", 3);
}

void after_reset() {

    object ob = find_object
        (PATH + "simple_npc");    

    // if(!ob) will return true if ob is 0... in other words, it
    // didn't find an object with find_object().
    
    if(!ob) {
    
        // There's no object with that filename loaded, so we load it
        // and then move it into this_object()... which in the case of
        // this example, is the room we just coded.  
        
        ob=load_object(PATH + "simple_npc");
        
        // For information on how the move() function works, you 
        // can check out 'help move'.  But briefly, the first argument
        // is where the object is to be moved to... the second is the
        // message that objects in the destination get when the object
        // enters.  $N will be replaced with the short of the object.
        // 'appear$s' is a pluralisation code... it will change to 
        // 'appear' when more than one object enters at the same time,
        // and 'appears' when only one enters.
        
        ob->move(this_object(), "$N appear$s with a wet squelch.\n");
    }
    
    // if(!environment(ob)) will return true if ob returns 0 for 
    // environment... in other words, it's loaded, but located in 
    // null-space.
    
    else if(!environment(ob)) {
        ob->move(this_object(), "$N appear$s with a wet squelch.\n");
    }
        
}  