.DT
haggle_handler.c
Disk World autodoc help
haggle_handler.c

.SH Description
.SP 5 5

This handler controls the npcs haggling the price of something with 
a player.  Currently it only handles sell barganing.


Factors of relevance: 
.EP
.SO 6 3 -11

	1. 	merchants' guild rating 
	2. 	race of customer vs. shopkeeper 
.EO
.SP 5 5

.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/shops/bank.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^bargain_factor%^RESET%^
.EI
.SI 5
int bargain_factor(object keeper, object customer)
.EI
.SP 7 5

This method determines the barganing factor between the customer and
the shop keeper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
keeper - the shop keeper
.EP
.SP 9 5
customer - the customer of the shop

.EP

.SI 3
* %^BOLD%^insult_factor%^RESET%^
.EI
.SI 5
int insult_factor(object keeper, object customer)
.EI
.SP 7 5

This method determines the insult factor between the customer and the
shop keeper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
keeper - the shop keeper
.EP
.SP 9 5
customer - the customer in the shop

.EP

.SI 3
* %^BOLD%^sell_haggle%^RESET%^
.EI
.SI 5
int * sell_haggle(int value, int offer, int asking, int insults, string place, object keeper, object customer)
.EI
.SP 7 5

This method does the actual haggle on a sell.
The logic is as follows:

.EP
.SP 7 5
if offer > asking then
.EP
.SP 7 5
   give back a boggle response
.EP
.SP 7 5
else if offer == asking then
.EP
.SP 7 5
   sell the thing and give an accepted saying
.EP
.SP 7 5
else if offer * insult_factor > asking * 100 then
.EP
.SP 7 5
   adjustment = ( ( asking - offer ) * bargain_factor ) / 100
.EP
.SP 7 5
   asking -= adjustment
.EP
.SP 7 5
   return an adjustment message
.EP
.SP 7 5
else
.EP
.SP 7 5
   return an insulted message
.EP
.SP 7 5
endif
.EP
.SP 7 5
The return from this function is an array of three elements, the first is the a flag saying if the item was accepted or not, the second is the current asking price and the last is the current insult level. 
.EP
.SP 7 5
({
.EP
.SP 7 5
   flag (0/1), // This is 0 if unable to sell, 1 if sold
.EP
.SP 7 5
   asking,     // the current asking price
.EP
.SP 7 5
   insults,    // the current insult level
.EP
.SP 7 5
})
.EP
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
value - the real value of the item
.EP
.SP 9 5
offer - the offered value for the item
.EP
.SP 9 5
asking - the current asking price
.EP
.SP 9 5
insults - the current insult level
.EP
.SP 9 5
place - the place for the money strings
.EP
.SP 9 5
keeper - the shop keeper
.EP
.SP 9 5
customer - the customer
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array as described above
.EP


