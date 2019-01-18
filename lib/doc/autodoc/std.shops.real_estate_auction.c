.DT
real_estate_auction.c
Disk World autodoc help
real_estate_auction.c

.SH Description
.SP 5 5

This method adds in an area to which the real estate office will look
for houses.
.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/shops/auction_house.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/room/auction.h, /include/nomic_system.h, /include/move_failures.h, /include/housing.h, /include/shops/bank.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_house%^RESET%^
.EI
.SI 5
int add_house(string house)
.EI
.SP 7 5

Add a house for auction.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^add_housing_area%^RESET%^
.EI
.SI 5
void add_housing_area(string area)
.EI

.SI 3
* %^BOLD%^after_reset%^RESET%^
.EI
.SI 5
void after_reset()
.EI
.SP 7 5

This runs through the list of vacant houses and checks to see if it
should add them up for auction or not.

.EP

.SI 3
* %^BOLD%^check_can_bid%^RESET%^
.EI
.SI 5
int check_can_bid(object person, class auction which, int bid_amount)
.EI
.SP 7 5

This method checks to make sure they have enough money in their bank
accounts to make the bid.  This function should be setup to be called
as the big checking function in the inherit of this room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person being checked for bidding
.EP
.SP 9 5
which - the auction being tested to be bid on
.EP
.SP 9 5
bid_amount - how much they are bidding

.EP

.SI 3
* %^BOLD%^do_agreement%^RESET%^
.EI
.SI 5
int do_agreement()
.EI
.SP 7 5

Request a new rental agreement for a house they might have
lost.

.EP

.SI 3
* %^BOLD%^housing_auction%^RESET%^
.EI
.SI 5
void housing_auction(int event_type, class auction auct, string house, string buyer, int cost, object * obs)
.EI
.SP 7 5

This function is called during the various phases of the housing
auction.  When each phase ends this method is called and anything
that needs to be done for the end of the phase is sorted out.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
event_type - the event that has finished
.EP
.SP 9 5
auct - the auction that just finished
.EP
.SP 9 5
house - the path of the house being auctioned
.EP
.SP 9 5
buyer - the buyer of the house (if sold)
.EP
.SP 9 5
cost - how much the house costs
.EP
.SP 9 5
obs - the things that were sold (rental agreementsin this case)

.EP

.SI 3
* %^BOLD%^query_city%^RESET%^
.EI
.SI 5
string query_city()
.EI
.SP 7 5

Returns the city the real estate place thinks it lives in.

.EP

.SI 3
* %^BOLD%^query_housing_areas%^RESET%^
.EI
.SI 5
string * query_housing_areas()
.EI
.SP 7 5

This method returns all the areas in which the real estate office will
look for houses.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the areas to look for houses

.EP

.SI 3
* %^BOLD%^query_language%^RESET%^
.EI
.SI 5
string query_language()
.EI
.SP 7 5

Returns the language the real estate place will use for its agreements.

.EP

.SI 3
* %^BOLD%^reset_auctions_to_under_offer%^RESET%^
.EI
.SI 5
void reset_auctions_to_under_offer()
.EI
.SP 7 5

This function runs down the list of current auctions and makes sure they 
are all registered as "Under Offer" so if for some reason they are not, 
they won't be put up for sale again.

.EP

.SI 3
* %^BOLD%^set_city%^RESET%^
.EI
.SI 5
void set_city(string where)
.EI
.SP 7 5

This is just a cosmetic function to help set up the agreements. 

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
where - the city to which the agreements will say the property 
defaults to when you cease to own it

.EP

.SI 3
* %^BOLD%^set_language%^RESET%^
.EI
.SI 5
void set_language(string lang)
.EI
.SP 7 5

This is just a cosmetic function to help set up the agreements. 

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
the - agreements be written in

.EP

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


