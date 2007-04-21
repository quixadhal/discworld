/**
 * This is the council treasury inheritable.  It does fun stuff!  Kudos to
 * Pinkfish for the deposit/withdrawl stuff, which I shamelessy stole 
 * from /std/bank.
 * @author Kili
 * @started Late October 2001
 */

inherit "/std/room";
inherit "/std/room/inherit/council_base";

#define TWO_WEEKS (60 * 60 * 24 * 7 * 2)
#define READ_LOG 1
#define DEPOSIT 2
#define MAGISTRATE 2
#define CITIZEN 1
#define ANYONE 0

#define COUNCIL_LOG_WITHDRAW_INDEX 0
#define COUNCIL_LOG_DEPOSIT_INDEX  1

#include <money.h>
#include <move_failures.h>
#include <nomic_system.h>

private nosave string _save_file;
private nosave string _archive;
// One toon. It just makes things easier, all right? =P
private int _balance = 200;
private int _read;
private int _dep;
private int *_log;

int do_withdraw(int,
                string,
                string);
int do_deposit(int);
int do_check();

class transaction {
   string person;
   int amount;
   string reason;
   int date;
   string action;
}

class transaction *transactions = ({ });

/**
 * Save the variables for this treasury.
 */
void save_me()
{
   if (!_save_file) {
      debug_printf("Error: No save file set.\n");
      return;
   }
   unguarded((: save_object, _save_file :));
   return;
}                               /* save_me() */

/**
 * Load the variables for this treasury.
 */
void load_me()
{
   if (!_save_file)
      return;

   if (file_size(_save_file + ".o") > 0) {
      unguarded((: restore_object, _save_file :));
      return;
   }
   return;
}

/**
 * This will check to see if the player attempting an action is allowed to
 * perform that action.
 * @param player The player attempting the action
 * @param type The type of action (deposting, reading logs, etc.).
 */
int security_check(object player,
                   int type)
{
   int person;
   int tmp;

   if (!type)
      return -1;

   if (!player)
      return -1;

// Just for testing, no longer needed.
//   if (player->query_creator())
//      return 1;
   if (NOMIC_HANDLER->is_magistrate_of(query_council_area(), this_player()->query_name())) {
      person = 2;
   }

   if (NOMIC_HANDLER->is_citizen_of(query_council_area(), this_player()->query_name())) {

      person = 1;
   } else {
      person = 0;
   }
   // What are they trying to do? Find that out and make tmp equal what
   // security level it is.
   if (type == READ_LOG)
      tmp = _read;

   if (type == DEPOSIT)
      tmp = _dep;

   // Do they need to be a magistrate or a citizen? Are they?
   switch (tmp) {
   case MAGISTRATE:
      return (person == 2);
      break;
   case CITIZEN:
      return (person >= 1);
      break;
   case ANYONE:
      return 1;
   }
}

/**
 * This will check the time of the last transactions, and move them into a
 * log file if they are over two weeks old.
 */
void check_time()
{
   string str;

   while (transactions[0]->date + TWO_WEEKS < time()) {
      str =
         capitalize(transactions[0]->person) + " " + transactions[0]->action +
         " " + MONEY_HAND->money_value_string(transactions[0]->amount, query_property("place"));
      str +=
         (transactions[0]->action ==
          "withdrew" ? " for " + transactions[0]->reason : "");
      str += " on " + amtime(transactions[0]->date) + ".\n";
      log_file(_archive, str, 0);
      transactions = transactions[1..];
   }
}                               /* check_time() */

/**
 * @ignore yes
 */
void log_action(object player,
                int amount,
                string action,
                string word)
{
   // This bit is actually superfluous, but why not!
   string reason = (word ? word : "no reason specified");
   class transaction new_trans;

   if (!player)
      return;

   if (!amount)
      return;

   if (!action)
      return;

 new_trans = new (class transaction, person: player->query_name(), amount: amount, reason: reason, date: time(), action:action);
   transactions += ({ new_trans });
   check_time();
   save_me();
}                               /* log_withdrawl */

