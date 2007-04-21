/**
 * This is the new errors command to deal with the new error handler
 * code.
 * @author Pinkfish
 * @started Mon Apr 28 12:01:45 PDT 2003
 */
inherit "/cmds/base";
#include <error_handler.h>
#include <db.h>
#include <nroff.h>
#include <player_handler.h>

class user_data {
   class error_query query;
   class error_summary* errors;
   int pos;
   int last_touched;
   string direction;
   string report;
}

private mapping _user_query;

void create() {
   _user_query = ([ ]);
}

int find_index(object person, int id) {
   int i;
   class error_summary* errors;

   if (!_user_query[person->query_name()]) {
      return 0;
   }
   _user_query[person->query_name()]->last_touched = time();
   errors = _user_query[person->query_name()]->errors;
   for (i = 0; i < sizeof(errors); i++) {
      if (id == errors[i]->id) {
         return i;
      }
   }
   return 0;
}

int max_index(object person) {
   if (!_user_query[person->query_name()]) {
      return 0;
   }
   _user_query[person->query_name()]->last_touched = time();
   return sizeof(_user_query[person->query_name()]->errors);
}

int id_at(object person, int index) {
   if (!_user_query[person->query_name()] || index < 0 ||
       index >= sizeof(_user_query[person->query_name()]->errors)) {
      return 0;
   }

   _user_query[person->query_name()]->last_touched = time();
   return _user_query[person->query_name()]->errors[index]->id;
}

string menu_line(object person) {
   string ret;

   // Figure out the nice little menu line.
   ret = "[" + (_user_query[person->query_name()]->pos + 1) + " of " +
          sizeof(_user_query[person->query_name()]->errors) + "] "
         "STFCOLHA-+PNGQ : ";
   return ret;
}

void finish_details(object person, int verbose,
                    int type, mixed data) {
   string ret;
   class error_details details;
   class error_complete complete;
   class error_comment comment;
   class error_forward forward;
   class error_replies reply;
   class user_data user_data;

   if (type != DB_SUCCESS) {
      tell_object(person, "%^BOLD%^%^RED%^Error: " + data + "%^RESET%^\n");
      return ;
   }

   user_data = _user_query[person->query_name()];

   complete = data[0];
   details = complete->details;
   ret = "%^BOLD%^%^CYAN%^Bug #" + details->summary->id + "%^RESET%^ " +
         details->summary->status + " " + details->summary->type + " " +
         details->summary->category + "\n";

   ret += "%^BOLD%^Date Reported%^RESET%^     : " +
             ctime(details->summary->entry_date) + "\n";
   ret += "%^BOLD%^Assigned To%^RESET%^       : " +
          details->summary->assigned_to + "\n";
   ret += "%^BOLD%^Reporter%^RESET%^          : " + details->summary->reporter + "\n";
   ret += "%^BOLD%^File name%^RESET%^         : " + details->summary->filename + "\n";
   if (verbose) {
      ret += "%^BOLD%^Directory%^RESET%^         : " + details->summary->directory + "\n";
   }

   if (details->summary->status == "FIXED" ||
       details->summary->status == "DENIED") {
      ret += "%^BOLD%^Fixed By%^RESET%^          : " + details->fixer +
             " (" + ctime(details->fix_date) + ")\n";
   }

   if (verbose) {
      if (sizeof(complete->forwards)) {
         ret += "%^BOLD%^Forwards%^RESET%^          :\n";
         // Print out the forwards.
         foreach (forward in complete->forwards) {
            ret += "$I$5=     " + forward->forwarder + " from " +
                   forward->old_directory + " at " +
                   ctime(forward->date)[4..15] + "\n";
         }
      }
   }

   ret += "$I$0=" + details->report;
   if (verbose) {
      ret += details->runtime;
   } else if (details->runtime && details->runtime != "") {
      ret += "[Runtime Available]\n";
   }

   user_data->report = details->report;

   if (verbose) {
      ret += "$I$3=   ";
      // Show the error replies before the comments.
      foreach (reply in complete->replies) {
         ret += "%^BOLD%^Date Sent%^RESET%^   : " + ctime(reply->senddate)[4..15] +
                "\n%^BOLD%^From%^RESET%^        : " + reply->sender +
                "\n%^BOLD%^To%^RESET%^          : " + reply->recipient +
                "\n%^BOLD%^Subject%^RESET%^     : " + reply->subject + "<br>" +
                reply->message + "\n";
      }
      if (sizeof(complete->replies) && sizeof(complete->comments)) {
         ret += "$I$0=-------------------------------------------------------------\n$I$3=   ";
      }
      foreach (comment in complete->comments) {
         ret += "%^BOLD%^Comment by " + comment->commenter + " at " +
                ctime(comment->date)[4..15] + "%^RESET%^\n" +
                comment->comment + "\n";
      }
   } else {
      if (sizeof(complete->replies) > 0) {
         ret += sizeof(complete->replies) + " error replies.\n";
      }
      if (sizeof(complete->comments) > 0) {
         ret += sizeof(complete->comments) + " comments.\n";
      }
   }
   person->set_finish_func("finish_more_details", this_object());
   tell_object(person, "$P$Bug #" + details->summary->id + "$P$" + ret);
}

