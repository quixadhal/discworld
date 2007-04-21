/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: telnet.c,v 1.1 1998/01/06 04:54:05 ceres Exp $
 * $Log: telnet.c,v $
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 * 
*/
/*
 * This handels the telnet sub option stuff.  Will be mostly for handleing
 * terminal types.  Though you can get other information out of the
 * telnet connection.
 */
#include "telnet.h"

nosave string last_term;
nosave int got_terminal;

void check_terminal_type(string type);

void telnet_suboption(string str) {
  switch (str[0]) {
    case TELOPT_TTYPE :
      if (str[1] != TELQUAL_IS)
        return ;
      check_terminal_type(str[2..1000]);
      break;
  }
} /* telnet_suboption() */

/* Send all the relevant junk down to get the information we want. */
void player_connected() {
/* Turn on the option */
  printf("%c%c%c", IAC, DO, TELOPT_TTYPE);
/* Get them to send us their first terminal type */
  printf("%c%c%c%c%c%c", IAC, SB, TELOPT_TTYPE, TELQUAL_SEND, IAC, SE);
} /* player_connected() */

void check_terminal_type(string type) {
  if (last_term == type) {
/* We have cycled.  This is the last valid type.  We failed!  */
    if (!got_terminal)
      this_object()->set_network_terminal_type(0);
    return ;
  }
  if (!got_terminal && this_object()->set_network_terminal_type(type))
    got_terminal = 1;
/* Ask them for thr next terminal type */
  printf("%c%c%c%c%c%c", IAC, SB, TELOPT_TTYPE, TELQUAL_SEND, IAC, SE);
  last_term = type;
} /* check_terminal_type() */
