mapping close_up;
#define CHAR "/net/daemon/chars/"

void create() {
  seteuid(getuid());
  close_up = ([ ]);
} /* create() */

void read_callback(int fd, string mess) {
  string str;
  object ob;

  sscanf(mess, "%s\n", mess);
  if (mess != "EVERYONE") {
    str = (string)"/secure/finger"->finger_info(mess);
    if (!str) {
      previous_object()->write_fd(fd, "No one called "+mess+" has ever "+
                                      "visited "+mud_name()+".\n");
    } else {
      previous_object()->write_fd(fd, str);
    }
    previous_object()->close_fd(fd); /* Close once flushed */
    if (find_player(mess)) {
      ob = clone_object(CHAR+"in_finger_demon");
      ob->setup_finger(mess);
    }
  } else {
/*
 * Ok... In here we need to get the long finger list.
 * Which list shall we use?
 * Who list seems appropriate to me.
 */
    str = (string)"/global/player"->who_string(75);
    if (!str) {
      previous_object()->write_fd(fd, "Some sort of fun internal error "+
                                      "occured.  You are happy.\n");
    } else {
      previous_object()->write_fd(fd, str);
    }
    previous_object()->close_fd(fd);
  }
} /* read_callback() */
