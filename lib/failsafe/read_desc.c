inherit "/failsafe/desc";
inherit "/failsafe/simul_efun";
/* this one also contians info on read_messages... */

static string read_mess;

void set_read_mess(string str) { 
  if (!read_mess && str)
    add_action("read", "read");
  read_mess = str; 
}

string query_read_mess() { return read_mess; }

void add_read_mess(string str) { 
  if (!read_mess) {
    add_action("read", "read");
    read_mess = "";
  }
  read_mess += str; 
}

init() {
  if (read_mess)
    add_action("read", "read");
}

int read(string str) {
  if (member_array(this_object(), find_match(str, environment())) == -1) {
    notify_fail("Usage: read <object>\n");
    return 0;
  }
  write(read_mess);
  return 1;
}

string long(string str) {
  if (read_mess)
    return ::long(str)+"It appears to have something written on it.\n";
  return ::long(str);
}
