.DT
map.c
Disk World autodoc help
map.c

.SH Description
.SP 5 5

This contains information about all the rooms in the mud.  It is used
by things like the route handler to determine routes between two
locations.


As coded by Wodan and overhauled by Olorin and overhauled again by Wodan. and rewritten by Olorin The thing is somehow improving though :)
.EP
.SP 10 5


Written by Wodan

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/handlers/inherit/distance_handler.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/config.h and /include/map_handler.h.
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

This method changes the current change time for the room in the
dirctory.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to change the room in
.EP
.SP 9 5
room - the room to change
.EP
.SP 9 5
time - the new change time for the room
.EP

.SI 3
* %^BOLD%^change_clusters%^RESET%^
.EI
.SI 5
void change_clusters(string dir, string room, int cluster)
.EI
.SP 7 5

This method change sthe current cluster for the given room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory in which to change the room
.EP
.SP 9 5
room - the room to change the cluster in
.EP
.SP 9 5
cluster - the new value for the cluster of the room
.EP

.SI 3
* %^BOLD%^change_co_ords%^RESET%^
.EI
.SI 5
void change_co_ords(string dir, string room, int * coord)
.EI
.SP 7 5

This method changes the coordinates of the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to change the coordinates in
.EP
.SP 9 5
room - the room to change the coordinates for
.EP
.SP 9 5
coord - the new coordinates for the room

.EP

.SI 3
* %^BOLD%^change_map%^RESET%^
.EI
.SI 5
void change_map(string dir, string room, string * dd)
.EI
.SP 7 5

This method changes the current values of the map values for the
specified room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to change the values in
.EP
.SP 9 5
room - the room to change the directory for
.EP
.SP 9 5
dd - the array containing the direction, destination pairs
.EP

.SI 3
* %^BOLD%^change_movezones%^RESET%^
.EI
.SI 5
void change_movezones(string dir, string room, string * zones)
.EI
.SP 7 5

This method changes the value of the move zones for the specified
room and directory.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to change the values in
.EP
.SP 9 5
room - the room the change the values in
.EP
.SP 9 5
zones - the new set of move zones
.EP

.SI 3
* %^BOLD%^change_shorts%^RESET%^
.EI
.SI 5
void change_shorts(string dir, string room, string sh)
.EI
.SP 7 5

This method changes the short description for the room and directory.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to change the short in
.EP
.SP 9 5
room - the room to change the short in
.EP
.SP 9 5
sh - the short description for the room
.EP

.SI 3
* %^BOLD%^change_sizes%^RESET%^
.EI
.SI 5
void change_sizes(string dir, string room, int * size)
.EI
.SP 7 5

This method changes the currently stored sizes for the room
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the direct to change the size in
.EP
.SP 9 5
room - the room to change the size in
.EP
.SP 9 5
size - the new size for the room
.EP

.SI 3
* %^BOLD%^check_map%^RESET%^
.EI
.SI 5
void check_map(string room)
.EI

.SI 3
* %^BOLD%^check_save_files%^RESET%^
.EI
.SI 5
void check_save_files()
.EI
.SP 7 5

This method checks the save files in the directory to check to make
sure all the rooms still exist in the game.

.EP

.SI 3
* %^BOLD%^clear_cache%^RESET%^
.EI
.SI 5
void clear_cache()
.EI
.SP 7 5

This method completely clears the cache of all information.  This method
is used for debugging purposes.
.EP

.SI 3
* %^BOLD%^compare_arrays%^RESET%^
.EI
.SI 5
int compare_arrays(mixed * ar1, mixed * ar2)
.EI
.SP 7 5

This method compares too arrays to see if they are the same.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ar1 - the first array to compare
.EP
.SP 9 5
ar2 - the second array to compare

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are not the same, 0 if they are
.EP

.SI 3
* %^BOLD%^del%^RESET%^
.EI
.SI 5
void del(string dir, string room)
.EI
.SP 7 5

This method deletes a room from the specified directory.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to delete the room from
.EP
.SP 9 5
room - the room to delete

.EP

.SI 3
* %^BOLD%^do_room%^RESET%^
.EI
.SI 5
void do_room(string room)
.EI
.SP 7 5

