/**
 * This is the handler for all things clubby, a club being a group of
 * players.  Each club must have a unique name.  It also handles elections
 * for various club positions.
 * @author Pinkfish
 * @started Sun Sep 27 03:35:42 EDT 1998
 */
inherit "/obj/handlers/inherit/club_family";

#define __CLUBS_H_NO_CLASSES

#include <clubs.h>
#include <mail.h>
#include <board.h>
#include <player_handler.h>

// Saved!
private mapping _elections;
private string *_boards;

// Special variables to control the club inner workings.
private nosave mapping _discussion_items;

/* Defines for all the initial name indexes */
#define CLUB_PRESIDENT_INDEX          "president"
#define CLUB_VICE_PRESIDENT_INDEX     "vice-president"
#define CLUB_TREASURER_INDEX          "treasurer"
#define CLUB_SECRETARY_INDEX          "secretary"
#define CLUB_ORDINARY_COMMITTEE_INDEX "committee"

/* The prefix for the club name when making boards. */
#define CLUB_BOARD_PREFIX "club "

/*
 * Defines for the things which are called into the discussion item
 * methods to attempt to verify things and stuff.  The verify is
 * called at the start to verify the information is correct.
 * The complete is called at the end to complete the discussion
 * idea.
 */
#define CLUB_DISCUSS_MESSAGE_VERIFY            1
#define CLUB_DISCUSS_MESSAGE_COMPLETE          2
#define CLUB_DISCUSS_MESSAGE_SHORT_DESCRIPTION 3
#define CLUB_DISCUSS_MESSAGE_LONG_DESCRIPTION  4
#define CLUB_DISCUSS_MESSAGE_NOMINATION        5

// The maximum size of the archive array.
#define CLUB_MAX_ARCHIVE_SIZE 50

int is_nomination_accepted(string club_name,
                           string position,
                           string person);
int complete_election(string club_name);
int start_election(string club_name);
int holds_position_of(string name, string position, string person);
string query_position_name(string club_name,
                           string position);
int set_position_holder(string club_name,
                                  string position,
                                  string person);
protected int add_archive(string club_name,
                          string subject,
                          string thingy);
string query_club_board_name(string club_name);

/**
 * This is for an elected club, elected clubs ellect their officals and
 * stuff.  The treasurer etc are actually the indexes to be used into the
 * positions mapping to determine the actual member which holds the
 * position.
 * @member treasurer the name of the treasurer of the club's position
 * @member president the name of the president of the club's position
 * @member vice_president the name of the vice president of the club's position
 * @member secretary the name of the secretary of the club's position
 * @member account_info information about the accounts
 * @member positions the special extra positions available in the club
 * @member last_election when the last election was held
 * @member discussion the current items up for discussion
 */
class election_info {
   string treasurer;
   string president;
   string vice_president;
   string secretary;
   // Keeps a transation history.
   //class club_transaction *transactions;
   mapping account_info;
   mapping positions;
   int last_election;
   int election_period;
   mapping options;
   class discussion_idea *discussion;
   class club_archive *archives;
   string announcement;
   int announce_time;
}

#define CLUB_NOMINATION        1
#define CLUB_NOMINATION_SECOND 3
#define CLUB_ELECTION          2

/**
 * This class which keeps track of a currently running election.
 * @member time_of_event the time the election started
 * @member state the current state of the election
 * @member people the current votes for the specified person.<br>
 * The format is: ([ position : ([ person : votes, ... ]) ... ])
 * @member voted_for which positions everyone has voted for.<br>
 * The format is: ([ person : ({ position, position, ... }) ])
 * @member nominated the people who have been nominated for each position
 * and who nominated them.<br>
 * This is in the format: ([ position: ([ person : ({ nominations }) ]) ])
 * A person must be seconded before they are actually added as
 * a potential for a position.
 * @member election_status the status of the election, this is used with
 * defines.
 */
class election_data {
   int time_of_event;
   int state;
   // This is in the format: ([ position : ([ person : no_votes ]) ])
   mapping people;
   // This is in the format: ([ position: ([ person : ({ nominations }) ]) ])
   // A person must be seconded before they are actually added as
   // a potential for a position.
   mapping nominated;
   // This is in the format : ([ person : ({ position, postion, ... }) ])
   mapping voted_for;
   // This is the format: ([ position : number_to_elect ])
   string election_status;
}

/**
 * This class keeps track of the accounts, it has the transaction
 * info for the account and other stuff.
 * @member transactions the transactions for the account
 * @member allowed_people the people that can use the account
 * @member allowed_positions the positions that can use the account
 */
class account_info {
   class club_transaction* transactions;
   string* allowed_people;
   string* allowed_positions;
}

// Predefination which need to go after the classes.
private class election_info query_elected_info(string name);

void create() {
   _elections = ([ ]);
   _boards = ({ });
   ::create();
   if (!_boards) {
      _boards = ({ });
   }
} /* create() */

/** @ignore yes */
protected void create_extra_data(string name) {
   class election_info e_info;
   class club_info data;
   class club_transaction bing;
   class account_info frog;

   if (query_club_type(name)== CLUB_ELECTED) {
      data = (class club_info)query_club_info(name);
      e_info = new(class election_info);
      e_info->treasurer = CLUB_TREASURER_INDEX;
      e_info->president = CLUB_PRESIDENT_INDEX;
      e_info->vice_president = CLUB_VICE_PRESIDENT_INDEX;
      e_info->secretary = CLUB_SECRETARY_INDEX;
      e_info->discussion = ({ });
      e_info->positions = ([
                             CLUB_TREASURER_INDEX : CLUB_UNKNOWN_MEMBER,
                             CLUB_PRESIDENT_INDEX : data->founder,
                             CLUB_VICE_PRESIDENT_INDEX : CLUB_UNKNOWN_MEMBER,
                             CLUB_SECRETARY_INDEX : CLUB_UNKNOWN_MEMBER,
                           ]);

      e_info->account_info = ([ ]);
      e_info->last_election = time();
      e_info->options = ([ ]);
      e_info->election_period = CLUB_INVALID_TIME;
      e_info->announcement = "";
      e_info->archives = ({ });
      e_info->announce_time = 0;
      data->extra_data = e_info;
      set_club_changed(name);
      bing = new(class club_transaction);
      bing->date = time();
      bing->amount = query_balance(name, CLUB_DEFAULT_ACCOUNT_NAME);
      bing->transaction_type = CLUB_TRANSACTION_START_BALANCE;
      bing->who = CLUB_CONTROLLER_MEMBER;
      frog = new(class account_info);
      frog->transactions = ({ bing });
      frog->allowed_people = ({ });
      frog->allowed_positions = ({ CLUB_TREASURER_INDEX,
                                   CLUB_PRESIDENT_INDEX });
      e_info->account_info[CLUB_DEFAULT_ACCOUNT_NAME] = frog;
      set_club_changed(name);
   } else {
      ::create_extra_data(name);
   }
} /* create_extra_data() */

/**
 * This method changes the type of the club.
 * @param name the name of the club to change
 * @param type the new type of the club
 * @return 1 on success, 0 on failure
 * @see create_club()
 * @see disband_club()
 * @see query_club_type()
 * @ignore yes
 */
