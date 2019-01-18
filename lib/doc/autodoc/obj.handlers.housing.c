.DT
housing.c
Disk World autodoc help
housing.c

.SH Description
.SP 5 5

A handler to track player houses.  This handler should only be used for
general houses that are available for sale or rent. Don't add club or
special (eg. guildmaster) houses here or they'll get sold or rented :)

The handler stores information about registered player houses, makes houses
vacant if their owners are idle and charges rent on rented houses.

.EP
.SP 10 5


Written by Ceres

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/nomic_system.h, /include/player_handler.h, /include/housing.h, /include/am_time.h and /include/login_handler.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^housing%^RESET%^
class housing {
string * other_rooms;
string owner;
string address;
string region;
int type;
int value;
string bank;
}

.EI
.SP 7 5

This class stores all the data about a given house. The class is used
within the houses mapping. The key of the mapping is the basename of the
house (the filename of the main room) and the value is this class, which
explains why this class doesn't have a name or filename field for the house.

.EP


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_house%^RESET%^
.EI
.SI 5
int add_house(string house, string * other_rooms, string address, string region, int type)
.EI
.SP 7 5

Add a new house to the handler.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - the basename of the house
.EP
.SP 9 5
other_rooms - an array of other rooms in the house
.EP
.SP 9 5
address - a string for the address
.EP
.SP 9 5
region - the region (used for house sales)
.EP
.SP 9 5
type - an integer. 1 for owned, 0 for rented.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 for failure

.EP

.SI 3
* %^BOLD%^add_rooms%^RESET%^
.EI
.SI 5
int add_rooms(string house, string * rooms)
.EI
.SP 7 5

Add secondary rooms to a house.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - the basename of the house
.EP
.SP 9 5
an - array of basenames for the rooms
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success 0 for failure

.EP

.SI 3
* %^BOLD%^change%^RESET%^
.EI
.SI 5
int change()
.EI

.SI 3
* %^BOLD%^modify_house%^RESET%^
.EI
.SI 5
int modify_house(string house, string * other_rooms, string address, string region, int type)
.EI
.SP 7 5

Modify a houses information.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - the basename of the house
.EP
.SP 9 5
other_rooms - an array of other rooms in the house
.EP
.SP 9 5
address - a string for the address
.EP
.SP 9 5
region - the region (used for house sales)
.EP
.SP 9 5
type - an integer. 1 for owned, 0 for rented.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 for failure

.EP

.SI 3
* %^BOLD%^query_address%^RESET%^
.EI
.SI 5
string query_address(string house)
.EI
.SP 7 5

Find out the address of a house.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - The basename of the house
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the address.

.EP

.SI 3
* %^BOLD%^query_all_housing_area_citizenships%^RESET%^
.EI
.SI 5
mapping query_all_housing_area_citizenships()
.EI
.SP 7 5

This method returns all of the current mappings between the housing
area and the citizenship.  This shouuld not be used for anything
except debugging.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all the housing area stuff

.EP

.SI 3
* %^BOLD%^query_bank%^RESET%^
.EI
.SI 5
string query_bank(string house)
.EI
.SP 7 5

Return which bank account rent will be removed from for a house.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - The basename of the house
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the bank name.

.EP

.SI 3
* %^BOLD%^query_house%^RESET%^
.EI
.SI 5
int query_house(string house)
.EI
.SP 7 5

Check if a given house is registered.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - The basename of the house
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if its registered, 0 otherwise.

.EP

.SI 3
* %^BOLD%^query_housing_area_bank%^RESET%^
.EI
.SI 5
string query_housing_area_bank(string area)
.EI
.SP 7 5

This method returns the current mappings between the housing area and the
initial bank to use.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the initial bank used, DEFAULT_BANK if none set

.EP

.SI 3
* %^BOLD%^query_housing_area_citizenship%^RESET%^
.EI
.SI 5
string query_housing_area_citizenship(string area)
.EI
.SP 7 5

This method returns the current mappings between the housing area and the
required citizenship.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the citizenship required, 0 if none

.EP

.SI 3
* %^BOLD%^query_housing_area_place%^RESET%^
.EI
.SI 5
string query_housing_area_place(string area)
.EI
.SP 7 5

This method returns the current mappings between the housing area and the
money area ("Ankh-Morpork", "Counterweight Continent", etc) to use.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the money area used, DEFAULT_PLACE if none set

.EP

.SI 3
* %^BOLD%^query_owner%^RESET%^
.EI
.SI 5
string query_owner(string house)
.EI
.SP 7 5

Find out the current owner of this house.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - The basename of the house
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the owners name.

.EP

.SI 3
* %^BOLD%^query_region%^RESET%^
.EI
.SI 5
string query_region(string house)
.EI
.SP 7 5

Find out the region a house is in for sale/rental purposes.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - The basename of the house
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the region.

.EP

.SI 3
* %^BOLD%^query_rooms%^RESET%^
.EI
.SI 5
string * query_rooms(string house)
.EI
.SP 7 5

Get a list of the houses rooms.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - The basename of the house
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a list of rooms.

.EP

