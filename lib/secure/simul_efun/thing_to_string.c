/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: thing_to_string.c,v 1.2 1998/03/24 07:28:59 pinkfish Exp $
 * $Log: thing_to_string.c,v $
 * Revision 1.2  1998/03/24 07:28:59  pinkfish
 * Add some docs to make the simulefuns more usefully lookupable.
 *
 * Revision 1.1  1998/01/06 05:13:15  ceres
 * Initial revision
 * 
*/
string mapping_to_string( mapping map );

/**
 * Cotains some simul_efuns.
 * @author Pinkfish
 */
/**
 * This method converts and input array into a string.
 * @param args the array to convert
 * @return the string value of the array
 * @see mapping_to_string()
 */
string array_to_string( mixed *args ) {
   int i;

   args = copy(args);
   for ( i = sizeof( args ) - 1; i > -1; i-- ) {
      if ( pointerp( args[ i ] ) ) {
         args[ i ] = array_to_string( args[ i ] );
         continue;
      }
      if ( mapp( args[ i ] ) ) {
         args[ i ] = mapping_to_string( args[ i ] );
         continue;
      }
      args[ i ] = sprintf( "%O", args[ i ] );
   } 
   return "({ "+ implode( args, ", " ) +" })";
} /* array_to_string() */

/**
 * This method converts and input mapping into a string.
 * @param args the mapping to convert
 * @return the string value of the mapping
 * @see array_to_string()
 */
string mapping_to_string( mapping map ) {
   int i;
   mixed *args;

   if( !mapp( map ) ) { 
      /* If this breaks badly, blame the idiot calling this function
       * with a functionpointer instead of a mapping as argument!
       * Olorin
       */
      return sprintf( "%O", map );
   }
   args = keys( map );
   for ( i = sizeof( args ) - 1; i > -1; i-- ) {
      if ( pointerp( map[ args[ i ] ] ) ) {
         args[ i ] = sprintf( "%O : %s", args[ i ],
               array_to_string( map[ args[ i ] ] ) );
         continue;
      }
      if ( mapp( map[ args[ i ] ] ) ) {
         args[ i ] = sprintf( "%O : %s", args[ i ],
               mapping_to_string( map[ args[ i ] ] ) );
         continue;
      }
      args[ i ] = sprintf( "%O : %O", args[ i ], map[ args[ i ] ] );
   }
   return "([ "+ implode( args, ", " ) +" ])";
} /* mapping_to_string() */