int change_club_type(string name, 
                     int type) {
   if (::change_club_type(name, type)) {
      start_election(name);
      return 1;
   }
   return 0;
} /* change_club_type() */

/**
 * This method disbands the club.  The club will be totaly zapped and
 * everything about it efficently munched.
 * @param name the name of the club to disband
 * @return 1 on success, 0 on failure
 * @see create_club()
 * @see check_extra_infromation()
 * @ignore yes
 */
int disband_club(string name) {
   if (::disband_club(name)) {
      map_delete(_elections, name);
      _boards -= ({ query_club_board_name(name) });
      save_main();
      return 1;
   }
   return 0;
} /* disband_club() */

/**
 * This method returns the elected info associated with the club.
 * @param name the name of the club to get the info for
 * @return the elected club class, 0 if no club or not elected type
 */
protected class election_info query_elected_info(string name) {
   class club_info data;
   class account_info fluff;

   if (is_elected_club(name)) {
      data = (class club_info)query_club_info(name);
      if (pointerp(data->extra_data->account_info)) {
         fluff = new(class account_info);
         fluff->transactions = data->extra_data->account_info;
         fluff->allowed_people = ({ });
         fluff->allowed_positions = ({ data->extra_data->president,
                                       data->extra_data->treasurer });
         data->extra_data->account_info = ([ CLUB_DEFAULT_ACCOUNT_NAME : fluff ]);
      }
      return data->extra_data;
   }
   return 0;
} /* query_elected_info() */

/**
 * This method returns the information about the specified account.
 * @param name the name of the club
 * @param account the name of the account
 * @return the account info
 */
protected class account_info query_account_info(string name, string account) {
   class election_info bing;

   bing = query_elected_info(name);
   if (bing) {
      if (!account) {
         account = CLUB_DEFAULT_ACCOUNT_NAME;
      }
      return bing->account_info[account];
   }
   return 0;
} /* query_account_info() */

/**
 * @ignore yes
 */
int remove_member(string club, string member) {
   class election_info bing;
   class account_info fluff;
   string account;
   string person;
   string position;
   string name;
   mapping nominations;
   string *people;

   if (::remove_member(club, member)) {
      bing = query_elected_info(club);
      if (bing) {
         foreach (account, fluff in bing->account_info) {
            fluff->allowed_people -= ({ member });
         }
         foreach (position, person in bing->positions) {
            if (person == member) {
               set_position_holder(club, position, CLUB_UNKNOWN_MEMBER);
            }
         }
      }
      name = normalise_name(club);
      if (_elections[name]) {
         foreach (position, nominations in _elections[name]->nominated) {
            map_delete(nominations, member);
            foreach (person, people in nominations) {
               nominations[person] = people - ({ member });
            }
         }
         foreach (position, nominations in _elections[name]->people) {
            map_delete(nominations, member);
         }
      }
      return 1;
   }
   return 0;
} /* remove_member() */

/**
 * This method returns the time of the last election.
 * @param name the name of the club to get the time for
 * @return the time of the last election
 */
int query_last_election(string name) {
   class election_info info;

   info = query_elected_info(name);
   if (info) {
      return info->last_election;
   }
   return 0;
} /* query_last_election() */

/**
 * This method returns the time of the next election.
 * @param name the name of the club to get the time for
 * @return the time of the next election
 */
int query_next_election(string name) {
   int bing;

   bing = query_last_election(name);
   if (bing) {
      return bing + CLUB_ELECTION_PERIOD;
   }
   return 0;
} /* query_next_election() */

/**
 * This method returns the minimum time between elections.
 * @param name the name of the club to get the time for
 * @return the minimum time for the next election
 */
int query_minimum_next_election(string name) {
   int bing;

   bing = query_last_election(name);
   if (bing) {
      return bing + CLUB_MINIMUM_ELECTION_PERIOD;
   }
   return 0;
} /* query_minimum_next_election() */

/**
 * This method will determine if the specified person is the president of
 * the club.
 * @param name the name of the club to check the president of
 * @param president the person to check for being the president
 * @return 1 if they are in the position, 0 if not
 */
int is_president_of(string name, string president) {
   class election_info info;

   info = query_elected_info(name);
   if (info) {
      return holds_position_of(name, info->president, president);
   }
   return 0;
} /* is_president_of() */

/**
 * This method will determine if the specified person is the vice president of
 * the club.
 * @param name the name of the club to check the vice president of
 * @param vice_president the person to check for being the vice president
 * @return 1 if they are in the position, 0 if not
 */
int is_vice_president_of(string name, string vice_president) {
   class election_info info;

   info = query_elected_info(name);
   if (info) {
      return holds_position_of(name, info->vice_president, vice_president);
   }
   return 0;
} /* is_vice_president_of() */

/**
 * This method will determine if the specified person is the secretary of
 * the club.
 * @param name the name of the club to check the secretary of
 * @param secretary the person to check to see if they are the secretary
 * @return 1 if they are in the position, 0 if not
 */
int is_secretary_of(string name, string secretary) {
   class election_info info;

   info = query_elected_info(name);
   if (info) {
      return holds_position_of(name, info->secretary, secretary);
   }
   return 0;
} /* is_secretary_of() */

/**
 * This method will determine if the specified person is the treasurer of
 * the club.
 * @param name the name of the club to check the treasurer of
 * @param treasurer the person to check to see if they are the treasurer
 * @return 1 if they are in the position, 0 if not
 */
int is_treasurer_of(string name, string treasurer) {
   class election_info info;

   info = query_elected_info(name);
   if (info) {
      return holds_position_of(name, info->treasurer, treasurer);
   }
   return 0;
} /* is_treasurer_of() */

/**
 * This method will determine if the specified person holds any 
 * position in the club.
 * @param name the name of the club to check for the positions
 * @param person the name of the person to check in the positions
 * @return 1 if they are in the position, 0 if not
 */
int holds_any_position_of(string name, string person) {
   class election_info info;
   string pos_name;
   string per_name;

   info = query_elected_info(name);
   if (info) {
      foreach (pos_name, per_name in info->positions) {
         if (per_name == person) {
            return 1;
         }
      }
   }
   return 0;
} /* holds_any_position_of() */

/**
 * This method will determine if the specified person holds a
 * position in the club.
 * @param name the name of the club to check for the position
 * @param position the name of the position to check
 * @param person the name of the person to check in the position
 * @return 1 if they are in the position, 0 if not
 * @see holds_any_position_of()
 * @see is_secretary_of()
 * @see is_president_of()
 * @see is_vice_president_of()
 * @see is_treasurer_of()
 */
int holds_position_of(string name, string position, string person) {
   class election_info info;

   info = query_elected_info(name);
   if (info) {
      position = query_position_name(name, position);
      return info->positions[position] == person;
   }
} /* holds_position_of() */

/**
 * @ignore yes
 */
int create_account(string name, string account) {
   if (!account) {
      account = CLUB_DEFAULT_ACCOUNT_NAME;
   }
   if (::create_account(name, account)) {
      class election_info info;
      class account_info frog;

      info = query_elected_info(name);
      if (info) {
         frog = new(class account_info);
         frog->transactions = ({ });
         frog->allowed_people = ({ });
         frog->allowed_positions = ({ info->president, info->treasurer });
         info->account_info[account] = frog;
         set_club_changed(name);
      }
      return 1;
   }
   return 0;
} /* create_account() */

