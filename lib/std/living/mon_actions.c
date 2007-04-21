/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: mon_actions.c,v 1.3 1998/06/09 21:31:31 ceres Exp $
 * $Log: mon_actions.c,v $
 * Revision 1.3  1998/06/09 21:31:31  ceres
 * Commented out some logging to improve performance.
 *
 * Revision 1.2  1998/03/12 09:18:29  pinkfish
 * Added documentaiotn.
 *
 * Revision 1.1  1998/01/06 04:29:08  ceres
 * Initial revision
 * 
 */
/**
 * This is the old method of handling npc actions.  Effects should now
 * always be used instead of these functions.
 * <p>
 * Do not use there functions!
 * @author Pinkfish
 */
#include "monster.h"

#undef DEBUG

mixed *mon_actions;

void create() {
  mon_actions = ({ });
}

/** @ignore yes */
void do_shadow() {
  int i;
  object ob;
  string fname;

/* do we wint the sleaze of the year award???? */
  this_object()->shadow_death();
  fname = (string)MONSTER_HAND->query_shadow_name();
  rm(fname+".c");
  for (i=0;i<sizeof(mon_actions);i+=2)
    write_file(fname+".c",
               "inherit \"/std/trigger/"+mon_actions[i]+"\";\n");
  write_file(fname+".c", read_file("/std/living/mon-shadow.c"));
  ob = clone_object(fname);
  ob->setup_shadow(this_object());
  rm(fname+".c");
}

/**
 * Do not use this method!  Use effects instead.
 */
int add_triggered_action(string name, string trigger, mixed ob,
                         string func) {
  int i, j;

#ifdef DEBUG  
  log_file("TRIGGERED", ctime(time())+": "+
           file_name(this_object())+"  prev: "+
      implode( map_array( previous_object( -1 ), (: file_name( $1 ) :) ),
      ", " ) +"\n" );
#endif  
  if ((i=member_array(trigger, mon_actions)) == -1) {
    mon_actions += ({ trigger, ({ name, ({ ob, func }) }) });
    do_shadow();
  } else if ((j=member_array(name, mon_actions[i+1])) == -1)
    mon_actions[i+1] += ({ name, ({ ob, func }) });
  else
    mon_actions[i+1][j+1] = ({ ob, func });
}

/** @ignore yes */
int remove_trigger(string name) {
  int i;

  if ((i=member_array(name, mon_actions)) == -1)
    return 0;
  mon_actions = delete(mon_actions, i, 2);
  if (!sizeof(mon_actions))
    this_object()->shadow_death();
  else
    do_shadow();
  return 1;
}

/** @ignore yes */
int remove_triggered_action(string trigger, string name) {
  int i, j;

  if ((i=member_array(trigger, mon_actions)) == -1)
    return 0;
  if ((j=member_array(name, mon_actions[i+1])) == -1)
    return 0;
  mon_actions[i+1] = delete(mon_actions[i+1], i, 2);
  if (!sizeof(mon_actions[i+1]))
    remove_trigger(trigger);
  return 1;
}

/** @ignore yes */
mixed *query_actions(string trigger) {
  int i;

  if ((i=member_array(trigger, mon_actions)) == -1)
    return ({ });
  return mon_actions[i+1];
}

/** @ignore yes */
mixed *query_mon_actions() { return mon_actions; }
