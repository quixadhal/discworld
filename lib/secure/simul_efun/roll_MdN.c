/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: roll_MdN.c,v 1.2 1998/03/24 07:28:59 pinkfish Exp $
 * $Log: roll_MdN.c,v $
 * Revision 1.2  1998/03/24 07:28:59  pinkfish
 * Add some docs to make the simulefuns more usefully lookupable.
 *
 * Revision 1.1  1998/01/06 05:13:15  ceres
 * Initial revision
 * 
*/
/**
 * Cotains some simul_efuns.
 * @author Pinkfish
 */
/**
 * This method rolls a bunch of dice to get a nice weighted medium.
 * @param dice the number of dice to roll
 * @param sides the number of sides each dice has
 * @return the total of the rolls
 * @see efun::random()
 * @see shuffle()
 * @example
 * roll_MdN(3, 6);
 * roll_MdN(1, 20);
 */
int roll_MdN( int dice, int sides ) {
   int roll;

   if ( ( dice > 0 ) && ( sides > 0 ) ) {
      while ( dice ) {
         roll += 1 + random( sides );
         dice--;
      }
   }
   return roll;
} /* roll_MdN() */
