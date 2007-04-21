/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: bank_handler.c,v 1.17 2003/05/07 23:08:10 ceres Exp $
 * 
*/
/**
 * Keep track of each banks data.
 * @change 12-11-97 Pinkfish
 * Removed the player_name variable.  Made a couple of the functions
 * private
 *
 * @change 21-11-97 Terano
 * Changed the get_data_file function so that if there is no data file 
 * for a player, it calls init_data
 */


#define RESTORE_PATH "/save/bank_accounts/"
//#define LOGGING
#define CHEAT_THRESHOLD 200000

class franchise {
   string office;
   int cost_per_month;
   int cost_per_new_account;
}

class bank_data {
   string master_office;
   class franchise* franchises;
   int cost_to_start;
   int default_cost_per_month;
   int default_cost_per_new_account;
}
 
private mapping accounts;
private nosave mapping _banks;

void do_cheat_check(string, string, int, string, int);

void create() {
   string str;

   _banks = ([ ]);
   str = unguarded((: read_file, RESTORE_PATH + "bank_data.o" :));
   if (str) {
      _banks = restore_variable(str);
   }
} /* create() */

private void save_banks() {
   unguarded((: write_file, RESTORE_PATH + "bank_data.o",
                save_variable(_banks), 1 :));
} /* save_banks() */

private void init_data( string word ) {
  accounts = ([ ]);
} /* init_data() */

/**
 * Start up the system by restoreing the players data.
 * @param word the name of the player being restored
 */
int get_data_file( string name ) {
  int success;

  success = unguarded((: restore_object, RESTORE_PATH+
           name[0..0]+"/"+name :));
  if(!success) {
    init_data(name);
    
    return success;
  }
  return 1;
} /* get_data_file() */

/**
 * Save the current data file to disk.
 * @param word the name of the player being saved
 */ 
private void save_data_file( string name ) {
  if(!unguarded((: save_object, RESTORE_PATH+name[0..0]+"/"+name :)))
    debug_printf("Problem saving " + RESTORE_PATH+name[0..0]+"/"+name);
} /* save_data_file() */

/**
 * Find out how much a player has at a given bank.
 * @param word the name of the player
 * @param bank_name the name of the bank
 * @return the current balance, -1 on failure
 */
int query_account( string word, string bank_name ) {
  get_data_file( word );
  if ( !accounts ) {
    return -1;
  }
  if ( member_array( bank_name, m_indices( accounts ) ) == -1 ) {
    return -1;
  }
  return accounts[ bank_name ];
} /* query_account() */

/**
 * Change the amount of money a player has at a given bank.
 * @param name the name of the player
 * @param bank_name the name of the bank
 * @param amount the amount to change the balance by
 */
void adjust_account( string name, string bank_name, int amount ) {
  int before;
  
  get_data_file( name );
  before = accounts[ bank_name ];
  accounts[bank_name] += amount;
  if ( accounts[ bank_name ] < 0 ) {
    accounts = m_delete( accounts, bank_name );
  }
  do_cheat_check(name, bank_name, amount, "by", before);
  save_data_file( name );
  return;
} /* adjust_account() */


/**
 * Sets the balance at a certain bank to the specificed amount.
 * @param name the name of the player
 * @param bank_name the name of the bank
 * @param amount the amount to set the bank to.
 * Query the accounts a player has.
 *
 * @param player the name of the player
 */
void set_account( string name, string bank_name, int amount ) {
  int before;
  get_data_file( name );
  before = accounts[ bank_name ];
  
  if ( amount < 0 ) {
    accounts = m_delete( accounts, bank_name );
  } else {
    accounts[ bank_name ] = amount;
  }
  do_cheat_check(name, bank_name, amount, "to", before);
  save_data_file( name );      
  return;
} /* set_account() */

/**
 * Query the accounts a player has.
 *
 * @param player the name of the player
 */
string *query_accounts(string player) {
  get_data_file(player);

  if(!accounts)
    return ({ });

  return keys(accounts);
}

/**
 * THis method creates a new bank.
 * @param name the name of the bank
 * @param master the location of the master bank
 */
void create_new_bank(string name, string master) {
   class bank_data data;

   if (_banks[name]) {
      return ;
   }
   data = new(class bank_data);
   data->master_office = master;
   data->franchises = ({ });
   data->cost_to_start = 2600000;
   data->default_cost_per_month = 360000;
   data->default_cost_per_new_account = 400;
   _banks[name] = data;
   save_banks();
} /* create_new_banks() */

/**
 * This method removes a bank from the current list ofbanks.
 * @param name the name of the bank to remove
 */
void remove_bank(string name) {
   map_delete(_banks, name);
   save_banks();
} /* remove_bank() */

/**
 * This method is to be used for testing.
 */
class bank_data query_bank_data(string name) {
   return copy(_banks[name]);
} /* query_bank_data() */

/**
 * This method adds a new franchise to the operation.
 * @param name the name of the bank set
 * @param franchise_office the new franchise to add
 */
void add_new_franchise(string name, string franchise_office) {
   class franchise franchise;

   if (!_banks[name]) {
      return ;
   }

   foreach (franchise in _banks[name]->franchises) {
      if (franchise->office == franchise_office) {
         return ;
      }
   }

   franchise = new(class franchise);
   franchise->office = franchise_office;
   franchise->cost_per_month = _banks[name]->default_cost_per_month;
   franchise->cost_per_new_account = _banks[name]->default_cost_per_new_account;
   _banks[name]->franchises += ({ franchise });
   save_banks();
   return ;
} /* add_new_franchies() */

/**
 * This method sets the new cost for opening a franchise for the bank.
 * @param bank the bank to change the cost for
 * @param amount the new amount to charge to open
 */
