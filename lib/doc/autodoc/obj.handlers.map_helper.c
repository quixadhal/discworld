.DT
map_helper.c
Disk World autodoc help
map_helper.c

.SH Description
.SP 5 5

This is the map helper object.  It contains all the current information
about the files.  It stores the current directory and keeps track of
if things have changed so it does not save or load excessively.
.EP
.SP 10 5


Written by Wodan

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/map_handler.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^change_changetimes%^RESET%^
.EI
.SI 5
void change_changetimes(string dir, string room, int time)
.EI
.SP 7 5

This method changes the time at which the specified room was last
changed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to change the last modified date for
.EP
.SP 9 5
room - the room to set the modied time for
.EP
.SP 9 5
time - the new time

.EP

.SI 3
* %^BOLD%^change_clusters%^RESET%^
.EI
.SI 5
void change_clusters(string dir, string room, int cluster)
.EI
.SP 7 5

This method will change the cluster for the specified room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to change the cluster for
.EP
.SP 9 5
room - the room to change the cluster for
.EP
.SP 9 5
cluster - the new cluster number

.EP

.SI 3
* %^BOLD%^change_co_ords%^RESET%^
.EI
.SI 5
void change_co_ords(string dir, string room, int * coord)
.EI
.SP 7 5

This method changes the coordinates for the specified room/.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to change the room in
.EP
.SP 9 5
room - the room to change in the directory
.EP
.SP 9 5
coord - the new coordinate

.EP

.SI 3
* %^BOLD%^change_map%^RESET%^
.EI
.SI 5
void change_map(string dir, string room, string * dd)
.EI
.SP 7 5

This method will change the destination direction array for the
given room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to change
.EP
.SP 9 5
room - the room to change
.EP
.SP 9 5
dd - the new destination directory

.EP

.SI 3
* %^BOLD%^change_movezones%^RESET%^
.EI
.SI 5
void change_movezones(string dir, string room, string * zones)
.EI
.SP 7 5

This method changes the move zones for the specified room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to change the move zones in
.EP
.SP 9 5
room - the room to change the move zones in
.EP
.SP 9 5
zones - the new move zones

.EP

.SI 3
* %^BOLD%^change_shorts%^RESET%^
.EI
.SI 5
void change_shorts(string dir, string room, string sh)
.EI
.SP 7 5

This method changes the short for the specified directory and room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to change the short for
.EP
.SP 9 5
room - the room the change the short for
.EP
.SP 9 5
sh - the new short for the room

.EP

.SI 3
* %^BOLD%^change_sizes%^RESET%^
.EI
.SI 5
void change_sizes(string dir, string room, int size)
.EI
.SP 7 5

This method changes the roomsize for the specified room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to change the room in
.EP
.SP 9 5
room - the room to change in the directory
.EP
.SP 9 5
coord - the new size;

.EP

.SI 3
* %^BOLD%^check_dir%^RESET%^
.EI
.SI 5
void check_dir(string dir)
.EI
.SP 7 5

This method attempts to load in the save file information for
the specified directory.  If the directory has no save file
then the mappings will all be initialised to empty.  It will cause
the previous directory to be saved if this is attempting to load a
different one.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to load the save information for
.EP

.SI 3
* %^BOLD%^del%^RESET%^
.EI
.SI 5
void del(string dir, string room)
.EI
.SP 7 5

This method deletes the specified room from the directory.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to remove the room from
.EP
.SP 9 5
room - the room to remove

.EP

.SI 3
* %^BOLD%^query_changetimes%^RESET%^
.EI
.SI 5
mapping query_changetimes(string dir)
.EI
.SP 7 5

This method returns the mapping of rooms to last modified dates.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to return the mapping for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping of last modified dates

.EP

.SI 3
* %^BOLD%^query_clusters%^RESET%^
.EI
.SI 5
mapping query_clusters(string dir)
.EI
.SP 7 5

This method returns the cluster mapping for the specified directory.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to return the cluster mapping for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the cluster mapping for the directory

.EP

.SI 3
* %^BOLD%^query_co_ords%^RESET%^
.EI
.SI 5
mapping query_co_ords(string dir)
.EI
.SP 7 5

This method returns the mapping of coordinates to rooms.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to return the mapping for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping of coordinates to rooms

.EP

.SI 3
* %^BOLD%^query_map%^RESET%^
.EI
.SI 5
mapping query_map(string dir)
.EI
.SP 7 5

This method returns the current destination direction mapping for
the given directory.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to return the mapping for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the destination direction mapping

.EP

.SI 3
* %^BOLD%^query_movezones%^RESET%^
.EI
.SI 5
mapping query_movezones(string dir)
.EI
.SP 7 5

This method returns the mapping of move zones for the specified
room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to return the move zones for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the move zone mapping

.EP

.SI 3
* %^BOLD%^query_shorts%^RESET%^
.EI
.SI 5
mapping query_shorts(string dir)
.EI
.SP 7 5

This method returns the mapping containing all the shorts for the
specified directory.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to return the shorts for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping of short descriptions

.EP

.SI 3
* %^BOLD%^query_sizes%^RESET%^
.EI
.SI 5
mapping query_sizes(string dir)
.EI
.SP 7 5

This method returns the mapping of sizes for rooms.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to return the mapping for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping of sizes for rooms.

.EP

.SI 3
* %^BOLD%^save_dir%^RESET%^
.EI
.SI 5
void save_dir()
.EI
.SP 7 5

This method saves the state of the currently loaded directory.
.EP


