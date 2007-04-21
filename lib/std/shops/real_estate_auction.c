inherit "/std/shops/auction_house";

#include <housing.h>
#include <money.h>
#include <move_failures.h>
#include <room/auction.h>
#include <nomic_system.h>
#include <morpork.h>

#define AUCTION_LENGTH (3600*24*5)
#define NEW_AGREEMENT_COST 4000

#define REAL_ESTATE_NEW_LOT_BIDDING "new_low_bidding"

private nosave string* _areas;
private nosave string _city;
private nosave string _language;

int add_house(string house);
int check_can_bid(object person,
                  class auction which,
                  int bid_amount);

void create() {
   _areas = ({ });
   _city = "Ankh-Morpork";
   _language = "morporkian";
   ::create();
   add_help_file("real_estate");
} /* create() */

void reset()
{
   call_out("after_reset", 10);
}

/**
 * This method adds in an area to which the real estate office will look
 * for houses.
 * @param area the area to look in
 */
void add_housing_area(string area) {
   _areas += ({ area });
} /* add_housing_area() */

/**
 * This method returns all the areas in which the real estate office will
 * look for houses.
 * @return the areas to look for houses
 */
string* query_housing_areas() {
   return _areas;
} /* query_housing_areas() */

/**
 * This runs through the list of vacant houses and checks to see if it
 * should add them up for auction or not.
 */
void after_reset()
{
   string house;
   string *available;
   int i;
   string area;

   // Find available houses and start an auction for them.
   foreach (area in _areas) {
      available = HOUSING->query_vacant(area);

      if (sizeof(available)) {
        i = 1;
        foreach(house in available) {
          call_out("add_house", i++, house);
        }
      }
   }
}

/**
 * Add a house for auction.
 * @string house the path of the house
 * @return 1 on success, 0 on failure
 */
int add_house(string house)
{
   object agreement;
   string text;
   int tim;

   //Don't auction houses that don't really exist
   if (member_array(house, keys(HOUSING->query_houses())) == -1) {
      return 0;
   }

   // Sanity check to make sure houses are only listed once.
   if(lower_case(HOUSING->query_owner(house)) != "for sale") {
     log_file("REAS", ctime(time()) + ": attempt to add house %s that's "
              "not for sale.\n", house);
     return 0;
   }
   
   // Reset information about the house.
   HOUSING->set_rent(house, 0);
   agreement = clone_object(AGREEMENT);
   agreement->set_city(_city);
   agreement->set_language(_language);
   agreement->set_house(house);
   text = HOUSING->query_address(house);
   tim = query_last_auction_finish_time();
   if (tim + 60 * 60 < time() + query_auction_times()["very long"]) {
      tim = 0;
   } else {
      tim = tim + 60 * 60 - time();
   }
   if (add_item_to_auction(({ agreement }), text,
                           400, "Patrician", tim,
                           "housing_auction", house)) {
      HOUSING->set_under_offer(house);
      broadcast_shop_event(REAL_ESTATE_NEW_LOT_BIDDING, this_player(),
                           house);
      log_file("REAS", ctime(time()) + ": Added house %s\n", house);
      return 1;
   }
}

/**
 * This function is called during the various phases of the housing
 * auction.  When each phase ends this method is called and anything
 * that needs to be done for the end of the phase is sorted out.
 * @param event_type the event that has finished
 * @param auct the auction that just finished
 * @param house the path of the house being auctioned
 * @param buyer the buyer of the house (if sold)
 * @param cost how much the house costs
 * @param obs the things that were sold (rental agreementsin this case)
 */
void housing_auction(int event_type,
                     class auction auct,
                     string house,
                     string buyer,
                     int cost,
                     object *obs)
{
   object book;
   //If we didn't sell it with the first reserve price,
   //try again with a smaller one. If we did sell it,
   //set the new owner!

   switch (event_type) {
   case AUCTION_WITHDRAW_PHASE:
      // Mark the house as For Sale again.
     if(lower_case(HOUSING->query_owner(house)) != "under offer") {
       write("House is not currently under offer!\n");
       log_file("REAS", ctime(time()) + ": attempted to withdraw %s when it "
                "was not 'Under Offer'.\n", house);
     } else {
       HOUSING->set_for_sale(house);
       log_file("REAS", ctime(time()) + ": %s back to For Sale.\n", house);
     }
     break;
   case AUCTION_CLAIM_PHASE:
      HOUSING->set_owner(house, buyer);
      HOUSING->set_value(house, cost);
      obs[0]->set_city(_city);
      obs[0]->set_language(_language);
      obs[0]->set_house(house);
      log_file("REAS", ctime(time()) + ": %O sold to %O for %O.\n",
               house, buyer,
               MONEY_HAND->money_value_string(cost, query_property("place")));
      //Throw in a copy of the home owners guide.
      book = clone_object(BOOK);
      if (book) {
         if (book->move(this_player()) != MOVE_OK) {
            book->move(this_object());
         }
      }
      write("A nice new house owner's guide pops up for you.\n");
      break;
   }
}

/**
 * Request a new rental agreement for a house they might have
 * lost.
 */
