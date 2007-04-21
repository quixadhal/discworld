// Pinkfish
// Started Wed May 30 21:37:15 PDT 2001

inherit "/std/room/furniture/games/card_base";
inherit "/std/room/furniture/games/multiplayer_base";
inherit "/std/room/furniture/commercial";

#include <money.h>
#include <room/card_base.h>

// The hand types.
#define HAND_TYPE_HIGH_CARD  1
#define HAND_TYPE_PAIR       2
#define HAND_TYPE_TWO_PAIR   3
#define HAND_TYPE_THREE      4
#define HAND_TYPE_STRAIGHT   5
#define HAND_TYPE_FLUSH      6
#define HAND_TYPE_FULL_HOUSE 7
#define HAND_TYPE_FOUR       8
#define HAND_TYPE_STRAIGHT_FLUSH 9

#define POKER_STATE_ANTE      0
#define POKER_STATE_PAID_ANTE 1
#define POKER_STATE_DEALT     2
#define POKER_STATE_BET       3
#define POKER_STATE_DISCARD   4
#define POKER_STATE_AFTER_DISCARD   5
#define POKER_STATE_FINAL_BET 6
#define POKER_STATE_FOLDED    7
#define POKER_STATE_END       8

#define TIE -1

class hand_type {
   int hand_type;
   class playing_card* kickers;
   class playing_card duplicate_1;
   class playing_card duplicate_2;
   class playing_card high_card;
}

class player_data {
   class playing_card* hand;
   class hand_type hand_type;
   int bet;
   int state;
}

#define BOARD_TAG "poker"

//
// This gives an estimated return on the payment.  For example 50%
// return would mean you get back approximately 50% of your money on
// average
//
private int _return;
private int _ante_amount;
private int _ante_house;
private int _house_cut;
private int _min_bet;
private int _max_bet;
private int _current_bet;
private int _pot;
private int _finished;
private int _poker_phase;
private int _turn_timeout;
private int _draw_round;
private int _max_draw_rounds;
private class playing_card* _deck;
private class playing_card* _discard;
private mapping _player_stats;

int query_hand_value(class playing_card* cards);
class hand_type query_hand_type(class playing_card* hand);

void create() {
   multiplayer_base::create();

   set_minimum_needed(2);
   add_help_file("poker");
   _ante_house = 400;
   _ante_amount = 400;
   _min_bet = 400;
   _max_bet = 1200;
   _max_draw_rounds = 1;
   _player_stats = ([ ]);
   set_shop_use_types(({ "poker" }));

   commercial::create();

   set_commercial_type("gambling");
   set_commercial_name("poker");
} /* create() */

string query_hand_type_string(class hand_type bing) {
   string ret;

   switch (bing->hand_type) {
   case HAND_TYPE_FULL_HOUSE :
      ret = "Full house";
      break;
   case HAND_TYPE_THREE :
      ret = "Three of a kind";
      break;
   case HAND_TYPE_FLUSH :
      ret = "Flush";
      break;
   case HAND_TYPE_STRAIGHT :
      ret = "Straight";
      break;
   case HAND_TYPE_STRAIGHT_FLUSH :
      ret = "Straight flush";
      break;
   case HAND_TYPE_PAIR :
      ret = "Pair";
      break;
   case HAND_TYPE_FOUR :
      ret = "Four of a kind";
      break;
   case HAND_TYPE_TWO_PAIR :
      ret = "Two pairs";
      break;
   case HAND_TYPE_HIGH_CARD :
      ret = "High card " + query_card_string(bing->high_card);
      break;
   default :
      ret = "Nothing";
      break;
   }
   return ret;
} /* query_hand_type_string() */

/**
 * This method shows the current status of the cards.
 * @param id the id to show the status for
 * @return the status of the cards
 */
string query_card_status(string id) {
   string id_bing;
   string ret;
   string* not_playing;
   class player_data data;
   string* womble;
   string place;
   int left;

   ret = "";
   not_playing = ({ });
   womble = query_player_ids();
   place = query_money_place();
   foreach (id_bing in womble) {
      if (is_person_playing(id_bing)) {
         ret += capitalize(id_bing) + " (" +
                query_player_cap_name(id_bing) + ")";
         data = query_player_data(id_bing);
         if (data) {
            if (data->state == POKER_STATE_FOLDED) {
               ret += " Folded!\n";
            } else {
               if (data->state == POKER_STATE_PAID_ANTE) {
                  ret += " (paid ante) ";
               }
               if (data->state == POKER_STATE_AFTER_DISCARD) {
                  ret += " (discarded) ";
               }
               if (data->state == POKER_STATE_BET ||
                   data->state == POKER_STATE_FINAL_BET) {
                  if (id_bing == query_current_player()) {
                     ret += " <-- Their bet ";
                  }
               }
               if (data->bet) {
                  ret += "  Current bet " + 
                         MONEY_HAND->money_value_string(data->bet, place);
                  ret += "\n";
               } else {
                  ret += "  No bet yet.\n";
               }
            }
         } else {
            ret += "\n";
         }
      } else {
         not_playing += ({ id_bing });
      }
   }

   if (sizeof(not_playing) > 1) {
      ret += query_multiple_short(map(not_playing, (: capitalize($1) :))) +
                                  " are not playing.\n";
   } else if (sizeof(not_playing) == 1) {
      ret += query_multiple_short(map(not_playing, (: capitalize($1) :))) +
                                  " is not playing.\n";
   }
   ret += "The pot is " +
          MONEY_HAND->money_value_string(_pot, place) + ".\n";
   if (is_game_started()) {
      if (_draw_round < _max_draw_rounds) {
         left = _max_draw_rounds - _draw_round;
         ret += left + " draw round" + (left>1?"s":"") + " left.\n";
      }
   }
   ret += "\n";
   if (id) {
      if (!_finished) {
         data = query_player_data(id);
         if (data && sizeof(data->hand)) {
            ret += "Your hand (" +
                   query_hand_type_string(data->hand_type) +
                   (data->state == POKER_STATE_FOLDED?" -- Folded":"") +
                   "):\n";
            if (data->state != POKER_STATE_FOLDED) {
               ret += query_hand_string(data->hand,
                                     CARD_HAND_THREE|CARD_HAND_LETTERS,
                                     this_player()->query_cols());
            }
         }
         if (_finished) {
            tell_all_players(query_player_cap_name(id) + " peeks at their hand on " +
                         the_short() + ".\n", ({ id }));
         }
      } else {
         foreach (id in query_started_player_ids()) {
            data = query_player_data(id);
            if (data && sizeof(data->hand)) {
               ret += query_player_cap_name(id);
               if (data->state == POKER_STATE_FOLDED) {
                  ret += " folded, so the cards are hidden.\n";
               } else {
                  ret += " hand (" +
                         query_hand_type_string(data->hand_type) + ":\n";
                  ret += query_hand_string(data->hand, CARD_HAND_SINGLE,
                                        this_player()->query_cols());
               }
            }
         }
      }
   }

   return ret;
}
   
