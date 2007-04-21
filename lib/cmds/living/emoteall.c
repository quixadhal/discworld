/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: emoteall.c,v 1.5 2002/03/17 12:21:51 mansarde Exp $
 * $Log: emoteall.c,v $
 * Revision 1.5  2002/03/17 12:21:51  mansarde
 * Fixed previous fix.  Can you believe I missed a + " " ? Duh.
 *
 * Revision 1.4  2002/03/16 18:41:27  mansarde
 * Fixed the colouring so it colours the name, not just the message.
 *
 * Revision 1.3  2001/11/07 22:28:28  pinkfish
 * Make it bold red.
 *
 * Revision 1.2  2001/07/23 13:07:11  taffyd
 * Made to use colour in emoteall
 *
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/
#include <player.h>
#include <drinks.h>

inherit "/cmds/base";

#define TP this_player()
#define BEEP sprintf("%c",7)

void my_mess(string fish, string erk);

mixed cmd(string str) {
  if(!TP->query_creator() && !TP->query_property("emoteall")) {
    return notify_fail(NOT_ALLOWED);
  }

  if(!str || str == "") {
    return notify_fail("Syntax : emoteall <string>\n");
  }

  if(!TP->query_creator() &&
     TP->adjust_sp(-EMOTEALL_COST*((strlen(str)/10)+1)+100) < 0) {
    return notify_fail(NO_POWER);
  }

  str = replace(str, BEEP, "");
  if (TP->query_volume(D_ALCOHOL)) {
    str = TP->drunk_speech(str);
  }
  log_file("ECHOS", ctime(time())+" "+TP->query_name()+" emotes "+
           "to all: "+str+"\n");
  str += "%^RESET%^";
  my_mess("You emoteall : ", TP->query_cap_name()+" " + str);
  TP->comm_event(users(), "player_emote_all", "%^BOLD%^%^RED%^" + TP->query_cap_name() + " " + str + "\n");
  return 1;
}

void my_mess(string fish, string erk) {
  if(!interactive(TP)) return;
  printf("%s%-=*s\n", fish, (int)TP->query_cols()-strlen(fish), 
          TP->fix_string(erk));
} /* my_mess() */
