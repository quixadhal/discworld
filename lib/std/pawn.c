/* pawn shop to allow people to get money for possessions and get them back
   within a certain period of time. */

inherit "std/room";

#include "shop.h"
#include "money.h"
#include "move_failures.h"

#define VALUE query_value() * 2 / 5

string *people;
object *objs;
int *times;
string store, shop;
int hold_time;
string *buy_mess, *sell_mess, list_mess, value_mess;

void create() {
  people = ({ });
  objs = ({ });
  times = ({ });
  store = "d/am/am/pawnstore";
  shop = "d/am/am/storeroom";
  hold_time = 1800;
  set_short("Pawn Shop");
  set_long(
"You are in a Pawn shop.  You can value, leave and reclaim things here.  "+
"There is a sign describing how the shop works.\n");
  set_light(60);
  buy_mess = ({ "You reclaim $ob$ for $money$.\n",
                "$client$ reclaims $ob$.\n" });
  sell_mess = ({ "You leave $ob$ for $money$.\n",
                 "$client$ leaves $ob$.\n" });
  list_mess = "$ob$\t\t$money$\n";
  value_mess = "The $ob$ would get you $money$.\n";
  ::create();
}

void set_sell_message(mixed str) { sell_mess = str; }
void set_buy_message(mixed str) { buy_mess = str; }
void set_value_message(mixed str) { value_mess = str; }
void set_list_message(mixed str) { list_mess = str; }

mixed query_sell_mess() { return sell_mess; }
mixed query_list_mess() { return list_mess; }
mixed query_value_mess() { return value_mess; }
mixed query_buy_mess() { return buy_mess; }

void dest_me() {
  int loop;

  if(!people) people = ({ });
  for(loop = 0; loop < sizeof(people); loop++) {
    if (!objs[loop]) continue;
    tell_object(find_living(people[loop]), 
"A little person runs up to you and hands you a note, then vanishes again.\n"+
"The note reads:\n"+
"Pawnbroker has gone broke.  " + objs[loop]->query_name() + " sold to shop.\n");
    objs[loop]->move(shop);
  }
  ::dest_me();
}

string pawn_parse(string str, object ob, object client, string money,
                         string extra) {
  string s1,s2,s3,rest;

  rest = "";
  while(sscanf(str,"%s$%s$%s", s1, s2, s3) == 3)
    switch (s2) {
      case "ob" :
        str = s1+ob->short()+s3;
        break;
      case "client" :
        str = s1+client->query_name()+s3;
        break;
      case "extra" :
        str = s1+extra+s3;
        break;
      case "money" :
        str = s1+money+s3;
        break;
      default :
        rest = s1+"$"+s2+"$";
        str = s3;
        break;
    }
  return rest+str;
}

void do_parse(mixed arr, object ob, object client, string money,
                         string extra) {
  if (stringp(arr))
    write(pawn_parse(arr, ob, client, money, extra));
  else {
    write(pawn_parse(arr[0], ob, client, money, extra));
    say(pawn_parse(arr[1], ob, client, money, extra));
  }
}

void init() {
  ::init();
  add_action("read", "read");
  add_action("value_item", "value");
  add_action("leave_item", "leave");
  add_action("reclaim_item", "reclaim");
  add_action("list_items", "list");
}

int read(string str) {
  if(str != "sign") {
     notify_fail("The only thing here to read is a sign.\n");
    return 0;
  }
  write("You can leave things here in exchange for cash.  The value of the "+
        "item determines the amount of cash you can get for it.  If you do "+
        "not return and claim your item within " + 
        (hold_time / 60) +  " minutes, then it "+
        "will be sold to a shop to reclaim its worth.\n"+
        "You can leave <item>, reclaim <item>, list, and value <item> here.\n"+
        "Selling things here gets you less money than the shop, but "+
        "reclaiming the item does not cost as much as buying it back from the "+
        "shop would.\n");
  return 1;
}

void set_storeroom(string str) {
  store = str;
}

string query_storeroom() {
  return store;
}

void set_shop(string str) {
  shop = str;
}

string query_shop() {
  return shop;
}

void set_hold_time(int time) {
  hold_time = time;
}

int query_hold_time() {
  return hold_time;
}

