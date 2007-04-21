inherit "obj/armour";

string owner, giver;

void setup() {
   set_name( "ring" );
   set_short( "wedding ring" );
   add_adjective( "wedding" );
   set_long( "This is a wedding ring crafted from the purest gold from the "+
      "mines of Rimward Klatch.\n" );
   set_main_plural( "wedding rings" );
   set_type( "ring" );
   add_property( "shop type", "jewellers" );
   add_property( "no recycling", 1 );
   set_weight( 1 );
   setup_armour( 1000 );
   set_percentage( 100 );
   set_damage_chance( 0 );
   owner = giver = "nobody";
   set_wear_remove_func( file_name( this_object() ), "wear_remove" );
} /* setup() */

void do_inscription() {
   string language;
   object player;

   /*
    * Inscribed in the language of the giver, unless
    * they're not online.
    */
   player = find_player( giver );

   if( !player ) {
      player = find_player( owner );
   }
   
   language = player->query_default_language();

   set_read_mess( "\"For you, "+ capitalize( owner ) +
         ", with eternal love, "+ capitalize( giver ) +".\"", language );
} /* do_inscription() */

string query_owner() { return owner; }

void set_owner( string word ) {
  owner = lower_case( word );
  do_inscription();
} /* set_owner() */

string query_giver() { return giver; }

void set_giver( string word ) {
  giver = lower_case( word );
  do_inscription();
} /* set_giver() */

void wear_remove( int wear ) {
   // This is to prevent people getting "The ring feels uncomfortable
   // on your finger" when they kill a married PK or steal their ring.
   if( environment() != this_player() )
      return;

   if ( living( environment() )  &&  wear ) {
      if ( (string)this_player()->query_name() != owner )
         write( "The ring feels uncomfortable on your finger.\n" );
      else
         write( "You are warmed anew by "+ capitalize( giver ) +"'s love.\n" );
   }
} /* wear_remove() */

mixed *stats() {
   return ::stats() + ({
      ({ "owner", owner }),
      ({ "giver", giver }),
   });
} /* stats() */

void init_dynamic_arg( mapping args ) {
   if ( args[ "::" ] )
      ::init_dynamic_arg( args[ "::" ] );
   if ( args[ "owner" ] )
      owner = lower_case( args[ "owner" ] );
   if ( args[ "giver" ] )
      giver = lower_case( args[ "giver" ] );
} /* init_dynamic_args() */

mapping query_dynamic_auto_load() {
   return ([
      "::" : ::query_dynamic_auto_load(),
      "owner" : owner,
      "giver" : giver
   ]);
} /* query_dynamic_auto_load() */
