.DT
service_provider.c
DW_wibble autodoc help
service_provider.c

.SH Description
.SP 5 5

The service provider object.  It is designed to be similar to
the peddler object, only instead of selling objects, it sells
services instead.

.EP
.SP 10 5


Written by Furcifer

March 2000

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/monster.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/armoury.h, /include/shops/bank.h, /include/money.h and /include/move_failures.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_service%^RESET%^
.EI
.SI 5
varargs int add_service(string service, string browse_info, string func, int cost, string * aliases)
.EI
.SP 7 5

This method adds a service to the list of services on offer.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
service - the name of the service
.EP
.SP 9 5
cost - what it costs
.EP
.SP 9 5
the - function which controls
.EP
.SP 9 5
*aliases - an array of aliases for this service.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^check_busy%^RESET%^
.EI
.SI 5
varargs int check_busy(object player)
.EI

.SI 3
* %^BOLD%^do_browse%^RESET%^
.EI
.SI 5
int do_browse(mixed indirect_obs, string dir_match, string indir_match, string * words)
.EI
.SP 7 5

The main entrance to the browse for things command.
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

The main entrance to the list stuff command.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_request%^RESET%^
.EI
.SI 5
int do_request(mixed indirect_obs, string dir_match, string indir_match, string * words)
.EI
.SP 7 5

The main entrance to the buy things command.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^remove_service%^RESET%^
.EI
.SI 5
int remove_service(string service)
.EI
.SP 7 5

This method removes a service.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
string - service to be removed
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on successful removal, 0 on failure

.EP

.SI 3
* %^BOLD%^reset_busy%^RESET%^
.EI
.SI 5
void reset_busy()
.EI

.SI 3
* %^BOLD%^set_busy%^RESET%^
.EI
.SI 5
void set_busy()
.EI


