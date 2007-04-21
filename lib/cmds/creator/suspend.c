/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: suspend.c,v 1.10 2002/03/11 01:45:26 pinkfish Exp $
 * 
 */
#include <parser.h>
#include <access.h>

inherit "/cmds/base";

// #define SUSPEND_MULTI 1
#define MULTI_SUSPEND 28*24 // Two weeks.
#define LIAISON "/d/liaison/master"

private int do_suspend(string, int, string);

// The lord version of suspend.
private int cmd(string name, int tim, string type, string reason) {

  if (this_player() != this_player(1)) {
    return 0;
  }

  if (!master()->query_lord(geteuid(previous_object())) &&
      !LIAISON->query_deputy(geteuid(previous_object()))) {
    return notify_fail("You must be a lord or liaison deputy to suspend "
                       "someone.");
  }

  if ( type[<1] != 's' ) { 
      type += "s";
  }

  switch (type) {
    case "days":
        tim *= 24;
    break;

    case "weeks":
        tim *= 24 * 7;
    break;

    case "months":
        tim  *= 24 * 7 * 4;
    break;

    case "years":
        tim *= 24 * 365; 
    break; 
    
    case "hours":
        // Do nothing.
    break; 

    default:
//         add_failed_mess( "You must give a reason.\n" ); 
//        return -1; 
  }
  
  if (!master()->query_lord (geteuid(previous_object())) 
      && LIAISON->query_deputy(geteuid(previous_object()))
      && tim > MULTI_SUSPEND) {
    return notify_fail ("You can only suspend for a maximum of " +
      query_num (MULTI_SUSPEND) + " hours.");
  }

  return do_suspend(name, tim, reason);
}

// The normal creator suspend
private int suspend(string who, string reason) {
  if(this_player() != this_player(1))
    return 0;

  if (!master()->query_lord(geteuid(previous_object())) &&
      !master()->query_senior(geteuid(previous_object())) &&
      !LIAISON->query_member(geteuid(previous_object()))) {
    return notify_fail("Only liaisons or seniors can do that.\n");
  }

  return do_suspend(who, 2, reason);
}

#ifdef SUSPEND_MULTI
// The liaison multiplayer suspend
private int suspend_multi(string who) {
  if(this_player() != this_player(1))
    return 0;

  if (!"/secure/master"->query_lord(geteuid(previous_object())) &&
      !LIAISON->query_deputy(geteuid(previous_object()))) {
    return notify_fail("You must be a lord or liaison deputy to suspend "
                       "someone for multiplaying.");
  }

  return do_suspend(who, MULTI_SUSPEND, "Multiplaying");
}
#endif

private int do_suspend(string name, int tim, string reason) {
  notify_fail("We failed!\n");

  if (!"/secure/bastards"->suspend_person(name, tim*60*60, reason)) {
    return 0;
  }

  write(name+" suspended until "+ctime(time()+tim*60*60)+"\n");
  return 1;
}

int help() {
  if(!"/secure/master"->query_lord(geteuid(previous_object())) &&
     !LIAISON->query_member(this_player()->query_name()))
    return notify_fail("You cannot use suspend.\n");

  
  if(this_player()->query_lord() ||
     master()->query_senior(geteuid(previous_object())) ||
     LIAISON->query_member(geteuid(previous_object())))
    write("suspend <name> <reason> -- will suspend a player for 2 hours.\n");

#ifdef SUSPEND_MULTI
  if(this_player()->query_lord() ||
     LIAISON->query_deputy(geteuid(this_player())))
    write("suspend <name> multiplaying -- will suspend a multiplayer for "
          "2 weeks.\n");
#endif

  if (LIAISON->query_deputy(geteuid(this_player()))) {
    write("suspend <name> <time> <reason> where time is in hours.\n");
    write ("You may suspend people for a maximum of " 
      + query_num (MULTI_SUSPEND) + " hours as a liaison deputy.\n");
  }

  if ("/secure/master"->query_lord(geteuid(this_player()))) {
    write("suspend <name> <time> <reason> where time is in hours.\n");
    write("suspend <name> <time> <hours|days|weeks|months|years> <reason> "
        "where time is in the units specified in the command.\n");
  }

  return 1;
}

public mixed *query_patterns() {
  mixed retval;
  
  retval = ({ "help", (: help() :) });

  if( master()->query_lord(geteuid(this_player())) ||
     master()->query_senior(geteuid(previous_object())) ||     
     LIAISON->query_member(geteuid(previous_object())))
    retval += ({ "<word'player'> <string'reason'>",
                   (: suspend($4[0], implode($4[1..], " ")) :) });
  
#ifdef SUSPEND_MULTI
  if(this_player()->query_lord() ||
     LIAISON->query_deputy(geteuid(this_player())))
    retval += ({ "<word'player'> multiplaying",
                   (: suspend_multi($4[0]) :) });
#endif
  
  if( master()->query_lord(geteuid(this_player())) ||
        LIAISON->query_deputy(geteuid(this_player()))) { 
        retval += ({ "<word'player'> <number'time'> <string'reason'>",
                   (: cmd($4[0], $4[1], "hours", implode($4[2..], " ") ) :) });
        retval += ({ "<word'player'> <number'time'> "
            "<word'hours|days|weeks|months|years'> <string'reason'>",
                   (: cmd($4[0], $4[1], $4[2], implode($4[3..], " ") ) :) });
  }

  return retval;
} /* query_patterns() */

