/**
 * A dispenser for papers.  It will sell papers to people for money!
 * @author Pinkfish
 * @started Thu May 24 15:51:25 PDT 2001
 */
inherit "/std/object";
#include <room/newspaper.h>
#include <money.h>
#include <move_failures.h>

private string _paper;

void setup() {
   set_name("box");
   set_short("newspaper box");
   add_adjective("box");
   add_help_file("newspaper_box");
   reset_get();
} /* setup() */

/**
 * This sets the current paper that will be dispensed.
 * @param paper the paper to dispense
 */
void set_paper(string paper) {
   int cost;
   string place;
   string* bits;

   _paper = paper;

   place = query_property("place");
   if (!place) {
      if (environment()) {
         place = environment()->query_property("place");
      } else if (previous_object()) {
         place = previous_object()->query_property("place");
      }
   }
   cost = NEWSPAPER_HANDLER->query_paper_cost(paper);
   set_short(_paper + " box");
   set_long("This is a battered looking metal box that is full of copies of " +
            (lower_case(paper)[0..3] == "the "?"":"the ") +
            paper + ".  There is a door on the front you could pull open "
            "beside which is small white writing saying " +
            MONEY_HAND->money_value_string(cost, place) + ".\n");
   if (lower_case(_paper)[0..3] == "the ") {
      add_property("determinate", "");
   }
   bits = explode(lower_case(_paper), " ");
   add_adjective(bits);
} /* set_paper() */

/** @ignore yes */
int do_buy() {
   int cost;
   int edition;
   string place;
   object ob;
   int *editions;

   place = query_property("place");
   if (!place) {
      place = environment()->query_property("place");
   }
   cost = NEWSPAPER_HANDLER->query_paper_cost(_paper);
   edition = NEWSPAPER_HANDLER->query_last_edition_num(_paper);
   if (!edition) {
      add_failed_mess("There is no edition to buy.\n");
      return -1;
   }

   if (this_player()->query_value_in(place) < cost) {
      add_failed_mess("You do not have enough money to pay for " +
                      _paper + ", you need " +
                      MONEY_HAND->money_value_string(cost, place) +
                      ".\n");
      return -1;
   }

   this_player()->pay_money(MONEY_HAND->create_money_array(cost, place), place);

   ob = clone_object("/obj/misc/newspaper");
   ob->set_paper(_paper);
   ob->set_edition(edition);
   if (ob->move(this_player()) != MOVE_OK) {
      ob->move(environment(this_player()));
      write("Unable to move the paper into your inventory, putting it on "
            "the ground.\n");
   }

   editions = this_player()->query_property("Paper " + _paper);
   if (!editions) {
      editions = ({ });
   }
   if (member_array(edition, editions) == -1) {
      editions += ({ edition });
      this_player()->add_property("Paper " + _paper, editions);
      // We only keep track of unique sales.
      NEWSPAPER_HANDLER->add_edition_paper_sold(_paper, edition,
                        NEWSPAPER_HANDLER->query_paper_cost(_paper));
   } else {
      NEWSPAPER_HANDLER->add_edition_revenue(_paper, edition,
                        NEWSPAPER_HANDLER->query_paper_cost(_paper));
   }

   add_succeeded_mess("$N $V a paper from $D.\n");
   return 1;
} /* do_buy() */

void init() {
   add_command("buy", "paper from <direct:object>", (: do_buy() :));
   add_command("pull", "[door] [on] <direct:object>", (: do_buy() :));
   add_command("pull", "open <direct:object>", (: do_buy() :));
} /* init() */
