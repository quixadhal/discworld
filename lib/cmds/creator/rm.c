/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: rm.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: rm.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* rm command, trial out by Turrican for a commands daemon. */

inherit "/cmds/base";

mixed cmd(string str) {
  int fsize, loop;
  string *filenames;
 
  seteuid(geteuid(this_player()));

  if (!str) {
    notify_fail("Usage: rm file [file ...]\n");
    return 0;
  }
  filenames = this_player()->get_files(str);
  if (!sizeof(filenames)) {
    notify_fail("No such file : "+ str+"\n");
    return 0;
  }
  for (loop = 0; loop < sizeof(filenames); loop++) {
    str = filenames[loop];
    fsize = file_size(str);
    if (str[<2..] == "/." || str[<3..] == "/..")
      continue;
    if (fsize == -1) {
      notify_fail("No such file or directory.\n");
      return 0;
    }
    if (fsize == -2) {
      if (!rmdir(str)) {
        notify_fail("Couldn't rm directory: " + str + "\n");
        return 0;
      }
    } else if (!rm(str)) {
      notify_fail("Can't remove file.\n");
      return 0;
    }
  }
  write("Ok.\n");
  return 1;
}
