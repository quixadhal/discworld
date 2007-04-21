/*  -*- LPC -*-  */
/*
 * $Locker: pinkfish $
 * $Id: newspaper_office.c,v 1.52 2003/05/23 22:26:02 wirble Exp pinkfish $
 *
 *
 */
/**
 * An office from which a player run newspaper can be run.
 * @author Obilix
 * @changed Pinkfish Fri Apr 27 12:08:29 PDT 2001
 * Turned into an inherit and made to use a newspaper handler for
 * distributing the actual text.
 */

#include <mail.h>
#include <money.h>
#include <move_failures.h>
#include <player_handler.h>
#include <room/newspaper.h>
#include <nroff.h>
#include <board.h>
#include <housing.h>

#define NEWSPAPER_POSTPONE_FLAG 1
#define NEWSPAPER_LOCK_FLAG 2

class our_article {
   class article art;
   int flags;
   int length;
   int suggested_payment;
}

class reporter {
   int date_added;
   int num_articles;
   int total_payed;
}

class category_data {
   int cost_per_add;
   int cost_per_ten_chars;
   int open;
}

class payee_data {
   int paid;
   string message;
}

private mapping _reporters;
private string* _editors;
private string* _setters;
private string* _sponsors;
private string _owner;
private mapping _payees;
private int _published_time;
private class our_article* _articles;
private class advert* _adverts;
private int _next_article_num;
private int _edition_number;
private int _locked;
private int _owner_extra_cash;
private mapping _category;
private nosave string _save_dir;
private nosave string _paper_name;
private string _bank_branch;
private nosave string _proxy;

int show_article(class our_article art, int source);
string* query_all_reporters();
string* query_all_setters();
string* query_all_editors();
string* query_all_sponsors();
mapping query_all_payees();
int is_reporter(string word);
int is_setter(string word);
int is_editor(string word);
int is_sponsor(string word);
void add_reporter(string word);
void add_setter(string word);
void add_editor(string word);
void add_sponsor(string word);
void remove_reporter(string word);
void remove_setter(string word);
void remove_editor(string word);
void remove_sponsor(string word);
void update_commands(string word);
string query_log_file_name();
string query_article_log_file_name();
int query_current_edition();
string query_paper_name();
string query_next_article_file_name();
string query_bank_branch();
mixed query_property(string);
string query_owner();
int is_owner(string word);

void create()
{
   _reporters = ([ ]);
   _editors = ({ });
   _payees = ([ ]);
   _articles = ({ });
   _category = ([ ]);
   _adverts = ({ });
   _setters = ({ });
   _sponsors = ({ });
   _next_article_num = 2;
}                               /* setup() */

/**
 * Saves the data in the file.
 */
protected void save_me()
{
   if (_save_dir && !_proxy) {
      unguarded( (: save_object(_save_dir + "main") :) );
   }
}                               /* save_me() */

/**
 * Loads the data in the file.
 */
protected void load_me()
{
   class article fluff;
   int i;

printf("%O %O\n", _save_dir, _proxy);
   if (_save_dir && !_proxy) {
      unguarded( (: restore_object(_save_dir + "main", 1) :) );
printf("%O %O\n", _save_dir, _proxy);
      if (!_articles) {
         _articles = ({ });
      }
      if (!_setters) {
         _setters = ({ });
      }
      if (!_sponsors) {
         _sponsors = ({ });
      }
      if (!_adverts) {
         _adverts = ({ });
      }
      for (i = 0; i < sizeof(_articles); i++) {
         if (sizeof(_articles[i]->art) == 6) {
            fluff = new(class article);
            fluff->file_name = _articles[i]->art->file_name;
            fluff->title = _articles[i]->art->title;
            fluff->author = _articles[i]->art->author;
            fluff->date_written = _articles[i]->art->date_written;
            fluff->type = _articles[i]->art->type;
            fluff->shown_name = _articles[i]->art->shown_name;
            fluff->colour = 0;
            _articles[i]->art = fluff;
         }
      }
   }
}                               /* save_me() */

private int a_type(int data) {
   return data & NEWSPAPER_ARTICLE_TYPE_MASK;
}
private int a_flags(int data) {
   return data & ~NEWSPAPER_ARTICLE_TYPE_MASK;
}

/**
 * This method sets the directory we are to use for all the
 * temporary files and data.  The actual pubished data will be stored
 * by the newspaper handler.
 * @param dir the directory to put stuff in
 */
void set_save_directory(string dir) {
   if (dir[<1] != '/') {
      dir += "/";
   }
   _save_dir = dir;
   load_me();
} /* set_save_directory() */

/**
 * This method saves an article.
 */
void save_article_text(class our_article art, string text) {
   if (a_type(art->art->type) != NEWSPAPER_ARTICLE_TYPE_HTML &&
       a_type(art->art->type) != NEWSPAPER_ARTICLE_TYPE_PLAIN) {
      return ;
   }
   unguarded( (:
       write_file(_save_dir + $(art->art->file_name), save_variable(({ $(art), $(text) })), 1) :) );
   unguarded( (: rm(_save_dir + $(art->art->file_name) + ".proc" ) :) );
   unguarded( (: rm(_save_dir + $(art->art->file_name) + ".nroff.o" ) :) );
} /* save_article_text() */

/**
 * THis method updates any information about the article if needed.
 */
private void update_article_info(class our_article art, string text) {
   int fsize;

   fsize = strlen(text);
   art->length = fsize;
   if (fsize < 0) {
      fsize = 0;
   }
   if (fsize > 4000) {
      fsize = 4000;
   }
   fsize = fsize / 100;
   art->suggested_payment = fsize;
} /* update_article_info() */

/**
 * This method loads the text of an article.
 */
string load_article_text(class our_article art) {
   mixed* bits;
   string str;


   str = unguarded( (: read_file(_save_dir + $(art->art->file_name)) :) );
   if (str) {
      bits = restore_variable(str);
      return bits[1];
   }
   return "Error loading the article.";
} /* load_article_text() */

/**
 * This method is used by the web, it loads the article by the file name.
 */
string load_article_text_by_file(string fname) {
   int i;

   for (i = 0; i < sizeof(_articles); i++) {
      if (_articles[i]->art->file_name == fname) {
         return load_article_text(_articles[i]);
      }
   }
   return "Lost the article.\n";
} /* load_article_text_by_file() */

/**
 * This method is used by the web, it loads the article by the file name.
 */
void save_article_text_by_file(string fname, string text) {
   int i;

   for (i = 0; i < sizeof(_articles); i++) {
      if (_articles[i]->art->file_name == fname) {
         //_articles[i]->art->length = strlen(text);
         update_article_info(_articles[i], text);
         return save_article_text(_articles[i], text);
      }
   }
    return ;
} /* load_article_text_by_file() */

/**
 * This method is used by the web, it loads the article by the file name.
 */
void set_article_title_by_file(string fname, string title) {
   int i;

   for (i = 0; i < sizeof(_articles); i++) {
      if (_articles[i]->art->file_name == fname) {
         _articles[i]->art->title = title;
         save_me();
         return ;
      }
   }
    return ;
} /* load_article_text_by_file() */

/**
 * This method deletes any bitsof the article off the disk.
 * @param article the article to delete
 */
void delete_article(class our_article article) {
   unguarded( (: rm(_save_dir + $(article->art->file_name)) :) );
   unguarded( (: rm(_save_dir + $(article->art->file_name) + ".proc" ) :) );
   unguarded( (: rm(_save_dir + $(article->art->file_name) + ".nroff.o" ) :) );
} /* delete_article() */

/**
 * This method returns the index of the article in the article array.
 * @return the article index
 */
int query_article_index(string match) {
   int index;

   match = lower_case(match);
   if (strlen(match) && (match[0] >= '0' && match[0] <= '9')) {
      sscanf(match, "%d", index);
      index--;
      if (index < 0 || index >= sizeof(_articles)) {
         return -1;
      }
      return index;
   }
   if (strlen(match) == 1) {
      index = match[0] - 'a';
      if (index < 0 || index >= sizeof(_articles)) {
         return -1;
      }
      return index;
   }
   return -1;
} /* query_article_index() */

class our_article* query_articles() {
   return _articles;
}

/**
 * This method finds an article based on a string.
 * @return the article
 */
class our_article query_article(string match) {
   int index;

   index = query_article_index(match);
   if (index == -1) {
      return 0;
   }
   return _articles[index];
} /* query_article() */

/**
 * This method tries to find a category based on some fuzzy matching.
 * @param category the category to lookup
 * @return the fixed up category name
 */
string query_category(string category) {
   string name;

   category = lower_case(category);
   foreach (name in keys(_category)) {
      if (lower_case(name) == category ||
          lower_case(name)[0..strlen(category) - 1] == category) {
         return name;
      }
   }
   return 0;
} /* query_category() */

/**
 * The file name of the log file for events.
 * @return the log file name
 */
string query_log_file_name() {
   return _save_dir + "event.log";
} /* query_log_file_name() */

/**
 * The file name of the log file for article events.
 * @return the article log file name
 */
string query_article_log_file_name() {
   return _save_dir + "aritcle.log";
} /* query_article_log_file_name() */

/**
 * This method returns the current edition of the newspaper./
 * @return the current edition of the newspaper
 */
int query_current_edition()
{
   return NEWSPAPER_HANDLER->query_current_edition_num(query_paper_name());
}

/**
 * This method returns the name of the paper.
 * @return the name of the paper
 */
string query_paper_name()
{
   return _paper_name;
}

/**
 * This method returns the name of the paper with a 'the' in front.
 * @return the name of the paper
 */
string query_the_paper_name()
{
   if (lower_case(_paper_name[0..3]) == "the") {
      return _paper_name;
   }
   return "the " + _paper_name;
}

/**
 * This method sets the name of the paper.
 * @param name the new name of the paper
 */
string set_paper_name(string name)
{
   _paper_name = name;
}

/**
 * This returns the next number to use for an article.
 * @return the next article number
 */
string query_next_article_file_name()
{
   while (file_size(_save_dir + _next_article_num + ".txt") != -1) {
      _next_article_num++;
   }
   return (_next_article_num++) + ".txt";
}

/**
 * This method returns the bank branch to use for depositing money for
 * editions.
 * @return the bank branch to use
 */
string query_bank_branch()
{
   return _bank_branch;
}

/**
 * This method sets the bank branch to use for depositing money when
 * editions are made.
 * @param bank the bank branch to use
 */
void set_bank_branch(string bank) {
   _bank_branch = bank;
} /* set_bank_branch() */

