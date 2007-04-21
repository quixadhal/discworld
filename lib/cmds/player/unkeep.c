/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: unkeep.c,v 1.4 2001/10/02 01:53:23 presto Exp $
 * $Log: unkeep.c,v $
 * Revision 1.4  2001/10/02 01:53:23  presto
 * Fixed runtime when unkeeping
 *
 * Revision 1.3  2000/01/19 01:54:22  presto
 * Was only allowing you to unkeep one thing at a time.
 * Fixed up a bunch of other ugliness as well.
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

int cmd( object *obs ) {
   object thing, *success;

   success = ({ });
   foreach (thing in obs)  {
      if (thing->query_keep())  {
         thing->reset_keep();
         success += ({ thing });
      }
   }
   if (!sizeof(success))  {
      write("You are not attempting to keep anything.\n");
   }
   else  {
      write("You will no longer attempt to keep " +
            query_multiple_short(success) + ".\n");
   }
   return 1;
} /* cmd() */

mixed *query_patterns() {
   return ({ "<indirect:object:me>", (: cmd( $1 ) :) });
} /* query_patterns() */
