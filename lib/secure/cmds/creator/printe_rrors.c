/*  -*- LPC -*-  */
/*
 * $Id: printe_rrors.c,v 1.4 2003/04/22 21:45:26 pinkfish Exp $
 */

/* printerrors command */

#include <log.h>
#include <board.h>
#include <error_handler.h>
#include <db.h>

inherit "/cmds/base";

void finish_details(string player, int result, mixed results);

int cmd(string str) {
  string player, ret;
  class error_query query;
   
  if (str) {
    if (str[0] != '/')  {
      str = this_player()->query_path() + "/" + str;
    }
    if (str[<1] == '0') {
      str = str[0..<2];
    }
    if (str[<1] == '/') {
      str = str[0..<2];
    }
    player = this_player()->query_name();
    write_file("/w/" + player + "/print_errors.txt", "", 1);

    write("%^YELLOW%^Starting to collect bugreports, this may take a "
          "while.%^RESET%^\n");
    query = new(class error_query);
    query->recursive = 1;
    query->dir = str;
    query->status = ({ ERROR_STATUS_OPEN, ERROR_STATUS_CONSIDERING,
                       ERROR_STATUS_FIXING });
    ERROR_HANDLER->do_query_multiple_bug_details(query,
                  (: finish_details($(player), $1, $2) :));
    return 1;
  } else
    return notify_fail("Usage:\nprinterrors <path>\n note that this will "
                       "overwrite any previous print_errors.txt file\n");
}

protected string get_bug(class error_complete complete) {
  string ret;
  class error_details error;
  class error_forward forward;
  class error_comment comment;

  error = complete->details;
  ret = sprintf("%s %s %s\n", error->summary->category, 
                              error->summary->type, error->summary->status);
  ret += sprintf("Filename       : %s\n", error->summary->filename);
  ret += sprintf("Directory      : %s\n", error->summary->directory);
  ret += sprintf("Made by        : %s\n", capitalize(error->summary->reporter));
  ret += sprintf("at             : %s\n\n", ctime(error->summary->entry_date));
  foreach (forward in complete->forwards) {
    ret += sprintf("Forwarded by   : %s\n", capitalize(forward->forwarder));
    ret += sprintf("from           : %s\n", forward->old_directory);
    ret += sprintf("at             : %s\n", ctime(forward->date));
  }
  ret += error->report;
  if (error->runtime && (error->runtime != "")) {
    ret += "[RUNTIME]\n" + error->runtime;
  }
  foreach (comment in complete->comments) {
      ret += sprintf("\nComment by     : %s\n", capitalize(comment->commenter));
      ret += sprintf(  "at             : %s\n", ctime(comment->date));
      ret += comment->comment;
  }
  ret +=
"------------------------------------------------------------------------------";
  return ret;
} /* get_bug() */

void finish_details(string player, int result, mixed results) {
   class error_complete* details;
   string str;

   if (result != DB_SUCCESS) {
      tell_creator(player, "Failed to return the results " + results);
      return ;
   }
   details = results;
   foreach (class error_complete detail in details) {
      str = get_bug(detail);
      unguarded( (: write_file("/w/" + $(player) + "/print_errors.txt", $(str)) :) );
   }
   tell_creator(player, "Finished gathering the details.\n");
}
