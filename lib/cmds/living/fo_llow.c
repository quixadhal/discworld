/*   -*- LPC -*-   */
/*
 * $Locker:  $
 * $Id: fo_llow.c,v 1.5 2002/04/08 09:18:49 trilogy Exp $
 */
inherit "/cmds/base";

#define TP this_player()

int cmd(object *obs) {
   object *ok, ob;
   object *already;
   string s;
   mapping hide_invis;
   int hiding, sneaking;
   
   ok = ({ });
   already = ({ });
   foreach (ob in obs) {
      if (member_array(TP, ob->query_followers()) == -1) {
         if (ob->add_follower(TP)) {
            ok += ({ ob });
         }
      } else {
         already += ({ ob });
      }
   }

   if (!sizeof(ok)) {
      if (sizeof(already)) {
         add_failed_mess("You are already following $I.\n", already);
      }
      obs -= already;
      if (sizeof(obs)) {
         if(member_array(TP, obs) == -1) {
            add_failed_mess("You cannot follow " + query_multiple_short(obs) +
                                          ".\n");
         } else {
            add_failed_mess("You can't follow " +
                          query_multiple_short(obs - ({TP}) + ({"yourself"})) +
                          ".\n");
         }
      }
      return 0;
   }

  hide_invis = ( mapping )this_player()->query_hide_invis();

  hiding = hide_invis[ "hiding" ] ? 1 : 0;
  sneaking = this_player()->query_sneak_level() ? 1 : 0;

  if( hiding || sneaking )
    write("You follow " + ( s = query_multiple_short( ok ) ) + " unseen.\n" );
  else {
   write("You follow "+(s=query_multiple_short(ok))+".\n");
   say(TP->one_short(1)+" follows "+s+".\n", ok);
   foreach (ob in ok)
      tell_object(ob, TP->one_short(1)+
                      " follows "+query_multiple_short((ok + ({"you"})) -
                      ({ ob }))+".\n");
  }
   return 1;
}

mixed *query_patterns() {
   return ({ "<indirect:living>", (: cmd($1) :) });
}
