/*  -*- LPC -*-  */
/*
 * $Id: bug_replies.c,v 1.10 2003/07/08 06:55:58 pinkfish Exp $ 
 */
/**
 * THis is the base object for handleing error replies.
 * @author Pinkfish
 * @started Fri Feb 14 11:50:29 PST 2003
 */

#include <error_handler.h>
#include <db.h>
#include <nroff.h>

#define HELP_FILE "/doc/helpdir/bug_replies"

class player_data {
   int position;
   int date;
   class error_replies* replys;
}

nosave private mapping _player_replies;

void print_menu_input();

void create() {
   _player_replies = ([ ]);
}

void print_bug_replies(object player) {
   string str;
   class error_replies* replies;
   int i;
   int pos;

   replies = _player_replies[player]->replys;
   if (!sizeof(replies)) {
      tell_object(player, "No bug replies.\n");
      return ;
   }
   pos = _player_replies[player]->position;
   str = "";
   for (i = 0; i < sizeof(replies); i++) {
      if (i == pos) {
         str += ">";
      } else {
         str += " ";
      }
      switch (replies[i]->status) {
      case "NEW" :
         str += "N";
         break;
      case "DELETED" :
         str += "D";
         break;
      default :
         str += " ";
         break;
      }
      str += (i+1) + ") " + replies[i]->sender + " Sub: " +
             replies[i]->subject + "\n";
   }
   player->set_finish_func("finish_more_replies", this_object());
   tell_object(player, "$P$Bug replies$P$" + str);
}

void finish_more_replies() {
   print_menu_input();
}

void print_menu(object player) {
   int len;

   len = sizeof(_player_replies[player]->replys);
   if (len == 0) {
      tell_object(player, "[None] QDRULH?<num>: ");
   } else {
      tell_object(player, "[1-" + len + "] QDRULH?<num>: ");
   }
}

void delete_message(int start_pos, int end_pos) {
   int pos;

   for (pos = start_pos - 1; pos <= end_pos - 1; pos++) {
      ERROR_HANDLER->do_status_error_reply(_player_replies[this_player()]->replys[pos]->id,
                                           "DELETED",
                                           (: 1 :));
      _player_replies[this_player()]->replys[pos]->status = "DELETED";
   }
   if (start_pos != end_pos) {
      write("Delete error reply " + start_pos + " from " + end_pos + ".\n");
   } else {
      write("Delete error reply " + (pos) + ".\n");
   }
}

void undelete_message(int start_pos, int end_pos) {
   int pos;

   for (pos = start_pos - 1; pos <= end_pos - 1; pos++) {
      ERROR_HANDLER->do_status_error_reply(_player_replies[this_player()]->replys[pos]->id,
                                        "READ",
                                        (: 1 :));
      _player_replies[this_player()]->replys[pos]->status = "READ";
   }
   if (start_pos != end_pos) {
      write("Undelete error reply " + start_pos + " from " + end_pos + ".\n");
   } else {
      write("Undelete error reply " + (pos) + ".\n");
   }
}

int valid_message(int pos) {
   pos--;
   if (pos < 0 || pos >= sizeof(_player_replies[this_player()]->replys)) {
      return 0;
   }
   return 1;
}

void print_menu_input() {
   print_menu(this_player());
   input_to("bug_replies_menu");
}

void read_message(int pos) {
   class error_replies reply;

   pos--;
   reply = _player_replies[this_player()]->replys[pos];
   this_player()->set_finish_func("print_menu_input", this_object());
   this_player()->more_string("Date Sent: " + ctime(reply->senddate) + "\n"
         "From: " + reply->sender + "\n"
         "Subject: " + reply->subject + "\n\n" +
         reply->message + "\n");

   if (reply->status == "NEW") {
      ERROR_HANDLER->do_status_error_reply(reply->id, "READ", (: 1 :));
      reply->status = "READ";
   }
}

int read_next_new() {
   int i;
   class error_replies* replies;

   replies = _player_replies[this_player()]->replys;
   for (i = 0; i < sizeof(replies); i++) {
      if (replies[i]->status == "NEW") {
         read_message(i + 1);
         return 1;
      }
   }
   write("No new unread error replies.\n");
   return 0;
}

void finish_editing_message(string mess, int pos) {
   class error_replies reply;

   if (!mess || !strlen(mess)) {
      write("Aborting send of message.\n");
   } else {
      reply = _player_replies[this_player()]->replys[pos];
      ERROR_HANDLER->do_add_error_reply(reply->id,
                             this_player()->query_name(),
                             reply->sender,
                             "Re: " + reply->subject,
                             mess);
      write("Sent message to " + reply->sender + "\n");
   }
   print_menu(this_player());
   input_to("bug_replies_menu");
}

