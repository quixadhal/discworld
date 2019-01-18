.DT
locksmith.c
Disk World autodoc help
locksmith.c

.SH Description
.SP 5 5

-- Inheritable for playerhousing locksmiths --
An NPC using this inherit is able to unlock the doors to playerhouses and
commercial properties when prompted to do so by the rightful owner.

-- How such an NPC works from a player's point of view --
- A player can "hire" the npc if he wants a door of his house opened.
- He then has to "pay" (*) the amount the npc demands (if that player killed
  the npc before, the base price is doubled).  He has 120 seconds to do so.
- After that happens, the npc will follow the player until the player
  "asks"s him to opens an exit door.
- Then player is prompted to enter [y]es [n]o".  If the player confirms and
  the handler doesn't return this as well, the npc complains and walks off
  (keeping the money, of course).  Otherwise it unlocks the door and then
  leaves.

(*) Giving the money to the npc does not work, it'll return it.

The same procedure applies if the player wants all doors opened.

All attempts to hire a locksmith with the result of that action are logged
in the file defined in LOG

In case of problems you can always try to:
1) look at the log to see if the player in question hired and payed the npc
2) call query_client() on the npc
3) call free_for_hire() on the npc
4) dest and re-load the npc


.EP
.SP 10 5


Written by Wirble

Started Nov. 2001

.EP

.SH Example
.SI 5
inherit "/std/npc/types/locksmith";

void setup() {
    // basic npc-setup
    set_savefile(SAVE + "glodson_killers");
    set_price(20000); // 50 $AM
    set_currency_region("Ankh-Morpork");
    set_domain("/d/am/");
}

.EI
.SH Inherits
.SP 5 5
This class inherits the following classes /obj/monster.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/shops/bank.h, /include/armoury.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^confirmation%^RESET%^
.EI
.SI 5
void confirmation(string response, object player)
.EI
.SP 7 5

This function is used to evaluate the client's response to the "is this the
door"-question.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
response - the response the player entered
.EP
.SP 9 5
player - the player doing the input

.EP

.SI 3
* %^BOLD%^free_for_hire%^RESET%^
.EI
.SI 5
void free_for_hire()
.EI
.SP 7 5

Use this function if something went wrong and the NPC does not react to the
client/new clients anymore.

.EP

.SI 3
* %^BOLD%^move_recording%^RESET%^
.EI
.SI 5
void move_recording()
.EI
.SP 7 5

This function gets called after every move of the NPC and records it for a
potential return.  It should not be called manually.

.EP

.SI 3
* %^BOLD%^query_client%^RESET%^
.EI
.SI 5
string query_client()
.EI
.SP 7 5

This queries the client's name.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the client or 0 for no client

.EP

.SI 3
* %^BOLD%^query_domain%^RESET%^
.EI
.SI 5
string query_domain()
.EI
.SP 7 5

This queries the NPC's domain.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the first 6 chars of the domain.

.EP

.SI 3
* %^BOLD%^query_killers%^RESET%^
.EI
.SI 5
string * query_killers()
.EI
.SP 7 5

This queries the killers of the NPC.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array listing the killers' names

.EP

.SI 3
* %^BOLD%^query_last_room%^RESET%^
.EI
.SI 5
string query_last_room()
.EI
.SP 7 5

This queries the last valid room the NPC was in.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the basename of the last room

.EP

.SI 3
* %^BOLD%^query_responses%^RESET%^
.EI
.SI 5
string * query_responses()
.EI
.SP 7 5

This queries the responses of the NPC.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of response-strings

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^set_currency_region%^RESET%^
.EI
.SI 5
int set_currency_region(string region)
.EI
.SP 7 5

This function sets the NPCs region for determining the currency.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
region - the region to use
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^set_domain%^RESET%^
.EI
.SI 5
int set_domain(string domain)
.EI
.SP 7 5

This sets the domain the NPC is restricted to.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
domain - the first 6 letters of the domain-path, ie "/d/am/"
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^set_price%^RESET%^
.EI
.SI 5
int set_price(int price)
.EI
.SP 7 5

This function sets the base price for unlocking a door.  If the savefile is
set, a killer will be charged the double price.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
price - the base-price in money-units
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^set_responses%^RESET%^
.EI
.SI 5
int set_responses(string * responses)
.EI
.SP 7 5

This function sets the NPC's responses for the various commands.  If not
used generic answers will be used.  Mind that you cannot enter only a few
responses.  It's all or nothing.  The array should be like this:
1) response to failed confirmation
2) response for not hired by that player
3) response for trying to enter another player's house
4) response for successful opening of the door
5) response for already hired by other player
6) response for not paid yet
7) response for attempt to hire
8) response for already paid
9) response for not enough money to pay
10) response for cannot find specified exit
11) response for successful payment (please include 'ask' hint)
12) response for not leaving the domain
13) response for already hired by that player
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
responses - an array with the responses, as explained above
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^set_savefile%^RESET%^
.EI
.SI 5
int set_savefile(string file)
.EI
.SP 7 5

Use this function to set the file where the killers of the NPC are stored,
if you want to charge the killers the double price for opening their house.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
file - the name of the file to save to
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP


