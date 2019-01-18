.DT
player_handler.c
Disk World autodoc help
player_handler.c

.SH Description
.SP 5 5

A hander to allow the testing of values on players when they aren't logged
in.  These methods used to exist in the login object but now they're here.



.EP
.SP 10 5


Written by Ceres

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/living/nationality and /global/family.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/access.h, /include/player_handler.h, /include/alias.h, /include/player.h, /include/login_handler.h and /include/playtesters.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^player_record%^RESET%^
class player_record {
int cached;
int touched;
int deleting;
int appealing;
int gender;
int creator;
int active;
int level;
string cap_name;
mapping map_prop;
mapping player_info;
string deity;
string guild;
mixed * guild_data;
int age;
int last;
int flags;
string last_on_from;
int start_time;
string family_name;
string player_title;
string password;
mixed * project;
mixed * plan;
mixed * reference;
mixed * signature;
string home_dir;
string nationality;
string nationality_region;
mixed nationality_data;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^clean_cache%^RESET%^
.EI
.SI 5
void clean_cache()
.EI

.SI 3
* %^BOLD%^load_player%^RESET%^
.EI
.SI 5
int load_player(string name)
.EI
.SP 7 5

This method will load in the player file.  It will return 0 if the player
file either does not exist or the input string is incorrect.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name to try and read in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the player file exists, 0 if not

.EP

.SI 3
* %^BOLD%^make_string%^RESET%^
.EI
.SI 5
string make_string(mixed * al, int max)
.EI
.SP 7 5

This method converts an alias into a string.  This is used by the
projects and plans and so on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
al - the alias to turn into a string
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the converted string

.EP

.SI 3
* %^BOLD%^my_find_player%^RESET%^
.EI
.SI 5
object my_find_player(string player)
.EI

.SI 3
* %^BOLD%^query_appeal_player_file_name%^RESET%^
.EI
.SI 5
string query_appeal_player_file_name(string name)
.EI
.SP 7 5

This method returns the path to the pending appeal player file.
This is the appeal player files actual normal location, it was written to
allow moving the player files around easier.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player whose file to find
.EP

.SI 3
* %^BOLD%^query_cap_name%^RESET%^
.EI
.SI 5
string query_cap_name(string str)
.EI
.SP 7 5

This method returns a players cap_name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the players cap name

.EP

.SI 3
* %^BOLD%^query_delete_player_file_name%^RESET%^
.EI
.SI 5
string query_delete_player_file_name(string name)
.EI
.SP 7 5

This method returns the path to the deleted player file.
This is the deleted player files actual normal location, it was written to
allow moving the player files around easier.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player whose file to find
.EP

.SI 3
* %^BOLD%^query_guild_data%^RESET%^
.EI
.SI 5
mixed query_guild_data()
.EI
.SP 7 5

This method is called by query_level() in the base guild
inheritable to determine the specialisation of the current
player.

.EP

.SI 3
* %^BOLD%^query_plan%^RESET%^
.EI
.SI 5
string query_plan(string name, int unused)
.EI
.SP 7 5

This method returns the players .plan even when they are off line.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the plan, "" if none

.EP

.SI 3
* %^BOLD%^query_player_disk_file_name%^RESET%^
.EI
.SI 5
string query_player_disk_file_name(string name)
.EI

.SI 3
* %^BOLD%^query_player_file_name%^RESET%^
.EI
.SI 5
string query_player_file_name(string name)
.EI
.SP 7 5

This method returns the path to the player file.
This is the player files actual normal location, it was written to
allow moving the player files around easier.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player whose file to find
.EP

.SI 3
* %^BOLD%^query_player_ram_file_name%^RESET%^
.EI
.SI 5
string query_player_ram_file_name(string name)
.EI

.SI 3
* %^BOLD%^query_prevent_shadow%^RESET%^
.EI
.SI 5
nomask int query_prevent_shadow(object ob)
.EI

.SI 3
* %^BOLD%^query_project%^RESET%^
.EI
.SI 5
string query_project(string name, int unused)
.EI
.SP 7 5

This method returns the players .project even when they are off line.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the project, "" if none

.EP

.SI 3
* %^BOLD%^query_reference%^RESET%^
.EI
.SI 5
string query_reference(string name)
.EI
.SP 7 5

This method returns the players .reference even when they are off line.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the reference, "" if none

.EP

.SI 3
* %^BOLD%^query_signature%^RESET%^
.EI
.SI 5
string query_signature(string name)
.EI
.SP 7 5

This method returns the signature to use on posts for the player
even when they are off line.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the signature, "" if none

.EP

.SI 3
* %^BOLD%^query_skill%^RESET%^
.EI
.SI 5
int query_skill(string skill)
.EI

.SI 3
* %^BOLD%^remove_cache_entry%^RESET%^
.EI
.SI 5
void remove_cache_entry(string name)
.EI

.SI 3
* %^BOLD%^special_add_property%^RESET%^
.EI
.SI 5
void special_add_property(string pname, string prop, mixed val)
.EI
.SP 7 5

This method updates the cached properties, it's called by
/secure/login.

.EP

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI

.SI 3
* %^BOLD%^test_active%^RESET%^
.EI
.SI 5
varargs int test_active(string player, int noload)
.EI
.SP 7 5

This method determines if a player is still active.
If you need to perform this on a lot of players please use the noload
parameter. When noload is set to 1 test_active will not attempt to load
the player file if it isn't currently loaded and will instead just do a
simple calculation of the players last login time. This is less accurate
but avoids lagging the mud.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the name of the user
.EP
.SP 9 5
noload - optional parameter to prevent test_active() loading the
              player 
file.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
active or inactive (1 or 0)

.EP

.SI 3
* %^BOLD%^test_age%^RESET%^
.EI
.SI 5
int test_age(string word)
.EI
.SP 7 5

This method determines the age of the player even if they are
not currently on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the players age

.EP

.SI 3
* %^BOLD%^test_appealing%^RESET%^
.EI
.SI 5
int test_appealing(string str)
.EI
.SP 7 5

This method figures out if the user is marked for deletion pending appeal.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are marked for deletion, 0 if they do not

.EP

.SI 3
* %^BOLD%^test_banished%^RESET%^
.EI
.SI 5
int test_banished(string name)
.EI
.SP 7 5

This method checks to see if the name is banished of not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the check for banishment.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it banished, 0 if not

.EP

.SI 3
* %^BOLD%^test_birthday%^RESET%^
.EI
.SI 5
string test_birthday(string str)
.EI
.SP 7 5

This method determines the birthday of the player even if they are
not currently on
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the players birthday (if set)

.EP

.SI 3
* %^BOLD%^test_creator%^RESET%^
.EI
.SI 5
int test_creator(string str)
.EI
.SP 7 5

This method determines if the player is a creator.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the player to test
.EP

.SI 3
* %^BOLD%^test_deity%^RESET%^
.EI
.SI 5
string test_deity(string word)
.EI
.SP 7 5

This method determines the deity of the player even if they are
not currently on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the players deity

.EP

.SI 3
* %^BOLD%^test_deleting%^RESET%^
.EI
.SI 5
int test_deleting(string str)
.EI
.SP 7 5

This method figures out if the user is marked for deletion.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are marked for deletion, 0 if they do not

.EP

.SI 3
* %^BOLD%^test_desc%^RESET%^
.EI
.SI 5
string test_desc(string str)
.EI
.SP 7 5

This method determines the description of the player even if they are
not currently on
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the players description

.EP

.SI 3
* %^BOLD%^test_email%^RESET%^
.EI
.SI 5
string test_email(string str)
.EI
.SP 7 5

This method determines the email of the player even if they are
not currently on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the players email

.EP

.SI 3
* %^BOLD%^test_family%^RESET%^
.EI
.SI 5
string test_family(string str)
.EI
.SP 7 5

This method returns the players family name
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the family name

.EP

.SI 3
* %^BOLD%^test_gender%^RESET%^
.EI
.SI 5
int test_gender(string str)
.EI
.SP 7 5

This method determines the gender of the player even if they are
not currently on
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the players gender
.EP

.SI 3
* %^BOLD%^test_guild%^RESET%^
.EI
.SI 5
string test_guild(string word)
.EI
.SP 7 5

This method determines the guild of the player even if they are
not currently on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the players guild

.EP

.SI 3
* %^BOLD%^test_guild_data%^RESET%^
.EI
.SI 5
mixed * test_guild_data(string word)
.EI
.SP 7 5

This method determines the guild data of the player even if they are
not currently on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the players guild data

.EP

.SI 3
* %^BOLD%^test_home_dir%^RESET%^
.EI
.SI 5
string test_home_dir(string str)
.EI
.SP 7 5

This method returns the players home directory
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the player to test

.EP

.SI 3
* %^BOLD%^test_homepage%^RESET%^
.EI
.SI 5
string test_homepage(string str)
.EI
.SP 7 5

This method determines the players homepage information even if they
are not currently on
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the players location

.EP

.SI 3
* %^BOLD%^test_ip_allowed%^RESET%^
.EI
.SI 5
int test_ip_allowed(string name, string ip)
.EI
.SP 7 5

Check if a players personal allow list permits logins from this IP
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - The player name.
.EP
.SP 9 5
ip - The IP address.

.EP

.SI 3
* %^BOLD%^test_last%^RESET%^
.EI
.SI 5
int test_last(string word, int noload)
.EI
.SP 7 5

This method determines the last log on of the player even if they are
not currently on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the players last log on

.EP

.SI 3
* %^BOLD%^test_last_on_from%^RESET%^
.EI
.SI 5
string test_last_on_from(string word)
.EI
.SP 7 5

This method determines the last log on of the player even if they are
not currently on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the players last log on

.EP

.SI 3
* %^BOLD%^test_level%^RESET%^
.EI
.SI 5
int test_level(string str)
.EI
.SP 7 5

This method determines the level of the player even if they are
not currently on
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the players level
.EP

.SI 3
* %^BOLD%^test_location%^RESET%^
.EI
.SI 5
string test_location(string str)
.EI
.SP 7 5

This method determines the players location finger information even if they
are not currently on
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the players location

.EP

.SI 3
* %^BOLD%^test_nationality%^RESET%^
.EI
.SI 5
string test_nationality(string str)
.EI
.SP 7 5

This method returns the players nationality.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the nationality

.EP

.SI 3
* %^BOLD%^test_nationality_data%^RESET%^
.EI
.SI 5
string test_nationality_data(string str)
.EI
.SP 7 5

This method returns the players nationality data.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the nationality data

.EP

.SI 3
* %^BOLD%^test_nationality_region%^RESET%^
.EI
.SI 5
string test_nationality_region(string str)
.EI
.SP 7 5

This method returns the players nationality region.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the nationality region

.EP

.SI 3
* %^BOLD%^test_player_killer%^RESET%^
.EI
.SI 5
mixed test_player_killer(string word, string str)
.EI
.SP 7 5

This method returns the current value of the player flag on the
player, even if they are not currently on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the player name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are a player killer, 0 if not

.EP

.SI 3
* %^BOLD%^test_player_title%^RESET%^
.EI
.SI 5
string test_player_title(string str)
.EI
.SP 7 5

This method returns the players title.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the title

.EP

.SI 3
* %^BOLD%^test_property%^RESET%^
.EI
.SI 5
mixed test_property(string word, string str)
.EI
.SP 7 5

This method returns the current value of the property on the
player, even if they are not currently on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the player name
.EP
.SP 9 5
str - the property to query
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the value of the property

.EP

.SI 3
* %^BOLD%^test_real_name%^RESET%^
.EI
.SI 5
string test_real_name(string str)
.EI
.SP 7 5

This method determines the real name of the player even if they are
not currently on
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the players real name
.EP

.SI 3
* %^BOLD%^test_start_time%^RESET%^
.EI
.SI 5
int test_start_time(string word)
.EI
.SP 7 5

This method determines the time the player started at.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the players last log on

.EP

.SI 3
* %^BOLD%^test_user%^RESET%^
.EI
.SI 5
int test_user(string str)
.EI
.SP 7 5

This method figures out if the user exists even if they are not on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the user
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they exist, 0 if they do not

.EP

.SI 3
* %^BOLD%^test_valid%^RESET%^
.EI
.SI 5
varargs int test_valid(string name)
.EI
.SP 7 5

Validate a name by checking if it, or bits of it are banished.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - The name to be validated.
.EP
.SP 9 5
full - Should we do full checks or just the basics.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it's ok, 0 if not.

.EP


