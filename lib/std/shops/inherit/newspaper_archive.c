/**
 * This is the newspaper archive inherit.  Allows you to buy back issues of the
 * paper.
 * @author Pinkfish
 * @started Wed May 23 15:56:13 PDT 2001
 */
inherit "/std/shops/inherit/shop_event";
#include <room/newspaper.h>
#include <shops/newspaper_archive.h>
#include <money.h>
#include <move_failures.h>

private nosave string _area;
private nosave string _paper;

mixed query_property(string name);

void create() {
   shop_event::create();
} /* create() */

/**
 * This sets the paper area to use for getting information from the archive.
 * @param area the area to set
 */
void set_paper_area(string area) {
   _area = area;
} /* set_paper_area() */

/**
 * This method returns the paper area for use in getting information form
 * the archive.
 * @return the paper area
 */
string query_paper_area() {
   return _area;
} /* query_paper_area() */

/**
 * This sets the paper to use for getting information from the archive.
 * @param area the area to set
 */
void set_paper(string paper) {
   _paper = paper;
} /* set_paper() */

/**
 * This method returns the paper for use in getting information form
 * the archive.
 * @return the paper area
 */
string query_paper() {
   return _paper;
} /* query_paper() */

/**
 * This method shows which papers are currently available for buying back
 * issues of.
 */
int do_list_papers() {
   string paper;
   string* papers;
   string str;
   int cost;
   string place;

   place = query_property("place");

   if (_paper) {
      papers = ({ _paper });
   } else {
      papers = NEWSPAPER_HANDLER->query_papers_in_area(_area);
   }
   if (!sizeof(papers)) {
      if (!broadcast_shop_event(NEWSPAPER_ARCHIVE_EVENT_NO_PAPERS, this_player())) {
         add_failed_mess("There do appear to be any newspapers to get "
                         "back issues of.\n");
      }
      return 0;
   }

   str = "Back issues of the following papers:\n";
   foreach (paper in papers) {
      cost = NEWSPAPER_HANDLER->query_paper_cost(paper) * 10;
      if (NEWSPAPER_HANDLER->query_last_edition_num(paper)) {
         if (NEWSPAPER_HANDLER->query_last_edition_num(paper) > 1) {
            str += "$I$5=" + paper + " issues, 1 to " + 
                   NEWSPAPER_HANDLER->query_last_edition_num(paper);
         } else {
            str += "$I$5=" + paper + " issue, 1";
         }
         str += " for " + MONEY_HAND->money_value_string(cost, place);
      } else {
         str += "$I$5=" + paper + " has no back issues.\n";
      }
      str += ".\n";
   }

   if (!broadcast_shop_event(NEWSPAPER_ARCHIVE_EVENT_LIST, this_player(),
                             str, papers)) {
      write("$P$Archives$P$" + str);
   }
   return 1;
} /* do_list_papers() */

/**
 * This method buys a back issue of the paper.
 * @param paper the paper to buy a back issue of
 * @param issue the issue number to buy
 */
int do_buy(string paper, int edition) {
   string found_paper;
   int pos;
   object ob;
   string place;
   string* papers;
   int cost;
   int* editions;

   place = query_property("place");

   if (_paper) {
      papers = ({ _paper });
   } else {
      papers = NEWSPAPER_HANDLER->query_papers_in_area(_area);
   }
   if (strlen(paper) == 1) {
      pos = lower_case(paper)[0] - 'a';
      if (pos < 0 || pos >= sizeof(papers)) {
         found_paper = papers[pos];
      }
   } else {
      for (pos = 0; pos < sizeof(papers); pos++) {
         if (lower_case(paper) == lower_case(papers[pos])) {
            found_paper = papers[pos];
         }
      }
   }

   if (!found_paper) {
      if (!broadcast_shop_event(NEWSPAPER_ARCHIVE_EVENT_INVALID_PAPER,
                                this_player(), paper)) {
         add_failed_mess("There is no paper called " + paper + ".\n");
      }
      return 0;
   }

   if (edition < 0 ||
       edition > NEWSPAPER_HANDLER->query_last_edition_num(found_paper)) {
      if (!broadcast_shop_event(NEWSPAPER_ARCHIVE_EVENT_INVALID_EDITION,
                                this_player(), paper, edition)) {
         add_failed_mess("There is no edition " + edition + " of the "
                         "paper " + found_paper + ".\n");
      }
      return 0;
   }

   // Check their money.
   cost = NEWSPAPER_HANDLER->query_paper_cost(found_paper) * 10;
   if (this_player()->query_value_in(place) < cost) {
      if (!broadcast_shop_event(NEWSPAPER_ARCHIVE_EVENT_TOO_POOR,
                                this_player(), paper, edition, cost)) {
         add_failed_mess("You need " +
                         MONEY_HAND->money_value_string(cost, place) +
                         " to buy a back issue of " + found_paper + ".\n");
      }
      return 0;
   }

   ob = clone_object("/obj/misc/newspaper");
   ob->set_paper(found_paper);
   ob->set_edition(edition);
   if (!broadcast_shop_event(NEWSPAPER_ARCHIVE_EVENT_MOVE_PAPER,
                             this_player(), paper, edition, cost, ob)) {
      if (ob->move(this_player()) != MOVE_OK) {
         ob->move(this_object());
         write("Unable to move the paper into your inventory, putting it "
               "on the floor.\n");
      }
   }
   this_player()->pay_money(MONEY_HAND->create_money_array(cost, place), place);

   if (!broadcast_shop_event(NEWSPAPER_ARCHIVE_EVENT_BOUGHT,
                             this_player(), paper, edition, cost, ob)) {
      add_succeeded_mess("$N buy$s edition " + edition + " of " +
                         found_paper + " for " +
                         MONEY_HAND->money_value_string(cost, place) + ".\n");
   }

   // Put that they bought this into their bought array.
   editions = this_player()->query_property("Paper " + found_paper);
   if (!editions) {
      editions = ({ });
   }
   if (member_array(edition, editions) == -1) {
      editions += ({ edition });
      this_player()->add_property("Paper " + found_paper, editions);
      NEWSPAPER_HANDLER->add_edition_paper_sold(_paper, edition,
                        cost);
   }
   return 1;
} /* do_buy() */

void init() {
   add_command("list", "[papers]", (: do_list_papers() :));
   add_command("buy", "<number'edition number'> of <string'paper name'>",
                (: do_buy($4[1], $4[0]) :));
} /* init() */
