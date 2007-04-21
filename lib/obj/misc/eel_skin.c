/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: eel_skin.c,v 1.1 2001/09/27 11:43:37 siel Exp $
 *
 *
 */

inherit "/std/object";

void setup()
{
  set_name( "skin" );
  set_short( "eel skin" );
  add_adjective( "eel" );
  set_main_plural( "eel skins" );
  set_long( "This is the skin of an eel.  It is very greasy and "
        "glistens prettily when you tilt it towards the light.\n" );
  set_weight( 3 );
  set_value( 100 );
}