/** @ignore yes */
int do_apply()
{
   if (_proxy) {
      return _proxy->do_apply();
   }
   tell_object(this_player(), "To apply for a job at " + query_the_paper_name() + ", please write "
               "us a letter outlining your areas of interest and what you think you "
               "would be able to contribute to the newspaper.  It will be sent to the "
               "editors of " + query_the_paper_name() +
               ", who will get back to you with an answer "
               "within a few days.\n");
   this_player()->do_edit(0, "mail_application");
   add_succeeded_mess(({ "", "$N start$s writing an application.\n" }));
   return 1;
}                               /* do_apply */

/** @ignore yes */
int mail_application(string text)
{
   string word;
   string *eds;

   if (!text) {
      tell_object(this_player(), "You decide not to apply just yet.\n");
      return 0;
   }
   eds = ({ });
   foreach(word in query_all_editors()) {
      if (PLAYER_HANDLER->test_user(word)) {
         eds += ({ word });
      }
   }
   if (!sizeof(eds)) {
      eds = ({ "pinkfish" });
   }
   AUTO_MAILER->auto_mail(implode(eds, ","), this_player()->query_name(),
                          "" + query_paper_name() + " application", "", text, 0, 0);
   log_file(query_log_file_name(),
                 "On " + ctime(time()) + " " + this_player()->query_short() +
                 " applied for a job at " + query_the_paper_name() + ".\n");
   tell_object(this_player(), "Your application has been submitted.  Thanks!\n");
   tell_room(this_object(),
             this_player()->the_short() + " finishes " + this_player()->query_possessive() +
             " application for a job at " + query_the_paper_name() + ".\n", this_player());
   return 1;
}                               /* mail_application */

/** @ignore yes */
int part_two(string str, string author, int html)
{
   if (!sizeof(str)) {
      tell_object(this_player(), "The article must have a name!\n");
      tell_room(this_object(), this_player()->the_short() +
                " stops writing to think of a name.\n",
                this_player());
      return 0;
   }
   if (!html) {
      tell_object(this_player(), "Please enter the text of your story.  It will make "
               "everyone's life much easier if it is less than 75 characters per "
               "line!\n\n"
               "%^BOLD%^<--------+10-------+20-------+30-------+40-------+50-------+60-"
               "------+70-->%^RESET%^\n");
   }
   this_player()->do_edit(0, "write_it", this_object(), 0, ({ str, author, html }));
   return 1;
}                               /* part_two() */

/** @ignore yes */
int do_submit(string str, string author, int html)
{
   if (_proxy) {
      return _proxy->do_submit(str, author, html);
   }
   if (_locked) {
      add_failed_mess("The newspaper office is currently locked for "
                      "submissions.\n");
      return 0;
   }

   if (!str) {
      add_failed_mess("Please enter the name of the article you wish "
                       "to submit.  A short but descriptive name would be good, and it will "
                       "automagically have your name appended to the end of it.\n");
      return 0;
   }
   if (!author) {
      author = this_player()->query_cap_name();
   }
   add_succeeded_mess(({ "", "$N start$s writing an article.\n" }));
   part_two(str, author, html);
   return 1;
}                               /* do_submit */

/** @ignore yes */
varargs int write_it(string article,
                     mixed *stuff,
                     class our_article cur_art)
{
   int fsize;
   string person;
   string title;
   string author;
   int html;

   title = stuff[0];
   author = stuff[1];
   html = stuff[2];

   if (!article) {
      tell_object(this_player(), "You decide not to submit your story after all.\n");
      tell_room(this_object(), this_player()->the_short() +
                    " stops writing the article.\n", this_player());
      return 0;
   }
   person = this_player()->query_cap_name();
   if (!cur_art) {
      cur_art = new(class our_article);
      cur_art->art = new(class article);
      cur_art->art->title = title;
      cur_art->art->shown_name = author;
      cur_art->art->file_name = query_next_article_file_name();
      cur_art->art->author = person;
      cur_art->art->date_written = time();
      if (html) {
         cur_art->art->type = NEWSPAPER_ARTICLE_TYPE_HTML;
      } else {
         cur_art->art->type = NEWSPAPER_ARTICLE_TYPE_PLAIN;
      }
      update_article_info(cur_art, article);
      cur_art->flags = 0;
   }

   _articles += ({ cur_art });
   save_article_text(cur_art, article);
   save_me();
   log_file(query_article_log_file_name(),
                 "On " + ctime(time()) + " " + person +
                 " submitted the story " + title + ".\n");
   log_file(query_article_log_file_name(), "Recommended payment for above "
                           "article: $" + fsize + ".\n");
   tell_object(this_player(), "Article submitted!\n");
   tell_room(this_object(), this_player()->the_short() +
              " submits an article to the newspaper.\n",
             this_player());
}                               /* write_it() */

/** @ignore yes */
int do_retrieve(string edn, int source)
{
   class our_article article;

   if (_proxy) {
      return _proxy->do_retrieve(edn, source);
   }
   add_succeeded_mess("");
   if (!sizeof(_articles)) {
      add_failed_mess("There do not seem to have be any articles "
                      "submitted for this edition yet.\n");
      return 0;
   }
   article = query_article(edn);
   if (!article) {
      add_failed_mess("Sorry, " + edn + " is an incorrect article "
                      "reference.\n");
      return 0;
   }
   if (!is_editor(this_player()->query_name()) &&
       !is_setter(this_player()->query_name()) &&
       lower_case(article->art->author) != this_player()->query_name()) {
      add_failed_mess("You can only read your own articles.\n");
      return 0;
   }
   show_article(article, source);
   return 1;
}                               /* do_retrieve */

/** @ignore yes */
int show_article(class our_article article, int source)
{
   string text;
   mixed* bits;
   int i;
   mixed* stuff;
   class advert data;

   switch (a_type(article->art->type)) {
   case NEWSPAPER_ARTICLE_TYPE_SECTION :
      text = "%^BOLD%^" + article->art->title + "%^RESET%^\n\n";
      for (i = 0; i < sizeof(_articles); i++) {
         if (_articles[i] == article) {
            i++;
            break;
         }
      }
      for (; i < sizeof(_articles); i++) {
         if (a_type(_articles[i]->art->type) != NEWSPAPER_ARTICLE_TYPE_SECTION) {
            text += sprintf("%2d .... %70-=s\n", i + 1,
                            _articles[i]->art->title);
         }
      }
      break;
   case NEWSPAPER_ARTICLE_TYPE_ADVERTS :
      text = "%^BOLD%^" + article->art->title + "%^RESET%^\n";
      stuff = unique_array(_adverts, (: $1->category :));
      foreach (bits in stuff) {
         text += "\n$I$0=" + bits[0]->category + "\n$I$3=   ";
         foreach (data in bits) {
            text += data->text + "\n-- " + data->author + "\n\n";
         }
      }
      break;
   case NEWSPAPER_ARTICLE_TYPE_HTML :
      if (!source) {
         // Do evil banana things.
         text = unguarded( (: NROFF_HAND->cat_file(_save_dir +
                              $(article->art->file_name) + ".nroff", 1) :) );
         if (!text) {
            // Make the html output.
            text = unguarded( (: read_file(_save_dir +
                                           $(article->art->file_name)) :) );
            if (text) {
               bits = restore_variable(text);
               bits[1] = NEWSPAPER_HANDLER->convert_html(bits[1]);
               unguarded( (: write_file(_save_dir + $(article->art->file_name) +
                              ".proc", $(bits[1]), 1) :) );
               if (unguarded( (: NROFF_HAND->create_nroff(_save_dir +
                               $(article->art->file_name) + ".proc",
                           _save_dir + $(article->art->file_name) + ".nroff") :) )) {
                  text = unguarded( (: NROFF_HAND->cat_file(_save_dir +
                                   $(article->art->file_name) + ".nroff") :) );
               } else {
                  text = "Unable to make nroff file.\n";
               }
            } else {
               text = "Unable to read the article.  Opps.\n";
            }
         }
         break;
      }
   case NEWSPAPER_ARTICLE_TYPE_PLAIN :
      text = load_article_text(article);
      break;
   }
   this_player()->more_string("The article you requested reads as follows:\n" +
                   text + "\n");
   return 1;
}                               /* show_article */

/** @ignore yes */
int do_logs(int article_log)
{
   string logfile;

   if (_proxy) {
      return _proxy->do_logs(article_log);
   }
   if (article_log) {
      logfile = read_file(query_article_log_file_name());
   } else {
      logfile = read_file(query_log_file_name());
   }
   if (!logfile) {
      add_failed_mess("There don't appear to be any logs.\n");
      return 0;
   }
   this_player()->more_string("The log contains:\n" + logfile + "\n");
   add_succeeded_mess("");
   return 1;
}                               /* do_logs() */

/** @ignore yes */
int do_articles(int scroll)
{
   string str;
   class our_article article;
   int pos;
   int editor;

   if (_proxy) {
      return _proxy->do_articles(scroll);
   }
   str = "";
   editor = is_editor(this_player()->query_name());
   if (sizeof(_articles)) {
      str = "The articles for edition " + query_current_edition() + " are:\n";
      pos = 1;
      foreach(article in _articles) {
         if (article->art->colour) {
            str += "%^" + article->art->colour + "%^";
         }
         switch (a_type(article->art->type)) {
         case NEWSPAPER_ARTICLE_TYPE_HTML :
         case NEWSPAPER_ARTICLE_TYPE_PLAIN :
            str += sprintf("$I$5=Page %2d) %s by %s (shown author %s) submitted at %s; length %d%s%s%s\n",
               pos, article->art->title,
               (editor || article->art->author == this_player()->query_name()?article->art->author:article->art->shown_name),
               article->art->shown_name,
               ctime(article->art->date_written),
               article->length,
               (editor?" suggested payment $" + article->suggested_payment:""),
               (article->flags & NEWSPAPER_POSTPONE_FLAG?" (postponed)":""),
               (article->flags & NEWSPAPER_LOCK_FLAG?" (locked)":""),
               (a_flags(article->art->type) & NEWSPAPER_ARTICLE_NO_XP_FLAG?" (no xp)":"") );
            break;
         case NEWSPAPER_ARTICLE_TYPE_SECTION :
            str += sprintf("$I$5=Page %2d) Section Header '%s'\n",
                           pos,
                           article->art->title);
            break;
         case NEWSPAPER_ARTICLE_TYPE_ADVERTS :
            str += sprintf("$I$5=Page %2d) Adverts\n", pos);
            break;
         }
         if (article->art->colour) {
            str += "%^RESET%^";
         }
         pos++;
      }
   } else {
      str = "There are no articles in this edition yet.\n";
   }
   if (!scroll) {
      write("$P$Articles$P$" + str);
   } else {
      write(str);
   }
   return 1;
}                               /* do_logs() */

