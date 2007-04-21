// Pinkfish
// Started Wed May 30 21:37:15 PDT 2001

inherit "/std/room/furniture/games/card_base";
inherit "/std/room/furniture/games/multiplayer_base";
inherit "/std/room/furniture/commercial";

#include <money.h>

#define BLACKJACK_BET 0
#define BLACKJACK_PLAYING 1
#define BLACKJACK_BUST 2
#define BLACKJACK_STAND 3

class player_data {
   class playing_card* hand;
   int starting_bet;
   int state;
}

#define BOARD_TAG "blackjack"

//
// This gives an estimated return on the payment.  For example 50%
// return would mean you get back approximately 50% of your money on
// average
//
private int _cost;
private int _finished;
private int _max_bet;
private int _min_bet;
private int _timeout_call;
private int _timeout_length;
private int _num_decks;
private int _soft_seventeen;
private int _double_on_doubles;
private class playing_card* _dealer_hand;
private class playing_card* _deck;
private class playing_card* _discard;

int query_hand_value(class playing_card* cards);

void create() {
   multiplayer_base::create();
   commercial::create();
} /* create() */

void setup() {
   set_name( "table" );
   set_short( "blackjack table" );
   add_adjective( ({ "blackjack" } ));
   add_alias("blackjack");
   set_long( "The green felt of the table looks nice and soft, there is "
             "a single deck of cards sitting in the middle of the table.  "
             "The table looks like it will seat 4 people.\n");
   set_allowed_positions(({"sitting", "lying", "kneeling", "meditating"}));
   set_allowed_room_verbs((["sitting" : "sits" ]));
   set_weight(2000);
   set_value(240000);
   // Minimum bid $1.
   _timeout_length = 5 * 60;
   _cost = 400;
   _num_decks = 1;

   add_player_id_type("green", 0);
   add_player_id_type("red", 0);
   add_player_id_type("blue", 0);
   add_player_id_type("maroon", 0);

   set_minimum_needed(1);
   add_help_file("blackjack");

   set_commercial_size(15);
   set_commercial_type("gambling");
   set_shop_use_types(({ "blackjack" }));
}

/**
 * This method shows the current status of the cards.
 * @param id the id to show the status for
 * @return the status of the cards
 */
string query_card_status() {
   string id_bing;
   string ret;
   string* not_playing;
   class player_data data;
   class playing_card card;
   string* womble;
   string place;

   ret = "";
   not_playing = ({ });
   womble = query_player_ids();
   place = environment()->query_property("place");
   foreach (id_bing in womble) {
      if (is_person_playing(id_bing) ||
          is_game_started()) {
         ret += capitalize(id_bing) + " (" +
                query_player_cap_name(id_bing) + ")";
         data = query_player_data(id_bing);
         if (data) {
            if (data->starting_bet) {
               ret += "  " + 
                      MONEY_HAND->money_value_string(data->starting_bet, place);
               if (sizeof(data->hand)) {
                  foreach (card in data->hand) {
                     ret += "  " + query_card_string(card);
                  }
               }
               if (data->state == BLACKJACK_STAND) {
                  ret += " (stand)";
               } else if (data->state == BLACKJACK_BUST) {
                  ret += " (bust)";
               }
               ret += "\n";
            } else {
               ret += "  No bet yet.\n";
            }
         } else {
            ret += "\n";
         }
      } else {
         not_playing += ({ id_bing });
      }
   }

   if (sizeof(not_playing)) {
      ret += query_multiple_short(map(not_playing, (: capitalize($1) :))) +
                               " are not playing.\n";
   }

   ret += "\n";
   if (sizeof(_dealer_hand)) {
      ret += "Dealer's cards ";
      if (!_finished) {
         ret += "XXXX";
      }
      foreach (card in _dealer_hand[(_finished?0:1)..]) {
         ret += "  " + query_card_string(card);
      }
      if (query_hand_value(_dealer_hand) > 21) {
         ret += " (bust)";
      }
      ret += "\n";
   }

   return ret;
}
   
/** @ignore yes */
string long(string str, int dark) {
   if (dark) {
      return ::long() +
             "It is too dark to make out the pieces on the board.\n";
   }
   return ::long() + query_card_status();
} /* long() */

/**
 * This method returns the next card from the deck.
 * @return the next card from the deck
 */
class playing_card query_next_card() {
   class playing_card card;

