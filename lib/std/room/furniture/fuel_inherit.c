/**
 *  You need a setup() in the inheriting fuel item.
 *  From here set_name for you fuel and whatever else you want.
 *  Also make sure that you have set a weight for the item as this is
 *  what determines how long it burns for.
 *
 * -> NOTE: <-
 *  You set the description for the fuel by using set_shorts and set_longs
 *  (with an s on the end) The argument to these functions must be an ARRAY.
 *  The first element is the normal description of the fuel, when it hasn't 
 *  been burnt. The rest describe the fuel after it has been burnt a little, 
 *  then a bit more...until finally its burnt out. There can be as 
 *  many elements in this description array as you want.
 *  See /obj/furnitures/misc/log.c for an example of some fuel.
 *  @author Aquilo
 */

inherit "/std/object";

private nosave int _amount; // The percentage amount of burnable material left
            // 0 - The log has been burnt to a crisp, no good as fuel anymore
            // 100 - All its weight is available to be burnt

private nosave string *_shorts;
private nosave string *_longs;

string fuel_short();
string fuel_long();

void create(){
   ::create();

   if(!_amount){
     _amount = 100;
   }
   set_short( (: fuel_short :) );
   set_long( (: fuel_long :) );
   add_property("no recycling", 1);
   add_property("fuel", 1);
} /* create() */

/**
 * This method sets the amount of fuel left.
 * @param i the amount of fuel left
 */
void set_amount_left(int i){  _amount = i;  }

/**
 * Thie method changes the amount of fuel left.
 * @param i amount to adjust the fuel by
 */
void adjust_amount_left(int i){  _amount += i;  }

/**
 * This method returns the amount of fuel left.
 * @return the amount of fuel left
 */
int query_amount_left(){  return _amount;  }

/**
 * This method sets the array of shorts to use for the various amounts
 * of the fuel left.  The number of shorts must match the number of
 * descriptions.
 * @param desc the shorts for the fuel left
 */
void set_shorts(string *desc){  _shorts = desc;  }

/**
 * This method sets the array of descriptions to use for the various amounts
 * of the fuel left.  The number of shorts must match the number of
 * descriptions.
 * @param desc the descriptions for the fuel left
 */
void set_longs(string *desc){  _longs = desc;  }

/**
 * This method returns the short description of the fuel.  It looks
 * of the description properly from the array of shorts.
 * @return the current short of the fuel
 */
string fuel_short(){
   int i;
   int x;

   x = sizeof(_shorts);
   for(i = 1; i <= x; i++){
      if( (_amount + 1) > 100 - ( i * 100 / x ) ){
         return _shorts[(i-1)];
      }
   }
   // Should never get here but return something just in case
   return "fuel";
} /* fuel_short() */

/**
 * This method returns the description of the fuel.  It looks
 * of the description properly from the array of longs.
 * @return the current long of the fuel
 */
string fuel_long(){
   int i;
   int x;

   x = sizeof(_longs);

   for(i = 1; i <= x; i++){
      if( ( _amount + 1 ) > ( 100 - ( i * 100 / x )) ){
         return _longs[(i-1)];
      }
   }
   // Should never get here but return something just in case
   return "fuel";
} /* fuel_long() */

/** @ignore yes */
mixed query_dynamic_auto_load() {
   return ([ "::" : ::query_dynamic_auto_load(),
             "amount" : _amount
          ]);
/** @ignore yes */
} /* query_dynamic_auto_load() */

void init_dynamic_arg(mapping arg) {
   _amount = arg[ "amount" ];
   ::init_dynamic_arg(arg["::"]);
} /* init_dynamic_arg() */
