#include "move_failures.h"
inherit "failsafe/object";
inherit "failsafe/extra_look";
inherit "failsafe/cute_look";

#define MAX_INVENT 40
#define C_CLOSED 1
#define C_TRANS 2
#define C_OPAQUE 1

static int max_weight, loc_weight;
static int closed, prevent_insert, trans;

void set_max_weight(int arg) { max_weight = arg; }
void set_open() { closed &= ~C_CLOSED; }
void set_closed() { closed |= C_CLOSED; }
void set_transperant() { trans = C_TRANS; }
void reset_transperant() { trans = 0; } /* this sets us back to normal */
void set_opaque() { trans = C_OPAQUE; }
int query_max_weight() { return max_weight; }
int query_loc_weight() { return loc_weight; }

int add_weight(int n) {
  if (!max_weight) return 1;
  if (n + loc_weight > max_weight) return 0;
  loc_weight += n;
  return 1;
}

string pretty_short() {
  string s;
  if (trans == C_TRANS) {
    s= query_contents(", it contains ");
    return ::short()+extract(s, 0, strlen(s)-3);
  }
  return ::short();
}

string pretty_plural() {
  string s;
  s = query_contents(", they contain ");
  return ::pretty_plural()+extract(s, 0, strlen(s)-3);
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

/* ok modifyed back to using writes... */
void long(string str) {
  string ret;

  ret = ::long(str);
  ret += calc_extra_look();
  if (trans != C_OPAQUE)
    ret += query_contents(short()+" contains:\n");
  return ;
/* return ret + query_contents(short()+" contains:\n");
 */
}

int test_remove(object ob, int flag) { return !closed; }
int test_add(object ob, int flag) { return !closed; }
int set_prevent_insert() { prevent_insert = 1; }
int reset_prevent_insert() { prevent_insert = 0; }
int query_prevent_insert() { return prevent_insert; }

varargs int move(mixed dest, mixed messin, mixed messout) {
  if (prevent_insert && loc_weight && !living(dest) && environment(dest))
    return MOVE_INVALID_DEST;
  return (int)::move(dest, messin, messout);
}