int finish_more_details() {
   object ob;

   ob = this_player();
   write(menu_line(ob));
   input_to("menu_response");
}

int finish_forward(object person, int pos, string new_dir, int type, mixed mess) {
   class user_data data;

   data = _user_query[person->query_name()];
   if (type != DB_SUCCESS) {
      tell_object(person, "Unable to forward bug #" +
                          data->errors[data->pos]->id +
                          " to " + new_dir + " (" + mess + ")");
   } else {
      tell_object(person, "Forwarded bug #" +
                          data->errors[data->pos]->id +
                          " from " + data->errors[data->pos]->directory +
                          " to " + new_dir);
      data->errors[data->pos]->directory = new_dir;
   }
}

void finish_change_status(object person, string status, int command,
                          string subject, int no_reply, int type, mixed mess) {
   class user_data data;


   data = _user_query[person->query_name()];

   if (type != DB_SUCCESS) {
      tell_object(person, "Database error: " + mess);
      if (!command) {
         tell_object(person, menu_line(person));
      }
      return ;
   }

   tell_object(person, "Updated status of bug #" + data->errors[data->pos]->id +
               " to " + status + (no_reply ?  " (no reply) " : "") +"\n");
   if (!command) {
      data = _user_query[person->query_name()];
      tell_object(person, menu_line(person));
      data->errors[data->pos]->status = status;
   }
}

void finish_status_message(object person, string status, int command,
                           string subject, string mess) {
   class user_data data;
   int pos;

   data = _user_query[person->query_name()];
   if (!mess) {
      tell_object(person, "Message aborted.\n");
      if (!command) {
         tell_object(person, menu_line(person));
      }
      return ;
   }

   if (status == "THANKS" || status == "TEMPORARY") {
      status = "FIXED";
   }
   if (status == "NOTREPRODUCIBLE" || status == "NOINFO" ||
       status == "NOTPOSSIBLE") {
      status = "DENIED";
   }

   pos = strsrch("$report$", mess);
   if (pos == -1) {
      mess += "\n\n" + data->report;
   } else {
      mess = replace_string(mess, "$report$", data->report);
   }

   if (!command) {
      input_to("menu_response");
   }
   // Do it.
   if (!ERROR_HANDLER->do_change_status(data->errors[data->pos]->id,
                        status, subject == 0, subject, person->query_name(),
                        mess,
                        (: finish_change_status, person, status, command, 
                                          subject, subject == 0 :))) {
      tell_object(person, "Unhappily an error occured.\n");
      if (!command) {
         tell_object(person, menu_line(person));
      }
   }
}

