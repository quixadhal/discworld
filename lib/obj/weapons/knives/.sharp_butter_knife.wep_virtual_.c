#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/weapon.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/weapons/knives/sharp_butter_knife.wep#1" );
  call_other( clone, "set_name", "knife"  );
  call_other( clone, "set_short", "sharp butter knife"  );
  call_other( clone, "set_main_plural", "sharp butter knives"  );
  call_other( clone, "add_adjective", ({ "sharp", "butter" })  );
  call_other( clone, "add_alias", ({ "utensil" })  );
  call_other( clone, "add_plural", "knives"  );
  call_other( clone, "set_long",  "This is a butter knife that has been sharpened a bit.\n"  );
  call_other( clone, "set_weight",  3  );
  call_other( clone, "set_length",  6  );
  call_other( clone, "set_width",  1  );
  call_other( clone, "set_material",  "steel"  );
  call_other( clone, "set_value",  75  );
  call_other( clone, "new_weapon",  500  );
  call_other( clone, "add_attack",  "slash", 70, ({ 15, 2, 20 }), "sharp", "sharp", 0  );
  call_other( clone, "add_attack",  "stab", 40, ({ 20, 2, 30 }), "pierce", "pierce", 0 );
   return clone;
} /* create_virtual_ob() */