/** @ignore yes */
string long(string str, int dark) {
   if (dark) {
      return ::long() +
             "It is too dark to make out the pieces on the board.\n";
   }
   return ::long() + query_card_status(find_player_id_of_person(this_player()));
} /* long() */

/**
 * See if the round has completed.
 */
int check_end_round() {
   int bet;
   string id;
   class player_data data;

   foreach (id in query_started_player_ids()) {
      data = query_player_data(id);
      if (data->state != POKER_STATE_FOLDED) {
         if (!data->bet) {
            return 0;
         } else if (!bet && data->bet) {
            bet = data->bet;
         } else if (bet != data->bet) {
            return 0;
         }
      }
   }

   if (bet) {
      call_out("complete_round", 2);
      _poker_phase = POKER_STATE_END;
      return 1;
   }
} /* check_end_round() */

/**
 * This method bounces to the next person to bid.
 */
void next_person_turn() {
   class player_data data;
   string start_id;

   start_id = query_current_player();
   do {
      increment_current_player();
      data = query_player_data(query_current_player());
   } while (data->state == POKER_STATE_FOLDED &&
            query_current_player() != start_id);
   // Only one person left...
   if (query_current_player() == start_id) {
      printf("Force end of game.\n");
      call_out("complete_round", 2, 1);
      _poker_phase = POKER_STATE_END;
   } else {
      call_out("tell_current_player", 0, "%^BOLD%^Your turn!%^RESET%^\n");
   }
} /* next_person_turn() */

/**
 * This deals cards to everyone.
 */
void deal_cards() {
   string id;
   class player_data data;

   _deck = make_deck(1, 0);
   _deck = shuffle_deck(_deck);
   foreach (id in query_currently_playing_ids()) {
      data = query_player_data(id);
      data->hand = sort_cards(_deck[0..4], 3);
      data->bet = 0;
      if (data->state != POKER_STATE_FOLDED) {
         if (!_max_draw_rounds) {
            data->state = POKER_STATE_FINAL_BET;
         } else {
            data->state = POKER_STATE_BET;
         }
      }
      data->hand_type = query_hand_type(data->hand);
      _deck = _deck[5..];
      if (query_player_object(id)) {
         tell_player(id, "Your hand:\n" +
                query_hand_string(data->hand, CARD_HAND_THREE|CARD_HAND_LETTERS,
                                  query_player_object(id)->query_cols()) +
            query_hand_type_string(data->hand_type) + ".\n");
      }
   }
   if (!_max_draw_rounds) {
      _poker_phase = POKER_STATE_FINAL_BET;
   } else {
      _poker_phase = POKER_STATE_BET;
   }
   next_person_turn();
   tell_all_players(query_player_cap_name(query_current_player()) +
                    " goes first.\n");
   //call_out("tell_current_player", 0, "%^BOLD%^Your turn!%^RESET%^\n");
   //tell_current_player("%^BOLD%^Your turn!%^RESET%^\n");
   _current_bet = 0;
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
      data->bet = 0;
      data->hand = ({ });
      data->state = POKER_STATE_ANTE;
      set_player_data(id, data);
   }
   _poker_phase = POKER_STATE_ANTE;
   _pot = 0;
   _finished = 0;
   _draw_round = 0;
   _discard = ({ });
   tell_all_players("%^BOLD%^Place your ante to start playing.%^RESET%^\n");
   return 1;
} /* start_game() */

/**
 * Checks to see if all the people playing have put in their first
 * bets.
 */
void check_for_finish_ante() {
   string id;
   class player_data data;
   object ob;

   foreach (id in query_currently_playing_ids()) {
      data = query_player_data(id);
      ob = query_player_object(id);
      if (ob && !interactive(ob)) {
         // Set them to folded.
         data->state = POKER_STATE_FOLDED;
      }
      if (ob && interactive(ob) && data->state == POKER_STATE_ANTE) {
         return ;
      }
   }

   _poker_phase = POKER_STATE_BET;
   // Move to the next state!
   call_out("deal_cards", 2);
} /* check_for_finish_ante() */

/**
 * This figures out if the card is higher than the other one or not.
 * @param card_new the new card o checxl
 * @param card_old the old card to check againt
 * @return 1 if the new card is higher, 0 if the old card is higher
 */
int is_card_higher(class playing_card card_new, class playing_card card_old) {
   if (card_new->number == card_old->number) {
      // Then it is based on suits.
      return 0;
   }

   if (card_new->number == CARD_NUMBER_ACE) {
      return 1;
   }
   if (card_old->number == CARD_NUMBER_ACE) {
      return 0;
   }
   if (card_new->number > card_old->number) {
      return 1;
   }
   return 0;
} /* is_card_higher() */

/**
 * This method returns the basic type of the hand.
 */
class hand_type query_hand_type(class playing_card* hand) {
   int high_num;
   int num;
   int id;
   int i;
   int j;
   class playing_card* tmp_hand;
   class hand_type ret_type;
   class playing_card tmp_card;

   ret_type = new(class hand_type);

   high_num = 0;
   for (i = 1; i < sizeof(hand); i++) {
      if (is_card_higher(hand[i], hand[high_num])) {
         high_num = i;
      }
   }
   ret_type->hand_type = HAND_TYPE_HIGH_CARD;
   ret_type->high_card = hand[high_num];
   //
   // First check for x of a kind.
   //
   high_num = 1;
   for (i = 0; i < sizeof(hand) - 1; i++) {
      num = 1;
      for (j = 0; j < sizeof(hand) - i - 1; j++) {
         if (hand[i]->number == hand[i + j + 1]->number) {
            num++;
         }
      }
      if (num > high_num) {
         high_num = num;
         id = hand[i]->number;
      }
   }

   tmp_hand = hand;
   if (high_num > 1) {
      // Remove the found cards from the list.
      for (i = 0; i < sizeof(tmp_hand); i++) {
         if (tmp_hand[i]->number == id) {
            ret_type->duplicate_1 = tmp_hand[i];
            tmp_hand = tmp_hand[0..i-1] + tmp_hand[i+1..];
            i--;
         }
      }
   }


