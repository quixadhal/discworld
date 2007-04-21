inherit "/std/object";

void create() {
   do_setup++;
   ::create();
   do_setup--;
   set_name( "label" );
   set_short( "small label" );
   add_adjective( "small" );
   set_main_plural( "small labels" );
   set_long( "This is a small label made from paper with some sort of "+
         "dried glue smeared on one side.  You could probably \"stick\" "+
         "it onto something.\n" );
   set_value( 100 );
   add_property( "writeable", 1 );
   add_property( "label", 1 );
   set_max_size( 200 );
   if ( !do_setup )
      this_object()->setup();
} /* create() */

void init() {
   this_player()->add_command( "stick", this_object(),
         "<direct:object:me> to <indirect:object>" );
} /* init() */

int do_stick( mixed *things ) {
   if ( sizeof( things ) > 1 ) {
      add_failed_mess("You can't stick $D to more than one thing at a time.\n");
      return 0;
   }
   if ( things[ 0 ] == this_object() ) {
      add_failed_mess("You cannott stick $D to itself!\n");
      return 0;
   }
   things[ 0 ]->add_effect( "/std/effects/object/label", this_object() );
/* 
   write( "You lick "+ the_short() +" and stick it to "+
         (string)things[ 0 ]->the_short() +".\n" );
   say( (string)this_player()->one_short() +" licks "+ a_short() +
         " and sticks it to "+ (string)things[ 0 ]->the_short() +".\n" );
*/
   add_succeeded_mess("$N lick$s $D and stick$s it to $I.\n", things[0..0]);
   return 1;
} /* do_stick() */

mapping query_static_auto_load() {
   if ( explode( file_name( this_object() ), "#" )[ 0 ] != "/obj/misc/label" ) {
      return ([ ]);
   }
   return int_query_static_auto_load();
} /* query_static_auto_load() */
