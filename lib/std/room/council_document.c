/**
 * A stable in which to house your lucky transport.
 * @author Pinkfish
 * @started Tue Nov 30 23:37:12 PST 1999
 */
inherit "/std/room/document_handler";
inherit "/std/room/inherit/council_base";
#include <nomic_system.h>

/** @ignore yes */
int is_open_for(string type, string name) {
   switch (type) {
   case "document_add" :
   case "document_delete" :
      return NOMIC_HANDLER->is_magistrate_of(query_council_area(), name);
   default :
      return 1;
   }
} /* is_open_for() */