   //
   // If you have more than one of a certain numbered card you cannot
   // have any of the other bits.
   //
   if (high_num == 2) {
      //
      // Could be two pair.
      //
      high_num = 1;
      for (i = 0; i < sizeof(tmp_hand) - 1; i++) {
         num = 1;
         for (j = 0; j < sizeof(tmp_hand) - i - 1; j++) {
            if (tmp_hand[i]->number == tmp_hand[i + j + 1]->number) {
               num++;
            }
         }
         if (num > high_num) {
            high_num = num;
            id = tmp_hand[i]->number;
         }
      }
      if (high_num == 2) {
         ret_type->hand_type = HAND_TYPE_TWO_PAIR;
         // Remove the found cards from the list.
         for (i = 0; i < sizeof(tmp_hand); i++) {
            if (tmp_hand[i]->number == id) {
               ret_type->duplicate_2 = tmp_hand[i];
               tmp_hand = tmp_hand[0..i-1] + tmp_hand[i+1..];
            }
         }
         if (is_card_higher(ret_type->duplicate_2, ret_type->duplicate_1)) {
            tmp_card = ret_type->duplicate_1;
            ret_type->duplicate_1 = ret_type->duplicate_2;
            ret_type->duplicate_2 = tmp_card;
         }
         ret_type->kickers = sort_cards(tmp_hand, 3);
      } else {
         ret_type->hand_type = HAND_TYPE_PAIR;
         high_num = 0;
         for (i = 1; i < sizeof(tmp_hand); i++) {
            if (is_card_higher(tmp_hand[i], tmp_hand[high_num])) {
               high_num = i;
            }
         }
         ret_type->kickers = sort_cards(tmp_hand, 3);
      }
      return ret_type;
   } else if (high_num == 3) {
      //
      // Could be a full house.
      //
      if (tmp_hand[0]->number == tmp_hand[1]->number) {
         ret_type->hand_type = HAND_TYPE_FULL_HOUSE;
         ret_type->duplicate_2 = tmp_hand[0];
         ret_type->kickers = ({ });
      } else {
         ret_type->hand_type = HAND_TYPE_THREE;
         ret_type->kickers = sort_cards(tmp_hand, 3);
      }
      return ret_type;
   } else if (high_num == 4) {
      ret_type->hand_type = HAND_TYPE_FOUR;
      ret_type->high_card = tmp_hand[0];
      return ret_type;
   }

   //
   // Check for a flush.
   //
   for (i = 0; i < sizeof(hand) - 1; i++) {
      if (hand[i + 1]->suit != hand[0]->suit) {
         break;
      }
   }
   if (i == sizeof(hand) - 1) {
      ret_type->hand_type = HAND_TYPE_FLUSH;
      ret_type->kickers = sort_cards(hand, 3);
      // Find the high card.
      high_num = 0;
      for (i = 1; i < sizeof(hand); i++) {
         if (is_card_higher(hand[i], hand[high_num])) {
            high_num = i;
         }
      }
      ret_type->high_card = hand[high_num];
   }

   //
   // Check for a straight.
   //
   tmp_hand = sort_array(hand, (: ((class playing_card)$1)->number -
                                  ((class playing_card)$2)->number :));
   if (tmp_hand[0]->number == CARD_NUMBER_ACE) {
      if (tmp_hand[1]->number == 2) {
         // Skip the ace at the start.
         j = 1;
      } else {
         // Move the ace to the end.
         j = 0;
         tmp_hand = tmp_hand[1..] + ({ tmp_hand[0] });
      }
   } else {
      j = 0;
   }
   for (i = j; i < sizeof(tmp_hand) - 1; i++) {
      if (tmp_hand[i]->number + 1 != tmp_hand[i + 1]->number) {
         if (tmp_hand[i + 1]->number != CARD_NUMBER_ACE ||
             tmp_hand[i]->number != CARD_NUMBER_KING) {
            break;
         }
      }
   }
   if (i == sizeof(tmp_hand) - 1) {
      if (ret_type->hand_type == HAND_TYPE_FLUSH) {
         ret_type->hand_type = HAND_TYPE_STRAIGHT_FLUSH;
         ret_type->high_card = tmp_hand[<1];
      } else {
         ret_type->hand_type = HAND_TYPE_STRAIGHT;
         ret_type->high_card = tmp_hand[<1];
      }
   }

   if (ret_type->hand_type == HAND_TYPE_HIGH_CARD) {
      ret_type->kickers = sort_cards(hand, 3);
   }

   return ret_type;
} /* query_hand_type() */

/**
 * This figures out which hand is greator than the other.
 */
