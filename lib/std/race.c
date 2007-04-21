/**
 * This is called off of the monster to set up a race and
 * profession...   Race should be first then profession..
 * @author Pinkfish
 * @started Sometime in 1991
 */

#include "race.h"

inherit "/std/basic/print_object";

private mapping _races;
private mapping _guilds;
private mapping _race_obs;
private mapping _guild_obs;

void create() {
  string tmp;
  
  _races = ([
      0                  : "/std/races/human", /* first the null race */
      "ape"              : "/std/races/ape",
      "arachnid"         : "/std/races/arachnid",
      "baboon"           : "/std/races/baboon",
      "basilisk"         : "/std/races/basilisk",
      "badger"           : "/std/races/badger",
      "bat"              : "/std/races/bat",
      "bear"             : "/std/races/bear",
      "beetle"           : "/std/races/beetle",
      "bird"             : "/std/races/bird",
      "blowfish"         : "/std/races/blowfish",
      "bufonid"          : "/std/races/bufonid",
      "cabbage"          : "/std/races/cabbage",
      "camel"            : "/std/races/camel",
      "cat"              : "/std/races/cat",
      "caterpillar"      : "/std/races/caterpillar",
      "chicken"          : "/std/races/chicken",
      "chimera"          : "/std/races/chimera",
      "cockroach"        : "/std/races/cockroach",
      "cow"              : "/std/races/cow",
      "crab"             : "/std/races/crab",
      "crocodile"        : "/std/races/crocodile",
      "deer"             : "/std/races/deer",
      "demon"            : "/std/races/demon",
      "dog"              : "/std/races/dog",
      "dryad"            : "/std/races/dryad",
      "duck"             : "/std/races/duck",
      "dwarf"            : "/std/races/dwarf",
      "duckling"         : "/std/races/duckling",
      "eagle"            : "/std/races/chicken",
      "elemental"        : "/std/races/elemental",
      "elephant"         : "/std/races/elephant",
      "elf"              : "/std/races/elf",
      "ferret"           : "/std/races/ferret",
      "fish"             : "/std/races/fish",
      "fox"              : "/std/races/fox",
      "frog"             : "/std/races/frog",
      "gargoyle"         : "/std/races/gargoyle",
      "giant"            : "/std/races/giant",
      "giant bat"        : "/std/races/giant_bat",
      "giant spider"     : "/std/races/giant_spider",
      "gnoll"            : "/std/races/gnoll",
      "gnome"            : "/std/races/gnome",
      "goat"             : "/std/races/goat",
      "golem"            : "/std/races/golem",
      "grflx"            : "/std/races/grflx",
      "guppy"            : "/std/races/guppy",
      "half elf"         : "/std/races/half_elf",
      "hare"             : "/std/races/hare",
      "hedgehog"         : "/std/races/hedgehog",
      "hengeyokai"       : "/std/races/hengeyokai",
      "horse"            : "/std/races/horse",
      "human"            : "/std/races/human",
      "igor"             : "/std/races/igor",
      "imp"              : "/std/races/imp",
      "insect"           : "/std/races/insect",
      "light"            : "/std/races/light",
      "lion"             : "/std/races/lion",
      "lobster"          : "/std/races/lobster",
      "luggage"          : "/std/races/luggage",
      "jellyfish"        : "/std/races/jellyfish",
      "mole"             : "/std/races/mole",
      "moose"            : "/std/races/moose",
      "mouse"            : "/std/races/mouse",
      "mustelidae"       : "/std/races/mustelidae",
      "newt"             : "/std/races/newt",
      "noble dragon"     : "/std/races/noble_dragon",
      "nymph"            : "/std/races/nymph",
      "peacock"          : "/std/races/peacock",
      "penguin"          : "/std/races/penguin",
      "panda"            : "/std/races/panda",
      "parrot"           : "/std/races/parrot",
      "pig"              : "/std/races/pig",
      "rabbit"           : "/std/races/rabbit",
      "raptor"           : "/std/races/raptor",
      "rat"              : "/std/races/rat",
      "rodent"           : "/std/races/rodent",
      "sargassum"        : "/std/races/sargassum",
      "satyr"            : "/std/races/satyr",
      "scorpion"         : "/std/races/scorpion",
      "sektarian demon"  : "/std/races/sektarian_demon",
      "shark"            : "/std/races/shark",
      "sprite"           : "/std/races/sprite",
      "sheep"            : "/std/races/sheep",
      "skunk"            : "/std/races/skunk",
      "slice human"      : "/std/races/slice_human",
      "snake"            : "/std/races/snake",
      "sphinx"           : "/std/races/sphinx",
      "spider"           : "/std/races/spider",
      "squirrel"         : "/std/races/squirrel",
      "stoat"            : "/std/races/stoat",
      "stone giant"      : "/std/races/stone_giant",
      "stone palm"       : "/std/races/stone_palm",
      "strange"          : "/std/races/strange",
      "swamp dragon"     : "/std/races/swamp_dragon",
      "toad"             : "/std/races/toad", 
      "tortoise"         : "/std/races/tortoise",
      "tree"             : "/std/races/tree",
      "troll"            : "/std/races/troll",
      "vulture"          : "/std/races/vulture",
      "unicorn"          : "/std/races/unicorn",
      "vampire"          : "/std/races/vampire",
      "vorpal bunny"     : "/std/races/vorpal_bunny",
      "weasel"           : "/std/races/weasel",
      "werewolf"         : "/std/races/werewolf",
      "wolf"             : "/std/races/wolf",
      "wombat"           : "/std/races/wombat",
      "wererat"          : "/std/races/wererat",
      "giant rat"        : "/std/races/giant_rat",
    ]);
    _guilds = ([
      0                  : "/std/guilds/standard",
      "fighter"          : "/std/guilds/warrior",
      "warrior"          : "/std/guilds/warrior",
      "cleric"           : "/std/guilds/priest",
      "priest"           : "/std/guilds/priest",
      "witch"            : "/std/guilds/witch",
      "wizard"           : "/std/guilds/wizard",
      "cat burglar"      : "/std/guilds/thief",
      "cut throat"       : "/std/guilds/thief",
      "thief"            : "/std/guilds/thief",
      "pickpocket"       : "/std/guilds/thief",
      "swashbuckler"     : "/std/guilds/thief",
      "monk"             : "/std/guilds/monk",
      "assassin"         : "/std/guilds/assassin",
    ]);

    _race_obs = ([ ]);
    // Cache the loaded race & guild objects.
    foreach(tmp in values(_races))
      _race_obs[tmp] = load_object(tmp);

    _guild_obs = ([ ]);
    foreach(tmp in values(_guilds))
      catch(_guild_obs[tmp] = load_object(tmp));

}

