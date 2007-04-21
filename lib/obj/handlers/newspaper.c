/**
 * This is a general newspaper edition handler.  It will deal with keeping
 * track of all the data for all the newspapers on the mud.
 * @author Pinkfish
 * @started Sat Apr 28 02:30:49 PDT 2001
 */
#include <room/newspaper.h>
#include <nroff.h>
#include <autodoc.h>
#include <login_handler.h>

#define SAVE_DIR "/save/newspaper/"


class edition {
   int date_published;
   class article* articles;
   class advert* adverts;
   string headline;
   int num_sold;
   int num_web_hits;
   int total_revenue;
}

class paper {
   class edition* published_editions;
   string dir_name;
   string long;
   string description;
   string office;
   int paper_cost;
   string language;
}

private mapping _papers;
private mapping _areas;
private mapping _xp;
private mapping _inform;

void load_me();
void save_me();

void create() {
   _papers = ([ ]);
   _areas = ([ ]);
   _xp = ([ ]);
   _inform = ([ ]);
   seteuid(getuid());
   load_me();
   if (!_inform) {
      _inform = ([ ]);
   }
} /* create() */

/**
 * This method checks to see if the paper exists.
 * @param paper the paper name to check for
 */
int is_paper(string paper) {
   return classp(_papers[paper]);
} /* is_paper() */

/**
 * This method adds a paper.
 * @param paper the name of the paper
 * @param long the description on the newspapers themselves
 * @param desc the description on the web page
 * @param office the path to the office
 */
void add_paper(string name, string long, string desc, string office,
    string language) {
   if (is_paper(name)) {
      return ;
   }
   _papers[name] = new(class paper);
   _papers[name]->published_editions = ({ });
   _papers[name]->dir_name = replace_string(lower_case(name), " ", "_");
   _papers[name]->long = long;
   _papers[name]->description = desc;
   _papers[name]->office = office;
   _papers[name]->paper_cost = 5 * 4;
   _papers[name]->language = language;
   save_me();
} /* add_paper() */

/**
 * This method returns the list of all the papers.
 * @return all the papers
 */
string* query_all_papers() {
   return keys(_papers);
} /* query_all_papers() */

/**
 * This method returns the current edition of the paper.
 * @param paper the paper to get the edition of
 * @return the current edition
 */
int query_current_edition_num(string paper) {
   if (!is_paper(paper)) {
      return 0;
   }
   return sizeof(_papers[paper]->published_editions) + 1;
} /* query_current_edition_num() */

/**
 * This method returns the last edition of the paper.
 * @param paper the paper to get the edition of
 * @return the last edition, 0 if there are no editions
 */
int query_last_edition_num(string paper) {
   if (!is_paper(paper)) {
      return 0;
   }
   return sizeof(_papers[paper]->published_editions);
} /* query_last_edition_num() */

/**
 * This checks to see if the specified edition is valid or not.
 * @param paper the paper to check
 * @param edition the edition to check
 * @return 1 if it ok, 0 if not
 */
int is_valid_edition(string paper, int edition) {
   if (!is_paper(paper)) {
      return 0;
   }

   if (edition > 0 && edition <= sizeof(_papers[paper]->published_editions)) {
      return 1;
   }
   return 0;
} /* is_valid_edition() */

/**
 * This method returns the date at which the specified issue was
 * published.
 * @param paper the paper to check
 * @param edition the edition to look up
 * @return the date at which the edition was published
 */
int query_edition_date(string paper, int edition) {
   if (!is_paper(paper)) {
      return 0;
   }

   if (!is_valid_edition(paper, edition)) {
      return 0;
   }

   return _papers[paper]->published_editions[edition - 1]->date_published;
} /* query_edition_date() */

/**
 * This method returns the articles in the specified edition of the
 * paper.
 * @param paper the paper to look up
 * @param edition the edition to look up
 * @return the articles in the edition
 */
class article* query_edition_articles(string paper, int edition) {
   if (!is_paper(paper)) {
      return ({ });
   }

