/*   -*- LPC -*-   */
/*
 * $Locker:  $
 * $Id: site.c,v 1.9 2001/06/01 20:58:28 ceres Exp $
 */
#include <parser.h>
#include <access.h>

inherit "/cmds/base";

/*
 * This will ban a site
 */
mixed cmd(string address, string level, string reason) {

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
   case "authnew":
     level = "4";
     break;
   case "normal":
      level = "1";
      break;
   }

   // the .* is nolonger needed in addresses (or names for that matter).
   address = replace(address, ".*", "");
   
   if (!"/secure/bastards"->change_access(address, to_int(level), reason)) {
     write("Error changing permissions.\n");
     return 0;
   }
   
   write("Access permisions changed.\n");
   printf("Site %s set to %s for %s\n", address, PERM_NAMES[to_int(level)],
          reason);
   return 1;
} /* cmd() */

int access(string filter) {
  string addr, ret;
  mapping list;
  int found;
  
  seteuid("Root");
  
  if (this_player() != this_player(1))
    return 0;
  
  list = "/secure/bastards"->query_all_access();
  
  ret = "";
  
  foreach(addr in keys(list)) {
    if(!filter || filter == "" || strsrch(addr, filter) != -1) {
      found = 1;
      ret += sprintf("%-20s %-18s %-=39s\n", addr,
                     PERM_NAMES[list[addr][ACCESS_LEVEL]],
                     list[addr][ACCESS_REASON]);
    }
  }
  if (!found)
    return notify_fail("No access control defined.\n");

  write("Current access list:\n");
  this_player()->more_string(ret, "site access");
  return 1;
}

int multi(int allow, string address) {
  if("/secure/bastards"->change_multi(address, allow, 0)) {
    if(allow)
      write("Site " + address + " has been permitted for multiplayers.\n");
    else
      write("Site " + address + " has been blocked for multiplayers.\n");
  } else
    write("Error multiplayer settings for site " + address + ".\n");
  
  return 1;
}

int list_multi(string filter) {
  string addr, ret;
  mapping list;
  int found;

  list = "/secure/bastards"->query_all_multi();

  ret = "";
  foreach(addr in keys(list)) {
    if(!filter || filter == "" || strsrch(addr, filter) != -1) {
      found = 1;
      ret += sprintf("%-30s\n", addr);
    }
  }
  if (!found)
    return notify_fail("No access control defined.\n");
  
  write("Current multuser site list:\n");
  this_player()->more_string(ret, "site access");
  return 1;
}

int help() {
   return notify_fail(
      "Syntax: site access\n"+
      "            site ban <ip number|site name> <level> <reason>\n"+
      "                     <level> := normal normal access,\n"+
      "                                nonew no new players, \n"+
      "                                authnew authorised new players\n"+
      "                                noaccess no access.\n");
}
mixed *query_patterns() {
   return ({ "ban <word'address'> <word'level'> <string'reason'>",
               (: cmd($4[0], $4[1], implode($4[2..], "")) :),
               "allow multi <word'address'>",
               (: multi(1, $4[0]) :),
               "disallow multi <word'address'>",
               (: multi(0, $4[0]) :),
               "list multi", (: list_multi("") :),
               "list multi <word'address'>", (: list_multi($4[0]) :),
               "access <word'address'>", (: access($4[0]) :),
               "access", (: access("") :),
               "", (: help() :),
               "help", (: help() :)
               });
} /* query_patterns() */
