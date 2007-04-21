/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: hl_ord.c,v 1.1 1998/01/06 05:11:09 ceres Exp $
 * $Log: hl_ord.c,v $
 * Revision 1.1  1998/01/06 05:11:09  ceres
 * Initial revision
 * 
*/
/* hlord command, trial out by Turrican for a commands daemon. */

#define HIST "/obj/handlers/hist_handler"

mixed cmd() {
  int i;
  mixed *hist;

  if (!master()->query_lord((string)geteuid(this_player()))) 
    return 0;
  seteuid(geteuid(this_player()));
  hist = HIST->query_lord_history();
  if (!pointerp(hist) || !sizeof(hist))
    return notify_fail("Nobody said anything on the lord channel.\n");
  write("The lord channel history is:\n");
  for (i=0;i<sizeof(hist);i++) {
    if (sizeof(hist[i]) > 2) {
      efun::tell_object(this_player(), 
           this_player()->fix_string(sprintf("*%s* %s%-=*s\n",
           ctime(hist[i][2])[11..18], hist[i][0],
           (int)this_player()->query_cols()-strlen(hist[i][0])-11,
           hist[i][1])));
    }
    else
      efun::tell_object(this_player(), 
           this_player()->fix_string(sprintf("%s%-=*s\n",
           hist[i][0], (int)this_player()->query_cols()-strlen(hist[i][0]),
           hist[i][1])));
    }
  return 1;
}

void dest_me() {
  destruct(this_object());
}

void clean_up() {
  dest_me();
}

void reset() {
  dest_me();
}
