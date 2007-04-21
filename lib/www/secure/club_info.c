/**
 * This will print out information about a specific club.
 * @author Pinkfish
 * @started Tue Nov  3 18:14:12 PST 1998
 */
#include <clubs.h>
inherit "/std/basic/club_listing";
#include <http.h>

private string create_header(string title) {
  return "<html><head>"
      "<title>" + title + "</title>"+
      "</head><body bgcolor=\"#ffffff\" TEXT=\"#000030\" LINK=\"#4a529c\" "
      "VLINK=\"#b57339\"><font face=\"arial,helvetica\">\n";
} /* create_header() */

string www_request(string str, mapping args, class http_request req) {
   string ret;

   if (args["club_name"] &&
       CLUB_HANDLER->is_club(args["club_name"])) {
         if (CLUB_HANDLER->is_family(args["club_name"])) {
            ret = create_header("Discworld family: " + args["club_name"]);
            ret += www_family_info_string(args["club_name"], req->user);
         } else {
            ret = create_header("Discworld club: " + args["club_name"]);
            ret += www_club_info_string(args["club_name"], 1, req->user);
         }
   } else {
      if (args["club name"]) {
         ret = create_header("Discworld club: "  + args["club_name"]);
         ret += "<h2>The club " + args["club_name"] +
                " is non-existant.</h2>\n";
      } else {
         ret = create_header("Discworld club: Unknown");
         ret += "<h2>Bad argument to the club listing.</h2>";
      }
   }
   ret += "/www/footer"->www_function("footer", ([ ]), req);
   return ret;
} /* www_request() */
