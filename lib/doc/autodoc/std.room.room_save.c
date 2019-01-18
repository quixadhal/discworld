.DT
room_save.c
Disk World autodoc help
room_save.c

.SH Description
.SP 5 5

A saving room inheritable.  This handles rooms which save their inventory,
or more specifically part of their inventory.

Which objects should be saved or not saved can be controlled by overriding
test_save.

Containers in this room that need to cause it to save its inventory should
generate a save event.

.EP
.SP 10 5


Written by ceres

.EP



.SH See also
.SP 5 5
test_save and event_save

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room, /std/room/inherit/room_save and /std/basic/auto_load.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player.h, /include/move_failures.h and /include/playtesters.h.
.EP