/**
 * @ignore yes
 */
varargs int view_transactions(string player)
{
   int i;
   int f;
   string ret;
   
   if (!security_check(this_player(), READ_LOG)) {
      return
         add_failed_mess("You aren't allowed to view the transactions.\n");
   }
   ret = "A peek at the log book reveals:\nTransactions\n---\n\n";

   if (!sizeof(transactions)) {
      ret += "None so far.\n";
      write("$P$Transactions$P$" + ret);
      return 1;
   }

   if(player) {
      for(i = 0; i <sizeof(transactions); i++) {
         if (transactions[i]->person != lower_case(player)) {
            continue;
         }
            ret += capitalize(transactions[i]->person) + " " +
                transactions[i]->action + " " +
                MONEY_HAND->money_value_string(transactions[i]->amount,
                query_property("place")) +
                (transactions[i]->action ==
                "withdrew" ? " for " + transactions[i]->reason : "") +
                " on " + amtime(transactions[i]->date)
                + ".\n";
         
      }
   } else {    
      for (i = 0; i < sizeof(transactions); i++) {
         
      ret += capitalize(transactions[i]->person) + " " +
          transactions[i]->action + " " +
          MONEY_HAND->money_value_string(transactions[i]->amount,
          query_property("place")) +
          (transactions[i]->action ==
          "withdrew" ? " for " + transactions[i]->reason : "") +
          " on " + amtime(transactions[i]->date)
          + ".\n";
      }
   }
   write("$P$Transactions$P$" + ret);
   return 1;
}                               /* view_transactions */

/**
 * @ignore yes
 */
void create()
{
   set_short("council treasury");
   set_long("This is the treasury for the council.\n");
   ::create();
   load_me();
   add_help_file( "council_treasury" );
}                               /* create() */

/**
 * @ignore yes
 */
void init()
{
   ::init();

   this_player()->add_command("withdraw", this_object(), "<number'amount'> "
                              "<string'type'> for <string'reason'>",
                              (: do_withdraw($4[0], $4[1], $4[2]) :));
   this_player()->add_command("deposit", this_object(), "<string>",
                              (: do_deposit($4[0]) :));
   this_player()->add_command("check", this_object(), "balance");
   this_player()->add_command("view", this_object(),
                              "transactions",
                              (: view_transactions() :));
   this_player()->add_command("view", this_object(),
                              "transactions [by] <word'player'>",
                              (: view_transactions($4[0]) :));
}                               /* init() */

/**
 * Set the save file for the treasury
 * @param file The file to save to
 */
void set_save_file(string file)
{
   _save_file = file;
}                               /* set_save_file() */

/**
 * Set the location of the treasury, to determine what type of currency to use
 * @param word The location
 */
void set_place(string word)
{
   add_property("place", word);
}                               /* set_place() */

/**
 * @ignore yes
 */
int do_check()
{
   write("The treasury contains " + MONEY_HAND->money_value_string(_balance,
                                                                   query_property("place")) +
         ".\n");
   return 1;
}                               /* do_check() */

/**
 * @ignore yes
 */
