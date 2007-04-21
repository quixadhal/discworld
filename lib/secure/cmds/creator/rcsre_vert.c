/*  -*- LPC -*-  */
/*
 * $Id: rcsre_vert.c,v 1.2 2002/08/05 04:10:07 presto Exp $
 */
inherit "/cmds/base";
#include <creator.h>

#define CMD_NUM 8

mapping globals = ([]), ret = ([]);

#define TP globals[fd]
#define RET ret[fd]

mixed cmd(string arg) {
  int nfiles, fd;
  string bit, *bits, *cmd;
  object *things;
  
  if (!arg) {
    return notify_fail("rcsrevert: No arguments.\n");
  }

  bits = explode(arg, " ");
  arg = "";
  bits -= ({ "", 0 });

  foreach(bit in bits) {
    string *files, file;
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
    return notify_fail("rcsrevert: no such file "+arg+".\n");
  }

  cmd = allocate(3);
  cmd[0] = "-w" + this_player()->query_name();
  cmd[1] = "-u";
  cmd[2] = "-l";
  cmd += explode(arg, " ");
#ifdef DEBUG  
  printf("CMD: %O\n", cmd);
#endif  
  fd = external_start(CMD_NUM, cmd, "read_call_back", "write_call_back",
                      "close_call_back");
    
  TP = this_player();
  RET = "";
  return 1;
}

// I _think_ this is the function to write the comment string to the ci command
void read_call_back(int fd, mixed mess) {
  mess = replace(mess, "/home/atuin/lib", "");
  RET += mess;
}

void write_call_back(int fd) {
  tell_object(TP, "rcsrevert: write_call_back() called.\n");
}

void close_call_back(int fd) {
  if (RET != "") {
    TP->more_string(RET);
  } else {
    tell_object(TP, "rcsin completed.\n");
  }
  
  map_delete(ret, fd);
  map_delete(globals, fd);
}
