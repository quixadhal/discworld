#include "money.h"
inherit "/std/room";

void init() {
  ::init();
  add_action("change", "ch*ange");
}

int change(string str) {
  string s1, type;
  object *obs, cont, ob;
  mixed *values,*change_arr;
  int val, mon, i;

  notify_fail("CHange is not a well beast.\n");
  notify_fail("Syntax: "+query_verb()+" <money> into <type>\n"+
               "       Eg: change 10 brass coins into copper\n");
  if(!str || ((sscanf(str, "%s into %s", s1, type) != 2) &&
    (sscanf(str, "%s to %s", s1, type) != 2)))
    return 0;
  cont = clone_object("/std/container");
  obs = find_match(s1, this_player());
  for (i=0;i<sizeof(obs);i++)
    if (obs[i]->query_property("money")) {
      mon = 1;
      obs[i]->move(cont);
    }
  if (!mon) {
    notify_fail("I can only change money.  Sorry.\n");
    cont->dest_me();
    return 0;
  }
  values = (mixed *)MONEY_HAND->query_values();
  if ((i = member_array(type, values)) == -1) {
    notify_fail("It would be nice if you changed to an existing money type.\n");
    cont->dest_me();
    return 0;
  }
  ob = present(MONEY_ALIAS, cont);
  val = (int)ob->query_value()*93/100;
  if (val < values[i+1]) {
    notify_fail("This is not even worth one "+type+" coin, I cannot change "+
                "to that!\n");
    ob->move(this_player());
    cont->dest_me();
    return 0;
  }
  this_player()->adjust_money(val/values[i+1], type);
  change_arr = MONEY_HAND -> create_money_array(val % values[i+1]);
  write("Ok, changed "+MONEY_HAND->money_string(ob->query_money_array())+
        " into "+(val/values[i+1])+" "+type+" coins.\n");
  for (i=0; i<sizeof(change_arr); i+=2) 
    this_player() -> adjust_money(change_arr[i+1],change_arr[i]);
  if(sizeof(change_arr))
    write("You were returned "+MONEY_HAND -> money_string(change_arr)+" as excess coinage.\n");
  ob->dest_me();
  cont->dest_me();
  return 1;
}
