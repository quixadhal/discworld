#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/weapon.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/weapons/knives/howondalandish_throwing_knife.wep#1" );
  call_other( clone, "set_name", "knife"  );
  call_other( clone, "add_adjective", ({ "howondalandish", "throwing" })  );
  call_other( clone, "set_short", "howondalandish throwing knife"  );
  call_other( clone, "set_main_plural", "howondalandish throwing knives"  );
  call_other( clone, "set_long", "From one of the more remote Howondalandish tribes, this knife "   "doesn't look throwable, much less a knife.  From a short handle, "   "spiky steel blades jut out at all angles, sure to deliver a nasty "   "shock to whoever gets hit with it.\n"  );
  call_other( clone, "set_weight", 9  );
  call_other( clone, "set_length", 9  );
  call_other( clone, "set_width", 3  );
  call_other( clone, "set_material", "steel"  );
  call_other( clone, "new_weapon", 200  );
  call_other( clone, "set_damage_chance", 10  );
  call_other( clone, "add_attack", "stab", 60, ({ 40, 3, 30 }), "pierce", "pierce", 0  );
  call_other( clone, "add_attack", "slash", 60, ({ 25, 5, 30 }), "sharp", "sharp", 0  );
  call_other( clone, "set_value", 7000 );
   return clone;
} /* create_virtual_ob() */
