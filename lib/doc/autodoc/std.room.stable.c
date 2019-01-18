.DT
stable.c
Disk World autodoc help
stable.c

.SH Description
.SP 5 5

A stable in which to house your lucky transport.
.EP
.SP 10 5


Written by Pinkfish

Started Tue Nov 30 23:37:12 PST 1999

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player.h, /include/shops/bank.h, /include/playtesters.h and /include/money.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^stable%^RESET%^
class stable {
int date_in;
int last_paid;
string name;
mixed * save_info;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_stable_type%^RESET%^
.EI
.SI 5
void add_stable_type(string type)
.EI
.SP 7 5

This method adds in a type of transport that can be stabled in this
stable.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of transport to add

.EP

.SI 3
* %^BOLD%^add_to_stable%^RESET%^
.EI
.SI 5
int add_to_stable(string name, object * horses)
.EI
.SP 7 5

This method adds the horses to the stable.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the player to whose stable we are adding the horse
.EP
.SP 9 5
horses - the horses to add
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are successfuly added

.EP

.SI 3
* %^BOLD%^adjust_balance%^RESET%^
.EI
.SI 5
void adjust_balance(string name, int amt)
.EI
.SP 7 5

This method adjusts the balance of the players stable account.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player
.EP
.SP 9 5
amt - the amount to adjust the account by

.EP

.SI 3
* %^BOLD%^can_stable_type%^RESET%^
.EI
.SI 5
int can_stable_type(string type)
.EI
.SP 7 5

This method checks to see if the specified type of transport can be
stabled here.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it can be stabled, 0 if not

.EP

.SI 3
* %^BOLD%^do_deposit%^RESET%^
.EI
.SI 5
int do_deposit(string mon)
.EI
.SP 7 5

This method adds money to the players stable account.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the money to be deposited
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_list%^RESET%^
.EI
.SI 5
int do_list()
.EI
.SP 7 5

This method shows you a list of all the horse you have in the stables.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_return%^RESET%^
.EI
.SI 5
int do_return(string num)
.EI
.SP 7 5

This method removes an animal from the stables and runs off with it.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_stable%^RESET%^
.EI
.SI 5
int do_stable(object * obs)
.EI
.SP 7 5

This method allows the person to stable their animal.  It will cost
them (of course).

.EP

.SI 3
* %^BOLD%^load_me%^RESET%^
.EI
.SI 5
void load_me()
.EI
.SP 7 5

This method loads the room.

.EP

.SI 3
* %^BOLD%^print_ok_message%^RESET%^
.EI
.SI 5
void print_ok_message(object me, object * horses)
.EI
.SP 7 5

This message prints out something happy when you actually do the
stabling.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
me - the person doing the stabling
.EP
.SP 9 5
horses - the horses being stabled

.EP

.SI 3
* %^BOLD%^query_balance%^RESET%^
.EI
.SI 5
int query_balance(string name)
.EI
.SP 7 5

This method returns the players current balance in their stable
account.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current stable balance

.EP

.SI 3
* %^BOLD%^query_cost_per_week%^RESET%^
.EI
.SI 5
int query_cost_per_week()
.EI
.SP 7 5

This method returns the cost per week in the stables.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the cost per week

.EP

.SI 3
* %^BOLD%^query_player_money%^RESET%^
.EI
.SI 5
int query_player_money(object player, string place)
.EI
.SP 7 5

This method returns the amount of money on the player and in their
account.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player to check the balance of
.EP
.SP 9 5
place - the place in which to check the money
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the total money on the player and in their account

.EP

.SI 3
* %^BOLD%^query_save_directory%^RESET%^
.EI
.SI 5
string query_save_directory()
.EI
.SP 7 5

This method queries the directory we will use to save the players
transports in.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the save directory

.EP

.SI 3
* %^BOLD%^query_stable_types%^RESET%^
.EI
.SI 5
string * query_stable_types()
.EI
.SP 7 5

This method returns the types of transports that can be stabled here.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the types of transport that can be stabled here

.EP

.SI 3
* %^BOLD%^query_stabling_cost%^RESET%^
.EI
.SI 5
int query_stabling_cost(string name, int pos)
.EI
.SP 7 5

This method returns the cost of stabling the horse up to the current
time.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the owner of the stable
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the cost of stabling the horse

.EP

.SI 3
* %^BOLD%^query_start_cost%^RESET%^
.EI
.SI 5
int query_start_cost()
.EI
.SP 7 5

This method returns the basic cost of stabling the animal.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the basic cost

.EP

.SI 3
* %^BOLD%^remove_player_money%^RESET%^
.EI
.SI 5
void remove_player_money(object player, int amt, string place)
.EI
.SP 7 5

This method removes money from the player, it will try and remove from
their stable balance before taking money off them.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player to remove the money off
.EP
.SP 9 5
amt - the amount to remove
.EP
.SP 9 5
place - the place the money is from

.EP

.SI 3
* %^BOLD%^save_me%^RESET%^
.EI
.SI 5
void save_me()
.EI
.SP 7 5

This method saves the room.

.EP

.SI 3
* %^BOLD%^set_balance%^RESET%^
.EI
.SI 5
void set_balance(string name, int amt)
.EI
.SP 7 5

This method sets the current balance of the player stable account.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player
.EP
.SP 9 5
amt - the amount to set the stable account as

.EP

.SI 3
* %^BOLD%^set_cost_per_week%^RESET%^
.EI
.SI 5
void set_cost_per_week(int cost_per_week)
.EI
.SP 7 5

This method sets the cost per week in the stables.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cost_per_week - the cost per week

.EP

.SI 3
* %^BOLD%^set_save_directory%^RESET%^
.EI
.SI 5
void set_save_directory(string directory)
.EI
.SP 7 5

This method sets the directory we will use to save the players
transports in.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
save - the directory to save the transports in

.EP

.SI 3
* %^BOLD%^set_start_cost%^RESET%^
.EI
.SI 5
void set_start_cost(int start_cost)
.EI
.SP 7 5

This method sets the basic cost of stabling the animal.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
start_cost - the basic cost

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^check_stable%^RESET%^
.EI
.SI 5
void check_stable(string mess, int cost, object * ok, string place)
.EI
.SP 7 5

This method is used to confirm that they want to spend theirn
money stabling the horse.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mess - what the player types
.EP
.SP 9 5
cost - the cost of the stabling
.EP
.SP 9 5
ok - the horses being stabled

.EP

.SI 3
* %^BOLD%^query_stable%^RESET%^
.EI
.SI 5
class stable * query_stable(string name)
.EI
.SP 7 5

This method will find the stable associated with the specified player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the found stable, or 0 if not found

.EP

.SI 3
* %^BOLD%^set_stable%^RESET%^
.EI
.SI 5
void set_stable(string name, class stable * value)
.EI
.SP 7 5

This method sets the stable to the new value.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the stable
.EP
.SP 9 5
value - the new value of the stable

.EP


