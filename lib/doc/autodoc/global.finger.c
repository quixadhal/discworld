.DT
finger.c
Disk World autodoc help
finger.c

.SH Description
.SP 5 5

This file contains all of the information related to a player,
their password, real name, birthday, location etc.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player.h, /include/mail.h, /include/clothing.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^finger_commands%^RESET%^
.EI
.SI 5
void finger_commands()
.EI
.SP 7 5

The commands related to the fingering of a player.
Just the change password command at the moment.

.EP

.SI 3
* %^BOLD%^finger_set%^RESET%^
.EI
.SI 5
int finger_set()
.EI
.SP 7 5

This method returns whether the player has set their finger info.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they have their finger information set, 0 if not

.EP

.SI 3
* %^BOLD%^query_birthday%^RESET%^
.EI
.SI 5
string query_birthday()
.EI
.SP 7 5

This method returns the players birthday.  The birthday will be
of the form "4th of july".
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current birthday
.EP

.SI 3
* %^BOLD%^query_desc%^RESET%^
.EI
.SI 5
string query_desc()
.EI
.SP 7 5

This method returns the current description of the player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current description

.EP

.SI 3
* %^BOLD%^query_email%^RESET%^
.EI
.SI 5
string query_email()
.EI
.SP 7 5

This method returns the current email address of the player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current email address

.EP

.SI 3
* %^BOLD%^query_homepage%^RESET%^
.EI
.SI 5
string query_homepage()
.EI
.SP 7 5

This method queries the current homepage for the player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
their homepage url

.EP

.SI 3
* %^BOLD%^query_is_birthday_today%^RESET%^
.EI
.SI 5
int query_is_birthday_today()
.EI
.SP 7 5

This method checks to see if it is currently the players birthday.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is their birthday, 0 if not
.EP

.SI 3
* %^BOLD%^query_main_zone_desc%^RESET%^
.EI
.SI 5
string query_main_zone_desc(object * wearing)
.EI
.SP 7 5

This method returns the main zone description to use in the
long of the player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the main zone description

.EP

.SI 3
* %^BOLD%^query_real_name%^RESET%^
.EI
.SI 5
string query_real_name()
.EI
.SP 7 5

This will return the real name of the player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the real name of the player
.EP

.SI 3
* %^BOLD%^query_rhosts%^RESET%^
.EI
.SI 5
string * query_rhosts()
.EI
.SP 7 5

Find out which IP addresses this user wants to be able to login from.

.EP

.SI 3
* %^BOLD%^query_where%^RESET%^
.EI
.SI 5
string query_where()
.EI
.SP 7 5

This methoid returns the location of the player.  The location
is the location in their funger infor, so like "Perth, oz, whatever".
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
their currently set location
.EP

.SI 3
* %^BOLD%^query_zone_desc%^RESET%^
.EI
.SI 5
string query_zone_desc(string zone)
.EI
.SP 7 5

This method returns the zone description for the specified zone.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
zone - the zone to get the description for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the zone description

.EP

.SI 3
* %^BOLD%^query_zone_desc_names%^RESET%^
.EI
.SI 5
string * query_zone_desc_names()
.EI
.SP 7 5

This method returns all the zones there are currently descriptions for.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the zones there are descriptions for

.EP

.SI 3
* %^BOLD%^set_birthday%^RESET%^
.EI
.SI 5
void set_birthday(string i)
.EI
.SP 7 5

This method sets the players current birthday.  The birthday
needs to be of the form:  "4th of July".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the new birthday to set
.EP

.SI 3
* %^BOLD%^set_desc%^RESET%^
.EI
.SI 5
void set_desc(string str)
.EI
.SP 7 5

This method sets the description of the player.  The
description is the little bit of text seen in the long.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the new description of the player
.EP

.SI 3
* %^BOLD%^set_email%^RESET%^
.EI
.SI 5
nomask void set_email(string str)
.EI
.SP 7 5

This method sets the current email address of the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the new email address

.EP

.SI 3
* %^BOLD%^set_homepage%^RESET%^
.EI
.SI 5
nomask void set_homepage(string str)
.EI
.SP 7 5

This method sets the players homepage.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
their - homepage url

.EP

.SI 3
* %^BOLD%^set_password%^RESET%^
.EI
.SI 5
void set_password(string pass)
.EI
.SP 7 5

This method changes the password of the player.  It can only
be called by the login object at startup.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pass - the new password

.EP

.SI 3
* %^BOLD%^set_real_name%^RESET%^
.EI
.SI 5
void set_real_name(string str)
.EI
.SP 7 5

This will set the real name of the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the new real name of the player

.EP

.SI 3
* %^BOLD%^set_rhosts%^RESET%^
.EI
.SI 5
void set_rhosts(string * ips)
.EI

.SI 3
* %^BOLD%^set_where%^RESET%^
.EI
.SI 5
void set_where(string str)
.EI
.SP 7 5

This method sets the currnet location of the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the new location

.EP

.SI 3
* %^BOLD%^set_zone_desc%^RESET%^
.EI
.SI 5
void set_zone_desc(string zone, string desc)
.EI
.SP 7 5

This method sets the description for the specific area of the body.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
zone - the zone to set the description for
.EP
.SP 9 5
desc - the description for that zone

.EP


