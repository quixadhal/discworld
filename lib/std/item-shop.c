#include "armoury.h"
#include "money.h"
inherit "/std/room";

object cont;

object query_cont() { return cont; }

#define MAX_PROP "max per reset"
#define TYPE "item shop type"
#define NUM_SOLD "number sold this reset"

void reset() {
  object ob;

  if (!cont)
    cont = clone_object("/std/container");
  ob = first_inventory(cont);
  while (ob) {
    ob->remove_property(NUM_SOLD);
    ob = next_inventory(ob);
  }
}

/* ps 0, means infinite */
int add_armour(string name, int max_per_reset) {
  object ob;

  if (!cont)
    cont = clone_object("/std/container");
  if (!(ob = (object)this_object()->create_object(name)))
    ob = (object)ARMOURY->request_armour(name,100);
  if (ob) {
    ob->move(cont);
    ob->add_property("armoury name", name);
    ob->add_property(MAX_PROP, max_per_reset);
    ob->add_property(TYPE, "armour");
    return 1;
  }
  return 0;
}

int add_weapon(string name, int max_per_reset) {
  object ob;

  if (!cont)
    cont = clone_object("/std/container");
  if (!(ob = (object)this_object()->create_object(name)))
    ob = (object)ARMOURY->request_weapon(name,100);
  if (ob) {
    ob->move(cont);
    ob->add_property("armoury name", name);
    ob->add_property(MAX_PROP, max_per_reset);
    ob->add_property(TYPE, "weapon");
    return 1;
  }
  return 0;
}

int add_object(string where, int max) {
  object ob;

  if(!cont)
    cont = clone_object("std/container");
  if (!(ob = (object)this_object()->create_object(where)))
    ob = clone_object(where);
  if(ob) {
    ob->add_property(TYPE, "object");
    ob->add_property(MAX_PROP, max);
    ob->add_property("armoury name", where);
    ob->move(cont);
    return 1;
  }
  return 0;
}


void init() {
  ::init();
  add_action("do_list", "list");
  add_action("do_browse", "browse");
  add_action("do_buy", "buy");
}

int query_cost(object ob) {
  return (int)ob->query_value();
}

int do_list(string str) {
  int i, num_left, max;
  string weap, arm, obj;
  object ob;

  weap = arm = obj = "";
  ob = first_inventory(cont);
  while(ob) {
    if(max = (int)ob->query_property(MAX_PROP)) {
      num_left = max - (int)ob->query_property(NUM_SOLD);
      if(num_left <= 0) {
        ob = next_inventory(ob);
        continue;
      }
    } else {
      num_left = -1;
    }
    switch (ob->query_property(TYPE)) {
      case "armour" :
        arm += "   "+capitalize((string)ob->short())+" for "+
           MONEY_HAND->money_value_string(this_object()->query_cost(ob));
        if(num_left == -1)
          arm += " (plenty left).\n";
        else
          arm += " (" + query_num(num_left, 0) + " left).\n";
        break;
      case "weapon" :
        weap += "   "+capitalize((string)ob->short())+" for "+
           MONEY_HAND->money_value_string(this_object()->query_cost(ob));
        if(num_left == -1)
          weap += " (plenty left).\n";
        else
          weap += " (" + query_num(num_left, 0) + " left).\n";
        break;
      case "object" :
        obj += "   "+capitalize((string)ob->short())+" for "+
           MONEY_HAND->money_value_string(this_object()->query_cost(ob));
        if(num_left == -1)
          obj += " (plenty left).\n";
        else
          obj += " (" + query_num(num_left, 0) + " left).\n";
        break;
    }
    ob = next_inventory(ob);
  }
  if (arm != "")
    printf("%=-*s", (int)this_player()->query_cols(), "Armours:\n"+arm);
  if (weap != "")
    printf("%=-*s", (int)this_player()->query_cols(), "Weapons:\n"+weap);
  if (obj != "")
    printf("%=-*s", (int)this_player()->query_cols(), "Items:\n"+obj);
  if (obj == "" && arm == "" && weap == "") {
    notify_fail("Nothing here to buy.\n");
    return 0;
  }
  return 1;
}

int do_buy(string str) {
  object *obs;
  int i, num, max;
  object ob;

  if (!str) {
    notify_fail("You must buy something.\n");
    return 0;
  }
  obs = find_match(str, cont);
  notify_fail("Buy something that is on the menu.\n");
  if (!sizeof(obs))
    return 0;
  for (i=0;i<sizeof(obs);i++) {
    ob = 0;
    if ((max = (int)obs[i]->query_property(MAX_PROP)) &&
        (max <= (num = (int)obs[i]->query_property(NUM_SOLD)))) {
      write("We are sold out of "+obs[i]->query_plural()+".\n");
      continue;
    }
    if (!(ob = (object)this_object()->create_object(obs[i]->query_property("armoury name"))))
      switch (obs[i]->query_property(TYPE)) {
        case "weapon" :
          ob = (object)ARMOURY->request_weapon(obs[i]->query_property("armoury name"),80+random(20));
          break;
        case "armour" :
          ob = (object)ARMOURY->request_armour(obs[i]->query_property("armoury name"),80+random(20));
          break;
        case "object" :
          ob = clone_object((string)obs[i]->query_property("armoury name"));
          break;
        }
    if (!ob) {
      string tmpstr;

      sscanf(file_name(obs[i]), "%s#", tmpstr);
      ob = clone_object(tmpstr);
    }
    if (this_object()->query_cost(ob) > this_player()->query_value()) {
      write("You cannot afford to buy "+ob->short()+".\n");
      continue;
    }
    if (ob->move(this_player())) {
      write("You cannot pick "+ob->short()+" up.\n");
      ob->dest_me();
      continue;
    }
    if(max) obs[i]->add_property(NUM_SOLD, num + 1);
    this_player()->pay_money(MONEY_HAND->create_money_array(
      this_object()->query_cost(ob)));
    write("You buy "+ob->short()+".\n");
    say(this_player()->query_cap_name()+" buys "+ob->short()+".\n");
  }
  return 1;
}

void dest_me() {
  int i;
  object *obs;

  if (cont) {
    obs = all_inventory(cont);
    for (i=0;i<sizeof(obs);i++)
      obs[i]->dest_me();
    cont->dest_me();
  }
  ::dest_me();
}

int do_browse(string str)
{
  object *obs;
  string s;
  int i;

  obs = find_match(str, cont);
  if(!sizeof(obs)) {
    notify_fail("Browse what item?\n");
    return 0;
  }
  s = "";
  for(i=0; i<sizeof(obs); i++) {
    s += "The " + (string)obs[i]->short() + " is priced at " +
      (string)MONEY_HAND->money_value_string(query_cost(obs[i])) +
      " and looks like:\n" + (string)obs[i]->long();
  }
  write(s);
  return 1;
}