   if (!is_valid_edition(paper, edition)) {
      return ({ });
   }

   return copy(_papers[paper]->published_editions[edition - 1]->articles);
} /* query_edition_articles() */

/**
 * This method returns the adverts in the specified edition of the
 * paper.
 * @param paper the paper to look up
 * @param edition the edition to look up
 * @return the adverts in the edition
 */
class advert* query_edition_adverts(string paper, int edition) {
   if (!is_paper(paper)) {
      return ({ });
   }

   if (!is_valid_edition(paper, edition)) {
      return ({ });
   }

   return copy(_papers[paper]->published_editions[edition - 1]->adverts);
} /* query_edition_adverts() */

/**
 * This method returns the office of the paper.
 * @param paper the paper to look up
 * @return the office of the paper
 */
string query_paper_office(string paper) {
   if (!is_paper(paper)) {
      return  0;
   }

   return _papers[paper]->office;
} /* query_paper_headline() */

/**
 * This method sets the office of the paper.
 * @param paper the paper to look up
 * @param office the new office of the paper
 */
void set_paper_office(string paper, string office) {
   if (!is_paper(paper)) {
      return  0;
   }

    _papers[paper]->office = office;
   save_me();
} /* query_paper_headline() */

/**
 * This method returns the headline for the selected edition of the paper.
 * @param paper the paper to look up
 * @param edition the edition to look up
 * @return the headline of the edition
 */
string query_paper_headline(string paper, int edition) {
   if (!is_valid_edition(paper, edition)) {
      return  "No news is good news.";
   }

   return _papers[paper]->published_editions[edition - 1]->headline;
} /* query_paper_headline() */

/**
 * This method returns the headline for the selected edition of the paper.
 * @param paper the paper to look up
 * @param edition the edition to look up
 * @param headline the headline of the issue
 */
void set_paper_headline(string paper, int edition, string headline) {
   if (!is_valid_edition(paper, edition)) {
      return  ;
   }

   _papers[paper]->published_editions[edition - 1]->headline = headline;
   save_me();
} /* set_paper_headline() */

/**
 * This method returns the total revenue for this edition.
 * @param paper the paper to look up
 * @param edition the edition to look up
 * @return the headline of the edition
 */
int query_edition_revenue(string paper, int edition) {
   if (!is_valid_edition(paper, edition)) {
      return  0;
   }

   return _papers[paper]->published_editions[edition - 1]->total_revenue;
} /* query_edition_revenue() */

/**
 * This method adds to the total revenue for this edition.
 * @param paper the paper to look up
 * @param edition the edition to look up
 * @param revenue the revenue to add
 */
void add_edition_revenue(string paper, int edition, int revenue) {
   if (!is_valid_edition(paper, edition)) {
      return  ;
   }

   _papers[paper]->published_editions[edition - 1]->total_revenue += revenue;
   save_me();
} /* add_edition_headline() */

/**
 * This method adds a web hit to the specified edition.
 * @param paper the paper to add a hit on
 * @param edition the edition to add a hit on
 */
void add_edition_web_hit(string paper, int edition) {
   if (!is_valid_edition(paper, edition)) {
      return  ;
   }

   _papers[paper]->published_editions[edition - 1]->num_web_hits++;
   _papers[paper]->published_editions[edition - 1]->total_revenue += NEWSPAPER_WEB_HIT_REVENUE;
   save_me();
} /* add_edition_web_hit() */

/**
 * This method adds a web paper sold to the specified edition.
 * @param paper the paper which was sold
 * @param edition the edition which was sold
 * @param cost the amount it was sold for
 */
void add_edition_paper_sold(string paper, int edition, int cost) {
   if (!is_valid_edition(paper, edition)) {
tell_creator("pinkfish", "Not valid edition %O %O\n", paper, previous_object());
      return  ;
   }

tell_creator("pinkfish", "Updating num sold %O %O\n", paper, edition);
   _papers[paper]->published_editions[edition - 1]->num_sold++;
   _papers[paper]->published_editions[edition - 1]->total_revenue += cost;
   save_me();
} /* add_edition_web_hit() */

