#include "path.h"
inherit "/std/room";

//The next line tells the MUD to expect an object to 
//be placed in this object (room).  We'll call it npc
object npc;


void setup() {
    
    set_light(70);
    set_short( "cloned NPC room" );
    add_property( "determinate", "the " );
    set_long( "This room holds a very simple NPC.  He is a fighter.  His "
    "code is within the room code.  If he's not here, type update, and"
    ", like magic, he should appear.  If he doesn't, panic.\n");
    
    add_exit("onwards", PATH +"room2", "door");
    }

void reset(){
        
        
        if (!npc){
        //When we need the object npc, we clone one, and make it a 
        //simple monster base.
    npc = clone_object( "/obj/monster" );
    
        //Give him a name, and short
    npc -> set_name("pugh");
    npc -> set_short("Pugh");
    
        //Make sure he is Pugh, not The Pugh
    npc -> add_property("determinate", "");
    
        //Add aliases, so players can call him what they like
    npc -> add_alias( ({"npc", "warrior", "man"}) );
    
        //Add a long description of him
    npc -> set_long("This is Pugh.  He was cloned here, and "
    "this is his world.  Welcome to it.\n");
    
        //Basic_setup allows us to define race, class and level
        //You must include these three things. Race and class
        //come from /std/race.c
    npc -> basic_setup("human", "warrior", 10);
    
        //gender 1 = male, 2 = female, 0 = neuter
    npc -> set_gender( 1 );
    
        //And finally, having made the object, we bring it to the room
    npc->move( this_object() );
    }
    }