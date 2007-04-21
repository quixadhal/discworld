/*
 * Command to list the effects on a person.
 */
inherit "/cmds/base";

int cmd( object * obs, int brief ) {
   object who;
   int num, * enums;
   mixed arg;

   if( !obs )
      obs = ({ this_player() });

   foreach( who in obs ) {
      write( "%^BOLD%^Effects on " + who->query_short() + ":%^RESET%^\n" );
      enums = who->effects_matching( "" );
      if( !sizeof( enums ) ) {
         write( "No effects.\n" );
         printf( "%s\n",
           "-----------------------------------------------------------" );
         continue;
      }

      foreach( num in enums ) {
         arg = who->arg_of(num);
         if ( brief && ( arrayp(arg) || classp(arg) || mapp(arg) ) ) {
            arg = "COMPLEX";
         }
         if ( !brief ) {
            printf( "%s\n",
              "-----------------------------------------------------------" );
         }
         printf( "[%d]   %s (%O)\n", num,
           find_object( who->eff_of(num) )->query_classification(),
           arg );
      }
      printf( "%s\n",
        "-----------------------------------------------------------" );
   }
   return 1;
}

mixed * query_patterns() {
   return ({
     "<indirect:wiz-present> verbose", (: cmd( $1, 0 ) :),
     "<indirect:wiz-present> [brief]", (: cmd( $1, 1 ) :),
     "[me] verbose", (: cmd( 0, 0 ) :),
     "[me] [brief]", (: cmd( 0, 1 ) :),
   });
}
