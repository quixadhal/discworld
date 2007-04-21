/*   -*- LPC -*-   */
/*
 * $Locker: presto $
 * $Id: money_handler.c,v 1.33 2002/03/03 06:15:45 presto Exp presto $
 */
/**
 * This handles all the methods for determining values of coins and
 * the current valid set of coins.   It also handles change calculation.
 * This was written originaly by Pinkfish, reworked significantly by
 * Deutha to add in the multiple currency areas.
 * @see /std/living/money.c
 * @author Pinkfish
 */
#include <money.h>

#define DEF_VALUE 1
#define SAVE_FILE "/save/money_handler"
#undef USE_VAULT

mapping values;
mapping symbols;
mapping details;
mapping aliases;
mapping adjectives;

mixed *merge_money_arrays(mixed *m_array1, mixed *m_array2);
varargs object *filter_legal_money_array(mixed *m_array, string where);


void create() {
   seteuid( (string)"/secure/master"->
      creator_file( file_name( this_object() ) ) );
   values = ([ "default": ({ "brass", 1, "copper", 10, "silver", 100,
                                          "gold", 2000, "platinum", 6000 }) ]);
   symbols = ([ ]);
   details = ([
      "brass": ({ "heads", "tails", "a head", "a tail", "brass", 0 }),
      "copper": ({ "heads", "tails", "a head", "a tail", "copper", 0 }),
      "silver": ({ "heads", "tails", "a head", "a tail", "silver", 0 }),
      "gold": ({ "heads", "tails", "a head", "a tail", "gold", 0 }),
      "platinum": ({ "heads", "tails", "a head", "a tail", "platinum", 0 }) ]);
   aliases = ([ ]);
   adjectives = ([ ]); 
   if ( file_size( SAVE_FILE +".o" ) > 0 ) {
      unguarded((: restore_object, SAVE_FILE :));
   }
   if ( !symbols ) {
      symbols = ([ ]);
   }
   if ( !aliases ) {
      aliases = ([ ]);
   }
   if ( !adjectives ) {
      adjectives = ([ ]);
   }
} /* create() */

/**
 * This method saves the current state of the money object.
 */
void save_me() { unguarded( (: save_object, SAVE_FILE :) ); }

/**
 * This method returns the mapping containing all the values of the
 * currently valid money types.   The mapping has keys of the domain
 * of the money and has a value of an array.   The array contains
 * alternating name, value pairs.
 * <pre>
 * ([ "default": ({ "brass", 1, "copper", 10, "silver", 100,
 *                                             "gold", 2000, "platinum", 6000 }) ])
 * </pre>
 * @return the mapping of values
 * @see query_values()
 * @see query_values_in()
 */
mapping query_all_values() { return copy( values ); }

/**
 * This method returns the current set of areas in which types can
 * be found.
 * @return the set of places
 */
string *query_all_places() {
   return keys(values);
} /* query_all_places() */

/**
 * This method returns the values in the default area.
 * This method returns the array as given in the value above.
 * It contains name, value pairs and is for the "default"
 * area.
 * @return the array of values
 * @see query_all_values()
 * @see query_values_in()
 */
mixed *query_values() { return copy( values[ "default" ] ); }

/**
 * This method returns the values in the specified area.
 * It contains name, value pairs and is for the "default"
 * area.
 * @return the array of values
 * @param where the area in which to return the values for
 * @see query_all_values()
 * @see query_values()
 * @see add_type()
 * @see query_mapped_values_in()
 */
mixed *query_values_in( string where ) {
   if ( !where || ( where == "" ) ) {
      where = "default";
   }
   return copy( values[ where ] );
} /* query_values_in() */

/**
 * This returns the smallest value in the specified area.  Used to
 * make sure we are not charging too little or too much when doing
 * comparisons.
 * @return the smallest value in the specified area
 * @param where the area to check
 */
int query_smallest_value_in(string where) {
   mixed* values;
   int value;
   int i;

   if (! where) {
      where = "default";
   }
   values = query_values_in(where);
   value = values[1];
   for (i = 2; i < sizeof(values); i += 2) {
      if (values[i + 1] < value) {
         value = values[i + 1];
      }
   }
   return value;
} /* query_smallest_value_in() */

/**
 * This adds a type of money to the money handler.
 * @param where the area in which to add the type of money
 * @param type the name of the money to add
 * @param value the value of the money
 * @see query_values_in()
 * @see remove_type()
 */
void add_type( string where, string type, int value ) {
   int i;

   if ( !values[ where ] ) {
      values[ where ] = ({ type, value });
      save_me();
      return;
   }
   if ( member_array( type, values[ where ] ) != -1 ) {
      return;
   }
   for ( i = 0; i < sizeof( values[ where ] ); i += 2 ) {
      if ( value < values[ where ][ i + 1 ] ) {
         values[ where ] = values[ where ][ 0 .. i - 1 ] + ({ type, value })
                           + values[ where ][ i .. sizeof( values ) ];
         save_me();
         return;
      }
   }
   values[ where ] += ({ type, value });
   save_me();
} /* add_type() */

/**
 * This method removes the type of money from the handler.
 * @param where the area to remove it from
 * @param type the type to remove
 * @see add_type()
 */
