#include "path.h"
inherit "/std/bank";

#define BANK_SAVE "/d/am/save/bank"

void setup() {
  set_light(100);
  set_short("Bing's bank");
  set_long("This is a brightly lit room, with friendly looking people "+
           "standing behind the polished bench, just waiting to rip you "+
           "off, ooops I mean serve you.  There is a sign hanging on the "+
           "wall.\n");

  add_exit("west", ROOM+"muligan4", "door");
  set_save_file(BANK_SAVE);

  add_item("bench", "A nice shiny bench made of the best oak.  It looks "+
                    "very expensive and fits in with the rest of the plush "+
                    "atmosphere of the room.\n");
  add_item(({ "teller", "people" }),
                     "A nice happy looking bunch of tellers.  They look "+
                     "happy and will to serve.  They don't look predatory "+
                     "at all.\n");
  add_item("wall", "They are amazingly lush.  You are surprised they are "+
                   "not carpeted.\n");
  add_item("floor", "Polished wooden floor boards.  You look around for the "+
                    "cleaning ladies who are going to clean up your foot "+
                    "prints.\n");

  add_sign("A small happy sign.\n",
           "Welcome to the First bank of Bing.\nThe commands are "+
           "\"open\" an account, \"close\" an account, \"deposit\" some "+
           "money, \"withdraw\" some money and get a \"balance\".  Please "+
           "note we charge 5% on deposits, yours happily Bing.\n");
  set_percentage(95);
}

