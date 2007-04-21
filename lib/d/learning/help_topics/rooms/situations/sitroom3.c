/* Basic situation room  - Shaggy */
#include <situations.h>
#include "path.h"

inherit "/std/outside";

object warrior;

void setup() {
   /* The "commented functions" and "keywords" properties are specific for 
    * rooms in the learning domain and is used to make it possible to search
    * for subjects. 
    * See /d/learning/handlers/search.c for the way this is done.
    */
  add_property( "commented functions", 
                ({  }) );
  add_property( "keywords", 
                ({ "situation" }) );
  
  set_short("Automated situations (with function pointers) example room");
  set_light(40);
  set_long("This is a combination of situations example that is automated "
           "and makes use of function pointers.  "
           "It is outside in a run down neighbourhood.  "
           "Just wait to experience the situations within a minute or so.  "
           "Please more here to see how it is done.\n"
           "%^CYAN%^Functions shown:%^RESET%^\n"
           "%^YELLOW%^add_situation,  automate_situation.%^RESET%^\n");
  
  add_exit( "next", SIT+"sitroom4", "path" );
  add_exit( "back", LEARNING +"search", "path" );
  add_exit( "exit", ENTRANCE, "path" );
  
  add_item( "window", "There is window up high with maroon curtains." );
  add_item( "curtain", "That is all you can see, sorry." );

  /* This is the first part of "seamstress" at work */
  add_situation( "sew1", 
    new(class situation,
      chats: ({"There is a distinct giggle.",
        "A woman's giggle drifts down from an upstairs window.",
        "A man murmurs something upstairs." }),
      chat_rate: ({ 10, 15 })
    )
  );
  /* This is the second part of "seamstress" at work 
     The end_func is a function to call at the end to do something
     a bit special. */

  add_situation( "sew2", 
    new(class situation, 
      start_mess: "There is a whump like something landing on a matress "
         "above.", 
      chats:({
        "A high-pitched giggle reaches the street from above.",
        "There is the creak of complaining coils from an upstairs window.",
        "You can make out a muffled voice from above." }),
      chat_rate: ({ 10, 15 }),
      end_mess: "The upstairs noises stop.\nYou hear the clink of coinage.",
      end_func: (: call_out("make_warrior",5) :)
    )    
  );
  /* This is an empty "pause" situation */
  add_situation( "pause", new(class situation) );

  /* This makes the set of situations above occur automatically and randomly
     through the day.  WHEN_ANY_TIME specifies that it can happen
     at any time of day.   more /include/situations.h to get the list
     of possible times of day filters.  500 is the chance out of 1000
     that this situation is started every 80 seconds.  Note that the
     situation will not wait for a player to enter -- it may be part
     way through when the room first loads. */

  automate_situation( ({ "sew1","sew2","pause" }), ({ 25,35,20 }), 
    WHEN_ANY_TIME, 500 );

} /* setup() */

/* This function makes warriors */
void make_warrior() 
{
  if (warrior) 
    call_out( (: $1->move( "/room/rubbish", 
     "", $1->the_short()+" races off to kill something.") :), 5, warrior );

  warrior = clone_object( "/d/am/chars/am_warrior_human" );
  warrior->move(this_object(),warrior->the_short()+
    " enters the street from a building nearby looking pleased with "
    +warrior->query_objective()+"self.");

} /* make_warrior() */