void remove_type( string where, string type ) {
   int i;

   if ( !values[ where ] ) {
      return;
   }
   i = member_array( type, values[ where ] );
   if ( i == -1 ) {
      return;
   }
   values[ where ] = delete( values[ where ], i, 2 );
   if ( !sizeof( values[ where ] ) ) {
      map_delete( values, where );
   }
   save_me();
} /* remove_type() */

/**
 * This method returns all the details for the current set of
 * coins.   The details are information which is shown when the coin
 * is looked at.   Stuff about heads and tails and things.
 * <pre>
 *   ([
 *      "brass": ({ "heads", "tails", "a head", "a tail", "brass", 0 }),
 *      "copper": ({ "heads", "tails", "a head", "a tail", "copper", 0 }),
 *      "silver": ({ "heads", "tails", "a head", "a tail", "silver", 0 }),
 *      "gold": ({ "heads", "tails", "a head", "a tail", "gold", 0 }),
 *      "platinum": ({ "heads", "tails", "a head", "a tail", "platinum", 0 }) ])
 * </pre>
 * The places correspond to:
 * <pre>
 * ({ forward short, reverse short,
 *      forward long, reverse long, composition, plural })
 * </pre>
 * @return the details array
 */
mapping query_details() { return copy( details ); }

/**
 * This method returns the details for a specified type of money.
 * It will return an array of the form:
 * <pre>
 * ({ "heads", "tails", "a head", "a tail", "brass", 0 })
 * </pre>
 * The places correspond to:
 * <pre>
 * ({ forward short, reverse short,
 *      forward long, reverse long, composition, plural })
 * </pre>
 * @param word the money type to get the type for
 * @see add_details()
 */
mixed *query_details_for( string word ) {
   if ( !details[ word ] ) {
      return ({ "heads", "tails", "a head", "a tail", "unknown", 0 });
   }
   return copy( details[ word ] );
} /* query_details_for() */

/**
 * This method adds the details for the given coin type into the current
 * list.
 * @param word the coin type the details are for
 * @param hd_sht the heads side short
 * @param tl_sht the tail side short
 * @param hd_lng the head side long
 * @param tl_lnd the tail side long
 * @param composition the composition of the money
 * @param plural the plural value of the object, if 0 then use default plural
 * @see query_details_for()
 * @see remove_details()
 */
void add_details( string word, string hd_sht, string tl_sht, string hd_lng,
                  string tl_lng, mixed composition, string plural ) {
   if ( details[ word ] ) {
      return;
   }
   details[ word ] = ({ hd_sht, tl_sht, hd_lng, tl_lng, composition, plural });
   save_me();
} /* add_details() */

/**
 * This method removes the specified detail. 
 * @param word the type of money to remove the details for
 * @see add_detail()
 * @see query_details_for()
 */
void remove_details( string word ) {
   if ( !details[ word ] ) {
      return;
   }
   map_delete( details, word );
   save_me();
} /* remove_details() */

/**
 * This method returns all the symbols for the current money areas in the
 * handler.   The return value is mapping with the key being the
 * money area and the value being the symboliser for the money.
 * The symboliser is called with a value to get the money to
 * print itself out nicely.   This is used when the quantity of
 * actual coins is not known and only the value of them is
 * known.
 * @return all of the symbols
 * @see query_symbol_for()
 * @see add_symbol()
 * @see remove_symbol()
 */
mapping query_symbols() { return copy( symbols ); }

/**
 * This method returns the symboliser for the specified money area.
 * @param word the area in which the money is occuring
 * @return the syboliser for the money area
 * @see query_symbols()
 * @see add_symbol()
 * @see remove_symbol()
 */
string query_symbol_for( string word ) {
   if ( !symbols[ word ] ) {
      return 0;
   }
   return copy( symbols[ word ] );
} /* query_symbol_for() */

/**
 * This method adds in a symboliser for a specified money area.
 * @example
 * add_symbol("Ankh-Morpork", "/d/am/money");
 * @example
 * // This is an example of a symboliser object
 * string symbolise_value( int value ) {
 *    int dollars, pence;
 * 
 *    dollars = value / 400;
 *    pence = ( value % 400 ) / 4;
 *    if ( !pence ) {
 *       return "A$"+ dollars;
 *    }
 *    if ( !dollars ) {
 *       return pence +"p";
 *    }
 *    if ( pence < 10 ) {
 *       return "A$"+ dollars +".0"+ pence;
 *    }
 *    return "A$"+ dollars +"."+ pence;
 * } /\* symbolise_value() *\/
 * @param word the money area to add the symbol for
 * @param symboliser the path to the symboliser object
 * @see query_symbols()
 * @see query_symbol_for()
 * @see remove_symbol()
 */
void add_symbol( string word, string symboliser ) {
   if ( symbols[ word ] ) {
      return;
   }
   symbols[ word ] = symboliser;
   save_me();
} /* add_symbol() */

/**
 * This method removes the symboliser for the particular money area.
 * @see query_symbols()
 * @see query_symbol_for()
 * @see add_symbol()
 * @param word the money area to remove the symbol for
 */
void remove_symbol( string word ) {
   if ( !symbols[ word ] ) {
      return;
   }
   map_delete( symbols, word );
   save_me();
} /* remove_symbol() */

