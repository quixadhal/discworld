#include "move_failures.h"
inherit "std/object";
inherit "std/basic/cute_look";

#define MAX_INVENT 40
#define C_CLOSED 1
#define C_TRANS 2
#define C_OPAQUE 1

static int max_weight, loc_weight;
static int prevent_insert;

void create() {
  object::create();
} /* create() */

void set_max_weight(int arg) { max_weight = arg; }
int query_max_weight() { return max_weight; }
int query_loc_weight() { return loc_weight; }

int add_weight(int n) {
  int old, new;

  if (::add_weight(n)) return 1;
  if (!max_weight) return 1;
  if (n + loc_weight > max_weight) return 0;
  loc_weight += n;
  return 1;
} /* add_weight() */

int transfer_all_to(object dest) {
  object *ob;
  int i;
 
  ob = all_inventory(this_object());
  for (i=0;i<sizeof(ob);i++)
    ob[i]->move(dest);
  if (first_inventory(this_object())) return 0;
  return 1;
} /* transfer_all_to() */

/* ok modifyed back to using writes... */
string long(string str, int dark) {
  string ret;

  ret = ::long(str);
  ret += calc_extra_look();
  if(this_object()->query_corpse())
    ret += (string)this_object()->query_living_contents(0);
  else
    ret += query_contents(short(dark)+" contains:\n", all_inventory() - 
                          query_labels());
  return ret;
} /* long() */

int test_remove(object ob, int flag) { return 1; }
int test_add(object ob, int flag) { return 1; }
int set_prevent_insert() { prevent_insert = 1; }
int reset_prevent_insert() { prevent_insert = 0; }
int query_prevent_insert() { return prevent_insert; }

varargs int move(mixed dest, mixed messin, mixed messout) {
  if (prevent_insert && loc_weight && !living(dest) && environment(dest))
    return MOVE_INVALID_DEST;
  return (int)object::move(dest, messin, messout);
} /* move() */

object *find_inv_match(string str) {
  return all_inventory(this_object());
} /* find_inv_match() */

mixed stats() {
  return ::stats() + ({
    ({ "loc_weight", query_loc_weight(), }),
    ({ "max_weight", query_max_weight(), }),
  });
} /* stats() */

mixed *query_init_data() {
  return object::query_init_data() + ({
         "max weight", max_weight, "set_max_weight",
         "prevent insert", prevent_insert, "set_prevent_insert",
         });
} /* query_init_data() */

mapping int_query_static_auto_load() {
  mapping tmp;

  tmp = ::int_query_static_auto_load();
  return ([ "::" : tmp, "max weight" : max_weight,
            "prevent insert" : prevent_insert,
    ]);
} /* int_query_static_auto_load() */

mapping query_dynamic_auto_load() {
  return ([ "::" : ::query_dynamic_auto_load(),
            "inv" : 
         "/global/player"->create_auto_load(all_inventory(this_object())-labels),
         ]);
} /* query_dynamic_auto_load() */

void init_dynamic_arg(mapping bing) {
  if (bing["::"])
    ::init_dynamic_arg(bing["::"]);
  if (bing["inv"])
    "/global/player"->load_auto_load(bing["inv"], this_object());
} /* init_dynamic_arg() */

void init_static_arg(mapping bing) {
  if (bing["::"])
     ::init_static_arg(bing["::"]);
  if (!undefinedp(bing["max weight"]))
    max_weight = bing["max weight"];
  if (!undefinedp(bing["prevent insert"]))
    prevent_insert = bing["prevent insert"];
} /* init_static_arg() */

mixed query_static_auto_load() {
  if (file_name(this_object())[0..13] == "/std/container")
    return int_query_static_auto_load();
  return ([ ]);
} /* query_static_auto_load() */

void dest_me() {
  object *obs;
  int i;

  obs = deep_inventory(this_object());
  for (i=0;i<sizeof(obs);i++)
    obs[i]->dest_me();
  ::dest_me();
} /* dest_me() */
