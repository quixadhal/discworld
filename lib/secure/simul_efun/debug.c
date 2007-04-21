/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: debug.c,v 1.5 2003/04/09 22:27:43 ceres Exp $
 * $Log: debug.c,v $
 * Revision 1.5  2003/04/09 22:27:43  ceres
 * Modified to not print debug messages to /room/rubbish since that's useless.
 *
 * Revision 1.4  2001/09/03 06:44:04  ceres
 * Modified to accept arrays for cres
 *
 * Revision 1.3  2001/06/01 21:16:40  ceres
 * Can't remmebr
 *
 * Revision 1.2  2000/05/22 20:33:31  ceres
 * Minor tweaks
 *
 * Revision 1.1  2000/04/16 20:10:23  wodan
 * Initial revision
 *
*/

/**
 * Debugging simul efuns.
 *
 * @author Ceres
 */

/**
 * This method will generate a debug inform for any creators in the room
 * where it is generated.  It will always attempt to identify the room
 * that the event should be sent to.
 *
 * @example
 * #ifdef DEBUG
 *   debug_printf("Error, player is: %O\n", player);
 * #endif
 *
 * @param all printf style parameters
 * @see printf
 */
void debug_printf(string fmt, mixed args ...) {
  object env;

  env = previous_object();
  if(env)
    while(environment(env))
      env = environment(env);

  if((!env || !env->query_is_room() || base_name(env) == "/room/rubbish") &&
     this_player()) {
    env = environment(this_player());
  }
  
  if(env) {
#if efun_defined(event)
    event(env, "inform", sprintf("%O:\n " + fmt, previous_object(), args ...),
          "debug");
#else
    this_object()->event(env, "inform", sprintf("%O:\n " + fmt,
                                                previous_object(), args ...),
                         "debug");
#endif
  }
}

/**
 * This method writes a message to a creator, it's mainly meant 
 * for debug messages where sending a message to a specific 
 * creator is smarter than sending it to players, who will only get confused.
 *
 * Typically you should use debug_printf() instead of this.
 *
 * @param cre the name or object to send the message to
 * @param fmt the format string
 * @param args the arguments to the sprintf
 *
 * @see debug_printf
 */
varargs void tell_creator(mixed cres, string fmt, mixed args ...)  {
  mixed cre;

  if(!arrayp(cres))
    cres = ({ cres });

  foreach(cre in cres) {
    if (stringp(cre)) {
#if efun_defined(find_player)
      cre = efun::find_player(cre);
#else
      cre = "/obj/handlers/livings"->find_player(cre);
#endif
    }

    if (objectp(cre) && cre->query_creator()) {
      if (sizeof(args)) {
        tell_object(cre, sprintf("%O:\n" + fmt, previous_object(), args ...));
      } else {
        tell_object(cre, sprintf("%O:\n"+fmt, previous_object()));
      }
    }
  }
}