/**
 * This method will pay a certain amount of money to club.  This will be
 * how long the club is payed until.  The club will cost a certain
 * amount for each member as well as a base cost.
 * @param name the name of the club
 * @param amount the amount to add
 * @param type the type of transaction
 * @param person the person doing the transaction
 * @param account the account being changed
 * @param information the information about the deposit
 * @return the amount of money not able to be placed in the account
 * @see remove_money()
 * @see query_balance()
 * @see query_transactions()
 */
int add_money(string name,
              int amount,
              int type,
              string person,
              string account,
              string information) {
   class account_info frog;
   class club_transaction bing;

   if (::add_money(name, amount, type, person, account)) {
      frog = query_account_info(name, account);
      if (frog) {
         bing = new(class club_transaction);
         bing->date = time();
         bing->amount = amount;
         bing->transaction_type = type;
         bing->who = person;
         bing->information = information;
         frog->transactions += ({ bing });
         set_club_changed(name);
      }
      return 1;
   }
   return 0;
} /* add_money() */

/**
 * This method removes money from the account.
 * @param name the name of the club
 * @param amount the amount to change the balance by
 * @param type the type of transaction
 * @param person the person doing the transaction
 * @param account the account being changed
 * @param information the information about the deposit
 * @return 1 if the removal is a success
 * @see pay_money()
 * @see query_balance()
 * @see query_transactions()
 */
int remove_money(string name,
                 int amount,
                 int type,
                 string person,
                 string account,
                 string information) {
   class account_info frog;
   class club_transaction bing;

   if (::remove_money(name, amount, type, person, account)) {
      frog = query_account_info(name, account);
      if (frog) {
         bing = new(class club_transaction);
         bing->date = time();
         // Make sure the amounts are correct even if they do
         // -ve values.
         bing->amount = -amount;
         bing->transaction_type = type;
         bing->who = person;
         bing->information = information;
         frog->transactions += ({ bing });
         set_club_changed(name);
      }
      return 1;
   }
   return 0;
} /* remove_money() */

/**
 * This method returns the transaction history for the club.  This will only
 * be kept track of for elected clubs.
 * @param name the name of the club
 * @param account the name of the account
 * @return the array of transactions
 * @see add_money()
 * @see remove_money()
 */
class club_transaction* query_transactions(string name, string account) {
   class account_info frog;
   class club_transaction bing;
   class club_transaction old;
   int i;

   if (is_elected_club(name)) {
      frog = query_account_info(name, account);
      if (sizeof(frog->transactions) &&
          sizeof(frog->transactions[0]) == 4) {
         for (i = 0; i < sizeof(frog->transactions); i++) {
            old = frog->transactions[i];
            bing = new(class club_transaction);
            bing->who = old->who;
            bing->transaction_type = old->transaction_type;
            bing->amount = old->amount;
            bing->date = old->date;
            bing->information = 0;
            frog->transactions[i] = bing;
            set_club_changed(name);
         }
      }
      return frog->transactions;
   }
   return ({ });
} /* query_transactions() */

/**
 * This method returns the string name of the transaction type.
 * @param type the type to return the transaction type for
 * @return the nice transaction type
 * @see query_transactions()
 */
string query_transaction_type_name(int type) {
   switch (type) {
      case CLUB_TRANSACTION_UPKEEP :
         return "upkeep";
      case CLUB_TRANSACTION_DEPOSIT :
         return "player deposit";
      case CLUB_TRANSACTION_INSIGNIA :
         return "insignia";
      case CLUB_TRANSACTION_START_BALANCE :
         return "start balance";
      case CLUB_TRANSACTION_WITHDRAWL :
         return "player withdrawl";
      case CLUB_TRANSACTION_TRANSFER :
         return "player transfer";
      default :
         return "unknown";
   }
} /* query_transaction_type_name() */

/**
 * This method returns the people which allowed to manipulate the
 * account.
 * @param club_name the name of the club
 * @param account the name of the account
 * @return the array of people which can manipulate the account
 * @see add_acount_allowed_person()
 * @see remove_account_allowed_person()
 */
string* query_account_allowed_people(string club_name, string account) {
   class account_info frog;

   frog = query_account_info(club_name, account);
   if (frog) {
      return frog->allowed_people;
   }
   return ({ });
} /* query_account_allowed_people() */

/**
 * This method checks to see if the specified person is allowed to
 * access the account.  It checks based on both position and name.
 * @param club_name the name of the club
 * @param account the name of the account
 * @param person the person's name to check
 * @return 1 if they can access the account, 0 if they cannot
 * @see query_account_allowed_people()
 * @see add_money()
 * @see remove_money()
 * @see query_account_allowed_positions()
 */
int is_allowed_to_modify_account(string club_name,
                               string account,
                               string person) {
   class account_info frog;
   string pos;

   frog = query_account_info(club_name, account);
   if (frog) {
      if (member_array(person, frog->allowed_people) != -1) {
         return 1;
      }
      foreach (pos in frog->allowed_positions) {
         if (holds_position_of(club_name, pos, person)) {
            return 1;
         }
      }
   }
   return 0;
} /* is_allowed_to_modify_account() */

/**
 * This method adds a person to the allowed people array.
 * @param club_name the name of the club
 * @param account the name of the account
 * @param person the person to add to the array
 * @see remove_acount_allowed_person()
 * @see query_account_allowed_person()
 */
int add_account_allowed_person(string club_name, string account, string person) {
   class account_info frog;

   frog = query_account_info(club_name, account);
   if (frog &&
       member_array(person, frog->allowed_people) == -1) {
      frog->allowed_people += ({ person });
      set_club_changed(club_name);
      return 1;
   }
   return 0;
} /* add_account_allowed_person() */

/**
 * This method removes a person from the allowed people array.
 * @param club_name the name of the club
 * @param account the name of the account
 * @param person the person to remove from the array
 * @see add_acount_allowed_person()
 * @see query_account_allowed_person()
 */
int remove_account_allowed_person(string club_name, string account, string person) {
   class account_info frog;

   frog = query_account_info(club_name, account);
   if (frog &&
       member_array(person, frog->allowed_people) != -1) {
      frog->allowed_people -= ({ person });
      set_club_changed(club_name);
      return 1;
   }
   return 0;
} /* remove_account_allowed_person() */

/**
 * This method returns the positions which are allowed to manipulate the
 * account.
 * @param club_name the name of the club
 * @param account the name of the account
 * @return the array of people which can manipulate the account
 */
string* query_account_allowed_positions(string club_name, string account) {
   class account_info frog;

   frog = query_account_info(club_name, account);
   if (frog) {
      return frog->allowed_positions;
   }
   return ({ });
} /* query_account_allowed_positions() */

/**
 * This method adds a position to the allowed positions array.
 * @param club_name the name of the club
 * @param account the name of the account
 * @param position the position to add to the array
 * @see remove_acount_allowed_position()
 * @see query_account_allowed_position()
 */
