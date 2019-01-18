.DT
repeater.c
Disk World autodoc help
repeater.c

.SH Description
.SP 5 5

This is used to repeat all events onto the object that requested it
(excluding events from the object itself).  The methods it calls on
the repeater object are:

.EP
.SO 6 2 -10

	*	repeat_event_person_say 
	*	repeat_event_emote 
	*	repeat_event_say 
	*	repeat_event_enter 
	*	repeat_event_exit 
.EO
.SP 5 5

.EP
.SP 10 5


Written by Pinkfish

Started Thu Sep 23 19:09:49 PDT 1999

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_repeater_owner%^RESET%^
.EI
.SI 5
object query_repeater_owner()
.EI
.SP 7 5

This method returns the owner of the repeater.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the object to inform of all events

.EP

.SI 3
* %^BOLD%^repeat_event_emote%^RESET%^
.EI
.SI 5
void repeat_event_emote(object caller, object ob, string mess)
.EI
.SP 7 5

This is called on the repeater object to process the actual message
event_emote message.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
caller - the repeater who sent the message
.EP
.SP 9 5
ob - the object that did the semote
.EP
.SP 9 5
mess - the message string

.EP

.SI 3
* %^BOLD%^repeat_event_enter%^RESET%^
.EI
.SI 5
void repeat_event_enter(object caller, object ob, string mess, object prev)
.EI
.SP 7 5

This is called on the repeater object to process the actual message
event_enter message.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
caller - the repeater who sent the message
.EP
.SP 9 5
ob - the object that did the event_say
.EP
.SP 9 5
mess - the message string
.EP
.SP 9 5
prev - the last environment of the object

.EP

.SI 3
* %^BOLD%^repeat_event_exit%^RESET%^
.EI
.SI 5
void repeat_event_exit(object caller, object ob, string mess, object dest)
.EI
.SP 7 5

This is called on the repeater object to process the actual message
event_exit message.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
caller - the repeater who sent the message
.EP
.SP 9 5
ob - the object that did the event_say
.EP
.SP 9 5
mess - the message string
.EP
.SP 9 5
exit - where the object is going to

.EP

.SI 3
* %^BOLD%^repeat_event_person_say%^RESET%^
.EI
.SI 5
void repeat_event_person_say(object caller, object ob, string start, string mess, string lang)
.EI
.SP 7 5

This is called on the repeater object to process the actual message
event_person_say message.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
caller - the repeater who sent the message
.EP
.SP 9 5
ob - the object that did the say
.EP
.SP 9 5
start - the start string
.EP
.SP 9 5
mess - the message string
.EP
.SP 9 5
lang - the language the message is in

.EP

.SI 3
* %^BOLD%^repeat_event_say%^RESET%^
.EI
.SI 5
void repeat_event_say(object caller, object ob, string mess, object * avoid)
.EI
.SP 7 5

This is called on the repeater object to process the actual message
event_say message.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
caller - the repeater who sent the message
.EP
.SP 9 5
ob - the object that did the event_say
.EP
.SP 9 5
mess - the message string
.EP
.SP 9 5
avoid - the array of objects not to print the message

.EP

.SI 3
* %^BOLD%^repeat_event_soul%^RESET%^
.EI
.SI 5
void repeat_event_soul(object caller, object ob, string mess, object * avoid)
.EI
.SP 7 5

This is called on the repeater object to process the actual message
event_soul message.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
caller - the repeater who sent the message
.EP
.SP 9 5
ob - the object that did the event_soul
.EP
.SP 9 5
mess - the message string
.EP
.SP 9 5
avoid - the array of objects not to print the message

.EP

.SI 3
* %^BOLD%^set_repeater_owner%^RESET%^
.EI
.SI 5
void set_repeater_owner(object owner)
.EI
.SP 7 5

This method sets the owner of the repeater.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
owner - the object that owns the repeater

.EP


