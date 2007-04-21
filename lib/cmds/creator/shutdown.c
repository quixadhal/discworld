/*  -*- LPC -*-  */
/*
 * $Locker: pinkfish $
 * $Id: shutdown.c,v 1.3 1999/10/28 02:35:46 ceres Exp pinkfish $
 * $Log: shutdown.c,v $
 * Revision 1.3  1999/10/28 02:35:46  ceres
 * can't remembe
 *
 * Revision 1.2  1999/03/08 10:31:35  pinkfish
 * Make it less error prone.
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* shutdown command. */
inherit "/cmds/base";

//#define GAME_LOG

mixed cmd(int tim, string reason, int force) {
  string str;

  seteuid(geteuid(this_player()));
  str = (string)this_player()->query_name()+" at "+ctime(time());

#ifdef GAME_LOG  
  log_file("GAME_LOG", "Game shutdown by ");
  log_file("GAME_LOG", str);
/*
  log_file("GAME_LOG", ctime(time()));
 */
  log_file("GAME_LOG", " for\n");
  log_file("GAME_LOG", num + " " + reason);
  log_file("GAME_LOG", "\n\n");
#endif  
  write("Ok... shutting down game... \n");

  if (tim < 10 && (!master()->query_lord(geteuid(this_object())) && !force)) {
    tim = 10;
  }
  if (!tim) {
    "/obj/shut"->end_it_all(reason);
  } else {
    "/obj/shut"->shut(tim, reason);
  }
  return 1;
} /* cmd() */

mixed *query_patterns() {
  return ({ "<number'number of minutes'> <string'reason'>",
              (: cmd($4[0], $4[1], 0) :),
              "now <string'reason'>", (: cmd(0, $4[0], 1) :),
              });
} /* query_patterns() */
