#include "path.h"
inherit "/std/room";

object npc;


void setup() {
    
    set_light(70);
    set_short( "room with a fashion-victim NPC" );
    add_property( "determinate", "the " );
    set_long( "The NPC is here is not content with last year's fashion.  "
    "He wants new, exciting clothes, to be unique to him.  He has some "
    "standard stuff, but he has things which are unique to him.\n");
    
    add_exit("backwards", PATH +"room5", "door");
    add_exit("onwards", PATH +"room7", "door");
    }

void reset(){
        
        
        if (!npc){
    npc = clone_object( PATH +"cuthbert" );
    npc->move( this_object() );
    }
    }