string query_name() {
  return "plant";
}

string query_plural() {
  return "plants";
}

string pretty_plural() {
  return "plants";
}

int id(string str) {
  return (str == "plant");
}

string *parse_command_id_list() { return ({ "plant" }); }
string *parse_command_plural_list() { return ({ "plants" }); }
string *parse_command_adjective_list() { return ({ }); }

object query_parse_id(mixed *arr) {
  if (arr[0] == 0)
    return this_object();
  if (arr[0] < 0) {
    arr[0]++;
    if (arr[0] != 0)
      return 0;
    arr[0] = -10321;
    return this_object();
  }
  arr[0]--;
  if (arr[0] != 0)
    return this_object();
  arr[0] = -10101;
  return this_object();
}

void attack_by(object ob) {
  ob->stop_fight(this_object());
}

string query_short() {
  return "plant";
}

string query_long() {
  return "A stringly looking plant.  It has leaves.\n";
}

string short() {
  return "plant";
}

string long() {
  return "A stringly looking plant.  It has leaves.\n";
}

string pretty_short() {
  return "plant";
}

#include "move_failures.h"
varargs int move(mixed dest, mixed messin, mixed messout) {
  if (!dest)
    return MOVE_EMPTY_DEST;
  if (environment() && !environment()->test_remove(this_object(), 1))
    return MOVE_NO_DROP;
  if (!dest->test_add(this_object(), 1))
    return MOVE_NO_GET;
  if (environment() && stringp(messout))
    say(messout);
  move_object(this_object(), dest);
  if (stringp(messin))
    say(messin);
  return MOVE_OK;
}

void dest_me() {
  destruct(this_object());
}

void dwep() {
  destruct(this_object());
}
