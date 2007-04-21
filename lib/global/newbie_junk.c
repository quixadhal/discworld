/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: newbie_junk.c,v 1.18 2003/07/25 05:51:49 pinkfish Exp $
 */
/**
 * Handles the new player stuff and the login gifts.
 *
 * @see /global/player
 * @author Pinkfish
 */
#include <armoury.h>
#include <config.h>
#ifndef  __DISTRIBUTION_LIB__
#include <priest.h>
#endif

#define CABBAGE "/obj/monster/cabbage"
#define LEMON "/obj/monster/muffled_lemon"
#define STICK "/obj/misc/boring_stick"
#define CAT "/obj/monster/present_cat"
#define BUG "/obj/misc/bug"
#define BINKY_PATH "/obj/misc/binky/"
#define BINKY_TOY (BINKY_PATH "my_binky")
#define BINKY_BRUSH (BINKY_PATH "binky_brush")
#define BINKY_COMB (BINKY_PATH "binky_comb")
#define BINKY_KEY (BINKY_PATH "binky_key")

/**
 * This method is called when a player first starts on the mud.  It gives
 * out all of the newbiew equipment and stuff.
 */
void start_player(object pl) {
  object ob, bucket, bottle, drink, label;
  seteuid(getuid());


  bucket = ARMOURY->request_item( "bucket small", 100 );
  bucket->set_short( "champagne bucket" );
  bucket->set_long( "This metal bucket was designed for a purpose, "
    "and looks like it would fit a champagne bottle perfectly.\n" );
  bucket->add_adjective( "champagne", "wine" );
  bucket->add_alias( "cooler" );
  bucket->add_read_mess( this_player()->query_cap_name(), 
    "etched characters", "general" );
  bucket->move(this_player());
    
  bottle = clone_object( "/obj/bottle" );
  bottle->set_max_volume( 750 );
  bottle->set_name( "bottle" );
  bottle->set_short( "champagne bottle" );
  bottle->set_main_plural( "champagne bottles" );
  bottle->add_adjective( "champagne" );
  bottle->add_alias( "champagne" );
  bottle->set_long( "An expensive-looking bottle of champagne, just right "
               "to celebrate being on " + mud_name() + ".\n" );
  bottle->set_weight( 5 );
  bottle->set_max_weight( 20 );
  bottle->set_full_weight( 25 );
  bottle->set_volume( 750 );
  bottle->set_value( 400 );
  bottle->set_leak_rate( 0 );
  drink = clone_object("/obj/reagents/generic_liquid.ob");
  drink->set_name("champagne");
  drink->set_short("fine champagne");
  drink->set_long("This is a fine champagne, $amount_size$.\n");
  drink->set_medium_alias("Champagne");
  drink->add_eat_effect("/std/effects/ingested/drunk", 500);
  drink->set_amount(750);
  drink->move(bottle);
  label = clone_object("/obj/misc/label");
  label->add_read_mess( "\n    _________________  \n"
                          "   |                 | \n"
                          "   |                 | \n"
                          "   | Vintage Century | \n"
                          "   |      of the     | \n"
                          "   |    Fruit Bat,   | \n"
                          "   |   Year of the   | \n"
                          "   |  Midnight Frog  | \n"
                          "   |                 | \n"
                          "   |_________________| \n", 
                     "cursive lettering on the bottle's label", "general" );
  bottle->add_effect("/std/effects/object/label", label);             
  bottle->move( bucket );

  ob = clone_object("/obj/armour");
  ob->set_name("sash");
  ob->set_long("A large colourful sash.\n");
  ob->add_read_mess( "Welcome to " + mud_name() + ", "+
                     capitalize( (string)(pl->query_name()) ) +".", 0,
                     "general");
  ob->set_type( "sash" );
  ob->set_weight(3);
  ob->set_adjective(({ "large", "colourful" }));
  ob->setup_armour(1000);
  ob->set_damage_chance(10);
  if (ob->move(pl)) {
    if (ob->move(environment(pl)))
      ob->dest_me();
  }
  pl->wear_armour( ob );

  ob = ARMOURY->request_item( "torch" );
  ob->add_read_mess( "\n" + mud_name() + " MUD, it brightens up your night!  "
                     "More torches can be purchased from the general stores " 
                     "found across the Disc!", "to the wood", 
                     "general" );
  ob->move( pl );
    
  ob = clone_object( "/d/liaison/NEWBIE/newbie_book" );
  ob->move( pl );

  pl->adjust_money( 8, "Pumpkin dollar" );
  pl->adjust_money( 100, "Pumpkin pence" );
} /* start_player() */

void give_junk(object pl) {
  call_out("start_player", 2, pl);
}
/**
 * Gives out the login gifts and anything else that should be done on
 * a certain login number.
 *
 * @param num the number of logins
 */