void set_bank_default_cost_to_open(string name, int amount) {
   if (!_banks[name]) {
      return ;
   }

   _banks[name]->cost_to_start = amount;
   save_banks();
   return ;
} /* set_bank_default_cost_to_open() */

/**
 * This method sets the new cost for opening a franchise for the bank.
 * @param bank the bank to change the cost for
 * @param amount the new amount to charge to open
 */
void set_bank_default_cost_per_new_account(string name, int amount) {
   if (!_banks[name]) {
      return ;
   }

   _banks[name]->default_cost_per_new_account = amount;
   save_banks();
   return ;
} /* set_bank_cost_per_new_account() */

/**
 * This method sets the new cost for opening a franchise for the bank.
 * @param bank the bank to change the cost for
 * @param franchise the franchise to change the amounts for
 * @param amount the new amount to charge to open
 */
void set_bank_franchise_cost_per_month(string name, string franchise_office,
                                     int amount) {
   class franchise franchise;

   if (!_banks[name]) {
      return ;
   }

   foreach (franchise in _banks[name]->franchises) {
      if (franchise->office == franchise_office) {
         franchise->cost_per_month = amount;
         save_banks();
         return ;
      }
   }

   return ;
} /* set_bank_franchise_cost_per_month() */

/**
 * This method sets the new cost for opening a franchise for the bank.
 * @param bank the bank to change the cost for
 * @param amount the new amount to charge to open
 */
void set_bank_franchise_cost_per_new_account(string name, int amount) {
   if (!_banks[name]) {
      return ;
   }

   _banks[name]->cost_per_new_account = amount;
   save_banks();
   return ;
} /* set_bank_franchise_cost_per_new_account() */

/**
 * This method sets the new cost for opening a franchise for the bank.
 * @param bank the bank to change the cost for
 * @param amount the new amount to charge to open
 */
void set_bank_default_cost_per_month(string name, int amount) {
   if (!_banks[name]) {
      return ;
   }

   _banks[name]->default_cost_per_month = amount;
   save_banks();
   return ;
} /* set_bank_default_cost_per_month() */

/**
 * This method returns the new cost for opening a franchise for the bank.
 * @param bank the bank to change the cost for
 * @param amount the new amount to charge to open
 */
int query_bank_default_cost_to_open(string name) {
   if (!_banks[name]) {
      return -1;
   }

   return _banks[name]->cost_to_start;
} /* query_bank_default_cost_to_open() */

/**
 * This method returns the new cost for opening a franchise for the bank.
 * @param bank the bank to change the cost for
 * @param amount the new amount to charge to open
 */
int query_bank_default_cost_per_new_account(string name) {
   if (!_banks[name]) {
      return -1;
   }

   return _banks[name]->default_cost_per_new_account;
} /* query_bank_cost_per_new_account() */

/**
 * This method returns the new cost for opening a franchise for the bank.
 * @param bank the bank to change the cost for
 * @param franchise the franchise to change the amounts for
 * @param amount the new amount to charge to open
 */
int query_bank_franchise_cost_per_month(string name, string franchise_office) {
   class franchise franchise;

   if (!_banks[name]) {
      return -1;
   }

   foreach (franchise in _banks[name]->franchises) {
      if (franchise->office == franchise_office) {
         return franchise->cost_per_month;
      }
   }

   return -1;
} /* query_bank_franchise_cost_per_month() */

/**
 * This method returns the new cost for opening a franchise for the bank.
 * @param bank the bank to change the cost for
 * @param amount the new amount to charge to open
 */
int query_bank_franchise_cost_per_new_account(string name,
                                              string franchise_office) {
   class franchise franchise;

   if (!_banks[name]) {
      return -1;
   }

   foreach (franchise in _banks[name]->franchises) {
      if (franchise->office == franchise_office) {
         return franchise->cost_per_new_account;
      }
   }
   return -1;
} /* query_bank_franchise_cost_per_new_account() */

/**
 * This method returns the new cost for opening a franchise for the bank.
 * @param bank the bank to change the cost for
 * @param amount the new amount to charge to open
 */
int query_bank_default_cost_per_month(string name) {
   if (!_banks[name]) {
      return -1;
   }

   return _banks[name]->default_cost_per_month;
} /* query_bank_default_cost_per_month() */

/**
 * This method returns the master office of the bank.
 * @param name the bank name to check in
 * @return the master office of the bank
 */
string query_bank_master_office(string name) {
   if (!_banks[name]) {
      return 0;
   }

   return _banks[name]->master_office;
} /* query_bank_master_officer() */

/**
 * This method set the master office of the bank.
 * @param name the bank name to check in
 * @param master the master office of the bank
 */
void set_bank_master_office(string name, string master) {
   if (!_banks[name]) {
      return 0;
   }

   _banks[name]->master_office = master;
   save_banks();
} /* set_bank_master_officer() */

/**
 * This method returns the current list of banks.
 * @return the current list of banks
 */
string* query_banks() {
   return keys(_banks);
} /* query_banks() */

/**
 * This method returns the franchises for the specified bank.
 * @param bank the bank to get the franchises of
 * @return the list of franchises
 */
string* query_franchises(string bank) {
   if (!_banks[bank]) {
      return ({ });
   }

   return map(_banks[bank]->franchises, (: $1->office :) );
} /* query_franchises() */

void do_cheat_check(string name, string bank_name, int amount, 
                    string str, int after) {
#ifdef LOGGING
  if((amount - after) > CHEAT_THRESHOLD) {
    log_file ("BANK_CHEAT", sprintf ("%s: %s deposited %d into %s.\n",
                                     ctime(time()), name, (amount-after),
                                     bank_name));
  }
#endif  
}
