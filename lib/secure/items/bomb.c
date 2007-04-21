inherit "/std/object";

void setup() {
  set_name( "bomb" );
  set_short( "bomb of departure" );
  set_long( "This is a handy creator object to get rid of annoying swine.\n" );
  set_main_plural( "bombs" );
  reset_drop();
} /* setup() */

void reset() {
  call_out( "detonate", 2 );
} /* reset() */

void detonate() {
  environment( this_object() )->quit();
  call_out( "detonate", 15 );
} /* detonate() */

void init_dynamic_arg( mapping args ) {
  ::init_dynamic_arg( args );
  call_out( "detonate", 15 );
} /* init_dynamic_arg() */
