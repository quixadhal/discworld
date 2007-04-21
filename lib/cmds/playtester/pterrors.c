/**
 * This command will display the errors for the current room the
 * pt is standing in.
 * @author Pinkfish
 * @started Thu Feb  6 17:33:05 PST 2003
 */
inherit "/cmds/base";
#include <db.h>
#include <error_handler.h>
#include <player_handler.h>

void finish_lookup(object player, int type, mixed summary) {
   string bugs;
   class error_summary error;

   if (type != DB_SUCCESS) {
      tell_object(player, "Error looking up the bugs.\n" + summary + "\n");
      return ;
   }

   if (!sizeof(summary)) {
      tell_object(player, "No bugs found.\n");
      return ;
   }

   bugs = "";
   foreach (error in summary) {
      bugs += "#" + error->id + " " +
              ctime(error->entry_date)[4..15] +
              " by " + error->reporter + " (" +
              error->type + " - " + error->category + " - " +
              error->status + ")";
      if (error->assigned_to) {
         bugs += " assigned to " + error->assigned_to;
      }
      bugs += "\n";
   }
   tell_object(player, "$P$Bug List$P$" + bugs);
}

int cmd() {
   string fname;

   fname = base_name(environment(this_player()));
   if (!ERROR_HANDLER->do_query_open_bugs_for_fname(fname,
                   (: finish_lookup($(this_player()), $1, $2) :))) {
      add_failed_mess("Error attempting to query the bugs.\n");
      return 0;
   }
   return 1;
}

int do_query(string reporter, string status) {
   class error_query query;

   query = new(class error_query);
   if (status) {
      query->status = ({ status });
   }
   query->reporter = reporter;
   if (!this_player()->query_property("pterrors order")) {
      query->order_by = ERROR_ORDER_BY_DATE;
   } else {
      query->order_by = this_player()->query_property("pterrors order");
   }
   // Save it for two minutes.
   this_player()->add_property("pterrors query", query, 120);
   return ERROR_HANDLER->do_query_bug_summary(query,
                   (: finish_lookup($(this_player()), $1, $2) :));
}

/**
 * Finds the next set of bugs based on the last query.
 */
int cmd_next() {
   class error_query query;

   query = this_player()->query_property("pterrors query");
   if (!query) {
      add_failed_mess("You have not done a bugs query recently enough.\n");
      return 0;
   }
   query->bug_offset += 150;
   this_player()->add_property("pterrors query", query, 120);
   if (!ERROR_HANDLER->do_query_bug_summary(query,
                   (: finish_lookup($(this_player()), $1, $2) :))) {
      add_failed_mess("Error attempting to query the bugs.\n");
      return 0;
   }
   return 1;
}

int cmd_mine(string status) {
   if (!do_query(this_player()->query_name(), status)) {
      add_failed_mess("Error attempting to query the bugs.\n");
      return 0;
   }
   return 1;
}

int cmd_order(string order) {
   switch (order) {
   case "date" :
      this_player()->add_property("pterrors order", ERROR_ORDER_BY_DATE);
      break;
   case "filename" :
      this_player()->add_property("pterrors order", ERROR_ORDER_BY_FILENAME);
      break;
   case "directory" :
      this_player()->add_property("pterrors order", ERROR_ORDER_BY_DIRECTORY);
      break;
   }
   write("Set your pterrors bug order to " + order + "\n");
   return 1;
}

int cmd_ptbugs(string name, string status) {
   if (!PLAYER_HANDLER->test_user(name)) {
      add_failed_mess("There is no player called " + name + ".\n");
      return 0;
   }
   if (!do_query(lower_case(name), status)) {
      add_failed_mess("Error attempting to query the bugs.\n");
      return 0;
   }
   return 1;
}


void finish_details(object player, int type, mixed details) {
   string bugs;
   class error_complete complete;
   class error_details error;

   if (type != DB_SUCCESS) {
      tell_object(player, "Error looking up the bug.\n" + details + "\n");
      return ;
   }

   if (!sizeof(details)) {
      tell_object(player, "No bugs found with that id.\n");
      return ;
   }

   bugs = "";
   foreach (complete in details) {
      error = complete->details;
      bugs += "%^BOLD%^#" + error->summary->id + " " +
              ctime(error->summary->entry_date)[4..15] +
              " by " + error->summary->reporter + " (" +
              error->summary->type + " - " + error->summary->category + " - " +
              error->summary->status + ")";
      if (error->summary->assigned_to) {
         bugs += " assigned to " + error->summary->assigned_to;
      }
      bugs += "\n%^RESET%^";
      bugs += error->report;
      if (player->query_creator()) {
         bugs += error->runtime;
      }
   }
   tell_object(player, "$P$Bug #" + error->summary->id + "$P$" + bugs);
}

int cmd_bug(int bug_id) {
   // This looks up the specific bug.
   if (!ERROR_HANDLER->do_query_bug_details(bug_id,
                   (: finish_details($(this_player()), $1, $2) :))) {
      add_failed_mess("Unable to lookup the bug.\n");
      return 0;
   }

   write("Looking up bug, please wait.\n");
   return 1;
}

mixed* query_patterns() {
   return ({ "", (: cmd :),
             "next", (: cmd_next() :),
             "mine", (: cmd_mine(0) :),
             "mine status {open|fixed|considering|denied}", (: cmd_mine($4[0]) :),
             "order {date|directory|filename}", (: cmd_order($4[0]) :),
             "player <string'name'>", (: cmd_ptbugs($4[0], 0) :),
             "player <string'name'> status {open|fixed|considering|denied}", (: cmd_ptbugs($4[0], $4[1]) :),
             "<number>", (: cmd_bug($4[0]) :) });
}
