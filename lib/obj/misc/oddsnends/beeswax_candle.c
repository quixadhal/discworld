/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: beeswax_candle.c,v 1.1 2002/03/26 01:47:04 belle Exp $
 *
 *
 */

inherit "/std/lightable";

void setup() {
   set_name( "candle" );
   set_short( "beeswax candle" );
   add_adjective( "beeswax" );
   set_long( "This is a small candle, like you'd "
         "see on a birthday cake.  It is quite soft and "
         "squidgy.\n" );
   set_fuel_messages( ({
         "It is almost burnt to a stub.", 10,
         "It is more than halfway burnt.", 50,
         "It is not yet halfway burnt.", 85,
	 "It is almost new.", 99,
         "It is new.", 100
         }) );
   set_empty_mess( "is burnt to a stub." );
   set_max_fuel( 900 );
   set_fuel( 900 );
   set_brightness( 60 );
   set_value( 24 );
   set_hold_required( 1 );
   add_property( "no recycling", 1 );
   new_weapon( 10 );
   set_weight( 1 );
   set_hide_cond( 1 );
}
