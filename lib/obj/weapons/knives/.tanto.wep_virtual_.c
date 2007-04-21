#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/weapon.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/weapons/knives/tanto.wep#1" );
  call_other( clone, "set_name", "tanto"  );
  call_other( clone, "set_short", "tanto"  );
  call_other( clone, "add_alias", "dagger"  );
  call_other( clone, "add_plural", ({"daggers","tantos"})  );
  call_other( clone, "set_main_plural", "tantos"  );
  call_other( clone, "set_long", "This large auriental dagger is an excellent weapon for close-in fighting.  It is short, well crafted and looks pretty durable.\n"  );
  call_other( clone, "set_weight", 9  );
  call_other( clone, "set_length", 10  );
  call_other( clone, "set_width", 3  );
  call_other( clone, "set_material", "steel"  );
  call_other( clone, "set_value", 4000  );
  call_other( clone, "new_weapon", 50000  );
  call_other( clone, "set_damage_chance", 2  );
  call_other( clone, "add_attack", "slash", 75, ({ 0, 2, 40 }), "sharp", "sharp", 0  );
  call_other( clone, "add_attack", "stab", 50, ({ 9, 4, 30 }), "pierce", "pierce", 0 );
   return clone;
} /* create_virtual_ob() */
