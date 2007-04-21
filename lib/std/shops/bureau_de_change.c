/**
 * This is the standard money changer inheritable.
 * @author Pinkfish
 * @change Deutha
 * Extensive changes to make it work with the new currency system.
 */
#include <money.h>
#include <move_failures.h>

inherit "/std/room/basic_room";

int standard, buy_only;
string place;
mapping exchanges;

int do_buy(object *obs, string country);

int query_standard() { return standard; }

void set_standard( int number ) { standard = number; }

int query_buy_only() { return buy_only; }

void set_buy_only() { buy_only = 1; }

string query_place() { return place; }

void set_place( string word ) { place = word; }

mapping query_exchanges() { return exchanges; }

void set_exchanges( mapping map ) { exchanges = map; }

string exchanges_list() {
    int i, value, smallest;
    string list, *places;
    if ( !sizeof( exchanges ) )
         return "There are no exchanges to be made here.";
    list = "";
    places = keys( exchanges );
    for ( i = 0; i < sizeof( places ); i++ ) {
         list += places[ i ] +":\n      $I$+20,+10="+
                  (string)MONEY_HAND->money_value_string( standard, place ) +
                  " will buy: ";
         value = ( exchanges[ places[ i ] ][ 0 ] * standard ) / 100;
         smallest = MONEY_HAND->smallest_value_in( places[ i ] );
         if (value % smallest != 0)  value += smallest - (value % smallest);
         list += (string)MONEY_HAND->money_value_string( value, places[ i ] );
         if ( buy_only ) {
             list += ".$I$-20,-10=";
             if ( i < sizeof( places ) - 1 )
                  list += "\n";
         } else {
             list += ";$I$-20,-10=\n$I$+20,+10=      for "+
                      (string)MONEY_HAND->money_value_string( standard,
                      place ) +", sell: ";
             value = ( 100 * standard ) / exchanges[ places[ i ] ][ 1 ];
             list += (string)MONEY_HAND->money_value_string( value,
                      places[ i ] ) +".$I$-20,-10=";
             if ( i < sizeof( places ) - 1 )
                  list += "\n";
         }
    }
    return list;
} /* exchanges_list() */

void create() {
   do_setup++;
   ::create();
   do_setup--;
   standard = 100;
   place = "default";
   exchanges = ([ ]);
   if ( !do_setup ) {
      this_object()->setup();
      this_object()->reset();
   }
} /* create() */

void init() {
   ::init();
   add_command("buy", "[from] <string'" + implode(keys(exchanges), "|") + 
       "'> with <indirect:object:me>",  (: do_buy( $1, $4[0] ) :) );

   if ( !buy_only ){
      add_command("sell" ,"<indirect:object:me'stuff'>",
                        (:this_object()->sell($1):));
   }
   add_command("list", "", (:this_object()->list():));
} /* init() */

int do_buy(object *things, string country) {
   int     value;
   int     smallest;
   object  thing;
   object  money;
   mixed  *change;
   mixed  *monies;
   mixed  *no_value;
   string  word;
    
   // debug_printf( "%O, %O, %O\n", things, args, pattern );

   change = ({ });
   foreach (thing in things)  {
      if (thing->query_property("money"))  {
         change = MONEY_HAND->merge_money_arrays(change,
                                                 thing->query_money_array());
#ifdef USE_VAULT
         MONEY_VAULT->add_to_list(thing);
#else
         thing->move("/room/rubbish");
#endif
      }
   }

   if (sizeof(change) == 0) {
      return notify_fail("You can only use money to buy currency here.  "
                         "Sorry.\n");
   }

   if (undefinedp(exchanges[country])) {
      foreach (word in keys(exchanges)) {
         if (lower_case(country) == lower_case(word)) {
            country = word;
            break;
         }
      }
      if (country != word) {
         add_failed_mess("You cannot exchange that currency here.  Only " + 
              query_multiple_short(keys(exchanges)) + " currency can be "
              "purchased at this shop.\n");
         this_player()->adjust_money(change);
         return 0;
      }
   }

   if (country == place) {
      this_player()->adjust_money(change);
      return notify_fail("You cannot buy currency from " + country +
            " here, since you're currently in " + country + ".\n");
   }

   monies = MONEY_HAND->filter_legal_money_to_array(change, place);
   no_value = monies[1];

   if (sizeof(monies[0]) != 0  &&  place != "default")  {
      monies = MONEY_HAND->filter_legal_money_to_array(monies[0], "default");
      if (sizeof(monies[0]) != 0)
         no_value = MONEY_HAND->merge_money_arrays(no_value, monies[0]);
      monies[0] = monies[1];
   }

   if (sizeof(no_value) != 0)  {
      this_player()->adjust_money(no_value);
      write("Sorry, but you cannot use " +
            MONEY_HAND->money_string(no_value) + " to buy currency here.\n");
   }

   if (sizeof(monies[0]) == 0) {
      return notify_fail("Sorry, but you didn't offer any acceptable money "
                         "to buy currency.\n");
   }

   value = (MONEY_HAND->query_total_value(monies[0], place) *
           exchanges[country][0]) / 100;
   smallest = MONEY_HAND->smallest_value_in(country);
   if (value % smallest != 0)  value += smallest - (value % smallest);
   change = MONEY_HAND->create_money_array(value, country);
   if (sizeof(change) == 0) {
      this_player()->adjust_money(monies[0]);
      return notify_fail("Sorry, but " +
            MONEY_HAND->money_string(monies[0]) + " is not worth anything "
            "in " + country + ".\n");
   }
   write("OK, you bought "+ MONEY_HAND->money_string(change) + " with " +
         MONEY_HAND->money_string(monies[0]) + ".\n");

#ifdef USE_VAULT
   money = MONEY_VAULT->get_money_ob();
#else
   money = clone_object(MONEY_OBJECT);
#endif
   money->set_money_array(change);
   if (money->move(this_player()) != MOVE_OK) {
      write("Unfortunately, you're too heavily burdened to accept all that "
            "money, so it's put on the table for you.\n");
      money->move(this_object());
   }

   say(this_player()->one_short() + " exchanges some currency.\n");

   return 1;
} /* do_buy() */


