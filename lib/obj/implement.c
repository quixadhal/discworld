/*  -*- LPC -*-  */
/*
 * $Id: weapon.c,v 1.24 2003/03/12 22:15:58 ceres Exp $
 */
inherit "/std/object";
inherit "/std/basic/holdable";

#include "money.h"
#include <move_failures.h>
#include <virtual.h>

/**
 * This is a generic held implement used for cutlery, lockpicks, tissues,
 * basically anything you could hold in your hand but which would not be a
 * weapon. Now of course you could try and fight someone with a dessert
 * spoon but it would be pretty silly so use this instead of a weapon.
 *
 * Best way to use this is via the .imp virtual object.
 *
 * @author Ceres
 * @date April 2003
 */
string *un_modifyable;
mixed *wield_func;

void create() {
   holdable::create();
   object::create();
   add_alias("implement");
   add_alias("utensil");
   add_plural("implements");
   if (!query_property("shop type")) {
      add_property("shop type", "general");
   }
}

/** @ignore yes */
varargs int move( mixed dest, string messin, string messout ) {
   int ret;
   int limb;
   object holder;

   //
   // Make sure we revert back to how we were before the move.  If the
   // move fails we should not be changing our state.
   //
   limb = query_my_limb();
   holder = query_holder();
   ret = holdable::move(dest);
   if (ret != MOVE_OK) {
      return ret;
   }
   ret = object::move( dest, messin, messout );
   if (ret != MOVE_OK && holder) {
      holder->set_hold(this_object(), limb);
   }
   return ret;
} /* move() */

void dest_me() {
  holdable::dest_me();
  object::dest_me();
} /* dest_me() */

void player_wield(int pos) {
  if (!environment()) {
    return;
  }
  this_object()->hold_item( environment(), pos );
} /* player_wield() */

mapping query_static_auto_load() {
   if ( base_name( this_object() ) == "/obj/implement" )
     return int_query_static_auto_load();
   return 0;
} /* query_static_auto_load() */

mapping int_query_static_auto_load() {
  mapping tmp;
  
  tmp = object::int_query_static_auto_load();
  return ([ "::" : tmp,
            "hold" : holdable::query_static_auto_load()
            ]);
} /* int_query_static_auto_load() */

mapping query_dynamic_auto_load() {
  mapping map;
  
  map = ([
          "::" : object::query_dynamic_auto_load(),
          "hold" : holdable::query_dynamic_auto_load(),
          ]);
  return map;
} /* query_dynamic_auto_load() */

void init_static_arg( mapping map ) {
  if ( !mapp( map ) ) return ;
  if ( map[ "::" ] ) {
    object::init_static_arg( map[ "::" ] );
  }
  holdable::init_static_arg(map);
} /* init_static_arg() */

void init_dynamic_arg( mapping map, object ) {
   mapping stat_temp;
   string virt_name, new_name;

   if ( map[ "::" ] )
      object::init_dynamic_arg( map[ "::" ] );

   holdable::init_dynamic_arg(map["hold"], 1);

   if( virt_name = query_property( VIRTUAL_NAME_PROP) ) {
     if( file_size( virt_name ) == -1 ) {
       new_name = ( CLONER )->other_file( virt_name );
       if( stringp( new_name ) && ( new_name != virt_name ) ) {
         add_property( VIRTUAL_NAME_PROP, new_name );
         virt_name = new_name;
       } else {
         if( VIRTUAL_HANDLER->query_forbidden( virt_name ) ) {
           call_out( "replace_me", 1 );
         } else {
           VIRTUAL_HANDLER->add_missing( virt_name );
         }
       }
     }
     if( file_size( virt_name ) != -1 &&
         query_property( "virtual time" ) < stat( virt_name )[1] ) {
       stat_temp = ( VIRTUAL_HANDLER )->new_data( virt_name );
       if( mapp( stat_temp ) ) {
         init_static_arg( stat_temp );
         add_property( "virtual time", time() );
       }
     }
   }
} /* init_dynamic_arg() */