int is_greator_hand(class hand_type new_hand,
                    class hand_type old_hand) {
   int i;

   if (new_hand->hand_type > old_hand->hand_type) {
      return 1;
   }
   if (new_hand->hand_type < old_hand->hand_type) {
      return 0;
   }
   if (new_hand->hand_type == old_hand->hand_type) {
      // Try and work out ties...
      switch (new_hand->hand_type) {
      case HAND_TYPE_HIGH_CARD :
         if (is_card_higher(new_hand->high_card,
                            old_hand->high_card)) {
            return 1;
         }
         if (is_card_higher(old_hand->high_card,
                            new_hand->high_card)) {
            return 0;
         }
         for (i = 1; i <= sizeof(new_hand->kickers); i++) {
            if (is_card_higher(new_hand->kickers[<i],
                               old_hand->kickers[<i])) {
               return 1;
            } else if (is_card_higher(old_hand->kickers[<i],
                                      new_hand->kickers[<i])) {
               return 0;
            }
         }
         break;
      case HAND_TYPE_FLUSH :
         for (i = 1; i <= sizeof(new_hand->kickers); i++) {
            if (is_card_higher(new_hand->kickers[<i],
                               old_hand->kickers[<i])) {
               return 1;
            } else if (is_card_higher(old_hand->kickers[<i],
                                      new_hand->kickers[<i])) {
               return 0;
            }
         }
         break;
      case HAND_TYPE_PAIR :
         if (is_card_higher(new_hand->duplicate_1,
                            old_hand->duplicate_1)) {
            return 1;
         }
         if (is_card_higher(old_hand->duplicate_1,
                            new_hand->duplicate_1)) {
            return 0;
         }
         for (i = 1; i <= sizeof(new_hand->kickers); i++) {
            if (is_card_higher(new_hand->kickers[<i],
                               old_hand->kickers[<i])) {
               return 1;
            } else if (is_card_higher(old_hand->kickers[<i],
                                      new_hand->kickers[<i])) {
               return 0;
            }
         }
         break;
      case HAND_TYPE_TWO_PAIR :
         if (is_card_higher(new_hand->duplicate_1,
                            old_hand->duplicate_1)) {
            return 1;
         }
         if (is_card_higher(old_hand->duplicate_1,
                            new_hand->duplicate_1)) {
            return 0;
         }
         if (is_card_higher(new_hand->duplicate_2,
                            old_hand->duplicate_2)) {
            return 1;
         }
         if (is_card_higher(old_hand->duplicate_2,
                            new_hand->duplicate_2)) {
            return 0;
         }
         if (is_card_higher(new_hand->kickers[0],
                            old_hand->kickers[0])) {
            return 1;
         }
         if (is_card_higher(old_hand->kickers[0],
                            new_hand->kickers[0])) {
            return 0;
         }
         break;
      case HAND_TYPE_THREE :
         if (is_card_higher(new_hand->duplicate_1,
                            old_hand->duplicate_1)) {
            return 1;
         }
         if (is_card_higher(old_hand->duplicate_1,
                            new_hand->duplicate_1)) {
            return 0;
         }
         for (i = 1; i <= sizeof(new_hand->kickers); i++) {
            if (is_card_higher(new_hand->kickers[<i],
                               old_hand->kickers[<i])) {
               return 1;
            } else if (is_card_higher(old_hand->kickers[<i],
                                      new_hand->kickers[<i])) {
               return 0;
            }
         }
         break;
      case HAND_TYPE_FOUR :
         if (is_card_higher(new_hand->duplicate_1,
                            old_hand->duplicate_1)) {
            return 1;
         }
         if (is_card_higher(old_hand->duplicate_1,
                            new_hand->duplicate_1)) {
            return 0;
         }
         if (is_card_higher(new_hand->kickers[0],
                            old_hand->kickers[0])) {
            return 1;
         }
         if (is_card_higher(old_hand->kickers[0],
                            new_hand->kickers[0])) {
            return 0;
         }
         break;
      case HAND_TYPE_FULL_HOUSE :
         if (is_card_higher(new_hand->duplicate_1,
                            old_hand->duplicate_1)) {
            return 1;
         }
         if (is_card_higher(old_hand->duplicate_1,
                            new_hand->duplicate_1)) {
            return 0;
         }
         if (is_card_higher(new_hand->duplicate_2,
                            old_hand->duplicate_2)) {
            return 1;
         }
         if (is_card_higher(old_hand->duplicate_2,
                            new_hand->duplicate_2)) {
            return 0;
         }
         break;
      case HAND_TYPE_STRAIGHT :
      case HAND_TYPE_STRAIGHT_FLUSH :
         // Use the high card as the kicker.
         if (is_card_higher(new_hand->high_card,
                            old_hand->high_card)) {
            return 1;
         }
         if (is_card_higher(new_hand->high_card,
                            old_hand->high_card)) {
            return 0;
         }
         break;
      }
   }
   return TIE;
} /* is_greator_hand() */

void test_hand_type() {
   class playing_card* other_tmp_hand;
   class playing_card* tmp_hand;

   other_tmp_hand = ({ new(class playing_card, suit: CARD_SUIT_HEARTS, number : 11),
                 new(class playing_card, suit: CARD_SUIT_HEARTS, number : 11),
                 new(class playing_card, suit: CARD_SUIT_HEARTS, number : 12),
                 new(class playing_card, suit: CARD_SUIT_HEARTS, number : 13),
                 new(class playing_card, suit: CARD_SUIT_HEARTS, number : 13) });
   tmp_hand = ({ new(class playing_card, suit: CARD_SUIT_HEARTS, number : 2),
                 new(class playing_card, suit: CARD_SUIT_HEARTS, number : 3),
                 new(class playing_card, suit: CARD_SUIT_HEARTS, number : 4),
                 new(class playing_card, suit: CARD_SUIT_HEARTS, number : 1),
                 new(class playing_card, suit: CARD_SUIT_HEARTS, number : 5) });
write(query_hand_type_string(query_hand_type(tmp_hand)) + " --\n" +
      query_hand_type_string(query_hand_type(other_tmp_hand)));
printf("%O\n", sizeof(query_hand_type(tmp_hand)->kickers));
printf("%O\n", sizeof(query_hand_type(other_tmp_hand)->kickers));
write("Result: " + is_greator_hand(query_hand_type(tmp_hand),
      query_hand_type(other_tmp_hand)) + "\n");
} 

/**
 * This method completes the round.
 */
void complete_round(int force_end) {
   string stuff;
   object ob;
   class player_data data;
   string id;
   string place;
   string* winner;
   class hand_type winning_hand_type;
   class playing_card* winning_hand;
   int paid;
   int discard;
   int result;
   int num;

   if (_finished) {
      return 0;
   }

   place = query_money_place();

   stuff = "";
   winner = ({ });
   foreach (id in query_started_player_ids()) {
      data = query_player_data(id);
      if ((data->state == POKER_STATE_BET || discard) && !force_end) {
         if (data->state != POKER_STATE_FOLDED) {
            data->state = POKER_STATE_DISCARD;
            data->bet = 0;
         }
         discard = 1;
      } else if (data->state != POKER_STATE_FOLDED) {
         num++;
         if (!winning_hand) {
            winning_hand = data->hand;
            winning_hand_type = data->hand_type;
            winner = ({ id });
         } else {
            data->hand_type = query_hand_type(data->hand);
            result = is_greator_hand(data->hand_type, winning_hand_type);
            if (result == TIE) {
               winner += ({ id });
            } else if (result) {
               winning_hand = data->hand;
               winning_hand_type = data->hand_type;
               winner = ({ id });
            }
         }
         stuff += query_player_cap_name(id) + "'s hand (" +
                  query_hand_type_string(data->hand_type) + "):\n" +
                  query_hand_string(data->hand, CARD_HAND_SINGLE,
                                     this_player()->query_cols()) + "\n";
      }
   }

   if (num == 1) {
      stuff = "All but one person folded so the cards are not revealed.\n";
   }

   if (discard) {
      foreach (id in query_started_player_ids()) {
         data = query_player_data(id);
         if (data->state == POKER_STATE_FOLDED) {
            tell_player(id, "%^BOLD%^Now into the discard phase.%^RESET%^\n"
               "You have folded.\n");
         } else {
            data->bet = 0;
            tell_player(id, "%^BOLD%^Now into the discard phase.%^RESET%^\n"
               "Your hand:\n" +
               query_hand_string(data->hand, CARD_HAND_THREE | CARD_HAND_LETTERS,
                                    80));
         }
      }
   } else {
      if (_house_cut) {
         stuff += "House takes " + 
            MONEY_HAND->money_value_string((_pot * _house_cut) / 100, place) +
                  ".\n";
        adjust_float((_pot * _house_cut) / 100);
        //_revenue += (_pot * _house_cut) / 100;
        _pot -= (_pot * _house_cut) / 100;
      }
      if (sizeof(winner) > 1) {
         stuff += "Tie for winning between " +
           query_multiple_short(map(winner, (: query_player_cap_name($1) :))) +
                  ", they each win " +
                  MONEY_HAND->money_value_string(_pot / sizeof(winner), place) +
                  ".\n";
         paid = _pot / sizeof(winner);
         foreach (id in winner) { 
            ob = query_player_object(id);
            if (ob) {
               ob->adjust_money(MONEY_HAND->create_money_array(paid, place), place);
               _player_stats[ob->query_name()] += paid;
            } else {
               stuff += "Unable to find " + id + " to pay them, money "
                               "going to the house.\n";
               adjust_float(paid);
               //_revenue += paid;
            }
         }
      } else {
         stuff += "Winner is " +
           query_multiple_short(map(winner, (: query_player_cap_name($1) :))) +
                  ", and they win " +
                  MONEY_HAND->money_value_string(_pot, place) +
                  ".\n";
         ob = query_player_object(winner[0]);
         ob->adjust_money(MONEY_HAND->create_money_array(_pot, place), place);
         _player_stats[ob->query_name()] += _pot;
      }

      // Do the dealer.
      tell_all_players("The players reveal their cards as:\n" +
                       stuff);
      tell_room(environment(), "The game ends with " + query_multiple_short(winner) +
                               " as the winner.\n");

      finish_game(query_multiple_short(winner));
      _finished = 1;
   }
} /* complete_round() */