int add_account_allowed_position(string club_name, string account, string position) {
   class account_info frog;

   frog = query_account_info(club_name, account);
   if (frog &&
       member_array(position, frog->allowed_positions) == -1) {
      frog->allowed_positions += ({ position });
      set_club_changed(club_name);
      return 1;
   }
   return 0;
} /* add_account_allowed_position() */

/**
 * This method removes a position from the allowed positions array.
 * @param club_name the name of the club
 * @param account the name of the account
 * @param position the position to remove from the array
 * @see add_acount_allowed_position()
 * @see query_account_allowed_position()
 */
int remove_account_allowed_position(string club_name, string account, string position) {
   class account_info frog;

   frog = query_account_info(club_name, account);
   if (frog &&
       member_array(position, frog->allowed_positions) != -1) {
      frog->allowed_positions -= ({ position });
      set_club_changed(club_name);
      return 1;
   }
   return 0;
} /* remove_account_allowed_position() */

/**
 * This method sets an option in the club.  Options are use for things
 * like setting if the club has a board associated with it, or other
 * optional elements.
 * @param club_name the name of the club to set the option in
 * @param option the name of the option to set
 * @param value the new value of the option
 * @return 1 on success, 0 on failure
 * @see query_elected_option()
 * @see is_elected_option()
 */
int set_elected_option(string club_name,
                       string option,
                       mixed value) {
   class election_info data;

   data = query_elected_info(club_name);
   if (data) {
      data->options[option] = value;
      set_club_changed(club_name);
      return 1;
   }
   return 0;
} /* set_elected_option() */

/**
 * This method removes an option in the club.  Options are use for things
 * like setting if the club has a board associated with it, or other
 * optional elements.
 * @param club_name the name of the club to remove the option in
 * @param option the name of the option to remove
 * @return 1 on success, 0 on failure
 * @see query_elected_option()
 * @see is_elected_option()
 */
int remove_elected_option(string club_name,
                          string option) {
   class election_info data;

   data = query_elected_info(club_name);
   if (data) {
      map_delete(data->options, option);
      set_club_changed(club_name);
      return 1;
   }
   return 0;
} /* remove_elected_option() */

/**
 * This method queries the option in the club.
 * @param club_name the name of the club
 * @param option the name of the option to query
 * @return the value of the option
 * @see set_elected_option()
 * @see is_elected_option()
 */
mixed query_elected_option(string club_name,
                           string option) {
   class election_info data;

   data = query_elected_info(club_name);
   if (data) {
      return data->options[option];
   }
   return 0;
} /* query_elected_option() */

/**
 * This method checks to see if the specified option exists in this
 * club.
 * @param club_name the name of the club
 * @param option the name of the option to test
 * @return 1 if the option exists, 0 if not
 * @see set_elected_option()
 * @see query_elected_option()
 */
int is_elected_option(string club_name,
                      string option) {
   class election_info data;

   data = query_elected_info(club_name);
   if (data) {
      return !undefinedp(data->options[option]);
   }
   return 0;
} /* is_elected_option() */

/**
 * This method starts an election for the specified club.
 * @param club_name the name of the club to start an election for
 * @return 1 if the election is successful started
 * @see nominate_person()
 * @see finish_nominations()
 * @see vote_for_person()
 */
int start_election(string club_name) {
   string name;
   string pos;
   class election_info bing;
   string *stuff;

   name = normalise_name(club_name);
   if (is_elected_club(name)) {
      if (!_elections[name]) {
         bing = query_elected_info(name);
         _elections[name] = new(class election_data);
         _elections[name]->people = ([ ]);
         _elections[name]->nominated = ([ ]);
         _elections[name]->voted_for = ([ ]);
         _elections[name]->time_of_event = time() + CLUB_NOMINATION_TIME;
         _elections[name]->state = CLUB_NOMINATION;
         // Setup all the correct bits for the positions.
         foreach (pos, stuff in bing->positions) {
            _elections[name]->people[pos] = ([ ]);
            _elections[name]->nominated[pos] = ([ ]);
            _elections[name]->voted_for[pos] = ({ });
         }
         save_main();
         send_broadcast_message(name,
                         "An election has just started!  Nominate your "
                         "favourite person now at the club control centre.");
         return 1;
      }
   }
   return 0;
} /* start_election() */

/**
 * This method nominates someone for a certain position in the club.
 * @param club_name the name of the club to nominate someone in
 * @param position the position to nominate someone for
 * @param person the person to nominate
 * @param nominating the person doing the nominating
 * @return 1 if successfuly nominated
 * @see start_election()
 * @see finish_nomination()
 * @see vote_for_person()
 */
int nominate_person(string club_name, string position, string person,
                    string nominating) {
   string name;

   name = normalise_name(club_name);
  
   if (is_elected_club(name)) {
      if (_elections[name] &&
          (_elections[name]->state == CLUB_NOMINATION ||
           _elections[name]->state == CLUB_NOMINATION_SECOND)) {
         // Check to see if the position exists.
         position = query_position_name(club_name, position);
         if (_elections[name]->nominated[position]) {
            if (!_elections[name]->nominated[position][person]) {
                _elections[name]->nominated[position][person] = ({ });
            }
            if (member_array(nominating,
                        _elections[name]->nominated[position][person]) == -1) {
               _elections[name]->nominated[position][person] += ({ nominating });
               save_main();
               return 1;
            }
         }
      }
   }
   return 0;
} /* nominate_person() */

/**
 * This method finishes the nomination stuff.  The return will be 0 if the
 * election is not valid for this club.  If the election was valid it will
 * return an array of positions that do not have enough nominated people to
 * complete the election for.  If the return array is 0 in size then the
 * next phase of the election is setup.
 * @param club_name the name of the club to finish the nomination for
 * @return see description for return values
 * @see start_election()
 * @see nominate_person()
 * @see vote_for_person()
 */
mixed finish_nomination(string club_name) {
   string name;
   string position_name;
   mapping position_data;
   string *data;
   string person;
   string *bad_positions;
   class election_info frog;

   name = normalise_name(club_name);

   bad_positions = ({ });
   if (is_elected_club(name)) {
      frog = query_elected_info(name);
      if (_elections[name] &&
          (_elections[name]->state == CLUB_NOMINATION ||
           _elections[name]->state == CLUB_NOMINATION_SECOND)) {
         // Check to make sure we have enough nominations for every
         // position.
         foreach (position_name, 
                  position_data in _elections[name]->nominated) {
            _elections[name]->people[position_name] = ([ ]);
            foreach (person, data in position_data) {
               if (is_nomination_accepted(club_name, position_name, person)) {
                  _elections[name]->people[position_name][person] = 0;
               }
            }
            if (sizeof(_elections[name]->people[position_name]) == 0) {
               bad_positions += ({ position_name });
            }
         }
         if (!sizeof(bad_positions) ||
             _elections[name]->state == CLUB_NOMINATION_SECOND) {

            // Elect anyone in a position that only had one nomination.
            foreach (position_name, position_data in _elections[name]->people) {
               if (sizeof(position_data) == 1) {
                  // Just elect them.
                  set_position_holder(club_name,
                                      position_name,
                                      keys(position_data)[0]);
                  map_delete(_elections[name]->people, position_name);
               }
            }
            // Remove all the bad positions.
            foreach (position_name in bad_positions) {
               set_position_holder(club_name,
                                   position_name,
                                   CLUB_UNKNOWN_MEMBER);
               map_delete(_elections[name]->people, position_name);
            }

            bad_positions = ({ });

            // Set the club as elected and do it!
            _elections[name]->state = CLUB_ELECTION;
            if (!sizeof(_elections[name]->people)) {
               save_main();
               complete_election(name);
            } else {
               _elections[name]->election_status = 0;
               _elections[name]->time_of_event = time() +
                                                CLUB_ELECTION_TIME;
               set_club_changed(name);
               save_main();
               send_broadcast_message(name,
                            "The nomination period has finished.  Vote "
                            "for your favourite person for each position now "
                            "at the club control centre.");
            }
         } else {
            _elections[name]->election_status = "No one has accepted "
                           "nominations for " +
                           query_multiple_short(bad_positions);
            save_main();
         }
         return bad_positions;
      }
   }
   return 0;
} /* finish_nomination() */

