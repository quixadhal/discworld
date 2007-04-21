inherit "/std/races/insect_base";
 
void setup() {
   set_name( "caterpillar" );
   set_long( "The elongated wormlike larva of a butterfly or moth.\n" );
   set_height( 5 );
   set_weight( 2 );
   set_desc( "a caterpillar, crawling along the ground" );

   set_stats( ({ -6, -6, -6, -6, -6 }) );
} /* setup() */
 
void player_heart_beat( string word, object eater ) {
   int i, time;
   object flowers, *things;

   if( !environment( eater ) ) return;
   if( eater->query_property( "eating" ) ) return;
   things = all_inventory( environment( eater ) ) - ({ eater });
   if( !sizeof( things ) ) return;

   for( i = 0; i < sizeof( things ); i++ ) {
      if( !(int)things[ i ]->id( "flowers" ) ) {
         things -= ({ things[ i ] });
         i--;
      } else {
         if( things[ i ]->query_property( "being eaten" ) ) {
            things -= ({ things[ i ] });
            i--;
         }
      }
   }

   if( !sizeof( things ) ) return;
   flowers = things[ random( sizeof( things ) ) ];

   tell_room( environment( eater ), "The " + (string)eater->query_short() +
      " discovers the " + (string)flowers->query_short() + " and starts to "
      + "munch.\n" );

   eater->add_property( "eating", 1 );
   flowers->add_property( "being eaten", 1 );
   time = (int)flowers->query_weight();
   time += random( time );

   call_out( "done_eating", time, eater, flowers );
} /* player_heart_beat() */
 
void done_eating( object eater, object flowers ) {
   if( !eater ) return;
   eater->remove_property( "eating" );
   if( !environment( eater ) ) return;
   if( !flowers ) return;
   if( member_array( flowers, all_inventory( environment( eater ) ) ) == -1 )
      return;

   tell_room( environment( eater ), "The " + (string)eater->query_short() +
      " finishes the " + (string)flowers->query_short() + " and burps " +
      "contentedly.\n" );

   flowers->dest_me();
} /* done_eating() */
