  /*  -*- LPC -*-  */
/*
 * $Id: rcso_ut.c,v 1.12 2003/04/23 20:58:33 pinkfish Exp $
 */
inherit "/cmds/base";
#include <creator.h>

#define CMD_NUM 3

mapping globals = ([]), files = ([]), ret = ([]);

#define TP globals[fd]
#define RET ret[fd]

mixed cmd(string arg) {
  int fd, nfiles = 0;
  int force;
  string cmd;
  string bit, *bits;
  object *things;
  
  if(!arg) {
    return notify_fail("rcsout: No arguments.\n");
  }

  bits = explode(arg, " ");
  arg = "";
  bits -= ({""});

  if (bits[0] == "-f") {
    force = 1;
    bits = bits[1..];
  }

  foreach(bit in bits){
    string *files = ({ }), file;
    if (bit[0] == '-'){
      arg += (" " + bit);
      continue;
    }

    if (sizeof(things = WIZ_PRESENT->wiz_present(bit, this_player()))) {
      int i;

      files = uniq_array(map(things, (: base_name($1) :)));
      for (i = 0; i < sizeof(files); i++) {
        if (file_size(files[i]) <= 0) {
          files[i] += ".c";
        }
      }
    }

    files += this_player()->get_files(bit);

    if (!sizeof(files)) {
      file = this_player()->get_path(bit);
      if(master()->valid_write(file, geteuid(this_player()), "cmd")) {
        arg += (" " + file[1..]);
        nfiles++;
      } else {
        notify_fail("You do not have write access to "+file+"\n");
      }
    } else {
      foreach(file in uniq_array(files)) {
        if(master()->valid_write(file, geteuid(this_player()), "cmd")) {
          arg += (" " + file[1..]);
          nfiles++;
        } else {
          notify_fail("You do not have write access to "+file+"\n");
        }
      }
    }
  }

  if(!nfiles) {
    return 0;
  }

  if (force) {
     cmd = " -l -f -w" + this_player()->query_name() +arg;
  } else  {
     cmd = " -l -w" + this_player()->query_name() +arg;
  }
#ifdef DEBUG    
  printf("CMD: %s\n", cmd);
#endif    
  fd = external_start(CMD_NUM, cmd, "read_call_back", "write_call_back",
                      "close_call_back");
    
  TP = this_player();
  RET = "";
  return 1;
}

void read_call_back(int fd, mixed mess) {
  mess = replace(mess, "/home/atuin/lib", "");
  RET += mess;
}

void write_call_back(int fd) {
  tell_object(TP, "rcsout: Whoops! fatal error.\n");
}

void close_call_back(int fd) {
  string file, *file_stuff;
  int i;

  if(RET != "")  {
    TP->more_string(RET);
    file_stuff = explode(RET, "\n");
    i = 0;
    while (i + 2 < sizeof(file_stuff))  {
      if (file_stuff[i + 2] == "done")  {  /* success! */
        sscanf(file_stuff[i], "%*s  -->  %s", file);
        RCS_HANDLER->add_lock(TP, file);
        i += 3;
      }
      else if (sscanf(file_stuff[i], "%*sNo such file or directory"))  {
        ++i;
      }
      else if (file_stuff[i + 1][0 .. 2] == "co:")  i += 2;
      else i += 3;
    }
  }
  else
    tell_object(TP, "rcsout completed.\n");

  map_delete(ret, fd);
  map_delete(globals, fd);
}
