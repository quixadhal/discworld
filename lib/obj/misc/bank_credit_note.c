/**
 * A credit note for transfering money to and from a franchise and a
 * master shop.
 * @author Pinkfish
 * @started Thu Jun 28 16:46:26 PDT 2001
 */
inherit "/std/object";
#include <money.h>

#define OBJECT_TAG "credit note"

private int _id;
private int _amount;
private string _franchise;
private string _bank;

void setup() {
   set_name("note");
   set_short("bank credit note");
   add_adjective( ({ "bank", "credit" }) );
   set_long("A small piece of paper which looks like some sort of "
            "bank transaction slip.\n");
   set_value(0);
   set_weight(1);
} /* setup() */

/**
 * This method is called to setup the read messages for the note once it
 * is made.
 */
void setup_read_messages() {
   string extra;
   string place;

   place = query_property("place");
   if (_amount < 0) {
     extra = "Agrees to pay " +
             MONEY_HAND->money_value_string(-_amount, place) +
             " to the master bank.\n";
   } else {
     extra = "Slip for a withdrawl of " +
             MONEY_HAND->money_value_string(-_amount, place) +
             " from the master bank.\n";
   }
   add_read_mess("Credit note #" + _id + " for the bank " +
                    _bank + " from the franchise " +
                    _franchise->query_address() + ".\n\n" + extra,
                    0, "common", 0);
} /* setup_read_messages() */

/**
 * This method sets the credit number id for the credit note.  This will
 * be unique for each franchise.
 * @param num the credit id
 */
void set_credit_num(int num) {
   _id = num;
} /* set_credit_num() */

/**
 * This method returns the credit number id for the credit not.
 * @return the credit id
 */
int query_credit_num() {
   return _id;
} /* query_credit_num() */

/**
 * This method sets the bank name of the credit note.
 * @param bank set the bank name
 */
void set_bank_name(string bank) {
   _bank = bank;
} /* set_bank_name() */

/**
 * This method returns the bank name for the credit note.
 * @return the bank name
 */
string query_bank_name() {
   return _bank;
} /* query_bank_name() */

/**
 * This method sets the franchise that will be used as the main base for
 * this credit note.
 * @param franchise the franchise as the base
 */
void set_franchise(string franchise) {
   _franchise = franchise;
} /* set_franchise() */

/**
 * This method returns the franchise for the credit note
 * @return this returns the franchise for the note
 */
string query_franchise() {
   return _franchise;
} /* query_franchise() */

/**
 * This method sets the amount the credit note is for.  +ve means that
 * we are transporting money to the master bank, -ve means we are taking
 * money out of the master bank.
 * @param amount the amount the note is for
 */
void set_amount(int amount) {
   _amount = amount;
} /* set_amount() */

/**
 * This method returns the amount the credit note is for.
 * @return the amount the note is for
 */
int query_amount() {
   return _amount;
} /* query_amount() */

/**
 * This checks to see if the note is valid still or not.
 */
int is_valid_note() {
   object bank;
   object* obs;
   object ob;
   int ret;

   bank = load_object(query_franchise());
   obs = bank->find_commercial_items("bank");
   //obs = filter(obs, (: $1->query_bank() :));
   if (sizeof(obs)) {
      foreach (ob in obs) {
         ret = ob->is_valid_note(query_credit_num());
         if (ret) {
            return 1;
         }
      }
   }
   return 0;
} /* is_valid_note() */

/**
 * This marks this credit note as processed.
 */
int do_process() {
   object bank;
   object* obs;
   object ob;
   int ret;

   bank = load_object(query_franchise());
   obs = bank->find_commercial_items("bank");
   //obs = filter(obs, (: $1->query_bank() :));
   if (sizeof(obs)) {
      foreach (ob in obs) {
         ret = ob->process_note(query_credit_num());
         if (ret) {
            return 1;
         }
      }
   }
   return 0;
} /* do_process() */

/** @ignore yes */
mixed* stats() {
   return ::stats() +
          ({ ({ "credit amount", query_amount() }),
             ({ "franchise", query_franchise() }),
             ({ "credit id", query_credit_num() }), });
} /* stats() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
   mapping map;

   map = ::query_dynamic_auto_load();
   add_auto_load_value(map, OBJECT_TAG, "amount", query_amount());
   add_auto_load_value(map, OBJECT_TAG, "franchise", query_franchise());
   add_auto_load_value(map, OBJECT_TAG, "id", query_credit_num());
   add_auto_load_value(map, OBJECT_TAG, "bank", query_bank_name());
   return map;
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_dynamic_arg(mapping map) {
   ::init_dynamic_arg(map);

   set_amount(query_auto_load_value(map, OBJECT_TAG, "amount"));   
   set_franchise(query_auto_load_value(map, OBJECT_TAG, "franchise"));   
   set_credit_num(query_auto_load_value(map, OBJECT_TAG, "id"));   
   set_bank_name(query_auto_load_value(map, OBJECT_TAG, "bank"));   
} /* init_dynamic_arg() */
