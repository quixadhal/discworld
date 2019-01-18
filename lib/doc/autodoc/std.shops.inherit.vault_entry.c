.DT
vault_entry.c
Disk World autodoc help
vault_entry.c

.SH Description
.SP 5 5

Standard vault inheritable object. There are two inheritables (this one and
/std/vault.c) which are provided to make it easy to setup your own
vaults.
This object should be inherited by your vault entrance area. It is used to
define most of the customisation for your vault.

You must use set_vault_name and set_vault_entrance. You must also define
a vault proprietor NPC and use set_proprietor() to let the system know
which NPC it is. You should also check for this proprietors location and
existance every reset. You may also want to provide some protection to
prevent the NPC being killed too frequently.

.EP
.SP 10 5


Written by Ceres
.EP



.SH See also
.SP 5 5
/std/vault_entrance.c

.EP
.SH Includes
.SP 5 5
This class includes the following files /include/shops/bank.h, /include/vault.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^check_idler%^RESET%^
.EI
.SI 5
void check_idler(object who)
.EI

.SI 3
* %^BOLD%^set_location%^RESET%^
.EI
.SI 5
void set_location(string str)
.EI
.SP 7 5

This function is used to set the location used for calculation of which
currency the vault proprietor will accept.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
location - The name of the location.

.EP

.SI 3
* %^BOLD%^set_open_cost%^RESET%^
.EI
.SI 5
void set_open_cost(int i)
.EI
.SP 7 5

This is used to set the cost to open an account (in monetary units)
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cost - The cost of a new account

.EP

.SI 3
* %^BOLD%^set_proprietor%^RESET%^
.EI
.SI 5
void set_proprietor(object ob)
.EI
.SP 7 5

This is the object that is the proprietor of the vault. This should be an
NPC.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
proprietor - The proprietor NPC object.

.EP

.SI 3
* %^BOLD%^set_vault_exit%^RESET%^
.EI
.SI 5
void set_vault_exit(string direc, mixed dest, string type)
.EI
.SP 7 5

Used to set the exit from this vault entrance room into the vault itself.
The parameters are just like those of add_exit.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
direction - The direction of the exit.
.EP
.SP 9 5
dest - The destination object. Usually the filename of the destination.
.EP
.SP 9 5
type - An exit type as in add_exit.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
void
.EP

.SI 3
* %^BOLD%^set_vault_name%^RESET%^
.EI
.SI 5
void set_vault_name(string str)
.EI
.SP 7 5

Used to set the name of the vault, as used for the save file. This should
be a unique name for this vault.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - The name of the vault.

.EP

.SI 3
* %^BOLD%^set_visit_cost%^RESET%^
.EI
.SI 5
void set_visit_cost(int i)
.EI
.SP 7 5

This is used to set the cost of a visit to the vault (in monetary units)
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cost - The cost of a vist

.EP


