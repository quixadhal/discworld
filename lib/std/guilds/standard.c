/*   -*- LPC -*-   */
/*
 * $Locker:  $
 * $Id: standard.c,v 1.10 2003/04/15 17:01:47 ceres Exp $
 */
#include <skills.h>
#include <tune.h>
#include <guilds.h>
#include <config.h>

inherit "/std/object";

nosave string gp_skill;
nosave string start_pos;
nosave mixed *commands;
nosave string *skills;
nosave mixed *spells;

protected void create() {
   gp_skill = "other.points";
   start_pos = CONFIG_START_LOCATION;
   skills = ({ "crafts", "faith", "fighting", "other" });
   commands = ({ });
   spells = ({ });
   ::create();
}

string query_gp_skill() { return gp_skill; }

void set_gp_skill( string word ) { gp_skill = word; }

void set_gp( object thing ) {
   if ( !thing ) {
      return;
   }
   thing->set_max_gp( 50 + (int)thing->query_skill_bonus( gp_skill ) );
} /* set_gp() */

string query_start_pos( object thing ) { return start_pos; }

void set_start_pos( string word ) { start_pos = word; }

string *query_skills() { return skills; }

void set_skills( string *words ) { skills = words; }

int add_command(string name, int add_if_player) {
   if (member_array(name, commands) != -1) {
      return 0;
   }
   commands += ({ name, ({ add_if_player }) });
   return 1;
}

int remove_command(string name) {
   int i;

   if ((i = member_array(name, commands)) == -1) {
      return 0;
   }
   commands = delete(commands, i, 1);
   return 1;
}

int do_command(string name, mixed param) {
   log_file("BAD_COMMANDS", "do_command() called on /std/guilds/standard.c\n");
   return 0;
}

int add_spell(string name, mixed ob, string func_name, int add_if_player) {
   int i;

   if ((i = member_array(name, spells)) != -1) {
      return 0;
   }
   spells += ({ name, ({ ob, func_name, add_if_player }), });
   return 1;
}

int remove_spell(string name) {
   int i;

   if ((i= member_array(name, spells)) == -1) {
      return 0;
   }
   spells = delete(spells, i, 2);
   return 1;
}

int cast_spell(string name, mixed bing) {
   int i;

   if ((i=member_array(name, spells)) == -1) {
      return 0;
   }
   return (int)call_other(spells[i+1][0], spells[i+1][1], bing);
}

mixed *query_spells() { return spells; }

string *query_commands() { return commands; }

/*
 * The next two functions should be redefined on any guild item you write...
 */
int query_skill_cost( string skill ) { return 10; }

int query_skill_max_level( string skill ) { return 10; }

/**
 * This is where you would set up any shadows you want on the player
 * object, or just do what you will..
 * @param pl the player being started
 */
void start_player(object pl) {
   int i;

   if (!spells) {
      spells = ({ });
   }
   for (i=0;i<sizeof(spells);i+=2) {
      if (spells[i+1][2] || !interactive(pl)) {
         pl->add_spell(spells[i], spells[i+1][0], spells[i+1][1]);
      }
   }
   if (!pointerp(commands)) {
      commands = ({ });
   }
   for (i=0;i<sizeof(commands);i+=2) {
      if (commands[i+1][0] || !interactive(pl)) {
         pl->add_known_command(commands[i]);
      }
   }
}

void leave_guild() { return; }

void player_quit() { return; }

void player_save() { return; }

void player_heart_beat( string guild, object thing ) {
/* This is already done in the /obj/monster and /global/player heart beats.
   int *rates;
   rates = this_player()->query_inc_rates();
   thing->adjust_gp( rates[ 0 ] );
   thing->adjust_hp( rates[ 1 ] );
*/
} /* player_heart_beat() */

void set_level( object thing, int level, string guild ) {
   call_out( "set_new_level", 1, ({ thing, level, guild }) );
} /* set_level() */

string adjust_primaries( object thing, int level ) {
  int i;
  string stat_list;
  
  stat_list = "";
  for ( i = 0; i < sizeof( skills ); i++ ) {
    if(level - thing->query_skill(skills[i]) > 0)
      thing->add_skill_level(skills[i], level - thing->query_skill(skills[i]));
    
    stat_list += (string)SKILL_OB->query_skill_stat(skills[i]);
    if(random(2)) {
      stat_list += "C";
    }
  }
  return stat_list;
}

