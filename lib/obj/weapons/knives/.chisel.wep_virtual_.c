#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/weapon.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/weapons/knives/chisel.wep#1" );
  call_other( clone, "set_name", "chisel"  );
  call_other( clone, "set_short", "chisel"  );
  call_other( clone, "set_long", "This is a tool with a sharp edge which is used to shape and cut metal, wood, or stone.\n"  );
  call_other( clone, "set_weight", 11  );
  call_other( clone, "set_length", 6  );
  call_other( clone, "set_width", 3  );
  call_other( clone, "set_material",  "steel"  );
  call_other( clone, "set_value", 300  );
  call_other( clone, "new_weapon", 19200  );
  call_other( clone, "set_damage_chance", 5  );
  call_other( clone, "add_attack", "slash", 30, ({ 10, 3, 10 }), "sharp", "sharp", 0  );
  call_other( clone, "add_attack", "stab", 30, ({ 10, 3, 10 }), "pierce", "pierce", 0 );
   return clone;
} /* create_virtual_ob() */
