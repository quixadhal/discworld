inherit "/cmds/base";

#include <cmds/twiki.h>

#define DAY (24 * 60 * 60)

int cmd(int days) {
   string str;

   if (days <= 0) {
      add_failed_mess("The number of days must be higher than 0.\n");
      return 0;
   }
   if (days > 10) {
      add_failed_mess("You are limited to a maximum of ten days of "
        "twiki changes.  Consult the automated commonroom twiki post "
        "for further historical information.\n");
      return 0;
   }
   
   str = TWIKI_HANDLER->find_changes(time() - days * DAY);
   write(str);
   return 1;
} /* cmd() */

int cmd_file(string fname) {
   string str;

   str = TWIKI_HANDLER->query_file(fname);
   if (!str) {
      add_failed_mess("The twiki file " + fname + " does not exist.\n");
      return 0;
   }

   write("$P$" + fname + "$P$" + str);
   return 1;
} /* cmd() */

mixed *query_patterns() {
   return ({ "changes [since] <number'days'>", (: cmd($4[0]) :),
             "view <string'file'>", (: cmd_file($4[0]) :) });
} /* query_patterns() */
