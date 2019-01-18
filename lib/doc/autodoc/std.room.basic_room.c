.DT
basic_room.c
Disk World autodoc help
basic_room.c

.SH Description
.SP 5 5

The standard room inheritable.  This contains all the stuff needed to
construct a standard room.
.EP
.SP 10 5


Written by Pinkfish
.EP



.SH See also
.SP 5 5
/std/room/outside.c

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/cute_look, /std/basic/help_files, /std/basic/property, /std/basic/effects, /std/basic/light, /std/basic/extra_look, /std/basic/export_inventory and /std/basic/desc.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/position.h, /include/nroff.h, /include/armoury.h, /include/dirs.h, /include/room.h, /include/situations.h, /include/move_failures.h, /include/weather.h and /include/door.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_alias%^RESET%^
.EI
.SI 5
void add_alias(mixed names, string word)
.EI
.SP 7 5

This method adds an exit alias to the room.
Aliases are convenient extra forms that can be attached to certain
exits. In the above functions, the variable names is either a string
or an array of strings and is, respectively, the alias or aliases
for the direction passed in word. Since, sometimes, the same alias
could be used for more than one exit, remove_alias() requires both
alias(es) and direction in order to remove the correct alias(es). 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
names - the exit names to alias
.EP
.SP 9 5
word - the name to alias them too
.EP

.SI 3
* %^BOLD%^add_day_item%^RESET%^
.EI
.SI 5
varargs int add_day_item(mixed shorts, mixed desc, mixed no_plural)
.EI
.SP 7 5

This method sets up an item which will only be displayed during the
day.  All of the standard add_item things are available with this
method.
.EP

.SI 3
* %^BOLD%^add_enchant%^RESET%^
.EI
.SI 5
int add_enchant(int number)
.EI
.SP 7 5

Adds number to the current enchantment level of the room.
The enchanment level controls things like what happens when you
flip coins and some special messages which give wizards some 
idea about magic levels.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the new enchantment level to set
.EP

.SI 3
* %^BOLD%^add_exit%^RESET%^
.EI
.SI 5
int add_exit(string direc, mixed dest, string type)
.EI
.SP 7 5

This method adds an exit to the room.   The direction is the direction in
which the exit should go.  This is something like "north" or "enter
gate".  The destination field is where the player will go when they
enter the exit.  The type is a set type that sets a whole bunch of
defaults for the room.  The destination can be either a strong
or an object.


The types are controlled by /obj/handlers/room_handler.c and the current types and what this all means are: 
.EP
.SP 10 5

.EP
.SP 10 5
road 
.EP
.SP 13 5
Wide road. 
.EP
.SP 10 5
path 
.EP
.SP 13 5
Narrower path 
.EP
.SP 10 5
door 
.EP
.SP 13 5
And exit with a door. Defaults to closed but not locked. 
.EP
.SP 10 5
secret 
.EP
.SP 13 5
A secret door. Defaults to closed but not locked. 
.EP
.SP 10 5
corridor 
.EP
.SP 13 5
A corridor (bing). 
.EP
.SP 10 5
hidden 
.EP
.SP 13 5
A hidden exit without a door 
.EP
.SP 7 5


The room aliases are used to expand things for exits. However they don't expand the entire exit name. They expand it in bits. For instance, if the exit was "enter live eel", you could add_alias("eel", "live eel"); and add_alias("bing", "enter"); to get both of the bits of the exit. So "bing eel", "enter eel", "bing live eel" etc would work.
.EP

.SI 3
* %^BOLD%^add_hidden_object%^RESET%^
.EI
.SI 5
int add_hidden_object(object thing)
.EI
.SP 7 5

This puts a hidden object into a room. A hidden object is an object that
exists in the room as far as all the find_match calls go. So, for look at's
and so on, but does not actually exist in the room so it does not show up
in the inventory when the player does a look. This is the method used for
putting signs and doors into rooms, that actually have shorts and you can
do things to, but do not show up in the inventory. The function init is also
called on these objects when init is called in the room. The only thing you
cannot put in your init function is an add_action. You can however define
up bunches of add_commands...


If this sounds complicated. Think of it as an object that IS in the room, but you cannot see it. 

A word of warning here, the init() function will *not* be called on all the players when the object is added as hidden. This means that the commands on it will not be available until the player re-enters the room. You could get around this by moving everyone out of the room and then back in again.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the hidden object to add
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 on a failure

.EP

.SI 3
* %^BOLD%^add_item%^RESET%^
.EI
.SI 5
varargs int add_item(mixed shorts, mixed desc, int no_plural)
.EI
.SP 7 5

This method adds an item description to a room.  This allows you to
set up objects which do not as such exist, but can be looked at for
instance.  There should be a lot of these in rooms.  The name of
the item can be multiple word, and the plural for it is
automagicaly added, unless the no_plural flag is set.  If the name
is an array all of the elements in the array respond to the
description.


If the desc is set to an array, you can use this for handling things like read messages and so on too. Every second element in the array is the description/text to be printed and the other element is the command upon which the text should be printed. The special command 'long' is used to set the long description. 

