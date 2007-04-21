/* situation room  - Shaggy */
#include <situations.h>
#include "path.h"

inherit "/std/room";

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
  
  set_short("Basic situation example room");
  set_light(100);
  set_long("This is a very simple situation example.  "
           "To start it \"press\" the button.  "
           "Please more here to see how it is done.\n"
           "%^CYAN%^Functions shown:%^RESET%^\n"
           "%^YELLOW%^add_situation,  change_situation.%^RESET%^\n");
  
  add_exit( "next", SIT+"sitroom2", "path" );
  add_exit( "back", LEARNING +"search", "path" );
  add_exit( "exit", ENTRANCE, "path" );
  
  add_item( "button", "There is a button on the wall you might \"press\".");

  /* add a situation called frog about a little frog.
     Note that the chat_rate is the minimum and maximum delay 
     between chats.  Any other chats on the room are unaffected
     and independent. */
  add_situation( "frog", 
    new(class situation, 
        start_mess: "A little frog appears from nowhere.\n",
        extra_look: "There is a little frog here.",
        add_items: ({ ({ "frog", "The frog is small but happy." }) }),
        chat_rate: ({ 10, 30 }),
        chats: ({"The little frog hops on the spot.",
                 "The little frog says:  Ribbit." }) ,
        end_mess: "The little frog disappears into thin air."
    )
  );

} /* setup() */

/* press button */
int do_press() 
{
  tell_object(this_player(),"You press the button.\n");

  tell_object(this_player(),"%^YELLOW%^You just started the frog situation.  "
              "It will go for 60 seconds.  It adds an add_item ( look frog ), "
              "an extra_look to the room ( look ) and some chats.%^RESET%^\n");

  /* start the situation called frog and have it go for 60 seconds */
  change_situation( "frog", 60 );

  return 1;

} /* do_press() */

/* The init() function just sets up the button */
void init()
{
  this_player()->add_command( "press", this_object(), "button" );
  ::init();

}/* init() */
