#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/weapon.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/weapons/knives/small_knife.wep#1" );
  call_other( clone, "set_name", "knife"  );
  call_other( clone, "set_short", "small knife"  );
  call_other( clone, "add_adjective", ({ "small" })  );
  call_other( clone, "add_alias", "utensil"  );
  call_other( clone, "add_plural", "utensils"  );
  call_other( clone, "set_long", "This small knife might be the sort of thing " "you'd use to peel an apple, slice carrots or " "stab in someone's kidney.  You could say " "that it's a multi-purpose utensil.\n"  );
  call_other( clone, "set_weight", 6  );
  call_other( clone, "set_length", 7  );
  call_other( clone, "set_width", 1  );
  call_other( clone, "set_material",  "steel"  );
  call_other( clone, "set_value", 50  );
  call_other( clone, "new_weapon", 500  );
  call_other( clone, "add_attack", "slash", 70, ({ 5, 2, 20 }), "sharp", "sharp", 0  );
  call_other( clone, "add_attack", "stab", 40, ({ 10, 2, 30 }), "pierce", "pierce", 0 );
   return clone;
} /* create_virtual_ob() */
