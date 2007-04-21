/* Cabbage race object. */

inherit "/std/races/base";

void setup() {
   set_name( "cabbage" );
   set_long("A green, leafy vegetable.  Probably full of vitamins and "+
      "minerals, and plenty of roughage.\n" );
   set_height( 20 );
   set_weight( 40 );
   set_desc( "a cute little green-fronded cabbage.  Awww" );
   set_skin( "fronds" );

   /* throw away the human bits and replace totally */
   bits = ({
      "frond", "frond", ({ 0, 100, ({"/std/bit", 10}) }),
      "stalk", "stalk", ({ 0, 500, 0 })
   });
   
   inedible = ({ });
   unrottable = ({ });

   set_stats( ({ -4, -4, -6, -4, -6 }) );
}  /* end setup */

void player_heart_beat( string word, object brassica ) {
   object *caterpillars;

   if( !environment( brassica ) ) {
      return; 
   }

   caterpillars = match_objects_for_existence( "caterpillar",
      environment( brassica ) );

   if( !sizeof( caterpillars ) ) {
      return;
   }

   if( brassica->caterpillars_here( caterpillars ) ) {
      return;
   }

   brassica->do_command( "gibber" );
   brassica->run_away();
} /* player_heart_beat() */

void set_unarmed_attacks( object thing ) {
   int number;

   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = 4 + sqrt( number ) / 2;

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "fronds" );

   thing->add_attack( "fronds", 160, ({ 4 * number, 5, 3 * number }),
      "sharp", "unarmed", 0 );

   thing->add_attack_message( "fronds", "sharp", ({
      50, "$N scratch$es $P $z with a frond.\n",
      100, "$N jab$s a sharp frond into $P $z.\n",
      140, "$N chew$s on $P $z.\n",
      180, "$N slash$es $P $z with $p fronds.\n",
      230, "$N climb$s onto %I and cuts $P $z with a sharp frond.\n", 
      0, "$N bite$s $P $z off!\n" }) );

   thing->tactics( "response dodge" );
   thing->tactics( "attitude offensive" );
} /* set_unarmed_attacks() */

