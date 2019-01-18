.DT
hospital.h
Disk World autodoc help
hospital.h

.SH Description
.SP 5 5

This controls where the hospital will log to by default if a logging 
directory is not set.

.EP
.SP 10 5

.EP

.SH Classes

.SI 3
* %^BOLD%^group_info%^RESET%^
class group_info {
string * move_zones;
mapping npcs;
mapping npc_commands;
int protect;
int defend;
int transient;
int max_population;
mixed * storage;
int no_creates;
int next_update;
}

.EI
.SP 7 5

This class stores all the information about an NPC group.

.EP

.SI 3
* %^BOLD%^npc_info%^RESET%^
class npc_info {
string path;
int unique;
int delay;
int transient;
int nocturnal;
int diurnal;
string * seasonal;
int max_population;
string * move_zones;
string * commands;
object * population;
int no_deaths;
int no_created;
int no_reused;
int next_update;
}

.EI
.SP 7 5

This class stores all the information about a particular NPC.

.EP

.SI 3
* %^BOLD%^regenerated_npc%^RESET%^
class regenerated_npc {
string type;
string load_position;
}

.EI
.SP 7 5

This class stores information about a regenerated NPC.
.EP

.SI 3
* %^BOLD%^unique_npc%^RESET%^
class unique_npc {
int next_regen_time;
}

.EI
.SP 7 5

This class stores all the information about unique NPCs.
.EP

.SI 3
* %^BOLD%^zone_info%^RESET%^
class zone_info {
int wealth;
int busy;
int npc_chance;
mapping npcs;
int group_chance;
mapping groups;
int zone_chance;
mapping zones;
}

.EI
.SP 7 5

This class stores all the information about a zone.
.EP


.SH Defines
.SI 3
* %^BOLD%^DEFAULT_LOG_DIRECTORY%^RESET%^
.EI

.SI 3
* %^BOLD%^DEFAULT_MAX_CACHE%^RESET%^
.EI

.SI 3
* %^BOLD%^DEFAULT_POPULATION%^RESET%^
.EI
.SP 7 5

This defines the default population for a group of NPCs if a default
is not set.

.EP

.SI 3
* %^BOLD%^HOSPITAL_ALIGNMENT_INHERIT%^RESET%^
.EI

.SI 3
* %^BOLD%^MAX_AM_LIVING%^RESET%^
.EI

.SI 3
* %^BOLD%^MAX_DJEL_LIVING%^RESET%^
.EI

.SI 3
* %^BOLD%^MAX_NAMED_LIVING%^RESET%^
.EI

.SI 3
* %^BOLD%^MINIMUM_DEATH_REGEN%^RESET%^
.EI

.SI 3
* %^BOLD%^MONSTER%^RESET%^
.EI

.SI 3
* %^BOLD%^POPULATION_UPDATE_TIME%^RESET%^
.EI
.SP 7 5

How often we recheck the population to make sure 0's etc have not snuck
in.

.EP

.SI 3
* %^BOLD%^REGEN_TIME%^RESET%^
.EI

