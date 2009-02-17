/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: cont_medium.c,v 1.28 2003/04/12 03:52:48 starr Exp $
*/
/**
 * Continuous Medium Object.
 *
 * amount: current quantity of medium in arbitrary units
 *
 * continuous: 1 for continuous (default), reset to 0 for normal
 *             object behaviour
 *
 * collective: 1 for collective, 0 for normal behaviour.
 *
 * weight_unit: ({ a, b }) means that b units have weight a
 *
 * value_scale (float): if set & continuous, value is ::query_value() * amount
 *                      * value_scale; else value is ::query_value()
 *
 * medium_alias: used for identification - choose something with
 *               capitals (e.g. "ChalkDust") so players can't
 *               use it as a valid alias by accident
 *
 * pile_name: "pile", "collection", "puddle", etc.
 *
 * amount_types: ([ "pinch": ({ 1, "pinches" }),
 *                  "handful": ({ 50, "handfuls" }) ])
 *
 * pile_types: ({ 50, "small", 100, "medium", 150, "large", "huge" });
 *
 * Added by Jeremy, 4-Nov-95:
 *
 * ground, longs, shorts, medium_aliases
 *
 * The above were removed by Jeremy, 18-Jun-97.  Hopefully no one will
 * miss them.
 * @author Jeremy
 *
 * Additions for collective objects:
 * Since "a" and "an" will be replaced with the number of objects, 
 * "$a" and "$an" can be used if there should be a non-changing "a" or "an"
 * in the long.
 * For verbs you can use $V<singular-verb>$<plural-verb>. Example: $Vglows$glow
 * If there are any words which should be pluralized, place a $P in front of it,
 * like $Pleaf or $Pword
 * Finally you can use $A<word> if you want "a word" in for one object and
 * "words" for more than one.
 */

#include <move_failures.h>
#include <parse_command.h>
#include <obj_parser.h>

inherit "/std/object";

private int amount;
private int continuous;
private int collective;

private nosave string my_filename;
private int *weight_unit;
private nosave int no_join;
private float value_scale;
private string medium_alias;
private string pile_name;
private string *_sorted_amount_names;
private mapping amount_types;
private mixed *pile_types;
private nosave mixed _short_name;

object make_medium_clone( int number );

void create() {
  int junk;

  do_setup++;
  ::create();
  do_setup--;
  continuous = 0;
  collective = 0;
  weight_unit = ({ 1, 200 });
  pile_name = "pile";
  if ( !do_setup )
    this_object()->setup();
  if( !clonep( this_object() ) )
    return;
  sscanf( file_name( this_object() ), "%s#%d", my_filename, junk );
  amount = 1;
}

/** @ignore yes */
void set_name( string word ) {
  ::set_name( word );
  if ( continuous )
    add_plural( word );
} /* set_name() */

/**
 * This method returns the amount of the stuff there is.  This is a
 * volume measurement.
 * @return the amount of stuff
 */
int query_amount() { return amount; }

/**
 * This method sets the amount of the stuff there is.
 * @param number the amount to set it to
 */
void set_amount( int number ) {
  if(environment() && continuous)
    environment()->add_volume(number - amount);

  amount = number;
  if(continuous || collective)
    set_weight((amount * weight_unit[ 0 ] ) / weight_unit[ 1 ]);
}


/**
 * This method changes the amount of stuff.
 * @param number the amount to change it by
 */
void adjust_amount(int number) {
  if(environment() && continuous)
    environment()->add_volume(number);

  amount += number;
  if(amount < 1) {
    no_join = 1;
    move( "/room/rubbish" );
    return;
  }
  if(continuous || collective)
    set_weight((amount * weight_unit[0]) / weight_unit[1]);
}

/**
 * This method tests to see if this object is continuous or not.
 * @return 1 if it is continuous, 0 if not
 */
int query_continuous() { return continuous; }

/**
 * This method tests to see if this object is a collective or not.
 * @return 1 if it is continuous, 0 if not.
 */
int query_collective() { return collective; }

/**
 * This method returns 1 if the object is available for mergin.
 * @return 1 if we can merge
 */
int query_merger() { return 1; }

/**
 * This method makes the object continuous.
 */
void set_continuous() {
  collective = 0;
  if (continuous) {
    return;
  }
  if ( environment() ) {
    environment()->add_volume( -200*query_weight() );
  }
  continuous = 1;
  if(environment()) {
    environment()->add_volume( amount );
  }
} /* set_continuous() */

