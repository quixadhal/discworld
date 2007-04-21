/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: surr_ender.c,v 1.3 1998/02/23 23:50:08 sin Exp $
 * $Log: surr_ender.c,v $
 * Revision 1.3  1998/02/23 23:50:08  sin
 * Ok, everything is working with the surrender command now.
 *
 * Revision 1.2  1998/02/23 23:18:17  sin
 * *blush* I made a boo-boo
 *
 * Revision 1.1  1998/02/23 23:17:06  sin
 * Initial revision
 *
 * 
*/
inherit "/cmds/base";

mixed cmd(int mode, int val) {
  if (!mode) {
    int surr = this_player()->query_surrender();

    if (surr)
      write(sprintf("You will surrender at %d%% of your max hps.\n", surr));
    else
      write("You will not surrender.\n");
    return 1;
  } else {
    if (val < 0 || val > 100)
      return notify_fail("Your surrender must be between 0 and 100.\n");
    this_player()->set_surrender(val);
    write(sprintf("Surrender set to %d%%\n", this_player()->query_surrender()));
    return 1;
  }
} /* cmd() */

mixed *query_patterns()
{
  return ({ "", (: cmd(0, 0) :),
            "<number>", (: cmd(1, $4[0]) :) });
}
