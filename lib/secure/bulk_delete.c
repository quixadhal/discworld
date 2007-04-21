/*  -*- LPC -*-  */
/*
 * $Id: bulk_delete.c,v 1.17 2003/07/22 00:53:15 ceres Exp $
 */
#include <player_handler.h>
#include <mail.h>

#define ERASE_RATE 30
#define MIN_DELAY 604800
// 180 days
#define MAIL_UNREAD_TIME 15552000
// 365 days
#define MAIL_INACTIVE_TIME 31536000

#define ALPHABET ({ "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", \
                    "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", \
                    "w", "x", "y","z" })
#define SAVEFILE "/secure/bulk_delete.os"
#define FOLDER_H "/obj/handlers/folder_handler.c"

nosave string *dirs, current, fname;
nosave class mail_header *folder;

private void delete_name(string name);
private void check_name(string name);

protected void create() {
  string var;

  if (mud_name() != "Discworld") {
    return;
  }
  seteuid("Root");
  dirs = ({ });
  if (file_size(SAVEFILE) >= 0) {
    var = unguarded((: read_file, SAVEFILE :));
    dirs = restore_variable(var);
  }
  call_out("continuous_erase", 1);
}

protected void reset() {
  string var;

  var = save_variable(dirs);
  unguarded((: write_file, SAVEFILE, var, 1 :));
  if (find_call_out("continuous_erase") == -1) {
    call_out("continuous_erase", 1);
  }
}

protected void continuous_erase(string *all_files) {
  if (!sizeof(all_files)) {
    if(!sizeof(dirs)) {
      dirs = ALPHABET;
    }

    current = dirs[random(sizeof(dirs))];
    log_file("EXPIRED", "%s Starting letter %s\n", ctime(time()), current);
    dirs -= ({ current });
    all_files = get_dir(PLAYER_SAVE_DIR + current + "/*.o");
    all_files += get_dir(PLAYER_SAVE_DIR + current + "/*.o.gz");
  }

  if (sizeof(all_files)) {
    if(all_files[0][<1] == 'z')
      check_name(all_files[0][0..<6]);
    else
      check_name(all_files[0][0..<3]);
  }
  call_out("continuous_erase", ERASE_RATE, all_files[1..]);
}

private void check_name(string name) {
  int *tmp, time_on, last_log_on, i;

  if (find_player(name))
    return;

  if(!PLAYER_HANDLER->test_user(name)) {
    delete_name(name);
    return;
  }

  // This check allows us to ignore recently logged in players
  // without having to restore their player object.
  if(time() - PLAYER_HANDLER->test_last(name, 1) < MIN_DELAY)
    return ;

  /*
   * NOTE: time_on is a negative number for reasons known only to the
   * derranged mind of whoever coded it that way -- Ceres
   */
  time_on = - PLAYER_HANDLER->test_age(name);
  last_log_on = PLAYER_HANDLER->test_last(name);
  
  // Now do the mail cleanup checks.
  if((time() - last_log_on) > MAIL_INACTIVE_TIME) {
    tmp = FOLDER_H->mail_count(name);
    if(tmp[0]) {
#ifdef DEBUG
      log_file("EXPIRED_DEBUG", "%s would have erased mail for %s\n",
               ctime(time()), name);
#else
      log_file("EXPIRED", "%s erased mail for %s\n", ctime(time()), name);
      FOLDER_H->delete_account(name);
    }
    return;
#endif
  }

  if(PLAYER_HANDLER->test_creator(name))
    return;

  if(PLAYER_HANDLER->test_property(name, "no delete"))
    return;
  
#ifdef DEBUG
  log_file("EXPIRED_DEBUG", "%s %s last logged in %s [%d] age x 60 [%d]\n",
           ctime(time()), name, ctime_elapsed(time() - last_log_on),
           time() - last_log_on, time_on * 60);
#endif

  if((time() - last_log_on) < MIN_DELAY)
    return;

  if((time() - last_log_on) > (time_on * 60)) {
#ifdef DEBUG
    log_file("EXPIRED_DEBUG", "%s would have erased: %s\n",
             ctime(time()), name);
#else
    delete_name(name);
    return;
#endif
  }

  if((time() - last_log_on) > MAIL_UNREAD_TIME) {
    tmp = FOLDER_H->mail_count(name);
    if(!tmp[1])
      return;

    folder = FOLDER_H->get_messages(name, "inbox");
    tmp = ({ });
    for(i=0; i<sizeof(folder); i++) {
      if(folder[i]->status == "N");
      tmp += ({ i });
      reset_eval_cost();
    }
    if(sizeof(tmp)) {
      FOLDER_H->delete_it(name, "inbox", tmp);
    }
  }
}

