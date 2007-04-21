/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: basic_trap.c,v 1.1 2000/06/08 01:48:53 pinkfish Exp $
 * $Log: basic_trap.c,v $
 * Revision 1.1  2000/06/08 01:48:53  pinkfish
 * Initial revision
 *
 * Revision 1.1  1998/01/06 04:40:23  ceres
 * Initial revision
 * 
*/
inherit "/std/effect_shadow";

// type == "lock", "unlock" or "pick"
// return 0 for ok, return 1 for failure
int trap_lock(string type) {
  if(type != "pick")
    return 0;

  tell_object(this_player(), "Trap triggered.\n");
  return 0;
}
