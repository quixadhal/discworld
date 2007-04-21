/*
 * $Locker:  $
 * $Id: wearable.c,v 1.24 2003/05/21 03:44:06 ceres Exp $
 */
/**
 * This file contains all the methods needed to make an object wearable
 * by a player or an npc.
 * @author Pinkfish
 * @see /obj/armour.c
 * @see /obj/clothing.c
 */
inherit "/std/basic/condition";

#include <clothing.h>
#include <move_failures.h>

private string *immune_to;
private string *wear_effects;
private mixed type;
private mixed *wear_remove_func;
nosave object worn_by;

void create() {
   condition::create();
   set_damage_chance( 25 );
   immune_to = ({ "cold", "sound", "gas", "mental" });
   wear_effects = ({ });
   wear_remove_func = ({ });
} /* create() */

/**
 * This method tells us if the object is wearable.
 * In the case of a wearable object it will always return 1.
 * @return always returns 1
 */
int query_wearable() { return 1; }

/** @ignore yes */
string long( string word, int dark ) {
   switch ( dark ) {
      case 0 :
         return cond_string();
      default :
         return "";
   }
} /* long() */

/**
 * This method returns the current function associated with
 * wearing and removing the item.
 * @return the current function for wearing and removing the clothing
 * @see set_wear_remove_func()
 */
mixed *query_wear_remove_func() {
   return wear_remove_func;
} /* query_wear_remove_func() */

/**
 * This method sets the current function associated with wearing and
 * removing the item.  The value of this function should be choosen
 * carefully, using an object reference for the name will work but
 * it will not then be able to restored from the save file.  The same
 * goes for using function pointers.  It is better to use a real
 * file name and a string function name.
 * <p>
 * The function will be called with two arguments, the first argument will
 * be 0 if the object is being removed or the object which it is
 * being worn by if it is non-zero.  The second argument will always
 * be the person who was wearing the item, or is about to wear the
 * item.
 * @example
 * inherit "/std/basic/wearable";
 *
 * void setup() {
 *    ...
 *    set_wear_remove_func(base_name(this_object()), "do_fluff");
 *    ...
 * } /\* setup() *\/
 *
 * void do_fluff(object ob, object player) {
 *    if (!ob) {
 *       write(capitalize(the_short()) + " is being removed.\n");
 *    } else {
 *       write(capitalize(the_short()) + " is being worn.\n");
 *    }
 * } /\* do_fluff() *\/
 * @param file the file to call the function on
 * @param func the function to call
 * @see query_wear_remove_func()
 */
void set_wear_remove_func( mixed file, string func ) {
   wear_remove_func = ({ file, func });
} /* set_wear_remove_func() */

/**
 * This method returns the list of effects to be added to the wearer when
 * it is worn.  These effects will automaticly be added when worn and
 * removed when taken off.
 * @return the list of effects to be added to the wearer when worn
 * @see set_wear_effects()
 * @see add_wear_effect()
 */
string *query_wear_effects() {
  if(!wear_effects)
    wear_effects = ({ });
  return copy(wear_effects);
} /* query_wear_effects() */

/**
 * This method sets the list of effects to be added to the wearer when it
 * is worn.  These effects will automaticly be added when worn and
 * removed when taken off.
 * @param effects the array of effects to be added to the wearer when worn
 * @see query_wear_effects()
 * @see add_wear_effect()
 */
void set_wear_effects( string *effects ) {
   wear_effects = effects;
} /* set_wear_effects() */

/**
 * This method adds a new wear effect to the current wear effect array.  These effects will automaticly be added when worn and
 * removed when taken off.
 * @param effect the effect to add
 * @see query_wear_effects()
 * @see set_wear_effects()
 */
void add_wear_effect( string effect ) {
   if ( member_array( effect, wear_effects ) == -1 ) {
      wear_effects += ({ effect });
   }
} /* add_wear_effect() */

/** @ignore yes */
void do_damage( string type, int amount ) {
   if ( member_array( type, immune_to ) == -1 ) {
      condition::do_damage( type, amount );
   }
} /* do_damage() */

/**
 * This method returns the person who is currently wearing the object.
 * @return the current wearer of the object
 * @see set_worn_by()
 */
object query_worn_by() {
   return worn_by;
} /* query_worn_by() */

/**
 * This method sets the object as being worn by the passed in object.
 * It calls all the various worn functions and sets up or removes
 * all the effects associated with the object.
 * <p>
 * If the object is alreadying being worn the wear_remove_function will
 * be called with an argument of 0.  The method taken_off will be
 * called on the object wearing the object for all the effects associated
 * with this object.
 * <p>
 * If the object is being set to be worn by someone the the wear_remove
 * function will be called with an argument being the person who is to
 * wear the object.   All of the effects associated with the
 * object will be added to the wearer.
 * <p>
 * This calls the method 'person_removing_item' on the effect when some
 * one removes the item.  This can be used to make sure the effects are
 * taken off when the item is removed.
 * @param thing the new person to wear the object (0 for worn by no one)
 * @return 1 if successful, 0 on failure
 * @see set_wear_remove_func()
 * @see add_wear_effect()
 */
