/**
 * This command offers to sell an object to another thingy.
 * @author Pinkfish
 * @started Mon Aug 17 07:02:04 EDT 1998
 */
#include <money.h>
#include <cmds/offer.h>

inherit "/cmds/base";

int offer_sale(object *to_sell, object *sell_to, string amt, int each) {
   int value;
   int num;
   string *fluff;
   string place;
   string *gumboot;
   object *fail;
   class offer_sale wombat;

   /* Try and figure out the value... */
   fluff = MONEY_HAND->query_all_places();
   gumboot = ({ });
   foreach (place in fluff) {
      value = MONEY_HAND->value_from_string(amt, place);
      if (value) {
        gumboot += ({ place });
      }
   }

   if (!sizeof(gumboot)) {
      add_failed_mess("Unable to understand the value of " + amt + ".\n",
                      to_sell);
      return 0;
   }

   if (member_array("Ankh-Morpork", gumboot) != -1) {
      gumboot = ({ "Ankh-Morpork" });
   }

   if (sizeof(gumboot) > 1) {
      add_failed_mess("Too many types of currency matched, matched currency "
                      "from " + query_multiple_short(gumboot) + ", try and "
                      "be more specific.  ie:  Ankh-Morpork dollars, not just "
                      "dollars.\n",
                      to_sell);
      return 0;
   }

   fail = this_player()->query_ignoring(sell_to);
   if (sizeof(fail)) {
      sell_to -= fail;
      if (!sizeof(sell_to)) {
         add_failed_mess("You cannot offer anything to $I since you are "
                         "ignoring them.\n", fail);
         return 0;
      }
   }

   fail = this_player()->query_ignored_by(sell_to);
   if (sizeof(fail)) {
      sell_to -= fail;
      if (!sizeof(sell_to)) {
         add_failed_mess("You cannot offer anything to $I since they are "
                         "ignoring you.\n", fail);
         return 0;
      }
   }

   /* Ok, so value must be the amount and gumboot[0] is the place. */
   place = gumboot[0];
   value = MONEY_HAND->value_from_string(amt, place);

   if (value < 0) {
      add_failed_mess("The value of the item was set too high.\n");
      return 0;
   }

   if (sizeof(sell_to) > 1) {
      add_failed_mess("Attempted to sell to $I, cannot sell to more than "
                      "one person.\n", sell_to);
      return 0;
   }

   if (sell_to[0] == this_player()) {
      add_failed_mess("You cannot sell $I to yourself.\n", to_sell);
      return 0;
   }

   if (each) {
      num = 0;
      
      foreach(object item in to_sell) { 
          // If it's collective, add up the individual number.
          if ( item->query_collective() || item->query_continuous() ) { 
             num += item->query_amount(); 
          }
          else {
            // If it's not, then it's just one item.
             num++;
          }
      }
      
      // Now multiply the value by the number of items.
      value = value * num;
   }

#ifdef CHECK_MONEY
   /* Check to see that the person has enough money. */
   if (sell_to[0]->query_value_in(gumboot[0]) < value) {
      add_failed_mess(sell_to[0]->the_short() + " does not have enough money "
                      "to buy $I.\n", to_sell);
      return 0;
   }

   /* And the right sort! of money. */
   if (!MONEY_HAND->query_person_payments(value, gumboot[0], sell_to[0], this_player())) {
      add_failed_mess(sell_to[0]->the_short() + " does not have the correct "
                      "change to pay " +
                      MONEY_HAND->money_value_string(value, gumboot[0]) +
                      " (and you do not have change to give back).\n");
      return 0;
   }
#endif

   wombat = new(class offer_sale);
   wombat->sell_obs = to_sell;
   wombat->seller = this_player();
   wombat->buyer = sell_to[0];
   wombat->value = value;
   wombat->place = gumboot[0];
   sell_to->add_respond_command("sell", this_player(), wombat);
   tell_object(sell_to[0],
               "You can accept the offer by typing:\n"
               "accept offer from " + this_player()->query_name() + "\n");
/*
   tell_object(this_player(), "You can withdraw the offer by typing in:\n"
                              "withdraw offer from " +
                              sell_to[0]->query_cap_name() + "\n");
 */
   /* Ok!  Now offer the sale. */
   add_succeeded_mess("$N offer$s to sell $I to " + sell_to[0]->the_short() +
                      " for " + MONEY_HAND->money_value_string(value,
                                            gumboot[0]) + ".\n", to_sell);

   return 1;
} /* offer_sale() */

mixed *query_patterns() {
   return ({
 "<indirect:object:me'sell object'> to <indirect:living> for <string>",
             (: offer_sale($1[0], $1[1], $4[2], 0) :),
 "<indirect:object:me'sell object'> to <indirect:living> for <string> each",
             (: offer_sale($1[0], $1[1], $4[2], 1) :) });
} /* query_patterns() */
