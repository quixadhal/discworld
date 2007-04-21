/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: armour.c,v 1.23 2003/05/21 03:44:20 ceres Exp $
 * 
 */
/**
 * This file contains all the armour related code for the living
 * objects.
 * @author Pinkfish
 */
#include <clothing.h>
#include <function.h>
#include <race.h>
#include <armour.h>

inherit "/std/armour_logic";

#undef INFORM 1

nosave string skin;
nosave object *wearing;
nosave mixed stopped;
nosave mixed *_armour_callbacks;
nosave int *_cids;

void create() {
   skin = "morphological field";
   wearing = ({ });
   if(!_armour_callbacks)
     _armour_callbacks = ({ ({ }), ({ }), ({ }) });
   
   if(!_cids)
     _cids = ({ 0, 0, 0 });
   
   ::create();
} /* create() */

/**
 * Register a callback for magical protection.
 *
 * @param level How close the protection is to the skin.
 * @param callback The object & function to be called.
 * @param data Any extra data to be passed.
 * @return an ID to identify this callback.
 *
 * @example:
 * register_armour_callback(ARM_INNER,
 *                          ({ "/std/effects/religious/shield", "callback" }),
 *                          ({ "pishe" });
 *
 * The callback function will be called as:
 *
 * callback(player, type, amount, zone, ({ "pishe" }));
 *
 */
int register_armour_callback(int level, mixed callback, mixed data) {
  _armour_callbacks[level] += ({ new(class armour_callback,
                                     id : ++(_cids[level]),
                                     callback: callback,
                                     data : data) });
  return _cids[level];
}

/**
 * Remove a registered callback, used when the protection ends.
 *
 * @param level The level of the callback
 * @param id The callbacks ID.
 *
 * @see register_armour_callback()
 */
int remove_armour_callback(int level, int id) {
  int i;

  for(i=0; i<sizeof(_armour_callbacks[level]); i++) {
    if((_armour_callbacks[level][i])->id == id) {
      _armour_callbacks[level] -= ({ _armour_callbacks[level][i] });
      return 1;
    }
  }

  return 0;
}

/**
 * Update the data for an armour callback.
 *
 * @param level The level of the callback
 * @param id The callbacks ID.
 * @param data The new data.
 *
 * @see register_armour_callback()
 */
int update_armour_callback(int level, int id, mixed data) {
  int i;
  
  for(i=0; i<sizeof(_armour_callbacks[level]); i++) {
    if((_armour_callbacks[level][i])->id == id) {
      _armour_callbacks[level][i]->data = data;
      return 1;
    }
  }
  return 0;
}

/**
 * Query all the callback data. This should typically only be used for
 * debugging purposes.
 */
mixed *query_armour_callbacks() { return copy(_armour_callbacks); }

/**
 * This method returns the current skin of the living object.
 * @return the skin of the object
 * @see set_skin()
 */
string query_skin() { return skin; }

/**
 * This method sets the current skin of the living object.  This can
 * be done by the race object and by specific spells or effects.
 * @param word the new skin type
 * @see query_skin()
 */
void set_skin( string word ) { skin = word; }

/**
 * This method returns all the objects you are currently wearing.
 * @return the current array of worn objects
 * @return the array of worn stuff
 * @see query_armours()
 */
object *query_wearing() {
   wearing = filter(wearing, (: $1 && environment($1) == this_object() :));
   return copy( wearing );
} /* query_wearing() */

/**
 * This method returns all the armours that the is currently being
 * worn.  This will always return the same value as query_wearing()
 * @see query_wearing()
 * @return the array of armours
 */
object *query_armours() {
   return query_wearing();
} /* query_armours() */

/**
 * This method will make sure all the armours are unworn by the living
 * object.
 * @see query_wearing()
 */
void clear_armours() {
   wearing->set_worn_by( 0 );
   wearing = ({ });
} /* clear_armours() */

/**
 * This returns the object which stopped the blow.  This is only valid
 * inside and after a query_ac() call.
 * @return the object which stopped the call
 */
mixed query_stopped() {
   return stopped;
} /* query_stopped() */

/**
 * This method sets the object which stops the call.  This should be
 * used to do weird stuff.  I have no idea what it should be used for
 * at all and I am just rambling.
 * @param arg the new value of the stopped object
 */
void set_stopped( mixed arg ) { stopped = arg; }

/**
 * This method checks to see if the two armour types are
 * equivilant or not.
 * @param comp the first type
 * @param type the second type
 */
