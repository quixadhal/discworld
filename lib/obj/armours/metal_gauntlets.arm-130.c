#include "virtual.h"

void dest_me() { destruct(this_object()); }

void create() {
object clone;

  seteuid((string)"/secure/master"->creator_file(this_object()));
  clone = (object)SERVER->create_virtual_object("/obj/armour.c", 1);
  clone->add_property("virtual name", "obj/armours/metal_gauntlets.arm");
  call_other(clone, "set_name", "gauntlets" );
  call_other(clone, "set_short", "pair of metal gauntlets" );
  call_other(clone, "add_adjective", ({ "pairs of metal", "pairs of" }) );
  call_other(clone, "add_alias", ({  }) );
  call_other(clone, "add_plural", ({ "pairs of metal gauntlets", "pairs of gauntlets" }) );
  call_other(clone, "set_long",  "This is a pair of gauntlets made from lots of pieces of metal painstakingly joined together by some poor soul.  They are relatively heavy and restrict your movement somewhat.  " );
  call_other(clone, "set_weight",  40 );
  call_other(clone, "set_value",  150 );
  call_other(clone, "setup_armour",  1000 );
  call_other(clone, "set_dex_minus",  1 );
  call_other(clone, "add_immune", ({  }) );
  call_other(clone, "set_type",  "glove" );
  call_other(clone, "set_damage_chance",  5 );
  call_other(clone, "add_ac",  "sharp", "sharp", 100 );