/** @ignore yes */
int do_resign()
{
   object *inv;
   object ob;
   string person;

   if (_proxy) {
      return _proxy->do_resign();
   }
   person = this_player()->query_name();
   remove_reporter(person);
   if (is_editor(person)) {
      remove_editor(person);
   }
   if (is_setter(person)) {
      remove_setter(person);
   }
   log_file(query_log_file_name(),
                 "On " + ctime(time()) + " " + this_player()->query_short() +
                 " resigned from " + query_the_paper_name() + ".\n");
   inv = match_objects_for_existence("press credentials", ({ this_player() }),
                                     this_player());
   foreach (ob in inv) {
      if (ob->query_paper_name() == query_paper_name()) {
         ob->dest_me();
      }
   }
   add_succeeded_mess(({ "You hand in your badge, and tender your "
                         "resignation from " + query_the_paper_name() +
                         ".  You can't help but feel a little "
                         "saddened as a result.\n",
                         "$N $V from " + query_the_paper_name() + ".\n" }));
   return 1;
}                               /* do_resign() */

/** @ignore yes */
int do_delete(string str)
{
   class our_article article;
   int i;

   if (_proxy) {
      return _proxy->do_delete(str);
   }
   article = query_article(str);
   if (!article) {
      add_failed_mess("There is no article referenced by " + str + ".\n");
      return 0;
   }

   if (!is_editor(this_player()->query_name()) &&
       this_player()->query_name() != lower_case(article->art->author)) {
      add_failed_mess("You cannot delete this article.\n");
      return 0;
   }

   delete_article(article);
   for (i = 0; i < sizeof(_articles); i++) {
      if (_articles[i] == article) {
         _articles = _articles[0..i - 1] + _articles[i+1..];
      }
   }
   save_me();

   log_file(query_article_log_file_name(),
                    "On " + ctime(time()) + " " + this_player()->query_short() +
                    " deleted the article " + article->art->title + ".\n");
   add_succeeded_mess(({ "Article deleted!\n",
                         "$N $V an article from "
                         "the current edition.\n" }));
   return 1;
}                               /* do_delete */

/** @ignore yes */
int do_hire(string person, int setter)
{
   string bing;
   object thatguy;

   if (_proxy) {
      return _proxy->do_hire(person, setter);
   }
   person = lower_case(person);
   if (setter) {
      if (is_setter(person)) {
         add_failed_mess("That person is already employed as a setter by " +
                         query_the_paper_name() + "!\n");
         return 0;
      }
      bing = "setter";
   } else {
      if (is_reporter(person)) {
         add_failed_mess("That person is already employed by " +
                         query_the_paper_name() + "!\n");
         return 0;
      }
      bing = "reporter";
   }
   if (!PLAYER_HANDLER->test_user(person)) {
      add_failed_mess("There is nobody called " + capitalize(person) + "!\n");
      return 0;
   }
   thatguy = find_player(person);
   if (thatguy) {
      tell_object(thatguy, "You have "
                  "just been hired by " + this_player()->the_short() +
                  " as a " + bing + " for " +
                  query_the_paper_name() + ".\n");
   }
   if (setter) {
      add_setter(person);
   } else {
      add_reporter(person);
   }
   log_file(query_log_file_name(),
                 "On " + ctime(time()) + " " + this_player()->query_short() +
                 " employed " + capitalize(person) + " as a " + bing + ".\n");
   add_succeeded_mess(capitalize(person) +
                      " is now employed by " +
                      query_the_paper_name() + " as a " + bing + ".\n");
   return 1;
}                               /* do_hire() */

/** @ignore yes */
int do_dismiss(string person)
{
   object thatguy;

   if (_proxy) {
      return _proxy->do_dismiss(person);
   }
   if (!is_reporter(person)) {
      add_failed_mess("That person is not employed by " +
                      query_the_paper_name() + "!\n");
      return 0;
   }
   remove_reporter(person);
   remove_editor(person);
   remove_setter(person);
   thatguy = find_player(person);
   if (thatguy) {
      tell_object(thatguy, "You have "
                  "been fired from " + query_the_paper_name() + " by " + this_player()->the_short() +
                  "!\n");
   }
   log_file(query_log_file_name(),
                 "On " + ctime(time()) + " " + this_player()->query_short() +
                 " fired " + capitalize(person) + ".\n");
   add_succeeded_mess(capitalize(person) +
                      " is no longer employed by " +
                      query_the_paper_name() + ".\n");
   return 1;
}                               /* do_dismiss() */

/** @ignore yes */
int do_sponsor_add(string person)
{
   if (_proxy) {
      return _proxy->do_sponsor_add(person);
   }
   person = lower_case(person);
   if (is_sponsor(person)) {
      add_failed_mess(person + " is already a sponsor of " +
                      query_the_paper_name() + "!\n");
      return 0;
   }

   if (!PLAYER_HANDLER->test_user(person)) {
      add_failed_mess("I am sorry, but " + person + " does not exist.\n");
      return 0;
   }

   add_sponsor(person);
   add_succeeded_mess(capitalize(person) +
                      " is now a sponsor of " + query_the_paper_name() + ".\n");
   return 1;
}                               /* do_sponsor_add() */

/** @ignore yes */
int do_sponsor_remove(string person)
{
   if (_proxy) {
      return _proxy->do_sponsor_remove(person);
   }
   person = lower_case(person);
   if (!is_sponsor(person)) {
      add_failed_mess(person + " is not a sponsor of " +
                      query_the_paper_name() + "!\n");
      return 0;
   }

   remove_sponsor(person);
   add_succeeded_mess(capitalize(person) +
                      " is not a sponsor of " +
                      query_the_paper_name() + " anymore.\n");
   return 1;
}                               /* do_sponsor_remove() */

/** @ignore yes */
int do_promote(string person)
{
   object thatguy;

   if (_proxy) {
      return _proxy->do_promote(person);
   }
   person = lower_case(person);
   if (!is_reporter(person)) {
      add_failed_mess("There is nobody by that name currently employed at " +
                      query_the_paper_name() + "!\n");
      return 0;
   }
   if (is_editor(person)) {
      add_failed_mess(capitalize(person) + " is already an editor!\n");
      return 0;
   }
   add_editor(person);
   thatguy = find_player(person);
   if (thatguy) {
      tell_object(thatguy, "You have "
                  "been promoted to editor of " + query_the_paper_name() + " by " +
                  this_player()->the_short() + "!\n");
   }
   log_file(query_log_file_name(),
                 "On " + ctime(time()) + " " + this_player()->query_short() +
                 " promoted " + capitalize(person) + " to " +
                 "editor.\n");
   add_succeeded_mess(capitalize(person) +
                      " is now an editor of " + query_the_paper_name() + ".\n");
   return 1;
}                               /* do_promote() */

/** @ignore yes */
int do_demote(string person, int editor)
{
   object thatguy;

   if (_proxy) {
      return _proxy->do_demote(person, editor);
   }
   person = lower_case(person);
   if (editor) {
      if (!is_editor(person)) {
         add_failed_mess
            ("There is nobody by that name currently employed as an "
             "editor at " + query_the_paper_name() + "!\n");
         return 0;
      }
      remove_editor(person);
   } else {
      if (!is_setter(person)) {
         add_failed_mess
            ("There is nobody by that name currently employed as a "
             "setter at " + query_the_paper_name() + "!\n");
         return 0;
      }
      remove_setter(person);
   }
   add_reporter(person);
   thatguy = find_player(person);
   if (thatguy) {
      tell_object(thatguy,
                  this_player()->the_short() +
                  " just demoted you as an editor of " + query_the_paper_name() + "!\n");
   }
   log_file(query_log_file_name(),
                 "On " + ctime(time()) + " " + this_player()->query_short() +
                 " demoted " + capitalize(person) +
                 " to reporter.\n");
   add_succeeded_mess(capitalize(person) +
                      " is no longer an editor of " + query_the_paper_name() + ".\n");
   return 1;
}                               /* do_demote */

/**
 * This is the status that everyone can see, not just the employees.
 */
int do_everyone_status() {
   string ret;

   if (_proxy) {
      return _proxy->do_everyone_status();
   }
   ret = query_the_paper_name() + " has the following employees:\n";
   ret += "$I$3=Owner: " +
          query_owner() +
          "\n";
   ret += "$I$3=Reporters: " +
          query_multiple_short(sort_array(query_all_reporters(), 0)) +
          "\n";
   ret += "$I$3=Setters  : " +
          (sizeof(query_all_setters())?
          query_multiple_short(sort_array(query_all_setters(), 0)):"no one") +
          "\n";
   ret += "$I$3=Editors  : " +
          (sizeof(query_all_setters())?
          query_multiple_short(sort_array(query_all_editors() - ({ query_owner() }), 0)):"no one") +
          "\n";

   write("$P$Status$P$" + ret);
   return 1;
} /* do_everyone_status() */

/** @ignore yes */
int do_status()
{
   string ret;
   int i;
   int last_num;

   if (_proxy) {
      return _proxy->do_status();
   }
   ret = query_the_paper_name() + " has the following employees:\n";
   ret += "$I$3=Owner: " +
          query_owner() +
          "\n";
   ret += "$I$3=Reporters: " +
          query_multiple_short(sort_array(query_all_reporters(), 0)) +
          "\n";
   ret += "$I$3=Setters  : " +
          (sizeof(query_all_setters())?
          query_multiple_short(sort_array(query_all_setters(), 0)):"no one") +
          "\n";
   ret += "$I$3=Editors  : " +
          (sizeof(query_all_setters())?
          query_multiple_short(sort_array(query_all_editors() - ({ query_owner() }), 0)):"no one") +
          "\n";
   ret += "$I$3=Sponsors : " +
          (sizeof(query_all_setters())?
          query_multiple_short(sort_array(query_all_sponsors(), 0)):"no one") +
          "\n";

   ret += "\n$I$0=Sale Statistics:\n";
   // Show the sale stats.
   last_num = NEWSPAPER_HANDLER->query_last_edition_num(_paper_name);
   for (i = last_num - 5; i <= last_num; i++) {
      if (i >= 1) {
         ret += "Edition " + i + " sold to " +
             NEWSPAPER_HANDLER->query_edition_num_sold(_paper_name, i) +
             " people.\n";
      }
   }
   ret += "\n";

   ret += "Using Bank  : " + _bank_branch + "\n";

   write("$P$Status$P$" + ret);
   return 1;
}                               /* do_reporters() */