int sell(object *things) {
   int value;
   int test_val;
   int first;
   string *places;
   string  a_place;
   object thing;
   object money;
   mixed *monies;
   mixed *change;
   mixed *m_array_a;
   mixed *m_array_b;
   mixed *m_array_c;

   m_array_a = ({ });
   foreach (thing in things)  {
      if (thing->query_property("money")) {
         m_array_a = MONEY_HAND->merge_money_arrays(m_array_a,
                         thing->query_money_array());
#ifdef USE_VAULT
         MONEY_VAULT->add_to_list(thing);
#else
         thing->move("/room/rubbish");
#endif
      }
   }

   if (sizeof(m_array_a) == 0) {
      return notify_fail("You can only sell currency here.  Sorry.\n");
   }

   places = m_indices(exchanges);

   m_array_b = ({ });
   m_array_c = ({ });
   first = 1;
   foreach (a_place in places)  {
      monies = MONEY_HAND->filter_legal_money_to_array(m_array_a, a_place);
      m_array_a = monies[1];

      if (sizeof(monies[0]) != 0)  {
         /*
          * OK, we need to pull off the provincial coins because they have a 
          * value of 0 in any other zone, so the player gets ripped off
          * if they do "sell coins" and they have a mixture of provincial
          * and non-provincial coinage.   Got that?
          */
         if (first  &&  a_place != "default")   {
            monies = MONEY_HAND->filter_legal_money_to_array(monies[0], "default");
            if (sizeof(monies[0]) != 0)
               m_array_c = MONEY_HAND->merge_money_arrays(m_array_c, monies[0]);
            monies[0] = monies[1];
            first = 0;
         }

         test_val = MONEY_HAND->query_total_value(monies[0], a_place) *
                    exchanges[a_place][1] / 100;
         if (test_val)   {
            value += test_val;
            m_array_b = MONEY_HAND->merge_money_arrays(m_array_b, monies[0]);
         }
         else
            m_array_c = MONEY_HAND->merge_money_arrays(m_array_c, monies[0]);
      }

      if (sizeof(m_array_a) == 0)
         break;
   }

   if (sizeof(m_array_c) != 0)
      m_array_a = MONEY_HAND->merge_money_arrays(m_array_a, m_array_c);

   if (sizeof(m_array_a) != 0) {
      write("Sorry, but you cannot sell " +
            MONEY_HAND->money_string(m_array_a) + " here.\n");
      this_player()->adjust_money(m_array_a);
   }

   if (sizeof(m_array_b) == 0) {
      return notify_fail( "Sorry, but you aren't offering to sell any "
            "acceptable coinage.\n");
   }
   change = MONEY_HAND->create_money_array(value, place);
   if (sizeof(change) == 0) {
      this_player()->adjust_money(m_array_b);
      return notify_fail("Sorry, but " +
            MONEY_HAND->money_string(m_array_b) + " is not "
            "worth anything in " + place + ".\n");
   }

   write("OK, you sold " + MONEY_HAND->money_string(m_array_b) + " for " +
         MONEY_HAND->money_string(change) + ".\n");

#ifdef USE_VAULT
   money = MONEY_VAULT->get_money_ob();
#else
   money = clone_object(MONEY_OBJECT);
#endif
   money->set_money_array(change);
   if (money->move(this_player()) != MOVE_OK) {
      write("Unfortunately, you're too heavily burdened to accept all that "
            "money, so it's put on the table for you.\n");
      money->move(this_object());
   }

   say(this_player()->one_short() + " exchanges some currency.\n");

   return 1;
} /* sell() */

int list() {
   write( exchanges_list() +"\n" );
   return 1;
} /* list() */
