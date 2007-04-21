#ifndef __STD_HOSPITAL_H
#define __STD_HOSPITAL_H

#define HOSPITAL_ALIGNMENT_INHERIT "/std/hospital/alignment"

#define REGEN_TIME (1 * 60 * 60)
#define DEFAULT_MAX_CACHE 10
/**
 * This controls where the hospital will log to by default if a logging 
 * directory is not set.
 */
#define DEFAULT_LOG_DIRECTORY "/log"
/**
 * This defines the default population for a group of NPCs if a default
 * is not set.
 */ 
#define DEFAULT_POPULATION 3
#define MINIMUM_DEATH_REGEN 3

/**
 * How often we recheck the population to make sure 0's etc have not snuck
 * in.
 */ 
#define POPULATION_UPDATE_TIME 15 * 60

/**
 * This class stores information about a regenerated NPC.
 * @param type the type of NPC that is to be regenerated
 * @param load_position the file name of the room where the NPC will 
 * regenerate.
 */ 
class regenerated_npc {
    string type;
    string load_position;
}

/**
 * This class stores all the information about a zone.
 * @param wealth The wealthiness of the zone (unused)
 * @param busy The business of the zone. ie. how frequently NPCs should be
 * created.
 * @param npc_chance The total chances for an npc in this zone.
 * @param npcs A list of the NPCs that are members of this zone.
 * @param group_chance The total chances for a group in this zone.
 * @param groups A list of the groups of NPCs in this zone.
 * @param zone_chance The total chances for a zone in this zone.
 * @param zones A list of the zones within this zone.
 */
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

/**
 * This class stores all the information about unique NPCs.
 * @param next_regen_time When this npc will next regenerate.
 */
class unique_npc {
   int next_regen_time;
}

/**
 * This class stores all the information about a particular NPC.
 *
 * @param path The relative path of the NPC
 * @param unique Is the NPC unique?
 * @param delay The regeneration delay for this NPC
 * @param transient If this NPC is transient.
 * @param nocturnal If this NPC is nocturnal.
 * @param diurnal If this NPC is diurnal.
 * @param seasonal A list of seasons during which this NPC is active.
 * @param max_population How many of these NPCs can exist at a time.
 * @param move_zones The move zones for this NPC
 * @param commands Commands to be executed by the NPC when it is created.
 * @param population A cache of the current population
 * @param no_deaths The number of recorded deaths
 * @param no_created The number of these NPCs we've created
 * @param no_reused How many of these NPCs have been reused (recycled)
 * @param next_update When the population data will next be updated.
 */
class npc_info {
   string path;
   int unique;
   int delay; // used by unique npcs

   int transient;
   int nocturnal;
   int diurnal;

   string* seasonal;
   
   int max_population;
   string* move_zones;
   string* commands;

   // Tracking information
   object* population;
   int no_deaths;
   int no_created;
   int no_reused;
   int next_update;
}

/**
 * This class stores all the information about an NPC group.
 *
 * @param move_zones The move zones for this NPC
 * @param npcs A list of the NPCs within this group.
 * @param npc_commands A list of commands to be executed on each of the group
 * members when the group is created.
 * @param protect Should these group members protect each other.
 * @param defend Should these group members defend each other.
 * @param transient Should these group members be considered transient.
 * @param max_population How many of these NPCs can exist at a time.
 * @param storage A cache of the population of NPCs in this group.
 * @param no_creates The number of these NPCs we've created
 * @param next_update When the population data will next be updated.
 */
class group_info {
   string* move_zones;
   // This is a list of npcs with a name as the key and a quantity as the value.
   mapping npcs;
   mapping npc_commands;
   int protect;
   int defend;
   int transient;
   
   int max_population;

   // Tracking details.
   mixed* storage;
   int no_creates;
   int next_update;
}

// These are old defines from some old hospital which AFAIK isn't actually
// being used anymore. Nevertheless, retained for backwards compatibility.
/*
 * Defines for hospitals on Discworld
 */
#define MONSTER "/obj/monster"

// the max number of named living objects. This is used to keep the regen
// rate under control.
#define MAX_NAMED_LIVING 600

#define MAX_AM_LIVING 450
// Due to the way the hospital works in KLK this is the number of
// hospital NPCs other npcs aren't counted.
#define MAX_DJEL_LIVING 100

#endif // __STD_HOSPITAL_H
