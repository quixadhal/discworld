inherit "/failsafe/simul_efun";
inherit "/failsafe/print_object";

static string name,
       *alias,
       *adjectives,
       *plurals;

/* name handleing stuff */
void set_name(string str) { name = str; }
string query_name() { return name; }
string query_cap_name() { return capitalize(name); }

void create() {
  alias = ({ });
  plurals = ({ });
  adjectives = ({ });
}

/* alias stuff */
void add_alias(mixed str) {
  if (pointerp(str)) {
    alias += str;
    return ;
  }
  if (member_array(str, alias) == -1)
    alias += ({ str });
}

int remove_alias(string str) {
  int i;
  if ((i=member_array(str, alias)) == -1)
    return 0;
  alias = delete(alias, i, 1);
  return 1;
}

string *query_alias() { return alias; }

int id(string str) {
  return (str == name) || (member_array(str, alias) != -1);
}

/* plural stuff */
void set_main_plural(string str) {
  plurals = ({ str }) + plurals;
}

string query_plural() { 
  if (sizeof(plurals))
    return plurals[0];
  return name;
}

string pretty_plural() {
  if (sizeof(plurals))
    return plurals[0];
  return name;
}

void add_plural(string str) { 
  plurals += ({ str });
}

void remove_plural(string str) {
  int i;

  if ((i=member_array(str, plurals)) != -1)
    plurals = delete(plurals,i,1);
}

void add_plurals(string *str) {
  plurals += str;
}

string *query_plurals() { return plurals; }

int id_plural(string str) {
  return (member_array(str, plurals) != -1);
}

void add_adjective(mixed str) {
  int i;

  if (pointerp(str))
    for (i=0;i<sizeof(str);i++)
      add_adjective(str[i]);
  else
    if (member_array(str, adjectives) == -1)
      adjectives += ({ str });
}

void remove_adjective(mixed str) {
  int i;

  if (pointerp(str))
    for (i=0;i<sizeof(str);i++)
      remove_adjective(str[i]);
  else
    if ((i=member_array(str, adjectives)) != -1)
      adjectives = delete(adjectives, i, 1);
}

string *query_adjectives() { return adjectives; }

/* ok parse command stuff */
string *parse_command_id_list() { return ({ name }) + alias; }
string *parse_command_plural_id_list() { return plurals; }
string *parse_command_adjectiv_id_list() { return adjectives; }

int query_parse_id(mixed *arr) {
  return (arr[0]-- > 0);
}
