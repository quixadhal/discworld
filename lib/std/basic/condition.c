/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: condition.c,v 1.14 2003/05/13 21:20:48 ceres Exp $
 */
/**
 * This class keeps track of the condition of a particular object.  It
 * handles damaging the object when it is hit and the lowest conditon to
 * which the object has been damaged.  The lowest conditon keeps track of
 * how hard the object is to fix.  If an object has been reduced to really
 * shaggy condition at one point, it will be a lot harder to fix from then
 * on.
 * @author Pinkfish
 */
#include <armoury.h>
#include <move_failures.h>

#define DAMAGE_NORM 200

private nosave int cond, max_cond, lowest_cond, damage_chance, hide_cond;

void create() {
   max_cond = 1;
} /* create() */

/* This should be masked. */
/**
 * This method breaks the object.  This moves it into the recycled
 * object section of the armoury or dests itself if this is not
 * possible.
 */
void break_me() {
  if(environment(this_object())) {
    if(living(environment(this_object())))
      tell_object(environment(this_object()),
                  capitalize(this_object()->the_short()) + " breaks!\n");
    else
      tell_room(environment(this_object()),
                capitalize(this_object()->the_short()) + " breaks!\n");
  }
  
  if((int)this_object()->move("/room/rubbish") != MOVE_OK)
    move_object("/room/rubbish");
}

/**
 * This method returns the current condition as a string.
 * @return the string value of the condition
 */
string cond_string() {
   string pronoun;
   string verb;

   if ( cond > max_cond ) {
      cond = max_cond;
   }
   if ( hide_cond )
      return "";

   pronoun = capitalize(this_object()->query_pronoun());
   verb = this_object()->query_is_pair() ? " are" : " is";
   switch ( ( 100 * cond ) / max_cond ) {
      case 91 .. 100 :
         return pronoun + verb + " in excellent condition.\n";
      case 81 .. 90 :
         return pronoun + verb + " in very good condition.\n";
      case 71 .. 80 :
         return pronoun + verb + " in good condition.\n";
      case 61 .. 70 :
         return pronoun + verb + " in decent condition.\n";
      case 51 .. 60 :
         return pronoun + verb + " in fairly good condition.\n";
      case 41 .. 50 :
         return pronoun + verb + " in fairly poor condition.\n";
      case 31 .. 40 :
         return pronoun + verb + " in poor condition.\n";
      case 21 .. 30 :
         return pronoun + verb + " in really poor condition.\n";
      case 11 .. 20 :
         return pronoun + verb + " in very poor condition.\n";
      case 1 .. 10 :
         return pronoun + verb + " in atrocious condition.\n";
      default :
         return pronoun + verb + " a complete wreck.\n";
   }
} /* cond_string() */

/**
 * This sets up how much damage an object can take before it 
 * breaks.  This should not be used in the actual object files
 * unless under special circumstances.  In weapon files, use 
 * new_weapon().  In armour files use setup_armour().  In 
 * clothing files, use setup_clothing().  Use ::Setup:: in 
 * clo, arm, and wep files.
 * <p>
 * The values may seem strange, as cloth can take more damage 
 * per unit weight than steel.  This is because steel weighs
 * so much, and cloth weighs so little.
 * <br><br>
 * o==================o=====================o<br>
 * |     Material     |  Maximum Condition  |<br>
 * |                  |   per unit weight   |<br>
 * o==================o=====================o<br>
 * | Cloth            |         90          |<br>
 * | Hide             |         80          |<br>
 * | Leather          |         70          |<br>
 * | Wood             |         60          |<br>
 * | Bone             |         45          |<br>
 * o==================o=====================o<br>
 * | Copper           |         30          |<br>
 * | Stone            |         40          |<br>
 * | Bronze           |         40          |<br>
 * | Iron             |         50          |<br>
 * | Steel            |         60          |<br>
 * | Klatchian Steel  |         70          |<br>
 * | Octiron          |         80          |<br>
 * o==================o=====================o
 * 
 * @see new_weapon
 * @see setup_armour
 * @see setup_clothing
 * @see set_weight
 * @see set_damage_chance
 */
void set_max_cond(int i) { max_cond = i; }

/**
 * This method sets the current condition of the object.  If this is lower
 * than the current lowest conditon, then the lowest condition will be modified
 * to be this.
 * @param i the new condition
 * @see adjust_condition
 */
void set_cond(int i) {
   cond = i;
   if (cond < 0) {
      cond = 0;
   }
   if (cond < lowest_cond) {
      lowest_cond = cond;
   }
   if ( cond <= 0 ) {
      if ( environment( this_object() ) &&
          !environment( this_object() )->query_property("don't break contents")) {
         this_object()->break_me();
      }
      return 0;
   }
} /* set_cond() */

/**
 * This method adjust the current condition by the given amount.  If
 * resultant condition is less than the lowest condition, then the lowest
 * condition is set to this value.  If it is greater than the maximum
 * condition.  Then the condition is set to the maximum condition.  If the
 * condition is less than or equal to 0, then the object is attempted to
 * be broken.
 * @param i the amount to adjust the condition by
 * @return the new condition
 * @see set_cond()
 */
int adjust_cond( int i ) {
   cond += i;
   if (cond < 0)
      cond = 0;
   if ( cond < lowest_cond )
      lowest_cond = cond;
   if ( cond > max_cond )
      cond = max_cond;
   if ( cond <= 0 ) {
      if ( !environment()->query_property( "don't break contents" ) )
         this_object()->break_me();
      return 0;
   }
   return cond;
} /* adjust_cond() */

