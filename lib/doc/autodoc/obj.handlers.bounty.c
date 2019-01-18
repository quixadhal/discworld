.DT
bounty.c
Disk World autodoc help
bounty.c

.SH Includes
.SP 5 5
This class includes the following files /include/shops/bank.h and /include/money.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^bounty%^RESET%^
class bounty {
string short;
string * victims;
int value;
int active;
int time;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^clear_bounties%^RESET%^
.EI
.SI 5
void clear_bounties()
.EI

.SI 3
* %^BOLD%^collect_bounty%^RESET%^
.EI
.SI 5
string collect_bounty()
.EI

.SI 3
* %^BOLD%^delete_bounty%^RESET%^
.EI
.SI 5
void delete_bounty(string word)
.EI

.SI 3
* %^BOLD%^inform_bounty%^RESET%^
.EI
.SI 5
void inform_bounty(object thing)
.EI

.SI 3
* %^BOLD%^issue_warrant%^RESET%^
.EI
.SI 5
void issue_warrant(string killer)
.EI

.SI 3
* %^BOLD%^query_all_bounties%^RESET%^
.EI
.SI 5
string query_all_bounties()
.EI

.SI 3
* %^BOLD%^query_all_collectors%^RESET%^
.EI
.SI 5
string query_all_collectors()
.EI

.SI 3
* %^BOLD%^query_bounty%^RESET%^
.EI
.SI 5
int query_bounty(string word)
.EI

.SI 3
* %^BOLD%^query_bounty_info%^RESET%^
.EI
.SI 5
string query_bounty_info(string word)
.EI

.SI 3
* %^BOLD%^query_bounty_value%^RESET%^
.EI
.SI 5
int query_bounty_value(string word)
.EI

.SI 3
* %^BOLD%^register_kill%^RESET%^
.EI
.SI 5
void register_kill(object victim, object * attackers)
.EI

.SI 3
* %^BOLD%^report_murder%^RESET%^
.EI
.SI 5
string report_murder(string killer)
.EI

.SI 3
* %^BOLD%^save_file%^RESET%^
.EI
.SI 5
void save_file()
.EI

.SI 3
* %^BOLD%^tidy_bounties%^RESET%^
.EI
.SI 5
void tidy_bounties()
.EI

.SI 3
* %^BOLD%^well_done%^RESET%^
.EI
.SI 5
void well_done(object thing, int gender)
.EI


