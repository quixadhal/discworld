/**
 * This will print out a list of all the current clubs or families.
 * @author Pinkfish
 * @started Tue Nov  3 18:14:12 PST 1998
 */
inherit "/std/basic/club_listing";
#include <http.h>
#include <top_ten_tables.h>
#define __CLUBS_H_NO_CLASSES
#include <clubs.h>

private string _family_string;
private string _club_string;

private string create_header(string title) {
  return "<html><head>"
      "<title>" + title + "</title>"+
      "</head><body bgcolor=\"#ffffff\" TEXT=\"#000030\" LINK=\"#4a529c\" "
      "VLINK=\"#b57339\"><font face=\"arial,helvetica\">";
} /* create_header() */

private string club_wrap(string name) {
   if (name) {
      return "<a href=\"club_info.c?club_name=" + name + "\">" + 
             CLUB_HANDLER->query_club_name(name) + "</a>";
   } else {
      return "Unknown";
   }
} /* club_info_wrap() */

private string top_families() {
  string str;
  mapping g_info;
  string guild;
  mixed *stuff;
  string bing;
  string *g_sorted;

  stuff = TOP_TEN_HANDLER->query_family_info();
  str = "<h2>Top families!</h2>\n<ul>\n"
         "<li>The most members " +
         club_wrap(stuff[TOP_TEN_LARGEST_FAMILY])  + ".\n"
         "<li>The oldest average age " +
         club_wrap(stuff[TOP_TEN_OLDEST_FAMILY]) + ".\n"
         "<li>Most average quest points " +
         club_wrap(stuff[TOP_TEN_MOST_QUEST_POINTS]) + ".\n
"
         "<li>Most player killers " +
         club_wrap(stuff[TOP_TEN_MOST_PKS]) + ".\n"
         "<li>Largest single gender " +
         club_wrap(stuff[TOP_TEN_SINGLE_GENDER]) + ".\n"
         "<li>Oldest average logon time " +
         club_wrap(stuff[TOP_TEN_OLDEST_LOGONS]) + ".\n"
         "<li>Most average relationships " +
         club_wrap(stuff[TOP_TEN_MOST_RELATIONSHIPS]) + ".\n";
   g_info = stuff[TOP_TEN_MOST_GUILD];
   g_sorted = sort_array(filter(keys(g_info), (: stringp($1) &&
                                                 file_size($1 + ".c") > 0 &&
                                                 !$1->query_real_guild_object() :)),
                         (: strcmp($1->query_name(), $2->query_name()) :));
   foreach (guild in g_sorted) {
      if (stringp(guild)) {
         bing = g_info[guild];
         str += sprintf("<li>%s %s.\n",
               "Most " + (guild->query_name()),
               club_wrap(bing));
      }
   }
   str += "</ul>\n";
   return str;
} /* top_families() */

private string top_clubs() {
  string str;
  mixed *stuff;
  string guild;
  string bing;
  string *g_sorted;
  mapping g_info;

  stuff = TOP_TEN_HANDLER->query_club_info();
  str = "<h2>Top clubs!</h2>\n<ul>\n"
         "<li>The most members " +
         club_wrap(stuff[TOP_TEN_LARGEST_FAMILY])  + ".\n"
         "<li>The oldest average age " +
         club_wrap(stuff[TOP_TEN_OLDEST_FAMILY]) + ".\n"
         "<li>Most average quest points " +
         club_wrap(stuff[TOP_TEN_MOST_QUEST_POINTS]) + ".\n
"
         "<li>Most player killers " +
         club_wrap(stuff[TOP_TEN_MOST_PKS]) + ".\n"
         "<li>Largest single gender " +
         club_wrap(stuff[TOP_TEN_SINGLE_GENDER]) + ".\n"
         "<li>Oldest average logon time " +
         club_wrap(stuff[TOP_TEN_OLDEST_LOGONS]) + ".\n";
   g_info = stuff[TOP_TEN_MOST_GUILD];
   g_sorted = sort_array(filter(keys(g_info), (: stringp($1) &&
                                                 file_size($1 + ".c") > 0 &&
                                                 !$1->query_real_guild_object() &&
                                                 $1->query_name() != "object" :)),
                         (: strcmp($1->query_name(), $2->query_name()) :));
   foreach (guild in g_sorted) {
      if (stringp(guild)) {
         bing = g_info[guild];
         str += sprintf("<li>%s %s.\n",
               "Most " + (guild->query_name()),
               club_wrap(bing));
      }
   }
   str += "</ul>\n";
   return str;
} /* top_clubs() */

/**
 * Cache the request for an hour...
 */
string www_request(string str, mapping args, class http_request req) {
   string ret;

   switch (args["type"]) {
      case "family" :
         ret = create_header("Current families");
         if (!_family_string) {
            _family_string = top_families();
            _family_string += www_club_list("unknown", 1, args["start"], 2000);
            call_out((: _family_string = 0 :), 120 * 60);
         }
         ret += _family_string;
         break;
      case "club" :
         ret = create_header("Current clubs");
         if (!_club_string) {
            _club_string = top_clubs();
            _club_string += www_club_list("unknown", 0, args["start"], 2000);
            call_out((: _club_string = 0 :), 120 * 60);
         }
         ret += _club_string;
         break;
   }
   ret += "/www/footer"->www_function("footer", ([ ]), req);
   return ret;
} /* www_request() */