protected int equivalent_armour_types( mixed comp, string type ) {
   string word;

   if ( stringp( comp ) ) {
      if ( CLOTHING_HANDLER->query_equivilant_type( comp ) ) {
         return CLOTHING_HANDLER->query_equivilant_type( comp ) == type;
      }
      return comp == type;
   }
   foreach ( word in comp ) {
      if ( equivalent_armour_types( word, type ) )
         return 1;
   }
   return 0;
} /* equivalent_armour_types() */

/**
 * This is the method used to get the living object to wear a piece of
 * armour or clothing.  This is called from inside the armour or clothing
 * code itself.  It will call the function set_worn_by() on the
 * armour or clothing and if this returns a non-zero result then it
 * will add it into the current list of worn types.  Assuming the
 * type of the armour fits into the allowed list.  It will also
 * call the functon hold_item on the object if it is required to be
 * held as well (ie: shield).
 * @param armour the armour to wear
 * @param doing_hold if this is called by the hold command
 * @return the failure message, or 0 if ok
 * @see remove_armour()
 * @see query_armours()
 * @see query_wearing()
 */
string wear_armour( object armour, int doing_hold ) {
   int no_limbs;
   string type, word, fail_mess;
   mixed types;
   object thing;
   mapping things;

   if (environment(armour) != this_object()) {
      return "must be in your inventory";
   }

   wearing -= ({ 0 });
   if (member_array(armour, wearing) > -1)
     return "since you already have " + armour->query_objective() + " on";

   types = armour->query_type();
   word = this_object()->not_allowed_to_wear( types );
   if ( word ) {
      return word;
   }
   things = ([ ]);
   if ( stringp( types ) ) {
      types = ({ types });
   }
   if ( !pointerp( types ) ) {
      return "oh dear";
   }
   foreach ( type in types ) {
      if ( CLOTHING_HANDLER->query_equivilant_type( type ) ) {
         type = CLOTHING_HANDLER->query_equivilant_type( type );
      }
      things[ type ] = ({ });
      foreach ( thing in wearing ) {
         if ( equivalent_armour_types( (mixed)thing->query_type(), type ) )  {
            things[ type ] += ({ thing });
         }
      }
   }
   word = (string)this_object()->query_race_ob();
   
   if ( !word )
     return "no race object";
   
   foreach ( type in keys( things ) ) {
      if ( (int)word->query_number_worn( type ) <= sizeof( things[ type ] ) ) {
         return "since you are already wearing "+
               query_multiple_short( things[ type ] );
      }
   }
   no_limbs = (int)armour->query_no_limbs();
   if ( no_limbs && !doing_hold ) {
      if ( member_array( armour,
            (object *)this_object()->query_holding() ) != -1 ) {
         return "already wearing";
      }
      if( sizeof( (int *)armour->hold_item( this_object(), -1 ) ) ) {
         return 0;
      } else {
         return "already holding";
      }
   }
   if ( !armour->set_worn_by( this_object() ) ) {
      if( fail_mess = armour->query_wear_failure_mess() )
         return fail_mess;
      else
         return "oh dear";
   }
   wearing += ({ armour });
   this_object()->do_burden_call();
   return 0;
} /* wear_armour() */

/**
 * This method will remove the armour from the living object.
 * @param thing the armour to remove
 * @return 1 if was unsuccessful and 0 if it was successful
 * @see wear_armour()
 * @see query_armours()
 * @see query_wearing()
 */
int remove_armour( object thing ) {
   int i;

   wearing -= ({ 0 });
   i = member_array( thing, wearing );
   if ( i == -1 ) {
      return 1;
   }
   if ( !thing->set_worn_by( 0 ) ) {
      return 1;
   }
   if ( member_array( thing, (object *)this_object()->query_holding() ) != -1 ) {
      if ( !sizeof( (int *)this_object()->set_unhold( thing ) ) ) {
         return 1;
      }
   }
   wearing = delete( wearing, i, 1 );
   this_object()->do_burden_call();
   return 0;
} /* remove_armour() */

/**
 * This function returns the amount of damage that can be blocked
 * by the armour on a specified zone.  It automatically
 * damages the armour, and sets the stopped object to be the
 * the object (piece of armour) that stopped the blow.
 *
 * @param type The type of damage, eg: "sharp", "blunt", "pierce".
 * @param amount The amount of damage that is being done.
 * @param zone The zone which the damage is being done through, eg: "head".
 * @return The amount of damage that will be blocked.
 * @see query_wearing()
 */