string query_subject_from_type(string type, class error_summary error) {
   switch (type) {
   case "THANKS" :
      return "Thanks for " + error->category + " " + error->type;
   case "NOTPOSSIBLE" :
      return "Not possible to fix " + error->category + " " + error->type;
   case "NOTREPRODUCIBLE" :
      return "Not able to reproduce " + error->category + " " + error->type;
   case "FIXED" :
      return "Fixed bug #" + error->id;
   case "DENIED" :
      return "Denied bug #" + error->id;
   default :
      return "Change status of bug #" + error->id + " to " +
             type;
   }
}

string query_message_from_type(object person, string type,
                               class error_summary error) {
   switch (upper_case(type)) {
   case "THANKS" :
      return "Thanks for the " + lower_case(error->type) + ".\n\nGood luck!\n" +
             person->query_cap_name();
   case "TEMPORARY" :
      return "Your " + lower_case(error->type) +
             " report was a temporary problem in the system and has "
             "since been resolved.  Thanks for reporting it."
             "\n\nGood luck!\n" +
             person->query_cap_name();
   case "NOTENOUGHINFORMATION" :
      return "Your " + lower_case(error->type) +
             " report did not contain enough information for this " +
             lower_case(error->type) + " to be resolved.  Please report "
             "again with more detailed information."
             "\n\nGood luck!\n" +
             person->query_cap_name();
   case "NOTREPRODUCIBLE" :
      return "Unable to reproduce your " + lower_case(error->type) +
             " report.  If you can track down exactly the circumstances "
             "which generate this error, please report it again."
             "\n\nGood luck!\n" +
             person->query_cap_name();
   case "NOTPOSSIBLE" :
      return "Thank you for your " + lower_case(error->type) +
             " report, this " + lower_case(error->type) +
             " is not possible to fix, because of code restrictions."
             "\n\nGood luck!\n" +
             person->query_cap_name();
   case "FIXED" :
   case "DENIED" :
      return "Thank you for your " + lower_case(error->type) +
             " report, this " + lower_case(error->type) +
             " has now been " + lower_case(type) + ".\n\nGood luck!\n" +
             person->query_cap_name();
   case "FIXING" :
      return "Thank you for your " + lower_case(error->type) +
             " report, this " + lower_case(error->type) +
             " is currently being fixed.\n\n"
             "Good luck!\n" +
             person->query_cap_name();
   case "CONSIDERING" :
      return "Thank you for your " + lower_case(error->type) +
             " report, this " + lower_case(error->type) +
             " is currently being considered for future fixing.\n\n"
             "Good luck!\n" +
             person->query_cap_name();
   case "CUSTOM" :
      return "custom";
   case "NONE" :
      return "none";
   case "OPEN" :
      return "Thanks for the " + lower_case(error->type) + ".\n\nGood luck!\n" +
             person->query_cap_name();
   }
}

void finish_change_type(object person, string new_type, int type, mixed mess) {
   class user_data data;

   data = _user_query[person->query_name()];
   if (type != DB_SUCCESS) {
      tell_object(person, "Sql Error: " + mess);
      tell_object(person, menu_line(person));
      return ;
   }

   tell_object(person, "Bug #" + data->errors[data->pos]->id +
                       " has a new type of " + new_type + "\n");
   tell_object(person, menu_line(person));
   data->errors[data->pos]->type = new_type;
}

void finish_change_category(object person, string new_category, int type, mixed mess) {
   class user_data data;

   data = _user_query[person->query_name()];
   if (type != DB_SUCCESS) {
      tell_object(person, "Sql Error: " + mess);
      tell_object(person, menu_line(person));
      return ;
   }

   tell_object(person, "Bug #" + data->errors[data->pos]->id +
                       " has a new category of " + new_category + "\n");
   tell_object(person, menu_line(person));
   data->errors[data->pos]->type = new_category;
}

