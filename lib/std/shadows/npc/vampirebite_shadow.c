/* Lemming, 03/07/2000 */


inherit "/std/effect_shadow";

object vamp_corpse( object corpse ) {
   string player_name;

   if( corpse && objectp( corpse )
       && !corpse->query_property( "vampire corpse" )
       && corpse->query_race_name() == "human" ) {
      corpse->add_property( "no burial", 1 );
      corpse->add_property( "vampire corpse", 1 );

      if( stringp( player->query_property( "determinate" ) ) )
         player_name = player->query_property( "determinate" ) +
            player->query_short();
      else
         player_name =
            "/global/events"->convert_message( player->a_short() );

      corpse->add_effect( "/std/effects/npc/vampirecorpse_effect",
         ({ player_name, copy( player->query_gender() ),
            copy( player->query_level() ), 0 }) );
   }

   return corpse;
} /* vamp_corpse() */

varargs object do_death( object thing, object weapon, string attack ) {
   int *effs;
   object corpse;

   effs = player->effects_matching( "body.bite.vampire" );
   call_out( "remove_this_effect", 2 );

   corpse = player->do_death( thing, weapon, attack );
   vamp_corpse( corpse );

   return corpse;
} /* do_death() */

object make_corpse() {
   object corpse;

   corpse = player->make_corpse();
   vamp_corpse( corpse );

   return corpse;
} /* make_corpse() */
