/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: baggage.c,v 1.27 2003/01/30 16:43:07 taffyd Exp $
 * $Log: baggage.c,v $
 * Revision 1.27  2003/01/30 16:43:07  taffyd
 * Added parse_command_adjectiv_id_list() method to add adjectives for 'closed' and 'locked' from the close_lock_container inheritable
 *
 * Revision 1.26  2001/11/14 02:50:06  presto
 * Added set_open call in create().  I think this is needed so that the "open" adjective
 * is added by default
 *
 * Revision 1.25  2001/08/25 02:00:44  ceres
 * Removed compile warning
 *
 * Revision 1.23  2001/06/01 22:27:25  ceres
 * Added help file
 *
 * Revision 1.22  2001/04/28 16:15:58  shrike
 * fixed runtiming move()
 *
 * Revision 1.21  2001/04/14 00:48:40  ceres
 * Returned the max item limit. Not sure where it disappeared to.
 *
 * Revision 1.20  2000/07/14 22:03:55  pinkfish
 * Stop it using a global variable from elsewhere.
 *
 * Revision 1.19  2000/04/19 21:13:59  ceres
 * Made it call set_closed() before set_locked() when restoring furniture.
 *
 * Revision 1.18  2000/03/28 04:49:03  ceres
 * Removed the short_status() stuff, it just doesn't work due to the callouts. If they're too short it gives the wrong short one way if its too long it does it the other way. There's no way to win.
 *
 */
/* Copied from /obj/container.c.  The idea is to remove the
 * liquid and potion-space stuff, since it's not needed for chests
 * and sacks.
 * @author Jeremy, Pinkfish, Ember
 * @see /obj/vessel.c
 */

#include <move_failures.h>

inherit "/std/container";
inherit "/std/basic/close_lock_container";
inherit "/std/basic/condition";
inherit "/std/basic/holdable";

void create() {
  do_setup++;
  container::create();
  close_lock_container::create();
  condition::create();
  holdable::create();
// set_max_weight( 5 );
  set_max_cond(400);
  set_cond(400);
  set_damage_chance(20);
  set_can_export_inventory();
  set_opaque();
  set_open();
  do_setup--;
  if ( !do_setup ) {
    this_object()->setup();
  }
  add_help_file("baggage");
} /* create() */

/** @ignore yes */
void init() {
  close_lock_container::init();
} /* init() */

/** @ignore yes */
int add_weight( int n ) {
  if ( !( ::add_weight( n ) ) ) return 0;
  if ( n >= 0 ) {
    remove_call_out( "check_breakages" );
    call_out( "check_breakages", 5 + random( 16 ) );
  }
  return 1;
} /* add_weight() */

/**
 * This method checks to see if any of the things contained in the
 * container should be broken.
 * @see /std/container->add_weight()
 */
void check_breakages() {
/* check loc_weight against fragility of every object and break some
 *   if necessary.  If the container has the "padded" property, adjust
 *   the chance of breakage.
 */
  object *obs, carrier;
  int i, amt, wt;

  // See if it's being carried by a living object
  carrier = environment(this_object());
  while (carrier && (!living(carrier)))
    carrier = environment(carrier);
  if (!carrier)
    return;
  obs = all_inventory();
  wt = query_loc_weight() - (int)query_property("padded");
  for (i=0;i<sizeof(obs);i++)
    if ((amt = obs[i]->query_property("fragile"))) {
      if (wt <= amt || ((wt - amt)*100)/amt <= random(100))
          obs[i] = 0;
    } else
      obs[i] = 0;

  obs = obs - ({ 0 });
  if (sizeof(obs) && environment(carrier)) {
    tell_room(environment(carrier), carrier->the_short()+" breaks "+
          (sizeof(obs)>1?"some things":"one thing")+" in "+
          query_multiple_short(({ this_object() }))+".\n", ({ carrier }));
    tell_object(carrier, "You break "+query_multiple_short(obs)+" in "+
          query_multiple_short(({ this_object() }))+".\n");
    obs->dest_me();
  }
}

/** @ignore yes */
string long( string word, int dark ) {
  string ret;

  ret = ::long( word, dark );
  ret += cond_string();
  if ( query_transparent() || !query_closed() ) {
    if (dark == 2 ||
        dark == -2) {
       if (query_contents() != "") {
          ret += "$C$$the_short:" + file_name(this_object()) + "$ contains "
                 "some items you cannot make out.\n";
       }
    } else {
       ret += query_contents( "$C$$the_short:"+ file_name( this_object() ) +
           "$ contains " );
    }
  }
  ret += long_status();
  return ret;
} /* long() */

/**
 * This method returns true if the object is open and prints a message
 * about the open status of the object.
 * @return 1 if it is open, 0 if not
 */