This method will decide if room should be checked, using the checks that
can be done without loading the room, queue the check of this room.
The callout is to alleviate a problem of too deep recursion errors
eg. when the room to be checked it loaded because of a command like drag
This functions should NOT be private, since it's the fundamental interface
toorooms.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room to check
.EP

.SI 3
* %^BOLD%^query_cache%^RESET%^
.EI
.SI 5
} string * query_cache()
.EI
.SP 7 5

This method returns the current cache stores in the map handler.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of directories currently loaded
.EP

.SI 3
* %^BOLD%^query_changetimes%^RESET%^
.EI
.SI 5
mapping query_changetimes(string dir)
.EI
.SP 7 5

This method returns all the change times for the specified directory.
The change times are the time at which the file was last changed.
This method returns a mapping, the key of the mapping is the
room and the index of the mapping is the time at which it changed
(an integer).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to return the change times for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the change times mapping
.EP

.SI 3
* %^BOLD%^query_cluster%^RESET%^
.EI
.SI 5
int query_cluster(string room)
.EI
.SP 7 5

This method returns the cluster number for the specified room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room to find the cluster for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the cluster number for the room
.EP

.SI 3
* %^BOLD%^query_clusters%^RESET%^
.EI
.SI 5
mapping query_clusters(string dir)
.EI
.SP 7 5

This method returns the current clusters for the specified dir.
The returned mapping has keys of the room and the values are
the cluster number.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to return the clusters for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the clusters mapping
.EP

.SI 3
* %^BOLD%^query_co_ord%^RESET%^
.EI
.SI 5
int * query_co_ord(string room)
.EI
.SP 7 5

This method returns the coordinates for the specified room.  This will
cause the room to load if possible to extract the coordinates.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room the get the coordinates of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the coordinates of the room, 0 on failure

.EP

.SI 3
* %^BOLD%^query_co_ords%^RESET%^
.EI
.SI 5
mapping query_co_ords(string dir)
.EI
.SP 7 5

This method returns the coordinates for the specified directory.
The mapping containers keys of the room and the values are the
coordinates of the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to return the coordinates for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the coordinates mapping
.EP

.SI 3
* %^BOLD%^query_dest_dir%^RESET%^
.EI
.SI 5
string * query_dest_dir(string room)
.EI
.SP 7 5

This method returns the destination directory array for the specified
room.  This is equivilant to the method of the same name on /std/room.c
Returns an array of ({ direction, destination, ... })
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room to return the destination direction array for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of direction, destination pairs
.EP

.SI 3
* %^BOLD%^query_destination%^RESET%^
.EI
.SI 5
string query_destination(string exit, string room)
.EI
.SP 7 5

This method returns the destination associated with the exit in the
room. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
exit - the exit in the room to check
.EP
.SP 9 5
room - the room in which to check for the exit
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the destination for the exit
.EP

.SI 3
* %^BOLD%^query_known%^RESET%^
.EI
.SI 5
int query_known(string room)
.EI
.SP 7 5

This method returns true if the room is known of to the map handler.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room to test for existance
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it exists, 0 if it does not

.EP

.SI 3
* %^BOLD%^query_map%^RESET%^
.EI
.SI 5
mapping query_map(string dir)
.EI
.SP 7 5

This method attempts to return the map for the current directory.  This
returns a mapping which contains an index being the room and the
indicies are direction, destination pairs.

.EP
.SP 7 5
([
.EP
.SP 7 5
   file : ({
.EP
.SP 7 5
             direction1,
.EP
.SP 7 5
             destination1,
.EP
.SP 7 5
             ...
.EP
.SP 7 5
           }),
.EP
.SP 7 5
 ])
.EP
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to load the information for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the directions mapping
.EP

.SI 3
* %^BOLD%^query_movezones%^RESET%^
.EI
.SI 5
mapping query_movezones(string dir)
.EI
.SP 7 5

This method returns all the move zones for the specified directory.
This method returns a mapping whose keys are the rooms and the
values are the movezones for that room.
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
the movezones mapping
.EP

.SI 3
* %^BOLD%^query_queue%^RESET%^
.EI
.SI 5
string * query_queue()
.EI
.SP 7 5

This method returns the current quere stores in the map handler.  The
queue is a list of rooms waiting to be processed into the map
handler.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current queue

