/**
 * The base object which all races will be inherited from.
 * @author Pinkfish
 * @started 1992 sometime
 */

#define QUEST_AVE 140
#define QUEST_MAX 570
#define BASE 50
#define SCALE 10
#define NORM 15
#define EXPERT 20
#define A_TYPES ([ \
      "claws" : "sharp", \
      "chew" : "sharp", \
      "beak" : "pierce", \
      "bite" : "pierce", \
      "horn" : "pierce", \
      "hands" : "blunt", \
      "feet" : "blunt", \
      "tail" : "blunt", \
      "hoof" : "blunt", \
      "wrestle" : "blunt", \
   ])

inherit "/std/object";

/* Original by who knows.
 * Modifications by Piecemaker, Ember, Presto, Deutha and Dragonkin.
 */

private int height;
private int *sight;
private int *stats;
private string desc;
private string skin;
string *inedible;
string *unrottable;
string *pluckable;
private string *limbs;
private mixed *acs;
private mixed *attacks;
private mapping _fixed_bits;
mixed *bits;

/* The acs array has element blocks:
 *
 *   name, type, amount
 *
 * The attacks array has element blocks:
 *
 *   name, chance, data relative to bonus of 200
 *
 * The bits array has element blocks:
 *
 *   short, name, ({ parent, percentage weight, ({ file name (0 is "/std/bit"),
 *         amount }), child bit 1, child bit 2, ... })
 *
 *   Note that if the size of ({ file name, amount }) part is less than two,
 *   it'll set the filename to default and the amount to 1
 */

void create() {
   do_setup++;
   ::create();
   do_setup--;

   sight = ({ 5, 20, 200, 300 });
   stats = allocate( 5 );
   desc = "a small non-descript thing";
   inedible = ({  });
   unrottable = ({  });
   pluckable = ({  });
   acs = ({ });
   attacks = ({ });
   bits = ({ });

   if ( !do_setup )
      this_object()->setup();
} /* create() */

/**
 * This method returns the base height of the npc type.
 * @return the current base height
 * @see set_height()
 */
int query_height() { return height; }

/**
 * This method sets the base height of the npc type.
 * @param number the new height
 * @see query_height()
 */
void set_height( int number ) { height = number; }

/**
 * Thie method returns the values of the race which determine visibility.
 * It returns an array of the format:<br>
 * <pre>({
 *    dark
 *    pitch dark,
 *    bright light,
 *    bright
 * })</pre>
 * @return the sight array as detailed above
 * @see set_sight()
 * @see query_dark()
 */
int *query_sight() { return copy(sight); }

/**
 * Thie method sets the current sight array for the npc.  This sets the
 * levels at which the race things things are too dark/too bright.<br>
 * <pre>({
 *    pitch dark,
 *    dark,
 *    bright,
 *    bright light
 * })</pre>
 * @param numbers as detailed above
 * @see query_sight()
 * @see query_dark()
 */
void set_sight( int *numbers ) { sight = numbers; }

/**
 * Determines if the race thinks it is dark or light.
 * @return 0 for normal, -1 for dark, -2 for pitch dark, 1 for bright,
 * 2 for toobright
 * @see set_sight()
 * @see query_sight()
 */
int query_dark( int light ) {
   if ( light < sight[ 0 ] ) {
      return -2;
   }
   if ( light < sight[ 1 ] ) {
      return -1;
   }
   if ( light > sight[ 3 ] ) {
      return 2;
   }
   if ( light > sight[ 2 ] ) {
      return 1;
   }
   return 0;
} /* query_dark() */

/**
 * This method queries the default bonus stat values for the race.<br>
 * <pre>({
 *   con,
 *   dex,
 *   int,
 *   str,
 *   wiz
 * })</pre>
 * @return the current stat offsets
 * @see set_stats()
 */
int *query_stats() { return copy(stats); }

