#include <room/newspaper.h>
#include <http.h>

/*
 * Makes all the '<' turn into $gt; and the '>' the same.
 */
string htmlify(string  str) {
  return replace(str, ({ "<", "&lt;", ">", "&gt;", "\n", "<br>",
                         "&", "&amp;" }));
} /* htmlify() */

string make_header(string title, string paper) {
   return "<html><head><title>" + title + "</title></head>\n"
          "<body bgcolor=#ffffff TEXT=#000030 LINK=#4a529c VLINK=#b57339>\n"
          "<font face=\"arial, helvetica\">\n"
          "<img align=left src=\"" +
                  ("/www/secure/newspaper"->find_logo_url(paper)) + "\">\n"
          "<h2>Discworld MUD</h2>\n"
          "<br clear=both>";
} /* make_header() */

string update_article(string paper, string article, class http_request req) {
   class article* arts;
   string ret;
   string* bottom;
   string office;
   string body;
   string bits;
   int i;

   office = NEWSPAPER_HANDLER->query_paper_office(paper);
   ret = make_header("Broken", paper);
   if (office) {
      // Check and make sure they are an editor first.
      arts = office->query_all_articles();


      if (!office->is_editor(req->user) &&
          !office->is_setter(req->user)) {
         // You can only editor your own articles.
         arts = filter(arts, (: lower_case(((class article)$1)->author) == $2 :), req->user);
      }
      for (i = 0; i < sizeof(arts); i++) {
         if (arts[i]->file_name == article) {
            // Update it onto the main file.
            if (arts[i]->title != req->body->data["title"]) {
               office->set_article_title_by_file(arts[i]->file_name, req->body->data["title"]);
            }
            body = replace_string(req->body->data["body"], "\r\n", "\n");
            office->save_article_text_by_file(arts[i]->file_name, body);
            ret = make_header(arts[i]->title, paper);
            bits = office->load_article_text_by_file(arts[i]->file_name);
            // Show the fluffer in an editable way.
            ret += "<form method=post action=\"editor.c?type=update&paper=" +
                      replace_string(paper, " ", "+") + "&article=" + 
                      arts[i]->file_name + "\">" +
                   "Subject: <input name=title type=text size=60 value=\"" +
                   arts[i]->title + "\"><br>\n"
                   "Shown name: <input name=name type=text size=60 value=\"" +
                   arts[i]->shown_name + "\"><br>\n"
                   "<textarea name=\"body\" rows=20 cols=75>\n" +
                   office->load_article_text_by_file(arts[i]->file_name) +
                   "</textarea><br>\n"
                   "<input type=submit value=\"Update\">\n"
                   "</form>\n";
            if (arts[i]->type == NEWSPAPER_ARTICLE_TYPE_PLAIN) {
               bits = replace_string(bits, "\n\n", "<p>");
               bits = replace_string(bits, "\n", "<br>");
            } else {
               bits = replace_string(bits, "<pre>", "</font><pre>");
            }
         }
      }
   }
   ret += "<hr width=75%>\n";
   ret += "<center><h1>Preview</h1></center>\n" + bits;
   ret += "<hr width=50%>\n";
   bottom = ({ });
   bottom += ({ "<a href=\"editor.c?type=paper&paper=" +
             replace_string(paper, " ", "+") + "\">Contents</a>" });
   ret += "<center><b>[ " + implode(bottom, " | ") + " ]</b></center>\n";
   return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
          "</body></html>";
} /* update_article() */

string show_article(string paper, string article, class http_request req) {
   class article* arts;
   string ret;
   string* bottom;
   string office;
   int i;
   string bits;
   string old_bits;
   string art_type;

   office = NEWSPAPER_HANDLER->query_paper_office(paper);
   ret = make_header("Broken", paper);
   if (office) {
      // Check and make sure they are an editor first.
      arts = office->query_all_articles();
      if (!office->is_editor(req->user) &&
          !office->is_setter(req->user)) {
         // You can only edit your own articles.
         arts = filter(arts, (: lower_case(((class article)$1)->author) == $2 :), req->user);
      }
      for (i = 0; i < sizeof(arts); i++) {
         if (arts[i]->file_name == article) {
            ret = make_header(arts[i]->title, paper);
            bits = office->load_article_text_by_file(arts[i]->file_name);
//tell_creator("pinkfish", "%O %O\n", arts[i]->title, arts[i]->file_name);
            old_bits = bits;
            bits = replace_string(bits, "<", "&lt;");
            bits = replace_string(bits, ">", "&gt;");
            if (!office->is_locked(arts[i])) {
               // Show the fluffer in an editable way.
               ret += "<form method=post action=\"editor.c?type=update&paper=" +
                         replace_string(paper, " ", "+") + "&article=" + 
                         arts[i]->file_name + "\">" +
                   "Subject: <input name=title type=text size=60 value=\"" +
                   arts[i]->title + "\"><br>\n"
                   "Shown name: <input name=name type=text size=60 value=\"" +
                   arts[i]->shown_name + "\"><br>\n"
                   "<textarea name=\"body\" rows=20 cols=75 wrap=hard>\n" +
                   bits +
                   //office->load_article_text_by_file(arts[i]->file_name) +
                   "</textarea><br>\n"
                   "<input type=submit value=\"Update\">\n"
                   "</form>\n";
            } else {
                ret += "Locked and uneditable.\n";
            }
            if (arts[i]->type == NEWSPAPER_ARTICLE_TYPE_PLAIN) {
               old_bits = replace_string(old_bits, "\n\n", "<p>");
               old_bits = replace_string(old_bits, "\n", "<br>");
               art_type = "plain";
            } else {
               art_type = "html";
               old_bits = replace_string(old_bits, "<pre>", "</font><pre>");
               old_bits = replace_string(old_bits, "</pre>",
                                     "</pre>\n<font face=\"helvetica,arial\">");
            }
         }
      }
   }
   ret += "<hr width=75%>\n";
   ret += "<center><h1>Preview (" + art_type + ")</h1></center>\n" + old_bits;
   ret += "<hr width=50%>\n";
   bottom = ({ });
   bottom += ({ "<a href=\"editor.c?type=paper&paper=" +
             replace_string(paper, " ", "+") + "\">Contents</a>" });
   ret += "<center><b>[ " + implode(bottom, " | ") + " ]</b></center>\n";
   return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
          "</body></html>";
} /* show_article() */

