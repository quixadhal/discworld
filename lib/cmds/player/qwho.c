/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: qwho.c,v 1.25 2003/01/16 23:23:09 ceres Exp $
 * 
 */
inherit "/cmds/base";

#include <login_handler.h>

int playercompare(object p1, object p2);

int cmd(string guild_name) {
   int i;
   int countcre;
   int countplay;
   int counttest;
   int countloginq;
   int countlogins;
   int compressed;
   string obtyp;
   string tmp;
   object *arr;
   string nam;
   string cres;
   string plays;
   string testers;
   string *login_q;
   string logins;
   int showmasters;
   mapping user_data;
   object *liaisons;
   string friends;
   int countFriends;
   int totalUsers;
   int totalPlayers;
   int totalCompressed;

   if (this_player()->query_creator()) {
      showmasters = 1;
   } else {
      showmasters = 0;
   }
   countcre = 0;
   countplay = 0;
   counttest = 0;
   compressed = 0; 
   cres = "";
   plays = "";
   testers = "";
   logins = "";
   friends = "";
   user_data = unique_mapping( users(), 
       (: "/d/liaison/master"->query_member( $1->query_name() ) :) );

   if ( user_data[1] ) {
       liaisons = sort_array( user_data[1], (: playercompare :) );
   }
   else {
       liaisons = ({ });
    }
   
   if ( user_data[0] ) { 
       arr = sort_array( user_data[0], (: playercompare :));
   }
   else {
       arr = ({ });
   }

   arr = liaisons + arr; 
   arr = filter(arr, (: (!$1->query_invis() || reference_allowed(this_player(),
     $1)) && !$1->query_login_ob() :) );
 
   if (guild_name) {
      // Do weird filtering!
      arr = "/cmds/player/who"->filter_users(arr, guild_name, 1);
      if (!sizeof(arr)) {
         add_failed_mess("Unable to find any members of " + 
                         guild_name + ".\n");
         return 0;
      }
   }
   
   for(i=0;i<sizeof(arr);i++) {
      nam = arr[i]->query_cap_name();
      if (!nam) {
         if (showmasters) {
            nam = "/secure/master";
         } else {
            continue;
         }
      }
      switch (arr[i]->query_invis()) {
      case 0:
         break;
      case 1:
         nam = "[" + nam + "]";
         break;
      case 2:
         nam = "[{" + nam + "}]";
         break;
      case 3:
         nam = "[<" + nam + ">]";
         break;
      default:
         nam = "?" + nam + "?";
         break;
      }
      obtyp = arr[i]->query_object_type();
      switch(obtyp) {
      case "X":
         break;
      case " ":
         break;
      case "C":
         nam += "(%^CYAN%^C%^RESET%^)";
         if ( "/d/liaison/master"->query_member((string)arr[i]->query_name())) {
            nam += "(%^YELLOW%^l%^RESET%^)";
         }
         break;
      case "S":
         nam += "(%^BOLD%^%^CYAN%^S%^RESET%^)";
         if ( "/d/liaison/master"->query_member((string)arr[i]->query_name())) {
            nam += "(%^YELLOW%^l%^RESET%^)";
         }
         break;
      case "T":
         nam += "(%^BOLD%^%^RED%^T%^RESET%^)";
         if ( "/d/liaison/master"->query_member((string)arr[i]->query_name())) {
            nam += "(%^YELLOW%^l%^RESET%^)";
         }
         break;
      case "D":
         nam += "(%^RED%^D%^RESET%^)";
         if ( "/d/liaison/master"->query_member((string)arr[i]->query_name())) {
            nam += "(%^YELLOW%^l%^RESET%^)";
         }
         break;
      default:
         break;
      }
      if (this_player()->is_friend(arr[i]->query_name())) {
         nam += "(%^GREEN%^F%^RESET%^)";
      }

      if (compressedp(arr[i])) {
          compressed++;
      }
      if (arr[i]->query_creator()) {
         countcre++;
         cres = cres + " " + nam;
      } else if (obtyp == "p") {
         counttest++;
         testers = testers + " " + nam;
      } else if(arr[i]->query_login_ob()) {
         countlogins++;
         logins = logins + " " + nam;
      } else if (this_player()->is_friend(arr[i]->query_name())) {
         countFriends++;
         friends = friends + " " + nam;
      } else {
         countplay++;
         plays = plays + " " + nam;
      }
   }

   if (guild_name) {
      // Do weird filtering!
      arr = LOGIN_HANDLER->query_login_queue();
      arr = "/cmds/player/who"->filter_users(arr, guild_name);
      login_q = map(arr, (: $1->query_cap_name() :));
   } else {
      login_q = map(LOGIN_HANDLER->query_login_queue(),
                 (: $1->query_cap_name() :));
   }
   login_q = filter(login_q, (: $1 :));
   login_q = sort_array(login_q, 1);
   
   countloginq = sizeof(login_q);
   
   tmp = sprintf("%%^BOLD%%^%d Creator%s:%%^RESET%%^%s\n",
                 countcre, countcre != 1 ? "s" : "", cres);
   write(tmp);
   if (guild_name != "creator" && guild_name != "creators") {
      switch (counttest) {
        case 0:
            break;
        
        case 1:
           write("%^BOLD%^" + counttest + " Playtester:%^RESET%^" + testers + "\n");
        break;
        
        default:
           write("%^BOLD%^" + counttest + " Playtesters:%^RESET%^" + testers + "\n");        
        break;
     }

      if (countFriends) {
         write("%^GREEN%^" + countFriends + " Friend" + 
             (countFriends > 1 ? "s" : "") + ":%^RESET%^" + friends + "\n");
      }
     
      write("%^BOLD%^" + countplay + " Players:%^RESET%^" + plays + "\n");
      if (countlogins < countloginq) {
         countlogins = countloginq;
      }
      if (countlogins-countloginq) {
         logins = replace(logins, ({ "[", "", "]", ""}));
         if(countloginq) {
            foreach(tmp in login_q) {
               logins = replace(logins, tmp+" ", "");
            }
         }
         write("%^BOLD%^" + (countlogins - countloginq) +
               " Logging in:%^RESET%^" + logins + "\n");
      }
      
      if(countloginq) {
         write("%^BOLD%^" + countloginq + " Queued:%^RESET%^ " +
               implode(login_q, " ") + "\n");
      }
   } else {
      countlogins = 0;
      counttest = 0;
      countplay = 0;
   }

   totalUsers = countcre + counttest + countplay + countlogins + countFriends;
   totalPlayers = counttest + countplay + countFriends; 
   totalCompressed = compressed - countFriends; 

   write("%^BOLD%^" + (totalPlayers) + " Players, " +
         totalUsers + " Total%^RESET%^\n" ); 
   
/*    +
         " Total (" + totalCompressed + " with MCCP, " + 
            (totalUsers - totalCompressed) + 
         " without)%^RESET%^\n");
*/    
   return 1;
} /* cmd() */

int playercompare(object p1, object p2) {
   return strcmp(p1->query_name(), p2->query_name());
} /* playercompare() */

mixed *query_patterns() {
   return ({ "", (: cmd(0) :),
             "<string'guild name'>", (: cmd($4[0]) :) });
} /* query_patterns() */