/**
 * This method removes the continuous property from the object.
 */
void reset_continuous() {
  if (!continuous) {
    return;
  }
  continuous = 0;
  if ( environment() ) {
    environment()->add_volume( -amount );
  }
} /* reset_continuous() */

/*
 * This method makes the object a collective.
 */
void set_collective() {
  collective = 1;
  continuous = 0;
  add_property( "determinate", "" );
}

/**
 * This method removes the collective property from the object.
 */
void reset_collective() {
  collective = 0;
}

/**
 * This method returns the weight units used for this type
 * of unit.  The weight unit is an array ({ a, b }) which means
 * that b units of stuff have weight a.
 * @return the weight array
 */
int *query_weight_unit() { return weight_unit; }

/**
 * This method sets the weight units used for this type
 * of stuff.  The weight unit is an array ({ a, b }) which means
 * that b units of stuff have weight a.
 * @param numbers the new weight unit
 */
void set_weight_unit( int *numbers ) { weight_unit = numbers; }

/**
 * This method checks to see if we are allowed to join with other
 * objects when we encounter them.  A join will occur if we end up in
 * same environment and the medium aliases match.
 * @return 1 if they cannot join, 0 if they can
 */
int query_no_join() { return no_join; }

/**
 * This method stops this object joining with other
 * objects when we encounter them.  A join will occur if we end up in
 * same environment and the medium aliases match.
 */
void set_no_join() { no_join = 1; }

/**
 * This method lets this object joining with other
 * objects when we encounter them.  A join will occur if we end up in
 * same environment and the medium aliases match.
 */
void reset_no_join() { no_join = 0; }

/**
 * This method returns the medium alias.  This should be a unique alias
 * for the medium, it is used when joining to see if we can join or not
 * It it used for identification - choose something with
 * capitals (e.g. "ChalkDust") so players can't
 * use it as a valid alias by accident.
 * @return the medium alias
 */
string query_medium_alias() { return medium_alias; }

/**
 * This method sets the medium alias for the object.
 * This should be a unique alias
 * for the medium, it is used when joining to see if we can join or not
 * It it used for identification - choose something with
 * capitals (e.g. "ChalkDust") so players can't
 * use it as a valid alias by accident.
 & @param word the medium alias
 */
void set_medium_alias( string word ) {
   medium_alias = word;
   add_alias( medium_alias );
} /* set_medium_alias() */

/**
 * This method returns the name we give the piles of this stuff.  This
 * is the name given to piles of the object.
 * @return the name of the pile
 */
string query_pile_name() { return pile_name; }

/**
 * This method returns the name we give the piles of this stuff.
 * @example
 * set_pile_name("puddle");
 * @param word the name of the pile
 */
void set_pile_name( string word ) { pile_name = word; }

/**
 * This method returns the types of the different amounts.
 * argument is a mapping which contains the name of the pile and
 * value is an array, the first element is the amount needed for
 * the name and the second element is the plural of the pile name.
 * @return the amount types mapping
 */
mapping query_amount_types() { return amount_types; }

/**
 * This method sets the sizes of the piles and their names.  The
 * argument is a mapping which contains the name of the pile and
 * value is an array, the first element is the amount needed for
 * the name and the second element is the plural of the pile name.
 * @example
 * set_amount_types( ([ "pinch": ({ 1, "pinches" }),
 *                  "handful": ({ 50, "handfuls" }) ]));
 * @param map the amount types mapping
 */
void set_amount_types( mapping map ) {
  amount_types = map;
  // This is for efficiency in amount_size() and amount_size_short()
  _sorted_amount_names = sort_array(m_indices( amount_types ),
      (: amount_types[$1][0] - amount_types[$2][0] :));
}

/**
 * This method returns the array of pile types.
 * The pile types array starts with a number, the number is the
 * cut off amount to get the size name in the next array element.
 * The numbers and strings alternate and the last element is the
 * the default size.
 * @return the array of pile types
 */
mixed query_pile_types() { return pile_types; }

/**
 * This method sets the name of the piles given the different
 * amounts of piles available.  The last pile type in the list is
 * the size to use for anything larger.  The pile sizes must be in
 * order.
 * @example
 * set_pile_types(({ 50, "small", 100, "medium", 150, "large", "huge" }));
 * @param args the pile types
 */
