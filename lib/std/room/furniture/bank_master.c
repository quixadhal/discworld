/**
 * This is the bank inheritable.  It handles all the bank transactions
 * and other such exciting stuff.
 * @author Pinkfish
 */
#include <money.h>
#include <move_failures.h>

nosave inherit "/std/room/furniture/commercial";
nosave inherit "/std/shops/inherit/bank_master";

#define OBJECT_TAG "bank master"

private nosave string _place;

void create() {
  add_help_file("bank_master");
  commercial::create();
  bank_master::create();

  set_commercial_name("bank");
} /* create() */

/**
 * This method does the actual save.
 */
void event_save(object ob) {
   environment()->event_save(ob);
} /* event_save() */

mixed *stats() {
  return commercial::stats() + 
         bank_master::stats();
} /* stats() */

/**
 * Find the object associated with the bank.
 * @param franchise the franchise to find the object for
 * @return the bank object
 */
object find_bank_object(string franchise) {
   object* obs;
   object room;

   room = load_object(franchise);
   if (room->query_bank()) {
      return room;
   }
   obs = filter(room->find_commercial_items(query_commercial_name()),
                (: $1->query_bank_name() == query_bank_name() :));
   if (sizeof(obs)) {
      return obs[0];
   }
   return 0;
} /* find_bank_object() */

/**
 * Show a list of all the franchises.
 */
int do_list_franchises() {
   string franchise;
   int pos;
   object bank_ob;
   string ret;
   string place;

   if (!sizeof(BANK_HANDLER->query_franchises(query_bank_name()))) {
      add_failed_mess("This bank has no franchises.\n");
      return 0;
   }

   place = query_money_place();

   ret = "Franchises:\n";
   foreach (franchise in BANK_HANDLER->query_franchises(query_bank_name())) {
      bank_ob = find_bank_object(franchise);
      ret += sprintf("$I$6=   %c %s Balance %s (cost per month %s)\n",
                      pos + 'A', franchise->the_short(),
                      MONEY_HAND->money_value_string(bank_ob->query_total_difference(), place),
                      MONEY_HAND->money_value_string(BANK_HANDLER->query_bank_franchise_cost_per_month(query_bank_name(), franchise), place));
      pos++;
   }
   write("$P$Franchises$P$" + ret);
   return 1;
} /* do_list_franchises() */

/**
 * Set how much it costs to open a franchise.
 */
int do_set_franchise_cost(string cost) {
   int amount;
   string place;

   place = query_money_place();
   amount = MONEY_HAND->value_from_string(cost, place);
   if (amount <= 0) {
      add_failed_mess("Unknown amount " + cost + ".\n");
      return 0;
   }

   BANK_HANDLER->set_bank_default_cost_to_open(query_bank_name(), amount);
   add_succeeded_mess("$N set$s the cost to open a franchise to " +
          MONEY_HAND->money_value_string(BANK_HANDLER->query_default_cost_to_start(query_bank_name()), place) + "\n");
   return 1;
} /* do_set_franchise_cost() */

/**
 * Set how much it costs to open a franchise.
 */
int do_set_default_monthly_cost(string cost) {
   int amount;
   string place;

   place = query_money_place();
   amount = MONEY_HAND->value_from_string(cost, place);
   if (amount <= 0) {
      add_failed_mess("Unknown amount " + cost + ".\n");
      return 0;
   }

   BANK_HANDLER->set_bank_default_cost_per_month(query_bank_name(), amount);
   add_succeeded_mess("$N set$s the cost to open a franchise to " +
          MONEY_HAND->money_value_string(BANK_HANDLER->query_default_cost_to_start(query_bank_name()), place) + "\n");
   return 1;
} /* do_set_default_cost() */

/**
 * This method sets the new name of the bank.
 * @param new_name the new bank name
 */
int do_set_main_bank_name(string new_name) {
   string* names;
   string name;

   if (query_bank_name()) {
      add_failed_mess("This bank already has a name.\n");
      return 0;
   }

   //
   // Check and see if the name is valid and not already used.
   //
   names = BANK_HANDLER->query_banks();
   foreach (name in names) {
      if (lower_case(name) == lower_case(new_name)) {
         add_failed_mess("There is already a bank called " +
                         name + ".\n");
         return 0;
      }
   }

   write("Do you really want to set the bank name to " + new_name +
         " (Make sure you have correct capitalisation)? ");
   input_to("check_bank_name", 0, new_name);
   add_succeeded_mess("");
   return 1;
} /* do_set_main_bank_name() */

/** @ignore yes */
void check_bank_name(string response, string new_name) {
   if (!strlen(response) || lower_case(response)[0] != 'y') {
      write("Ok, aborting.\n");
      return ;
   }
   write("You set the bank name to " + new_name + ".\n");
   BANK_HANDLER->create_new_bank(new_name, file_name(environment()));
   ::set_bank_name(new_name);
} /* set_bank_name() */

/**
 * This method sets the cut
 * @param percent the cut percentage
 */
int do_set_cut(int percent) {

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to change the paramaters of "
                   "$D.\n");
      return 0;
   }
   if (percent < 0) {
      add_failed_mess("You cannot set the cut less than 0.\n");
      return 0;
   }
   if (percent > 100) {
      add_failed_mess("You cannot set the cut to greator than 100%.\n");
      return 0;
   }

   set_percentage(percent);

   add_succeeded_mess("$N set$s the cut to " + percent + "% on $D.\n");
   return 1;
} /* do_set_cut() */

