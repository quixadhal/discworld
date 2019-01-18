.DT
water.c
Disk World autodoc help
water.c

.SH Description
.SP 5 5

This is the co-inheritable for rooms containing water.  It handles adding
the appropriate effects to objects entering the water, skillchecks to see
if they can move around, making their positions appropriate and things like
that.  It also takes care of some miscellaneous stuff like salinity,
turbidity, currents etc.  Rather than using this co-inheritable directly, it
is recommended that you inherit either /std/room/water_inside.c or
/std/room/water_outside.c.
When using this file, remember the following: if you mask event_enter you
should call the inherited function, and unforeseen consequences may result
from using modify_exit with the "function", "exit mess" or "enter mess"
options.  Using "exit mess" or "enter mess" will only disrupt the tailored
swimming/drifting/floating/sinking exit and entry messages, but changing the
exit function will allow people through exits without passing the
appropriate swimming skillcheck.  For this reason, it is recommended to mask
swim_exit instead, and check that the inherited function returns 1 before
allowing them to pass.
.EP
.SP 10 5


Written by Bakhtosh
.EP



.SH See also
.SP 5 5
/std/room/water_inside, /std/room/water_outside and /std/effects/other/immersed

.EP
.SH Includes
.SP 5 5
This class includes the following files /include/tasks.h, /include/position.h, /include/armoury.h and /include/room.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_enter_mess%^RESET%^
.EI
.SI 5
void add_enter_mess(object ob, string mess)
.EI
.SP 7 5

This function sets the next entry message from this room for a particular
object.  It is used automagically by the swim_exit function, but can be used
for other purposes if you feel like it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object for which the next entry message should be set
.EP
.SP 9 5
mess - the next entry message for the object
.EP

.SI 3
* %^BOLD%^add_exit_mess%^RESET%^
.EI
.SI 5
void add_exit_mess(object ob, string mess)
.EI
.SP 7 5

This function sets the next exit message from this room for a particular
object.  It is used automagically by the swim_exit function, but can be used
for other purposes if you feel like it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object for which the next exit message should be set
.EP
.SP 9 5
mess - the next exit message for the object
.EP

.SI 3
* %^BOLD%^add_flow%^RESET%^
.EI
.SI 5
void add_flow(string dir, int rate)
.EI
.SP 7 5

This function adds a water current flowing through a particular exit, which
may sweep objects through it or make it harder for them to swim through.
The second argument is the rate of flow.  If this is positive, then water
will be flowing from this room into the next one.  If it is positive, then
water will be flowing from the next room into this one.  It is up to the
coder of the specific rooms to ensure that the currents in different rooms
match one another.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the exit through which the current is flowing
.EP
.SP 9 5
rate - the strength of the current
.EP

.SI 3
* %^BOLD%^delete_flow%^RESET%^
.EI
.SI 5
void delete_flow(string dir)
.EI
.SP 7 5

This function removes a water current through a particular exit.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the exit for which any water current should be removed
.EP

.SI 3
* %^BOLD%^do_drift%^RESET%^
.EI
.SI 5
int do_drift()
.EI
.SP 7 5

This function is for the drift command, which will allow someone to start
drifting freely rather than swimming.  It is identical to the float command,
except for the messages.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
whether the command succeeded
.EP

.SI 3
* %^BOLD%^do_float%^RESET%^
.EI
.SI 5
int do_float()
.EI
.SP 7 5

This function is for the float command, which will allow someone to start
floating freely rather than swimming.  It is identical to the drift command,
except for the messages.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
whether the command succeeded
.EP

.SI 3
* %^BOLD%^do_swim%^RESET%^
.EI
.SI 5
int do_swim()
.EI
.SP 7 5

This function is for the swim command, which will allow someone to stop
drifting freely and start swimming and resisting anything that tries to move
them.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
whether the command succeeded
.EP

.SI 3
* %^BOLD%^get_enter_mess%^RESET%^
.EI
.SI 5
string get_enter_mess(object ob, string direc)
.EI
.SP 7 5

This function returns the appropriate entry message for the specified object
in the specified direction.  If a value has been set by add_enter_mess then
it is returned.  The query_origin function is used to find a replacement for
the "$F" token.  It is used automagically by the swim_exit function, but can
be overridden if you feel like it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object for which the entry message should be found
.EP
.SP 9 5
direc - the direction in which the object is exiting
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the entry message for this object
.EP

