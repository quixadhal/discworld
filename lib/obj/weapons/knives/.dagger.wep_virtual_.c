#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/weapon.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/weapons/knives/dagger.wep#1" );
  call_other( clone, "set_name", "dagger"  );
  call_other( clone, "set_short", "dagger"  );
  call_other( clone, "add_adjective", ({ })  );
  call_other( clone, "add_alias", ({ })  );
  call_other( clone, "add_plural", ({ "daggers" })  );
  call_other( clone, "set_long",  "This is a largish looking knife.\n"  );
  call_other( clone, "set_material", "steel"  );
  call_other( clone, "set_weight", 10  );
  call_other( clone, "set_length", 11  );
  call_other( clone, "set_width", 2  );
  call_other( clone, "set_value",  200  );
  call_other( clone, "new_weapon", 12800  );
  call_other( clone, "set_damage_chance", 5  );
  call_other( clone, "add_attack", "slash", 75, ({ 10, 5, 15 }), "sharp", "sharp"  );
  call_other( clone, "add_attack", "stab", 50, ({ 10, 2, 25 }), "pierce", "pierce" );
   return clone;
} /* create_virtual_ob() */
