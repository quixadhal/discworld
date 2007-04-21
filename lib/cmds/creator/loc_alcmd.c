/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: loc_alcmd.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: loc_alcmd.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* localcmd command, trial out by Turrican for a commands daemon. */
/* New localcmd Pinkfish 19/5/93 */

#include <creator.h>

mixed cmd(string str) {
  int flags, i, j, pos;
  mixed flag_types;
  object *obs;
  string *coms, flag;
  mixed *junk;

  if (!str) str = "";
  flag_types = ({ "priority", "star", "function", "object" });
  while (strlen(str) && str[0] == '-') {
    if (sscanf(str, "-%s %s", flag, str) != 2) {
      flag = str[1..100];
      str = "";
    }
    i = member_array(flag, flag_types, 1);
    if (i != -1) {
      flags |= 1 << i;
    }
  }
  if (!flags) {
    if (!strlen(str))
      coms = actions_defined(this_player());
    else {
      obs = WIZ_PRESENT->wiz_present(str, this_player());
      if (!sizeof(obs)) {
        notify_fail("Could not find "+str+" to get the commands off.\n");
        return 0;
      }
      coms = map(obs, (: actions_defined($1) :));
    }
  } else {
    if (!strlen(str))
      junk = actions_defined(0, 0, flags);
    else {
      obs = WIZ_PRESENT->wiz_present(str, this_object());
      if (!sizeof(obs)) {
        notify_fail("Could not find "+str+" to get the commands off.\n");
        return 0;
      }
/* I should be able to do this...  
 * Darn didnt work...  sigh...
      junk = actions_defined(obs, 0, flags);
 */
      junk = map(obs, (: actions_defined($1, 0, $(flags)) :));
    }
    coms = ({ });
    for (i=1;i<sizeof(junk);i+=2) {
      pos = 0;
      str = junk[i-1];
      if ((flags&0xc) == 0xc) {
        str = WIZ_PRESENT->desc_f_object(junk[i][0])+"->"+junk[i][1]+":"+str;
        pos += 2;
      } else if (flags&0x8) {
/* First object... */
        str = WIZ_PRESENT->desc_object(junk[i][0])+":"+str;
        pos++;
      } else if (flags&0x4) {
        str = junk[i][0]+"%"+str;
        pos++;
      }
      if (flags&0x2) {
        if (j = junk[i][pos]) {
          if (strlen(junk[i-1]) == j)
            str += "*";
          else {
            j = strlen(str)-junk[i][pos];
            str = str[0..j]+"*"+str[j+1..10000];
          }
        }
        pos++;
      }
      if (flags&1) {
        str += "|"+junk[i][pos];
      }
      coms += ({ str });
    }
  }
  this_player()->more_string( sprintf( "%-#*s\n", 
      (int)this_player()->query_cols(),
      implode( coms, "\n" ) ), "Commands" );
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
