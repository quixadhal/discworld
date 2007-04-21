inherit "/std/races/humanoid";

void setup() {
   set_name( "ape" );
   set_long( "The apes are any members of the family Pongidae of large "
      "semi-erect primates, including the chimpanzee, gorilla, orangutan "
      "and gibbon.\n" );
   set_weight( 2500 );
   set_height( 175 );

   set_stats( ({ 4, 4, 0, 4, 0 }) );
} /* setup() */

int query_eat( string type ) { return 1; }

string query_desc( object thing ) {
   if( (int)thing->query_gender() == 1 )
      return "He is your average hairy young male ape.\n";

   return "She is your average hairy young female ape.\n";
} /* query_desc() */

void new_set_level( mixed *args ) {
   int level;
   object thing;

   if( sizeof(args) < 2 || !args[1] )
      return;

   level = args[ 0 ];
   thing = args[ 1 ];

   thing->add_ac( "blunt", "blunt", 15 + level / 10 );
   thing->add_ac( "sharp", "sharp", 10 + level / 15 );
   thing->add_ac( "pierce", "pierce", 12 + level / 12 );

   thing->reset_all();
} /* new_set_level() */

int query_dark( int light ) {
   if( light < 10 ) return 1;
   if( light > 200 ) return 2;

   return 0;
} /* query_dark() */

void set_unarmed_attacks( object thing ) {
   int number;

   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = 10 + ( 3 * sqrt( number ) ) / 2;

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->add_attack( "hands", 75, ({ 2 * number, 8, number }),
      "blunt", "unarmed", "unarmed_hands" );
   thing->add_attack( "feet", 25, ({ 3 * number, 10, number }),
      "blunt", "unarmed", 0 );
} /* set_unarmed_attacks() */
