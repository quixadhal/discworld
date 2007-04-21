#include "path.h"
inherit "/std/room";

object npc;


void setup() {
    
    set_light(70);
    set_short( "variable NPC room" );
    add_property( "determinate", "the " );
    set_long( "This room clones an NPC from a single file.  However, the NPC"
    " has built in variety.  Its description changes randomly.  Typing "
    "update will bring a new version of the NPC here, each time it"
    " will be different.\n");
    
    add_exit("backwards", PATH +"room2", "door");
    add_exit("onwards", PATH +"room4", "door");
    }

void reset(){
        
        
        if (!npc){
    npc = clone_object( PATH +"rat" );
    npc->move( this_object() );
    }
    }