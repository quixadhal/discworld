#include "move_failures.h"
#include "money.h"
inherit "/std/basic/light";
inherit "/std/basic/move";
/* this will also handle value... */

static int weight;
int value;

create() {
}

void adjust_weight(int w) {
  if (environment())
    environment()->add_weight(w);
  weight += w;
}

void set_weight(int w) {
  if (environment())
    environment()->add_weight(w-weight);
  weight = w;
}
int query_weight() { return weight; }

varargs int adjust_money(mixed amt, string type) {
  int i;

  if (pointerp(amt)) {
    value += (int)MONEY_HAND->query_total_value(amt);
    if (value < 0)
      value = 0;
    return value;
  }
  amt = (int)MONEY_HAND->query_total_value( ({ type, amt }) );
  value += amt;
  if (value < 0)
    value = 0;
  return value;
}

int adjust_value(int i) { return (value += i); }

mixed *query_money_array() { 
  return (mixed *)MONEY_HAND->create_money_array(value);
}

int query_money(string type) { 
  int i;
  mixed *m_a;

  m_a = (mixed *)MONEY_HAND->create_money_array(value); 
  if ((i=member_array(type, m_a)) == -1)
    return 0;
  return m_a[i+1];
}

void set_value(int i) { value = i; }
int query_value() { return value; }

move(mixed dest, mixed messout, mixed messin) {
  int i;
  object from;

  from = environment();
  if (!dest->add_weight(weight))
    return MOVE_TOO_HEAVY;
  i = ::move(dest, messout, messin);
  if (i == MOVE_OK) {
    if (from) {
      from->add_weight(-weight);
      from->adjust_light(-query_light());
    }
    environment()->adjust_light(query_light());
  } else
    dest->add_weight(-weight);
  return i;
}

void dest_me() {
  if (environment()) {
    environment()->add_weight(-weight);
    set_light(0);
  }
  ::dest_me();
}

mixed *query_init_data() {
   return light::query_init_data() +
          move::query_init_data() +
      ({ "weight", weight, "set_weight/p/",
         "value", value, "set_value/p/" });
} /* query_init_data() */
