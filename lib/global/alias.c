/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: alias.c,v 1.12 2001/06/16 05:01:27 presto Exp $
 * $Log: alias.c,v $
 * Revision 1.12  2001/06/16 05:01:27  presto
 * Put in a missing set of braces in query_player_alias
 *
 * Revision 1.11  2001/06/16 00:15:35  ceres
 * Fixed possible runtime in query_player_alias
 *
 * Revision 1.10  2001/03/11 01:42:01  ceres
 * Restricted alias lenghts
 *
 * Revision 1.9  1999/04/03 23:19:40  dragonkin
 * Fixed so that player aliases called "" can be removed by anyone.
 *
 * Revision 1.8  1999/03/10 09:44:56  pinkfish
 * Rip out the guts into the alias commands.
 *
 * Revision 1.7  1999/03/09 06:04:38  presto
 *  Forcibly unlocked by pinkfish
 *
 * Revision 1.6  1999/02/08 14:15:04  wodan
 * changed add_actions to other things (/cmds stuff and add_command)
 *
 * Revision 1.5  1999/01/27 00:03:46  ceres
 *  Forcibly unlocked by pinkfish
 *
 * Revision 1.4  1998/12/30 21:45:13  ceres
 * Fixed code for checking for colour sequences so it works properly.
 *
 * Revision 1.3  1998/12/30 21:42:27  olorin
 *  Forcibly unlocked by ceres
 *
 * Revision 1.2  1998/02/21 19:05:47  pinkfish
 * Fix up the problem with the command queue and aliases.
 *
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 * 
*/
/**
 * The alias control module for players.
 * @author Pinkfish
 */
#include <alias.h>
#include <player.h>
inherit "/global/history";

mapping aliases;
nosave mapping doing_alias;

protected void exec_alias(string verb, string args);
protected string *expand_alias(string verb, string args);
protected void set_doing_alias(string verb);
protected int is_doing_alias(string verb);
protected string *run_alias(string verb, string args);
int remove_alias_thing(string);
protected int alias(string);
protected int edit_alias(string);

/**
 * This method sets the alias to the new value.
 * @param name the name of the alias
 * @param value the value to set the alias too
 * @see query_player_alias()
 */
int add_player_alias(string name, mixed *value) {
   if (file_name(previous_object()) != ALIAS_CMD &&
       file_name(previous_object()) != EDIT_ALIAS_CMD) {
      return 0;
   }

   if(name == "unalias" || name == "alias" || name == "ealias")
     return 0;
  
   aliases[name] = value[0..1023];
   return 1;
} /* add_player_alias() */

/**
 * This method returns the value of the specified alias.
 * @param name the name of the alias to query
 * @return the value of the alias
 * @see add_player_alias()
 */
mixed *query_player_alias(string name) {
  if(!mapp(aliases))  {
    aliases = ([ ]);
    return 0;
  }
    
   return copy(aliases[name]);
} /* query_player_alias() */

/**
 * This method will remove the alias from the player.
 * @param name the name of the alias
 * @see query_player_alias()
 * @see add_player_alias()
 */
int remove_player_alias(string name) {
   if ((file_name(previous_object()) != UNALIAS_CMD) &&
       name != "" &&
       !this_player(1)->query_lord()) {
      printf("%O\n", file_name(previous_object()));
      return 0;
   }
   map_delete(aliases, name);
   return 1;
} /* remove_player_alias() */

/**
 * This method adds all the alias commands onto the player.  The commands
 * are 'alias', 'unalias', 'ealias', 'END_ALIAS' and the "*" pattern
 * expand the alias.
 */
void alias_commands() {
   add_command("END_ALIAS", "<string>", (:remove_alias_thing($4[0]):));
} /* alias_commands() */

/**
 * This method zaps all the current aliases defined.  This can only be
 * called by a high lord.
 * @return 0 on failure and 1 on success
 */
int remove_all_aliases() {
   if (!this_player(1)->query_lord()) {
      write("You can't do that :)\n");
      return 0;
   }

   aliases = ([ ]);
   return 1;
} /* remove_all_aliases() */


/**
 * This method returns a complete list of all the aliases
 * defined on the player.  This is the internal mapping so it will
 * be quite unreadable.
 * @see print_aliases()
 * @see /include/alias.h
 * @return the mapping of aliases
 */
mapping query_aliases() { 
   return copy(aliases); 
} /* query_aliases() */

/**
 * This method tells us if the given name is an alias.
 * @param verb the verb to check
 * @return 1 if it is an alias, 0 if not
 */
int is_alias(string verb) {
   return aliases[verb] != 0;
} /* is_alias() */

