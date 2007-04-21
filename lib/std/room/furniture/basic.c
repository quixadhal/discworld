/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: basic.c,v 1.23 2003/02/14 21:55:25 pinkfish Exp $
 *
 *
 */
/**
 * This is a furniture item for making all kinds of furniture.
 */
#include <move_failures.h>
#include <position.h>
#include <room/placement.h>

inherit "/std/object";
inherit "/std/basic/item_chat";
inherit "/std/room/furniture/inherit/furniture_base";

void create() {
  do_setup++;
  object::create();
  furniture_base::create();

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

/* Furniture is much harder to carry than just its weight, therefore we
 * multiply its weight by 5 to determine if someone can carry it.
 */
/** @ignore yes */
varargs int move( mixed dest, string messin, string messout ) {
  int i, t, f;
  object from;

  if(stringp(dest)) {
    dest = find_object(dest);
  }

  if (!move_check(dest)) {
     write(the_short() + " resists being picked up and shocks you slightly.\n");
     return MOVE_INVALID_DEST;
  }

  if(!dest) {
    return MOVE_INVALID_DEST;
  }

  if(living(dest)) {
    t = (int)this_object()->query_complete_weight() * 2;
    if(!(dest->add_weight(t))) {
      return MOVE_TOO_HEAVY;
    }
  }

  i = ::move(dest, messin, messout);

  if(i != MOVE_OK) {
    dest->add_weight(-t);
    return i;
  }

  from = environment();
  if(from && living(from)) {
    f = (int)this_object()->query_complete_weight() * 2;
    from->add_weight(-f);
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

/** @ignore yes */
mixed query_static_auto_load() {
  if ( base_name(this_object()) + ".c" == __FILE__ ) {
    return int_query_static_auto_load();
  }
  return ([ ]);
}

/** @ignore yes */
mixed query_dynamic_auto_load() {
   mapping junk;

   junk = object::query_dynamic_auto_load();
   if (!junk) {
      junk = ([ ]);
   }
   junk["bonded"] = query_bonded();
   return junk;
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_dynamic_arg(mapping map) {
   object::init_dynamic_arg(map);
   set_bonded(map["bonded"]);
} /* init_dynamic_arg() */

/** @ingore yes */
string query_help_file_directory() {
   return "/doc/furniture/";
} /* query_help_file_directory() */

/** @ingore yes */
mixed *stats() {
   return ::stats() +
          ({ ({ "bonded", query_bonded() }) });
} /* stats() */
