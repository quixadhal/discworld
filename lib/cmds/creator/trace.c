/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: trace.c,v 1.2 2000/04/17 17:25:23 ceres Exp $
 * $Log: trace.c,v $
 * Revision 1.2  2000/04/17 17:25:23  ceres
 * Removed apply since it seems to crash the driver.
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* call command, trial out by Turrican for a commands daemon. */

#include <creator.h>
#include <parser.h>

inherit "/cmds/base";

#define OPTIONS ({ "off", "call", "call_other", "return", "args", "exec", "heart_beat", })
  
mixed cmd(string arg) {
  string *args;
  int     i, j, trace_arg;
  
  /* ok lets make this clever...
   */
  notify_fail( "Usage: trace <" + implode(OPTIONS, "|") + ">\n" );

  if( !arg ) return 0;

  args = explode( arg, " " );
  
  if( !sizeof( args ) )
    return 0;
  
  for( i = 0; i < sizeof( args ); i++ )
    if( (j = member_array( args[ i ], OPTIONS )) != -1 )
    {
      if( !j )
      {
        trace( 0 );
        write( "Trace off.\n" );
        return 1;
      }
      else
        trace_arg |= ({ 0, 1, 2, 4, 8, 16, 32, 64, 128 })[ j ];
    }
    else if( sscanf( args[ i ], "%d", j ) && j != 64)
      trace_arg |= j;

  trace( trace_arg | 128 );
  write( "Trace level: " + trace_arg + "\n" );
  return 1;
}
