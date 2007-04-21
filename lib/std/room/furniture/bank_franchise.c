// Pinkfish
// Started Wed May 30 21:37:15 PDT 2001

inherit "/std/room/furniture/commercial";
inherit "/std/shops/inherit/bank_franchise";
#include <money.h>
#include <am_time.h>

#define SAVE_TIME 300

#define OBJECT_TAG "bank franchise"

#define MONTH_LENGTH (AM_SECONDS_PER_WEEK * 4)

private nosave int _maximum_total_difference;
private int _last_paid;

//
// This gives an estimated return on the payment.  For example 50%
// return would mean you get back approximately 50% of your money on
// average
//
void create() {
   _maximum_total_difference = 4000;
   _last_paid = time();
   bank_franchise::create();
   set_shop_use_types(({ "use", "buy" }));
   commercial::create();

   set_commercial_name("bank");
   set_commercial_information("A franchise is connected up to a main "
          "bank.  You will pay a monthly fee to the main bank that is "
          "settable by the owner of the main bank.  The maximum balance "
          "of the bank counter is variable, but it is controlled at purchase "
          "time.  The bank balance and the float of the shop are tied "
          "together.");
   add_help_file("bank_franchise");
} /* create() */


/**
 * A call to make sure this registers as a player bank.
 * @return 1 for player banks
 */
int query_player_bank() {
   return 1;
} /* query_player_bank() */

/**
 * This method sets the maximum total difference allowed by this
 * franchise.
 * @param max the maximum total difference
 */
void set_maximum_total_difference(int max) {
   _maximum_total_difference = max;
} /* set_maximum_total_difference() */

/**
 * This method returns the maximum total difference allowed by this
 * franchise.
 * @return the maximum allowed total difference
 */
int query_maximum_total_difference() {
   return _maximum_total_difference;
} /* query_maximum_total_difference() */

/**
 * This method sets the last paid date of the franchise.
 * @param paid the last paid date
 */
void set_last_paid(int paid) {
   _last_paid = paid;
} /* set_last_paid() */

/**
 * This method returns the last paid date of the franchise
 * franchise.
 * @return the last paid date
 */
int query_last_paid() {
   return _last_paid;
} /* query_last_paid() */

/**
 * @ignore yes
 * With franchises the float and the accounts are tied.
 */
void adjust_account(string person, int amount) {
   adjust_float(amount);
} /* adjust_account() */

/** @ignore yes */
object find_main_bank() {
   object office;
   object* obs;

   office = load_object(BANK_HANDLER->query_bank_master_office(query_bank_name()));
   if (!office) {
      return 0;
   }

   if (office->query_bank()) {
      return office;
   }

   obs = filter(office->find_commercial_items(query_commercial_name()),
                (: $1->query_bank_name() == query_bank_name() :));
   if (!sizeof(obs)) {
      return 0;
   }

   return obs[0];
} /* find_main_bank() */

/** @ignore yes */
int can_adjust_account(string person, int amount) {
   int amt;

   amt = query_total_difference() + amount;
   if (amt < -query_maximum_total_difference()  ||
       amt > query_maximum_total_difference()) {
      if (amt < 0) {
         add_failed_mess("This bank does not have the funds to handle "
                            "your request.\n");
      } else {
         add_failed_mess("This bank does not have the vault space to handle "
                            "your request.\n");
      }
      return 0;
   }
   return ::can_adjust_account(person, amount);
} /* can_adjust_account() */

/** @ignore yes */
string query_commercial_information() {
   string* bits;
   string bank;
   string place;

   place = previous_object()->query_money_place();
   bits = ({ });
   foreach (bank in BANK_HANDLER->query_banks()) {
      bits += ({ bank + " costs " + 
                 MONEY_HAND->money_value_string(BANK_HANDLER->query_bank_default_cost_per_month(bank), place) +
                 " per month and " +
                 MONEY_HAND->money_value_string(BANK_HANDLER->query_bank_default_cost_to_open(bank), place) +
                 " to open" });
   }
   return ::query_commercial_information() + 
          "This counter has a fixed maximum bank balance of " +
          MONEY_HAND->money_value_string(query_maximum_total_difference(), place) +
          ".  The costs of the franchises are " + query_multiple_short(bits) +
          ".";
} /* query_commercial_information() */

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

/** @Ignore yes */
int is_allowed(string name) {
   return commercial::is_allowed(name);
} /* is_allowed() */

void init() {
   bank_franchise::init();
   commercial::init();

   if (is_allowed(this_player()->query_name())) {
      add_command("set", "percentage <number> on <direct:object>",
                  (: do_set_cut($4[0]) :));
      add_command("set", "account cost <string'open cost'> on <direct:object>",
                  (: do_set_open_cost($4[0]) :));
   }
} /* init() */

string query_franchise() {
   return file_name(environment());
} /* query_franchise() */

void event_save(object ob) {
   environment()->event_save(ob);
} /* event_save() */

mapping query_commercial_options() {
   string bank;
   mapping ret;

   ret = ([ ]);
   foreach (bank in BANK_HANDLER->query_banks()) {
      ret[bank] = BANK_HANDLER->query_bank_default_cost_to_open(bank);
   }
   return ([ "branch" : ret ]);
} /* query_commercial_options() */