/**
 * This method checks to see if everyone has discarded and ready for the
 * next round.
 */
void finish_discard() {
   string id;
   class player_data data;

   foreach (id in query_started_player_ids()) {
      data = query_player_data(id);
      if (data->state != POKER_STATE_AFTER_DISCARD &&
          data->state != POKER_STATE_FOLDED) {
         return 0;
      }
   }

   _draw_round++;

   foreach (id in query_started_player_ids()) {
      data = query_player_data(id);
      if (data->state == POKER_STATE_AFTER_DISCARD) {
         if (_draw_round >= _max_draw_rounds) {
            data->state = POKER_STATE_FINAL_BET;
         } else {
            data->state = POKER_STATE_BET;
         }
      }
   }
   _current_bet = 0;
   _poker_phase = POKER_STATE_FINAL_BET;
   call_out("tell_all_players", 0,
            "Discard round completed.  Now onto the " +
            (_draw_round >= _max_draw_rounds?"final ":"") + "betting round.\n");
   next_person_turn();
   //call_out("tell_current_player", 0, "%^BOLD%^Your turn!%^RESET%^\n");
} /* finish_discard() */

/**
 * Places your bet.
 */
int do_ante() {
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

   data = query_player_data(id);
   if (data->state != POKER_STATE_ANTE) {
      add_failed_mess("You have already put in your ante.\n");
      return 0;
   }

   place = query_money_place();

   amount = _ante_house + _ante_amount;
   if (this_player()->query_value_in(place) < amount) {
      add_failed_mess("You do not have the needed " +
                      MONEY_HAND->money_value_string(amount, place) +
                      " to meet the ante.\n");
      return 0;
   }

   _player_stats[this_player()->query_name()] -= amount;
   this_player()->pay_money(MONEY_HAND->create_money_array(amount, place),
                            place);

   adjust_float(_ante_house);
   //_revenue += _ante_house;
   _pot += _ante_amount;
   data->state = POKER_STATE_PAID_ANTE;
   environment()->event_save(this_object());

   // This is for the starting bet.
   check_for_finish_ante();

   add_succeeded_mess("$N $V " +
                      MONEY_HAND->money_value_string(amount, place) +
                      " on $D.\n");
   return 1;
} /* do_ante() */

/**
 * This hits you for another card.
 */
int do_discard(string throw_away) {
   int i;
   string id;
   class player_data data;
   string* bits;
   int* new_bits;

   throw_away = lower_case(throw_away);
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
   if (!sizeof(data->hand)) {
      add_failed_mess("You cannot discard before you have been dealt "
                      "cards.\n");
      return 0;
   }

   if (data->state != POKER_STATE_DISCARD) {
      add_failed_mess("You are not in the discard phase.\n");
      return 0;
   }

   if (throw_away && throw_away != "none") {
      throw_away = lower_case(replace_string(throw_away, " ", ""));
      bits = explode(throw_away, ",");
      if (sizeof(filter(bits, (: strlen($1) > 1 :))) > 0) {
         add_failed_mess("Some of the card references " + throw_away +
                         " are invalid.  Use a command seperated list of "
                         "card ids.\n");
         return 0;
      }
      // Figure out the cards to throw.
      new_bits = map(bits, (: $1[0] - 'a' :));
      if (sizeof(filter(new_bits, (: $1 < 0 || $1 >= 5 :))) > 0) {
         add_failed_mess("Some of the card references " + throw_away +
                         " are invalid.  Use a command seperated list of "
                         "card ids.\n");
         return 0;
      }

      for (i = 0; i < sizeof(new_bits); i++) {
         if (member_array(new_bits[i],
                          new_bits[0..i-1] + new_bits[i+1..]) != -1) {
            add_failed_mess("You have referenced the card " +
                            sprintf("%c", 'A' + new_bits[i]) + " twice.\n");
            return 0;
         }
      }

      new_bits = sort_array(new_bits, (: $2 - $1 :));
      for (i = 0; i < sizeof(new_bits); i++) {
         _discard += ({ data->hand[new_bits[i]] });
         data->hand = data->hand[0..new_bits[i]-1] +
                      data->hand[new_bits[i]+1..];
      }
      i = 5 - sizeof(data->hand) - 1;
      if (sizeof(_deck) < i) {
         _deck += _discard;
         _deck = shuffle_array(_deck);
         _discard = ({ });
      }
      data->hand += _deck[0..i];
      _deck = _deck[i + 1..];
      data->hand = sort_cards(data->hand, 3);
      data->hand_type = query_hand_type(data->hand);
      add_succeeded_mess(({ "", "$N draw$s " + query_num(sizeof(new_bits)) +
                                " new card" +
                               (sizeof(new_bits) > 1?"s":"") + " on $D.\n" }));
      write("Your new hand (" +
            query_hand_type_string(data->hand_type) +
            "):\n" +
            query_hand_string(data->hand, CARD_HAND_THREE,
                                  this_player()->query_cols()));
      data->state = POKER_STATE_AFTER_DISCARD;
   } else {
      new_bits = ({ });
      data->state = POKER_STATE_AFTER_DISCARD;
      add_succeeded_mess("$N do$es not discard any cards on $D.\n");
   }

   finish_discard();

   return 1;
} /* do_discard() */

