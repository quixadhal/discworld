#include "virtual.h"

void dest_me() { destruct(this_object()); }

void create() {
object clone;

  seteuid((string)"/secure/master"->creator_file(this_object()));
  clone = (object)SERVER->create_virtual_object("/obj/armour.c", 1);
  clone->add_property("virtual name", "obj/armours/platemail.arm");
  call_other(clone, "set_name", "platemail" );
  call_other(clone, "set_short", "platemail" );
  call_other(clone, "add_adjective", ({  }) );
  call_other(clone, "add_alias", ({  }) );
  call_other(clone, "add_plural", ({  }) );
  call_other(clone, "set_long",  "Made of a series of interlocking plate things all over the place.  Looks horribely complicated.  " );
  call_other(clone, "set_weight",  400 );
  call_other(clone, "set_value",  900 );
  call_other(clone, "setup_armour",  25000 );
  call_other(clone, "set_dex_minus",  -2 );
  call_other(clone, "add_immune", ({  }) );
  call_other(clone, "set_type",  "armour" );
  call_other(clone, "set_damage_chance",  6 );
  call_other(clone, "add_ac",  "first", "sharp", 80 );
  call_other(clone, "add_ac",  "second", "blunt", 85 );
