inherit "std/pub";
/* was room/pub.... but we dont have any pubs yet ;) */
#include "pub.h"
#include "path.h"
mixed top_list, top_deaths;
void add_to_list(object ob);

#define HOSPITAL "d/am/hospital"
 
object board, *cabbages;

void reset() {
  if (!board) {
    board = clone_object("/obj/misc/board");
    board->move(this_object());
    board->set_datafile("frog");
  }
}

void setup() {
  add_menu_item("Morpokian beer", ALCOHOL, 20, 5, 10, 10,
          "slurp down your beer", "slurps down the beer");
  add_menu_item("Fine ale", ALCOHOL, 100, 25, 50, 50,
          "start to feel light-headed", "drinks the ale");
  add_menu_item("Ankh whiskey", ALCOHOL, 200, 50, 100, 100,
          "feel a burning all down your throat", "drinks the whiskey" );
  add_menu_item("Ancient scumble", ALCOHOL, 1000, 100, 500, 500,
          "see monsters come through the wall",
          "grins and drinks the Ancient scumble");
  add_menu_item("Back burner", ALCOHOL, 12000, 200, 6000, 6000,
          "feel the world start to slip away",
          "eyes go glassy and he stares fixedly at the ceiling");
  add_menu_alias("burner", "Back burner");
  add_menu_alias("back burner", "Back burner");
  add_menu_alias("back", "Back burner");
  add_menu_alias("beer", "Morpokian beer");
  add_menu_alias("ale", "Fine ale");
  add_menu_alias("whisky", "Ankh whiskey");
  add_menu_alias("whiskey", "Ankh whiskey");
  add_menu_alias("scumble", "Ancient scumble");
  add_menu_alias("nasty", "Ancient scumble");
  add_menu_alias("death", "Ancient scumble");
  add_property("no throw out", 1);
  set_short("The Mended Drum");
  set_long(
"The bar of the infamous Mended Drum (built on the ruins of the Broken "+
"Drum) is as dirty as the rest of the pub.  Inground "+
"dirt appears to be the style of decor here.  The rubbish must have been "+
"carefully collected over many years selecting the best and most pungent "+
"to get the current selection.  Just above the bar, in "+
"grubby letters you can hardly read, is a menu.\n");
   add_exit("secret", "/w/common", "secret");
  modify_exit("secret", ({ "function", "query_leave" }));
  add_exit("south", ROOM+"mendeddrum-foyer", "door");
set_light(100);
  set_zone("pub with no monsters");

  add_item("extremely dirty bar",
           "Yes it is extremely dirty.  The glasses definitely don't "+
           "glide down this one to someone's hand.\n");
  add_item("people",
           "There are so many of them I didn't mention them in the long "+
           "description.  But they are here, pushing and shoving you "+
           "and generaly making you wish you weren't standing in front of "+
           "the bar.\n");
} /* setup() */

dest_me() {
  if (board)
    board->dest_me();
  ::dest_me();
}


int query_leave() {
  return (int)this_player()->query_property("demon");
} /* query_leave() */
