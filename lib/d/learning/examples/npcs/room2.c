#include "path.h"
inherit "/std/room";

object npc;


void setup() {
    
    set_light(70);
    set_short( "seperate file NPC room" );
    add_property( "determinate", "the " );
    set_long( "The NPC in here is cloned from another file, and brought here."
    "  This method means the same NPC can easily be loaded into a variety "
    "of locations, without the need to repeat much code.  The npc file is "
    "in /d/learning/examples/npcs/pugh2.c.\n");
    
    add_exit("backwards", PATH +"room1", "door");
    add_exit("onwards", PATH +"room3", "door");
    }

void reset(){
        
        
        if (!npc){
    npc = clone_object( PATH +"pugh2" );
    npc->move( this_object() );
    }
    }