/**
 * Lockout - lets a player suspend themselves from the MUD.
 * @author Ceres
 */
#include <player_handler.h>
#include <playerinfo.h>

inherit "/cmds/base";

nosave mapping info = ([ ]);


private int cmd(int tim, string type, string reason, int use_alts) {
   string p1;
   string* alts;

   if (this_player() != this_player(1))
    return 0;

   if(type[<1] != 's')
      type += "s";

  switch (type) {
   case "days":
      tim *= 24;
      break;

   case "weeks":
      tim *= 24 * 7;
    break;
   case "hours":
     tim = tim;
     break;
      
   default:
      return notify_fail("Invalid time.\n");
  }

  if (tim > 24 * 31) {
    return notify_fail( "You can't lock yourself out for more than "
                        "one month!\n" );
  }

  if(sizeof(reason) > 80) {
    return notify_fail("Reason cannot be more than 80 characters.\n");
  }

   if (use_alts) {
      p1 = PLAYERINFO_HANDLER->query_alt_of(this_player()->query_name());

      alts = (string*)PLAYERINFO_HANDLER->query_alts(this_player()->query_name()) |
             (string*)PLAYERINFO_HANDLER->query_alts(p1) |
             ({ this_player()->query_name() });
   } else {
      alts = ({ this_player()->query_name() });
   }


  info[this_player()->query_name()] = ({ tim, reason, alts });
  write("This will lockout " + query_multiple_short(alts) + ".\n");
  write("Enter your password: ");
  input_to("get_pw", 1, this_player());
  return 1;
}

int get_pw(string pass, object player) {
   int tim;

  if (!PLAYER_HANDLER->test_password(player->query_name(), pass)) {
    write("\nSorry, incorrect password.\n");
    return 0;
  }

   if(!info[player->query_name()]) {
      write("\nSomething went wrong, please try again.\n");
      return 0;
   }
   
  tim = info[player->query_name()][0];
  if (tim < 24) {
    write("\nYour lockout will last " + tim  + " hour" +
          (tim == 1 ? "" : "s" ) + ".  It cannot be " + 
          "undone.  Are you completely sure you wish to continue?\n");
  } else {
    write("\nYour lockout will last " + tim / 24 + " day" +
          ((tim / 24) == 1 ? "" : "s" ) + ".  It cannot be " + 
          "undone.  Are you completely sure you wish to continue?\n");
  }
  input_to("confirm", 0, this_player());
  return 1;
}

int confirm(string yesno, object player) {
   int tim;
   string reason;
   string* alts;
   string name;

   if (yesno[0] != 'y') {
      write("Ok, cancelling lockout...\n");
      write("Lockout cancelled.\n");
      return 1;
   }

   if(!info[player->query_name()]) {
      write("Something went wrong, please try again.\n");
      return 0;
   }
   tim = info[player->query_name()][0];
   reason = info[player->query_name()][1];
   alts = info[player->query_name()][2];

   foreach (name in alts) {
      if (!"/secure/bastards"->lockout_person(name,
                                           tim*60*60, reason)) {
         write("Sorry, we failed.\n");
         return 0;
      }
   }
   write("Ok, you are now locked out.  Quit when ready.\n");
   return 1;
}

// Answer the command patterns
mixed *query_patterns() {
   return ({ "<number'number of'> "
                     "<word'days|weeks|hours'> <string'reason'>",
                     (: cmd($4[0], $4[1], implode($4[2..], " "), 0 ) :),
             "alts <number'number of'> "
                     "<word'days|weeks|hours'> <string'reason'>",
                     (: cmd($4[0], $4[1], implode($4[2..], " "), 1 ) :) });
}