string show_paper(string paper, class http_request req) {
   int i;
   string ret;
   string office;
   string up_down;
   class article* arts;
   int editor;
   int setter;

   ret = make_header(paper, paper);
   ret += "<h3>" + paper + "</h3>\n";

   office = NEWSPAPER_HANDLER->query_paper_office(paper);
   if (office) {
      if (office->is_editor(req->user)) {
         ret += "<a href=\"logo_upload.c?paper=" +
                replace_string(paper, " ", "+") + "\">Upload logo</a>";
         editor = 1;
      }
      // Check and make sure they are an editor first.
      arts = office->query_all_articles();
//tell_creator("pinkfish", "%O\n", implode(map(arts, (: $1->file_name :)), ", "));
      if (!office->is_editor(req->user) &&
          !office->is_setter(req->user)) {
         // You can only editor your own articles.
         arts = filter(arts, (: lower_case(((class article)$1)->author) == $2 :), req->user);
      } else {
         setter = 1;
      }
      if (!sizeof(arts)) {
         ret += "No articles for you to edit.\n";
      } else {
         ret += "<ul>\n";
         for (i = 0; i < sizeof(arts); i++) {
            if (arts[i]->colour) {
               ret += "<font color=" + lower_case(arts[i]->colour) + ">";
            }
            if (setter) {
               up_down = "<font size=-5>[ <a href=\"editor.c?type=move_up&paper=" +
                      replace_string(paper, " ", "+") + "&article=" + 
                      arts[i]->file_name + "\">" + "Up</a> | " +
                      "<a href=\"editor.c?type=move_down&paper=" +
                      replace_string(paper, " ", "+") + "&article=" + 
                      arts[i]->file_name + "\">" + "Down</a> ]</font>";
            } else {
               up_down = "";
            }
            switch (arts[i]->type) {
            case NEWSPAPER_ARTICLE_TYPE_SECTION :
               ret += "<center><h4>" + up_down + arts[i]->title + "</h4></center>\n";
               break;
            case NEWSPAPER_ARTICLE_TYPE_HTML :
            case NEWSPAPER_ARTICLE_TYPE_PLAIN :
               ret += "<li>" + up_down + "<a href=\"editor.c?type=article&paper=" +
                      replace_string(paper, " ", "+") + "&article=" + 
                      arts[i]->file_name + "\">" +
                      arts[i]->title + "</a>" +
                      (editor?" by " + arts[i]->author : "") +
                      " shown " + arts[i]->shown_name;
               if (office->is_postponed(arts[i])) {
                  ret += " (postponed)";
               }
               if (office->is_locked(arts[i])) {
                  ret += " (locked)";
               }
               ret += ".\n";
               break;
            }
            if (arts[i]->colour) {
               ret += "</font>";
            }
         }
         ret += "</ul>\n";
      }
   } else {
      ret += "Unable to find the office for the paper.\n";
   }
   return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
          "</body></html>";
} /* show_paper() */

string move_article(string paper, string article, class http_request req, string dir) {
   class article* arts;
   string ret;
   string office;
   int i;

   office = NEWSPAPER_HANDLER->query_paper_office(paper);
   ret = make_header("Broken", paper);
   if (office) {
      // Check and make sure they are an editor first.
      arts = office->query_all_articles();

      if (office->is_editor(req->user)) {
         for (i = 0; i < sizeof(arts); i++) {
            if (arts[i]->file_name == article) {
               office->move_article(i, dir);
            }
         }
      }
   }

   return show_paper(paper, req);
} /* update_article() */

// this is the function which the webserver calls
string www_request(string str, mapping args, class http_request req) {
   string office;
   string ret;

   if (args["paper"]) {
      office = NEWSPAPER_HANDLER->query_paper_office(args["paper"]);
      if (office) {
         // Check and make sure they are an editor first.
         if (!office->is_editor(req->user) &&
             !office->is_setter(req->user)) {
            ret = make_header(args["paper"], args["paper"]);
            ret += "<h1>You must be an editor to edit the paper</h1>\n";
            return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
                    "</body></html>";
         }
      } else {
         ret = make_header(args["paper"], args["paper"]);
         ret += "<h1>You must be an editor to edit the paper</h1>\n";
         return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
                    "</body></html>";
      }
   }

   switch (args["type"]) {
   case "move_up" :
      return move_article(args["paper"], args["article"], req, "up");
   case "move_down" :
      return move_article(args["paper"], args["article"], req, "down");
   case "update" :
      return update_article(args["paper"], args["article"], req);
   case "article" :
      return show_article(args["paper"], args["article"], req);
   case "paper" :
      return show_paper(args["paper"], req);
   default :
      return make_header("Papers", args["paper"]) +
             query_multiple_short(map(NEWSPAPER_HANDLER->query_all_papers(),
                           (: "<a href=\"editor.c?type=paper&paper=" +
                              replace_string($1, " ", "+") +
                              "\">" + $1 + "</a>" :) ));
   }
}
