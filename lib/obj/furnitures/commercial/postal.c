/**
 * @author Pinkfish
 * Started Mon Jun 18 20:15:58 PDT 2001
 */

inherit "/std/room/furniture/commercial";

#include <money.h>
#include <mail.h>

#define BOARD_TAG "postal"

int _num_uses;

void setup() {
   set_name( "counter" );
   set_short( "postal counter" );
   add_adjective("postal");
   set_long( "A wooden counter that looks like a place you could write "
             "and read mail.\n");
   set_allowed_positions(({"sitting", "lying", "kneeling", "meditating"}));
   set_allowed_room_verbs((["sitting" : "sits" ]));
   set_weight(2000);
   set_value(6400000);
   add_help_file("postal");

   set_commercial_size(25);
   set_commercial_type("utility");
   set_shop_use_types(({ "pickler" }));
}

int do_mail(string to) {
   if (MAIL_TRACK->mail(to)) {
      _num_uses++;
      return 1;
   }
   return 0;
} /* do_pickle() */

string query_main_status(int hint) {
   string place;

   place = query_money_place();
   return the_short() + " status:\n"
          "\n$I$6=   Number of  uses : " + _num_uses + "\n";
} /* query_main_status() */

void init() {
   commercial::init();
   add_command("mail", "<string'to'>",
               (: do_mail($4[0]) :));
   add_command("mail", "[handle] [on] <direct:object> with <indirect:object>",
               (: do_mail(0) :));
} /* init() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
   mapping map;

   map = commercial::query_dynamic_auto_load();
   add_auto_load_value(map, BOARD_TAG, "uses", _num_uses);
   return map;
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_dynamic_arg(mapping map, object player) {
   commercial::init_dynamic_arg(map, player);

   _num_uses = query_auto_load_value(map, BOARD_TAG, "uses");
} /* init_dynamic_arg() */