int ensure_open() {
  if ( query_locked() ) {
      write( "The "+ short( 0 ) +" is locked.\n" );
      return 0;
   }
   if ( query_closed() ) {
      if ( do_open() ) {
         write( "You open the "+ short( 0 ) +".\n" );
         return 1;
      } else {
         write( "You can't open the "+ short( 1 ) +".\n" );
         return 0;
      }
   }
   return 1;
} /* ensure_open() */

/** @ignore yes */
mixed stats() {
   return container::stats() + close_lock_container::stats();
} /* stats() */

/** @ignore yes */
mapping int_query_static_auto_load() {
  return ([
    "::" : container::int_query_static_auto_load(),
    "condition" : condition::query_static_auto_load(),
    "hold" : holdable::query_static_auto_load(),
    "trans" : query_transparent(),
    "difficulty" : query_difficulty(),
    "key" : query_key(),
    "trap open func" : query_open_trap_func(),
    "trap lock func" : query_lock_trap_func(),
    "trap open ob" : query_open_trap_ob(),
    "trap lock ob" : query_lock_trap_ob(),
  ]);
} /* int_query_static_auto_load() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
   return ([
     "::" : container::query_dynamic_auto_load(),
     "condition" : condition::query_dynamic_auto_load(),
     "hold" : holdable::query_dynamic_auto_load(),
     "locked" : query_locked(),
     "stuck" : query_stuck(),
     "closed" : query_closed(),
   ]);
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_dynamic_arg(mapping map, object) {
  object money;

  if (map["::"]) {
    container::init_dynamic_arg(map["::"]);
  }

  if (map["condition"]) {
    condition::init_dynamic_arg(map["condition"]);
  }
  if (map["hold"]) {
    holdable::init_dynamic_arg(map["hold"]);
  }

  if (sizeof(map["money"])) {
     money = clone_object("/obj/money");
     money->set_money_array(map["money"]);
     money->move(this_object());
  }
  // Note, closed must be done before locked!
  if (map["closed"]) {
     set_closed();
  } else {
     set_open();
  }
  if (map["locked"]) {
     set_locked();
  } else {
     set_unlocked();
  }
  set_stuck(map["stuck"]);
} /* init_dynamic_arg() */

/** @ignore yes */
void init_static_arg(mapping args) {
  if (args["::"]) {
    ::init_static_arg(args["::"]);
  }
  if (args["condition"]) {
    condition::init_static_arg(args["condition"]);
  }
  if (args["hold"]) {
    holdable::init_static_arg(args["hold"]);
  }
  if (!undefinedp(args["trans"])) {
    if (args["trans"]) {
       set_transparent();
    } else {
       set_opaque();
    }
  }
  if (!undefinedp(args["difficulty"])) {
    set_difficulty(args["difficulty"]);
  }
  if (!undefinedp(args["key"])) {
    set_key(args["key"]);
  }
  if (!undefinedp(args["trap open func"])) {
    set_open_trap(args["trap open ob"], args["trap open func"]);
  }
  if (!undefinedp(args["trap lock func"])) {
    set_lock_trap(args["trap lock ob"], args["trap lock func"]);
  }

  //
  // Make sure you cannot have infite capacity bags.
  //
  if (!query_max_weight()) {
     set_max_weight(5);
  }

} /* init_static_arg() */

/** @ignore yes */
mixed query_static_auto_load() {
  if ( !query_name() || ( query_name() == "object" ) ) {
    return 0;
  }
  if ( explode( file_name( this_object() ), "#" )[ 0 ] == "/obj/baggage" ) {
    return int_query_static_auto_load();
  }
  return ([ ]);
} /* query_static_auto_load() */

/** @ignore yes */
int can_find_match_recurse_into(object looker) {
   if (query_closed()) {
      return 0;
   }
   return ::can_find_match_recurse_into(looker);
} /* can_find_match_recurse_into() */

/**
 * @ignore yes
 * Thijs is added so that it acts like a living object and
 * things like non-movable signs cannot be added to it.
 */
int test_add(object ob, int flag) {
  if(flag)
    return 0;
  return ::test_add(ob, flag);
} /* test_add() */

/**
 * @ignore yes
 * Added here to make it expose its inventory upwards when moved.
 */
varargs int move(mixed dest, string mess1, string mess2) {
   object from;
   int result;

   result = holdable::move(dest);
   if (result != MOVE_OK) {
      return result;
   }
   from = environment();
   result = container::move(dest, mess1, mess2);
   if (result == MOVE_OK) {
      we_moved(from, environment());
   }
   return result;
} /* move() */

/**
 * @ignore yes 
 */
string *parse_command_adjectiv_id_list() {
    return container::parse_command_adjectiv_id_list() + 
        close_lock_container::parse_command_adjectiv_id_list();
} /* parse_command_adjectiv_id_list() */ 

/** @ignore yes */
void break_me() {
  all_inventory()->move( environment(), "$N fall$s out of " + a_short() +"." );
  ::break_me();
} /* break_me() */

/** @ignore yes */
void dest_me() {
   close_lock_container::dest_me();
   container::dest_me();
} /* dest_me() */