/**
 * This method sets up a vote for a specific person for a position.
 * @param club_name the name of the club to make the vote in
 * @param position the position you are voting for
 * @param person the person to vote for
 * @param voter the person doing the voting
 * @return 1 on success, 0 on failure
 * @see start_election()
 * @see finish_nomination()
 * @see nominate_person()
 */
int vote_for_person(string club_name,
                    string position,
                    string person,
                    string voter) {
   string name;

   name = normalise_name(club_name);
   if (is_club(name)) {
      if (_elections[name] &&
          _elections[name]->state == CLUB_ELECTION) {
         position = query_position_name(club_name, position);
         // Make sure they have not voted for this position already.
         if (_elections[name]->voted_for[position] &&
             member_array(voter, 
                          _elections[name]->voted_for[position]) == -1) {
            // Whooo!  Lets make sure the person they are voting for is ok.
            if (!undefinedp(_elections[name]->people[position][person])) {
               _elections[name]->people[position][person]++;
               _elections[name]->voted_for[position] += ({ voter });
               save_main();
               return 1;
            }
         }
      }
   }
} /* vote_for_person() */

/**
 * This method completes the election.
 * @param club_name the name of the club the election is being completed for
 * @see start_election()
 * @see finish_nomination()
 * @see nominate_person()
 * @see vote_for_person()
 */
int complete_election(string club_name) {
   string name;
   string position;
   mapping people;
   string person_name;
   int count;
   int max_count;
   string *max_people;
   mapping elected_positions;
   int pos;
   class election_info frog;

   name = normalise_name(club_name);
   if (is_elected_club(name) &&
       _elections[name] &&
       _elections[name]->state = CLUB_ELECTION) {
      elected_positions = ([ ]);
      // Ok, we are electing happily.
      foreach (position, people in _elections[name]->people) {
         max_count = 0;
         max_people = ({ });
         if (sizeof(people) > 1) {
            foreach (person_name, count in people) {
               if (count > max_count) {
                  max_count = count;
                  max_people = ({ person_name });
               } else if (count == max_count) {
                  max_people += ({ person_name });
               }
            }
         } else {
            max_people = keys(people);
         }
         
         if (sizeof(max_people) == 0) {
           _elections[name]->election_status = "No one has voted for "
             "the position of " + position;
           // No one has voted!
           return 0;
         }
         // Ok...  Two people have the same count?  Choose a random one
         // of them.  Who will know? :)
         pos = random(sizeof(max_people));
         person_name = max_people[pos];
         elected_positions[position] = person_name;
      }

      frog = query_elected_info(name);
      // Ok, election finished.  Setup all the positions.
      foreach (position, person_name in elected_positions) {
         set_position_holder(club_name, position, person_name);
      }
      frog->last_election = time();
      set_club_changed(name);
      touch_club(name);
      map_delete(_elections, name);
      save_main();
      send_broadcast_message(name, "The election has finished!");
      return 1;
   }
   return 0;
} /* complete_election() */

/**
 * This method checks to see if there is currently an election in progress.
 * @param club_name the name of the club to check for an election
 * @return 1 if there is an election in progress, 0 if not
 * @see complete_election()
 * @see start_election()
 * @see is_nomination_in_progress()
 * @see is_voting_in_progress()
 */
int is_election_in_progress(string club) {
   string name;

   name = normalise_name(club);
   if (is_elected_club(name) &&
       _elections[name]) {
      return 1;
   }
} /* is_election_in_progress() */

/**
 * This method checks to see if the election is in the nomination phase.
 * @param club the name of the club to check
 * @see is_election_in_progress()
 * @see is_voting_in_progress()
 */
int is_nomination_in_progress(string club) {
   string name;

   name = normalise_name(club);
   if (is_elected_club(name) &&
       _elections[name] &&
       (_elections[name]->state == CLUB_NOMINATION ||
        _elections[name]->state == CLUB_NOMINATION_SECOND)) {
      return 1;
   }
} /* is_nomination_in_progress() */

/**
 * This method checks to see if the election is in the voting phase.
 * @param club the name of the club to check
 * @see is_election_in_progress()
 * @see is_nomination_in_progress()
 */
int is_voting_in_progress(string club) {
   string name;

   name = normalise_name(club);
   if (is_elected_club(name) &&
       _elections[name] &&
       _elections[name]->state == CLUB_ELECTION) {
      return 1;
   }
} /* is_voting_in_progress() */

/**
 * This method returns the list of available positions for the club.
 * @param club_name the name of the club
 * @return the list of available positions
 */
string *query_valid_positions(string club_name) {
   string name;
   class election_info frog;

   name = normalise_name(club_name);
   if (is_elected_club(name)) {
      frog = query_elected_info(club_name);
      return keys(frog->positions);
   }
   return ({ });
} /* query_valid_positions() */

/**
 * This method returns the committee of the club.  The committee is anyone
 * who holds a position.
 * @param club_name the name of the club
 * @return the array of people who hold positions
 */
string *query_committee(string club_name) {
   string name;
   class election_info frog;

   name = normalise_name(club_name);
   if (is_elected_club(name)) {
      frog = query_elected_info(club_name);
      return values(frog->positions);
   }
   return ({ });
} /* query_committee() */

/**
 * This method determines if this is a valid position for the club.
 * @param club_name the name of the club
 * @param position the position to check
 * @return 1 if the position is ok, 0 if not
 * @see query_valid_positions()
 * @see nominate_person()
 * @see vote_for_person()
 */
int is_valid_position(string club_name,
                      string position) {
   string *pos;

   pos = map(query_valid_positions(club_name), (: lower_case :));
   return (member_array(lower_case(position), pos) != -1);
} /* is_valid_position() */

/**
 * This method determines if the position is one of the 4 basic positions.
 * @param club_name the name of the club to check
 * @param position the position to check
 * @return 1 if it is, 0 if not
 * @see is_valid_position()
 */
int is_basic_position(string club_name,
                      string position) {
   class election_info frog;

   if (is_club(club_name) &&
       is_valid_position(club_name, position)) {
      frog = query_elected_info(club_name);
      position = query_position_name(club_name, position);
      return (position == frog->president) ||
             (position == frog->vice_president) ||
             (position == frog->secretary) ||
             (position == frog->treasurer);
   }
   return 0;
} /* is_basic_position() */

