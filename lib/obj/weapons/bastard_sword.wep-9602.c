#include "virtual.h"

void dest_me() { destruct(this_object()); }

void create() {
object clone;

  seteuid((string)"/secure/master"->creator_file(this_object()));
  clone = (object)SERVER->create_virtual_object("/obj/weapon.c", 1);
  clone->add_property("virtual name", "obj/weapons/bastard_sword.wep");
  call_other(clone, "set_name", "sword" );
  call_other(clone, "set_short", "bastard sword" );
  call_other(clone, "add_adjective", ({ "bastard", "bloody", "big" }) );
  call_other(clone, "add_alias", ({  }) );
  call_other(clone, "add_plural", ({  }) );
