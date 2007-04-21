/* situation room  - Shaggy */
#include <situations.h>
#include "path.h"

inherit "/std/outside";

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
  
  set_short("Combination of situations example room");
  set_light(40);
  set_long("This is a combination of situations example.  "
           "It is outside in a dark neighbourhood.  "
           "To start a combination of situations \"press\" the button.  "
           "Please more here to see how it is done.\n"
           "%^CYAN%^Functions shown:%^RESET%^\n"
           "%^YELLOW%^add_situation,  change_situation.%^RESET%^\n");
  
  add_exit( "next", SIT+"sitroom3", "path" );
  add_exit( "back", LEARNING +"search", "path" );
  add_exit( "exit", ENTRANCE, "path" );
  
  add_item( "button", "There is a button on the wall you might \"press\".");

  add_item( "house", "There are old wooden houses here.");

  /* add a situation called house about a house */
  add_situation( "house", 
    new(class situation,
      add_items: ({ 
        ({ "nearby house", "The wooden house has thin walls so you can "
             "easily hear what goes on inside." }),
        ({ "voice", "There are voices coming from a nearby house here." }) 
            }),
      end_mess: "You hear running footsteps in the alley on the other side "
             "of the house.\nThe nearby wooden house falls eerily silent."
    )
  );
  /* add a situation called argue about an argument in the house */
  add_situation( "argue", 
    new(class situation,
      chats:({"A man yells shrilly, something that sounds like an accusation.",
        "A man with a deep voice threatens loudly.",
        "A woman's voice calls for calm." }),
      chat_rate: ({ 10, 30 })
   )    
  );  
  /* add a situation called murder about a nasty conclusion */
  add_situation( "murder", 
    new(class situation,
      start_mess:"There is a loud bang, as if something fell over or "
        "was struck solidly within the wooden house here.", 
      chats:({"A man says something in shocked voice.",
              "A woman's voice hisses for silence.",
              "There is the sound of something heavy being moved inside the "
              "wooden house here." }),
      chat_rate: ({ 5, 15 })
    )
  );

} /* setup() */

/* press button */
int do_press() 
{
  tell_object(this_player(),"You press the button.\n");

  tell_object(this_player(),"%^YELLOW%^You just started the argue and house "                "situations.  It will go for 80 seconds and then the murder "
              "and house situations will replace it for 40 more seconds.  "
              "As house follows itself immediately it just continues without "
              "being changed and its end message is given only at the very " 
              "end.%^RESET%^\n");

  /* start the situation called argue and house together for 80 seconds 
     and then murder and house together for 40 seconds.  You can
     specify as many situations as you need in the comma separated list. */
  change_situation( ({ "argue,house", "murder,house" }), ({ 80, 40 }) );

  return 1;

} /* do_press() */

/* The init() function just sets up the button */
void init()
{
  this_player()->add_command( "press", this_object(), "button" );
  ::init();

}/* init() */
