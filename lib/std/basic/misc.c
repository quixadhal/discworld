/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: misc.c,v 1.16 2003/03/07 01:10:01 ceres Exp $
 * 
 */
/**
 * This class contains information relating to the value and the weight
 * of the object.
 * @author Pinkfish
 */
#include <money.h>
#include <move_failures.h>

#define WATCH_DEFINE "fluff'n'stuff"
#define PRICE_INDEX "/obj/handlers/cpi_handler"

inherit "/std/basic/light";
inherit "/std/basic/consecrate";
inherit "/std/basic/move";
/* this will also handle value... */

nosave int weight; // People have to ask explicitly for weightlessness
nosave int length = 1;
nosave int width = 1;                 
nosave int value;
nosave mapping value_info;
void check_log_status( object *parray );

void create() {
  weight = 1;
  light::create();
  
  if(this_object()->query_property(WATCH_DEFINE))
    call_out( "check_log_status", 1, previous_object(-1) );
  
  value_info = ([ ]);
}

/**
 * This method sets the width to the given amount.
 * @param w the amount to set the width to
 * @see query_width()
 */
void set_width(int w) { width = w; }

/**
 * This method queries the width of an object. The width should be the shorter
 * side of an object and the length its longer one. Sadly someone chose inches
 * as the size units. :(
 *
 * @param w the amount to set the width to
 * @see set_width()
 */
int query_width() { return width; }

/**
 * This method sets the length to the given amount. The length should be the
 * long side of an object, the width its shorter one. Sadly someone chose
 * inches as the size units. :(
 *
 * @param w the amount to set the length to.
 * @see query_length()
 */
void set_length(int l) { length = l; }

/**
 * This method queries the length of an object.
 * @param w the amount to set the length to
 * @see set_length()
 */
int query_length() { return length; }

/**
 * This method adjusts the weight by the given amount.
 * @param w the amount to change the weight by
 * @see set_weight()
 * @see query_weight()
 */
void adjust_weight(int w) {
    if (environment()) {
        environment()->add_weight(w);
    }
    weight += w;
} /* adjust_weight() */

/**
 * This method sets the weight of the object.  One weight unit
 * is 50 grams.
 * <p>
 * When setting the weights of weapons use the following guide:
 * <pre>
 * o===================o=====================o================o
 * | Weapon            | Approx. Weight (kg) |  Weight Units  |
 * o===================o=====================o================o
 * | Dagger            |        0.5          |       10       |
 * | War Hammer        |        1.1          |       22       |
 * | Mace              |        1.3          |       26       |
 * | Flail             |        1.5          |       30       |
 * | Pole Axe          |        2.3          |       46       |
 * | Short Sword       |        0.8          |       16       |
 * | Broad Sword       |        1.1          |       22       |
 * | Long Sword        |        1.4          |       28       |
 * | Bastard Sword     |        1.9          |       38       |
 * | Two-handed Sword  |        2.7          |       42       |
 * o===================o=====================o================o
 * </pre>
 * @param w the amount to change the weight by
 * @see adjust_weight()
 * @see query_weight()
 */
void set_weight(int w) {
    if (environment())
        environment()->add_weight(w-weight);
    weight = w;
} /* set_weight() */

/**
 * This method returns the current weight of the object.
 * @return the current weight of the object
 * @see set_weight()
 * @see adjust_weight()
 */
int query_weight() { return weight; }
/**
 * This method returns the current weight of the object. It does basicly
 * the same thing as the previous call.  NB: this_object()->query_weight()
 * is pretty much the same as query_weight(), except it takes shadows into
 * account.
 * @return the current weight of the object
 * @see set_weight()
 * @see adjust_weight()
 * @see query_weight()
 */
int query_complete_weight() { return (int)this_object()->query_weight(); }

/**
 * This method changes the value of the object by a certain number of
 * coins.  The coins can be any type.
 * @see /obj/handlers/money_handler->query_total_value()
 * @see adjust_value()
 * @see query_money_array()
 * @see set_value()
 * @return the new value
 */
