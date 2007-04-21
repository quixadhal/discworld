/*  -*- LPC -*-  */
/*
 * $Id: indent.c,v 1.2 2003/03/25 19:11:03 pinkfish Exp $
 */
inherit "/cmds/base";
#include <creator.h>

#define CMD_NUM 4

mapping globals = ([]), ret = ([]), cmds = ([]);

#define TP globals[fd]
#define RET ret[fd]
#define CMDS cmds[this_player()]

mixed cmd(string arg) {
  int nfiles = 0;
  int fd;
  string bit;
  string *bits;
  object *things;
  
  if (!arg) {
    return notify_fail("rcsin: No arguments.\n");
  }

  bits = explode(arg, " ");
  arg = "";
  bits -= ({ "", 0 });

  foreach(bit in bits) {
    string files = "", file;
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
        if (file_size(file) <= 0) {
          file += ".c";
        }

        arg += (" " + file[1..]);
        nfiles++;
      }
    }
  }

  if(!nfiles) {
    return notify_fail("rcsin: no such file "+arg+".\n");
  }

  fd = external_start(CMD_NUM, explode(arg, " "),
                      "read_call_back", "write_call_back",
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
  tell_object(TP, "rcsin: Write_call_back called.\n");
}

void close_call_back(int fd) {
  string file, *file_stuff;
  int i;

  if (RET != "") {
    TP->more_string(RET);
    file_stuff = explode(RET, "\n");
    i = 0;
    while (i + 2 < sizeof(file_stuff))  {
      if (file_stuff[i + 2] == "done")  {  /* success! */
        sscanf(file_stuff[i], "%*s  <--  %s", file);
        RCS_HANDLER->remove_lock(TP, file);
        i += 3;
      }
      else if (sscanf(file_stuff[i], "%*sNo such file or directory"))  {
        ++i;
      }
      else if (file_stuff[i + 1][0..2] == "ci:") {
          i += 2;
      }
      else {
          i += 3;
      }
    }
  } else {
    tell_object(TP, "rcsin completed.\n");
  }
  
  map_delete(ret, fd);
  map_delete(globals, fd);
}
