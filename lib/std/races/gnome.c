inherit "/std/races/humanoid";
 
void setup() {
   set_name( "gnome" );
   set_long( "This is the race for gnomes.\n" );
   set_desc( "a short-looking gnome" );
   set_weight( 25 );
   set_height( 20 );

   set_sight( ({ 0, 5, 150, 200 }) );
   set_stats( ({ -2, 8, 0, 6, 0 }) );
} /* setup() */
 
/* 
int query_skill_bonus( int lvl, string skill ) {
   if( skill == "other.valueing.gems" )
      return 30 + random( 70 );
   if( skill == "other.woodland" )
      return 10 + random( 50 );

   return 0;
}  query_skill_bonus() */