int do_withdraw(int number,
                string type,
                string reason)
{
   int amount;
   int best;
   object money;

   if (!NOMIC_HANDLER->is_magistrate_of(query_council_area(), this_player()->query_name())) {
      notify_fail("You must be a magistrate to withdraw funds.\n");
      return 0;
   }

   if (number <= 0) {
      notify_fail("You must withdraw something.\n");
      return 0;
   }

   money = clone_object(MONEY_OBJECT);
   money->set_money_array((mixed *) MONEY_HAND->query_values_in(query_property("place")));
   if ((best = (int) money->find_best_fit(type)) == -1) {
      notify_fail("That currency is not legal tender here.  Sorry.\n");
      money->dest_me();
      return 0;
   }
   type = ((mixed *) money->query_money_array())[best];
   amount = number * ((mixed *) money->query_money_array())[best + 1];
   if ((amount > _balance) || (amount < 0)) {
      notify_fail("There isn't that much in the account.\n");
      money->dest_me();
      return 0;
   }
   money->set_money_array(({ type, number }));
   _balance -= amount;
   save_me();
   this_player()->add_succeeded_mess(this_object(),
                                     ({ "You withdraw " +
                                        (string) money->short() + ".\n" +
                                        "There is " +
                                        (string) MONEY_HAND->
                                        money_value_string(_balance,
                                                           query_property("place")) +
                                        " in your account after the withdrawal.\n",
                                        "$N withdraws some money from the treasury.\n" }),
                                     ({ }));
   if ((int) money->move(this_player()) != MOVE_OK) {
      write
         ("You are too heavily burdened to collect the money, so the teller "
          + "puts it on the counter for you.\n");
      money->move(this_object());
   }

   if (_log[COUNCIL_LOG_WITHDRAW_INDEX]) {
      log_action(this_player(), amount, "withdrew", reason);
   }

   this_player()->save();
   return 1;
}                               /* do_withdraw() */

/**
 * @ignore yes
 */
int do_deposit(string words)
{
   int amount;
//int total;
   mixed money;
   object *tender;
   object legal;
   object illegal;
   string howmuch;

   if (!security_check(this_player(), DEPOSIT)) {
      return add_failed_mess("You are not allowed to deposit.\n");
   }

   money = (mixed) MONEY_HAND->parse_money(words, this_player(), query_property("place"));
   if (intp(money)) {
      switch (money) {
      case NO_MATCH:
         return notify_fail("You do not have \"" + words + "\".\n");
      case NO_MONEY:
         return notify_fail("You can only deposit money.\n");
      default:
         return notify_fail("You can only deposit legal tender.\n");
      }
   }
   // Figure out the legal tender
   tender = MONEY_HAND->filter_legal_tender(money, query_property("place"));
   legal = tender[0];
   illegal = tender[1];
   if (legal) {
      amount = legal->query_value_in(query_property("place"));
      // Minimum amount added to prevent depositing 1c 1000 times to avoid
      // charges.
      if (amount < 200) {
         legal->move(this_player());
         if (illegal) {
            illegal->move(this_player());
         }
         add_failed_mess("That is too small an amount to deposit.\n");
         return 0;
      }

      _balance += amount;
      save_me();
   }
   // Give them back their illegal money and zap the other stuff.
   if (illegal) {
      illegal->move(this_player());
   }

   if (legal) {
      howmuch = legal->the_short();
      legal->move("/room/rubbish");
      this_player()->save();
   } else {
      add_failed_mess("Unable to find any legal money to deposit.\n");
      return 0;
   }

   write("You give the teller " + howmuch + ".\n");

   this_player()->add_succeeded_mess(this_object(),
                                     ({ "$N $V " + words + ".\n"
                                        "This gives a total of " +
                                        MONEY_HAND->
                                        money_value_string(_balance,
                                                           query_property("place")) +
                                        " in the treasury.\n",
                                        "$N deposits some money into the treasury.\n" }),
                                     ({ }));

   if (_log[COUNCIL_LOG_DEPOSIT_INDEX])
      log_action(this_player(), amount, "deposited", "");
   return 1;
}                               /* do_deposit() */

/**
 * This sets what you must be to read the log and deposit cash, and also
 * wether you are logging withdrawls and deposits.
 * @param read_log 2 for magistrate, 1 for citizen, 0 for anyone is able to
 * read the log
 * @param deposits Same as for read_log, to see who can deposit
 * @param logging ({withdrwals, deposits}), are we logging? 0 for no, 1 for yes
 */
void set_security_levels(int read_log,
                         int deposits,
                         int *logging)
{
   _read = read_log;
   _dep = deposits;
   _log = logging;
}                               /* set_security_levels() */

/**
 * This sets the file to which old logs will be transferred after two weeks.  
 * You can then modify the council archives to read the file.
 * @param word The file to write to
 */
void set_archive(string word)
{
   _archive = word;
}                               /* set_archive */
