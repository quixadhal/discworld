#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/weapon.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/weapons/knives/black_handled_ceremonial_bronze_knife.wep#1" );
  call_other( clone, "set_name", "knife"  );
  call_other( clone, "set_short", "black handled ceremonial bronze knife"  );
  call_other( clone, "add_adjective", ({ "black", "handled", "ceremonial", "bronze" })  );
  call_other( clone, "set_main_plural", "black handled ceremonial bronze knives"  );
  call_other( clone, "add_plural", "knives"  );
  call_other( clone, "set_long", "This is a ceremonial knife, for use in magic rituals.  Its ornate handle is made of ebony and its long bronze blade has been carved with mystic runes.\n"  );
  call_other( clone, "set_weight", 11  );
  call_other( clone, "set_length", 13  );
  call_other( clone, "set_width", 4  );
  call_other( clone, "set_material", "copper"  );
  call_other( clone, "set_value", 1000  );
  call_other( clone, "new_weapon", 2000  );
  call_other( clone, "add_attack", "slash", 70, ({ 10, 2, 20 }), "sharp", "sharp", 0  );
  call_other( clone, "add_attack", "stab", 40, ({ 20, 2, 60 }), "pierce", "pierce", 0 );
   return clone;
} /* create_virtual_ob() */