/**
 * This method sets the current default bonuses of the stats.  The parameter
 * is an array of numbers, with the numbers corresponding to the stats as
 * shown below.  It is highly inadvisable to set any stat bonus to less than
 * -6, since in conjunction with the guild stat settings this could combine
 * to give a negative total for that stat.  Negative stats can cause
 * permanent unconsciousness.
 * <pre>({
 *   con,
 *   dex,
 *   int,
 *   str,
 *   wis
 * })</pre>
 * @param numbers sets the default bonuses for the stats
 * @see query_stats()
 */
void set_stats( int *numbers ) { stats = numbers; }

/**
 * This method returns the current skin of the race.
 * @return the current skin of the race
 * @see set_skin()
 */
string query_skin() { return skin; }

/**
 * This method sets the skin of the race.  This is what is given back
 * when the npc's corpse is skinned.
 * @param word the skin of the race
 * @see query_skin()
 */
void set_skin( string word ) { skin = word; }

/**
 * This method returns the current description of the npc.
 * @param thing the object which is being described
 * @return the description of the npc
 * @see query_desc()
 */
string query_desc( object thing ) {
   return capitalize( (string)thing->query_pronoun() ) +" is "+ desc +".\n";
} /* query_desc() */

/**
 * This method sets the description for the npc.
 * @param words the description of the npc
 * @see set_desc()
 */
void set_desc( string words ) { desc = words; }

/**
 * This method returns the current limbs of the race.
 * @return the limbs on the race
 * @see find_limbs()
 */
string *query_limbs() {
   if ( !limbs ) {
      this_object()->find_limbs();
   }
   return copy(limbs);
} /* query_limbs() */

/**
 * This method figures out the current set of limbs on the race.  A creature
 * is assigned one "limb" for every "hand" found in the body part array.  The
 * number of limbs is the number of weapons that can be held.
 * @see query_limbs()
 */
void find_limbs() {
   int i;
   string limb;

   limbs = ({ });
   for ( i = 0; i < sizeof( bits ); i += 3 ) {
     if ( sscanf( bits[ i ], "%s hand", limb ) == 1 ) {
       limbs += ({ bits[ i ] });
     }
   }
} /* find_limbs() */

/**
 * The defaults acs for npcs when they are fighting unarmed.
 * @return the default acs
 * @see add_ac()
 */
mixed *query_acs() { return copy(acs); }

/**
 * Adds a default ac to the npc for when they fight unarmed.  Please look
 * add the armour object for more details on this.
 * @param ac_name the name of the ac
 * @param ac_type the type of the ac
 * @param ac_amount the damage/roll stuff to take off
 * @see query_acs()
 */
void add_ac( string ac_name, string ac_type, mixed ac_amount ) {
   acs += ({ ac_name, ac_type, ac_amount });
} /* add_ac() */

/**
 * The default attacks for the npc when they are fighting unarmed.
 * @return the default attacks
 * @see add_attack()
 */
mixed *query_attacks() { return copy(attacks); }

/**
 * This method adds a default attack to the npc.  Please see the
 * help on the weapons for more details on this.
 * @param attack_name the name of the attack
 * @param attack_chance the chance of the attack occuring
 * @param attack_data the damage roll for the attack
 * @see query_attacks()
 */
void add_attack( string attack_name, int attack_chance, int *attack_data ) {
   attacks += ({ attack_name, attack_chance, attack_data });
} /* add_attack() */

/**
 * This method returns all the bits for the npc.
 * @return the complete array of bits
 * @see add_bit()
 * @see query_bit()
 * @see query_possible_bits()
 * @see query_all_inedible()
 * @see query_all_unrottable()
 * @see query_all_pluckable()
 * @see remove_bit()
 */
mixed *query_bits() { return bits; }

/**
 * This method returns the bits which are children of the specified bit.
 * @param word the bit to look for the children of
 * @return the children bits
 * @see add_bit()
 * @see query_bits()
 * @see remove_bit()
 */
mixed *query_bit( string word ) {
   int i;

   i = member_array( word, bits );
   if ( ( i == -1 ) || ( i % 3 ) ) {
      return ({ });
   }
   return bits[ i .. i + 2 ];
} /* query_bit() */