int set_worn_by( object thing ) {
   int i;

   /* Do nothing if we are already worn by them. */
   if ( thing == worn_by ) {
      return 1;
   }

   /* We need this thing check here because if thing is 0 we are removing the object! */
   /* Taffyd */

   if (thing && environment() != thing) {
      return 0;
   }

   /* First remove any possible concealment off us */
   this_object()->remove_hide_invis( "concealed" );

   /* If there is a move function, then call it. */
   if ( sizeof( wear_remove_func ) ) {
    if ( !objectp( wear_remove_func[ 0 ] ) && !objectp( load_object( wear_remove_func[ 0 ] ) ) ) {
        debug_printf( "Wear/Remove func is %O. Not called.\n", wear_remove_func );
      return 0;
    }

    if ( worn_by ) {
         call_other( wear_remove_func[ 0 ], wear_remove_func[ 1 ], 0, worn_by );
      }
      if ( thing ) {
         call_other( wear_remove_func[ 0 ], wear_remove_func[ 1 ], thing, thing );
      }
   }

   /* If there are some wear effects, set them up or remove them. */
   if ( sizeof( wear_effects ) ) {
      for ( i = 0; i < sizeof( wear_effects ); i++ ) {
         if ( worn_by ) {
            worn_by->taken_off( this_object(), wear_effects[ i ] );
         }
         if ( thing ) {
            thing->add_effect( wear_effects[ i ], this_object() );
         } else {
            wear_effects[i]->person_removing_item(this_object(), worn_by);
         }
      }
   }

   /*
    * Call the functions on the object doing the wearing to remove or add
    * the item.
    */
   if ( worn_by ) {
      worn_by->now_removed( this_object() );
      this_object()->add_adjective("worn");
      this_object()->remove_adjective("unworn");
   } else {
      this_object()->remove_adjective("worn");
      this_object()->add_adjective("unworn");
   }
   if ( thing ) {
      thing->now_worn( this_object() );
   }
   worn_by = thing;

   return 1;
} /* set_worn_by() */

/**
 * This returns the list of types of damage that the object is immune to.
 * @return the list of damage we are immune to
 * @see /std/basic/condition.c
 * @see add_immune_to()
 * @see remove_immune_to()
 */
string *query_immune_to() {
   return immune_to;
} /* query_immune_to() */

/**
 * This adds a new type of damage that the object is immune to.
 * The parameter can either be a string or an array of strings
 * being the types of damage to be immune to.
 * @param args the type of damage to be immune to
 * @example
 * inherit "/std/basic/wearable";
 *
 * void setup() {
 *    ...
 *    add_immune_to("sharp");
 *    ...
 * } /\* setup() *\/
 * @example
 * inherit "/std/basic/wearable";
 *
 * void setup() {
 *    ...
 *    add_immune_to("sharp");
 *    ...
 * } /\* setup() *\/
 * @see remove_immune_to()
 * @see query_immune_to()
 */
void add_immune_to( mixed args ) {
   int i;

   if ( pointerp( args ) ) {
      for ( i = 0; i < sizeof( args ); i++ ) {
         add_immune_to( args[ i ] );
      }
   } else {
      if ( member_array( args, immune_to ) == -1 ) {
         immune_to += ({ args });
      }
   }
} /* add_immune_to() */

/**
 * This method removes a type of damage that the weapon is immune
 * to.
 * @param args the type(s) of damage to remove immunity too
 * @see add_immune_to()
 * @see query_immune_to()
 */
void remove_immune_to( mixed args ) {
   int i;

   if ( pointerp( args ) ) {
      for ( i = 0; i < sizeof( args ); i++ ) {
         remove_immune_to( args[ i ] );
      }
   } else {
      i = member_array( args, immune_to );
      if ( i != -1 ) {
         immune_to = delete( immune_to, i, 1 );
      }
   }
} /* remove_immune_to() */

/**
 * This method returns the current type(s) associated with the object.
 * If this method returns a string then there is only one type for this
 * object.  If it returns a string then there is more than one
 * type associated with an object.  An example of something with more
 * than one type is a skirt, which is a dress and a shirt at the
 * same time.
 * @return the current type of the item
 * @see set_type()
 */
string query_type() {
   return type;
} /* query_type() */

/** @ignore yes */
private void log_bad_type( mixed type ) {
  string word, str;

  if ( !clonep( this_object() ) ) {
      return;
  }
  word = (string)this_object()->query_property( "virtual name" );
  if ( word ) {
     str = sprintf( "BAD_TYPE %s (%s) = %O\n", word,
           (string)this_object()->query_short(), type );
  } else {
     str = sprintf( "BAD_TYPE %s (%s) = %O\n", file_name(),
           (string)this_object()->query_short(), type );
  }
  if ( this_player()) {
    write(str);
  }
  call_out("move", 2, "/room/broken");
} /* log_bad_type() */

