inherit "/std/races/fish_base";

void setup() {
   set_name( "shark" );
   set_long( "A big mean shark.\n" );
   set_height( 15 );
   set_weight( 8000 );
   set_desc( "a fish, the kind with fins" );

   set_stats( ({ 10, 14, 2, 21, -4 }) );
} /* setup() */


void check_water( object player ) {
   if( player && !environment( player )->query_water() )
      call_out( "do_drown", 4, player );
} /* check_water() */


void do_drown( object player ) {
   int i;

  if(!player || !environment(player) || environment(player)->query_water() )
      return;

   tell_room( environment( player ) , player->one_short() + " gasps for "
      "air.\n" );

   i = player->query_max_hp() / 5;
   if( i < 20 ) i = 20;
   player->adjust_hp( -i );

   call_out( "do_drown", 4, player );
} /* do_drown() */


void start_player( object thing ) {
   ::start_player( thing );

   thing->add_property( "gills", 1 );
   //thing->add_property("lives in water", 1);
   thing->add_enter_commands( (: check_water :) );
   thing->set_default_position( ({ "lying" }) );
   thing->return_to_default_position( 0 );
} /* start_player() */


void set_unarmed_attacks( object thing ) {
   int number;

   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = 4 + sqrt( number ) * 2;

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );
   thing->remove_attack( "bite" );

   thing->add_attack( "bite", 100, ({ 2 * number, 5, number }),
      "pierce", "unarmed", 0 );

   thing->tactics( "response dodge" );
} /* set_unarmed_attacks() */


//int lives_in_water() { return 1; }
