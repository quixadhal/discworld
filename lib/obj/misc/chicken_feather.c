/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: chicken_feather.c,v 1.1 2001/09/27 11:33:39 siel Exp $
 *
 *
 */


inherit "/std/object";

void setup() {
 set_name("feather");
 set_short("chicken feather");
 set_main_plural("chicken feathers");
 add_adjective("chicken");
 add_plural("feathers");
 add_adjective("chicken");
 set_weight(1);
 set_long("This is a feather that was plucked from a chicken.\n");
}