private void fixup_bits() {
   string* str_bits;
   string pl;
   int i;

   if (_fixed_bits) {
      return ;
   }

   _fixed_bits = ([ ]);

   for ( i = sizeof( bits ) - 3; i > -1; i -= 3 ) {
      str_bits = explode(bits[i], " ");
      pl = pluralize(str_bits[<1]);
      if (!_fixed_bits[str_bits[<1]]) {
         _fixed_bits[str_bits[<1]] = ({ });
      }
      _fixed_bits[str_bits[<1]] += ({ ({ str_bits[0..<2], i, 0 }) });
      if (!_fixed_bits[pl]) {
         _fixed_bits[pl] = ({ });
      }
      _fixed_bits[pl] += ({ ({ str_bits[0..<2], i, 1 }) });
   }
} /* fixup_bits() */

/** @ignore yes 
 * For backwards compatibility, as corpses are currently broken in
 * the way they call the query_possible_bits() method. 
 */ 
private string *query_old_possible_bits( string word ) {
   int i;
  int j;
  string *possibles;

  possibles = ({ });
  for ( i = sizeof( bits ) - 3; i > -1; i -= 3 ) {
    if ( ( bits[ i ] == word ) ||
          ( bits[ i + 1 ] == word ) ||
          !word) {
       if (arrayp( bits[i+2][2] )) {
         for ( j = 0; j < bits[i + 2][2][1]; j++) {
           possibles += ({ bits[ i ] });
         }
       } else {
         possibles += ({ bits[ i ] });
       }
     }
   }
   return possibles;
} /* query_old_possible_bits() */


/**
 * This method returns the names of all the possible bits for the
 * npc.
 * @param word the bits to look for the children under
 * @return the array of possible bit names
 * @see add_bit()
 * @see query_bits()
 * @see query_possible_plural_bits()
 * @see remove_bit()
 */
string *query_possible_bits( string word ) {
  string* str_bits;
  string* adj;
  string *possibles;
  mixed* bing;
  
  if(!word) {
    return query_old_possible_bits( word ); 
  }

  fixup_bits();

  str_bits = explode(word, " ");
  possibles = ({ });
  if (_fixed_bits[str_bits[<1]]) {
     adj = str_bits[0..<2];
     foreach (bing in _fixed_bits[str_bits[<1]]) {
        if (!bing[2] &&
            sizeof(adj) == sizeof(adj & bing[0])) {
           possibles += ({ bits[bing[1]] });
        }
     }
  }
  return possibles;
} /* query_possible_bits() */

/**
 * This method returns the plural names for all the bits.
 * @param word the bit to find the children of
 * @return the plural names of all the bits
 * @see add_bit()
 * @see query_bits()
 * @see query_possible_bits()
 * @see remove_bit()
 */
string *query_possible_plural_bits( string word ) {
  string* str_bits;
  string* adj;
  string *possibles;
  mixed* bing;

  fixup_bits();

  str_bits = explode(word, " ");
  possibles = ({ });
  if (_fixed_bits[str_bits[<1]]) {
     adj = str_bits[0..<2];
     foreach (bing in _fixed_bits[str_bits[<1]]) {
        if (bing[2] &&
            sizeof(adj) == sizeof(adj & bing[0])) {
           possibles += ({ bits[bing[1]] });
        }
     }
  }
  return possibles;
} /* query_possible_plural_bits() */

/**
 * This method adds a bit to the current bit setup.
 * @param bit_short the short of the bit
 * @param bit_name the name of the bit
 * @param bit_array the array of children bits
 * @see query_bits()
 * @see remove_bit()
 */
void add_bit( string bit_short, string bit_name, mixed *bit_array ) {
   int i;

   i = member_array( bit_short, bits );
   if ( !( i % 3 ) ) {
      bits[ i ] = bit_short;
      bits[ i + 1 ] = bit_name;
      bits[ i + 2 ] = bit_array;
      return;
   }
   bits += ({ bit_short, bit_name, bit_array });
} /* add_bit() */

/**
 * This method removes a bit from the current bit array
 * @param word the name of the bit to remove
 * @see add_bit()
 * @see query_bits()
 */
