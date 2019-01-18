.DT
card_base.h
Disk World autodoc help
card_base.h

.SH Description
.SP 5 5

This is a base to make up randomised decks of cards.
.EP
.SP 10 5


Written by Pinkfish

Started Thu Dec  7 07:34:10 PST 2000

.EP

.SH Classes

.SI 3
* %^BOLD%^playing_card%^RESET%^
class playing_card {
int suit;
int number;
}

.EI
.SP 7 5

The playing card class.
.EP


.SH Defines
.SI 3
* %^BOLD%^CARD_HAND_LETTERS%^RESET%^
.EI
.SP 7 5
Put a set of letters over the top of the cards to match up. 
.EP

.SI 3
* %^BOLD%^CARD_HAND_NO_ADORNMENTS%^RESET%^
.EI
.SP 7 5
When generating a hand, this leaves off all the frilly bits. 
.EP

.SI 3
* %^BOLD%^CARD_HAND_SINGLE%^RESET%^
.EI
.SP 7 5
When generating a hand this makes a 3x1 card. 
.EP

.SI 3
* %^BOLD%^CARD_HAND_THREE%^RESET%^
.EI
.SP 7 5
When generating a hand this makes a 3x3 card. 
.EP

.SI 3
* %^BOLD%^CARD_HAND_TWO%^RESET%^
.EI
.SP 7 5
When generating a hand this makes a 2x2 card. 
.EP

.SI 3
* %^BOLD%^CARD_NUMBER_ACE%^RESET%^
.EI
.SP 7 5
The number for an ace. 
.EP

.SI 3
* %^BOLD%^CARD_NUMBER_JACK%^RESET%^
.EI
.SP 7 5
The number for a jack. 
.EP

.SI 3
* %^BOLD%^CARD_NUMBER_KING%^RESET%^
.EI
.SP 7 5
The number for a jack. 
.EP

.SI 3
* %^BOLD%^CARD_NUMBER_QUEEN%^RESET%^
.EI
.SP 7 5
The number for a jack. 
.EP

.SI 3
* %^BOLD%^CARD_SUIT_CLUBS%^RESET%^
.EI
.SP 7 5
The suit define for clubs. 
.EP

.SI 3
* %^BOLD%^CARD_SUIT_DIAMONDS%^RESET%^
.EI
.SP 7 5
The suit define for diamonds. 
.EP

.SI 3
* %^BOLD%^CARD_SUIT_HEARTS%^RESET%^
.EI
.SP 7 5
The suit define for hearts. 
.EP

.SI 3
* %^BOLD%^CARD_SUIT_JOKER%^RESET%^
.EI
.SP 7 5
The suit define for jokers. 
.EP

.SI 3
* %^BOLD%^CARD_SUIT_SPADES%^RESET%^
.EI
.SP 7 5
The suit define for spades. 
.EP

