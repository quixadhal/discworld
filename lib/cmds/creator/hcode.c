/*   -*- LPC -*-   */
/*
 * $Locker:  $
 * $Id: hcode.c,v 1.1 2001/01/18 01:49:16 pinkfish Exp $
 * $Log: hcode.c,v $
 * Revision 1.1  2001/01/18 01:49:16  pinkfish
 * Initial revision
 *
 * Revision 1.2  2001/01/18 01:35:17  pinkfish
 * Fix up the code.
 *
 * Revision 1.1   1998/01/06 05:25:41   ceres
 * Initial revision
 * 
*/
/* hcre command, trial out by Turrican for a commands daemon. */
inherit "/cmds/base";

#include <broadcaster.h>

mixed cmd(string) {
   int i;
   mixed *hist;

   if (!wizardp(this_player())) {
      return 0;
   }
   hist = HIST_HANDLER->query_chat_history("code");
   if (!pointerp(hist) || !sizeof(hist)) {
      return notify_fail("Nobody said anything on the code channel.\n");
   }
   write("The code channel history is:\n");
   for (i=0;i<sizeof(hist);i++) {
      if (sizeof(hist[i]) > 2) {
         efun::tell_object(this_player(), 
                (string)this_player()->fix_string(sprintf("*%s* %s%-=*s\n",
                ctime(hist[i][2])[11..18], hist[i][0], 
                (int)this_player()->query_cols()-strlen(hist[i][0])-11,
                hist[i][1])));
      } else {
         efun::tell_object(this_object(), 
                (string)this_player()->fix_string(sprintf("%s%-=*s\n",
                hist[i][0], (int)this_player()->query_cols()-strlen(hist[i][0]),
                hist[i][1])));
      }
   }
   return 1;
}