varargs int adjust_money(mixed amt, string coin) {
    if (pointerp(amt)) {
        value += (int)MONEY_HAND->query_total_value(amt);
        if (value < 0) {
            value = 0;
        }
        return value;
    }
    amt = (int)MONEY_HAND->query_total_value( ({ coin, amt }) );
    value += amt;
    if (value < 0) {
        value = 0;
    }
    return value;
} /* adjust_money() */

/**
 * This method changes the current value of the object.
 * @param i the amount to change the value by
 * @return the new value of the object
 */
int adjust_value(int i) {
    value += i;
    return value;
} /* adjust_value() */

/**
 * This method returns the value of the object as a money array.  The
 * money array is a list oif coinages followed by a number of coins.
 * Eg: <code>({ "copper", 10, "silver", 12 }).</code>
 * @see /obj/handlers/money_handlers->create_money_array()
 * @see set_value()
 * @see adjust_value()
 * @see adjust_money()
 * @see query_value()
 */
mixed *query_money_array() { 
    return (mixed *)MONEY_HAND->create_money_array(value);
} /* query_money_array() */

/**
 * This method returns the number of coins of a certain
 * type that are in the value of the object.  This is not
 * a good estimate of value or anything, no idea why it is
 * here really.  I am sure I had a good reason
 * at the time :)
 * @param type the type of coin to look for
 * @return the number of coins of that type
 * @see /obj/handlers/money_handlers->create_money_array()
 * @see set_value()
 * @see adjust_value()
 * @see adjust money()
 * @see query_money_array()
 * @see query_value()
 */
int query_money(string type) { 
    int i;
    mixed *m_a;

    m_a = (mixed *)MONEY_HAND->create_money_array(value); 
    i = member_array(type, m_a);
    if (i == -1) {
        return 0;
    }
    return m_a[i+1];
} /* query_money() */

/**
 * This method sets the value of the object.  The actual value in
 * coins and so forth is worked out by the money handler based on the
 * value.
 * @param number the new value of the object
 * @see query_value()
 * @see adjust_value()
 * @see query_money_array()
 */
void set_value( int number ) { value = number; }

/**
 * This method sets the value information for a type of
 * special object.  This is the extra value information associated
 * with the "artifact", "enchantment" or "material" of the object.
 * @param word the type of value information
 * @param number the amount to set it to
 * @see query_value_at()
 * @see remove_value_info()
 * @see set_value()
 */
void set_value_info( string word, int number ) {
    value_info[ word ] = number;
} /* set_value_info */

/**
 * This method removes the value information for a type of
 * special object.  This is the extra value information associated
 * with the "artifact", "enchantment" or "material" of the object.
 * @param word the type of value information to remove
 * @see query_value_at()
 * @see set_value_info()
 * @see set_value()
 */
void remove_value_info( string word ) {
    if ( undefinedp( value_info[ word ] ) )
        return;
    value_info = m_delete( value_info, word );
} /* remove_value_info() */

/**
 * This method returns the current value of the object.
 * @return the current value of the object
 */
int query_value() { return value; }

/**
 * This method returns the vase value of the object.  This is before
 * any scaling from things like condition occurs.
 * @return the base value
 */
int query_base_value() {
   return value;
} /* query_base_value() */

/**
 * This method returns the information associated for all the special
 * bits of the object.  The value infor maping ihas keys which are
 * the type of value info and the value is the value of it.
 * @return the value info mapping
 * @see set_value_info()
 * @see query_value_at()
 * @see remove_value_info()
 */
mapping query_value_info() { return value_info; }

/**
 * This method figures out how much an object will cost in a certain
 * place.  For instance at a shop that does not handle artifcacts an
 * artifact will be bought and sold cheaply.  The current types
 * of "artifact", "enchantment" and "material" are recognised.
 * A property in the shop of the type "artifact valued" will
 * cause the values of that type to be taken into account.
 * @param place the object to find the value at
 * @return the value in the shop
 * @see query_value()
 * @see set_value_info()
 * @see adjust_value()
 */
