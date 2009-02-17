/*  -*- LPC -*-  */
/*
 * $Id: shield.c,v 1.1 2003/05/13 20:10:13 ceres Exp $
 */
#include <move_failures.h>
#include <virtual.h>

#define AC_LIMIT 100

#undef INFORM

inherit "/obj/armour";
inherit "/std/basic/holdable";

int no_limbs;
nosave int held_in;

void create() {
   do_setup++;
   armour::create();
   add_alias( ({ "armour", "shield" }) );
   add_plural( ({ "armours", "shields" }) );
   add_help_file("armour");
   if ( !query_property( "shop type" ) )
      add_property( "shop type", "armoury" );
   do_setup--;
   if ( !do_setup )
      this_object()->setup();
} /* create() */

int query_no_limbs() { return no_limbs; }

void set_no_limbs( int number ) { no_limbs = number; }

mixed set_holder( object thing, int number ) {
   if (!::set_holder(thing, number))  return 0;
   if(!thing) {
     worn_by = 0;
     held_in = 0;
   } else {
     worn_by = thing;
     held_in = number;
   }
   return 1;
} /* set_holder() */

int query_wearable() { return 0; }

/**
 * @ignore yes
 */
varargs int move( mixed dest, string messin, string messout ) {
   int flag;
   if ( worn_by && ( worn_by != dest ) )
      if ( !sizeof( (int *)worn_by->set_unhold( this_object() ) ) )
         return MOVE_NO_UNHOLD;
   flag = armour::move( dest, messin, messout );
   if ( ( flag == MOVE_OK ) && worn_by )
      set_worn_by( 0 );
   return flag;
} /* move() */

/**
 * @ignore yes
 */
void dest_me() {
   set_worn_by( 0 );
   armour::dest_me();
} /* dest_me() */

/**
 * @ignore yes
 */
mixed *stats() {
   return armour::stats() + ({ ({ "no_limbs", no_limbs }) });
} /* stat() */

/**
 * @ignore yes
 */
mapping int_query_static_auto_load() {
   return ([
      "::" : armour::int_query_static_auto_load(),
      "no limbs" : no_limbs,
   ]);
} /* query_static_auto_load() */

/**
 * @ignore yes
 */
mapping query_static_auto_load() {
   if ( base_name(this_object()) != __FILE__[0..<3]) {
      return ([ ]);
   }
   return int_query_static_auto_load();
} /* query_static_auto_load() */

/**
 * @ignore yes
 */
mapping query_dynamic_auto_load() {
   mapping map;
   map = ([
      "::" : armour::query_dynamic_auto_load(),
   ]);
   return map;
} /* query_dynamic_auto_load() */

/**
 * @ignore yes
 */
void init_static_arg( mapping map ) {
   if ( !mapp( map ) )
      return;
   if ( map[ "::" ] )
      armour::init_static_arg( map[ "::" ] );
   if ( map[ "no limbs" ] )
      no_limbs = map[ "no limbs" ];

} /* init_static_arg() */

void replace_me(){
  object receipt;

  receipt = clone_object( "/std/object" );
  receipt->set_name( "receipt" );
  receipt->set_short( "destructed item receipt" );
  receipt->add_adjective( ({ "destructed", "item" }) );
  receipt->set_long( "This seems to be a small piece of paper.\n" );
  receipt->set_read_mess( "According to our sources, your "+query_short()+" was not "
                          "allowed to exist.  Have a nice day." );
  receipt->move( environment() );
  receipt->set_weight( 1 );
  destruct( this_object() );
}

/**
 * @ignore yes
 */
void init_dynamic_arg( mapping map, object ) {
   if ( map[ "::" ] )
      armour::init_dynamic_arg( map[ "::" ] );

} /* init_dynamic_arg() */

