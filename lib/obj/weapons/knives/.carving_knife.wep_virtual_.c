#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/weapon.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/weapons/knives/carving_knife.wep#1" );
  call_other( clone, "set_name", "knife"  );
  call_other( clone, "set_short", "carving knife"  );
  call_other( clone, "add_adjective", ({ "carving" })  );
  call_other( clone, "add_alias", "utensil"  );
  call_other( clone, "add_plural", "utensils"  );
  call_other( clone, "set_long", "This is a knife such as you might use to carve up your Octeday roast, and then use to substantiate your arguments with your spouse as to who should do the washing up.\n"  );
  call_other( clone, "set_weight", 9  );
  call_other( clone, "set_length", 12  );
  call_other( clone, "set_width", 2  );
  call_other( clone, "set_material", "steel"  );
  call_other( clone, "set_value", 500  );
  call_other( clone, "new_weapon", 2000  );
  call_other( clone, "add_attack", "slash", 70, ({ 10, 2, 20 }), "sharp", "sharp", 0  );
  call_other( clone, "add_attack", "stab", 40, ({ 20, 2, 60 }), "pierce", "pierce", 0 );
   return clone;
} /* create_virtual_ob() */
