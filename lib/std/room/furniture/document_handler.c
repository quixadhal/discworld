/**
 * The furniture object for the document handler.
 * @author Pinkfish
 * @started Tue Jan 15 16:43:39 PST 2002
 */

inherit "/std/room/furniture/commercial";
inherit "/std/room/furniture/inherit/document_handler";

#include <room/document_handler.h>

void create() {
   document_handler::create();
   commercial::create();
   add_help_file("document_handler");
   set_shop_use_types( ({ "document_view",
                          "document_add",
                          "document_delete" }) );
} /* create() */

/** @ignore yes */
string query_save_dir() {
   return commercial::query_save_dir() + "/" + query_commercial_id();
} /* query_save_dir() */

/** @ignore yes */
void load_me() {
   if (!query_commercial_id()) {
      setup_new_commercial_id();
   }

   // Make the directory for our stuff to go in.
   if (unguarded( (: file_size(query_save_dir()) :) ) != -2) {
      unguarded( (: mkdir(query_save_dir()) :));
   }
} /* load_me() */

void save_me() {
   environment()->event_save(this_object());
} /* save_me() */

/** @ignore yes */
void init() {
   add_command("list", "documents [from] <direct:object>", (: do_list() :));
   add_command("view", "document <string'id'> [from] <direct:object>",
                (: do_display($4[0]) :));
   add_command("add", "documents {html|plain} called <string:quoted'title'> to <direct:object>",
               (: do_add_document($4[1], ($4[0] == "html"?DOCUMENT_HTML:DOCUMENT_PLAIN)) :));

   add_command("delete", "document <string'id'> [from] <direct:object>",
                (: do_delete_document($4[0]) :));
} /* init() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
   mapping map;

   map = commercial::query_dynamic_auto_load();
   document_handler::query_dynamic_auto_load(map);
   return map;
}

void init_dynamic_arg(mapping map) {
   document_handler::init_dynamic_arg(map);
   commercial::init_dynamic_arg(map);
} /* init_dynamic_arg() */