/** @ignore yes */
string query_alias_for( string type ) {
   log_file("OBSOLETE_CALLS",
      sprintf("%O %s: %O called query_alias_for\n",
         this_object(),
         ctime(time()),
         previous_object()
         ));
   if ( !details[ type ] ) return "coin";
   if ( !details[ type ][ 5 ] ) return "coin";
   return explode( type, " " )[ sizeof( explode( type, " " ) ) - 1 ];
} /* query_alias_for() */

/**
 * This method sets the aliases for the specified money type.
 * @param type the type of money to set the aliases for
 * @param words the aliases for the money
 * @see query_aliases_for()
 * @param type the type of money to set the aliases for
 * @param words the aliases for the money
 */
void set_aliases_for(string type, string *words) {
   if (!words || !sizeof(words)) {
      if (aliases[type]) {
         map_delete(aliases, type);
      }
   } else {
      aliases[type] = words;
   }
   save_me();
   return;
} /* set_aliases_for() */

/**
 * This method returns all the current aliases for the given type
 * of money.
 * @param type the type of money to get the aliases for
 * @return the aliases for the money type
 * @see set_aliases_for()
 */
string *query_aliases_for( string type ) {
   string *ret;

   if ( !details[ type ] || !details[ type ][ MONEY_DETAILS_PLURAL ] ) {
      ret = ({ "coin" });
   } else {
      ret = ({ explode(type, " ")[<1] });
   }
   if ( aliases[ type ] ) {
      ret += aliases[ type ];
   }
   return ret;
} /* query_aliases_for() */

/**
 * This method sets the adjectives for the specified money type.
 * @param type the type of money to set the adjectives for
 * @param words the adjectives for the money
 * @see query_adjectives_for()
 * @param type the type of money to set the adjectives for
 * @param words the adjectives for the money
 */
void set_adjectives_for(string type, string *words) {
   if (!words || !sizeof(words)) {
      if (adjectives[type]) {
         map_delete(adjectives, type);
      }
   } else {
      adjectives[type] = words;
   }
   save_me();
   return;
} /* set_adjectives_for() */

/**
 * This method returns all the current adjectives for the given type
 * of money.
 * @param type the type of money to get the aliases for
 * @return the adjectives for the money type
 * @see set_adjectives_for()
 */
string *query_adjectives_for( string type ) {
   string *ret;

   if ( adjectives[ type ] ) {
      ret = adjectives[ type ];
   }
   else {
      ret = ({ });
   }
   
   return ret;
} /* query_adjectives_for() */

/**
 * This method returns the short description of the money type.
 * @param type the money type to get the short description for
 * @return the short description for the money object
 * @see query_main_plural_for()
 */
string query_short_for( string type ) {
   if ( !details[ type ] ) {
      return type +" coin";
   }
   if ( !details[ type ][ MONEY_DETAILS_PLURAL ] ) {
      return type +" coin";
   }
   return type;
} /* query_short_for() */

/**
 * This method returns the short plural description of the money type.
 * This returns just the one word, like 'coins' or 'talons'.
 * @param type the money type to get the short plural description for
 * @return the short plural description for the money object
 * @see set_plural_for()
 * @see query_main_plural_for()
 */
string query_plural_for( string type ) {
   if ( !details[ type ] ) {
      return "coins";
   }
   if ( !details[ type ][ MONEY_DETAILS_PLURAL ] ) {
      return "coins";
   }
   return details[ type ][ MONEY_DETAILS_PLURAL ];
} /* query_plural_for() */

/**
 * This method sets the plural for the specified money type.
 * @param type the money to set the plural for
 * @param plural the new plural for the money
 * @see query_plural_for()
 * @see query_main_plural_for()
 */
void set_plural_for( string type, string plural ) {
   if ( !details[ type ] ) {
      return;
   }
   details[ type ][ MONEY_DETAILS_PLURAL ] = plural;
   save_me();
} /* set_plural_for() */

/**
 * This method returns the main short plural description of the money type.
 * This returns the expanded plural version like 'Ankh-Morpork pennies'.
 * @param type the money type to get the short plural description for
 * @return the short plural description for the money object
 * @see set_plural_for()
 * @see query_plural_for()
 * @see query_short_for()
 */
string query_main_plural_for( string type ) {
   string *type_exp;

   if ( !details[ type ] ) {
      return type +" coins";
   }
   if ( !details[ type ][ MONEY_DETAILS_PLURAL ] ) {
      return type +" coins";
   }
   type_exp = explode(type, " ");
   return implode(type_exp[0 .. <2] +
      ({ details[ type ][ MONEY_DETAILS_PLURAL ] }), " " );
} /* query_main_plural_for() */

/**
 * This method returns the value of a specified type of money in a certain
 * money area.
 * @param type the type of money to get the value for
 * @param where the money area the money is in
 * @return the integer value of the money
 * @see query_total_value()
 */
varargs int query_value( string type, string where ) {
   int i, j, count, total;
   string *places;

   if ( !where || ( where == "" ) ) {
      where = "default";
   }
   if ( where == "mean" ) {
      places = m_indices( values );
      for ( i = 0; i < sizeof( places ); i++ ) {
         if ( ( j = member_array( type, values[ places[ i ] ] ) ) != -1 ) {
            count++;
            total += values[ places[ i ] ][ j + 1 ];
         }
      }
      if ( !count ) {
         return 0;
      }
      if ( !( total / count ) ) {
         return 0;
      }
      return total / count;
   }
   if ( !values[ where ] ) {
      return 0;
   }
   i = member_array( type, values[ where ] );
   if ( i == -1 ) {
      return 0;
   }
   return values[ where ][ i + 1 ];
} /* query_value() */

