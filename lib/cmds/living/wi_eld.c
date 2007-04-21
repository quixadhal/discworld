/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: wi_eld.c,v 1.1 1998/01/06 05:28:43 ceres Exp $
 * $Log: wi_eld.c,v $
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";

mixed cmd(string str) {
  if (!interactive(this_player()))
    return "/cmds/living/ho_ld"->cmd(str);
  return notify_fail("Use hold; see help hold for more details.\n");
}
