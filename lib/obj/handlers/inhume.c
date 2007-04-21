/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: inhume.c,v 3.16 2001/04/28 09:49:45 shrike Exp $
 *
*/
/*
 * Note: This is no longer the inhume handler.  I have moved the handler
 * to /obj/handlers/secure_handlers, which is a secure directory, To
 * prevent cheating creators from being able to share information with
 * their player friends.  If you believe you need access to the handler,
 * please speak to me or to your lord, who will be able to grant you access
 * to query the handler for info.
 *
 * --
 * Shrike
 */
#include <inhume.h>

void create() {
   string * prev;
   string msg;

   prev = map( previous_object(-1), (: file_name($1) :) );
   msg = sprintf( "%s: /obj/handlers/inhume loaded: previous_object(-1) = %O",
     ctime( time() ), prev );
   tell_creator( "shrike", msg );
   log_file( LOG, msg );
} /* create() */