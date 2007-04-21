/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: guild_base.c,v 1.6 2001/05/14 03:07:42 presto Exp $
 *
 * $Log: guild_base.c,v $
 * Revision 1.6  2001/05/14 03:07:42  presto
 * Use /include/cmds/guild_base.h
 *
 * Revision 1.5  1998/08/23 07:30:34  pinkfish
 * Fix up the basse with stuff.!
 *
 * Revision 1.4  1998/08/22 01:26:24  pinkfish
 * First go at makeing a base teaching guild command object.
 *
 * Revision 1.2  1998/03/26 10:37:45  pinkfish
 * Fix it up so the web version of the help file looks better..
 *
 * Revision 1.1  1998/03/13 16:46:53  pinkfish
 * Initial revision
 *
 */

/**
 * This is the base object for all the guild/race commands.  It handles
 * all the common bits of guild/race commands, including teachability
 * and help files.
 * @author Pinkfish
 */
inherit "/cmds/base";

#include <nroff.h>
#include <cmds/guild_base.h>

#define NROFF_SOURCE "/doc/known_command/"

/*
 * This is the base for guild commands.  Handles the
 * nroff file stuff.
 */
nosave string _nroff_fname;
nosave class teach_skill *_teach_skills;
nosave string _command_name;
nosave string _teach_guild;

void create() {
   _teach_skills = ({ });
} /* create() */

/**
 * Set the name of the nroff help file.
 * @param str the nroff help file name
 */
void set_nroff_file(string str) {
   _nroff_fname = str;
} /* set_nroff_file() */

/**
 * This method returns the name of the nroff help file associated with this
 * command.
 * @return the nroff help file name
 */
string query_nroff_file() {
  return _nroff_fname;
} /* query_nroff_file() */

/*
 * Makea string from a nroff input...
 */
private string nroff_file(string name, int html) {
   string nroff_fn;
   string str;

   if (name[0] != '/') {
      name = NROFF_SOURCE + name; 
   }
   nroff_fn = NROFF_DIR + replace(name, "/", ".");
   if (html) {
      str = NROFF_HAND->html_file(nroff_fn, 1);
   } else {
      str = NROFF_HAND->cat_file(nroff_fn, 1);
   }
   if (!str) {
      NROFF_HAND->create_nroff(name, nroff_fn);
      if (html) {
         str = NROFF_HAND->html_file(nroff_fn, 0);
      } else {
         str = NROFF_HAND->cat_file(nroff_fn, 0);
      }
   }
   return str;
} /* nroff_file() */

/**
 * This method returns the help data as a function pointer.
 * The function pointer needs to be evaluated to create the actual text.
 * @return the function pointer for the help text
 */ 
function help_function() {
   if (_nroff_fname) {
      return (: nroff_file(_nroff_fname, 0) :);
   }
   return 0;
} /* help_function() */

/**
 * This method returns the help as a string.
 * @return the help as a string
 */
string help() {
   if (_nroff_fname) {
      return nroff_file(_nroff_fname, 0);
   }
   return 0;
} /* help() */

/**
 * This method returns the help in a html format for use with the
 * web.
 * @return a html string
 */
string query_www_help() {
   if (_nroff_fname) {
      return nroff_file(_nroff_fname, 1);
   }
   return 0;
} /* help() */

/**
 * This method sets the teach level for the command.  The player must
 * be over this level in the teaching skill to be able to teach this
 * command to the player.  If this is set to 0, then teaching is inhibited.
 * @param level the new teaching level
 */
void add_teach_skill(string skill, int teach, int learn) {
   class teach_skill bing;

   bing = new(class teach_skill);
   bing->skill = skill;
   bing->teach = teach;
   bing->learn = learn;
   _teach_skills += ({ bing });
} /* set_teach_level() */

/**
 * This method returns the current teach skill level.
 * @return the current teach skill level
 */
class teach_skill *query_teach_skills() {
   return _teach_skills;
} /* query_teach_skills() */

/**
 * This method sets the name of the command.  This is used in the teaching
 * to teach the command to the student.
 * @param name the new command name
 */
void set_command_name(string name) {
   _command_name = name;
} /* set_command_name() */

/**
 * This method returns the current command name of the command.
 * @return the current command name
 */
string query_command_name() {
   return _command_name;
} /* query_command_name() */

/**
 * This method sets the command to be only able to be taught to a specific
 * guild.  If this is set to 0 then the command can be taught to
 * any guild, this is the default.
 * @param guild the guild object it can be taught to
 */
void set_teach_guild(string guild) {
   _teach_guild = guild;
} /* set_teach_guild() */

/**
 * This method returns the currently set specific guild.  If this is 0 then
 * the command can be taught to any guild.
 * @return the current teach guild
 */
string query_teach_guild() {
   return _teach_guild;
} /* query_teach_guild() */

/**
 * This method checks to see if the command can be taught or not.
 * @param teacher the person doing the teaching
 * @param student the person being taught
 * @return 0 on failure, 1 on success, -1 if teacher is too low level, -2
 * if student is too low level
 */
int can_teach_command(object teacher, object student) {
   class teach_skill bing;

   if (!sizeof(_teach_skills) ||
       !_command_name) {
     return 0;
   }

   foreach (bing in _teach_skills) {
      if (teacher->query_skill(bing->skill) < bing->teach) {
         return -1;
      }

      if (student->query_skill(bing->skill) < bing->learn) {
         return -2;
      }
   }

   if (_teach_guild &&
       student->query_guild_ob() != _teach_guild) {
      return -3;
   }

   return 1;
} /* can_teach_command() */

/**
 * This method actually teaches the command to the student.
 * @param teacher the teacher of the command
 * @param student the student of the command
 * @return 0 on failure, 1 on success, -1 if teacher is too low level, -2
 * if student is too low level
 */
int teach_command(object teacher, object student) {
   int ret;

   ret = can_teach_command(teacher, student);
   if (ret == 1) {
      student->add_known_command(_command_name);
   }
   return ret;
} /* teach_command() */
