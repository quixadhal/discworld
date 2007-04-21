#include <http.h>
#include <clubs.h>
#include <player_handler.h>

string www_function(string fname, mapping fields, class http_request req) {
   string ret;
   string *clubs;
   string bing;

   ret = "";
   if (req->user) {
      if (PLAYER_HANDLER->test_property(req->user, "philosopher")) {
         ret += "<h3><a href=\"boards.c?board=philosophers\">"
            "<font face=\"arial, helvetica\">Philosophers</a></h3>\n";
      }
      clubs = CLUB_HANDLER->query_boards();
      foreach (bing in clubs) {
         if (CLUB_HANDLER->is_member_of(bing, req->user)) {
            ret += "<h3><a href=\"boards.c?board=" +
               CLUB_HANDLER->query_club_board_name(bing) + "\">"
               "<font face=\"arial, helvetica\">" +
               CLUB_HANDLER->query_club_name(bing) + "</a></h3>\n";
         }
      }

      ret += "<h3><a href=\"boards.c?board=flame\">"
         "<font face=\"arial, helvetica\">Flame</a></h3>\n";
      ret += "<h3><a href=\"boards.c?board=fluff\">"
         "<font face=\"arial, helvetica\">Fluff</a> for all your happy feelings</h3>\n";
      ret += "<h3><a href=\"boards.c?board=never+wending+story\">"
         "<font face=\"arial, helvetica\">Never wending story</a></h3>\n";
      ret += "<h3><a href=\"boards.c?board=alt.fan.pratchett\">"
         "<font face=\"arial, helvetica\">alt.fan.pratchett</a></h3>\n";
      ret += "<h3><a href=\"boards.c?board=equality\">"
         "<font face=\"arial, helvetica\">Equality</a></h3>\n";
   }
   return ret;
} /* www_function() */