void set_commercial_option(string type, string name, object room) {
   switch (type) {
   case "branch" :
      set_bank_name(name);
      BANK_HANDLER->add_new_franchise(name,
                                      file_name(room->query_main_room()));
      break;
   }
} /* set_commercial_option() */

/**
 * This returns the amount of money te franchise will cost per month.
 * @return the cost per month
 */
int query_monthly_fee() {
   return BANK_HANDLER->query_bank_franchise_cost_per_month(
             query_bank_name(), query_franchise());
} /* query_monthly_fee() */

/** @ignore yes */
int is_open_for(string type, string name) {
   if (query_monthly_fee() == -1) {
      return 0;
   }
   return ::is_open_for(type, name);
} /* is_open_for() */

/**
 * This method sets up the calout for the next payment period.
 */
void setup_call_out() {
   int tim;

   if (!_last_paid) {
      _last_paid = time();
   }
   tim = _last_paid + MONTH_LENGTH - time();
   call_out("make_payment", tim);
} /* setup_call_out() */

/**
 * This method makes the payment for the month.
 */
void make_payment() {
   _last_paid += MONTH_LENGTH;
   adjust_float(-query_monthly_fee());
   //find_master_bank()->adjust_float(query_monthly_fee());
   setup_call_out();
} /* make_payment() */

string query_main_status(int hints) {
   string ret;
   string place;

   // Make sure it is added.
   add_help_file("bank_franchise");
   place = query_money_place();
   ret = "$I$0=" + the_short() + ":\n"
         "$I$6=   Bank Branch                  : " + query_bank_name() + "\n"
         "$I$6=   Revenue                      : " +
             MONEY_HAND->money_value_string(query_revenue(), place) + "\n"
           "$I$6=   Master Address               : " +
             BANK_HANDLER->query_bank_master_office(query_bank_name())->query_address() +
         "\n$I$6=   Maximum allowed bank balance : " +
             MONEY_HAND->money_value_string(query_maximum_total_difference(), place) +
         " (fixed)\n$I$6=   Bank Balance                 : " +
             MONEY_HAND->money_value_string(query_total_difference(), place) +
         "\n$I$6=   Credit Notes Outstanding     : " +
             MONEY_HAND->money_value_string(query_outstanding_credit_value(), place) + "\n";
   if (hints) {
      ret += "$I$6=     Hint: list credit notes\n";
      ret += "$I$6=     Hint: create credit note\n";
      ret += "$I$6=     Hint: recind credit note <id>\n";
   }
   ret += "$I$6=   Monthly Fee                  : " +
             MONEY_HAND->money_value_string(query_monthly_fee(), place) +
         " (set by bank owner)\n"
         "$I$6=   Next Pay Day                 : " +
             AM_TIME_HANDLER->query_am_time(_last_paid + MONTH_LENGTH) +
             " (" + ctime(_last_paid + MONTH_LENGTH) + ")\n"
         "$I$6=   Percentage                   : " +
             query_percentage() + "%"
         "\n$I$6=   Open account cost            : " +
             MONEY_HAND->money_value_string(query_account_cost(), place) +
       "\n";
   if (hints) {
      ret += "$I$6=     Hint: set open cost <amount> on <bank>\n";
   }
   return ret;
} /* query_main_status() */

mapping query_dynamic_auto_load() {
   mapping map;

   map = ::query_dynamic_auto_load();
   add_auto_load_value(map, OBJECT_TAG, "bank", query_bank_name());
   add_auto_load_value(map, OBJECT_TAG, "open", query_account_cost());
   add_auto_load_value(map, OBJECT_TAG, "percentage", query_percentage());
   add_auto_load_value(map, OBJECT_TAG, "difference", query_total_difference());
   add_auto_load_value(map, OBJECT_TAG, "notes", query_credit_notes());
   add_auto_load_value(map, OBJECT_TAG, "credit num", query_credit_num());
   add_auto_load_value(map, OBJECT_TAG, "last paid", query_last_paid());
   return map;
} /* query_dynamic_auto_load() */

void init_dynamic_arg(mapping map, object player) {
   commercial::init_dynamic_arg(map, player);
   set_bank_name(query_auto_load_value(map, OBJECT_TAG, "bank"));
   set_account_cost(query_auto_load_value(map, OBJECT_TAG, "open"));
   set_percentage(query_auto_load_value(map, OBJECT_TAG, "percentage"));
   set_total_difference(query_auto_load_value(map, OBJECT_TAG, "difference"));
   set_credit_notes(query_auto_load_value(map, OBJECT_TAG, "notes"));
   set_credit_num(query_auto_load_value(map, OBJECT_TAG, "credit num"));
   set_last_paid(query_auto_load_value(map, OBJECT_TAG, "last paid"));
   setup_call_out();
} /* init_dyanmic_arg() */

mixed* stats() {
   return bank_franchise::stats() + commercial::stats() + ({
          ({ "last paid", ctime(_last_paid) }) });
} /* stats() */
