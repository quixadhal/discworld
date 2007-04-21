/**
 * This method will buy the objects from the specified player.
 * @author Pinkfish
 * @started Mon Aug 17 07:45:07 EDT 1998
 */
#include <cmds/offer.h>
#include <money.h>
#include <move_failures.h>

inherit "/cmds/base";

int is_in(object ob, object source);

int do_buy(object *people) {
   object person;
   int ret;
   class offer_sale wombat;
   object *obs;
   object *moved;
   mixed *payment;
   object ob;
   int i;

   foreach (person in people) {
      wombat = this_player()->query_respond_command("sell", person);
      if (wombat) {
         // Check to make sure the objects are still in their inventory.
         obs = filter(wombat->sell_obs, (: is_in($1, $2) :),
                      wombat->seller);
         if (sizeof(obs) != sizeof(wombat->sell_obs)) {
            add_failed_mess("$I no longer has " +
                            query_multiple_short(wombat->sell_obs - obs) +
                            " to buy.\n",
                            ({ person }) );
         } else if (this_player()->query_value_in(wombat->place) <
                    wombat->value) {
            add_failed_mess("You no longer have enough money to buy " +
                            query_multiple_short(wombat->sell_obs) +
                            " from $I.\n", ({ person }));
         } else {
            // Get money (hopefully) from the nice array of money
            // the other person has.
            payment = MONEY_HAND->query_person_payments(wombat->value,
                                                        wombat->place,
                                                        this_player(), person);
            if (!payment) {
               add_failed_mess("You no longer have the correct change " 
                               "to pay for $I.\n", ({ person }) );
            } else {
               // Lets see if we can move stuff first.
               moved = ({ });
               foreach (ob in wombat->sell_obs) {
                  if (ob->move(this_player()) == MOVE_OK) {
                     moved += ({ ob });
                  }
               }
               // Check to see if all the objects were moved.
               if (sizeof(moved) != sizeof(wombat->sell_obs)) {
                  moved->move(person);
                  tell_object(person, "Unable to move " +
                                      query_multiple_short(wombat->sell_obs -
                                                           moved) +
                                      " out of your inventory, cancelling "
                                      "the sale.\n");
                  add_failed_mess("Unable to move some objects out "
                                  "of the inventory of $I, canceling sale.\n",
                                  ({ person }) );
               } else {
                  if (sizeof(payment[MONEY_PAY_CHANGE])) {
                     tell_object(person, "You accept " +
                            MONEY_HAND->money_string(payment[MONEY_PAY_RETURN],
                                                       wombat->place)+
                              " from " + this_player()->the_short() + 
                              " and give back " +
                            MONEY_HAND->money_string(payment[MONEY_PAY_CHANGE],
                                                       wombat->place) +
                              " in change.\n");
                     tell_object(this_player(), "You give " +
                            MONEY_HAND->money_string(payment[MONEY_PAY_RETURN],
                                                       wombat->place)+
                              " to " + person->the_short() +
                              " and receive back " +
                            MONEY_HAND->money_string(payment[MONEY_PAY_CHANGE],
                                                       wombat->place) +
                              " in change.\n");
                  } else {
                     tell_object(person, "You accept " +
                            MONEY_HAND->money_string(payment[MONEY_PAY_RETURN],
                                                       wombat->place) +
                              " from " + this_player()->the_short() +".\n");
                     tell_object(this_player(), "You give " +
                            MONEY_HAND->money_string(payment[MONEY_PAY_RETURN],
                                                       wombat->place) +
                              " to " + person->the_short() + ".\n");
                  }
                  for (i = 0; i < sizeof(payment[MONEY_PAY_RETURN]); i += 2) {
                     this_player()->adjust_money(-payment[MONEY_PAY_RETURN][i+1],
                                                 payment[MONEY_PAY_RETURN][i]);
                     person->adjust_money(payment[MONEY_PAY_RETURN][i+1],
                                          payment[MONEY_PAY_RETURN][i]);
                  }
                  for (i = 0; i < sizeof(payment[MONEY_PAY_CHANGE]); i += 2) {
                     person->adjust_money(-payment[MONEY_PAY_CHANGE][i+1],
                                                 payment[MONEY_PAY_CHANGE][i]);
                     this_player()->adjust_money(payment[MONEY_PAY_CHANGE][i+1],
                                          payment[MONEY_PAY_CHANGE][i]);
                  }
                  add_succeeded_mess("$N buy$s " +
                                     query_multiple_short(wombat->sell_obs) +
                                     " from $I.\n", ({ person }) );
                  ret++;
               }
            }
         }
         this_player()->remove_respond_command("sell", person);
      } else {
         add_failed_mess("Sorry, $I has not offered to sell you anything.\n",
                         ({ person }) );
      }
   }
   return ret;
} /* do_buy() */

/**
 * This method checks to make sure that ob is contained somewhere in the
 * source.
 */
int is_in(object ob, object source) {
   object env;

   if (!ob) {
      return 0;
   }
   env = ob;
   do {
      env = environment(env);
      if (env == source) {
         return 1;
      }
   } while (env);
   return 0;
} /* is_in() */

mixed *query_patterns() {
   return ({ "offer from <indirect:living'person'>", (: do_buy($1) :) });
} /* query_patterns() */

/* This is needed to so we can move stuff out of the players inventory. */
int query_theft_command() {
   return 1;
} /* query_theft_command() */
