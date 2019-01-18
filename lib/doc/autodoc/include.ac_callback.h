.DT
ac_callback.h
Disk World autodoc help
ac_callback.h

.SH Description
.SP 5 5

This file contains the classes and defines used in the 
armour call back system. 

.EP
.SP 10 5

.EP

.SH Classes

.SI 3
* %^BOLD%^ac_callback_data%^RESET%^
class ac_callback_data {
object ob;
int id;
function func;
int priority;
mixed extra;
}

.EI
.SP 7 5

This class stores armour class call back data.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
ob - the object that owns the callback.
.EP
.SP 9 5
id - the callback id, used to remove it.
.EP
.SP 9 5
func - the function called when it is triggered. (either 
evaluated if it is a function pointer, or call_other()'d if it 
is an array in the format ({ object, function }) 
.EP
.SP 9 5
priority - an integer indicating the priority of the call back,
functions with a higher priority are evaluated first.
.EP
.SP 9 5
extra - any extra data.

.EP


