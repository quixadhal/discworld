#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/weapon.c", 1 );
  clone->add_property( "virtual name", "/d/learning/newbie/introduction/examples/simple_weapon.wep#1" );
  call_other( clone, "set_name",  "mop"  );
  call_other( clone, "set_short",  "dirty mop"  );
  call_other( clone, "set_long",  "This is a dirty mop, dripping with ooze.\n"  );
  call_other( clone, "add_adjective",  "dirty"  );
  call_other( clone, "set_weight",  20  );
  call_other( clone, "set_value",  2000  );
  call_other( clone, "new_weapon", 10000  );
  call_other( clone, "add_attack",  "prod", 50, ({ 5, 6, 6 }), "blunt", "blunt", 0  );
  call_other( clone, "add_attack",  "whack", 50, ({ 10, 10, 5 }), "blunt", "blunt", 0 );
   return clone;
} /* create_virtual_ob() */
