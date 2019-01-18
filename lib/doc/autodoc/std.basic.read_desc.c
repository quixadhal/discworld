.DT
read_desc.c
Disk World autodoc help
read_desc.c

.SH Description
.SP 5 5

This method keeps track of all the messages on the object for
reading etc.  Keeps track of the language, the text and the
size.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/desc.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/language.h and /include/nroff.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_read_mess%^RESET%^
.EI
.SI 5
varargs mixed add_read_mess(mixed str, string type, string lang, int size)
.EI

.SI 3
* %^BOLD%^query_cur_size%^RESET%^
.EI
.SI 5
int query_cur_size()
.EI
.SP 7 5

This method returns the current amount of wreiting size on the
object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current amount of writing on the object

.EP

.SI 3
* %^BOLD%^query_max_size%^RESET%^
.EI
.SI 5
int query_max_size()
.EI
.SP 7 5

This method returns the current maximum size of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current maximum size
.EP

.SI 3
* %^BOLD%^query_read_mess%^RESET%^
.EI
.SI 5
mixed * query_read_mess()
.EI
.SP 7 5

This method returns the current read messae array on the object.
The returned array consists of an array of arrays.  The internal
array has 4 elements, the first is the text of the mnessage
the second is the type of the message,m the third is the languae
and the last element is the size of the wiritn .


The defines in the file /include/language.h should be used for the indexes into this array.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the read message array
.EP

.SI 3
* %^BOLD%^query_read_short%^RESET%^
.EI
.SI 5
string query_read_short(object player, int ignore_labels)
.EI
.SP 7 5

This method returns the short description to use with the reading.
It will go: You read <read short>: 

It will automaticly add a $name$ onto the end of the string for you. This will be replaced with the correct a/the short for the message which is printed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player reading the message

.EP

.SI 3
* %^BOLD%^query_readable_message%^RESET%^
.EI
.SI 5
string query_readable_message(object player, int ignore_labels)
.EI
.SP 7 5

This method returns the read message for the object.  This is what should
be displayed to the player if they attempt to read this object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the read message, 0 if there is none

.EP

.SI 3
* %^BOLD%^remove_read_mess%^RESET%^
.EI
.SI 5
int remove_read_mess(string str, string type, string lang)
.EI
.SP 7 5

This method removes a piece of writing off the object.


If you know the actual message or the language or the type you can remove that message. 

Using the type and or language is a very dodgy way of doing this. Using a combination is much better. 

Any of the parameters set to a non-zero value will be used for the search.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the text string
.EP
.SP 9 5
type - the type of the text
.EP
.SP 9 5
lang - the language it is written in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if the text was not found, 1 if it was

.EP

.SI 3
* %^BOLD%^set_cur_size%^RESET%^
.EI
.SI 5
void set_cur_size(int siz)
.EI
.SP 7 5

This method sets the current amount of writing size on
the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
siz - the new current size

.EP

.SI 3
* %^BOLD%^set_max_size%^RESET%^
.EI
.SI 5
void set_max_size(int siz)
.EI
.SP 7 5

This method sets the maximum amount of writing to be able
to be placed onto the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
siz - the maximum size
.EP


