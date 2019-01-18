.DT
situation_changer.c
Disk World autodoc help
situation_changer.c

.SH Description
.SP 5 5

situation changer object associated with a room object.

.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/situations.h and /include/am_time.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^situation_timing%^RESET%^
class situation_timing {
mixed label;
mixed duration;
mixed when;
int chance;
int * endat;
mixed background;
mixed category;
int it;
mapping it_data;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_situation%^RESET%^
.EI
.SI 5
void add_situation(mixed label, class situation sit)
.EI
.SP 7 5

Adds a situation to the room.  These situations can be
invoked manually with start_situation or automatically via
automate_situation.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
label - string or number labelling the situation
.EP
.SP 9 5
sit - a structure (class) containing all the bits
of the situation you want to add.
eg. 
start_func function to be called at start of situation 
            that might be used to load NPC's or anything
            beyond a message.

            The start function is passed the label, 
            a do_start_mess flag and the room object.  
            If the flag is 1 the situation is starting 
            rather than being reloaded.  Thus if 
            do_start_mess is 0 then you should avoid
            any obvious start messages and make it look
            like the situation is already underway.

end_func function to be called an the end of a situation.  
            The end function is only
            passed the label and the room object.

start_mess message told to the room at start of situation

end_mess message told to the room at end of situation

extra_look extra look string appended to rooms long 
            during the situation

chats an array of chat strings to be active 
            during the situation 

add_items a mixed array of ({ item, item description }) 
            pairs to be active during the situation

random_words sets of words of the form ({ ({ "option1","option2" }),
            ({ "adjective1","adjective2" }), ... }).  One of the
            the first set replaces #1 in any text above and one of
            the second set replaces #2 in any text above and so on.
            The random choice is fixed for the duration of any one
            situation.

.EP

.SI 3
* %^BOLD%^automate_situation%^RESET%^
.EI
.SI 5
void automate_situation(mixed label, mixed duration, mixed when, mixed chance, mixed category)
.EI
.SP 7 5

Automate starting and ending of a situations.
These situations can be invoked manually with start_situation.
The automated starting and ending is unaffected by the room 
unloading.  When the room reloads the situation will be 
restarted unless its duration is up.
You must include the file situations.h for the definitions
of the when masks.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
label - (mixed) label of the situation to start 
 up.  If you pass an array such as ({ "frog1", "frog2" }) for the 
label then that set of situations are started one at
a time and the total duration is split evenly between them.
Label is usually an integer or a string or an array of
integers and/or strings.
If the string is a list of labels
separated by , then multiple situations
are started using those labels.
.EP
.SP 9 5
duration - (int) total time (seconds) the overall situation 
should last.  If an array is specified for duration each
situation gets it's own little one.  If -1 is specified as
a duration for one part that situation is background complimentary
situation to the rest that is on when the rest are off.
.EP
.SP 9 5
when - (int) a time of the day mask.  This limits when
the situation is allowed to occur.  The mask is composed of
the allowed hours in AM time ( 24 hours clock, (1<<hour) and 
combined with | (OR) ).   You can just use these
predefined masks and ingore how it works:
 WHEN_WEE_HOURS, WHEN_EARLY_MORNING, WHEN_LATE_MORNING, WHEN_AFTERNOON
 WHEN_EVENING, WHEN_LATENIGHT, WHEN_MIDDAY, WHEN_MORNING, 
 WHEN_EARLY_MORNING, WHEN_LATE_MORNING, WHEN_NIGHT, WHEN_DAY
 WHEN_ANY_TIME    
The masks are defined in /include/situations.h.
.EP
.SP 9 5
chance -  (int) chance in 1000 of starting the situation
 This is tested every duration seconds.
.EP

.SI 3
* %^BOLD%^change_situation%^RESET%^
.EI
.SI 5
varargs mixed change_situation(mixed label, mixed duration, mixed words, mixed handle)
.EI
.SP 7 5

Starts one or more situations that will end after a
specified duration.  You can use an array and make
further situations commence when others end.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
label - (mixed) label of the situation to start 
 up.  If you pass an array such as ({ "frog1", "frog2" }) for the 
label then that set of situations are started one at
a time and the total duration is split evenly between them.
Label is usually an integer or a string or an array of
integers and/or strings.
If the string is a list of labels
separated by , then multiple situations
are started using those labels.
.EP
.SP 9 5
duration - (int) total time (seconds) the overall situation 
should last.  You can put an array of durations -- one for each
situation if the label lists more than one situation and then
the overall time is the sum of the numbers.
-1 means indefinite so having any situations after
something with -1 duration is pointless.
.EP
.SP 9 5
handle - is an internal thing that should only be called with 0
unless you really know what you are doing.
.EP
.SP 9 5
words - is a list of replacements for #n in the text OR
       a random number seed, it is
       passed to choose_words.
       eg. ({ "#1", "frog", "#2", "honey" }) or 22
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
call_out that is propogating the changes
This is useful if you want to be able to kill the whole
set without disturbing other situations.
.EP

.SI 3
* %^BOLD%^check_situations%^RESET%^
.EI
.SI 5
void check_situations()
.EI
.SP 7 5

Tests for enabling situation managing.
If situation managing is already active or turned off
it does nothing.
.EP

.SI 3
* %^BOLD%^choose_words%^RESET%^
.EI
.SI 5
void choose_words(mixed label, mixed choice)
.EI
.SP 7 5

This function selects word replacements for #n in the text.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
label - Situation label to choose for
.EP
.SP 9 5
choice - A random seed (integer) or
              a set of pairs exactly the same as the second
              argument to replace.
.EP

.SI 3
* %^BOLD%^end_situation%^RESET%^
.EI
.SI 5
void end_situation(mixed label)
.EI
.SP 7 5

Ends a situation previously added and started on the room
that is managed by this object.
These situations can be invoked manually with start_situation 
or automatically via automate_situation. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
label - string or number labelling the situation
.EP

.SI 3
* %^BOLD%^extra_look%^RESET%^
.EI
.SI 5
string extra_look()
.EI

.SI 3
* %^BOLD%^insert_words_chats%^RESET%^
.EI
.SI 5
string * insert_words_chats(class situation sit, string * words)
.EI
.SP 7 5

Replaces #1 in text with the one of the first array of words in wordlist
and #2 with one of the second array of words and so on...
Each string in the string array is changed.

.EP

.SI 3
* %^BOLD%^insert_words_items%^RESET%^
.EI
.SI 5
mixed * insert_words_items(class situation sit, string * words)
.EI
.SP 7 5

Replaces #1 in text with the one of the first array of words in wordlist
and #2 with one of the second array of words and so on...
For items only the item text is changed, not the key words.

.EP

.SI 3
* %^BOLD%^make_seed%^RESET%^
.EI
.SI 5
void make_seed(int xval, int yval)
.EI
.SP 7 5

Makes a seed value for the random part of when 
situations turn on and off.  The two ints must be 
constant for a given room -- like the coordinates.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
xval - integer to use to make a seed (eg. x coordinate)
.EP
.SP 9 5
yval - integer to use to make a seed (eg. y coordinate)

.EP

.SI 3
* %^BOLD%^manage_situations%^RESET%^
.EI
.SI 5
void manage_situations()
.EI
.SP 7 5

Starts and ends situations according to the information
in the sittiming array.  It is called continuously 
automatically while there are interactives in the room.

.EP

.SI 3
* %^BOLD%^query_current_situations%^RESET%^
.EI
.SI 5
int * query_current_situations()
.EI
.SP 7 5

returns situations currently turned on.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
int array of situation labels

.EP

.SI 3
* %^BOLD%^query_possible%^RESET%^
.EI
.SI 5
int query_possible(class situation_timing sit, int it, int tod, int cnt)
.EI

.SI 3
* %^BOLD%^query_room%^RESET%^
.EI
.SI 5
object query_room()
.EI

.SI 3
* %^BOLD%^query_sittiming%^RESET%^
.EI
.SI 5
mixed * query_sittiming()
.EI
.SP 7 5

returns sittiming class with info about automated situations 
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
class sittiming

.EP

.SI 3
* %^BOLD%^query_situations%^RESET%^
.EI
.SI 5
mapping query_situations()
.EI
.SP 7 5

returns mapping of situations. 
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
mapping of situations

.EP

.SI 3
* %^BOLD%^query_status%^RESET%^
.EI
.SI 5
int query_status()
.EI
.SP 7 5

returns status of situation manager.  
If it is sleeping it will turn on again if a
player enters the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
status 0 off 1 on 2 sleeping

.EP

.SI 3
* %^BOLD%^set_room%^RESET%^
.EI
.SI 5
object set_room(object room_o)
.EI

.SI 3
* %^BOLD%^shutdown_all_situations%^RESET%^
.EI
.SI 5
void shutdown_all_situations()
.EI
.SP 7 5

Shuts down all current and pending situations.  It also turns off the
automated situation manager so no more are added.  It does not
destruct this object so all the add_situations are still loaded
and may be recommenced with automate_situation or change_situation.  
dest_me is the appropriate call to permanently remove all situations.
.EP

.SI 3
* %^BOLD%^shutdown_situation%^RESET%^
.EI
.SI 5
void shutdown_situation(int handle, mixed label)
.EI
.SP 7 5

Shuts down a change_situation based on the call_out handle
returned by the call to change_situation.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
handle - call_out handle.  If 0 then the last
known handle is used.
.EP
.SP 9 5
label - label or array of labels of situations to clean 
up with end_situation
.EP
.SP 9 5
the_room - the room
.EP

.SI 3
* %^BOLD%^start_situation%^RESET%^
.EI
.SI 5
void start_situation(mixed label, int do_start_mess)
.EI
.SP 7 5

Starts a situation previously added to the room
that is managed by this object.
These situations can be invoked manually with 
start_situation or automatically via
automate_situation. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
label - string or number labelling the situation
.EP
.SP 9 5
do_start_mess - 0 to supress the start_mess string
       This is to fake it that a situation has been 
       going for a while when really you just loaded it.
.EP


