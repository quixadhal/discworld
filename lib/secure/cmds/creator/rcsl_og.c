/*  -*- LPC -*-  */
/*
 * $Id: rcsl_og.c,v 1.5 2001/05/24 06:47:25 taffyd Exp $
 */
inherit "/cmds/base";
#include <creator.h>

#define CMD_NUM 1

mapping globals = ([]), files = ([]), ret = ([]);

#define TP globals[fd]
#define RET ret[fd]

mixed cmd(string arg) {
  int fd, nfiles = 0;
  string cmd;
  string bit, *bits;
  object *things;
  
  if(!arg)
    return notify_fail("rcslog: No arguments.\n");


  notify_fail("rcslog: No such file "+arg+"\n");
  bits = explode(arg, " ");
  arg = "";
  bits -= ({""});

  foreach(bit in bits){
    string *files = ({ }), file;
    if (bit[0] == '-'){
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

  if(!nfiles)
    return 0;
    
  cmd =  arg;
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
  tell_object(TP, "rcslog: Whoops! fatal error.\n");
}

void close_call_back(int fd) {
  if(RET != "")
    TP->more_string(RET);
  else
    tell_object(TP, "rcslog completed.\n");
  map_delete(ret, fd);
  map_delete(globals, fd);
}
