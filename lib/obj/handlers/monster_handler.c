/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: monster_handler.c,v 1.3 1999/06/22 01:53:32 ceres Exp $
 * $Log: monster_handler.c,v $
 * Revision 1.3  1999/06/22 01:53:32  ceres
 * No idea why this thing is counting, but now it saves its count less frequently
 *
 * Revision 1.2  1998/05/10 14:52:34  pinkfish
 * Added in documentation.
 *
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
/**
 * This handler is used for the (now obsolette) monster action code.
 * Please see the effects code instead.
 * @author Pinkfish
 */

int num;

void create() {
  seteuid("Room");
  unguarded((: restore_object, "/save/monster-handler" :));
} /* create() */

/** @ignore yes */
string query_shadow_name() {
  num++;
  return "/tmp/mon-shad/frog-"+num;
} /* query_shadow_name() */

void reset() {
  unguarded((: save_object, "/save/monster-handler" :));
}

void dest_me() {
  unguarded((: save_object, "/save/monster-handler" :));
}
