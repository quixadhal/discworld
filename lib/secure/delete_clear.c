/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: delete_clear.c,v 1.7 2003/03/22 04:52:12 ceres Exp $
*/
/*
 * This object goes through all the files in the deleted players dir
 * and checks to see if they have timed out.  If they have, it
 * deletes them.
 *
 * Pinkfish - 13th of January 1996
 */
#include <player_handler.h>
#include <refresh.h>

#define ONE_DAY (60*60*24)

#define SAVE_FILE "/save/del_pl_check"

protected void do_checks();
protected void do_delete_check();
protected void do_appeal_check();

void create() {
  seteuid("Root");
  unguarded((: restore_object(SAVE_FILE) :));
  do_checks();
}

protected void do_checks() {
  unguarded((: do_delete_check() :));
  unguarded((: do_appeal_check() :));
  save_object(SAVE_FILE);
  call_out("do_checks", ONE_DAY);
}

protected  void do_delete_check() {
  string *bits, name;
  int i;
  mixed *rubbish;

  bits = get_dir(PLAYER_SAVE_DIR+DELETE_DIR+"/*.o");
  bits += get_dir(PLAYER_SAVE_DIR+DELETE_DIR+"/*.o.gz");
  for (i=0;i<sizeof(bits);i++) {
    rubbish = stat(PLAYER_SAVE_DIR+DELETE_DIR+"/"+bits[i]);
    if (rubbish[1]+(ONE_DAY*10) < time()) {
      log_file( "EXPIRED", "Requested player deletion: "+bits[ i ] +".\n" );
    name = explode( bits[i], "." )[ 0 ]; 

      user_event( "inform", "Auto deleting user "+name, "delete");
      "/secure/related_files"->delete_related_files(name, 1);
      unguarded((: rm( PLAYER_SAVE_DIR+DELETE_DIR+"/"+$(bits[i])) :));
      REFRESH_HANDLER->player_deleted(name);
    }
  }
}

protected  void do_appeal_check() {
  string *bits, name;
  int i;
  mixed *rubbish;

  bits = get_dir(PLAYER_SAVE_DIR+APPEAL_DIR+"/*.o");
  bits += get_dir(PLAYER_SAVE_DIR+APPEAL_DIR+"/*.o.gz");
  for (i=0;i<sizeof(bits);i++) {
    rubbish = stat(PLAYER_SAVE_DIR+APPEAL_DIR+"/"+bits[i]);
    if (rubbish[1]+(ONE_DAY*28) < time()) {
      log_file( "EXPIRED", ctime(time()) + " appeal player deletion: "+
                bits[i]+".\n");
      name = explode( bits[i], "." )[ 0 ]; 
      
      user_event( "inform", "Auto deleting user "+name, "delete");
      "/secure/related_files"->delete_related_files(name, 1);
      unguarded((: rm( PLAYER_SAVE_DIR+APPEAL_DIR+"/"+$(bits[i])) :));
      REFRESH_HANDLER->player_deleted(name);
    }
  }
}