void remove_bit( string word ) {
   int i;

   i = member_array( word, bits );
   if ( ( i == -1 ) || ( i % 3 ) ) {
      return;
   }
   bits = delete( bits, i, 3 );
} /* remove_bit() */

/**
 * This method figures out a modifier for the height based on the base
 * weight and height.
 * @param number the height/weight to modify
 * @return the modified heigh/weight
 */
int modifier( int number ) {
   return ( number * ( roll_MdN( 20, 21 ) - 220 ) ) / 2000;
} /* modifier() */

/**
 * This method is called when the npc is first created.  This sets up
 * the weight/height/stats etc.
 * @param thing the npc being created
 */
void start_player( object thing ) {
   if ( !thing->query_base_weight() ) {
      thing->set_base_weight( 1 + weight + modifier( weight ) );
   }
   if ( !thing->query_height() ) {
      thing->set_height( 1 + height + modifier( height ) );
   }
   thing->adjust_bonus_con( stats[ 0 ] );
   thing->adjust_bonus_dex( stats[ 1 ] );
   thing->adjust_bonus_int( stats[ 2 ] );
   thing->adjust_bonus_str( stats[ 3 ] );
   thing->adjust_bonus_wis( stats[ 4 ] );
   thing->reset_all();
   if ( skin ) {
      thing->set_skin( skin );
   }
} /* start_player() */

/**
 * This method figures out how many of each sort of thing 
 * can be worn.
 * @param type the type to check for numbers worn
 * @return the number of type to worn
 */
int query_number_worn( string type ) {
   switch ( type ) {
      case "armband" :
         return 2;
      case "badge" :
         return 15;
      case "bracelet" :
         return 4;
      case "earring" :
        return 2;/* added by ceres since piercing isn't implemented yet */
//         return (int)previous_object()->query_piercings( "ear" );
      case "garter" :
         return 2;
      case "shoulder" :
         return 2;
      case "necklace" :
         return 5;
      case "ring" :
         return 8;
      case "sash" :
         return 2;
      case "shirt" :
         return 2;
      case "belt scabbard" :
         return 2;
      case "small scabbard" :
         return 2;
      case "bandaid" :
         return 5;
      default :
         return 1;
   }
} /* query_number_worn() */

int query_number_wielded( string type ) { return 1; }

int query_skill_bonus( int number, string word ) { return 0; }

int player_start( object thing ) { return 1; }

int player_quit( object thing ) { return 1; }

/**
 * This method sets the level of the npc.
 * @param thing the npc to set the level of
 * @param level the current level
 */
void set_level( object thing, int level ) {
   int i;

   if ( !thing ) {
      return;
   }
   for ( i = sizeof( acs ) - 3; i > -1; i -= 3 ) {
      thing->add_ac( acs[ i ], acs[ i + 1 ], acs[ i + 2 ] );
   }
} /* set_level() */

int *calc_attack( int *data, int number ) {
   int base;
   int num;
   int rand;
   int rating;
   int scale;

   base =  data[ 0 ] * ( BASE + SCALE * number );
   num = data[ 1 ];
   rand = data[ 2 ] * ( BASE + SCALE * number );
   rating = QUEST_AVE * ( base + num * rand ) +
         QUEST_MAX * ( base + ( num * ( 1 + rand ) ) / 2 );
   scale = BASE + SCALE * NORM;
   rating /= scale;
   rating = ( ( BASE + SCALE * EXPERT ) * rating + QUEST_AVE * QUEST_MAX ) /
         ( 2 * QUEST_AVE * QUEST_MAX );
   if ( rating > scale ) {
      scale = ( rating + scale ) / 2;
   }
   return ({ base / scale, num, rand / scale });
} /* calc_attack() */

/**
 * This method sets up the unarmed attacks on the npc.
 * @param thing the npc to setup the attacks on
 */
void set_unarmed_attacks( object thing ) {
   int i, number;

   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = sqrt( number );

   for ( i = sizeof( attacks ) - 3; i > -1; i -= 3 ) {
      thing->remove_attack( attacks[ i ] );
      thing->add_attack( attacks[ i ], attacks[ i + 1 ],
            calc_attack( attacks[ i + 2 ], number ), A_TYPES[ attacks[ i ] ],
            "unarmed", "unarmed_"+ attacks[ i ] );
   }
} /* set_unarmed_attacks() */