int do_check() {
   // Get another card.
   if (!is_game_started()) {
      add_failed_mess("The game has not started.\n");
      return 0;
   }

   if (_poker_phase != POKER_STATE_BET &&
       _poker_phase != POKER_STATE_FINAL_BET) {
      add_failed_mess("Not a betting phase.\n");
      return 0;
   }

   if (!is_current_player(this_player())) {
      add_failed_mess("Not your turn to bet.\n");
      return 0;
   }

   if (_current_bet) {
      add_failed_mess("You cannot check since betting has started.\n");
      return 0;
   }

   add_succeeded_mess("$N check$s on $D.\n");
   next_person_turn();
   return 1;
} /* do_stand() */

int do_call() {
   class player_data data;
   int amt;
   string place;

   // Get another card.
   if (!is_game_started()) {
      add_failed_mess("The game has not started.\n");
      return 0;
   }

   if (_poker_phase != POKER_STATE_BET &&
       _poker_phase != POKER_STATE_FINAL_BET) {
      add_failed_mess("Not a betting phase.\n");
      return 0;
   }

   if (!is_current_player(this_player())) {
      add_failed_mess("Not your turn to bet.\n");
      return 0;
   }

   if (!_current_bet) {
      add_failed_mess("No one has bet anything for you to call.\n");
      return 0;
   }

   place = query_money_place();
   data = query_player_data(query_current_player());
   if (_current_bet && data->bet == _current_bet) {
      // They are called.
      call_out("complete_round", 2);
      if (_poker_phase == POKER_STATE_ANTE) {
         _poker_phase = POKER_STATE_DISCARD;
      } else {
         _poker_phase = POKER_STATE_ANTE;
      }
   } else {
      amt = _current_bet - data->bet;
      if (this_player()->query_value_in(place) < amt) {
         add_failed_mess("You do not have the necessary " +
                         MONEY_HAND->money_value_string(amt, place) +
                         " to call the bet.\n");
         return 0;
      }
      _player_stats[this_player()->query_name()] -= amt;
      this_player()->pay_money(MONEY_HAND->create_money_array(amt, place),
                               place);
      _pot += amt;
      data->bet = _current_bet;
      if (!check_end_round()) {
         next_person_turn();
      }
   }

   add_succeeded_mess("$N call$s the bet by adding " +
                      MONEY_HAND->money_value_string(amt, place) +
                      " to the pot for a total bet of " +
                      MONEY_HAND->money_value_string(_current_bet, place) +
                      " on $D.\n");
   return 1;
} /* do_call() */

int do_raise(string amount) {
   class player_data data;
   string place;
   int amt;
   int raise_amt;

   // Get another card.
   if (!is_game_started()) {
      add_failed_mess("The game has not started.\n");
      return 0;
   }

   if (_poker_phase != POKER_STATE_BET &&
       _poker_phase != POKER_STATE_FINAL_BET) {
      add_failed_mess("Not a betting phase.\n");
      return 0;
   }

   if (!is_current_player(this_player())) {
      add_failed_mess("Not your turn to bet.\n");
      return 0;
   }

   place = query_money_place();
   amt = MONEY_HAND->value_from_string(amount, place);
   raise_amt = amt;
   if (!amt) {
      add_failed_mess("The value " + amount + " is not a valid "
                      "money amount.\n");
      return 0;
   }

   if (amt < _min_bet) {
      add_failed_mess("The minimum bet is " +
                      MONEY_HAND->money_value_string(_min_bet, place) +
                      ".\n");
      return 0;
   }
   if (amt > _max_bet) {
      add_failed_mess("The maximum bet is " +
                      MONEY_HAND->money_value_string(_max_bet, place) +
                      ".\n");
      return 0;
   }

   data = query_player_data(query_current_player());
   amt = _current_bet + amt - data->bet;
   if (this_player()->query_value_in(place) < amt) {
      add_failed_mess("You do not have the nessessary " +
                      MONEY_HAND->money_value_string(amt, place) +
                      " to raise the bet.\n");
      return 0;
   }
   _player_stats[this_player()->query_name()] -= amt;
   this_player()->pay_money(MONEY_HAND->create_money_array(amt, place),
                               place);
   _current_bet = data->bet + amt;
   data->bet = _current_bet;
   _pot += amt;

   add_succeeded_mess("$N raise$s the bet by " +
                      MONEY_HAND->money_value_string(raise_amt, place) +
                      " to " +
                      MONEY_HAND->money_value_string(_current_bet, place) +
                      " on $D.\n");
   next_person_turn();
   return 1;
} /* do_raise() */

int do_bet(string amount) {
   class player_data data;
   string place;
   int amt;
   int raise_amt;

   // Get another card.
   if (!is_game_started()) {
      add_failed_mess("The game has not started.\n");
      return 0;
   }

   if (_poker_phase != POKER_STATE_BET &&
       _poker_phase != POKER_STATE_FINAL_BET) {
      add_failed_mess("Not a betting phase.\n");
      return 0;
   }

   if (!is_current_player(this_player())) {
      add_failed_mess("Not your turn to bet.\n");
      return 0;
   }

   place = query_money_place();
   amt = MONEY_HAND->value_from_string(amount, place);
   if (!amt) {
      add_failed_mess("The value " + amount + " is not a valid "
                      "money amount.\n");
      return 0;
   }

   if (amt == _current_bet) {
      return do_call();
   }

   if (amt < _current_bet) {
      add_failed_mess("You have to bet higher than the current bet of " +
                      MONEY_HAND->money_value_string(_current_bet, place) +
                      " money amount.\n");
      return 0;
   }

   amt = amt - _current_bet;
   raise_amt = amt;

   if (amt < _min_bet) {
      add_failed_mess("The minimum bet is " +
                      MONEY_HAND->money_value_string(_min_bet, place) +
                      ".\n");
      return 0;
   }
   if (amt > _max_bet) {
      add_failed_mess("The maximum bet is " +
                      MONEY_HAND->money_value_string(_max_bet, place) +
                      ".\n");
      return 0;
   }

   data = query_player_data(query_current_player());
   amt = _current_bet + amt - data->bet;
   if (this_player()->query_value_in(place) < amt) {
      add_failed_mess("You do not have the nessessary " +
                      MONEY_HAND->money_value_string(amt, place) +
                      " to raise the bet.\n");
      return 0;
   }
   _player_stats[this_player()->query_name()] -= amt;
   this_player()->pay_money(MONEY_HAND->create_money_array(amt, place),
                               place);
   _current_bet = data->bet + amt;
   data->bet = _current_bet;
   _pot += amt;

   add_succeeded_mess("$N raise$s the bet by " +
                      MONEY_HAND->money_value_string(raise_amt, place) +
                      " to " +
                      MONEY_HAND->money_value_string(_current_bet, place) +
                      " on $D.\n");
   next_person_turn();
   return 1;
} /* do_raise() */

