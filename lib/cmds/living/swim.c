/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: swim.c,v 1.1 2001/03/09 01:51:21 pinkfish Exp $
 * $Log: swim.c,v $
 * Revision 1.1  2001/03/09 01:51:21  pinkfish
 * Initial revision
 *
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/position_base";
#include <position.h>

void create() {
   ::create();
   setup_position("swim", "around", SWIMMING);
} /* create() */
