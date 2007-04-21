/*  -*- LPC -*-  */
/*
 * $Id: rcsco_mment.c,v 1.3 2002/07/05 19:04:59 pinkfish Exp $
 */
inherit "/cmds/base";
#include <creator.h>

#define CMD_NUM 8

mapping globals = ([]), ret = ([]);

#define TP globals[fd]
#define RET ret[fd]

class rcscomment_args {
  object player;
  string arg;
  string revision;
}

mixed cmd(string arg) {
  int nfiles = 0;
  string bit, *bits;
  object *things;
  string* files;
  string file;
  
  if (!arg) {
    return notify_fail("rcscomment: No arguments.\n");
  }

  bits = explode(arg, " ");
  arg = "";
  bits -= ({ "", 0 });

  foreach(bit in bits) {
    if (bit[0] == '-') {
      arg += (" " + bit);
      continue;
    }

    files = this_player()->get_files(bit);
    if (sizeof(files)) {
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
    return notify_fail("rcscomment: no such file "+arg+".\n");
  }

  printf("Enter a revision number: ");
  input_to("get_rev", 0, new(class rcscomment_args, player : this_player(),
                             arg : arg));
  return 1;
} /* cmd() */

protected void get_rev(string revision, class rcscomment_args args) {
  if (!strlen(revision)) {
    printf("No revision given, aborting.\n");
    return;
  }
  args->revision = revision;

  printf("Enter a comment.\n");
  args->player->do_edit(0, "do_comment", 0, 0, args);
} /* get_rev() */

void do_comment(string comment, class rcscomment_args args) {
  int fd;
  string *cmd = allocate(2);

  if (!strlen(comment)) {
    printf("No comment given, aborting.\n");
    return;
  }

  cmd[0] = sprintf("-w%s", args->player->query_name());
  cmd[1] = sprintf("-m%s:%s", args->revision, comment);
  cmd += explode(args->arg, " ");
#ifdef DEBUG  
  printf("CMD: %O\n", cmd);
#endif  
  fd = external_start(CMD_NUM, cmd, "read_call_back", "write_call_back",
                      "close_call_back");

  TP = args->player;
  RET = "";
}

void read_call_back(int fd, mixed mess) {
  mess = replace(mess, "/home/atuin/lib", "");
  RET += mess;
}

void write_call_back(int fd) {
  tell_object(TP, "rcscomment: write_call_back() called.\n");
}

void close_call_back(int fd) {
  if (RET != "") {
    TP->more_string(RET);
  } else {
    tell_object(TP, "rcscomment completed.\n");
  }

  map_delete(ret, fd);
  map_delete(globals, fd);
}
