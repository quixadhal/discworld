/*  -*- LPC -*-  */
/*
 * $Locker: ceres $
 * $Id: rmp_layer.c,v 1.7 2003/04/06 02:33:56 ceres Exp ceres $
 */

/* rmplayer, deletes a player's save file and calls the refresh
 * handler on that player.
 */

#include <refresh.h>
#include <player_handler.h>
#include <player_handler.h>


inherit "/cmds/base";

private void sure(string response, string player, string lord, string reason,
                  int immediately) {
  if(previous_object() && 
     member_array(find_object("/secure/simul_efun"), 
                  previous_object( -1 ) ) == -1 ) {
    log_file("CHEAT", ctime(time()) +
             ": illegal attempt to delete a player.\nTrace: " +
             back_trace());
    write("Illegal use of rmplayer.\n");
    return;
  }
  
  if (response == "YES") {
    string olduid, src, ext;
    
    write("Ok.  Deleting\n");
    olduid = geteuid();
    seteuid("Root");
    if(file_size(PLAYER_HANDLER->query_player_ram_file_name(player)+".o.gz") > 0){
      src = PLAYER_HANDLER->query_player_ram_file_name(player);
      ext = ".o.gz";
    } else if(file_size(PLAYER_HANDLER->query_player_disk_file_name(player)+".o.gz") > 0) {
      src = PLAYER_HANDLER->query_player_disk_file_name(player);
      ext = ".o.gz";
    } else if(file_size(PLAYER_HANDLER->query_player_disk_file_name(player)+".o") > 0) {
      src = PLAYER_HANDLER->query_player_disk_file_name(player);
      ext = ".o";
    } else if(file_size(PLAYER_HANDLER->query_delete_player_file_name(player) + ".o.gz") > 0) {
      src = PLAYER_HANDLER->query_delete_player_file_name(player);
      ext = ".o.gz";
    } else if(file_size(PLAYER_HANDLER->query_delete_player_file_name(player)+ ".o") > 0) {
      src = PLAYER_HANDLER->query_delete_player_file_name(player);
      ext = ".o";
    }

    if(src && ext) {
      if(!immediately)
        unguarded((: cp($(src) + $(ext), "/save/players/appeal/"+
                        $(player)+$(ext)):));
      unguarded((: rm($(src)+$(ext)) :));
      if(src == PLAYER_HANDLER->query_player_ram_file_name(player)) {
        src = PLAYER_HANDLER->query_player_disk_file_name(player);
        unguarded((: rm($(src)+$(ext)) :));
      }
      PLAYER_HANDLER->remove_cache_entry( player );
      unguarded((: write_file, "/log/SUSPEND",
                 ctime(time()) + " "+ player + " deleted by " + lord +
                 " because " + reason + ".\n" :) );
      seteuid(olduid);
    }
  } else {
    write("Ok.  Not deleting\n");
  }
  return;
}

int cmd(string player, string reason, int immediately) {
  if (!master()->query_liaison_deputy_or_lord(previous_object(-1))) {
    unguarded((: log_file, "/log/CHEAT", ctime(time()) +
               ": illegal attempt to delete a player.\nTrace: " +
               back_trace() :));
    return notify_fail("Sorry: only directors can delete players.\n");
  }
  
  /* At this point, I'm satisfied that the object is _allowed_ to
   * delete players.  But is this a player?
   */
  if (!PLAYER_HANDLER->test_user(player)) {
    return notify_fail("Sorry, but " + player + " isn't a player\n");
  }
  
  /* Is a non H trying to delete an H or L? */
  if (!master()->high_programmer(previous_object(-1)) &&
      master()->query_lord(player)) {
    unguarded( (: log_file, "/log/CHEAT", ctime(time()) +
                ": Illegal attempt to delete a director.\nTrace: " +
                back_trace() :) );
    return notify_fail("Sorry.\n");
  }

  if(!master()->query_lord(previous_object(-1)) &&
       PLAYER_HANDLER->test_age(player) < - (86400)) {
    add_failed_mess("Only directors can immediately delete players over "
                    "24 hours old.\n");
    return 0;
  }
  
  if (!master()->query_lord(previous_object(-1)) &&
      PLAYER_HANDLER->test_creator(player)) {
    add_failed_mess("You cannot delete creators.\n");
    return 0;
  }
   
  if(!reason || reason == "") {
    return notify_fail("You must give a reason.\n");
  }
  
  /* Ok.  Looks like everything is copasetic. */
  write("This will permanently delete the player " + player + ".\n" +
        "THIS IS PERMANENT.\n" +
        "Are you sure?  (only \"YES\" will delete the player)  ");
  input_to((: sure :), 0, player, this_player()->query_name(), reason,
           immediately);
  return 1;
}

mixed *query_patterns() {
  return ({ "<word'player name'> <string'reason'>", (: cmd($4[0], $4[1], 0) :),
            " immediately <word'player name'> <string'reason'>",
              (: cmd($4[0], $4[1], 1) :) });
}
