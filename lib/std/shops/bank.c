/*
 * $Id: bank.c,v 1.6 2003/05/08 15:58:21 carmine Exp $
 */
/**
 * This is the bank inheritable.  It handles all the bank transactions
 * and other such exciting stuff.
 * @author Pinkfish
 */
#include <money.h>
#include <move_failures.h>

#define NAME (string)this_player()->query_name()
#define CAP_NAME (string)this_player()->one_short()
#define POSS (string)this_player()->query_possessive()
#define HANDLER "/obj/handlers/bank_handler"
#define OLD_BANK "/d/am/save/bank"

nosave inherit "/std/room/basic_room";

int total_account, total_made;
nosave int account_cost, percentage;
nosave string bank_name, place;

int do_withdraw(int num, string type);
int do_deposit(string str);

nosave string save_file;

void create() {
  account_cost = 0;
  percentage = 90;
  total_account = 0;
  total_made = 0;
  bank_name = "Bing's First";
  place = "default";
  add_help_file("bank");
  ::create();
} /* create() */

void init() {
  add_command( "balance", "" );
  add_command( "withdraw", "<number> <string>",
               (: do_withdraw($4[0], $4[1]) :) );
  add_command( "open", "account" );
  add_command( "close", "account" );
  add_command( "deposit", "<string>", (: do_deposit($4[0]) :) );
  ::init();
} /* init() */

/**
 * This method returns the amount of money it costs to start an account
 * at this bank.
 * @return the account cost
 */
int query_account_cost() { return account_cost; }

/**
 * This method sets the amount of money that it costs to start an account
 * at this bank.
 * @param number the cost of starting an account
 */
void set_account_cost( int number ) { account_cost = number; }

/**
 * This method queries the percentage the bank charges on transactions.
 * @return the percentage the bank charges
 */
int query_percentage() { return percentage; }

/**
 * This method sets the percentage the bank charges on transactions.
 * @param number the percentage to charge
 */
void set_percentage( int number ) { percentage = number; }

/**
 * This method returns the total value of all the accounts.
 * @return the totaly value of all the accounts
 */
int query_total_account() { return total_account; }

/**
 * This method returns the amount of money the bank has made off the
 * players.
 * @param total_made the amount of money mae
 */
int query_total_made() { return total_made; }

string query_bank_name() { return bank_name; }

void set_bank_name( string word ) { bank_name = word; }

string query_place() { return place; }
string query_money_place() { return query_place(); }

void set_place( string word ) { place = word; }

string query_save_file() { return save_file; }

void set_save_file( string word ) {
  save_file = word;
  if ( file_size( save_file +".o" ) > 0 )
    unguarded((: restore_object, save_file :));
} /* set_save_file() */

void do_save() {
   if ( !save_file ) {
      return;
   }
   unguarded( (: save_object, save_file :) );
} /* do_save() */

int get_account() {
  int amount;

  if ( amount = (int)this_player()->query_property( "bank "+ OLD_BANK ) ) {
    HANDLER->adjust_account( NAME, bank_name, amount );
    this_player()->remove_property( "bank "+ OLD_BANK );
    total_account += amount;
    do_save();
  }
  return (int)HANDLER->query_account( NAME, bank_name );
} /* get_account() */

int set_account( int amount ) {
  HANDLER->set_account( NAME, bank_name, amount );
} /* set_account() */

int do_balance() {
  int amount;

  amount = get_account();
  if ( amount < 0 ) {
    notify_fail( "You do not have an account here.\n" );
    return 0;
  }
  if ( !amount ) {
     this_player()->add_succeeded_mess(this_object(),
       ({ "Your account is empty.\n",
           "$N checks the balance of $p account.\n" }), ({ }) );
    return 1;
  }
  this_player()->add_succeeded_mess(this_object(),
     ({ "You have "+ (string)MONEY_HAND->money_value_string( amount, place ) +
        " in your account.\n",
        "$N checks the balance of $p account.\n" }), ({ }) );
  return 1;
} /* do_balance() */

int do_withdraw( int number, string type ) {
  int amount;
  int best;
  int total;
  object money;

  total = get_account();

  if ( total < 0 ) {
    notify_fail( "You do not have an account here.\n" );
    return 0;
  }

  if ( number <= 0 ) {
    notify_fail( "You must withdraw something.\n" );
    return 0;
  }

  money = clone_object( MONEY_OBJECT );
  money->set_money_array( (mixed *)MONEY_HAND->query_values_in( place ) );
  if ( ( best = (int)money->find_best_fit( type ) ) == -1 ) {
    notify_fail( "That currency is not legal tender here.  Sorry.\n" );
    money->dest_me();
    return 0;
  }
  type = ( (mixed *)money->query_money_array() )[ best ];
  amount = number * ( (mixed *)money->query_money_array() )[ best + 1 ];
  if ( ( amount > total ) ||
       ( amount < 0 ) ) {
    notify_fail( "You don't have enough money in your account.  Sorry.\n" );
    money->dest_me();
    return 0;
  }
  money->set_money_array( ({ type, number }) );
  set_account( total - amount );
  total_account -= amount;
  do_save();
  this_player()->add_succeeded_mess(this_object(),
   ({ "You withdraw "+ (string)money->short() +".\n" + 
      "You have "+ (string)MONEY_HAND->money_value_string( get_account(), 
                                                           place ) +
        " in your account after the withdrawal.\n",
      "$N withdraws some money from $p account.\n"
       }), ({ }) );
  if ( (int)money->move( this_player() ) != MOVE_OK ) {
    write( "You are too heavily burdened to collect the money, so the teller "+
      "puts it on the counter for you.\n" );
    money->move( this_object() );
  }
  this_player()->save();
  return 1;
} /* do_withdraw() */