/**
 * This method checks to see if the specified bit of the npc is edible or
 * not.
 * @param bit the bit to check
 * @return 1 if the bit is edible, 0 if not
 * @see add_bit()
 * @see query_bits()
 * @see query_all_inedible()
 * @see remove_bit()
 */
int query_eat( string bit ) {
  if (member_array( bit, inedible) == -1) {
    return 1;
  }
  return 0;
} /* query_eat() */

/**
 * This method checks to see if the bit is unrottable, ie: a bit of
 * skeleton.
 * @param bit the bit to check
 * @return 1 if unrottable, 0 if rottable
 * @see add_bit()
 * @see query_bits()
 * @see query_all_unrottable()
 * @see remove_bit()
 */
int query_unrottable( string bit ) {
  if (member_array( bit, unrottable) == -1) {
    return 0;
  }
  return 1;
} /* query_unrottable() */

/**
 * This method checks to see if the bit is pluckable.
 * @param bit the bit to check
 * @return 1 if pluckable, 0 if not
 * @see add_bit()
 * @see query_bits()
 * @see query_all_pluckable()
 * @see remove_bit()
 */
int query_pluckable( string bit ) {
  if (member_array( bit, pluckable) == -1) {
    return 0;
  }
  return 1;
} /* query_pluckable() */

/**
 * This method returns the array of all the inedible bits of the race.
 * @return all the inedible bits
 * @see add_bit()
 * @see query_bits()
 * @see query_eat()
 * @see remove_bit()
 */
string *query_all_inedible() {
   return inedible;
} /* query_all_inedible() */

/**
 * This method returns the array of all the unrottable bits of the race.
 * @return all the unrottable bits
 * @see add_bit()
 * @see query_bits()
 * @see query_unrottable()
 * @see remove_bit()
 */
string *query_all_unrottable() {
   return unrottable;
} /* query_all_unrottable() */

/**
 * This method returns the array of all the pluckable bits of the race.
 * @return all the pluckable bits of the race
 * @see add_bit()
 * @see query_bits()
 * @see query_pluckable()
 * @see remove_bit()
 */
string *query_all_pluckable() {
   return pluckable;
} /* query_all_pluckable() */


// Very generic. These are the zones
string *query_target_zones() {
  return ({ "head", "head", "neck",
              "chest", "back", "back", "back", "lower back", 
              "left front leg", "left front leg", 
              "stomach", "stomach", 
              "right front leg", "right front leg",
              "left rear leg", "left rear leg",
              "right rear leg", "right rear leg",});
}

string map_target_zone(string zone) {
  switch(zone) {
  case "abdomen":
    return "stomach";
  case "hands":
  case "arms":
    return (random(2) ? "left " : "right ") + "front leg";
  case "feet":
  case "legs":
    return (random(2) ? "left " : "right ") + "rear leg";
  default:
    return zone;
  }
}

string map_armour_zone(string zone) {
  switch(zone) {
  case "thorax":
  case "body":
  case "breast":
  case "trunk":
    return "chest";

  case "tail":
    return "back";
    
  case "stomach":
    return "abdomen";
    
  case "left arm":
  case "right arm":
  case "left front leg":
  case "right front leg":
  case "left petral fin":
  case "right petral fin":
  case "left wing":
  case "right wing":
  case "branches":
    return "arms";

  case "left hand":
  case "right hand":
  case "left middle leg":
  case "right middle leg":
  case "left front paw":
  case "right front paw": 
    return "hands";

  case "left leg":
  case "right leg":
  case "left back leg":
  case "right back leg":
  case "left rear leg":
  case "right rear leg":
  case "dorsal fin":
    return "legs";
    
  case "left foot":
  case "right foot":
  case "left rear paw":
  case "left rear paw":
  case "left rear foot":
  case "left rear foot":
  case "left claw":
  case "right claw":
  case "root":
    return "feet";
    
  default:
    return zone;
  }
}
