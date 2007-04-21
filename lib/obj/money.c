#include <money.h>
#include <move_failures.h>
#include <obj_parser.h>

#define COIN_WEIGHT 10

#define DEF_VALUE 1
//#undef USE_VAULT
#define USE_VAULT DEF_VALUE
//#define DEBUGGER "presto"

inherit "/std/object";

private nosave int no_join;
private nosave int _already_joined;
private mixed *money_array;
private nosave string _long;

int query_value_in(string where);

void create() {
   ::create();
   set_name( "coin" );
   add_alias( MONEY_ALIAS );
   add_plural( "coins" );
   set_main_plural( "coins" );
   add_property( "determinate", "" );
   add_property( "no one", 1 );
   add_property( "money", 1 );
   money_array = ({ });
} /* create() */

/** @ignore yes
 * Money is a collective object.
 */
int query_collective() { return 1; }

mapping query_dynamic_auto_load() {
   if (!_already_joined) {
      return ([
         "::" : ::query_dynamic_auto_load(),
         "money array" : money_array,
      ]);
   }
   add_alias(MONEY_ALIAS);
} /* query_dynamic_auto_load() */

void init_dynamic_arg( mapping map, object ) {
   ::init_dynamic_arg( map[ "::" ] );
   money_array = map[ "money array" ];
   if (sizeof(money_array) < 2)  call_out("dest_me", 0);
   short_d = _long = 0;
   if ( find_call_out( "check_adjectives" ) == -1 ) {
      call_out( "check_adjectives", 0 );
   }
} /* init_dynamic_arg() */

int query_merger() { return 1; }

int query_no_join() { return no_join; }

int query_already_joined() { return _already_joined; }
void reset_already_joined() { _already_joined = 0; }
void set_already_joined() { _already_joined = 1; }

void set_no_join() {
#ifdef DEBUGGER
   if (find_player(DEBUGGER) == this_player())
      tell_creator(DEBUGGER, "Called set_no_join\n");
#endif

   no_join = 1;
   call_out( "merge_coins", 0 );
} /* set_no_join() */


void force_no_join()  { no_join = 1; }
void reset_no_join() { no_join = 0; }

/**
 * This method returns the number of coins in the object.
 * @return the number of coins
 */
int query_number_coins() {
   int i;
   int tot;

   if (_already_joined) {
      return 0;
   }
   for ( i = 1; i < sizeof( money_array ); i += 2 ) {
      tot += money_array[ i ];
   }
   return tot;
} /* query_number_coins() */

/**
 * This method fixes up the weight of the money object to be what it should
 * be.
 */
void fixup_money_weight() {
   set_weight( 1 + query_number_coins() / COIN_WEIGHT );
} /* fixup_money_weight() */
   

int group_object() { return ( query_number_coins() > 1 ); }

/**
 * This method goes through the coins and sets up all the adjectives
 * and plurals it needs to.
 */
void check_adjectives() {
   int i;
   string* bits;

   set_adjectives( ({ }) );
   set_aliases( ({ }) );
   set_plurals( ({ }) );
   if ( !sizeof( money_array )) {
     if(file_name(environment()) != MONEY_VAULT) {
       _already_joined = 0;
#ifdef DEBUGGER
      if (find_player(DEBUGGER) == this_player())
          tell_creator(DEBUGGER, "MOVING %O to rubbish room\n", this_object());
#endif
#ifdef USE_VAULT
       MONEY_VAULT->add_to_list(this_object());
#else
       move("/room/rubbish");
#endif
     }
     return;
   }
   add_alias( MONEY_ALIAS );
   add_plural( "coins" );
   add_plural("money");
   for ( i = 0; i < sizeof( money_array ); i += 2 ) {
      bits = explode(lower_case(money_array[ i ]), " ");
      if (sizeof(bits) > 1) {
         add_adjective(bits[0..<2] );
      } else {
         add_adjective(bits);
      }
      if (MONEY_HAND->query_origin_of(money_array[i]) == "default")
         add_adjective(({ "provincial", "default" }));
      add_alias(MONEY_HAND->query_aliases_for(money_array[i]));
      add_adjective(MONEY_HAND->query_adjectives_for(money_array[i]));
      add_plural( (string)MONEY_HAND->query_plural_for( money_array[ i ] ) );
   }
   fixup_money_weight();
} /* check_adjectives() */

mixed *query_money_array() {
   if (!_already_joined) {
      return money_array;
   }
   return ({ });
} /* query_money_array() */