/**
 * This method figures out the actualy index of the position in the mappings
 * and stuff associated with the position.
 * @param club_name the name of the club
 * @param position the position to create the index for
 * @return the position index
 * @see query_valid_positions()
 * @see nominate_person()
 * @see vote_for_person()
 */
string query_position_name(string club_name,
                           string position) {
   string *pos;
   string *real_pos;
   int i;

   real_pos = query_valid_positions(club_name);
   pos = map(real_pos, (: lower_case :));
   i = member_array(lower_case(position), pos);
   if (i == -1) {
      return 0;
   }
   return real_pos[i];
} /* query_position_name() */

/**
 * This method returns the members of the particular position.
 * @param club_name the name of the club
 * @param position the name of the position
 * @return the array of members of the position
 * @see query_valid_position()
 * @see is_valid_position()
 */
string query_position_members(string club_name, string position) {
   class election_info frog;

   if (is_elected_club(club_name) &&
       is_valid_position(club_name, position)) {
      position = query_position_name(club_name, position);
      frog = query_elected_info(club_name);
      if (frog->positions[position]) {
         return frog->positions[position];
      }
   }
   return "not found";
} /* query_position_members() */

/**
 * This method creates a position in the club.
 * @param name the name of the club to create the position in
 * @param position the name of the position
 * @see query_position_members()
 * @see is_valid_position()
 * @see query_valid_positions()
 */
int create_position_in_club(string name,
                            string position) {
   class election_info frog;

   if (is_elected_club(name) &&
       !is_voting_in_progress(name) &&
       !is_valid_position(name, position)) {
      frog = query_elected_info(name);
      frog->positions[position] = CLUB_UNKNOWN_MEMBER;
      set_club_changed(name);
      if (is_nomination_in_progress(name)) {
         name = normalise_name(name);
         _elections[name]->nominated[position] = ([ ]);
         _elections[name]->voted_for[position] = ({ });
         _elections[name]->people[position] = ([ ]);
         save_main();
      }
      return 1;
   }
   return 0;
} /* create_position_in_club() */

/**
 * This method removes a position from a club.  The 4 basic positions cannot
 * be removed.
 * @param name the name of the club to remove the position from
 * @param position the position to remove
 * @return 1 on success, 0 on failure
 * @see create_position_in_club()
 */
int remove_position_in_club(string name,
                            string position) {
   class election_info frog;
   class account_info green;
   string account;

   if (is_elected_club(name) &&
       is_valid_position(name, position) &&
       !is_voting_in_progress(name) &&
       !is_basic_position(name, position)) {
      position = query_position_name(name, position);
      frog = query_elected_info(name);
      map_delete(frog->positions, position);
      set_club_changed(name);
      if (is_nomination_in_progress(name)) {
         name = normalise_name(name);
         map_delete(_elections[name]->nominated, position);
         map_delete(_elections[name]->voted_for, position);
         map_delete(_elections[name]->people, position);
         save_main();
      }
      foreach (account, green in frog->account_info) {
         green->allowed_positions -= ({ position });
      }
      send_observer_event("club_event_remove_position", 
                          name,
                          position);
      return 1;
   }
   return 0;
} /* remove_position_in_club() */

/**
 * This method changes the name of the position in the club.
 * @param name the name of the club to create the position in
 * @param position the name of the position to change
 * @param new_position the new name of the position
 * @see create_position_in_club()
 * @see is_valid_position()
 */
int change_position_name_in_club(string name,
                                 string position,
                                 string new_position) {
   class election_info frog;
   class account_info green;
   string account;
   int pos;

   if (is_elected_club(name) &&
       !is_valid_position(name, new_position) &&
       !is_voting_in_progress(name) &&
       is_valid_position(name, position)) {
      position = query_position_name(name, position);
      frog = query_elected_info(name);
      frog->positions[new_position] = frog->positions[position];
      map_delete(frog->positions, position);
      // Check to see if it is any of the special indexes.
      if (frog->president == position) {
         frog->president = new_position;
      } else if (frog->vice_president == position) {
         frog->vice_president = new_position;
      } else if (frog->secretary == position) {
         frog->secretary = new_position;
      } else if (frog->treasurer == position) {
         frog->treasurer = new_position;
      }
      set_club_changed(name);
      name = normalise_name(name);
      if (is_nomination_in_progress(name)) {
         _elections[name]->nominated[new_position] = 
                                         _elections[name]->nominated[position];
         map_delete(_elections[name]->nominated, position);
         _elections[name]->voted_for[new_position] =
                                         _elections[name]->voted_for[position];
         map_delete(_elections[name]->voted_for, position);
         _elections[name]->people[new_position] =
                                         _elections[name]->people[position];
         map_delete(_elections[name]->people, position);
         save_main();
      }
      foreach (account, green in frog->account_info) {
         pos = member_array(position, green->allowed_positions);
         if (pos != -1) {
            green->allowed_positions[pos] = new_position;
         }
      }
      send_observer_event("club_event_change_position_name", 
                          name,
                          position,
                          new_position);
      return 1;
   }
   return 0;
} /* change_position_name_in_club() */

/**
 * This method sets the specified person as the holder of the position.
 * @param club_name the name of the club to set the position in
 * @param position the name of the position
 * @param person the person who will hold it
 * @return 1 on success, 0 on failure
 */
int set_position_holder(string club_name,
                                  string position,
                                  string person) {
   class election_info info;

   info = query_elected_info(club_name);
   if (info) {
      if (is_valid_position(club_name, position)) {
         if (is_member_of(club_name, person) ||
             person == CLUB_UNKNOWN_MEMBER) {
            position = query_position_name(club_name, position);
            info->positions[position] = person;
            set_club_changed(club_name);
            send_observer_event("club_event_position_holder", 
                                club_name,
                                position,
                                person);
            return 1;
         }
      }
   }
   return 0;
} /* set_position_holder() */

/**
 * This method checks to see if the person has already voted in the election.
 * If the position is not being voted on then, then it will always assume
 * they have voted for that position.
 * @param club_name the name of the club
 * @param position the position they have voted in
 * @param person the person to check to see if they have voted
 * @return 1 if they have voted, 0 if not
 * @see is_valid_position()
 * @see vote_for_person()
 * @see has_nominated_person()
 */
int has_person_voted(string club_name,
                    string position,
                    string person) {
   string name;

   name = normalise_name(club_name);
   if (is_valid_position(club_name, position)) {
      if (_elections[name]) {
         position = query_position_name(club_name, position);
         if (_elections[name]->voted_for[position]) {
            return (member_array(person,
                              _elections[name]->voted_for[position]) != -1);
         } else {
            return 1;
         }
      }
   }
   return 0;
} /* has_person_voted() */

/**
 * This method checks to see if the person has already nominated
 * someone for this position.
 * @param club_name the name of the club
 * @param position the position they have voted in
 * @param person the person to check to see if they have voted
 * @see is_valid_position()
 * @see vote_for_person()
 * @see has_person_voted()
 */
int has_nominated_person(string club_name,
                         string position,
                         string nominated,
                         string by) {
   string name;

   name = normalise_name(club_name);
   if (is_valid_position(club_name, position)) {
      position = query_position_name(club_name, position);
      if (_elections[name] &&
          _elections[name]->nominated[position][nominated]) {
         return (member_array(by,
                      _elections[name]->nominated[position][nominated]) != -1);
      }
   }
} /* has_nominated_person() */