/**
 * This method checks to see if the specified race is valid or not.
 * @param race the race to check for validity
 * @return 1 if it exists, 0 if it does not
 */ 
int query_valid_race( string race ) {
    return !undefinedp(_races[race]);
} /* query_valid_race() */

/**
 * This method sets the level of the npc with the specified attributes.
 * @param lvl the level to set the npc at
 * @param race the race to set the npc with
 * @param guild the guild to set the npc with
 */
void set_level( int lvl, string race, string guild ) {
    object ob;
    string race_ob, guild_ob;

    if (_races[race]) {
        race_ob = _races[race];
    } else {
        race_ob = _races[0];
    }
    if (_guilds[guild]) {
        guild_ob = _guilds[guild];
    } else {
        guild_ob = _guilds[0];
    }
    ob = previous_object();
    ob->set_no_check(1);
    ob->set_race_ob(race_ob);
    /* Done in /global/guild-race
       race_ob->start_player(ob);
    */
    race_ob->set_level(ob, lvl);
    ob->set_guild_ob(guild_ob);
    /* Done in /global/guild-race
       guild_ob->start_player(ob);
    */
    guild_ob->set_level(ob, lvl, guild);
    ob->race_guild_commands();
} /* set_level() */

private int add_race( string name, mixed ob ) {
    if (_races[name]) {
        return 0;
    }
    _races[name] = ob;
    return 1;
} /* add_race() */

private int remove_race( string name ) {
    if (!_races[name]) {
        return 0;
    }
    _races[name] = 0;
    return 1;
} /* remove_race() */

/**
 * This method returns all the current races in the race object.  It is a
 * mapping with matches between the name of the race and race object.
 * @return the race mapping
 */ 
mapping query_races() {
    return copy(_races);
} /* query_racs() */

/**
 * This method returns the path associated with the specified race.
 * @param race_name the name of the race
 * @return the path of the race
 */
string query_race_path( string race_name ) {
    return _races[race_name];
} /* query_race_path() */

private int add_guild( string name, mixed ob ) {
    if (_guilds[name]) {
        return 0;
    }
    _guilds[name] = ob;
    return 1;
} /* add_guild() */

private int remove_guild( string name ) {
    if (!_guilds[name]) {
        return 0;
    }
    _guilds[name] = 0;
    return 1;
} /* remove_guild() */

/**
 * This method returns all the current guilds in the race object.  It is a
 * mapping with matches between the name of the guild and guild object.
 * @return the race mapping
 */ 
mapping query_guilds() {
    return copy(_guilds);
} /* query_guilds() */

/**
 * This method returns the path associated with the specified guild.
 * @param guild_name the name of the guild
 * @return the path of the guild
 */
string query_guild_path(string guild_name) {
    return _guilds[guild_name];
} /* query_guild_path() */

/**
 * This method is called from inside the npc to do the heartbeat on the
 * guild and race objects.
 * @param race the race to do a heart beat on
 * @param guild the guild to do a heart beat on
 * @param race_ob the object to use for the race
 * @param guild_ob the object to use for the guild
 */ 
void monster_heart_beat(string race,  string guild, mixed race_ob,
                        mixed guild_ob) {
  object tmp;
  
  if(stringp(race_ob)) {
    if(!_race_obs[race_ob])
      _race_obs[race_ob] = find_object(race_ob);
    tmp = _race_obs[race_ob];
  } else if(objectp(race_ob))
    tmp = race_ob;
  if(tmp)
    tmp->player_heart_beat( race, previous_object() );

  if(stringp(guild_ob)) {
    if(!_guild_obs)
      _guild_obs = ([ ]);
    if(!_guild_obs[guild_ob]) {
      _guild_obs[guild_ob] = find_object(guild_ob);
    }
    tmp = _guild_obs[guild_ob];
  } else if(objectp(guild_ob))
    tmp = guild_ob;

  if(tmp)
    tmp->player_heart_beat(guild, previous_object());
} /* monster_heart_beat() */
