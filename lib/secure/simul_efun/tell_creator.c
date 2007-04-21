/*  -*- LPC -*-  
 * $Id: tell_creator.c,v 1.6 1999/05/12 12:02:32 turrican Exp $
 */
/**
 * This method writes a message to a creator, it's mainly meant 
 * for debug messages where sending a message to a specific 
 * creator is smarter than sending it to players, who will only get confused.
 *
 * @param cre the name or object to send the message to
 * @param fmt the format string
 * @param args the arguments to the sprintf
 */
varargs void tell_creator(mixed cre, string fmt, mixed args ...) {
  if (stringp(cre)) {
#if efun_defined(find_player)
    cre = efun::find_player(cre);
#else
    cre = "/obj/handlers/livings"->find_player(cre);
#endif
  }
  if (objectp(cre) && cre->query_creator()) {
    if (sizeof(args)) {
      tell_object(cre, sprintf(fmt, args ...));
    } else {
      tell_object(cre, fmt);
    }
  }
} /* tell_creator()*/

