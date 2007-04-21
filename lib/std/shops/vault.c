/*
 * $Locker:  $
 * $Id: vault.c,v 1.11 2003/05/23 14:12:40 carmine Exp $
 */

/**
 * Standard vault inheritable object. There are two inheritables (this one and
 * /std/vault_entry.c) which are provided to make it easy to setup your own
 * vaults.
 *
 * This one handles the actual vault room itself. When using this room you
 * must use set_vault_name and set_vault_entrance. You should also define a
 * setup_container function to customise your vault drawers.
 *
 * @author Ceres
 * @see /std/vault_entrance.c
 */
#include <vault.h>

inherit "/std/room/basic_room";

private object _container;
private string _vault_name;
private string _entry;
private string _there;

/**
 * Used to set the exit from this vault room (usually out to the vault entry
 * room. The parameters are just like those of add_exit.
 * @param direction The direction of the exit.
 * @param dest The destination object. Usually the filename of the destination.
 * @param type An exit type as in add_exit.
 * @return void
 * @see add_exit
 */
void set_vault_exit(string direc, mixed dest, string type) {
  _entry = dest;
  add_exit(direc, dest, type);
  modify_exit( direc, ({ "function", "check_done" }) );
  modify_exit( direc, ({ "closed", 1 }) );
}

/**
 * Used to set the name of this vault. The name is used as part of the save
 * file for the vault and should uniquely idenitfy this vault.
 *
 * @param name The name of the vault.
 * @return void
 */
void set_vault_name(string str) { _vault_name = str; }

/**
 * This method initialised a nice container to stuff everything we
 * save into.
 * @param owner the owner to make a container for
 * @return the nice container
 */
object init_container(object person) {
  object container;
  string owner;

  owner = person->query_name();
  container = clone_object( "/obj/cont_save" );
  container->set_max_weight( 1000 );
  container->set_open();
  container->set_stuck( 1 );
  container->set_can_recurse(1);
  container->reset_get();
  container->set_save_file( VAULT_SAVE +_vault_name+ "/"+ owner,
                           person );
  if ( !master()->file_exists( VAULT_SAVE +_vault_name+"/"+ owner +".o" ) ) {
    container->add_property( "owner", owner );
    container->do_save();
  }
  
  container->move(this_object());
  container->add_property("there", "here");
  this_object()->setup_container(container);
  container->add_property( "owner", owner );
  _there = container->query_property("there");
  
  return container;
} /* init_container() */

/* Call this from vault_entry since this_player() cannot be trusted. */
/**
 * @ignore yes
 */
void event_enter(object thing, string mess, object from) {
  string owner;

  if( !userp( thing ) )
    return;

  owner = thing->query_name();

   // If there's already a container here figure out if we need to dest it
   // and create a new one.
   // Don't dest it if the owner is in the vault and the person entering
   // is a creator -- useful for debugging.
  if ( _container ) {
    if((string)_container->query_property( "owner" ) == owner)
      return;
    else if(thing->query_creator() &&
            find_player(_container->query_property( "owner" )) &&
            environment(find_player(_container->query_property( "owner" ))) ==
            this_object())
         return;
      else {
        //remove_hidden_object( _container );
        _container->dest_me();
        _container = 0;
      }
  }

  _container = init_container(thing);
  call_out("fix_owner", 2, owner);
} /* event_enter() */

/**
 * @ignore yes
 *
 * This seems to be necessary coz the drawer's save their properties and some
 * don't have an owner property set! It's a hack but it was the best I could
 * come up with at the time.
 */
void fix_owner(string owner) {
  if (_container) {
     _container->add_property( "owner", owner );
     if(_there) {
       _container->add_property("there", _there);
     }
  }
}

/**
 * @ignore yes
 */
int check_done( string verb, object thing, string special ) {
  if ( thing->query_auto_loading() ) {
    tell_room( this_object(), "The door swings shut.\n" );
    tell_object( thing, "Your vaults's inventory is still being generated.\n"
                "Please wait until it is completed.\n" );
      return notify_fail( "" );
   }
   return 1;
} /* check_done() */

/**
 * @ignore yes
 */
void event_exit( object thing, string message, object to ) {
  if(_container && _container->query_property("owner") == thing->query_name()) {
    //remove_hidden_object(_container);
    _container->move("/room/void");
    call_out((: _container->dest_me() :), 1);
  }
  if(interactive(thing))
    thing->save_me();
}

/**
 * @ignore yes
 */
string query_quit_handler() {
  return _entry;
}

/**
 * Define this function in your vault room to perform setup on the vault
 * container. Use it to customise the name, short, long, size etc. of your
 * drawers (or whatever your vault uses.
 *
 * @param container The object to perform your customisation on.
 * @return void
 */
void setup_container(object container) {
  container->set_name( "drawer" );
  container->set_short( "drawer" );
  container->add_alias( "drawer" );
  container->set_long("The single open drawer is sticking straight "
                      "out of the wall like...  well, like a drawer sticking "
                      "straight out of the wall.  " );
}

/**
 * @ignore yes
 * Make sure they can't draw circles or erect tents here.
 */
int trap_action( string str ) {
   string st1;

   if( sscanf( str,"%s %s", str, st1 ) );
      switch(str) {
      case "circle":
         tell_object( this_player(),
               "Something tells you that drawing a circle in this location "
               "would be a bad idea.\n");
         return 1;
      case "erect":
         tell_object( this_player(),
               "You can't seem to get the tent to unfold properly.  "
               "Perhaps you should try it somewhere else.\n");
         return 1;
      default:
         return 0;
    }
} /* trap_action() */

/** @ignore yes */
void event_net_dead(object ob) {
   // Toss them out...
   ob->move_with_look(_entry);
} /* event_net_dead() */

/**
 * @ignore yes
 */
void init() {
   ::init();
   this_player()->command_override( (: trap_action :) );
} /* init() */


/**
 * @ignore yes
 */
int query_vault_room() {
   return 1;
} /* query_vault_room() */