.SI 3
* %^BOLD%^query_type%^RESET%^
.EI
.SI 5
int query_type(string house)
.EI
.SP 7 5

Find out the type of a house. ie. Whether it's rented or owned.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - The basename of the house
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the type.

.EP

.SI 3
* %^BOLD%^query_vacant%^RESET%^
.EI
.SI 5
string * query_vacant(string region)
.EI
.SP 7 5

Function to get a list of houses that are vacant in a given region.
This function is used by the auction system or other sales mechanism.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
region - Region to list houses in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a list of houses.

.EP

.SI 3
* %^BOLD%^query_value%^RESET%^
.EI
.SI 5
int query_value(string house)
.EI
.SP 7 5

Find out the current value of a house.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - The basename of the house
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the value.

.EP

.SI 3
* %^BOLD%^remove_house%^RESET%^
.EI
.SI 5
int remove_house(string house)
.EI
.SP 7 5

Remove a house from the list of houses

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - the basename of the house
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success 0 for failure

.EP

.SI 3
* %^BOLD%^remove_housing_area_citizenship%^RESET%^
.EI
.SI 5
void remove_housing_area_citizenship(string area)
.EI
.SP 7 5

This method removes the mapping between a housing area and it's citizenship
requirements.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the housing area to remove the link from

.EP

.SI 3
* %^BOLD%^remove_rooms%^RESET%^
.EI
.SI 5
int remove_rooms(string house, string * rooms)
.EI
.SP 7 5

Remove secondary rooms from a house.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - the basename of the house
.EP
.SP 9 5
an - array of basenames for the rooms
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success 0 for failure

.EP

.SI 3
* %^BOLD%^rename_house%^RESET%^
.EI
.SI 5
int rename_house(string house, string newhouse)
.EI
.SP 7 5

Rename a house. This will remove the "main" room and replace it with
another.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - the current basename of the house
.EP
.SP 9 5
newhouse - the new basename.

.EP

.SI 3
* %^BOLD%^set_address%^RESET%^
.EI
.SI 5
int set_address(string house, string address)
.EI
.SP 7 5

Set the address of this house.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - The basename of the house
.EP
.SP 9 5
address - The new address of the house
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success 0 for failure

.EP

.SI 3
* %^BOLD%^set_bank%^RESET%^
.EI
.SI 5
int set_bank(string house, string bank)
.EI
.SP 7 5

Set the bank account to be used for rent on this house.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - The basename of the house
.EP
.SP 9 5
owner - The new bank to use
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success 0 for failure

.EP

.SI 3
* %^BOLD%^set_for_sale%^RESET%^
.EI
.SI 5
int set_for_sale(string house)
.EI
.SP 7 5

Mark a house as being For Sale this puts it in the list of possible
houses to go into the vacant list.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - the basename of the house
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success 0 for failure

.EP

.SI 3
* %^BOLD%^set_housing_area_citizenship%^RESET%^
.EI
.SI 5
void set_housing_area_citizenship(string area, string citizenship)
.EI
.SP 7 5

This method sets a mapping between a housing area and the citizenship
required to hold a house there.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the housing area to link
.EP
.SP 9 5
citizenship - the citizenship the housing area needs

.EP

.SI 3
* %^BOLD%^set_housing_area_money%^RESET%^
.EI
.SI 5
void set_housing_area_money(string area, string bank, string place)
.EI

.SI 3
* %^BOLD%^set_in_arrears%^RESET%^
.EI
.SI 5
int set_in_arrears(string house)
.EI
.SP 7 5

Mark a house as being in arrears.  This is the step just prior to being
repossessed.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - the basename of the house
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success 0 for failure

.EP

.SI 3
* %^BOLD%^set_owner%^RESET%^
.EI
.SI 5
int set_owner(string house, string owner)
.EI
.SP 7 5

Set the owner of this house.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - The basename of the house
.EP
.SP 9 5
owner - The new owner of the house
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success 0 for failure

.EP

.SI 3
* %^BOLD%^set_region%^RESET%^
.EI
.SI 5
int set_region(string house, string region)
.EI
.SP 7 5

Set the region of this house.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - The basename of the house
.EP
.SP 9 5
region - The new region of the house
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success 0 for failure

.EP

.SI 3
* %^BOLD%^set_type%^RESET%^
.EI
.SI 5
int set_type(string house, int type)
.EI
.SP 7 5

Set the type of a house. ie. Whether it's rented or owned.
(1 for rented, 0 for owned).

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - The basename of the house
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the type.

.EP

.SI 3
* %^BOLD%^set_under_offer%^RESET%^
.EI
.SI 5
int set_under_offer(string house)
.EI
.SP 7 5

Mark a house as being Under Offer (to prevent anyone else trying to sell
it too.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - the basename of the house
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success 0 for failure

.EP

.SI 3
* %^BOLD%^set_value%^RESET%^
.EI
.SI 5
int set_value(string house, int value)
.EI
.SP 7 5

Set the value of a particular house.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
house - the basename of the house
.EP
.SP 9 5
value - the value in monetary units.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success 0 for failure

.EP


