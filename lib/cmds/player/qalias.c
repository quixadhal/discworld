int cmd()
{
   string *aliases, alias_mess, message;
   int cols;
   
   aliases = keys( this_player()->query_aliases() );
   
   if( !sizeof( aliases ) )
   {
      tell_object( this_player(), "You have no aliases.\n" );
      return 1;
   }
   
   message = "\nYou have the following aliases:\n";
   
   // Alphabetize.
   
   aliases = sort_array( aliases, (: strcmp :) );
   alias_mess = implode( aliases, ", " );
   
   cols = this_player()->query_cols() - 3;
   
   message += sprintf( "   %-=*s\n", cols, alias_mess );

   message += "A total of " + sizeof( aliases ) + " aliases.\n";
   
   tell_object( this_player(), message );
   
   return 1;
   
} /* cmd() */


mixed *query_patterns()
{
   return ({ "", (: cmd() :) });
} /* query_patterns() */

