.DT
gossip_handler.c
Disk World autodoc help
gossip_handler.c

.SH Description
.SP 5 5

Deals out random gossip to people who request it.


Add rumours!
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/gossip.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_gossip%^RESET%^
.EI
.SI 5
void add_gossip(string name, string mess)
.EI
.SP 7 5

Add a juicy bit of gossip.  This is called by ethe gossip
effects on the npcs.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the person who said the gossip
.EP
.SP 9 5
mess - what they said

.EP

.SI 3
* %^BOLD%^add_rumour%^RESET%^
.EI
.SI 5
void add_rumour(string name, string mess)
.EI
.SP 7 5

Rumours are another method of dealing out gossip.
Rumours are added by creators...
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the person saying the rumour
.EP
.SP 9 5
mess - the rumour message

.EP

.SI 3
* %^BOLD%^query_filter%^RESET%^
.EI
.SI 5
string * query_filter()
.EI
.SP 7 5

The current words we are filtering.  If any of these words are in the
gossip, we filter it out and do not save it.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current filter array

.EP

.SI 3
* %^BOLD%^query_gossip%^RESET%^
.EI
.SI 5
mixed * query_gossip()
.EI
.SP 7 5

Tell us all the gossip.  This returns the complete
array of gossip which the npcs are drawing from.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of two element arrays
.EP

.SI 3
* %^BOLD%^query_random_gossip%^RESET%^
.EI
.SI 5
string * query_random_gossip()
.EI
.SP 7 5

Returns a random piece of gossip.  Finds a random bit of juicy
gossip and returns it.  The first element of the returned
array is the person who said the gossip and the second bit is
what they said.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a two element array containing the gossip

.EP

.SI 3
* %^BOLD%^query_rumours%^RESET%^
.EI
.SI 5
mixed * query_rumours()
.EI
.SP 7 5

All of the current rumours.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of teo element arrays
.EP

.SI 3
* %^BOLD%^remove_gossip%^RESET%^
.EI
.SI 5
varargs string remove_gossip(string match, int force)
.EI
.SP 7 5

Sometimes you just gotta remove some gossip

.EP


