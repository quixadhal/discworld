/* -*- LPC -*- */
#include "path.h"

inherit "/std/room";

object teacher;
object sign;

void setup() {
   set_short( "foyer of the terrain school" );
   add_property( "determinate", "the " );
   set_long( "This is the foyer to the terrain teacher's school.  "
      "The walls here are hung with beautiful landscapes of complex tracts "
      "of land, and beneath each landscape is a map of some kind.  "
      "An intricate chandelier hangs from the ceiling, casting an even "
      "glow on all the landscapes.  "
      "Comfortable benches line the walls here, with small tables "
      "between them.  "
      "The floor is covered with a plush green carpet, which would be nice "
      "if not for the heavy mud stains across it.\n" );
   add_item( "landscape",
      "The landscapes show mountains, jungles, forests, "
      "oceans, rivers... all sorts of different terrains, including a "
      "city and a village." );
   add_item( "map",
      "Beneath each landscape hangs a single topographical "
      "map which appears to be a detailed representation of the landscape "
      "above it." );
   add_item( "chandelier",
      "Crystal teardrops scatter the light of a dozen "
      "candles around the room, casting an easy, comfortable light by which "
      "you can see.", 1 );
   add_item( "crystal teardrop", 
      "The candlelight makes it too hard to see "
      "the crystals clearly." );
   add_item( "candle", 
      "Nestled into the midst of the chandelier, the twelve "
      "candles provide all the light in this room.");
   add_item( "bench",
     ({ "long", "These wooden benches have comfortable leather cushions "
           "firmly attached.",
        "position", "a wooden bench",
     }) );
   add_item( "table",
     ({ "long", "These tables are covered with magazines talking "
           "about far away places and exotic architecture.",
        "position", "a table",
     }) );
   add_item( "plush green carpet",
      "This deep pile carpet was once a rich evergreen colour, but "
      "years of muddy footprints have stained much of it a deep, "
      "ugly brown." );
   add_item( "mud stains",
      "Someone has b een walking on this formerly beautiful carpet with "
      "muddy boots." );
   add_item( "cushions",
      "Thick leather cushions, stuffed with something very soft, and "
      "affixed quite securely to the benches." );
   add_item( "magazines",
     ({ "long", "Architectural and travel magazines sit on the "
           "various tables around the room.",
        "read", "\"Ruins of The Temple of Offler!\", "
           "\"Beautiful Ankh-Morpork!\", \"Design of the Tower of Art\", "
           "and \"Scenic Ramtops Vacations!\" are just the most visible "
           "of the titles that scream up at you.  Hopefully the "
           "magazines are kept just for their pictures, which seem far "
           "more interesting than their text.",
     }) );
   set_light( 60 );

   add_exit( "grassyfield", PATH "grassystep", "hidden" );
   add_exit( "desert", PATH "sandtent", "hidden" );
   add_exit( "mountaintop", PATH "mountaincabin", "hidden" );
   add_exit( "back", LEARNING +"search", "path" );
   add_exit( "exit", ENTRANCE, "path" );
   /* The "commented functions" and "keywords" properties are specific for 
    * rooms in the learning domain and is used to make it possible to search
    * for subjects. 
    * See /d/learning/handlers/search.c for the way this is done.
    */
   add_property( "commented functions", 
                ({  }) );
   add_property( "keywords", 
                ({ "terrain" }) );

   sign = add_sign(
      "A cardboard sign, with text scrawled messily across the front.\n",
      "Awye FRoM TH OFFICe",
      "cardboard sign",
      "sign",
      "common" );
   if (sign) sign->set_get();
} /* setup() */

void init() {
   if ( !this_player() )
      return;
   if ( !interactive( this_player() ) )
      return;
   if (!teacher) {
      teacher = load_object( CHARS "terrain_teacher" );
   } /* setup() */   if ( teacher ) {
      teacher->pupil_arrived( this_object() );
   }
} /* init() */
