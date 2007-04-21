/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: spells.c,v 1.2 2001/06/03 20:21:47 pinkfish Exp $
 * $Log: spells.c,v $
 * Revision 1.2  2001/06/03 20:21:47  pinkfish
 * Change the variable name.
 *
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 * 
*/
#include "spells.h"

nosave mixed *_spell_effects;

void create() {
  _spell_effects = ({ });
} /* create() */

/* temp for testing */
mixed query_effects() { return _spell_effects; }
int queue_commands() { return sizeof( _spell_effects ); }

int add_spell_effect(int no_rnds, string type, string name, object callee,
                     string func, mixed params) {
  int i,j;

   if (strsrch(file_name(this_object()),"global")==-1)
      tell_object(find_living("olorin"),
                  sprintf("add_spell_effect in %s\n",
                          file_name(this_object())));
  if ((i=member_array(type, _spell_effects)) != -1)
    if ((j=member_array(name, _spell_effects[i+1])) == -1)
      _spell_effects[i+1] += ({ name, ({ no_rnds, callee, func, params }) });
    else
      _spell_effects[i+1][j+1] = ({ no_rnds, callee, func, params });
  else
    _spell_effects += ({ type, ({ name, ({ no_rnds, callee, func, params }) }) });
} /* add_spell_effect() */

int remove_spell_effect(string name) {
  int i, j;

   if (strsrch(file_name(this_object()),"global")==-1)
      tell_object(find_living("olorin"),
                  sprintf("remove_spell_effect in %s\n",
                          file_name(this_object())));
  for (i=0;i<sizeof(_spell_effects);i+=2)
    if ((j=member_array(name, _spell_effects[i+1])) != -1) {
      _spell_effects[i+1] = delete(_spell_effects[i+1], j, 2);
      if (!sizeof(_spell_effects[i+1])) {
        _spell_effects = delete(_spell_effects, i, 2);
        i -= 2;
      }
    }
  return 1;
} /* remove_spell_effect() */

mixed query_spell_effect(string name) {
   int i;

   if (strsrch(file_name(this_object()),"global")==-1)
      tell_object(find_living("olorin"),
                  sprintf("query_spell_effect in %s\n",
                          file_name(this_object())));
  if ((i=member_array(name, _spell_effects)) == -1)
    return 0;
  if (sizeof(_spell_effects[i+1]))
    return _spell_effects[i..i]+_spell_effects[0..1];
  return 0;
} /* query_spell_effect() */

mixed *query_spell_effects_type(string type) {
  int i, j;
  mixed *ret;

  if ((i=member_array(type, _spell_effects)) == -1)
    return ({ });
  ret = ({ });
  for (j=0;j<sizeof(_spell_effects);j+=2)
    ret += ({ _spell_effects[i], _spell_effects[i+1][j..j+1] });
  return ret;
} /* query_spell_effects_type() */

int do_spell_effects(object attacker) {
  int i, j;

  this_object()->remove_property("casting");
  for (i=0;i<sizeof(_spell_effects);i+=2)
    for (j=0;j<sizeof(_spell_effects[i+1]);j+=2) {
      call_out("do_call_out_effect", 1, ({ _spell_effects[i+1][j+1], attacker }));
      _spell_effects[i+1][j+1][SP_NO_RNDS]--;
      if ( !_spell_effects[i+1][j+1][SP_NO_RNDS] || !_spell_effects[i+1][j+1][SP_OBJECT]) {
        _spell_effects[i+1] = delete(_spell_effects[i+1], j, 2);
        j -= 2;
        if (!sizeof(_spell_effects[i+1])) {
          _spell_effects = delete(_spell_effects, i, 2);
          i -= 2;
          break;
        }
      }
    }
    return 0;
} /* do_spell_effects() */

void do_call_out_effect(mixed *params) {
  if (params[0][SP_OBJECT])
    call_other(params[0][SP_OBJECT],
             params[0][SP_FUNC],
             params[1],
             params[0][SP_PARAM],
             params[0][SP_NO_RNDS]);
} /* do_effect() */
