/*
 * A guide for new home owners.
 */
inherit "/std/book";
#include <room/newspaper.h>

#define AUTO_LOAD_TAG "newspaper"

private string _paper;
private int _edition;

void setup() {
  set_name("newspaper");
  set_short("newspaper");
  add_alias("paper");
  add_plural("papers");
  set_long("A bunch of nice newspapers.\n");
  set_main_plural("newspapers");
  set_weight(10);
  set_value(10);
  set_ignore_saved_pages(1);
}

int query_binding_force() {
  return 100;
}

void setup_paper() {
   class article* articles;
   class advert* adverts;
   class advert data;
   string* bits;
   int i;
   int j;
   string header;
   string index;
   string fluff;
   int page;
   mixed* stuff;
   object ob;

   if (_paper && _edition) {
      bits = explode(_paper, " ");
      if (bits[0] != "The") {
         set_short("copy of the " + _paper);
         add_adjective( ({ "the" }) );
         set_main_plural("copies of the " + _paper);
      } else {
         set_short("copy of " + _paper);
         set_main_plural("copies of " + _paper);
         add_plural_adjective("copies");
      }
      bits = map(bits, (: lower_case($1) :));
      add_adjective( ({ "copy", "of" }) );
      add_adjective(bits[0..<2]);
      add_alias(lower_case(bits[<1]));
      //if (bits[0] == "the") {
      add_property("determinate", "a ");
      //} else {
         //add_property("determinate", "The ");
      //}
      page = query_open_page();
      articles = NEWSPAPER_HANDLER->query_edition_articles(_paper, _edition);
      set_no_pages(sizeof(articles) + 1);
      index = "Edition #" + _edition + " of " + _paper + ".\n%^BOLD%^" +
              NEWSPAPER_HANDLER->query_paper_headline(_paper, _edition) +
              "%^RESET%^\n\n";
      index += sprintf("%2d .... %70-=s\n", 1, "Contents");
      for (i = 0; i < sizeof(articles); i++) {
         set_open_page(i + 2);
         ob = query_current_page();
         ob->set_value(0);
         if (articles[i]->colour) {
            index += "%^" + articles[i]->colour + "%^";
         }
         switch (articles[i]->type) {
         case NEWSPAPER_ARTICLE_TYPE_ADVERTS :
            header = "%^BOLD%^%^CYAN%^'Adverts'%^RESET%^\n\n";
            adverts = NEWSPAPER_HANDLER->query_edition_adverts(_paper, _edition);
            stuff = unique_array(adverts, (: $1->category :) );
            foreach (adverts in stuff) {
               header += "\n$I$0=" + adverts[0]->category + "\n$I$3=   ";
               foreach (data in adverts)  {
                  header += data->text + "\n-- " + data->author + "\n\n";
               }
            }
            index += sprintf("%2d .... %70-=s\n", i + 2, articles[i]->title);
            break;
         case NEWSPAPER_ARTICLE_TYPE_SECTION :
            header = "%^BOLD%^" + articles[i]->title + "'%^RESET%^\n\n";
            for (j = i + 1; j < sizeof(articles); j++) {
               if (articles[j]->type == NEWSPAPER_ARTICLE_TYPE_SECTION) {
                  break;
               }
               header += sprintf("%2d .... %70-=s\n", i + 2, articles[j]->title);
            }
            index += sprintf("%2d -=== %65|=s ===-\n", i + 2, articles[i]->title);
            break;
         case NEWSPAPER_ARTICLE_TYPE_HTML :
         case NEWSPAPER_ARTICLE_TYPE_PLAIN :
            header = "%^BOLD%^%^CYAN%^'" + articles[i]->title + "'%^RESET%^ at " +
                     ctime(articles[i]->date_written)[0..10] + "\n" +
                     "by " + articles[i]->shown_name;
            header += NEWSPAPER_HANDLER->query_article_text_plain(articles[i]);
            index += sprintf("%2d .... %70-=s\n", i + 2, articles[i]->title);
            break;
         }
         if (articles[i]->colour) {
            index += "%^RESET%^";
         }
         set_read_mess( header, NEWSPAPER_HANDLER->query_language_for( _paper ),
             0 );
      }
      set_open_page(1);
      set_read_mess( index, NEWSPAPER_HANDLER->query_language_for( _paper ), 0);
      ob = query_current_page();
      ob->set_value(0);

      fluff = NEWSPAPER_HANDLER->query_paper_long(_paper);
      if (fluff[<1] != '\n') {
         fluff += "\n";
      }
      set_long(fluff + "Edition " + _edition + " of " + _paper + ".\n");
      set_open_page(page);
   }
} /* setup_paper() */

void set_paper(string paper) {
   _paper = paper;
   set_long(NEWSPAPER_HANDLER->query_paper_long(_paper));
   setup_paper();
} /* set_paper() */

void set_edition(int edition) {
   _edition = edition;
   setup_paper();
} /* set_edition() */

mapping query_dynamic_auto_load() {
   mapping map;

   map = ::query_dynamic_auto_load();
   add_auto_load_value(map, AUTO_LOAD_TAG, "paper", _paper);
   add_auto_load_value(map, AUTO_LOAD_TAG, "edition", _edition);
   return map;
} /* query_dynamic_auto_load() */

void init_dynamic_arg(mapping map, object player) {
   ::init_dynamic_arg(map);
   set_paper(query_auto_load_value(map, AUTO_LOAD_TAG, "paper"));
   set_edition(query_auto_load_value(map, AUTO_LOAD_TAG, "edition"));
} /* init_dynamic_arg() */
