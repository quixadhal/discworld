/*
 * returns the owner of ob (for error logging)
 * all calls to this should call get_prg_name()
 */
string get_wiz_name(mixed file) { 
  object ob;

  if (!previous_object())
     return "root";
  if (objectp(file)) file = file_name(file);
  file = explode(file, "/") - ({ "" });
  if (file[0] == "w") {
      return file[1];
  }
  if (file[1] == "d") {
      return "dom";
  }
  return "root";
} /* get_wiz_name() */

string get_dom_name(mixed file) {
  if (objectp(file)) file = file_name(file);
  file = explode(file, "/") - ({ "" });
  if (file[0] == "d" && sizeof(file) >= 2)
    return file[1];
} /* get_dom_name() */

/*
 * return the owner of file (for error logging)
 */
string get_prg_name(mixed file) {
  object obj;

  if (objectp(file)) file = file_name(file);
  file = explode(file, "/") - ({ "" });
  if (file[0] == "w") return file[1];
  return "root";
} /* get_prg_name() */

/*
 * Write an error message into a log file. The error occured in the object
 * 'file', giving the error message 'message'.
 */
void log_error(string file, string message)
{
  string name;

  if (this_player() && this_player()->query_wizard()) {
      tell_object(this_player(),"E "+message);
  }
  name = get_wiz_name(file);
  if (name == 0) {
      name = "log";
  }
  if (name == "log") {
      log_file(name, message);
  } else if (name == "dom") {
      write_file("/d/"+get_dom_name(file)+"/error-log", message);
  } else if (name != "root") {
      write_file("/w/"+name+"/error-log", message);
  }
} /* log_error() */

void do_log(string person, string text) {
  if (file_name(previous_object())[0..7] != "/global/")
    return ;
  rm("/w/"+person+"/"+PLAYER_ERROR_LOG);
  write_file("/w/"+person+"/"+PLAYER_ERROR_LOG, text);
} /* do_log() */
