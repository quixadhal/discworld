/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: rehash.c,v 1.3 2002/07/18 02:10:43 pinkfish Exp $
 * $Log: rehash.c,v $
 * Revision 1.3  2002/07/18 02:10:43  pinkfish
 * Make it work with the armoury properly.
 *
 * Revision 1.2  2002/07/18 02:05:01  pinkfish
 * make it handle rehashing the armoury and help files.
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* rehash command, trial out by Turrican for a commands daemon. */
#include <armoury.h>

#define CMD_D "/secure/command"

mixed cmd(string str) {
  if (!str)
    return notify_fail("Syntax: rehash <path>\n");
  if (file_size(str) != -2)
    return notify_fail(str+": Path not found.\n");

  if (str[0..4] == "/doc/") {
     "/cmds/player/help"->rehash_specific_dir(str);
     write("Rehashed help dir: " + str + "\n");
     return 1;
  }

  if (str[0..4] == "/obj/" || str[0..2] == "/d/") {
     if (ARMOURY->rehash_specific_dir(str)) {
        write("Rehashed armoury dir: " + str + "\n");
        return 1;
     }
  }

  CMD_D->eventRehash(str);
  write(str+": Rehashed.\n");
  return 1;
}

void dest_me() {
  destruct(this_object());
}

void clean_up() {
  dest_me();
}

void reset() {
  dest_me();
}
