#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/scabbard.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/scabbards/belt_loop.sca#1" );
  call_other( clone, "set_name", "loop"  );
  call_other( clone, "add_adjective", ({ "belt", "belt loop" })  );
  call_other( clone, "add_alias", "belt loop"  );
  call_other( clone, "set_short", "belt loop"  );
  call_other( clone, "set_long", "This loop is made from the finest leather, designed to "         "hold axes or hammers safely.\n"  );
  call_other( clone, "set_weight", 8  );
  call_other( clone, "set_value", 800  );
  call_other( clone, "set_type", "belt scabbard"  );
  call_other( clone, "add_property", "no recycling", 1  );
  call_other( clone, "setup_scabbard", 10000  );
  call_other( clone, "set_damage_chance", 15  );
  call_other( clone, "set_types", ({ "axe","hammer" }) );
   return clone;
} /* create_virtual_ob() */
