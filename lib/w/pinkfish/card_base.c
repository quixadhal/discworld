/**
 * This is a base to make up randomised decks of cards.
 * @author Pinkfish
 * @started Thu Dec  7 07:34:10 PST 2000
 */
#include "card_base.h"

#define CARD_NUM_MAX CARD_NUMBER_KING

class playing_card* make_deck(int number_of_decks,
                              int number_of_jokers) {
   class playing_card* deck;
   class playing_card card;
   int i;
   int suit;
   int num;

   for (i = 0; i < number_of_decks; i++) {
      for (suit = CARD_SUIT_HEARTS; suit <= CARD_SUIT_SPADES; suit++) {
         for (num = 1; num <= CARD_NUM_MAX; num++) {
            card = new(class playing_card);
            card->suit = suit;
            card->number = num;
            deck += ({ card });
         }
      }
   }
   for (i = 0; i < number_of_jokers; i++) {
      card = new(class playing_card);
      card->suit = CARD_SUIT_JOKER;
      deck += ({ card });
   }
   return deck;
} /* make_deck() */

/**
 * This method scrabled the deck and shuffles it.
 * @param deck the deck to shuffle
 * @return a shuffled deck
 */
class playing_card* shuffle_deck(class playing_card* deck) {
   class playing_card* new_deck;
   int pos;
   int i;

   for (i = 0; i < 2; i++) {
      new_deck = ({ });
      while (sizeof(deck)) {
         pos = random(sizeof(deck));
         new_deck += deck[pos..pos];
         deck = deck[0..pos - 1] + deck[pos+1..];
      }
      deck = new_deck;
   }
   return deck;
} /* shuffle_deck() */

/**
 * This method returns the three character string for the card.
 * @return the three character string for the card
 */
string query_card_string(class playing_card card) {
   string ret;

   ret = "";
   switch (card->suit) {
   case CARD_SUIT_SPADES :
      ret = "S";
      break;
   case CARD_SUIT_HEARTS :
      ret = "H";
      break;
   case CARD_SUIT_DIAMONDS :
      ret = "D";
      break;
   case CARD_SUIT_CLUBS :
      ret = "C";
      break;
   case CARD_SUIT_JOKER :
      return "*J*";
   }

   if (card->number > 10) {
      switch (card->number) {
      case CARD_NUMBER_JACK :
         ret = "J " + ret;
         break;
      case CARD_NUMBER_QUEEN :
         ret += "Q " + ret;
         break;
      case CARD_NUMBER_KING :
         ret += "K " + ret;
         break;
      }
   } else {
      ret = sprintf("%2d%s", card->number, ret);
   }
   return ret;
} /* query_card_string() */

/**
 * This method determines if the card colour is red.
 * @param card the card to check
 * @return 1 if it is, 0 if not
 */
int is_card_red(class playing_card card) {
   return card->suit == CARD_SUIT_HEARTS || card->suit == CARD_SUIT_DIAMONDS;
} /* is_card_red() */

/**
 * This method determines if the card colour is black.
 * @param card the card to check
 * @return 1 if it is, 0 if not
 */
int is_card_black(class playing_card card) {
   return card->suit == CARD_SUIT_SPADES || card->suit == CARD_SUIT_CLUBS;
} /* is_card_black() */

/**
 * This method checks to see if the card is a joker.
 * @param card the card to check
 * @return 1 if it is, 0 if not
 */
int is_card_joker(class playing_card card) {
   return card->suit == CARD_SUIT_JOKER;
} /* is_card_joker() */

/**
 * This method makes a array for a 3x3 card.
 * @param card the card to make 3x3
 * @return the three line array
 */
string* query_card_three(class playing_card card) {
   string* lines;

   lines = allocate(3);
   switch (card->suit) {
   case CARD_SUIT_HEARTS :
      lines[0] = "H  ";
      lines[2] = "  H";
      break;
   case CARD_SUIT_DIAMONDS :
      lines[0] = "D  ";
      lines[2] = "  D";
      break;
   case CARD_SUIT_CLUBS :
      lines[0] = "C  ";
      lines[2] = "  C";
      break;
   case CARD_SUIT_SPADES :
      lines[0] = "S  ";
      lines[2] = "  S";
      break;
   case CARD_SUIT_JOKER :
      lines[0] = "J *";
      lines[1] = " O ";
      lines[2] = "* K";
      return lines;
   }

   if (card->number <= 10) {
      lines[1] = sprintf(" %2d", card->number);
   } else {
      switch (card->number) {
      case CARD_NUMBER_KING :
         lines[1] = " K ";
         break;
      case CARD_NUMBER_QUEEN :
         lines[1] = " Q ";
         break;
      case CARD_NUMBER_JACK :
         lines[1] = " J ";
         break;
      }
   }
   return lines;
} /* query_card_three() */

/**
 * This method makes a array for a 2x2 card.
 * @param card the card to make 2x2
 * @return the two line array
 */
string* query_card_two(class playing_card card) {
   string* lines;

   lines = allocate(2);
   switch (card->suit) {
   case CARD_SUIT_HEARTS :
      lines[0] = "H ";
      break;
   case CARD_SUIT_DIAMONDS :
      lines[0] = "D ";
      break;
   case CARD_SUIT_CLUBS :
      lines[0] = "C ";
      break;
   case CARD_SUIT_SPADES :
      lines[0] = "S ";
      break;
   case CARD_SUIT_JOKER :
      lines[0] = "J*";
      lines[1] = "*O";
      return lines;
   }

   if (card->number <= 10) {
      lines[1] = sprintf("%-2d", card->number);
   } else {
      switch (card->number) {
      case CARD_NUMBER_KING :
         lines[1] = " K";
         break;
      case CARD_NUMBER_QUEEN :
         lines[1] = " Q";
         break;
      case CARD_NUMBER_JACK :
         lines[1] = " J";
         break;
      }
   }
   return lines;
} /* query_card_three() */

/**
 * This method returns a string showing the hand.
 * @param hand the hand to show
 * @param three 1 for a 3x3, 0 for a 2x2
 * @return a string representation of the hand
 */
string query_hand_string(class playing_card* hand, int flags) {
   string hand_str;
   int i;
   int j;
   string top;
   string line;
   string start;
   string end;
   mixed card_str;

   hand_str = "";
   if (!(flags & CARD_HAND_NO_ADORNMENTS)) {
      if (flags & CARD_HAND_THREE ||
          flags & CARD_HAND_SINGLE) {
         top = "+---+";
      } else {
         top = "+--+";
      }

      //
      // Make the top of the card.
      //
      line = "";
      for (i = 0; i < sizeof(hand); i++) {
         line += top;
      }
      line += "\n";
      hand_str += line;

      start = "|";
      end = "|";
   } else {
      line = "";
      start = " ";
      end = " ";
   }

   hand_str += "\n";

   if (flags & CARD_HAND_THREE) {
      card_str = map(hand, (: query_card_three :));
   } else if (flags & CARD_HAND_SINGLE) {
      card_str = map(hand, (: ({ query_card_string($1) }) :));
   } else {
      card_str = map(hand, (: query_card_two :));
   }

   for (j = 0; j < sizeof(card_str[0]); j++) {
      for (i = 0; i < sizeof(card_str); i++) {
         card_str += start + (card_str[i][j]) + end;
      }
   }

   hand_str += line;

   return hand_str;
} /* query_hand_string() */