void finish_assign_bug(object person, string assigner, int type, mixed mess) {
   class user_data data;

   data = _user_query[person->query_name()];
   if (type != DB_SUCCESS) {
      tell_object(person, "Sql Error: " + mess);
      tell_object(person, menu_line(person));
      return ;
   }

   tell_object(person, "Bug #" + data->errors[data->pos]->id +
                       " is assigned to " + assigner + "\n");
   tell_object(person, menu_line(person));
   data->errors[data->pos]->assigned_to = assigner;
}

void finish_adding_comment(object person, int type, mixed mess) {
   if (type != DB_SUCCESS) {
      tell_object(person, "SQL Error: " + mess);
      tell_object(person, menu_line(person));
   }

   tell_object(person, "Added a comment to the bug.\n");
   tell_object(person, menu_line(person));
}

void finish_comment(object person, int id, string comment) {
   if (!comment) {
      write("Aborted adding the comment.\n");
      write(menu_line(person));
      input_to("menu_response");
      return ;
   }
   input_to("menu_response");
   if (!ERROR_HANDLER->do_comment(id, person->query_name(), comment,
                             (: finish_adding_comment, person :))) {
      write("Big bad error.\n");
      write(menu_line(person));
   }
}

int do_bug_details(object person, int id, int verbose) {
   if (!ERROR_HANDLER->do_query_bug_details(id, (: finish_details, person,
                                                   verbose :))) {
      tell_object(person, "Error doing stuff.");
   }
   return 1;
}

int do_forward(object person, string new_dir) {
   class user_data data;

   data = _user_query[person->query_name()];
   if (!ERROR_HANDLER->do_forward(data->errors[data->pos]->id,
                person->query_name(), new_dir,
                    (: finish_forward, person, data->pos, new_dir :))) {
      write("A horrible error occured.\n");
      return 0;
   }
   return 1;
}

int do_change_status(object person, string status, int command, string subject,
                     string mess) {
   if (mess == "custom") {
      tell_object(person, "User $report$ to place the error report in the "
                  "mail.  If $report$ is not included, the report will be "
                  "added onto the end.\n");
      person->do_edit("", (: finish_status_message, person, status, command,
                                                    subject :));
   } else if (mess == "none") {
      finish_status_message(person, status, command, 0, "");
   } else {
      finish_status_message(person, status, command, subject, mess);
   }
}

int do_change_type(object person, string type) {
   class user_data data;

   data = _user_query[person->query_name()];
   if (!ERROR_HANDLER->do_change_type(data->errors[data->pos]->id, type,
       (: finish_change_type, person, type :))) {
      tell_object(person, "Error doing stuff.\n");
   }
   return 1;
}

int do_change_category(object person, string category) {
   class user_data data;

   data = _user_query[person->query_name()];
   if (!ERROR_HANDLER->do_change_category(data->errors[data->pos]->id, category,
       (: finish_change_category, person, category :))) {
      tell_object(person, "Error doing stuff.\n");
   }
   return 1;
}

int do_assign_bug(object person, string assigner) {
   class user_data data;

   data = _user_query[person->query_name()];
   if (!ERROR_HANDLER->do_assign_bug_to(data->errors[data->pos]->id, assigner,
       (: finish_assign_bug, person, assigner :))) {
      tell_object(person, "Error doing stuff.\n");
   }
   return 1;
}

int do_comment(object person) {
   class user_data data;

   data = _user_query[person->query_name()];
   this_player()->do_edit("", (: finish_comment, person, 
                                   data->errors[data->pos]->id :));
}

int do_help(object player) {
   string str;

   str = NROFF_HAND->cat_file("/doc/creatornr/errors", 1);
   if (!str) {
      NROFF_HAND->create_nroff("/doc/creator/commands/errors", "/doc/creatornr/errors");
      str = NROFF_HAND->cat_file("/doc/creatornr/errors", 1);
   }
   player->set_finish_func("finish_more_details", this_object());
   tell_object(player, "$P$Error help$P$" + str);
}