private void delete_name( string name ) {
  log_file("EXPIRED", "%s Timed out player deletion: %s\n", ctime(time()),
           name);

  fname = PLAYER_HANDLER->query_player_file_name(name);

  unguarded((: rm,  fname + ".o" :));
  unguarded((: rm, fname + ".o.gz" :));

#ifdef USE_RAMDISK
  fname = PLAYER_HANDLER->query_player_ram_file_name(name);
  unguarded((: rm,  fname + ".o" :));
  unguarded((: rm, fname + ".o.gz" :));
  
  fname = PLAYER_HANDLER->query_player_disk_file_name(name);
  unguarded((: rm,  fname + ".o" :));
  unguarded((: rm, fname + ".o.gz" :));
#endif
  
  "/secure/related_files"->delete_related_files(name, 1);
  PLAYER_HANDLER->remove_cache_entry(name);
} /* delete_name() */

int delete_files(string letter) {
  string *all_files;

  if (!sizeof(filter(previous_object(-1), (: interactive($1) :)))) {
    unguarded((: write_file, "/log/CHEAT", ctime( time() ) +
               ": illegal attempt to delete timed out player files using "
               "/secure/bulk_delete.\nTrace: " + back_trace() :));
    return 0;
  }
  if (!master()->high_programmer(previous_object(-1))) {
    unguarded((: write_file, "/log/CHEAT", ctime( time() ) +
               ": illegal attempt to delete timed out player files using "
               "/secure/bulk_delete.\nTrace: " + back_trace() :));
    return 0;
  }
  log_file("EXPIRED", "Manually Requested Processing of %s.\n", letter);
  current = letter;
  all_files = get_dir(PLAYER_SAVE_DIR+ letter +"/*.o");
  all_files += get_dir(PLAYER_SAVE_DIR+ letter +"/*.o.gz");
  if (!sizeof(all_files))
    return notify_fail("Directory empty.\n");
  dirs -= ({ current });
  call_out("continuous_erase", 5, all_files);
  return 1;
} /* delete_files() */

int clean_up_files(string dir) {
  int i;
  string *all_files = ({ });

  if (!sizeof(filter(previous_object(-1), (: interactive($1) :)))) {
    unguarded((: write_file, "/log/CHEAT", ctime( time() ) +
               ": illegal attempt to delete unused files using "
               "/secure/bulk_delete.\nTrace: " + back_trace() :));
    return 0;
  }
  if (!master()->high_programmer(previous_object(-1))) {
    unguarded((: write_file, "/log/CHEAT", ctime( time() ) +
               ": illegal attempt to delete unused files using "
               "/secure/bulk_delete.\nTrace: " + back_trace() :));
    return 0;
  }
  switch (dir) {
  case "artifacts" :
    all_files = get_dir("/save/"+ dir +"/*");
    break;
  case "mail" :
    all_files = get_dir("/save/"+ dir +"/*inbox.o");
    break;
  case "bank_accounts" :
  case "library" :
    all_files = get_dir("/save/"+ dir +"/*.o");
    break;
  case "vaults" :
    all_files = get_dir("/save/vaults/*/*.o");
    break;
  case ".dead_ed_files" :
    all_files = get_dir("/w/.dead_ed_files/");
    break;
  default :
    return notify_fail("Invalid directory.\n");
  }
  all_files -= ({ ".", ".." });
  if (!sizeof(all_files)) {
    return notify_fail("Directory empty.\n");
  }
  for (i = sizeof(all_files) - 1; i > -1; i--) {
    switch (dir) {
    case "artifacts" :
      call_out((: check_name :), 5 * (i + 1), all_files[i]);
      break;
    case "mail" :
      call_out((: check_name :), 5 * (i + 1),
               explode(all_files[i], "inbox")[0]);
      break;
    case ".dead_ed_files" :
      call_out((: check_name :), 5 * (i + 1),
               explode(all_files[i], "-")[0]);
      break;
    default :
      call_out((: check_name :), 5 * (i + 1), all_files[i][0..<3]);
      break;
    }
  }
  return 1;
} /* clean_up_files() */

mixed *stats() {
  return ({
    ({ "current letter", current }),
    ({ "remaining dirs", sizeof(dirs) ? implode(dirs, ", ") : 0 })
  });
}
