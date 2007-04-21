/*  -*- LPC -*-  */
/*
 * $Id: exe_c.c,v 1.9 2000/07/01 01:18:28 pinkfish Exp $
 * 
 */
#include <cmds/options.h>
inherit "/cmds/base";

#define LOG_FILE "/d/admin/log/EXEC.log"

mixed do_exec(string str, string ref suc) {
  mixed ret;
  string file;
  string wiz_dir;
  string file_header;
  object ob;
        
  if (!this_player()) {
    suc = "fail";
    return 0;
  }
  wiz_dir = "/w/" + this_player()->query_name();
  if (file_size(wiz_dir) != -2) {
    suc = "fail";
    return notify_fail("Directory: " + wiz_dir + " does not exist.\n");
  }
  file = wiz_dir + "/exec_tmp";
  if (ob = find_object(file)) {
    file->dest_me();
    if(ob)
      destruct(ob);
  }
  if (file_size(file + ".c") > 0) {
    rm(file+".c");
  }
  unguarded((: write_file(LOG_FILE, ctime(time()) + " - " + 
                          this_player()->query_name() + ": " + $(str) +
                          "\n") :));
  file_header = this_player()->query_property(OPTION_EXEC_INCLUDE);
  if (file_header) {
     write_file(file + ".c", "#include \"" + file_header + "\"\n\n");
  }
  write_file(file + ".c",
             "void dest_me() { destruct(this_object()); }\n"
             "mixed do_call() {\n" + str + ";\n}\n");
  suc = catch(ret = file->do_call());

  if ((ob = find_object(file))) {
    ob->dest_me();
  }
  rm(file + ".c");
  return ret;
} /* do_exec() */

int cmd(string str) {
  string err;

  mixed ret = do_exec(str, ref err);
  if (err == "fail")
    return 0;
  if (err == 0) {
    this_player()->more_string(sprintf("\nReturns: %O\n", ret),
                               "Exec results");
  } else {
    printf("Exec failed: %s", err);
  }
  return 1;
}
