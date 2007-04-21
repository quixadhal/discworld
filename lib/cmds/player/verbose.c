/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: verbose.c,v 1.4 2000/09/02 22:29:19 ceres Exp $
 * $Log: verbose.c,v $
 * Revision 1.4  2000/09/02 22:29:19  ceres
 * Added error message
 *
 * Revision 1.3  2000/04/27 01:20:26  pinkfish
 * Add some brackets in.
 *
 * Revision 1.2  1999/02/13 20:57:00  ceres
 * Newbie error = instead of ==
 *
 * Revision 1.1  1999/02/09 01:30:25  ceres
 * Initial revision
 *
 * Revision 1.4  1999/02/09 00:23:41  ceres
 * Increased the max email address length to 50.
 *
 * Revision 1.2  1999/02/03 00:52:50  pinkfish
 * Fix it up so you can only enter certain sized bits of data into the fields.
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";

#define TP this_player()

int cmd(string which, string type) {
  string t, disp;

  if(!type) {
    disp = "Your settings are: ";
    foreach(t in TP->query_verbose_types()) {
      disp += t + (TP->query_verbose(t) ? " (verbose) " : " (brief) ");
    }
    write(disp + "\n");
    return 1;
  }

  if(type == "all") {
    foreach(t in TP->query_verbose_types()) {
      TP->set_verbose(t, which == "verbose");
    }
    write("Ok\n");
    return 1;
  } else if(member_array(type, TP->query_verbose_types()) != -1) {
    TP->set_verbose(type, which == "verbose");
    write("Ok\n");
    return 1;
  } else {
    write("No such option.\n");
    return 1;
  }
}


mixed *query_patterns() {
  return ({ "<word'type'>", (: cmd("verbose", $4[0]) :),
            "", (: cmd("verbose", 0) :),
         });
} /* query_patterns() */