int do_fold() {
   class player_data data;
   string id;
   int not_folded;

   // Get another card.
   if (!is_game_started()) {
      add_failed_mess("The game has not started.\n");
      return 0;
   }

   if (!is_current_player(this_player())) {
      add_failed_mess("Not your turn to bet.\n");
      return 0;
   }

   data = query_player_data(query_current_player());
   data->state = POKER_STATE_FOLDED;

   add_succeeded_mess("$N fold$s on $D.\n");
   // Figure out if there is only one non-folded player left.
   foreach (id in query_started_player_ids()) {
      data = query_player_data(id);
      if (data->state != POKER_STATE_FOLDED) {
         not_folded++;
      }
   }
   if (not_folded == 1) {
      call_out("complete_round", 0, 1);
      _poker_phase = POKER_STATE_END;
   } else if (!check_end_round()) {
      next_person_turn();
   }
   return 1;
} /* do_fold() */

/**
 * Starts a nice furry game.
 */
int do_start() {
   int old_pot;

   if (!is_open_for("poker", this_player()->query_name())) {
      add_failed_mess("The poker table is not open.\n");
      return 0;
   }

   //
   // There must eb enough money in the float for everyone to bid the max amo
   // amount and win with a poker.
   //
   if (!is_playing(this_player())) {
      add_failed_mess("You must be playing the game to start it.\n");
      return 0;
   }

   old_pot = _pot;
   if (!start_game()) {
      add_failed_mess("You need at least two people to play poker.\n");
      return 0;
   }

   adjust_float(old_pot);
   //_revenue += old_pot;
   add_succeeded_mess("$N $V a game on $D.\n");
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

   finish_game(0);
   reset_game();
   return 1;
} /* do_finish() */

string query_main_status(int hint) {
   string place;
   string ret;
   string name;
   int amt;

   place = query_money_place();
   ret = "$I$0=Poker table:\n"
          "$I$6=   Maximum bet: " +
              MONEY_HAND->money_value_string(_max_bet, place) +
          "\n$I$6=   Minimum bet: " +
              MONEY_HAND->money_value_string(_min_bet, place) +
          "\n$I$6=   Ante       : " +
              MONEY_HAND->money_value_string(_ante_amount, place) +
          "\n$I$6=   House Ante : " +
              MONEY_HAND->money_value_string(_ante_house, place) +
          "\n$I$6=   Draw Rounds: " + _max_draw_rounds +
          "\n$I$6=   Cut        : " + _house_cut + "%"
          "\n$I$6=   Revenue    : " +
              MONEY_HAND->money_value_string(query_revenue(), place) +
          "\n$I$0=";
   foreach (name, amt in _player_stats) {
      ret += sprintf(" %-13s: %s\n" , name,
                     MONEY_HAND->money_value_string(amt, place));
   }
   return ret + "\n";
} /* query_main_status() */

/**
 * This method sets the ante amounts.
 * @param str the amount string
 * @param max_bet if it a max or min bet to set
 */
int do_set_ante(string str, int ante_game) {
   string place;
   int value;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to change the paramaters of "
                   "$D.\n");
      return 0;
   }
   if (lower_case(str) == "none" && !ante_game) {
      value = 0;
   } else {
      place = query_money_place();
      value = MONEY_HAND->value_from_string(str, place);
      if (!value) {
         add_failed_mess("Unable to parse the string " + str + ".\n");
         return 0;
      }
   }
   if (ante_game) {
      _ante_amount = value;
      add_succeeded_mess("$N set$s the ante amount to play to " +
          MONEY_HAND->money_value_string(value, place) + " on $D.\n");
   } else {
      _ante_house = value;
      add_succeeded_mess("$N set$s the ante amount for the house to " +
          MONEY_HAND->money_value_string(value, place) + " on $D.\n");
   }

   return 1;
} /* do_set_ante() */

/**
 * This method sets the ante amounts.
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
 * This method sets the cut
 * @param percent the cut percentage
 */
int do_set_cut(int percent) {

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to change the paramaters of "
                   "$D.\n");
      return 0;
   }
   if (percent < 0) {
      add_failed_mess("You cannot set the cut less than 0.\n");
      return 0;
   }
   if (percent > 100) {
      add_failed_mess("You cannot set the cut to greator than 100%.\n");
      return 0;
   }

   _house_cut = percent;

   add_succeeded_mess("$N set$s the cut to " + percent + "% on $D.\n");
   return 1;
} /* do_set_cut() */

/**
 * This method sets the maximum number of draw rounds.
 * @param percent the cut percentage
 * @param draw the maximum number of draw rounds
 */
int do_set_draw(int draw) {

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to change the paramaters of "
                   "$D.\n");
      return 0;
   }
   if (draw < 0) {
      add_failed_mess("You cannot set the number of draws less than 0.\n");
      return 0;
   }
   if (draw > 5) {
      add_failed_mess("You cannot set the draw to greator than 5.\n");
      return 0;
   }

   _max_draw_rounds = draw;

   add_succeeded_mess("$N set$s the number of draw rounds to " + draw + " on $D.\n");
   return 1;
} /* do_set_draw() */

/**
 * This method shows the current set of house rules.  This will contain
 * any modifiable bits.
 */
