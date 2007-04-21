/*  -*- LPC -*-  */
inherit "/cmds/base";
#include <player.h>

mixed cmd_list() {
   string* titles;
//   string* multiple;
//   string bing;

   if (this_player()->query_player_title()) {
      write("Your player title is currently set to: " +
            this_player()->query_player_title() + ".\n");
   } else {
      write("Your player title is currently not set.\n");
   }

   titles = this_player()->query_all_player_titles();
/*
   multiple = PLAYER_TITLE_HANDLER->query_allowed_multiple_titles();
   foreach (bing in multiple) {
     if (!sizeof(filter(explode(bing, " "),
                        (: member_array($1, $2) == -1 :)))) {
        titles += ({ bing });
     }
   }
 */

   write("You can set your player title to one of " +
         query_multiple_short(map(titles, (: capitalize($1) :))) + ".\n");
   return 1;
} /* cmd_list() */

int cmd_set(string title) {
    mixed mess;
//   string* bits;
//   string womble;

   title = lower_case(title);
   //bits = explode(title, " ");

/*
   if (sizeof(bits) > 1 &&
       !PLAYER_TITLE_HANDLER->is_allowed_multiple_title(title)) {
      add_failed_mess("You cannot use a title with more than 1 bit.\n");
      return 0;
   }
 */

   if (strlen(title) > 20) {
      add_failed_mess("Your title is too long.\n");
   }

   if (member_array(title,
                    this_player()->query_all_player_titles()) == -1) {
      add_failed_mess("The title '" + title + "' is not valid.\n");
      return 0;
   }

/*
   foreach (womble in bits) {
      if (member_array(womble,
                       this_player()->query_all_player_titles()) == -1) {
          add_failed_mess("The title '" + womble + "' is not valid.\n");
          return 0;
       }
   }
 */

   if (this_player()->query_player_title() &&
       lower_case(this_player()->query_player_title()) == title) {
      add_failed_mess("You already have your player title set to '" + title +
                      "'.\n");
      return 0;
   }

   this_player()->set_player_title(title);

   if ( sizeof( this_player()->query_hide_invis() ) ) { 
       mess = ({ "$N set$s $p player title to " + capitalize(title) +  
           ".\n", "" });
   }
   else {
       mess = "$N set$s $p player title to " + capitalize(title) +  ".\n";
   }
   add_succeeded_mess(mess);
   return 1;
} /* cmd_set() */

int cmd_reset() {
   if (!this_player()->query_player_title()) {
      add_failed_mess("Your player title is already unset.\n");
      return 0;
   }
   this_player()->set_player_title(0);
   add_succeeded_mess("$N reset$s $p player title.\n");
   return 1;
} /* cmd_reset() */

mixed *query_patterns() {
   return ({ "", (: cmd_list :),
             "<string'title'>", (: cmd_set($4[0]) :),
             "none", (: cmd_reset() :) });
} /* query_patterns() */
