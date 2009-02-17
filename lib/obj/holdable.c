/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: holdable.c,v 1.1 2001/04/18 23:20:37 pinkfish Exp $
 *
 *
 */
/**
 * The holdable inherit.  This is to make something holdable that is not
 * a weapon, for example torches and candles.
 * @author Pinkfish
 * @started Wed Apr 18 16:13:17 PDT 2001
 */

#include <move_failures.h>
inherit "/std/object";
inherit "/std/basic/holdable";

void create() {
   holdable::create();
   object::create();
} /* create() */

/** @ignore yes */ 
void dest_me() {
   holdable::dest_me();
   object::dest_me();
} /* dest_me() */

/** @ignore yes */ 
mixed query_dynamic_auto_load() {
  return ([ "::" : object::query_dynamic_auto_load(),
            "hold" : holdable::query_dynamic_auto_load(),
          ]);
} /* query_dynamic_auto_load() */

/** @ignore yes */ 
void init_dynamic_arg( mapping arg ) {
   if (arg["hold"]) {
      holdable::init_dynamic_arg(arg["hold"], arg["is lit"]);
   }
   ::init_dynamic_arg(arg["::"]);
} /* init_dynamic_arg() */

/** @ignore yes */ 
void init_static_arg( mapping map ) {
   if ( !mapp( map ) ) {
      return ;
   }
   if ( map[ "::" ] ) {
      object::init_static_arg( map[ "::" ] );
   }
   holdable::init_static_arg(map["hold"]);
} /* init_static_arg() */

/** @ignore yes */ 
mapping int_query_static_auto_load() {
   mapping tmp;

   tmp = object::int_query_static_auto_load();
   return ([ "::" : tmp,
      "hold" : holdable::query_static_auto_load(),
          ]);
} /* int_query_static_auto_load() */

/** @ignore yes */ 
mapping query_static_auto_load() {
   if ( base_name( this_object() ) == "/obj/holdable" ) {
      return int_query_static_auto_load();
   }
   return 0;
} /* query_static_auto_load() */

/** @ignore yes */ 
varargs int move( mixed ob, string mess1, string mess2 ) {
   int ret;

   ret = holdable::move(ob);
   if (ret != MOVE_OK) {
      return ret;
   }
   return object::move( ob, mess1, mess2 );
} /* move() */
