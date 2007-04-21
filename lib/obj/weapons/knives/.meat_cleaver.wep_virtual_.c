#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/weapon.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/weapons/knives/meat_cleaver.wep#1" );
  call_other( clone, "set_name", "cleaver"  );
  call_other( clone, "set_short", "meat cleaver"  );
  call_other( clone, "add_alias", ({ "utensil", "knife" })  );
  call_other( clone, "add_adjective", "meat"  );
  call_other( clone, "set_main_plural", "meat cleavers"  );
  call_other( clone, "add_plural", ({ "utensils", "knives" })  );
  call_other( clone, "set_long", "This is a butcher's implement for cutting animal carcasses into joints or pieces.  It is a large, rectangular knife and is quite heavy.\n"  );
  call_other( clone, "set_weight", 16  );
  call_other( clone, "set_length",  11  );
  call_other( clone, "set_width", 5  );
  call_other( clone, "set_material",  "steel"  );
  call_other( clone, "set_value", 400  );
  call_other( clone, "new_weapon", 750  );
  call_other( clone, "set_damage_chance", 5  );
  call_other( clone, "add_attack", "slash", 50, ({ 10, 4, 10 }), "sharp", "sharp", 0  );
  call_other( clone, "add_attack", "hack", 90, ({ 20, 3, 20 }), "sharp", "sharp", 0 );
   return clone;
} /* create_virtual_ob() */