int do_list_errors() {
   class user_data data;
   class error_summary error;
   string str;
   int i;

   data = _user_query[this_player()->query_name()];
   str = "";
   i = 1;
   str += sprintf("ndx: #<num> %5-s %4-s %4-s by %11-s %15-s [%s]\n",
                  "Status", "Cat", "Type", "Reporter", "Filename", "Directory");
   foreach (error in data->errors) {
      str += sprintf("$I$3=%3d: #%d %5-s %4-s %4-s by %11-s %15-s [%s]\n",
                     i++, error->id, error->status[0..4], error->category[0..3],
                     error->type, error->reporter, error->filename[<15..],
                     error->directory[<15..]);
   }
   this_player()->set_finish_func("finish_more_details", this_object());
   write("$P$Error list$P$" + str);
}

int show_current_bug(object player, int verbose) {
   class user_data data;

   data = _user_query[player->query_name()];
   if (verbose == -1) {
      verbose = player->query_verbose("errors");
   }
   return do_bug_details(player, data->errors[data->pos]->id, verbose);
}

string expand_short_type(string name) {
   string str;

   str = upper_case(name);
   switch (str) {
   case "F" :
      return ERROR_STATUS_FIXED;
   case "D" :
      return ERROR_STATUS_DENIED;
   case "I" :
      return ERROR_STATUS_FIXING;
   case "C" :
      return ERROR_STATUS_CONSIDERING;
   case "O" :
      return ERROR_STATUS_OPEN;
   case "U" :
      return "CUSTOM";
   case "R" :
      return "NOTREPRODUCIBLE";
   case "R" :
      return "NOTPOSSIBLE";
   case "N" :
      return "NONE";
   case "T" :
      return "THANKS";
   case "M" :
      return "NOTENOUGHINFORMATION";
   case "E" :
      return "TEMPORARY";
   default :
      return str;
   }
}

void finish_change_status_command(object player, int id, string status,
                        string messtype, int type, mixed data) {
   string mess;
   string subject;
   class error_summary error;

   if (type != DB_SUCCESS) {
      tell_object(player, "SQL error: " + data + "\n");
      return ;
   }

   if (!sizeof(data)) {
      tell_object(player, "No bug found with an id of #" + id + "\n");
      return ;
   }

   error = data[0];

   _user_query[player->query_name()] = new(class user_data);
   _user_query[player->query_name()]->errors = data;
   _user_query[player->query_name()]->pos = 0;

   status = expand_short_type(status);
   // Now lets see if they asked for a standard reply or not.
   if (messtype) {
      messtype = expand_short_type(messtype);
      mess = query_message_from_type(player, messtype, error);
      subject = query_subject_from_type(messtype, error);
      if (!mess) {
         tell_object(player, "The reply type " + messtype + " was not valid.\n");
      } else {
         if (member_array(status, ERROR_TYPE) != -1) {
            tell_object(player, "The error type must be one of " +
                  query_multiple_short(ERROR_TYPE) + ".\n");
         } else {
            // Allow the person to enter their own message.
            do_change_status(player, status, 1, subject, mess);
            return ;
         }
      }
   } else {
      mess = query_message_from_type(player, status, error);
      subject = query_subject_from_type(status, error);
      // Change the status.
      if (mess) {
         do_change_status(player, status, 1, subject, mess);
         return ;
      }
   }
   tell_object(player, "Syntax: s {OPEN|FIXED|CONSIDERING|DENIED|THANKS|NOTPOSSIBLE|NOTREPRODUCIBLE|NOTENOUGHINFORMATION} [{none|custom|fixed|denied|notpossible|notreproducible|notenoughinformation}]\n");
   tell_object(player, "The default message is based on the type it is changed to.\n");
   tell_object(player, "The type has some short forms: o -> open, f -> fixed, d ->denied\n");
   tell_object(player, "   c ->considering, t -> thanks, n -> none, u -> custom, i -> fixing.\n");
   tell_object(player, "   r ->notreproducible, p -> notpossible, m -> notenoughinformation.\n");
   tell_object(player, "   t ->temporary.\n");
}

