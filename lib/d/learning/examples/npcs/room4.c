#include "path.h"
inherit "/std/room";

object npc;


void setup() {
    
    set_light(70);
    set_short( "chatting NPC room" );
    add_property( "determinate", "the " );
    set_long( "The NPC in here gets to talk a little bit.  He has both random"
    " chats, and chats for use in combat which make him more entertaining.  "
    "Some chats appear more often than others.\n");
    
    add_exit("backwards", PATH +"room3", "door");
    add_exit("onwards", PATH +"room5", "door");
    }

void reset(){
        
        
        if (!npc){
    npc = clone_object( PATH +"barley" );
    npc->move( this_object() );
    }
    }