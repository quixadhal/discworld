/**
 * This is a base to make up randomised decks of cards.
 * @author Pinkfish
 * @started Thu Dec  7 07:34:10 PST 2000
 */
#ifndef __CARD_BASE_H
/** @ignore yes */
#define __CARD_BASE_H

/** The suit define for hearts. */
#define CARD_SUIT_HEARTS   1
/** The suit define for diamonds. */
#define CARD_SUIT_DIAMONDS 2
/** The suit define for clubs. */
#define CARD_SUIT_CLUBS    3
/** The suit define for spades. */
#define CARD_SUIT_SPADES   4
/** The suit define for jokers. */
#define CARD_SUIT_JOKER    5

/** The number for a jack. */
#define CARD_NUMBER_JACK    11
/** The number for a jack. */
#define CARD_NUMBER_QUEEN   12
/** The number for a jack. */
#define CARD_NUMBER_KING    13
/** The number for an ace. */
#define CARD_NUMBER_ACE     1

/** When generating a hand this makes a 3x3 card. */
#define CARD_HAND_THREE         1
/** When generating a hand this makes a 3x1 card. */
#define CARD_HAND_SINGLE        2
/** When generating a hand this makes a 2x2 card. */
#define CARD_HAND_TWO           4
/** When generating a hand, this leaves off all the frilly bits. */
#define CARD_HAND_NO_ADORNMENTS 8
/** Put a set of letters over the top of the cards to match up. */
#define CARD_HAND_LETTERS       16

#ifndef CARD_BASE_NO_CLASSES
/**
 * The playing card class.
 * @element suit the suit of the card
 * @element number the number of the card
 */
class playing_card {
   int suit;
   int number;
}
#endif

#endif