int do_rules() {
   string ret;
   string place;

   place = query_money_place();
   ret = "The rules for this table are:\n";
   ret += "$I$6=   Maximum amount that can be raised " +
              MONEY_HAND->money_value_string(_max_bet, place) +
          "\n$I$6=   Minimum amount that can be raised " +
              MONEY_HAND->money_value_string(_min_bet, place) +
          "\n$I$6=   Total ante " +
              MONEY_HAND->money_value_string(_ante_amount + _ante_house, place) +
          "\n$I$6=   Cut of the pool : " + _house_cut +
          "\n$I$6=   Draw Rounds     : " + _max_draw_rounds +
          "\n$I$6=   House Ante      : " +
              MONEY_HAND->money_value_string(_ante_house, place);
   write("$P$Rules$P$" + ret);
} /* do_rules() */

int do_reset() {
   if (::do_reset()) {
      _player_stats = ([ ]);
      add_succeeded_mess("$N clear$s the player stats.\n");
      return 1;
   }
} /* do_reset() */

void init() {
   multiplayer_base::init();
   commercial::init();
   add_command("discard", "<string'card'> on <direct:object>",
               (: do_discard($4[0]) :));
   add_command("ante", "on <direct:object>",
               (: do_ante() :));
   add_command("check", "on <direct:object>",
               (: do_check() :));
   add_command("call", "on <direct:object>",
               (: do_call() :));
   add_command("bet", "<string'total bet'> on <direct:object>",
               (: do_bet($4[0]) :));
   add_command("raise", "<string'raise'> on <direct:object>",
               (: do_raise($4[0]) :));
   add_command("fold", "on <direct:object>",
               (: do_fold() :));
   add_command("finish", "game on <direct:object>",
               (: do_finish() :));
   add_command("start", "game on <direct:object>",
               (: do_start() :));
   add_command("rules", "<direct:object>",
               (: do_rules() :));
   add_command("rules", "{for|on|of} <direct:object>",
               (: do_rules() :));

   if (environment()->is_allowed(this_player()->query_name())) {
      add_command("set", "draw <number'rounds'> on <direct:object>",
                  (: do_set_draw($4[0]) :));
      add_command("set", "cut <number'percentage'> on <direct:object>",
                  (: do_set_cut($4[0]) :));
      add_command("set", "ante house <string'amount|none'> on <direct:object>",
                  (: do_set_ante($4[0], 0) :));
      add_command("set", "ante game <string'amount'> on <direct:object>",
                  (: do_set_ante($4[0], 1) :));
      add_command("set", "minimum bet <string'amount'> on <direct:object>",
                  (: do_set_bet($4[0], 0) :));
      add_command("set", "maximum bet <string'amount'> on <direct:object>",
                  (: do_set_bet($4[0], 1) :));
   }
} /* init() */

void resign_person(string id) {
   class player_data data;
   int not_folded;

   data = query_player_data(id);
   if (data && data->state != POKER_STATE_FOLDED) {
      // Make them fold.
      tell_all_players(query_player_cap_name(id) +
                       " resigns and folds.\n");
      data->state = POKER_STATE_FOLDED;
      foreach (id in query_started_player_ids()) {
         data = query_player_data(id);
         if (data->state != POKER_STATE_FOLDED) {
            not_folded++;
         }
      }
      if (not_folded == 1) {
         call_out("complete_round", 0, 1);
         _poker_phase = POKER_STATE_END;
      }
   } else {
      // Check and see if the round finished.
      switch (_poker_phase) {
      case POKER_STATE_ANTE :
         check_for_finish_ante();
         break;
      case POKER_STATE_BET :
         check_end_round();
         break;
      case POKER_STATE_DISCARD :
         finish_discard();
         break;
      }
   }
} /* resign_person() */

void event_exit(object ob, string mess, object to) {
   string id;

   if (userp(ob) && to != environment()) {
      id = find_player_id_of_person(ob);
      if (id) {
         resign_person(id);
      }
   }
} /* event_exit() */

void multiplayer_someone_resigns(string id, string name) {
   resign_person(id);
} /* multiplayer_someone_resigns() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
   mapping map;

   map = commercial::query_dynamic_auto_load();
   multiplayer_base::query_dynamic_auto_load(map);
   add_auto_load_value(map, BOARD_TAG, "return", _return);
   //add_auto_load_value(map, BOARD_TAG, "revenue", _revenue);
   add_auto_load_value(map, BOARD_TAG, "ante amount", _ante_amount);
   add_auto_load_value(map, BOARD_TAG, "house cut", _house_cut);
   add_auto_load_value(map, BOARD_TAG, "ante house", _ante_house);
   add_auto_load_value(map, BOARD_TAG, "min bet", _min_bet);
   add_auto_load_value(map, BOARD_TAG, "max bet", _max_bet);
   add_auto_load_value(map, BOARD_TAG, "current bet", _current_bet);
   add_auto_load_value(map, BOARD_TAG, "pot", _pot);
   add_auto_load_value(map, BOARD_TAG, "deck", _deck);
   add_auto_load_value(map, BOARD_TAG, "poker phase", _poker_phase);
   add_auto_load_value(map, BOARD_TAG, "draw round", _draw_round);
   add_auto_load_value(map, BOARD_TAG, "max draw rounds", _max_draw_rounds);
   add_auto_load_value(map, BOARD_TAG, "discard", _discard);
   add_auto_load_value(map, BOARD_TAG, "player stats", _player_stats);
   return map;
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_dynamic_arg(mapping map, object player) {
   commercial::init_dynamic_arg(map, player);
   multiplayer_base::init_dynamic_arg(map, player);

   _return = query_auto_load_value(map, BOARD_TAG, "return");
   //_revenue = query_auto_load_value(map, BOARD_TAG, "revenue");
   _ante_amount = query_auto_load_value(map, BOARD_TAG, "ante amount");
   _ante_house = query_auto_load_value(map, BOARD_TAG, "ante house");
   _house_cut = query_auto_load_value(map, BOARD_TAG, "house cut");
   _deck = query_auto_load_value(map, BOARD_TAG, "deck");
   _current_bet = query_auto_load_value(map, BOARD_TAG, "current bet");
   _pot = query_auto_load_value(map, BOARD_TAG, "pot");
   _min_bet = query_auto_load_value(map, BOARD_TAG, "min bet");
   _max_bet = query_auto_load_value(map, BOARD_TAG, "max bet");
   _poker_phase = query_auto_load_value(map, BOARD_TAG, "poker phase");
   _draw_round = query_auto_load_value(map, BOARD_TAG, "draw round");
   _max_draw_rounds = query_auto_load_value(map, BOARD_TAG, "max draw rounds");
   _discard = query_auto_load_value(map, BOARD_TAG, "discard");
   _player_stats = query_auto_load_value(map, BOARD_TAG, "player stats");
   if (!_player_stats) {
      _player_stats = ([ ]);
   }
} /* init_dynamic_arg() */