   if (!sizeof(_deck)) {
      if (!sizeof(_discard)) {
         _deck = make_deck(_num_decks, 0);
         _deck = shuffle_deck(_deck);
      } else {
         _deck = make_deck(_num_decks, 0);
         _deck = shuffle_deck(_deck);
         //_deck = shuffle_deck(_discard);
      }
      tell_all_players("The dealer shuffles the deck before the next card "
                       "is dealt.\n");
      _discard = ({});
   }
   card = _deck[0];
   _deck = _deck[1..];
   return card;
} /* query_next_card() */

/**
 * This deals cards to everyone.
 */
void deal_cards(string id) {
   class player_data data;
   string hands;

   if (!sizeof(_dealer_hand)) {
      _dealer_hand = ({ query_next_card(), query_next_card() });
   }
   // Everyone gets one card...
   hands = "";
   //foreach (id in query_currently_playing_ids()) {
      data = query_player_data(id);
      data->hand = ({ query_next_card(), query_next_card() });
      hands += query_player_cap_name(id) + " hand is: "+
                      query_card_string(data->hand[0]) + "  " +
                      query_card_string(data->hand[1]) +
                      "  (total " +
                      query_hand_value(data->hand) + ")\n";
   //}
   hands += "Dealer's cards: XXXX " +
             query_card_string(_dealer_hand[0]) + "\n"
             "You need to stay or hit yourself for more cards.\n";
   tell_all_players(hands);
} /* deal_cards() */

/** @ignore yes */
int start_game() {
   class player_data data;
   string id;

   randomise_player_numbers();
   if (!::start_game()) {
      return 0;
   }
   foreach (id in query_player_ids()) {
      data = new(class player_data);
      data->starting_bet = 0;
      data->hand = ({ });
      data->state = BLACKJACK_BET;
      set_player_data(id, data);
   }
   _dealer_hand = ({ });
   _finished = 0;
   //redeal_cards();
   tell_all_players("Place your starting bets.\n");
   return 1;
} /* reset_game() */

/**
 * This figures out the value of the cards.
 * @param cards the crds to check
 * @return the value of the hand
 */
int query_hand_value(class playing_card* cards) {
   class playing_card bing;
   int value;
   int no_aces;

   foreach (bing in cards) {
      if (bing->number == 1) {
         no_aces++;
      } else if (bing->number >= 10) {
         value +=  10;
      } else {
         value += bing->number;
      }
   }

   if (no_aces > 0) {
      if (no_aces > 1) {
         value += no_aces - 1;
      }
      if (value <= 10) {
         value += 11;
      } else {
         value += 1;
      }
   }

   return value;
} /* query_hand_value() */

/**
 * This figures out if the value is a 'soft' one of the specified type.
 * @param cards the crds to check
 * @return 1 if it is a soft result or not
 */
int is_soft_result(class playing_card* cards) {
   class playing_card bing;
   int value;
   int no_aces;

   foreach (bing in cards) {
      if (bing->number == 1) {
         no_aces++;
      } else if (bing->number >= 10) {
         value +=  10;
      } else {
         value += bing->number;
      }
   }

   if (no_aces > 0) {
      if (no_aces > 1) {
         value += no_aces - 1;
      }
      if (value <= 10) {
         return 1;
      }
   }

   return 0;
} /* query_hand_value() */

/**
 * Checks to see if all the people playing have put in their first
 * bets.
 */
void finish_bet(string id) {
   //string id;
   class player_data data;

   data = query_player_data(id);
   // Move to the next state!
   call_out("deal_cards", 2, id);
} /* check_for_finish_bet() */