void set_pile_types( mixed *args ) { pile_types = args; }

/**
 * This method returns the maximum number of piles of the
 * object that can be seen.  Any more than this turns into 'many'.
 * @return the maximum number of piles that can be seen
 */
int query_max_seen() { return 12; }

/**
 * This method returns the value of the object.  If the object is
 * not continuous, or the value_scale has not been set, the value is
 * the value set with set_value(), as usual.  Otherwise, the value is
 * scaled by value_scale and multiplied by the current amount.
 * @return the value of the object
 * @see set_value()
 * @see set_value_scale()
 * @see query_value_scale()
 */
int query_value() {

  if (!floatp(value_scale) || !continuous) {
    if(collective)
      return (amount * ::query_value() );
    return ::query_value();
  }
  return to_int(::query_value() * value_scale * amount);
} /* query_value() */

/**
 * This method sets the amount by which to scale the value of a
 * continuous object.  If not set, the value will be that set with
 * set_value().
 * @param f the value scale
 * @see query_value_scale()
 * @see query_value()
 */
void set_value_scale(float f) { value_scale = f; }

/**
 * This method returns the current value scale.
 * @return the value scale
 * @see set_value_scale()
 * @see query_value()
 */
float query_value_scale() { return value_scale; }

/**
 * This returns as an integer 'how many' we have of a particular
 * type.
 * @return int representing the amount of a particular pile.
 */
int query_how_many() {
  int biggest_amount;
  int how_many;
  string size;
  string biggest_size;

  if(!amount)
    return 0;

  if(!m_sizeof(amount_types)) {
    if(collective)
//      Wrong! Wrong! -- Terribly wrong!
//      return query_num(amount);
        return amount;
    return 0;
  }

  biggest_size = _sorted_amount_names[0];
  foreach (size in _sorted_amount_names) {
    if (amount < amount_types[size][0]) {
      break;
    }

    biggest_size = size;
  }

  biggest_amount = amount_types[biggest_size][0];

  how_many = ( amount + biggest_amount / 2 ) / biggest_amount;

  return how_many;
}

/**
 * This method returns a string which tells us how much stuff we
 * have.
 * @return string representing the amount of stuff we have
 */
varargs string amount_size(int exact) {
   int biggest_amount;
   int how_many;
   string size;
   string biggest_size;
   string biggest_plural;

   if ( !amount ) {
      return "none at all";
   }
   if ( !m_sizeof( amount_types ) ) {
      return "minus one Cornish pint";
   }
   biggest_size = _sorted_amount_names[0];
   foreach (size in _sorted_amount_names) {
     if (amount < amount_types[size][0]) {
       break;
     }
     biggest_size = size;
   }
   biggest_amount = amount_types[biggest_size][0];
   biggest_plural = amount_types[biggest_size][1];
   how_many = ( amount + biggest_amount / 2 ) / biggest_amount;
   if ( !how_many ) {
      return "apparently not containing any "+ biggest_plural;
   }
   if ( how_many == 1 ) {
      return "about one "+ biggest_size;
   }
   if ( !exact  &&  how_many > query_max_seen() ) {
      return "more "+ biggest_plural +" than you can count";
   }
   return "about "+ query_num( how_many ) +" "+ biggest_plural;
} /* amount_size() */

/**
 * This method returns a string which tells us how much stuff we
 * have.
 * @return string representing the amount of stuff we have
 */
string amount_size_short() {
   int biggest_amount;
   int how_many;
   string size;
   string biggest_size;
   string ret;
   string biggest_plural;

   if ( !amount || !sizeof( amount_types ) ||
        (query_property("determinate") == "some ") ) {
     if(collective)
       return query_num(amount, 20) + " ";
     return "";
   }
   if (_short_name && _short_name[0] == amount) {
      return _short_name[1];
   }
   biggest_size = _sorted_amount_names[0];
   foreach (size in _sorted_amount_names) {
     if (amount < amount_types[size][0]) {
       break;
     }
     biggest_size = size;
   }
   biggest_amount = amount_types[biggest_size][0];
   biggest_plural = amount_types[biggest_size][1];
   how_many = ( amount + biggest_amount / 2 ) / biggest_amount;

   if(!how_many) {
     if(collective)
       ret = query_num(amount) + " ";
     else
       ret = "a smidgeon of ";
   } else if(collective && amount < biggest_amount) {
     ret = query_num(amount) + " ";
   } else if(how_many == 1) {
     ret = "one "+ biggest_size + " of ";
   } else {
     ret = query_num( how_many, query_max_seen()) +" "+ biggest_plural +
       " of ";
   }
   _short_name = ({ amount, ret });
   return ret;
} /* amount_size_short() */

