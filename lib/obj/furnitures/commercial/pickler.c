/**
 * @author Pinkfish
 * Started Mon Jun 18 20:15:58 PDT 2001
 */

inherit "/std/room/furniture/commercial";

#include <money.h>

#define FEE_PER_USE 40

#define BOARD_TAG "pickler"

private int _revenue;
private int _cost;

void setup() {
   set_name( "pickler" );
   set_short( "pickler" );
   set_long( "A small wooden box with a handle on it, it has some words "
             "written on it.\n");
   add_read_mess("Pickler, pull for results", 0, "common");
   set_allowed_positions(({"sitting", "lying", "kneeling", "meditating"}));
   set_allowed_room_verbs((["sitting" : "sits" ]));
   set_weight(2000);
   set_value(240000);
   add_help_file("pickler");

   set_commercial_size(2);
   set_commercial_type("utility");
   set_shop_use_types(({ "pickler" }));
   _cost = 80;
}

int do_pickle(object* obs) {
   object* pickleable;
   string place;
   int amt;
   int here_amt;

   if (!is_open_for("pickler", this_player()->query_name())) {
      add_failed_mess("The pickler is not open.\n");
      return 0;
   }

   place = query_money_place();
   pickleable = filter(obs, (: !$1->query_cured() &&
                               ($1->query_property("cureable") ||
                                $1->query_decay_speed()) :));
   if (!sizeof(pickleable)) {
      add_failed_mess("You cannot pickle any of " +
                      query_multiple_short(obs) + ".\n");
      return 0;
   }

   amt = sizeof(pickleable) * _cost;
   here_amt = amt - sizeof(pickleable) * FEE_PER_USE;
   if (here_amt < 0) {
      if (query_float() < -here_amt) {
          add_failed_mess(environment()->the_short() + " does not "
                          "have enough money to pickle anything.\n");
          return 0;
      }
   }

   if (this_player()->query_value_in(place) < amt) {
      add_failed_mess("You do not have the needed " +
                      MONEY_HAND->money_value_string(amt, place) +
                      " to pay for $I to be pickled.\n", pickleable);
      return 0;
   }

   this_player()->pay_money(MONEY_HAND->create_money_array(amt, place), place);
   pickleable->do_cure();
   adjust_float(here_amt);
   _revenue += here_amt;
   add_succeeded_mess("$N pickle$s $I on $D.\n", pickleable);
   return 1;
} /* do_pickle() */

int do_set_cost(string amount) {
   int amt;
   string place;

   place = query_money_place();

   amt = MONEY_HAND->value_from_string(amount, place);
   if (amt <= 0) {
      add_failed_mess("The value " + amount + " is invalid.\n");
      return 0;
   }

   _cost = amt;
   add_succeeded_mess("$N set$s the cost of using $D to " +
                      MONEY_HAND->money_value_string(amt, place) + ".\n");
   return 1;
} /* do_set_cost() */

string query_main_status(int hint) {
   string place;

   place = query_money_place();
   return the_short() + " status:\n"
          "\n$I$6=   Fee Per Use : " +
              MONEY_HAND->money_value_string(FEE_PER_USE, place) + " (fixed)"
          "\n$I$6=   Cost Per Use: " +
              MONEY_HAND->money_value_string(_cost, place) +
          "\n$I$6=   Profit      : " +
             MONEY_HAND->money_value_string(_revenue, place) + "\n";
} /* query_main_status() */

int do_stats() {
   string ret;

   ret = query_main_status(0);
   write(ret);
   add_succeeded_mess("");
   return 1;
} /* do_stats() */

void init() {
   commercial::init();
   add_command("use", "<direct:object> to [pickle] <indirect:object>",
               (: do_pickle :));
   add_command("use", "<direct:object> with <indirect:object>",
               (: do_pickle :));
   add_command("pickle", "<indirect:object> with <direct:object>",
               (: do_pickle :));
   add_command("pull", "[handle] [on] <direct:object> with <indirect:object>",
               (: do_pickle :));
   if (environment()->is_allowed(this_player()->query_name())) {
      add_command("status", "<direct:object>",
                  (: do_stats() :));
      add_command("set", "cost <string'cost'> on <direct:object>",
                  (: do_set_cost($4[0]) :));
   }
} /* init() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
   mapping map;

   map = commercial::query_dynamic_auto_load();
   add_auto_load_value(map, BOARD_TAG, "revenue", _revenue);
   add_auto_load_value(map, BOARD_TAG, "cost", _cost);
   return map;
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_dynamic_arg(mapping map, object player) {
   commercial::init_dynamic_arg(map, player);

   _revenue = query_auto_load_value(map, BOARD_TAG, "revenue");
   _cost = query_auto_load_value(map, BOARD_TAG, "cost");
} /* init_dynamic_arg() */
