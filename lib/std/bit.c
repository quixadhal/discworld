#include "corpse.h"
inherit "std/object";

#include <bit.h>
#define DECAY_TIME 60

mixed *bit_data;
object race_ob;
int corpse_weight;
string race_name;
int decay, cured;

void create() {
   ::create();

   set_short("anonymous bit");
   set_long("This is an unknown bit of some creature.\n");
   corpse_weight = STD_CORPSE_WEIGHT;
   set_weight(5);
   set_name("bit");
   add_property("cureable", 1);
} /* create() */

void init() {
  this_player()->add_command("eat",this_object());
} /* init() */

int do_eat() {
  int i;

  i = (int)race_ob->query_eat(bit_data[BIT_NAME]);
  if (i)
    call_out("dest_me",0);
  return i;
} /* do_eat() */

void set_race_ob(object s) {
   race_ob = s;
} /* set_race_ob() */

void set_race_name(string s) {
   race_name = s;
} /* set_race_name() */

void setup_long() {
  if (decay > 80) {
   if (race_name) {
      set_short(race_name + " " + bit_data[BIT_NAME]);
      set_long("This is a fresh " + (bit_data[BIT_NAME]) + 
         " severed from the " +
         "corpse of " + add_a(race_name) + ".\n");
   } else {
      set_short(bit_data[BIT_NAME]);
      set_long("This is a fresh " + (bit_data[BIT_NAME]) +
         " severed from the " +
         "corpse of an unknown creature.\n");
   }
  } else if (decay > 50) {
   if (race_name) {
      set_short(race_name + " " + bit_data[BIT_NAME]);
      set_long("This is " + add_a(bit_data[BIT_NAME]) + " severed from the " +
         "corpse of " + add_a(race_name) + ".\n");
   } else {
      set_short(bit_data[BIT_NAME]);
      set_long("This is " + add_a(bit_data[BIT_NAME]) + " severed from the " +
         "corpse of an unknown creature.\n");
   }
  } else if (decay > 30) {
   if (race_name) {
      set_short(race_name + " " + bit_data[BIT_NAME]);
      set_long("This is the partially decayed remains of " +
         add_a(bit_data[BIT_NAME]) + " severed from the " +
         "corpse of " + add_a(race_name) + ".\n");
   } else {
      set_short(bit_data[BIT_NAME]);
      set_long("This is the partially decayed remains of " +
         add_a(bit_data[BIT_NAME]) + " severed from the " +
         "corpse of an unknown creature.\n");
   }
  } else {
   if (race_name) {
      set_short(race_name + " " + bit_data[BIT_NAME]);
      set_long("This is the almost unrecognizable remains of " +
         add_a(bit_data[BIT_NAME]) + " severed from the " +
         "corpse of " + add_a(race_name) + ".\n");
   } else {
      set_short(bit_data[BIT_NAME]);
      set_long("This is the almost unrecognizable remains of " +
         add_a(bit_data[BIT_NAME]) + " severed from the " +
         "corpse of an unknown creature.\n");
   }
  }
} /* setup_long() */

void set_corpse_weight(int i) {
   corpse_weight = i;
} /* set_corpse_weight() */

void set_bit(string s, int dec) {
   bit_data = (mixed *)race_ob->query_bit(s);
   if (!bit_data || !sizeof(bit_data)) return;
   if (race_name)
     add_adjective(race_name);
   else
     add_adjective("unknown");
   add_alias(bit_data[BIT_NAME]);
   if (bit_data[BIT_ALIAS] && strlen(bit_data[BIT_ALIAS]))
      add_alias(bit_data[BIT_ALIAS]);
   set_weight(bit_data[BIT_EXTRA][EXTRA_WEIGHT] * corpse_weight  / STD_CORPSE_WEIGHT);
   decay = dec;
   setup_long();
   BIT_CONTROLLER->add_bit(this_object());
   return;
} /* set_bit() */

int do_decay() {
  if (!environment()) {
    dest_me();
    return 0;
  }
  if (living(environment())) {
    decay -= 5;
  } else
    decay -= (int)environment()->decay_rate();
  if (decay < 0) {
    dest_me();
    return 0;
  }
  if (decay == 80 || decay == 50 || decay == 30)
    setup_long();
  return 1;
} /* do_decay() */

object query_race_ob() {
   return race_ob;
} /* query_race_ob() */

string query_race_name() {
   return race_name;
} /* query_race_name() */

mixed query_bit_data() {
   return bit_data;
} /* query_bit_datae() */

void remove_bit(string s) {
   int i;

   for (i = EXTRA_SUBBIT; i < sizeof(bit_data[BIT_EXTRA]); i++) {
      if (bit_data[BIT_EXTRA][i] == s) {
          bit_data[BIT_EXTRA] = delete(bit_data[BIT_EXTRA], i, 1);
          i--;
      }
   }
} /* remove_bit() */

object *find_inv_match(string s) {
   int i;
   mixed *nbit;
   object nbitobj;

   for (i = EXTRA_SUBBIT; i < sizeof(bit_data[BIT_EXTRA]); i++) {
      nbit = (mixed *)race_ob->query_bit(bit_data[BIT_EXTRA][i]);
      if (!nbit || !sizeof(nbit)) continue;
      if (nbit[BIT_NAME] == s || nbit[BIT_ALIAS] == s) {
         nbitobj = clone_object("std/bit");
         nbitobj->set_race_ob(race_ob);
         nbitobj->set_race_name(race_name);
         nbitobj->set_corpse_weight(corpse_weight);
         nbitobj->set_bit(nbit[BIT_NAME], decay);
         remove_bit(nbit[BIT_NAME]);
         return ({ nbitobj });
      }
   }
   return ({ });
} /* find_inv_match() */

int query_decay() { return decay; }

void dest_me() {
  BIT_CONTROLLER->remove_bit(this_object());
  ::dest_me();
} /* dest_me() */

mixed query_static_auto_load() {
  return int_query_static_auto_load();
} /* query_static_auto_load() */

mixed query_dynamic_auto_load() {
  return ({ bit_data, race_ob, corpse_weight, race_name, decay, cured,
            ::query_dynamic_auto_load() });
} /* query_dynamic_auto_load() */

void init_dynamic_arg(mixed arg) {
  if (mappingp(arg)) {
    ::init_dynamic_arg(arg);
    return ;
  }
  bit_data = arg[0];
  race_ob = arg[1];
  corpse_weight = arg[2];
  race_name = arg[3];
  decay = arg[4];
  cured = arg[5];
  ::init_dynamic_arg(arg[6]);
  if (!cured)
    BIT_CONTROLLER->add_bit(this_object());
  setup_long();
} /* init_dynamic_arg() */

void do_cure() {
  BIT_CONTROLLER->remove_bit(this_object());
} /* do_cure() */
