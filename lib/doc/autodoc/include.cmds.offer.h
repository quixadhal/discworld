.DT
offer.h
Disk World autodoc help
offer.h

.SH Description
.SP 5 5

This file has all the defines for offer/accept npc handlers.
.EP
.SP 10 5


Written by Pinkfish

Started Tue Aug 18 06:47:13 EDT 1998

.EP

.SH Classes

.SI 3
* %^BOLD%^offer_sale%^RESET%^
class offer_sale {
int value;
string place;
object * sell_obs;
object seller;
object buyer;
}

.EI
.SP 7 5

The class for offering a sale to another player/npc.

.EP