int do_deposit( string words ) {
   int amount;
   int total;
   int tmp;
   mixed money;
   object *tender;
   object legal;
   object illegal;
   string howmuch;

   total = get_account();
   if ( total < 0 ) {
      notify_fail("You don't have an account here.\n");
      return 0;
   }

   money = (mixed)MONEY_HAND->parse_money( words, this_player(), place );
   if ( intp( money ) ) {
      switch( money ) {
         case NO_MATCH :
            return notify_fail( "You do not have \""+ words +"\".\n" );
         case NO_MONEY :
            return notify_fail( "You can only deposit money.\n" );
         default :
            return notify_fail( "You can only deposit legal tender.\n" );
      }
   }

   // Figure out the legal tender
   if (!place) {
      place = "default";
   }
   tender = MONEY_HAND->filter_legal_tender( money, place);
   legal = tender[0];
   illegal = tender[1];
   if (legal) {
     amount = legal->query_value_in(place);
     if (place != "default") {
       amount += legal->query_value_in("default");
     }

     // Minimum amount added to prevent depositing 1c 1000 times to avoid
     // charges.
     if(amount < 200) {
       if( legal->move( this_player() ) != MOVE_OK ) {
          legal->move( environment( this_player() ) );
          tell_object( this_player(), "You lose hold of some coins and "
             "they tumble to the floor.\n" );
          tell_room( environment( this_player() ), this_player()->one_short() + 
             "accidentally drops some coins.\n", this_player() );
       }
       if(illegal)
         if( illegal->move( this_player() ) != MOVE_OK ) {
            illegal->move( environment( this_player() ) );
            tell_object( this_player(), "You lose hold of some coins and "
               "they tumble to the floor.\n" );
            tell_room( environment( this_player() ), this_player()->one_short() + 
             "accidentally drops some coins.\n", this_player() );
         }
       add_failed_mess("That is too small an amount to deposit.\n");
       return 0;
     }
     
     tmp = ( amount * ( 100 - percentage ) ) / 100;
     total_made += tmp;
     amount = amount - tmp;
     set_account( total + amount );
     total_account += amount;
     do_save();
   }

   // Give them back their illegal money and zap the other stuff.
   if (illegal) {
      if( illegal->move( this_player() ) != MOVE_OK ) {
         illegal->move( environment( this_player() ) );
         tell_object( this_player(), "You lose hold of some coins and "
            "they tumble to the floor.\n" );
         tell_room( environment( this_player() ), this_player()->one_short() + 
            "accidentally drops some coins.\n", this_player() );
       }
   }

   if (legal) {
      howmuch = legal->the_short();
      legal->move( "/room/rubbish" );
      this_player()->save();
   } else {
      add_failed_mess("Unable to find any legal money to deposit.\n");
      return 0;
   }

   write( "You give the teller "+ howmuch +".\n" );
   write( "After expenses, this leaves "+ (string)MONEY_HAND->
          money_value_string( amount, place ) +".\n" );

   this_player()->add_succeeded_mess(this_object(),
       ({  "$N $V "+words+".\n"
           "This gives a total of " +
           MONEY_HAND->money_value_string(total + amount, place ) +
           " in your account.\n",
           "$N deposit$s some money into $p account.\n" }), ({ }) );

   return 1;
} /* do_deposit() */

int do_open() {
  int amount;
  object money;

  if ( get_account() >= 0 ) {
    notify_fail( "You already have an account here.\n" );
    return 0;
  }
  if ( account_cost ) {
    if ( !( money = present( MONEY_ALIAS, this_player() ) ) ) {
      notify_fail( "You do not have any money.\n" );
      return 0;
    }
    amount = (int)money->query_value_in( place );
    if ( place != "default" )
      amount += (int)money->query_value_in( "default" );
    if ( amount < account_cost ) {
      notify_fail( "You do not have enough money to open an account.\n" );
      return 0;
    }
    write( "It will cost you "+ (string)MONEY_HAND->money_value_string(
      account_cost, place ) +" to open the account.\nDo you still wish to "+
      "open an account here? " );
    input_to( "check_open" );
    return 1;
  }
  set_account( 0 );
  this_player()->add_succeeded_mess(this_object(),
     "$N $V an account.\n", ({ }) );
  return 1;
} /* do_open() */

