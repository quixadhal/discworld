/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: base.c,v 1.4 2001/04/24 23:16:49 ceres Exp $
 * $Log: base.c,v $
 * Revision 1.4  2001/04/24 23:16:49  ceres
 * Trying to fix a runtime
 *
 * Revision 1.3  1999/12/17 02:15:05  pinkfish
 * Remove the is in stuff from here and makeit a simul_efun.
 *
 * Revision 1.2  1999/12/16 23:04:57  pinkfish
 * Add in some code to check for being in the player or the environment.
 *
 * Revision 1.1  1998/01/06 05:24:54  ceres
 * Initial revision
 * 
*/
/**
 * This is basic inherit module for inheriting into commands.
 * Please inherit this into all command objects.
 * <p>
 * It sets up all the functions needed to do useful stuff with a 
 * command object.
 * @author Pinkfish
 */

void create() {
  seteuid(getuid(this_object()));
} /* create() */

/** @ignore yes */
void dest_me() {
  if(this_object())
    destruct(this_object());
} /* dest_me() */

/** @ignore yes */
int clean_up() {
  dest_me();
  return 1;
} /* clean_up() */

/** @ignore yes */
void reset() {
  dest_me();
} /* reset() */
