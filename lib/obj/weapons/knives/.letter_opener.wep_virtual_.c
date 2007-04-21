#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/weapon.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/weapons/knives/letter_opener.wep#1" );
  call_other( clone, "set_name", "opener"  );
  call_other( clone, "set_short", "pearl-handled letter opener"  );
  call_other( clone, "add_adjective", ({ "pearl-handled", "pearl", "handled", "letter" })  );
  call_other( clone, "set_main_plural", "pearl-handled letter openers"  );
  call_other( clone, "set_long", "This letter opener has a lovely pearl handle.  It has a pointy blade that could come in handy in a crunch.\n"  );
  call_other( clone, "set_weight", 4  );
  call_other( clone, "set_length", 8  );
  call_other( clone, "set_width", 1  );
  call_other( clone, "set_material", "steel"  );
  call_other( clone, "set_value", 5000  );
  call_other( clone, "new_weapon", 1600  );
  call_other( clone, "set_damage_chance", 8  );
  call_other( clone, "add_attack", "stab", 40, ({ 30, 3, 10 }), "pierce", "pierce", 0 );
   return clone;
} /* create_virtual_ob() */
