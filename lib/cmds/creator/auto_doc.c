/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: auto_doc.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: auto_doc.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";

#include <autodoc.h>

int cmd(string str) {
   string *files;
   string file;

   files = this_player()->get_files(str);
   if (!sizeof(files)) {
      notify_fail("Unable to find the files " + str + ".\n");
      return 0;
   }
   foreach (file in files) {
      if (AUTODOC_HANDLER->recreate_documentation(file)) {
         write("Recreating documentation for " + file + ".\n");
      } else {
         write("Unable to recreate documentation for " + file + ".\n");
      }
   }
   return 1;
} /* cmd() */

mixed *query_patterns() { 
   return ({ "<string>", (: cmd($4[0]) :) });
 } /* query_patterns() */
