object me, foll;
string me_name;
#define ME "/std/trigger/shadow/follow_player"

void setup_shadow(object ob) {
  me_name = ob->query_name();
  me = ob;
  shadow(me, 1);
  foll = previous_object();
} /* setup_shadow() */

int move_player(string dir, string dest, mixed mess, object followee,
                mixed enter) {
  int i;

  i = me->move_player(dir, dest, mess, followee, enter);
  if (environment(me) && i) {
    environment(me)->add_property(me_name+" here", dir);
    ME->remove_frog_property(environment(me), me_name);
  }
  return i;
} /* move_player() */

void zap_shadow(object ob) {
/* Just in case there is more than one on us... */
  if (interactive(previous_object())) {
    destruct(this_object());
    return ;
  }
  if (previous_object() == foll) {
    destruct(this_object());
    return ;
  }
  me->zap_shadow();
} /* zap_shadow() */

void remove_frog_property(object ob, string me_name) {
  call_out("bingle_wop", 360, ob, me_name);
} /* remove_frog_property() */

void bingle_wop(object ob, string nam) {
  if (ob)
    ob->remove_property(nam+" here");
} /* bingle_wop() */
