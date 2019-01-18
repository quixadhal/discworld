.DT
delivery.c
Disk World autodoc help
delivery.c

.SH Description
.SP 5 5

Inheritable Delivery System


This object provides a way of storing and sending objects to players while they are not online. It is roughly based on the concepts found in Post Offices that support parcel depositing, and also Bernita's Flower Shop. (Which delivers flowers to people when they log on.) 

This object is easy to use, and can be combined with most other objects on the Disc. (ie, /obj/monster and /std/room). See delivery_room.c for a pre-made room which handles the delivery of objects. 

Note that this system uses static login calls. These must be configured first and turned on manually in order for the delivery system to work. 

In order to use this inheritable the following things must be done: 
.EP
.SO 6 2 -10

	*	The object that uses it created, and this object inherited. 
	*	The save file set using set_save_file(). 
	*	Any extra settings such as delivery message and delay configured. 
	*	setup_delivery() called on the object to finalize setup. 
.EO
.SP 5 5

.EP
.SP 10 5


Written by Taffyd

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /global/auto_load.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/move_failures.h, /include/login_handler.h and /include/login.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^delivery_item%^RESET%^
class delivery_item {
object delivery_ob;
string sent_by;
int submit_time;
int delay_time;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_delivery%^RESET%^
.EI
.SI 5
void add_delivery(mixed who, mixed sender, object item, int delay)
.EI
.SP 7 5

This method adds a new item to the delivery mapping for the specified 
recipient.  This should always be used instead of set_delivery(), as it
makes sure that all the data structures have been configured correctly.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
who - the person to receive the item. 
.EP
.SP 9 5
sender - the person sending the file. 
.EP
.SP 9 5
item - the item to be sent
.EP
.SP 9 5
delay - how many seconds to wait before delivering item on the next
login.

.EP

.SI 3
* %^BOLD%^check_delivery%^RESET%^
.EI
.SI 5
void check_delivery(mixed person, string type)
.EI
.SP 7 5

This method is called by the login handler every time someone logs 
on to the Disc.  It handles the delivery of items and all sorts of 
nifty things.



.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the name of the person logging on
.EP
.SP 9 5
type - the type of login event


.EP

.SI 3
* %^BOLD%^clean_delivery_mapping%^RESET%^
.EI
.SI 5
void clean_delivery_mapping()
.EI

.SI 3
* %^BOLD%^clear_delivery%^RESET%^
.EI
.SI 5
void clear_delivery()
.EI
.SP 7 5

This method is used to remove all deliveries currently stored within the 
delivery system.


Be very careful when using this function! It will save the cleared delivery mapping after it has been called. 
.EP

.SI 3
* %^BOLD%^load_file%^RESET%^
.EI
.SI 5
void load_file()
.EI
.SP 7 5

This method loads retrieves data from the save file, and handles the 
autoloading of the items.

.EP

.SI 3
* %^BOLD%^query_burdened_mess%^RESET%^
.EI
.SI 5
string query_burdened_mess()
.EI
.SP 7 5

This returns the current delivery message that is displayed to the 
recipient of a delivery, if it is too heavy go to in their inventory.



.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the delivery message

.EP

.SI 3
* %^BOLD%^query_delivery%^RESET%^
.EI
.SI 5
varargs mixed query_delivery(string person)
.EI
.SP 7 5

This method returns the delivery details for a particular person, or
the entire delivery mapping of one doesn't exist.



.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person's delivery details to query. 

.EP

.SI 3
* %^BOLD%^query_delivery_delay%^RESET%^
.EI
.SI 5
int query_delivery_delay()
.EI
.SP 7 5

This method returns how long after the login an item is delivered
to the recipient.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of seconds unt    


.EP

.SI 3
* %^BOLD%^query_delivery_mess%^RESET%^
.EI
.SI 5
string query_delivery_mess()
.EI
.SP 7 5

This returns the current delivery message that is displayed to the 
recipient of a delivery.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the delivery message

.EP

.SI 3
* %^BOLD%^query_save_file%^RESET%^
.EI
.SI 5
string query_save_file()
.EI
.SP 7 5

This method is used to query the save file in which the delivery data
is stored.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
 the path of the save file used to store delivery details.

.EP

.SI 3
* %^BOLD%^save_file%^RESET%^
.EI
.SI 5
void save_file()
.EI
.SP 7 5

This method saves all of the delivery data into the save file, handling 
saving of the auto loading as well.

.EP

.SI 3
* %^BOLD%^setup_delivery%^RESET%^
.EI
.SI 5
void setup_delivery()
.EI
.SP 7 5

This method adds the object into the login handler's static login calls 
list.  It only ever needs to be called _once_, and doesn't need to be 
called every time the object loads. 


Once this is done, the object will automatically be loaded whenever someone logs on. 

This must be called in order for the delivery system to work at all. 
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^deliver_item%^RESET%^
.EI
.SI 5
void deliver_item(string who, object ob, string sent_by)
.EI
.SP 7 5

This method is called when an item is being delivered to a player.  The 
delivery message or the burdened mess is used depending on how the item
is delivered, and then it is removed from the delivery mapping.


Note that this routine uses add_message() to display movement messages to the player, instead of tell_object() or a traditional move message. This is so that the messages can be 'clumped' together properly, otherwise it just doesn't work. 


.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
who - the person to deliver to
.EP
.SP 9 5
ob - the object to deliver
.EP
.SP 9 5
sent_by - the person that send the parcel
.EP
.SP 9 5
position - its location in the recepient's delivery mapping.

.EP

.SI 3
* %^BOLD%^set_burdened_mess%^RESET%^
.EI
.SI 5
void set_burdened_mess(string s)
.EI
.SP 7 5

This method sets the message that is displayed when an item is too 
heavy to be placed in the victim's inventory. This uses the same format
as the normal delivery message.



.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - message to display
.EP

.SI 3
* %^BOLD%^set_delivery%^RESET%^
.EI
.SI 5
void set_delivery(mapping x)
.EI
.SP 7 5

This is used to set all of the delivery mapping manually.  You should 
not use this method.  

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the new delivery mapping.
.EP

.SI 3
* %^BOLD%^set_delivery_delay%^RESET%^
.EI
.SI 5
void set_delivery_delay(int new_time)
.EI
.SP 7 5

This method is used to specify how many seconds to wait after login 
until the item is delivered.  Note that if this is too long, then the
receipient may log out before you can give them the item.  If the 
receipient logs out, then the item will be destroyed.


This is set to 5 by default. 


.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
new_time - the time in seconds after log in to deliver the message.

.EP

.SI 3
* %^BOLD%^set_delivery_mess%^RESET%^
.EI
.SI 5
void set_delivery_mess(string s)
.EI
.SP 7 5

This method sets the delivery message that is displayed to the player 
when they log on. $S is substituted in the string with the name of 
the player who sent the message, $N is replaced with the a_short()
of the item being sent. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the new delivery string
.EP

.SI 3
* %^BOLD%^set_save_file%^RESET%^
.EI
.SI 5
void set_save_file(string str)
.EI
.SP 7 5

This method sets the save file to a file name so that auto loading and 
delivery details can be successfully saved.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the new path.

.EP

.SI 3
* %^BOLD%^valid_delivery%^RESET%^
.EI
.SI 5
int valid_delivery(class delivery_item delivery, string person)
.EI
.SP 7 5

This method is called by the check_delivery() function when it validates 
someone's deliveries.  It is used to determine whether or not it's time
for a delivery to be delivered, and whether or not to remove it.



.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
delivery - the delivery item to be tested
.EP
.SP 9 5
person - the person to deliver the item to.



.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the delivery is not ready, 0 if it is. (yes, this is kind
of backwards.)

.EP


