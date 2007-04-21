inherit "/std/races/insect_base";
 
void setup() {
   set_name( "insect" );
   set_long( "A nondescript insect.\n" );
   set_height( 5 );
   set_weight( 2 );
   set_desc( "a nondescript insect" );

   set_stats( ({ -6, 4, -6, -6, -6 }) );
} /* setup() */
 
int query_eat( string type ) {
   return ( member_array( type, inedible ) == -1 );
} /* query_eat() */

void new_set_level( mixed *args ) {
   int level;
   object thing;

   if( sizeof(args) < 2 || !args[1] )
      return;

   level = args[ 0 ];
   thing = args[ 1 ];

   thing->add_skill_level( "other.health", level * 2 );
   thing->add_ac( "blunt", "blunt", level / 2 );
   thing->add_ac( "pierce", "pierce", level / 4 );
   thing->add_ac( "sharp", "sharp", level / 3 );
} /* new_set_level() */

int query_dark( int light ) { return 0; }

void set_unarmed_attacks( object thing ) {
   int number;

   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = 4 + sqrt( number );

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "bite" );

   thing->add_attack( "bite", 75, ({ 3 * number, 10, number }),
      "pierce", "unarmed", 0 );

   thing->tactics( "response dodge" );
} /* set_unarmed_attacks() */
