inherit "/std/basic/print_object";
/*
 * This is called off of the monster to set up a race and
 * proffesion...  Race should be first then proffesion..
 */
#include "race.h"

mapping races,
        guilds;

void create() {
  races = ([
    0          : "/std/races/unknown", /* first the null race */
    "troll"    : "/std/races/troll",
    "chicken"  : "/std/races/chicken",
    "human"    : "/std/races/human",
    "elf"      : "/std/races/elf",
    "dwarf"    : "/std/races/dwarf",
    "fish"     : "/std/races/fish",
    "guppy"    : "/std/races/guppy",
    "penguin"  : "/std/races/penguin",
    "half elf" : "/std/races/half_elf",
    "half orc" : "/std/races/half_orc",
  ]);
  guilds = ([
    0              : "/std/guilds/standard",
    "fighter"      : "/std/guilds/fighter",
    "cleric"       : "/std/guilds/priest",
    "priest"       : "/std/guilds/priest",
    "witch"        : "/std/guilds/witch",
    "wizard"       : "/std/guilds/wizard",
    "thief"        : "/std/guilds/thief",
    "cat burglar"  : "/std/guilds/thief",
    "cut throat"   : "/std/guilds/thief",
    "pickpocket"   : "/std/guilds/thief",
    "swashbuckler" : "/std/guilds/thief",
  ]);
} /* create() */

int query_valid_race(string race) {
  return races[race];
} /* query_valid_race() */

void set_level(int lvl, string race, string class) {
  object ob;
  string race_ob, guild_ob;
 
  if (races[race])
    race_ob = races[race];
  else
    race_ob = races[0];
  if (guilds[class])
     guild_ob = guilds[class];
  else
    guild_ob = guilds[0];
  ob = previous_object();
  ob->set_no_check(1);
  ob->set_race_ob(race_ob);
  race_ob->start_player(ob);
  race_ob->set_level(ob, lvl);
  ob->set_guild_ob(guild_ob);
  guild_ob->start_player(ob);
  guild_ob->set_level(ob, lvl, class);
} /* set_level() */

int add_race(string name, mixed ob) {
  if (races[name]) return 0;
  races[name] = ob;
  return 1;
} /* add_race() */

int remove_race(string name) {
  if (!races[name]) return 0;
  races[name] = 0;
  return 1;
} /* remove_race() */

mapping query_races() { return races; }

int add_guild(string name, mixed ob) {
  if (guilds[name]) return 0;
  guilds[name] = ob;
  return 1;
} /* add_guild() */

int remove_guild(string name) {
  if (!guilds[name]) return 0;
  guilds[name] = 0;
  return 1;
} /* remove_guild() */

mapping query_guilds() { return guilds; }

void monster_heart_beat(string race, string class, mixed race_ob, mixed class_ob) {
  race_ob->player_heart_beat(race);
  class_ob->player_heart_beat(class);
} /* monster_heart_beat() */
