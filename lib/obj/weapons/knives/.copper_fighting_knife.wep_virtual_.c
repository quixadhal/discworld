#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/weapon.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/weapons/knives/copper_fighting_knife.wep#1" );
  call_other( clone, "set_name", "knife"  );
  call_other( clone, "set_short", "copper fighting knife"  );
  call_other( clone, "set_main_plural", "copper fighting knives"  );
  call_other( clone, "add_adjective", ({ "copper", "fighting" })  );
  call_other( clone, "set_long", "This Djelian copper knife is designed for fighting.  Its straight blade and hilt are undecorated, clean and functional.\n"  );
  call_other( clone, "set_weight", 9  );
  call_other( clone, "set_length", 10  );
  call_other( clone, "set_width", 2  );
  call_other( clone, "set_material", "copper"  );
  call_other( clone, "set_value", 180  );
  call_other( clone, "new_weapon", 32000  );
  call_other( clone, "add_attack", "slash", 100, ({ 10, 5, 15 }), "sharp", "sharp"  );
  call_other( clone, "add_attack", "stab", 50, ({ 10, 2, 20 }), "pierce", "pierce" );
   return clone;
} /* create_virtual_ob() */