/**
 * This method determines the total value of a specified money array.
 * A money array consists of pairs of values ({ type, number })
 * @param mon_array the array to find the value of
 * @param where the money area to get the value in
 * @return the total value as an integer
 * @see query_value()
 */
varargs int query_total_value( mixed mon_array, string where ) {
   int i, amt;

   if ( !where || ( where == "" ) ) {
      where = "default";
   }
   for ( i = 0; i < sizeof( mon_array ); i += 2 ) {
      amt += mon_array[ i + 1 ] * query_value( mon_array[ i ], where );
   }
   return amt;
} /* query_total_value() */

/**
 * This method converts a money array into a string so it can be displayed.
 * @param mon_array the money array to convert into a string
 * @see money_value_string()
 */
string money_string( mixed mon_array ) {
   int i;
   string ret;

   if ( !sizeof( mon_array ) ) {
      return "nothing";
   }
   ret = "";
   while( i < sizeof( mon_array ) ) {
      if ( !mon_array[ i + 1 ] ) {
         mon_array = delete( mon_array, i, 2 ); 
      } else {
         i += 2;
      }
   }
   for ( i = 0; i < sizeof( mon_array ); i += 2 ) {
      ret += mon_array[ i + 1 ] +" ";
      if ( mon_array[ i + 1 ] == 1 ) {
         ret += query_short_for( mon_array[ i ] );
      } else {
         ret += query_main_plural_for( mon_array[ i ] );
      }
      if ( i == sizeof( mon_array ) - 4 ) {
         ret += " and ";
      } else if ( i != sizeof( mon_array ) - 2 ) {
         ret += ", ";
      }
   }
   return ret;
} /* money_string() */

/**
 * This method creates a money array from a certain value in a particular
 * money area. A money array consists of ({ type, number }) pairs in an
 * array.   ie: ({ "brass", 12, "copper", 24 }).
 * @example 
 * place = query_property("place");
 * if (!place) {
 *      place = "default";
 * }
 * mon_array = create_money_array( 1000, place);
 * @param value the value to get the money array for
 * @param where the money area to get the value in
 * @return a money array for the value in the area
 * @see money_value_string()
 */
varargs mixed *create_money_array( int value, string where ) {
   int i, amt;
   mixed *mon_array;

   if ( !where || ( where == "" ) ) {
      where = "default";
   }
   if ( !value ) {
      return ({ });
   }
   mon_array = ({ });
   for ( i = sizeof( values[ where ] ) - 2; i >= 0; i -= 2 ) {
      if ( value >= values[ where ][ i + 1 ] ) {
         mon_array += ({ values[ where ][ i ], amt = value /
                         values[ where ][ i + 1 ] });
         value -= amt * values[ where ][ i + 1];
      }
   }
   return mon_array;
} /* create_money_array() */

/**
 * This method returns a string which is based on the value of
 * the money in a certain money area.
 * @param value the value to get the string for
 * @param where the place to get the string for
 * @return a string of the money value in the certain money area
 * @see create_money_array()
 * @see money_string()
 * @see value_from_string()
 */
varargs string money_value_string( int value, string where ) {
   string symboliser;

   if ( !where || ( where == "" ) ) {
      where = "default";
   }
   if ( !symbols[ where ] ) {
      if (value < 0) {
         return "negative " + money_string( create_money_array( -value,
                                                                where ) );
      } else {
         return money_string( create_money_array( value, where ) );
      }
   }
   symboliser = symbols[ where ];
   return (string)symboliser->symbolise_value( value );
} /* money_value_string() */

/**
 * This method attempts to find a money value from a string.  It will
 * attempt to do fuzzy matching of the type.  This means it will match on
 * partial matches, this could lead to somewhat weird behaviour...  So it
 * goes...  It will return a money array, rather than a value
 * @param str the string to find the value of
 * @return a money array of the types matched
 * @see money_value_string()
 */
