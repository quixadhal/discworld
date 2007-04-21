/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: no_follow.c,v 1.1 1998/01/06 04:39:52 ceres Exp $
 * $Log: no_follow.c,v $
 * Revision 1.1  1998/01/06 04:39:52  ceres
 * Initial revision
 * 
*/
inherit "/std/effect_shadow";

int add_follower(object ob) {
   call_out("do_command", 0, "lose everyone");
   return player->add_follower(ob);
} /* add_follower() */