/**
 * This method returns the number of papers which have been sold.
 * @param paper the paper which was sold
 * @param edition the edition to check
 * @return the number of papers that have been sold
 */
int query_edition_num_sold(string paper, int edition) {
   if (!is_valid_edition(paper, edition)) {
      return  0;
   }

   return _papers[paper]->published_editions[edition - 1]->num_sold;
} /* query_edition_num_sold() */

/**
 * This method returns the number of web hits for the editon.
 * @param paper the paper which was hit
 * @param edition the edition to check
 * @return the number of web hits on the edition
 */
int query_edition_num_web_hits(string paper, int edition) {
   if (!is_valid_edition(paper, edition)) {
      return  0;
   }

   return _papers[paper]->published_editions[edition - 1]->num_web_hits;
} /* query_edition_num_web_hits() */

/**
 * This method checks to see if the area exists or not.
 * @param area the area to check for
 */
int is_paper_area(string area) {
   return pointerp(_areas[area]);
} /* is_paper_area() */

/**
 * This method adds a paper area.
 * @param area the area to add
 */
void add_paper_area(string area) {
   if (is_paper_area(area)) {
      return ;
   }
   _areas[area] = ({ });
} /* add_paper_area() */

/**
 * This method adds a paper to a paper area.
 * @param paper the paper to add to the area
 * @param area the area to add the paper to
 */
void add_paper_to_area(string paper, string area) {
   if (!is_paper(paper) || !is_paper_area(area)) {
      return ;
   }

   _areas[area] += ({ paper });
   save_me();
} /* add_paper_to_area() */

/**
 * This method returns all the articles in the specified paper area.
 * @param area the area to lookup
 * @return the papers in the area, returns a null array when there are no papers
 */
string* query_papers_in_area(string area) {
   if (_areas[area]) {
      return _areas[area];
   }
   return ({ });
} /* query_papers_in_area() */

/**
 * This moves the data for the article into the correct location.
 * @param article the article to set the text for
 * @param text the text to set
 */
void set_article_text(class article art, string text) {
   unguarded( (:
      write_file(SAVE_DIR + $(art->file_name), save_variable(({ $(art), $(text) })), 1) :) );
} /* set_article_text() */

/**
 * This method adds an object to be informed of a paper being published.
 * When the paper is published, the function event_publish_paper,
 * is called on all the objects in this list.
 * @param paper the paper to be informed about
 * @param ob the object to be informed
 */
void add_newspaper_inform(string paper, string ob) {
   if (!_inform[paper]) {
      _inform[paper] = ({ });
   }
   if (!stringp(ob)) {
      return ;
   }
   if (member_array(ob, _inform[paper]) != -1) {
      return ;
   }
   _inform[paper] += ({ ob });
   save_me();
} /* add_newspaper_infom() */

/**
 * This method removes an object to be informed of a paper being published.
 * @param paper the paper to no longer be informed about
 * @param ob the object to no longer be informed
 */
void remove_newspaper_inform(string paper, string ob) {
   if (!_inform[paper]) {
      return ;
   }
   _inform[paper] -= ({ ob });
   save_me();
} /* remove_newspaper_infom() */

/**
 * This method returns the list of objects that want to be informed about
 * this paper.
 * @param paper the paper to be informed about
 * @return the list of objects to inform
 */
string* query_newspaper_informs(string paper) {
   if (!_inform[paper]) {
      return ({ });
   }
   return _inform[paper];
} /* query_newspaper_informs() */

/**
 * This method reads in the data associated with the specified article.
 * @param article the article to read the text for
 * @return the article text
 */
string query_article_text(class article art) {
   string str;
   string* bits;

   str = unguarded( (: read_file(SAVE_DIR + $(art->file_name)) :) );
   if (str) {
      bits = restore_variable(str);
      return bits[1];
   }
   return "Error reading the article text.";
} /* query_article_text() */

/**
 * This method reads in the data associated with the specified article
 * in html.
 * @param article the article to read the text for
 * @return the article text
 */
