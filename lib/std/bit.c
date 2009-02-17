/**
 * The bit object is the object created to handle bits of corpses, like
 * fingers and eyes and so on.
 * @author Ember
 */
#include <bit.h>
#include <corpse.h>

inherit "std/object";

#define DECAY_TIME 60

mixed *bit_data, *bits;
mixed race_ob;
int corpse_weight;
string race_name, *bits_gone = ({ });
int decay, cured;

void set_bits();
object make_bit(string which_bit);
object *make_bits(string *what_bits);
string *query_bits_left();
string *query_possible_bits(string);

void create() {
  ::create();

  set_short("anonymous bit");
  set_long("This is an unknown bit of some creature.\n");
  corpse_weight = STD_CORPSE_WEIGHT;
  set_weight(5);
  set_name("bit");
  add_property( "corpse bit", 1 );
  add_property("cureable", 1);
} /* create() */

void init() {
  add_command("eat", "<direct:object>");
} /* init() */


/**
 * This method checks to see if the bit is edible or not.
 * @return 1 if it is edible, 0 if not
 */
int query_edible() {
  string bit;
  
  if(!race_ob->query_eat(bit_data[BIT_NAME])) {
    return 0;
  }

  foreach(bit in query_possible_bits(0) - bits_gone) {
    if(!race_ob->query_eat(bit)) {
      return 0;
    }
  }
  return 1;
} /* query_edible() */

/** 
 * The main eat entry point.
 * @return 1 on success, 0 on failure
 */
int do_eat() {
  if(!query_edible())
    return 0;
  move( "/room/rubbish" );
  return 1;
} /* do_eat() */

/**
 * THis method checks to see if the bit can inherently not decay.
 * Like trolls teeth and other such things.
 * @return 1 if it cannot decay, 0 if it can
 */
int no_decay() {
  if( race_ob && bit_data ) {
    return race_ob->query_unrottable(bit_data[BIT_NAME]);
  }
} /* no_decay() */

/**
 * This method sets the race object associated with the corpse.
 * @param s the race object to set
 */
void set_race_ob(object s) {
  race_ob = s;
} /* set_race_ob() */

/**
 * This method sets the name of the race associated with the corpse.
 * @param s the race name to set
 */
void set_race_name(string s) {
  race_name = s;
} /* set_race_name() */

/**
 * This method setsup the long and short based on the current decay
 * levels of the corpse.
 */
void setup_long() {
  if ( !bit_data ) {
    return;
  }
  if (no_decay()) {
    if (race_name) {
      set_short( race_name +" "+ bit_data[ BIT_NAME ] );
      set_main_plural( race_name +" "+ pluralize( bit_data[ BIT_NAME ] ) );
      set_long("This is the " + (bit_data[BIT_NAME]) + " of " +
      add_a(race_name) + ".\n");
    } else {
      set_short( bit_data[ BIT_NAME ] );
      set_main_plural( pluralize( bit_data[ BIT_NAME ] ) );
      set_long("This is the " + (bit_data[BIT_NAME]) +
               " of an unknown creature.\n");
    }
  } else if (decay > 80) {
    if (race_name) {
      set_short( race_name +" "+ bit_data[ BIT_NAME ] );
      set_main_plural( race_name +" "+ pluralize( bit_data[ BIT_NAME ] ) );
      set_long("This is a fresh " + (bit_data[BIT_NAME]) + 
               " severed from the corpse of " + add_a(race_name) + ".\n");
    } else {      set_short( bit_data[ BIT_NAME ] );
      set_main_plural( pluralize( bit_data[ BIT_NAME ] ) );
      set_long("This is a fresh " + (bit_data[BIT_NAME]) +
               " severed from the corpse of an unknown creature.\n");
    }
  } else if (decay > 50) {
    if (race_name) {
      set_short( race_name +" "+ bit_data[ BIT_NAME ] );
      set_main_plural( race_name +" "+ pluralize( bit_data[ BIT_NAME ] ) );
      set_long("This is " + add_a(bit_data[BIT_NAME]) + " severed from the "
               "corpse of " + add_a(race_name) + ".\n");
    } else {
      set_short( bit_data[ BIT_NAME ] );
      set_main_plural( pluralize( bit_data[ BIT_NAME ] ) );
      set_long("This is " + add_a(bit_data[BIT_NAME]) + " severed from the "
               "corpse of an unknown creature.\n");
    }
  } else if (decay > 30) {
    if (race_name) {
      set_short( race_name +" "+ bit_data[ BIT_NAME ] );
      set_main_plural( race_name +" "+ pluralize( bit_data[ BIT_NAME ] ) );
      set_long("This is the partially decayed remains of " +
               add_a(bit_data[BIT_NAME]) + " severed from the "
               "corpse of " + add_a(race_name) + ".\n");
    } else {
      set_short( bit_data[ BIT_NAME ] );
      set_main_plural( pluralize( bit_data[ BIT_NAME ] ) );
      set_long("This is the partially decayed remains of " +
      add_a(bit_data[BIT_NAME]) + " severed from the "
      "corpse of an unknown creature.\n");
    }
  } else {
    if (race_name) {
      set_short( race_name +" "+ bit_data[ BIT_NAME ] );
      set_main_plural( race_name +" "+ pluralize( bit_data[ BIT_NAME ] ) );
      set_long("This is the almost unrecognizable remains of " +
               add_a(bit_data[BIT_NAME]) + " severed from the "
               "corpse of " + add_a(race_name) + ".\n");
    } else {
      set_short( bit_data[ BIT_NAME ] );
      set_main_plural( pluralize( bit_data[ BIT_NAME ] ) );
      set_long("This is the almost unrecognizable remains of " +
               add_a(bit_data[BIT_NAME]) + " severed from the " +
               "corpse of an unknown creature.\n");
    }
  }
  add_extra_look(this_object());
  
  if ( cured ) {
    set_long( query_long() +"It seems to have been pickled.\n" );
  }
} /* setup_long() */