/**
 * This method returns the list of people that have been nominated for
 * the specified position in the specified club.
 * @param club_name the name of the club
 * @param position the position to get the names of
 * @see is_nominate_for()
 * @see nominate_person()
 */
string *query_nominated_for(string club_name,
                            string position) {
   string name;

   name = normalise_name(club_name);
   if (is_valid_position(club_name, position)) {
      if (_elections[name]) {
         position = query_position_name(club_name, position);
         if (is_voting_in_progress(club_name)) {
            if (_elections[name]->people[position]) {
               return keys(_elections[name]->people[position]);
            }
            return ({ });
         } else if (is_nomination_in_progress(club_name)) {
            return keys(_elections[name]->nominated[position]);
         }
      }
   }
   return ({ });
} /* query_nominated_for() */

/**
 * This method returns the people who nominated the specified person
 * to the position.
 * @param club_name the name of the club
 * @param position the position they were nominated for
 * @param nominated the person who was nominated
 * @return who nominated them
 * @see query_nominated_for()
 * @see is_nomination_accepted()
 */
string *query_nominators_for(string club_name,
                             string position,
                             string nominated) {
   string name;

   name = normalise_name(club_name);
   if (is_valid_position(club_name, position)) {
      if (_elections[name]) {
         position = query_position_name(club_name, position);
         if (is_nomination_in_progress(club_name) &&
            _elections[name]->nominated[position][nominated]) {
            return _elections[name]->nominated[position][nominated];
         }
      }
   }
   return ({ });
} /* query_nominators_for() */

/**
 * This method checks to see if the specified person has been nominated for
 * the position.
 * @param club_name the name of the club
 * @param position the position they have been nominated for
 * @param person the person who is nominated to that position
 * @return 1 if they have been nominated, 0 if not
 * @see has_person_voted()
 * @see nominate_person()
 * @see is_valid_position()
 * @see is_nomination_accepted()
 */
int is_nominated_for(string club_name,
                     string position,
                     string person) {
   if (is_valid_position(club_name, position)) {
      return member_array(person,
                          query_nominated_for(club_name, position)) != -1;
   }
} /* is_nominated_for() */

/**
 * This method checks to see if the nomination has been accepted.  Currently
 * a nomination is accepted if there is more than 2 people nominated and one
 * of them is the person who is nominated.
 * @param club_name the name of the club to check
 * @param position the position they are nominated for
 * @param person the person who has been nominated
 * @return 1 if they are nominated, 0 if not
 * @see is_nominated_for()
 * @see query_nominators_for()
 */
int is_nomination_accepted(string club_name,
                           string position,
                           string person) {
   string *nom;

   if (is_nominated_for(club_name, position, person)) {
      nom = query_nominators_for(club_name, position, person);
      if (sizeof(nom) > 1 &&
          member_array(person, nom) != -1) {
         return 1;
      }
   }
   return 0;
} /* is_nomination_accepted() */

/**
 * This method returns the time the next event will occur.  The next event
 * will be either the end of the nomination phase or the end of the
 * voting phase.
 * @param club_name the name of the club to get the event of
 * @return the time of the next event
 * @see complete_election()
 * @see start_election()
 * @see finish_nomination()
 */
int query_next_event_time(string club_name) {
   if (is_club(club_name)) {
      club_name = normalise_name(club_name);
      if (_elections[club_name]) {
         return _elections[club_name]->time_of_event;
      }
   }
} /* query_next_event_time() */

/**
 * This method queries the status associated with the election.  The
 * status will say things like when certain positions have not been
 * voted for and other such stuff.
 * @param club the club name to get the status of
 * @return the status string of the club
 * @see complete_election()
 * @see finish_nomination()
 */
string query_election_status(string club_name) {
   if (is_elected_club(club_name)) {
      if (is_election_in_progress(club_name)) {
         club_name = normalise_name(club_name);
         return _elections[club_name]->election_status;
      }
   }
} /* query_election_stutus() */

/**
 * This method returns the current announcement string.
 * @param club_name the name of the club
 * @return the announcement string
 */
string query_announcement(string club_name) {
   class election_info bing;

   bing = query_elected_info(club_name);
   if (bing) {
      return bing->announcement;
   }
   return "";
} /* query_announcement() */

/**
 * This method returns the time at which the current announcement
 * was added.
 * @param club_name the name of the club
 * @return the time the announcement was added
 */
int query_announcement_time(string club_name) {
   class election_info bing;

   bing = query_elected_info(club_name);
   if (bing) {
      return bing->announce_time;
   }
   return 0;
} /* query_announcement_time() */

/**
 * This sets the current announcement.
 * @param club_name the name of the club
 * @param announce the new announcement
 * @return 1 on success, 0 on failure
 */
int set_announcement(string club_name,
                     string announce) {

   class election_info bing;

   bing = query_elected_info(club_name);
   if (bing) {
      // Archive the old announcement.
      if (bing->announce_time &&
          strlen(bing->announcement)) {
         add_archive(club_name,
                     "Announcement",
                     "Announcement on " + ctime(time()) + ":\n\n" +
                     bing->announcement);
      }
      bing->announce_time = time();
      bing->announcement = announce;
      set_club_changed(club_name);
      send_broadcast_message(club_name, "A new announcement has been added.");
      return 1;
   } 
   return 0;
} /* set_announcement() */

private class club_archive* convert_archives(mixed *archives) {
   string archive;
   class club_archive bing;
   int i;

   for (i = 0; i < sizeof(archives); i++) {
      archive = archives[i];
      if (stringp(archive)) {
         bing = new(class club_archive);
         bing->archive_date = time();
         bing->subject = "unknown";
         bing->matter = archive;
         archives[i] = bing;
      }
   }
   return archives;
} /* convert_archives() */

/**
 * This method adds to the archives of things for the current club.
 * @param club_name the name of the club
 * @param thingy the thing to archive
 * @return 1 on success, 0 on failure
 */
protected int add_archive(string club_name,
                          string subject,
                          string thingy) {
   class election_info data;
   class club_archive fluff;

   data = query_elected_info(club_name);
   if (data) {
      fluff = new(class club_archive);
      fluff->subject = subject;
      fluff->matter = thingy;
      fluff->archive_date = time();
      data->archives += ({ fluff });
      if (sizeof(data->archives) > CLUB_MAX_ARCHIVE_SIZE) {
         data->archives = data->archives[<CLUB_MAX_ARCHIVE_SIZE..];
      }
      if (stringp(data->archives[0])) {
         data->archives = convert_archives(data->archives);
      }
      set_club_changed(club_name);
      return 1;
   }
   return 0;
} /* add_archive() */

/**
 * This method returns the archives for this club.
 * @param club_name the name of the club to get archives from
 * @return the archives for the club
 */
class club_archive *query_archives(string club_name) {
   class election_info data;

   data = query_elected_info(club_name);
   if (data) {
      if (sizeof(data->archives) &&
          stringp(data->archives[0])) {
         data->archives = convert_archives(data->archives);
         set_club_changed(club_name);
      }
      return data->archives;
   }
   return ({ });
} /* query_archives() */

