inherit "/cmds/base";
 
int cmd() {
   write(this_player()->query_path() + "\n");
   return 1;
} /* cmd() */
 
mixed *query_patterns() {
   return ({ "", (: cmd :) });
} /* query_patterns() */

