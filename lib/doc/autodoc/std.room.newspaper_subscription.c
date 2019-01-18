.DT
newspaper_subscription.c
Disk World autodoc help
newspaper_subscription.c

.SH Description
.SP 5 5

Newspaper subscription room
This room handles subscriptions for one specific newspaper.
.EP
.SP 10 5


Written by Wirble

Started August 2001

.EP



.SH See also
.SP 5 5
/obj/handlers/newspaper and help:subscription_room
.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/mail.h, /include/player_handler.h, /include/move_failures.h, /include/shops/bank.h, /include/room/newspaper.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^event_publish_paper%^RESET%^
.EI
.SI 5
void event_publish_paper(string paper)
.EI
.SP 7 5

This function has to be called whenever a new issue of the paper is
released.

.EP

.SI 3
* %^BOLD%^query_money_place%^RESET%^
.EI
.SI 5
string query_money_place()
.EI
.SP 7 5

This method returns the money place for the newsper.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the money place for the newspaper

.EP

.SI 3
* %^BOLD%^query_price_international_newspaper_box%^RESET%^
.EI
.SI 5
int query_price_international_newspaper_box()
.EI
.SP 7 5

This returns the base cost of the international newspaper box.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the base cost of the international newspaper box.

.EP

.SI 3
* %^BOLD%^query_price_newspaper_box%^RESET%^
.EI
.SI 5
int query_price_newspaper_box()
.EI
.SP 7 5

This returns the base cost of the newspaper box.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the base cost of the newspaper box.

.EP

.SI 3
* %^BOLD%^query_price_per_issue_box%^RESET%^
.EI
.SI 5
int query_price_per_issue_box()
.EI
.SP 7 5

This method queries the cost per issue for boxes.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
cost per issue for a box

.EP

.SI 3
* %^BOLD%^query_price_per_issue_subscriber%^RESET%^
.EI
.SI 5
int query_price_per_issue_subscriber()
.EI
.SP 7 5

This queries the prices for subscriptions and boxes.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
price per issue to parcel subscribers

.EP

.SI 3
* %^BOLD%^set_money_place%^RESET%^
.EI
.SI 5
void set_money_place(string new_region)
.EI
.SP 7 5

This sets the region the newspaper is in. i.e. Ankh-Morpork or Klatch.  This
will be used for determining the correct currency.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
new_region - The region to use.

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^set_paper%^RESET%^
.EI
.SI 5
void set_paper(string new_paper)
.EI
.SP 7 5

This sets the paper this particular room is for.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
new_paper - The paper's official name with correct capitalization.

.EP

.SI 3
* %^BOLD%^set_parcel%^RESET%^
.EI
.SI 5
void set_parcel(string parcel_region)
.EI
.SP 7 5

This sets the parcel region for the room and thereby determines to which post
office the newspapers will be delivered, i.e. Ankh-Morpork.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
parcel_region - The region to use.

.EP

.SI 3
* %^BOLD%^set_savefile%^RESET%^
.EI
.SI 5
void set_savefile(string filename)
.EI
.SP 7 5

This sets the savefile for the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
filename - The full filename to use

.EP