void complete_round() {
   int value;
   string stuff;
   object ob;
   int new_value;
   class playing_card card;
   class player_data data;
   string id;
   string place;
   int paid;
   string* winners;
   string* losers;

   remove_call_out(_timeout_call);
   _timeout_call = 0;
   place = environment()->query_property("place");
   value = query_hand_value(_dealer_hand);
   while (value < 17 ||
          (value == 17 && _soft_seventeen && is_soft_result(_dealer_hand))) {
      _dealer_hand += ({ query_next_card() });
      value = query_hand_value(_dealer_hand);
   }


   stuff = "";
   foreach (card in _dealer_hand) {
      stuff += "  " + query_card_string(card);
   }
   if (value > 21) {
      stuff += "  Total " + value + " (bust)\n";
      value = 0;
   } else {
      stuff += "  Total " + value + "\n";
   }
   winners = ({ });
   losers = ({ });
   foreach (id in query_currently_playing_ids()) {
      data = query_player_data(id);
      if (data->state == BLACKJACK_STAND) {
         new_value = query_hand_value(data->hand);
         if (new_value == 21 && sizeof(data->hand) == 2) {
            winners += ({ id });
            // They get a pay out.
            ob = query_player_object(id);
            if (ob) {
               ob->adjust_money(MONEY_HAND->create_money_array((data->starting_bet * 5) / 2,
                                           place), place);
            }
            //_pay_out += (data->starting_bet * 3) / 2;
            //_revenue -= data->starting_bet;
            adjust_float(-(data->starting_bet * 3) / 2);
            if (ob) {
               stuff += ob->query_cap_name();
            } else {
               stuff += id;
            }
            stuff += " gets a payout of " +
                     MONEY_HAND->money_value_string((data->starting_bet * 3) / 2, place) +
                     " (plus their original money back).\n";
            paid = 1;
         } else if (new_value > value) {
            winners += ({ id });
            // They get a pay out.
            ob = query_player_object(id);
            if (ob) {
               ob->adjust_money(MONEY_HAND->create_money_array(data->starting_bet * 2,
                                           place), place);
            }
            //_pay_out += data->starting_bet;
            //_revenue -= data->starting_bet;
            adjust_float(-data->starting_bet);
            if (ob) {
               stuff += ob->the_short();
            } else {
               stuff += id;
            }
            stuff += " gets a payout of " +
                     MONEY_HAND->money_value_string(data->starting_bet, place) +
                     " (plus their original money back).\n";
            paid = 1;
         } else {
            losers += ({ id });
         }
      }
   }
   if (!paid) {
      stuff += "No one gets paid anything.\n";
   }
   // Do the dealer.
   tell_all_players("The dealer reveals their cards as " +
                    stuff);
   if (sizeof(winners)) {
      tell_room(environment(),
                query_multiple_short(winners) + " win" +
                (sizeof(winners) > 1?"":"s") + " the blackjack hand.\n");
   } else {
      tell_room(environment(),  "No one wins the blackjack hand.\n");
   }

   finish_game(0);
   _finished = 1;
} /* complete_round() */

void force_finish() {
   if (is_game_started()) {
      complete_round();
   }
} /* force_finish() */

/**
 * Places your bet.
 */
int do_bet(string str) {
   string place;
   string id;
   int amount;
   class player_data data;

   if (!is_game_started()) {
      add_failed_mess("The game has not started.\n");
      return 0;
   }

   id = find_player_id_of_person(this_player());
   if (!id) {
      add_failed_mess("You are not playing.\n");
      return 0;
   }

   place = environment()->query_property("place");

   amount = MONEY_HAND->value_from_string(str, place);
   if (!amount) {
      add_failed_mess("Invalid bet amount.\n");
      return 0;
   }
   if (this_player()->query_value_in(place) < amount) {
      add_failed_mess("You do not have that much to bid.\n");
      return 0;
   }

   if (amount < _min_bet) {
      add_failed_mess("The minimum bet for $D is " +
         MONEY_HAND->money_value_string(_min_bet, place) + ".\n");
      return 0;
   }
   if (amount > _max_bet) {
      add_failed_mess("The maximum bet for $D is " +
         MONEY_HAND->money_value_string(_max_bet, place) + ".\n");
      return 0;
   }

   data = query_player_data(id);
   if (data->starting_bet) {
      add_failed_mess("You have already bet on $D.\n");
      return 0;
   }
   this_player()->pay_money(MONEY_HAND->create_money_array(amount, place),
                            place);

   //_revenue += amount;
   adjust_float(amount);

   // This is for the starting bet.
   data->starting_bet = amount;
   data->state = BLACKJACK_PLAYING;
   finish_bet(id);
   add_succeeded_mess("$N $V " +
                      MONEY_HAND->money_value_string(amount, place) +
                      " on $D.\n");
   return 1;
} /* do_bet() */

/**
 * This hits you for another card.
 */