/** @ignore yes */
int do_claim()
{
   object badge;
   object *deepinv;
   object *inv;

   if (_proxy) {
      return _proxy->do_claim();
   }
   deepinv = deep_inventory(this_player()) + ({ this_player() });
   inv = filter(deepinv, (: $1->query_paper_name() == query_paper_name() :));
   if (sizeof(inv)) {
      add_failed_mess
         ("You already have a set of credentials!  Why would you "
          "want another?\n");
      return 0;
   }
   badge = clone_object(NEWSPAPER_PRESS_CREDITIALS);
   badge->set_drop();
   badge->set_paper_name(query_paper_name());
   if (badge->move(this_player()) != MOVE_OK) {
      badge->move(this_object());
      tell_object(this_player(),
                  "The credentials are too heavy for you, so they fall "
                  "to the floor.\n");
   }
   badge->reset_drop();
   log_file(query_log_file_name(),
                 "On " + ctime(time()) + " " + this_player()->query_short() +
                 " claimed some new credentials.\n");
   add_succeeded_mess(({ "A new set of credentials appear from thin air, as "
                         "if by magic!\n",
                         "$N $V a set of credentials.\n" }));
   return 1;
}

/** @ignore yes */
int do_publish(string headline)
{
   class our_article* bing;

   if (_proxy) {
      return _proxy->do_publish(headline);
   }
   bing = filter(_articles, (: !$1->flags & NEWSPAPER_POSTPONE_FLAG :));
   if (!sizeof(bing)) {
      add_failed_mess("You cannot publish with no articles!\n");
      return 0;
   }
   write("Do you wish to publish this edition with a headline of:\n" +
          headline + "\nResponse [y/n]? ");
   add_succeeded_mess("");
   input_to("really_publish", 0, headline);
   return 1;
}                               /* do_publish() */

/**
 * This method figures out the total profit from the paper.
 */
int query_total_profit(int num_articles) {
   int last_pub;
   int amount;
   int edition;
   int adds;
   class advert data;
   class our_article bing;
   string text;

   if (num_articles == -1) {
      num_articles = 0;
      foreach (bing in _articles) {
         if (!(bing->flags & NEWSPAPER_POSTPONE_FLAG)) {
            text = load_article_text(bing);
            if (strlen(text) > 500) {
               num_articles++;
            }
         }
      }
   }

   foreach (data in _adverts) {
      adds += data->value;
   }

   edition = NEWSPAPER_HANDLER->query_last_edition_num(_paper_name);
   last_pub = NEWSPAPER_HANDLER->query_edition_date(_paper_name, edition);
   // Work out the amount based on how many much money they were sold for.
   amount = NEWSPAPER_HANDLER->query_edition_num_sold(_paper_name, edition) * 100;
   // First we scale it based on the number of articles.  Ignore number of
   // articles.
   //amount = (amount * num_articles * 400) / 14;
   amount = amount * 400 + adds;
   // Next we decrease it based on the time since the last edition.
   if ((time() - last_pub) < 42 * 24 * 60 * 60) {
      return amount;
   }
   last_pub = (time() - last_pub) - 42 * 24 * 60 * 60;
   last_pub = last_pub / (24 * 60 * 60);
   amount = ( amount / 150 ) * (150 - last_pub);
   return amount + _owner_extra_cash;
} /* query_total_profit() */

/** @ignore yes */
void really_publish(string char, string headline)
{
   int article_count;
   int amount;
   int total;
   string *things;
   string temp;
   class article* to_publish;
   string* text;
   class our_article bing;
   class payee_data data;
   string stuff;
   string place;
   int i;
   int editor_bonus;
   int num_editors;

   place = query_property("place");
   char = lower_case(char);
   things = ({ });
   if (strlen(char) && char[0] == 'y') {
      tell_object(this_player(),
                  "Okay, warm up the frogs, let's see if everything is "
                  "sorted out.\n");
   } else {
      tell_object(this_player(), "Okay, publishing aborted.\n");
      return 0;
   }

   to_publish = ({ });
   text = ({ });
   foreach (bing in _articles) {
      if (!(bing->flags & NEWSPAPER_POSTPONE_FLAG)) {
         to_publish += ({ copy(bing->art) });
         text += ({ load_article_text(bing) });
         if (strlen(text[<1]) > 500) {
            article_count++;
         }
      }
   }

   amount = query_total_profit(article_count);
   foreach (temp, data in _payees) {
      total += data->paid;
   }
   num_editors = sizeof(query_all_editors() - ({ query_owner() }));
   editor_bonus = (amount * 5 / 100) / num_editors;
   total += (amount * 5 / 100) * num_editors;
   //total += sizeof(query_all_editors() - ({ query_owner() })) *
   //          amount * 5 / 100;

   if (amount < total) {
      add_failed_mess("You have allocated paying your reporters " + total +
                      " when you only have " + amount + " total profit.\n");
      return 0;
   }

   if (!NEWSPAPER_HANDLER->publish_paper(query_paper_name(),
                                         headline,
                                         to_publish,
                                         text,
                                         _adverts,
                                         query_all_editors())) {
      tell_object(this_player(), "There was an error making the new issue!\n");
      return ;
   }
   _adverts = ({ });
   _owner_extra_cash = 0;
   stuff = "Edition summary.\nArticles posted:\n";
   foreach (bing in _articles) {
      if (!(bing->flags & NEWSPAPER_POSTPONE_FLAG)) {
         stuff += bing->art->title + " by " + bing->art->shown_name + "\n";
         // Delete it!
         delete_article(bing);
      }
   }
   _articles = filter(_articles, (: $1->flags & NEWSPAPER_POSTPONE_FLAG :));

   stuff += "\nTotal profit " +
            MONEY_HAND->money_value_string(amount, place) +
            ".\n";
   // Split it up.
   BANK_HANDLER->adjust_account(query_owner(), query_bank_branch(),
                                (amount - total));
   amount = editor_bonus;
   if (sizeof(query_all_editors() - ({ query_owner() })) > 0) {
      stuff += "\nEditors bonus " +
               query_multiple_short(query_all_editors() - ({ query_owner() })) +
               " paid " +
               MONEY_HAND->money_value_string(amount, place) + "each.\n";
      foreach (temp in query_all_editors()) {
         if (temp != query_owner()) {
            BANK_HANDLER->adjust_account(temp, query_bank_branch(),
                                         (amount));
            _reporters[temp]->total_payed += amount;
         }
      }
   }

   stuff += "\nReporters paid:\n";
   foreach (temp, data in _payees) {
      BANK_HANDLER->adjust_account(temp, query_bank_branch(), data->paid);
      if (is_reporter(temp)) {
         _reporters[temp]->total_payed += data->paid;
         stuff += temp + " " +
               MONEY_HAND->money_value_string(data->paid, place) + ".\n";
         i++;
         call_out((: AUTO_MAILER->auto_mail($1, $2, $3, $4, $5) :), i * 4,
                   temp,
                                this_object()->query_short(),
                                "Payment for new edition.",
                                "",
                                "A new edition has been released and "
                                "you have been paid!\n\n"
                                "For your work on the newspaper you "
                                "have been paid " +
                                MONEY_HAND->money_value_string(data->paid, place) +
                                ".\n\n" +
                                (data->message?data->message:"") );
      }
   }

   // Send this mail to the editors and the owner.
   call_out((: AUTO_MAILER->auto_mail($1, $2, $3, $4, $5) :), i * 4,
                          implode(query_all_editors(), ","),
                          this_object()->query_short(),
                          "Published new edition.",
                          implode(query_all_editors(), ","),
                          stuff);

   log_file(query_log_file_name(),
                 "On " + ctime(time()) + " " + this_player()->query_short() +
                 " published an edition of " + query_the_paper_name() + ".\n");
   _published_time = time();
   _payees = ([ ]);
   rename(query_article_log_file_name(),
             _save_dir + "article" + query_current_edition() + ".log");
   save_me();
   write("$P$Summary$P$" +
               "The pages have been sent to the printery, and another "
               "edition of " + query_the_paper_name() +
               " is about to hit the streets!  You feel rather "
               "impressed with your efforts.\n\n" + stuff);
   return;
}                               /* really_publish() */

/** @ignore yes */
int do_summary() {
   int amount;
   int adds;
   string person;
   int total;
   class our_article bing;
   class payee_data payee_data;
   string text;
   string ret;
   int article_count;
   string place;
   class advert data;
   int i;
   int last_num;
   int num_editors;
   int editor_bonus;

   if (_proxy) {
      return _proxy->do_summary();
   }
   ret = "";
   // Show the sale stats.
   last_num = NEWSPAPER_HANDLER->query_last_edition_num(_paper_name);
   for (i = last_num - 5; i <= last_num; i++) {
      if (i >= 1) {
         ret += "Edition " + i + " sold to " +
             NEWSPAPER_HANDLER->query_edition_num_sold(_paper_name, i) +
             " people.\n";
      }
   }
   ret += "\n";

   place = query_property("place");
   foreach (bing in _articles) {
      if (!(bing->flags & NEWSPAPER_POSTPONE_FLAG)) {
         text = load_article_text(bing);
         if (strlen(text) > 500) {
            article_count++;
         }
      }
   }

   ret += "Article count of " + article_count + "\n";

   if (!_adverts) {
      _adverts = ({ });
   }

   foreach (data in _adverts) {
      adds += data->value;
   }

   amount = query_total_profit(article_count);
   foreach (person, payee_data in _payees) {
      total += payee_data->paid;
   }
   num_editors = sizeof(query_all_editors() - ({ query_owner() }));
   editor_bonus = (amount * 5 / 100) / num_editors;
   total += editor_bonus * num_editors;

   ret += "Using Bank  : " + _bank_branch + "\n";
   ret += "Total Profit: " +
         MONEY_HAND->money_value_string(amount, place) +
         ", total editor bonus " +
         MONEY_HAND->money_value_string(editor_bonus * num_editors, place) +
         ", total paid to reporters " +
         MONEY_HAND->money_value_string(total, place) +
         ", profit from adverts " +
         MONEY_HAND->money_value_string(adds, place) +
         (_owner_extra_cash > 0 ?
         ", " + MONEY_HAND->money_value_string(_owner_extra_cash, place) +
            " extra added by the owner":"") +
         ".\n\n";

   ret += "Owner/Editors:\n";
   ret += "   " + query_owner() + " " +
          MONEY_HAND->money_value_string((amount - total), place) +
          " (owner)\n";
   ret += "Reporters:\n";
   foreach (person, payee_data in _payees) {
      if (!is_editor(person)) {
         ret += "$I$6=   " + person + " " +
                MONEY_HAND->money_value_string(payee_data->paid, place) +
                (payee_data->message?" " + payee_data->message:"") + ".\n";
      }
   }
   ret += "$I$0=Editor Bonuses:\n";
   foreach (person in query_all_editors()) {
      if (person != query_owner()) {
         if (!_payees[person]) {
            _payees[person] = new(class payee_data);
         }
         ret += "   " + person  + " " +
                MONEY_HAND->money_value_string(editor_bonus, place) +
                " Total " +
                MONEY_HAND->money_value_string((editor_bonus + _payees[person]->paid), place) +
                "\n";
      }
   }
   write("$P$Profits$P$" + ret);
   return 1;
} /* do_summary() */

