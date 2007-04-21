/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: sit.c,v 1.2 2002/02/22 01:15:04 ceres Exp $
 * $Log: sit.c,v $
 * Revision 1.2  2002/02/22 01:15:04  ceres
 * Fixed typo
 *
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/position_base";
#include <position.h>

void create() {
   ::create();
   if(this_player()->query_position() == "lying")
     setup_position("sit", "up", SITTING);
   else
     setup_position("sit", "down", SITTING);
} /* create() */