varargs int query_ac( string type, int amount, string zone ) {
  int part, total, i, max;
  string word, *types;
  mixed most;
  object thing;
  object *defenses;
  
  stopped = 0;
#ifdef INFORM
  event( environment(), "inform", "zone "+ zone, "combat" );
#endif
  
  if ( !zone ) {
    switch(random(21)) {
    case 0..1: zone = "head"; break;
    case 2: zone = "neck"; break;
    case 3..6: zone="chest"; break;
    case 7..9: zone="abdomen"; break;
    case 10..12: zone = "arms"; break;
    case 13..14:
      zone = "hands"; break;
    case 15..18: zone = "legs"; break;
    case 19: zone = "feet"; break;
    case 20: zone = "back"; break;
    }
#ifdef INFORM
    event( environment(), "inform", "picked zone "+ zone,
           "combat" );
#endif
  }

  // Do the 'outside the armour' callbacks.
  if(sizeof(_armour_callbacks[ARM_OUTER]) && amount) {
    for(i=0; i<sizeof(_armour_callbacks[ARM_OUTER]); i++) {
      if(!_armour_callbacks[ARM_OUTER][i]->callback[0])
        remove_armour_callback(ARM_OUTER,
                               _armour_callbacks[ARM_OUTER][i]->id);
      else {
        part = call_other(_armour_callbacks[ARM_OUTER][i]->callback[0],
                          _armour_callbacks[ARM_OUTER][i]->callback[1],
                          this_object(),
                          type, amount, zone,
                          _armour_callbacks[ARM_OUTER][i]->data);
        if(part > max) {
          max = part;
          most = stopped;
        }
        total += part;
        amount -= part;
        if(amount < 1)
          break;
      }
    }
  }
  
  if(zone != "self" && amount > 0) {
    types = CLOTHING_HANDLER->query_clothing_zone( zone );
    if(!types) {
      log_file("ARMOUR_DEBUG", "No types for zone: %s", zone);
      return 0;
    }
    
    foreach ( word in  types) {
      wearing -= ({ 0 });
      defenses = wearing + filter_array(this_object()->query_holding(),
                                        (: $1 && $1->query_armour() :));
      foreach ( thing in defenses )
        if ( equivalent_armour_types( (mixed)thing->query_type(), word ) ) {
#ifdef INFORM
          event( environment(), "inform", "testing object "+
                 thing->short(), "combat" );
#endif
          part = (int)thing->query_ac( type, amount, zone );
          if (!stopped && part)
            stopped = thing;
          if(part > max) {
            max = part;
            most = stopped;
          }
          total += part;
          amount -= part;
        }
      if (amount < 1)
        break;
    }
  }

  // Do the 'against the skin' callbacks.
  if(sizeof(_armour_callbacks[ARM_INNER]) && amount > 0) {
    for(i=0; i<sizeof(_armour_callbacks[ARM_INNER]); i++) {
      if(!_armour_callbacks[ARM_INNER][i]->callback[0])
        remove_armour_callback(ARM_INNER,
                               _armour_callbacks[ARM_INNER][i]->id);
      else {
        part = call_other(_armour_callbacks[ARM_INNER][i]->callback[0],
                          _armour_callbacks[ARM_INNER][i]->callback[1],
                          this_object(),
                          type, amount, zone,
                          _armour_callbacks[ARM_INNER][i]->data);
        if(part > max) {
          max = part;
          most = stopped;
        }
        total += part;
        amount -= part;
        if(amount < 1)
          break;
      }
    }
  }

  // Does their race have special ac?
  if(amount > 0 && this_object()->query_race_ob() &&
     function_exists("query_ac", load_object(this_object()->query_race_ob()))){
    part = (this_object()->query_race_ob())->query_ac(type, amount, zone);
    if(part > max) {
      max = part;
      most = stopped;
    }
    total += part;
    amount -= part;
  }

  // Now do their skin.
  if(amount > 0) {
    if(!stopped && type != "skin")
      stopped = skin;

    // Dry them out if fire gets through.
    if(type == "fire") {
      if(this_object()->effects_matching("body.wetness") &&
         sizeof((int *)this_object()->effects_matching("body.wetness"))) {
        this_object()->add_effect("/std/effects/other/wetness", -amount );
      }
    }
    
    // Do the bodies personal AC.
    part = ::query_ac(type, amount, zone);
    if(part > max) {
      max = part;
      most = stopped;
    }
    total += part;
    amount -= part;
  }
  
  if(amount > 0 && most) {
    stopped = most;
  }
  return total;

}

/** @ignore yes */
mixed *stats() {
   return ::stats() + ({
      ({ "skin", skin }),
   });
} /* stats() */
