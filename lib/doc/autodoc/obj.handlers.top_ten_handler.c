.DT
top_ten_handler.c
Disk World autodoc help
top_ten_handler.c

.SH Description
.SP 5 5

Top ten player lists - overall and for each guild.


archive and top_ten is of the format: 

({ ({ name (guild), rating, level, age }), ... }) 

guild_top_tens is of the format: 

([ guild: ({ ({ name, rating, level, age }), ... }), ... ]) 
.EP
.SP 10 5


Written by Deutha

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/library.h, /include/top_ten_tables.h, /include/player_handler.h, /include/player.h, /include/am_time.h, /include/skills.h, /include/clubs.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_guild_top_ten%^RESET%^
.EI
.SI 5
void add_guild_top_ten(string word)
.EI
.SP 7 5

Add in a new table.  This should be used when a new guild is created
and their table needs to be defined.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the name of the table to create

.EP

.SI 3
* %^BOLD%^calculate_age_modifier%^RESET%^
.EI
.SI 5
int calculate_age_modifier(object thing, int algorithm)
.EI

.SI 3
* %^BOLD%^calculate_rating%^RESET%^
.EI
.SI 5
int calculate_rating(object thing)
.EI
.SP 7 5

Figure out the rating for the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the object to get the ratingof
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current rating

.EP

.SI 3
* %^BOLD%^check_family_and_club_info%^RESET%^
.EI
.SI 5
void check_family_and_club_info()
.EI
.SP 7 5

Stuff to make sure that all the clubs still exist.

.EP

.SI 3
* %^BOLD%^check_tables%^RESET%^
.EI
.SI 5
void check_tables()
.EI
.SP 7 5

Wander over the top ten tables checking to see if everyone
still exists.

.EP

.SI 3
* %^BOLD%^excise_name%^RESET%^
.EI
.SI 5
varargs void excise_name(string word1, string word2)
.EI
.SP 7 5

Remove the named player from the named table.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word1 - the name of the player
.EP
.SP 9 5
word2 - the name of the table, 0 for the main table

.EP

.SI 3
* %^BOLD%^inform_of_club%^RESET%^
.EI
.SI 5
void inform_of_club(string club, int family, int type, mixed num)
.EI

.SI 3
* %^BOLD%^player_skill_advance%^RESET%^
.EI
.SI 5
void player_skill_advance(string word, object thing)
.EI
.SP 7 5

Called when a player advances their skills.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the skill being advanced
.EP
.SP 9 5
thing - the player which advanced their skils

.EP

.SI 3
* %^BOLD%^query_archive%^RESET%^
.EI
.SI 5
mixed * query_archive()
.EI
.SP 7 5

The complete archived top ten table.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the archived top ten table

.EP

.SI 3
* %^BOLD%^query_average%^RESET%^
.EI
.SI 5
int query_average()
.EI
.SP 7 5

Some sort of weight average used in the rating calulation.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the weighted average

.EP

.SI 3
* %^BOLD%^query_club_info%^RESET%^
.EI
.SI 5
mixed * query_club_info()
.EI

.SI 3
* %^BOLD%^query_family_info%^RESET%^
.EI
.SI 5
mixed * query_family_info()
.EI

.SI 3
* %^BOLD%^query_guild_top_tens%^RESET%^
.EI
.SI 5
mapping query_guild_top_tens()
.EI
.SP 7 5

The mapping which is the guild top ten tables.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the guild top ten mapping

.EP

.SI 3
* %^BOLD%^query_ordered_table%^RESET%^
.EI
.SI 5
mixed * query_ordered_table(string table_name)
.EI
.SP 7 5

Returns the ordered list of people on the top ten list.  If the table
name is 0 or it is "main" the main table is checked.  If the 
table name is "archive" then the archive is used.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
table_name - the name of the tanble to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of arrays of the top ten information
.EP

.SI 3
* %^BOLD%^query_skills%^RESET%^
.EI
.SI 5
mixed * query_skills()
.EI
.SP 7 5

The skils list with weights.  This is used to help generate the
rating.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the skils list with ratings

.EP

.SI 3
* %^BOLD%^query_top_ten%^RESET%^
.EI
.SI 5
mixed * query_top_ten()
.EI
.SP 7 5

Return the complete list of top ten tables.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of arrays being the top ten tab;les

.EP


