#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/weapon.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/weapons/knives/klatchian_steel_dirk.wep#1" );
  call_other( clone, "set_name", "dirk"  );
  call_other( clone, "set_short", "Klatchian steel dirk"  );
  call_other( clone, "add_adjective", ({ "klatchian", "steel" })  );
  call_other( clone, "add_alias", "dagger"  );
  call_other( clone, "add_plural", "daggers"  );
  call_other( clone, "set_long", "This is a very long, curving knife with a wickedly sharp point.  It is made of Klatchian steel.\n"  );
  call_other( clone, "set_weight", 12  );
  call_other( clone, "set_length", 18  );
  call_other( clone, "set_width", 3  );
  call_other( clone, "set_material", "steel"  );
  call_other( clone, "set_value", 400  );
  call_other( clone, "new_weapon", 32000  );
  call_other( clone, "set_damage_chance", 5  );
  call_other( clone, "add_attack", "slash", 75, ({ 10, 5, 20 }), "sharp", "sharp"  );
  call_other( clone, "add_attack", "stab", 50, ({ 10, 2, 27 }), "pierce", "pierce" );
   return clone;
} /* create_virtual_ob() */
