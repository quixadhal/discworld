#include "corpse.h"
inherit "std/container";
inherit "std/living/carrying";
 
#include <bit.h>

#define DECAY_TIME      60
 
string owner, race_ob, race_name;
string *bits_gone;
int decay;
/* for nicer formatting of exa corpse */
object weapon, *armours;

void set_race_ob(string race_ob);

int query_corpse() { return 1; }
string query_owner() { return owner; }

int prevent_insert() {
  write("The corpse is too big.\n");
  return 1;
}

string query_name() {
  if (!::query_name())
    return "someone";
  return ::query_name();
}
 
void setup() {
  bits_gone = ({ });
  add_property("cureable", 1);
  owner = "noone";
  race_name = "womble";
  decay = 10;
  add_plural("corpses");
  add_alias("corpse");
  set_short("corpse");
  set_long("A corpse, it looks dead.\n");
  set_weight(STD_CORPSE_WEIGHT);
  set_race_ob("/std/races/unknown");
}
 
void set_owner(string n, object ob) {
  owner = n;
  set_name("corpse");
  add_alias("corpse of " + n);
  set_short("corpse of " + capitalize(n));
  if (ob)
    set_main_plural("corpses of "+ob->query_main_plural());
  else
    set_main_plural("corpses of "+pluralize(n));
  add_plural("corpses");
  set_long("This is the dead body of "+capitalize(n)+".\n");
  if (ob && ob->query_weight()) set_weight(ob->query_weight());
  else set_weight(STD_CORPSE_WEIGHT);
  call_out("decay", DECAY_TIME);
}

void set_race_name(string str) { race_name = str; }
string query_race_name() { return race_name; }

void decay() {
object ob,ob2;
  decay -= 1;
  if (!race_name)
    race_name = (string)race_ob->query_name();
  if (decay == 7) {
    set_short("somewhat decayed remains of "+query_name());
    set_main_plural("somewhat decayed remains of "+query_name());
  }
  if (decay == 5) {
    set_short("decayed remains of "+race_name);
    set_main_plural("decayed remains of "+race_name);
    set_long("This is the dead body of "+add_a(race_name)+".\n");
  }
  if (decay > 0) {
    call_out("decay", DECAY_TIME);
    return ;
  }
/* ok destroys all the things in the corpse */
  transfer_all_to(environment());
  ob = first_inventory(this_object());
  while (ob) {
    ob2 = next_inventory(ob);
    ob->dest_me();
    ob = ob2;
  }
  destruct(this_object());
}
 
set_race_ob(s)
{
   race_ob = s;
}

query_race_ob()
{
   return race_ob;
}

find_inv_match(s)
{
   string bit;
   object bitobj, weap;
   int i,j;

   bit = race_ob->query_possible_bits(s);
   if (!bit || !sizeof(bit)) return all_inventory();

   bit -= bits_gone;	/* take out of all possible bits the bits_gone */
   if (!sizeof(bit)) return bit;

   weap = (object) this_player()->query_weapon();
   if(!weap) {
     write("You can't cut bits from a corpse with your bare hands.\n");
     return ({ });
   } else if(!weap->id("dagger") && !weap->id("knife")) {
     write("You can only cut things from a corpse with a knife or dagger.\n");
     return ({ });
   }
   bit = race_ob->query_bit(bit[0]);
   bitobj = clone_object("/std/bit");
   bitobj->set_race_ob(race_ob);
   if (race_name) bitobj->set_race_name(race_name);
   else bitobj->set_race_name(race_ob->query_name());
   bitobj->set_corpse_weight(query_weight());
   bitobj->set_bit(bit[0], decay*10);
   bitobj->set_decay_bit(bit[0]);
   for (i = 0; i < sizeof(bits_gone); i++) {
      j = member_array(bits_gone[i], bit[BIT_EXTRA]);
      if (j >= 0)
         bitobj->remove_bit(bits_gone[i]);
   }
   bits_gone += ({ bit[BIT_NAME] }) + bit[BIT_EXTRA][3..50];
   if (s == "head") {
      set_long(query_long() + "It is decapitated.\n");
      set_short("decapitated corpse of " + capitalize(owner));
   }
   if (environment())
     bitobj->move(environment());
   return ({ bitobj });
}

query_bits_gone() { return bits_gone; }

/* this for formatting of objects sake */
object *query_armours() { return armours + ({ }); }
void set_armours(object *arm) { armours = arm; }
void remove_armour(object arm) { armours -= ({ arm }); }

object query_weapon() { return weapon; }
void set_weapon(object wpn) { weapon = wpn; }
void unwield_weapon(object wpn) { if(weapon == wpn) weapon = 0; }


string query_possessive() { return "its"; }

