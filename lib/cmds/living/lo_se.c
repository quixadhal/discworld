/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: lo_se.c,v 1.2 2000/04/22 05:33:22 ceres Exp $
 * $Log: lo_se.c,v $
 * Revision 1.2  2000/04/22 05:33:22  ceres
 * Fixed bug with error return
 *
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";

mixed cmd(object *obs, string str) {
  object *ok, ob;
  string s;

  if (str == "everyone")
    obs = this_player()->query_followers();
  
  if (!sizeof(obs)) {
    write("You are not being followed by anyone.\n");
    return 1;
  }

  ok = ({ });
  foreach (ob in obs)
    if (this_player()->remove_follower(ob))
      ok += ({ ob });

  if (!sizeof(ok)) {
    if (member_array(this_player(), obs) == -1)
      return notify_fail("You are not being followed by "+
                         query_multiple_short(obs)+".\n");
    else
      return notify_fail("You are not being followed by " +
                         query_multiple_short(obs - ({this_player()}) +
                                              ({"yourself"})) + ".\n");
  }
  
  write("You lose "+(s=query_multiple_short(ok))+".\n");
  say(this_player()->the_short()+" loses "+s+".\n", ok);
  foreach (ob in ok)
    tell_object(ob, this_player()->the_short() + " loses " +
                query_multiple_short((ok + ({ "you" })) - ({ ob }))+".\n");
  return 1;
}

mixed *query_patterns() {
  return ({ "everyone", (: cmd(0, "everyone") :),
            "<indirect:living>", (: cmd($1, 0) :) });
}
