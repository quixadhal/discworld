/*  -*- LPC -*-  */
/*
 * $Id: lo_ad.c,v 1.2 1999/05/27 13:53:08 turrican Exp $
 */

inherit "/cmds/base";

mixed cmd(string str) {
  string *filenames, err, fname, *failed, *succ, ret, load_err;
  int size;

  if (!str) {
    return notify_fail("Load what?\n"); 
  }
  filenames = this_player()->get_cfiles(str);
  if (!sizeof(filenames)) {
    return notify_fail("No such object.\n");
  }
  failed = succ = ({ });
  load_err = "";
  foreach (fname in filenames) {
    if (file_size(fname) < 0) {
      failed += ({ fname });
      continue;
    }
    if ((err = catch(fname->load_up_with_yellow()))) {
      load_err += sprintf("Failed to load %s, error: %s", fname, err);
    } else {
      succ += ({ fname });
    }
  }
  ret = "";
  if ((size = sizeof(failed))) {
    ret += sprintf("%s %s not %sregular file%s.\n",
                   query_multiple_short(failed), (size == 1 ? "is" : "are"),
                   (size == 1 ? "a " : ""), (size == 1 ? "" : "s"));
  }
  if (strlen(load_err)) {
    ret += load_err;
  }
  if (sizeof(succ)) {
    ret += sprintf("$I$5=$C$Loaded %s.\n", query_multiple_short(succ));
  }
  this_player()->show_message("$P$Load$P$" + ret);
  return 1;
}
