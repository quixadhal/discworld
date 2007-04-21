/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: command.c,v 1.5 2001/03/07 03:30:21 ceres Exp $
 * 
*/
/**
 * Handles commands from directories for living objects.
 *    /global/command.c
 *    from the Nightmare IVr1 Object Library
 *    handles commands of living objects
 *    created by Descartes of Borg 950323
 *    Hacked for Discworld by Turrican 4-11-95
 */

#include <command.h>

nosave string current_verb, current_cmd;
private nosave int Forced;
private nosave string *SearchPath;

/*  ***************  /lib/command.c driver applies  ***************  */

protected void create() {
   current_verb = "";
  SearchPath = ({ DIR_LIVING_CMDS });
} /* create() */

/** What is the current verb? */
string query_current_verb() { return current_verb; }
/** What are the args to the current verb? */
string query_current_cmd() { return current_cmd; }

/*  ***************  /lib/command.c command lfuns  ***************  */

/**
 * Try and execute the command.  It calls the fuinction command_shadowed
 * with the verb and args as parameters if the command is found to exist.
 * @param args the args to use for the verb
 * @return 1 on success, 0 on failure
 */
protected int cmdAll(string args) {
  class command verb;
  mixed tmp;

  verb = new(class command);
  verb->args = args;
  current_cmd = args;
  
  if( !((int)CMD_D->GetCommand(verb, SearchPath)) ) {
    return 0;
  }

/*
 * This allows other code to find out what verb has been used,
 * since query_verb() will always return "" (verb is *).
*/
   current_verb = verb->verb;

  if (CMD_D->IsGRCommand(verb->filepart) &&
      !this_object()->query_known_command(verb->verb))
    return 0;
  if (this_object()->command_shadowed(verb->verb, verb->args))
    return 1;
  if( !(tmp = (mixed)call_other(verb->file, "cmd", verb->args, verb->verb)) )
    return 0;
  else if( stringp(tmp) ) {
    write("Error: "+tmp+"\n");
    return 1;
  }
  else return 1;
} /* cmdAll() */

/**
 * This method tries to lookup the patterns for the given verb.
 * @param verb the verb to get the patterns for
 * @return the array of patterns
 * @see /obj/handlers/pattern_compiler.c
 * @see /gloal/new_parse->add_command()
 */
protected mixed *cmdPatterns(string verb) {
  if (CMD_D->IsGRCommand(verb) &&
      !this_object()->query_known_command(verb))
    return 0;
  return CMD_D->GetCommandPatterns(verb, SearchPath);
} /* cmdPatterns() */

/*  ***************  /lib/command.c lfuns  ***************  */

/**
 * Add in the command catch all action.
 */
protected void command_commands() {
#if efun_defined(add_action)
  add_action( "cmdAll", "*", -1 );
#endif
} /* command_commands() */

/**
 * A force event.
 * @param cmd the command to force
 * @return 0 on failure and 1 on success
 */
int eventForce(string cmd) {
  string err;
  int res;
  
  if(member_array(call_stack(0)[1], 
                  ({  "/cmds/creator/show_help.c", 
                      "/d/am/broad_way/cryer_order.c",
                      "/obj/handlers/new_soul.c", 
                      "/std/living/force.c"
                        })) == -1)
    error("illegal force");
  Forced = 1;
  err = catch(res = command(cmd));
  Forced = 0;
  if(err) error(err);
  return res;
} /* eventForce() */

/*  **********  /lib/command.c data manipulation functions  ********** */

/**
 * Add a directory(s) into the search path for commands.  If the input is
 * an array then the members of that are added to the search path.
 * @param val the path to array
 * @return the new search path
 */
string *AddSearchPath(mixed val) {
  if (stringp(val)) {
     val = ({ val });
  } else if(!pointerp(val)) {
     error("Bad argument 1 to AddSearchPath()\n");
  }
  SearchPath = uniq_array(SearchPath + val);
  return SearchPath;
} /* AddSearchPath() */

/**
 * Remove a directory(s) from the search path.  If the input is
 * an array then the members of that are added to the search path.
 * @param val the directory(s) to remove
 * @return the new search path
 */
string *RemoveSearchPath(mixed val) {
  if(stringp(val)) {
    val = ({ val });
  } else if(!pointerp(val)) {
    error("Bad argument 1 to RemoveSearchPath()\n");
  }
  SearchPath -= val;
  return SearchPath;
} /* RemoveSearchPath() */

/**
 * This method returns the current search path
 * @return the current search path
 */
string *GetSearchPath() { return SearchPath; }

/**
 * This method returns if we are being forced or not
 * @return 1 if we are being forced
 */
int GetForced() { return Forced; }

/**
 * This tells us if we are a client.
 * @return always returns 0
 */
string GetClient() { return 0; }