/**
 * This function does most of the work.  It handles the response from the
 * creator and does what it needs to with the error.
 */
void menu_response(string str) {
   string* bits;
   class user_data data;
   int print_menu_line;
   class error_summary error;
   string mess;
   string subject;
   int id;

   bits = explode(str, " ");
   if (!sizeof(bits)) {
/*
      write("You need to specify something to do.\n");
      write(menu_line(this_player()));
      input_to("menu_response");
      return ;
 */
      bits = ({ "n" });
   }

   data = _user_query[this_player()->query_name()];
   switch(lower_case(bits[0])) {
   case "v" :
      if (sizeof(bits) > 1) {
         show_current_bug(this_player(), bits[1] == "verbose");
      } else {
         show_current_bug(this_player(), -1);
      }
      break;
   case "+" :
   case "n" :
      // next bug.
      if (data->pos < sizeof(data->errors) - 1) {
         data->pos++;
         print_menu_line = !show_current_bug(this_player(), -1);
      } else {
         write("No next bug to go to.\n");
         print_menu_line = 1;
      }
      break;
   case "-" :
   case "p" :
      // previous bug.
      if (data->pos > 0) {
         data->pos--;
         print_menu_line = !show_current_bug(this_player(), -1);
      } else {
         write("No previous bug to go to.\n");
         print_menu_line = 1;
      }
      break;
   case "f" :
      // Forward the bug somewhere.
      if (sizeof(bits) == 2) {
         if (do_forward(this_player(), bits[1])) {
            input_to("menu_response");
         } else {
            print_menu_line = 1;
         }
      } else {
         write("Need to specify the destination directory (and nothing else)\n");
         print_menu_line = 1;
      }
      break;
   case "s" :
      // Change the status of the bug
      if (sizeof(bits) > 1) {
         bits[1] = expand_short_type(bits[1]);
         // Now lets see if they asked for a standard reply or not.
         error = data->errors[data->pos];
         if (sizeof(bits) > 2) {
            bits[2] = expand_short_type(bits[2]);
            mess = query_message_from_type(this_player(), bits[2], error);
            subject = query_subject_from_type(bits[1], error);
            if (!mess) {
               write("The reply type " + bits[2] + " was not valid.\n");
               print_menu_line = 1;
            } else {
               if (member_array(bits[1], ERROR_TYPE) != -1) {
                  write("The error type must be one of " +
                        query_multiple_short(ERROR_TYPE) + ".\n");
                  print_menu_line = 1;
               } else {
                  // Allow the person to enter their own message.
                  do_change_status(this_player(), bits[1], 0, subject, mess);
                  input_to("menu_response");
               }
            }
         } else {
            mess = query_message_from_type(this_player(), bits[1], error);
            subject = query_subject_from_type(bits[1], error);
            // Change the status.
            if (mess) {
               do_change_status(this_player(), bits[1], 0, subject, mess);
               input_to("menu_response");
            }
         }
         if (!mess) {
            write("Syntax: s {OPEN|FIXED|CONSIDERING|DENIED|THANKS|NOTPOSSIBLE|NOTREPRODUCIBLE|NOTENOUGHINFORMATION} [{none|custom|fixed|denied|notpossible|notreproducible|notenoughinformation}]\n");
            write("The default message is based on the type it is changed to.\n");
            write("The type has some short forms: o -> open, f -> fixed, d ->denied\n");
            write("   c ->considering, t -> thanks, n -> none, u -> custom, i -> fixing.\n");
            write("   r ->notreproducible, p -> notpossible, m -> notenoughinformation.\n");
            write("   t ->temporary.\n");
            print_menu_line = 1;
         }
      } else {
         write("Syntax: s {OPEN|FIXED|CONSIDERING|DENIED|THANKS|NOTPOSSIBLE|NOTREPRODUCIBLE|NOTENOUGHINFORMATION} [{none|custom|fixed|denied|notpossible|notreproducible|notenoughinformation}]\n");
         write("The default message is based on the type it is changed to.\n");
         write("The type has some short forms: o -> open, f -> fixed, d ->denied\n");
         write("   c ->considering, t -> thanks, n -> none, u -> custom, i -> fixing.\n");
         write("   r ->notreproducible, p -> notpossible, m -> notenoughinformation.\n");
         write("   t ->temporary.\n");
         print_menu_line = 1;
      }
      break;
   case "t" :
      // Change the type of the bug.
      if (sizeof(bits) > 1) {
         bits[1] = upper_case(bits[1]);
         if (member_array(bits[1], ERROR_TYPE) != -1) {
            do_change_type(this_player(), bits[1]);
            input_to("menu_response");
         } else {
            write("The type must be one of " +
                  query_multiple_short(ERROR_TYPE) + ".\n");
            print_menu_line = 1;
         }
      } else {
         write("Syntax: t {TYPO|BUG|IDEA}\n");
         print_menu_line = 1;
      }
      break;
   case "o" :
      // Change the category of the bug.
      if (sizeof(bits) > 1) {
         bits[1] = upper_case(bits[1]);
         if (member_array(bits[1], ERROR_CATEGORIES) != -1) {
            do_change_category(this_player(), bits[1]);
            input_to("menu_response");
         } else {
            write("The type must be one of " +
                  query_multiple_short(ERROR_CATEGORIES) + ".\n");
            print_menu_line = 1;
         }
      } else {
         write("Syntax: t {ROOM|OBJECT|COMMAND|HELP|RITUAL|SPELL|GENERAL}\n");
         print_menu_line = 1;
      }
      break;
   case "g" :
      // goto a specific bug.
      if (sizeof(bits) > 0) {
         id = to_int(bits[1]);
         if (id <= sizeof(data->errors)) {
            data->pos = id - 1;
            show_current_bug(this_player(), -1);
         } else {
            // See if we can find it.
            id = find_index(this_player(), id);
            if (id == -1) {
               write("Unable to find the bug #" + id + "\n");
               print_menu_line = 1;
            }
         }
      } else {
         write("Syntax: g <num>\nNum can either be the index number or the "
               "bug id.\n");
         print_menu_line = 1;
      }
      break;
   case "c" :
      // comment on the bug.
      do_comment(this_player());
      break;
   case "i" :
   case "l" :
      do_list_errors();
      break;
   case "q" :
      write("Bye bye.\n");
      return ;
   case "a" :
      if (sizeof(bits) > 1) {
         if (!PLAYER_HANDLER->test_creator(bits[1])) {
            write("You can only assign a bug to a creator.\n");
            print_menu_line = 1;
         } else {
            do_assign_bug(this_player(), bits[1]);
            input_to("menu_response");
         }
      } else {
         write("You need to specify someone to assign the error to.\n");
         print_menu_line = 1;
      }
      break;
   case "h" :
      do_help(this_player());
      break;
   default :
      write("Bad command.\n");
      print_menu_line = 1;
      break;
   }
   if (print_menu_line) {
      write(menu_line(this_player()));
      input_to("menu_response");
   }
}

