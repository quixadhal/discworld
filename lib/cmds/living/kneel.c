/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: kneel.c,v 1.1 1998/01/06 05:28:43 ceres Exp $
 * $Log: kneel.c,v $
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/position_base";
#include <position.h>

void create() {
   ::create();
   setup_position("kneel", "down", KNEELING);
} /* create() */