/**
 * This sets the lowest condition for the object.  The lowest cond
 * variable is used for repairing the armour...
 * See where it was repaired to last time, this determines how difficult
 * repairs will be...
 * @param i the new lowest condition
 * @see adjust_cond()
 * @see set_cond()
 */
void set_lowest_cond(int i) { lowest_cond = i; }

/**
 * This sets whether the condition string should be returned or not.  This
 * is for objects what indicate their condition with different long
 * descriptions instead of the default condition string.
 * @param i set it to 1 to hide the condition string.
 */
void set_hide_cond(int i) { hide_cond = i; }

/**
 * This method returns the current conditon.
 * @return the current condition
 */
int query_cond() {
  if ( cond > max_cond ) {
    cond = max_cond;
  }
  return cond;
}

/**
 * This method returns the current maximum conditon.
 * @return the current maximum condition
 */
int query_max_cond() { return max_cond; }

/**
 * This method returns the current lowest conditon.
 * @return the current lowest condition
 */
int query_lowest_cond() { return lowest_cond; }

/**
 * This method returns whether the condition string returns "" or something 
 * meaning full.
 * @return the current lowest condition
 */
int query_hide_cond() { return hide_cond; }

/**
 * This method is used to setup the objects condition.  Once the maximum
 * condition has been set, this is used to set the maximum condition to
 * this percentage of the maximum condition.
 * It sets the lowest conditon a random value from 70-100% of the
 * current condition.
 * @param i the percentage condiion
 * @see /obj/handlers/armoury.c
 * @see set_cond()
 * @see set_lowest_cond()
 * @see query_max_cond()
 */
void set_percentage(int i) {
   if (i > 100) {
      i = 100;
   }
   set_cond((query_max_cond() * i) / 100);
   set_lowest_cond((query_cond()*(random(30) + 70)) / 100);
} /* set_percentage() */

/** 
 * This method returns the percentage of the damage which is done to the
 * object which will be carried through.
 * @return the damage chance
 * @see do_damage()
 */
int query_damage_chance() { return damage_chance; }

/**
 * This method sets the percentage of the damage which is done to the
 * object which will be carried through.
 * <p>
 * Use the following chart to set the damage chance.
 * <br><br>
 * o==================o=====================o<br>
 * |     Material     |    Damage Chance    |<br>
 * o==================o=====================o<br>
 * | Cloth            |         20          |<br>
 * | Hide             |         17          |<br>
 * | Bone             |         16          |<br>
 * | Leather          |         15          |<br>
 * | Wood             |         12          |<br>
 * | Copper           |         10          |<br>
 * | Bronze           |          8          |<br>
 * | Iron             |          6          |<br>
 * | Steel            |          5          |<br>
 * | Klatchian Steel  |          3          |<br>
 * | Stone            |          3          |<br>
 * | Octiron          |          0          |<br>
 * o==================o=====================o
 *
 * @see new_weapon
 * @see setup_armour
 * @see setup_clothing
 * @see set_weight
 * @see set_max_cond
 * @see query_damage_chance()
 * @see do_damage()
 * @param number the new damage chance
 */
void set_damage_chance( int number ) { damage_chance = number; }

/**
 * This method damages the object.  The conditon is reduced by
 * the amount multiplied by 1 + 2 * random(damage_chance) ewhic
 * is then divided by 100.  This means it does approximately
 * the percentage of of the damage done to the object to its condition.
 * @see set_damage_chance()
 * See query_damage_chance()
 */
void do_damage( string type, int amount ) {
   int wholes, parts;

   wholes = amount * ( 1 + random( damage_chance ) + random( damage_chance ) );
   parts = wholes % DAMAGE_NORM;
   wholes /= DAMAGE_NORM;
   if ( random( DAMAGE_NORM ) < parts )
      wholes++;
   adjust_cond( -wholes );
} /* do_damage() */

/**
 * @ignore
 */
mixed *stats() {
   return 
      ({
         ({ "condition", cond }),
         ({ "max cond", max_cond }),
         ({ "lowest cond", lowest_cond }),
         ({ "damage chance", damage_chance }),
         ({ "hide cond", hide_cond }),
      });
} /* stats() */

/**
 * @ignore
 */
mapping query_static_auto_load() {
   return ([
      "max cond" : max_cond,
      "damage chance" : damage_chance,
      "hide cond" : hide_cond
   ]);
} /* query_static_auto_load */

/**
 * @ignore
 */
void init_static_arg( mapping map ) {
   if ( !undefinedp( map[ "max cond" ] ) )
      max_cond = map[ "max cond" ];
   if ( !undefinedp( map[ "damage chance" ] ) )
      damage_chance = map[ "damage chance" ];
   if ( !undefinedp( map[ "hide cond" ] ) )
      hide_cond = map[ "hide cond" ];
} /* init_static_arg() */

/**
 * @ignore
 */
mapping query_dynamic_auto_load() {
   if ( cond > max_cond ) {
      cond = max_cond;
   }

   if ( lowest_cond > cond ) {
      lowest_cond = cond;
   }

   return ([
      "cond" : cond,
      "lowest cond" : lowest_cond
   ]);
} /* query_dynamic_auto_load */

/**
 * @ignore
 */
void init_dynamic_arg( mapping map, object ) {
   if ( !undefinedp( map[ "cond" ] ) )
      cond = map[ "cond" ];
   if ( !undefinedp( map[ "lowest cond" ] ) )
      lowest_cond = map[ "lowest cond" ];
} /* init_dynamic_arg() */