/**
 * This method sets the type(s) which are associated with the
 * item.  If the parameter is a string then a single type is associated
 * with the item, if the parameter is an array then a list of types
 * is associated with the object.  If any of these types are not
 * legal and error message will be produced.
 * @param word the new type(s) to set for the object
 * @see query_type()
 */
void set_type( mixed word ) {
  int i;
  
   if(!stringp(word) && !pointerp(word))
      return log_bad_type(word);

   type = word;
   if(stringp(word)) {
     if(CLOTHING_HANDLER->query_equivilant_type(word))
       type = CLOTHING_HANDLER->query_equivilant_type(word);
     if(!CLOTHING_HANDLER->query_valid_type(word))
       log_bad_type( word );
     return;
   }
   
   for(i=0; i<sizeof(word); i++) {
     if(CLOTHING_HANDLER->query_equivilant_type(word[i]))
       type[i] = CLOTHING_HANDLER->query_equivilant_type(word[i]);
     if (!CLOTHING_HANDLER->query_valid_type(word[i]))
       log_bad_type( word );
     return;
   }
} /* set_type() */

/** @ignore yes */
int modify_value( int amount ) {
   return ( amount * ( 10 + ( 90 * query_cond() ) / query_max_cond() ) ) / 100;
} /* modify_value() */

/** @ignore yes */
void player_wear() {
   if ( !environment() ) {
      return;
   }
   environment()->wear_armour( this_object() );
} /* player_wear() */

/** @ignore yes */
void break_me() {
   if ( !worn_by ) {
      return condition::break_me();
   }
   tell_object( worn_by, "%^RED%^$C$"+ (string)this_object()->the_short() +
         " breaks!%^RESET%^\n" );
   tell_room( environment( worn_by ), (string)worn_by->the_short() +"'s "+
         this_object()->short( 0 ) +" breaks!\n", worn_by );
   worn_by->remove_armour( this_object() );

   if((int)this_object()->move("/room/rubbish") != MOVE_OK)
     move_object("/room/rubbish");
} /* break_me() */

/** @ignore yes */
mixed *stats() {
   int i;
   string *stuff;

   stuff = condition::stats() +
      ({
         ({ "type", type }),
      });
   if ( sizeof( wear_effects ) ) {
      for ( i = 0; i < sizeof( wear_effects ); i++ ) {
         stuff += ({ ({ "wear effect", wear_effects[ i ] }) });
      }
   }
   for ( i = 0; i < sizeof( immune_to ); i++ ) {
      stuff += ({ ({ "immune to", immune_to[ i ] }) });
   }
   if (worn_by) {
      stuff += ({ ({ "worn by", worn_by->short() }) });
   }
   return stuff;
} /* stats() */

/** @ignore yes */
mapping query_static_auto_load() {
   return ([
      "condition" : condition::query_static_auto_load(),
      "type" : type,
      "wear remove func" : wear_remove_func
   ]);
} /* query_static_auto_load() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
   return ([
      "condition" : condition::query_dynamic_auto_load(),
      "wear effects" : wear_effects,
      "immune" : immune_to,
      "worn" : ( worn_by != 0 )
   ]);
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_static_arg( mapping map ) {
   if ( map[ "type" ] )
      type = map[ "type" ];
   if ( map[ "wear remove func" ] )
      wear_remove_func = map[ "wear remove func" ];
   if ( !undefinedp( map[ "condition" ] ) ) {
      condition::init_static_arg( map[ "condition" ] );
   } 
#ifdef AFTER_1999_I_BELIEVE   
   else { /* if you read ths and it's 1999, remove this crap
             * and only keep the call above :)
             */
      if ( !undefinedp( map[ "max cond" ] ) )
         max_cond = map[ "max cond" ];
      if ( !undefinedp( map[ "damage chance" ] ) )
         damage_chance = map[ "damage chance" ];
   }
#endif
} /* init_static_arg() */

/** @ignore yes */
void init_dynamic_arg( mapping map, object ) {
   if ( map[ "effect" ] )
      wear_effects = ({ map[ "effect" ] });
   if ( pointerp( map[ "wear effects" ] ) )
      wear_effects = map[ "wear effects" ];
   if ( map[ "immune" ] )
      immune_to = map[ "immune" ];
   if ( map[ "worn" ] )
      call_out( "player_wear", 0 );
   if ( !undefinedp( map[ "condition" ] ) ) {
      condition::init_dynamic_arg( map[ "condition" ] );
   }
#ifdef AFTER_1999_I_BELIEVE
   } else { /* if you read this and it's 1999, remove this crap
             * and only keep the call above :) */
      cond = map[ "cond" ];
      lowest_cond = map[ "lowest cond" ];
   }
#endif
} /* init_dynamic_arg() */

/**
 * Unset the wear_remove func.
 */
void remove_wear_remove_func() {
    wear_remove_func = ({ }); 
}
