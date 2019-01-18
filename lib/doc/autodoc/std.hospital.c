.DT
hospital.c
Disk World autodoc help
hospital.c

.SH Description
.SP 5 5
 This is a generalised hospital inheritable for the creation of NPCs.


The NPCs within the hospital are defined in the configuration files which are parsed by the hospital. The configuration files can define zones, groups and npcs. 

A zone is a region of the mud. It can have any name you choose and is then used as the first parameter to load_random_npc(). A zone may be made up of other zones, groups and npcs. 

A group is a group of NPCs. It may be made up of other groups and npcs. A group has a name by which it is referenced by zones and other groups. 

An npc is the definition of a specific type of NPC. 
.EP
.SP 10 5


Written by Taffyd

Started December 3, 1998.
.EP

.SH Change history
.SP 5 5
Ceres November 14th 2002


Altered to work as a hospital for terrain NPCs especially the transient
ones. It now handles transient npcs as well as conditions for
diurnal, nocturnal and seasonal npcs.

Pinkfish March 10th


Major rewrite of bits to make it use a lisp like data file format.

Carmine April 9th


Added some alignment-generation code.
.EP

.SH Example
.SI 5
A typical hospital setup would look like this:

void setup() {
  set_save_file(SAVE + "/hospital");
  set_data_directory(DATA + "/hospital");
  set_log_directory(LOG + "/hospital");
  set_npc_path(CHARS + "/hospital");
  set_not_replaceable(1);

  set_short("Our hospital");
  set_long("This is the hospital.\n");
}

These are examples of zones, groups & npcs.