string query_article_text_html(class article art) {
   string str;
   string* bits;

   str = unguarded( (: read_file(SAVE_DIR + $(art->file_name)) :) );
   if (str) {
      bits = restore_variable(str);
      switch (art->type & NEWSPAPER_ARTICLE_TYPE_MASK) {
      case NEWSPAPER_ARTICLE_TYPE_PLAIN :
         bits[1] = replace_string(bits[1], "\n\n", "<p>");
         bits[1] = replace_string(bits[1], "\n", "<br>");
         return bits[1];
      case NEWSPAPER_ARTICLE_TYPE_HTML :
         bits[1] = replace_string(bits[1], "<pre>", "</font><pre>");
         bits[1] = replace_string(bits[1], "</pre>",
                                  "</pre>\n<font face=\"helvetica,arial\">");
         return bits[1];
      }
      return bits[1];
   }
   return "Error reading the article text.";
} /* query_article_text_html() */

/**
 * This method changes an input string into a 'nroff' equivilant output
 * string.
 * @param str the string to convert
 */
string convert_html(string input) {
   return AUTODOC_NROFF->convert_html(input);
} /* convert_html() */

/**
 * This method reads in the data associated with the specified article
 * in plain output.
 * @param article the article to read the text for
 * @return the article text
 */
string query_article_text_plain(class article art) {
   string str;
   string fluff;
   string* bits;

   switch (art->type & NEWSPAPER_ARTICLE_TYPE_MASK) {
   case NEWSPAPER_ARTICLE_TYPE_SECTION :
      return "";
   case NEWSPAPER_ARTICLE_TYPE_PLAIN :
      str = unguarded( (: read_file(SAVE_DIR + $(art->file_name)) :) );
      if (str) {
         bits = restore_variable(str);
         return bits[1];
      }
      break;
   case NEWSPAPER_ARTICLE_TYPE_HTML :
      // Check and see if we have an nroff file already.
      fluff = unguarded( (: NROFF_HAND->cat_file(SAVE_DIR + $(art->file_name) + ".nroff", 1) :) );
      if (!fluff) {
         // Make the html output.
         str = unguarded( (: read_file(SAVE_DIR + $(art->file_name)) :) );
         if (str) {
            bits = restore_variable(str);
            bits[1] = convert_html(bits[1]);
            unguarded( (: write_file(SAVE_DIR + $(art->file_name) + ".proc",
                                     $(bits[1]), 1) :) );
            unguarded( (: NROFF_HAND->create_nroff(SAVE_DIR + $(art->file_name) + ".proc",
                                SAVE_DIR + $(art->file_name) + ".nroff") :) );
            str = unguarded( (: NROFF_HAND->cat_file(SAVE_DIR + $(art->file_name) + ".nroff") :) );
            return str;
         }
      } else {
         return fluff;
      }
   }
   return "Error reading the article text.";
} /* query_article_text_plain() */

/**
 * This method returns the long description of the paper.  The long
 * description is used in the newspapers around the place as it's
 * description.
 * @param paper the paper to get the long for
 */
string query_paper_long(string paper) {
   if (!is_paper(paper)) {
      return "This is a very transperant and unhappy newspaper.\n";
   }

   return _papers[paper]->long;
} /* query_paper_long() */

/**
 * This method sets the description of the paper.  The description is
 * what is shown on the web page for the paper.
 * @param paper the paper to set the long for
 * @param long the new long description of the paper
 */
void set_paper_description(string paper, string description) {
   if (!is_paper(paper)) {
      return ;
   }

   _papers[paper]->description = description;
   save_me();
} /* set_paper_description() */

/**
 * This method returns the description description of the paper.  The long
 * description is used in the newspapers around the place as it's
 * description.
 * @param paper the paper to get the long for
 */
string query_paper_description(string paper) {
   if (!is_paper(paper)) {
      return "This is a very transperant and unhappy newspaper.\n";
   }

   return _papers[paper]->description;
} /* query_paper_description() */

