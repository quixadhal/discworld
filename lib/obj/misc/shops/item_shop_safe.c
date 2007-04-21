inherit "/obj/baggage";

void set_max_float( int );
int query_max_float();

void setup() {
  set_name( "safe" );
  set_short( "cash safe" );
  set_long( "This safe is used to keep the shop's float in.  It's securely set "
    "in the wall.\n" );
  add_adjective( "float" );
  add_property( "there", "in the wall" );
  set_difficulty( 10 );

  reset_get(); 
  set_closed();
  set_locked();

  set_key( "non-existent key" );
  add_property( "no recycling", 1 );
  set_pick_skill( "covert.lockpick.safes" );

  set_weight( 2000 );
  set_max_weight( 5000 );

  set_value( 120000 );
} /* setup() */

int pick_unlock( object player ){
  this_object()->set_unlocked();
    return 1;
} /* pick_unlock() */

int query_accept_money() {
  return 1;
}