mixed* money_array_from_string(string str, string where) {
   int value;
   int number;
   int i;
   int pos;
   int frog;
   int match;
   int max_match;
   string plural;
   string *bits;
   string type;
   string match_name;
   mixed *stuff;
   mixed* ret_arr;

   if (!where) {
      where = "default";
   }

   if (symbols[ where ]) {
      value = symbols[ where ]->unsymbolise_string( str );
      if (value) {
         return create_money_array(value, where);
      }
   }
   ret_arr = ({ });
   stuff = values[ where ];
   if (!value && stuff) {
      /* Try and figure it out long hand. */
      bits = explode(str, " ") - ({ "and", ",", "" });
      for (i = 0; i < sizeof(bits); i++) {
         /* First we search for a number. */
         if (sscanf(bits[i], "%d", number) == 1 &&
             i + 1 < sizeof(bits)) {
            /*
             * Cool, now see if the next thing is a money type.  Go for longest
             * possible matching string
             */
            i++;
            type = bits[i];
            max_match = 0;
            do {
               match = 0;
               if (type[<1] == ',') {
                  type = type[0..<2];
               }
               type = lower_case(type);
               for (pos = 0; pos < sizeof(stuff); pos += 2) {
                  plural = query_plural_for(stuff[pos]);
                  // Find the last space and splice ourselves in.
                  frog = strsrch(stuff[pos], " ", -1);
                  if (frog) {
                     plural = stuff[pos][0..frog] + plural;
                  }
                  // Do fuzzy matching.
                  if (lower_case(stuff[pos]) == type ||
                      strsrch(lower_case(stuff[pos]), type) != -1 ||
                      lower_case(plural) == type ||
                      strsrch(lower_case(plural), type) != -1) {
                     match = stuff[pos + 1];
                     match_name = stuff[pos];
                  }
               }
               if (match) {
                  i++;
                  max_match = match;
                  if (i < sizeof(bits)) {
                     type += " " + bits[i];
                  }
               }
            } while (match && i < sizeof(bits));
            if (max_match) {
               ret_arr += ({ match_name, number });
            }
            i--;
         } else {
            i++;
         }
      }
   }
   return ret_arr;
} /* value_from_string() */

/**
 * This method attempts to find a money value from a string.  It will
 * attempt to do fuzzy matching of the type.  This means it will match on
 * partial matches, this could lead to somewhat weird behaviour...  So it
 * goes...
 * @param str the string to find the value of
 * @see money_value_string()
 * @example
 * // This will tell us the integer money value of the string.
 * write(MONEY_HAND->value_from_string("1 dollar and 12 pence",
 *                                     "Ankh-Morpork"));
 */
int value_from_string(string str, string where) {
   return query_total_value(money_array_from_string(str, where), where);
} /* value_from_string() */

/**
 * This method calculates the change of a certain value from a
 * given money array.   This makes sure that the change does not include
 * money that does not actually exist.
 * @param value the value of the change to calculate
 * @param mon_array the money array to determine the change from
 * @return the money array containing the change to use
 * @see make_payment()
 * @see pay_amount_from()
 */
mixed *calc_change( int value, mixed *mon_array ) {
   int i, num;
   mixed *ret;

   ret = ({ });
   for ( i = sizeof( mon_array ) - 2; i >= 0; i -= 2 ) {
      if ( value >= mon_array[ i + 1 ] ) {
         num = value / mon_array[ i + 1 ];
         value = value % mon_array[ i + 1 ];
         ret += ({ mon_array[ i ], num });
         if ( !value ) {
            return ret;
         }
      }
   }
   return ret;
} /* calc_change() */

/**
 * This method makes a payment from a money array.  It returns the
 * depleted money array, the amount taken out and the change
 * needed.  If the type is not set, then the best fit for the value
 * is found from the array.
 * <p>
 * The return array is formated as:<br>
 * ({ depleted_money_array, change, taken_from })<br>
 * The change is an integer value.
 * @example
 * ret = make_money_array_payment("Lancre Crown", 2, mon_array, "Lancre", 0);
 * @param type the type of money to take out (ie: "Lancre Crown")
 * @param value the amount of the type to take out
 * @param mon_array the money array to use
 * @param where the money area
 * @param use_default allow the use of the default money type
 * @return the return array as formated above
 */
mixed *make_money_array_payment( string type,
                                 int value,
                                 mixed *mon_array,
                                 string where,
                                 int use_default) {
   
   int i;
   int j;
   int num;
   int total;
   int cur_match;
   string mon_name;
   mixed *poss_values;
   mixed *ret;

   /* Figure out the money type. */
   if ( !where || ( where == "" ) ) {
      where = "default";
   }

   /* See if the money is there and its all easy. */
   if (type) {
      i = member_array( type, mon_array );
      if ( i != -1 ) {
         if ( value <= mon_array[ i + 1 ] ) {
            mon_array[ i + 1] -= value;
            return ({ ({ type, value }), 0, mon_array });
         }
      }

      /*
       * Damn, its not easy.  Figure out the real value and see if we can
       * get it out of the arrays.
       */
      value *= query_value( type, where );
   }

   // Cannot make a 0 value payment.
   if (!value) {
      return 0;
   }

   /* Check to make sure the total is ok. */
   total = query_total_value( mon_array, where );
   if (use_default  &&  where != "default") {
      total += query_total_value( mon_array, "default" );
   }
   /* If the value is more than the total...  */
   if ( value > total ) {
      return 0;
   }

   /* Get the possible values. */
   poss_values = ({ });
   if ( where != "default" && use_default ) {
      poss_values += values[ "default" ];
   }
   poss_values += values[ where ];

   /* Determine the return array */
   ret = ({ });
   /* This attempts an exact match of coins. */
   for ( i = ( sizeof( poss_values ) - 2 ); i >= 0; i -= 2 ) {
      j = member_array( poss_values[ i ], mon_array );
      if ( j != - 1 ) {
         if ( poss_values[ i + 1 ] <= value ) {
            num = value / poss_values[ i + 1 ];
            if ( num > mon_array[ j + 1 ] ) {
               num = mon_array[ j + 1 ];
            }
            mon_array[ j + 1] -= num;
            value -= num * poss_values[ i + 1 ];
            ret += ({ poss_values[ i ], num });
            if ( !value ) {
               return ({ ret, value, mon_array });
            }
         }
      }
   }

   /* No exact match...   So we need to figure out how much change to give. */
   /* One zoom through the array finding which one has the closest match. */
   cur_match = value + 10000000;
   for (i = 0; i < sizeof(poss_values); i +=2 ) {
      j = member_array( poss_values[ i ], mon_array);
      if (j != -1 &&
          mon_array[j + 1] > 0 &&
          poss_values[i + 1] >= value &&
          poss_values[i + 1] - value <= cur_match - value) {
         cur_match = poss_values[i + 1];
         mon_name = poss_values[i];
      }
   }

   if (mon_name) {
      j = member_array(mon_name, mon_array);
      i = member_array(mon_name, poss_values);
      mon_array[j + 1] -= 1;
      value = poss_values[i + 1] - value;
      ret += ({ poss_values[ i ], 1 });
   } else {
      return 0;
   }

   return ({ ret, value, mon_array });
} /* make_money_array_payment() */