(zone
  (name "desert")
  (busy 5)
  (group
    (name "vultures")
    (chance 1)
  )
  (npc
    ("name "lizard")
    (chance 1)
  )
)

(group
  (name "vultures")
  (transient "true")
  (npc
    (name "vulture")
    (quantity 1 + 1d5)
  )
)

(npc
  (name "vulture")
  (path "vulture")
  (transient true)
)
(npc
  (name "lizard")
  (path "lizard")
  (transient true)
  (diurnal true)
)

The above configuration file defines a zone "desert". The zone has a
business factor of 5 which means rooms in the zone will typically get a
chance at cloning npcs 5% of the time (every 20 rooms).



The zone is made up of a group called "vultures" and an npc called
"lizard".



The vultures group is transient (ie. the NPCs will be removed from the
room when the player leaves the room, rather like wandering monsters) and
contains 1 + 1d5 "vulture" npcs.



The vulture NPC is transient and can be found in the filename "vulture.c" in
the path defined by set_npc_path(). The lizard NPC is also transient and
diurnal (it only comes out during the day) and can be found in the filename
"lizard.c" in the path defined by set_npc_path().




.EI
.SH Inherits
.SP 5 5
This class inherits the following classes /std/hospital/alignment and /std/room.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/armoury.h, /include/data.h, /include/hospital.h and /include/weather.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_report%^RESET%^
.EI
.SI 5
int do_report()
.EI
.SP 7 5

Prints a report of all the npcs in the hospital currently.

.EP

.SI 3
* %^BOLD%^get_item%^RESET%^
.EI
.SI 5
int get_item(object destination, string * items)
.EI
.SP 7 5

This function provides an interface to the armoury, selecting
an item at random from the 'items' array and attempting to
clone it.


Any items of jewellery, clothing, weapons, armour or scabbards that are available in the armoury can be accessed using this function. 


.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
destination - where to place the item that is fetched
from the armoury.
.EP
.SP 9 5
items - an array of items, of which one will be selected,
cloned, and then moved to destination.



.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the item was successfully created, 0 if it was not.

.EP

.SI 3
* %^BOLD%^hospital_log_file%^RESET%^
.EI
.SI 5
void hospital_log_file(string file, string format, mixed * args ...)
.EI
.SP 7 5

This method provides log_file() functionality but for /d/am/log
instead.  You should use this method to record errors and whatever
that are domain specific instead of using log_file().
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
file - the file name to log to. This will have /d/am/log/
inserted before it.
.EP
.SP 9 5
format - the text to log, can contain sprintf() format specifiers.
.EP
.SP 9 5
args - the arguments for any sprintf format thingies

.EP

.SI 3
* %^BOLD%^is_group_under_max_population%^RESET%^
.EI
.SI 5
int is_group_under_max_population(string name)
.EI
.SP 7 5

This method checks to see if the number of npcs of this
type is under the maximum population.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
npc - the npc to check to see if it is under population max
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if is under, 0 if is not

.EP

.SI 3
* %^BOLD%^is_npc_under_max_population%^RESET%^
.EI
.SI 5
int is_npc_under_max_population(string name)
.EI
.SP 7 5

THis method checks to see if the number of npcs of this
type is under the maximum population.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
npc - the npc to check to see if it is under population max
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if is under, 0 if is not

.EP

.SI 3
* %^BOLD%^load_group_npcs%^RESET%^
.EI
.SI 5
object * load_group_npcs(string group_name)
.EI
.SP 7 5

This method creates the npcs for the specificed group of npcs.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
group_name - the name of the group
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the npcs

.EP

.SI 3
* %^BOLD%^load_npc_object%^RESET%^
.EI
.SI 5
object load_npc_object(string npc_name)
.EI
.SP 7 5

This method loads the specific npc.  It will do everything it needs
to create the npc.  In the case of unique npcs they are not cloned.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
npc_name - the name of the npc to load
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the npc object

.EP

.SI 3
* %^BOLD%^load_random_npc%^RESET%^
.EI
.SI 5
object * load_random_npc(string zone, int wealth, int busy)
.EI
.SP 7 5

This method randomly selects an NPC or a group of NPC's from
the list of defined NPC's for the room/region.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
zone - This is a zone (as defined in the configuration files).
.EP
.SP 9 5
wealth - How wealthy this particular part of this zone is.
.EP
.SP 9 5
busy - How busy this particular part of this zone is.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Returns a group of NPCs randomly selected.

.EP

.SI 3
* %^BOLD%^make_unique%^RESET%^
.EI
.SI 5
int make_unique(mixed who)
.EI
.SP 7 5

This method returns whether or not it is time to regenerate a
unique NPC. The regeneration time is controlled by the REGEN_TIME
#define.



.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
who - this is the name or object reference of the NPC
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is time to make the unique NPC, or 0 if it is not.

.EP

.SI 3
* %^BOLD%^npc_died%^RESET%^
.EI
.SI 5
void npc_died(object ob)
.EI
.SP 7 5

This method is called when a specific npc dies so we can do some
tracking on it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
npc_ob - the npc object

.EP

.SI 3
* %^BOLD%^npc_path%^RESET%^
.EI
.SI 5
string npc_path(string str)
.EI
.SP 7 5

This method expands the path for the given npc by combining the npc_path
set in set_npc_path with the relative path defined in the configuration
file.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
path - the path to expand
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the expanded path

.EP

.SI 3
* %^BOLD%^query_data_directory%^RESET%^
.EI
.SI 5
string query_data_directory()
.EI
.SP 7 5

This method returns the directory to scan for new files.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the directory to scan new data
.EP

.SI 3
* %^BOLD%^query_debug_npc_info%^RESET%^
.EI
.SI 5
mapping query_debug_npc_info()
.EI
.SP 7 5

This returns all the npcs in the debug data array.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
this returns the debug information for the npcs

.EP

.SI 3
* %^BOLD%^query_deities%^RESET%^
.EI
.SI 5
string * query_deities()
.EI
.SP 7 5

This function returns an array of the gods in Ankh-Morpork



.EP

.SI 3
* %^BOLD%^query_disable_npc_generation%^RESET%^
.EI
.SI 5
int query_disable_npc_generation()
.EI
.SP 7 5

This method returns the flag that disables the npc generation.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the flag that disables the npc generation
.EP

.SI 3
* %^BOLD%^query_group_info%^RESET%^
.EI
.SI 5
class group_info query_group_info(string group)
.EI
.SP 7 5

This returns the group info class for the group in question.  THis should
only be used for debugging.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
group - the name of the group to get the info on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the class of the group

.EP

.SI 3
* %^BOLD%^query_npc_current_population%^RESET%^
.EI
.SI 5
int query_npc_current_population(string npc, int clean)
.EI
.SP 7 5

This method returns the current population of this npc
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
npc - the name of the npc
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the npc population

.EP

.SI 3
* %^BOLD%^query_npc_info%^RESET%^
.EI
.SI 5
class npc_info query_npc_info(string npc)
.EI
.SP 7 5

This returns the npc info class for the npc in question.  THis should
only be used for debugging.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
npc - the name of the npc to get the info on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the class of the npc

.EP

.SI 3
* %^BOLD%^query_npc_max_population%^RESET%^
.EI
.SI 5
int query_npc_max_population(string npc)
.EI
.SP 7 5

This method returns the maximum population of this npc.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
npc - the name of the npc
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the npc population

.EP

.SI 3
* %^BOLD%^query_registered_npcs%^RESET%^
.EI
.SI 5
string * query_registered_npcs()
.EI
.SP 7 5

This function returns the ids of all the NPCs which are registered with the 
hospital, ie the ones that have to be specified with add_hospital_npc().

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
All the ids.

.EP

.SI 3
* %^BOLD%^query_save_file%^RESET%^
.EI
.SI 5
string query_save_file()
.EI
.SP 7 5

This returns the location where hospital data will be stored.

.EP

.SI 3
* %^BOLD%^regen_after_death%^RESET%^
.EI
.SI 5
void regen_after_death(object dead_npc)
.EI
.SP 7 5

This method is called by the npc.death effect after an NPC that is handled
by the hospital has died. It tells the room where it was generated to
make a new NPC and to all sorts of funky things.


This is not fully implemented yet. 
.EP

.SI 3
* %^BOLD%^reset_unique%^RESET%^
.EI
.SI 5
void reset_unique(string who)
.EI
.SP 7 5

This method resets the unique status of the npc so it can be tested for
loading over again.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
npc - the name of the npc

.EP

.SI 3
* %^BOLD%^roll_dice%^RESET%^
.EI
.SI 5
int roll_dice(mixed die)
.EI
.SP 7 5

This method rolls the dice, if a dice is specified.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dice - the dice or number
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the actual number

.EP

.SI 3
* %^BOLD%^scan_for_new_data%^RESET%^
.EI
.SI 5
int scan_for_new_data(int force)
.EI
.SP 7 5

This method scans through all the files and sees if anything has
changed.

.EP

.SI 3
* %^BOLD%^set_data_directory%^RESET%^
.EI
.SI 5
void set_data_directory(string dir)
.EI
.SP 7 5

This method sets the directory to scan for new files. The data directory
stores the configuration files for the hospital that define the zones,
groups and npcs.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to scan for new files

.EP

.SI 3
* %^BOLD%^set_disable_npc_generation%^RESET%^
.EI
.SI 5
void set_disable_npc_generation(int value)
.EI
.SP 7 5

This method sets the flag which enables or disables npc generation. This
is mostly used for debugging.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
value - the new value of the flag

.EP

.SI 3
* %^BOLD%^set_domain%^RESET%^
.EI
.SI 5
void set_domain(string domain)
.EI
.SP 7 5

This method sets the domain string used when requesting things
from the hospital.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
domain - the domain name

.EP

.SI 3
* %^BOLD%^set_max_cache%^RESET%^
.EI
.SI 5
void set_max_cache(int num)
.EI
.SP 7 5

This method sets the maximum number of transient npcs of a given type
that the hospital will cache. 

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the maximum number

.EP

.SI 3
* %^BOLD%^set_npc_path%^RESET%^
.EI
.SI 5
void set_npc_path(string path)
.EI
.SP 7 5

This method sets the directory for npcs.  The filename for npcs defined
in the configuration files is realtive to this path.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the director for npcs.

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^finish_compiling%^RESET%^
.EI
.SI 5
void finish_compiling(string fname, mapping data)
.EI
.SP 7 5

This is called by the data compiler when the system has
finished compiling the data file.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
data - the data to deal with

.EP

.SI 3
* %^BOLD%^set_log_directory%^RESET%^
.EI
.SI 5
void set_log_directory(string new_dir)
.EI
.SP 7 5

This method sets the directory in to which hospital_log_file will
log by default. If the directory does not exist, then it is set to
DEFAULT_LOG_DIRECTORY.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
new_dir - the directory to log to.
.EP

.SI 3
* %^BOLD%^set_save_file%^RESET%^
.EI
.SI 5
void set_save_file(string new_save)
.EI
.SP 7 5

This method is used to set the save file for the hospital.  This
file is where all of the hospitals NPC data is stored while it is
not active and in between reboots.


If this file does not exist when the hospital is loaded, then the variables will be initialised to their defaults and %^BOLD%^restore_default_npcs()%^RESET%^called. 


.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
new_save - the file name to save the hospital data in

.EP