void logged_on_times(int num) {
  object ob;
  int bing;

  this_player()->remove_property("binky level");
  switch (num) {
  case 3 :
    /* Third time, give them a free cabbage */
    ob = clone_object(CABBAGE);
    if(!environment(this_player()))
      return;
    ob->move(environment(this_player()));
    ob->set_owner(this_player());
    ob->init_command("follow "+this_player()->query_name());
    tell_object(this_player(),
                "%^YELLOW%^Congratulations on logging onto " + mud_name() + " for the "
                "third time.  Please accept this cabbage as a token of our "
                "appreciation.%^RESET%^\n");
    break;
  case 10 :
    /* Tenth time.  Give them a badge... */
    ob = clone_object("/obj/clothing");
    ob->set_name("badge");
    ob->set_type("badge");
    ob->set_short("fruitbat flavoured badge");
    ob->add_adjective(({ "fruitbat", "flavoured" }));
    ob->set_weight(1);
    ob->set_damage_chance(5);
    ob->setup_clothing(10000);
    ob->set_long("The badge is brightly fruitbat flavoured with a bit of "
                 "banana around the edges.  You are very proud to own it.\n");
    ob->add_read_mess("Congratulations on being a 10 time loginer to "
                      "" + mud_name() + " MUD!\n", 0,
                      this_player()->query_default_language());
    
    if (ob->move(this_player()))
      ob->move(environment(this_player()));
    tell_object(this_player(),
                "%^CYAN%^A little fruitbat flies down out of a small "
                "non-existent black hole and gives you a badge.  The "
                "fruitbat chirps excitedly at you in fruitbat and then "
                "leaps into your badge.%^RESET%^\n");
    break;
  case 42 :
    ob = clone_object("/obj/misc/paper");
    ob->add_read_mess("What do you get when you multiply 6 by nine.", 0,
                      this_player()->query_default_language());
    if (ob->move(this_player()))
      ob->move(environment(this_player()));
    tell_object(this_player(),
                "%^YELLOW%^This is your 42nd login.  You suddenly discover "
                "a piece of paper in your hand, something froths up in your "
                "mind and you just have to write it on the paper.\nAfterwards "
                "you stare blankly down at the paper not remembering what "
                "you just wrote.%^RESET%^\n");
    break;
  case 100 :
    /* 100th time...  A singing lemon, up to a point. */
    ob = clone_object(LEMON);
    ob->set_owner(this_player());
    tell_object(this_player(),
                "%^YELLOW%^Congratulations!  You have now logged onto "
                "" + mud_name() + " 100 times.  You are now officially addicted.  "
                "This lemon will help you on your travels.%^RESET%^\n");
    break;
  case 150 :
    /* 150th time. */
    ob = clone_object(STICK);
    if (ob->move(this_player()))
      ob->move(environment(this_player()));
    tell_object(this_player(),
                "%^YELLOW%^This is your 150th login!  Everybody here "
                "wishes you a good time, to help you in designs we have "
                "provided this stick for you.%^RESET%^\n");
    break;
  case 200 :
    ob = clone_object(CAT);
    ob->set_owner(this_player());
    tell_object(this_player(),
                "A cat slinks in from somewhere and rubs itself up "
                "against your leg.\n");
    ob->move(environment(this_player()));
    break;
  case 512 :
    ob = clone_object(BUG);
    tell_object(this_player(),
                "A strange black noise starts to slowly grow louder and "
                "louder.  Suddenly in a bunch of silvery wiggling a black "
                "bug crawls into your inventory.\n");
    ob->move(this_player());
    break;
  case 990 :
    ob = clone_object(BINKY_KEY);
    ob->move(this_player());
    tell_object(this_player(),
                "You hear a tinkling noise in the distance which slowly "
                "grows closer.  Something sparkly and shiny drops out of "
                "the sky into your hands.  It appears to be some sort of "
                "key.\n");
    break;
  case 1000 :
    ob = clone_object(BINKY_TOY);
    ob->move(environment(this_player()));
    tell_object(this_player(),
                "You feel a chill in your bones and turn around to see "
                "a large imposing figure wearing a black coat.  He smiles "
                "down at you in a way that makes your hair stand on end "
                "and places a small horse on the ground in front of "
                "you.\nDeath says: TREAT HIM WELL AND COMB HIM OFTEN.\n");
    break;
  case 1010 :
    ob = clone_object(BINKY_COMB);
    ob->move(this_player());
    tell_object(this_player(),
                "Something black and unseen taps you on the shoulder.\n"
                "Death says: I FORGOT THIS.\n"
                "Death gives you a small ivory comb.\n");
    break;
  case 1020 :
    ob = clone_object(BINKY_BRUSH);
    ob->move(this_player());
    tell_object(this_player(),
                "You feel a brush in your hands, you wonder where this "
                "came from?\n");
    break;
  }

  // See how long the player has been on.
  bing = -this_player()->query_time_on();
  if (bing / (24 * 60 * 60) > 100) {
    this_player()->add_player_title("old man");
    this_player()->add_player_title("old woman");
  }
} /* no_login_times() */
