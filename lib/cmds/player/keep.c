/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: keep.c,v 1.5 2003/03/03 01:47:24 ceres Exp $
 * $Log: keep.c,v $
 * Revision 1.5  2003/03/03 01:47:24  ceres
 * Fixed so it'll load
 *
 * Revision 1.4  2003/03/02 22:21:26  pinkfish
 * Allow yuou to list what you are keeping.
 *
 * Revision 1.3  2000/01/19 01:59:36  presto
 * removed all those nasty unused function arguments
 *
 * Revision 1.2  1999/10/27 01:48:31  pinkfish
 * Fix them up to use a nice new effect instead of a horrible shadow.
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
#define EFFECT "/std/effects/object/identity_hold"

inherit "/cmds/base";

int cmd( object *obs )  {
   object thing;

   foreach(thing in obs) {
      thing->add_effect( EFFECT, ([ "keep" : 1 ]) );
   }
   write( "You will now attempt to keep "+
         (string)query_multiple_short(obs)+".\n" );

   return 1;
} /* cmd() */

int cmd_list() {
   object* obs;

   // Find all the objects we are keeping.
   obs = all_inventory(this_player());
   obs = filter(obs, (: $1->query_keep() :) );
   if (!sizeof(obs)) {
      add_failed_mess("You are not keeping anything.\n");
      return 0;
   }
   write("You are keeping " + query_multiple_short(obs) + ".\n");
   return 1;
}

mixed *query_patterns() {
   return ({ "<indirect:object:me>", (: cmd( $1 ) :),
             "", (: cmd_list :) });
} /* query_patterns() */
