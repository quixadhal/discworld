#include "path.h"
/*
 * Example room for using vessels.
 */

inherit "/std/room";

object wine_bottle, glass, cup;

void setup() {
   set_short("room of vessel and liquid examples");
   add_property( "determinate", "the " );
   set_long( "This room gives some simple examples of using vessels.  "
            "There should be a wine bottle, a wine glass, and a "
            "coffee cup in here.  There are also a few usage notes "
            "on the wall (\"read note1\", \"read note2\", etc).\n" );
   set_light( 100 );
   add_item( "note1",
            ({ "long", "This note describes how to use the vessels.",
               "read", "The vessels all have the following commands: "
                   "pour, fill, drink, empty.  There are a few others, "
                  "but they're mainly carry-overs from the old "
                  "container code, and probably don't work correctly.  "
                  "Try pouring from one object to another, drinking, etc.  "
                  "Try mixing equal parts of wine and Klatchian coffee.  "
                  "Use the \"stat\" command to check on the volumes (eg, "
                  "\"stat bottle\", \"stat all in cup\").  If you want " 
                  "to start over with a fresh vessel, dest it and " 
                  "\"call reset() here\".\n"
            }) );
   add_item( "note2",
            ({ "long", "This is a little note on liquid units of measure.",
               "read", "The base unit for a liquid is the drop.  Four "
                  "drops is roughly 1 cc, and 200 drops of water weighs "
                  "one Discworld weight unit.  Here are some common "
                  "amounts (I believe these are all British measures, "
                  "but I could have made an error):\n"
                  "    1 teaspoon =      20 drops\n"
                  "    1 tablespoon =    60 drops\n"
                  "    1 ounce =        120 drops\n"
                  "    1 cup =          960 drops\n"
                  "    1 pint =        2400 drops\n"
                  "    1 quart =       4800 drops\n"
                  "    1 gallon =     19200 drops\n"
            }) );
   add_item( "note3",
            ({ "long", "This is a note on adding effects to food, "
                  "especially liquids.",
               "read", "To add an effect to food so that it will be "
                  "applied to the player when consumed, use "
                  "add_eat_effect(<effect>, <arg>).  <arg> (which "
                  "should be an integer) is multiplied by the weight "
                  "of the amount consumed; for liquids, this is generally "
                  "200 drops per weight unit.\n\n"
                  "For example, the wine in the wine bottle adds the "
                  "drunk effect; a final <arg> of 3900 for this effect "
                  "will last 10 minutes on a player with constitution 13 "
                  "(trust me on this).  If we want one pint (2400 drops) "
                  "of wine to have this effect, the <arg> to "
                  "add_eat_effect() should be 3900/(2400/200), or "
                  "325.\n"
            }) );
   add_item( "note4",
            ({ "long", "This is a note on using continuous objects.",
               "read", "Continuous objects (those derived from "
                  "/obj/cont_medium, such as /obj/food) are a way "
                  "to have multiple identical instances of an item "
                  "without having to have a clone for each one.  Each "
                  "unique continuous object is identified by its "
                  "\"medium alias\".  The medium alias must be a single "
                  "word without embedded spaces; by convention, it is "
                  "made by concatenating a series of capitalized words "
                  "that describe it, such as \"RedWine\" or "
                  "\"GroundPurpleMushroom\".  All objects with a common "
                  "medium alias in the same environment are combined "
                  "into a single object.  Also, the reaction handler "
                  "uses the medium alias to identify objects that "
                  "react with one another.\n"
            }) );
   add_exit( "back", LEARNING +"search", "path" );
   add_exit( "exit", ENTRANCE, "path" );
   /* The "commented functions" and "keywords" properties are specific for 
    * rooms in the learning domain and is used to make it possible to search
    * for subjects. 
    * See /d/learning/handlers/search.c for the way this is done.
    */
   add_property( "commented functions", 
                ({ "make_bottle", "add_eat_effect", "set_medium_alias",
                   "set_max_weight", "set_max_volume" }) );
   add_property( "keywords", 
                ({ "bottle", "vessel", "liquid" }) );
} /* setup() */

void reset() {
   call_out( "check_vessels", 3 );
} /* reset() */

void check_vessels() {
   object wine, coffee;
   
   /* each of these if's check if the vessel has been dest'ed or moved out of 
    * the room
    */
   if ( !wine_bottle || ( environment( wine_bottle ) != this_object() ) ) {
      /* The bottle is gone, make a new one
       */
      wine_bottle = clone_object( "/obj/bottle" );
      /* For backwards compatibility, make_bottle() expects the
       * volume to be cubic centimeters (or at least that's the way
       * it worked out).
       */
      wine_bottle->make_bottle( "green", 600 );
      wine_bottle->move( this_object(), "Jeremy walks into the room and "
                        "leaves $N before walking out again." );
      /* I only fill the bottle when a new one is cloned, to avoid
       * confusing people.
       */
      wine = clone_object( "/obj/reagents/generic_liquid.ob" );
      wine->set_name( "wine" );
      wine->set_short( "red wine" );
      wine->set_long( "This is a deep red wine, $amount_size$.\n");
      wine->set_medium_alias( "RedWine" );
      wine->add_adjective( "red" );
      wine->add_eat_effect( "/std/effects/ingested/drunk", 325 );
      /* This will completely fill the bottle
       */
      wine->set_amount( 2400 );
      wine->move( wine_bottle );
   }
   if ( !glass || ( environment( glass ) != this_object() ) ) {
      glass = clone_object( "/obj/vessel" );
      glass->set_name( "glass" );
      glass->set_short( "wine glass" );
      glass->set_long( "This is a fine crystal wine glass.\n" );
      glass->set_weight( 4 );
      glass->set_max_weight( 8 );
      // Make it hold about half a cup
      glass->set_max_volume( 480 );
      glass->add_adjective( ({ "crystal", "wine" }));
      glass->move( this_object(), "Jeremy walks into the room and "
                        "leaves $N before walking out again.");
   }
   if ( !cup || ( environment( cup ) != this_object() ) ) {
      cup = clone_object( "/obj/vessel" );
      cup->set_name( "cup" );
      cup->set_short( "coffee cup" );
      cup->set_long( "This is a delicate china coffee cup.  The intricate "
                    "artwork on the side depicts some sort of sexual "
                    "orgy.  Why it's on a coffee cup is beyond me.\n" );
      cup->set_weight( 4 );
      cup->set_max_weight( 8 );
      cup->set_max_volume( 960 );
      cup->add_adjective( ({ "china", "coffee" }) );
      cup->move( this_object(), "Jeremy walks into the room and "
                "leaves $N before walking out again." );
      coffee = clone_object( "/obj/reagents/generic_liquid.ob" );
      coffee->set_name( "coffee" );
      coffee->set_short( "coffee" );
      coffee->set_long( "This is some Klatchian coffee, $amount_size$.  "
                       "It is very dark and aromatic.\n" );
      coffee->set_medium_alias( "KlatchianCoffee" );
      coffee->add_adjective( "klatchian" );
      coffee->add_eat_effect( "/std/effects/ingested/knurd", 325 );
      coffee->set_amount( 960 );
      coffee->move( cup );
   }
} /* check_vessels() */