varargs int adjust_money( mixed amount, string type ) {
   int i;
   int ret;

   if ( pointerp( amount ) ) {
      for ( i = 0; i < sizeof( amount ); i += 2 ) {
         adjust_money( amount[ i + 1 ], amount[ i ] );
      }
      fixup_money_weight();
      return 1;
   }
   if ( !stringp( type ) || !intp( amount ) || _already_joined) {
      return 0;
   }
   short_d = _long = 0;
   if ( ( i = member_array( type, money_array ) ) == -1 ) {
      money_array += ({ type, amount });
      ret = 1;
   } else {
      money_array[ i + 1 ] += amount;
      if ( money_array[ i + 1 ] <= 0 ) {
         money_array = delete( money_array, i, 2 );
         ret = 0;
      } else {
         ret = 1;
      }
   }
   if ( find_call_out( "check_adjectives" ) == -1 ) {
      call_out( "check_adjectives", 0 );
   }
   fixup_money_weight();
   if (ret) {
      return money_array[ i + 1 ];
   }
   return 0;
} /* adjust_money() */

void set_money_array( mixed *new_array ) {
   short_d = _long = 0;
   money_array = new_array;
   if ( find_call_out( "check_adjectives" ) == -1 ) {
      call_out( "check_adjectives", 0 );
   }
   fixup_money_weight();
} /* set_money_array() */


/** @ignore yes */
string *half_symbol_short()  {
   int i;
   int value;
   string *retval;
   string *zones = ({ });

   for (i = 0; i < sizeof(money_array); i += 2)
      zones |= ({ MONEY_HAND->query_origin_of(money_array[i]) });

   retval = allocate(sizeof(zones));
   for (i = 0; i < sizeof(zones); i++)  {
      value = query_value_in(zones[i]);
      retval[i] = MONEY_HAND->money_value_string(value, zones[i]);
   }
   return retval;
}


/** @ignore yes */
string *half_short( int full ) {
   int i;
   string *retval;

   retval = ({ });
   for ( i = 0; i < sizeof( money_array ); i += 2 ) {
      if ( money_array[ i + 1 ] < 1 ) {
         continue;
      }
      if ( !full && ( money_array[ i + 1 ] > 12 ) ) {
         retval += ({ "some "+ (string)MONEY_HAND->query_main_plural_for(
               money_array[ i ] ) });
         continue;
      }
      if ( money_array[ i + 1 ] == 1 ) {
         retval += ({ "one "+
               (string)MONEY_HAND->query_short_for( money_array[ i ] ) });
         continue;
      }
      retval += ({ money_array[ i + 1 ] +" "+
            (string)MONEY_HAND->query_main_plural_for( money_array[ i ] ) });
   }
   return retval;
} /* half_short() */

/** @ignore yes */
string short( int dark ) {
   string *retval;

   if ( short_d ) {
      return short_d;
   }
   retval = half_short( 0 );
   if ( !sizeof( retval ) ) {
      return 0;
   }
   if ( sizeof( retval ) == 1 ) {
      short_d = retval[ 0 ];
   } else {
      short_d = query_multiple_short( retval );
   }
   return short_d;
} /* short() */

/** @ignore yes */
string long( string word, int dark ) {
   int i;
   mixed *details;

   if (dark < -1 || dark > 1) {
      return "It is too dark to see anything about the coins.\n";
   }

   if ( _long ) {
      return _long;
   }
   if ( !sizeof( money_array ) ) {
      return "This is some money that isn't money.\n";
   }
   _long = "";
   for ( i = 0; i < sizeof( money_array ); i += 2 ) {
      details = (mixed *)MONEY_HAND->query_details_for( money_array[ i ] );
      if ( money_array[ i + 1 ] == 1 ) {
         _long += "The "+ (string)MONEY_HAND->query_short_for(
               money_array[ i ] ) +" shows ";
      } else {
         _long += "The "+ (string)MONEY_HAND->query_main_plural_for(
               money_array[ i ] ) +" show ";
      }
      _long += details[ 2 ] +" on the reverse, and "+ details[ 3 ] +
            " on the obverse.\n";
   }
   return _long;
} /* long() */

/** @ignore yes */
string query_long_details( string word, int dark, object looker) {
   if (dark < -1 || dark > 1) {
      return "It is too dark to see anything about the coins.\n";
   }
   return long(word, dark);
} /* query_long_details() */

/**
 * This method creates a new money object from the current object.  It uses
 * the current objects values to make the money and removes the avlues from
 * the coins.
 * @param number the number of coins to remove
 * @param type the type of coins to remove
 * @return the new money object
 */
