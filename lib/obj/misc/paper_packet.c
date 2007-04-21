inherit "/obj/baggage";

void setup() {
   set_name( "packet" );
   set_short( "waxed paper packet" );
   add_adjective( ({ "waxed", "paper" }) );
   set_main_plural( "waxed paper packets" );
   set_long( "This is a small packet, rather like an envelope, of thick "+
         "paper that has been waxed to make it more resilient.\n" );
   set_weight( 1 );
   set_value( 10 );
   set_max_weight( 3 );
   set_transparent();
   add_property("writeable", 1);
} /* setup() */

int test_add( object thing, int flag ) {
   string word;
   object *things;
   if ( !this_player() )
      return ::test_add( thing, flag );
   things = all_inventory( this_object() );
   if ( !sizeof( things ) )
      return ::test_add( thing, flag );
   word = (string)things[ 0 ]->query_medium_alias();
   if ( word == (string)thing->query_medium_alias() )
      return ::test_add( thing, flag );
   write( "You don't want to contaminate "+ ( stringp( word ) ?
         (string)things[ 0 ]->the_short() :
         (string)thing->the_short() ) +", do you?\n" );
   return 0;
} /* test_add() */

object query_substance() {
   object *things;
   things = all_inventory( this_object() );
   if ( !sizeof( things ) )
      return 0;
   if ( !stringp( (string)things[ 0 ]->query_medium_alias() ) )
      return 0;
   return things[ 0 ];
} /* query_substance() */

string short( int dark ) {
   object thing = query_substance();
   if ( dark || !objectp( thing ) )
      return ::short( dark );
   return ::short( dark ) +" of "+ (string)thing->short( dark );
} /* short() */

string query_plural( int dark ) {
   object thing = query_substance();
   if ( dark || !objectp( thing ) )
      return ::query_plural( dark );
   return ::query_plural( dark ) +" of "+ (string)thing->short( dark );
} /* query_plural() */

string *parse_command_id_list() {
   object thing = query_substance();
   if ( !objectp( thing ) )
      return ::parse_command_id_list();
   return ::parse_command_id_list() +
         ({ ::short( 0 ) +" of "+ (string)thing->short( 0 ) });
} /* parse_command_id_list() */

string *parse_command_plural_id_list() {
   object thing = query_substance();
   if ( !objectp( thing ) )
      return ::parse_command_plural_id_list();
   return ::parse_command_plural_id_list() +
         ({ ::query_plural( 0 ) +" of "+ (string)thing->short( 0 ) });
} /* parse_command_plural_id_list() */

string *parse_command_adjectiv_id_list() {
   object thing = query_substance();
   if ( !objectp( thing ) )
      return ::parse_command_adjectiv_id_list();
   return ::parse_command_adjectiv_id_list() +
         (string *)thing->parse_command_adjectiv_id_list() +
         (string *)thing->parse_command_id_list();
} /* parse_command_adjectiv_id_list() */