int query_value_at( object place ) {
    int i, total;
    string *info;
    mixed how;

    if ( !value_info ) {
        value_info = ([ ]);
    }
    total = (int)this_object()->query_value();
    info = m_indices( value_info );
    for ( i = 0; i < sizeof( info ); i++ ) {
        how = (mixed)place->query_property( info[ i ] +" valued" );
        if ( how ) {
            switch ( info[ i ] ) {
            case "artifact" :
                total += ( value_info[ "artifact" ] * ( how[ 0 ] +
                    how[ 1 ] * (int)this_object()->query_charges() ) ) / how[ 2 ];
                break;
            case "enchantment" :
                if ( this_object()->query_max_enchant() )
                    total += ( value_info[ "enchantment" ] * how *
                      (int)this_object()->query_enchant() ) /
                    (int)this_object()->query_max_enchant();
                break;
            case "material" :
                total += (int)this_object()->query_weight() *
                (int)PRICE_INDEX->query_price( (string)this_object()->
                  query_material(), how );
                break;
            default :
                total += value_info[ info[ i ] ];
            }
        }
    }
    return total;
} /* query_value_at() */

/**
 * This method always returns the most expensive value of an item.
 * This should be used for things like theif quotas and anything which
 * requires the real actual value of an item.
 * @param place the place to find the values in
 */
int query_value_real(string place) {
   int i;
   int total;
   string *info;

   if ( !value_info ) {
      value_info = ([ ]);
   }
   total = (int)this_object()->query_value();
   info = m_indices( value_info );
   for ( i = 0; i < sizeof( info ); i++ ) {
      switch ( info[ i ] ) {
      case "artifact" :
         total += ( value_info[ "artifact" ] * ( 3 +
             1 * (int)this_object()->query_charges() ) ) / 1;
         break;
      case "enchantment" :
         if ( this_object()->query_max_enchant() )
             total += ( value_info[ "enchantment" ] * 10 *
               (int)this_object()->query_enchant() ) /
             (int)this_object()->query_max_enchant();
         break;
      case "material" :
         total += (int)this_object()->query_weight() *
         (int)PRICE_INDEX->query_price( (string)this_object()->
           query_material(), place );
         break;
      default :
         total += value_info[ info[ i ] ];
         break;
      }
   }
   return total;
} /* query_value_real() */

/** @ignore yes */
varargs int move( mixed dest, string messin, string messout ) {
  int i, w;
  object from;

  if( stringp(dest) ) {
   if ( strsrch( dest , "#") != -1 )
     dest = find_object(dest);
   else
     dest = load_object(dest);
  }
  if(!dest)
    return MOVE_INVALID_DEST;

  w = (int)this_object()->query_complete_weight();

  if(!(dest->add_weight(w)))
    return MOVE_TOO_HEAVY;

  from = environment();

  i = ::move(dest, messin, messout);

  if(i != MOVE_OK) {
    dest->add_weight(-w);
    return i;
  }

  if(from)
    from->add_weight(-w);
  
  if(query_light_needs_inform()) {
    if(from)
      inform_of_light_level_change(from);
    inform_of_light_level_change(dest);
  }
  
  return i;
} 

/** @ignore yes */
void dest_me() {
    if ( environment() ) {
        environment()->add_weight( -(int)this_object()->query_complete_weight() );
        if ( query_my_light() )
            adjust_light( -query_my_light() );
    }
    ::dest_me(); 
} /* dest_me() */

/** @ignore yes */
mixed *stats() {
    int i;
    string *info;
    mixed *guff;

    guff = ({ ({ "value", value }) });
    if ( !value_info )
        value_info = ([ ]);
    info = m_indices( value_info );
    for ( i = 0; i < sizeof( info ); i++ )
        guff += ({ ({ "(info) "+ info[ i ], value_info[ info[ i ] ] }) });
    return light::stats() + consecrate::stats() + guff;
} /* stats() */

/** @ignore yes */
// Monitor for people passing around items that they shouldn't be.
void check_log_status( object *parray ) {
  if ( this_object()->query_property( WATCH_DEFINE ) && 
       clonep( this_object() ) &&
       arrayp( parray ) && 
       member_array(load_object("/secure/cmds/creator/cl_one"), parray) != -1){
    log_file( "WATCH", "%s TO: %s. TP: %s.\n", ctime(time()),
              base_name( this_object() ), parray[2]->query_name() );
    this_object()->add_property( "CT", time() );
    user_event( "inform", sprintf( "[Restricted] %s cloned %s.",
                                   parray[2]->query_cap_name(), 
                                   base_name( this_object() ) ), "cheat" );
  }
}

 
