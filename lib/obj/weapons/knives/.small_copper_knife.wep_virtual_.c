#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/weapon.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/weapons/knives/small_copper_knife.wep#1" );
  call_other( clone, "set_name", "knife"  );
  call_other( clone, "set_short", "small copper knife"  );
  call_other( clone, "set_main_plural", "small copper knives"  );
  call_other( clone, "add_adjective", ({ "small", "copper" })  );
  call_other( clone, "set_long", "This is a small, Djelian, copper knife.  Its straight blade and hilt are undecorated, clean and functional.\n"  );
  call_other( clone, "set_weight", 5  );
  call_other( clone, "set_length", 8  );
  call_other( clone, "set_width", 3  );
  call_other( clone, "set_material",  "copper"  );
  call_other( clone, "set_value", 100  );
  call_other( clone, "new_weapon", 16000  );
  call_other( clone, "add_attack", "slash", 100, ({ 10, 5, 10 }), "sharp", "sharp"  );
  call_other( clone, "add_attack", "stab", 50, ({ 10, 2, 15 }), "pierce", "pierce" );
   return clone;
} /* create_virtual_ob() */
