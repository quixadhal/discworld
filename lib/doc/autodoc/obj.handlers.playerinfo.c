.DT
playerinfo.c
Disk World autodoc help
playerinfo.c

.SH Description
.SP 5 5

Playerinfo database handler.

This handler collects all the fascinating information about players and
their sins. At the moment, the following events are supported:
replace (all replacements, added by Presto's wand),
gag (gagging and ungagging, added by the gagger),
suspend (player suspension, added by the "suspend" command),
meteor (meteoring a player, added by the "meteor" command),
multiplay (various events added by the multiplayer handler),
harassment (comments about cases of harassment, added via 'addevent'),
misc (misc. comments, added via 'addevent'),
cheat (currently unused)
 *
The "replace" and "multiplay" events are debounced (merged). In addition,
the "replace" events expire in 30 days.

.EP
.SP 10 5


Written by Fiona

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/applications.h, /include/refresh.h, /include/player_handler.h, /include/playerinfo.h and /include/newbiehelpers.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^source%^RESET%^
class source {
string * add;
string * delete;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^acknowledge_alert%^RESET%^
.EI
.SI 5
int acknowledge_alert(object creator, string player, string event, int idx, string update, mixed * extra)
.EI

.SI 3
* %^BOLD%^add_alt%^RESET%^
.EI
.SI 5
string add_alt(object creator, string player, string alt)
.EI
.SP 7 5

Add an alt character name to this player's dossier. This function succeeds
if both characters are not "main", or only one if them is "main". Both
players will have their dossiers modified.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the name of the player
.EP
.SP 9 5
alts - the names of the alt characters to add
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a string describing the outcome of the function call

.EP

.SI 3
* %^BOLD%^add_entry%^RESET%^
.EI
.SI 5
int add_entry(object creator, string player, string event, string comment, mixed * extra)
.EI
.SP 7 5

Add a new entry to the player's database.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
source - the creator or another object trying to add the event
.EP
.SP 9 5
player - the name of the player
.EP
.SP 9 5
event - event the name of the event to be added
.EP
.SP 9 5
comment - arbitrary comment text (more than one line is OK)
.EP
.SP 9 5
extra - arbitrary array of arbitrary objects (can be 0)
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
nonzero if the entry was successfully added to the database

.EP

.SI 3
* %^BOLD%^clear_alerts_for%^RESET%^
.EI
.SI 5
void clear_alerts_for(string player)
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - The name of the player.

.EP

.SI 3
* %^BOLD%^decrement_alerts_for%^RESET%^
.EI
.SI 5
int decrement_alerts_for(string player)
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - The name of the player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The updated number of alerts for that player.

.EP

.SI 3
* %^BOLD%^delete_alt%^RESET%^
.EI
.SI 5
string delete_alt(object creator, string player, string alt)
.EI
.SP 7 5

Delete an alt character name from this player's dossier. Note that both
players have their dossier modified.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the name of the player
.EP
.SP 9 5
alts - the name of the alt characters to delete
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a string describing the outcome of the function call

.EP

.SI 3
* %^BOLD%^delete_entry%^RESET%^
.EI
.SI 5
int delete_entry(object creator, string player, string event, int n)
.EI
.SP 7 5

Delete an entry from the playerinfo database.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
source - the creator or another object trying to add the event
.EP
.SP 9 5
player - the name of the player
.EP
.SP 9 5
event - the name of the event of the entry being deleted
.EP
.SP 9 5
n - the index of the entry being deleted
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
nonzero if the entry was successfully deleted

.EP

.SI 3
* %^BOLD%^increment_alerts_for%^RESET%^
.EI
.SI 5
int increment_alerts_for(string player)
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - The name of the player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The updated number of alerts for that player.

.EP

.SI 3
* %^BOLD%^is_alert%^RESET%^
.EI
.SI 5
int is_alert(string player, int idx)
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - The name of the player
.EP
.SP 9 5
idx - The number of the event to check for alert status.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if the event is not an alert, 1 if it is.

.EP

.SI 3
* %^BOLD%^load_handler_data%^RESET%^
.EI
.SI 5
void load_handler_data()
.EI

.SI 3
* %^BOLD%^player_remove%^RESET%^
.EI
.SI 5
void player_remove(string player)
.EI
.SP 7 5

Remove the player's data file.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the name of the player

.EP

.SI 3
* %^BOLD%^query_access_history%^RESET%^
.EI
.SI 5
string query_access_history(string player)
.EI

.SI 3
* %^BOLD%^query_alerts%^RESET%^
.EI
.SI 5
mapping query_alerts()
.EI
.SP 7 5

 @return The alerts mapping.

.EP

.SI 3
* %^BOLD%^query_alerts_for%^RESET%^
.EI
.SI 5
int query_alerts_for(string player)
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - Name of the player to query
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The number of alerts for that player

.EP

.SI 3
* %^BOLD%^query_alt_of%^RESET%^
.EI
.SI 5
string query_alt_of(string player)
.EI
.SP 7 5

Return which player this player is an alt of (if any).
param player A players name
return the name of the players main alt.

.EP

.SI 3
* %^BOLD%^query_alts%^RESET%^
.EI
.SI 5
string * query_alts(string player)
.EI

.SI 3
* %^BOLD%^query_dossier%^RESET%^
.EI
.SI 5
varargs string query_dossier(object source, string player, int lastn)
.EI
.SP 7 5

Print all entries from the given player's dossier.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
source - the creator who requested the report
.EP
.SP 9 5
player - the name of the player

.EP

.SI 3
* %^BOLD%^query_event%^RESET%^
.EI
.SI 5
string query_event(object source, string player, string event)
.EI
.SP 7 5

Print all entries from the given player's dossier with the given event
type.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
source - the creator who requested the report
.EP
.SP 9 5
player - the name of the player
.EP
.SP 9 5
event - the name of the event

.EP

.SI 3
* %^BOLD%^query_events%^RESET%^
.EI
.SI 5
string * query_events()
.EI
.SP 7 5

Answer the list of all possible events.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
array of all event types recognized by the playerinfo handler

.EP

.SI 3
* %^BOLD%^query_interleaved%^RESET%^
.EI
.SI 5
string query_interleaved(object source, string player, string event)
.EI
.SP 7 5

Print all entries for this player and all alts, in chronological order.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
source - the creator who requested the report
.EP
.SP 9 5
player - the name of the player

.EP

.SI 3
* %^BOLD%^query_name%^RESET%^
.EI
.SI 5
string query_name()
.EI

.SI 3
* %^BOLD%^query_online_alerts%^RESET%^
.EI
.SI 5
string * query_online_alerts()
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
A list of the currently-online players who have unacknowledged alerts

.EP

.SI 3
* %^BOLD%^query_source_ok%^RESET%^
.EI
.SI 5
int query_source_ok(string event, string source)
.EI
.SP 7 5

Check if the event has come from the valid source.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
event - the name of the event
.EP
.SP 9 5
source - the object trying to add the event
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
nonzero if the event can be added by the given source

.EP

.SI 3
* %^BOLD%^query_timeouts%^RESET%^
.EI
.SI 5
mapping query_timeouts()
.EI

.SI 3
* %^BOLD%^reregister_parent%^RESET%^
.EI
.SI 5
void reregister_parent(string player)
.EI

.SI 3
* %^BOLD%^save_handler_data%^RESET%^
.EI
.SI 5
void save_handler_data()
.EI

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^query_deleter_ok%^RESET%^
.EI
.SI 5
int query_deleter_ok(string event, object deleter)
.EI
.SP 7 5

Check if the request to delete an event came from the valid source.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
event - the name of the event
.EP
.SP 9 5
remover - the object that tries to remove the event

.EP


