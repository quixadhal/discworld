#include "move_failures.h"
#include "money.h"
inherit "/failsafe/move";
/* this will also handle value... */

static int weight;
mixed *money_array;

create() {
  money_array = ({ });
}

void set_weight(int w) { weight = w; }
int query_weight() { return weight; }

int adjust_money(int amt, string type) {
  int i;
  if ((i=member_array(type, money_array)) == -1) {
    i = sizeof(money_array);
    money_array += ({ type, amt });
  } else
    money_array[i+1] += amt;
  if (amt <= 0) {
    money_array = delete(money_array, i, 2);
    return 0;
  }
  return money_array[i+1];
}

mixed *query_money_array() { return money_array; }
int query_money(string type) { 
  int i;
 
  if ((i=member_array(type, money_array)) == -1)
    return 0;
  return money_array[i+1];
}

int query_value() { return (int)MONEY_HAND->query_total_value(money_array); }

move(mixed dest, mixed messout, mixed messin) {
  int i;
  object from;

  from = environment();
  if (!dest->add_weight(weight))
    return MOVE_TOO_HEAVY;
  i = ::move(dest, messout, messin);
  if (from && i == MOVE_OK)
    from->add_weight(-weight);
  return i;
}
