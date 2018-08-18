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

void alt_move(mixed dest, object ob){
  if(!ob)
    return;
  evaluate(bind((:move_object, dest:), ob));
}

/*
 * $Locker:  $
 * $Id: extract.c,v 1.2 1998/03/24 07:28:59 pinkfish Exp $
 * $Log: extract.c,v $
 * Revision 1.2  1998/03/24 07:28:59  pinkfish
 * Add some docs to make the simulefuns more usefully lookupable.
 *
 * Revision 1.1  1998/01/06 05:13:15  ceres
 * Initial revision
 * 
*/
varargs string extract(string str, int start, int end) {
  if (end)
    return str[start..end];
  return str[start..];
} /* extract() */

/*
 * Works like explode with REVERSIBLE_EXPLODE_STRING, that is:
 * implode(explode(str, del), del)==str
 */
string* rexplode(string str, string del)
{
#ifdef __REVERSIBLE_EXPLODE_STRING__
   return explode(str, del);
#elif defined(__SANE_EXPLODE_STRING__)
   if(!strlen(str))
      return ({""});
   return explode(del+str+del, del);
#else
   string *leading = ({ });
   string *trailing = ({ });
   int del_length = 0;

   //error("Define REVERSIBLE_EXPLODE_STRING or SANE_EXPLODE_STRING");
   if(!strlen(str))
      return ({ });
   if(!strlen(del))
       return explode(str, del);

   del_length = strlen(del);

   while(strlen(str) >= del_length && str[0..(del_length-1)] == del) {
       // We found a leading delimiter!
       leading = leading + ({ "" });
       str = str[(del_length)..];
   }
   while(strlen(str) >= del_length && str[<(del_length)..<1] == del) {
       // We also have a trailing delimiter.
       trailing = trailing + ({ "" });
       str = str[0..<(del_length+1)];
   }
   return leading + explode(str, del) + trailing;
#endif
}

/*
 * Works like explode with SANE_EXPLODE_STRING, that is like
 * reversible_explode_string but a single delimiter is removed
 * from the both start and end of the output array if present.
 */
string* sexplode(string str, string del)
{
#ifdef __REVERSIBLE_EXPLODE_STRING__
   mixed m;
   if(!strlen(str))
      return ({});
   m=explode(str, del);
   if(m[<1]=="")
      m=m[0..<2];
   if(!strlen(str))
      return ({});
   if(m[0]=="")
      m=m[1..<1];
   return m;
#elif defined(__SANE_EXPLODE_STRING__)
   return explode(str, del);
#else
   error("Define REVERSIBLE_EXPLODE_STRING or SANE_EXPLODE_STRING");
#endif
}

