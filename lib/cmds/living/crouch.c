/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: crouch.c,v 1.1 1998/01/06 05:28:43 ceres Exp $
 * $Log: crouch.c,v $
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/position_base";
#include <position.h>

void create() {
   ::create();
   setup_position("crouch", "down", CROUCHING);
} /* create() */
