#include "parse_command.h"
inherit "/std/basic/print_object";

static string name,
       *alias,
       *adjectives,
       *plurals;

/* name handleing stuff */
void set_name(string str) { name = str; }
string query_name() { return name; }
string query_cap_name() {
   if(!name)
     return "Someone";
   return capitalize(name);
} /* query_cap_name() */

void create() {
  alias = ({ });
  plurals = ({ });
  adjectives = ({ });
  name = "object";
} /* create() */

/* alias stuff */
void set_aliases(string *str) {
  alias = str;
} /* set_aliases() */

void add_alias(mixed str) {
  if (pointerp(str)) {
    alias += str;
    return ;
  }
  if (member_array(str, alias) == -1)
    alias += ({ str });
} /* add_alias() */

int remove_alias(string str) {
  int i;
  if ((i=member_array(str, alias)) == -1)
    return 0;
  alias = delete(alias, i, 1);
  return 1;
} /* remove_alias() */

string *query_alias() { return alias; }

int id(string str) {
  return (str == name) || (member_array(str, alias) != -1);
} /* id() */

int full_id(string str) {
  int loop;

  for(loop = 0; loop < sizeof(adjectives); loop++) {
  }
  return 0;
} /* full_id */


/* plural stuff */
void set_plurals(string *str) {
  plurals = str;
} /* set_plurals() */

void add_plural(mixed str) { 
  int i;

  if (pointerp(str))
    plurals += str;
  else
    plurals += ({ str });
} /* add_plural() */

void remove_plural(string str) {
  int i;

  if ((i=member_array(str, plurals)) != -1)
    plurals = delete(plurals,i,1);
} /* remove_plurals() */

void add_plurals(string *str) {
  plurals += str;
} /* add_plurals() */

string *query_plurals() { return plurals; }

int id_plural(string str) {
  return (member_array(str, plurals) != -1);
} /* id_plural() */

void set_adjectives(string *str) {
  adjectives = str;
} /* set_adjectives() */

void add_adjective(mixed str) {
  int i;

  if (pointerp(str)) {
    for (i=0;i<sizeof(str);i++)
      add_adjective(str[i]);
    return ;
  }
  if (stringp(str))
    str = explode(str, " ");
  for (i=0;i<sizeof(str);i++)
    if (member_array(str[i], adjectives) == -1)
      adjectives += ({ str[i] });
} /* add_adjective() */

void remove_adjective(mixed str) {
  int i;

  if (pointerp(str))
    for (i=0;i<sizeof(str);i++)
      remove_adjective(str[i]);
  else
    if ((i=member_array(str, adjectives)) != -1)
      adjectives = delete(adjectives, i, 1);
} /* remove_adjective() */

string *query_adjectives() { return adjectives; }

/* ok parse command stuff */
string *parse_command_id_list() { return ({ name }) + alias; }
string *parse_command_plural_id_list() { return plurals; }
string *parse_command_adjectiv_id_list() { return adjectives; }

object query_parse_id(mixed *arr) {
  if (arr[P_THING] == 0)
    return this_object();
  if (arr[P_THING] < 0) { /* specific object case */
    arr[P_THING]++;
    if (arr[P_THING] != 0)
      return 0;
    arr[P_THING] = -10321;
    return this_object();
  }
  arr[P_THING]--; /* lots of objects case */
  if (arr[P_THING] != 0)
    return this_object();
  arr[P_THING] = -10101;
  return this_object();
}

object query_frac_parse_id(mixed *arr) {
  if (arr[P_THING] < 0) {
    arr[P_THING]++;
    if (arr[P_THING] != 0)
      return 0;
    arr[P_THING] = -10235;
    return 0; /* it refered to me and I am pretty depressed about it.
               * I want to break free! */
  }
  if (arr[P_THING] == 0)
    if ((arr[P_MAX_NUM]*arr[P_TOP])/arr[P_BOT] > arr[P_CUR_NUM]++)
      return this_object();
    else
      return 0;
  if ((arr[P_THING]*arr[P_TOP])/arr[P_BOT] > arr[P_CUR_NUM]++)
    return this_object();
  return 0;
}

void attack_by(object ob) {
  ob->stop_fight(this_object());
}

/* std/basic/id: */
mixed *query_init_data() {
   return ({ "name", name, "set_name/p/",
             "alias", alias, "add_alias/p/",
             "adjectives", adjectives, "add_adjective/p/",
             "plurals", plurals, "add_plurals/p/" });
} /* query_init_data() */
