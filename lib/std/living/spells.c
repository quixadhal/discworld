#include "spells.h"

static mixed *effects;

void create() {
  effects = ({ });
} /* create() */

/* temp for testing */
mixed query_effects() { return effects; }

int add_spell_effect(int no_rnds, string type, string name, object callee,
                     string func, mixed params) {
  int i,j;

  if ((i=member_array(type, effects)) != -1)
    if ((j=member_array(name, effects[i+1])) == -1)
      effects[i+1] += ({ name, ({ no_rnds, callee, func, params }) });
    else
      effects[i+1][j+1] = ({ no_rnds, callee, func, params });
  else
    effects += ({ type, ({ name, ({ no_rnds, callee, func, params }) }) });
} /* add_spell_effect() */

int remove_spell_effect(string name) {
  int i, j;

  for (i=0;i<sizeof(effects);i+=2)
    if ((j=member_array(name, effects[i+1])) != -1) {
      effects[i+1] = delete(effects[i+1], j, 2);
      if (!sizeof(effects[i+1])) {
        effects = delete(effects, i, 2);
        i -= 2;
      }
    }
  return 1;
} /* remove_spell_effect() */

mixed query_spell_effect(string name) {
  int i, j;

  if ((i=member_array(name, effects)) == -1)
    return 0;
  if (sizeof(effects[i+1]))
    return effects[i..i]+effects[0..1];
  return 0;
} /* query_spell_effect() */

mixed *query_spell_effects_type(string type) {
  int i, j;
  mixed *ret;

  if ((i=member_array(type, effects)) == -1)
    return ({ });
  ret = ({ });
  for (j=0;j<sizeof(effects);j+=2)
    ret += ({ effects[i], effects[i+1][j..j+1] });
  return ret;
} /* query_spell_effects_type() */

int do_spell_effects(object attacker) {
  int i, j;

  this_object()->remove_property("casting");
  for (i=0;i<sizeof(effects);i+=2)
    for (j=0;j<sizeof(effects[i+1]);j+=2) {
      call_out("do_call_out_effect", 1, ({ effects[i+1][j+1], attacker }));
      if (!--effects[i+1][j+1][SP_NO_RNDS]) {
        effects[i+1] = delete(effects[i+1], j, 2);
        j -= 2;
      }
    }
    return 0;
} /* do_spell_effects() */

void do_call_out_effect(mixed *params) {
  this_object()->adjust_xp(call_other(params[0][SP_OBJECT],
             params[0][SP_FUNC],
             params[1],
             params[0][SP_PARAM],
             params[0][SP_NO_RNDS]));
} /* do_effect() */