/**
 * This method sets the ante amounts.
 * @param str the amount string
 */
int do_set_open_cost(string str) {
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
   set_account_cost(value);
   add_succeeded_mess("$N set$s the account open cost to " +
          MONEY_HAND->money_value_string(value, place) + " on $D.\n");

   return 1;
} /* do_set_open_cost() */

/**
 * This method allows you to change the cost per month to the specific
 * franchise.
 * @param id the franchise id
 * @param amount the new amount to cost per month
 */
int do_set_franchise_cost_per_month(string id, string amount) {
   string *franchises;
   int id_num;
   int amt;

   id = lower_case(id);
   if (strlen(id) != 1) {
      add_failed_mess("Must be one letter.\n");
      return 0;
   }
   franchises = BANK_HANDLER->query_franchises(query_bank_name());
   id_num = id[0] - 'a';
   if (id_num < 0 || id_num >= sizeof(franchises)) {
      add_failed_mess("The id of the franchises must be between 'A' and '" +
                      sprintf("%c", 'A' + sizeof(franchises) - 1) + ".\n");
      return 0;
   }

   amt = MONEY_HAND->value_from_string(amount, query_money_place());
   if (amt <= 0) {
      add_failed_mess("The amount " + amount + " is invalid.\n");
      return 0;
   }

   BANK_HANDLER->set_bank_franchise_cost_per_month(query_bank_name(),
               franchises[id_num], amt);
   add_succeeded_mess(({ "You set the cost per month for the franchise " +
                         franchises[id_num]->query_address() + " to " +
                         MONEY_HAND->money_value_string(amt, query_money_place()) + ".\n",
                         "" }));
   return 1;
} /* do_set_franchise_cost_per_month() */

/** @ignore yes */
string query_main_status(int hints) {
   string ret;
   string place;

   place = query_money_place();
   ret = "$I$0=" + the_short() + ":\n";
   ret += "$I$6=   Bank Name             : " +
          query_bank_name() + "\n";
   ret += "$I$6=   Revenue               : " +
          MONEY_HAND->money_value_string(query_revenue(), place) + "\n";
   ret += "$I$6=   Cost to buy franchise : " +
          MONEY_HAND->money_value_string(BANK_HANDLER->query_bank_default_cost_to_open(query_bank_name()), place) + "\n";
   if (hints) {
      ret += "$I$9=      Hint: set buy franchise cost <amount> on <object>\n";
   }
   ret += "$I$6=   Franchise cost per month : " +
          MONEY_HAND->money_value_string(BANK_HANDLER->query_bank_default_cost_per_month(query_bank_name()), place) + "\n";
   if (hints) {
      ret += "$I$9=      Hint: set default franchise monthly cost <amount> on <object>\n";
   }
   ret += "$I$6=   Percentage            : " +
             query_percentage() + "%\n";
   if (hints) {
      ret += "$I$9=      Hint: set percentage <number> on <counter>\n";
   }

   ret += "$I$6=   Cost to open account  : " +
          MONEY_HAND->money_value_string(query_account_cost(), place) + "\n";
   if (hints) {
      ret += "$I$9=      Hint: set account cost <amount> on <object>\n";
   }
   ret += "$I$6=   Number of Franchises  : " +
          sizeof(BANK_HANDLER->query_franchises(query_bank_name())) + "\n";
   return ret;
} /* query_main_status() */

mapping query_dynamic_auto_load() {
   mapping map;

   map = ::query_dynamic_auto_load();
   add_auto_load_value(map, OBJECT_TAG, "bank", query_bank_name());
   add_auto_load_value(map, OBJECT_TAG, "open", query_account_cost());
   add_auto_load_value(map, OBJECT_TAG, "percentage", query_percentage());
   add_auto_load_value(map, OBJECT_TAG, "difference", query_total_difference());
   return map;
} /* query_dynamic_auto_load() */

void init_dynamic_arg(mapping map, object player) {
   commercial::init_dynamic_arg(map, player);
   set_bank_name(query_auto_load_value(map, OBJECT_TAG, "bank"));
   set_account_cost(query_auto_load_value(map, OBJECT_TAG, "open"));
   set_percentage(query_auto_load_value(map, OBJECT_TAG, "percentage"));
   set_total_difference(query_auto_load_value(map, OBJECT_TAG, "difference"));
} /* init_dyanmic_arg() */

void init() {
   commercial::init();
   bank_master::init();

   if (is_allowed(this_player()->query_name())) {
      add_command("set", "bank name <string'name'> on <direct:object>",
                  (: do_set_main_bank_name($4[0]) :));
      add_command("set", "buy franchise cost <string'cost'> on <direct:object>",
                  (: do_set_franchise_cost($4[0]) :));
      add_command("set", "franchise monthly cost <string'cost'> on <direct:object>",
                  (: do_set_default_monthly_cost($4[0]) :));
      add_command("set", "percentage <number> on <direct:object>",
                  (: do_set_cut($4[0]) :));
      add_command("set", "account cost <string'open cost'> on <direct:object>",
                  (: do_set_open_cost($4[0]) :));
      add_command("set", "franchise monthly cost <string'cost'> for <string'franchise id'> on <direct:object>",
                  (: do_set_franchise_cost_per_month($4[1], $4[0]) :));
      add_command("list", "franchises on <direct:object>",
                  (: do_list_franchises :));
   } 
} /* init() */
