/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: dead_duck.c,v 1.5 2001/11/27 12:03:41 siel Exp $
 *
 *
 */


#include <move_failures.h>

#define DUCK_FEATHER "/obj/misc/duck_feather.ob"
#define MIN_FEATHERS 3
#define MAX_FEATHERS 5

inherit "/obj/food";

void    setup();
void    init();
int     do_pluck( object *indirect_obs, string indir_match, string dir_match, 
                 mixed *args, string pattern );
mixed   query_dynamic_auto_load();
void    init_dynamic_arg(mapping arg, object);

int plucked; /* Is the duck already plucked? */

void setup()
{
  set_name( "duck" );
  set_short( "dead duck" );
  add_alias( ({ "corpse", "duck corpse" }) );
  add_adjective( "dead" );
  set_main_plural( "dead ducks" );
  set_long( function()
            {
              if( plucked ) 
                return
                  "This is the dead body of a bedraggled duck.  Judging "
                  "from the lack of feathers it is ready to be "
                  "cooked.\n";
              return
                  "This is the dead body of a bedraggled duck.  "
                  "Most people would pluck it and then cook it.\n";
            } );
  set_weight( 10 );
  set_value( 400 );
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

  if ( plucked ) return notify_fail( "The duck is already plucked and "
    "bare of any feathers.\n" );

  plucked = 1;
  for ( int i = 0; i < feathers; i++ ) {
    pluck = clone_object( DUCK_FEATHER );
    if( MOVE_OK != pluck->move( this_player() ) )
        pluck->move( environment() );
  }
  tell_room( environment( this_player() ),
        this_player()->one_short() + " plucks a dead duck with reckless "
          "abandon.\n",
        this_player() );
  return notify_fail( "You pluck the bedraggled, dead duck cheerfully.\n" );
}

mixed query_dynamic_auto_load() {

  return ([ "::" : ::query_dynamic_auto_load(),
            "plucked" : plucked ]);
} /* query_dynamic_auto_load() */

void init_dynamic_arg(mapping arg, object) {

  ::init_dynamic_arg(arg["::"]);
  plucked = arg["plucked"];

} /* init_dynamic_arg() */
