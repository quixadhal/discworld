#include "monster.h"
mixed *spells;

void create() {
  spells = ({ });
}

void do_shadow() {
  int i, num;
  object ob;
  string fname;

/* do we wint the sleaze of the year award???? */
  this_object()->shadow_death();
  fname = (string)MONSTER_HAND->query_shadow_name();
  rm(fname+".c");
  for (i=0;i<sizeof(spells);i+=2)
    write_file(fname+".c",
               "inherit \"/std/trigger/"+spells[i]+"\";\n");
  write_file(fname+".c", read_file("/std/living/mon-shadow.c"));
  ob = clone_object(fname);
  ob->setup_shadow(this_object());
  rm(fname+".c");
}

int add_triggered_action(string name, string trigger, mixed ob,
                         string func) {
  int i, j;

  if ((i=member_array(trigger, spells)) == -1) {
    spells += ({ trigger, ({ name, ({ ob, func }) }) });
    do_shadow();
  } else if ((j=member_array(name, spells[i+1])) == -1)
    spells[i+1] += ({ name, ({ ob, func }) });
  else
    spells[i+1][j+1] = ({ ob, func });
}

int remove_trigger(string name) {
  int i;

  if ((i=member_array(name, spells)) == -1)
    return 0;
  spells = delete(spells, i, 2);
  if (!sizeof(spells))
    this_object()->shadow_death();
  else
    do_shadow();
  return 1;
}

int remove_triggered_action(string trigger, string name) {
  int i, j;

  if ((i=member_array(trigger, spells)) == -1)
    return 0;
  if ((j=member_array(name, spells[i+1])) == -1)
    return 0;
  spells[i+1] = delete(spells[i+1], i, 2);
  if (!sizeof(spells[i+1]))
    remove_trigger(trigger);
  return 1;
}

mixed *query_actions(string trigger) {
  int i;

  if ((i=member_array(trigger, spells)) == -1)
    return ({ });
  return spells[i+1];
}

mixed *query_spells() { return spells; }
