/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: ta_il.c,v 1.4 1999/10/21 01:26:45 shrike Exp $
 * $Log: ta_il.c,v $
 * Revision 1.4  1999/10/21 01:26:45  shrike
 * switched over to using file_length() to avoid errors on too-large files.
 *
 * Revision 1.3  1999/10/12 22:39:53  shrike
 * *sob* it's right this time, honest! :P
 *
 * Revision 1.1  1998/01/06 05:10:49  ceres
 * Initial revision
 * 
*/
/* tail command, trial out by Turrican for a commands daemon. */

#include <creator.h>

inherit "/cmds/base";

mixed cmd( string str, int num ) {
   string *filenames;
   int loop;
   object *obs;
  
   if (num < 0)  {
      num = -num;
   }
   if (num == 0)  {
      num = 20;
   }

   if ( !str ) {
      notify_fail( "Tail what file(s)/object(s)?\n" );
      return 0;
   }
   if ( !sizeof( filenames = this_player()->get_files( str ) ) ) {
      if ( sizeof( obs = WIZ_PRESENT->wiz_present( str, this_player() ) ) ) {
         filenames = map( obs, (: base_name($1) :) );
         filenames = map( filenames,
           (: ( file_size($1) <= 0 ? $1 + ".c" : $1 ) :) );
      } else
         return notify_fail( str + ": No such file(s)/object(s).\n" );
   }
   
   foreach( str in filenames )
      if( ( file_size( str ) == -2 ) || ( str[<2..<1] == ".." ) )
         filenames -= ({ str });

   for( loop = 0; loop < sizeof( filenames ); loop++ ) {
      str = filenames[loop];
      if( sizeof( filenames ) > 1 ) {
         printf( "%*'-'s\nFILE : %s\n", 80, "", str );
      }
      if ( file_size( str ) < 0)
         write( "That file/object is not readable, or does not exist.\n" );
      else {
         write( read_file( str, ( ( file_length( str ) + 1 ) - num ) ) );
      }   
   }
   return 1;
}

mixed *query_patterns()  {
   return ({ "<number> <string>", (: cmd($4[1], to_int( $4[0] ) ) :) ,
             "<string>", (: cmd($4[0], 20) :) });
}
