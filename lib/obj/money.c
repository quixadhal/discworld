inherit "/std/object";
#include "move_failures.h"
#include "money.h"


mixed *money_array;
static int dont_join;

void create() {
  money_array = ({ });
  ::create();
  set_name("coin");
  add_alias("Some Money For Me");
  add_plural("coins");
  set_main_plural("coins");
  add_property("determinate", "");
  add_property("no one",1);
  add_property("money",1);
}

/*
void init() {
  add_action("do_help", "help");
}
 */

int do_help(string str) {
  mixed *values;
  int i;

  if (!str || str != "money")
    return 0;
  write("Money values.\n");
  values = (mixed *)MONEY_HAND->query_values();
  str = "";
  for (i=0;i<sizeof(values);i+=2)
    str += sprintf("%10-s : %4d\n", values[i], values[i+1]);
  printf("%#-*s\n", this_player()->query_cols(), str);
  return 1;
}

varargs int adjust_money(mixed amt, string type) {
  int i, j, val;
  
  if (pointerp(amt)) {
    for (i=0;i<sizeof(amt);i+=2)
      adjust_money(amt[i+1], amt[i]);
    return 1;
  }
  if (!stringp(type) || !intp(amt))
    return 0;
  if ((i=member_array(type, money_array)) == -1) {
    add_adjective(type);
/* order - highest value to the left of money_array */
    for(i=0; i<sizeof(money_array); i+=2) {
      val = (int)MONEY_HAND->query_value(money_array[i]);
      if((int)MONEY_HAND->query_value(type) > val) break;
    }

    money_array = money_array[0..(i-1)] + ({ type, amt }) +
       money_array[i..sizeof(money_array)];
  } else {
    money_array[i+1] += amt;
    if (money_array[i+1] <= 0) {
      remove_adjective(type);
      money_array = delete(money_array,i,2);
      return 0;
    }
  }
  return money_array[i+1];
}

mixed *query_money_array() { return money_array; }

void set_money_array(mixed *arr) { 
  int i;

  money_array = arr;
  adjectives = ({ }); /* clear them... */
  for (i=0;i<sizeof(money_array);i+=2)
    add_adjective(money_array[i]);
}

string *half_short() {
  string *retval;
  int i;

  retval = ({ });
  for (i=0;i<sizeof(money_array);i+=2)
    if (money_array[i+1]>0)
      if (this_player() != environment() && money_array[i+1] > 10)
        retval += ({ "Some "+money_array[i] });
      else
        retval += ({ money_array[i+1]+" "+
                     money_array[i] });
  return retval;
}

string short(int dark) {
  mixed retval;

  retval = half_short();
  if (!sizeof(retval))
    return 0;
  if (sizeof(retval) == 1)
  {
    if (money_array[sizeof(money_array)-1] == 1) return retval[0] +" coin";
    return retval[0]+" coins";
  }
  return implode(retval[0..sizeof(retval)-2], ", ")+
         " and "+retval[sizeof(retval)-1] + " coins";
}
/*
string *pretty_short(int dark) {
  return half_short();
}
*/
object new_money_object(int num, string type) {
  int i;
  object ob;

  if (!sizeof(money_array))
    return 0;
  if (!type)
    i = 0;
  else if ((i = member_array(type, money_array)) == -1)
    return 0;
  if (num > money_array[i+1])
    num = money_array[i+1];
  ob = clone_object(MONEY_OBJECT);
  ob->adjust_money(num, type);
  money_array[i+1] -= num;
  if (money_array[i+1] <= 0)
  {
    remove_adjective(type);
    money_array = delete(money_array, i, 2);
  }
  return ob;
}

varargs int move(mixed dest, mixed messin, mixed messout) {
  mixed i;
  int j;
  object ob, mon;

  j = (int)::move(dest, messin, messout);
  if (j != MOVE_OK)
    return j;
  if (objectp(dest))
    ob = dest;
  else {
    ob = find_object(dest);
    if (!ob) {
      call_other(ob, "??");
      ob = find_object(dest);
    }
  }
  remove_alias("Some Money For Me");
  remove_call_out("move");
  if (ob && (mon = present("Some Money For Me", ob)) && !dont_join) {
    for (j=0;j<sizeof(money_array);j+=2)
      mon->adjust_money(money_array[j+1], money_array[j]);
    dont_join = 1;
    move("/room/void");
    call_out("dest_me",0);
    return MOVE_OK;
  }
  add_alias("Some Money For Me");
  return MOVE_OK;
}

object query_parse_id(mixed *arr) {
  int i, j, num;
  string s1, s2, *bit;
  object ob;

  if (arr[0] == 0) {
    bit = explode(arr[1], " ");
    if (bit[sizeof(bit)-1] != "coins")
      if (environment() != this_player() && sizeof(money_array))
        return this_object();
      else
        return 0;
    if (sizeof(bit) == 1)
      return this_object();
    i = member_array(bit[0], money_array);
    if (i == -1)
      return 0;
    num = money_array[i+1];
    if (!num) return 0;
    s1 = money_array[i];
    money_array = delete(money_array, i, 2);
    ob = clone_object(MONEY_OBJECT);
    ob->set_money_array(({ s1, num }));
    ob->do_move_call_out(environment());

    return ob;
  }
/* assume if they are after a particular one they are not interested in
 * us... */
  if (arr[0] <= 0 || !sizeof(money_array))
    return 0;
  i = -1;
  for (j=0;j<sizeof(money_array);j+=2)
    if (sscanf(arr[1], "%s"+money_array[j]+"%s",s1,s2))
      i = j;
  if (i == -1) {
    i = member_array("copper",money_array);
    if (i== -1)
      i = 0;
  }
  if (arr[0] > money_array[i+1])
    num = money_array[i+1];
  else
    num = arr[0];
  if (!num) return 0;
  money_array[i+1] -= num;
  arr[0] -= num;
  ob = clone_object(MONEY_OBJECT);
  ob->set_money_array( ({ money_array[i], num }) );
  ob->do_move_call_out(environment());
  return ob;
}

/* you cannot sell money... */
int do_not_sell() { return 1; }
void do_move_call_out(object ob) { call_out("move",0,ob); }

int query_value() { return (int)MONEY_HAND->query_total_value(money_array); }

int query_number_coins()
{
  int i, tot;

  if(!sizeof(money_array)) return 0;
  for(i=1;i<sizeof(money_array);i+=2)
    tot += money_array[i];
  return tot;
}
