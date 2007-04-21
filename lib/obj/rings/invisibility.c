inherit "/obj/ring";

void setup() {
   set_name( "ring" );
   set_short( "clear crystal ring" );
   add_adjective( ({ "clear", "crystal" }) );
   set_main_plural( "clear crystal rings" );
   set_long( "The ring is made from a colourless stone that seems to refract "+
         "light in a strange way.  The outer edge has eight sides.\n" );
   set_material( "calcite" );
   set_value( 200 );
   set_value_info( "artifact", 5000 );
   set_weight( 1 );
   set_enchant( 2 );
   add_property( "no recycling", 1 );
   add_property( "fluff'n'stuff", 1 );
   add_property( "artifact form", ([
      "ring of invisibility" : 2
   ]) );
   adjust_charges( 1 + random( 7 ) );
   set_level( 60 );
   set_zapper( file_name( this_object() ) );
} /* setup() */

string query_ring_name() { return "ring of invisibility"; }

void zap( object dummy, object person, object ring ) {
   person->add_effect( "/std/effects/magic/invisibility",
         roll_MdN( 7, (int)ring->query_level() ) );
} /* zap() */

/*
int failed_zap( object dummy, object person, object ring ) {
Should make them go blind...
} failed_zap() */
