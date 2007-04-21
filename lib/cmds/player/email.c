/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: email.c,v 1.1 1998/01/06 05:29:43 ceres Exp $
 * $Log: email.c,v $
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
#include <player.h>
inherit "/cmds/base";

#define TP this_player()

int cmd(string str) {
   string email;

  email = TP->query_email();
   
   if (!str) {
      write("Your current email address is "+email+"\n");
      write("To clear use \"email CLEAR\"\n");
      write("For the email address to be only visible by lords prepend a :\n");
      write("eg email :frog@frogcentral.froguni.swamp\n");
      return 1;
   }
   if (str == "CLEAR")
      email = "";
   else
      email = str;
   if (email != "")
      write("Email address set to "+str+".\n");
   else
      write("Email address is null.\n");
   TP->set_email(email);
   return 1;
}
