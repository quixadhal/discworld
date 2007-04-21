#include "path.h"

inherit "/std/room";

int open;

void setup() {
   set_short("add_item room #5b, item descriptions that change with time");
   set_light(100);

   set_long("add_item room #5b, item description that change with time\n"
            "*BEEEP* *BEEEP* WARNING! WARNING! *BEEEP* *BEEEP*\n"
            "This room is crap, and uses a non longer valid way of changing "
            "descriptions, if you see anything like this, be sure to bug "
            "report it.\nAlso note that \"I\" is Mithal:)\n"
	    +"There are a few other uses for the pasting function. One of "
	    +"them is to provide descriptions that change depending on "
	    +"certan variables.  In this room there is a chest.  Every other "
	    +"time you look at it, it is open.  In the code there is a "
	    +"global variable 'open' that is switched between 1 and 0 every "
	    +"time 'look_chest' is called.  The function returns 'The chest "
	    +"is open' if open == 1, else 'The chest is closed'. "
	    +"Unfortunately, the exact code cannot be put here... since the "
	    +"pasting function would work as you read the help text. *smile* "
	    +"Thus & is used in place of @ in the example code.\n"
	    +"A chest.\nA note.\n");

   
   add_item("note", ({ "long", "It can be read.",
		       "read", "Here is the code for the chest:\n"
       +"   add_item(\"chest\",\"The Chest is \"+\n"
      +"        \"&&look_chest:\"+file_name(this_object())+\"&&\\n\");\n\n"
     +"Again, the & signs are used to represent @ signs.\n"
     +"The code for look_chest:\n\n"
     +"   string do_chest() { \n"
       +"      if( open ) { open = 0; return \"Open.\"; } \n"
       +"      open = 1;  return \"Closed!\";  } \n\n"
     +"  I think this is a much better way to deal with stuff than the famous "
     +" modify_item.\nUse and be happy!\n" }));

      add_item("chest","The chest is "+
               "@@look_chest:"+file_name(this_object())+"@@\n");
   
   open = 0;
   add_exit("east",PATH+"action_verbs2","path");
   add_exit("west",PATH+"action_verbs","path");
   add_exit( "start", MAIN, "path" );
   add_exit( "exit", LEARNING +"search", "path" );
}

string look_chest() {
   if(open) { open = 0; return "Open."; }
   open = 1; return "Closed!";  
}
    
   

