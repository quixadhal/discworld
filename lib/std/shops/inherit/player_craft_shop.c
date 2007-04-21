/**
 * This is the file containing extra controls for the player run
 * craft shop.  We will assume that all player run craft shops will be
 * rooms and not npcs.
 * @author Pinkfish
 * @started Fri Apr 21 13:37:44 PDT 2000
 */
inherit "/std/basic/expressions";
inherit "/std/shops/inherit/craft_shop_category";
#include <player.h>
#include <money.h>
#include <move_failures.h>
#include <shops/craft_shop.h>
#include <player_handler.h>
#include <clubs.h>
#define EXPRESSION_NO_CLASSES
#include <expressions.h>
#include <mail.h>
#include <nroff.h>


//#define PRESTO_DEBUG

#define CHARGE_MULT 10 /* This is out of /obj/weapons/maces/cleric_rod.c */
#define PLAYER_CRAFT_SHOP_CHECK_PROP "player craft shop check"

#define PLAYER_CRAFT_SHOP_SELL      1
#define PLAYER_CRAFT_SHOP_SELL_AUTO 2
#define PLAYER_CRAFT_SHOP_BUY       3
#define PLAYER_CRAFT_SHOP_APPROVE   4

#define PLAYER_CRAFT_SHOP_EXPR_DENY   1
#define PLAYER_CRAFT_SHOP_EXPR_ACCEPT 2

class approval_item  {
   mixed value;
   string list_name;
   int markup;
   int final_cost;
}

class expression_type {
   int type;
   class parse_node* condition;
   class parse_node* value;
   string list_name;
}

class approval {
   mixed hairy;
   class expression_type* expressions;
   int low_cost;
   int high_cost;
   mapping items;
   int num_allowed;
   int high_cost_deny;
}

class approval_obs {
   string seller;
   string category;
   int value;
   string name;
   int* saved;
   int enter_time;
   int* checkout;
}

#if USE_TRANSACTIONS
class shop_transaction {
   int time;
   string person;
   string objects;
   string name_cat;
   int type;
   int amount;
   mixed extra;
}
#endif

class shop_stats {
   int num_sold;
   int value_sold;
}

class seller_information {
   int max_sellable;
   int value_limit;
   int deny_value_limit;
}

private class approval_obs* _waiting_for_approval;
private class approval_obs* _approved;
private class approval _automatic_approval;
// This is the list of allowable list names.
private string* _list_names;
private int _automatic_percentage;
private int _royalty_percentage;
private int _current_save_num;
private int _total_outgoing;
private int _total_ingoing;
private string _shop_name;
private class parse_node *_tax_expression;
#if USE_TRANSACTIONS
private class shop_transaction* _transactions;
#endif
private mapping _sell_stats;
private int _stats_start;
private mapping _sellers;
private string *_black_list;
private class parse_node* _buy_expression;
private class parse_node* _use_expression;
private string _auto_load_sign_str;
private int _broken_sign;
private mapping _num_checked_so_far;

private nosave string _parcel_post;
private nosave int _something_checkedout;
private nosave int _maximum_inventory;
private nosave object _sign_ob;
private nosave int _round_value;
private nosave int _max_list_names;
private nosave int _max_items_in_queue;
private nosave object _royalty_ob;

protected void confirm_approval(string answer, class approval_obs approve, int final_cost);
private mixed* parse_or(string str);
private int variable_player_level(string seller, int cost, object* ob);
private string variable_player_guild(string seller, int cost, object* ob);
private string variable_player_deity(string seller, int cost, object* ob);
private string variable_player_family(string seller, int cost, object* ob);
private string variable_player_order(string seller, int cost, object* ob);
private object* variable_objects(string seller, int cost, object* ob);
private string function_object_type(object*, string seller, int cost, object* ob);
private int function_object_value(object*, string seller, int cost, object* ob);
private int function_object_base_value(object*, string seller, int cost, object* ob);
private int function_object_condition(object*, string seller, int cost, object* ob);
private int function_object_enchant(object*, string seller, int cost, object* ob);
private object* function_object_stolen(object*, string seller, int cost, object* ob);
private object* function_object_warded(object*, string seller, int cost, object* ob);
private int function_object_charges(object*, string seller, int cost, object *ob);
private int variable_sale_cost(string seller, int cost, object* ob, string name, string category);
private string variable_sale_name(string seller, int cost, object* ob, string name, string category);
private string variable_sale_category(string seller, int cost, object* ob, string name, string category);
private int function_club_member(string club, string seller, int cost, object* ob);
private object* function_object_contains_spell(object*, string spell, string seller, int cost, object* ob);
private object* function_object_contains_imbue(object*, string spell, string seller, int cost, object* ob);
private int function_object_percentage_liquid(object*, string liquid, string seller, int cost, object* ob);
private object* function_object_matching(object*, string match, string seller, int cost, object* ob);
private object* function_object_short(object* obs, string match, string seller, int cost, object* fluff);
private int function_inventory_number(string short, string seller, int cost, object* ob);
private object* function_contents(object ob, string seller, int cost, object* obs);


/*
 * This methods must be defined in an upper level class.
 */
string query_owner();
int is_allowed(string person);
void do_save();
void event_save(object thing);
string *query_allowed();
void set_short(string short);
void add_property(string name, mixed value);

void create() {
   class parse_node* expr;

   _round_value = 1;
   if (!_waiting_for_approval) {
      _waiting_for_approval = ({ });
   }
   if (!_approved) {
      _approved = ({ });
   }
   if (!_list_names) {
      _list_names = ({ });
   }
   if (!_automatic_approval) {
      _automatic_approval = new(class approval);
      //_automatic_approval->sellers = ({ });
      //_automatic_approval->categories = ([ ]);
      _automatic_approval->expressions = ({ });
      _automatic_approval->items = ([ ]);
   }
   if (!_automatic_percentage) {
      _automatic_percentage = 10;
   }
#if USE_TRANSACTIONS
   if (!_transactions) {
      _transactions = ({ });
   }
#endif
   if (!_sell_stats) {
      _sell_stats = ([ ]);
   }
   if (!_stats_start) {
      _stats_start = time();
   }
   if (!_sellers) {
      _sellers = ([ ]);
   }
   if (!_black_list) {
      _black_list = ({ });
   }
   if (!_num_checked_so_far) {
      _num_checked_so_far = ([ ]);
   }
   if (!_tax_expression)  {
      _tax_expression = ({ });
   }
   //
   // Set it up so that selling it the normal way gets no money.
   //
   set_cut(100);
   _max_items_in_queue = 100;
   craft_shop_category::create();
   expressions::create();
   if (_shop_name) {
      set_short(_shop_name);
      add_property("determinate", "");
   }
   set_always_ask_price(1);
   if (!_buy_expression) {
      expr = parse_boolean_string("false");
      _buy_expression = expr;
   }
   if (!_use_expression) {
      expr = parse_boolean_string("false");
      _use_expression = expr;
   }

   add_allowed_variable("level", EXPRESSION_TYPE_INTEGER,
                        (: variable_player_level :) );
   add_allowed_variable("guild", EXPRESSION_TYPE_STRING,
                        (: variable_player_guild :) );
   add_allowed_variable("deity", EXPRESSION_TYPE_STRING,
                        (: variable_player_deity :) );
   add_allowed_variable("family", EXPRESSION_TYPE_STRING,
                        (: variable_player_family :) );
   add_allowed_variable("order", EXPRESSION_TYPE_STRING,
                        (: variable_player_order :) );
   add_allowed_variable("objects", EXPRESSION_TYPE_OBJECT +
                                   EXPRESSION_TYPE_ARRAY_OFFSET,
                        (: variable_objects :) );
   add_allowed_function("objectenchant", EXPRESSION_TYPE_INTEGER,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET }),
                        (: function_object_enchant :) );
   add_allowed_function("objectvalue", EXPRESSION_TYPE_MONEY,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET }),
                        (: function_object_value :) );
   add_allowed_function("objectbasevalue", EXPRESSION_TYPE_MONEY,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET }),
                        (: function_object_base_value :) );
   add_allowed_function("objectcondition", EXPRESSION_TYPE_INTEGER,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET }),
                        (: function_object_condition :) );
   add_allowed_function("objecttype", EXPRESSION_TYPE_STRING,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET }),
                        (: function_object_type :) );
   add_allowed_function("objectwarded", EXPRESSION_TYPE_OBJECT + EXPRESSION_TYPE_ARRAY_OFFSET,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET }),
                        (: function_object_warded :) );
   add_allowed_function("objectstolen", EXPRESSION_TYPE_OBJECT + EXPRESSION_TYPE_ARRAY_OFFSET,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET }),
                        (: function_object_stolen :) );
   add_allowed_variable("salecost", EXPRESSION_TYPE_MONEY,
                        (: variable_sale_cost :) );
   add_allowed_variable("salename", EXPRESSION_TYPE_STRING,
                        (: variable_sale_name :) );
   add_allowed_variable("salencategory", EXPRESSION_TYPE_STRING,
                        (: variable_sale_category :) );
   add_allowed_function("charges", EXPRESSION_TYPE_INTEGER,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET }),
                        (: function_object_charges :) );
   add_allowed_function("clubmember", EXPRESSION_TYPE_BOOLEAN,
                        ({ EXPRESSION_TYPE_STRING }),
                        (: function_club_member :) );
   add_allowed_function("containsspell", EXPRESSION_TYPE_OBJECT + EXPRESSION_TYPE_ARRAY_OFFSET,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET,
                           EXPRESSION_TYPE_STRING }),
                        (: function_object_contains_spell :) );
   add_allowed_function("containsimbue", EXPRESSION_TYPE_OBJECT + EXPRESSION_TYPE_ARRAY_OFFSET,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET,
                           EXPRESSION_TYPE_STRING }),
                        (: function_object_contains_imbue :) );
   add_allowed_function("percentageliquid", EXPRESSION_TYPE_INTEGER,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET,
                           EXPRESSION_TYPE_STRING }),
                        (: function_object_percentage_liquid :) );
   add_allowed_function("objectmatch", EXPRESSION_TYPE_OBJECT + EXPRESSION_TYPE_ARRAY_OFFSET,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET,
                           EXPRESSION_TYPE_STRING }),
                        (: function_object_matching :) );
   add_allowed_function("objectshort", EXPRESSION_TYPE_OBJECT + EXPRESSION_TYPE_ARRAY_OFFSET,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET,
                           EXPRESSION_TYPE_STRING,
                           EXPRESSION_TYPE_BOOLEAN }),
                        (: function_object_short :) );
   add_allowed_function("inventorynumber", EXPRESSION_TYPE_INTEGER,
                        ({ EXPRESSION_TYPE_STRING, }),
                        (: function_inventory_number :) );
   add_allowed_function("contents", EXPRESSION_TYPE_OBJECT +
                                    EXPRESSION_TYPE_ARRAY_OFFSET,
                        ({ EXPRESSION_TYPE_OBJECT }),
                        (: function_contents :) );
} /* create() */

/**
 * This method strips off enclosing quotation marks, if any
 */
private string strip_quotes(string str)  {
   if (str)  {
      if (str[0] == '\"')  str = str[1 .. ];
      if (str[<1] == '\"')  str = str[0 .. <2];
   }
   return str;
}


/**
 * This method sets the maximum inventory size for the shop.
 * @param size the maximum inventory size
 */
void set_maximum_waiting_queue_size(int size) {
   _max_items_in_queue = size;
} /* set_maximum_waiting_queue_size() */

/**
 * This method returns the maximum inventory size for the shop.
 * @return the maximum inventory size
 */
int query_maximum_waiting_queue_size() {
   return _max_items_in_queue;
} /* query_maximum_waiting_queue_size() */

/**
 * This method sets the maximum inventory size for the shop.
 * @param size the maximum inventory size
 */
void set_maximum_inventory_size(int size) {
   _maximum_inventory = size;
} /* set_maximum_inventory_size() */

/**
 * This method returns the maximum inventory size for the shop.
 * @return the maximum inventory size
 */
int query_maximum_inventory_size() {
   return _maximum_inventory;
} /* query_maximum_inventory_size() */

/**
 * This method sets the parcel post to use for rejecting items.
 * @param parcel the parcel post office
 */
void set_parcel_post(string parcel) {
   _parcel_post = parcel;
} /* set_parcel_post() */

/**
 * This method tells us the current parcel post used for rejecting items.
 * @return the current parcel post
 */
string query_parcel_post() {
   return _parcel_post;
} /* query_parcel_post() */

/**
 * This method sets the name of the shop.
 * @param name the name of the shop
 */
void set_shop_name(string name) {
   _shop_name = name;
   add_property("determinate", "");
   set_short(_shop_name);
} /* set_shop_name() */

/**
 * This method returns the name of the shop.
 * @return the name of the shop
 */
string query_shop_name() {
   return _shop_name;
} /* query_shop_name() */

/**
 * THis method sets the amount to round all the sell values for.  THis is
 * deal with annoying currencies like AM money which does not have a
 * '1' item.
 * @param round the price to round at
 */
void set_round_price(int round) {
   _round_value = round;
} /* set_round_price() */

/**
 * This method returns the amount to round all sell values for.
 * @return the round price
 */
int query_round_prices() {
   return _round_value;
} /* query_round_prices() */

/**
 * This method returns the current approval list for the shop.
 */
class approval_obs* query_approval_list(int approved) {
   if (approved) {
      return _approved;
   } else {
      return _waiting_for_approval;
   }
} /* query_approval_list() */

/**
 * This method returns the number of items currently in the shop by
 * the specified person.  This includes items waiting for approval.
 */
int query_number_of_items_listed(string name) {
   int num;
   class approval_obs bing;

   name = lower_case(name);
   num = sizeof(query_controller()->query_owner_sellables(name));
   foreach (bing in _waiting_for_approval) {
      if (lower_case(bing->seller) == name) {
         num += sizeof(bing->saved);
      }
   }
   return num;
} /* query_number_of_items_listed() */

/**
 * This method returns the royalties for the specific person.
 * @param person the name of the person to get the royalties of
 * @return the the royalties
 */
int query_royalty(string name) {
   return query_controller()->query_royalty(name);
} /* query_royalty() */

/**
 * This method adjusts the royalty.
 * @param person the person to adjust
 * @param amt the amoutn to adjust by
 */
void adjust_royalty(string name, int amount) {
   query_controller()->adjust_royalty(name, amount);
} /* adjust_royalty() */

/**
 * This method checks to see if the specified item is approved.
 * @param seller the name of the seller
 * @param value the amount it is sold for
 * @param name the name of the item being solde
 * @param sellables the sold items
 * @param category the category of the sold item
 * @param final_cost the final cost of the item
 * @return 1 if is approved, 0 if not, -1 if denied, string if it is approved
 *         with a list name
 */
mixed is_item_approved(string seller, int value, string name, object* sellables,
                     string category, int ref final_cost) {
   string short;
   object ob;
   int item_approval;
   class expression_type bing;
   class parse_node frog;
   int cost;
   mixed item;

   final_cost = ((value * (100 + _automatic_percentage)) / 100);

   value = value - (cost % _round_value);

   if (is_allowed(seller)) {
      // Owners sell for the exact amount the specified.
      final_cost = value;
      return 1;
   }

   //
   // This is the value we don't automatically accept at.
   //
   seller = lower_case(seller);
   if (_sellers[seller] &&
       _sellers[seller]->deny_value_limit) {
      if (_sellers[seller]->deny_value_limit < value) {
         return -1;
      }
   } else if (_automatic_approval->high_cost_deny &&
              _automatic_approval->high_cost_deny <= value) {
      return -1;
   }

   //
   // Check through the automatic approval stuff to see if we should
   // approve it.
   //
   foreach (ob in sellables) {
      if (!ob) {
         // Always deny things with broken objects references.
         return -1;
      }
      short = ob->query_short();
      if (_automatic_approval->items[short]) {
         if (classp(_automatic_approval->items[short])) {
            item = ((class approval_item)_automatic_approval->items[short])->value;
         } else {
            item = _automatic_approval->items[short];
         }
         if (pointerp(item)) {
            frog = evaluate_expression(item,
                                     seller,
                                     value,
                                     ({ ob }),
                                     name,
                                     category);
            cost = frog->value - (frog->value % _round_value);
            if (cost < value) {
               item_approval = 0;
               break;
            }
            item_approval++;
         } else if (item < value) {
            item_approval = 0;
            break;
         } else {
            item_approval++;
         }
      }
   }

   if (item_approval) {
      if (classp(_automatic_approval->items[short]))  {
         class approval_item womble;

         womble = _automatic_approval->items[short];
         if (sizeof(womble) == 3) {
            _automatic_approval->items[short] = new(class approval_item,
                   markup : womble->markup,
                   value : womble->value,
                   list_name : womble->list_name,
                   final_cost : 0);
            womble = _automatic_approval->items[short];
         }
         if (womble->markup) {
            final_cost = ((value * (100 + womble->markup)) / 100);
         } else if (womble->final_cost) {
            final_cost = womble->final_cost;
         }
         if (womble->list_name) {
            return womble->list_name;
         } else {
            return 1;
         }
      } else {
         return 1;
      }
   }

   //
   // Then check any setup expressions.
   //
   if (!pointerp(_automatic_approval->expressions)) {
      _automatic_approval->expressions = ({ });
   }
   foreach (bing in _automatic_approval->expressions) {
      if (evaluate_expression(bing->condition,
                              seller,
                              value,
                              sellables,
                              name,
                              category)->value) {
         if (bing->type == PLAYER_CRAFT_SHOP_EXPR_ACCEPT) {
            frog = evaluate_expression(bing->value,
                                 seller,
                                 value,
                                 sellables,
                                 name,
                                 category);
            cost = frog->value - (frog->value % _round_value);
            if (cost >= value) {
               if (bing->list_name) {
                  return bing->list_name;
               }
               else if (classp(_automatic_approval->items[short]))  {
                  return ((class approval_item)_automatic_approval->items[short])->list_name;
               }
               else return 1;
            }
         } else {
            return -1;
         }
      }
   }

   if (_sellers[seller] &&
       _sellers[seller]->value_limit) {
      if (_sellers[seller]->value_limit < value) {
         return 0;
      } else if (classp(_automatic_approval->items[short]))  {
         return ((class approval_item)_automatic_approval->items[short])->list_name;
      }
      else return 1;
   } else if (_automatic_approval->high_cost &&
              _automatic_approval->high_cost < value) {
      return 0;
   }

   if (_automatic_approval->low_cost &&
       _automatic_approval->low_cost >= value) {
      if (classp(_automatic_approval->items[short]))  {
         return ((class approval_item)_automatic_approval->items[short])->list_name;
      }
      else return 1;
   }

   return 0;
} /* is_item_approved() */

