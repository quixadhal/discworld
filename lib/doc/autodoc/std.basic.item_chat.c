.DT
item_chat.c
Disk World autodoc help
item_chat.c

.SH Description
.SP 5 5
-*- LPC -*-
This class handles atmospheric chat messages.

.EP
.SP 10 5

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
* %^BOLD%^chat_off%^RESET%^
.EI
.SI 5
void chat_off()
.EI
.SP 7 5

This stops the object chatting.  This is done automaticly when leaving
the prescence of all interactive objects.
.EP

.SI 3
* %^BOLD%^chat_on%^RESET%^
.EI
.SI 5
void chat_on()
.EI
.SP 7 5

This starts the object chatting.  This is done automaticly when entering
the presence of an interactive object
.EP

.SI 3
* %^BOLD%^check_chat%^RESET%^
.EI
.SI 5
void check_chat()
.EI
.SP 7 5

This method makes sure that we are chatting if we should be.
.EP

.SI 3
* %^BOLD%^fix_saved_chat%^RESET%^
.EI
.SI 5
string fix_saved_chat(string chat)
.EI
.SP 7 5
Fixes saved chats

.EP

.SI 3
* %^BOLD%^item_chat%^RESET%^
.EI
.SI 5
void item_chat(mixed * args)
.EI
.SP 7 5

This method adds atmospheric chat messages to the object.
The array which is passed into the object has three 
elements, the first element is the minimum time between chats, the second
parameter is the maximum time bewteen chats and the third parameter
is the list of actual chats.


Item chats are strings which are printed at (semi) random intervals to living objects holding or in the same room as someone holding the object. Currently they will not be heard outside containers. They are used to add atmosphere to an Item. A chat will be picked at random from the array of chats with a frequency controlled by the times min and max. ie. one will be picked every n seconds where is varies between min and max seconds. Please don't make the values for min and max too small or the messages just become annoying! 

Currently, there's a maximum on the min/max values, every value higher than 320 seconds is treated as that. 

The chats can contain one of $a_short$, $the_short$ and $poss_short$ which will be replaced with the corresponding value. 

To call a function defined on the item in place of a chat message use "#function_name" in place of a chat string. 

To use item chats, the object needs to inherit "/std/basic/item_chat" and if the object defines an init() function, that function should also call ::init() 

To make the chatting object save the values, it's necessary to define a couple of supporting functions in the item: 
.EP
.SP 7 5
mapping query_dynamic_auto_load() {
.EP
.SP 7 5
  mapping tmp;
.EP
.SP 7 5

.EP
.SP 7 5
  tmp = ([ "::" : object::query_dynamic_auto_load(),
.EP
.SP 7 5
           "chat" : item_chat::query_dynamic_auto_load(), ]);
.EP
.SP 7 5
  return tmp;
.EP
.SP 7 5
} /* query_dynamic_auto_load() */
.EP
.SP 7 5

.EP
.SP 7 5
void init_dynamic_arg(mapping map) {
.EP
.SP 7 5
  if (map["::"])
.EP
.SP 7 5
    object::init_dynamic_arg(map["::"]);
.EP
.SP 7 5
  if (map["chat"])
.EP
.SP 7 5
    item_chat::init_dynamic_arg(map["chat"]);
.EP
.SP 7 5
} /* init_dynamic_arg() */
.EP
.SP 7 5

.EP
.SP 7 5
Use object in the previous if the object is inheriting /std/object, if not, then use whatever it's inheriting from instead. 
.EP

.SI 3
* %^BOLD%^make_chat%^RESET%^
.EI
.SI 5
void make_chat()
.EI
.SP 7 5

This method does the actual chat, it prints a message
and figures that stuff out.
.EP

.SI 3
* %^BOLD%^query_chats%^RESET%^
.EI
.SI 5
mixed * query_chats()
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The raw data telling us what to chat and when

.EP


