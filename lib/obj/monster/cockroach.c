inherit "/obj/monster";

void setup() {
   set_name("cockroach");
   set_short("cockroach");
   set_race("cockroach");
   set_level( random( 1 + random( 3 ) ) );
   set_main_plural("cockroaches");
   add_alias( "roach" );
   add_plural( "roaches" );
   add_adjective("dirty");
   set_long("This is a huge dirty great big cockroach, the size of your "
            "hand at least.  It waves it feelers at you and looks "
            "horrible and black.  Not only can cockroaches survive "
            "a nuclear blast, they can survive in Ankh-Morpork!\n");
   load_chat( 5, ({
         10, ":scuttles around a bit.",
         10, ":cleans its feelers.",
         10, ":rushes around in circles.",
         1,  ":jerks off in a strange direction.",
      }) );
   set_wimpy( 30 );
   load_a_chat( 50, ({
         1, ":oozes horrible white stuff.",
         1, ":makes a sort of scraping noise."
      }) );
} /* setup() */

