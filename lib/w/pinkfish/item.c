#include "parse_command.h"
string *lng,
       *shrt,
       *adjs,
       *plu,
       *name;
mapping verb,
        plural;
int cur_desc;

void create() {
  adjs = ({ });
  lng = ({ "" });
  shrt = ({ "" });
  plu = ({ });
  name = ({ });
  verb = ([ ]);
  plural = ([ ]);
}

string short() {
  return shrt[cur_desc];
}

string pretty_short() { return short(); }

string query_plural() { return pluralize(shrt[cur_desc]); }
string pretty_plural() { return query_plural(); }

string long() {
  if (!lng[cur_desc])
    return "Error in items";
  return lng[cur_desc];
}

void set_long(string s) { lng[cur_desc] = s; }
string query_long() { return lng[cur_desc]; }

int drop() { return 1; }
int get() { return 1; }

void setup_item(mixed nam, string long) {
  string *bits, s;
  int i;

  if (pointerp(nam)) {
    adjs = name = plu = ({ });
    if (sizeof(nam) > 0)
      shrt += ({ nam[0] });
    for (i=0;i<sizeof(nam);i++) {
      bits = explode(nam[i], " ");
      name += ({ (s=bits[sizeof(bits)-1]) });
      verb[s] = ({ bits[0..sizeof(bits)-2], sizeof(lng) });
      plural[(s=pluralize(s))] = ({ bits[0..sizeof(bits)-2], sizeof(lng)-1 });
      plu += ({ s });
      adjs += bits[0..sizeof(bits)-2];
    }
    lng += ({ long });
    return ;
  }
  shrt += ({ nam });
  bits = explode(nam, " ");
  name += ({ (s=bits[sizeof(bits)-1]) });
  verb[s] = ({ bits[0..sizeof(bits)-2], sizeof(lng) });
  plural[(s=pluralize(s))] = ({ bits[0..sizeof(bits)-2], sizeof(lng) });
  plu += ({ s });
  adjs += ({ bits[0..sizeof(bits)-2] });
  lng += ({ long });
}

string *parse_command_id_list() { return name; }
string *parse_command_plural_list() { return plu; }
string *parse_command_adjectiv_list() { return adjs; }

object query_parse_id(mixed *arr) {
  string *bits;
  mixed stuff;
  int i;

  if (arr[P_THING] == 0) {
    bits = explode(arr[P_STR], " ");
    if (!(stuff = plural[bits[sizeof(bits)-1]]))
      return 0;
    for (i=0;i<sizeof(bits)-2;i++)
      if (member_array(bits[i], stuff[0]) == -1)
        return 0;
    cur_desc = stuff[1];
    return this_object();
  }
  if (arr[P_THING] < 0) { /* specific object case */
    bits = explode(arr[P_STR], " ");
    if (!(stuff = verb[bits[sizeof(bits)-1]]))
      return 0;
    for (i=0;i<sizeof(bits)-2;i++)
      if (member_array(bits[i], stuff[0]) == -1)
        return 0;
    arr[P_THING]++;
    if (arr[P_THING] != 0)
      return 0;
    cur_desc = stuff[1];
    arr[P_THING] = -10321;
    return this_object();
  }
  bits = explode(arr[P_STR], " ");
  if (!(stuff = plural[bits[sizeof(bits)-1]]))
    return 0;
  for (i=0;i<sizeof(bits)-2;i++)
    if (member_array(bits[i], stuff[0]) == -1)
      return 0;
  arr[P_THING]--; /* lots of objects case */
  cur_desc = stuff[1];
  if (arr[P_THING] != 0)
    return this_object();
  arr[P_THING] = -10786;
  return this_object();
}

void dest_me() {
  destruct(this_object());
  return ;
}

void dwep() {
  destruct(this_object());
  return ;
}

int move() { return 1; }
