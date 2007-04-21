#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/scabbard.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/scabbards/dagger_scabbard.sca#1" );
  call_other( clone, "set_name", "scabbard"  );
  call_other( clone, "add_adjective", "dagger"  );
  call_other( clone, "set_short", "dagger scabbard"  );
  call_other( clone, "set_long", "Similar in design to the scabbard made for longer bladed weapons."         "  In fact you could say they were the same, except for the size.\n"  );
  call_other( clone, "set_weight", 3  );
  call_other( clone, "set_value", 30  );
  call_other( clone, "set_type", "small scabbard"  );
  call_other( clone, "setup_scabbard", 10000  );
  call_other( clone, "set_damage_chance", 15  );
  call_other( clone, "set_types", ({ "dagger", "knife" }) );
   return clone;
} /* create_virtual_ob() */
