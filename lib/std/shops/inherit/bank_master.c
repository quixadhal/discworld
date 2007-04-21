/**
 * This is the bank's main branch inherit.  Allows people to trade in
 * credit notes for the money, or deposit credit notes.
 * @author Pinkfish
 * @started Fri Jun 15 01:05:55 PDT 2001
 */
inherit "/std/shops/inherit/bank_base";
#include <money.h>

int query_float();

/**
 * Processes the specified credit objects to figure out if they need to deposit
 * or withdraw money.
 * @param obs the objects to process
 * @return 1 on success, 0 on failure
 */
int do_process(object *obs) {
   object ob;
   int amount;
   int found;
   object* not_credit;
   object* not_valid;
   string place;

   place = query_money_place();

   not_valid = ({ });
   not_credit = ({ });
   foreach (ob in obs) {
      if (ob->query_credit_note() ||
          ob->query_bank_name() != query_bank_name()) {
         not_credit += ({ ob });
         obs -= ({ ob });
      } else if (ob->is_valid_note()) {
         amount += ob->query_credit_balance();
      } else {
         not_valid += ({ ob });
         obs -= ({ ob });
      }
   }

   if (!sizeof(obs)) {
      if (sizeof(not_credit)) {
         add_failed_mess("None of $I are credit notes.\n", not_credit);
      }
      if (sizeof(not_valid)) {
         add_failed_mess("None of $I are valid credit notes.\n", not_valid);
      }
      return 0;
   }

   foreach (ob in obs) {
      amount = ob->query_amount();
      if (amount > 0) {
         // This means they are depositing money (overall).
         if (this_player()->query_value_in(query_money_place()) < amount) {
            add_failed_mess("You do not have the needed " +
                            MONEY_HAND->money_value_string(amount, place) +
                            " to deposit.\n");
            continue;
         }
         else if (ob->do_process()) {
            this_player()->pay_money(MONEY_HAND->create_money_array(amount,
                   place), place);
            // Tell them all they have been processed.
            add_succeeded_mess("$N $V depositing " +
                               MONEY_HAND->money_value_string(amount, place) +
                               " from $I.\n", ({ ob }));
            found++;
         } else {
            add_failed_mess("You cannot process $I for some reason.\n",
                            ({ ob }));
            continue;
         }
      } else if (amount < 0) {
         // They are withdrawing money (overall).
         //if (query_float() < -amount) {
            //add_failed_mess("This place does not have enough money to trade in your "
                            //"slips for " +
                            //MONEY_HAND->money_value_string(-amount, place) +
                            //".\n");
         //}
         if (ob->do_process()) {
            this_player()->adjust_money(MONEY_HAND->create_money_array(-amount, place));
            add_succeeded_mess("$N $V withdrawing " +
                               MONEY_HAND->money_value_string(-amount, place) +
                               " from $I.\n", ({ ob }));
            found++;
         } else {
            add_failed_mess("You cannot process $I for some reason.\n",
                            ({ ob }));
            continue;
         }
      }
      ob->move("/room/rubbish");
   }
   return found;
} /* do_process() */

void init() {
   ::init();
   add_command("process", "<indirect:object>");
} /* init() */