The special type 'position' is used to allow people to use that item to do positions on, like stand, sit, lie etc. 

This method also allows you to setup add_command patterns. If the name after the verb is just a string, then the string will be printed when that verb is used. If it is just a function pointer then the function will be evaluated and the return result printed. If it is an array, then the first element must be a function pointer and the second optional element is the pattern to use for that method. Multiple patterns and functions may be specified.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
shorts - the short description of the item
.EP
.SP 9 5
desc - the description of the item
.EP
.SP 9 5
no_plural - do not automaticaly add a plural for the item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successfully added, 0 if not
.EP

.SI 3
* %^BOLD%^add_night_item%^RESET%^
.EI
.SI 5
varargs int add_night_item(mixed shorts, mixed desc, mixed no_plural)
.EI
.SP 7 5

This method sets up an item which will only be displayed during the
night.  All of the standard add_item things are available with this
method.
.EP

.SI 3
* %^BOLD%^add_room_chats%^RESET%^
.EI
.SI 5
void add_room_chats(string * new_chats)
.EI
.SP 7 5

Adds more chats to the existing set of room chats
managed by this chatter object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
new_chats - an array of new chat strings
.EP

.SI 3
* %^BOLD%^add_sign%^RESET%^
.EI
.SI 5
varargs object add_sign(string sign_long, mixed sign_read_mess, string sign_short, mixed sign_name, string sign_language)
.EI
.SP 7 5

This method adds a sign into the room.  Any of these elements can
be set to 0, except the long description.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
sign_long - the long description of the sign
.EP
.SP 9 5
sign_read_mess - the readable message on the sign
.EP
.SP 9 5
sign_short - the short description of the sign
.EP
.SP 9 5
sign_name - the name of the sign
.EP
.SP 9 5
sign_language - the language the sign is written in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the object for the sign

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
of the situation you want to add.  It should be a
variable of class situation.  You should include
situations.h where this class is defined.
Every part is optional.
eg. 
start_func  function to be called at start of situation 
            that might be used to load NPC's or anything
            beyond a message.

            The start function is passed the label, 
            a do_start_mess flag and the room object.  
            If the flag is 1 the situation is starting 
            rather than being reloaded.  Thus if 
            do_start_mess is 0 then you should avoid
            any obvious start messages and make it look
            like the situation is already underway.

end_func    function to be called an the end of a situation.  
            The end function is only
            passed the label and the room object.

start_mess  message told to the room at start of situation

end_mess    message told to the room at end of situation

extra_look  extra look string appended to rooms long 
            during the situation
chat_rate   an array of 2 numbers giving the minimum and
            maximum delay between chats.  If this is set
            then the chats are not merged with the
            existing chats but added independently with
            their own chat rates as given.

chats       an array of chat strings to be active 
            during the situation 

add_items   a mixed array of ({ item, item description }) 
            pairs to be active during the situation