/** @ignore yes */
int do_deposit(string amt) {
   int amt_int;
   string place;

   if (_proxy) {
      return _proxy->do_deposit(amt);
   }

   if (this_player()->query_name() != query_owner()) {
      add_failed_mess("Only the owner can do this.\n");
      return 0;
   }

   place = query_property("place");
   amt_int = MONEY_HAND->value_from_string(amt, place);
   if (amt_int <= 0) {
      add_failed_mess("The amount " + amt + " is invalid.\n");
      return 0;
   }

   // See if the owner has the right amount in the bank.
   if (BANK_HANDLER->query_account(this_player()->query_name(), query_bank_branch()) <
       amt_int) {
      add_failed_mess("You do not have " +
                      MONEY_HAND->money_value_string(amt_int, place) +
                      " in " + query_bank_branch() + ".\n");
      return 0;
   }

   _owner_extra_cash += amt_int;
   save_me();
   BANK_HANDLER->adjust_account(this_player()->query_name(), _bank_branch, -amt_int);

   add_succeeded_mess(({ "$N deposit " +
                      MONEY_HAND->money_value_string(amt_int, place) +
                      " for extra cash to pay reporters.\n",
                      "$N deposits some extra cash for reporters.\n" }));
   return 1;
} /* do_deposit() */

/** @ignore yes */
int do_set_bank_branch(string branch)
{
   string* accounts;

   if (_proxy) {
      return _proxy->do_set_bank_branch(branch);
   }
   accounts = BANK_HANDLER->query_accounts(this_player()->query_name());
   if (member_array(branch, accounts) == -1) {
      add_failed_mess("You need to specify a bank you have an account "
                      "with: " + query_multiple_short(accounts) +
                      ".\n");
      return 0;
   }
   _bank_branch = branch;

   save_me();
   add_succeeded_mess("$N set$s the bank branch to " + branch + ".\n");
   return 1;
} /* do_set_bank_branch() */

/** @ignore yes */
int do_pay(string person,
           string amt)
{
   string place;
   string fluff;
   int amount;
   int profit;
   int total;
   class payee_data data;

   if (_proxy) {
      return _proxy->do_pay(person, amt);
   }
   place = query_property("place");
   if (amt == "none" || amt == "0") {
      amount = 0;
   } else {
      amount = MONEY_HAND->value_from_string(amt, place);
      if (amount <= 0) {
         add_failed_mess("Sorry, you must pay reporters a "
                         "positive amount.\n");
         return 0;
      }
   }
   if (!is_reporter(person)) {
      add_failed_mess("There is nobody by that name currently employed at " +
                       query_the_paper_name() + "!\n");
      return 0;
   }

   if (person == query_owner()) {
      add_failed_mess("The owner gets paid anyway.\n");
      return 0;
   }

   profit = query_total_profit(-1);
   profit = profit * 95 / 100;
   foreach (fluff, data in _payees) {
      total += data->paid;
   }

   if (profit - total - amount < 0) {
      add_failed_mess("You are not making enough money to pay " +
                      person + " " +
                      MONEY_HAND->money_value_string(amount, place) +
                      ".\n");
      return 0;
   }

   if (!_payees[person]) {
      _payees[person] = new(class payee_data);
   }
   _payees[person]->paid = amount;

   if (_payees[person]->paid < 0) {
      add_succeeded_mess(({ "You can't pay anybody less than nothing!  "
                            "Amount owing reset to nothing.\n", "" }));
      map_delete(_payees, person);
      save_me();
      return 1;
   }
   if (!_payees[person]->paid) {
      add_succeeded_mess(({ "Ok, amount owing set to nothing.\n", "" }));
      map_delete(_payees, person);
      save_me();
      return 1;
   }
   add_succeeded_mess(({ "Ok, " + capitalize(person) + " will be paid a "
                         "total of " +
                     MONEY_HAND->money_value_string(_payees[person]->paid, place) +
                         " for this edition.\n", "" }));
   // Do the payment when the issue is done.
   save_me();
   return 1;
}                               /* do_pay() */

/** @ignore yes */
int do_pay_tag(string person,
               string tag)
{
   if (_proxy) {
      return _proxy->do_pay_tag(person, tag);
   }
   if (tag == "none") {
      tag = 0;
   }

   if (!is_reporter(person)) {
      add_failed_mess("There is nobody by that name currently employed at " +
                       query_the_paper_name() + "!\n");
      return 0;
   }

   if (person == query_owner()) {
      add_failed_mess("The owner gets paid anyway.\n");
      return 0;
   }

   if (!_payees[person]) {
      add_failed_mess("You can only set a reason for someone who is "
                      "being paid.\n");
      return 0;
   }

   _payees[person]->message = tag;

   if (!tag) {
      add_succeeded_mess(({ "You remove the message from " +
                            capitalize(person) + ".\n", "" }));
   } else {
      add_succeeded_mess(({ "You set a message to " + capitalize(person) +
                            " of " + tag + ".\n", "" }));
   }
   // Do the payment when the issue is done.
   save_me();
   return 1;
}                               /* do_pay() */

/** @ignore yes */
int do_title(string id, string title)
{
   class our_article art;

   if (_proxy) {
      return _proxy->do_title(id, title);
   }
   art = query_article(id);
   if (!art) {
      add_failed_mess("The article you wish to edit doesn't exist!\n");
      return 0;
   }
   if (!is_editor(this_player()->query_name()) &&
       !is_setter(this_player()->query_name()) &&
       lower_case(art->art->author) != this_player()->query_name()) {
      add_failed_mess("You can only edit your own articles.\n");
      return 0;
   }

   art->art->title = title;
   save_me();
   add_succeeded_mess(({ "You set the title to " + title + ".\n",
                         "$N sets the title of an article.\n" }));
   return 1;
}                               /* do_title() */

/** @ignore yes */
int do_author(string id, string author)
{
   class our_article art;

   if (_proxy) {
      return _proxy->do_author(id, author);
   }
   art = query_article(id);
   if (!art) {
      add_failed_mess("The article you wish to edit doesn't exist!\n");
      return 0;
   }
   if (!is_editor(this_player()->query_name()) &&
       !is_setter(this_player()->query_name()) &&
       lower_case(art->art->author) != this_player()->query_name()) {
      add_failed_mess("You can only edit your own articles.\n");
      return 0;
   }

   art->art->shown_name = author;
   save_me();
   add_succeeded_mess(({ "You set the author to " + author + ".\n",
                         "$N sets the author of an article.\n" }));
   return 1;
}                               /* do_author() */

/** @ignore yes */
int do_change(string id, string type)
{
   class our_article art;

   if (_proxy) {
      return _proxy->do_change(id, type);
   }
   art = query_article(id);
   if (!art) {
      add_failed_mess("The article you wish to edit doesn't exist!\n");
      return 0;
   }
   if (!is_editor(this_player()->query_name()) &&
       !is_setter(this_player()->query_name()) &&
       lower_case(art->art->author) != this_player()->query_name()) {
      add_failed_mess("You can only edit your own articles.\n");
      return 0;
   }

   if (type == "html") {
      art->art->type = NEWSPAPER_ARTICLE_TYPE_HTML | a_flags(art->art->type);
   } else {
      art->art->type = NEWSPAPER_ARTICLE_TYPE_PLAIN | a_flags(art->art->type);
   }
   save_me();
   unguarded( (: rm(_save_dir + $(art->art->file_name) + ".proc" ) :) );
   unguarded( (: rm(_save_dir + $(art->art->file_name) + ".nroff.o" ) :) );
   add_succeeded_mess(({ "You set the article type to " + type + ".\n",
                         "$N sets the type of an article.\n" }));
   return 1;
}                               /* do_author() */

/** @ignore yes */
int do_change_xp(string id, string type)
{
   class our_article art;

   if (_proxy) {
      return _proxy->do_change_xp(id, type);
   }
   art = query_article(id);
   if (!art) {
      add_failed_mess("The article you wish to edit doesn't exist!\n");
      return 0;
   }
   if (!is_editor(this_player()->query_name())) {
      add_failed_mess("Only the editors can do this.\n");
      return 0;
   }

   if (type == "none") {
      art->art->type |= NEWSPAPER_ARTICLE_NO_XP_FLAG;
   } else {
      art->art->type &= ~NEWSPAPER_ARTICLE_NO_XP_FLAG;
   }
   save_me();
   add_succeeded_mess(({ "You set the article xp return to " + type + ".\n",
                         "$N sets the xp return of an article.\n" }));
   return 1;
}                               /* do_author() */

/** @ignore yes */
int do_change_colour(string id, string colour)
{
   class our_article art;

   if (_proxy) {
      return _proxy->do_change_colour(id, colour);
   }
   art = query_article(id);
   if (!art) {
      add_failed_mess("The article you wish to edit doesn't exist!\n");
      return 0;
   }
   if (!is_editor(this_player()->query_name()) &&
       !is_setter(this_player()->query_name()) &&
       lower_case(art->art->author) != this_player()->query_name()) {
      add_failed_mess("You can only edit your own articles.\n");
      return 0;
   }

   if (colour != "none") {
      art->art->colour = upper_case(colour);
   } else {
      art->art->colour = 0;
   }
   save_me();
   add_succeeded_mess(({ "You set the article colour to " + colour + ".\n",
                         "$N sets the colour of an article.\n" }));
   return 1;
}                               /* do_author() */

