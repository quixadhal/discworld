inherit "std/object";
#include "pub.h"

int weight_per_bite;
string eat_object, eat_func;

void init() {
  ::init();
  this_player()->add_command("eat", this_object());
}

int query_weight_per_bite() { return weight_per_bite; }
string query_eat_object() { return eat_object; }
string query_eat_func() { return eat_func; }

void set_weight_per_bite(int i) { weight_per_bite = i; }
void set_eat_object(string s) { eat_object = s; }
void set_eat_func(string s) { eat_func = s; }
void setup_eat(string eo, string ef) {
  eat_object  = eo;
  eat_func = ef;
}

int do_eat(object *dests, string s1, string s2, string prep) {
  if (sizeof(dests)) {
    notify_fail("Eat "+prep+"???\n");
    return 0;
  }

  if (weight <= 0) {
    call_out("dest_me", 0);
    return 1;
  }
  if (!weight_per_bite) {
    call_out("dest_me", 0);
    if (eat_func)
      if (eat_object)
        call_other(eat_object, eat_func, this_object());
      else
        call_other(this_object(), eat_func, this_object());
    return 1;
  }
  environment()->add_weight(-weight_per_bite);
  weight -= weight_per_bite;
  if (eat_func)
    if (eat_object)
      call_other(eat_object, eat_func, this_object());
    else
      call_other(this_object(), eat_func, this_object());
  if (weight <=0) {
    call_out("dest_me", 0);
    environment()->add_weight(-weight);
  }
  return 1;
}
