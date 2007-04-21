/*   -*- LPC -*-   */
/*
 * $Locker:  $
 * $Id: related_files.c,v 1.14 2002/11/18 01:37:21 ceres Exp $
 */
#include <playerinfo.h>
#include <nomic_system.h>

void delete_related_files( string name, int mail, int refresh_type ) {
   string *files;
   string area;
   string dir;
   
   if (previous_object() != find_object("/secure/bulk_delete") &&
       previous_object() != find_object("/secure/delete_clear") &&
       previous_object() != find_object("/obj/handlers/refresh")) {
     if (!master()->high_programmer(previous_object(-1)) &&
         ((string)previous_object()->query_name() != name)) {
       tell_object(find_player("ceres"), "Not doing erasing. %O\n",
                   previous_object());
       unguarded( (: write_file, "/log/CHEAT", ctime( time() ) +
                   ": illegal attempt to delete related files using "+
                   "/secure/related_files\nTrace: "+ back_trace() :) );
       return;
     }
   }
   unguarded( (: rm, "/save/artifacts/"+ name :) );
   unguarded( (: rm, "/save/bank_accounts/"+name[0..0] + "/" + name + ".o" :) );
   unguarded( (: rm, "/save/cmr_library/"+ name +".o" :) );
   catch("/obj/handlers/library"->restart(name));
   unguarded( (: rm, "/save/library/"+ name[0..0] + "/" + name + ".o" :) );
   unguarded( (: rm, "/save/philosophies/"+ name[0..0] + "/"+ name + ".o" :) );

   foreach(dir in get_dir("/save/vaults/*"))
     if(dir != "." && dir != "..")
       unguarded( (: rm, "/save/vaults/" + dir + "/" + name + ".o" :) );

   unguarded( (: rm, "/save/newsrc/"+name[0..0]+"/"+name+".o" :) );
   
   files = get_dir( "/w/.dead_ed_files/"+ name +"-*" );
   if ( sizeof( files ) ) {
      map(files, (: unguarded( (: rm, "/w/.dead_ed_files/"+ $1 :) ) :) );
   }

   if ( mail ) {
      // We only delete their player info if we also delete their mail...
      "/obj/handlers/folder_handler"->delete_account( name );
      //PLAYERINFO_HANDLER->player_remove( name );
   }

   "/obj/handlers/secure_handlers/inhume"->player_refreshed( name );
   "/obj/handlers/guild_things/thieves_ledger"->delete_crimes( name );
   "/obj/handlers/guild_things/quota/thief_quota"->remove_thief(name);
   "/obj/handlers/guild_things/quota/thief_quota"->remove_player(name);
   "/d/am/buildings/apex/admin_office"->remove_member( name );
   "/d/am/items/watch_ledger"->delete_crimes( name );

   foreach (area in NOMIC_HANDLER->query_citizenship_areas()) {
      NOMIC_HANDLER->remove_citizen(area, name);
   }
} /* delete_related_files() */
