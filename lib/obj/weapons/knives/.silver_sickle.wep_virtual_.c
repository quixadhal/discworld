#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/weapon.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/weapons/knives/silver_sickle.wep#1" );
  call_other( clone, "set_name", "sickle"  );
  call_other( clone, "set_short", "silver sickle"  );
  call_other( clone, "add_adjective", "silver"  );
  call_other( clone, "set_long", "This is a dagger with a crescent shaped blade cast from silver.\n"  );
  call_other( clone, "set_weight", 9  );
  call_other( clone, "set_length", 12  );
  call_other( clone, "set_width", 8  );
  call_other( clone, "set_material",  "silver"  );
  call_other( clone, "set_value",  2000  );
  call_other( clone, "new_weapon", 12800  );
  call_other( clone, "set_damage_chance", 5  );
  call_other( clone, "add_attack",  "slash", 75, ({ 10, 5, 15 }), "sharp", "sharp"  );
  call_other( clone, "add_attack",  "stab", 50, ({ 10, 2, 25 }), "pierce", "pierce" );
   return clone;
} /* create_virtual_ob() */