.SI 3
* %^BOLD%^get_exit_mess%^RESET%^
.EI
.SI 5
string get_exit_mess(object ob, string direc)
.EI
.SP 7 5

This function returns the appropriate exit message for the specified object
in the specified direction.  If a value has been set by add_exit_mess then
it is returned.  It is used automagically by the swim_exit function, but can
be overridden if you feel like it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object for which the exit message should be found
.EP
.SP 9 5
direc - the direction in which the object is exiting
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the exit message for this object
.EP

.SI 3
* %^BOLD%^get_swim_enum%^RESET%^
.EI
.SI 5
int get_swim_enum(object thing)
.EI
.SP 7 5

This function returns the effect number of the swimming/immersion effect on
the object specified.  If there isn't one, it adds the effect.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the object for which the swimming effect number should be found
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the swimming effect number
.EP

.SI 3
* %^BOLD%^get_water%^RESET%^
.EI
.SI 5
object get_water()
.EI
.SP 7 5

This function returns some appropriate water from the room.  Its appearance
will depend on the clarity and salinity set in the room.  The quantity of
the water object that it returns is not fixed, and will generally be set by
whatever function called it.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
some water from the room
.EP

.SI 3
* %^BOLD%^get_water_surface_light%^RESET%^
.EI
.SI 5
int get_water_surface_light()
.EI
.SP 7 5

This function returns the amount of light that will filter down to this
room from those above it if set_use_surface_light has been called.  It is
intended to be used only by the update_water_light function, but has been
separated out to allow it to be masked.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the amount of light that filters down from rooms above this one
.EP

.SI 3
* %^BOLD%^lives_in_water%^RESET%^
.EI
.SI 5
int lives_in_water(object ob)
.EI

.SI 3
* %^BOLD%^query_above_room%^RESET%^
.EI
.SI 5
object query_above_room()
.EI
.SP 7 5

This function returns the room above this one, in the direction set by
set_up_dir.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the room above this one
.EP

.SI 3
* %^BOLD%^query_below_room%^RESET%^
.EI
.SI 5
object query_below_room()
.EI
.SP 7 5

This function returns the room below this one, in the direction set by
set_down_dir.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the room below this one
.EP

.SI 3
* %^BOLD%^query_bottom%^RESET%^
.EI
.SI 5
int query_bottom()
.EI
.SP 7 5

This function returns 1 if this is a bottom room, and 0 otherwise.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
whether this is a bottom room
.EP

.SI 3
* %^BOLD%^query_bottom_mess%^RESET%^
.EI
.SI 5
string query_bottom_mess()
.EI
.SP 7 5

This function returns the position that non-living items will have when they
are lying on the bottom in this room.
.EP

.SI 3
* %^BOLD%^query_bottom_room%^RESET%^
.EI
.SI 5
object query_bottom_room()
.EI
.SP 7 5

This function returns the bottom room in a vertical stack of water rooms.
If there is a bottom water room, it is returned, else, if the bottom room is
not water, or there is no bottom room, 0 is returned.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the first room below this one to be on the bottom
.EP

.SI 3
* %^BOLD%^query_clarity%^RESET%^
.EI
.SI 5
int query_clarity()
.EI
.SP 7 5

This function returns the current clarity of the water, which is an integer
variable between 1 and 100.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current clarity of the water
.EP

.SI 3
* %^BOLD%^query_death_reason%^RESET%^
.EI
.SI 5
string query_death_reason()
.EI
.SP 7 5