string extra_look() {
  if(sizeof(bits_gone))
    return "It appears to be missing the " + query_multiple_short(bits_gone) + ".\n";
  return "";
}

/**
 * This method sets the weight of the bit.
 * @param i the weight of the bit
 */
void set_corpse_weight(int i) { corpse_weight = i; }

/**
 * This method sets the bit that this actually is.
 * @param s the name of the bit
 * @param dec the current decay numbe
 */
void set_bit(string s, int dec) {
  int temp;

  bit_data = (mixed *)race_ob->query_bit(s);
  if (!bit_data || !sizeof(bit_data)) {
    return;
  }
  if (race_name) {
    add_adjective(race_name);
  } else {
    add_adjective("unknown");
  }
  add_adjective( explode( s, " " ) );
  if ( pointerp( bit_data[ BIT_EXTRA ][ EXTRA_VALUE ] ) &&
       ( sizeof( bit_data[ BIT_EXTRA ][ EXTRA_VALUE ] ) > 3 ) ) {
    set_value(bit_data[BIT_EXTRA][EXTRA_VALUE][3]);
  }
  add_alias( bit_data[ BIT_NAME ] );
  add_plural( pluralize( bit_data[ BIT_NAME ] ) );
  if ( bit_data[ BIT_ALIAS ] && strlen( bit_data[ BIT_ALIAS ] ) ) {
    add_alias(bit_data[BIT_ALIAS]);
    add_plural( pluralize( bit_data[ BIT_ALIAS ] ) );
  }
  if ((temp=bit_data[BIT_EXTRA][EXTRA_WEIGHT] * corpse_weight
             / STD_CORPSE_WEIGHT) > 0) {
    set_weight(temp);
  } else {
    set_weight(1);
  }
  if(dec) {
    decay = dec;
  } else {
    decay = 100;
  }
  if (!no_decay()) {
      BIT_CONTROLLER->add_bit(this_object());
  }
  setup_long();
  set_bits();
  return;
} /* set_bit() */

/**
 * This method is called by the bits controller to do the actual
 * decaying of the object.
 * @return 1 if it still exists, 0 if it has finished decaying
 */
int do_decay() {
  int rate;

  if (!environment()) {
    move( "/room/rubbish" );
    return 0;
  }

  rate = 5 + (int)environment()->query_property( "decay rate" );
  if ( rate > 0 ) {
    decay -= rate;
  }
  if (decay < 0) {
    tell_object(environment(),
                capitalize(the_short()) + " decays to dust.\n");
    move( "/room/rubbish" );
    return 0;
  }
  if (decay == 80 || decay == 50 || decay == 30) {
    setup_long();
  }
  return 1;
} /* do_decay() */

