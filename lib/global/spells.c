#include "spells.h"
mixed spells;

void create() {
  spells = ([ ]);
}

void spell_commands() {
  add_action("show_spells", "spells");
  add_action("cast", "cast");
}

string help_spell(string str) {
  if (!mappingp(spells))
   spells = ([ ]);
  if (!m_sizeof(spells))
    return 0;
  if (!spells[str])
    return 0;
  return (string)spells[str][S_OBJECT]->help(str);
}
 
/*
 * all the spells stuff from here to the next silly comment
 */
int show_spells(string str) {
  string *frog;
  int i;

  if (!mappingp(spells))
   spells = ([ ]);
  if (!m_sizeof(spells)) {
    write("You don't know any spells.\n");
    return 1;
  }

  frog = m_indices(spells);
  write("You know the following spells:\n");
  printf("%#-*s\n", this_player()->query_cols(), implode(frog, "\n"));
/* the above will use printf... but it is broken at the moment. */
  return 1;
}

int cast(string str) {
  int i, j;
  string *s,s1;
 
  if (!str) {
    notify_fail("Usage: cast <spell> [<target>]\n");
    return 0;
  }
  s = explode(str, " ");
  s1 = s[0];
  j = 1;
  while (!spells[s1] && j < sizeof(s))
    s1 = implode(s[0..j++], " ");
  if (!spells[s1]) {
    notify_fail("There is no such spell as "+str+"\n");
    return 0;
  }
/* ok we found our spell...
 * cast it...
 * Or, at least try to ;)
 */
 return (int)call_other(spells[s1][S_OBJECT], spells[s1][S_FUNC],
                                    implode(s[j..sizeof(s)], " "));
}

mixed query_spells() { return spells + ([ ]); }

int add_spell(string name, mixed ob, mixed func) {
  int i;
  mapping tmp;

  if (pointerp(spells)) {
    tmp = ([ ]);
    for (i=0;i<sizeof(spells);i+=2)
      tmp[spells[i]] = spells[i+1];
    spells = tmp;
  }
  if (!mappingp(spells))
    spells = ([ ]);
  spells[name] = ({ ob, func });
  return 1;
}

int remove_spell(string name) {
  int i;

  spells = m_delete(spells, name);
  return 1;
}

int query_spell(string type) {
  if (!mappingp(spells)) return 0;
  return spells[type];
}

int spell_teach(object *obs, string spell) {
  int i;
  object *succ, *me_low, *him_low;
  string ret;

  if (!mappingp(spells)) spells = ([ ]);
  if (!spells[spell])
    return 0;
  succ = me_low = him_low = ({ });
  for (i=0;i<sizeof(obs);i++) {
    if (!living(obs[i]))
      continue;
    switch (spells[spell][0]->teach(obs[i], spell)) {
      case 1 :
        succ += ({ obs[i] });
        break;
      case -1 :
        me_low += ({ obs[i] });
        break;
      case -2 :
        him_low += ({ obs[i] });
        break;
    }
  }
  if (!succ) {
    ret = "";
    if (sizeof(me_low))
      ret += "You are too low a level to teach "+query_multiple_short(obs)+
             " "+spell+".\n";
    if (sizeof(him_low))
      ret += query_multiple_short(obs)+" is too low a level to learn "+
             spell+".\n";
    write(ret);
    return 1;
  }
  write("You successfuly teach "+query_multiple_short(succ)+" "+spell+".\n");
  if (sizeof(me_low))
    write("You are to low a level to teach "+query_multiple_short(obs-succ)+
           " "+spell+".\n");
  if (sizeof(him_low))
    write(query_multiple_short(obs)+" is too low a level to learn "+
          spell+".\n");
  return 1;
}