/** @ignore yes */
int do_editing(string id)
{
   class our_article art;
   string contents;

   if (_proxy) {
      return _proxy->do_editing(id);
   }
   if (_locked) {
      add_failed_mess("The newspaper office is currently locked for "
                      "submissions.\n");
      return 0;
   }
   art = query_article(id);
   if (!art) {
      add_failed_mess("The article you wish to edit doesn't exist!\n");
      return 0;
   }
   if (!is_editor(this_player()->query_name()) &&
       lower_case(art->art->author) != this_player()->query_name()) {
      add_failed_mess("You can only edit your own articles.\n");
      return 0;
   }

   if (art->flags & NEWSPAPER_LOCK_FLAG) {
      add_failed_mess("This article is locked.\n");
      return 0;
   }

   contents = load_article_text(art);
   add_succeeded_mess("");
   this_player()->do_edit(contents, "finish_editing", this_object(), 0, art);
   return 1;
}                               /* do_editing() */

/**
 * This method moves an article up or down in the current article list.
 * @param index the index of the article to move
 * @param up_down move it up or down
 */
int move_article(int index, string up_down) {
   class our_article womble;

   if (index == -1) {
      return 0;
   }

   womble = _articles[index];
   if (up_down == "up") {
      if (index > 0) {
         _articles = _articles[0..index - 1] + _articles[index+1..];
         _articles = _articles[0..index - 2] + ({ womble }) +
                     _articles[index - 1..];
         save_me();
      } else {
         return 0;
      }
   } else {
      if (index < sizeof(_articles) - 1) {
         _articles = _articles[0..index - 1] + _articles[index+1..];
         _articles = _articles[0..index] + ({ womble }) +
                     _articles[index + 1..];
         save_me();
      } else {
         return 0;
      }
   }
   return 1;
}

/** @ignore yes */
int do_move(string article, string up_down) {
   int index;
   class our_article womble;

   if (_proxy) {
      return _proxy->do_move(article, up_down);
   }
   index = query_article_index(article);
   if (index == -1) {
      add_failed_mess("Article reference " + index + " is out of bounds.\n");
      return 0;
   }

   womble = _articles[index];
   if (!move_article(index, up_down)) {
      add_failed_mess("Unable to move the article.\n");
      return 0;
   }
   add_succeeded_mess(({ "You moved the article " + womble->art->title + " " +
                         up_down + ".\n", "" }));

   return 1;
}

/** @ignore yes */
int do_move_pos(string article, string pos) {
   int index;
   int to_index;
   class our_article womble;

   if (_proxy) {
      return _proxy->do_move_pos(article, pos);
   }
   index = query_article_index(article);
   if (index == -1) {
      add_failed_mess("Article reference " + index + " is out of bounds.\n");
      return 0;
   }
   to_index = query_article_index(pos);
   if (to_index == -1) {
      add_failed_mess("Article reference " + pos + " is out of bounds.\n");
      return 0;
   }

   if (to_index == index) {
      add_failed_mess("You cannot move the article there, it is already "
                      "there.\n");
      return 0;
   }

   womble = _articles[index];
   if (index < to_index) {
      _articles = _articles[0..index - 1] + _articles[index + 1..to_index] +
                  ({ _articles[index] }) + _articles[to_index+1..];
   } else {
      _articles = _articles[0..to_index - 1] +
                  ({ _articles[index] }) +
                  _articles[to_index..index - 1] +
                  _articles[index+1..];
   }
   save_me();
   add_succeeded_mess(({ "You moved the article " + womble->art->title +
                         " to " + pos + ".\n", "" }));

   return 1;
}

/** @ignore yes */
int do_postpone(string id, string on_off) {
   class our_article art;

   if (_proxy) {
      return _proxy->do_postpone(id, on_off);
   }
   art = query_article(id);
   if (!art) {
      add_failed_mess("The article reference " + id + " does not exist.\n");
      return 0;
   }

   if (on_off == "on") {
      art->flags |= NEWSPAPER_POSTPONE_FLAG;
      add_succeeded_mess(({ "You postpone the article " + art->art->title +
                            " to a later issue.\n",
                            "" }));
   } else {
      art->flags &= ~NEWSPAPER_POSTPONE_FLAG;
      add_succeeded_mess(({ "You unpostpone the article " + art->art->title +
                            " so it will be in this issue.\n",
                            "" }));
   }
   save_me();
   return 1;
} /* do_postpone() */

/** @ignore yes */
int do_lock_article(string id, string on_off) {
   class our_article art;

   if (_proxy) {
      return _proxy->do_lock_article(id, on_off);
   }
   art = query_article(id);
   if (!art) {
      add_failed_mess("The article reference " + id + " does not exist.\n");
      return 0;
   }

   if (on_off == "on") {
      art->flags |= NEWSPAPER_LOCK_FLAG;
      add_succeeded_mess(({ "You lock the article " + art->art->title +
                            ".\n",
                            "" }));
   } else {
      art->flags &= ~NEWSPAPER_LOCK_FLAG;
      add_succeeded_mess(({ "You unlock the article " + art->art->title +
                            ".\n",
                            "" }));
   }
   save_me();
   return 1;
} /* do_lock() */

/** @Ignore yes */
int do_category_list() {
   string ret;
   string place;
   string name;
   class category_data data;

   if (_proxy) {
      return _proxy->do_category_list();
   }
   if (!sizeof(_category)) {
      add_failed_mess("There are no categories.\n");
      return 0;
   }

   ret = "";
   place = query_property("place");
   foreach (name, data in _category) {
      ret += "$I$5=" + name + " costs ";
      if (data->cost_per_add) {
         ret += MONEY_HAND->money_value_string(data->cost_per_add, place);
      } else {
         ret += "nothing";
      }
      ret += " per add and ";
      if (data->cost_per_ten_chars) {
         ret += MONEY_HAND->money_value_string(data->cost_per_ten_chars,
                                               place);
      } else {
         ret += "nothing";
      }
      ret += " for every ten characters.";
      if (!data->open) {
         ret += "  The category is currently closed.\n";
      } else {
         ret += "\n";
      }
   }
   write("$P$Category$P$" + ret);
   return 1;
} /* do_category_list() */

/** @Ignore yes */
int do_advert_list() {
   string ret;
   string place;
   class advert data;
   int editor;
   int found;
   int pos;

   if (_proxy) {
      return _proxy->do_advert_list();
   }
   if (!sizeof(_adverts)) {
      add_failed_mess("There are no adverts.\n");
      return 0;
   }

   ret = "";
   place = query_property("place");
   editor = is_editor(this_player()->query_name());
   pos = 1;
   foreach (data in _adverts) {
      if (editor ||
          lower_case(data->author) == this_player()->query_name()) {
         if (editor) {
            ret += "$I$5=" + pos + ") ";
         } else {
            ret += "$I$5=";
         }
         ret += data->author + " cost " +
                MONEY_HAND->money_value_string(data->value, place) +
                " written at " + ctime(data->date_written) + ":\n" +
                data->text + "\n";
         found = 1;
      }
      pos++;
   }
   if (!found) {
      add_failed_mess("There are no adverts.\n");
      return 0;
   }
   write("$P$Category$P$" + ret);
   return 1;
} /* do_advert_list() */

/** @ignore yes */
int do_advert_remove(int id) {
   class advert fluff;

   if (_proxy) {
      return _proxy->do_advert_remove(id);
   }
   if (!sizeof(_adverts)) {
      add_failed_mess("There are no adverts.\n");
      return 0;
   }

   if (!is_editor(this_player()->query_name())) {
      add_failed_mess("You must be an editor to remove adverts.\n");
      return 0;
   }

   if (id < 1 || id > sizeof(_adverts)) {
      add_failed_mess("The id " + id + " is invalid.\n");
      return 0;
   }

   fluff = _adverts[id - 1];
   _adverts = _adverts[0..id-2] + _adverts[id..];
   add_succeeded_mess("$N remove$s an advert by " + fluff->author + ".\n");
   return 1;
} /* do_advert_remove() */

/** @Ignore yes */
int do_category_remove(string category) {
   if (_proxy) {
      return _proxy->do_category_remove(category);
   }
   category = query_category(category);
   if (!category) {
      add_failed_mess("You must have a category to delete it.\n");
      return 0;
   }

   if (sizeof(filter(_adverts, (: $1->category == $2 :), category))) {
      add_failed_mess("You have adverts in that category, you cannot "
                      "remove it.\n");
      return 0;
   }

   map_delete(_category, category);
   save_me();
   add_succeeded_mess(({ "You delete the category " + category + ".\n",
                          "" }));
   return 1;
} /* do_category_remove() */

/** @Ignore yes */
int do_category_add(string category) {
   string new_category;

   if (_proxy) {
      return _proxy->do_category_add(category);
   }
   new_category = query_category(category);
   if (new_category) {
      add_failed_mess("You must have a category to delete it.\n");
      return 0;
   }

   _category[category] = new(class category_data,
                             open : 0,
                             cost_per_add : 40,
                             cost_per_ten_chars : 40);
   save_me();
   add_succeeded_mess(({ "You add the category " + category + ".\n",
                          "" }));
   return 1;
} /* do_category_add() */

/** @Ignore yes */
int do_category_open(string category, string open) {
   if (_proxy) {
      return _proxy->do_category_open(category, open);
   }
   category = query_category(category);
   if (!category) {
      add_failed_mess("You must have a category to delete it.\n");
      return 0;
   }

   _category[category]->open = open == "open";
   save_me();
   add_succeeded_mess(({ "You " + open + " the category " + category + ".\n",
                          "" }));
   return 1;
} /* do_category_open() */

/** @Ignore yes */
int do_category_cost(string category, string cost) {
   int value;
   string place;

   if (_proxy) {
      return _proxy->do_category_cost(category, cost);
   }
   category = query_category(category);
   if (!category) {
      add_failed_mess("You must change a cost for a category.\n");
      return 0;
   }

   place = query_property("place");
   if (lower_case(cost) == "none") {
      value = 0;
   } else {
      value = MONEY_HAND->value_from_string(cost, place);
      if (!value) {
         add_failed_mess("The value " + cost + " is not a valid money "
                         "value.\n");
         return 0;
      }
   }

   _category[category]->cost_per_add = value;
   save_me();
   add_succeeded_mess(({ "You set the cost per add in category " +
                         category + " to " +
                         MONEY_HAND->money_value_string(value, place) +
                         ".\n",
                          "" }));
   return 1;
} /* do_category_cost() */

/** @Ignore yes */
int do_category_cost_per_ten_chars(string category, string cost) {
   int value;
   string place;

   if (_proxy) {
      return _proxy->do_category_cost_per_ten_chars(category, cost);
   }
   category = query_category(category);
   if (!category) {
      add_failed_mess("You must change a cost for a category.\n");
      return 0;
   }

   place = query_property("place");
   if (lower_case(cost) == "none") {
      value = 0;
   } else {
      value = MONEY_HAND->value_from_string(cost, place);
      if (!value) {
         add_failed_mess("The value " + cost + " is not a valid money "
                         "value.\n");
         return 0;
      }
   }

   _category[category]->cost_per_ten_chars = value;
   save_me();
   add_succeeded_mess(({ "You set the cost per ten characters in category " +
                         category + " to " +
                         MONEY_HAND->money_value_string(value, place) +
                         ".\n",
                          "" }));
   return 1;
} /* do_category_cost_per_ten_chars() */