/**
 * This method returns a string describing how big the pile is.
 * @return the size of this pile of stuff
 */
string pile_size() {
   int i;

   if ( !sizeof( pile_types ) ) {
      return "a surreal "+ pile_name;
   }
   for ( i = 0; i < ( sizeof( pile_types ) - 1 ); i += 2 ) {
      if ( amount <= pile_types[ i ] ) {
         return add_a( pile_types[ i + 1 ] ) +" "+ pile_name;
      }
   }
   return add_a( pile_types[ sizeof( pile_types ) - 1 ] ) +" "+ pile_name;
} /* pile_size() */

/** @ignore yes */
varargs string pretty_short( object thing ) {
  if(collective) {
    if(amount > 1)
      return pretty_plural(thing);
    else
      return add_a(::pretty_short(thing));
  }
  return amount_size_short() + ::pretty_short( thing );
} /* pretty_short() */

/** @ignore yes */
varargs string pretty_plural( object thing ) {
  if(amount > 1)
    return amount_size_short() + ::pretty_plural( thing );
  return ::pretty_plural(thing);
}

#ifdef 0
/** @ignore yes */
string short(int dark) {
  string ret;

  ret = ::short( dark );

  if(!ret || ret == "something" || !collective)
    return(ret);

  if(amount == 1)
    return add_a(ret);

  return( query_num(amount, 9999 ) + " " + query_main_plural());
}
#endif

/** @ignore yes */
string long(string words, int dark ) {
  int i;
  string ret;
  string *bits;

  if(continuous)
    return replace( ::long( words, dark ),
                    ({ "$amount_size$", amount_size(),
                         "$pile_size$", pile_size() }) );

  ret = replace( ::long( words, dark ), "$N", short( dark ) );

  if(collective && amount > 1) {
    ret = replace( ret, ",$", "$comma_dollar$" );
    ret = replace( ret, ({ ",", " $spaced_comma$", ";", " $spaced_semi$",
        ":", " $spaced_colon$", ".", " $spaced_period$" }) ); 
    ret = replace( ret, "$comma_dollar$", ",$");
    bits = explode(ret, " ");
    for(i = 0; i < sizeof(bits); i++) {
        switch( bits[ i ] ) {
        case "This":
          bits[i] = "These";
          break;
        case "this":
          bits[i] = "these";
          break;
        case "is":
          bits[i] = "are";
          break;
        case "a":
        case "an":
          bits[i] = amount_size_short();
          bits[i] = bits[i][0..<2];
          break;
        case "it":
           bits[i] = "they";
           break;
        case "It":          
           bits[i] = "They";
           break;
        case "its":
           bits[i] = "their";
           break;
        case "Its":
           bits[i] = "Their";
           break;
        case "$a":
        case "$an":
           bits[i] = bits[i][1..];
           break;
        default:
           // handling verbs
           if( bits[i][0..1] == "$V" ) {
              bits[i] = replace_string( bits[i], "$V", "" );
              bits[i] = explode( bits[i], "$" )[1];
           };
           // handling manual pluralization
           if( bits[i][0..1] == "$P" ) {
              bits[i] = replace_string( bits[i], "$P", "" );
              bits[i] = pluralize( bits[i] );
           };
           // handling disappearing "a"s
           if( bits[i][0..1] == "$A" ) {
              bits[i] = replace_string( bits[i], "$A", "" );
              bits[i] = pluralize( bits[i] );
           };
           // was there before
           if(strsrch(bits[i], query_plural(dark)) == -1)
               bits[i] = replace(bits[i], short(dark), query_plural(dark));
        }
      }
    ret = implode(bits, " ");
    ret = replace(ret, ({ " $spaced_comma$", ",", " $spaced_semi$", ";",
        " $spaced_colon$", ":", " $spaced_period$", "." })  ); 
  // added for removing tags
  } else if( collective && amount == 1 ) {
     bits = explode(ret, " ");
     for(i = 0; i < sizeof(bits); i++) {
        if( bits[i][0..1] == "$V" ) {
           bits[i] = replace_string( bits[i], "$V", "" );
           bits[i] = explode( bits[i], "$" )[0];
        };
        if( bits[i][0..1] == "$P" ) {
           bits[i] = replace_string( bits[i], "$P", "" );
        };
        if( bits[i] == "$a" || bits[i] == "$an" ) {
           bits[i] = bits[i][1..];
        };
        if( bits[i][0..1] == "$A" ) {
           bits[i] = replace_string( bits[i], "$A", "" );
           bits[i] = add_a( bits[i] );
        };
     };
     ret = implode( bits, " " );
  };
  return ret;
}

