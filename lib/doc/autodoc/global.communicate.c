.DT
communicate.c
Disk World autodoc help
communicate.c

.SH Description
.SP 5 5

The communication handling code for the player object.  Controls all
the basic channels and basic communication stuff.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player.h, /include/language.h, /include/drinks.h, /include/library.h and /include/playtesters.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^language_info%^RESET%^
class language_info {
string cur_lang;
int mangle_accent;
string default_lang;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_language%^RESET%^
.EI
.SI 5
void add_language(string lang)
.EI
.SP 7 5

This method will add a language to a player/npc.  It is used to make sure
that a player or npc has a specific language.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lang - the language to add

.EP

.SI 3
* %^BOLD%^adjust_max_sp%^RESET%^
.EI
.SI 5
int adjust_max_sp(int number)
.EI
.SP 7 5

This method adjusts the current maximum social points.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the amount to adjust it by
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new maximum social points

.EP

.SI 3
* %^BOLD%^adjust_sp%^RESET%^
.EI
.SI 5
int adjust_sp(int number)
.EI
.SP 7 5

This method changes the current number of social points
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the amount to change the social points by
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current social points

.EP

.SI 3
* %^BOLD%^comm_event%^RESET%^
.EI
.SI 5
void comm_event(mixed thing, string type, string start, string rest, string lang, string accent)
.EI

.SI 3
* %^BOLD%^comm_event_to%^RESET%^
.EI
.SI 5
void comm_event_to(object ob, string event_type, string start, string type, string words, object * others, string lang, object me, string accent)
.EI

.SI 3
* %^BOLD%^communicate_commands%^RESET%^
.EI
.SI 5
void communicate_commands()
.EI

.SI 3
* %^BOLD%^do_converse%^RESET%^
.EI
.SI 5
void do_converse(string str)
.EI

.SI 3
* %^BOLD%^do_whisper%^RESET%^
.EI
.SI 5
void do_whisper(object ob, string event_type, string start, string type, string words, object * others, string lang, object me, string accent)
.EI

.SI 3
* %^BOLD%^query_current_language%^RESET%^
.EI
.SI 5
string query_current_language()
.EI
.SP 7 5

This method returns the language the person is currently speaking.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the spoken language
.EP

.SI 3
* %^BOLD%^query_default_language%^RESET%^
.EI
.SI 5
string query_default_language()
.EI
.SP 7 5

This method returns the default language to use for the person.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the default language

.EP

.SI 3
* %^BOLD%^query_ignored_by%^RESET%^
.EI
.SI 5
object * query_ignored_by(object * people)
.EI
.SP 7 5

This method returns the people in the array who are ignoring this
player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
people - the people to check to see for ignoring
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of people that are ignoring this player
.EP

.SI 3
* %^BOLD%^query_ignoring%^RESET%^
.EI
.SI 5
object * query_ignoring(object * people)
.EI
.SP 7 5

This method returns the people in the array who this player is currently
ignoring.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
people - the people to check to see if they are being ignored
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the people who are being ignored from the array
.EP

.SI 3
* %^BOLD%^query_mangle_accent%^RESET%^
.EI
.SI 5
int query_mangle_accent()
.EI
.SP 7 5

This method returns the mangle accents flag.  If this is set to 1 then the
accents will be mangled in the speech text, if it is set to 0 then
the accent will just be added to the say string instead.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mangle accent flag

.EP

.SI 3
* %^BOLD%^query_max_sp%^RESET%^
.EI
.SI 5
int query_max_sp()
.EI
.SP 7 5

This returns the maximum number of social points available.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the maximum social points

.EP

.SI 3
* %^BOLD%^query_real_max_sp%^RESET%^
.EI
.SI 5
int query_real_max_sp()
.EI

.SI 3
* %^BOLD%^query_sp%^RESET%^
.EI
.SI 5
int query_sp()
.EI
.SP 7 5

This method returns the current social points.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current social points

.EP

.SI 3
* %^BOLD%^query_tell_reply_list%^RESET%^
.EI
.SI 5
mixed query_tell_reply_list()
.EI
.SP 7 5

This method returns the players tell reply list.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the tell reply list.

.EP

.SI 3
* %^BOLD%^set_default_language%^RESET%^
.EI
.SI 5
void set_default_language(string def)
.EI
.SP 7 5

This method sets the default language to use for the person.  The default
language is their native tongue, so things in this language show as
not being a special language.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
def - the default language

.EP

.SI 3
* %^BOLD%^set_language%^RESET%^
.EI
.SI 5
int set_language(string str)
.EI
.SP 7 5

This method sets the language we are currently speaking.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the language we are current speaking
.EP

.SI 3
* %^BOLD%^set_mangle_accent%^RESET%^
.EI
.SI 5
void set_mangle_accent(int flag)
.EI
.SP 7 5

This method sets the mangle accents flag.  If this is set to 1 then the
accents will be mangled in the speech text, if it is set to 0 then
the accent will just be added to the say string instead.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
flag - the new value of the flag

.EP

.SI 3
* %^BOLD%^set_max_sp%^RESET%^
.EI
.SI 5
int set_max_sp(int number)
.EI
.SP 7 5

This method sets the maximum social points.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the maxmum social points
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new maximum social points

.EP

.SI 3
* %^BOLD%^set_sp%^RESET%^
.EI
.SI 5
int set_sp(int number)
.EI
.SP 7 5

This method sets the current social points for the player
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the number of social points

.EP

.SI 3
* %^BOLD%^set_tell_reply_list%^RESET%^
.EI
.SI 5
int set_tell_reply_list(mixed list)
.EI
.SP 7 5

This method sets the players tell reply list.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
list - the reply list.
.EP
.SP 9 5
tim - the timeout

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^do_wombat_repeat%^RESET%^
.EI
.SI 5
int do_wombat_repeat(string str, string com)
.EI


