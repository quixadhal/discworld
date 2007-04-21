// Pinkfish
// Started Wed May 30 21:37:15 PDT 2001

inherit "/std/room/furniture/commercial";
#include <money.h>
#include <move_failures.h>

private mapping _money;
private int _cut;
private int _cost;

#define BOARD_TAG "money changer"

void setup() {
   set_name( "counter" );
   set_short( "money changer counter" );
   add_adjective( ({ "money", "changer" } ));
   add_alias("changer");
   set_long( "A box with a bunch of holes in it, it looks like it will "
             "change coins into other types of coins.\n");
   set_allowed_positions(({"sitting", "lying", "kneeling", "meditating"}));
   set_allowed_room_verbs((["sitting" : "sits" ]));
   set_weight(2000);
   set_value(240000);
   // Minimum bid $1.

   add_help_file("money_changer");
   set_commercial_size(5);
   _cut = 10;
   _cost = 400;
   _money = ([ ]);
}

/**
 * This method changes the money into a specific type of thing.
 * @param words the money to change
 * @param type what to change it into
 */
int change( string words, string type ) {
   int val_index;
   int value;
   int value_type;
   int value_unit;
   int value_change;
   int value_num;
   int i;
   string place;
   mixed money;
   object changed;
   object *monies;
   mixed *change;
   mixed *values;
   mixed *money_bits;

   place = query_money_place();
   if ( !place || ( place == "" ) ) {
      place = "default";
   }

   values = (mixed *)MONEY_HAND->query_values_in( place );
   MONEY_OBJECT->set_money_array( values );
   val_index = (int)MONEY_OBJECT->find_best_fit( type );
   if ( val_index == -1 ) {
      return notify_fail( "The coin type \""+ type +
            "\" is not legal tender here.\n" );
   }

   money = (mixed)MONEY_HAND->parse_money( words, this_player(), place );
   if ( intp( money ) ) {
      switch( money ) {
         case NO_MATCH :
            return notify_fail( "You do not have \""+ words +"\".\n" );
         case NO_MONEY :
            return notify_fail( "You can only change money here.\n" );
         default :
            return notify_fail( "You can only change legal tender.\n" );
      }
   }

   /* Pull out the provincial coins otherwise the player gets ripped off */
   if (place != "default")  {
      monies = MONEY_HAND->filter_legal_tender(money, "default");
      if (monies[0])  monies[0]->move(this_player());
      if (!monies[1])  {
         return notify_fail( "You can only change legal tender.\n" );
      }
      money = monies[1];
   }

   //
   // The money bits.
   //
   money_bits = money->query_money_array();

   value = (int)money->query_value_in( place );
   //
   // Check and see if they have any of the type of money which we are trying
   // to change to in the money object.
   //
   i = member_array(values[val_index], money->query_money_array());
   if (i != -1) {
      value_type = money->query_money_array()[i + 1] * values[val_index + 1];
   }
   value_unit = (values[val_index + 1] * (100 + _cut)) / 100;
//printf("%O %O %O\n", value_unit, values[val_index + 1], _cut);
   //
   // Figure out the end value.
   //
   value_change = (value - value_type) % value_unit;
   value_num = (value - value_type) / value_unit;
   value = value_num * values[val_index + 1] + value_type;
   if ( value < values[ val_index + 1 ] ) {
      money->move( this_player() );
      return notify_fail( MONEY_HAND->money_string(money->query_money_array(), place) +
                         " is not even worth one "+ values[ val_index ] +
                       ".\n" );
   }

   //
   // Check and see if they have enough type of that sort of money.
   //
   if (_money[values[val_index]] < value_num) {
      money->move( this_player() );
      add_failed_mess("The money changer does not have enough " +
                      MONEY_HAND->query_main_plural_for(values[val_index]) +
                      " to pay you.\n");
      return 0;
   }

   _money[values[val_index]] -= value_num;
   for (i = 0; i < sizeof(money_bits); i += 2) {
      _money[money_bits[i]] += money_bits[i + 1];
   }

   changed = clone_object( MONEY_OBJECT );
   changed->set_money_array( ({ values[ val_index ],
                                value / values[ val_index + 1 ] }));
   change = (mixed *)MONEY_HAND->create_money_array( value_change, place );
   for (i = 0; i < sizeof(changed); i += 2) {
      _money[change[i]] -= change[i + 1];
   }
   this_player()->add_succeeded_mess(this_object(),
    ({ "You changed "+query_multiple_short(money->half_short( 1 ) ) +
         " into "+
         query_multiple_short( changed->half_short( 1 ) ) +".\n",
       "$N $V some money into "+type+".\n" }), ({ }) );
        
   //write( "You get "+ query_multiple_short( (string)money->half_short( 1 ) ) +
         //" changed into "+
         //query_multiple_short( (string)changed->half_short( 1 ) ) +".\n" );
   money->move( "/room/rubbish" );
   if ( sizeof( change ) ) {
      changed->adjust_money( change );
      write( "You were returned "+ (string)MONEY_HAND->money_string( change )
            +" in change.\n" );
   }
   if ( (int)changed->move( this_player() ) != MOVE_OK ) {
      write( "Unfortunately, you're too heavily burdened to accept all that "+
            "money, so it's put on the table for you.\n" );
      changed->move( this_object() );
   }
   return 1;
} /* change() */