/** @ignore yes */
string query_long(string words, int dark)  {
   return long(words, dark);
}

object new_collective(int number) {
  object ob;
  object place;

  ob = make_medium_clone(number);
  adjust_amount(-number);
  ob->set_no_join();
  if(environment()) {
    place = (object)environment()->query_property( "medium dest" );
    if(!place)
      place = environment();
    ob->move(place);
  }
  ob->reset_no_join();
  ob->call_out("merge", 1);
  return ob;
}

/*
 * This is a separate function so that things that inherit this can
 * redefine it to meet their requirements.
 */
int merge_criteria(object ob) {
  return medium_alias == ob->query_medium_alias();
}

void merge() {
  object *obs;

  if(environment())
     obs = filter(all_inventory(environment()) - ({ this_object() }),
                  "merge_criteria", this_object());

  if(sizeof(obs) && objectp(obs[0])) {
    no_join = 1;
    move("/room/rubbish");
    obs[0]->adjust_amount(amount);
  }
  return;
}

/** @ignore yes */
varargs int move( mixed dest, string messin, string messout ) {
  int check;

  if((check = ::move(dest, messin, messout)) != MOVE_OK)
    return check;

  if(no_join || !medium_alias || (!continuous && !collective))
    return MOVE_OK;

  call_out("merge", 0);
  return MOVE_OK;
}

/**
 * This method makes a duplicate of ourselves at a different size.
 * @param number the new size of the object
 * @return the new object
 */
object make_medium_clone( int number ) {
   object medium;
   mapping map;
   
   medium = clone_object( explode( file_name( this_object() ), "#" )[ 0 ] );
   map = (mapping)this_object()->query_dynamic_auto_load();
   medium->init_dynamic_arg( map );
   map = (mapping)this_object()->query_static_auto_load();
   if ( map ) {
      medium->init_static_arg( map );
   }

   medium->set_amount( number );

   //
   // Scale the enchantmant.
   //
   medium->set_enchant( query_enchant() * medium->query_max_enchant() /
                        query_max_enchant() );
   return medium;
} /* make_medium_clone() */


/** @ignore yes */
object cont_query_parse_id( mixed *args ) {
   int i;
   string *sizes;
   object medium, place;

   if ( args[ P_THING ] < 0 ) {
      args[ P_THING ]++;
      if ( args[ P_THING ] ) {
         return 0;
      }
      args[ P_THING ] = -99999;
      return this_object();
   }
   sizes = m_indices( amount_types );
   if ( args[ P_THING ] == 1 ) {
      for ( i = 0; i < sizeof( sizes ); i++ ) {
         if ( member_array( sizes[ i ], explode( args[ P_STR ], " " ) ) !=
               -1 ) {
            if ( amount_types[ sizes[ i ] ][ 0 ] > amount ) {
               return 0;
            }
            medium = make_medium_clone( amount_types[ sizes[ i ] ][ 0 ] );
            place = (object)environment()->query_property( "medium dest" );
            if ( !place ) {
               place = environment();
            } else {
               remove_call_out( "no_env_move" );
               call_out( "dest_old_env", 1, environment() );
            }
            adjust_amount( -amount_types[ sizes[ i ] ][ 0 ] );
            this_object()->set_no_env_move( place );
            medium->move( place );
            args[ P_THING ] = -99999;
            return medium;
         }
      }
      args[ P_THING ] = -99999;
      return this_object();
   }
   for ( i = 0; i < sizeof( sizes ); i++ ) {
      if ( member_array( amount_types[ sizes[ i ] ][ 1 ],
            explode( args[ P_STR ], " " ) ) != -1 ) {
         if ( amount_types[ sizes[ i ] ][ 0 ] * args[ P_THING ] > amount ) {
               return 0;
         }
         medium = make_medium_clone( amount_types[ sizes[ i ] ][ 0 ] *
                args[ P_THING ] );
         place = (object)environment()->query_property( "medium dest" );
         if ( !place ) {
            place = environment();
         } else {
            remove_call_out( "no_env_move" );
            call_out( "dest_old_env", 1, environment() );
         }
         adjust_amount( -amount_types[ sizes[ i ] ][ 0 ] * args[ 0 ] );
         this_object()->set_no_env_move( place );
         medium->move( place );
         args[ P_THING ] = -99999;
         return medium;
      }
   }
   return 0;
}

