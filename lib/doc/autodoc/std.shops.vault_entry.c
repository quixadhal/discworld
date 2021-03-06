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
.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room and /std/shops/inherit/vault_entry.
.EP

