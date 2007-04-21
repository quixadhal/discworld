/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: effects.c,v 1.3 2000/05/08 18:05:06 pinkfish Exp $
 * $Log: effects.c,v $
 * Revision 1.3  2000/05/08 18:05:06  pinkfish
 * Fix up the shadow to work correctly with the effects inheritable using
 * private variables.
 *
 * Revision 1.2  1999/04/21 11:47:21  wodan
 * fixed small bug in stats function
 *
 * Revision 1.1  1998/01/06 04:39:04  ceres
 * Initial revision
 * 
*/
#define EFF_OB_NAME 0
#define EFF_ARG 1

#define EEQ_INCTIME 0
#define EEQ_FUNC 1
#define EEQ_EFF_OB 2
#define EEQ_INTERVAL 3
#define EEQ_FLAGS 4

#define EFF_SIZE 2
#define EEQ_SIZE 5

inherit "/std/basic/effects";

private object shadowed;

void setup_shadow( object thing ) {
   shadow( thing, 1 );
   shadowed = thing;
} /* setup_shadow() */

void destruct_shadow( object thing ) {
   if ( thing == this_object() ) {
      destruct( this_object() );
   } else {
      thing->destruct_shadow( thing );
   }
} /* destruct_shadow() */

void set_effs( mixed *args ) {
  ::set_effs(args);
} /* set_effs() */

void set_eeq( mixed *args ) {
  ::set_eeq(args);
} /* set_eeq() */

mixed *stats() {
  mixed *tmp = (mixed *)shadowed->stats();

  if(!arrayp(tmp)) {
    tmp = ({});
  }
  return ::stats() + tmp; 
} /* stats() */
