/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: surface.c,v 1.17 2002/11/18 01:46:35 ceres Exp $
 *
 *
 */
/**
 * This is a furniture item for making all kinds of furniture.
 */
#include <move_failures.h>
#include <player.h>

inherit "/std/object";
inherit "/std/basic/cute_look";
inherit "/std/basic/item_chat";
inherit "/std/room/furniture/inherit/furniture_base";

private nosave int _loc_weight, _max_weight;
private nosave string *_shadows;
private nosave object _player;
private nosave string *_aliases;
private nosave int _use_internals;

//#define DEBUG

void create() {
  do_setup++;
  furniture_base::create();
  object::create();
  
  add_alias( "furniture" );
  add_plural( "furniture" );
  set_opaque();
  
  do_setup--;
  if ( !do_setup ) {
    this_object()->setup();
  }
  // Don't respond to 'get all'
  set_no_all(1);
} 

/**
 * @ignore yes
 */
void init() {
  string alias;

  if(_aliases)
    foreach(alias in _aliases) {
      add_command(alias, "<indirect:object:me> {in|on|into} <indirect:object>",
                  (: "/cmds/living/put"->cmd($1, $3, $4) :) );
    }
  ::init();
}

/**
 * Set aliases for 'put' for this object.  This allows you to setup an item
 * so that things can be hung from it, or stood on it or somesuch.
 *
 * @param aliases aliases for the verb 'put'.
 */
void set_put_aliases(string *aliases) {
  _aliases = aliases;
}

/**
 * This sets the flag to say if the object will allow things on it to be
 * used.  This is useful for tables and so on.
 * @param allow 1 if they are allowed to be used, 0- if not
 */
void set_allow_use_internals(int allow) {
   _use_internals = allow;
} /* set_allow_use_internals() */

/**
 * This method returns the flag to say if the object will allow things on it
 * to be used.
 * @return 1 if it is allowed, 0 if not
 */
int query_allow_use_internals() {
   return _use_internals;
} /* query_allow_use_internals() */

/** @ignore yes */
/* Furniture is much harder to carry than just its weight, therefore we
 * multiply its weight by 5 to determine if someone can carry it.
 */
varargs int move( mixed dest, string messin, string messout ) {
  int i, w;
  object from;

  if (!move_check(dest)) {
     write(the_short() + " resists being picked up and shocks you slightly.\n");
     return MOVE_INVALID_DEST;
  }

  w = (int)this_object()->query_complete_weight() * 5;
  if ( from = environment() ) {
    from->add_weight( -w );
  }
  if(!dest) {
    return MOVE_INVALID_DEST;
  }

  if (!( dest->add_weight( w ) ) ) {
    if ( from ) {
      from->add_weight( w );
    }
    return MOVE_TOO_HEAVY;
  }
  if ( stringp( dest ) ) {
    dest = find_object( dest );
  }
  i = ::move( dest, messin, messout );
  if ( i != MOVE_OK ) {
    if ( from ) {
      from->add_weight( w );
    }
    dest->add_weight( -w );
  } else {
    if (_use_internals) {
      if (from) {
        from->remove_use_internal_object(this_object());
      }
      dest->add_use_internal_object(this_object());
    }
    if (query_light_needs_inform()) {
      if ( from ) {
        inform_of_light_level_change(from);
      }
      inform_of_light_level_change(dest);
    }
  }
  return i;
} /* move() */

/** @ignore yes
 * This stops people dragging items away if they aren't allowed to.
 */
int get(mixed dest) {
  if(!move_check(dest)) {
    write("You feel it would be wrong to take " + the_short() + ".\n");
    return 2;
  }
  return ::get(dest);
}

/**
 * @ignore yes
 * Makes sure furniture is removed from the save file
 * when its removed from this room.
 */
int test_remove(object thing, int flag, mixed dest) {
  mixed *stuff;
  object thief;
  
  if(environment() && this_player()) {
    thief = this_player();
    if(thief->query_caster())
      thief = find_player(thief->query_caster());
    else if(thief->query_owner())
      thief = thief->query_owner();
    
    stuff = thing->query_property("dropped");
    
    /*
     * It's theft if the item is being removed by someone other than the
     * occupier and someone other than the person who dropped it.
     */
    if(function_exists("test_occupier", environment()) &&
       !environment()->test_occupier(thief->query_name()) &&
       (!sizeof(stuff) || stuff[0] != thief->query_name()) &&
       thief && environment(thief)) {
      event(environment(thief), "theft", thief, this_object(), ({ thing }));
    }
    
#ifdef DEBUG
    if(base_name(environment()) != "/room/rubbish")
      log_file(base_name(environment()) + ".log",
               "%s %s moved from %s to %s.\n", ctime(time())[4..18],
               thing->query_short(), this_object()->query_short(),
               dest->query_short());
#endif
    event(environment(), "save");
  }
  
  return 1;
}

/** @ignore yes */
int test_add( object thing, int flag ) {
  if(environment())
    event(environment(), "save");

#ifdef DEBUG
  if(environment() && base_name(environment()) != "/room/rubbish")
    log_file(base_name(environment()) + ".log",
             "%s %s put on %s by %s.\n", ctime(time())[4..18],
             thing->query_short(), this_object()->query_short(),
             this_player() ? this_player()->query_name() : "Unknown");
#endif

  return 1;
} /* test_add() */

