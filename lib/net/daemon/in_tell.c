mapping close_up;
#define TELL_DEMON "/net/daemon/chars/tell_demon"

void create() {
  seteuid(getuid());
  close_up = ([ ]);
} /* create() */

void read_callback(int fd, string mess) {
  string str, person, mud, who, message;
  object ob;

  if (sscanf(mess, "%s@%s tells %s: %s", person, mud, who, message) != 4) {
    previous_object()->write_fd(fd, "Tell_Server@"+mud_name()+" tells "+
                                    "whoever-the-heck-this-is: You have "+
                                    "an incorrect message format.\n");
  } else if (!find_player(who)) {
    previous_object()->write_fd(fd, "Tell_Server@"+mud_name()+" tells "+
                                    person+": Either "+who+" isn't logged on "+
                                    "or you are a right womble.\n");
  } else {
    ob = clone_object(TELL_DEMON);
    ob->setup_tell(person, mud, who, message);
    previous_object()->write_fd(fd, "Tell_server@"+mud_name()+" tells "+
                                    person+": The message is on its way.\n");
  }
  previous_object()->close_fd(fd);
} /* read_callback() */
