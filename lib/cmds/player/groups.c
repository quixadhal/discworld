/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: groups.c,v 1.4 2001/02/01 17:30:43 pinkfish Exp $
 *
 * $Log: groups.c,v $
 * Revision 1.4  2001/02/01 17:30:43  pinkfish
 * Fix up some bugs.
 *
 * Revision 1.3  1999/06/21 09:15:11  tape
 * Made it patch directly into "group list".
 *
 * Revision 1.2  1999/06/20 09:34:54  taffyd
 * Temporarily patched for new groups system.
 *
 * Revision 1.1  1998/07/13 03:42:54  pinkfish
 * Initial revision
 *
*/

#include <group_handler.h>

inherit "/cmds/base";

mixed cmd(string str, string pattern) {
   return GROUP_CMD->group_command_control( "list", "", "", "",
                                            ({ str }), pattern );
} /* cmd() */

mixed *query_patterns() {
   return ({ "", (: cmd("", "list") :),
             "[with] <string'player'>",
             (: cmd($4[0], "list [with] <string'player'>") :) });
} /* query_patterns() */