/**
 * This method returns the race object associated with the bit.
 * @return the race object
 */
object query_race_ob() { return race_ob; }

/**
 * This method returns the name of the race associated with this
 * bit.
 * @return the name of the race
 */
string query_race_name() { return race_name; }

/**
 * This method returns the bit data for the bit.  The return array is of
 * the format same format as used in the races.
 * @return the bit data
 * @see /std/races/basic.c
 */
mixed query_bit_data() { return bit_data; }

/**
 * This method returns the current decay level of the corpse
 * @return the current decay level
 */
int query_decay() { return decay; }

/** @ignore yes */
void dest_me() {
  BIT_CONTROLLER->remove_bit(this_object());
  ::dest_me();
} /* dest_me() */

/** @ignore yes */
mixed query_static_auto_load() {
  return int_query_static_auto_load();
} /* query_static_auto_load() */

/** @ignore yes */
mixed query_dynamic_auto_load() {
  return ({ bit_data, race_ob, corpse_weight, race_name,
              decay, cured, bits_gone, ::query_dynamic_auto_load() });
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_dynamic_arg(mixed arg, object) {
  if (mapp(arg)) {
    ::init_dynamic_arg(arg);
    return ;
  }
  bit_data = arg[0];
  race_ob = arg[1];
  corpse_weight = arg[2];
  race_name = arg[3];
  decay = arg[4];
  cured = arg[5];
  bits_gone = arg[6];
  ::init_dynamic_arg(arg[7]);
  
  if ((!cured) && (!no_decay())) {
    BIT_CONTROLLER->add_bit(this_object());
  }
  setup_long();
  if(sizeof(bit_data))
    set_bits();
} /* init_dynamic_arg() */

/**
 * This method cures the corpse.  A cured bit no longer decays.
 */
void do_cure() {
  if ( cured ) {
    return;
  }
  cured = 1;
  BIT_CONTROLLER->remove_bit( this_object() );
  set_long( query_long() +"It seems to have been pickled.\n" );
  set_short( "cured " + query_short());
  set_main_plural( "cured " + query_main_plural() );
  add_adjective("cured");
} /* do_cure() */

/**
 * This returns the cured state of the bit.
 * @return 1 if it is cured, 0 if not
 */
int query_cured() { return cured; }

/**
 * This method returns the vector used with the potion space.  I 
 * think this call is now obsolete and not used.
 * @return the vectorof the bit
 */
int *query_vect() { return bit_data[BIT_EXTRA][EXTRA_VALUE]; }

/**
 * This method returns the possible bits that can be removed
 * from this bit.  If the bits are restricted then the
 * possible bits are the ones that are contained by that bit.
 * @param word restrict the possible bits
 * @return the list of possible bits
 */
string *query_possible_bits( string word ) {
  int i, j;
  string *possibles;

  possibles = ({ });
  for ( i = sizeof( bits ) - 3; i > -1; i -= 3 ) {
    if ( ( bits[ i ] == word ) ||
         ( bits[ i + 1 ] == word ) ||
         !word) {
      if (arrayp( bits[i+2][2] )) {
        for ( j = 0; j < bits[ i + 2][2][1]; j++ ) {
          possibles += ({ bits[ i ] });
        }
      } else {
        possibles += ({ bits[ i ] });
      }
    }
  }
  return possibles;
} /* query_possible_bits() */

/**
 * This returns the plural of all the possible bit names.
 * If the bits are restricted then the
 * possible bits are the ones that are contained by that bit.
 * @param word restrict the possible bits
 * @return the array of possible bit names
 */
string *query_possible_plural_bits( string word ) {
  int i, j;
  string *possibles;

  possibles = ({ });
  for ( i = sizeof( bits ) - 3; i > -1; i -= 3 ) {
    if (((bits[i] && pluralize(bits[ i ]) == word )) ||
        ((bits[i+1] && pluralize(bits[ i + 1 ]) == word )) ||
        !word) {
      if (arrayp( bits[i+2][2] )) {
        for ( j = 0; j < bits[ i + 2][2][1]; j++ ) {
          possibles += ({ bits[ i ] });
        }
      } else {
        possibles += ({ bits[ i ] });
      }
    }
  }
  return possibles;
} /* query_possible_plural_bits() */

/**
 * This method figures out of the specified bit is left.
 * @return the name of the bit if it is left
 * @param s the bit to check if it is left
 */
string query_bit_left(string s) {
  string *poss_bits;

  poss_bits = query_possible_bits(s);
  if (!poss_bits || !sizeof(poss_bits)) {
     return 0;
  }

  poss_bits = poss_bits - bits_gone;

  if (!sizeof(poss_bits)) {
    return 0;
  }

  return poss_bits[0];
} /* query_bit_left() */

/**
 * This method figures out of the specified bit is left using the plural name.
 * @return the name of the bit if it is left
 * @param s the bit to check if it is left
 */
string *query_bit_left_pl(string s) {
  string *poss_bits;

  poss_bits = query_possible_plural_bits(s);
  if (!poss_bits || !sizeof(poss_bits)) {
    return 0;
  }

  poss_bits = poss_bits - bits_gone;
  if (!sizeof(poss_bits)) {
    return 0;
  }

  return poss_bits;
} /* query_bit_left() */

/** @ignore yes */
object *find_inv_match( string s, object looker ) {
  string bit, *bit_pl;
  object *weap, wep, tmp;
  int cut;

  bit = query_bit_left(s);
  bit_pl = query_bit_left_pl(s);

  if (!bit && !sizeof(bit_pl))
    return all_inventory();
  cut = 0;
  if(looker)
    weap = (object *)looker->query_weapons();
  if (sizeof(weap)) {
    foreach( wep in weap ) {
      if (wep->id("dagger") || wep->id("knife"))
        cut = 1;
    }
  }
  if (bit) {
    if (cut || (string)race_ob->query_pluckable(bit)) {
      tmp = make_bit(bit);
      return ({ tmp  });
    }
    if (sizeof(weap)) {
      tell_object( looker, "You can only cut things from "+a_short()+
                           " with a knife or dagger.\n" );
    } else {
      tell_object( looker, "You can't cut bits from "+a_short()+" with your "
                           "bare hands.\n" );
    }
    return ({ });
  }
  if (sizeof(bit_pl)) {
    if (cut) {
      if (sizeof(bit_pl) > 5) {
        return make_bits(bit_pl[0..4]);
      } else {
        return make_bits(bit_pl);
      }
    }
    foreach( bit in bit_pl ) {
      if (!((string)race_ob->query_pluckable(bit)))
        bit_pl -= ({ bit });
    }
    if (sizeof(bit_pl)) {
      if (sizeof(bit_pl) > 5) {
        return make_bits(bit_pl[0..4]);
      } else {
        return make_bits(bit_pl);
      }
    }
    if (sizeof(weap)) {
      tell_object( looker, "You can only cut things from "+a_short()+
                           " with a knife or dagger.\n" );
    } else {
      tell_object( looker, "You can't cut bits from "+a_short()+" with your "
                           "bare hands.\n" );
    }
    return ({ });
  }
}  /* find_inv_match() */

/**
 * This method creates a bit from the current bit.
 * @param which_bit the name of the bit to create
 * @return the object that is the new bit
 */
object make_bit(string which_bit) {
  mixed *bit;
  object bitobj;
  int i,j;

  bit = race_ob->query_bit(which_bit);
  if ((sizeof(bit[2][2]) > 1) && stringp(bit[2][2][1])) {
    bitobj = clone_object( bit[2][2] );
  } else if (bit[2][2] == 0) {
    bitobj = clone_object("/std/bit");
  } else {
    bitobj = clone_object("/std/bit");
  }
  bitobj->set_race_ob(race_ob);
  if (race_name) bitobj->set_race_name(race_name);
  else bitobj->set_race_name(race_ob->query_name());
  bitobj->set_corpse_weight(query_weight());
  if(!race_ob->query_eat(bit[BIT_NAME])) {
    bitobj->set_bit(bit[0], 0);
  } else {
    bitobj->set_bit(bit[0], (decay * 2) / 3);
  }

  for (i = 0; i < sizeof(bits_gone); i++) {
    j = member_array(bits_gone[i], bit[BIT_EXTRA]);
    if (j >= 0) {
      bitobj->add_bit_gone(bits_gone[i]);
    }
  }
  bits_gone += ({ bit[BIT_NAME] }) + bit[BIT_EXTRA][3..50];
  if (environment()) {
    bitobj->move(environment());
  }
  return bitobj;
} /* make_bit() */

/**
 * This method makes a bunch of bits at once.
 * @param what_bits the list of bits to create
 * @return the array of made bits
 */
object *make_bits(string *what_bits) {
  string bit;
  object *bit_pl = ({ });
  
  foreach(bit in what_bits) {
    bit_pl += ({ make_bit(bit) });
  }
  return bit_pl;
} /* make_bits() */

/**
 * This method returns the current array of bits gone from this
 * bit.
 * @return the list of missing bits
 */
string *query_bits_gone() { return bits_gone; }

mixed *add_bit_gone( string bit ) {
  string *poss_bits, tempbit;
  mixed *bit_details;
  int i;

  poss_bits = query_possible_bits( bit ) - bits_gone;
  if ( !sizeof( poss_bits ) ) return 0;
  bit_details = (mixed *)race_ob->query_bit( poss_bits[ 0 ] );
  bits_gone += ({ bit_details[ BIT_NAME ] });
  foreach (tempbit in bit_details[ BIT_EXTRA ][ 3 .. sizeof(bit_details[BIT_EXTRA]) ]) {
    if (arrayp(race_ob->query_bit( tempbit )[2][2]) && intp(race_ob->query_bit( tempbit )[2][2][1])) {
      for (i = 0; i < race_ob->query_bit( tempbit )[2][2][1]; i++) {
        bits_gone += ({ tempbit });
      }
    }
  }
  return bit_details;
} /* add_bit_gone() */

/**
 * This method sets the array of gone bits on the bit.
 * @param bits the set of bits gone to set
 */
void set_bits_gone( string *bits ) {
  int i;

  bits_gone = ({ });
  for ( i = 0; i < sizeof( bits ); i++ ) {
    add_bit_gone( bits[ i ] );
  }
} /* set_bits_gone() */

/**
 * This method returns the current array of bits left.
 * @return the array of bits left
 */
string *query_bits_left() {
  int i, j;
  string *all_bits;
  mixed *bit_pl;

  bit_pl = (mixed *)race_ob->query_bits();
  all_bits = ({ });
  for ( i = 0; i < sizeof( bit_pl ); i += 3 ) {
    if (arrayp( bit_pl[i+2][2] )) {
      for ( j = 0; j < bit_pl[ i + 2][2][1]; j++ ) {
        all_bits += ({ bit_pl[ i ] });
      }
    } else {
      all_bits += ({ bit_pl[ i ] });
    }
  }
  return all_bits - bits_gone;
} /* query_bits_left */

/**
 * This method sets all the basic bits for the bit when it is first created.
 */
void set_bits() {
  int i, j;
  mixed *these_bits, *all_bits;

  bits = ({ });

  these_bits = bit_data;
  all_bits = race_ob->query_bits();

  for (i=3; i<sizeof(these_bits[2]); i++) {
    for (j=0; j<sizeof(all_bits); j = j+3) {
      if ((these_bits[0] == all_bits[j+2][0]) && (these_bits[2][i] == all_bits[j])) {
        bits += all_bits[j..j+2];
        delete(all_bits, j, 2);
        break;
      }
    }
  }
} /* set_bits */

string query_determinate(object caller)  {
   return race_ob->query_determinate(caller);
}

string query_medium_alias() { 
  string temp;
  string *temp2 = ({ });

    if ( !sizeof( bit_data ) )
        return "";

    //Dodgy string munging stuff. OH YEAH!
    temp = bit_data[0];
    temp2 = explode( temp, " " );
    temp2 = map( temp2, (: capitalize( $1 ) :) );
    temp = implode( temp2, "" );

    return temp + "Of" + capitalize( race_ob->query_name() ) ;
}

int query_collective() {
  if(query_verb() == "get" || query_verb() == "take")
    return 1;
}
