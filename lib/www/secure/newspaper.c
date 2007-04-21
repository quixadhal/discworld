#include <room/newspaper.h>
#include <http.h>
#include <player_handler.h>

mapping _allowed_editions;
mapping _colours;

void create() {
   _allowed_editions = ([ ]);
   _colours = ([ ]);
   _colours["red"] = "#AF0A28";
   _colours["magenta"] = "#550064";
} /* create() */

void reset() {
   _allowed_editions = ([ ]);
} /* reset() */

/**
 * This method returns the path of the logo.
 */
string find_logo_url(string paper) {
   if (stringp(paper)) {
      paper = lower_case(paper);
      paper = replace_string(paper, " ", "_");
      if (file_size("/www/" + NEWSPAPER_WWW_LOGO_BASE + paper + ".gif") > 0) {
         return "http://discworld.imaginary.com/" + NEWSPAPER_WWW_LOGO_BASE +
                 paper + ".gif";
      }
      if (file_size("/www/" + NEWSPAPER_WWW_LOGO_BASE + paper + ".jpg") > 0) {
         return "http://discworld.imaginary.com/" + NEWSPAPER_WWW_LOGO_BASE +
                 paper + ".jpg";
      }
   }
   return "http://discworld.imaginary.com/external/pics/dw5.gif";
} /* find_logo_url() */

int is_allowed_to_read(int edition, string paper, string person) {
   string office;

   if (!person) {
      return 0;
   }

  if (PLAYER_HANDLER->test_creator (person)) {
    return 1;
  }
   if(!_allowed_editions[person])
     _allowed_editions[person] = ([ ]);

   if(!_allowed_editions[person][paper]) {
     _allowed_editions[person][paper] = PLAYER_HANDLER->test_property(person, "Paper " + paper);
     if(!_allowed_editions[person][paper]) {
       _allowed_editions[person][paper] = ({ });
     }
   } else if (find_player(person)) {
     _allowed_editions[person][paper] = find_player(person)->query_property("Paper " + paper);
   }

   if(!_allowed_editions[person][paper])
     _allowed_editions[person][paper] = ({ });
   
   if(member_array(edition, _allowed_editions[person][paper]) != -1) {
     return 1;
   }

   office = NEWSPAPER_HANDLER->query_paper_office(paper);
   if (office) {
      if (office->is_editor(person)) {
         return 1;
      }
   }
   return 0;
} /* is_allowed_to_read() */

/*
 * Makes all the '<' turn into $gt; and the '>' the same.
 */
string htmlify(string  str) {
  return replace(str, ({ "<", "&lt;", ">", "&gt;", "\n", "<br>",
                         "&", "&amp;" }));
} /* htmlify() */

string make_header(string title, string paper) {
   return "<html<head><title>" + title + "</title></head>\n"
          "<body bgcolor=#ffffff TEXT=#000030 LINK=#4a529c VLINK=#b57339>\n"
          "<font face=\"arial, helvetica\">\n"
          "<img align=left src=\"" + find_logo_url(paper) + "\">\n"
          "<h2>Discworld MUD</h2>\n"
          "<br clear=both>";
} /* make_header() */