int do_hit() {
   int value;
   int not_done;
   string id;
   class player_data data;
   class playing_card card;

   // Get another card.
   if (!is_game_started()) {
      add_failed_mess("The game has not started.\n");
      return 0;
   }

   id = find_player_id_of_person(this_player());
   if (!id) {
      add_failed_mess("You are not playing.\n");
      return 0;
   }

   data = query_player_data(id);

   if (data->state != BLACKJACK_PLAYING) {
      add_failed_mess("You are out of the game and cannot get any more "
                      "cards.\n");
      return 0;
   }

   if (!sizeof(data->hand)) {
      add_failed_mess("You cannot hit before you have been dealt cards.\n");
      return 0;
   }

   card = query_next_card();
   data->hand += ({ card });
   value = query_hand_value(data->hand);
   if (value > 21) {
      add_succeeded_mess("$N $V and get$s " + query_card_string(card) +
                         " giving a total of " + value +
                         " and going bust on $D.\n");
      data->state = BLACKJACK_BUST;
      foreach (id in query_currently_playing_ids()) {
         data = query_player_data(id);
         if (data->state != BLACKJACK_BUST &&
             data->state != BLACKJACK_STAND) {
            not_done = 1;
         }
      }
      if (!not_done) {
          remove_call_out(_timeout_call);
          _timeout_call = call_out("complete_round", 2);
      }
   } else {
      add_succeeded_mess("$N $V and gets " + query_card_string(card) +
                         " giving a total of " + value + " on $D.\n");
   }
   return 1;
} /* do_hit() */

int do_stand() {
   int value;
   string id;
   class player_data data;
   int not_done;

   // Get another card.
   if (!is_game_started()) {
      add_failed_mess("The game has not started.\n");
      return 0;
   }

   id = find_player_id_of_person(this_player());
   if (!id) {
      add_failed_mess("You are not playing.\n");
      return 0;
   }

   data = query_player_data(id);

   if (data->state != BLACKJACK_PLAYING) {
      add_failed_mess("You are not playing and set yourself to stand.\n");
      return 0;
   }

   value = query_hand_value(data->hand);

   data->state = BLACKJACK_STAND;
   foreach (id in query_currently_playing_ids()) {
      data = query_player_data(id);
      if (data->state != BLACKJACK_BUST &&
          data->state != BLACKJACK_STAND) {
         not_done = 1;
      }
   }
   if (!not_done) {
       remove_call_out(_timeout_call);
       _timeout_call = call_out("complete_round", 2);
   }
   add_succeeded_mess("$N $V with a total of " + value +
                      " on $D.\n");
   return 1;
} /* do_stand() */

/**
 * Starts a nice furry game.
 */
int do_start() {
   if (!is_open_for("blackjack", this_player()->query_name())) {
      add_failed_mess("The blackjack table is not open.\n");
      return 0;
   }

   if (_timeout_call) {
      add_failed_mess("Someone is still playing, you cannot start a "
                      "new game yet.\n");
      return 0;
   }

   //
   // There must eb enough money in the float for everyone to bid the max amo
   // amount and win with a blackjack.
   //
   if (query_float() < (_max_bet * 3 * 4) / 2) {
      if (is_allowed(this_player()->query_name())) {
         add_failed_mess("The float is too low for the table to open.\n");
         return 0;
      }
      add_failed_mess("The blackjack table is not open.\n");
      return 0;
   }

   if (!is_playing(this_player())) {
      add_failed_mess("You must be playing the game to start it.\n");
      return 0;
   }

   if (!start_game()) {
      add_failed_mess("You need at least three people to play modern art.\n");
      return 0;
   }

   add_succeeded_mess("$N $V a game on $D.\n");
   remove_call_out(_timeout_call);
   _timeout_call = call_out("force_finish", _timeout_length);
   tell_all_players("Timeout for this game is " + (_timeout_length / 60) +
                    " minutes.\n");
   return 1;
} /* do_start() */

/**
 * If it is finished early...  Oh dear.
 */
int do_finish() {
   string person;

   if (!is_game_started()) {
      add_failed_mess("The game has not started.\n");
      return 0;
   }

   person = find_player_id_of_person(this_player());
   if (!person) {
      add_failed_mess("You must actually be playing to finish the game.\n");
      return 0;
   }

   force_finish();
   return 1;
} /* do_finish() */

