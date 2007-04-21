/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: dinfo.c,v 1.4 2000/08/19 21:38:27 ceres Exp $
 * $Log: dinfo.c,v $
 * Revision 1.4  2000/08/19 21:38:27  ceres
 * Fixed to use player_handler instead of finger
 *
 * Revision 1.3  2000/06/22 23:33:15  ceres
 * Modified to use query_domains()
 *
 * Revision 1.2  1999/10/12 03:25:46  jeremy
 * Added sort parameters.  Also added "Now" to show those currently
 * logged on.
 *
 * Revision 1.1  1998/01/06 05:29:21  ceres
 * Initial revision
 * 
*/

#include <player_handler.h>

string master;

string dinfo( string name, int cols, string option );
string get_last_log( int last_log_on );

mixed cmd(string name, string option) {
  if (!name) {
    return notify_fail("You must give a domain.\n");
  }
  this_player()->more_string(dinfo(name, (int)this_player()->query_cols(),
                                   option), "Domain Info");
  return 1;
}

string dinfo( string name, int cols, string option ) {
  string ret, *members;
  int i;

  if(member_array(name, "/secure/master"->query_domains()) == -1)
    return "No such domain: " + name + "\n";
  
  master = "/d/"+name+"/master";
  write(master+"\n");
  members = master->query_members();
  switch (option[0..0]) {
  case "n":
    members = sort_array(members, 1);
    break;
  case "p":
    members = sort_array(members,
                         (: strcmp(master->query_project($1),
                                   master->query_project($2)) :));
    break;
  case "l":
    members = sort_array(members,
                         (:PLAYER_HANDLER->test_last($2) -
                          PLAYER_HANDLER->test_last($1):));
    break;
  }
  ret = "The current members of this domain are:";
  
  for (i=0;i<sizeof(members);i++) {
    ret += sprintf("\n%-12s: Last login: %s\n",
                   capitalize(members[i]),
                   find_player(members[i])?
                     "%^GREEN%^%^BOLD%^Now%^RESET%^"
                     :get_last_log((int)PLAYER_HANDLER->test_last(members[i])),
                   );
    ret += sprintf("              Project   : %-=*s\n", cols-27,
                   master->query_project(members[i]));
  }
  return ret;
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

string get_last_log(int last_log_on) {
    string retval;
    int tmp_time, sec, min, hour, day;

/* Should be a nice number.... */
    tmp_time = time()-last_log_on;
    if (!tmp_time) {
      sec = min = hour = day = 0;
    }
    if(tmp_time > 24*60*60) {
      retval = (day = tmp_time/(24*60*60))+" day"+
                (day<2?"":"s")+" ago.";
      if(day > 14)
        retval = "%^RED%^"+retval+"%^RESET%^";
      else if(day > 7)
        retval = "%^YELLOW%^"+retval+"%^RESET%^";
      else
        retval = "%^GREEN%^"+retval+"%^RESET%^";
    } else
      retval = "%^GREEN%^Today%^RESET%^";
    
    return retval;
}

mixed query_patterns() {
  return ({
    "<string'domain'>", (: cmd($4[0], " ") :),
    "<string'domain'> [by] {n|name|p|project|l|login}",
      (: cmd($4[0], $4[1]) :)
  });
}
