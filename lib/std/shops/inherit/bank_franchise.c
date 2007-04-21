/**
 * The inherit to use for franchises for the bank.
 * @author Pinkfish
 * @started Fri Jun 15 18:52:48 PDT 2001
 */
inherit "/std/shops/inherit/bank_base";
#include <money.h>
#include <shops/bank.h>

class credit_note {
   int num;
   int amt;
   int made;
   string maker;
}

private int _credit_num;
private class credit_note* _notes;

void create() {
   _notes = ({ });
   ::create();
} /* create() */

/** @ignore yes */
int is_allowed(string name) {
   return 0;
}

/**
 * Are they allowed for credit transactions, listing outstanding credit,
 * making credit notes, etc.  This should allow the standard owner and the
 * owner of the main branch.
 * @param name the player name
 */
int is_allowed_credit(string name) {
   return is_allowed(name) ||
          name == query_main_branch_owner();
} /* is_allowed_credit() */

/**
 * This allows the current credit number for credit notes to be set.
 * @param num the new credit number
 */
protected void set_credit_num(int num) {
   _credit_num = num;
} /* set_credit_num() */

/**
 * This method returns the current credit number.
 * @return the current credit number
 */
int query_credit_num() {
   return _credit_num;
} /* query_credit_num() */

/**
 * This allowed the set of outstanding credit notes to be set.
 * @prarm notes the set of outstanding notes
 */
protected void set_credit_notes(class credit_note* notes) {
   _notes = notes;
} /* set_credit_notes() */

/**
 * This method returns the array of outstanding credit notes.
 * @return the array of outstanding credit notes
 */
class credit_note* query_credit_notes() {
   return copy(_notes);
} /* query_credit_notes() */

/**
 * This method returns the amount of money currently in outstanding credit
 * notes.
 * @return the amount in outstanding credit notes
 */
int query_outstanding_credit_value() {
   int diff;
   class credit_note note;

   foreach (note in _notes) {
      diff += note->amt;
   }
   return diff;
} /* query_outstanding_credit_value() */

/**
 * This method creates a credit note for the store based on wild and
 * exciting stuff.
 * @return 1 on success, 0 on failure
 */
int do_create() {
   object ob;
   int diff;

   if (!is_allowed_credit(this_player()->query_name())) {
      add_failed_mess("The bank is not open for creating credit notes.\n");
      return 0;
   }

   diff = query_total_difference() - query_outstanding_credit_value();

   if (diff == 0) {
      add_failed_mess("The bank is currently at a zero balance with the "
                      "outstanding credit notes.\n");
      return 0;
   }

   ob = clone_object(BANK_CREDIT_NOTE);
   ob->set_credit_num(++_credit_num);
   ob->set_franchise(query_franchise());
   ob->set_bank_name(query_bank_name());
   ob->set_amount(query_total_difference());
   ob->add_property("place", query_money_place());
   ob->setup_read_messages();
   ob->move(this_player());
   if (!_notes) {
      _notes = ({ });
   }
   _notes += ({ new(class credit_note, num : ob->query_credit_num(),
                                   amt : ob->query_amount(),
                                   maker : this_player()->query_cap_name(),
                                   made : time()) });
   event_save(this_object());
   //adjust_total_difference(-query_total_difference());
   add_succeeded_mess("$N $V " + ob->a_short() + ".\n");
   return 1;
} /* do_create() */

/** @ignore yes */
int do_list() {
   class credit_note note;
   string ret;
   string place;

   if (!is_allowed_credit(this_player()->query_name())) {
      add_failed_mess("The bank is not open for listing credit notes.\n");
      return 0;
   }

   place = query_money_place();

   if (!sizeof(_notes)) {
      add_failed_mess("No credit notes to list.\n");
      return 0;
   }

   ret = "Credit notes\n\n";
   foreach (note in _notes) {
      ret += "$I$6=" + sprintf("%3d) ", note->num) +
             ctime(note->made) + ": by " +
             note->maker + " for " +
             MONEY_HAND->money_value_string(note->amt, place) + "\n";
   }

   write("$P$Credit Notes$P$" + ret);
   return 1;
} /* do_list() */

/** @ignore yes */
int do_recind(int id) {
   class credit_note note;
   int i;
   string place;

   if (!is_allowed_credit(this_player()->query_name())) {
      add_failed_mess("The bank is not open for listing credit notes.\n");
      return 0;
   }

   for (i = 0; i < sizeof(_notes); i++) {
      note = _notes[i];
      if (note->num == id) {
          place = query_money_place();
          _notes = _notes[0..i-1] + _notes[i+1..];
          add_succeeded_mess(({ 
             "You recind #" + note->num + " made at " +
             ctime(note->made) + " by " +
             note->maker + " for " +
             MONEY_HAND->money_value_string(note->amt, place) + "\n", "" }));
          return 1;
      }
   }

   add_failed_mess("Unable to find credit note #" + id + ".\n");
   return 0;
} /* do_recind() */

/**
 * This method checks to find if the note is valid or not.
 * @param id the not to check
 * @return 1 on success, 0 on failure
 */
int is_valid_note(int id) {
   int i;

   for (i = 0; i < sizeof(_notes); i++) {
      if (_notes[i]->num == id) {
         return 1;
      }
   }
   return 0;
} /* is_valid_note() */

/**
 * THis is called when a specific credit note is processed
 * @param id the id of the note to process
 * @return 1 on a sucessful processing
 */
int process_note(int id) {
   int i;

   if (!is_valid_note(id)) {
      return 0;
   }
   for (i = 0; i < sizeof(_notes); i++) {
      if (_notes[i]->num == id) {
         adjust_total_difference(-_notes[i]->amt);
         _notes = _notes[0..i-1] + _notes[i+1..];
         i--;
      }
   }
   event_save(this_object());
   return 1;
} /* process_note() */


void init() {
   ::init();
   add_command("create", "credit note");
   add_command("list", "credit notes");
   add_command("recind", "credit note <number'id'>", (: do_recind($4[0]) :));
} /* init() */