string query_main_status(int hint) {
   string place;
   string ret;

   place = query_money_place();
   ret = "$I$0=Blackjack table:\n"
          "$I$6=   Table is " +
             (query_float() >= (_max_bet * 3 * 4) / 2?"open.\n":
              "closed! (Float needed: " +
              MONEY_HAND->money_value_string((_max_bet * 3 * 4) / 2, place) +
              "; current: " +
              MONEY_HAND->money_value_string(query_float(), place) + ")\n") +
          "$I$6=   Timeout length: " + (_timeout_length / 60) + " minutes.\n";
   if (hint) {
      ret += "$I$6=      set timeout <number> on <table>\n";
   }
   ret += "$I$6=   Maximum bet: " +
              MONEY_HAND->money_value_string(_max_bet, place) + "\n";
   if (hint) {
      ret += "$I$6=      set maximum bet <amount> on <table>\n";
   }
   ret += "$I$6=   Minimum bet: " +
              MONEY_HAND->money_value_string(_min_bet, place) + "\n";
   if (hint) {
      ret += "$I$6=      set minimum bet <amount> on <table>\n";
   }
   ret += "$I$6=   Num Decks  : " + _num_decks + "\n";
   if (hint) {
      ret += "$I$6=      set num decks <amount> on <table>\n";
   }
   ret += "$I$6=   Soft 17    : " + (_soft_seventeen?"on":"off") + "\n";
   if (hint) {
      ret += "$I$6=      set hit on soft seventeen {on|off} on <table>\n";
   }
/*
   ret += "$I$6=   Doubles    : " + (_double_on_doubles?"on":"off") + "\n";
   if (hint) {
      ret += "$I$6=      set doubles {on|off} on <table>\n";
   }
 */
   ret += "$I$6=   Float needed: " +
              MONEY_HAND->money_value_string((_max_bet * 3 * 4) / 2, place) +
          " (max players * max bet * 3 / 2)\n$I$6=   Revenue: " +
          MONEY_HAND->money_value_string(query_revenue(), place) +
          "\n\n";
   return ret;
} /* query_main_status() */

/**
 * This method sets the bet boundaries.
 * @param str the amount string
 * @param max_bet if it a max or min bet to set
 */
int do_set_bet(string str, int max_bet) {
   string place;
   int value;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to change the paramaters of "
                   "$D.\n");
      return 0;
   }
   place = query_money_place();
   value = MONEY_HAND->value_from_string(str, place);
   if (!value) {
      add_failed_mess("Unable to parse the string " + str + ".\n");
      return 0;
   }
   if (max_bet) {
      _max_bet = value;
      add_succeeded_mess("$N set$s the maximum bet to " +
          MONEY_HAND->money_value_string(value, place) + " on $D.\n");
   } else {
      _min_bet = value;
      add_succeeded_mess("$N set$s the minimum bet to " +
          MONEY_HAND->money_value_string(value, place) + " on $D.\n");
   }

   return 1;
} /* do_set_bet() */

/**
 * This method sets the timeout for the table.
 */
int do_set_timeout(int length) {
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to change the paramaters of "
                   "$D.\n");
      return 0;
   }

   if (length <= 0) {
      add_failed_mess("The timeout must be greator than 0.\n");
      return 0;
   }
   _timeout_length = length * 60;
   add_succeeded_mess("$N set$s the timeout on $D to " + length + " minutes.\n");
   return 1;
} /* do_set_timeout() */

/**
 * This method sets the soft_seventeen for the table.
 */
int do_set_soft_seventeen(int value) {
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to change the paramaters of "
                   "$D.\n");
      return 0;
   }

   _soft_seventeen = value;
   add_succeeded_mess("$N set$s the soft seventeen dealer hit on $D " +
                      (value?"on":"off") + ".\n");
   return 1;
} /* do_set_soft_seventeen() */

/**
 * This method sets the soft_seventeen for the table.
 */
int do_set_double_on_doubles(int value) {
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to change the paramaters of "
                   "$D.\n");
      return 0;
   }

   _double_on_doubles = value;
   add_succeeded_mess("$N set$s the double on doubles on $D " +
                      (value?"on":"off") + ".\n");
   return 1;
} /* do_set_double_on_doubles() */

/**
 * This method sets the timeout for the table.
 */
int do_set_num_decks(int num_decks) {
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to change the paramaters of "
                   "$D.\n");
      return 0;
   }

   if (num_decks <= 0) {
      add_failed_mess("The timeout must be greator than 0.\n");
      return 0;
   }
   if (num_decks >= 10) {
      add_failed_mess("The number of decks must be less than 10.\n");
      return 0;
   }
   _num_decks = num_decks;
   add_succeeded_mess("$N set$s the number of decks on $D to " + num_decks +
                      ".\n");
   return 1;
} /* do_set_num_decks() */

