#define NEW_SOUL "/obj/soul/soul"
/* the bit of the soul that all players should have ;) */
inherit "/std/object";

int soul_on;

void setup(int arg) {
  soul_on = 1;
  set_name("soul2");
}

int query_auto_load() {
  return soul_on;
}

int init_arg(int s) {
  soul_on = s;
  if (soul_on)
    write("Soul enablabled, bing happily.\n");
  else
    write("Soul disabled.  You sure you know what your doing?\n");
}

void init() {
  add_action("soul_com","*");
  add_action("soul_toggle", "no_soul");
}

int soul_com(string str) {
  int i;
  string s1, s2;

  if (sscanf(str, "%s %s", s1, s2) != 2) {
    s1 = str;
    s2 = 0;
  }
/* Ths soul may be crashing it... so lets find out! */
  if (!soul_on) return 0;
  i = (int)(NEW_SOUL)->cmd_soul(s1, s2);
  return i;
}

string long(string s, int dark) {
  mapping data;
  string str, *names, ret;
  int len, i;

  str = "";
  data = (mapping)NEW_SOUL->query_soul_data();
  names = m_indices(data);
  return sprintf("New improved hoopy soul by Zellski.\n"+
       "This soul currently has %d soul commands.\n%-*#s\n"+
       "Use \"no_soul\" to toggle the soul on and off.\n",
        sizeof(names), this_player()->query_cols(), implode(names,"\n"));
}

int drop() { return 1; }
int get() { return 1; }


int soul_toggle(string str) {
  if (!str)
    soul_on = !soul_on;
  else if (str == "on")
    soul_on = 1;
  else if (str == "off")
    soul_on = 0;
  else {
    notify_fail("Syntax: "+query_verb()+" [on|off]\n");
    return 0;
  }
  if (soul_on)
    write("Soul enabled.\n");
  else
    write("Soul disabled.\n");
  return 1;
}
