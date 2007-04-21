/**
 * Addalt - adds a record about a player's alt character.
 * @author Fiona
 */
#include <player_handler.h>
#include <playerinfo.h>

inherit "/cmds/base";

int cmd(string alt) {
  string p1;
  string p2;

  alt = lower_case(alt);
  if (!PLAYER_HANDLER->test_user(alt))
    return notify_fail(alt + " is not a player.\n");

   p1 = PLAYERINFO_HANDLER->query_alt_of(this_player()->query_name());
   if(!p1 || p1 == "") {
     p1 = this_player()->query_name();
   }
   
   p2 = PLAYERINFO_HANDLER->query_alt_of(alt);
   if(!p2 || p2 == "") {
     p2 = alt;
   }
   
   if(p1 == p2) {
     add_failed_mess("You and " + alt + " are already registered.\n");
     return 0;
   }

   if(sizeof(PLAYERINFO_HANDLER->query_alts(p2)) &&
      sizeof(PLAYERINFO_HANDLER->query_alts(p1))) {
     add_failed_mess("Sorry, there is a clash of alts here, you'll need to "
                     "ask a creator for assistance.\n");
     return 0;
   }
  write("Enter your password (this character): ");
  input_to("get_pw", 1, this_player(), alt);
  return 1;
}

void get_pw(string pass, object player, string alt) {
  if (!PLAYER_HANDLER->test_password(player->query_name(), pass)) {
    write("Sorry, incorrect password.\n");
    return;
  }
  write("\nEnter the password for " + alt + ": ");
  input_to("get_alt_pw", 1, player, alt);
}

void get_alt_pw(string pass, object player, string alt) {
  string result;
  string p1, p2;
  write("\n");

  if(this_player() != player) {
    write("Something has gone wrong, player objects don't match.\n");
    return;
  }

  if (!PLAYER_HANDLER->test_password(alt, pass)) {
    write("Sorry, incorrect password.\n");
    return;
  }

  //Do we need a p1 variable? -- Terano.
  p1 = PLAYERINFO_HANDLER->query_alt_of(player->query_name());
  if(!p1 || p1 == "") {
    p1 = player->query_name();
  }

   p2 = PLAYERINFO_HANDLER->query_alt_of(alt);
   if(!p2 || p2 == "") {
     p2 = alt;
   }

   if(sizeof(PLAYERINFO_HANDLER->query_alts(p2))) {
     write("Registering " + p1 + " as an alt of " + p2 + ".\n");
     result = PLAYERINFO_HANDLER->add_alt(0, p2, p1);
   } else {
     write("Registering " + p2 + " as an alt of " + p1 + ".\n");
     result = PLAYERINFO_HANDLER->add_alt(0, p1, p2);
   }

   write(result);
}

int cmd_list() {
   string* alts;
   string p1;

   p1 = PLAYERINFO_HANDLER->query_alt_of(this_player()->query_name());
   if (!p1) {
      p1 = this_player()->query_name();
   }
   alts = (string*)PLAYERINFO_HANDLER->query_alts(p1) |
          (string*)PLAYERINFO_HANDLER->query_alt_of(this_player()->query_name());
   if (!sizeof(alts)) {
      write("No alts registered.\n");
      return 1;
   }
   write("Your registered alts are " + query_multiple_short(alts) + ".\n");
   return 1;
}

// Answer the command patterns
mixed *query_patterns() {
   return ({ "", (: cmd_list :),
             "list", (: cmd_list :),
             "<word'alt-name'>", (: cmd($4[0]) :) });
}