/** @ignore yes */
object coll_query_parse_id( mixed *arr ) {
   string *bits;

   if((arr[0] < 0)) {
     return 0;
   }
   if(arr[0] == 0) {
     bits = explode( arr[ 1 ], " " );
     if(sizeof(bits) == 1) {
       if(bits[0] == query_main_plural()) {
         return this_object();
       }
     }
     if(member_array(bits[sizeof(bits) - 1 ], query_plurals()) == -1) {
       return this_object();
     }
     return 0; /// ????
   }

   if(id(arr[1])) {
     return new_collective(amount - arr[0]);
   }
}

object query_parse_id( mixed *args ) {
  if(continuous)
    return cont_query_parse_id(args);

  return coll_query_parse_id(args);
}

/** @ignore yes */
mixed* cont_parse_match_object(string* input, object player,
                               class obj_match_context context) {
   int result;
   string size;
   mixed* amt;
   int make_amt;
   int smallest;
   int num;
   object medium;
   object place;

   result = ::is_matching_object(input, player, context);
   if (!continuous || !result) {
      if (result) {
         if (update_parse_match_context(context, 1, result)) {
            return ({ result, ({ this_object() }) });
         }
      }
      return 0;
   }


   smallest = amount;
   foreach (size, amt in amount_types) {
      if ( member_array( size, input) != -1) {
         if ( amt[ 0 ] > amount ) {
            return 0;
         }
         make_amt = amt[0];
      } else if ( member_array( amt[1], input ) != -1 ) {
         if ( amt[ 0 ] > amount ) {
            return 0;
         }
         make_amt = amt[0];
      }
      if (smallest > amt[0]) {
         smallest = amt[0];
      }
   }

   if (!make_amt) {
     if(!amount)
       make_amt = 1;
     else
       make_amt = amount;
   }

   num = update_parse_match_context(context, (amount / make_amt), result);
   if (!num) {
      return 0;
   }

   make_amt *= num;

   if (context->fraction) {
      make_amt = (make_amt * context->fraction[0]) / context->fraction[1];
   }

   if (make_amt < smallest) {
      make_amt = smallest;
   }

   if (make_amt < amount) {
      medium = make_medium_clone( make_amt );
      place = (object)environment()->query_property( "medium dest" );
      if ( !place ) {
         place = environment();
      } else {
         remove_call_out( "no_env_move" );
         call_out( "dest_old_env", 1, environment() );
      }
      adjust_amount( -make_amt );
      this_object()->set_no_env_move( place );
      medium->move( place );
      return ({ result, ({ medium }) });
   }
   return ({ result, ({ this_object() }) });
}

/** @ignore yes */
mixed* coll_parse_match_object(string* input, object viewer,
                               class obj_match_context context) {
  int ret;
  int num;
  int make_amt;
  string size;
  mixed *amt;
  int smallest;

  ret = ::is_matching_object(input, viewer, context);
  if (!ret) {
    return 0;
  }

  if (sizeof(input) == 1 &&
      !context->number_included &&
      !context->ordinal &&
      (member_array(input[<1], query_plurals()) != -1 ||
       input[<1] == "all" || input[<1] == "thing")) {
    return ({ OBJ_PARSER_MATCH_PLURAL, ({ this_object() }) });
  }

  if(amount_types) {
    smallest = amount;
    foreach (size, amt in amount_types) {
      if(member_array( size, input) != -1) {
        if(amt[0] > amount) {
          return 0;
        }
        make_amt = amt[0];
        ret = OBJ_PARSER_MATCH_SINGULAR;
      } else if(member_array( amt[1], input ) != -1) {
        if(amt[ 0 ] > amount) {
          return 0;
        }
        make_amt = amt[0];
        ret = OBJ_PARSER_MATCH_PLURAL;
      }
      if (smallest > amt[0]) {
        smallest = amt[0];
      }
    }
  }
  if(!make_amt)
    make_amt = 1;

  num = update_parse_match_context(context, to_int(amount / make_amt), ret);

  if (!num)
    return 0;

  num *= make_amt;

  if (context->fraction) {
    num = (num  * context->fraction[0]) / context->fraction[1];
  }

  if(num < amount) {
    new_collective(amount - num);
    return ({ ret, ({ this_object() }) });
  }
  return ({ ret, ({ this_object() }) });
}

