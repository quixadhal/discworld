/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: cure_wand.c,v 1.1 1998/01/06 04:58:17 ceres Exp $
 * $Log: cure_wand.c,v $
 * Revision 1.1  1998/01/06 04:58:17  ceres
 * Initial revision
 * 
*/
/* -*- LPC -*-
 */
inherit "/std/effects/healing/basic_cure";
inherit "/obj/wand";

void create() {
   do_setup++;
   wand::create();
   basic_cure::create();
   do_setup--;
   if ( do_setup )
      this_object()->setup();
} /* create() */
