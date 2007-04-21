#include <config.h>
#include <db.h>

inherit "/cmds/base";

int db;

void finish_request(int type, mixed* data, object person) {
   string ret;
   mapping row;

   if (type == DB_SUCCESS) {
      ret = "";
      foreach (row in data) {
         ret += sprintf("%-15s %s\n", row["Forwarder"], "" + row["bing"]);
      }
   } else {
      ret = "Some sort of horrible error!\n";
   }
   person->more_string(ret, "details");
} /* finish_request() */

int cmd(string who){
   string request;

   if(who == "all") {
      request = "select Forwarder, count(*) as bing from forwards "
                   "where Forwarder is not NULL "
                   "group by Forwarder order by bing desc";
   } else {
      request = "select Forwarder, count(*) as bing from forwards "
                   "where Forwarder = '" + who + "' "
                   "group by Forwarder order by bing desc";
   }

   DB_HANDLER->make_sql_request("errors", CONFIG_DB_USER, "", request,
                                (: finish_request($1, $2, $(this_player())) :));
   add_succeeded_mess("");
   return 1;
}

mixed *query_patterns() {
   return ({ "<string'person|all'>", (: cmd($4[0]) :) });
} /* query_pattern() */
