/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: find_match.c,v 1.24 2001/02/28 01:29:05 presto Exp $
 * $Log: find_match.c,v $
 * Revision 1.24  2001/02/28 01:29:05  presto
 * Put in different version of is_in_me_or_environment()
 *
 * Revision 1.23  2000/09/07 21:13:24  pinkfish
 * Fix the docs.
 *
 * Revision 1.22  2000/09/07 21:11:30  pinkfish
 * Update the documentation.
 *
 * Revision 1.21  2000/06/28 03:49:11  pinkfish
 * Stop it runtiming.
 *
 * Revision 1.20  2000/06/26 19:55:02  pinkfish
 * Turn on the new parser for everyone.
 *
 * Revision 1.19  2000/06/24 03:48:44  presto
 * Line 163, was using ob[i] when ob was not an array.
 *
 * Revision 1.18  2000/06/23 03:40:56  pinkfish
 * Fix up the references to find_match.
 *
 * Revision 1.17  2000/06/10 08:20:34  pinkfish
 * Make pts able to use the system as well.
 *
 * Revision 1.16  2000/06/07 23:30:02  pinkfish
 * Fix up the return codes of the parser.
 *
 * Revision 1.15  2000/05/31 21:40:26  pinkfish
 * Some changes to make it work more correctly.
 *
 * Revision 1.14  2000/05/31 00:16:10  pinkfish
 * Add in some stuff to do matching of ambiguous things too.
 *
 * Revision 1.13  2000/05/26 18:48:37  pinkfish
 * Add in hooks to the new parser.
 *
 * Revision 1.12  2000/04/28 01:19:03  pinkfish
 * Stop people using "all" in dark/bright rooms.
 *
 * Revision 1.11  2000/04/28 01:13:14  pinkfish
 * Fix up some things to do with <cc> in <xx>
 *
 * Revision 1.10  1999/12/07 23:10:22  pinkfish
 * Fix up an error that was causing 0's to get stuck in the return array.
 *
 * Revision 1.9  1999/12/07 23:02:05  pinkfish
 * Fixes and changes.
 *
 * Revision 1.8  1998/09/30 14:38:39  wodan
 * added prototype for add_action free drivers.
 *
 * Revision 1.7  1998/09/01 21:19:46  pinkfish
 * Speed up the find_match code by using a function pointer for the
 * map methods.
 *
 * Revision 1.6  1998/09/01 21:04:28  pinkfish
 * Clean up the code slightly, plus make the syntaxes:
 * 'my book' and 'book here' work.
 *
 * Revision 1.5  1998/07/28 01:36:15  pinkfish
 * Stop '0' evaluating to everything.
 *
 * Revision 1.4  1998/03/24 07:28:59  pinkfish
 * Add some docs to make the simulefuns more usefully lookupable.
 *
 * Revision 1.3  1998/03/07 13:49:40  pinkfish
 * Fixes because of the new inherit structor of the simul_efun object.
 *
 * Revision 1.2  1998/03/06 05:31:47  pinkfish
 * Handle expanded inventory stuff...
 *
 * Revision 1.1  1998/01/06 05:13:15  ceres
 * Initial revision
 * 
 */
#define OBJ_PARSER_NO_CLASSES
#include <obj_parser.h>
inherit "/secure/simul_efun/obj_parser";

#include <playtesters.h>

private nosave mixed *rest;

mixed *query_strange_inventory(mixed *arr);
private object query_simul_efun_id(object ob, mixed *arr);
private object query_frac_simul_efun_id(object ob, mixed *arr);

#if !efun_defined(living)
int living(object);
#endif

/* If anyone can tell me what this does...
 * I would be most apprecative, Pinkfish... Yes yes ok i did write it.
 */
/**
 * The find_match simul_efun.
 * @author Pinkfish
 */

/**
 * This method checks to see if the specified object is in the player
 * or the environment.  If it has no environment it is considered to
 * be here.
 * @param ob the object to test
 * @param player the player to check against
 * @return 1 if it is, 0 if not
 */

int is_in_me_or_environment(object thing, object person)  {
   object env;

   if ((env = environment(thing)) == environment(person))
      return 1;

   // This is a special case if 'thing' is a room.
   // Needed for the 'get' command, and others (I assume)
   if (!env)
      return 1;

   while (env  &&  !living(env))
      env = environment(env);

   if (env == person)
      return 1;

   return 0;
}

/**
 * This method returns the array of objects that are only inside the
 * player in question, or in their environment.  It excludes any items
 * in other peoples inventorys.
 * @param obs the objects to filer
 * @param player the player to check
 * @return the filtered objects
 */
object* filter_in_me_or_environment(object* obs, object player) {
   return filter(obs, (: is_in_me_or_environment($1, $2) :), player);
} /* filter_in_me_or_environment() */
