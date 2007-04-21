/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: diff.c,v 1.3 2003/03/25 20:02:48 wodan Exp $
 * $Log: diff.c,v $
 * Revision 1.3  2003/03/25 20:02:48  wodan
 * Forcibly released due to inactivity
 *
 * Revision 1.2  1998/02/24 15:50:32  wodan
 * number of external command was changed
 *
 * Revision 1.1  1998/01/06 05:10:49  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";
#include <creator.h>

mapping globals = ([]), files = ([]), ret = ([]);

#define TP globals[fd]
#define FILE files[fd]
#define RET ret[fd]

mixed cmd(string arg) {
  int fd, num = 0;
  string last = "";
  
  if(!arg)
    arg = "";
  else {
    string bit, *bits = explode(arg, " ");
    arg = "";
    bits -= ({""});

    foreach(bit in bits){
      mixed *st;
      if (bit[0] == '-'){
        continue;
      }
      bit = this_player()->get_path(bit)[1..];
      if (bit == "")
        return notify_fail("Your wombles just expired.\n");
      if (!master()->valid_read(bit, this_player(), 0))
        return notify_fail("Permission denied: " + bit + " .\n");
      if(sizeof(st = unguarded((:stat, bit:)))){
        if(stringp(st[0]))
          return notify_fail("Can't use diff on directories.\n");
      }
      arg += " " + last;
      last = bit;
      num ++;
    }
    
  } 

  if (num == 2)
    arg += " " + last;

  last = "/" + last;

  if (num > 3)
    return notify_fail("Can't compare more than two files");

  if (num == 3 && file_size(last) > 0)
    return notify_fail("File " + last + " exists.\n");

  fd = external_start(7, arg, "read_call_back", "write_call_back",
                      "close_call_back");
  if (fd == -1)
    return notify_fail("diff failed somehow.\n");

  TP = this_player();
  if (num == 3 && master()->valid_write(last, this_player(), 0))
    FILE = last;
  RET = "";

  return 1;
}

void read_call_back(int fd, mixed mess) {
  mess = replace(mess, "/usr/bin/", "");
  RET += mess;
}

void write_call_back(int fd) {
  tell_object(TP, "diff: Whoops! fatal error.\n");
}

void close_call_back(int fd) {
  if (FILE)
    write_file(FILE, RET);
  else {
    if(RET == "")
      RET = "those files are the same";
    TP->more_string(RET);
  }
  map_delete(ret, fd);
  map_delete(files, fd);
  map_delete(globals, fd);
}
