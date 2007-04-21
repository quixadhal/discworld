/*   -*- LPC -*-   */
/*
 * $Locker:  $
 * $Id: site.c,v 1.8 2000/09/23 05:50:26 pinkfish Exp $
 */
#include <parser.h>
#include <access.h>

inherit "/cmds/base";

private string print_access(string bit, mapping bing, int depth, int cols, string filter);

/*
 * This will ban a site
 */
mixed cmd(string ip, string ident, string level, string reason) {

   seteuid("Root");
   
   if (this_player() != this_player(1))
      return 0;

   switch(level) {
   case "nonew":
      level = "2";
      break;
   case "noacccess":
      level = "3";
      break;
   case "normal":
      level = "1";
      break;
   }
   
   if (!"/secure/bastards.old"->change_access(explode(ip, "."), ident,
                                                             to_int(level), reason)) {
      write("Error changing permissions.\n");
      return 0;
   }
   
   write("Access permisions changed.\n");
   printf("User %s at site %s set to %s for %s\n",
             ident, ip, PERM_NAMES[to_int(level)], reason);
   return 1;
} /* cmd() */

int access(string filter) {
   mixed bing;
   
   seteuid("Root");
   
   if (this_player() != this_player(1))
      return 0;

   bing = (mixed)"/secure/bastards.old"->query_all_access();

   if (!m_sizeof(bing)) {
      notify_fail("No access control defined.\n");
      return 0;
   }
   write("Current access list:\n");
   bing = print_access("", bing, 0, (int)this_player()->query_cols(), filter);
   this_player()->more_string(bing, "site access");
   return 1;
}

private string print_access(string bit, mapping bing, int depth, int cols,
                            string filter) {
   mixed *bits;
   int i;
   string ret;
   string colour;

   ret = "" ;
   if (this_player() != this_player(1)) {
      return 0;
   }
   bits = m_indices(bing);
   if (depth == 4) {
      if(!filter || filter == "" ||
          strsrch(bit, filter) != -1 || strsrch(bits[i], filter) != -1) {
         /* Do the ident printing... */
         for (i=0;i<sizeof(bits);i++) {
            switch (bing[bits[i]][0]) {
            case NO_NEW:
               colour = "%^CYAN%^";
               break;
            case NO_ACCESS:
               colour = "%^YELLOW%^";
               break;
            default :
               colour = "";
               break;
            }
            write(replace(bit, ".*", "")[1..] + "\n");
            "/secure/bastards"->change_access(replace(bit, ".*", "")[1..],
                                              bing[bits[i]][0],
                 "/secure/bastards.old"->query_reason(explode(bit, "."), bits[i]));
         }
      }
    
      return "";
   }
   for (i=0;i<sizeof(bits);i++) {
      ret += print_access(bit+"."+bits[i], bing[bits[i]], depth+1, cols,
                          filter);
   }
   return ret;
} /* print_access() */

int help() {
   return notify_fail(
      "Syntax: site access\n"+
      "            site ban <ip number> <ident> <level> <reason>\n"+
      "                         <level> := normal [1] normal access,\n"+
      "                                          nonew [2] no new players, \n"+
      "                                          noaccess [3] no access.\n");
}
mixed *query_patterns() {
   return ({ "ban <word'address'> <word'ident'> <word'level'> "
                     "<string'reason'>",
                     (: cmd($4[0], $4[1], $4[2], implode($4[3..], "")) :),
                     "access <word'address'>", (: access($4[0]) :),
                     "access", (: access("") :),
                     "", (: help() :),
                     "help", (: help() :)
                     });
} /* query_patterns() */