/** @ignore yes */
int do_submit_section(string name) {
   class our_article cur_art;

   if (_proxy) {
      return _proxy->do_submit_section(name);
   }
   if (_locked) {
      add_failed_mess("The newspaper office is currently locked.\n");
      return 0;
   }
   cur_art = new(class our_article);
   cur_art->art = new(class article);
   cur_art->art->title = name;
   cur_art->art->shown_name = this_player()->query_name();
   cur_art->art->author = this_player()->query_name();
   cur_art->art->date_written = time();
   cur_art->art->type = NEWSPAPER_ARTICLE_TYPE_SECTION;

   _articles += ({ cur_art });
   save_me();
   add_succeeded_mess("$N $V a section.\n");
   return 1;
} /* do_submit_section() */

/** @Ignore yes */
int do_submit_add(string category) {
   if (_proxy) {
      return _proxy->do_submit_add(category);
   }
   if (_locked) {
      add_failed_mess("The newspaper office is currently locked.\n");
      return 0;
   }

   category = query_category(category);
   if (!category) {
      add_failed_mess("You must specify an existing category to "
                      "submit an add in.\n");
      return 0;
   }

   if (!_category[category]->open) {
      add_failed_mess("The category must be open to submit an add "
                      "for it.\n");
      return 0;
   }

   add_succeeded_mess(({ "", "$N starts submiting an add to " +
                 this_object()->the_short() +
                      ".\n" }));
   write("Please submit the text of your add:\n");
   this_player()->do_edit(0, "finish_add", this_object(), 0, category);
   //input_to("finish_add", 0, category)category;
   return 1;
} /* do_submit_add() */

/** @Ignore yes */
int do_lock(int locked) {

   if (_proxy) {
      return _proxy->do_lock(locked);
   }
   _locked = locked;
   if (_locked) {
      add_succeeded_mess("$N lock$s " + this_object()->the_short() + ".\n");
   } else {
      add_succeeded_mess("$N unlock$s " + this_object()->the_short() + ".\n");
   }
   return 1;
} /* do_lock() */

/** @ignore yes */
void finish_add(string str, string category) {
   int value;
   string place;

   if (!str || !strlen(str)) {
      write("Ok, Aborted writing an add.\n");
      return ;
   }

   if (is_sponsor(this_player()->query_name())) {
      write("You add in the category " + category + " with a text of:\n" +
             str + "\n\nThis will cost you nothing since you are a sponsor.\n"
               ", do you want to do this? ");
      input_to("confirm_add", 0, str, category, 0);
   } else {
      place = query_property("place");
      value = _category[category]->cost_per_add +
              _category[category]->cost_per_ten_chars * strlen(str) / 10;
      if (this_player()->query_value_in(place) < value) {
         write("You add in the category " + category + " with a text of:\n" +
               str + "\n\nThis will cost you " +
               MONEY_HAND->money_value_string(value, place) +
               ", but you do not have enough money!  Sorry.\n");
      } else {
         write("You add in the category " + category + " with a text of:\n" +
               str + "\n\nThis will cost you " +
               MONEY_HAND->money_value_string(value, place) +
               ", do you want to do this? ");
         input_to("confirm_add", 0, str, category, value);
      }
   }
} /* finish_add() */

/** @ignore yes */
void confirm_add(string str, string text, string category, int value) {
   class advert add;
   string place;
   int found;
   class our_article bing;

   if (!str || !strlen(str)) {
      return 0;
   }

   if (lower_case(str)[0] != 'y') {
      write("Ok, aborting.\n");
      return ;
   }

   if (!is_sponsor(this_player()->query_name())) {
      place = query_property("place");
      if (this_player()->query_value_in(place) < value) {
         add_failed_mess("You do not have " +
            MONEY_HAND->money_value_string(value, place) +
            " to pay for the add.\n");
         return ;
      }

      this_player()->pay_money(MONEY_HAND->create_money_array(value, place),
                               place);
   }

   add = new(class advert);
   add->author = this_player()->query_cap_name();
   add->text = text;
   add->category = category;
   add->value = value;
   add->date_written = time();
   _adverts += ({ add });
   foreach (bing in _articles) {
      if (a_type(bing->art->type) == NEWSPAPER_ARTICLE_TYPE_ADVERTS) {
         found = 1;
         break;
      }
   }
   if (!found) {
      _articles += ({ new(class our_article,
                          art : new(class article,
                                    title : "Adverts",
                                    author : query_owner(),
                                    shown_name : query_owner(),
                                    date_written : time(),
                                    type : NEWSPAPER_ARTICLE_TYPE_ADVERTS
                                   )
                          ) });
   }
   save_me();
   write("Ok, your advertisement is added.\n");
} /* confirm_add() */

/** @ignore yes */
int finish_editing(string story, class our_article art)
{
   if (!story) {
      tell_object(this_player(), "Ok, no editing saved.\n");
      return 0;
   }
   save_article_text(art, story);
   log_file(query_article_log_file_name(),
                 "On " + ctime(time()) + " " + this_player()->query_short() +
                 " edited the story " + art->art->title + ".\n");
}                               /* finish_editing() */


/** @ignore yes */
int do_transfer_ownership(string new_owner) {
   int value;
   string place;

   if (!is_owner(this_player()->query_name())) {
      add_failed_mess("You must be the owner to transfer the ownership.\n");
      return 0;
   }

   if (_proxy) {
      add_failed_mess("You must be in the main office to transfer ownership.\n");
      return 0;
   }

   if (!PLAYER_HANDLER->test_user(new_owner)) {
      add_failed_mess("You must transfer ownership to someone that exists.\n");
      return 0;
   }

   //
   // Find out how much the paper cost inthe first place to demand that as
   // a fee.
   //
   place = query_property("place");
   value = HOUSING->query_value(base_name(this_object()));
   if (this_player()->query_value_in(place) < value &&
       BANK_HANDLER->query_account(this_player()->query_name(), query_bank_branch()) <
       value) {
      add_failed_mess("You need at least " +
                      MONEY_HAND->money_value_string(value, place)  +
                      " to transfer the ownership, in hand or in the bank.\n");
      return 0;
   }


   write("It will cost you " + MONEY_HAND->money_value_string(value, place)  +
         " in fees to transfer ownership to " +
         new_owner + ".\nDo you want to do this? ");
   input_to("check_transfer", 0, new_owner, value);
   add_succeeded_mess("");
   return 1;
}

/** @ignore yes */
void check_transfer(string test, string new_owner, int value) {
   string place;

   test = lower_case(test);
   if (!strlen(test) || test[0] != 'y') {
      write("Aborted.\n");
      return 0;
   }

   // Check the money again.
   place = query_property("place");
   if (this_player()->query_value_in(place) < value  &&
       BANK_HANDLER->query_account(this_player()->query_name(), query_bank_branch()) <
       value) {
      write("You need at least " +
                      MONEY_HAND->money_value_string(value, place)  +
                      " to transfer the ownership.\n");
      return 0;
   }

   // Do it.
   if (this_player()->query_value_in(place) >= value ) {
      this_player()->pay_money(MONEY_HAND->create_money_array(value, place), place);
   } else {
       BANK_HANDLER->adjust_account(this_player()->query_name(), query_bank_branch(), -value);
   }
   write("Transfering the ownership from " +
         query_owner() + " to " + new_owner + ".\n");
   HOUSING->set_owner(base_name(this_object()), new_owner);
}

/**
 * This method returns all the current reporters of the magazine.
 * @return all the current reporters
 */
string* query_all_reporters()
{
   return keys(_reporters);
}

/**
 * This method returns all the current setters of the magazine.
 * @return all the current setters
 */
string* query_all_setters()
{
   return copy(_setters);
}

/**
 * This method returns all the current sponsors of the magazine.
 * @return all the current sponsors
 */
string* query_all_sponsors()
{
   return copy(_sponsors);
}

/**
 * This method returns all the current editors of the magazine.
 * @return all the current editors
 */
string* query_all_editors()
{
   return _editors + ({ query_owner() });
}

/**
 * This method returns all the current payees of the magazine.
 * @return who is to be payed and how much
 */
mapping query_all_payees()
{
   return copy(_payees);
}

/**
 * This method returns the 'article' class bits of the articles.
 * @return all the articles.\
 */
class article* query_all_articles() {
   return map(_articles, (: copy($1->art) :) );
} /* query_all_articles() */

/**
 * This method checks to see if the specified article is postponed
 * or not.
 * @param art the article to check
 * @return 1 if it is postponed, 0 if not
 */
int is_postponed(class article art) {
   class our_article rabbit;

   foreach (rabbit in _articles) {
      if (rabbit->art->file_name == art->file_name) {
         return rabbit->flags & NEWSPAPER_POSTPONE_FLAG;
      }
   }
   return 0;
} /* is_postponned() */

/**
 * This method checks to see if the specified article is locked
 * or not.
 * @param art the article to check
 * @return 1 if it is locked, 0 if not
 */
int is_locked(class article art) {
   class our_article rabbit;

   foreach (rabbit in _articles) {
      if (rabbit->art->file_name == art->file_name) {
         return rabbit->flags & NEWSPAPER_LOCK_FLAG;
      }
   }
   return 0;
} /* is_locked() */

/**
 * This method checks to see if the specified person is an reporter for
 * the paper or not.
 * @param word the person to check
 * @return 1 if they are a reporter
 */
int is_reporter(string word)
{
   if (_proxy) {
      return _proxy->is_reporter(word);
   }
   return classp(_reporters[word]);
}

/**
 * This method checks to see if the specified person is an setter for
 * the paper or not.
 * @param word the person to check
 * @return 1 if they are a setter
 */
int is_setter(string word)
{
   if (_proxy) {
      return _proxy->is_setter(word);
   }
   return member_array(word, _setters) != -1;
}

/**
 * This method checks to see if the specified person is a sponsor for
 * the paper or not.
 * @param word the person to check
 * @return 1 if they are a sponsor
 */
int is_sponsor(string word)
{
   if (_proxy) {
      return _proxy->is_sponsor(word);
   }
   return member_array(word, _sponsors) != -1;
}

