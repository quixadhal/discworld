#include "socket.h"
#include "inet.h"

object pl;
string who;

void dest_me();

void create() {
  seteuid(getuid());
} /* create() */

int do_finger(string str) {
  string name, host;
  object ob;

  if (sscanf(str, "%s@%s", name, host) == 2) {
    lower_case(name);
/*
    if (name == "")
      name = "EVERYONE";
 */
    ob = clone_object(file_name(this_object()));
    ob->finger_away(name, host, this_player());
    return 1;
  }
/*
 * Should be a local finger...  no idea why they want to do that though.
 * shrug.
 */
  return 0;
} /* do_finger() */

void finger_away(string name, string host, object me) {
  who = name;
  pl = me;
  INETD->open_to_service("finger", INETD_TYPE, host, name);
  call_out("dest_me", 5*60*60);
} /* finger_away() */

void read_callback(int fd, string mess) {
  tell_object(pl, mess);
  pl->finger_callback(2, mess);
} /* read_callback() */

void connected(int fd) {
  if (who)
    previous_object()->write_fd(fd, who+"\n");
  else
    previous_object()->write_fd(fd, "EVERYONE\n");
  who = 0;
} /* write_callback() */

void close_callback() {
  pl->finger_callback(0);
  dest_me();
} /* close_callback() */

void failed(string reason) {
  pl->finger_callback(1, reason);
  tell_object(pl, "We failed for "+reason+" reason.\n");
  dest_me();
} /* failed() */

void dest_me() {
  destruct(this_object());
} /* dest_me() */
