/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: clock.c,v 1.6 2001/06/01 20:54:45 ceres Exp $
 *
 *
 */
/**
 * The base file for clocks in player houses.
 * @author Ceres
 */

#include <move_failures.h>
#include <position.h>

inherit "/obj/clock";
inherit "/std/room/furniture/inherit/furniture_base";

/** @ignore yes */
void chime(string mess) {
  int *co_ords;

  if(environment(this_object())) {
    co_ords = (int *)environment(this_object())->query_co_ord();
    "/obj/handlers/broadcaster"->broadcast_event(users(), co_ords,
                                                 mess, 20, 1, 0 );
    tell_room(environment(this_object()), mess);
  }
} /* chime() */



void create() {
   do_setup++;
   clock::create();
   add_alias( "furniture" );
   add_plural( "furniture" );
   set_opaque();
   furniture_base::create();

   do_setup--;
   if ( !do_setup ) {
     this_object()->setup();
   }
} /* create() */

private int do_real_move(mixed dest, string messin, string messout) {
   return clock::move(dest, messin, messout);
} /* do_real_move() */

/** @ignore yes */
/* Furniture is much harder to carry than just its weight, therefore we
 * multiply its weight by 5 to determine if someone can carry it.
 */
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
    debug_printf("Added %d", t);
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
} /* query_static_auto_load() */
