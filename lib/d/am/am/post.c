inherit "/std/room";
#include "path.h"
#include "mail.h"

void setup() {
  set_co_ord(({ -55, 100, 0 }));
  set_short("Ankh Morpork's Post Office");
  set_long("A large spacious room with large pictures of frogs of the world "+
           "adorning its walls.  This is obviously the Post office.  You can "+
           "tell by the way all the frogs have post office hats on.  "+
           "There is a small ragged looking sign attached to the wall "+
           "above the demons cage.  "+
           "Beside the counter is a small cage, there appears to be "+
            "some things in it.  To the south is a red door.\n");
  set_light(85);
  add_exit("north", ROOM+"filgree9", "door");
  modify_exit("north", ({ "open", 1 }));
  add_exit("south", ROOM+"post_mail", "door");
  add_alias("demons", "demon");
  add_item("cage", "A small cage full of demons.  They appear to be playing "+
                   "a complex game of cards.  They glare balefully at you "+
                   "when you look at them and mutter something rude about "+
                   "stupid mail.\n");
  add_item("counter", "A large imposing counter stretching accross the length "+
                      "of the room.  It looks like the post office caters for "+
                      "more people than use it, you can tell by the way the "+
                      "forty tellers behind the counter are avidly watching "+
                      "a snail crawl across the counter.\n");
  add_item("snail", "A small common garden snail.  You have no idea how it "+
                    "survived long enough in Anhk Morpok to get here.\n");
  add_item("teller", "The tellers are all watching a snail crawl "+
                     "across the counter.\n");
  add_item("frog", "There are about ten pictures of frogs hanging on the "+
                   "walls of the post office.  They all have captions "+
                   "under them which say things like \"Sir Frog Hanible\" "+
                   "is honoured for the great work he achieved during the "+
                   "mosquito war of 1456.\n");
  add_item("demon", "The demons are sitting in the small cage playing "+
                    "cards, they grin evily at you as you look at them.\n");
  add_sign("A small faded sign above the demons cage.", "It has these "+
                   "words written on it in an ancient script. \"Adventurers "+
                   "are called on to gather together the instuments of "+
                   "power to sedate a great beast that lives in the land.  "+
                   "The weapons need for this battle are an ancient coin, "+
                   "a strange fish, Ankh river water, a mystical shell, "+
                   "the great cauldron, its lid and a clear ladel.  Once "+
                   "these items of power have been gathered they must "+
                   "be taken to the picnic area outside Ankh-Morpork, "+
                   "whereapon the shell must be blown to call forth "+
                   "the creatures.\"\n");
} /* setup() */

int mail(string str) {
  MAIL_TRACK->mail(str);
  return 1;
} /* mail() */

int send_message(string to) {
  object ob;

  tell_room(this_object(),
      "One of the tellers lifts himself off his seat with great effort "+
      "releases one of the demons from the cage.\n");
  ob = clone_object(CHAR+"post_demon");
  ob->move(this_object());
  ob->do_command("south");
  this_player()->add_message(ob, "The Demon takes your note with a disgusted "+
              "look.\n");
  tell_room(environment(this_player()), "The demon takes "+
            this_player()->query_cap_name()+"'s note with a disgusted look.\n",
            this_player());
  if (find_player(to))
    ob->goto_player(to, this_player()->query_cap_name());
  else
    ob->goto_dest("/d/am/uu/uni/comm_room", this_player()->query_cap_name());
  ob->set_home(file_name(this_object()));
  return 1;
} /* send_message() */

void after_call(string to) {
  object ob;

  ob = find_living(to);
  if (!ob)
    return ;
  if (environment(ob) == this_object())
    say("The demon wanders back in the door and looks in disgust at "+
       to+" and waves a note at "+ find_player(to)->query_objective() +" before getting back into the cage "+
        "and starting a rowdy argument about whether or not he lost the "+
        "hand.\n", this_player());
    else {
      tell_room(environment(ob), "A demon wanders up to "+to+" shows him a "+
                "note and slouches off again.\n", ({ ob }));
      call_out("come_back",5);
    }
    tell_object(ob, "A demon wanders up to you, waves a note lazily at you "+
                    "and departs the way he came.  You might have some mail.\n");
} /* after_call() */

void come_back() {
  tell_room(this_object(), "A demon wanders in, goes over to the cage opens "+
            "it and puts himself away, just before starting a loud argument "+
            "with the other demons about whose turn it is.\n");
} /* come_back() */
