/*
Liaison hall room - people can add exits here - see the sign.
This is designed so people don't add to the commonroom, add here instead.
Lucifer, 10 January 2002 at 2am, the ungodly hour.
Thank you to Drakkos for the mirror code...
*/


inherit "/std/room/basic_room";

string do_mirror();

void setup() {
    set_short("Liaison hallway");
    set_light(100);
    add_property("determinate", "the ");
    set_long("This is the homely corridor of the Liaison Domain.  There are "
             "a lot of doors leading off here, hopefully taking the plucky "
             "entrant a-wandering into Liaison workrooms.  Exercise caution "
             "as some Creators prefer you to ask or at least knock first.  "
             "Apart from the doors here there are some paintings of various "
             "people, as well as a large floor to ceiling mirror. Taking "
             "pride of place on one of the walls, however, is an ancient "
             "looking scroll housed in a picture frame.  There is "
             "also a sign here.\n");
    add_item("doors", "Lots of doors.  Tall doors, short doors, red doors, "
             "blue doors, green doors, thin doors, wide doors, old doors, "
             "grimy doors, wooden doors, metal doors and even a padded "
             "red linonium door - kinky.  However, you can only go through "
             "the doors marked in the exits, sorry.");
    add_item("paintings", "Paintings of people who have been important to "
             "the domain.  There's one of Ohdamn, Moonchild, Olorin, Shalla, "
             "Kaea, Nevvyn, Wyvyrn and - strangely enough - You.");
    add_item("painting of ohdamn", "The painting of Ohdamn shows a very "
             "tall man with reddish blond hair.  He appears to be carrying "
             "a box of chocolates and looks to be in the middle of a rousing "
             "love song, which may have something to do with the bottle of "
             "rum in his other hand.");
    add_item(({"mission", "scroll", "statement"}), "It's the liaison "
             "statement of mission!\n"
             "Or mission statement, if you would prefer.\n\n"
             "Our mission in the Liaison Domain is to give continuous "
             "support to the creator and player bases and facilitate "
             "communcation between them.\n"
             "Through a diverse range of activities, we aim to maintain "
             "Discworld MUD as an enjoyable, interesting, in-theme and "
             "safe virtual space.\n"
             "We seek to contribute to Discworld MUD as a visible and helpful "
             "helpful first point of creator contact for players.  Additionally, "
             "we will work within assigned creator areas to assist in the "
             "development and implementation of concepts.\n\n"
             "Impressive, huh?");
    add_alias("mission", "statement");
    
    add_item("frame", "It's just a frame holding a large scroll in place.");
    add_item("hair", "It's hair.  It's reddish blond and it's Ohdamn's.");
    add_item("rum", "It's rum.  Held very securely by Ohdamn.");
    add_item("chocolates", "They are chocolates.  On Ohdamn's painting.");
    add_item("painting of moonchild", "The painting of Moonchild shows a "
             "looming lady with a big whip and an evil grin.  She appears "
             "to be half in shadows.");
    add_item("whip", "A fearsome looking whip that seems to fit well in "
             "Moonchild's hand.");
    add_item("painting of olorin", "Olorin's picture appears to be only "
             "half done - as if the artist didn't have time to finish it.");
    add_item("painting of shalla", "Shalla's painting depicts a pretty "
             "lady and although she is fully clothed, your attention is "
             "drawn to her socks.  Strange.");
    add_item("socks", "Stop looking - that's illegal in some countries!");
    add_item("painting of kaea", "Kaea's painting shows a young lady with a "
             "silly looking grin on her face and giggling.  Seems to say it "
             "all really...");
    add_item("painting of nevvyn", "The painting of Nevvyn shows a man with a "
             "pint hefted in a salute and a cheeky looking grin on his face.");
    add_item("pint", "A half empty pint in Nevvyn's hand.  Says it all!");
    add_item("painting of wyvyrn", "Wyvyrn's painting depicts a ... well, a "
             "Wyvyrn.  He looks a bit like a dragon and has green wings - but "
             "seems to have the distinct disadvantage of looking cute.");
    add_item("wings", "Wings.  On Wyvyrn.");
    add_alias("painting of me", "painting of you");
    add_item("painting of you", "The painting of you is ... well, blank with the "
             "words 'Your Name Here' on it.  You get the feeling its a rather "
             "cheesy way of saying everyone is important!  If it wasn't so "
             "cheesy it might make you feel better.");                    
    add_sign("This is an important sign, for viewing along with the "
             "paintings.\n",
             "If you are having problems seeing the paintings, look at "
             "painting of <person>.  "
             "Please note: These paintings are done on collective images.  "
             "Basically, if you don't like your portrait, don't beat Luci "
             "up.  Thanks!");                                             
    add_item("mirror", (: do_mirror() :));
    
    add_exit("common", "/d/liaison/common", "door");
    add_exit("drum", "/d/am/am/mendeddrum", "path");
    
/* Please add exits to this room from your commonroom using this line
   of code:
   "/d/liaison/cre_corrdior"->add_exit(CREATOR, "workroom path", "door");
   Thanks!
*/
}

string do_mirror() {  
  string text;
  
 
  text = "The mirror is a full length floor to ceiling job, a full length "
    "mirror in the true sense of the word.  It's here to make sure you look "
    "you look your best.  Good job you are checking!\n\n";
  text += this_player()->long(this_player());
  tell_room (this_object(), this_player()->short() + " checks " + 
    this_player()->query_objective() + "self in the big mirror.  Poser.\n",
    this_player());
  return text;
}

