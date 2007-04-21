/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: clubs.c,v 1.3 2002/10/26 22:49:30 ceres Exp $
*/

#include <clubs.h>
#include <mail.h>

inherit "/cmds/base";

#define CONTROL_ROOM "/std/room/club_control_room"

// Length of a list entry
#define ELEN 20

nosave string *bad_words = ({ "black", "blood", "cyber", "dark", "penis",
                              "cock", "pussy", "fuck", "shit", "death",
                              "dragon", "fight", "hell", "mage", "pink",
                              "lord", "shadow", "evil", "kill", "slay",
                              "hate", "anti", "bastard" });

private void do_invalid(string extra, int width);
private int do_delete(string extra, string reason, int width);

mixed cmd(string command, string extra, string rest) {

  if (this_player() != this_player(1))
    return 0;

  switch(command) {
  case "invalid":
    do_invalid(extra, this_player()->query_cols());
    break;
  case "delete":
    return do_delete(extra, rest, this_player()->query_cols());
  }
  return 1;
} /* cmd() */

private string club_line(string club, int width, int verbose) {
  string tmp, tmp2, member;
  int clubtype, i;

  clubtype = CLUB_HANDLER->query_club_type(club);
  
  tmp = sprintf("%%^BOLD%%^%%^RED%%^%-9s%%^RESET%%^ %-15s\n", 
                CLUB_ARRAY[clubtype]+":", club);

  if(verbose) {
        tmp+= sprintf(" %%^BOLD%%^Club Name:%%^RESET%%^ %s\n",
                  CLUB_HANDLER->query_club_name(club));
    tmp += sprintf(" %%^BOLD%%^Founder:%%^RESET%%^ %s\n"
                   " %%^BOLD%%^Members:%%^RESET%%^\n",
                   CLUB_HANDLER->query_founder(club));
    i = 0;
    tmp2 = "";
    foreach(member in CLUB_HANDLER->query_members(club)) {
      tmp2 += sprintf("%-15s ", member);
      if((i-1 * ELEN) % (width/ELEN) == 0  && tmp2 != "") {
        tmp += this_player()->fix_string("   " + tmp2 + "\n", width, 10);
        tmp2 = "";
      }
      i++;
    }
  }
  return tmp;
}

private int check_dodgy_name(string club) {
  string s1, s2, lname, word;
  
  lname = lower_case(club);
  lname = replace(lname, ({"'", "", "_", "", " ", ""}) );
  foreach(word in bad_words)
    if ( sscanf( lname, "%s" + word + "%s", s1, s2 ) == 2 )
      return 1;
  return 0;
}

private void do_invalid(string extra, int width) {
  string club, *clubs, tmp, tmp2;
  int ok;
 
  tmp = "";
  tmp2 = "";
  
  clubs = sort_array(CLUB_HANDLER->query_clubs(),
                     (: strcmp($1, $2 ) :) );

  foreach(club in clubs) {
    ok = 1;
    
    switch(CLUB_HANDLER->query_club_type(club)) {
    case CLUB_FAMILY:
      ok = CONTROL_ROOM->is_valid_family_name(club);
      break;
    case CLUB_PERSONAL:
    case CLUB_ELECTED:
      ok = CONTROL_ROOM->is_valid_club_name(club);
      break;
    }

   
    if(!ok)
      tmp += club_line(club, width, extra == "verbose");
    else if(check_dodgy_name(club))
      tmp2 += club_line(club, width, extra == "verbose");

  }

  if(tmp == "")
    write("No invalid clubs.\n");
  else
    this_player()->more_string(tmp, "Clubs", 1);

  if(tmp2 == "")
    write("No dodgy clubs.\n");
  else
    this_player()->more_string("Dodgy names:\n\n" + tmp2, "Clubs", 1);
  
}

private int do_delete(string club, string reason, int width) {
  string founder, members, message, cname;

  if (!"/secure/master"->query_lord(geteuid(previous_object())))
    return notify_fail("You must be a lord to delete a club.");

  if(!club)
    return notify_fail("You must give a club name.\n");

  if(!CLUB_HANDLER->is_club(club))
    return notify_fail(club+" is not a current club.\n");
  
  write("Disbanding: " + club);

  founder = CLUB_HANDLER->query_founder(club);
  members = implode(CLUB_HANDLER->query_members(club), ", ");
  
  cname = CLUB_HANDLER->query_club_name(club);
  
  if(CLUB_HANDLER->disband_club(club)) {
    write(" succeeded.\n");

    message = sprintf("The club %s has been deleted by %s for "
                      "%s\n", cname,
                      this_player()->query_name(),
                      reason);
    MAILER->do_mail_message(founder, this_player()->query_name(),
                            "Your club has been deleted.",
                            members, message);
  } else
    write(" failed.\n");
  
  return 1;
}

mixed *query_patterns() {
  return ({
            "invalid", (: cmd("invalid", "", "") :),
            "invalid <string>", (: cmd("invalid", $4[0], "") :),
            "delete <word> <string>", (: cmd("delete", $4[0], $4[1]) :),
        });
} /* query_patterns() */