/**
 * This method makes a payment of a particular amount in a particular
 * money area.   Please note that player or living objects can double
 * as money objects in this circumstance.  The first element of the
 * payment array is the values which should be used to take off
 * the player, the second element is the change needed to be payed
 * back.
 * @param type the type of money to pay in (ie: "Lancre Crown")
 * @param value the number of the type to pay
 * @param thing the thing which is doing the payment (money object)
 * @param where the money area the payment will occur in
 * @return the payment array
 * @see pay_amount_from()
 * @see calc_change()
 */
varargs mixed *make_payment( string type, int value, object thing,
                             string where ) {
   mixed *mon_array;
   mixed *stuff;

   if (!type) {
     return 0;
   }

   mon_array = copy(thing->query_money_array());
   stuff = make_money_array_payment( type, value, mon_array, where, 1);
   if (!stuff) {
      return stuff;
   }
   if (stuff[MONEY_PAY_CHANGE]) {
      return ({ stuff[MONEY_PAY_RETURN], calc_change(stuff[MONEY_PAY_CHANGE],
                                                     values[where]) });
   }
   return ({ stuff[MONEY_PAY_RETURN], stuff[MONEY_PAY_CHANGE] });
} /* make_payment() */

/**
 * This method makes a payment from a specified money object.
 * @param value the amount to pay
 * @param money the money object to pay from
 * @param where the money area the payment occurs in
 * @return the change object
 * @see make_payment()
 * @see calc_change()
 */
varargs object pay_amount_from(int value, object money, string where) {
   int i;
   object change;
   mixed *m_array, *p_array;
   mixed *change_array;
   mixed *pay_array;

   if (!where || where == "") {
      where = "default";
   }
   change_array = ({ });
   pay_array = ({ });
   m_array = create_money_array(value, where);
   for (i = 0; i < sizeof(m_array); i += 2) {
      p_array = make_payment(m_array[i], m_array[i + 1], money, where);
      if (!pointerp(p_array)) {
         continue;
      }

      if (sizeof(p_array[0]) > 0)
         pay_array = merge_money_arrays(pay_array, p_array[0]);
      if (sizeof(p_array[1]) > 0) {
         change_array = merge_money_arrays(change_array, p_array[1]);
      }
   }

   if (sizeof(pay_array) > 0)  {
      for (i = 0; i < sizeof(pay_array); i += 2) {
         pay_array[i + 1] = -pay_array[i + 1];
      }
      money->adjust_money(pay_array);
   }

   if (sizeof(change_array) == 0) {
      return 0;
   }

#ifdef USE_VAULT
   change = MONEY_VAULT->get_money_ob();
   change->set_money_array(change_array);
#else
   change = clone_object(MONEY_OBJECT);
#endif
   return change;

} /* pay_amount_from() */

/**
 * This method creates a money object of a certain value in a certain
 * money area.
 * @param value the value to create the new money object with
 * @param where the area to create the new money object in
 * @return the new money object
 */
varargs object make_new_amount( int value, string where ) {
   object money;

   if ( !where || ( where == "" ) ) {
      where = "default";
   }
#ifdef USE_VAULT   
   money = MONEY_VAULT->get_money_ob();
#else
   money = clone_object( MONEY_OBJECT );
#endif
   money->set_money_array( create_money_array( value, where ) );
   if ( !(int)money->query_value_in( where ) ) {
      money->dest_me();
      return 0;
   }
   return money;
} /* make_new_amount() */

/**
 * This method figures out the legal and illegal tender money from
 * the specified money object in the specified money area.   This method
 * returns a two element array which consists of the legal and illegal
 * tender for the given money area.   ({ legal, illegal }).  WARNING:
 * This method destructs the money object passed to it.
 * @param money the money object to get the legal tender from
 * @param where the money area the tender is for
 * @return an two element array of objects ({ legal, illegal })
 * @see parse_money()
 */
varargs object *filter_legal_tender(object money, string where) {
   mixed *m_array;

   m_array = (mixed *)money->query_money_array();
   money->dest_me();

   return filter_legal_money_array(m_array, where);

} /* filter_legal_tender() */


