#include <drinks.h>
#include <cwc.h>
#include <config.h>

inherit "/obj/ring";

void setup() {
  set_name( "ring" );
  set_short( "blue crystal ring" );
  add_adjective( ({ "blue", "crystal" }) );
  set_main_plural( "blue crystal rings" );
  set_long( "The ring is a single piece of $material$, cut and polished "+
      "until its facets sparkle in the light.\n" );
  set_material( "sapphire" );
  set_value( 500 );
  set_value_info( "artifact", 10000 );
  set_weight( 1 );
  set_enchant( 3 );
  add_property( "artifact form", ([
    "ring of recall" : 3
  ]) );
  adjust_charges( 1 + random( 11 ) );
  set_level( 30 );
  set_zapper( file_name( this_object() ) );
} /* setup() */

string query_ring_name() { return "ring of recall"; }

string find_start_location( object person ) {
    string pos;

    if ( file_name( environment( person ) )[0..19] == "/d/cwc/Bes_Pelargic/" ) {
        return CWC_START_LOCATION;
    }

    pos = person->query_start_pos();

    if ( pos[ 0..19 ] == "/d/cwc/Bes_Pelargic/" && file_name( environment( person ) )[ 0..19 ] != "/d/cwc/Bes_Pelargic/" ) {
        return CONFIG_START_LOCATION;
    }
    
    return pos;
} /* find_start_location() */ 

void zap( object dummy, object person, object ring ) {
   string place;
   place = query_property( "destination" );
   if ( !place ) { 
      call_out( "move_person", 0, person, find_start_location( person ) );
   }
   else {
      call_out( "move_person", 0, person, place );
   }
} /* zap() */

int failed_zap( object dummy, object person, object ring ) {
  //call_out( "move_person", 0, person, ({
  //    "/d/klatch/tsort/desert/roads/road3",
  //    "/d/sur/Sheepridge/firkin",
  //})[ random( 2 ) ] );
  person->add_succeeded_mess( ring, "$N $V $D, but nothing happens.\n", ({ }) );
  return 1;
} /* failed_zap() */

void move_person( object person, string place ) {
  tell_creator( "taffyd", "%O, %s\n", person, place );

  if ( !find_object( place ) )
    place->force_load();
  if ( !find_object( place ) ) {
    tell_object( person, "Please contact a creator and ask that \""+ place +
        "\" be checked.  You are also owed a charge on this ring.\n" );
    return;
  }
  if ( environment( person ) == find_object( place ) ) {
    tell_object( person, "You momentarily feel dislocated from reality; "+
        "the feeling passes, but something has changed...\n" );
    person->adjust_volume( D_ALCOHOL, ( 500 + random( 500 ) ) * ( 1 -
        2 * random( 2 ) ) );
    return;
  }
  tell_object( person, "You suddenly feel as if something yanks you "+
      "across the dimensions.\n" );
  person->move_with_look( place, "A spot of blue appears, enlarges and "+
      "turns into $N.", "$N shrinks and becomes red, then disappears "+
      "altogether." );
} /* move_person() */
