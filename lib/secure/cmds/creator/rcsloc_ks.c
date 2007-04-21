/*  -*- LPC -*-  */
/*
 * $Id: rcsloc_ks.c,v 1.2 2000/10/30 00:58:14 presto Exp $
 */
inherit "/cmds/base";
#include <creator.h>

#define DEBUG 1

#define CMD_NUM 5

mapping globals = ([]), ret = ([]);

#define TP globals[fd]
#define RET ret[fd]

mixed cmd(string arg) {
  int nfiles = 0;
  string bit, *bits, file, locker, *files, fname;
  object *things;
  
  if (!arg) {
    return notify_fail("rcslocks: No arguments.\n");
  }

  bits = explode(arg, " ");
  arg = "";
  bits -= ({ "", 0 });

  files = ({ });
  foreach(bit in bits) {
    if (bit[0] == '-') {
      arg += (" " + bit);
      continue;
    }
    if (sizeof(files = this_player()->get_files(bit))) {
      foreach(file in files) {
        arg += (" " + file[1..]);
        nfiles++;
      }
    } else {
      if(sizeof(things = WIZ_PRESENT->wiz_present(bit, this_player()))) {
        file = file_name(things[0]);
        sscanf(file, "%s#%*d", file);
        if (file_size(file) <= 0)
          file += ".c";

        arg += (" " + file[1..]);
        nfiles++;
      }
    }
  }
  
  if(!nfiles) {
    return notify_fail("rcslocks: no such file "+arg+".\n");
  }

  // Now check if the file is locked and who it is locked by!

  foreach(fname in files) {
    string lockline;

    bits = explode(fname, "/");
    file = "/" + (string)implode(bits[0..<2], "/") + "/RCS/" + bits[<1] + ",v";

    if (file_size(file) < 0) {
      write("File " + fname + " is not on RCS.\n");
    } else {
      lockline = read_file(file, 4, 1);
      if (strsrch(lockline, "locks; strict:") == -1) {
        bits = explode(read_file(file, 5, 1), ":");
        if(bits[0][0] == '\t') {
          locker = bits[0][1..];
          write("File " + fname + " locked by " + locker + ".\n");
        }
        else write(fname + " is not locked.\n");
      }
    }
  }
  return 1;
}
