#include <http.h>
#include <playtesters.h>
#include <player_handler.h>

string www_function(string arg, mapping fields, class http_request req) {
   string ret;
   int start;

   ret = "";

   if (PLAYTESTER_HAND->query_playtester(req->user) ||
       PLAYER_HANDLER->test_creator(req->user)) {
      ret += "<td valign=\"top\">\n<font face=\"arial,helvetica\">\n";
      ret += "<h3>Playtesters</h3>\n<ul>\n";
      ret += "<li><a href=\"http://discworld.imaginary.com/twiki/pt/bin/view/Playtesters/WebHome\">Playtesters Wiki</a>\n";
      ret += "<li><a href=\"http://discworld.imaginary.com:5678/~Playtesters/\">Playtesters Webpage</a>\n";
      ret += "<li><a href=\"http://discworld.imaginary.com:5678/~Playtesters/secure/idea.c\">Feedback Centre</a>\n";
      ret += "</ul>\n";
      start = 1;
   }
   if (PLAYER_HANDLER->test_creator(req->user)) {
      if (!start) {
         ret += "<td valign=\"top\">\n<font face=\"arial,helvetica\">\n";
      }
      ret += "<h3>Creator</h3>\n<ul>\n";
      ret += "<li><a href=\"creator/bugs\">Bug statistics</a>\n";
      ret += "<li><a href=\"source_browser.c\">Source browser</a>\n";
      ret += "<li><a href=\"error_query.html\">Error handler</a>\n";
      ret += "<li><a href=\"http://discworld.imaginary.com/cgi-bin/fom_creator\">Creator FAQ</a>\n";
      ret += "<li><a href=\"../lpc_for_dummies/index.htm\">LPC for Dummies</a>\n";
      ret += "<li><a href=\"creator/project.c\">Project Tracker</a>\n";
      ret += "<li><a href=\"http://discworld.imaginary.com/twiki/bin/view/Main/WebHome\">Wiki Project Information</a>\n";
      ret += "</ul>\n";
   }
   if (start) {
      ret += "</td>\n";
   }
   return ret;
} /* www_function() */
