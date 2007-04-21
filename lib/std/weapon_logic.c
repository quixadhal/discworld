/**
 * This class has alkl the stuff for anything which can damage something
 * else, this is included into living and weapons.
 * @author Pinkfish.
 */
#include <weapon.h>

nosave string *attack_names;
nosave string *attack_types;
nosave mixed *attack_data;
nosave mapping special_messages;

void create() {
   attack_names = ({ });
   attack_types = ({ });
   attack_data = ({ });
   special_messages = ([ ]);
} /* create() */

string *query_attack_names() { return copy( attack_names ); }
string *query_attack_types() { return uniq_array( attack_types ); }
mixed *query_attack_data() { return copy( attack_data ); }
mapping query_special_messages() { return copy( special_messages ); }

/**
 * This method adds a special attack message set onto the weapon.  The type
 * and name are around this way to correspond
 * to the parameters to add_attack.
 * @see /obj/handlers/attack_messages
 *
 * @param name the name of the attack (0 for none)
 * @param type the type of attack
 * @param data the attack data
 * @see query_special_message()
 * @example
 * inherit "/obj/weapon";
 *
 * void setup() {
 *   ...
 *   add_attack_message("gumboot", "blunt", ({
 *     0, "$N gumboot$s at $I with $D",
 *    20, "$N lightly gumboot$s $I in the $z with $D",
 *    60, "$N gumboot$s $I in the $z with $D",
 *  5000, "$N heavily gumboot$s $I in the $z with $D" }));
 *   ...
 * } /\* setup() *\/
 */
void add_attack_message(string name, string type, string *data) {
   string nam;

   nam = type;
   if (name) {
      nam += "-" + name;
   }
   special_messages[nam] = data;
} /* add_attack_message() */

/**
 * This method returns the attack message associated with the
 * type and name.  The type and name are around this way to correspond
 * to the parameters to add_attack.
 * @param name the name of the attack
 * @param type the type of the attack
 * @return the attack message array, 0 for none
 */
mixed *query_attack_message(string name, string type) {
   if (special_messages[type + "-" + name]) {
      return special_messages[type + "-" + name];
   }
   if (special_messages[type]) {
     return special_messages[type];
   }
   return 0;
} /* query_attack_message() */

/**
 * This method adds an attack onto the object.  The name of the attack
 * must be unique on each object.
 * @param a_name attack name
 * @param change the chance of it occuring
 * @param damage the damage it will do
 * @param type the type of the attack
 * @param skill the skill used by the attack
 * @param func the special function
 * @param bogus_1 errrr.
 * @param bogus_2 frog.
 * @see remove_attack()
 */
varargs int add_attack( string a_name, int chance, int *damage, string type,
      string skill, mixed func, mixed bogus_1, mixed bogus_2 ) {
   if ( stringp( bogus_1 ) ) {
      write( file_name( this_object() ) +" is using the obselete syntax "+
            "of add_attack.\n" );
      return 0;
   }
   if ( member_array( a_name, attack_names ) != -1 )
      return 0;
   attack_names += ({ a_name });
   attack_types += ({ type });
   attack_data += ({ chance, damage, type, skill, func });
   return 1;
} /* add_attack() */

/**
 * This method removes the attack of the given name.
 * @param a_name the name of the attack to remove
 * @see add_attack()
 */
void remove_attack( string a_name ) {
   int i;

   i = member_array( a_name, attack_names );
   if ( i == -1 )
      return;
   attack_names = delete(attack_names, i, 1);
   attack_types = delete(attack_types, i, 1);
   attack_data = delete( attack_data, i * W_ARRAY_SIZE, W_ARRAY_SIZE );
} /* remove_attack() */

int modify_damage( int damage, string attack_name ) { return damage; }

int calc_attack( int number, int percent ) {
   int damage, *data;

   data = attack_data[ number * W_ARRAY_SIZE + W_DAMAGE ];
   damage = data[ F_FIXED ] + roll_MdN( data[ F_NUM ], data[ F_DIE ] );
   damage = (int)this_object()->modify_damage( damage,
         attack_names[ number ] );
   damage = ( damage * percent ) / 100;
   return damage;
} /* calc_attack() */

