/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: resetpw.c,v 1.7 2003/03/25 20:58:32 ceres Exp $
*/
#include <parser.h>
#include <liaison.h>
#include <player_handler.h>
inherit "/cmds/base";

string basename;
string file;

int do_reset(string player, string newpw) {
  string password;
  string *bits, t1, t2;
  int i;

  if(base_name(previous_object())[0..12] != "/secure/login" &&
     base_name(previous_object())[0..13] != "/secure/nlogin") {
    if (!sizeof(filter(previous_object(-1), (: interactive($1) :)))) {
      unguarded((: write_file, "/log/CHEAT", ctime( time() ) +
                 ": illegal attempt to reset password.\nTrace: " + back_trace()
                 :));
      return 0;
    }
    if (!master()->high_programmer(previous_object(-1)) &&
        !(master()->query_director(previous_object(-1)) &&
          this_player(1)->query_name() == "/d/liaison/master"->query_lord())) {
      unguarded((: write_file, "/log/CHEAT", ctime( time() ) +
                 ": illegal attempt to reset password.\nTrace: " + back_trace()
                 :));
      return 0;
    }
  }

  seteuid("Root");

  /* Restore them... */
  if (PLAYER_HANDLER->test_user(player)) {
    basename = "/save/ramdisk/players/" + player[0..0] + "/" + player;
    if(file_size(basename+".o.gz")>0 ||
       file_size(basename+".o")>0) {
      unguarded((: uncompress_file(basename+".o.gz") :));
    } else {
      basename = "/save/players/" + player[0..0] + "/" + player;
      if(file_size(basename+".o.gz")>0 ||
         file_size(basename + ".o") > 0) {
        unguarded((: uncompress_file(basename+".o.gz") :));
      } else {
        // Player might be set to delete
        basename = "/save/players/delete/" + player;
        if(file_size(basename+".o.gz")>0) {
          unguarded((: uncompress_file(basename+".o.gz") :));
        }
      }
    }
    file = unguarded( (: read_file(basename + ".o") :));
    if(!file) {
      write("Error reading file " + basename + ".\n");
      return notify_fail("Error reading file " + basename +".o\n");
    }
    bits = explode(file, "\n");
    
    for (i = 0; i < sizeof(bits); i++) {
      if (bits[i][0..strlen("password")-1] == "password") {
        t1 = bits[i];
        password = restore_variable(bits[i][strlen("password") + 1..]);
        break;
      }
    }
    password = crypt(newpw, newpw[0..1]);
    t2 = "password " + save_variable(password);

    file = replace(file, t1, t2);
    unguarded( (: rm(basename + ".old") :) );
    unguarded( (: rename(basename+".o", basename+".old") :) );
    unguarded( (: write_file(basename + ".o", file) :));
    unguarded( (: compress_file(basename+".o") :));
    PLAYER_HANDLER->remove_cache_entry(player);
    return 1;

  }
  return 0;
}

/*
 * This will reset a players password.
 */
mixed cmd(string player, string newpw) {
  if(!newpw || newpw == "")
    return notify_fail("Syntax: resetpw <player> <new password>\n");

  if(do_reset(player, newpw)) {
    write(sprintf("Reset password for %s to %s\n", player, newpw));
  } else {
    write("Unable to find player "+player+".\n");
  }
  
  return 1;
} /* cmd() */

mixed *query_patterns() {
  return ({ "<word> <string>", (: cmd($4[0], $4[1]) :),
            });
} /* query_patterns() */