/**
 * This method sets the long of the paper.  The long description is
 * used in the newspapers around the place.
 * @param paper the paper to set the long for
 * @param long the new long description of the paper
 */
void set_paper_long(string paper, string long) {
   if (!is_paper(paper)) {
      return ;
   }

   _papers[paper]->long = long;
   save_me();
} /* set_paper_long() */

/**
 * This method sets the cost at which the paper will be sold.
 * @param paper the paper to set the cost of
 * @param cost the new cost of the paper
 */
void set_paper_cost(string paper, int cost) {
   if (!is_paper(paper)) {
      return ;
   }

   _papers[paper]->paper_cost = cost;
   save_me();
} /* set_paper_cost() */

/**
 * This method returns the cost at which the paper will be sold.
 * @param paper the paper to set the cost of
 * @return the cost at which the paper will be sold
 */
int query_paper_cost(string paper) {
   if (!is_paper(paper)) {
      return 0;
   }

   return _papers[paper]->paper_cost;
} /* set_paper_cost() */

/**
 * This method publishes a new edition of the specified paper.
 * @param paper the paper to publish an edition of
 * @param articles the articles in the paper
 * @param text the corresponding text to the articles
 * @param adds the adverts in the paper
 * @return 1 if successful, 0 if not
 */
int publish_paper(string paper,
                  string headline,
                  class article* articles,
                  string* text,
                  class advert* adds,
                  string* editors) {
   string paper_dir;
   string tmp;
   class paper fluff;
   class article article;
   class edition womble;
   int i;
   int edition;
   int total_xp;
   int type;

   if (!is_paper(paper)) {
      return 0;
   }

   fluff = _papers[paper];
   edition = sizeof(fluff->published_editions) + 1;
   paper_dir = SAVE_DIR + fluff->dir_name;
   unguarded( (: mkdir($(paper_dir)) :));
   paper_dir += "/" + edition;
   unguarded( (: mkdir($(paper_dir)) :));

   foreach (article in articles) {
      article->file_name = fluff->dir_name + "/" + edition + "/" +
                           article->file_name;
      type = article->type & NEWSPAPER_ARTICLE_TYPE_MASK;
      // Do not give xp for adds or sections.
      if ((type == NEWSPAPER_ARTICLE_TYPE_PLAIN ||
           type == NEWSPAPER_ARTICLE_TYPE_HTML) &&
           !(article->type & NEWSPAPER_ARTICLE_NO_XP_FLAG)) {
         _xp[lower_case(article->author)] += NEWSPAPER_XP_PER_ARTICLE;
         total_xp += NEWSPAPER_XP_PER_ARTICLE;
      }
   }

   // make it 10%
   total_xp = total_xp * 10 / 100;
   total_xp += NEWSPAPER_XP_EDITOR;
   if (sizeof(editors)) {
      foreach (tmp in editors) {
         _xp[tmp] += total_xp / sizeof(editors);
      }
   }

   // Copy the article data into the relevant place.
   for (i = 0; i < sizeof(articles); i++) {
      set_article_text(articles[i], text[i]);
   }

   womble = new(class edition);
   womble->articles = articles;
   womble->adverts = adds;
   womble->date_published = time();
   womble->headline = headline;
   fluff->published_editions += ({ womble });

   save_me();

   foreach (tmp in query_newspaper_informs(paper)) {
      call_out((: $1->event_publish_paper($2) :), 2, tmp, paper);
   }
   return 1;
} /* publish_paper() */