object new_money_object(mixed number, string type) {
   int i;
   object money;
   object env;
   object per;

#ifdef DEBUGGER
   if (find_player(DEBUGGER) == this_player())
      tell_creator(DEBUGGER, "Creating new money object from %O\n", this_object());
#endif

   if (!sizeof(money_array) || _already_joined) {
      return 0;
   }

   if (pointerp(number))  {
#ifdef USE_VAULT
      money = MONEY_VAULT->get_money_ob();
#else
      money = clone_object(MONEY_OBJECT);
#endif
      money->set_no_join();
      money->adjust_money(number);
      for (i = 1; i < sizeof(number); i += 2)
         number[i] = -number[i];
      adjust_money(number);
   }
   else  {
      if (!type || type == "") {
         return 0;
      }
      if ((i = member_array(type, money_array)) == -1) {
         return 0;
      }
      if (number > money_array[i + 1]) {
         return 0;
      }
#ifdef USE_VAULT
      money = MONEY_VAULT->get_money_ob();
#else
      money = clone_object(MONEY_OBJECT);
#endif
      money->set_no_join();
      money->adjust_money(number, type);
      adjust_money(-number, type);
   }

   // Make sure we do this after the weights are correct.
   if (money->move(environment()) != MOVE_OK) {
      env = environment();
      do {
        env = environment(env);
      } while (env && money->move(env) != MOVE_OK);
      per = environment();
      while (per && !living(per)) {
         per = environment(per);
      }

      if (money && env)
        tell_object(per, "Oops, the money " + money->the_short() +
                    " has found life in your inventory too heavy and "
                    "has gone to " + env->the_short() + ".\n");
   }
   return money;
} /* new_money_object() */

/**
 * This method merges two coin object together.  Or attempts to anyway.
 * This will occur whenever a coin object moves.
 * @return the merged coin object
 */
object merge_coins() {
   object money;
   
   if (_already_joined)   return this_object();

   remove_alias( MONEY_ALIAS );
   if(environment()) {
     money = present( MONEY_ALIAS, environment() );
   }
   add_alias( MONEY_ALIAS );
   
   if ( objectp( money ) &&
        money != this_object())
   {
#ifdef USE_VAULT
      MONEY_VAULT->add_to_list(this_object());
#else
      move("/room/rubbish");
#endif
      money->adjust_money( money_array );
      money->fixup_money_weight();
      //
      // Zap the money array, this should stop money duplication.
      //
      _already_joined = 1;

#ifdef DEBUGGER
      if (find_player(DEBUGGER) == this_player())
         tell_creator(DEBUGGER, "Merging %O and %O\n", this_object(), money);
#endif
      return money;
   }

#ifdef DEBUGGER
   if (find_player(DEBUGGER) == this_player())
      tell_creator(DEBUGGER, "Merge coins returning %O\n", this_object());
#endif
   return this_object();
} /* merge_coins() */

/** @ignore yes */
varargs int move( mixed dest, string messin, string messout ) {
   int i;
   
   if (_already_joined)
     return MOVE_INVALID_DEST;

   // Prevent people putting money into containers.
   if(objectp(dest) && environment(dest) && !living(dest) &&
      !dest->query_corpse() && !dest->query_accept_money())
     return MOVE_INVALID_DEST;
   
   i = ::move( dest, messin, messout );
   if ( i != MOVE_OK ) {
      return i;
   }

   if((file_name(environment()) == MONEY_VAULT))
     return MOVE_OK;

   if ( ( file_name( environment() ) == "/room/rubbish" ) || no_join ) {
      reset_no_join();
      return MOVE_OK;
   }

   merge_coins();
   return MOVE_OK;
} /* move() */

public int find_best_fit( mixed word ) {
   int i;
   int best;
   int best_rating;
   int rating;
   string against;
   string *words;

   if ( !word || ( word == "" ) ) {
      return 0;
   }
   if (stringp(word)) {
      words = explode( lower_case( word ), " " ) - ({ "coin", "coins" });
   } else {
      words = word;
   }
   best = -1;
   for ( i = 0; i < sizeof( money_array ); i += 2 ) {
      rating = 0;
      against = "X "+ replace( money_array[ i ] +" "+
            (string)MONEY_HAND->query_plural_for( money_array[ i ] ),
            " coins", "" ) +" X";
      foreach( word in words ) {
         if ( sscanf( against, "%*s "+ word +" %*s" ) == 2 ) {
            rating++;
         }
      }
      if ( rating > best_rating ) {
         best_rating = rating;
         best = i;
      }
   }
   return best;
} /* find_best_fit() */

