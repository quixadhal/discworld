.DT
armoury.c
Disk World autodoc help
armoury.c

.SH Description
.SP 5 5

Keeps track of all the standard armours, weapons and clothing.

.EP
.SP 10 5


Written by Furball
.EP



.SH See also
.SP 5 5
/include/armoury.h
.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/move_failures.h and /include/armoury.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^choose_small_item%^RESET%^
.EI
.SI 5
object choose_small_item(string area)
.EI
.SP 7 5

This will choose a small item from the specific area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the are to check in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the small object to choose

.EP

.SI 3
* %^BOLD%^query_area%^RESET%^
.EI
.SI 5
mapping query_area(string domain)
.EI
.SP 7 5

Returns the list of domain armoury items.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
domain - the domain/area to get the items from.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the area sub-mapping.

.EP

.SI 3
* %^BOLD%^query_smalls%^RESET%^
.EI
.SI 5
object * query_smalls(string area)
.EI
.SP 7 5

This returns the list of small objects available for the area.  These
are things that can be fgound in gutters and so on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area of the recycling
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of small objects

.EP

.SI 3
* %^BOLD%^rehash_specific_dir%^RESET%^
.EI
.SI 5
int rehash_specific_dir(string dir)
.EI
.SP 7 5

This method rehashes a specific directory of things.  At the moment we
fudge this a little.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to rehash
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it found something to rehash

.EP

.SI 3
* %^BOLD%^remap_file_path%^RESET%^
.EI
.SI 5
string remap_file_path(string path)
.EI
.SP 7 5

This method tries to find a match for the object if it has moved between
directories.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
path - the old path
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new path

.EP

.SI 3
* %^BOLD%^request_item%^RESET%^
.EI
.SI 5
object request_item(string word, int percent, string area)
.EI
.SP 7 5

General item retreival function.  It returns a new item
it handles any sort of item.  The percentage is the
percentage condition in which it is returned.
The "word" parameter is the name of the requested item, essentially
the filename without directory and with "_"'s changed to " "'s. The
"percent" parameter is used as measure of how good it
is compared to a fully functional one. So a 50% dagger is only 50% as
good as a 100% one.


The file armoury.h defines the ARMOURY variable. You need to include this into your file to use it. You should include this file and uyse ythis define rather than the full path to the armoury, as otherwise your code could stuff up :)
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the name of the item to retreive
.EP
.SP 9 5
percent - the percentage condition to be in
.EP
.SP 9 5
area - the area to choose the item in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the requested object, or 0 on failure
.EP