void set_max_weight(int arg) { _max_weight = arg; }
int query_max_weight() { return _max_weight; }
int query_loc_weight() { return _loc_weight; }
int query_has_surface() { return 1; }

/** @ignore yes */
void update_loc_weight() {
  int i;
  object *contents;
  _loc_weight = 0;
  contents = all_inventory( this_object() );
  for ( i = 0; i < sizeof( contents ); i++ )
    _loc_weight += (int)contents[ i ]->query_complete_weight();
} /* update_loc_weight() */

int query_complete_weight() {
  return ::query_complete_weight() + query_loc_weight();
} /* query_complete_weight() */

int add_weight( int n ) {
  if ( !_max_weight ) return 1;
  if ( n + _loc_weight > _max_weight ) return 0;
  if ( !environment() ) {
    _loc_weight += n;
    return 1;
  }
  if ( !( environment()->add_weight( n ) ) ) return 0;
  _loc_weight += n;
  return 1;
} /* add_weight() */

int transfer_all_to(object dest) {
  object *ob;
  int i;

  ob = all_inventory(this_object());
  for (i=0;i<sizeof(ob);i++)
    ob[i]->move(dest);
  if (first_inventory(this_object())) return 0;
  return 1;
} /* transfer_all_to() */

string long( string word, int dark ) {
   object *things;
   things = (object *)this_object()->find_inv_match( "all", this_player() );
   if ( !sizeof( things ) )
      return object::long( word, dark ) +"There is nothing on "+
            the_short() +".\n";
   return object::long( word, dark ) + query_contents( "On " +
            the_short() +" "+ ( sizeof( things ) == 1 ? "is " :
            "are " ), things );
} /* long() */

object *find_inv_match( string words, object looker ) {
   int i;
   object *things;

   things = all_inventory( this_object() );
   for ( i = sizeof( things ) - 1; i > -1; i-- ) {
      if ( !things[ i ]->short( 0 ) ) {
         things = delete( things, i, 1 );
         continue;
      }
      if ( !looker )
         continue;
      if ( !things[ i ]->query_visible( looker ) )
         things = delete( things, i, 1 );
   }
   return things;
} /* find_inv_match() */

void set_player( object thing ) { _player = thing; }
object query_player() { return _player; }

mixed stats() {
  return ::stats() + ({
    ({ "loc_weight", query_loc_weight(), }),
    ({ "max_weight", query_max_weight(), }),
  });
} /* stats() */

mapping int_query_static_auto_load() {
  return ([
    "::" : ::int_query_static_auto_load(),
    "max weight" : _max_weight,
    "allowed positions" : query_allowed_positions(),
  ]);
} /* int_query_static_auto_load() */

mapping query_dynamic_auto_load() {
  mixed inventory;

  // Try the main player object.
  if (!inventory) {
    catch(inventory = PLAYER_OB->
        create_auto_load( all_inventory( this_object() ) ) );
  }

  return ([ "::" : ::query_dynamic_auto_load(),
            "inv" : inventory,
            "bonded" : query_bonded()
         ]);
} /* query_dynamic_auto_load() */

/**
 * This method handles moving objects into the inventory from an auto
 * load.  This should be over ridden by things inheriting us to
 * make sure that the objects can be moved into the inventory.  It
 * should handle the bypassing of open/close/locked etc flags.
 * @param ob the object to move into ourselves
 */
int handle_restore_inventory(object ob) {
  return (int)ob->move(this_object());
} /* handle_restore_inventory() */

void init_dynamic_arg( mapping bing ) {
  function f;

  if ( bing[ "::" ] ) {
    ::init_dynamic_arg( bing[ "::" ] );
  }
  if ( bing[ "inv" ] ) {
    f = (: handle_restore_inventory($1) :);
    if (_player) {
      _player->load_auto_load_to_inventory( bing["inv"], this_object(),
                                            _player, f);
    } else {
      PLAYER_OB->load_auto_load_to_inventory( bing["inv"], this_object(),
                                              this_player(), f);
    }
  }
  set_bonded(bing["bonded"]);
} /* init_dynamic_arg() */

void init_static_arg(mapping bing) {
  if (bing["::"]) {
     ::init_static_arg(bing["::"]);
  }
  if (!undefinedp(bing["max weight"])) {
    _max_weight = bing["max weight"];
  if(!undefinedp(bing["allowed_positions"]))
    set_allowed_positions(bing["allowed_positions"]);
  }
} /* init_static_arg() */

/** @ignore yes */
mixed query_static_auto_load() {
  if ( base_name(this_object()) + ".c" == __FILE__ )
    return int_query_static_auto_load();
  return ([ ]);
} /* query_static_auto_load() */

void dest_me() {
  int i;
  object shadow;

  for ( i = 0; i < sizeof( _shadows ); i++ ) {
    if ( shadow = find_object( _shadows[ i ] ) ) {
      shadow->destruct_shadow( shadow );
    }
  }
  all_inventory()->dest_me();
  object::dest_me();
} /* dest_me() */

/** @ingore yes */
string query_help_file_directory() {
   return "/doc/furniture/";
} /* query_help_file_directory() */
