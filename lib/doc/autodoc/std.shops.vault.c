.DT
vault.c
Disk World autodoc help
vault.c

.SH Description
.SP 5 5

Standard vault inheritable object. There are two inheritables (this one and
/std/vault_entry.c) which are provided to make it easy to setup your own
vaults.

This one handles the actual vault room itself. When using this room you
must use set_vault_name and set_vault_entrance. You should also define a
setup_container function to customise your vault drawers.

.EP
.SP 10 5


Written by Ceres
.EP



.SH See also
.SP 5 5
/std/vault_entrance.c

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/shops/bank.h, /include/money.h and /include/vault.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^init_container%^RESET%^
.EI
.SI 5
object init_container(object person)
.EI
.SP 7 5

This method initialised a nice container to stuff everything we
save into.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
owner - the owner to make a container for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the nice container

.EP

.SI 3
* %^BOLD%^set_vault_exit%^RESET%^
.EI
.SI 5
void set_vault_exit(string direc, mixed dest, string type)
.EI
.SP 7 5

Used to set the exit from this vault room (usually out to the vault entry
room. The parameters are just like those of add_exit.
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

Used to set the name of this vault. The name is used as part of the save
file for the vault and should uniquely idenitfy this vault.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - The name of the vault.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
void

.EP

.SI 3
* %^BOLD%^setup_container%^RESET%^
.EI
.SI 5
void setup_container(object container)
.EI
.SP 7 5

Define this function in your vault room to perform setup on the vault
container. Use it to customise the name, short, long, size etc. of your
drawers (or whatever your vault uses.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
container - The object to perform your customisation on.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
void

.EP