void init() {
   multiplayer_base::init();
   commercial::init();
   add_command("bet", "<string'amount'> on <direct:object>",
               (: do_bet($4[0]) :));
   add_command("hit", "on <direct:object>",
               (: do_hit() :));
   add_command("stay", "on <direct:object>",
               (: do_stand() :));
/*
   add_command("finish", "game on <direct:object>",
               (: do_finish() :));
 */
   add_command("start", "[new] game on <direct:object>",
               (: do_start() :));

   if (environment()->is_allowed(this_player()->query_name())) {
      add_command("set", "minimum bet <string'amount'> on <direct:object>",
                  (: do_set_bet($4[0], 0) :));
      add_command("set", "maximum bet <string'amount'> on <direct:object>",
                  (: do_set_bet($4[0], 1) :));
      add_command("set", "timeout <number'minutes'> on <direct:object>",
                  (: do_set_timeout($4[0]) :));
      add_command("set", "num decks <number'num decks'> on <direct:object>",
                  (: do_set_num_decks($4[0]) :));
/*
      add_command("set", "double on doubles {on|off} on <direct:object>",
                  (: do_set_double_on_doubles($4[0] == "on") :));
 */
      add_command("set", "hit on soft seventeen {on|off} on <direct:object>",
                  (: do_set_soft_seventeen($4[0] == "on") :));
   }
} /* init() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
   mapping map;

   map = commercial::query_dynamic_auto_load();
   multiplayer_base::query_dynamic_auto_load(map);
   //add_auto_load_value(map, BOARD_TAG, "return", _return);
   add_auto_load_value(map, BOARD_TAG, "cost", _cost);
   //add_auto_load_value(map, BOARD_TAG, "pay out", _pay_out);
   //add_auto_load_value(map, BOARD_TAG, "revenue", _revenue);
   add_auto_load_value(map, BOARD_TAG, "dealer hand", _dealer_hand);
   add_auto_load_value(map, BOARD_TAG, "deck", _deck);
   add_auto_load_value(map, BOARD_TAG, "discard", _discard);
   add_auto_load_value(map, BOARD_TAG, "finished", _finished);
   add_auto_load_value(map, BOARD_TAG, "max bet", _max_bet);
   add_auto_load_value(map, BOARD_TAG, "min bet ", _min_bet);
   add_auto_load_value(map, BOARD_TAG, "timeout", _timeout_length);
   add_auto_load_value(map, BOARD_TAG, "num decks", _num_decks);
   add_auto_load_value(map, BOARD_TAG, "soft seventeen", _soft_seventeen);
   add_auto_load_value(map, BOARD_TAG, "double on doublet", _double_on_doubles);
   return map;
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_dynamic_arg(mapping map, object player) {
   commercial::init_dynamic_arg(map, player);
   multiplayer_base::init_dynamic_arg(map, player);

   //_return = query_auto_load_value(map, BOARD_TAG, "return");
   _cost = query_auto_load_value(map, BOARD_TAG, "cost");
   //_pay_out = query_auto_load_value(map, BOARD_TAG, "pay out");
   //_revenue = query_auto_load_value(map, BOARD_TAG, "revenue");
   _dealer_hand = query_auto_load_value(map, BOARD_TAG, "dealer hand");
   _deck = query_auto_load_value(map, BOARD_TAG, "deck");
   _discard = query_auto_load_value(map, BOARD_TAG, "discard");
   _finished = query_auto_load_value(map, BOARD_TAG, "finished");
   _min_bet = query_auto_load_value(map, BOARD_TAG, "min bet");
   _max_bet = query_auto_load_value(map, BOARD_TAG, "max bet");
   _timeout_length = query_auto_load_value(map, BOARD_TAG, "timeout");
   _num_decks = query_auto_load_value(map, BOARD_TAG, "num decks");
   _soft_seventeen = query_auto_load_value(map, BOARD_TAG, "soft seventeen");
   _double_on_doubles = query_auto_load_value(map, BOARD_TAG, "double on doublet");

   if (!_num_decks) {
      _num_decks = 1;
   }

   if (is_game_started()) {
      remove_call_out(_timeout_call);
      _timeout_call = call_out("force_finish", _timeout_length);
   }
} /* init_dynamic_arg() */
