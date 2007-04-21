/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: co_mmands.c,v 1.2 2003/03/25 23:12:44 drakkos Exp $
 * $Log: co_mmands.c,v $
 * Revision 1.2  2003/03/25 23:12:44  drakkos
 * Added a line for PT commands
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";

int cmd( ) {
  string *cmds;
  write( "Your current commands are:\n"+ sprintf( "%-#*s\n",
         (int)this_player()->query_cols(), implode( sort_array(
         (string *)this_player()->query_known_commands(), 1 ),
         "\n" ) ) );
         
  if (this_player()->query_playtester()) {
    cmds = get_dir ("/cmds/playtester/");
    
    cmds = filter (cmds, (: strsrch ($1, ".c") == (sizeof ($1) - 2) :));
    write( "\n\nYour playtester commands are:\n"+ replace (sprintf( "%-#*s\n",
         (int)this_player()->query_cols(), implode( sort_array(
         (string *)cmds, 1 ),
         "\n" ) ), ({".c", ""})) );
        
  }
   return 1;
} /* cmd() */

mixed* query_patterns() {
   return ({ "", (: cmd() :) });
}