With this function here, creators will be able to point and laugh when
someone dies by drowning in a water room because they'll see the death
reason in an inform.  It will also show up in the death log.  It would be a
good idea to mask this function to return a reason more specific to the
place where the drowning may occur (such as "drowning in the Djel"), or at
least a humorous one (such as "failing to realise that scuba gear hasn't
been invented yet") so that creators will have something to laugh at.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the reason for death
.EP

.SI 3
* %^BOLD%^query_default_search_description%^RESET%^
.EI
.SI 5
string * query_default_search_description()
.EI
.SP 7 5

This sets up different search returns for the default 'not there'
search result.

.EP

.SI 3
* %^BOLD%^query_down_dir%^RESET%^
.EI
.SI 5
string query_down_dir()
.EI
.SP 7 5

This function returns the direction that is currently considered to be down
in this room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current direction considered to be down
.EP

.SI 3
* %^BOLD%^query_float_in_mess%^RESET%^
.EI
.SI 5
string query_float_in_mess()
.EI
.SP 7 5

This function returns the message that will be displayed when an object
floats into this room, with the usual $-expansion not done.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current entry message for floating objects
.EP

.SI 3
* %^BOLD%^query_float_mess%^RESET%^
.EI
.SI 5
string query_float_mess()
.EI
.SP 7 5

This function returns the position that non-living items will have when they
are floating in this room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current position for floating items
.EP

.SI 3
* %^BOLD%^query_float_out_mess%^RESET%^
.EI
.SI 5
string query_float_out_mess()
.EI
.SP 7 5

This function returns the message that will be displayed when an object
floats out of this room, with the usual $-expansion not done.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current entry message for floating objects
.EP

.SI 3
* %^BOLD%^query_flow%^RESET%^
.EI
.SI 5
int query_flow(string dir)
.EI
.SP 7 5

This function returns the strength of the current flowing through a
particular exit, if any.  A positive value represents a current flowing from
this room into the next one, and a negative value represents a current
flowing from the next room into this one.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the exit which should have its current returned
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current through this ext
.EP

.SI 3
* %^BOLD%^query_flows%^RESET%^
.EI
.SI 5
mapping query_flows()
.EI
.SP 7 5

This function returns a mapping of all the current flows through exits in
this room.  The keys of the mapping are the exits through which the currents
flow, and the values are the rates of flow.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a mapping of the current flows through exits in this room
.EP

.SI 3
* %^BOLD%^query_nonfloat_mess%^RESET%^
.EI
.SI 5
string query_nonfloat_mess()
.EI
.SP 7 5

This function returns the position that non-living items will have when they
are neither sinking nor floating in this room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current position for drifting items
.EP

.SI 3
* %^BOLD%^query_origin%^RESET%^
.EI
.SI 5
string query_origin(string dir)
.EI
.SP 7 5

This function returns the opposite to the direction of a particular exit.
This information is normally only accessible within the room handler, but it
is cached here in the origins mapping by the add_exit in this file.  If no
value is found, "elsewhere" will be returned.  This value is used in exit
messages to correctly display where they are coming from.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the exit for which the opposite direction should be found
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the opposite of the specified direction
.EP

.SI 3
* %^BOLD%^query_salinity%^RESET%^
.EI
.SI 5
int query_salinity()
.EI
.SP 7 5

This function returns the current salinity of the water, which is an integer
variable between 0 and 100.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current salinity of the water
.EP

.SI 3
* %^BOLD%^query_sink_in_mess%^RESET%^
.EI
.SI 5
string query_sink_in_mess()
.EI
.SP 7 5

This function returns the message that will be displayed when an object
sinks into this room, with the usual $-expansion not done.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current entry message for sinking objects
.EP

.SI 3
* %^BOLD%^query_sink_out_mess%^RESET%^
.EI
.SI 5
string query_sink_out_mess()
.EI
.SP 7 5

This function returns the message that will be displayed when an object
sinks out of this room, with the usual $-expansion not done.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current exit message for sinking objects
.EP

.SI 3
* %^BOLD%^query_sinking_mess%^RESET%^
.EI
.SI 5
string query_sinking_mess()
.EI
.SP 7 5

This function returns the position that non-living items will have when they
are sinking in this room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current position for sinking items
.EP

.SI 3
* %^BOLD%^query_surface%^RESET%^
.EI
.SI 5
int query_surface()
.EI
.SP 7 5

This function returns 1 if this is a surface room, and 0 otherwise.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
whether this is a surface room
.EP

.SI 3
* %^BOLD%^query_surface_room%^RESET%^
.EI
.SI 5
object query_surface_room()
.EI
.SP 7 5

This function returns the top room in a vertical stack of water rooms.  If
there is a surface water room, it is returned, else, if the top room is not
water, or there is no surface room, 0 is returned.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the first room above this one to be on the surface
.EP

.SI 3
* %^BOLD%^query_sweep_in_mess%^RESET%^
.EI
.SI 5
string query_sweep_in_mess()
.EI
.SP 7 5

This function returns the message that will be displayed when an object
is swept into this room by a current, with the $-expansion not done.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current entry message for objects swept by a current
.EP

.SI 3
* %^BOLD%^query_sweep_out_mess%^RESET%^
.EI
.SI 5
string query_sweep_out_mess()
.EI
.SP 7 5

This function returns the message that will be displayed when an object
is swept out of this room by a current, with the $-expansion not done.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current exit message for objects swept by a current
.EP

.SI 3
* %^BOLD%^query_terrain_map_colour%^RESET%^
.EI
.SI 5
string query_terrain_map_colour()
.EI

.SI 3
* %^BOLD%^query_turbidity%^RESET%^
.EI
.SI 5
int query_turbidity()
.EI
.SP 7 5

This function returns the current turbidity of the water, which is an
integer variable between 1 and 100.  It will be equal to 100 minus the
current clarity of the water.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current turbidity of the water
.EP

.SI 3
* %^BOLD%^query_turbulence%^RESET%^
.EI
.SI 5
int query_turbulence()
.EI
.SP 7 5

This function returns the current turbulence of the water, which is a
non-negative integer.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current turbulence of the water
.EP

.SI 3
* %^BOLD%^query_underwater%^RESET%^
.EI
.SI 5
int query_underwater()
.EI
.SP 7 5

This function returns 1 if this room is underwater (that is, it is not a
surface room), and 0 otherwise.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
whether this is an underwater room
.EP

.SI 3
* %^BOLD%^query_up_dir%^RESET%^
.EI
.SI 5
string query_up_dir()
.EI
.SP 7 5

This function returns the direction that is currently considered to be up in
this room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current direction considered to be up
.EP

.SI 3
* %^BOLD%^query_water%^RESET%^
.EI
.SI 5
int query_water()
.EI
.SP 7 5

This function returns 1 to indicate that this is a water room.  It fulfills
the same purpose as the inherits efun in this case, but may be slightly
easier to use.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1
.EP

.SI 3
* %^BOLD%^query_water_surface_light%^RESET%^
.EI
.SI 5
int query_water_surface_light()
.EI
.SP 7 5

This function returns the amount of light that will filter down to rooms
below this one that have had set_use_surface_light called in them.  Its
default is to return a value based on the light in this room and the clarity
of the water.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the amount of light that filters down to rooms below this one
.EP

.SI 3
* %^BOLD%^query_water_traction_bonus%^RESET%^
.EI
.SI 5
int query_water_traction_bonus(object thing, int buoyancy)
.EI
.SP 7 5

This function returns the bonus that objects get to move along the bottom or
to resist such movement here.  If this should be anything unusual, such as
for a very smooth bottom or one with handles, this function should be masked
to return something different.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the object that is moving along the bottom
.EP
.SP 9 5
buoyancy - the object's relative buoyancy
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the traction bonus on the bottom here
.EP

.SI 3
* %^BOLD%^set_bottom%^RESET%^
.EI
.SI 5
void set_bottom(int val)
.EI
.SP 7 5

This function sets whether or not this room has a solid surface or bottom in
it.  If it does, then items may appear as being on the bottom, and living
objects are more able to resist currents (as they have something to hold on
to).  The parameter of this function can be 1 to make this room have a
bottom, 0 to make it have no bottom, or -1 (the default) to make it decide
whether or not it has a bottom by checking to see if it has any exits in the
current down direction (as set by set_down_dir).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
val - whether or not the room has a bottom
.EP

.SI 3
* %^BOLD%^set_bottom_mess%^RESET%^
.EI
.SI 5
void set_bottom_mess(string mess)
.EI
.SP 7 5

This function sets the position that non-living items will have when they
are lying on the bottom in this room.  The default is "lying on the bottom".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mess - the new position for items lying on the bottom
.EP

.SI 3
* %^BOLD%^set_clarity%^RESET%^
.EI
.SI 5
void set_clarity(int how_clear)
.EI
.SP 7 5

This function sets the clarity of the water as an integer variable between 1
and 100.  The main effect of this is to reduce the light levels of the room
to simulate the obscuring effect of turbid water.  The default clarity is
90.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
how_clear - the new clarity of the water
.EP

.SI 3
* %^BOLD%^set_down_dir%^RESET%^
.EI
.SI 5
void set_down_dir(string dir)
.EI
.SP 7 5

This function sets the direction that is considered to be down by this room.
This is used by several exit messages.  Also, dense objects will be inclined
to move in this direction.  The default, of course, is "down".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mes - the new direction to be considered to be down
.EP

.SI 3
* %^BOLD%^set_float_in_mess%^RESET%^
.EI
.SI 5
void set_float_in_mess(string mess)
.EI
.SP 7 5

This function sets the message that will be displayed when an object floats
into this room.  The string "$up$" will be replaced by the current up
direction for this room (as set by set_up_dir), and the usual other
$-expansion for messages will occur, including the replacement of "$F" by
the direction from which they are arriving.  The default is "$N float$s $up$
from $F.".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mess - the new entry message for floating objects
.EP

.SI 3
* %^BOLD%^set_float_mess%^RESET%^
.EI
.SI 5
void set_float_mess(string mess)
.EI
.SP 7 5

This function sets the position that non-living items will have when they
are floating in this room.  The default is "floating nearby".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mess - the new position for floating items
.EP

.SI 3
* %^BOLD%^set_float_out_mess%^RESET%^
.EI
.SI 5
void set_float_out_mess(string mess)
.EI
.SP 7 5

This function sets the message that will be displayed when an object floats
out of this room.  The string "$up$" will be replaced by the current up
direction for this room (as set by set_up_dir), and the usual other
$-expansion for messages will occur.  The default is "$N float$s $up$.".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mess - the new exit message for floating objects
.EP

.SI 3
* %^BOLD%^set_nonfloat_mess%^RESET%^
.EI
.SI 5
void set_nonfloat_mess(string mess)
.EI
.SP 7 5

This function sets the position that non-living items will have when they
are neither sinking nor floating in this room.  The default is "drifting
nearby".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mess - the new position for drifting items
.EP

.SI 3
* %^BOLD%^set_salinity%^RESET%^
.EI
.SI 5
void set_salinity(int how_salty)
.EI
.SP 7 5

This function sets the salinity of the water as an integer variable between
0 and 100.  Higher salinity will have a small positive effect on the
buoyancy of objects in the room.  The default salinity is 0.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
how_salty - the new salinity of the water
.EP

.SI 3
* %^BOLD%^set_sink_in_mess%^RESET%^
.EI
.SI 5
void set_sink_in_mess(string mess)
.EI
.SP 7 5

This function sets the message that will be displayed when an object sinks
into this room.  The string "$down$" will be replaced by the current down
direction for this room (as set by set_down_dir), and the usual other
$-expansion for messages will occur, including the replacement of "$F" by
the direction from which they are arriving.  The default is "$N sink$s
$down$ from $F.".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mess - the new entry message for sinking objects
.EP

.SI 3
* %^BOLD%^set_sink_out_mess%^RESET%^
.EI
.SI 5
void set_sink_out_mess(string mess)
.EI
.SP 7 5

This function sets the message that will be displayed when an object sinks
out of this room.  The string "$down$" will be replaced by the current down
direction for this room (as set by set_down_dir), and the usual other
$-expansion for messages will occur.  The default is "$N sink$s $down$.".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mess - the new exit message for sinking objects
.EP

.SI 3
* %^BOLD%^set_sinking_mess%^RESET%^
.EI
.SI 5
void set_sinking_mess(string mess)
.EI
.SP 7 5

This function sets the position that non-living items will have when they
are sinking in this room.  The default is "sinking nearby".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mess - the new position for sinking items
.EP

.SI 3
* %^BOLD%^set_surface%^RESET%^
.EI
.SI 5
void set_surface(int val)
.EI
.SP 7 5

This function sets whether or not this room has an interface with air or
surface in it.  If it does, then living objects may breathe here, and
turbidity does not effect visibility.  The parameter of this function can be
1 to make this room have a surface, 0 to make it have no surface, or -1 (the
default) to make it decide whether or not it has a surface by checking to
see if it has any exits in the current up direction (as set by set_up_dir).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
val - whether or not the room has a surface
.EP

.SI 3
* %^BOLD%^set_sweep_in_mess%^RESET%^
.EI
.SI 5
void set_sweep_in_mess(string mess)
.EI
.SP 7 5

This function sets the message that will be displayed when an object is
swept into this room by a current.  The usual $-expansion for messages
will occur.  The default is "$N $V$0=is,are$V$ swept in from $F by the
current.".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mess - the new entry message for objects swept by a current
.EP

.SI 3
* %^BOLD%^set_sweep_out_mess%^RESET%^
.EI
.SI 5
void set_sweep_out_mess(string mess)
.EI
.SP 7 5

This function sets the message that will be displayed when an object is
swept out of this room by a current.  The usual $-expansion for messages
will occur, including the replacement of "$T" by the direction in which they
are moving.  The default is "$N $V$0=is,are$V$ swept $T by the current.".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mess - the new exit message for objects swept by a current
.EP

.SI 3
* %^BOLD%^set_turbidity%^RESET%^
.EI
.SI 5
void set_turbidity(int how_murky)
.EI
.SP 7 5

This function is an alternative method of setting the clarity of the water.
The turbidity is defined as 100 minus the clarity of the water.  The default
turbidity is 10.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
how_murky - the new turbidity of the water
.EP

.SI 3
* %^BOLD%^set_turbulence%^RESET%^
.EI
.SI 5
void set_turbulence(int how_turbulent)
.EI
.SP 7 5

This function sets the turbulence of the water as a non-negative integer
variable.  A random number up to the turbulence is added to the difficulty
of all skillchecks to leave the room via a water exit with swim_exit as the
exit function, except when the exiting object is not moving of its own
accord (such as when it is being swept along by a current).  The default
turbulence is 100.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
how_turbulent - the new turbulence of the water
.EP

.SI 3
* %^BOLD%^set_up_dir%^RESET%^
.EI
.SI 5
void set_up_dir(string dir)
.EI
.SP 7 5

This function sets the direction that is considered to be up by this room.
This is used by several exit messages, and by the water effect to decide
which way a panicking player will flee to try to reach the surface.  For
this reason, it should be set to the name of an exit which leads towards a
surface room.  Also, buoyant objects will be inclined to move in this
direction.  The default, of course, is "up".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mess - the new direction to be considered to be up
.EP

.SI 3
* %^BOLD%^set_use_surface_light%^RESET%^
.EI
.SI 5
void set_use_surface_light(int val)
.EI
.SP 7 5

This function sets whether the room will use the light levels of the
surface to determine its own.  If the function is called with a non-zero
value, the the current light level in the room will be overridden by a new
value based on the light of the room found by query_above_room.  If the room
above is an outside room, with light levels that depend on the time of day,
then the light in this room will be updated every time it is queried.  A
water room will default to using this option, but it will be overridden by
any calls to adjust_light (including calls to set_light).  It is possible to
call set_water_light to avoid this, but it should not be necessary.  If a
series of rooms with vertical exits between them are all set to use surface
light, then the effect will be for the light of the room at the top of the
stack to filter down through the rest, appropriately attenuated by
turbidity.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
val - whether the room should use light from the surface
.EP

.SI 3
* %^BOLD%^soak%^RESET%^
.EI
.SI 5
varargs void soak(object ob, int ignore_location)
.EI
.SP 7 5

This function makes things wet.  Anything entering a water room has this
function called on it by event_enter, and will have the wetness effect added
to it if appropriate, as well as to any appropriate objects inside it if it
isn't waterproof.  Open containers will also be filled with water.  The
function checks to see that the object is indeed inside the room, unless the
optional extra argument is non-zero.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to be soaked
.EP
.SP 9 5
ignore_location - whether the object should be soaked wherever it is
.EP

.SI 3
* %^BOLD%^swim_exit%^RESET%^
.EI
.SI 5
int swim_exit(string dir, object ob, string mess)
.EI
.SP 7 5

This is an exit function set for any exits in a water room.  It finds the
difficulty of swimming through the exit, on the basis of the current through
that exit, the buoyancy of the object and the extra difficulty that it has
in swimming.  The swimming/immersion effect calculates the latter two with
calc_buoyancy and swim_difficulty respectively.  The object is subjected to
a skillcheck in other.movement.swimming to see if it can move through the
exit.
There is a guildpoint cost equal to 1/20th of the difficulty of the
skillcheck.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the direction in which the object is leaving
.EP
.SP 9 5
ob - the object that is trying to leave
.EP
.SP 9 5
mess - a weird extra string that isn't relevant here
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
whether or not the object can move through the exit
.EP

.SI 3
* %^BOLD%^update_water_light%^RESET%^
.EI
.SI 5
void update_water_light()
.EI
.SP 7 5

This function updates the amount of light filtering down to this room from
the rooms above it, if it is necessary to do so.  It is called by
query_light in /std/room/water_inside and /std/room/water_outside.
.EP