/**
 * This method checks to see if the specified person is an editor for
 * the paper or not.
 * @param word the person to check
 * @return 1 if they are a editor
 */
int is_editor(string word)
{
   if (_proxy) {
      return _proxy->is_editor(word);
   }
   if (master()->query_administrator(word)) {
      return 1;
   }
   if (word == query_owner()) {
      return 1;
   }
   return member_array(word, _editors) != -1;
}

/**
 * This method checks to see if the specified person is an owner of
 * the paper or not.
 * @param word the person to check
 * @return 1 if they are a owner
 */
int is_owner(string word)
{
   if (_proxy) {
      return _proxy->is_owner(word);
   }
   if (master()->query_administrator(word)) {
      return 1;
   }
   if (word == query_owner()) {
      return 1;
   }
   return 0;
}

/**
 * This method adds a new reporter to the paper.
 * @param word the new reporter to add
 */
void add_reporter(string word)
{
   if (!_reporters[word]) {
      _reporters[word] = new(class reporter);
      _reporters[word]->date_added = time();
      update_commands(word);
      save_me();
   }
}                               /* add_reporter() */

/**
 * This method adds a new sponsor to the paper.
 * @param word the new sponsor to add
 */
void add_sponsor(string word)
{
   if (!is_sponsor(word)) {
      _sponsors += ({ word });
      update_commands(word);
      save_me();
   }
}                               /* add_sponsor() */

/**
 * This method adds a new setter to the paper.
 * @param word the new setter to add
 */
void add_setter(string word)
{
   if (!is_setter(word)) {
      if (!is_reporter(word)) {
         add_reporter(word);
      }
      _setters += ({ word });
      update_commands(word);
      save_me();
   }
}                               /* add_setter() */

/**
 * This method adds a new editor to the paper.
 * @param word the new editor to add
 */
void add_editor(string word)
{
   if (!is_editor(word)) {
      if (!is_reporter(word)) {
         add_reporter(word);
      }
      _editors += ({ word });
      update_commands(word);
      save_me();
   }
}                               /* add_editor() */

/**
 * This method removes a reporter from the paper.
 * @param word the reporter to remove
 */
void remove_reporter(string word)
{
   if (!is_reporter(word)) {
      return;
   }
   map_delete(_reporters, word);
   update_commands(word);
   save_me();
}                               /* remove_reporter() */

/**
 * This method removes a editor from the paper.
 * @param word the editor to remove
 */
void remove_editor(string word)
{
   if (!is_editor(word)) {
      return;
   }
   _editors -= ({ word });
   update_commands(word);
   save_me();
}                               /* remove_editor() */

/**
 * This method removes a setter from the paper.
 * @param word the setter to remove
 */
void remove_setter(string word)
{
   if (!is_setter(word)) {
      return;
   }
   _setters -= ({ word });
   update_commands(word);
   save_me();
}                               /* remove_setter() */

/*
 * This method removes a sponsor from the paper.
 * @param word the sponsor to remove
 */
void remove_sponsor(string word)
{
   if (!is_sponsor(word)) {
      return;
   }
   _sponsors -= ({ word });
   save_me();
}                               /* remove_editor() */

/**
 * This method moves the player in and out of the room to fix up their
 * commands.
 * @param word the player to move
 */
void update_commands(string word)
{
   object player;

   player = find_player(word);
   if (player && environment(player) == this_object()) {
      player->move("/room/void");
      player->move(file_name(this_object()));
   }
}                               /* update_commands() */

/**
 * This method returns the current owner of the paper.
 * @return the owner of the paper
 */
string query_owner() {
   if (_proxy) {
      return _proxy->query_owner();
   }
   return _owner;
} /* query_owner() */

/**
 * This method sets the current owner of the paper.
 * @param person the new owner
 */
void set_owner(string person) {
   _owner = person;
   save_me();
} /* set_owner() */

/**
 * This method is called by the housing system when the ownership changes.
 * @param old_owner the old owner
 * @param new_owner the newowner
 */
void ownership_change(string old_owner, string new_owner) {
   set_owner(new_owner);
}

/**
 * This method sets up a proxy for the room.  A proxy means all the commands
 * are mirrored to the real location, so save files do not get messed up.
 * You set this if you have any extra rooms you want to be able to control
 * the paper.
 * @param proxy the proxy to set
 */
void set_proxy(string proxy) {
   _proxy = proxy;
} /* set_proxy() */

/**
 * This method returns the proxy for this room.
 * @return the proxy for the room
 */
string query_proxy() {
   return _proxy;
} /* query_proxy() */

/**
 * This method is here to control access to a board if one is requested.
 * It allows only reporters to be able to see or write to it and it
 * allows editors to be able to eat messages from it.
 */
int board_access_check(int type, string board, object previous, string name) {
   switch (type) {
   case B_ACCESS_READ :
      return is_reporter(lower_case(name));
   case B_ACCESS_WRITE :
      return is_reporter(lower_case(name));
   case B_ACCESS_DELETE :
      // Players can always delete their own notes, do not need to code
      // for that.
      return is_editor(name);
   }
} /* board_access_check() */

void init()
{
   string person;

   person = this_player()->query_name();
   add_command("advert", "categories", (: do_category_list() :));
   add_command("advert", "list", (: do_advert_list() :));
   add_command("category", "list", (: do_category_list() :));
   add_command("submit", "advert {in|to|into} <string'category'>",
               (: do_submit_add($4[1]) :));
   add_command("submit", "advert <string'category'>",
               (: do_submit_add($4[0]) :));
   if (!is_editor(person) && !is_reporter(person) && !is_setter(person)) {
      add_command("apply", "", (: do_apply() :));
      add_command("status", "", (: do_everyone_status() :));
      return;
   }
   add_command("submit", "text <string:quoted'article name'> as <string'author'>",
               (: do_submit($4[0], $4[1], 0) :));
   add_command("submit", "html <string:quoted'article name'> as <string'author'>",
               (: do_submit($4[0], $4[1], 1) :));
   add_command("submit", "text <string'article name'>",
               (: do_submit($4[0], 0, 0) :));
   add_command("submit", "html <string'article name'>",
               (: do_submit($4[0], 0, 1) :));
   add_command("retrieve", "<string'article'>", (: do_retrieve($4[0], 0) :));
   add_command("retrieve", "source <string'article'>",
               (: do_retrieve($4[0], 1) :));
   add_command("resign", "", (: do_resign() :));
   add_command("status", "", (: do_status() :));
   add_command("status", "room", (: do_status() :));
   add_command("claim", "", (: do_claim() :));
   add_command("articles", "", (: do_articles(0) :));
   add_command("articles", "scroll", (: do_articles(1) :));
   add_command("edit", "<string'article'>", (: do_editing($4[0]) :));
   add_command("delete", "<string'article'>", (: do_delete($4[0]) :));
   add_command("title", "<string'article'> <string'new title'>",
               (: do_title($4[0], $4[1]) :));
   add_command("author", "<string'article'> <string'new author name'>",
               (: do_author($4[0], $4[1]) :));
   add_command("change", "type of <string'article'> to {html|plain}",
               (: do_change($4[0], $4[1]) :));
   if (!is_setter(person) && !is_editor(person)) {
      return;
   }
   add_command("change", "colour of <string'article'> to {red|blue|green|cyan|magenta|yellow|brown|none}",
               (: do_change_colour($4[0], $4[1]) :));
   add_command("move", "<string'article'> {up|down}",
               (: do_move($4[0], $4[1]) :));
   add_command("move", "<string'article'> to <string'position'>",
               (: do_move_pos($4[0], $4[1]) :));
   if (!is_editor(person)) {
      return;
   }
   add_command("change", "xp [return] of <string'article'> to {none|normal}",
               (: do_change_xp($4[0], $4[1]) :));
   add_command("submit", "section <string'section name'>",
               (: do_submit_section($4[0]) :));
   add_command("add", "reporter <string'person'>", (: do_hire($4[0], 0) :));
   add_command("add", "setter <string'person'>", (: do_hire($4[0], 1) :));
   add_command("add", "editor <string'person'>", (: do_promote($4[0]) :));
   add_command("add", "sponsor <string'person'>", (: do_sponsor_add($4[0]) :));
   //add_command("promote", "<string'person'>", (: do_promote($4[0]) :));
   add_command("remove", "editor <string'person'>", (: do_demote($4[0], 1) :));
   add_command("remove", "setter <string'person'>", (: do_demote($4[0], 0) :));
   add_command("remove", "reporter <string'person'>", (: do_dismiss($4[0]) :));
   add_command("remove", "sponsor <string'person'>", (: do_sponsor_remove($4[0]) :));
   add_command("log", "", (: do_logs(0) :));
   add_command("log", "article", (: do_logs(1) :));
   //add_command("unmark", "<string'article'>", (: do_unmark($4[0]) :));
   add_command("pay", "bank branch <string'bank name'>",
               (: do_set_bank_branch($4[0]) :));
   add_command("pay", "<word'person'> <string'amount|none'>",
               (: do_pay($4[0], $4[1]) :));
   add_command("pay", "<word'person'> message <string'message|none'>",
               (: do_pay_tag($4[0], $4[1]) :));
   add_command("profit", "", (: do_summary() :));
   add_command("profit", "deposit extra <string'cash'>",
               (: do_deposit($4[0]) :));
   add_command("postpone", "<string'article'> {on|off}",
               (: do_postpone($4[0], $4[1]) :));
   add_command("lock", "articles {on|off}", (: do_lock($4[0] == "on") :));
   add_command("lock", "article <string'article'> {on|off}",
               (: do_lock_article($4[0], $4[1]) :));
   add_command("publish", "with headline <string'headline'>",
               (: do_publish($4[0]) :));
   add_command("category", "{open|close} <string'catgory'>",
               (: do_category_open($4[1], $4[0]) :));
   add_command("category", "add <string'catgory'>",
               (: do_category_add($4[0]) :));
   add_command("category", "remove <string'catgory'>",
               (: do_category_remove($4[0]) :));
   add_command("category", "cost <string'category'> per add <string'cost|none'>",
               (: do_category_cost($4[0], $4[1]) :));
   add_command("category", "cost <string'category'> per ten characters <string'cost|none'>",
               (: do_category_cost_per_ten_chars($4[0], $4[1]) :));
   add_command("advert", "remove <number'id'>", (: do_advert_remove($4[0]) :));

   if (!is_owner(this_player()->query_name())) {
      return ;
   }
   add_command("transfer", "paper ownership to <string'name'>",
               (: do_transfer_ownership($4[0]) :));
}                               /* init() */

class advert* query_ads() { return _adverts; }


