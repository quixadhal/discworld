
/**
 * This is an inherit for grains that can be processed into flour.
 *
 * @creator Vashti
 * @started 21st January 2003
 */
 
// eat mess
// chew command

#include <move_failures.h>
#include <state_change.h>

#define TO this_object()
#define TP this_player()

#define THRESHOLD 5

inherit "/obj/food";

int _max_strip;

/** @ignore yes
 * Grains pretty much all work alike, but all of this can be overridden.
 */
void create() {
  do_setup++;
  ::create();
  do_setup--;

  set_name( "grain" );
  set_short( "broken grain" );
  set_long( "This is some funky, but broken, grain.\n" );
  
  _max_strip = THRESHOLD;
  
  remove_help_file( "food" );
  add_help_file( "grain" );
  
  add_alias( "plant" );
  add_plural( "plants" );

  set_collective();
  
  set_amount( 1 );
  set_weight_unit( ({ 1, 10 }) );

  set_eat_mess( ({
    "$N choke down $D. It absorbs all the moisture from your mouth and sticks "
      "in your throat.  Looks like you just aren't designed to eat unmilled "
      "grain.\n",
    "$N chokes down $D, coughing and rubbing $p throat unhappily.\n"
  }) );
  
  if ( ! do_setup ) {
    TO->setup();
    TO->reset();
  }
} /* create */


/** @ignore yes */
void init() {
  ::init();
  
  TP->add_command( "chew", TO );
  TP->add_command( "strip", TO );
} /* init */


/**
 * This sets the maximum number of items that can be stripped by
 * hand at once.
 */
void set_max_strippable( int num ) {
  _max_strip = num;
} /* set_max_strippable */


/**
 * This returns the maximum number of items that can be stripped
 * by hand at once.
 */
int query_max_strippable() {
  return _max_strip;
} /* query_max_strippable */


/** @ignore yes */
int do_chew() {
  int size;
  
  size = query_amount();
  
  if ( ! size ) {
    add_failed_mess( "You can't chew thin air.\n" );
    return 0;
  }
  
  if ( size > 1 ) {
    add_failed_mess( "You can't chew more than one " + short(0)
      + " at a time.\n" );
    return 0;
  }

  TP->set_position( "idly chewing " + TO->a_short() );
  
  add_succeeded_mess(
    "$N place$s $D between $p teeth, and chew$s it idly.\n" );
  
  return 1;
} /* do_chew */


/** @ignore yes */
int do_strip() {
  int i;
  object stripped;
  
  /* Are they being Silly? */
  if ( query_amount() > query_max_strippable() ) {
    add_failed_mess( "That's far too much $D for you to strip by hand.  "
      "Perhaps you should look for a tool of some kind.\n" );
    return 0;
  }
  
  stripped = STATE_CHANGE->transform( TO, "thresh" );
  if ( ! objectp( stripped ) ) {
    add_failed_mess( "For some reason, you can't strip $D.\n" );
    return 0;
  }
  
  i = stripped->move( TP );
  if ( i != MOVE_OK ) {
    stripped->move( environment( TP ) );
    add_succeeded_mess( "$N strip$s the grain from $D with $p fingers, "
      "spilling some onto the "
      + ( environment( TP )->query_property( "location" ) == "inside" ? "floor"
      : "ground" ) + ".\n" );
  } else {
    add_succeeded_mess( "$N strip$s the grain from $D with $p fingers.\n" );
  }
  
  move( "/room/rubbish" );
  return 1;
} /* do_strip */


mapping query_static_auto_load() {
  return int_query_static_auto_load();
} /* query_static_auto_load */
