/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: dead_eel.c,v 1.2 2001/11/14 13:46:23 siel Exp $
 *
 *
 */


#include <move_failures.h>

#define EEL_SKIN "/obj/misc/eel_skin"

inherit "/obj/food";

void    setup();
void    init();
int     do_skin( object *indirect_obs, string indir_match, string dir_match, 
                 mixed *args, string pattern );
int     test_sharp( object weapon ); 
mixed   query_dynamic_auto_load();
void    init_dynamic_arg(mapping arg, object);

int skinned; /* 1 = the dead eel is already skinned */

void setup()
{
  set_name( "eel" );
  set_short( "dead eel" );
  add_alias( ({ "corpse", "eel corpse" }) );
  add_adjective( "dead" );
  set_main_plural( "dead eels" );
  set_long( function()
	    {
	      if( skinned ) 
		return
		  "This is the dead body of an unfortunate eel.  "
		  "As if being dead wasn't bad enough, it has also "
		  "been skinned.\n";
	      return
		  "This is the dead body of an unfortunate eel.  "
		  "Most people skin such a fish before eating it.\n";
	    } );
  set_weight( 60 ); /* Wrestling eels are big */
  set_value( 200 );
  set_weight_per_bite( 2 );
}

void init()
{
  ::init();
  this_player()->add_command( "skin", this_object(),
        "<direct:object:me-here> with <indirect:object:me>" );
}

int do_skin( object *indirect_obs, string indir_match, string dir_match, 
   mixed *args, string pattern )
{
  object skin;

  if ( skinned ) return notify_fail( "The eel is already skinned.\n" );
  if( !test_sharp( indirect_obs[0] ) )
    {
      tell_room( environment( this_player() ),
	     this_player()->one_short() + " unsuccessfully tries to "
             "skin an eel with " + indirect_obs[0]->one_short() + ".\n",
             this_player() );
      return notify_fail( indirect_obs[0]->the_short() +
            " only makes a shallow mark in the eel skin.  "
            "It would probably be easier to skin the eel "
            "with some kind of sharp implement.\n" );
    }
  skinned = 1;
  skin = clone_object( EEL_SKIN );
  if( MOVE_OK != skin->move( this_player() ) )
        skin->move( environment() );
  tell_room( environment( this_player() ),
        this_player()->one_short() + " skins an eel with "
        + indirect_obs[0]->one_short() + ".\n",
        this_player() );
  return notify_fail( "You skin the dead eel with " +
        indirect_obs[0]->the_short() + ".\n" );
}

int test_sharp( object weapon )
{
  int i;
  mixed *pongo;

  pongo = weapon->weapon_attacks();
  for( i = 1; i < sizeof( pongo ); i++ )
    {
      if( pongo[i] == "sharp" ) return 1; /* Ok, it's a sharp weapon */
    }
  return 0; /* Not a sharp weapon */

} /* test_sharp */

mixed query_dynamic_auto_load() {

  return ([ "::" : ::query_dynamic_auto_load(),
            "skinned" : skinned ]);
} /* query_dynamic_auto_load() */

void init_dynamic_arg(mapping arg, object) {

  ::init_dynamic_arg(arg["::"]);
  skinned = arg["skinned"];

} /* init_dynamic_arg() */
