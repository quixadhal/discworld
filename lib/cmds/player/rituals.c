/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: rituals.c,v 1.1 1998/01/06 05:29:43 ceres Exp $
 * $Log: rituals.c,v $
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
#include <spells.h>

inherit "/cmds/base";

int cmd( string words ) {
   string spell, *religious;
   mapping spells;
   spells = this_player()->query_spells();
   if ( !sizeof( spells ) ) {
      write( "You don't know any religious rituals.\n" );
      return 1;
   }
   religious = ({ });
   foreach ( spell in keys( spells ) ) {
      if ( sscanf( spells[ spell ][ 0 ], "%*spriest%*s" ) == 2 ) {
         religious += ({ spell });
         continue;
      }
      if ( sscanf( spells[ spell ][ 0 ], "%*srituals%*s" ) == 2 ) {
         religious += ({ spell });
         continue;
      }
      if ( sscanf( spells[ spell ][ 0 ], "%*swizard%*s" ) == 2 )
         continue;
      if ( sscanf( spells[ spell ][ 0 ], "%*switch%*s" ) == 2 )
         continue;
      if ( sscanf( spells[ spell ][ 0 ], "%*sspell%*s" ) == 2 )
         continue;
   }
   if ( !sizeof( religious ) ) {
      write( "You don't know any religious rituals.\n" );
      return 1;
   }
   if ( sizeof( religious ) ) {
      write( "You know the following religious ritual"+
            ( sizeof( religious ) == 1 ? "" : "s" ) +":\n" );
      printf( "%#-*s\n", (int)this_player()->query_cols(),
            implode( sort_array( religious, 1 ), "\n" ) );
   }
   return 1;
} /* cmd() */
