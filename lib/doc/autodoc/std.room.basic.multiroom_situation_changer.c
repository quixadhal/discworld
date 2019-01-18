.DT
multiroom_situation_changer.c
Disk World autodoc help
multiroom_situation_changer.c

.SH Description
.SP 5 5

situation changer object associated with multiple room objects.
It keeps a list of rooms and checks them all in turn. It 
behaves similarly to the room_handler.  It's main purpose is
to avoid duplicating a large chat database for many rooms
that all use it. 

.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/situations.h and /include/am_time.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_situation%^RESET%^
.EI
.SI 5
void add_situation(int num, function * func, string startmess, string endmess, string extralookstring, string * chats, mixed * add_items)
.EI
.SP 7 5

Adds a situation for the rooms.  These situations can be
invoked manually with start_situation or automatically via
automate_situations.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - number labelling the situation
.EP
.SP 9 5
func - function to be called at start of situation 
            that might be used to load NPC's.  If it is
            a set of two function pointers ({ f1, f2 })
            the second function is called when the
            situation is ended.  The start function is 
            passed the num label of the room and a
            do_start_mess flag.  If the flag is one
            the situation is starting rather than
            being reloaded.  The end function is only
            passed the num label.
.EP
.SP 9 5
startmess - message told to the room at start of situation
.EP
.SP 9 5
endmess - message told to the room at end of situation
.EP
.SP 9 5
extralooksting - extra look string appended to rooms long 
description during the situation
.EP
.SP 9 5
chats - an array of chat strings to be active 
during the situation 
.EP
.SP 9 5
add_items - a mixed array of ({ item, item description }) 
pairs to be active during the situation
.EP

.SI 3
* %^BOLD%^automate_situations%^RESET%^
.EI
.SI 5
void automate_situations(mixed * chatargs, int xval, int yval, mixed * edata, object room)
.EI
.SP 7 5

Awakes starting and ending of situations.
These situations can be invoked manually with start_situation.
The awaked starting and ending is unaffected by the room 
unloading.  When the room reloads the situation will be 
restarted unless its duration is up.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
chatargs - same as arguments to room chat:
   ({ min, max, ({ chat1, chat2, ... }) })
   if you want no non-situational chats just put ({ min, max, ({ }) })
   min, max are always needed being the minimum/maximum time
   between all chats.  chat1 is a string containing a chat.
.EP
.SP 9 5
xval - First random seed number (eg. x coordinate)
.EP
.SP 9 5
yval - Second random seed number (eg. y coordinate)
.EP
.SP 9 5
sittiming - A mixed array containing the information 
   about when the situations added by add_situation are 
   automatically started and ended:

   ({ info1, info2, info3, ... })

 Each info is a mixed array as follows:

   ({ number, duration, when, chance })
              -- or --
   ({ ({ num0, num1, num2, ..., numN }) , when, duration, percent })

number     (int) label of the situation to start 
           up if number is an array then a set of
           situations are started one at a time.
           The total duration is divided evenly 
           between the situations labelled by the 
           numbers in the array which are started 
           and ended in the order given.  The 
           entire set always gets used.

duration   (int) time (minutes) situation should last for

when       (int) a nighttime/daytime mask
           This determines when during the day in
           Ankh-Morpork time the situation may occur.  
           The masks are defined in evolvingtime.h
           It can be a function pointer in which case it is 
           expected to return a mask value.

chance     (int) 1/1000 chance per period of duration minutes 
           of getting into the situation
.EP

.SI 3
* %^BOLD%^check_situations%^RESET%^
.EI
.SI 5
void check_situations(object room)
.EI
.SP 7 5

Tests for enabling situation managing.
If situation managing is already active or turned off
it does nothing.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room
.EP

.SI 3
* %^BOLD%^end_situation%^RESET%^
.EI
.SI 5
void end_situation(int num, object room)
.EI
.SP 7 5

Ends a situation previously added and started on the room
that is managed by this object.
These situations can be invoked manually with start_situation 
or automatically via automate_situations. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - number labelling the situation
.EP
.SP 9 5
room - room to end situation on
.EP

.SI 3
* %^BOLD%^extra_look%^RESET%^
.EI
.SI 5
string extra_look(object room)
.EI

.SI 3
* %^BOLD%^manage_rooms_situations%^RESET%^
.EI
.SI 5
void manage_rooms_situations()
.EI
.SP 7 5

Starts and ends situations according to the information
in the sittiming mapping for each room in the rooms array.  
It is called continuously 
automatically while there are interactives in the room.

.EP

.SI 3
* %^BOLD%^manage_situations%^RESET%^
.EI
.SI 5
void manage_situations(object room)
.EI
.SP 7 5

Starts and ends situations according to the information
in the sittiming mapping.  It is called continuously 
automatically while there are interactives in the room.

.EP

.SI 3
* %^BOLD%^query_current_situations%^RESET%^
.EI
.SI 5
int * query_current_situations(object room)
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
* %^BOLD%^query_rooms%^RESET%^
.EI
.SI 5
object * query_rooms()
.EI

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
* %^BOLD%^shutdown_situations%^RESET%^
.EI
.SI 5
void shutdown_situations(object room)
.EI
.SP 7 5

Shuts down all current and pending situations.  It also turns off the
awaked situation manager so no more are added.  It does not
destruct this object so all the add_situations are still loaded
and make be recommenced with automate_situations.  dest_me is
the appropriate call to permanently remove all situations.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - room to shutdown situations on
.EP

.SI 3
* %^BOLD%^start_situation%^RESET%^
.EI
.SI 5
void start_situation(int num, int do_start_mess, object room)
.EI
.SP 7 5

Starts a situation previously added for the room
that is managed by this object.
These situations can be invoked manually with 
start_situation or automatically via
automate_situations. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - number labelling the situation
.EP
.SP 9 5
do_start_mess - 0 to supress the start_mess string
       This is to fake it that a situation has been 
       going for a while when really you just loaded it.
.EP
.SP 9 5
room - room to add situation to
.EP


