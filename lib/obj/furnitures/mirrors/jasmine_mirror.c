/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: jasmine_mirror.c,v 1.1 2002/03/02 04:56:14 saffra Exp $
 *
 *
 */

// Rue 1.2000

inherit "/std/room/furniture/mirror";

string reflect();

void setup(){

  set_name( "mirror" );
  set_short( "jasmine mirror" );
  add_adjective( ({ "jasmine", "flower", "oval" }) );
  set_long("Delicately carved jasmine flowers and bead trim surround this "
           "oval mirror.  The flowers are so detailed that they almost "
           "appear to be real.\n");

  set_allowed_room_verbs( (["hanging" : "hangs", 
                            "gracing"  : "graces"   ]) );
  set_weight(60);
  set_value(50000);

}
