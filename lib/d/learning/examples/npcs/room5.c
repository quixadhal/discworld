#include "path.h"
inherit "/std/room";

object npc;


void setup() {
    
    set_light(70);
    set_short( "room with a non-naked NPC" );
    add_property( "determinate", "the " );
    set_long( "We ought to clothe our NPCs and give them weapons, else how "
    "will the poor darlings survive the winter, and fend off attackers?  "
    "The easiest way is to give them items which already exist on the MUD"
    " via the armoury.\n");
    
    add_exit("backwards", PATH +"room4", "door");
    add_exit("onwards", PATH +"room6", "door");
    }

void reset(){
        
        
        if (!npc){
    npc = clone_object( PATH +"macgrew" );
    npc->move( this_object() );
    }
    }