void finish_query(object player, int type, mixed data) {
   if (type != DB_SUCCESS) {
      tell_object(player, "Error: " + data + "\n");
      return ;
   }

   _user_query[player->query_name()] = new(class user_data);
   _user_query[player->query_name()]->errors = data;
   _user_query[player->query_name()]->pos = 0;
   if (!sizeof(data)) {
      tell_object(player, "No bugs with that query.\n");
      return ;
   }
   show_current_bug(player, -1);
}

int setup_query(object player, class error_query query) {
   if (!ERROR_HANDLER->do_query_bug_summary(query, (: finish_query, player :))) {
      add_failed_mess("Unable to setup the query.\n");
      return 0;
   }
   return 1;
}

int errors_change_status(int id, string status, string messtype) {
   class error_query query;

   query = new (class error_query);
   query->id = id;
   if (!ERROR_HANDLER->do_query_bug_summary(query,
                     (: finish_change_status_command, this_player(), id, status,
                        messtype :))) {
      add_failed_mess("Unable to setup the query.\n");
      return 0;
   }
   write("Looking up bug #" + id + " to fix.\n");
   return 1;
}

int errors_this_dir(int recursive) {
   class error_query query;

   query = new (class error_query);
   query->dir = this_player()->query_path();
   query->recursive = recursive;
   query->status = ({ ERROR_STATUS_OPEN, ERROR_STATUS_CONSIDERING,
                      ERROR_STATUS_FIXING });
   return setup_query(this_player(), query);
}

