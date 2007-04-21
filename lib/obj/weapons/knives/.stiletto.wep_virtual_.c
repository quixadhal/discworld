#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/weapon.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/weapons/knives/stiletto.wep#1" );
  call_other( clone, "set_name", "stiletto"  );
  call_other( clone, "set_short", "stiletto"  );
  call_other( clone, "add_alias", "knife"  );
  call_other( clone, "add_adjective", "slender"  );
  call_other( clone, "set_main_plural", "stilettoes"  );
  call_other( clone, "add_plural", "knives"  );
  call_other( clone, "set_long", "This is a slender knife with a dark steel blade, and sharp tip.  "         "It's perfect for slipping between someone's ribs.\n"  );
  call_other( clone, "set_weight", 7  );
  call_other( clone, "set_length", 10  );
  call_other( clone, "set_width", 1  );
  call_other( clone, "set_material",  "steel"  );
  call_other( clone, "set_value", 2000  );
  call_other( clone, "new_weapon",  5000  );
  call_other( clone, "set_damage_chance",  5  );
  call_other( clone, "add_attack",  "stab", 70, ({ 40, 4, 20 }), "pierce", "pierce"  );
  call_other( clone, "add_attack",  "stabstab", 70, ({ 40, 4, 20 }), "pierce", "pierce"  );
  call_other( clone, "set_no_limbs", 1 );
   return clone;
} /* create_virtual_ob() */