/**
 * This method returns all the clubs which currently have boards.
 * This is their normalised names.
 * @return the list of boards
 */
string *query_boards() {
   return _boards;
} /* query_boards() */

/**
 * This method checks to see if the club has a board.
 * @param club_name the name of the club to check
 * @return 1 on success, 0 on failure
 * @see query_boards()
 */
int query_club_board(string club_name) {
   string name;

   name = query_club_board_name(club_name);
   return member_array(name, _boards) != -1;
} /* query_club_board() */

/**
 * This method returns the name of the board for the club.
 * @param club_name the name of the club to get the board for
 * @return the name of the club, 0 if no board
 */
string query_club_board_name(string club_name) {
   return CLUB_BOARD_PREFIX + normalise_name(club_name);
} /* query_club_board_name() */

/**
 * This method creates a board for the club.
 * @param club_name the name of the club to create the board for
 * @return 1 on success, 0 on failure
 * @see query_club_board()
 */
int create_club_board(string club_name) {
   string name;

   name = query_club_board_name(club_name);
   if(is_elected_club(club_name) || this_player()->query_director()) {
      // Got to be elected to have a board.
      if (BOARD_HAND->is_board(name)) {
         // It already exists.
         return 0;
      }
      BOARD_HAND->create_board(name,
                               B_PRIV_ACCESS_RESTRICTED_METHOD |
                               B_PRIV_NO_INFORM,
                               "frog");
      BOARD_HAND->set_method_access_call(name,
                                         "check_board_post",
                                         CLUB_HANDLER);
      BOARD_HAND->set_timeout(10);
      BOARD_HAND->set_maximum(60);
      BOARD_HAND->set_minimum(10);
      _boards += ({ name });
      save_main();
      return 1;
   }
   return 0;
} /* create_club_board() */

/**
 * This method removes the club's board.
 * @param club_name the name of the club to remove the board from
 * @return 1 on success, 0 on failure
 */
int remove_club_board(string club_name) {
   string name;

   if (query_club_board(club_name)) {
      name = query_club_board_name(club_name);
      BOARD_HAND->delete_board(name);
      _boards -= ({ name });
      save_main();
      return 1;
   }
} /* remove_club_board() */

/**
 * This method checks to see what access the person has to the board.
 * @param type the type of operation
 * @param board the name of the board
 * @param previous the previous object
 * @param name the name of the person reading the message
 * @param num the number of posts blasted when posting
 * @return 1 on success, 0 on failure
 */
int check_board_post(int type,
                     string board,
                     string previous,
                     string name,
                     int num) {
   string club;

   club = board[strlen(CLUB_BOARD_PREFIX)..];
   switch (type) {
   case B_ACCESS_READ :
      return is_member_of(club, name);
   case B_ACCESS_WRITE :
      return is_member_of(club, name);
   case B_ACCESS_DELETE :
      return holds_any_position_of(club, name);
   case B_ACCESS_INFORM :
      if (!num) {
         send_broadcast_message(club, 
                                name + " posts a message to the board.");
      } else {
         send_broadcast_message(club, 
                                name + " posts a message to the board and " +
                                num + " message" +
                                (num > 1?"s":"") + " explode in sympathy.");
      }
      return 1;
   }
} /* check_board_post() */

/**
 * This method will mail a messsage to the committee.  Useful for
 * informing members of problems and things.
 * @param club_name the name of the club to mail
 * @param subject the subject to use
 * @param body the body of the message
 */
protected void do_mail_committee(string club_name,
                            string subject,
                            string body) {
   string *committee;

   body += "\n\nYours,\nFindlewoodlwinks\n(Club Handler)\n";
   committee = query_committee(club_name) - ({ CLUB_UNKNOWN_MEMBER });
   if (!sizeof(committee)) {
      if (PLAYER_HANDLER->test_user(query_founder(club_name))) {
         committee = ({ query_founder(club_name) });
      } else {
         return ;
      }
   }
   // Ok, now send the mail out...
   MAILER->do_mail_message(implode(committee, ","),
                           "Patricians office",
                           subject,
                           "",
                           body);
} /* do_mail_committee() */

/**
 * This method checks the extra information for the club.  Please note
 * this does *not* save the information.  You need to do this yourself.
 * @param name the name of the club to check
 * @see check_clubs()
 */
protected void check_elected_information(string name,
                                         string member,
                                         int startup) {
   class election_info data;
   string position_name;
   string person;
   mapping gruff;
   string *nom;
   int cre_club;
   int votes;

   name = normalise_name(name);
   data = query_elected_info(name);
   if (data) {
      if (!mapp(data->options)) {
         data->options = ([ ]);
         set_club_changed(name);
      }

      foreach (position_name, person in data->positions) {
         if (person != CLUB_UNKNOWN_MEMBER &&
             (!is_member_of(name, person) ||
              !PLAYER_HANDLER->test_active(person) ||
              member == person)) {
            set_position_holder(name, position_name, CLUB_UNKNOWN_MEMBER);
         }
      }

      if (query_next_election(name) < time() ||
          _elections[name]) {
         if (!_elections[name]) {
            start_election(name);
         } else {
            switch (_elections[name]->state) {
               case CLUB_NOMINATION_SECOND :
               case CLUB_NOMINATION :
                  if (_elections[name]->time_of_event < time()) {
                     if (sizeof(finish_nomination(name))) {
                        _elections[name]->time_of_event = time() +
                                                          CLUB_ELECTION_TIME;
                        _elections[name]->state = CLUB_NOMINATION_SECOND;
                        save_main();
                     }
                  }
                  break;
               case CLUB_ELECTION :
                  if (_elections[name]->time_of_event < time()) {
                     complete_election(name);
                  }
                  break;
            }
         }
      }

      if (_elections[name] &&
          startup) {
         // Go through the votes and the nominations and remove people
         // that are not members any more or are now creators...
         cre_club = is_creator_club(name);
         foreach (position_name, gruff in _elections[name]->people) {
            foreach (person, votes in gruff) {
               if ((!cre_club &&
                    PLAYER_HANDLER->test_creator(person)) ||
                   !is_member_of(name, person) ||
                   !PLAYER_HANDLER->test_user(person)) {
                  map_delete(gruff, person);
               }
            }
         }

         foreach (position_name,
                  gruff in _elections[name]->nominated) {
            foreach (person, nom in gruff) {
               if ((!cre_club &&
                    PLAYER_HANDLER->test_creator(person)) ||
                   !is_member_of(name, person) ||
                   !PLAYER_HANDLER->test_user(person)) {
                  map_delete(gruff, person);
               }
            }
         }
      }
   }
} /* check_elected_information() */

/*
void fixup_ic() {
   class election_data frog;

   frog = new(class election_data);
   frog->state = CLUB_ELECTION;
   frog->time_of_event = time() + CLUB_ELECTION_TIME;
   frog->people = ([ "secretary" : ([ "stinine" : 0, "brighid" : 0 ]),
                     "treasurer" : ([ "magicman" : 0, "primus" : 0, "todd" : 0 ]) ]);
   frog->voted_for = ([ "secretary" : ({ }), "treasurer" : ({ }) ]);
   frog->nominated = ([ ]);
   _elections["ic" ] = frog;
   save_main();
}
 */
