/**
 * This is the bank inheritable.  It handles all the bank transactions
 * and other such exciting stuff.
 * @author Pinkfish
 */
#include <money.h>
#include <move_failures.h>

nosave inherit "/std/room/basic_room";
nosave inherit "/std/shops/inherit/bank_master";

private int _total_account;
private int _total_made;
private nosave string _place;

nosave string _save_file;

void create() {
  _total_account = 0;
  _total_made = 0;
  add_help_file("bank");
  ::create();
} /* create() */

/** @ignore yes */
int query_float() {
   return 2400000;
} /* query_float() */

/** @ignore yes */
void adjust_float(int amt) {
} /* adjust_float() */

/** @ignore yes */
int is_open_for(string type, string person) {
   return 1;
} /* is_open_for() */

/**
 * This method retuns the money place for the bank.
 * @return the money place
 */
string query_money_place() { return _place; }

/**
 * This method sets the place that the money will be setup from.
 * @param word the place to set
 */
void set_place( string word ) { _place = word; }

/**
 * This method returns the save file used for the room.
 * @return the save file used
 */
string query_save_file() { return _save_file; }

/**
 * This method sets the save file for the bank.
 * @param word the save file name for the bank
 */
void set_save_file( string word ) {
  _save_file = word;
  if ( file_size( _save_file +".o" ) > 0 ) {
    unguarded((: restore_object, _save_file :));
  }
} /* set_save_file() */

/**
 * This method does the actual save.
 */
void event_save(object ob) {
   if ( !_save_file ) {
      return;
   }
   unguarded( (: save_object, _save_file :) );
} /* event_save() */

mixed *stats() {
  return basic_room::stats() + 
         bank_master::stats() + ({
         ({ "save file", query_save_file() }) });
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
    if ( ( amount = BANK_HANDLER->query_account( player_name, query_bank_name() ) ) ) {
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
      BANK_HANDLER->adjust_account( player_name, query_bank_name(), -loss );
      total += loss;
    }
  }
  if ( !total ) return 0;
  if ( !( number = total / 10000 ) ) number = 1;
  if ( number > 5 ) number = 5;
  //total_account -= total;
  amount = total / 3;
  //total_made += 2 * amount;
  event_save(this_object());
  mon_array = MONEY_HAND->create_money_array( amount / number, _place );
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

void init() {
   basic_room::init();
   bank_master::init();
} /* init() */
