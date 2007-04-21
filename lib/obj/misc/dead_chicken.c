/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: dead_chicken.c,v 1.6 2001/11/27 12:03:32 siel Exp $
 *
 *
 */


#include <move_failures.h>

#define CHICKEN_FEATHER "/obj/misc/chicken_feather"
#define MIN_FEATHERS 3
#define MAX_FEATHERS 5

inherit "/obj/food";

void    setup();
void    init();
int     do_pluck( object *indirect_obs, string indir_match, string dir_match, 
                 mixed *args, string pattern );
mixed   query_dynamic_auto_load();
void    init_dynamic_arg(mapping arg, object);

int plucked; /* Is the chicken already plucked? */

void setup()
{
  set_name( "chicken" );
  set_short( "dead chicken" );
  add_alias( ({ "corpse", "chicken corpse" }) );
  add_adjective( "dead" );
  set_main_plural( "dead chickens" );
  set_long( function()
            {
              if( plucked ) 
                return
                  "This is the dead body of a bedraggled chicken.  "
                  "Judging from the total lack of feathers it is "
                  "ready to be cooked.\n";
              return
                  "This is the dead body of a bedraggled chicken.  "
                  "Most people would pluck the few feathers on it "
                  "and then cook it.\n";
            } );
  set_weight( 8 );
  set_value( 300 );
  set_weight_per_bite( 2 );
}

void init()
{
  ::init();
  this_player()->add_command( "pluck", this_object(),
        "<direct:object:me-here>" );
}

int do_pluck( object *indirect_obs, string indir_match, string dir_match, 
   mixed *args, string pattern )
{
  object pluck;
  int feathers = random( MAX_FEATHERS - MIN_FEATHERS ) + MIN_FEATHERS;

  if ( plucked ) return notify_fail( "The chicken is already plucked and "
    "bare of any feathers.\n" );

  plucked = 1;
  for ( int i = 0; i < feathers; i++ ) {
    pluck = clone_object( CHICKEN_FEATHER );
    if( MOVE_OK != pluck->move( this_player() ) )
        pluck->move( environment() );
  }
  tell_room( environment( this_player() ),
        this_player()->one_short() + " plucks a dead chicken with reckless "
          "abandon.\n",
        this_player() );
  return notify_fail( "You pluck the bedraggled, dead chicken cheerfully.\n" );
}

mixed query_dynamic_auto_load() {

  return ([ "::" : ::query_dynamic_auto_load(),
            "plucked" : plucked ]);
} /* query_dynamic_auto_load() */

void init_dynamic_arg(mapping arg, object) {

  ::init_dynamic_arg(arg["::"]);
  plucked = arg["plucked"];

} /* init_dynamic_arg() */
