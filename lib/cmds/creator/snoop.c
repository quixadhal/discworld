/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: snoop.c,v 1.3 2000/04/24 01:57:43 presto Exp $
 * $Log: snoop.c,v $
 * Revision 1.3  2000/04/24 01:57:43  presto
 * Display an approprite message if you try to cancel a snoop and you
 * aren't snooping anyone.
 *
 * Revision 1.2  1998/05/01 15:03:17  pinkfish
 * Changed to use add command.
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
 */
/* snoop command, trial out by Turrican for a commands daemon. */
inherit "/cmds/base";

int do_snoop_cancel() {
  seteuid(geteuid(this_player()));
  if (!query_snooping(this_player()))
    write("You aren't snooping anyone.\n");
  else  {
    snoop(this_player(), 0);
    write("Ok, snoop cancelled.\n");
  }
  return 1;
} /* do_snoop_cancel() */

int do_snoop(object *obs) {
  object targ;

  targ = obs[0];
  if(!snoop(this_player(), targ)) {
    notify_fail("You fail to snoop " + targ->the_short() + ".\n");
    return 0;
  }
  write("Ok, snooping "+targ->the_short()+".\n");
  return 1;
} /* do_snoop() */

mixed *query_patterns() {
  return ({ "<indirect:player>", (: do_snoop($1) :),
            "", (: do_snoop_cancel() :) });
} /* query_patterns() */
