#include "socket.h"
#include "inet.h"

object pl;
string who, message;

void dest_me();

void create() {
  seteuid(getuid());
} /* create() */

/* Whats all this then? */
int do_tell(string str) {
  string name, host, mess;
  object ob;

  if (sscanf(str, "%s@%s %s", name, host, mess) == 3) {
    lower_case(name);
    if (name == "")
      name = "EVERYONE";
    ob = clone_object(file_name(this_object()));
    ob->tell_away(name, host, mess, this_player());
    return 1;
  }
  return 0;
} /* do_finger() */

void tell_away(string name, string host, string mess, object me) {
  who = name;
  pl = me;
  message = mess;
  INETD->open_to_service("tell", INETD_TYPE, host);
  call_out("dest_me", 5*60*60);
} /* finger_away() */

void read_callback(int fd, string mess) {
  tell_object(pl, mess);
  pl->tell_callback(2, mess);
} /* read_callback() */

void connected(int fd) {
  previous_object()->write_fd(fd, pl->query_name()+"@"+mud_name()+
                                  " tells "+who+": "+message+"\n");
} /* write_callback() */

void close_callback() {
  pl->tell_callback(0);
  dest_me();
} /* close_callback() */

void failed(string reason) {
  pl->tell_callback(1, reason);
  tell_object(pl, "We failed for "+reason+" reason.\n");
  dest_me();
} /* failed() */

void dest_me() {
  destruct(this_object());
} /* dest_me() */
