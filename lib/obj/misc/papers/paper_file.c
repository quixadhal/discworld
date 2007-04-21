/* This will deal with happy and fluffy files. and turn them into a page. */
inherit "/obj/misc/paper";

string file_name;

void create() {
  seteuid(getuid());
  ::create();
} /* create() */

void set_file_name(string fname) {
  file_name = fname;
} /* set_file_name() */

string query_file_name() {
  return file_name;
} /* query_file_name() */

mixed *query_read_mess() {
  return read_file(file_name);
} /* query_read_mess() */

void init() {
  this_player()->add_command("read", this_object());
  ::init();
} /* init() */
