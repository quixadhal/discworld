#include "move_failures.h"
inherit "std/object";
inherit "std/basic/cute_look";

#define MAX_INVENT 40
string col;

void setup() {
  set_name("paper");
  col = ({ "red", "blue", "green", "pink" })[random(4)];
  set_short(col+" wrapping paper");
  add_adjective("wrapping");
  add_alias("present");
  add_adjective(col);
  set_weight(5);
  set_long("A piece of lovely "+col+" coloured wrapping paper.  Its looks "+
           "like it could be used to wrap up presents.\n"+
           "wrap <object> in paper.\n");
}

int add_weight(int n) {
  return 1;
}

int transfer_all_to(object dest) {
  object *ob;
  int i;
 
  ob = all_inventory(this_object());
  for (i=0;i<sizeof(ob);i++)
    ob[i]->move(dest);
  if (first_inventory(this_object())) return 0;
  return 1;
}

string long(string str, int dark) {
  string ret;

  ret = ::long(str);
  ret += calc_extra_look();
  if (sizeof(all_inventory()))
    ret += "Is wrapped up!\n";
  else
    ret += "Is unwrapped.\n";
  return ret;
}

int test_remove(object ob, int flag) { return 1; }
int test_add(object ob, int flag) { return !sizeof(all_inventory()); }

object *find_inv_match(string str) {
  return all_inventory(this_object());
}

void init() {
  ::init();
  this_player()->add_command("wrap", this_object(), "%I %p %D");
  this_player()->add_command("unwrap", this_object());
}

int do_wrap(object *obs) {
  int i;

  obs = obs - ({ this_object() });
  if (sizeof(all_inventory()) || !sizeof(obs))
    return 0;

  for (i=0;i<sizeof(obs);i++)
    if (environment() == environment(obs[i])) {
      obs[i]->move(this_object());
      this_player()->add_succeeded(obs[0]);
      call_out("set_short", 0, col+" present");
      call_out("set_weight", 0, 10+obs[0]->query_weight());
      return 1;
    }
  return 0;
}

int do_unwrap() {
  object *obs;
  int i;

  if (!sizeof(all_inventory()))
    return 0;
  write("You open your present and find "+
        query_multiple_short(all_inventory())+".\n");
  obs = all_inventory();
  for (i=0;i<sizeof(obs);i++)
    if (obs[i]->move(environment()))
      if (environment(environment()))
         obs[i]->move(environment());
  call_out("set_short", 0, col+" wrapping paper");
  call_out("set_weight", 0, 10);
  return 1;
}
