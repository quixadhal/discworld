string mapping_to_string( mapping map );

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

varargs string extract(string str, int start, int end) {
  if (end)
    return str[start..end];
  return str[start..];
} /* extract() */
