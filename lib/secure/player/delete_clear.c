/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: delete_clear.c,v 1.1 1998/01/06 05:13:01 ceres Exp $
 * $Log: delete_clear.c,v $
 * Revision 1.1  1998/01/06 05:13:01  ceres
 * Initial revision
 * 
*/
/*
 * This object goes through all the files in the deleted players dir
 * and checks to see if they have timed out.  If they have, it
 * deletes them.
 *
 * Pinkfish - 13th of January 1996
 */
#include <player_handler.h>

#define ONE_DAY (60*60*24)

#define SAVE_FILE "/save/del_pl_check"

/* Do the clear once every day */
int last_clear;

void create() {
  seteuid("Root");
  restore_object(SAVE_FILE);
  if (last_clear+ONE_DAY < time()) {
    call_out("do_delete_check", 0);
  } else {
    call_out("do_delete_check", last_clear+ONE_DAY-time());
  }
} /* create() */

protected void do_delete_check() {
  string *bits;
  int i;
  mixed *rubbish;

  bits = get_dir("/players/"+DELETE_DIR+"/*.o");
  for (i=0;i<sizeof(bits);i++) {
    rubbish = stat("/players/"+DELETE_DIR+"/"+bits[i]);
    if (rubbish[1]+(ONE_DAY*10) < time()) {
      log_file("DELETE_PL", ctime(time())+": Auto deleteing "+bits[i]+"\n");
      PLAYER_HANDLER->remove_player(bits[i]);
      rm("/players/"+DELETE_DIR+"/"+bits[i]);
    }
  }
  last_clear = time();
  save_object(SAVE_FILE);
} /* do_delete_check() */
