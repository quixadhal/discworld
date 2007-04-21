/*  -*- LPC -*-  */
/*
 * $Id: upt_ime.c,v 1.14 2003/03/02 23:50:32 pinkfish Exp $
 */
#include <player.h>
inherit "/cmds/base";

mixed cmd(int brief) {
  mapping u;
  int r;
  string str;
  string output;
  
  str = "/secure/finger"->time_elapsed_string(uptime());
#ifndef __DISTRIBUTION_LIB__
  output = "A'Tuin has been paddling through space for "+

      str + ".";
  if (brief) {
     write(output + "\n");
     return 1;
  }
  output += "  He (or as it may be, she) has been "
      "performing "+ query_load_average()+".";
#else
  output += mud_name() + " has been up for "+ str +
      ".  The load average is " +
      query_load_average()+".";
  if (brief) {
     write(output + "\n");
     return 1;
  }
#endif

  u = rusage();

#ifndef __DISTRIBUTION_LIB__
  output += sprintf("  This has taken %.2f%% of his (or her) energy and %.2fMB "
                    "of his (or her) mental capacity.  ",
                    ((u["stime"] + u["utime"]) / 10.0) / uptime(),
                    (memory_info() / 1024000.0));
#else
  output += sprintf("  This has taken %.2f%% of the cpu and %.2fMB of "
                    "of memory.  ",
                    ((u["stime"] + u["utime"]) / 10.0) / uptime(),
                    (memory_info() / 1024000.0));
#endif

#ifndef __DISTRIBUTION_LIB__
  r = "/obj/handlers/garbage"->query_next_reboot();
  if (r == -1) {
    output += "It is unknown when he (or she) will be "
      "taking his (or her) next rest.";
  } else if (r < 0 || find_object(OFFLER)) {
    output += "He (or she) will be taking his (or her) next rest "
      "any moment now.";
  } else {
    str = "";
  
    if(r / (60*60*24)) {
      str += (string) (r / (60*60*24)) + " day";
      str += ((r /(60*60*24)) > 1 ? "s" : "");
    }

    if(r / ((60*60)) % 24) {
      if(str != "") {
        str += " and ";
      }
      str += (string)((r/(60*60))%24);
      str += " hour" + (((r/(60*60))%24) > 1 ? "s" : "");
    } else if(str == "") {
      switch(r/ 60) {
      case 0..7:
        str = "less than 15 minutes";
        break;
      case 8..22:
        str = "about a quarter of an hour";
        break;
      case 23..38:
        str = "about half an hour";
        break;
      case 39..53:
        str = "about three quarters of an hour";
        break;
      default:
        str = "just under an hour";
        break;
      }
    }

    output += "He (or she) will be taking his (or her) next rest "
      "in " + str + ".";
  }
#endif
  write(sprintf("%-=*s\n",
                  (int)this_player()->query_cols(), output));
  return 1;
}

mixed* query_patterns() {
   return ({ "", (: cmd(0) :),
             "{brief|verbose}", (: cmd($4[0] == "brief") :) });
}