int reply_message(int pos) {
   write("Replying to message:\n");
   //read_message(pos);
   pos--;
   this_player()->do_edit("", "finish_editing_message", this_object(), 0, pos);
   return 1;
}

void help_command() {
   string nroff_fn;
   string str;

   nroff_fn = HELP_FILE + ".o";
   str = NROFF_HAND->cat_file(nroff_fn, 1);
   if (!str) {
      NROFF_HAND->create_nroff(HELP_FILE, nroff_fn);
      str = NROFF_HAND->cat_file(nroff_fn, 0);
   }
   this_player()->set_finish_func("print_menu_input", this_object());
   this_player()->more_string(str);
}

void bug_replies_menu(string inp) {
   int pos;
   int new_pos;
   int end_pos;

   // First strip any start and end spaces.
   while (strlen(inp) && inp[0] == ' ') {
      inp = inp[1..];
   }
   while (strlen(inp) && inp[<1] == ' ') {
      inp = inp[0..<2];
   }
   end_pos = -1;
   // Now figure out what we have here.
   if (!strlen(inp)) {
      if (read_next_new()) {
         return ;
      }
   } else {
      if (inp[0] >= '0' && inp[0] <= '9') {
         // Number!
         while (pos < strlen(inp) && inp[pos] >= '0' && inp[pos] <= '9') {
            pos ++;
         }
         pos = to_int(inp[0..pos-1]);
         if (valid_message(pos)) {
            read_message(pos);
            return ;
         } else {
            write("Message " + pos + " is invalid.\n");
         }
      } else {
         // See if there is a number after us.
         while (pos < strlen(inp) && (inp[pos] < '0' || inp[pos] > '9')) {
            pos++;
         }
         if (pos < strlen(inp)) {
            new_pos = pos + 1;
            while (new_pos < strlen(inp) && inp[new_pos] >= '0' && inp[new_pos] <= '9') {
               new_pos++;
            }
            pos = to_int(inp[pos..new_pos-1]);
            if (inp[new_pos] == '-') {
               // Look for the next spot.
               end_pos = new_pos + 1;
               while (end_pos < strlen(inp) && inp[end_pos] >= '0' && inp[end_pos] <= '9') {
                  end_pos++;
               }
               end_pos = to_int(inp[new_pos + 1..end_pos-1]);
            }
         } else {
            pos = -1;
         }
         switch(lower_case(inp)[0]) {
         case 'q' :
            write("Exiting the system.\n");
            map_delete(_player_replies, this_player());
            return ;
         case 'r' :
            if (valid_message(pos)) {
               if (reply_message(pos)) {
                  return ;
               }
            } else {
               write("Message " + pos + " is invalid.\n");
            }
            break;
         case 'd' :
            if (end_pos == -1) {
               end_pos = pos;
            }
            if (valid_message(pos)) {
               if (valid_message(end_pos)) {
                  if (end_pos >= pos) {
                     delete_message(pos, end_pos);
                  } else {
                     write("Your end position (" + end_pos + ") must be higher "
                           "than the start position (" + pos + ").\n");
                  }
               } else {
                  write("Message " + end_pos + " is invalid.\n");
               }
            } else {
               write("Message " + pos + " is invalid.\n");
            }
            break;
         case 'u' :
            if (end_pos == -1) {
               end_pos = pos;
            }
            if (valid_message(pos)) {
               if (valid_message(end_pos)) {
                  if (end_pos >= pos) {
                     undelete_message(pos, end_pos);
                  } else {
                     write("Your end position (" + end_pos + ") must be higher "
                           "than the start position (" + pos + ").\n");
                  }
               } else {
                  write("Message " + end_pos + " is invalid.\n");
               }
            } else {
               write("Message " + pos + " is invalid.\n");
            }
            break;
         case 'h' :
         case '?' :
            help_command();
            return ;
         case 'l' :
            print_bug_replies(this_player());
            return ;
         }
      }
   }
   print_menu(this_player());
   input_to("bug_replies_menu");
}

void bug_replies_result(int type, mixed* data, object player) {
   if (type != DB_SUCCESS) {
      tell_object(player, "Error retrieving replies.\n");
   } else {
      _player_replies[player] = new(class player_data, date : time(), 
                                    replys : data);
      print_bug_replies(player);
      //print_menu(player);
   }
}

/**
 * This method handles printing out any bug replies associated with
 * the player.
 * @param only_new only display new error replies
 */
int bug_replies(int only_new) {
  _player_replies[this_player()] = new(class player_data, date : time(), 
                                replys : ({ }));
   if (ERROR_HANDLER->do_error_replies(this_player()->query_name(), only_new,
                             (: bug_replies_result($1, $2, $(this_player()) ) :))) {
      input_to("bug_replies_menu");
      return 1;
   }
   add_failed_mess("Unable to find the bug replies.\n");
   return 0;
}