int errors_by_person(string person) {
   class error_query query;

   query = new (class error_query);
   query->reporter = lower_case(person);
   query->status = ({ ERROR_STATUS_OPEN, ERROR_STATUS_CONSIDERING,
                      ERROR_STATUS_FIXING });
   return setup_query(this_player(), query);
}

int errors_assigned_to_person(string person) {
   class error_query query;

   query = new (class error_query);
   query->assigned_to = lower_case(person);
   query->status = ({ ERROR_STATUS_OPEN, ERROR_STATUS_CONSIDERING,
                      ERROR_STATUS_FIXING });
   return setup_query(this_player(), query);
}

int errors_assigned_to_me() {
   return errors_assigned_to_person(this_player()->query_name());
}

int errors_in_dir(string str, int recursive) {
   class error_query query;
   string path;

   path = this_player()->get_path(str);
   query = new (class error_query);
   query->dir = path;
   query->recursive = recursive;
   query->status = ({ ERROR_STATUS_OPEN, ERROR_STATUS_CONSIDERING,
                      ERROR_STATUS_FIXING });
   return setup_query(this_player(), query);
}

int errors_with_id(int id) {
   class error_query query;

   query = new (class error_query);
   query->id = id;
   query->status = ({ ERROR_STATUS_OPEN, ERROR_STATUS_CONSIDERING,
                      ERROR_STATUS_FIXING });
   return setup_query(this_player(), query);
}

int errors_for_file(string file) {
   class error_query query;

   query = new (class error_query);
   query->file_name = file;
   query->status = ({ ERROR_STATUS_OPEN, ERROR_STATUS_CONSIDERING,
                      ERROR_STATUS_FIXING });
   return setup_query(this_player(), query);
}

int errors_in_this_room() {
   class error_query query;

   query = new (class error_query);
   query->file_name = file_name(environment(this_player()));
   query->status = ({ ERROR_STATUS_OPEN, ERROR_STATUS_CONSIDERING,
                      ERROR_STATUS_FIXING });
   return setup_query(this_player(), query);
}

mixed* query_patterns() {
   return ({ "", (: errors_this_dir(0) :),
             "status <number'bug id'> <word'status'>",
                   (: errors_change_status($4[0], $4[1], 0) :),
             "status <number'bug id'> <word'status'> <word'reply'>",
                   (: errors_change_status($4[0], $4[1], $4[2]) :),
             "recursive", (: errors_this_dir(1) :),
             "mine", (: errors_assigned_to_me :),
             "here", (: errors_in_this_room :),
             "assigned <string'person'>", (: errors_assigned_to_person($4[0]) :),
             "by <string'person'>", (: errors_by_person($4[0]) :),
             "id <number'bug id'>", (: errors_with_id($4[0]) :),
             "file <string'file'>", (: errors_for_file($4[0]) :),
             "dir <string'dir'>", (: errors_in_dir($4[0], 0) :),
             "dir recursive <string'dir'>", (: errors_in_dir($4[0], 1) :) });
}
