/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: start.c,v 1.4 2002/07/16 23:20:53 pinkfish Exp $
 * $Log: start.c,v $
 * Revision 1.4  2002/07/16 23:20:53  pinkfish
 * Make it work with extra start locations.
 *
 * Revision 1.3  2001/03/04 05:07:55  presto
 * Fixed typo chose --> choose
 *
 * Revision 1.2  1999/05/17 21:47:04  pinkfish
 * Fix up an error.
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";

int clear_starts() {
  write( "Clearing your list of starting positions...\n" );
  this_player()->reset_starts();
  return 1;
} /* clear_starts() */

int list_starts() {
   int z, i;
   string *starts;

   starts = this_player()->query_starts();
   z = ( sizeof( starts ) - 2 ) / 2;
   if ( z == -1 ) {
      /* No starting positions... */
      write( "Your starting position is " +
             this_player()->query_start_pos()->the_short() + ".\n");
      return 1;
   }
   if ( !z ) {
      /* Only one starting position */
      write( "Your starting position is "+ starts[ 1 ] +".\n" );
      return 1;
   }
   /* Tell us our current starting positions! */
   write( "Your current starting position is "+ starts[ 1 ] +".\n"+
         "You can also choose your starting position from:\n" );
   for ( i = 1; i <= z; i++ )
      write( sprintf( "% 5d: $I$7=%s\n", i, starts[ 2 * i + 1 ] ) );
   write( "Choose a new starting position using: start <number>\n"+
         "To clear your list of starting positions, use: start clear\n" );
   return 1;
} /* list_starts() */

int set_start( int num ) {
   string *starts;
   int z;

   starts = this_player()->query_starts();
   z = ( sizeof( starts ) - 2 ) / 2;

   if (sizeof(starts) < 2) {
      add_failed_mess("You only have one starting location.\n");
      return 0;
   }

   if (z == 0) {
      add_failed_mess("You have no starting locations to choose from at all.\n");
      return 0;
   }

   if ( ( num < 1 ) || ( num > z ) ) {
      add_failed_mess( "You must choose a number from 1 to "+ z +".\n" );
      return 0;
   }
   num *= 2;
   write( "Your new starting position is "+ starts[ num + 1 ]  +".\n" );
   this_player()->set_start_location(starts[num]);
   return 1;
} /* start_stuff() */

/*
 * Not really sure they should be able to clear them...  Anyway...
 *
 * Yes, they should: file names change, and creators make mistakes.
*/

mixed *query_patterns() {
  return ({ "<number>", (: set_start($4[0]) :),
            "clear", (: clear_starts() :),
            "", (: list_starts() :) });
} /* query_patterns() */
