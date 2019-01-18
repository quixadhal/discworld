.DT
creator.c
Disk World autodoc help
creator.c

.SH Description
.SP 5 5

This is the creator player object.  It gives the creators the
bonus extra commands needed by happy creators.
.EP
.SP 10 5


Written by Pinkfish
.EP



.SH See also
.SP 5 5
/global/player.c

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /global/wiz_file_comm.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h and /include/login_handler.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_creator%^RESET%^
.EI
.SI 5
int query_creator()
.EI
.SP 7 5

This method tells us if the object is a creator or not.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the object is a creator, 0 if not.

.EP

.SI 3
* %^BOLD%^receive_snoop%^RESET%^
.EI
.SI 5
void receive_snoop(string bing)
.EI
.SP 7 5

This method is used by the snoop efun to allow creator to see
what other players are doing.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bing - the message to receive

.EP


