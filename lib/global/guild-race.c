/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: guild-race.c,v 1.12 2003/03/06 13:26:39 terano Exp $
 * 
 */
/**
 * This file contains all the information relating to guilds and races
 * that is  stored on the player.
 *
 * @author Pinkfish
 */
#include <player.h>
#include <command.h>

string race_ob, guild_ob,  *known_commands;
mixed *guild_data;

nosave string _race;

/**
 * This method adds all the guild and race commands to the player
 * when they start up.
 * @see add_known_command()
 */
void race_guild_commands() {
  string command;
  string *bing;
  string *paths = ({ });

  if (!known_commands) {
    known_commands = ({ "skills", "rearrange", "gp", "newbie" });
  }

  foreach (command in known_commands) {
    bing = CMD_D->GetPaths(command);
    if (sizeof(bing)) {
      // Remove any paths that aren't /cmds/guild-race coz this was causing
      // trouble.
      bing = filter(bing, (: $1[0..15] == "/cmds/guild-race" :));
      paths += bing;
    }
  }
  this_object()->AddSearchPath(paths);
  if (guild_ob) {
    catch(guild_ob->start_player(this_object()));
  }

  catch(race_ob->start_player(this_object()));
  this_object()->query_limbs();
} /* race_guild_commands() */

/**
 * This method returns the title associated with this player for their
 * guild.
 * @return the guilds title
 */
string query_gtitle() {
  string tmp;
  
  if (guild_ob)
    if (catch((tmp = (string)guild_ob->query_title(this_object()))))
        tmp = "Has a broken guild";
  return tmp;
} /* query_gtitle() */

/**
 * This method sets the players current race.  This should never be
 * used, all players should be humans.
 * the set functions should not be called indisciminately...
 * Be careful when using them, so that you don't screw up a character ok?
 * @param str the players new race
 * @see query_race_ob()
 */
void set_race_ob(string str) {
  string frog;
  if(sscanf(str, "/%s", frog)==1) 
    str = extract(str, 1);
  if (str[0..strlen("std/races")-1]!="std/races") {
    write("Illegal path to set_race_ob.\n");
    return;
  }
  if(str[0..strlen("std/races/god")-1] == "std/races/god" &&
     !"/secure/master"->god(geteuid())) {
    write("The heavens rumble and the earth shakes.  You stop.\n");
  }
  else
    race_ob = str;
} /* set_race_ob() */

/**
 * This method returns ther current race object associated with the
 * player.
 * @return the race object
 * @see set_race_ob()
 */
string query_race_ob() { return race_ob; }

/**
 * This method sets the current guild associated with the player.
 * Be very careful when setting this variable, if you get it wrong
 * bunches of things on the mud will mysteriously break.
 * @param str the new guild object
 * @see query_guild_ob()
 */
void set_guild_ob( string str ) {
  if(str) {
    if(str[0..10] != "/std/guilds") {
      tell_object(this_object(), "Attempt to set invalid guild object " +
                  str + "\n");
      return;
    } else if(!find_object(str)) {
      tell_object(this_object(), "No such object "+ str +"\n");
      return;
    }
  }
  guild_ob = str;
}

/**
 * This method returns the current guild object associated with the
 * player.
 * @return the current guild object
 * @see set_guild_ob()
 */
 string query_guild_ob() {
   if ( !guild_ob ) {
      return 0;
   }
   if(guild_ob[0..10] != "/std/guilds")
     return 0;
   
   return guild_ob;
} /* query_guild_ob() */

/**
 * This method sets the data associated with the guild for the player.
 * This is used for stuff like the orders for wizards and the type of
 * specialisation for theives.
 * @param dat the data to associate with the guild
 * @see query_guild_data()
 */
void set_guild_data(mixed *dat) { guild_data = dat; }
/**
 * This method returns the current guild data for the player.
 * @see set_guild_data()
 * @return the current guild data
 */
mixed *query_guild_data() {
  return guild_data;
} /* query_guild_data() */

/**
 * This method returns the name of the race associated with the
 * player.
 * @return the race name
 * @see query_race_ob()
 * @see set_race_ob()
 */
string query_race() { /* should return the name of our race */
  if(!_race)
    _race = (string)race_ob->query_name();
  return _race;
} /* query_race() */

/**
 * This method handles help calls associated with commands on the
 * player.  This will either return a string or a function pointer.
 * @return the help for the command
 * @param str the command to get help on
 * @see add_known_command()
 */
string help_command(string str) {
  class command cmd = new(class command, verb : str);
  string path;
  mixed func;

  if ((member_array(str, known_commands) != -1 ||
       this_object()->query_creator()) &&
      CMD_D->HandleStars(cmd)) {
    path =  CMD_D->GetPaths(cmd->verb)[0] + "/" + cmd->verb;
    func = path->help_function();
    if (func) {
       return func;
    }
    return path->help();
  }
  return 0;
} /* help_command() */

/**
 * This method returns the array of commands the player knows.
 * @return the list of commands the player knows
 * @see add_known_command()
 * @see query_known_command()
 * @see remove_known_command()
 */
string *query_known_commands() { return copy(known_commands); }

/**
 * This method checks to see if the command is known by the player.
 * @param word the command to check
 * @return 1 if known, 0 if not
 * @see add_known_command()
 * @see query_known_commands()
 * @see remove_known_command()
 */
int query_known_command( string word ) {
   return ( member_array( word, known_commands ) != -1 );
} /* query_known_command() */

/**
 * This method adds a command to the player.
 * @param str the command to add
 * @return 1 if was successfuly added, 0 if not
 * @see query_known_commands()
 * @see query_known_command()
 * @see remove_known_command()
 */
int add_known_command(string str) {
  string *paths;

  if (member_array(str, known_commands) != -1) {
    return 0;
  }
  if (!CMD_D->IsGRCommand(str)) {
    return 0;
  }
  paths = CMD_D->GetPaths(str);
  //
  // Filter out the /w directories.
  //
  paths = filter(paths, (: $1[0..15] == "/cmds/guild-race" :));
  if (sizeof(paths)) {
    this_object()->AddSearchPath(paths);
  }
  known_commands += ({ str });

  //Something from Terano to assist in debugging what's up with covert commands.
  if ( interactive( this_object() ) ) 
    log_file( "ADD_KNOWN_COMMAND", "%s: %s was added to %s by: %O.\n", ctime( time() ), 
        str, this_player()->query_name(), map( previous_object( -1 ), (: ({ $1,  $1->query_name() }) :) ) );
  
  return 1;
} /* add_known_command() */

/**
 * This method removes a command from the player.
 * @param str the command to remove
 * @return 1 if it was successful, 0 if it was unsuccessful
 * @see add_known_command()
 * @see query_known_command()
 * @see query_known_commands()
 */
int remove_known_command(string str) {
  int i;

  i = member_array(str, known_commands);
  if (i == -1) {
    return 0;
  }
  known_commands = known_commands[0..i-1] + known_commands[i+1..];
  return 1;
} /* remove_known_command() */
