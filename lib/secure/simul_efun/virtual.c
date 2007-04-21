/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: virtual.c,v 1.7 2002/11/14 05:10:37 presto Exp $
 */
#include "virtual.h"

/**
 * Cotains some simul_efuns.
 * @author Pinkfish
 */
/**
 * This method creates a new object.  This is overridden to handle the
 * virtual object stuff.
 * See the documentation on the efun clone_object for more information.
 *
 * @param str the object to clone
 * @return the newly cloned object
 */
object garbage_handler;
object clone_object(string str, mixed *args ...) {
  object ob;

  if (sizeof(args) == 0)
    ob = efun::clone_object(str);
  else
    ob = efun::clone_object(str, args ...);
  if (!ob && file_size(str) > 0) {
    ob = SERVER->create_virtual_object(str, 1);
  }
  if (ob)  {
    if(!garbage_handler)
      catch(garbage_handler = load_object("/obj/handlers/garbage"));
    if(garbage_handler)
      catch(garbage_handler->cloned(ob));
  }
  return ob;
} /* clone_object() */
