/* Gruper, Feb 26, 2000 */

#include <command.h>

inherit "/cmds/base";

mixed cmd() {
   write("Nothing happens.\n" );
   return 1;
} /* cmd() */

mixed *query_patterns() {
   return ({ "", (: cmd() :) });
} /* query_patterns() */