/**
 * This method checks to see if the shop is a closed shop and then checks
 * to see if the person is allowed.
 * @param person the person to check to see if they are allowed
 * @return 1 if they are allowed, 0 if not
 */
int is_allowed_to_use_shop(string person) {
   class parse_node frog;

   if (is_allowed(person) || !sizeof(_use_expression)) {
      return 1;
   }

   frog = evaluate_expression(_use_expression, person, 0, ({ }), 0, 0);
   return frog->value;
} /* is_allowed_to_use_shop() */

/**
 * @ignore yes
 */
int check_open(object player, string type) {
   if (!::check_open(player)) {
      return 0;
   }

   // Make sure that can only sell stuff if the waiting list is not full.
   if (type == "sell") {
      if (sizeof(query_approval_list(0)) + sizeof(query_approval_list(1)) >= _max_items_in_queue) {
         add_failed_mess("Unable to sell things, the shop is full.\n");
         return 0;
      }
   }

   if (member_array(player->query_name(), _black_list) != -1) {
      add_failed_mess("You are not allowed to use this shop.\n");
      return 0;
   }

   if (!is_allowed_to_use_shop(player->query_name())) {
      add_failed_mess("The shop is currently closed.\n");
      return 0;
   }

   return 1;
} /* check_open() */

/**
 * @ignore yes
 */
void set_controller(string name) {
   ::set_controller(name);
   query_controller()->set_dont_use_name(1);
} /* set_controller() */

/**
 * @ignore yes
 */
int is_allowed_to_sell(object *obs, string name, string *sellable_names) {
   int num;
   int max;

   if (_automatic_approval->num_allowed ||
       _sellers[name]) {
      num = query_number_of_items_listed(name) + sizeof(obs);
      if (_sellers[name] && _sellers[name]->max_sellable) {
         max = _sellers[name]->max_sellable;
      } else {
         max = _automatic_approval->num_allowed;
      }

      if (num > max) {
         add_failed_mess("You cannot have more than " + max + " items "
                         "listed at " + the_short() + ", with this sale you "
                         "would have " +
                         num + ".\n");
         return 0;
      }
   }

   if (member_array(lower_case(name), _black_list) != -1) {
      add_failed_mess("You are not allowed to sell things here.\n");
      return 0;
   }

   //
   // Otherwise see if we are over the shops total maximum size.
   //
   num = sizeof(query_controller()->query_sell_list_obs());
   if (num + sizeof(obs) > query_maximum_inventory_size()) {
      add_failed_mess("The shop is full, it can only hold " +
                      query_maximum_inventory_size() + " and it currently "
                      "holds " + num + ".\n");
      return 0;
   }
   return 1;
} /* is_allowed_to_sell() */

/**
 * This method finds out the maximum value that this item is allowed
 * to be sold for based on the current settings of the shop.
 * @param person the person doing the selling
 * @param items the item being sold
 * @return ({ queue limit, deny limit })
 */
int* query_maximum_sale_value_both(string person, object item) {
   int max_deny;
   int max_accept;
   string short;
   class expression_type stuff;
   mixed app_item;

   if (_sellers[person]) {
      if (_sellers[person]->value_limit) {
         max_accept = _sellers[person]->value_limit;
      }
      if (_sellers[person]->deny_value_limit) {
         max_deny = _sellers[person]->deny_value_limit;
      }
   }
   if (!max_accept) {
      max_accept = _automatic_approval->high_cost;
   }
   if (!max_deny) {
      max_deny = _automatic_approval->high_cost_deny;
   }

   short = item->query_short();
   if (_automatic_approval->items[short]) {
      if (classp(_automatic_approval->items[short]))  {
         app_item = ((class approval_item)_automatic_approval->items[short])->value;
      } else {
         app_item= _automatic_approval->items[short];
      }
      if (pointerp(app_item)) {
         max_accept = evaluate_expression(app_item,
                                        person, 0, ({ item }), 0, 0)->value;
      } else if (app_item < max_deny) {
         max_accept = app_item;
      }
   }

   foreach (stuff in _automatic_approval->expressions) {
      if (evaluate_expression(stuff->condition, person, 0, ({ }), 0, 0)->value) {
         if (stuff->type == PLAYER_CRAFT_SHOP_EXPR_DENY) {
            max_deny = evaluate_expression(stuff->value, person, 0, ({ item }), 0, 0)->value;
         } else {
            max_accept = evaluate_expression(stuff->value, person, 0, ({ item }), 0, 0)->value;
         }
      }
   }

   max_accept -= max_accept % _round_value;
   max_deny -= max_deny % _round_value;

   return ({ max_accept, max_deny });
} /* query_maximum_sale_value() */

/**
 * This method will return the maximum value at which the set of items
 * will be denied.
 * @param person the person selling the item
 * @param obs the set of objects
 * @return the deny value
 */
int query_maximum_sale_value(string person, object* obs) {
   int* stuff;
   object ob;
   int cur_deny;

   foreach (ob in obs) {
      stuff = query_maximum_sale_value_both(person, ob);
      if (!cur_deny && stuff[1]) {
         cur_deny = stuff[1];
      } else if (cur_deny && cur_deny > stuff[1]) {
         cur_deny = stuff[1];
      }
   }
   return cur_deny;
} /* query_maximum_sale_value() */

/** @ignore yes */
string query_extra_price_information(string seller, object *obs) {
   int* max;
   int* max_tmp;
   string ret;
   string place;
   object ob;

   max = ({ 0, 0 });
   foreach (ob in obs) {
      max_tmp = query_maximum_sale_value_both(seller, ob);
      if (max_tmp[0] && max[0] > max_tmp[0]) {
         max[0] = max_tmp[0];
      }
      if (max_tmp[1] && max[1] > max_tmp[1]) {
         max[1] = max_tmp[1];
      }
   }

   ret = "";
   place = query_property("place");
   if (!place) {
      place = "default";
   }
   if (max[0]) {
      ret += "the maximum price you can sell this straight into the "
             "inventory for is " +
             MONEY_HAND->money_value_string(max[0], place);
   }
   if (max[1]) {
      if (max[0]) {
         ret += " and ";
      }
      ret += "the maximum price you can sell the item for at all is " +
             MONEY_HAND->money_value_string(max[1], place);
   }
   return capitalize(ret) + ".\n";
} /* query_extra_price_information() */


/**
 * This method creates a save file for the specified objects autoloading
 * capability.  If the number to write to is non-null then it will
 * write to that object.
 * @param ob the object to get an autoload number for
 * @param fixed_num the file number to write to
 * @return the auto load number
 * @see save_it()
 */
protected int create_auto_load_file(mixed ob, int fixed_num) {
   mixed *auto_load;
   string tmp;

   if (objectp(ob)) {
      if (this_player()) {
         catch(auto_load = this_player()->create_auto_load(({ ob })));
      }
      if (!auto_load) {
         catch(auto_load = PLAYER_OB->create_auto_load(({ ob })));
      }
      if (!auto_load) {
         // Make it error on the last one...
         auto_load = AUTO_LOAD_OB->create_auto_load(({ ob }));
      }
   } else if (pointerp(ob)) {
      auto_load = ob;
   } else {
      printf("Error!  Dammit!\n");
   }
   if (!fixed_num) {
      do {
         tmp = do_read_file(CRAFT_SHOP_DATA_SAVE_FILE,
                        "app_" + _current_save_num);
         if (tmp) {
            _current_save_num++;
         }
      } while (tmp);
      fixed_num = _current_save_num;
   }
   do_save_file(CRAFT_SHOP_DATA_SAVE_FILE,
            auto_load,
            "app_" + fixed_num);
   return fixed_num;
} /* create_auto_load_file() */

/**
 * This method creates a real object from the save file number.
 * @param num the save file number
 * @return the nice shiny new object
 * @see create_auto_load_file()
 */
protected object create_real_auto_load_object(int num, object player) {
   string auto_load;
   object *obs;

   auto_load = do_read_file(CRAFT_SHOP_DATA_SAVE_FILE,
                        "app_" + num);
   if (userp(player)) {
      obs = player->load_auto_load_to_array(auto_load, player);
   } else if (this_player()) {
      obs = this_player()->load_auto_load_to_array(auto_load, player);
   } else {
      obs = PLAYER_OB->load_auto_load_to_array(auto_load, player);
   }
   if (sizeof(obs)) {
      return obs[0];
   }
   return 0;
} /* create_object_from_auto_load() */

/**
 * This method turns all the specified id into an item to be checked.
 * @param id the id of the object to create
 * @param player the player to use for autoloading
 */
object create_checkout_object(class approval_obs approve,
                            int id,
                            object player) {
   object ob;
   int i;

   ob = create_real_auto_load_object(id, player);
   if (ob) {
      ob->add_effect("/std/effects/object/no_save");
      ob->add_property(PLAYER_CRAFT_SHOP_CHECK_PROP, ({ approve, id }));
      i = member_array(id, approve->saved);
      approve->checkout[i] = 1;
      event_save(this_object());
      _something_checkedout++;
   }
   return ob;
} /* create_checkout_object() */

/**
 * This method updates the checked out item and changes to to be what
 * we now have...
 * @param ob the object to destroy
 */
int destroy_checkout_object(object ob) {
   int id;
   int *ids;
   int i;
   class approval_obs approve;

   if (!ob) {
      return 0;
   }

   if (ob->move("/room/rubbish") != MOVE_OK) {
      return 0;
   }
   //
   // Zap the effect too.
   //
   ids = ob->effects_matching("/std/effects/object/no_save"->query_classification());
   foreach (id in ids) {
      ob->delete_effect(id);
   }

   id = ob->query_property(PLAYER_CRAFT_SHOP_CHECK_PROP)[1];
   approve = ob->query_property(PLAYER_CRAFT_SHOP_CHECK_PROP)[0];
   i = member_array(id, approve->saved);
   approve->checkout[i] = 0;

   ob->remove_property(PLAYER_CRAFT_SHOP_CHECK_PROP);
   create_auto_load_file(ob, id);
   event_save(this_object());
   _something_checkedout--;
   if (_something_checkedout < 0) {
       _something_checkedout = 0;
   }
   return 1;
} /* destroy_checkout_object() */

/**
 * This method checks to see if the specified item is a checked out
 * item.
 * @param ob the object to check
 * @return 1 if it is, 0 if not
 */
int is_checkout_object(object ob) {
   return ob->query_property(PLAYER_CRAFT_SHOP_CHECK_PROP);
} /* is_checkout_object() */

/**
 * This method adds an item to the list of items that need to be approved.
 * @param seller name of the player
 * @param sellables the list of items to sell
 * @param value the cost of the item
 * @param name the name of the items
 * @param category the category of the item
 * @param approved a flag to say if this was approved or not
 * @return obs the objects we bought
 */
object* add_to_approval_list(string seller, object* sellables, int value,
                          string name, string category, int approved) {
   class approval_obs stuff;
   object ob;
   object* obs;

   obs = ({ });
   foreach (stuff in query_approval_list(approved)) {
      if (lower_case(stuff->name) == lower_case(name) &&
          stuff->seller == seller &&
          stuff->value == value) {
         stuff->category = category;
         foreach (ob in sellables) {
            if (ob->move("/room/rubbish") == MOVE_OK) {
               stuff->saved += ({ create_auto_load_file(ob, 0) });
               stuff->checkout += ({ 0 });
               obs += ({ ob });
            }
         }
         if (sizeof(obs)) {
            event_save(this_object());
         }
         return obs;
      }
   }


   stuff = new(class approval_obs);
   stuff->seller = seller;
   stuff->value = value;
   stuff->name = name;
   stuff->category = category;
   stuff->saved = ({ });
   stuff->enter_time = time();
   foreach (ob in sellables) {
      if (ob->move("/room/rubbish") == MOVE_OK) {
         stuff->saved += ({ create_auto_load_file(ob, 0) });
         obs += ({ ob });
      }
   }
   stuff->checkout = allocate(sizeof(stuff->saved));
   if (sizeof(obs)) {
      if (approved) {
         _approved += ({ stuff });
      } else {
         _waiting_for_approval += ({ stuff });
      }
      event_save(this_object());
   }
   return obs;
} /* add_to_approval_list() */

/**
 * This method removes the item from the approval list.
 * @param approve the item to remove
 */
void remove_from_approval_list(class approval_obs approve) {
   int i;
   int pos;

   for (i = 0; i < sizeof(_approved); i++) {
      if (_approved[i] == approve) {
         //
         // Zap the files...
         //
         foreach (pos in approve->saved) {
            do_save_file(CRAFT_SHOP_REMOVE_DATA_SAVE_FILE,
                         0,
                         "app_" + pos);
         }
         _approved = _approved[0..i-1] + _approved[i+1..];

         event_save(this_object());
         return ;
      }
   }
   for (i = 0; i < sizeof(_waiting_for_approval); i++) {
      if (_waiting_for_approval[i] == approve) {
         //
         // Zap the files...
         //
         foreach (pos in approve->saved) {
            do_save_file(CRAFT_SHOP_REMOVE_DATA_SAVE_FILE,
                         0,
                         "app_" + pos);
         }
         _waiting_for_approval = _waiting_for_approval[0..i-1] + _waiting_for_approval[i+1..];

         event_save(this_object());
         return ;
      }
   }
} /* remove_from_approval_list() */

/**
 * This method adds in transaction into the transaction list.
 * @param person the person doing the thing
 * @param objects the string name of the objects
 * @param type the type of the transaction
 * @param amount the cost of the transaction
 * @param name_cat the name/category of the item
 * @param extra anything extra about the transaction
 */
void add_transaction(string person, object* objects, int type, int amount,
                     string name_cat, mixed extra) {
#if USE_TRANSACTIONS
   class shop_transaction bing;
   string str;

   str = query_multiple_short(objects, 0, 1);

   bing = new(class shop_transaction);
   bing->time = time();
   bing->person = person;
   bing->objects = str;
   bing->type = type;
   bing->amount = amount;
   bing->name_cat = name_cat;
   bing->extra = extra;
   _transactions += ({ bing });
   event_save(this_object());
#endif
} /* add_transaction() */

#if USE_TRANSACTIONS
/**
 * This method truns a transaction into a string.
 * @param trans the transaction
 * @return the transaction as a string
 */
string query_transaction_string(class shop_transaction trans) {
   string ret;
   string amt;
   string place;

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   amt = MONEY_HAND->money_value_string(trans->amount, place);
   ret = ctime(trans->time)[4..<9] + ": " + trans->person;
   switch (trans->type) {
   case PLAYER_CRAFT_SHOP_SELL :
      ret += " sells " + trans->objects + " (" + trans->name_cat +
             ") for " + amt;
      break;
   case PLAYER_CRAFT_SHOP_SELL_AUTO :
      ret += " sells " + trans->objects + " (" + trans->name_cat +
             ") [accepted] for " + amt;
      break;
   case PLAYER_CRAFT_SHOP_APPROVE :
      ret += " approves " + trans->objects + " (" + trans->name_cat +
             ") sold by " + trans->extra + " for " + amt;
      break;
   case PLAYER_CRAFT_SHOP_BUY :
      ret += " buys " + trans->objects + " (" + trans->name_cat +
             ") sold by " + trans->extra + " for " + amt;
      break;
   }
   return ret;
} /* query_transaction_string() */
#endif

/**
 * This method is called to complete the sale completely.  It is split
 * up into a second function to allow the extra sell stuff
 * to work neatly.
 * @param value the value of the objects to sell
 * @param name the list name
 * @param sellable the list of objects to sell
 * @param category the category of the object, 0 if none
 * @ignore yes
 */
protected void complete_sale(int value, string name, object *sellable,
                             string category) {
   string place;
   object *bought;
   mixed approve;
   int final_cost;

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   //
   // Make sure we mess with the price before we do any of the approval
   // things.
   //
   _num_checked_so_far = ([ ]);
   approve = is_item_approved(this_player()->query_name(),
                        value, name, sellable, category, ref final_cost);
   if (approve == -1) {
      write("This item is denied sale here, it is not something the "
            "owner wishes to buy.\n");
      say(this_player()->query_cap_name() + " finished attempting to sell " +
                query_multiple_short(sellable) + " to " + the_short() + ".\n");
      return ;
   }

   if (approve == 1 || stringp(approve)) {
      if (stringp(approve)) {
         name = approve;
      }
   }

   write("Waiting for approval from the shop owner for the items " +
         query_multiple_short(sellable) + " with the name '" +
         name + "' selling for " +
         MONEY_HAND->money_value_string(value, place) +
         " in category " + category + ".\nIf the item is accepted the "
         "money will be paid into your royalty pool.\n");

   bought = add_to_approval_list(this_player()->query_cap_name(), sellable,
                        value, name, category, approve != 0);
   if (sizeof(bought)) {
      add_transaction(this_player()->query_name(),
                   bought,
                   PLAYER_CRAFT_SHOP_SELL,
                   value * sizeof(sellable),
                   name + " in " + category,
                   0);
      if (sizeof(bought) != sizeof(sellable)) {
         write("Unable to sell " + query_multiple_short(sellable - bought) +
                  " to " + the_short() + ".\n");
      }
      say(this_player()->the_short() + " completes selling something "
       "to the shop.\n");
   } else {
      write("Unable to sell " + query_multiple_short(sellable) + " to " +
            the_short() + ".\n");
   }
} /* complete_sale() */

/**
 * This method returns the position as a two letter value.
 * @param pos the position to mangle
 * @return the letter value
 */
string query_letter_value(int pos) {
   return sprintf("%c%c", 'A' + (pos / 26), 'A' + (pos % 26));
} /* query_letter_value() */