void set_new_level( mixed *args ) {
   int i, j, level;
   string stat_list;
   object thing;

   thing = args[ 0 ];
   if ( !thing ) {
      return;
   }
   level = args[ 1 ];

   thing->add_skill_level("fighting", level / 4 );
   thing->add_skill_level("other", level / 4 );
   switch(thing->query_guild_ob()) {
   case "/std/guilds/warrior":
   case "/std/guilds/assassin":
     thing->add_skill_level("other.health", level);
     break;
   case "/std/guilds/thief":
     thing->add_skill_level("other.health", (level * 3) / 4);
     break;
   default:
     thing->add_skill_level("other.health", level / 2);
   }
   thing->add_skill_level("other.perception", level / 3);

   
   stat_list = adjust_primaries( thing, level );
   
   
   // Unique NPCs, non-sentient npcs or thise with "keep setup stats"
   // don't require custom stat arrangements.
   if(thing->query_property( "unique" ) ||
      thing->query_property( "keep setup stats" ) ||
      (!thing->query_race_ob() ||
       !(thing->query_race_ob())->query_humanoid()))
     return;
   
   thing->adjust_con( -6 + random( 3 ) );
   thing->adjust_dex( -6 + random( 3 ) );
   thing->adjust_int( -6 + random( 3 ) );
   thing->adjust_str( -6 + random( 3 ) );
   thing->adjust_wis( -6 + random( 3 ) );
   for ( i = 0; i < 25; i++ ) {
      j = random( strlen( stat_list ) );
      switch ( stat_list[ j .. j ] ) {
         case "C" :
            thing->adjust_con( 1 );
            break;
         case "D" :
            thing->adjust_dex( 1 );
            break;
         case "I" :
            thing->adjust_int( 1 );
            break;
         case "S" :
            thing->adjust_str( 1 );
            break;
         default :
            thing->adjust_wis( 1 );
            break;
      }
   }
} /* set_new_level() */

/**
 * This method figures out the level of the npc or player.
 * @param thing the thing to find the level of
 * @return the level of the thing
 */
int query_level( object thing ) {
   int i, lvl;

   if ( !thing || !sizeof( skills ) ) {
      return 0;
   }
   for ( i = 0; i < sizeof( skills ); i++ ) {
      lvl += (int)thing->query_skill( skills[ i ] );
   }
   lvl /= sizeof( skills );
   return lvl;
} /* query_level() */

/**
 * This method returns the title of the player.
 * @param player the player to find the title of
 * @return the title name
 */
string query_title( object player ) { return "the Adventurer"; }

/**
 * This method is called whenever a player advances a skill.
 * @param thing the player advacing the skill
 * @param skill the name of the skill
 * @param level the level it is advanced to
 */
void skills_advanced( object thing, string skill, int level ) {
   if ( strsrch( skill, "covert" ) != -1 ) {
      if ( ( (int)thing->query_skill( "covert.stealth" ) > 5 ) &&
             !thing->query_known_command( "peek" ) ) {
         tell_object( thing, "You realise that you could use your newly "
                      "improved stealth to \"peek\" covertly at someone, and "
                      "maybe they won't notice...\n" );
         thing->add_known_command( "peek" );
      }
   } else if ( strsrch( skill, "fighting.combat.parry.held" ) != -1 ) {
      if ( ( (int)thing->query_skill( "fighting.combat.parry.held" ) > 50 ) &&
                  !thing->query_known_command( "riposte" ) ) {
         tell_object( thing, "You realise that you could use your newly "
                     "improved parry to \"riposte\" someones attack.\n" );
         thing->add_known_command( "riposte" );
      }
   }
} /* skills_advanced() */

/**
 * Dummy function for guilds without positions
 * This method adds a new position for the subguild.
 * @param sub_guild the sub guild
 * @param position position to add
 * @param max the maximum number of people appointed to this position
 * @param abilities the abilities associated with this position
 * @param description the descriptions associated with this position
 * ({ title|({male_title,female_title}), ... })
 * @return DEITY_POSITION_OK if successful, DEITY_POSITION_FAIL if the
 * position already exists
 * @see remove_position()
 * @see query_position()
 * @see query_positions()
 */
