/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: womble_brooch.c,v 1.2 2000/06/27 17:33:59 ceres Exp $
 *
 * $Log: womble_brooch.c,v $
 * Revision 1.2  2000/06/27 17:33:59  ceres
 * Modified to have the correct jewellery info
 *
 * Revision 1.1  2000/03/01 06:06:45  ceres
 * Initial revision
 *
 * Revision 1.2  1998/10/17 17:37:30  aragorn
 * Made the brooch compatible with the jewlery blorpler spell.
 *
 * Revision 1.1  1998/01/16 02:18:32  arby
 * Initial revision
 *
*/
/* Coded by Talonia for newbie area womble quest.
   Nov. '95
*/

#include "path.h"
inherit "/obj/armour";

void setup()
{
  set_name("brooch");
  add_adjective( ({ "womble", "brass" }) );
  set_short("womble brooch");
  set_long("A brass brooch with a womble engraved "
    "upon it.\n");
  add_alias("jewellery");
  add_plural("jewellery");
  set_type("badge");
  setup_armour(1000);
  set_percentage(100);
  set_damage_chance(0);
  set_weight(2);
  add_property("wombles brooch", 1);
  add_property( "shop type", "jewellers" );
}