int check_open( string word ) {
  object money;

  word = lower_case( word );
  if (word [ 0 ] == /* "n" */ 110 ) {
    write( "Okay, not opening an account.\n" );
    return 1;
  }
  if ( word[ 0 ] != /* "y" */ 121 ) {
    write( "Do you want to open an account? Answer \"yes\" or \"no\".\n" );
    input_to( "check_open" );
    return 1;
  }
  set_account( 0 );
  write( "You now have an account here.\n" );
  money = present( MONEY_ALIAS, this_player() );
  MONEY_HAND->pay_amount_from( account_cost, money, place );
  total_made += account_cost;
  do_save();
  this_player()->save();
  return 1;
} /* check_open() */

int do_close() {
  int total;
  object money;

  total = get_account();
  if ( total < 0 ) {
    notify_fail( "You do not have an account here.\n" );
    return 0;
  }

  if ( total ) {
    if ( money = (object)MONEY_HAND->make_new_amount( total, place ) ) {
      write( "You receive "+ (string)money->short() +" on closing "+
        "your account.\n" );
      if ( (int)money->move( this_player() ) != MOVE_OK ) {
        write( "You are too heavily burdened to accept all that money, so "+
          "the teller puts it on the counter for you.\n" );
        money->move( this_object() );
      }
    } else {
      write( "The amount of money in your account was not even worth a "+
        "coin of the lowest denomination in the local currency.  Sorry.\n"+
        "Your account is now closed.\n" );
      total_made += total;
    }
    total_account -= total;
  } else
    write( "You close your account.\n" );
  this_player()->add_succeeded_mess(this_object(),
     "$N $V an account.\n", ({ }) );
  set_account( -1 );
  do_save();
  this_player()->save();
  return 1;
} /* do_close() */

mixed *stats() {
  return ::stats() + ({
    ({ "account cost", account_cost }),
    ({ "percentage", percentage }),
    ({ "total accounts", total_account }),
    ({ "total made", total_made }),
    ({ "bank name", bank_name }),
    ({ "place", place }),
    ({ "save file", save_file }) });
} /* stats() */

int make_bank_robbers() {
  int i, amount, loss, number, percent_loss, total;
  string player_name;
  object *bags, *money, *players, *robbers;
  mixed *mon_array;
  percent_loss = 2 + random( 4 );
  players = users();
  for ( i = 0; i < sizeof( players ); i++ ) {
    player_name = (string)players[ i ]->query_name();
    if ( ( amount = (int)HANDLER->query_account( player_name, bank_name ) ) ) {
      switch( amount ) {
        case 1 .. 10:
          loss = 0;
          break;
        case 11 .. 100:
          loss = ( amount * percent_loss ) / 100;
          break;
        case 101 .. 1000:
          loss = ( amount * percent_loss * 2 ) / 100;
          break;
        case 1001 .. 10000:
          loss = ( amount * percent_loss * 3 ) / 100;
          break;
        case 10001 .. 100000:
          loss = ( amount * percent_loss * 4 ) / 100;
          break;
        default:
          loss = ( amount * percent_loss * 5 ) / 100;
      }
      HANDLER->adjust_account( player_name, bank_name, -loss );
      total += loss;
    }
  }
  if ( !total ) return 0;
  if ( !( number = total / 10000 ) ) number = 1;
  if ( number > 5 ) number = 5;
  total_account -= total;
  amount = total / 3;
  total_made += 2 * amount;
  do_save();
  mon_array = (mixed *)MONEY_HAND->create_money_array( amount / number, place );
  bags = allocate( number );
  money = allocate( number );
  robbers = allocate( number );
  for ( i = 0; i < number; i++ ) {
    money[ i ] = clone_object( MONEY_OBJECT );
    money[ i ]->set_money_array( mon_array );
    bags[ i ] = clone_object( "/obj/container" );
    bags[ i ]->set_name( "bag" );
    bags[ i ]->set_short( "money bag" );
    bags[ i ]->add_adjective( "money" );
    bags[ i ]->add_property( "determinate", "a " );
    bags[ i ]->set_main_plural( "money bags" );
    bags[ i ]->set_long( "This is a roughly sewn sacking bag with a big "+
      "\"$\" sign on it.\n" );
    bags[ i ]->set_weight( 10 );
    bags[ i ]->set_max_weight( 3 * (int)money[ i ]->query_weight() + 10 );
    robbers[ i ] = clone_object( "/obj/monster" );
    robbers[ i ]->set_name( "robber" );
    robbers[ i ]->set_short( "bank robber" );
    robbers[ i ]->add_adjective( "bank" );
    robbers[ i ]->add_property( "determinate", "a " );
    robbers[ i ]->set_main_plural( "bank robbers" );
    robbers[ i ]->set_long( "This is a bank robber.  You can tell by the "+
      "stripey jersey and the mask over his eyes.\n" );
    robbers[ i ]->set_race( "human" );
    robbers[ i ]->set_guild( "thief" );
    robbers[ i ]->set_gender( 1 + random( 1 ) );
    robbers[ i ]->set_level( 20 + random( 30 ) + random( 40 ) );
    robbers[ i ]->reset_all();
    money[ i ]->move( bags[ i ] );
    bags[ i ]->move( robbers[ i ] );
    robbers[ i ]->move( this_object() );
  }
  return amount;
} /* make_bank_robbers() */