string query_main_status(int hint) {
   string type;
   string place;
   int num;
   string str;

   place = query_money_place();
   str = "$I$0=Money Changer:\n";
   str += "$I$6=   Percentage cut " + _cut + "%\n";
/*
   str += "$I$6=   Cost per use " +
          MONEY_HAND->money_value_string(_cost, place) + "\n";
 */
   foreach (type, num in _money) {
      str += "$I$6=   " + num + " " + type + "\n";
   }

   return str;
} /* query_main_status() */

/**
 * This method sets the ante amounts.
 * @param str the amount string
 * @param max_bet if it a max or min bet to set
 */
int do_set_cost(string str) {
   string place;
   int value;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to change the paramaters of "
                   "$D.\n");
      return 0;
   }
   place = query_money_place();
   value = MONEY_HAND->value_from_string(str, place);
   if (!value) {
      add_failed_mess("Unable to parse the string " + str + ".\n");
      return 0;
   }
   _cost = value;
   add_succeeded_mess("$N set$s the cost of each use of $D to " +
          MONEY_HAND->money_value_string(value, place) + ".\n");

   return 1;
} /* do_set_cost() */

/**
 * This method sets the ante amounts.
 * @param str the amount string
 * @param max_bet if it a max or min bet to set
 */
int do_set_cut(int number) {
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to change the paramaters of "
                   "$D.\n");
      return 0;
   }

   if (number < 0) {
      add_failed_mess("You need to set the cut to a number greator than 0.\n");
      return 0;
   }

   if (number > 500) {
      add_failed_mess("You need to set the cut to a number less than 500.\n");
      return 0;
   }

   _cut = number;
   add_succeeded_mess("$N set$s the cut for $D to " + _cut + "%.\n");
   return 1;
} /* do_set_cut() */

/**
 * This method deposits coins into the changer.
 * @param str the amount string
 * @param max_bet if it a max or min bet to set
 */
int do_deposit(int num, string str) {
   mixed* values;
   int i;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to change the paramaters of "
                   "$D.\n");
      return 0;
   }

   values = this_player()->query_money_array();
   for (i = 0; i < sizeof(values); i += 2) {
      if (lower_case(values[i]) == lower_case(str)) {
         break;
      }
   }
   //i = member_array(str, values);
   if (i >= sizeof(values)) {
      add_failed_mess("You do not have any " + str + " to deposit.\n");
      return 0;
   }

   if (values[i + 1] < num) {
      add_failed_mess("You do not have " + num + " of " + values[i] +
                      ", you only have " + values[i+1] + ".\n");
      return 0;
   }

   _money[values[i]] += num;
   this_player()->adjust_money(-num, values[i]);

   add_succeeded_mess("$N deposit$s " + num + " of " + str + " in $D.\n");
   return 1;
} /* do_deposit() */

/**
 * This method withdraws money from the changer.
 * @param str the amount string
 * @param max_bet if it a max or min bet to set
 */
int do_withdraw(int num, string str) {
   string tmp;
   int bing;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to change the paramaters of "
                   "$D.\n");
      return 0;
   }

   foreach (tmp, bing in _money) {
      if (lower_case(str) == lower_case(tmp)) {
         if (num > bing) {
            add_failed_mess("The $D does not have " +
                            num + " of " + tmp + " it only has " +
                            bing + ".\n");
            str = tmp;
         }
      }
   }

   if (!_money[str]) {
      add_failed_mess("You do not appear to have any " + str + " in $D.\n");
      return 0;
   }

   _money[str] -= num;
   if (!_money[str]) {
      map_delete(_money, str);
   }
   this_player()->adjust_money(num, str);

   add_succeeded_mess("$N withdraw$s " + num + " of " + str + " from $D.\n");
   return 1;
} /* do_deposit() */

void init() {
   commercial::init();
   add_command("change", "<string'money'> into <string'type'> on <direct:object>",
                  (: change($4[0], $4[1]) :));
   if (environment()->is_allowed(this_player()->query_name())) {
      add_command("set", "cut <number'percentage'> on <direct:object>",
                  (: do_set_cut($4[0]) :));
      add_command("deposit", "<number> of <string'type'> on <direct:object>",
                  (: do_deposit($4[0], $4[1]) :));
      add_command("withdraw", "<number> of <string'type'> on <direct:object>",
                  (: do_withdraw($4[0], $4[1]) :));
/*
      add_command("set", "cost of use <string'amount'> on <direct:object>",
                  (: do_set_cost($4[0]) :));
 */
   }
} /* init() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
   mapping map;

   map = commercial::query_dynamic_auto_load();
   add_auto_load_value(map, BOARD_TAG, "cut", _cut);
   add_auto_load_value(map, BOARD_TAG, "money", _money);
   add_auto_load_value(map, BOARD_TAG, "cost", _cost);
   return map;
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_dynamic_arg(mapping map, object player) {
   commercial::init_dynamic_arg(map, player);

   _cut = query_auto_load_value(map, BOARD_TAG, "cut");
   _money = query_auto_load_value(map, BOARD_TAG, "money");
   _cost = query_auto_load_value(map, BOARD_TAG, "cost");
} /* init_dynamic_arg() */
