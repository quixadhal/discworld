/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: brief.c,v 1.4 2000/09/27 01:36:02 presto Exp $
 * $Log: brief.c,v $
 * Revision 1.4  2000/09/27 01:36:02  presto
 * Fixed printing "No such option" when it shouldn't
 *
 * Revision 1.3  2000/09/02 22:28:22  ceres
 * Added error mess
 *
 * Revision 1.2  1999/02/09 01:36:59  ceres
 * Forgot to remove some debug info
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
    foreach(t in TP->query_verbose_types())
      disp += t + (TP->query_verbose(t) ? " (verbose) " : " (brief) ");
    write(disp + "\n");
  }

  else if(type == "all") {
    foreach(t in TP->query_verbose_types())
      TP->set_verbose(t, which == "verbose");
    write("Ok\n");
  } else if(member_array(type, TP->query_verbose_types()) != -1) {
    TP->set_verbose(type, which == "verbose");
    write("Ok\n");
  } else {
    write("No such option.\n");
  }
  return 1;
  
}


mixed *query_patterns() {
  return ({ "<word'type'>", (: cmd("brief", $4[0]) :),
            "", (: cmd("brief", 0) :),
         });
} /* query_patterns() */
