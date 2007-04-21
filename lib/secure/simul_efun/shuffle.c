/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: shuffle.c,v 1.2 1998/03/24 07:28:59 pinkfish Exp $
 * $Log: shuffle.c,v $
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
 * This method randomizes an array.  It places all the elements of an
 * array back in random order.
 * @param args the array to shuffle
 * @return the randomized array
 * @see roll_MdN()
 * @see efun::random()
 */
mixed *shuffle( mixed *args ) {
   int i, j;
   mixed temp;
   if ( !pointerp( args ) )
      return ({ });
   if ( sizeof( args ) < 2 )
      return args;
   for ( i = 0; i < sizeof( args ); i++ ) {
      j = random( i + 1 );
      if ( i == j )
         continue;
      temp = args[ i ];
      args[ i ] = args[ j ];
      args[ j ] = temp;
   }
   return args;
} /* shuffle() */
