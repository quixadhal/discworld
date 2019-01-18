.DT
broadcaster.c
Disk World autodoc help
broadcaster.c

.SH Description
.SP 5 5

Basic sight and sound broadcaster.

.EP
.SP 10 5

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_object_to_channel%^RESET%^
.EI
.SI 5
void add_object_to_channel(string channel_name, object ob)
.EI
.SP 7 5

This method adds an object to the list of objects to be told about the
specfied channel.  The method called on the object for the channel
will be event_channel_message.  The method will be called with
three arguements, the first is the object generating the event
the second is the channel the event is generated on and the
third is the message being sent to the channel.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
channel_name - the name of the channel
.EP
.SP 9 5
ob - the object to add to the list

.EP

.SI 3
* %^BOLD%^broadcast_event%^RESET%^
.EI
.SI 5
void broadcast_event(object * things, int * centre, string message, int range, int inside, int underwater)
.EI
.SP 7 5

Sends an event out to all the things.  It checks to make sure they
can hear the event and all that stuff.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
things - things that get told about it 
.EP
.SP 9 5
centre - coords where event occurs 
.EP
.SP 9 5
message - what the things get told 
.EP
.SP 9 5
range - range of telling in room units 
.EP
.SP 9 5
inside - tell things if indoors 
.EP
.SP 9 5
underwater - tell things if underwater 

.EP

.SI 3
* %^BOLD%^broadcast_to_channel%^RESET%^
.EI
.SI 5
void broadcast_to_channel(mixed ob, string channel, mixed message)
.EI
.SP 7 5

Inform channel of message.  The message will get sent to all the objects
added to the list to the method event_channel_message.  The method
will be called with three arguements, first is the object that started
the event, the second the channel it is being send to and the last is the
message being sent to the channel.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object creating the channel event
.EP
.SP 9 5
channel - the channel to inform people about
.EP
.SP 9 5
message - the message to tell the channel about

.EP

.SI 3
* %^BOLD%^get_direc_dist%^RESET%^
.EI
.SI 5
mixed * get_direc_dist(int * co_ord1, int * co_ord2)
.EI
.SP 7 5

This method returns the square distance, the name of the direction
the sound comes from and the up down offset.  It is used by the shout
code and by various other things that require this inofmraiont.
The return array is formated as:
.EP
.SP 7 5
({ square_distance, name_of_direction, z_offset })
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
co_ord1 - the first co-ordinate
.EP
.SP 9 5
co_ord2 - the co-ordinate to compare against
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array as described above

.EP

.SI 3
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
void init_dynamic_arg(mapping arg)
.EI

.SI 3
* %^BOLD%^is_channel%^RESET%^
.EI
.SI 5
int is_channel(string channel)
.EI
.SP 7 5

This method checks to see if the channel exists.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
channel - the channel name to check for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the channel eixsts, 0 if not
.EP

.SI 3
* %^BOLD%^npc_hear_shouts%^RESET%^
.EI
.SI 5
void npc_hear_shouts(object newlistener)
.EI
.SP 7 5

Adds an NPC to the list of NPC's who are to receive shout events.
The NPC need not be removed from this list when he dies -- desting
the NPC object has the same effect as calling npc_unhear_shouts().
event_person_shout() is activated on the NPC for all shouts.
NPC's do not normally detect shouts.
event_person_shout() on the NPC should be replaced with similar code 
to that in the broadcaster to determine if the shouter is within 
range and to generate the desired response.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
newlistener - NPC object to be added to the list

.EP

.SI 3
* %^BOLD%^npc_shout_event%^RESET%^
.EI
.SI 5
void npc_shout_event(object shouter, string start, string message, string lang, int * coord, int range)
.EI
.SP 7 5

This method is called by the shout command to filter the shouts onto
the npcs that are listening.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
shouter - the person who shouted
.EP
.SP 9 5
start - the start of the message
.EP
.SP 9 5
message - the message shouted
.EP
.SP 9 5
lang - the language the message is in
.EP
.SP 9 5
coord - the co-ordinate it was shouted from
.EP
.SP 9 5
range - the range of the shout

.EP

.SI 3
* %^BOLD%^npc_unhear_shouts%^RESET%^
.EI
.SI 5
int npc_unhear_shouts(object oldlistener)
.EI
.SP 7 5

Removes an NPC from the list of NPC's who are to receive shout events.
The NPC need not be removed from this list when he dies -- desting
the NPC object has the same effect as calling npc_unhear_shouts().

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
oldlistener - NPC object to be removed from the list
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successfully removed

.EP

.SI 3
* %^BOLD%^query_channel_history%^RESET%^
.EI
.SI 5
mixed * query_channel_history(string channel)
.EI
.SP 7 5

This message returns the current history list for the channel.  The
array is:
.EP
.SP 7 5
({ ({ person, message }), ... })
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
channel - the channel to get the history of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the history of the channel
.EP

.SI 3
* %^BOLD%^query_channel_members%^RESET%^
.EI
.SI 5
object * query_channel_members(string channel)
.EI
.SP 7 5

This method must only be used for debugging purposes.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
channel - the channel which has all these things on it
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of objects in the channel
.EP

.SI 3
* %^BOLD%^query_next_channel_number%^RESET%^
.EI
.SI 5
int query_next_channel_number(string channel)
.EI
.SP 7 5

This method keeps track of an ever increasing number for a specified
channel type.  THis is used in the wizards spell (for instance) to
keep track of unique ids for wizard channels.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
channel - the name of the channel
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the next number in the series

.EP

.SI 3
* %^BOLD%^remove_object_from_channel%^RESET%^
.EI
.SI 5
int remove_object_from_channel(string channel_name, object ob)
.EI
.SP 7 5

This method removes an object from the list of objects to be told about the
specified channel.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
channel_name - the name of the channel
.EP
.SP 9 5
ob - the object to add to the channel

.EP


