#include <room/newspaper.h>
#include <http.h>

private mapping _hash;

void create() {
   _hash = ([ ]);
   seteuid(getuid());
}

string make_header(string title, string paper) {
   return "<html><head><title>" + title + "</title></head>\n"
          "<body bgcolor=#ffffff TEXT=#000030 LINK=#4a529c VLINK=#b57339>\n"
          "<font face=\"arial, helvetica\">\n"
          "<img align=left src=\"" + 
                  ("/www/secure/newspaper"->find_logo_url(paper)) + "\">\n"
          "<h2>Discworld MUD</h2>\n"
          "<br clear=both>";
} /* make_header() */

// this is the function which the webserver calls
string www_request(string str, mapping args, class http_request req) {
   string paper;
   string office;

   office = NEWSPAPER_HANDLER->query_paper_office(args["paper"]);
   if (!office || !office->is_editor(req->user)) {
      return make_header("Papers", args["paper"]) +
             "<h1>You are not allowed to update the logo</h1></body></html>";
   }

   switch (args["type"]) {
   case "downloaded" :
      if (_hash[req->user] != args["hash"]) {
          return make_header("Papers", args["paper"]) +
                "<h1>Failure! (" + _hash[req->user] + " -- " + args["hash"] + "</h1></body></html>";
      }
      if (sscanf(_hash[req->user], "%*s====%s", paper) != 0) {
         paper = lower_case(paper);
         paper = replace_string(paper, " ", "_");
         tell_creator("pinkfish", "Uploaded logo ( " + geteuid(this_object()) + " " + paper + ").\n");
         // Figure out the paper from other things.
         unguarded( (: rm("/www/external/newspaper/logos/logo" + $(paper) + ".jpg") :));
         unguarded( (: rm("/www/external/newspaper/logos/logo" + $(paper) + ".gif") :));
         unguarded( (: rename("/www/secure/editors/logo_new" + $(args["ext"]),
                "/www/external/newspaper/logos/logo" + $(paper) + $(args["ext"]) ) :));
         return make_header("Papers", args["paper"]) +
                "<h1>Uploaded the logo</h1>" +
                "<a href=\"/secure/editors/editor.c?type=paper&paper=" +
                args["paper"] + "\">Return the paper listing</a></body></html>";
      }
      break;
   default :
      _hash[req->user] = random(100000) + "====" + args["paper"];
      if (!write_file("/net/save/newspaper_logo_hash_value", "" + _hash[req->user], 1)) {
          return make_header("Papers", args["paper"]) +
                "<h1>Failure!</h1></body></html>";
      }
      return make_header("Papers", args["paper"]) +
             "<form method=post enctype=\"multipart/form-data\" action=\"http://discworld.imaginary.com/cgi-bin/newspaper/cgiupload.py\">\n" +
             "<input name=file type=file>\n"
             "<input name=hash type=hidden value=\"" + _hash[req->user] + "\">\n"
             "<input name=paper type=hidden value=\"" + args["paper"] + "\">\n"
             "<input name=igloo type=submit>\n</form></body></html>";
   }
}
