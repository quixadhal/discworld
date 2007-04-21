/*  -*- LPC -*-  */
/*
 * $Id: liaison_s.c,v 1.4 2000/01/18 00:23:45 turrican Exp $
 */
inherit "/cmds/base";

private string idle_time(object person) {
  int hours, mins, secs;
  string s, m, h;

  secs = query_idle(person);
  mins = secs / 60;
  secs %= 60;
  hours = mins / 60;
  mins %= 60;
  s = ("0"+secs)[<2..<1];
  m = ("0"+mins)[<2..<1];
  if ( !hours )  {
    return " (idle: "+m+":"+s+")";
  }
  h = ("0"+hours)[<2..<1];
  return " (idle: "+h+":"+m+":"+s+")";
} /* idle_time() */

mixed cmd( ) {
   int i, j;
   string *members, *member_alias, *aliases;
   object person;
   
   members = (string *)"/d/liaison/master"->query_members();
   aliases = ({ });
   i = sizeof( members );
   while ( i-- ) {
      person = find_player( members[ i ] );
      if ( ! person ) {
         member_alias = "/d/liaison/master"->member_alias( members[ i ] );
         j = sizeof( member_alias );
         while ( j-- ) {
            if ( find_player( member_alias[ j ] ) ) {
               aliases += ({ capitalize( member_alias[ j ] ) +"("+
                     capitalize( members[ i ] ) +")" });
            }
         }
         members = delete( members, i, 1 );
      } else if (!person) {
          members = delete(members, i, 1);
      } else {
         if ( !interactive( person ) ) {
            members = delete( members, i, 1 );
         } else {
            members[ i ] = capitalize( members[ i ] );
            switch( person->query_invis()  )  {
            case 0:
               break;
            case 1:
               members[ i ] = sprintf( "(%s)", members[ i ] );
               break;
            case 2:
               members[ i ] = sprintf( "({%s})", members[ i ] );
               break;
            default:
               members[ i ] = sprintf( "([%s])", members[ i ] );
               break;
            }
            members[ i ] = capitalize( members[ i ] );
            if (person->query_busy()) {
               members[ i ] += " (busy)";
            }
            if ( query_idle( person ) > 120 )  {
               members[ i ] += idle_time( person );
            }
         }
      }
   }

   if ( this_player()->query_creator() || !sizeof( members )) {
      members += aliases;
   }
   if ( !sizeof( members ) ) {
      add_failed_mess( "There are no Liaison creators currently logged on.\n" );
      return 0;
   }

   if ( sizeof( members ) > 1 ) {
      write( "Liaison creators logged on are "+
            query_multiple_short( members ) +".\n" );
   } else {
      write( "The only Liaison creator logged on is "+ members[ 0 ] +".\n" );
   }
   return 1;
} /* cmd() */

mixed *query_patterns() {
   return ({ "", (: cmd() :) });
} /* query_patterns() */
