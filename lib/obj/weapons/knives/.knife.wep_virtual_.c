#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/weapon.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/weapons/knives/knife.wep#1" );
  call_other( clone, "set_name", "knife"  );
  call_other( clone, "set_short", "knife"  );
  call_other( clone, "add_adjective", ({ "medium-sized", "medium" })  );
  call_other( clone, "set_main_plural", "knives"  );
  call_other( clone, "set_long", "This is a medium-sized knife, the sort of thing someone might keep in a boot to ward off a would-be attacker.  You just have to hope that it's not then used to prepare your dinner.\n"  );
  call_other( clone, "set_weight", 8  );
  call_other( clone, "set_length", 7  );
  call_other( clone, "set_width", 2  );
  call_other( clone, "set_material",  "steel"  );
  call_other( clone, "set_value", 100  );
  call_other( clone, "new_weapon", 9000  );
  call_other( clone, "add_attack",  "slash", 70, ({ 10, 3, 20 }), "sharp", "sharp", 0  );
  call_other( clone, "add_attack",  "stab", 40, ({ 15, 3, 30 }), "pierce", "pierce", 0 );
   return clone;
} /* create_virtual_ob() */
