inherit "/std/effect_shadow";

/** @ignore */
mixed move( mixed dest, string messin, string messout ) {
   int ret;

   ret = player->move( dest, messin, messout );
   if( ret == 0 ) {
      call_out( "find_corpse", 0 );
   }

   return ret;
} /* move() */

/** @ignore */
string *query_valid_bits( object corpse ) {
   object race_obj;
   string *edible_bits;

   if( !corpse )
      return ({ });

   if( !corpse->query_corpse() )
      return ({ });

   edible_bits = corpse->query_edible_bits_left();
   if( !sizeof( edible_bits ) )
      return ({ });

   race_obj = corpse->query_race_ob();
   if( !race_obj )
      return ({ });

   return filter( edible_bits,
                  (: sizeof( $(race_obj)->query_bit($1)[2] ) == 3 :) );
} /* query_valid_bits() */

/** @ignore */
protected void find_corpse() {
   object ob, *obs;

   obs = all_inventory( environment(player) );

   foreach( ob in obs ) {
      if( sizeof( query_valid_bits(ob) ) ) {
         remove_call_out( "eat_corpse" );
         call_out( "eat_corpse", 1 + random(120), ob );
         break;
      }
   }
} /* find_corpse() */

/** @ignore */
protected void eat_corpse( object corpse ) {
   string which_bit;
   string *valid_bits;

   if( !corpse ) {
      call_out( "find_corpse", 1 + random(2*60) );
      return ;
   }

   valid_bits = query_valid_bits( corpse );

   if( sizeof( valid_bits ) && environment(corpse) == environment(player) ) {
      which_bit = valid_bits[ random( sizeof( valid_bits ) ) ];
      corpse->add_bit_gone( which_bit );

      tell_room( environment( player ), "$C$" + player->the_short() +
         " tears the " + which_bit + " from " + corpse->the_short() + " and "
         "begins eating.\n" );
   }

   if( sizeof( valid_bits ) > 1 ) {
      remove_call_out( "eat_corpse" );
      call_out( "eat_corpse", 1 + random(2*60), corpse );
   } else {
      call_out( "find_corpse", 1 + random(2*60) );
   }
} /* eat_corpse() */

/** @ignore */
void event_enter( object ob, string mess, object from ) {
   player->event_enter( ob, mess, from );

   if( sizeof( query_valid_bits(ob) ) ) {
      remove_call_out( "eat_corpse" );
      call_out( "eat_corpse", 10, ob );
   }
} /* event_enter() */