random_words  sets of words to insert into text to replace 
              the special character #n where n is a number.
            The form of the array is ({  #1array, #2array, ... })
            where #1array = ({ "#1word1","#1word2",... }) etc.
            For the duration of the situation one of the strings
            in #1array is used to replace all instances of #1
            in the extra_look, start_mess, end_mess, chats
            and key and the long description part of the add_items.
            In a situation compounded of many situations
            the same random seed is used for choosing all #1's
            for each individual situation for the duration, 
            and a different seed for all #2's etc. 

.EP

.SI 3
* %^BOLD%^add_use_internal_object%^RESET%^
.EI
.SI 5
void add_use_internal_object(object thing)
.EI
.SP 7 5

This method adds an object whose interior bits want to be able to export
commands.  You can use this for tables an so on, so that stuff on a table
can still be used.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the thing whose inventory bits are to be exported

.EP

.SI 3
* %^BOLD%^add_zone%^RESET%^
.EI
.SI 5
void add_zone(string zone)
.EI
.SP 7 5

This method adds a move zone into the current list of movement zones
for the room.  The move zones are used by npcs to see which rooms they
are allowed to move into.
.EP

.SI 3
* %^BOLD%^attack_speed%^RESET%^
.EI
.SI 5
int attack_speed()
.EI
.SP 7 5

This method sets the default attack speed for the room.
This defaults to 15.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the default attack speed

.EP

.SI 3
* %^BOLD%^automate_situation%^RESET%^
.EI
.SI 5
varargs void automate_situation(mixed label, mixed duration, mixed when, mixed chance, mixed category)
.EI
.SP 7 5

Automate starting and ending of a situation.
These situations can be invoked manually with start_situation.
The automated starting and ending is unaffected by the room 
unloading.  When the room reloads the situation will be 
restarted unless its duration is up.
You must include the file situations.h for the definitions
of the when masks.  The random seed needs to be set
consistently for the situations.  This is probably ok
unless your rooms are clones (see make_situation_seed). 
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
-1 is a special duration.  It means that the situaton given that
duration is not part of the set but a special background or
default situation that occurs all the time except when
this automated situation is going.
.EP
.SP 9 5
when - (int) a time of the day mask.  This limits when
the situation is allowed to occur.  The mask is composed of
the allowed hours in AM time ( 24 hours clock, (1<<hour) and 
combined with | (OR) ).   You can just use these
predefined masks and ignore how it works:
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
.SP 9 5
category - (optional) if you specify a cateory for the situation
                then no situations with the same category will overlap.
                category would usually be a string eg. "boats".
.EP

.SI 3
* %^BOLD%^calc_co_ord%^RESET%^
.EI
.SI 5
void calc_co_ord()
.EI
.SP 7 5

This method calculates the co-ordinates of this room.  The co-ordinates
are based on the surrounding rooms co-ordinates, if one of those rooms
are loaded.  The function 'query_do_not_use_coords' is called on the
rooms to see if the co-ordinates are allowed to leak out further or
not.
.EP

.SI 3
* %^BOLD%^calc_exits%^RESET%^
.EI
.SI 5
void calc_exits()
.EI
.SP 7 5

This method calculates all the exit strings to be used for this room.

.EP

.SI 3
* %^BOLD%^calc_long_exit%^RESET%^
.EI
.SI 5
void calc_long_exit()
.EI
.SP 7 5

This method creates the long exit description used in the room long
descriptions.
.EP

.SI 3
* %^BOLD%^calc_short_exit_string%^RESET%^
.EI
.SI 5
string calc_short_exit_string()
.EI
.SP 7 5

This method returns the exit string used when in brief mode.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the brief exit string
.EP

.SI 3
* %^BOLD%^can_use_for_co_ords%^RESET%^
.EI
.SI 5
int can_use_for_co_ords(string other)
.EI
.SP 7 5

This method is designed to allow a little more flexability in which rooms
can be used for co-ordinates.  It should be overridden by higher up
rooms to do different checks.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it can be used, 0 if not

.EP

.SI 3
* %^BOLD%^change_situation%^RESET%^
.EI
.SI 5
varargs mixed change_situation(mixed label, mixed duration, mixed words)
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
label - (mixed) label of the situation to start up.  
If you pass an array such as ({ "frog1", "frog2" }) for the 
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
-1 is a special duration.  It means that the labelled situation goes on
forever (and thus nothing after it in the array will ever go).
.EP
.SP 9 5
words - is a list of replacements for #n in the text OR
a random number seed to use to choose words from random_words.
eg. ({ "#1", "frog", "#2", "honey" }) or 22

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
handle useful for halting the changes later.  It is typically
a small positive integer.
.EP

.SI 3
* %^BOLD%^end_situation%^RESET%^
.EI
.SI 5
void end_situation(mixed label)
.EI
.SP 7 5

Ends a situation previously added and started on the room.  
These situations can be invoked manually with start_situation 
or automatically via automate_situation. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
label - label for the situation
.EP

.SI 3
* %^BOLD%^expand_alias%^RESET%^
.EI
.SI 5
string expand_alias(string word)
.EI

.SI 3
* %^BOLD%^find_inv_match%^RESET%^
.EI
.SI 5
object * find_inv_match(string words, object looker)
.EI
.SP 7 5

This method returns all the matchable objects in the room.  This is used
by find_match to determine the group of objects to select from.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
words - the words to match on
.EP
.SP 9 5
looker - the person doing the pmacthing
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of objects to match on

.EP

.SI 3
* %^BOLD%^flush_co_ord%^RESET%^
.EI
.SI 5
void flush_co_ord()
.EI
.SP 7 5

This method resets the co-ordinates for the room to zero, and resets
a flag so that calc_co_ord() can be called again to redetermine 
the room's co-ordinates.
.EP

.SI 3
* %^BOLD%^is_allowed_position%^RESET%^
.EI
.SI 5
int is_allowed_position(string poss)
.EI
.SP 7 5

This method tells us if the passed i nposition is
allowed in this type of room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
poss - the position to check

.EP

.SI 3
* %^BOLD%^make_situation_seed%^RESET%^
.EI
.SI 5
void make_situation_seed(int xval, int yval)
.EI
.SP 7 5

Makes a seed value for the random part of when 
situations turn on and off.  The two ints should be
constant for a given room -- eg. the coordinates.
If this function is not called the seed is 
generated from the file_name of the object.
For this reason, rooms that are clones will produce
erratic results unless this function is called.
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
* %^BOLD%^modify_exit%^RESET%^
.EI
.SI 5
int modify_exit(mixed direc, mixed * data)
.EI
.SP 7 5

This method modifies the parameters for the exit.  See the docs in
/doc/new/room/modify_exit for more complete information.

.EP

.SI 3
* %^BOLD%^modify_item%^RESET%^
.EI
.SI 5
int modify_item(string word, mixed new_desc)
.EI
.SP 7 5

This method will modify certain bits of the specified item.  This will
change only the bits of the pattern that are specified.  If you wish to
remove elements a better method would be to remove the item and
then readd it.  The format of the new_desc array is the same as in the
add_item code.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the name of the item to change
.EP
.SP 9 5
new_desc - the bits of the item to change
.EP

.SI 3
* %^BOLD%^query_aliases%^RESET%^
.EI
.SI 5
string * query_aliases()
.EI
.SP 7 5

This method returns the current exit aliases for the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the exit aliases of the room
.EP

.SI 3
* %^BOLD%^query_background_enchant%^RESET%^
.EI
.SI 5
int query_background_enchant()
.EI
.SP 7 5

This method returns the background enchantment of the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the theft handler of the room
.EP

.SI 3
* %^BOLD%^query_bright_mess%^RESET%^
.EI
.SI 5
string query_bright_mess()
.EI
.SP 7 5

This method returns the message to use when it is too bright to see in
the room.  It defaults to: "It's too bright to see anything!".
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the message to print when it is too bright
.EP

.SI 3
* %^BOLD%^query_chatter%^RESET%^
.EI
.SI 5
object query_chatter()
.EI
.SP 7 5

This method returns the current chatter object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the chatter object
.EP

.SI 3
* %^BOLD%^query_co_ord%^RESET%^
.EI
.SI 5
int * query_co_ord()
.EI
.SP 7 5

Returns the current co-ordinates of the room.  The co-ordinates are
3d, ({ x, y, z }).  So an array with three elements.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current co-ordinates
.EP

.SI 3
* %^BOLD%^query_co_ord_calculated%^RESET%^
.EI
.SI 5
int query_co_ord_calculated()
.EI
.SP 7 5

This tells us if the co-ordinates were set or if they were calculated.
If they were set with set_co_ord then the value of this will be 0
otherwise it will be 1.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is calculated, 0 if it is not
.EP

.SI 3
* %^BOLD%^query_dark_mess%^RESET%^
.EI
.SI 5
string query_dark_mess()
.EI
.SP 7 5

This is the message to print instead of the room description when the
room is dark.  It defaults to the message "It's dark in here isn't it?".
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the dark message
.EP

.SI 3
* %^BOLD%^query_day%^RESET%^
.EI
.SI 5
int query_day()
.EI
.SP 7 5

This method returns whether or not it is daytime.  The value is
automatically updated whenever anyone enters the room and so is more
efficient than checking with the weather handler.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for day, 0 for night.

.EP

.SI 3
* %^BOLD%^query_day_items%^RESET%^
.EI
.SI 5
mixed * query_day_items()
.EI
.SP 7 5

These are the items only visible during the day.

.EP

.SI 3
* %^BOLD%^query_day_long%^RESET%^
.EI
.SI 5
string query_day_long()
.EI
.SP 7 5

This method returns the long description of the room during the day.  This is
used to print out different strings for rooms during the day and during
the night.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the night long for the room.

.EP

.SI 3
* %^BOLD%^query_default_position%^RESET%^
.EI
.SI 5
mixed query_default_position()
.EI
.SP 7 5

This method returns the current default position asigned to this
room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current default position

.EP

.SI 3
* %^BOLD%^query_dest_dir%^RESET%^
.EI
.SI 5
varargs string * query_dest_dir(object thing)
.EI
.SP 7 5

Returns an array containing just the destinations and directions used to
get there. This is useful for monster or whatever that you want to scan a
room for exits to leave out of. The array is of the format. ({ direction1,
destination1, direction2, destination2, ... })


The thing passed in is used as the basis for the relative directions if it is an object. If it is not an object then this is ignored altogether.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - used to get the relative directions according to thing
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of direction, destination pairs

.EP

.SI 3
* %^BOLD%^query_dest_other%^RESET%^
.EI
.SI 5
varargs mixed * query_dest_other(string direc)
.EI
.SP 7 5

This returns information about the exits in the room.  This is the
information set by modify_exit().  The values from this are probably
not very useful for normal coding.
.EP

.SI 3
* %^BOLD%^query_destination%^RESET%^
.EI
.SI 5
string query_destination(string exit)
.EI
.SP 7 5

This method returns the destination room for an exit.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
exit - the exit name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the path of the destination room, or ROOM_VOID on error
.EP

.SI 3
* %^BOLD%^query_direc%^RESET%^
.EI
.SI 5
varargs string * query_direc(object thing)
.EI
.SP 7 5

This method just returns all the directions available to leave from
the room.


The thing passed in is used as the basis for the relative directions if it is an object. If it is not an object then this is ignored altogether. 

%^BOLD%^Strawberries%^RESET%^

Starting from a above,
.EP
.SP 7 5
Working slowly down under.
.EP
.SP 7 5
Sliding up the sides
.EP
.SP 7 5
Eating a meal, fresh cream and syrup.
.EP
.SP 7 5


Round and round, and round again
.EP
.SP 7 5
Grining micheviously
.EP
.SP 7 5
One tongue at play
.EP
.SP 7 5
Firm and hard, fresh strawberries today. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - used to get the relative directions according to thing
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of directions

.EP

.SI 3
* %^BOLD%^query_door%^RESET%^
.EI
.SI 5
string query_door(mixed dest, string name)
.EI
.SP 7 5

This method determines if the specified exit is a door or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dest - the destination to check for being a door
.EP
.SP 9 5
name - the name of the door
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if it is not a door, the direction if it is
.EP

.SI 3
* %^BOLD%^query_door_control%^RESET%^
.EI
.SI 5
varargs mixed query_door_control(string direc, string name)
.EI
.SP 7 5

This returns the information about the door in the specified direction.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
direc - the direction to query the door in
.EP
.SP 9 5
name - the name of the exit
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the door control information
.EP

.SI 3
* %^BOLD%^query_door_open%^RESET%^
.EI
.SI 5
int query_door_open(string direc)
.EI
.SP 7 5

This method checks to see if the door is open.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
direc - the direction of the door
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
-1 on an error, 0 for closed, 1 for open
.EP

.SI 3
* %^BOLD%^query_dynamic_enchant%^RESET%^
.EI
.SI 5
float query_dynamic_enchant()
.EI
.SP 7 5

This method returns the current dynamic enchantment of the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the theft handler of the room
.EP

.SI 3
* %^BOLD%^query_enchant%^RESET%^
.EI
.SI 5
int query_enchant()
.EI
.SP 7 5

Returns the current enchantment level of the room.  The enchanment
level controls things like what happens when you flip coins and
some special messages which give wizards some idea about magic
levels.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current enchantment
.EP

.SI 3
* %^BOLD%^query_exit%^RESET%^
.EI
.SI 5
int query_exit(string direc)
.EI
.SP 7 5

This method determines if there is an exit in the specified direction.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
direc - the exit to test for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it exists, 0 if it does now

.EP

.SI 3
* %^BOLD%^query_exits%^RESET%^
.EI
.SI 5
string * query_exits()
.EI
.SP 7 5

This returns the current array of exits.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the exits array
.EP

.SI 3
* %^BOLD%^query_hidden_objects%^RESET%^
.EI
.SI 5
object * query_hidden_objects()
.EI
.SP 7 5

This returns the current array of hidden objects.  The hidden objects
are used to allow things to not actually be in the room description
but be able to be manipulated by commands.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of hidden objects

.EP

.SI 3
* %^BOLD%^query_is_room%^RESET%^
.EI
.SI 5
int query_is_room()
.EI
.SP 7 5

Returns 1 to indicate that this object is a room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 to indicate that this is a room

.EP

.SI 3
* %^BOLD%^query_item%^RESET%^
.EI
.SI 5
object query_item()
.EI
.SP 7 5

This method returns the current item object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current item object
.EP

.SI 3
* %^BOLD%^query_keep_room_loaded%^RESET%^
.EI
.SI 5
int query_keep_room_loaded()
.EI
.SP 7 5

This method returns the status of the keep room loaded flag.  If they
flag is non-0 then the room with not be unloaded.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the status of the keep room loaded flag

.EP

.SI 3
* %^BOLD%^query_linker%^RESET%^
.EI
.SI 5
object query_linker()
.EI
.SP 7 5

This method returns the current linker object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the linker object

.EP

.SI 3
* %^BOLD%^query_long_exit%^RESET%^
.EI
.SI 5
string query_long_exit()
.EI
.SP 7 5

This returns the long exit string.  This is calculated when it is
first needed by the calc_long_exit function.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the long exit string
.EP

.SI 3
* %^BOLD%^query_long_exit_mxp%^RESET%^
.EI
.SI 5
string query_long_exit_mxp()
.EI
.SP 7 5

This returns the long exit string with mxp codeds added. 
This is calculated when it is
first needed by the calc_long_exit function.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the long exit string
.EP

.SI 3
* %^BOLD%^query_look%^RESET%^
.EI
.SI 5
string query_look(string direc)
.EI

.SI 3
* %^BOLD%^query_look_func%^RESET%^
.EI
.SI 5
mixed * query_look_func(string direc)
.EI

.SI 3
* %^BOLD%^query_night_items%^RESET%^
.EI
.SI 5
mixed * query_night_items()
.EI
.SP 7 5

These are the items only visible at night.

.EP

.SI 3
* %^BOLD%^query_night_long%^RESET%^
.EI
.SI 5
string query_night_long()
.EI
.SP 7 5

This method returns the long description of the room at night.  This is
used to print out different strings for rooms during the day and during
the night.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the night long for the room.

.EP

.SI 3
* %^BOLD%^query_not_replaceable%^RESET%^
.EI
.SI 5
int query_not_replaceable()
.EI
.SP 7 5

This method checks to see if the program is replaceable.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the program is not replaceable
.EP

.SI 3
* %^BOLD%^query_relative%^RESET%^
.EI
.SI 5
int query_relative(string direc)
.EI
.SP 7 5

This method checks to see if the exit is a relative one.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
direc - the direction to check
.EP

.SI 3
* %^BOLD%^query_room_chats%^RESET%^
.EI
.SI 5
mixed * query_room_chats()
.EI
.SP 7 5

Returns the current set of room chats
managed by the chatter object.  
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
pointer to the mixed array of chat args
.EP

.SI 3
* %^BOLD%^query_room_day_chats%^RESET%^
.EI
.SI 5
mixed * query_room_day_chats()
.EI
.SP 7 5

This returns the set of chats visible only during the day.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the chat array used during the day

.EP

.SI 3
* %^BOLD%^query_room_default_chats%^RESET%^
.EI
.SI 5
mixed * query_room_default_chats()
.EI
.SP 7 5

This returns the default set of chats, these are mixed with the night
and day chats to generate the current set.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mixed set of chats

.EP

.SI 3
* %^BOLD%^query_room_night_chats%^RESET%^
.EI
.SI 5
mixed * query_room_night_chats()
.EI
.SP 7 5

This returns the set of chats visible only at night.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the chat array used at night.

.EP

.SI 3
* %^BOLD%^query_room_size%^RESET%^
.EI
.SI 5
mixed query_room_size()
.EI
.SP 7 5

This method queries the size of the room.  The default size of a room
is 10x10x10.  A room can be any rectangular size, this method will return
an array of three elements if the room is a non-cube.  If it returns
a single number then the room is assumed to be cubic.


({ north-south size, east-west size, up-down size }) 

The sizes are all radii's so they are half the actual width of the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the size of the room
.EP

.SI 3
* %^BOLD%^query_room_size_array%^RESET%^
.EI
.SI 5
int * query_room_size_array()
.EI
.SP 7 5

This method returns the size of the room as a three element array always.


({ north-south size, east-west size, up-down size }) 

The sizes are all radii's so they are half the actual width of the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the size of the room as a three element array
.EP

.SI 3
* %^BOLD%^query_short_exit_string%^RESET%^
.EI
.SI 5
string query_short_exit_string()
.EI
.SP 7 5

This method returns the short exit string.  The short exit string is the
string used in 'brief' mode of a players look.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the short exit string
.EP

.SI 3
* %^BOLD%^query_situation_changer%^RESET%^
.EI
.SI 5
object query_situation_changer()
.EI
.SP 7 5

This method returns the current situation changer object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the situation changer object
.EP

.SI 3
* %^BOLD%^query_size%^RESET%^
.EI
.SI 5
int query_size(string direc)
.EI
.SP 7 5

This method returns the size of the exit.  This is used to check to make
sure that people can enter it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
direc - the direction of the exit to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the size of the exit
.EP

.SI 3
* %^BOLD%^query_terrain%^RESET%^
.EI
.SI 5
object query_terrain()
.EI
.SP 7 5

This method returns the current terrain object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the terrain object
.EP

.SI 3
* %^BOLD%^query_theft_handler%^RESET%^
.EI
.SI 5
string query_theft_handler()
.EI
.SP 7 5

This method returns the current theft handler for the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the theft handler of the room
.EP

.SI 3
* %^BOLD%^query_use_internal_objects%^RESET%^
.EI
.SI 5
object * query_use_internal_objects()
.EI
.SP 7 5

This method returns all the current use internal objects available.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of use internal objects here

.EP

.SI 3
* %^BOLD%^query_visibility%^RESET%^
.EI
.SI 5
int query_visibility()
.EI
.SP 7 5

This method returns the visibility in this room.  The visibility should
be a percentage which represents the percentage of the maximum visibility
in the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the visbility

.EP

.SI 3
* %^BOLD%^query_wall%^RESET%^
.EI
.SI 5
object query_wall()
.EI
.SP 7 5

This method returns the current wall object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the wall object
.EP

.SI 3
* %^BOLD%^query_zones%^RESET%^
.EI
.SI 5
string * query_zones()
.EI
.SP 7 5

This method returns the set of move zones for this room.  This is used
by npcs to see which rooms they are allowed to move into.
.EP

.SI 3
* %^BOLD%^remove_alias%^RESET%^
.EI
.SI 5
void remove_alias(mixed names, string word)
.EI
.SP 7 5

This method removes the exit aliases from the room.
Aliases are convenient extra forms that can be attached to certain
exits. In the above functions, the variable names is either a string
or an array of strings and is, respectively, the alias or aliases
for the direction passed in word. Since, sometimes, the same alias
could be used for more than one exit, remove_alias() requires both
alias(es) and direction in order to remove the correct alias(es). 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
names - the names to remove
.EP
.SP 9 5
word - what they were aliased to
.EP

.SI 3
* %^BOLD%^remove_exit%^RESET%^
.EI
.SI 5
int remove_exit(string direc)
.EI
.SP 7 5

This method removes the specified exit from the room.
.EP

.SI 3
* %^BOLD%^remove_hidden_object%^RESET%^
.EI
.SI 5
int remove_hidden_object(object thing)
.EI
.SP 7 5

This method removes a hidden object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the hidden object to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^remove_item%^RESET%^
.EI
.SI 5
int remove_item(string word)
.EI
.SP 7 5

This method will attempt to remove the item defined by the given string.
This will remove everything associated with that item, verbs, patterns,
everything.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the name of the item to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 on a failure
.EP

.SI 3
* %^BOLD%^remove_room_chats%^RESET%^
.EI
.SI 5
void remove_room_chats(string * dead_chats)
.EI
.SP 7 5

Removes chats from the set of room chats
managed by this chatter object.  If there are no chats
left the chatter is destructed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dead_chats - an array of chat strings to remove
.EP

.SI 3
* %^BOLD%^remove_use_internal_object%^RESET%^
.EI
.SI 5
void remove_use_internal_object(object thing)
.EI
.SP 7 5

This method removes an object whose interor bits want to export.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the object to remove

.EP

.SI 3
* %^BOLD%^remove_zone%^RESET%^
.EI
.SI 5
void remove_zone(string zone)
.EI
.SP 7 5

This method removes a move zone from the current list of movement zones
for the room.  The move zones are used by npcs to see which rooms they
are allowed to move into.
.EP

.SI 3
* %^BOLD%^reset_exits%^RESET%^
.EI
.SI 5
void reset_exits()
.EI
.SP 7 5

This method removes all the current exits in the room.
.EP

.SI 3
* %^BOLD%^room_chat%^RESET%^
.EI
.SI 5
varargs void room_chat(mixed * args, object chatobj)
.EI
.SP 7 5

This method sets up the room chats.
Room chats are strings which are printed at (semi) random intervals
in rooms.  They are used to add atmosphere to a room. A chat will
be picked at random from the array of chats with a frequency
controlled by the times min and max. ie. one will be picked every n
seconds where is varies between min and max seconds. Please don't
make the values for min and max too small or the messages just
become annoying!


The argument to the room_chat method is an array of the format:
.EP
.SP 7 5
({ int min, int max, ({ string *chats }) }). In place of a chat string you may use "#function_name" where function_name is a function that exists on the room object. 

Repeated calls to this function overwrite the chats for the default chatter. 

If this function is used in combination with the day/night chats then this sets the default chat items for the chatter. The frequency of chats will be taken from the day/night chats, but the messages from here will be merged into the day/night chats.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
args - the room chat arguments
.EP
.SP 9 5
chatobj - chatter object in case the default offends you.
               This argument may be omitted in which case you get 
               /std/room/basic/chatter.c
.EP

.SI 3
* %^BOLD%^room_day_chat%^RESET%^
.EI
.SI 5
void room_day_chat(mixed * args)
.EI
.SP 7 5

This method sets up chats for when the room is in the day cycle.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
args - the chatter arguements
.EP

.SI 3
* %^BOLD%^room_night_chat%^RESET%^
.EI
.SI 5
void room_night_chat(mixed * args)
.EI
.SP 7 5

This method sets up chats for when the room is in the night cycle.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
args - the chatter arguements
.EP

.SI 3
* %^BOLD%^set_background_enchant%^RESET%^
.EI
.SI 5
void set_background_enchant(int number)
.EI
.SP 7 5

This method sets the background enchantment of the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the theft handler of the room
.EP

.SI 3
* %^BOLD%^set_bright_mess%^RESET%^
.EI
.SI 5
void set_bright_mess(string word)
.EI
.SP 7 5

This method sets the bright message associated with the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the new bright message
.EP

.SI 3
* %^BOLD%^set_chat_min_max%^RESET%^
.EI
.SI 5
void set_chat_min_max(int min, int max)
.EI
.SP 7 5

Allows the chat interval to be changed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
min - minimum interval between chats (seconds)
.EP
.SP 9 5
max - maximum interval between chats (seconds)

.EP

.SI 3
* %^BOLD%^set_co_ord%^RESET%^
.EI
.SI 5
void set_co_ord(int * new_co_ord)
.EI
.SP 7 5

Sets the current co-ordinates of the room.  The co-ordinates are
3d, ({ x, y, z }).  So an array with three elements.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
new_co_ord - the new co-ordinates for the room.
.EP

.SI 3
* %^BOLD%^set_dark_mess%^RESET%^
.EI
.SI 5
void set_dark_mess(string word)
.EI
.SP 7 5

This method sets the dark message associated with the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the new dark message
.EP

.SI 3
* %^BOLD%^set_day_long%^RESET%^
.EI
.SI 5
void set_day_long(string str)
.EI
.SP 7 5

This method sets the long description to display during the day time.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the new day long description
.EP

.SI 3
* %^BOLD%^set_default_position%^RESET%^
.EI
.SI 5
void set_default_position(mixed stuff)
.EI
.SP 7 5

This method sets the default position for the room.  Se the set
default position in the living code for a more complete
example of this.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pos - the default position
.EP

.SI 3
* %^BOLD%^set_dynamic_enchant%^RESET%^
.EI
.SI 5
void set_dynamic_enchant(float number)
.EI
.SP 7 5

This method sets the current dynamic enchantment of the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the theft handler of the room
.EP

.SI 3
* %^BOLD%^set_enchant%^RESET%^
.EI
.SI 5
int set_enchant(int number)
.EI
.SP 7 5

Sets the current enchantment level of the room.  The enchanment
level controls things like what happens when you flip coins and
some special messages which give wizards some idea about magic
levels.
When called from the room itself, it sets a background level of 
enchantment that don't decay, when called from another object it
sets the current enchantment which then decays towards the background 
level.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the new enchantment level to set
.EP

.SI 3
* %^BOLD%^set_keep_room_loaded%^RESET%^
.EI
.SI 5
void set_keep_room_loaded(int flag)
.EI
.SP 7 5

This method sets the flag that enables or disables the room being
cleaned up.  If they flag is set to 1, then room is never cleaned up.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
flag - the room being cleaned up flag
.EP

.SI 3
* %^BOLD%^set_linker%^RESET%^
.EI
.SI 5
varargs int set_linker(string * rooms, string d_prep, string s_prep, string r_name)
.EI
.SP 7 5

This method sets up a linkage between the current room and othert
rooms.  The linkage broadcasts things like says and enter/exit
messages between the rooms.


The the dynamic preposition is used when someone enters/exits the room, the static preposition is used when someone says something in the room. The dynamic proposition defaults to "into" and the static preposition defaults to "in".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
rooms - the rooms to link together
.EP
.SP 9 5
d_prep - the dynamic preposition
.EP
.SP 9 5
s_prep - the static preposition
.EP
.SP 9 5
r_name - the name of the room/area
.EP

.SI 3
* %^BOLD%^set_night_long%^RESET%^
.EI
.SI 5
void set_night_long(string str)
.EI
.SP 7 5

This method sets up the night long for the room.  This will be the
long description displayed at night in the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the new night long description
.EP

.SI 3
* %^BOLD%^set_not_replaceable%^RESET%^
.EI
.SI 5
void set_not_replaceable(int replace)
.EI
.SP 7 5

This method sets a property to make the program replaceable.  A program
will only be replaced if there is only a setup() function in the room.
A reset() or a create() will stop the room from being replace and in
fact any other function existing in there will stop it from being
replaced as well.
.EP

.SI 3
* %^BOLD%^set_room_size%^RESET%^
.EI
.SI 5
void set_room_size(mixed number)
.EI
.SP 7 5

This method sets the rooms principle radii.  If the parameter isa single
number then the room is assumed to be cubic and dimension applies in
all directions.  If the input is a three element array then the elements
apply to all the directions.
.EP
.SP 7 5
({ north-south size, east-west size, up-down size }) 

The sizes are all radii's so they are half the actual width of the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the new size of the room
.EP

.SI 3
* %^BOLD%^set_situation_changer%^RESET%^
.EI
.SI 5
varargs object set_situation_changer(mixed changer)
.EI
.SP 7 5

Set a situation changer (in place of the default).
If there is no argument you get the default:
/std/room/basic/situation_changer.
You call this before any other situation related functions.
If you create your own changer it should inherit one of
/std/room/basic/situation_changer or
/std/room/basic/multiroom_situation_changer or
otherwsie provide the functionality of those objects.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
changer - optional parameter specifying either a path for
the changer object or an existing object to use.
.EP

.SI 3
* %^BOLD%^set_terrain%^RESET%^
.EI
.SI 5
int set_terrain(string terrain_name)
.EI

.SI 3
* %^BOLD%^set_theft_handler%^RESET%^
.EI
.SI 5
void set_theft_handler(string word)
.EI
.SP 7 5

This method sets the current theft handler for the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the new theft handler for the room
.EP

.SI 3
* %^BOLD%^set_wall%^RESET%^
.EI
.SI 5
void set_wall(mixed * args)
.EI

.SI 3
* %^BOLD%^set_zone%^RESET%^
.EI
.SI 5
void set_zone(string zone)
.EI
.SP 7 5

This method adds a move zone into the current list of zones.
This method is depreciated, add_zone should be used instead.
.EP

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
and make be recommenced with automate_situation.  dest_me is
the appropriate call to permanently remove all situations.  The
call is passed to the situation changer object.  If none exists
then nothing happens.  The situation changer is created when
an add_situation call is performed. 
.EP

.SI 3
* %^BOLD%^shutdown_situation%^RESET%^
.EI
.SI 5
void shutdown_situation(int call, mixed label)
.EI
.SP 7 5

Shuts down a situation or set of situations initiated with
change_situation based on the call_out handle
returned by the call to change_situation.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
callout - call_out handle.  If 0 then the last
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
void start_situation(int label, int do_start_mess)
.EI
.SP 7 5

Starts a situation previously added to the room.  These situations can be
invoked manually with start_situation or automatically via
automate_situation.  The call is passed to the situation
changer object.  If there isn't one nothing happens. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
label - label for the situation as passed to add_situation
.EP
.SP 9 5
do_start_mess - 0 to supress the start_mess string
       This is to fake it that a situation has been 
       going for a while when really you just loaded it.
.EP

.SI 3
* %^BOLD%^stop_room_chats%^RESET%^
.EI
.SI 5
void stop_room_chats()
.EI
.SP 7 5

This method stops all the room chats for the room.  It also removes
all the room chats, so if you want to have any more you must
add them again.
.EP