/**
 * This function returns 0 or more attacks for this weapon against the
 * given target.
 *
 * @param percent the attack percentage
 * @param target who is being hit
 *
 * @return an array of 0 or more attacks. This array is indexed by the
 * AT_ macros defined in combat.h
 *
 * @see combat.h
 */
mixed *weapon_attacks(int percent, object target ) {
   int i, *order;
   mixed *attacks;
   if ( !percent )
      percent = 100;
   order = ({ });
   for ( i = 0; i < sizeof( attack_names ); i++ )
      order += ({ i });
   order = shuffle( order );
   attacks = ({ });
   for ( i = 0; i < sizeof( order ); i++ ) {
      if ( random( percent ) <
            attack_data[ order[ i ] * W_ARRAY_SIZE + W_CHANCE ] ) {
         attacks += ({ calc_attack( order[ i ], percent ),
               attack_data[ order[ i ] * W_ARRAY_SIZE + W_SKILL ],
               attack_data[ order[ i ] * W_ARRAY_SIZE + W_TYPE ],
               attack_names[ order[ i ] ] });
      }
   }
   return attacks;
} /* weapon_attacks() */

void attack_function( string a_name, int damage, object attack_ob,
      object attack_by ) {
   int i;

   i = member_array( a_name, attack_names );
   if ( i == -1 ) {
      return;
   }
   i *= W_ARRAY_SIZE;
   if ( !attack_data[ i + W_FUNCTION ] ) {
      return;
   }
   if ( stringp( attack_data[ i + W_FUNCTION ] ) ) {
      call_other( this_object(), attack_data[ i + W_FUNCTION ],
            damage, attack_ob, attack_by, attack_data[ i + W_TYPE ], a_name );
   } else {
      call_other( attack_data[ i + W_FUNCTION ][ 1 ],
            attack_data[ i + W_FUNCTION ][ 0 ], damage, attack_ob, attack_by,
            attack_data[ i + W_TYPE ], a_name );
   }
} /* attack_function() */

/**
 * This method attempts to work out what type of weapon this is.
 * @return the weapon type
 */
string query_weapon_type() {
   int i;
   string type;

   for ( i = 0; i < sizeof( attack_data ); i += W_ARRAY_SIZE ) {
      if ( !type ) {
         type = attack_data[ i + W_SKILL ];
         continue;
      }
      if ( type != attack_data[ i + W_SKILL ] )
         return "mixed";
   }
   return type;
} /* query_weapon_type() */

/** @ignore yes */
mixed weapon_stats() {
   int i, j;
   string bit;
   mixed *ret;
   ret = ({ });
   for ( i = 0; i < sizeof( attack_data ); i += W_ARRAY_SIZE, j++ ) {
      ret += ({
         ({ "attack #"+ j, attack_names[ j ] }),
         ({ "   chance", attack_data[ i + W_CHANCE ] })
      });
      if ( attack_data[ i + W_DAMAGE ][ F_FIXED ] )
         bit = attack_data[ i + W_DAMAGE ][ F_FIXED ] +"+";
      else
         bit = "";
      if ( attack_data[ i + W_DAMAGE ][ F_NUM ] )
         bit += attack_data[ i + W_DAMAGE ][ F_NUM ] +"d"+
               attack_data[ i + W_DAMAGE ][ F_DIE ];
      else
         if ( attack_data[ i + W_DAMAGE ][ F_DIE ] )
            bit += "1d"+ attack_data[ i + W_DAMAGE ][ F_DIE ];
      ret += ({
         ({ "   damage", bit }),
         ({ "     type", attack_data[ i + W_TYPE ] }),
         ({ "    skill", attack_data[ i + W_SKILL ] })
      });
      if ( stringp( attack_data[ i + W_FUNCTION ] ) )
         ret += ({ ({ " function", attack_data[ i + W_FUNCTION ] }) });
      else
         if ( pointerp( attack_data[ i + W_FUNCTION ] ) )
            ret += ({
               ({ " function", attack_data[ i + W_FUNCTION ][ 0 ] }),
               ({ "called on", attack_data[ i + W_FUNCTION ][ 1 ] })
            });
   }
   return ret;
} /* weapon_stats() */

/** @ignore yes */
mixed stats() { return weapon_stats(); }