string show_article(string paper, string edition, string article, class http_request req) {
   class advert* ads;
   class advert ad;
   class article* arts;
   int edt;
   int art_no;
   string ret;
   string* bottom;
   mixed* stuff;

   sscanf(edition, "%d", edt);
   sscanf(article, "%d", art_no);
   // Check the edition stuff now.
   if (!is_allowed_to_read(edt, paper, req->user)) {
      ret = make_header("Unable to read article", paper);
      ret += "<h3>You need to buy this issue in the game before you can read it "
             "on the web.</h3>\n";
      return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
          "</body></html>";
   }

   arts =  NEWSPAPER_HANDLER->query_edition_articles(paper, edt);
   ret = make_header(arts[art_no]->title, paper);
   if (arts[art_no]->type == NEWSPAPER_ARTICLE_TYPE_ADVERTS) {
      ads = NEWSPAPER_HANDLER->query_edition_adverts(paper, edt);
      if (ads) {
         stuff = unique_array(ads, (: $1->category :));
         foreach (ads in stuff) {
            ret += "<center><h1>" + ads[0]->category + "</h1></center>\n";
            //ret += "<dl>\n";
            foreach (ad in ads) {
               ret += "<div align=right><h4><i>" + ad->author +
                      "</i></h4></div><p>\n</font><pre>\n" +
                      sprintf("%-=*s\n", 75, ad->text) +
                      "</pre><font face=\"helvetica,arial\">\n";
               ret += "<hr width=10%>\n";
            }
            //ret += "</dl>\n";
         }
      }
   } else {
      ret += "<h3>" + arts[art_no]->title + "</h3>";
      if (!(arts[art_no]->type & NEWSPAPER_ARTICLE_ANONYMOUS_FLAG)) {
         ret += "by " + arts[art_no]->shown_name + "<p>";
      }
      ret += replace(NEWSPAPER_HANDLER->query_article_text_html(arts[art_no]),
                     ({ "<pre>", "</font><pre>",
                        "</pre>", "</pre><font face=helvetica,arial>" }));
   }
   NEWSPAPER_HANDLER->add_edition_web_hit(paper, edt);
   ret += "<hr width=50%>\n";
   bottom = ({ });
   if (art_no > 0) {
      bottom += ({ "<a href=\"http://discworld.imaginary.com:5678/secure/newspaper.c?type=article&paper=" +
             replace_string(paper, " ", "+") + "&edition=" + edition +
             "&article=" + (art_no - 1) + "\">Previous Page</a>" });
   }
   if (art_no + 1 < sizeof(arts)) {
      bottom += ({ "<a href=\"http://discworld.imaginary.com:5678/secure/newspaper.c?type=article&paper=" +
             replace_string(paper, " ", "+") + "&edition=" + edition +
             "&article=" + (art_no + 1) + "\">Next Page</a>" });
   }
   bottom += ({ "<a href=\"http://discworld.imaginary.com:5678/secure/newspaper.c?type=edition&paper=" +
             replace_string(paper, " ", "+") + "&edition=" + edition +
             "\">Contents</a>" });
   ret += "<center><b>[ " + implode(bottom, " | ") + " ]</b></center>\n";
   return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
          "</body></html>";
} /* show_article() */

string show_edition(string paper, string edition, class http_request req) {
   class article* arts;
   int edt;
   int i;
   string ret;
   string office;
   string* editors;
   string* bottom;
   string owner;

   ret = make_header("Edition " + edition + " of " + paper, paper);
   sscanf(edition, "%d", edt);
   ret += "<h3>" + NEWSPAPER_HANDLER->query_paper_headline(paper, edt) + "</h3>\n";
   ret += "<b><i>Edition " + edition + " of " + paper + "</b></i>\n\n";
   ret += "<ul>\n";
   arts =  NEWSPAPER_HANDLER->query_edition_articles(paper, edt);
   for (i = 0; i < sizeof(arts); i++) {
      if (arts[i]->colour) {
         if (_colours[lower_case(arts[i]->colour)]) {
            ret += "<font color=" + _colours[lower_case(arts[i]->colour)] + ">";
         } else {
            ret += "<font color=" + lower_case(arts[i]->colour) + ">";
         }
      }
      switch (arts[i]->type) {
      case NEWSPAPER_ARTICLE_TYPE_ADVERTS :
         ret += "<h4>";
         ret += "<a href=\"http://discworld.imaginary.com:5678"
               "/secure/newspaper.c?type=article&paper=" +
                replace_string(paper, " ", "+") + "&edition=" + edition +
                "&article=" + i + "\">Adverts</a></h4>\n";
         break;
      case NEWSPAPER_ARTICLE_TYPE_SECTION :
         ret += "<h4>" + arts[i]->title + "</h4>\n";
         break;
      case NEWSPAPER_ARTICLE_TYPE_PLAIN :
      case NEWSPAPER_ARTICLE_TYPE_HTML :
         ret += "<li><a href=\"http://discworld.imaginary.com:5678/secure/newspaper.c?type=article&paper=" +
                replace_string(paper, " ", "+") + "&edition=" + edition +
                "&article=" + i + "\">" + arts[i]->title + "</a>";
         if (!(arts[i]->type & NEWSPAPER_ARTICLE_ANONYMOUS_FLAG)) {
            ret += " by " + arts[i]->shown_name + "\n";
         }
         break;
      }
      if (arts[i]->colour) {
         ret += "</font>";
      }
   }
   ret += "</ul>\n";
   office = NEWSPAPER_HANDLER->query_paper_office(paper);
   if (office) {
      owner = office->query_owner();
      editors = sort_array( uniq_array( office->query_all_editors() ), 1 );
      ret += "<p><i>The current owner is " + 
            "<a href=\"finger.c?player=" + owner + "\">" + owner + "</a>"
            " and the editors are " +
             query_multiple_short(map(editors,
                            (: "<a href=\"finger.c?player=" + $1 + "\">" +
                               $1 + "</a>" :)));
      if (office->query_address()) {
         ret += " and is located at " + office->query_address();
      }
      ret += "</i>\n";
   }
   bottom = ({ });
   bottom += ({ "<a href=\"newspaper.c?type=paper&paper=" +
             replace_string(paper, " ", "+") +
             "\">Editions</a>" });
   if (!req->user) {
      bottom += ({ "<a href=\"http://discworld.imaginary.com:5678/secure/newspaper.c?type=edition&paper=" +
                   replace_string(paper, " ", "+") +
                   "&edition=" + edt + "\">Login</a>" });
   }
   ret += "<hr width=50%>\n"
          "<center><b>[ " + implode(bottom, " | ") + " ]</b></center>\n";
   return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
          "</body></html>";
} /* show_edition() */

