int save_ed_setup(object wiz, int setup) {
  wiz->set_ed_setup(setup);
  return 1;
} /* save_ed_setup() */

int retrieve_ed_setup(object wiz) {
  return (int)wiz->query_ed_setup();
} /* retrieve_ed_setup() */

/*
 * make_path_absolute:
 *   Used by ed.c to expand relative path names on read and write.
 */
string make_path_absolute(string str) {
  if (this_player()) {
    return (string)this_player()->get_path(str);
  }
} /* make_path_absolute() */

/*
 * Give a file name for edit preferences to be saved in.
 */
string get_ed_buffer_save_file_name(string file) {
  string *file_ar;

  file_ar = explode(file,"/") - ({ "" });
  file = file_ar[sizeof(file_ar)-1];
  write("File saved in \"/w/.dead_ed_files/" +
        this_player()->query_name() + "-" + file + "\"\n");
  return "/w/.dead_ed_files/" + this_player()->query_name() + "-" + file;
} /* get_ed_buffer_save_file_name() */
