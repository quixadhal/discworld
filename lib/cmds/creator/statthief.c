/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: statthief.c,v 1.3 2003/03/21 02:03:30 ceres Exp $
 * $Log: statthief.c,v $
 * Revision 1.3  2003/03/21 02:03:30  ceres
 * Modified to use player handler
 *
 * Revision 1.2  2000/04/03 18:10:48  dragonkin
 * Added a check to make sure that the person actually had a quota, as it otherwise created one then...
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
#include <player_handler.h>

inherit "/cmds/base";

#define QUOTA "/obj/handlers/guild_things/quota"
#define MONEY "/obj/handlers/money_handler"

string cost_str(int amt);
string time_str(int time);

int cmd(string str) {
  if(!str || !PLAYER_HANDLER->test_user(str))
    return notify_fail("Get thief stats for who?\n");

  if( !QUOTA->query_quota_data( str ) )
    return notify_fail("Sorry, "+str+" does not have a quota.\n");
  
  printf("Thief statistics for: %s\n", str);
  printf("  Quota: \n");
  printf("    Min %s\n", cost_str((int)QUOTA->query_thief_minquota(str)));
  printf("    Max %s\n", cost_str((int)QUOTA->query_thief_maxquota(str)));
  
  printf("  Debts:\n");
  printf("    Debts %s\n", cost_str(QUOTA->query_thief_debts(str)));
  printf("    Fines %s\n", cost_str(QUOTA->query_thief_fines(str)));
  
  printf("  This period:\n");
  printf("    Stolen %s\n", cost_str(QUOTA->query_total_stolen(str)));
  printf("    Time left %s\n", time_str(QUOTA->query_time_to_reset(str)));
  return 1;
}

string cost_str(int amt) {
  return MONEY->money_string(MONEY->create_money_array(amt, "Ankh-Morpork"));
}

string time_str(int time) {
  string str;

  str = "";
  if (time > 86400)
    str += sprintf("%d days, ", time/86400);
  if (time > 3600)
    str += sprintf("%d hours, ", (time/3600)%24);
  str += sprintf("%d minutes and %d seconds", 
                     (time/60)%60, time%60);
  return str;
}  