/** @ignore yes */
mixed *parse_match_object(string *input, object player,
                          class obj_match_context context) {
  if(continuous)
    return cont_parse_match_object(input, player, context);

  return coll_parse_match_object(input, player, context);
}


/**
 * This method is for dealing with the object when bits are shaved off.
 * If it is split up by someone referencing it we are moved into a
 * special environment so we do not interfer with the old pile.  The
 * property "medium dest" will be used as the place to go, otherwise
 * the environment will be used.  This method sticks it into a special
 * on the fly created container.
 * @param dest the place to move too  (this will be the "medium dest" or
 * the environment()
 */
void set_no_env_move( object dest ) {
   object place;

   if ( file_name( environment() ) == "/room/rubbish" ) {
      return;
   }
   place = clone_object( "/std/container" );
   place->add_property( "medium dest", dest );
   move( place );
   call_out( "no_env_move", 0, dest, place );
} /* set_no_env_move() */

/**
 * This method checks to see if we are still in our silly little container
 * and if we are, moves us back.  We always dest the old container
 * @param dest where we are going to
 * @param place the temporary container
 */
void no_env_move( object dest, object place ) {
   if ( environment() == place ) {
      move( dest );
   }
   call_out( "dest_old_env", 1, place );
} /* no_env_move() */

/**
 * This method destructs the temporary container.
 * @param place the object to destroy
 */
void dest_old_env( object place ) {
   if ( place ) {
      place->dest_me();
   }
} /* dest_old_env() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
   return ([
      "::": ::query_dynamic_auto_load(),
      "continuous": continuous,
      "collective": collective,
      "amount": amount,
   ]);
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_dynamic_arg( mapping map, object ) {
  if ( !mapp( map ) )
    return;
  if ( map[ "::" ] )
    ::init_dynamic_arg( map[ "::" ]);
  if ( map[ "amount" ] )
    set_amount( map[ "amount" ] );
  /* This ensures that volumes are set properly in vessels */
  if ( !undefinedp( map[ "continuous" ] ) )
    if ( map[ "continuous" ] ) {
      set_continuous();
    } else {
      reset_continuous();
    }
  if(!undefinedp(map["collective"]))
    collective = map["collective"];
}

/** @ignore yes */
mapping int_query_static_auto_load() {
   return ([
      "::": ::int_query_static_auto_load(),
      "weight_unit": weight_unit,
      "medium_alias": medium_alias,
      "pile_name": pile_name,
      "amount_types": amount_types,
      "pile_types": pile_types,
      "value_scale": value_scale,
   ]);
}

/** @ignore yes */
mapping query_static_auto_load() {
  if ( base_name(this_object()) + ".c" == __FILE__ )
      return int_query_static_auto_load();
   return 0;
} /* query_static_auto_load() */

/** @ignore yes */
void init_static_arg( mapping map ) {
   if ( !mapp( map ) )
      return;
   if ( map[ "::" ] )
      ::init_static_arg( map[ "::" ] );
   if ( !undefinedp( map[ "medium_alias" ] ) )
      medium_alias = map[ "medium_alias" ];
   if ( pointerp( map[ "weight_unit" ] ) )
      weight_unit = map[ "weight_unit" ];
   if ( stringp( map[ "pile_name" ] ) )
      pile_name = map[ "pile_name" ];
   if ( mapp( map[ "amount_types" ] ) )
      set_amount_types(map[ "amount_types" ]);
   if ( pointerp( map[ "pile_types" ] ) )
      pile_types = map[ "pile_types" ];
   if ( floatp( map[ "value_scale" ] ) )
      value_scale = map[ "value_scale" ];
} /* init_static_arg() */

/** @ignore yes */
mixed *stats() {
   return ::stats() + ({
      ({ "amount", amount }),
      ({ "medium alias", medium_alias }),
      ({ "continuous", continuous }),
      ({ "collective", collective }),
      ({ "value scale", value_scale }),
   });
} /* stats() */