/** @ignore yes */
void load_me() {
   string paper;
   class paper fluff;
   class paper rabbit;
   //class edition green;
   //class article igloo;
   //int i;

   unguarded( (: restore_object(SAVE_DIR + "main", 1) :) );
   foreach (paper, fluff in _papers) {
      if (sizeof(fluff) == 6) {
         rabbit = new(class paper);
         rabbit->published_editions = fluff->published_editions;
         rabbit->dir_name = fluff->dir_name;
         rabbit->long = fluff->long;
         rabbit->description = fluff->description;
         rabbit->office = fluff->office;
         rabbit->paper_cost = 5 * 4;
         rabbit->language = "general";
         _papers[paper] = rabbit;
         fluff = rabbit;
      } else if( sizeof( fluff ) == 7 ) {
         rabbit = new(class paper);
         rabbit->published_editions = fluff->published_editions;
         rabbit->dir_name = fluff->dir_name;
         rabbit->long = fluff->long;
         rabbit->description = fluff->description;
         rabbit->office = fluff->office;
         rabbit->paper_cost = 5 * 4;
         rabbit->language = fluff->language;
         _papers[paper] = rabbit;
         fluff = rabbit;
      }
/*
      foreach (green in fluff->published_editions) {
         for (i = 0; i < sizeof(green->articles); i++) {
            if (sizeof(green->articles[i]) == 6) {
               igloo = new(class article);
               igloo->file_name = green->articles[i]->file_name;
               igloo->title = green->articles[i]->title;
               igloo->author = green->articles[i]->author;
               igloo->date_written = green->articles[i]->date_written;
               igloo->type = green->articles[i]->type;
               igloo->shown_name = green->articles[i]->shown_name;
               igloo->colour = 0;
               green->articles[i] = igloo;
            }
         }
      }
 */
/*
      for (i = 0; i < sizeof(fluff->published_editions); i++) {
         if (sizeof(fluff->published_editions[i]) == 4) {
            green = new(class edition);
            green->date_published = fluff->published_editions[i]->date_published;
            green->articles = fluff->published_editions[i]->articles;
            green->adverts = fluff->published_editions[i]->adverts;
            green->headline = fluff->published_editions[i]->headline;
            fluff->published_editions[i] = green;
         }
      }
 */
   }
} /* load_me() */

/** @ignore yes */
void save_me() {
   unguarded( (: save_object(SAVE_DIR + "main") :) );
} /* save_me() */

/** @ignore yes */
mixed* stats() {
   string paper;
   string person;
   class paper data;
   mixed* ret;
   int last;
   int xp;

   ret = ({ });
   foreach (paper, data in _papers) {
      last = query_last_edition_num(paper);
      ret += ({ ({ paper + ": office", data->office }),
                ({ paper + ": dir name", data->dir_name }),
                ({ paper + ": last issue", last }),
                ({ paper + ": current issue", query_current_edition_num(paper) }),
                ({ paper + ": num sold", query_edition_num_sold(paper, last) }),
                ({ paper + ": num web hits", query_edition_num_web_hits(paper, last) }),
                ({ paper + ": revenue", query_edition_revenue(paper, last) }),
              });
   }
   foreach (person, xp in _xp) {
      ret += ({ ({ person + ": xp", xp }) });
   }
   return ret;
} /* stats() */

/**
 * @ignore yes
 * This is the login function to check the person for any exciting things like
 * waiting for xp.
 */
void player_login_check(string player, string bing) {
   object play;
   int xp;

   if (bing != LOGIN) {
      return ;
   }

   xp = _xp[player];
   if (xp) {
      play = find_player(player);
      play->adjust_xp(xp);
      map_delete(_xp, player);
      save_me();
      tell_object(play, "%^YELLOW%^You feel like the experience of writing "
                        "an article has made you a better person.\n%^RESET%^");
   }
} /* player_login_check() */

/**
 * This function sets the language for the given paper.
 * @param paper The name of the paper.
 * @param language The language to use.
 */
void set_language_for( string paper, string language ) {
    if( is_paper( paper ) )
        _papers[ paper ]->language = language;
} /* void set_language_for() */


/**
 * This function returns the language for the given newspaper.
 * @param paper The name of the paper.
 * @return The language for the paper, "general" for none or 0 if for error
 */
string query_language_for( string paper ) {
    if( is_paper( paper ) ) {
        if( !undefinedp( _papers[ paper ]->language ) )
            return _papers[ paper ]->language;
        return "general";    
    }
    return 0;
} /* string query_language_for() */