/**
 * This method is identical to filter_legal_tender, except that it takes
 * a money array rather than a money object
 * @param m_array the money array to get the legal tender from
 * @param where the money area the tender is for
 * @return an two element array of objects ({ legal, illegal })
 * @see parse_money()
 * @see filter_legal_tender()
 */
varargs object *filter_legal_money_array(mixed *m_array, string where) {
   int i;
   object good, no_good;
   mixed *poss_values;
   mixed *good_array = ({ });
   mixed *no_good_array = ({ });

   if (sizeof(m_array) == 0) {
      return ({ 0, 0 });
   }
   if (!(poss_values = values[where])) {
      poss_values = ({ });
   }
   if (!where || where == "") {
     where = "default";
   }

   for (i = 0; i < sizeof(m_array); i += 2) {
      if (member_array(m_array[i], poss_values) != -1) {
         good_array += ({ m_array[i], m_array[i + 1] });
      } else {
         no_good_array += ({ m_array[i], m_array[i + 1] });
      }
   }

   if (sizeof(good_array) == 0) {
      good = 0;
   }
   else  {
#ifdef USE_VAULT     
      good = MONEY_VAULT->get_money_ob();
#else
      good = clone_object(MONEY_OBJECT);
#endif
      good->set_money_array(good_array);
   }

   if (sizeof(no_good_array) == 0) {
      no_good = 0;
   }
   else  {
#ifdef USE_VAULT
     no_good = MONEY_VAULT->get_money_ob();
#else
     no_good = clone_object(MONEY_OBJECT);
#endif
     no_good->set_money_array(no_good_array);
   }

   return ({ good, no_good });

} /* filter_legal_money_array() */


/**
 * This method merges two money arrays together and returns the
 * merged array
 * @param m_array1 the first money array
 * @param m_array2 the second money_array
 * @return a money array of m_array1 and m_array2 joined
 */
mixed *merge_money_arrays(mixed *m_array1, mixed *m_array2)  {
   int i;
   int idx;
   mixed *new_m_array;

   if (!m_array1)
      m_array1 = ({ });
   if (!m_array2)
      m_array2 = ({ });

   /* Loop over smallest array for efficiency */
   if (sizeof(m_array1) < sizeof(m_array2))  {
      new_m_array = copy(m_array2);
      for (i = 0; i < sizeof(m_array1); i += 2)  {
         idx = member_array(m_array1[i], new_m_array);
         if (idx > -1)  {
            new_m_array[idx + 1] += m_array1[i + 1];
            if (new_m_array[idx + 1] <= 0)
               new_m_array = delete(new_m_array, idx, 2);
         }
         else
            new_m_array += ({ m_array1[i], m_array1[i + 1] });
      }
   }
   else  {
      new_m_array = copy(m_array1);
      for (i = 0; i < sizeof(m_array2); i += 2)  {
         idx = member_array(m_array2[i], new_m_array);
         if (idx > -1)  {
            new_m_array[idx + 1] += m_array2[i + 1];
            if (new_m_array[idx + 1] <= 0)
               new_m_array = delete(new_m_array, idx, 2);
         }
         else
            new_m_array += ({ m_array2[i], m_array2[i + 1] });
      }
   }
   return new_m_array;

} /* merge_money_arrays() */


/**
 * This method determines all the money from the player object and moves
 * it into a container.  It then figured out the legal tender for
 * specified money area and tells the player if the given money is
 * legal tender for the current area.  It will automatically return the
 * illegal tender and send a message to the player about it.
 * @param words the string to match the money on
 * @param player the player who is attempting the transaction
 * @param place the money area the transaction is taking place
 * @return a money object consisting of the legal tender
 * @see filter_legal_tender()
 */
varargs mixed parse_money(string words, object player, string place) {
   object  thing;
   object *monies;
   object *things;
   mixed  *m_array;
   mixed  *m_array2;
   int     i;

   things = match_objects_for_existence(words, ({ player }));
   if (sizeof(things) == 0) {
      // If not here, try a money_array_from_string thing.
      m_array = money_array_from_string(words, place);
       if (sizeof(m_array) == 0) {
         return NO_MATCH;
      }
      // Now, check and see if the money exists.
      thing = present(MONEY_ALIAS, player);
      if (!thing) {
         return NO_MATCH;
      }
      for (i = 0; i < sizeof(m_array); i += 2) {
         if (thing->query_money(m_array[0]) < m_array[1]) {
            return NO_MATCH;
         }
      }
   }
   else {
      things = filter(things, (: $1->query_property("money") :));
      if (sizeof(things) == 0)
         return NO_MONEY;

      m_array = ({ });
      foreach (thing in things)  {
         m_array = merge_money_arrays(m_array, thing->query_money_array());
#ifdef USE_VAULT
         // Not sure if this is correct.
         MONEY_VAULT->add_to_list(thing);
#endif
      }
   }

   monies = filter_legal_money_array(m_array, place);

   /* Illegal money */
   if (monies[1])  {
      tell_object(player, monies[1]->the_short() +
            (monies[1]->query_number_coins() == 1 ? " is" : " are") +  
            " not legal tender here.\n");
      m_array = monies[1]->query_money_array();
#ifdef USE_VAULT
      MONEY_VAULT->add_to_list(monies[1]);
#endif
   }
   else
      m_array = ({ });

   /* Legal money */
   if (monies[0])  {
      m_array2 = copy(monies[0]->query_money_array());
      for (i = 0; i < sizeof(m_array2); i += 2)
         m_array2[i + 1] = -m_array2[i + 1];
      merge_money_arrays(m_array, m_array2);
      this_player()->adjust_money(m_array);
      return monies[0];
   }
   else return NO_LEGAL;

} /* parse_money() */


