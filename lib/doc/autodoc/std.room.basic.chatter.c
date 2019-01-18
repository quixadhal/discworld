.DT
chatter.c
Disk World autodoc help
chatter.c

.SH Description
.SP 5 5

The standard chatter inheritable for room chats.  
.EP
.SP 10 5


Written by Pinkfish
.EP



.SH See also
.SP 5 5
/std/room.c and room_chat

.EP
.SH Includes
.SP 5 5
This class includes the following files /include/room.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
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
* %^BOLD%^check_chat%^RESET%^
.EI
.SI 5
void check_chat()
.EI
.SP 7 5

Checks that the chatter is on and being activated
properly from the room_handler. 
.EP

.SI 3
* %^BOLD%^query_room_chats%^RESET%^
.EI
.SI 5
mixed * query_room_chats()
.EI
.SP 7 5

Returns the the set of room chats
managed by this chatter object.  
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
pointer to the mixed array of chat args
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
* %^BOLD%^setup_chatter%^RESET%^
.EI
.SI 5
void setup_chatter(object my_room, mixed * chat_args)
.EI
.SP 7 5

Gives the chatter a room to chat at and the chat args.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
my_room - room object
.EP
.SP 9 5
chat_args - chat data: ({ min, max, chats array })
.EP


