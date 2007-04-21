/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: mortar_pestle.c,v 1.2 2001/09/06 11:43:00 siel Exp $
 *
 *
 */

inherit "/obj/misc/grinder";

void setup() {
  set_name( "pestle" );
  set_short( "mortar and pestle" );
  set_long( "Roughly hewn from stone, the mortar's bowl has been "
      "smoothed from long use.  The stone pestle used for grinding "
      "herbs within the bowl is smooth as well and hangs from a "
      "leather thong tied through a ring in the side of the bowl.\n" );
  set_main_plural( "mortars and pestles" );
  add_adjective( ({ "mortar and", "rough", "stone", "smooth" }) );
  add_alias( ({ "mortar", "pestle", "bowl" }) );
  set_value( 800 );
  set_weight( 44 );
  set_max_grindable( 8 ); // eight stalks or ~ 1 lb. of stuff at once
  set_max_weight( 8 );    // It can hold what it can grind
}