/**
 * This method goes from a letter value to a number.
 * @param letter the letters
 * @return the number
 */
int query_number_value(string letter) {
   if (!strlen(letter)) {
      return -1;
   }

   letter = lower_case(letter);
   if (strlen(letter) < 2) {
      if (letter[0] >= 'a' && letter[0] <= 'z') {
         return letter[0] - 'a';
      }
   }
   if (strlen(letter) > 2) {
      return -1;
   }
   if (letter[0] >= 'a' && letter[0] <= 'z') {
      if (letter[1] >= 'a' && letter[1] <= 'z') {
         return (letter[0] - 'a') * 26 + (letter[1] - 'a');
      }
   }
   return -1;
} /* query_number_value() */

/**
 * This method returns the approval class from the string name.  THis is
 * assumed to be called from inside a command.
 * @param name the string name
 * @param approved from the approved or waiting list
 * @return the approval object
 */
class approval_obs query_approval_class(string name) {
   int pos;

   if (name[0] == '-') {
      pos = query_number_value(name[1..]);
   } else {
      pos = query_number_value(name);
   }
   if (pos == -1) {
      add_failed_mess(name + " is not a valid number.\n");
      return 0;
   }

   if (name[0] == '-') {
      if (!sizeof(query_approval_list(0))) {
         add_failed_mess("There is nothing in the approval list currently.\n");
         return 0;
      }
      if (pos >= sizeof(query_approval_list(0))) {
         add_failed_mess("The " + name + " is out of range, must be between AA-" +
                         query_letter_value(sizeof(query_approval_list(0)) - 1) +
                         ".\n");
      }

      return query_approval_list(0)[pos];
   }

   if (pos >= sizeof(query_approval_list(1))) {
      add_failed_mess("The " + name + " is out of range, must be between AA-" +
                      query_letter_value(sizeof(query_approval_list(1)) - 1) +
                      ".\n");
      return 0;
   }

   return query_approval_list(1)[pos];
} /* query_approval_class() */

/** @ignore yes */
int ownership_change(string old_owner, string new_owner) {
   class parse_node* expr;

   if (old_owner == new_owner) {
      return 0;
   }

   //
   // Setup all the defaults.
   //
   _automatic_approval = new(class approval);
   //_automatic_approval->sellers = ({ });
   //_automatic_approval->categories = ([ ]);
   _automatic_approval->items = ([ ]);
   _automatic_approval->expressions = ({ });
   _automatic_percentage = 10;
#if USE_TRANSACTIONS
   _transactions = ({ });
#endif
   _sell_stats = ([ ]);
   _stats_start = time();
   _sellers = ([ ]);
   _black_list = ({ });
   expr = parse_boolean_string("false");
   _buy_expression = expr;
   _use_expression = expr;
} /* ownership_change() */

/** @ignore yes */
void inform_of_buy(int value, object *obs, object player, string *sellers,
                   string *names, string *cats, int *values) {
   string name_cat;
   int i;
   class shop_stats stat;

   _total_ingoing += value;
   adjust_royalty(query_owner(), value);
   for (i = 0; i < sizeof(obs); i++) {
      name_cat = names[i] + " in " + cats[i];
      add_transaction(this_player()->query_name(),
                      obs[i..i],
                      PLAYER_CRAFT_SHOP_BUY,
                      values[i],
                      name_cat,
                      sellers[i]);
      stat = _sell_stats[sellers[i] + " - " + name_cat];
      if (!stat) {
         stat = new(class shop_stats);
      }
      stat->num_sold++;
      stat->value_sold += values[i];
      _sell_stats[sellers[i] + " - " + name_cat] = stat;
   }
} /* inform_of_buy() */

/** @ignore yes */
object* check_for_checkout(object ob) {
   object* obs = ({ });

   if (_something_checkedout) {
      //
      // Give them a look.
      //
      if (living(ob)) {
         obs = filter(deep_inventory(ob), (: is_checkout_object($1) :));
      } else {
         if (is_checkout_object(ob)) {
            obs = ({ ob });
         }
      }
      if (sizeof(obs)) {
         foreach (ob in obs) {
            if (!destroy_checkout_object(ob)) {
               obs -= ({ ob });
            }
         }
      }
      return obs;
   }
   return ({ });
} /* check_for_checkout() */

/** @ignore yes */
void event_exit(object ob, string message, object to) {
   object *obs;

   obs = check_for_checkout(ob);
   if (sizeof(obs)) {
      tell_object(ob, "You suddenly find the uncheckout items " +
                      query_multiple_short(obs) + " check themselves "
                      "back in.\n");
      obs->move("/room/rubbish");
   }
} /* event_exit() */

/** @ignore yes */
void event_dest_me(object ob) {
   check_for_checkout(ob);
   if (_sign_ob) {
      _sign_ob->dest_me();
   }
} /* event_dest_me() */

/**
 * This method returns all the checkedout objects to the shop.
 */
void return_all_checkedout_objects() {
   object ob;
   object* obs;

   if (_something_checkedout) {
      obs = filter(deep_inventory(this_object()), (: is_checkout_object($1) :));
      if (sizeof(obs)) {
         foreach (ob in obs) {
            destroy_checkout_object(ob);
         }
      }
      tell_room(this_object(), query_multiple_short(obs) +
                " mysteriously check themselves back in.\n");
   }
} /* return_all_checkedout_objects() */

/** @ignore yes */
void dest_me() {
   ::dest_me();
} /* dest_me() */

/**
 * @ignore yes
 * In a player run shop we change this so that only allowed people can change
 * an item once it is sold.
 */
int is_able_to_change(object ob) {
   return is_allowed(this_player()->query_name());
} /* is_able_change_ob() */

/**
 * This method returns the current level of the owners money.
 */
int query_owners_money() {
   return query_royalty(query_owner());
} /* query_owners_money() */

/**
 * This method changes the current level of the owners money.
 */
void adjust_owners_money(int amt) {
   adjust_royalty(query_owner(), amt);
} /* adjust_owners_money() */

/**
 * This method returns the current sign object for the shop.
 * @return the current sign object
 */
object query_sign_object() {
   object* obs;
   if (!_sign_ob && _auto_load_sign_str && !_broken_sign) {
      if (catch(obs = PLAYER_OB->load_auto_load_to_array(_auto_load_sign_str))) {
         _broken_sign = 1;
      } else {
         if (sizeof(obs)) {
            _sign_ob = obs[0];
            _sign_ob->reset_get();
         } else {
            _broken_sign = 1;
         }
      }
   }
   return _sign_ob;
} /* query_sign_object() */

/**
 * This method sets the current sign object.
 * @param sign the new sign object
 */
void set_sign_object(object ob) {
   if (ob) {
      _auto_load_sign_str = PLAYER_OB->create_auto_load(({ ob }));
      ob->reset_get();
   } else {
      _auto_load_sign_str = 0;
   }
   _sign_ob = ob;
   event_save(this_object());
} /* set_sign_object() */


/**
 * @ignore yes
 */
int do_buy(object *obs) {
   int status;
/*
   int markup;
   int i;
   int pos;
   int bing;
   mixed approve;
   object *sellables;
   object *bought;
   string name;
   class approval_obs app_obs;
 */

   if (!evaluate_expression(_buy_expression, this_player()->query_name(), 0, ({ }), 0, 0)->value &&
       !is_allowed_to_use_shop(this_player()->query_name())) {
      add_failed_mess("You cannot buy anything at this shop.\n");
      return 0;
   }
   status = ::do_buy(obs);
   if (!status)  return 0;

/*
   foreach(app_obs in query_approval_list(approved))  {
      sellables = ({ });
      for (i = 0; i < sizeof(app_obs->saved); i++) {
         pos = app_obs->saved[i];
         if (!app_obs->checkout[i]) {
            sellables += ({ create_real_auto_load_object(pos, this_player()) });
         }
         else {
            sellables->move("/room/rubbish");
            sellables = ({ });
         }
      }

      if (sizeof(sellables))  {
         _num_checked_so_far = ([ ]);
         approve = is_item_approved(lower_case(app_obs->seller), app_obs->value,
                                    app_obs->name, sellables, app_obs->category,
                                    ref markup);
      } else  {
         approve = 0;
      }

      if (approve == 1 || stringp(approve)) {
         if (stringp(approve)) {
            name = approve;
         } else  {
            name = app_obs->name;
         }

         if (markup) {
            bing = ((app_obs->value * (100 + markup)) / 100);
         } else {
            bing = ((app_obs->value * (100 + _automatic_percentage)) / 100);
         }
         bing -= bing % _round_value;

         bought = query_controller()->buy_objects(sellables, name, bing,
                                app_obs->seller, app_obs->category, 1);
         if (sizeof(bought)) {
            add_transaction(app_obs->seller, sellables,
                            PLAYER_CRAFT_SHOP_SELL_AUTO,
                            app_obs->value * sizeof(sellables),
                            name + " in " + app_obs->category, 0);
            remove_from_approval_list(app_obs);
            if (query_owner() != app_obs->seller)  {
               _total_outgoing += app_obs->value * sizeof(sellables);
               adjust_royalty(query_owner(),
                              -app_obs->value * sizeof(sellables));
               adjust_royalty(app_obs->seller,
                              app_obs->value * sizeof(sellables));
               event_save(this_object());
            }
         }
         break;
      }
   }
 */

   return 1;
} /* do_buy() */

/**
 * This method runs through the objects and checks to see what sort of levels
 * they are allowed to sell them for to this shop.
 * @return 1 on success, 0 on failure
 */
int do_check_sell(object *obs) {
   int *stuff;
   object ob;
   string ret;
   string place;

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   ret = "";
   foreach (ob in obs) {
      stuff = query_maximum_sale_value_both(this_player()->query_name(), ob);
      ret += "$I$5=$C$" + the_short() + ": ";
      if (stuff[0]) {
         ret += "will be queued for more than " +
                MONEY_HAND->money_value_string(stuff[0], place);
      }
      if (stuff[0] && stuff[1]) {
         ret += " and ";
      }
      if (stuff[1]) {
         ret += "will be denied for more than " +
                MONEY_HAND->money_value_string(stuff[1], place);
      }
      ret += ".\n";
   }
   write("$P$Check Sell$P$" + ret);
   add_succeeded_mess(({ "", "$N checks the sale of $I.\n" }), obs);
   return 1;
} /* do_check_sell() */

/**
 * List the items for approval.
 */
int do_list_approval(int approved) {
   class approval_obs approve;
   int pos;
   int shown;
   int allowed;
   int checkout;
   int i;
   int fluff;
   string place;
   string ret;
   object* obs;
   object *current;
   object* result;
   string *possible_names;
   string start;
   mapping result_type;

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   allowed = is_allowed(this_player()->query_name());
   if (!allowed) {
      approved = 1;
   }
   if (!approved) {
      start = "-";
   } else {
      start = "";
   }

   result_type = ([ ]);
   ret = "";
   current = query_controller()->create_all_real_objects(this_player(),
                 query_controller()->query_sell_list_obs());
   while (allowed || approved) {
      if (approved == 2) {
         approved = 0;
         start = "-";
      }
      foreach (approve in query_approval_list(approved)) {
         if (lower_case(approve->seller) == this_player()->query_name() ||
             allowed) {
            checkout = sizeof(filter(approve->checkout, (: $1 :)));
   
            obs = ({ });
            for (i = 0; i < sizeof(approve->saved); i++) {
               fluff = approve->saved[i];
               obs += ({ create_real_auto_load_object(fluff, this_player()) });
            }
            obs -= ({ 0 });
            possible_names = obs->query_short();
            obs->move("/room/rubbish");
            if (!result_type[lower_case(approve->name)]) {
               result = filter(current,
                        (: member_array($1->query_short(),
                                        $(possible_names)) > -1 :)); 
               result_type[lower_case(approve->name)] = sizeof(result);
            }
            ret += start + query_letter_value(pos) + ") " +
                  approve->seller + "'s " + approve->name + " for " +
                  MONEY_HAND->money_value_string(approve->value, place) +
                  " in " + approve->category + ", " +
                  sizeof(approve->saved) + " objects (" +
                  query_multiple_short(obs) + ") " +
                  result_type[lower_case(approve->name)] + " in stock";
   
            if (checkout) {
               ret += " and " + checkout + " checked out.\n";
            } else {
               ret += ".\n";
            }
            shown++;
         }
         pos++;
      }
      if (!allowed && approved) {
         approved = 2;
         pos = 0;
      } else {
         break;
      }
   }
   if (sizeof(current)) {
      current->dest_me();
   }

   if (!shown) {
      add_failed_mess("No items to approve at the moment.\n");
      return 0;
   }

   write("$P$Approval list$P$The current approval items are:\n" + ret);
   return 1;
} /* do_list_approval() */

/**
 * This method returns the item to the person that started to sell it.
 * This only works before the item has been approved.
 * @param name the name of the item to return
 */
int do_return(string name) {
   object *obs;
   object *ok;
   object *here;
   object *fail;
   object *checkout;
   object money;
   class approval_obs approve;
   object ob;
   int pos;
   int i;
   int value;
   string place;

   approve = query_approval_class(name);
   if (!approve) {
      return 0;
   }

   if (lower_case(approve->seller) != this_player()->query_name()) {
      add_failed_mess("You must be the one that sold the item to return "
                      "it.\n");
      return 0;
   }

   obs = ({ });
   checkout = ({ });
   if (sizeof(filter(approve->checkout, (: $1 :))) > 0 &&
       _something_checkedout) {
      //
      // Try and return it.
      //
      return_all_checkedout_objects();
   }

   for (i = 0; i < sizeof(approve->saved); i++) {
      pos = approve->saved[i];
      if (!approve->checkout[i]) {
         obs += ({ create_real_auto_load_object(pos, this_player()) });
      } else {
         checkout += ({ create_real_auto_load_object(pos, this_player()) });
      }
   }

   obs -= ({ 0 });
   checkout -= ({ 0 });

   if (sizeof(checkout)) {
      place = query_property("place");
      if (!place) {
         place = "default";
      }

      write("The items " + query_multiple_short(checkout) +
            " are currently checked out.  You are reimbused the amount "
            "they would have cost to sell.\n");
      value = approve->value * sizeof(checkout);
      money = MONEY_HAND->make_new_amount( value, place);
      money->move(this_player());
      _total_outgoing += approve->value * sizeof(checkout);
   }

   ok = ({ });
   here = ({ });
   fail = ({ });
   foreach (ob in obs) {
      if (ob->move(this_player()) == MOVE_OK) {
         ok += ({ ob });
      } else if (ob->move(this_object())) {
         here += ({ ob });
      } else {
         fail += ({ ob });
      }
   }
   remove_from_approval_list(approve);

   if (sizeof(ok)) {
      add_succeeded_mess("$N $V $I.\n", map(ok, (: $1->poss_short() :)));
   }
   if (sizeof(here)) {
      add_succeeded_mess("$N $V their $I and they get put on the floor.\n",
                         map(ok, (: $1->poss_short() :)));
   }
   if (sizeof(fail)) {
      add_succeeded_mess( ({ "You find that $I do not want to be moved.\n",
                             "" }), fail);
   }
   return 1;
} /* do_return() */

/**
 * This method approves the specified item.
 */
int do_approve_item(string name, string money_str) {
   int cost;
   object *obs;
   class approval_obs approve;
   string place;
   int final_cost;
   int num;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to approve items.\n");
      return 0;
   }

   approve = query_approval_class(name);
   if (!approve) {
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   if (query_owners_money() < 0) {
      add_failed_mess("You cannot do this until the shop's deficit is payed "
                      "off.\n");
      return 0;
   }

   num = sizeof(query_controller()->query_sell_list_obs());
   if (num + sizeof(approve->saved) > query_maximum_inventory_size()) {
      add_failed_mess("You cannot approve anything, the shop is already "
                      "at the maximum number allowed.\n");
      return 0;
   }

   cost = approve->value * sizeof(obs);
   if (this_player()->query_value_in(place) < cost) {
      add_failed_mess("You need at least " +
                      MONEY_HAND->money_value_string(cost, place) +
                      " to put this into the shop.\n");
      return 0;
   }

   if (money_str) {
      final_cost = MONEY_HAND->value_from_string(money_str, place);
      if (!final_cost) {
         add_failed_mess("The amount " + money_str + " is not a valid "
                         "money cost.\n");
         return 0;
      }
   }

   if (sizeof(filter(approve->checkout, (: $1 :))) > 0 &&
       _something_checkedout) {
      //
      // Try and return it.
      //
      write("One of these objects is still checkedout, do you wish to "
            "continue this without these items? ");
      input_to("confirm_approval", 0, approve, final_cost);
      add_succeeded_mess( ({ "", "$N approves something in the shop.\n" }) );
      return 1;
   }


   confirm_approval("y", approve, final_cost);
   return 1;
} /* do_approve_item() */

