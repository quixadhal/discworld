.DT
player_shop.c
Disk World autodoc help
player_shop.c

.SH Description
.SP 5 5

This is the handler for player-run shops.



%^BOLD%^Description%^RESET%^</p> </p> 

This handler ensures that the very short name (and therefore save file directories) are unique for each shop. Anybody may add a new shop but only a Lord may remove or update an existing shop's details. </p> </p>
.EP
.SP 10 5


Written by Ringo

Started 28th January 2001

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/mail.h and /include/player_shop.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_shop%^RESET%^
.EI
.SI 5
int add_shop(string short_name, string office_path)
.EI
.SP 7 5

Add a shop to the handler.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
short_name - The shop name passed to set_very_short()
.EP
.SP 9 5
office_path - The path to the shop's office.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 if this shop already exists or no such path.
.EP

.SI 3
* %^BOLD%^auto_mail%^RESET%^
.EI
.SI 5
void auto_mail(string to, string from, string sub, string cc, string body)
.EI
.SP 7 5

Send mail

.EP

.SI 3
* %^BOLD%^query_shop%^RESET%^
.EI
.SI 5
string query_shop(string short_name)
.EI
.SP 7 5

Query a shop's path.
Used by the office to check this is a valid shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
short_name - The shop name passed to set_very_short()
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The path to the office, or a blank string if no such shop.
.EP

.SI 3
* %^BOLD%^query_shops%^RESET%^
.EI
.SI 5
mapping query_shops()
.EI
.SP 7 5

Query the list of shops.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The shops mapping.

.EP

.SI 3
* %^BOLD%^remove_shop%^RESET%^
.EI
.SI 5
int remove_shop(string short_name)
.EI
.SP 7 5

Remove a shop from the handler.
Lords only.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
short_name - The shop name passed to set_very_short()
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 if this shop doesn't exist.
.EP

.SI 3
* %^BOLD%^update_shop%^RESET%^
.EI
.SI 5
int update_shop(string short_name, string office_path)
.EI
.SP 7 5

Update a shop's path.
Lords only.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
short_name - The shop name passed to set_very_short()
.EP
.SP 9 5
office_path - The path to the shop's office.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 if this shop doesn't exists or no such path.
.EP