int value_item(string str) {
  object *ob;
  int loop;

  if(!str) {
    notify_fail("Value what?\n");
    return 0;
  }
  ob = find_match(str, this_player());
  if(!sizeof(ob)) {
    notify_fail("You do not have a " + str + ".\n");
    return 0;
  }
  for(loop = 0; loop < sizeof(ob); loop ++) {
    do_parse(value_mess, ob[loop], this_player(),
(string)MONEY_HAND->money_string((mixed *)MONEY_HAND->create_money_array((int)
   "/std/shop"->scaled_value((int)ob[loop]->query_value()) * 4 / 5)), "");
  }
  return 1;
}

int leave_item(string str) {
  object *ob, *left;
  mixed *ma;
  string person;
  int loop, money_to_give, the_time, pos;

  if(!str) {
    notify_fail("Leave what?\n");
    return 0;
  }
  ob = find_match(str, this_player());
  if(!sizeof(ob)) {
    notify_fail("You do not have a " + str + ".\n");
    return 0;
  }
  person = (string)this_player()->query_name();
  the_time = time();
  left = ({ });
  for(loop = 0, money_to_give = 0; loop < sizeof(ob); loop++) {
    int temp, loop2;

    if (environment(ob[loop]) != this_player())
      continue;
    temp = (int)"/std/shop"->scaled_value((int)ob[loop]->query_value()) * 4 / 5;
    if(temp > MAX_AMOUNT) {
       temp = MAX_AMOUNT;
    }
    if (ob[loop]->move(store))
      continue;
    money_to_give += temp;
    people += ({ person });
    objs += ({ ob[loop] });
    times += ({ the_time });
    left += ({ ob[loop] });
    call_out("end_hold", hold_time, ob[loop]);
  }
  ma = (mixed *)MONEY_HAND->create_money_array(money_to_give);
  this_player()->adjust_money(ma);
  say((string)this_player()->query_cap_name() + " leaves " + 
      query_multiple_short(left) + " here.\n");
  write("The pawnbroker gives you " +
       MONEY_HAND->money_string(ma) + ".\n");
  return 1;
}

void end_hold(object ob) {
  int pos;

  if(!ob) {
    return;
  }
  pos = member_array(ob, objs);
  ob->move(shop);
  if(pos == -1) {
    return 0;
  }
  tell_object(find_living(people[pos]), "Time limit for " + ob->query_name() + " has expired.\n");
  people = delete(people, pos, 1);
  objs = delete(objs, pos, 1);
  times = delete(times, pos, 1);
}

int reclaim_item(string str) {
  object *ob;
  int loop, money_to_give;

  if(!str) {
    notify_fail("reclaim what?\n");
    return 0;
  }
  ob = find_match(str, store);
  if(!sizeof(ob)) {
    notify_fail("There are no " + str + "'s here.\n");
    return 0;
  }
  for(loop = 0; loop < sizeof(ob); loop++) {
    int pos, amt, ob_amt;

    pos = member_array(ob[loop], objs);
    if(pos != -1) {
      amt = (int)this_player()->query_value();
      ob_amt = (int)"/std/shop"->scaled_value((int)ob[loop]->query_value());
      if (ob_amt < amt) {
        amt -= ob_amt;
        money_to_give -= ob_amt;
        do_parse(buy_mess, ob[loop], this_player(),
(string)MONEY_HAND->money_string((mixed *)MONEY_HAND->create_money_array(ob_amt)),
                "");
        if(ob[loop]->move(this_player()) != MOVE_OK) {
          ob[loop]->move(this_object());
          write("It is too heavy for you and he puts it down in front of you.\n");
        }
        objs = delete(objs, pos, 1);
        people = delete(people, pos, 1);
        times = delete(times, pos, 1);
      }
      else {
        write("You cannot afford to reclaim "+
              (string)objs[pos]->query_name()+".\n");
      }
    }
  }
  write("This is costing you " + money_to_give + "\n");
  this_player()->pay_money(({ "brass", -money_to_give }) );
  return 1;
}

int list_items() {
  int loop, flag;
  string person;

  person = (string)this_player()->query_name();
  for(flag = loop = 0; loop < sizeof(objs); loop++) {
    if(people[loop] == person) {
      flag = 1;
      do_parse(list_mess, objs[loop], this_player(),
(string)MONEY_HAND->money_string((mixed *)MONEY_HAND->create_money_array(
    (int)"/std/shop"->scaled_value((int)objs[loop]->query_value()))), "");
    }
  }
  if(!flag) {
    write("Nothing left here by you.\n");
  }
  return 1;
}
