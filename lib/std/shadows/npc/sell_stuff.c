/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: sell_stuff.c,v 1.1 1998/01/06 04:39:52 ceres Exp $
 * $Log: sell_stuff.c,v $
 * Revision 1.1  1998/01/06 04:39:52  ceres
 * Initial revision
 * 
*/
/*
 * The sell stuff shadow.
 */
inherit "/std/effect_shadow";

void stopped_route() {
  object *obs;
  int i;
  mixed room;

  room = arg();

  if (pointerp(room)) {
    room = room[0];
  }

  if (file_name(environment(player)) == room) {
    /* Sell stuff... */
    obs = all_inventory(player);
    for (i=0;i<sizeof(obs);i++) {
      if (!obs[i]->query_property("start equipment"))
        call_out("sell_the_stuff", 1, obs[i]);
        call_out( "drop_the_stuff", 2, obs[ i ] );
    }
    call_out( "bury_the_stuff", 3, query_multiple_short( obs ) );
  }
  call_out("remove_this_effect", 4);
} /* stopped_route() */

protected void sell_the_stuff(object ob) {
  player->do_command("sell "+ob->query_name());
} /* sell_the_stuff() */

protected void drop_the_stuff( object thing ) {
   player->do_command( "drop "+ (string)thing->query_name() );
} /* drop_the_stuff() */

protected void bury_the_stuff( string words ) {
   player->do_command( "bury "+ words );
} /* bury_the_stuff() */