string show_paper(string paper, class http_request req) {
   int i;
   int top;
   string ret;
   string bing;
   string office;
   string* editors;
   string* bottom;
   string owner;

   ret = make_header(paper, paper);
   ret += "<h3>" + paper + "</h3>\n";
   top = NEWSPAPER_HANDLER->query_current_edition_num(paper);
   if (NEWSPAPER_HANDLER->query_paper_description(paper)) {
      ret += NEWSPAPER_HANDLER->query_paper_description(paper);
   }

   if (top > 1) {
      ret += "<p><center>Current Issue:<br><b>";
      for (i = top - 1; i >= 1; i--) {
         bing = ctime(NEWSPAPER_HANDLER->query_edition_date(paper, i));
         if (i <= top - 2) {
            if (i == top - 2) {
               ret += "</b></center><hr width=25%><ul>\n";
            }
            ret += "<li>\n";
         }
         ret += "<a href=\"newspaper.c?type=edition&paper=" +
                replace_string(paper, " ", "+") +
                "&edition=" + i + "\">Edition " + i + "</a> <b>" +
                NEWSPAPER_HANDLER->query_paper_headline(paper, i) +
                "</b> <i>" +
                bing[0..10] + bing[20..] +
                "</i>\n";
         if (is_allowed_to_read(i, paper, req->user)) {
            ret += "</b><font size=-2>(bought)</font>\n";
         }
      }
      if (top > 2) {
         ret += "</ul>\n";
      } else {
         ret += "</b></center>\n";
      }
   } else {
      ret += "No editions yet.\n";
   }
   office = NEWSPAPER_HANDLER->query_paper_office(paper);
   if (office) {
      editors = sort_array( uniq_array( office->query_all_editors() ), 1 );
      owner = office->query_owner();
      ret += "<p><i>The current owner is " + 
            "<a href=\"finger.c?player=" + owner + "\">" + owner + "</a>"
            " and the editors are " +
             query_multiple_short(map(editors,
                            (: "<a href=\"finger.c?player=" + $1 + "\">" +
                               $1 + "</a>" :)));
      if (office->query_address()) {
         ret += " and is located at " + office->query_address();
      }
      ret += "</i>\n";
   }
   bottom = ({ });
   bottom += ({ "<a href=\"newspaper.c\">Papers</a>" });
   if (!req->user) {
      bottom += ({ "<a href=\"http://discworld.imaginary.com:5678/secure/newspaper.c?type=paper&paper=" +
                   replace_string(paper, " ", "+") +
                   "\">Login</a>" });
   }
   ret += "<hr width=50%>\n"
          "<center><b>[ " + implode(bottom, " | ") + " ]</b></center>\n";
   return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
          "</body></html>";
} /* show_paper() */

string show_all_papers(class http_request req) {
   string ret;
   string paper;
   int last;
   string bing;

   ret = make_header("All The Discworld Papers", "empty set");
   ret += "<h3>Where papers are seasonal</h3>\n";
   ret += "<ul>\n";
   foreach (paper in NEWSPAPER_HANDLER->query_all_papers()) {
      ret += "<li><a href=\"newspaper.c?type=paper&paper=" +
             replace_string(paper, " ", "+") + "\">" + paper + "</a>\n";
      last = NEWSPAPER_HANDLER->query_last_edition_num(paper);
      if (last) {
          bing = ctime(NEWSPAPER_HANDLER->query_edition_date(paper, last));
          ret += "<i>last edition released at " + bing[0..10] + bing[20..] +
             " (<b>" +
             NEWSPAPER_HANDLER->query_paper_headline(paper, last) +
             "</b>)</i>.\n";
      } else {
         ret += "<i>no editions released yet</i>.\n";
      }
   }
   ret += "</ul>\n";
   return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
          "</body></html>";
} /* show_all_papers() */

// this is the function which the webserver calls
string www_request(string str, mapping args, class http_request req) {
   switch (args["type"]) {
   case "article" :
      return show_article(args["paper"], args["edition"], args["article"], req);
   case "edition" :
      return show_edition(args["paper"], args["edition"], req);
   case "paper" :
      return show_paper(args["paper"], req);
   default :
      return show_all_papers(req);
   }
}

mapping query_allowed_editions() {
   return _allowed_editions;
} /* query_allowed_editions() */