.EP

.SI 3
* %^BOLD%^query_room_size%^RESET%^
.EI
.SI 5
int * query_room_size(string room)
.EI
.SP 7 5

This method returns the size of the room, it will attempt to load
the room if it is not in map handlers current rooms.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room to find the size of

.EP

.SI 3
* %^BOLD%^query_short%^RESET%^
.EI
.SI 5
string query_short(string room)
.EI
.SP 7 5

This method returns the short description of the room.  This will return
the short description of named cloned objects as well as attempting to
force the room to load if it is not found in the internal
cache.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room to return the short description of

.EP

.SI 3
* %^BOLD%^query_shorts%^RESET%^
.EI
.SI 5
mapping query_shorts(string dir)
.EI
.SP 7 5

This method will return all the shorts for the specified directory.
The mapping returned has keys of the room names and values of the
short description of the room.
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
the shorts descriptions mapping
.EP

.SI 3
* %^BOLD%^query_sizes%^RESET%^
.EI
.SI 5
mapping query_sizes(string dir)
.EI
.SP 7 5

This method will return the sizes for all the rooms in the directory.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to return the sizes mapping for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the sizes mapping
.EP

.SI 3
* %^BOLD%^query_zones%^RESET%^
.EI
.SI 5
string * query_zones(string room)
.EI
.SP 7 5

This method returns the move zones for the specified room.  It will
load the room if it is not available in the map handlers cache.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room to get the zones for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of move zones for the room
.EP

.SI 3
* %^BOLD%^queue_room%^RESET%^
.EI
.SI 5
varargs void queue_room(string room, int force, string reason)
.EI
.SP 7 5

This method will queue the room without doing any tests.
This should only be used by people who know what they are doing
as it bypasses several checks.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room to queue
.EP
.SP 9 5
force - if it is forced to queue
.EP
.SP 9 5
reason - the reason it was queued

.EP

.SI 3
* %^BOLD%^set_cluster%^RESET%^
.EI
.SI 5
void set_cluster(string room, int cluster)
.EI
.SP 7 5

This method sets the cluster number for the specified room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room to set the cluster number for
.EP
.SP 9 5
cluster - the new cluster number
.EP

.SI 3
* %^BOLD%^set_co_ord%^RESET%^
.EI
.SI 5
void set_co_ord(string room, int * coord)
.EI
.SP 7 5

This method sets the coordinates for the specified room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room to set the coordinates for
.EP
.SP 9 5
coord - the coordinates to set the room to
.EP

.SI 3
* %^BOLD%^set_stopped%^RESET%^
.EI
.SI 5
void set_stopped(int s)
.EI
.SP 7 5

This method sets the stopped attribute of the map handler.
If the stopped attribute is non-zero then the map handler will 
stop processing the currently queued rooms.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the new value of the stopped flag

.EP

.SI 3
* %^BOLD%^static_query_co_ord%^RESET%^
.EI
.SI 5
int * static_query_co_ord(string room)
.EI
.SP 7 5

This method will return the coordinates of the room only using the
internal map information.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room to get the coordinates of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the coordinates of the room, 0 on failure

.EP

.SI 3
* %^BOLD%^static_query_short%^RESET%^
.EI
.SI 5
string static_query_short(string room)
.EI
.SP 7 5

This method returns the short description of the room only from
within the map handlers files.  This will not cause the room
to load and will not work on cloned file names.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room to return the short description of
.EP

.SI 3
* %^BOLD%^test_room%^RESET%^
.EI
.SI 5
int test_room(string room)
.EI
.SP 7 5

This method tests to see if the specified room exists and is loadable.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room to test for existance
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it exists and 0 if it does not

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^update_cache%^RESET%^
.EI
.SI 5
void update_cache(string directory, int load)
.EI
.SP 7 5

This method goes through the cahce on the directory and
removes all the current references to the directory.  If the
load parameter is non-zero then the directory is forced to
be reloaded even if it already exists.  If this parameter is
not set then the directory will only be loaded if it is not
already in the cache.


This method uses the functions in the map helper to set the values in all the internal variables. It also swaps the specified directory onto the end of the cache list, so it will be removed later.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
directory - the directory to update
.EP
.SP 9 5
load - should we force this to reload
.EP