/**
 * This method runs the alias and executes all the commands in the
 * alias.  You should probably use run_alias() instead.  This calls
 * set_doing_alias() to setup blocking.
 * @param verb the name of the alias
 * @param args the arguments associated with the alias
 * @see run_alias()
 * @see set_doing_alias()
 */
protected void exec_alias(string verb, string args) {
   string *bing;
   string comm;

   bing = run_alias(verb, args);
   if (bing) {
      set_doing_alias(verb);
      foreach (comm in bing) {
         command(comm);
      }
   }
} /* exec_alias() */

/**
 * Attempt to expand the alias.    This will look up the alias and
 * see if it is defined.  If it is, it will attempt to expand the 
 * alias. This does not call set_doing_alias().  This will
 * return 0 if the alias does not exist, or the alias is already
 * being run.
 * @return the array if the alias was expanded, 0 if failed
 * @param verb the name of the alias to expand
 * @param args the arguments to the alias
 * @see exec_alias()
 * @see is_doing_alias()
 * @see set_doing_alias()
 * @see expand_alias()
 */
protected string *run_alias(string verb, string args) {
   if (!mapp(aliases)) {
      aliases = ([ ]);
   }
   if (!aliases[verb] || is_doing_alias(verb)) {
      return 0;
   }
   return expand_alias(verb, args);
} /* run_alias() */

/**
 * This method checks to see if the player is doing the specified alias
 * already.
 * @param verb the verb to check
 * @see exec_alias()
 * @see set_doing_alias()
 */
protected int is_doing_alias(string verb) {
   if (!doing_alias) {
      doing_alias = ([ ]);
   }
   if (doing_alias[verb]) {
      return 1;
   }
   return 0;
} /* is_doing_alias() */

/**
 * This method sets us as currently doing the given alias.
 * @param verb the alias to do
 * @see is_doing_alias()
 */
protected void set_doing_alias(string verb) {
  doing_alias[verb] = 1;
  in_alias_command++;
} /* set_doing_alias() */

/**
 * This method expands the alias from the input string thingy.
 * @param verb the verb to expand
 * @param args the arguments to the verb
 * @see run_alias()
 */
protected string *expand_alias(string verb, string args) {
   int i;
   int num;
   string *bits;
   string line;
   mixed stuff;
   string *ret;
   
   /* Default expansion :) */
   if (!aliases[verb]) {
      return ({ verb + " " + args });
   }
   ret = ({ });
   stuff = aliases[verb];
   line = "";
   if (!args) {
      args = "";
   }
   bits = explode(verb + " " + args, " ");
   for (i = 0; i < sizeof(stuff); i++) {
      if (stringp(stuff[i])) {
         line += stuff[i];
      } else {
         num = stuff[i] & ALIAS_MASK;
         switch (stuff[i] - num) {
         case NEW_LINE :
            ret += ({ line });
            line = "";
            break;
         case ALL_ARGS :
            line += args;
            break;
         case ONE_ARG  :
            if (num < sizeof(bits)) {
               line += bits[num];
            }
            break;
         case TO_ARG   :
            line += implode(bits[1..num], " ");
            break;
         case FROM_ARG :
            line += implode(bits[num..100], " ");
            break;
         case ALL_ARG  :
            i++;
            if (args == "") {
               line += stuff[i];
            } else {
               line += args;
            }
            break;
         case ARG_THING :
            i++;
            if (num < sizeof(bits)) {
               line += bits[num];
            } else {
               line += stuff[i];
            }
            break;
         case ALL_IFARG :
            i++;
            if (args == "") {
               i += stuff[i];
            }
            break;
         case IFARG_THING :
            i++;
            if (num >= sizeof(bits)) {
               i += stuff[i];
            }
            break;
         case ELSE_THING :
            i++;
            i += stuff[i];
            break;
         case CURR_LOC :
            i++;
            line += file_name(environment())+".c";
            break;
         case END_IF :
            break;
         }
      }
   }
   if (line != "") {
      ret += ({ line });
   }
   ret += ({ "END_ALIAS " + verb });
   return ret[0..1023];
} /* expand_alias() */

/**
 * This is called to signify the end of an alias.  This is needed
 * keep track of recursive aliases and such like.
 * @param verb the verb name to remove
 * @return always returns 1
 * @see set_doing_alias()
 * @see is_doing_alias()
 */
int remove_alias_thing(string verb) {
   in_alias_command--;
   map_delete(doing_alias, verb);
   this_player()->adjust_time_left(-DEFAULT_TIME);
   return 1;
} /* remove_alias_thing() */
