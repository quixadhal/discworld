/**
 * This command will allow players to control their friends.
 * @author Pinkfish
 * @started Mon Mar 12 22:41:23 PST 2001
 */

inherit "/cmds/base";
#include <player.h>
#include <player_handler.h>

int cmd_display(string person, string pattern) {
   string* friends;
   string ret;

   if (person == "online") {
      friends = filter(this_player()->query_friends(), (: find_player($1) :));
   } else if (person != "") {
      person = this_player()->expand_nickname(person);
      if (!this_player()->is_friend(person)) {
         add_failed_mess(person + " is not your friend.\n");
         return 0;
      }
      friends = ({ person });
   } else {
      friends = this_player()->query_friends();
   }

   if (pattern) {
      if (regexp(pattern,
           "[\\[\\]\\(\\)\\*\\?\\+][\\[\\]\\(\\)\\*\\?\\+]+") ) {
         add_failed_mess("Bad pattern to alias.\n");
         return 0;
      }
      if (pattern[0] == '*' || pattern[0] == '+') {
         add_failed_mess("Cannot start a regular expression with a '*' or '+', "
                         "try: '.*' or '.+'.\n");
         return 0;
      }
      if (catch(regexp("test str", pattern))) {
         add_failed_mess("Pattern has an error in it, mismatched brackets?\n");
         return 0;
      }
      // Now we filter out the ones we don't want.
      foreach (person in friends) {
         if (!regexp(this_player()->query_friend_tag(person), pattern)) {
            friends -= ({ person });
         }
      }
      if (!sizeof(friends)) {
         add_failed_mess("The tag '" + pattern + "' did not exist "
                         "in your friends list.\n");
         return 0;
      }
   }

   if (!sizeof(friends)) {
     if(person == "online") {
       add_failed_mess("You do not have any friends online.\n");
     } else {
       add_failed_mess("You do not have any friends.\n");
     }
     return 0;
   }

   ret = "";
   foreach (person in sort_array(friends, 0)) {
      ret += "$I$5=" + capitalize(person) + ": " +
             this_player()->query_friend_tag(person) + "%^RESET%^\n";
   }
   write("$P$Friends$P$" + ret);
   return 1;
} /* cmd_display() */

int cmd_add(string person, string tag) {
   person = lower_case(person);
   person = this_player()->expand_nickname(person);

   if(person == this_player()->query_name()) {
     add_failed_mess("Are you really that lonely?\n");
     return 0;
   }
   
   if (!PLAYER_HANDLER->test_user(person)) {
      add_failed_mess(person + " does not exist.\n");
      return 0;
   }

   if (strlen(tag) > PLAYER_MAX_FRIEND_TAG_LEN) {
      add_failed_mess("Your tag for " + person + " is too long, the "
                      "maximum is " + PLAYER_MAX_FRIEND_TAG_LEN + ".\n");
      return 0;
   }

   if (!this_player()->is_friend(person)) {
      this_player()->add_friend(person, tag);
      add_succeeded_mess(({ "You add " + person + " with a tag of: " +
                            tag + "%^RESET%^.\n",
                            "" }));
   } else {
      this_player()->add_friend(person, tag);
      add_succeeded_mess(({ "You change " + person + "'s tag to: " +
                            tag + "%^RESET%^.\n",
                            "" }));
   }
   return 1;
} /* cmd_add() */

int cmd_remove(string person) {
   person = lower_case(person);
   person = this_player()->expand_nickname(person);
   if (!this_player()->is_friend(person)) {
      add_failed_mess("Sorry, " + person + " is not your friend to remove.\n");
      return 0;
   }

   this_player()->remove_friend(person);
   add_succeeded_mess(({ "You remove " + person + " from your friends list.\n",
                         "" }));
   return 1;
} /* cmd_remove() */

int cmd_clear() {
   write("This will clear all of your friends.  Are you sure you want to do "
         "this? ");
   input_to("clear_check");
   add_succeeded_mess("");
   return 1;
} /* cmd_clear() */

void clear_check(string str) {
   string friend;

   if (!strlen(str) || str[0] != 'y') {
      write("Ok, aborting.\n");
      return ;
   }

   foreach (friend in this_player()->query_friends()) {
      this_player()->remove_friend(friend);
   }
   write("Friend list cleared.\n");
} /* clear_check() */

mixed *query_patterns() {
   return ({ "<word'friend'>",
                (: cmd_display($4[0], 0) :),
             "",
                (: cmd_display("", 0) :),
             "list <string>",
                (: cmd_display("", $4[0]) :),
             "online",
                (: cmd_display("online", 0) :),
             "add <word'friend'>",
                (: cmd_add($4[0], "needs no introduction") :),
             "add <word'friend'> <string'tag'>",
                (: cmd_add($4[0], $4[1]) :),
             "remove <word'friend'>",
                (: cmd_remove($4[0]) :),
             "clear",
                (: cmd_clear() :),
             });
} /* query_patterns() */