int do_agreement()
{
   string *houses;
   int i;
   string place;

   place = query_property("place");
   if (this_player()->query_value_in(place) < NEW_AGREEMENT_COST) {
      add_failed_mess("You need " +
                      MONEY_HAND->
                      money_value_string(NEW_AGREEMENT_COST,
                                         place) +
                      " to get a new agreement.\n");
      return 0;
   }

   houses = filter(keys(HOUSING->query_houses()),
                   (: HOUSING->query_owner($1) == $2 :),
                   this_player()->query_name());
   if (!sizeof(houses)) {
      add_failed_mess("You are not renting any houses.\n");
      return 0;
   }
   for (i = 0; i < sizeof(houses); i++) {
      printf("%c) %s %s\n", i + 'A',
             HOUSING->query_address(houses[i]),
             HOUSING->query_region(houses[i]));
   }
   write("It will cost you " + MONEY_HAND->
                      money_value_string(NEW_AGREEMENT_COST,
                                         place) + " for a new "
         "agreement.\n");
   write("Which house to choose? ");
   input_to("new_agreement_choice", 0, houses);
   add_succeeded_mess(({ "",
                         "$N enquires about a new rental agreement.\n" }));
   return 1;
}                               /* do_agreement() */

/** @ignore yes */
protected void new_agreement_choice(string choice,
                          string *houses)
{
   int index;
   object agreement;
   string place;

   place = query_property("place");
   choice = lower_case(choice);
   if (!strlen(choice)) {
      write("Ok, canceling agreement check.\n");
      return;
   }

   index = choice[0] - 'a';
   if (index < 0 || index >= sizeof(houses)) {
      write("Choice out of bounds.\n");
      return;
   }

   if (this_player()->query_value_in(place) < NEW_AGREEMENT_COST) {
      add_failed_mess("You need " +
                      MONEY_HAND->
                      money_value_string(NEW_AGREEMENT_COST,
                                         place) +
                      " to get a new agreement.\n");
      return 0;
   }

   this_player()->pay_money(MONEY_HAND->
                            create_money_array(NEW_AGREEMENT_COST, place));
   agreement = clone_object(AGREEMENT);
   agreement->set_city(_city);
   agreement->set_language(_language);
   agreement->set_house(houses[index]);
   if (agreement->move(this_player()) != MOVE_OK) {
      write("Unable to move the agreement into your inventory.\n");
      return 0;
   }
   write("Created a new agreement for " +
         HOUSING->query_address(houses[index]) + ".\n");
}                               /* new_agreement_choice() */

/**
 * This method checks to make sure they have enough money in their bank
 * accounts to make the bid.  This function should be setup to be called
 * as the big checking function in the inherit of this room.
 * @param person the person being checked for bidding
 * @param which the auction being tested to be bid on
 * @param bid_amount how much they are bidding
 */
int check_can_bid(object person,
                  class auction which,
                  int bid_amount)
{
   int total_money,
     total_bids;
   string *accounts;
   string account;
   class auction auction;

   accounts = "/obj/handlers/bank_handler"->query_accounts(person->
                                                           query_name());
   foreach(account in accounts) {
      total_money += "/obj/handlers/bank_handler"->query_account(person->
                                                                 query_name(),
                                                                 account);
   }

   foreach(auction in query_auctions()) {
      if (auction->current_bidder &&
          lower_case(auction->current_bidder) == person->query_name()) {
         total_bids += auction->bid;
      }
   }

   if (total_bids + bid_amount > total_money * 3) {
      add_failed_mess
         ("You do not have sufficient funds in your bank accounts "
          "to cover such a bid.\n");
      return 0;
   }

   return 1;
}                               /* check_can_bid() */

/** @ignore yes */
void init()
{
   ::init();
   add_command("request", "[agreement]", (: do_agreement() :));
}                               /* init() */

/**
 * This function runs down the list of current auctions and makes sure they 
 * are all registered as "Under Offer" so if for some reason they are not, 
 * they won't be put up for sale again.
 */
void reset_auctions_to_under_offer() {
    class auction auc;
    
    foreach( auc in query_auctions() ) {
        HOUSING->set_under_offer(auc->extra);
    }
    
} /* reset_auctions_to_under_offer() */

/**
 * This is just a cosmetic function to help set up the agreements. 
 *
 * @param where the city to which the agreements will say the property 
 * defaults to when you cease to own it
 */
void set_city( string where ) {
    _city = where;
} /* set_city() */

/**
 * This is just a cosmetic function to help set up the agreements. 
 *
 * @param the agreements be written in
 */
void set_language( string lang ) {
    _language = lang;
} /* set_language() */

/**
 * Returns the city the real estate place thinks it lives in.
 */
string query_city() {
    return _city;
} /* query_city() */

/**
 * Returns the language the real estate place will use for its agreements.
 */
string query_language() {
    return _language;
} /* query_language() */

mixed *stats() {
  return ::stats() +
    ({ 
        ({ "areas", _areas }),
        ({ "city", _city }),
        ({ "language", _language }),
    });
}