/** @ignore yes */
object query_parse_id( mixed *arr ) {
   int i;
   string *bits;
   object money;

#ifdef DEBUG
   debug_printf("%O", arr );
#endif
   if ( ( arr[ 0 ] < 0 ) || !sizeof( money_array ) ) {
      return 0;
   }
   if ( arr[ 0 ] == 0 ) {
      bits = explode( arr[ 1 ], " " );
      if ( sizeof( bits ) == 1 ) {
         if ( bits[ 0 ] == "coins" ) {
            return this_object();
         }
      }
      if ( member_array( bits[ sizeof( bits ) - 1 ], query_plurals() ) == -1 ) {
         return this_object();
      }
      i = find_best_fit( arr[ 1 ] );
      if ( i == -1 ) {
         return 0;
      }
      money = new_money_object( money_array[ i + 1 ], money_array[ i ] );
      return money;
   }
   i = find_best_fit( arr[ 1 ] );
   /* Not very satisfactory... */
   if ( i == -1 ) {
      i = 0;
   }
   if ( arr[ 0 ] > money_array[ i + 1 ] ) {
      return 0;
   }
   money = new_money_object( arr[ 0 ], money_array[ i ] );
   return money;
} /* query_parse_id() */

/** @ignore yes */
mixed* parse_match_object(string* input, object viewer,
                          class obj_match_context context) {
   int ret;
   int found;
   int num;
   int i;
   int j;
   int success;
   object money;
   mixed *matched;
   string where;
   string *bits;
   string name;

   if (!sizeof(money_array)) {
      return 0;
   }

   ret = ::is_matching_object(input, viewer, context);
   if (!ret) {
      return 0;
   }

   if (sizeof(input) == 1 &&
       !context->number_included &&
       !context->ordinal &&
       (input[<1] == "coins" || input[<1] == "money" ||
        input[<1] == "all" || input[<1] == "thing" ||
        input[<1] == "one")) {
      return ({ OBJ_PARSER_MATCH_PLURAL, ({ this_object() }) });
   }
   else if (sizeof(input) >= 2  &&
            member_array(input[0], query_adjectives()) > -1  &&
            (input[<1] == "coins"  ||  input[<1] == "money"))
   {
      success = 0;
      name = lower_case(input[0]);
      if (name == "provincial"  ||  name == "default")  {
         success = 1;
         where = "default";
      }
      else for (i = 0; i < sizeof(money_array); i += 2)  {
         bits = explode(lower_case(money_array[i]), " ");
         /*
          * Find area that matches adjective....
          * ie, adjective 'djelian' == area 'Djelibeybi'
          */
         if (member_array(name, bits) > -1)  {
            /* OK, where does this money type come from... */
            where = MONEY_HAND->query_origin_of(money_array[i]);
            if (!where)
               return 0;

            if (where != "default")
               success = 1;
            break;
         }
      }
      if (success)  {
         /* Separate the money from the desired area */
         matched = MONEY_HAND->filter_legal_money_to_array(money_array,
                                                           where);
         if (sizeof(matched[0]) == 0)
            return 0;

         /* Count the number of coins */
         for (j = 1; j < sizeof(matched[0]); j+= 2)
           num += matched[0][j];

         if (!update_parse_match_context(context, num, ret))
            return 0;

         /* Split off a new money object from this one */
         money = new_money_object(matched[0], "");
         if (!money)
            return 0;

         return ({ ret, ({ money }) });
      }
   }

   found = find_best_fit(input);
//printf("CHecking %O found %O\n", input, found);
   if (found == -1) {
      return 0;
   }

   //
   // Lets see how many we have...
   //
   num = money_array[found + 1];
//if (this_player()->query_name() == "pinkfish") {
//printf("%O %O\n", num, ret);
//}
   num = update_parse_match_context(context, num, ret);
   if (!num) {
      return 0;
   }

   money = new_money_object( num, money_array[found]);
   if (!money) {
      return 0;
   }
   return ({ ret, ({ money }) });
} /* parse_match_object() */

/** @ignore yes */
int do_not_sell() { return 1; }

/** @ignore yes */
int query_value() { return 0; }

/** @ignore yes */
int query_value_in( string where ) {
   return (int)MONEY_HAND->query_total_value( money_array, where );
} /* query_value_in() */

/**
 * This method returns the amount of type of money there is in the array.
 * @param type the type of money to check
 * @return the number of coins of that type
 */
int query_money( string type ){
   int i;

   i = member_array( type, money_array );
   if (i == -1 ) {
      return 0;
   }
   return money_array[i + 1];
} /* query_money () */

mixed *stats() {
  return ::stats() +
    ({ ({ "value" , query_multiple_short(half_short(1)) }) });
}