int add_position( string sub_guild, string position, 
                           int max, string* abilities, mixed* description ) {
    return POSITION_FAIL;
} /* add_position() */

/**
 * Dummy function for guilds without positions
 * This method removes a position for this subguild.
 * @param sub_guild the sub guild
 * @param position position to remove
 * @return POSITION_OK if successful, POSITION_FAIL if the
 * position didn't exist
 * @see add_position()
 * @see query_position()
 * @see query_positions()
 */
int remove_position( string sub_guild, string position ) {
    return POSITION_FAIL;
} /* remove_position() */

/**
 * Dummy function for guilds without positions
 * This method returns all positions for this guild.
 * @param sub_guild the sub guild
 * @return 0 if the position don't exist, an array of appointments
 * if is does, with 0's for unfilled positions
 * @see add_position()
 */
mapping query_positions( string sub_guild, string position ) {
    return 0;
} /* query_positions() */

/**
 * Dummy function for guilds without positions
 * This method returns the appointments for this position in this 
 * subguild.
 * @param sub_guild the sub guild
 * @param position position to return
 * @return 0 if the position don't exist, an array of appointments
 * if is does, with 0's for unfilled positions
 * @see add_position()
 */
string *query_position_appointments( string sub_guild, string position ) {
    return 0;
} /* query_position_appointments() */

/**
 * Dummy function for guilds without positions
 * This method returns the description array for this position.
 * @param sub_guild the sub guild
 * @param position position to return
 * @return 0 if the position don't exist, an array of descriptions if 
 * it does, currently defined to hold the title
 * ({ title|({male_title,female_title}), ... })
 * @see add_position()
 */
mixed *query_position_description( string sub_guild, string position ) {
    return 0;
} /* query_position_description() */

/**
 * Dummy function for guilds without positions
 * This method returns the abilities for a position.
 * @param sub_guild the sub guild
 * @param position position to return
 * @return 0 if the position don't exist, an array of abilities
 * if is does
 * @see add_position()
 */
string *query_position_abilities( string sub_guild, string position ) {
    return 0;
} /* query_position_abilities() */

/**
 * Dummy function for guilds without positions
 * This method removes all appointments for a guild.   
 * @param sub_guild the sub guild
 * @see add_position()
 */
void reset_positions( string sub_guild ) {
} /* reset_position() */

/**
 * Dummy function for guilds without positions
 * This method appoints the player to a position.   If sub_guild is 0,
 * the guild object is supposed to figure out the subguild from the 
 * player_name.
 * @param sub_guild the sub guild
 * @param position the position
 * @param player_name the player
 * @return POSITION_FAIL if it failed, POSITION_OK if it succeeded.
 */
int appoint_position( string sub_guild, string position,
                                 string player_name ) {
    return POSITION_FAIL;
} /* appoint_position() */

/**
 * Dummy function for guilds without positions
 * This method dismisses the player from a position.   If sub_guild is 0,
 * the guild object is supposed to figure out the subguild from the 
 * player_name.
 * @param sub_guild the sub guild
 * @param position the position
 * @param player_name the player
 * @return POSITION_FAIL if it failed, POSITION_OK if it succeeded.
 */
int dismiss_position( string sub_guild, string position,
                                 string player_name ) {
    return POSITION_FAIL;
} /* dismiss_position() */

/**
 * Dummy function for guilds without positions
 * This method returns the position held by the player. If sub_guild is
 * 0, the guild object is supposed to figure out what sub_guild (if any)
 * is relevant.
 * @param sub_guild the sub guild
 * @param player_name the player
 * @return the position the player is appointed to in sub_guild.
 */
string query_position( string sub_guild, string player_name ) {
    return 0;
} /* query_position() */

/**
 * Dummy function for guilds without positions
 * Test if a player/npc has a specific ability.
 * @param thing either a string, being the name of a player, or an object
 * being the actual player/npc to check for a guild ability.
 * @param ability the ability to check for.
 * @return 1 if thing has that ability, 0 if not.
 */
int query_guild_ability( mixed thing, string ability ) {
    return 0;
} /* query_guild_ability() */