/**
 * This method makes a payment from one person to another.
 * This method figures out what money should be given to the player
 * and what should be taken from the other to make a payment of the
 * correct value in the correct place.
 * @param value the value to pay
 * @param place the place to make the payment in
 * @param payer the person the money is payed from
 * @param payee the person the money is payed to
 * @return two element array, or 0 if it cannot be done
 */
mixed *query_person_payments(int value, string place,
                             object payer, object payee) {
   mixed *stuff;
   mixed *mon_array;
   mixed *rabbit;

   mon_array = copy(payer->query_money_array());
   stuff = make_money_array_payment( 0, value, mon_array, place, 0);
   if (!stuff) {
      return 0;
   }
   if (stuff[MONEY_PAY_CHANGE]) {
      /*
       * Ok, now check to see if we can get the change from the other
       * guy.
       */
      mon_array = copy(payee->query_money_array());
      rabbit = make_money_array_payment( 0, stuff[MONEY_PAY_CHANGE],
                                         mon_array, place, 0);
      if (!rabbit || rabbit[MONEY_PAY_CHANGE]) {
         return 0;
      }
      return ({ stuff[MONEY_PAY_RETURN], rabbit[MONEY_PAY_RETURN] });
   }
   return ({ stuff[MONEY_PAY_RETURN], ({ }) });
} /* query_person_payments() */

/**
 * This returns a list of valid coin types
 * @return an array of valid coin types
 */
string * query_valid_types(){
  mixed *tmp;
  string *elem, *valid_types = ({ });
  int i;
      
  tmp = values(query_all_values());
  foreach( elem in tmp ){ 
    for( i = 0; i < sizeof( elem ); i += 2 ){
      if ( stringp( elem[i] ) )
        valid_types += ({ elem[i] });
    }
  } return valid_types;
}/* query_valid_types() */

/**
 * This takes a coin type and returns the place it is associated with.
 * @param type the coin type i.e. "Ankh-Morpork dollar"
 * @return the place i.e. "Ankh-Morpork"
 */
string query_origin_of( string type ){
  string elem, *places;
  
  if( member_array( type, query_valid_types() ) == -1 )
    return 0;
  
  places = query_all_places();
  
  foreach( elem in places ){
    if( member_array( type,query_values_in( elem ) ) != -1 )
      return elem;
  }
}/* query_origin_of() */

/**
 * This converts a currency type's alias (i.e. "royal" ) and returns
 * its 'real' names (i.e. "Ankh-Morpork royal"
 * @param word the alias to find the real name of
 * @return an array of real names, or 0 if it's not a real alias
 */
string * query_aliases_of( string word ){
  string *types, elem, *aliases = ({ });
  
  types = query_valid_types();
  
  foreach( elem in types ){
    if( member_array( word,query_aliases_for( elem ) ) != -1 )
      aliases += ({ elem });
  } return aliases;
}/* query_alias_of */

/**
 *This returns the value of a currency type.
 * @param type currency type
 * @return an int of the currency type's value
 */
int query_value_of( string type ){
  if( member_array( type, query_valid_types() ) == -1 )
    return 0;
   return query_value( type, query_origin_of( type ) );
}/* query_value_of() */


/**
 * This returns the smallest unit of currency in this place.
 * @param place The place to query.
 * @return A string of the smallest unit of currency
 * @see smallest_value_in()
 */
string smallest_in( string place ) {
   int i, smallest, r;
   mixed *values;

   values = query_values_in( place );
   for ( i = 1; i < sizeof( values ); i += 2 ) {
      if ( !smallest || ( values[i] < smallest ) ) {
         smallest = values[i];
         r = i;
      }
   }
   return values[r-1];
}
/* smallest_in() */


/**
 * This returns the smallest value of currency in this place.
 * @param place The place to query.
 * @return the value of the smallest unit of currency
 * @see smallest_in()
 */
int smallest_value_in( string place ) {
   int i, smallest, r;
   mixed *values;

   values = query_values_in( place );
   for ( i = 1; i < sizeof( values ); i += 2 ) {
      if ( !smallest || ( values[i] < smallest ) ) {
         smallest = values[i];
         r = i;
      }
   }
   return values[r];
}
/* smallest_value_in() */


/**
 * This method returns the values in the specified area.
 * It contains name, value pairs and is for the "default"
 * area.  It is returned as a mapping for easier sorting
 * & manipulation.
 * @param where the area in which to return the values for
 * @see query_all_values()
 * @see query_values()
 * @see add_type()
 * @see query_values_in()
 */
mapping query_mapped_values_in( string where ) {
   mapping values = ([]);
  mixed * array;
  int i;

   if ( !where || ( where == "" ) )
      where = "default";
  array = query_values_in(where);
   for ( i = 1; i < sizeof(array); i += 2 )
  {
      values += ([array[i-1]:array[i]]);
   }
   return values;
} /* query_mapped_values_in() */