protected void confirm_approval(string answer,
                                class approval_obs approve,
                                int final_cost) {
   int pos;
   int i;
   int bing;
   int cost;
   object *obs;
   object *bought;
   object *checkout;
   string place;

   if (strlen(answer) < 1 || lower_case(answer)[0] != 'y') {
      write("Ok, canceled the approval of the item.\n");
      return ;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   //
   // Approval means we create all the objects, remove them, and then
   // send them off to the main engine...
   //
   obs = ({ });
   checkout = ({ });
   for (i = 0; i < sizeof(approve->saved); i++) {
      pos = approve->saved[i];
      if (!approve->checkout[i]) {
         obs += ({ create_real_auto_load_object(pos, this_player()) });
      } else {
         checkout += ({ create_real_auto_load_object(pos, this_player()) });
      }
   }

   obs -= ({ 0 });
   checkout -= ({ 0 });

   if (sizeof(checkout)) {
      write("The items " + query_multiple_short(checkout) + " were "
            "checked out and therefore lost.\n");
   }

   if (!final_cost) {
      final_cost = ((approve->value * (100 + _automatic_percentage)) / 100);
   }
   final_cost -= final_cost % _round_value;
   bought = query_controller()->buy_objects(obs, approve->name,
                                   final_cost,
                                   approve->seller, approve->category, 1);
   if (sizeof(bought)) {
      cost = approve->value * (sizeof(obs) + sizeof(checkout));
      cost -= cost % _round_value;
      _total_outgoing += cost;
      remove_from_approval_list(approve);
      adjust_royalty(lower_case(approve->seller), cost);
      //
      // Try and remove from the cash float first.
      //
      if (query_owners_money() > 0) {
         if (query_owners_money() > cost) {
            adjust_owners_money(-cost);
            cost = 0;
         } else {
            cost -= query_owners_money();
            adjust_owners_money(-query_owners_money());
         }
      }
      if (cost > 0) {
         if (this_player()->query_value_in(place) > 0) {
            bing = this_player()->query_value_in(place);
            if (bing > cost) {
               bing = cost;
               cost = 0;
            } else {
               cost -= bing;
            }
            if (bing) {
               this_player()->pay_money(MONEY_HAND->create_money_array(bing, place), place);
            }
         }
         if (cost > 0) {
            adjust_owners_money(-cost);
         }
      }
      write("You approve " + query_multiple_short(obs) + " to be sold as " +
            approve->name + " in " + approve->category + " for " +
            MONEY_HAND->money_value_string(approve->value, place) + ".\n");
      add_succeeded_mess( ({ "", "$N approves something in the shop.\n" }) );
      add_transaction(this_player()->query_name(),
                      obs,
                      PLAYER_CRAFT_SHOP_APPROVE,
                      approve->value * (sizeof(obs) + sizeof(checkout)),
                      approve->name + " in " + approve->category,
                      approve->seller);
   } else {
      write("Unable to move the items into the shops inventory.  "
            "Contact a creator or error report this please.\n");
   }
   return ;
} /* do_approve_item() */

/**
 * This method rejects the specified item.
 */
int do_approve_reject(string name, string mess, int reject_pos) {
   object *obs;
   object *ok;
   object *here;
   object *checkout;
   object ob;
   object play;
   class approval_obs approve;
   int pos;
   int i;
   int ret;
   string ob_mess;

   approve = query_approval_class(name);
   if (!approve) {
      return 0;
   }

   if (lower_case(approve->seller) == this_player()->query_name()) {
      //
      // Return the stuff to them.
      //
      return do_return(name);
   }

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You're not allowed to reject items.\n");
      return 0;
   }


   if (sizeof(filter(approve->checkout, (: $1 :))) > 0 &&
       _something_checkedout) {
      //
      // Try and return it.
      //
      return_all_checkedout_objects();
   }

   if (reject_pos != -1 &&
       (reject_pos <= 0 ||
       reject_pos > sizeof(approve->saved))) {
      add_failed_mess("You cannot reject the item of position " + reject_pos +
                      " since there are only " + sizeof(approve->saved) +
                      " items.\n");
      return 0;
   }

   if (reject_pos != -1) {
      reject_pos--;
   }

   //
   // Approval means we create all the objects, remove them, and then
   // send them off to the main engine...
   //
   obs = ({ });
   checkout = ({ });
   for (i = 0; i < sizeof(approve->saved); i++) {
      if (reject_pos == -1 ||
          reject_pos == i) {
         pos = approve->saved[i];
         if (!approve->checkout[i]) {
            obs += ({ create_real_auto_load_object(pos, this_player()) });
         } else {
            checkout += ({ create_real_auto_load_object(pos, this_player()) });
         }
      }
   }

   obs -= ({ 0 });
   checkout -= ({ 0 });

   if (member_array(0, obs) != -1) {
      add_failed_mess("There seems to be a problem with this rejection set, "
                      "one of the items is 0.\n");
      return 0;
   }

   if (reject_pos != -1 && sizeof(checkout)) {
      add_failed_mess("The item $I has been checked out, you cannot reject "
                      "it.\n", checkout);
      return 0;
   }

   if (sizeof(checkout)) {
      write("The items " + query_multiple_short(checkout) + " were "
            "checked out and lost.\n");
      adjust_royalty(lower_case(approve->seller),
                                approve->value * sizeof(checkout));
      AUTO_MAILER->auto_mail(lower_case(approve->seller),
                            this_player()->query_name(),
                            "Rejected items", "",
                            sprintf("%-=75s",
                                   this_player()->query_name() + " lost your " +
                                   query_multiple_short(checkout, "the", 1) +
                                   ",so you have been paid but the items "
                                   "were not returned.\n"));
   }

   if (!sizeof(obs)) {
      write("Nothing to give back or send off.\n");
   } else {
      //
      // They are here, so give it back.
      //
      play = find_player(lower_case(approve->seller));
      if (play  &&  environment(play) == this_object()) {
         ok = ({ });
         here = ({ });
         foreach (ob in obs) {
            if (ob) {
               if (ob->move(play) == MOVE_OK) {
                  ok += ({ ob });
               } else if (ob->move(this_object()) == MOVE_OK) {
                  here += ({ ob });
               }
            }
         }
         if (sizeof(ok)) {
            tell_object(play, "Your items: " +
                        query_multiple_short(obs) + " have been rejected "
                        "and placed in your inventory.\n");
         }
         if (sizeof(here)) {
            tell_object(play, "Your items: " +
                        query_multiple_short(obs) + " have been rejected "
                        "and placed on the floor here.\n");
         }
         write("You reject the item.\n");
      } else {
         if (!_parcel_post) {
            add_failed_mess("Eeek!  No parcel post defined on this room.\n");
            return 0;
         }

         ob_mess = "The shop " + this_player()->convert_message(the_short()) +
                   " rejected " + query_multiple_short(obs, "the", 1) +
                   " because:\n";
         ret = _parcel_post->deposit_parcel(obs, lower_case(approve->seller), 1);
         if (ret != 1) {
            if (ret == -5) {
               write("The player " + approve->seller + " has been denied use "
                     "of the parcel post system, tossing objects away.\n");
               obs->move("/room/rubbish");
               if (mess) {
                  write("Still sending the message to the person.\n");
                  mess += "\nYou were denied use of the parcel post system "
                          "so your objects were thrown away.\n";
               }
            } else if (ret == -4) {
               write("You have been denied use of the postal system, perhaps "
                     "you could try clearing this up with the creators?\n");
               obs->move("/room/rubbish");
               return 1;
            } else if (ret == -2) {
               write("The player " + approve->seller + " no longer exists, "
                     "tossing objects away.\n");
               obs->move("/room/rubbish");
            } else {
               add_failed_mess("Unable to send the parcel for some reason (" +
                            ret + ").\n");
                  obs->move("/room/rubbish");
               return 0;
            }
         }
         if (ret != -2) {
            if (mess) {
               AUTO_MAILER->auto_mail(approve->seller, this_player()->query_name(),
                               "Rejected items", "",
                               ob_mess + mess);
            } else {
               write("You reject the item and it is parcel posted back to the "
                  "person who "
                  "sent it.\nWould you like to send them a note about it as well? ");
               obs -= ({ 0 });
               input_to("check_reject_note", 0, ob_mess,
                        approve->seller);
            }
         }
      }
   }

   if (reject_pos != -1) {
      approve->saved = approve->saved[0..reject_pos-1] + approve->saved[reject_pos+1..];
      event_save(this_object());
      if (!sizeof(approve->saved)) {
         reject_pos = -1;
      }
   }

   if (reject_pos == -1) {
      remove_from_approval_list(approve);
   }
   add_succeeded_mess(({ "", "$N reject$s a sellable object.\n" }));
   return 1;
} /* do_approve_reject() */

/**
 * @ignore yes
 */
void check_reject_note(string str, string ob_str, string name) {
   str = lower_case(str);
   if (!strlen(str) ||
       (str[0] != 'y' &&
        str[0] != 'n')) {
      write("Please answer yes or no.\nWould you like to send them "
            "a note about it as well?");
      input_to("check_reject_note", 0, ob_str, name);
      return ;
   }
   if (str[0] == 'n') {
      write("ok, bye then!\n");
      return ;
   }
   this_player()->do_edit("Your items " + ob_str + " were rejected "
                         "from " + the_short() + ".\n\n",
                         "send_reject_note", this_object(), 0, name);
} /* check_reject_note() */

/** @ignore yes */
void send_reject_note(string mess, string name) {
   if (!mess) {
      write("Aborted.\n");
      return ;
   }
   //write("Sending to " + name + ".\n");
   AUTO_MAILER->auto_mail(name, this_player()->query_name(),
                            "Rejected items", "", mess);
} /* send_reject_note() */

/**
 * This method browses the specified item.
 */
int do_approve_browse(string name) {
   object *obs;
   object ob;
   string read;
   string ret;
   class approval_obs approve;
   int pos;
   string place;

   approve = query_approval_class(name);
   if (!approve) {
      return 0;
   }

   if (lower_case(approve->seller) == this_player()->query_name()) {
      //
      // Return the stuff to them.
      //
      return do_return(name);
   }

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to reject items.\n");
      return 0;
   }


   if (query_owners_money() < 0) {
      add_failed_mess("You cannot do this until the shop's deficit is payed "
                      "off.\n");
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   //
   // Create the objects, browse them, kill them.
   //
   obs = ({ });
   foreach (pos in approve->saved) {
      obs += ({ create_real_auto_load_object(pos, this_player()) });
   }

   obs -= ({ 0 });

   ret = "";
   foreach (ob in obs) {
      ret += ob->the_short() + ": (Base cost " +
             MONEY_HAND->money_value_string(ob->query_base_value(), place) +
             "; scaled cost " +
             MONEY_HAND->money_value_string(ob->query_value(), place) +
             ")\n" + ob->long();
      read = ob->query_readable_message();
      if (read) {
         ret += "You read " +
                 replace_string(ob->query_read_short(this_player()), "$name$",
                         ob->a_short()) + ":\n" +
                 ob->query_readable_message();
      }
   }

   obs->move("/room/rubbish");

   write("$P$Browse list$P$" + ret);

   add_succeeded_mess("$N browse$s an item waiting for approval.\n");
   return 1;
} /* do_approve_browse() */

/**
 * This method checks out the specified item.
 */
int do_approve_checkout(string name) {
   object *obs;
   object *bad;
   object *checkout;
   object ob;
   class approval_obs approve;
   int pos;
   int i;

   approve = query_approval_class(name);
   if (!approve) {
      return 0;
   }

   if (lower_case(approve->seller) == this_player()->query_name()) {
      //
      // Return the stuff to them.
      //
      return do_return(name);
   }

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to reject items.\n");
      return 0;
   }


   if (query_owners_money() < 0) {
      add_failed_mess("You cannot do this until the shop's deficit is payed "
                      "off.\n");
      return 0;
   }

   //
   // Create the objects, browse them, kill them.
   //
   obs = ({ });
   checkout = ({ });
   for (i = 0; i < sizeof(approve->saved); i++) {
      pos = approve->saved[i];
      if (!approve->checkout[i]) {
         ob = create_checkout_object(approve, pos, this_player());
         if (!ob) {
            write("Unable to create " + pos + approve->name + ", weird...\n");
         } else {
            obs += ({ ob });
         }
      } else {
         checkout += ({ create_checkout_object(approve, pos, this_player()) });
      }
   }

   checkout->move("/room/rubbish");

   if (!sizeof(obs) &&
       sizeof(checkout)) {
      if (sizeof(checkout) > 0) {
         add_failed_mess("$I are already checked out.\n", checkout);
      } else {
         add_failed_mess("$I is already checked out.\n", checkout);
      }
      return 0;
   }

   bad = ({ });
   foreach (ob in obs) {
      if (ob->move(this_player()) != MOVE_OK) {
         bad += ({ ob });
         destroy_checkout_object(ob);
      }
   }

   if (sizeof(bad)) {
      write("Unable to move " + query_multiple_short(bad) + " into your "
            "inventory for you to check.\n");
   }

   obs -= bad;
   if (sizeof(obs)) {
      write("Moved " + query_multiple_short(obs) + " into your inventory "
            "for you to check.\n");
      add_succeeded_mess("$N check$s out $I.\n", obs);
   }

   return 1;
} /* do_approve_checkout() */

/**
 * This method checks in a checked out object.
 */
int do_approve_checkin(object* obs) {
   object ob;
   object* ok;

   ok = ({ });
   foreach (ob in obs) {
      if (is_checkout_object(ob)) {
         destroy_checkout_object(ob);
         ok += ({ ob });
      }
   }

   if (!sizeof(ok)) {
      add_failed_mess("None of $I have been checked out.\n", obs);
      return 0;
   }

   add_succeeded_mess("$N check$s $I back in.\n", ok);
   return 1;
} /* do_approve_checkin() */

/**
 * This method changes the name of the item to be approved.
 * @param name the reference
 * @param new_name the new name for the item
 */
int do_approve_name_change(string name, string new_name) {
   class approval_obs approve;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to approve items.\n");
      return 0;
   }

   if (query_owners_money() < 0) {
      add_failed_mess("You cannot do this until the shop's deficit is payed "
                      "off.\n");
      return 0;
   }

   approve = query_approval_class(name);
   if (!approve)  return 0;
   write("You change the name of the item waiting to be approved from " +
         approve->name + " to " + new_name + ".\n");
   approve->name = new_name;
   add_succeeded_mess( ({ "", "$N messes with the approval items.\n" }));
   event_save(this_object());
   return 1;
} /* do_approve_name_change() */

/**
 * This method changes the category of the item waiting to be approved.
 * @param name the reference
 * @param new_category the new category for the item
 */
int do_approve_category_change(string name, string new_category) {
   class approval_obs approve;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to approve items.\n");
      return 0;
   }

   approve = query_approval_class(name);
   if (!approve) {
      return 0;
   }

   if (query_owners_money() < 0) {
      add_failed_mess("You cannot do this until the shop's deficit is payed "
                      "off.\n");
      return 0;
   }

   new_category = query_real_category_name(new_category);
   if (!new_category ||
       !query_controller()->is_valid_category(new_category)) {
      add_failed_mess("You must choose a category that exists.\n");
      return 0;
   }

   approve->category = new_category;
   add_succeeded_mess(({
      "You change the category of the approval item " + name + ".\n",
      "$N messes with the approval items.\n" }));
   event_save(this_object());
   return 1;
} /* do_approve_category_change() */

/**
 * This method sets the automatic low acception level.
 * @param cost the cost to use a low level
 */
int do_approve_auto_low(string cost) {
   string place;
   int value;

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to set automatic approval items.\n");
      return 0;
   }

   if (query_owners_money() < 0) {
      add_failed_mess("You cannot do this until the shop's deficit is payed "
                      "off.\n");
      return 0;
   }

   value = value_from_string(cost, place);
   _automatic_approval->low_cost = value;
   event_save(this_object());
   if (!value) {
      add_succeeded_mess(({ "You disable the automatic acception of items of "
                            "low value.\n",
                            "$N fiddle$s with something in the shop.\n" }) );
      return 1;
   }

   add_succeeded_mess(({ "You set the shop to automatically accept any item "
                         "costing less than " +
                         MONEY_HAND->money_value_string(value, place) + ".\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_auto_low() */

/**
 * This method sets the automatic high cut off level.
 * @param cost the cost to use a high cut off level
 */
int do_approve_auto_high(string cost) {
   string place;
   int value;

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to set automatic approval items.\n");
      return 0;
   }

   if (query_owners_money() < 0) {
      add_failed_mess("You cannot do this until the shop's deficit is payed "
                      "off.\n");
      return 0;
   }

   value = value_from_string(cost, place);
   if (!value) {
      _automatic_approval->high_cost = value;
      add_succeeded_mess(({ "You disable the automatic placing items into the "
                            "approve list of items of high value.\n",
                            "$N fiddle$s with something in the shop.\n" }) );
      return 1;
   }

   _automatic_approval->high_cost = value;
   event_save(this_object());
   add_succeeded_mess(({ "You set the shop to automatically put any item "
                         "costing more than " +
                         MONEY_HAND->money_value_string(value, place) +
                         " into the approve list.\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_auto_high() */

/**
 * This method sets the automatic high cut off level for denying sales.
 * @param cost the cost to use a high cut off level
 */
int do_approve_auto_high_deny(string cost) {
   string place;
   int value;

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to set automatic approval items.\n");
      return 0;
   }

   if (query_owners_money() < 0) {
      add_failed_mess("You cannot do this until the shop's deficit is payed "
                      "off.\n");
      return 0;
   }

   value = value_from_string(cost, place);
   if (!value) {
      _automatic_approval->high_cost_deny = 0;
      add_succeeded_mess(({ "You disable the automatic denying of items of "
                            "high value.\n",
                            "$N fiddle$s with something in the shop.\n" }) );
      return 1;
   }

   _automatic_approval->high_cost_deny = value;
   event_save(this_object());
   add_succeeded_mess(({ "You set the shop to automatically deny any item "
                         "costing more than " +
                         MONEY_HAND->money_value_string(value, place) + ".\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_auto_high() */

/**
 * This method sets the automatic items to allow.
 * @param item the items short to allow
 */
int do_approve_auto_item_add(string item, string money, string list_name,
                             int markup, string money_str) {
   int value;
   string place;
   int final_cost;
   mixed app_item;
   class approval_item new_app_item;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to set automatic approval items.\n");
      return 0;
   }

   if (query_owners_money() < 0) {
      add_failed_mess("You cannot do this until the shop's deficit is payed "
                      "off.\n");
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   value = value_from_string(money, place);
   if (!value) {
      add_failed_mess("The value " + money + " is invalid.\n");
      return 0;
   }

   if (markup < 0) {
      markup = 0;
   }

   item = strip_quotes(item);
   list_name = strip_quotes(list_name);

   if (classp(_automatic_approval->items[item])) {
      app_item = ((class approval_item)_automatic_approval->items[item])->value;
   } else {
      app_item = _automatic_approval->items[item];
   }
   if (app_item == value) {
      add_failed_mess("The item " + item + " already has a cut off "
                      "value of " +
                      MONEY_HAND->money_value_string(value, place) + ".\n");
      return 0;
   }

   if (money_str) {
      final_cost = MONEY_HAND->value_from_string(money_str, place);
      if (!final_cost) {
         add_failed_mess(money_str + " is an invalid money value.\n");
         return 0;
      }
   }

   new_app_item = new(class approval_item);
   new_app_item->list_name = list_name;
   new_app_item->value = value;
   new_app_item->markup = markup;
   new_app_item->final_cost = final_cost;
   _automatic_approval->items[item] = new_app_item;

   place = query_property("place");

   event_save(this_object());
   add_succeeded_mess(({ "You set the shop to automatically accept any item "
                         "with a short of " + item + " up to a cost "
                         "of " + MONEY_HAND->money_value_string(value, place) +
                         (list_name ? " as \"" + list_name + "\"" : "") +
                         (markup ? " with a markup of " + markup + "%" : "") +
                         (final_cost ? " with a final cost of " +
                           MONEY_HAND->money_value_string(final_cost, place) + "" : "") +
                         ".\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_auto_item_add() */

/**
 * This method sets the automatic items to allow.
 * @param item the items short to allow
 */
int do_approve_auto_item_add_object(object *obs, string money,
                                    string list_name, int markup,
                                    string money_str) {
   int value;
   string place;
   object ob;
   string name;
   object *ok;
   object *bad;
   mixed item;
   class approval_item new_item;
   int final_cost;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to set automatic approval items.\n");
      return 0;
   }

   if (query_owners_money() < 0) {
      add_failed_mess("You cannot do this until the shop's deficit is payed "
                      "off.\n");
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   value = value_from_string(money, place);
   if (!value) {
      add_failed_mess("The value " + money + " is invalid.\n");
      return 0;
   }

   if (money_str) {
      final_cost = MONEY_HAND->value_from_string(money_str, place);
      if (!final_cost) {
         add_failed_mess(money_str + " is an invalid money value.\n");
         return 0;
      }
   }

   if (markup < 0) {
      markup = 0;
   }
   bad = ({ });
   ok = ({ });

   list_name = strip_quotes(list_name);
   foreach (ob in obs) {
      name = ob->query_short();
      if (classp(_automatic_approval->items[name])) {
         item = ((class approval_item)_automatic_approval->items[name])->value;
      } else {
         item = _automatic_approval->items[name];
      }
      if (item == value) {
         bad += ({ name });
      } else {
         new_item = new(class approval_item);
         new_item->value = value;
         new_item->list_name = list_name;
         new_item->markup = markup;
         new_item->final_cost = final_cost;
         _automatic_approval->items[name] = new_item;
         ok += ({ name });
      }
   }
   event_save(this_object());
   if (sizeof(ok)) {
      add_succeeded_mess(({ "You set the shop to automatically accept any of "
                         "$I up to a cost "
                         "of " + MONEY_HAND->money_value_string(value, place) +
                         (list_name ? " as \"" + list_name + "\"" : "") +
                         (markup ? " with a markup of " + markup + "%" : "") +
                         (final_cost ? " with a final cost of " +
                           MONEY_HAND->money_value_string(final_cost, place) + "" : "") +
                         ".\n",
                         "$N fiddle$s with something in the shop.\n" }), ok );
      return 1;
   } else {
      add_failed_mess("You cannot set $I to a value limit of " +
                      MONEY_HAND->money_value_string(value, place) +
                      " since it already has this limit.\n");
      return 0;
   }
} /* do_approve_auto_item_add_object() */

/**
 * This method sets the automatic items to allow.
 * @param item the items short to allow
 */
int do_approve_auto_item_add_expr(string item, string expr,
                                  string list_name, int markup,
                                  string money_str) {
   class approval_item new_app_item;
   class parse_node* value;
   string place;
   int final_cost;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to set automatic approval items.\n");
      return 0;
   }

   if (query_owners_money() < 0) {
      add_failed_mess("You cannot do this until the shop's deficit is payed "
                      "off.\n");
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   value = parse_money_string(expr);
   if (!sizeof(value)) {
      add_failed_mess(query_last_expression_error() + ".\n");
      return 0;
   }

   if (money_str) {
      final_cost = MONEY_HAND->value_from_string(money_str, place);
      if (!final_cost) {
         add_failed_mess(money_str + " is an invalid money value.\n");
         return 0;
      }
   }

   if (markup < 0) {
      markup = 0;
   }

   item = strip_quotes(item);
   list_name = strip_quotes(list_name);

   new_app_item = new(class approval_item);
   new_app_item->list_name = list_name;
   new_app_item->value = value;
   new_app_item->markup = markup;
   new_app_item->final_cost = final_cost;
   _automatic_approval->items[item] = new_app_item;

   event_save(this_object());
   add_succeeded_mess(({ "You set the shop to automatically accept any item "
                         "with a short of " + item + " with an expression "
                         "of " + query_expression_string(value, 1) +
                         (list_name ? " as \"" + list_name + "\"" : "") +
                         (markup ? " with a markup of " + markup + "%" : "") +
                         (final_cost ? " with a final cost of " +
                           MONEY_HAND->money_value_string(final_cost, place) + "" : "") +
                         ".\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_auto_item_add_expr() */

/**
 * This method sets the automatic items to allow.
 * @param item the items short to allow
 */
int do_approve_auto_item_add_object_expr(object *obs, string expr,
                          string list_name, int markup, string money_str) {
   class parse_node* value;
   object ob;
   string name;
   class approval_item new_item;
   int final_cost;
   string place;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to set automatic approval items.\n");
      return 0;
   }

   if (query_owners_money() < 0) {
      add_failed_mess("You cannot do this until the shop's deficit is payed "
                      "off.\n");
      return 0;
   }

   value = parse_money_string(expr);
   if (!sizeof(value)) {
      add_failed_mess(query_last_expression_error() + ".\n");
      return 0;
   }

   if (money_str) {
      place = query_property("place");
      final_cost = MONEY_HAND->value_from_string(money_str, place);
      if (!final_cost) {
         add_failed_mess(money_str + " is an invalid money value.\n");
         return 0;
      }
   }

   if (markup < 0) {
      markup = 0;
   }
   list_name = strip_quotes(list_name);
   foreach (ob in obs) {
      name = ob->query_short();
      new_item = new(class approval_item);
      new_item->value = value;
      new_item->list_name = list_name;
      new_item->markup = markup;
      new_item->final_cost = final_cost;
      _automatic_approval->items[name] = new_item;
   }
   event_save(this_object());

   add_succeeded_mess(({ "You set the shop to automatically accept any of "
                      "$I with an expression "
                      "of " + query_expression_string(value, 1) +
                      (list_name ? " as \"" + list_name + "\"" : "") +
                         (markup ? " with a markup of " + markup + "%" : "") +
                         (final_cost ? " with a final cost of " +
                           MONEY_HAND->money_value_string(final_cost, place) + "" : "") +
                         ".\n",
                       "$N fiddle$s with something in the shop.\n" }), obs );
   return 1;
} /* do_approve_auto_item_add_object_expr */

/**
 * This method checks to see if the specified object can really be sold or
 * not automatically.
 * @param obs the objects to test
 */
int do_approve_auto_item_test(object* obs) {
   object ob;
   string place;
   mixed cost;
   int found;
   int allowed;
   class expression_type stuff;
   class parse_node frog;

   allowed = is_allowed(this_player()->query_name());

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   foreach (ob in obs) {
      found = 0;
      cost = _automatic_approval->items[ob->query_short()];
      if (classp(cost)) {
         cost = ((class approval_item)cost)->value;
      }
      if (cost) {
         if (pointerp(cost)) {
            frog = evaluate_expression(cost, this_player()->query_name(),
                                        0, ({ ob }), 0, 0);
            if (allowed) {
               write("$I$5=The item " + ob->the_short() + " can be sold if it "
                 "costs less than " +
                  query_expression_string(cost, 1) +
                  ";\n" + ob->query_short() + " - " +
                  MONEY_HAND->money_value_string(frog->value, place) +
                  ".\n");
            } else {
               write("$I$5=The item " + ob->the_short() + " can be sold if it "
                 "costs less than " +
                  " " + MONEY_HAND->money_value_string(frog->value, place) +
                  ".\n");
            }
         } else {
            write("$I$5=The item " + ob->the_short() + " can be sold if it "
              "costs less than " +
               MONEY_HAND->money_value_string(cost, place) +
               " (" + ob->query_short() + ").\n");
         }
         found |= 1;
      }

      foreach (stuff in _automatic_approval->expressions) {
         frog = evaluate_expression(stuff->condition,
                                    this_player()->query_name(),
                                    0, ({ ob }), 0, 0);
         if (frog->value) {
            if (stuff->type == PLAYER_CRAFT_SHOP_EXPR_ACCEPT) {
               frog = evaluate_expression(stuff->value,
                                      this_player()->query_name(),
                                      0, ({ ob }), 0, 0);
               if (allowed) {
                  write("$I$5=The item " + ob->the_short() +
                        " can be sold if it "
                        "costs less than " +
                        query_expression_string(stuff->value, 1) +
                        ";\n" + ob->query_short() + " - " +
                        MONEY_HAND->money_value_string(frog->value, place) +
                        ".\n");
               } else {
                  write("$I$5=The item " + ob->the_short() +
                        " can be sold if it "
                        "costs less than " +
                        MONEY_HAND->money_value_string(frog->value, place) +
                        ".\n");
               }
               found |= 2;
            }
            else found |= 4;
         }
      }
      if (found & 4)  {
         write("$I$5=The item " + ob->the_short() + " will be denied sale.\n");
      }
      else if (!found) {
         write("$I$5=The item " + ob->the_short() + " will not be "
               "automatically accepted.\n");
      }
   }
   add_succeeded_mess(({ "",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_auto_item_test() */

/**
 * This method sets the automatic items to allow.
 * @param item the item to accept
 */
int do_approve_auto_item_remove(string item) {
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to set automatic approval items.\n");
      return 0;
   }

   if (query_owners_money() < 0) {
      add_failed_mess("You cannot do this until the shop's deficit is payed "
                      "off.\n");
      return 0;
   }

   item = strip_quotes(item);
   if (!_automatic_approval->items[item]) {
      add_failed_mess("The item " + item + " is not in the list.\n");
      return 0;
   }

   map_delete(_automatic_approval->items, item);
   event_save(this_object());
   add_succeeded_mess(({ "You remove the item " + item +
                         " from the list of automatically accepted items.\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_auto_item_remove() */

/**
 * This method sets the automatic items to allow.
 * @param item the item to accept
 */
int do_approve_auto_item_remove_object(object *obs) {
   object ob;
   object *bad;
   object *ok;
   string name;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to set automatic approval items.\n");
      return 0;
   }

   if (query_owners_money() < 0) {
      add_failed_mess("You cannot do this until the shop's deficit is payed "
                      "off.\n");
      return 0;
   }

   ok = ({ });
   bad = ({ });
   foreach (ob in obs) {
      name = ob->query_short();
      if (!_automatic_approval->items[name]) {
         bad += ({ ob });
      } else {
         ok += ({ ob });
         map_delete(_automatic_approval->items, name);
      }
   }
   event_save(this_object());
   if (sizeof(ok)) {
      add_succeeded_mess(({ "You remove $I from the list of "
                            "automatically accepted items.\n",
                         "$N fiddle$s with something in the shop.\n" }), ok );
      return 1;
   }
   add_failed_mess("None of $I are in the shops allow item list for you "
                   "to remove.\n", bad);
   return 0;
} /* do_approve_auto_item_remove() */

/**
 * This method sets the automatic expressions to allow.
 * @param expression the expressions short to allow
 */
int do_approve_auto_expression_add(string expression, string value_str,
                                   int type, string list_name) {
   class parse_node* expr;
   class parse_node* value;
   class expression_type bing;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to set automatic approval items.\n");
      return 0;
   }

   if (query_owners_money() < 0) {
      add_failed_mess("You cannot do this until the shop's deficit is payed "
                      "off.\n");
      return 0;
   }

   expr = parse_boolean_string(expression);
   if (!sizeof(expr)) {
      add_failed_mess(query_last_expression_error() + ".\n");
      return 0;
   }

   if (type == PLAYER_CRAFT_SHOP_EXPR_ACCEPT) {
      value = parse_money_string(value_str);
      if (!sizeof(value)) {
         add_failed_mess(query_last_expression_error() + ".\n");
         return 0;
      }
   }

   list_name = strip_quotes(list_name);
   bing = new(class expression_type);
   bing->type = type;
   bing->condition = expr;
   bing->value = value;
   bing->list_name = list_name;
   _automatic_approval->expressions += ({ bing });

   event_save(this_object());
   if (type == PLAYER_CRAFT_SHOP_EXPR_ACCEPT) {
      add_succeeded_mess(({ "You set the shop to automatically accept any item "
                         "matching the expression " +
                         query_expression_string(expr, 0) + " with a cost "
                         "of " + query_expression_string(value, 0) +
                         " as '" + list_name + "'.\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   } else {
      add_succeeded_mess(({ "You set the shop to automatically deny any item "
                         "matching the expression " +
                         query_expression_string(expr, 0) + ".\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   }
   return 1;
} /* do_approve_auto_expression_add() */

/**
 * This method removes an allowed expression
 * @param id the expression id to remove
 */
int do_approve_auto_expression_remove(string idstr) {
   class expression_type expr;
   int id;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to set automatic approval items.\n");
      return 0;
   }

   if (query_owners_money() < 0) {
      add_failed_mess("You cannot do this until the shop's deficit is payed "
                      "off.\n");
      return 0;
   }

   id = query_number_value(idstr);
   if (id == -1 || id >= sizeof(_automatic_approval->expressions)) {
      add_failed_mess("The id " + idstr + " is invalid.\n");
      return 0;
   }

   expr = _automatic_approval->expressions[id];
   _automatic_approval->expressions = _automatic_approval->expressions[0..id - 1] +
                                      _automatic_approval->expressions[id + 1..];

   event_save(this_object());
   add_succeeded_mess(({ "You remove the expression " +
                         query_expression_string(expr->condition, 1) +
                         " cost: " +
                         query_expression_string(expr->value, 1) +
                         " from the list of allowed expressions.\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_auto_item_remove() */

/**
 * This method sets the percentage to add to the sell price.
 * @param num the number to set the percentage to
 */
int do_approve_percentage(mixed num) {
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to set automatic approval items.\n");
      return 0;
   }

   if (query_owners_money() < 0) {
      add_failed_mess("You cannot do this until the shop's deficit is payed "
                      "off.\n");
      return 0;
   }

   if (num <= 0) {
      add_failed_mess("The percentage to add must be greator than 0.\n");
      return 0;
   }

   _automatic_percentage = num;
   add_succeeded_mess(({ "You set the percentage to add to the sale price "
                         "to " + _automatic_percentage + ".\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_percentage() */

/**
 * This method sets the limit on number of items sellable by all the players.
 * @param limit the default limit
 */
int do_approve_limit(string limit) {
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to set any limits on selling "
                      "items.\n");
      return 0;
   }

   if (limit == "disable")  {
      _automatic_approval->num_allowed = 0;
      add_succeeded_mess(({ "You disable the default number of sold items "
                         "per person.\n",
                         "$N fiddle$s with something in the shop.\n" }) );
      return 1;
   }
   else if (to_int(limit) > 0)  {
      _automatic_approval->num_allowed = to_int(limit);
      add_succeeded_mess(({ "You set the default number of sold items per "
                            "person to " + _automatic_approval->num_allowed + ".\n",
                            "$N fiddle$s with something in the shop.\n" }) );
      return 1;
   }
   return 0;
} /* do_approve_limit() */

/**
 * This method sets the limit per person to be a specified value.
 * @param person the person to set a limit on
 * @param value the limit to set
 */
int do_approve_limit_person_items(string name, string value) {
   int amt;
   class seller_information info;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to set any limits on selling "
                      "items.\n");
      return 0;
   }

   name = lower_case(name);

   if (!PLAYER_HANDLER->test_user(name)) {
      add_failed_mess("The person " + name + " does not exist.\n");
      return 0;
   }

   amt = to_int(value);
   info = _sellers[name];
   if (!info) {
      info = new(class seller_information);
   }
   info->max_sellable = amt;
   if (info->value_limit || info->max_sellable || info->deny_value_limit) {
      _sellers[name] = info;
   } else {
      map_delete(_sellers, name);
   }

   if (amt)
      add_succeeded_mess(({ "You set the maximum number of items sellable by " +
                            name + " to " + info->max_sellable + ".\n",
                            "$N fiddle$s with something in the shop.\n" }) );
   else
      add_succeeded_mess(({ "You remove the limit for the maximum number of "
         "items sellable by " + name + ".\n",
         "$N fiddle$s with something in the shop.\n" }));
   return 1;
} /* do_approve_limit_person_items() */

/**
 * This method sets the limit at which a person can automatically sell
 * to the shop.
 * @param person the person to set a limit on
 * @param money the limit to set it to
 */
int do_approve_limit_person_value(string name, string money) {
   class seller_information info;
   int value;
   string place;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to set any limits on selling "
                      "items.\n");
      return 0;
   }

   name = lower_case(name);

   if (!PLAYER_HANDLER->test_user(name)) {
      add_failed_mess("The person " + name + " does not exist.\n");
      return -1;
   }

   money = lower_case(money);
   if (money == "disable" || money == "0") {
      value = 0;
   } else {
      place = query_property("place");
      if (!place) {
         place = "default";
      }

      value = value_from_string(money, place);
      if (!value) {
         add_failed_mess("The value " + value + " is invalid, please use "
                         "'disable' to disable this feature for the person.\n");
         return 0;
      }
   }

   info = _sellers[name];
   if (!info) {
      info = new(class seller_information);
   }
   info->value_limit = value;
   if (info->value_limit || info->max_sellable || info->deny_value_limit) {
      _sellers[name] = info;
   } else {
      map_delete(_sellers, name);
   }

   if (value) {
      add_succeeded_mess(({ "You set the maximum cost of items automatically "
                            "approved by " +
                         name + " to " +
                         MONEY_HAND->money_value_string(info->value_limit, place) +
                         ".\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   } else {
      add_succeeded_mess(({ "You disable the maximum cost of items "
                         "automatically approved by " +
                         name + ".\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   }
   return 1;
} /* do_approve_limit_person_value() */

/**
 * This method sets the limit at which a person can automatically sell
 * to the shop (deny limit).
 * @param person the person to set a limit on
 * @param money the limit to set it to
 */
int do_approve_limit_person_value_deny(string name, string money) {
   class seller_information info;
   int value;
   string place;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to set any limits on selling "
                      "items.\n");
      return 0;
   }

   name = lower_case(name);

   if (!PLAYER_HANDLER->test_user(name)) {
      add_failed_mess("The person " + name + " does not exist.\n");
      return -1;
   }

   money = lower_case(money);
   if (money == "disable" || money == "0") {
      value = 0;
   } else {
      place = query_property("place");
      if (!place) {
         place = "default";
      }

      value = value_from_string(money, place);
      if (!value) {
         add_failed_mess("The value " + value + " is invalid, please use "
                         "'disable' to disable this feature for the person.\n");
         return 0;
      }
   }

   info = _sellers[name];
   if (!info) {
      info = new(class seller_information);
   }
   info->deny_value_limit = value;
   if (info->deny_value_limit || info->max_sellable || info->value_limit) {
      _sellers[name] = info;
   } else {
      map_delete(_sellers, name);
   }

   if (value) {
      add_succeeded_mess(({ "You set the maximum cost of items accepted by " +
                         name + " to " +
                         MONEY_HAND->money_value_string(info->deny_value_limit, place) +
                         ".\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   } else {
      add_succeeded_mess(({ "You disable the maximum cost of items to be sold by " +
                         name + ".\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   }
   return 1;
} /* do_approve_limit_person_value() */

/**
 * This method removes any limits set on the player.
 * @param person the person to remove the limit on
 */
int do_approve_limit_person_remove(string name) {
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to remove limits "
                      "off someone.\n");
      return 0;
   }

   name = lower_case(name);

   if (!_sellers[name]) {
      add_failed_mess("The person " + name + " does not have any limits placed "
                      "on them.\n");
      return 0;
   }
   map_delete(_sellers, name);
   event_save(this_object());
   add_succeeded_mess(({ "You remove any limits set on " + name + ".\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_limit_person_remove() */

/**
 * This method adds the person to the black list.
 * @param person the person to add to the black list
 */
int do_approve_black_list_add(string name) {
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to add people to the black list.\n");
      return 0;
   }

   name = lower_case(name);

   if (!PLAYER_HANDLER->test_user(name)) {
      add_failed_mess("Sorry, " + name + " does not play here.\n");
      return 0;
   }

   if (is_allowed(name)) {
      add_failed_mess("This person is in the list of people allowed to use "
                      "the shop.  They cannot be put on a black list.\n");
      return 0;
   }

   if (member_array(name, _black_list) != -1) {
      add_failed_mess("The person " + name + " is already in the "
                      "black list.\n");
      return 0;
   }
   _black_list += ({ name });
   event_save(this_object());
   add_succeeded_mess(({ "You add " + name + " to the black list.\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_black_list_add() */

/**
 * This method adds their ignore list to the black list
 */
int do_approve_black_list_add_ignore() {
   string *people;
   string *bad;
   string *ok;
   string name;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to add people to the black list.\n");
      return 0;
   }

   people = this_player()->query_property("ignoring");
   if (!people) {
      add_failed_mess("You are not ignoring anyone.\n");
      return 0;
   }

   bad = ({ });
   ok = ({ });
   foreach (name in people) {
      name = lower_case(name);

      if (!PLAYER_HANDLER->test_user(name)) {
         bad += ({ name });
      } else if (member_array(name, _black_list) != -1) {
         bad += ({ name });
      } else {
         _black_list += ({ name });
         ok += ({ name });
      }
   }
   event_save(this_object());
   if (!sizeof(ok)) {
      add_failed_mess("Your ignore list is already added to the black list.\n");
      return 0;
   }

   add_succeeded_mess(({ "You add " + query_multiple_short(ok) + " to the "
                         "black list.\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_black_list_add() */

/**
 * This method removes the person from the black list.
 * @param person the person to remove from the black list
 */
int do_approve_black_list_remove(string name) {
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to remove people from "
                      "the black list.\n");
      return 0;
   }

   name = lower_case(name);

   if (member_array(name, _black_list) == -1) {
      add_failed_mess("The person " + name + " is not in the black list.\n");
      return 0;
   }
   _black_list -= ({ name });
   event_save(this_object());
   add_succeeded_mess(({ "You remove " + name + " from the black list.\n",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_black_list_remove() */

/**
 * This method allows a function to be added to the base set of user 
 * functions.
 * @param def the name and arguements
 * @param expr the expression itself
 * @return 1 on success, 0  on failure
 */
int do_user_function_add(string def, string expr) {
   string name;

if (this_player()->query_name() != "presto")
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to add functions to this store.\n");
      return 0;
   }

   //
   // Ok, try and parse the arguments and function name first.
   //
   name = parse_user_expression(def, expr);
   if (name) {
      if (strlen(query_last_expression_warning())) {
         write("WARNING!!! Any changes of types could cause expressions "
               "currently using the functions to fail.\n" +
               query_last_expression_warning());
      }
      write("You successfully add:\n" + query_user_function_string(name));
      add_succeeded_mess(({ "",
                         "$N fiddle$s with something in the shop.\n" }) );
      event_save(this_object());
      return 1;
   }

   add_failed_mess(query_last_expression_error() + "\n");
   return 0;
} /* do_user_function_add() */

/**
 * This method allows a function to be added to the base set of user 
 * functions.
 * @param def the name and arguements
 * @param expr the expression itself
 * @return 1 on success, 0  on failure
 */
int do_user_function_remove(string name) {
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to remove functions from this store.\n");
      return 0;
   }

   //
   // Ok, try and parse the arguments and function name first.
   //
   if (query_user_function_return_type(name) != EXPRESSION_TYPE_ERROR) {
      if (remove_user_expression(name)) {
         add_succeeded_mess(({ "You remove the function " + name + ".\n",
                         "$N fiddle$s with something in the shop.\n" }) );
         return 1;
      }
   }

   add_failed_mess(query_last_expression_error() + "\n");
   return 0;
} /* do_user_function_remove() */

/**
 * This method shows the status of all the user defined functions.
 * These can be used in other exciting functions.
 */
int do_approve_status_internal_functions() {
   string* names;
   string name;
   string ret;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to see the status of this store.\n");
      return 0;
   }

   names = query_function_names();
   ret = "Functions:\n";
   foreach (name in names) {
      ret += query_type_name(query_function_type(name)) + " " + name +
             "(" + implode(map(query_function_args_types(name),
                               (: query_type_name($1) :)), ", ") +
             ")\n";
   }

   ret += "\nVariables:\n";
   names = query_variable_names();
   foreach (name in names) {
      ret += query_type_name(query_variable_type(name)) + " " + name +
             "\n";
   }
   write("$P$Internal functions$P$" + ret);
   add_succeeded_mess(({ "",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_status_functions() */

/**
 * Get help on a specific function.
 * @param name the name of the function to get help on
 */
int do_function_help(string name) {
   string nroff_fn;
   string str;
   string fname;

   fname = CRAFT_SHOP_EXPRESSION_HELP_DIR + name;
   if (file_size(fname) <= 0) {
      add_failed_mess("There is no help for the function '" + name + "'.\n");
      return 0;
   }

   nroff_fn = NROFF_DIR + replace(fname, "/", ".");
   str = NROFF_HAND->cat_file(nroff_fn, 1);
   if (!str) {
      NROFF_HAND->create_nroff(fname, nroff_fn);
      str = NROFF_HAND->cat_file(nroff_fn, 0);
   }

   write("$P$function: " + name + "$P$" + str);
   return 1;
} /* do_function_help() */

/**
 * Get help on a specific function.
 * @param name the name of the function to get help on
 */
int do_function_help_list() {
   string *names;

   names = get_dir(CRAFT_SHOP_EXPRESSION_HELP_DIR);
   if (!sizeof(names)) {
      add_failed_mess("There is no help on any functions.\n");
      return 0;
   }

   names -= ({ ".", "..", "RCS" });

   write(sprintf("%-#*s\n", this_player()->query_cols(), implode(names, "\n")));
   return 1;
} /* do_function_help_list() */

/**
 * This method shows the status of all the user defined functions.
 * These can be used in other exciting functions.
 */
int do_approve_status_functions(int hints) {
   string* names;
   string name;
   string ret;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to see the status of this store.\n");
      return 0;
   }

   names = query_user_function_names();
   if (!sizeof(names)) {
      add_failed_mess("Could not find any user defined expressions.\n");
      return 0;
   }
   ret = "";
   foreach (name in names) {
      ret += query_user_function_string(name) + "\n";
   }
   if (hints) {
      ret += "$I$0=       Hint: add function <function> as <expression>";
      ret += "$I$0=       Hint: remove function <function>";
   }
   write("$P$User functions$P$" + ret);
   add_succeeded_mess(({ "",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_status_functions() */

/**
 * This method shows the current status of the shop.
 */
int do_approve_status(int status) {
   string ret;
   string place;
   mixed expr;
   string str;
   string name;
   class seller_information stuff;
   string *bits;
   int pos;
   int hint;
   int show_expr;
   class expression_type bing;
   class expression_type binger;
   class approval_item approval;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to see the status of this store.\n");
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   hint = status & 1;
   show_expr = status & 2;

   ret = "";
   if (!show_expr) {
      ret += "$I$5=The owner of the shop is " + query_owner() + ".\n";
      ret += "$I$5=The name of the shop is " + _shop_name + ".\n";
      if (hint) {
         ret += "$I$0=       Hint: name shop <name>\n";
      }
      if (query_sign_object()) {
         ret += "$I$5=The shop currently has a sign set.\n";
      } else {
         ret += "$I$5=The shop does not currently have a sign set.\n";
      }
      if (hint) {
         ret += "$I$0=       Hint: set sign <object>\n";
         ret += "$I$0=       Hint: remove sign\n";
      }
      if (sizeof(query_allowed())) {
         ret += "$I$5=People allowed to change the shop are " +
                query_multiple_short(sort_array(query_allowed(), 0)) + ".\n";
      } else if (hint) {
         ret += "$I$5=Only the owner can change the shop.\n";
      }
      if (hint) {
         ret += "$I$0=       Hint: allow <person>\n";
         ret += "$I$0=       Hint: deny <person>\n";
         ret += "$I$0=       Hint: allow list\n";
      }
      if (sizeof(_black_list)) {
         ret += "$I$5=People denied access to the shop are ";
         ret += query_multiple_short(sort_array(_black_list, 0)) + "\n";
      } else {
         ret += "$I$0=Noone is denied access to the shop.\n";
      }
      if (hint) {
         ret += "$I$0=       Hint: add <person> to blacklist\n";
         ret += "$I$0=       Hint: remove <person> from blacklist\n";
         ret += "$I$0=       Hint: add ignore to blacklist\n";
      }
      ret += "$I$0=\n";

      ret += "The current percentage to automatically add to the buy price is: " +
             _automatic_percentage + "%\n";
      if (hint) {
         ret += "$I$0=       Hint: markup <percent>\n";
      }
      ret += "Limits on what can be sold.\n";
      ret += "Maximum number of items allowed in the shop : ";
      if (query_maximum_inventory_size()) {
         ret += query_maximum_inventory_size() + " currently " +
                sizeof(query_controller()->query_sell_list_obs()) + " (fixed).\n";
      } else {
         ret += "disabled (fixed).\n";
      }
      ret += "Maximum number of items allowed per player  : ";
      if (_automatic_approval->num_allowed) {
         ret += _automatic_approval->num_allowed + ".\n";
      } else {
         ret += "disabled.\n";
      }
      if (hint) {
         ret += "$I$0=       Hint: set max number <number|disable>\n";
      }
      ret += "Automatically queue sales more than         : ";
      if (_automatic_approval->high_cost) {
         ret += MONEY_HAND->money_value_string(_automatic_approval->high_cost, place) +
                ".\n";
      } else {
         ret += "disabled.\n";
      }
      if (hint) {
         ret += "$I$0=       Hint: set high queue cost <cost|disable>\n";
      }
      ret += "Automatically deny sales more than          : ";
      if (_automatic_approval->high_cost_deny) {
         ret += MONEY_HAND->money_value_string(_automatic_approval->high_cost_deny, place) +
                ".\n";
      } else {
         ret += "disabled.\n";
      }
      if (hint) {
         ret += "$I$0=       Hint: set high deny cost <cost|disable>\n";
      }
      if (sizeof(_sellers)) {
         ret += "Limits set per player.\n";
         ret += sprintf("   %-15s %-15s %-20s %s\n", "Name", "Max Sellable",
                        "Approve Limit", "Deny limit (per item)");
         bits = sort_array(keys(_sellers), 0);
         foreach (name in bits) {
            stuff = _sellers[name];
            ret += sprintf("   %-15s %-15s %-20s %s\n", name,
                       (stuff->max_sellable?stuff->max_sellable+"":"disabled"),
                       (stuff->value_limit?MONEY_HAND->money_value_string(stuff->value_limit, place):"disabled"),
                       (stuff->deny_value_limit?MONEY_HAND->money_value_string(stuff->deny_value_limit, place):"disabled"));
         }
      } else {
         ret += "No limits set per player.\n";
      }
      if (hint) {
         ret += "$I$0=       Hint: set max number <max sellable|disable> for <player>\n";
         ret += "$I$0=       Hint: set high queue cost <cost|disable> for <player>\n";
         ret += "$I$0=       Hint: set high deny cost <cost|disable> for <player> to <cost|disable>\n";
         //ret += "$I$0=       Hint: approve limit remove <player>\n";
      }

      ret += "\nAutomatically approve sales less than       : ";
      if (_automatic_approval->low_cost) {
         ret += MONEY_HAND->money_value_string(_automatic_approval->low_cost, place) +
                ".\n";
      } else {
         ret += "disabled.\n";
      }
      if (hint) {
         ret += "$I$0=       Hint: set low approve cost <cost|disable>\n";
      }
   }

   if (show_expr) {
      if (sizeof(_automatic_approval->items)) {
         ret += "$I$0=%^BOLD%^Automatically accept sales of items%^RESET%^\n";
         foreach (str, expr in _automatic_approval->items) {
            if (classp(expr)) {
               approval = (class approval_item)expr;
               expr = ((class approval_item)expr)->value;
            } else {
               approval = new(class approval_item);
            }
            if (pointerp(expr) && intp(expr[0])) {
               map_delete(_automatic_approval->items, str);
            } else {
               ret += sprintf("$I$10=   %-20s", "* " + str);
               if (sizeof(approval) == 3) {
                  approval = new(class approval_item,
                         markup : approval->markup,
                         value : approval->value,
                         list_name : approval->list_name,
                         final_cost : 0);
               }
               if (approval->list_name) {
                  ret += " listed as '" + approval->list_name + "'";
               }
               if (approval->markup) {
                  ret += " markup of " + approval->markup;
               }
               if (approval->final_cost) {
                  ret += " final cost of " +
                  MONEY_HAND->money_value_string(approval->final_cost, place);
               }
               ret += " max value:";
               if (pointerp(expr)) {
//printf("%O\n", str);
                  str = query_expression_string(expr, 0);
                  if (strsrch(str, "\n") != -1) {
                     ret += "\n" + str;
                  } else {
                     ret += str;
                  }
               } else {
                  ret += MONEY_HAND->money_value_string(expr, place);
               }
               ret += "\n";
            }
         }
      } else {
         ret += "Automatically accept sales of               : (none setup)\n";
      }
      if (hint) {
         ret += "$I$0=       Hint: auto add name <name> up to <value>\n";
         ret += "$I$0=       Hint: auto add name <name> up to <value> as <list name>\n";
         ret += "$I$0=       Hint: auto add name <name> up to <value> with markup <percentage>\n";
         ret += "$I$0=       Hint: auto add name <name> up to <value> with value <final cost>\n";
         ret += "$I$0=       Hint: auto add name <name> up to <value> as <list name> with markup <percentage>\n";
         ret += "$I$0=       Hint: auto add name <name> up to <value> as <list name> with value <final cost>\n";
         ret += "$I$0=       Hint: auto add name <name> with <expression>\n";
         ret += "$I$0=       Hint: auto add name <name> with <expression> as <list name>\n";
         ret += "$I$0=       Hint: auto add name <name> with <expression> and markup <percentage>\n";
         ret += "$I$0=       Hint: auto add name <name> with <expression> and value <final cost>\n";
         ret += "$I$0=       Hint: auto add name <name> with <expression> as <list name> with markup <percentage>\n";
         ret += "$I$0=       Hint: auto add object <object> up to <value>\n";
         ret += "$I$0=       Hint: auto add object <object> up to <value> as <list name>\n";
         ret += "$I$0=       Hint: auto add object <object> up to <value> with markup <percentage>\n";
         ret += "$I$0=       Hint: auto add object <object> up to <value> with value <final cost>\n";
         ret += "$I$0=       Hint: auto add object <object> up to <value> as <list name> with markup <percentage>\n";
         ret += "$I$0=       Hint: auto add object <object> up to <value> as <list name> with value <final cost>\n";
         ret += "$I$0=       Hint: auto add item object <object> with <expression>\n";
         ret += "$I$0=       Hint: auto add item object <object> with <expression> as <list name>\n";
         ret += "$I$0=       Hint: auto add item object <object> with <expression> with markup <percentage>\n";
         ret += "$I$0=       Hint: auto add item object <object> with <expression> with value <final cost>\n";
         ret += "$I$0=       Hint: auto add item object <object> with <expression> as <list name> with markup <percentage>\n";
         ret += "$I$0=       Hint: auto add item object <object> with <expression> as <list name> with value <final cost>\n";
         ret += "$I$0=       Hint: auto remove name <name>\n";
         ret += "$I$0=       Hint: auto remove object <object>\n";
      }
      ret += "\n$I$0=%^BOLD%^Automatic approval expressions.%^RESET%^\n";
      if (sizeof(_automatic_approval->expressions)) {
         //ret += "$I$0=Automatic approval expressions (if any of these evaluate as "
                //"true the item is accepted):\n";
         pos = 0;
         foreach (bing in _automatic_approval->expressions) {
            if (sizeof(bing) == 3) {
               binger = new(class expression_type);
               binger->type = bing->type;
               binger->condition = bing->condition;
               binger->value = bing->value;
               bing = binger;
               event_save(this_object());
            }

            if (bing->type != PLAYER_CRAFT_SHOP_EXPR_DENY) {
               str = query_expression_string(bing->value, 0);
               if (strsrch(str, "\n") != -1) {
                  str = "\n" + str;
               }
               str = " accept value: " + str;
               if (bing->list_name) {
                  str += " as " + bing->list_name;
               }
            } else {
               str = " -- deny item";
            }
            ret += "$I$5=" + query_letter_value(pos) + ") " +
                   query_expression_string(bing->condition, 0) + str + ".\n";
            pos++;
         }

      } else {
         ret += "$I$0=No automatic approval expressions setup.\n";
      }
      if (hint) {
         ret += "$I$0=       Hint: auto add approve expression <condition> cost <value> as <list name>\n";
         ret += "$I$0=       Hint: auto deny expression <expression>\n";
         ret += "$I$0=       Hint: auto remove expression <id>\n";
      }
   }

   ret += "\n$I$0=";
   if (sizeof(_buy_expression)) {
      ret += "Buy condition: " + query_expression_string(_buy_expression, 0) +
             ".\n";
   } else {
      ret += "Anyone can buy from the shop.\n";
   }
   if (hint) {
      ret += "$I$0=       Hint: buy expression <expression|disable>\n";
   }

   if (sizeof(_use_expression)) {
      ret += "Use condition: " + query_expression_string(_use_expression, 0) +
             ".\n";
   } else {
      ret += "Anyone can use the shop.\n";
   }
   if (hint) {
      ret += "$I$0=       Hint: use expression <expression|disable>\n";
   }

   if (!show_expr) {
      ret += "$I$0=Use 'expressions' to see the expressions.\n";
   }
   ret += "$I$0=Use 'functions' to see the user defined "
          "functions.\n";

   write("$P$Status$P$" + ret);

   add_succeeded_mess(({ "",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_status() */

/**
 * This method pays off the cash deficit (if one exists).
 */
int do_approve_pay_deficit() {
   int cost;
   string place;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to pay off the deficit for " +
                      the_short() + ".\n");
      return 0;
   }

   if (query_owners_money() >= 0) {
      add_failed_mess("There is no deficit here to pay off.\n");
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   cost = -query_owners_money();
   if (this_player()->query_value_in(place) >= cost) {
      adjust_royalty(query_owner(), cost);

      this_player()->pay_money(MONEY_HAND->create_money_array(cost, place), place);
      add_succeeded_mess("$N pay$s off the deficit in " + the_short() + ".\n");
      return 1;

   }
   add_failed_mess("You do not have enough money to pay off the deficit "
                   "of " + MONEY_HAND->money_value_string(cost, place) +
                   ".\n");
   return 0;
} /* do_approve_pay_deficit() */

/**
 * This method sets up the expression to use when calculating the
 * ability to buy from the shop.
 */
int do_approve_buy_expression(string str) {
   class parse_node* expr;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to add restrictions to the shop.\n");
      return 0;
   }

   if (str == "disable")  {
      _buy_expression = ({ });
      add_succeeded_mess(({"You disable checking to see if someone "
         "can buy from the shop.\n",
         "$N fiddle$s about with " + the_short() + " a bit.\n"}));
      return 1;
   }
   expr = parse_boolean_string(str);

   if (!sizeof(expr)) {
      add_failed_mess(query_last_expression_error() + ".\n");
      return 0;
   } else {
      _buy_expression = expr;
      add_succeeded_mess(({ "You set the expression to use when checking "
                            "to see if someone can buy from the shop to " +
                            query_expression_string(expr, 1) +
                            ".\n",
                            "$N fiddle$s about with " + the_short() +
                                " a bit.\n" }));
      event_save(this_object());
   }
   return 1;
} /* do_approve_buy_expression() */

/**
 * This method sets up the expression to use when calculating the
 * ability to use the shop.
 */
int do_approve_use_expression(string str) {
   class parse_node* expr;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to add restrictions to the shop.\n");
      return 0;
   }

   if (str == "disable")  {
      _use_expression = ({ });
      add_succeeded_mess(({ "You disable checking to see if "
         "someone can use the shop.\n",
         "$N fiddle$s about with " + the_short() + " a bit.\n" }));
      return 1;
   }
   expr = parse_boolean_string(str);

   if (!sizeof(expr)) {
      add_failed_mess(query_last_expression_error() + ".\n");
      return 0;
   } else {
      _use_expression = expr;
      add_succeeded_mess(({ "You set the expression to use when checking "
                            "to see if someone can use the shop to " +
                            query_expression_string(expr, 1) +
                            ".\n",
                            "$N fiddle$s about with " + the_short() +
                                " a bit.\n" }));
      event_save(this_object());
   }
   return 1;
} /* do_approve_use_expression() */

/**
 * This method allows the player to add a new category to the list.
 * @param category the new category name
 */
int do_add_new_category(string category) {
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to add categories.\n");
      return 0;
   }

   if (query_controller()->is_valid_category(category)) {
      add_failed_mess("The category already exists.\n");
      return 0;
   }

   add_shop_category(category);
   add_succeeded_mess("$N add$s a new category.\n");
   return 1;
} /* do_add_new_category() */

/**
 * This method allows the player to remove a category from the list.
 * @param category the category to remove
 */
int do_remove_category(string category) {
   object* obs;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to remove categories.\n");
      return 0;
   }

   category = query_real_category_name(category);
   if (!category ||
       !query_controller()->is_valid_category(category)) {
      add_failed_mess("The category " + category + " does not exist.\n");
      return 0;
   }

   if (category == query_default_category()) {
      add_failed_mess("You cannot remove the default category.\n");
      return 0;
   }


   obs = query_controller()->query_sell_list_obs();
   obs = filter(obs, (: query_controller()->query_category_of_shop_object($1)  == $2:), category );

   if (sizeof(obs)) {
      add_failed_mess("The category '" + category + "' is not empty "
                     "and cannot be removed.\n");
      return 0;
   }

   write("Do you wish to remove the category " + category + " from " +
         the_short() + " (y/n)?");
   input_to("check_remove_category", 0, category);
   add_succeeded_mess(({ "", "$N removes a category from the shop.\n" }));
   return 1;
} /* do_remove_category() */

/**
 * This method allows the player to remove a category from the list.
 * @param category the category to rename
 * @param new_category the new name of a category
 */
int do_rename_category(string category, string new_category) {
   object* obs;
   object ob;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to rename categories.\n");
      return 0;
   }

   category = query_real_category_name(category);
   if (!category ||
       !query_controller()->is_valid_category(category)) {
      add_failed_mess("The category " + category + " does not exist.\n");
      return 0;
   }

   if (category == query_default_category()) {
      add_failed_mess("You cannot remove the default category.\n");
      return 0;
   }

   add_shop_category(new_category);
   obs = query_controller()->query_sell_list_obs();
   obs = filter(obs, (: query_controller()->query_category_of_shop_object($1)  == $2:), category );

   if (sizeof(obs)) {
      foreach (ob in obs) {
         query_controller()->change_category_of_shop_object(ob, new_category);
      }
   }
   remove_shop_category(category);

   add_succeeded_mess("$N rename$s category " + category + " to " +
                      new_category + ".\n");
   return 1;
} /* do_rename_category() */

/**
 * This method is called into to check to see if we can remove the category.
 * @param answer their answer
 * @param category the category to remove
 */
protected void check_remove_category(string answer, string category) {
   class approval_obs approve;

   if (!strlen(answer) ||
       lower_case(answer)[0] != 'y') {
      write("Aborting deleting the category " + category + ".\n");
      return ;
   }

   remove_shop_category(category);
   write("Removed the shop category " + category + ".\n");
   foreach (approve in query_approval_list(0)) {
      if (approve->category == category) {
         approve->category = query_default_category();
      }
   }
   foreach (approve in query_approval_list(1)) {
      if (approve->category == category) {
         approve->category = query_default_category();
      }
   }
   event_save(this_object());
   return ;
} /* check_remove_category() */

/**
 * This method sets the default category for the place.
 * @param default_cat the default category
 */
int do_set_default_category(string default_cat) {
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to set the default category.\n");
      return 0;
   }

   default_cat = query_real_category_name(default_cat);
   if (!default_cat ||
       !query_controller()->is_valid_category(default_cat)) {
      add_failed_mess("The category " + default_cat + " does not exist.\n");
      return 0;
   }

   set_default_category(default_cat);
   add_succeeded_mess("$N set$s the default category for " + the_short() +
                      ".\n");
   event_save(this_object());
   return 1;
} /* do_set_default_category() */

/**
 * This method lists the current categories and shows us which one is
 * currently the default.
 */
int do_list_categories() {
   string cat;
   string ret;

   ret = "";
   foreach (cat in query_controller()->query_categories()) {
      if (cat == query_default_category()) {
         ret += cat + " (default)\n";
      } else {
         ret += cat + "\n";
      }
   }

   write("$P$Category list$P$The current categories are:\n" + ret);
   add_succeeded_mess(({ "", "$N browses the categories.\n" }));
   return 1;
} /* do_list_categories() */

/**
 * This method gives the shop a name.
 */
int do_name_shop(string name) {
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You cannot set the name of " + the_short() + ".\n");
      return 0;
   }

   set_shop_name(name);
   add_succeeded_mess("$N set$s the shop name to '" + name + "'.\n");
   return 1;
} /* do_name_shop() */

/**
 * This method shows the stats on all the sold items, the number and value
 * of each one sold.
 * @param type the type of sorting to do
 */
int do_stats_items(int type) {
   class shop_stats stat;
   string ret;
   string *bits;
   string place;
   string name;


   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You cannot read " + the_short() + "'s ledger.\n");
      return 0;
   }

   if (!sizeof(_sell_stats)) {
      add_failed_mess("Nothing has been sold in this transaction section "
                      "yet.\n");
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   switch (type) {
   case 1:
      bits = sort_array(keys(_sell_stats), (: _sell_stats[$1]->num_sold - _sell_stats[$2]->num_sold :));
      break;
   case 2:
      bits = sort_array(keys(_sell_stats), (: _sell_stats[$1]->value_sold - _sell_stats[$2]->value_sold :));
      break;
   default :
      bits = sort_array(keys(_sell_stats), 0);
      break;
   }

   ret = "Statistics for items sold since " + ctime(_stats_start) + ".\n";
   foreach (name in bits) {
      stat = _sell_stats[name];
      ret += name + ": " + stat->num_sold + " for " +
             MONEY_HAND->money_value_string(stat->value_sold, place) + ".\n";
   }

   write("$P$Sold stats$P$" + ret);
   add_succeeded_mess(({ "", "$N looks at the ledger.\n" }));
   return 1;
} /* do_stats_items() */

#if USE_TRANSACTIONS
/**
 * This method shows a list of all the transactions that have occured in the
 * shop.
 */
int do_stats_transactions() {
   class shop_transaction bing;
   string ret;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You cannot read " + the_short() + "'s ledger.\n");
      return 0;
   }

   ret = "Statistics for transactions since " + ctime(_stats_start) + ".\n";
   foreach (bing in _transactions) {
      ret += "$I$5=" + query_transaction_string(bing) + "\n";
   }

   write("$P$Sold stats$P$" + ret);
   add_succeeded_mess(({ "", "$N looks at the ledger.\n" }));
   return 1;
} /* do_stats_transactions() */
#endif

/**
 * This method shows stats about money and stuff in the shop.
 */
int do_stats_money() {
   string ret;
   int stock_value;
   string place;
   object ob;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You cannot read " + the_short() + "'s ledger.\n");
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   ret = "";
   ret += "\nCurrent money spent " +
          MONEY_HAND->money_value_string(_total_outgoing, place) +
          " and money made " +
          MONEY_HAND->money_value_string(_total_ingoing, place) + ".\n";

   foreach (ob in query_controller()->query_sell_list_obs()) {
      stock_value += ob->query_value();
   }

   ret += "The current stock value is " +
             MONEY_HAND->money_value_string(stock_value, place) +
          ".\n";

   ret += "\n";
   if (query_owners_money() > 0) {
      ret += "The owner current has a cash float (royalties) of " +
             MONEY_HAND->money_value_string(query_owners_money(), place) +
             ".\n";
   } else if (query_owners_money() < 0) {
      ret += "The owner current has a cash deficit of " +
             MONEY_HAND->money_value_string(-query_owners_money(), place) +
             ".\n";
   }

   write(ret);
   add_succeeded_mess(({ "", "$N looks at the ledger.\n" }));
   return 1;
} /* do_stats_money() */

/**
 * This method sets the sign you wish to use for your shop.
 * The sign will be displayed outside the shop for passer bys.
 * @param obs the sign
 */
int do_set_sign(object* obs) {
   object ob;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You cannot set the sign for the shop.\n");
      return 0;
   }

   if (sizeof(obs) > 1) {
      add_failed_mess("You must only specifiy one sign.\n");
      return 0;
   }

   if (living(obs[0])) {
      add_failed_mess("You have to wait till " + obs[0]->the_short() +
                      " dies first.\n");
      return 0;
   }

   if (obs[0]->get()) {
      add_failed_mess("Cannot set something as a sign that you cannot get.\n");
      return 0;
   }

   ob = query_sign_object();
   if (obs[0]->move("/room/rubbish") == MOVE_OK) {
      set_sign_object(obs[0]);
   } else {
      add_failed_mess("Unable to move $I off you.\n", obs[0..0]);
      return 0;
   }
   if (query_sign_object() != ob) {
      if (ob) {
         ob->set_get();
         if (ob->move(this_player()) != MOVE_OK) {
            ob->move(this_object());
         }
         add_succeeded_mess(({ "You remove the sign " +ob->the_short() +
                               " and set " + obs[0]->the_short() +
                               " as your shop sign.\n",
                               "$N sets the sign for outside " + the_short() +
                               ".\n" }));
      } else {
         add_succeeded_mess(({ "You set " + obs[0]->the_short() +
                               " as your shop sign.\n",
                               "$N sets the sign for outside " + the_short() +
                               ".\n" }));
      }
      return 1;
   } else {
      obs[0]->move(this_player());
      add_failed_mess("You cannot set the sign for some reason.\n");
      return 0;
   }
} /* do_set_sign() */

/**
 * This method removes the sign from the shop.
 */
int do_remove_sign() {
   object ob;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You cannot set the sign for the shop.\n");
      return 0;
   }

   if (!query_sign_object()) {
      add_failed_mess("You cannot remove the sign since there is no sign.\n");
      return 0;
   }

   ob = query_sign_object();
   ob->set_get();
   if (ob->move(this_player()) == MOVE_OK) {
      add_succeeded_mess("$N $V $I from " + the_short() +
                         ".\n", ({ ob }));
      set_sign_object(0);
      return 1;
   } else {
      ob->reset_get();
      add_failed_mess("You cannot remove the sign, unable to hold it?\n");
      return 0;
   }
} /* do_remove_sign() */

/**
 * This method collects royalities up to a certain amount.  This is
 * only usable by the owner of the shop.
 * @param amount the amount to collect
 * @return 1 on success, 0 on failure
 */
int do_collect_partial_royalties(string amount) {
   int amt;
   string place;

   if (query_owner() != this_player()->query_name()) {
      add_failed_mess("Only the owner can do this.\n");
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }
   amt = value_from_string(amount, place);
   if (!amt) {
      add_failed_mess("The value " + amount + " is not a valid money "
                      "amount.\n");
      return 0;
   }

   if (amt > query_owners_money()) {
      add_failed_mess("You cannot collect more money than you have.\n");
      return 0;
   }

   query_controller()->pay_out_royalty(this_player()->query_name(), place, amt);
   add_succeeded_mess(({ "You collect " +
                      MONEY_HAND->money_value_string(amt, place) +
                      " in royalties.\n",
                      "$N collects some money in royalties.\n" }));
   return 1;
} /* do_collect_partial_royalites() */

private int variable_player_level(string seller, int cost, object* ob) {
   return PLAYER_HANDLER->test_level(seller);
} /* variable_player_level() */

private string variable_player_guild(string seller, int cost, object* ob) {
   string guild;

   guild = PLAYER_HANDLER->test_guild(seller);
   if (guild) {
      return guild->query_name();
   }
   return "";
} /* variable_player_guild() */

private string variable_player_order(string seller, int cost, object* ob) {
   string guild;

   guild = PLAYER_HANDLER->test_guild(seller);
   if (guild) {
      guild = guild->query_wizard_order();
      if (guild) {
         return replace_string(lower_case(guild), "_", " ");
      }
   }
   return "";
} /* variable_player_order() */

private string variable_player_deity(string seller, int cost, object* ob) {
   string deity;

   deity = PLAYER_HANDLER->test_deity(seller);
   if (deity) {
      return deity;
   }
   return "";
} /* variable_player_deity() */

private string variable_player_family(string seller, int cost, object* ob) {
   string family;

   family = PLAYER_HANDLER->test_family(seller);
   if (family) {
      return family;
   }
   return "";
} /* variable_player_family() */

private int function_object_base_value(object* obs, string seller, int cost, object* fluff) {
   int value;
   object ob;
   int tmp;

   value = 1000000000;
   foreach (ob in obs) {
      tmp = ob->query_base_value();
      if (tmp < value) {
         value = tmp;
      }
   }
   return value;
} /* function_object_base_value() */

private int function_object_value(object* obs, string seller, int cost, object* fluff) {
   int value;
   object ob;
   int tmp;

   value = 1000000000;
   foreach (ob in obs) {
      tmp = ob->query_value();
      if (tmp < value) {
         value = tmp;
      }
   }
   return value;
} /* function_object_value() */

private int function_object_condition(object* obs, string seller, int cost, object* fluff) {
   int cond;
   object ob;
   int tmp;
   int max;

   cond = 100;
   foreach (ob in obs) {
      tmp = ob->query_cond();
      max = ob->query_max_cond();
      if (max) {
         tmp = tmp * 100 / max;
         if (tmp < cond) {
            cond = tmp;
         }
      }
   }
   return cond;
} /* function_object_condition() */

private int function_object_enchant(object*obs, string seller, int cost, object* fluff) {
   int enchant;
   object ob;
   int tmp;
   int max;
   string bing;

   if (!sizeof(obs)) {
      return 0;
   }

   //
   // First check to see if any of the owners can actually detect this.
   //
   if (!PLAYER_HANDLER->test_guild(query_owner())->query_see_octarine()) {
      foreach (bing in query_allowed()) {
         if (PLAYER_HANDLER->test_guild(bing)->query_see_octarine()) {
            tmp = 1;
         }
      }
      if (!tmp) {
         return 0;
      }
   }

   enchant = 100;
   foreach (ob in obs) {
      tmp = ob->query_enchant();
      max = ob->query_max_enchant();
      tmp = tmp * 100 / max;
      if (tmp < enchant) {
         enchant = tmp;
      }
   }
   return enchant;
} /* function_object_enchant() */

private object* variable_objects(string seller, int cost, object* obs) {
   return obs;
} /* function_object_type() */

private string function_object_type(object* obs, string seller, int cost, object* fluff) {
   string type;
   string old_type;
   object ob;

   if (!sizeof(obs)) {
      return 0;
   }

   //
   // First check to see if any of the owners can actually detect this.
   //
   foreach (ob in obs) {
      if (ob->query_weapon()) {
         type = "weapon";
      } else if (ob->query_armour()) {
         type = "armour";
      } else if (ob->query_clothing()) {
         type = "clothing";
      } else {
         type = "misc";
      }
      if (old_type && old_type != type) {
         type = "mixed";
      }
      old_type = type;
   }

   return type;
} /* function_object_type() */

private object* function_object_stolen(object* obs, string seller, int cost, object* fluff) {
   obs = filter(obs, (: $1->query_property("stolen") :));
   return obs;
} /* function_object_stolen() */

private object* function_object_warded(object* obs, string seller, int cost, object* fluff) {
   string classification;

   classification = "/std/effects/object/ward"->query_classification();
   obs = filter(obs, (: sizeof($1->effects_matching($2)) :), classification);
   return obs;
} /* function_object_warded() */

private string variable_sale_name(string seller, int cost, object* ob, string name, string category) {
   if (name) {
      return lower_case(name);
   }
   return "";
} /* variable_sale_name() */

private string variable_sale_category(string seller, int cost, object* ob, string name, string category) {
   if (category) {
      return lower_case(category);
   }
   return "";
} /* variable_sale_category() */

private int variable_sale_cost(string seller, int cost, object* ob, string name, string category) {
   return cost;
} /* variable_sale_category() */

private object* function_object_contains_spell(object* obs, string spell_name,
                          string seller, int cost, object* fluff) {
   string bing;
   int tmp;

   if (!sizeof(obs)) {
      return 0;
   }

   //
   // First check to see if any of the owners can actually detect this.
   //
   if (!PLAYER_HANDLER->test_guild(query_owner())->query_see_spells()) {
      foreach (bing in query_allowed()) {
         if (PLAYER_HANDLER->test_guild(bing)->query_see_spells()) {
            tmp = 1;
         }
      }
      if (!tmp) {
         return 0;
      }
   }

   obs = filter(obs, (: $1->query_magic_scroll() &&
           ($2 == "any" || lower_case($1->query_spell_name()) == lower_case($2)) :), spell_name);
   return obs;
} /* function_object_contains_spell() */

private object* function_object_contains_imbue(object* obs, string ritual_name,
                          string seller, int cost, object* fluff) {
   if (!sizeof(obs)) {
      return ({ });
   }


   obs = filter(obs, (: $1->query_faith_imbue() &&
      ($2 == "any" ||
       lower_case($1->query_ritual_name()) == lower_case($2)) :), ritual_name );
   return obs;
} /* function_object_contains_ritual() */

private int function_object_percentage_liquid(object* obs, string liquid_name,
                          string seller, int cost, object* fluff) {
   object ob;
   object* liquids;
   object liq;
   int volume;
   int found;
   int tmp;

   if (!sizeof(obs)) {
      return 0;
   }

   volume = 100;
   foreach (ob in obs) {
      //
      // See what liquids are in the object
      //
      liquids = filter(all_inventory(ob), (: $1->query_liquid() :));
      if (sizeof(liquids))  {
         foreach (liq in liquids) {
            if (liq->query_short() &&
                lower_case(liq->query_short()) == lower_case(liquid_name)) {
               tmp = liq->query_weight() * 100 / ob->query_max_weight();
               if (tmp < volume) {
                  volume = tmp;
                  found = 1;
               }
            }
         }
      }
      // Problem: some things (like healing vials) don't actually have
      // anything in them.  So we have to do something different.
      else if (ob->query_liquid_short() &&
               lower_case(ob->query_liquid_short()) ==
               lower_case(liquid_name))
      {
         tmp = ob->query_water_volume() * 100 / ob->query_max_volume();
         if (tmp < volume)  {
            volume = tmp;
            found = 1;
         }
      }
   }
   if (found) {
      return volume;
   }
   return 0;
} /* function_object_percentage_liquid() */

private int function_object_charges(object* obs, string seller, int cost, object *fluff)  {
   int total = 0;
   int possible = 0;
   int charges;
   mixed info;
   object ob;

   foreach (ob in obs)  {
      charges = ob->query_charges();
      if (charges)  {
         info = ob->query_static_auto_load();
         if (pointerp(info)  &&  sizeof(info) == 2  &&  info[0] == charges)  {
            possible += info[1] * CHARGE_MULT;
            total += charges;
         }
      }
   }
   if (possible)
      return 100 * total / possible;
   else
      return 0;
} /* function_object_charges() */

private object* function_object_matching(object* obs, string match,
                          string seller, int cost, object* fluff) {
   object ob;
   string *bits;
   string bit;
   object* ret;

   bits = explode(match, ",");
   ret = ({ });
   foreach (ob in obs)  {
      foreach(bit in bits)   {
         if (ob && ob->full_id(bit)) {
            ret += ({ ob });
         }
      }
   }
   return ret;
} /* function_object_matching() */

private object* function_object_short(object* obs, string match,
                          string seller, int cost, object* fluff) {
   string* bits;
   object* result;

   bits = explode(match, ",");
   result = filter(obs, (: member_array($1->query_short(), $2) != -1 :),
                   bits);
   return result;
} /* function_object_short() */

private int function_inventory_number(string inventory_type,
                          string seller, int cost, object* fluff) {
   object* result;
   int quantity;
   string lc_type;

/*
   lc_type = lower_case(inventory_type);
   if (undefinedp(_num_checked_so_far[lc_type])) {
      _num_checked_so_far[lc_type] = 1;
   }
 */

   result = filter(query_controller()->query_sell_list_obs(),
     (: lower_case(query_controller()->query_name_of_shop_object($1)) == $2 :),
                   lc_type);
   quantity = sizeof(result); //  + _num_checked_so_far[lc_type];

   //++_num_checked_so_far[lc_type];
   return quantity;
} /* function_inventory_number() */

private int function_club_member(string club, string seller, int cost, object* ob) {
   return CLUB_HANDLER->is_member_of(club, seller);
} /* function_club_member() */

private object* function_contents(object ob, string seller, int cost, object* obs) {
   if (!ob) {
      return ({ });
   }
   return all_inventory(ob);
} /* function_contents() */

/**
 * Tells the playe they have some royalties to pick up.
 * @param player the player to tell about the royalties
 */
void inform_of_royalties(object player) {
   if (player && environment(player) == this_object()) {
      tell_object(player, "You have some royalties to pick up.\n");
   }
} /* inform_of_royalties() */

void init() {
   if (query_controller()->query_royalty(this_player()->query_name())) {
      call_out("inform_of_royalties", 5);
   }
   add_command("waiting", "", (: do_list_approval(1) :));
   add_command("waiting", "unapproved", (: do_list_approval(0) :));
   add_command("reject", "<string'id'>",
               (: do_approve_reject($4[0], 0, -1) :));
   add_command("retrieve", "<string'id'>", (: do_return($4[0]) :) );
   add_command("return", "<string'id'>", (: do_return($4[0]) :) );
   add_command("value", "<indirect:object>",
               (: do_approve_auto_item_test($1) :));
   if (!is_allowed(this_player()->query_name())) {
      ::init();
      return ;
   }

   add_command("approve", "<string'id'>", (: do_approve_item($4[0], 0) :));
   add_command("approve", "<string'id'> for <string'value'>",
               (: do_approve_item($4[0], $4[1]) :));
   add_command("reject", "<string'id'> position <number>",
               (: do_approve_reject($4[0], 0, $4[1]) :));
   add_command("reject", "<string'id'> message <string'reject message'>",
               (: do_approve_reject($4[0], $4[1], -1) :));
   add_command("reject", "<string'id'> position <number> message <string'reject message'>",
               (: do_approve_reject($4[0], $4[2], $4[1]) :));
/*
   add_command("approve", "reject <string'id'> <number'position'>",
               (: do_approve_reject_num($4[0], $4[1]) :));
 */
   add_command("set", "low approve cost <string'cost|disable'>",
               (: do_approve_auto_low($4[0]) :));
   add_command("auto", "add approve expression <string'condition'> cost <string'value'> as <string'list name'>",
               (: do_approve_auto_expression_add($4[0], $4[1],
                            PLAYER_CRAFT_SHOP_EXPR_ACCEPT, $4[2]) :));
   add_command("auto", "add deny expression <string'condition'>",
               (: do_approve_auto_expression_add($4[0], 0,
                            PLAYER_CRAFT_SHOP_EXPR_DENY, 0) :));
   add_command("auto", "remove expression <string'id'>",
               (: do_approve_auto_expression_remove($4[0]) :));
   add_command("markup", "<number'percentage'>",
               (: do_approve_percentage($4[0]) :));
   add_command("auto", "add name <string'name'> up to <string'value'>",
               (: do_approve_auto_item_add($4[0], $4[1], 0, 0, 0) :));
   add_command("auto", "add name <string'name'> up to <string'value'> with value <string'final cost'>",
               (: do_approve_auto_item_add($4[0], $4[1], 0, 0, $4[2]) :));
   add_command("auto", "add name <string'name'> up to <string'value'> with markup <number'percentage'>",
               (: do_approve_auto_item_add($4[0], $4[1], 0, $4[2], 0) :));
   add_command("auto", "add name <string'name'> up to <string'value'> as <string'list name'>",
               (: do_approve_auto_item_add($4[0], $4[1], $4[2], 0, 0) :));
   add_command("auto", "add name <string'name'> up to <string'value'> as <string'list name'> with value <string'final cost'>",
               (: do_approve_auto_item_add($4[0], $4[1], $4[2], 0, $4[3]) :));
   add_command("auto", "add name <string'name'> up to <string'value'> as <string'list name'> with markup <number'percentage'>",
               (: do_approve_auto_item_add($4[0], $4[1], $4[2], $4[3], 0) :));
   add_command("auto", "add name <string'name'> with <string'expression'>",
               (: do_approve_auto_item_add_expr($4[0], $4[1], 0, 0, 0) :));
   add_command("auto", "add name <string'name'> with <string'expression'> and markup <number'percentage'>",
               (: do_approve_auto_item_add_expr($4[0], $4[1], 0, $4[2], 0) :));
   add_command("auto", "add name <string'name'> with <string'expression'> and value <string'final cost'>",
               (: do_approve_auto_item_add_expr($4[0], $4[1], 0, 0, $4[2]) :));
   add_command("auto", "add name <string'name'> with <string'expression'> as <string'list name'>",
               (: do_approve_auto_item_add_expr($4[0], $4[1], $4[2], 0, 0) :));
   add_command("auto", "add name <string'name'> with <string'expression'> as <string'list name'> and markup <number'percentage'>",
               (: do_approve_auto_item_add_expr($4[0], $4[1], $4[2], $4[3], 0) :));
   add_command("auto", "add name <string'name'> with <string'expression'> as <string'list name'> and value <string'final cost'>",
               (: do_approve_auto_item_add_expr($4[0], $4[1], $4[2], 0, $4[3]) :));
   add_command("auto", "add object <indirect:object> up to <string'value'>",
               (: do_approve_auto_item_add_object($1, $4[1], 0, 0, 0) :));
   add_command("auto", "add object <indirect:object> up to <string'value'> with markup <number'percentage'>",
               (: do_approve_auto_item_add_object($1, $4[1], 0, $4[2], 0) :));
   add_command("auto", "add object <indirect:object> up to <string'value'> with value <string'final cost'>",
               (: do_approve_auto_item_add_object($1, $4[1], 0, 0, $4[2]) :));
   add_command("auto", "add object <indirect:object> up to <string'value'> as <string'list name'>",
               (: do_approve_auto_item_add_object($1, $4[1], $4[2], 0, 0) :));
   add_command("auto", "add object <indirect:object> up to <string'value'> as <string'list name'> with markup <number'percentage'>",
               (: do_approve_auto_item_add_object($1, $4[1], $4[2], $4[3], 0) :));
   add_command("auto", "add object <indirect:object> up to <string'value'> as <string'list name'> with value <string'final cost'>",
               (: do_approve_auto_item_add_object($1, $4[1], $4[2], 0, $4[3]) :));
   add_command("auto", "add object <indirect:object> with <string'expression'>",
               (: do_approve_auto_item_add_object_expr($1, $4[1], 0, 0, 0) :));
   add_command("auto", "add object <indirect:object> with <string'expression'> and markup <number'percentage'>",
               (: do_approve_auto_item_add_object_expr($1, $4[1], 0, $4[2], 0) :));
   add_command("auto", "add object <indirect:object> with <string'expression'> and value <string'final cost'>",
               (: do_approve_auto_item_add_object_expr($1, $4[1], 0, 0, $4[2]) :));
   add_command("auto", "add object <indirect:object> with <string'expression'> as <string'list name'>",
               (: do_approve_auto_item_add_object_expr($1, $4[1], $4[2], 0, 0) :));
   add_command("auto", "add object <indirect:object> with <string'expression'> as <string'list name'> and markup <number'percentage'>",
               (: do_approve_auto_item_add_object_expr($1, $4[1], $4[2], $4[3], 0) :));
   add_command("auto", "add object <indirect:object> with <string'expression'> as <string'list name'> and value <string'final cost'>",
               (: do_approve_auto_item_add_object_expr($1, $4[1], $4[2], 0, $4[3]) :));
   add_command("auto", "remove name <string'name'>",
               (: do_approve_auto_item_remove($4[0]) :));
   add_command("auto", "remove object <indirect:object>",
               (: do_approve_auto_item_remove_object($1) :));
   add_command("set", "high queue cost <string'cost|disable'>",
               (: do_approve_auto_high($4[0]) :));
   add_command("set", "high deny cost <string'cost|disable'>",
               (: do_approve_auto_high_deny($4[0]) :));
   add_command("set", "max number <string'max sellable|disable'>",
               (: do_approve_limit($4[0]) :));
   add_command("set", "max number <string'max sellable|disable'> for <string'player'>",
               (: do_approve_limit_person_items($4[1], $4[0]) :));
   add_command("set", "high queue cost <string'cost|disable'> for <string'player'>",
               (: do_approve_limit_person_value($4[1], $4[0]) :));
   add_command("set", "high deny cost <string'cost|disable'> for <string'player'>",
               (: do_approve_limit_person_value_deny($4[1], $4[0]) :));
   add_command("approve", "limit remove <string'player'>",
               (: do_approve_limit_person_remove($4[0]) :));
   add_command("add", "<string'player'> to blacklist",
               (: do_approve_black_list_add($4[0]) :));
   add_command("add", "ignore to blacklist",
               (: do_approve_black_list_add_ignore() :));
   add_command("remove", "<string'player'> from blacklist",
               (: do_approve_black_list_remove($4[0]) :));
   add_command("add",
               "function <string'function'> as <string'expression'>",
               (: do_user_function_add($4[0], $4[1]) :));
   add_command("remove",
               "function <string'function'>",
               (: do_user_function_remove($4[0]) :));
   add_command("status", "",
               (: do_approve_status(0) :));
   add_command("expressions", "",
               (: do_approve_status(2) :));
   add_command("functions", "internal",
               (: do_approve_status_internal_functions() :));
   add_command("functions", "",
               (: do_approve_status_functions(0) :));
   add_command("functions", "help list",
               (: do_function_help_list() :));
   add_command("functions", "help <string'function name'>",
               (: do_function_help($4[0]) :));
   add_command("functions", "hints",
               (: do_approve_status_functions(1) :));
   add_command("expressions", "hints",
               (: do_approve_status(3) :));
   add_command("status", "hints",
               (: do_approve_status(1) :));
   add_command("browse", "waiting <string'id'>",
               (: do_approve_browse($4[0]) :));
   add_command("checkout", "<string'id'>",
               (: do_approve_checkout($4[0]) :));
   add_command("checkin", "<indirect:object>",
               (: do_approve_checkin($1) :));
   add_command("pay", "deficit",
               (: do_approve_pay_deficit() :));
   add_command("buy", "expression <string'expression|disable'>",
               (: do_approve_buy_expression($4[0]) :));
   add_command("use", "expression <string'expression|disable'>",
               (: do_approve_use_expression($4[0]) :));

   add_command("name", "shop <string'shop name'>",
               (: do_name_shop($4[0]) :));

   add_command("set", "sign <indirect:object'sign'>",
               (: do_set_sign($1) :) );
   add_command("remove", "sign",
               (: do_remove_sign() :) );

   add_command("stats", "items by name", (: do_stats_items(0) :));
   add_command("stats", "items by number", (: do_stats_items(1) :));
   add_command("stats", "items by value", (: do_stats_items(2) :));
#if USE_TRANSACTIONS
   add_command("stats", "transactions", (: do_stats_transactions() :));
#endif
   add_command("stats", "money", (: do_stats_money() :));

   add_command("check", "sell <indirect:object>",
                (: do_check_sell($1) :));

   add_command("category", "list", (: do_list_categories() :));
   add_command("category", "default <string'category'>",
               (: do_set_default_category($4[0]) :));
   add_command("category", "add <string'category'>",
               (: do_add_new_category($4[0]) :));
   add_command("category", "remove <string'category'>",
               (: do_remove_category($4[0]) :));
   add_command("category", "rename <string'category'> to <string'category'>",
               (: do_rename_category($4[0], $4[1]) :));

   add_command("collect", "royalties of <string'amount'>",
               (: do_collect_partial_royalties($4[0]) :));

   ::init();
} /* init() */
