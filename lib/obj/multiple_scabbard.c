#include <move_failures.h>

inherit "/obj/scabbard";

int capacity, throwable;

int query_clothing() { return 1; }
int query_throwable() { return throwable; }
int query_capacity() { return capacity; }
string query_pocket_mess() { return "$ob_short$"; }
void set_throwable(int i) { throwable = i; }
void set_capacity(int i) { capacity = i; }

void create() {
   do_setup++;
   ::create();
   do_setup--;
   set_no_types_in_long(1);
   if( !do_setup )
      this_object()->setup();
} /* create() */

void throw( object * knives, object * targets ) {
   ( "cmds/living/th_row" )->cmd( ({ knives, targets }), 0, "at" );
}

int do_hurl( object * knives, object * targets, int targeted ) {
   object knife, target;

   if( this_player()->query_property( "dead" ) ) {
      add_failed_mess( "You're dead!\n" );
      return -1;
   }

   if( this_object()->query_worn_by() != this_player() ) {
      add_failed_mess( "You must be wearing " + this_object()->the_short()
        + " to hurl weapons from it.\n" );
      return targeted ? -1 : 0;
   }

   if( sizeof( targets ) > 1 ) {
      add_failed_mess( "You can only throw at one target at a time.\n" );
      return -1;
   }
   target = targets[0];

   if( target == this_player() ) {
      add_failed_mess( "Don't be silly.\n" );
      return -1;
   }

   if( target->query_property( "dead" ) ) {
      add_failed_mess( target->the_short(1) + " is dead.\n" );
      return -1;
   }

   if( pk_check( this_player(), target ) ) {
      add_failed_mess( "You really shouldn't do that.\n" );
      return 0;
   }

   if( sizeof( knives ) > 1 ) {
      add_failed_mess( "You can only throw one weapon at a time.\n" );
      return 0;
   }
   knife = knives[0];

   if( knife->move( this_player() ) != MOVE_OK ) {
      add_failed_mess( "You can't draw " + knife->the_short() + " from "
        + the_short() + ".\n" );
      return targeted ? -1 : 0;
   }

   if( this_player()->query_free_limbs() < 1 ) {
      knife->move( this_object() );
      add_failed_mess( "You need a free hand.\n" );
      return -1;
   }

   if( !sizeof( knife->hold_item( this_player() ) ) ) {
      knife->move( this_object() );
      add_failed_mess( "You cannot hold " + knife->the_short() + ".\n" );
      return -1;
   }

   add_succeeded_mess( "$N snatch$es " + knife->a_short() + " from $D.\n" );
   call_out( "throw", 0, knives, targets );
   return 1;
} /* do_hurl() */

/** @ignore yes */
int test_add( object thing, int flag, int noprint ) {
   int foo;

   if(flag)
      return 0;

   if( !environment( thing ) )
      return ::test_add( thing, flag );
   if( sizeof( all_inventory( this_object() ) ) >= capacity ) {
      if( !noprint )
         write( the_short() + " is already full.\n" );
      return 0;
   }

   foo = test_type_ok( thing, flag );
   if(foo)
      return foo;
   if( !noprint )
      write( thing->the_short() + " doesn't fit very well in "
        + the_short() + ".  " + sheath );
   return 0;
} /* test_add() */

void init() {
   if( query_throwable() ) {
      add_command( "hurl",
        "<indirect:object:direct-obs'weapon'> at <indirect:living:here>",
        (: do_hurl( $1[0], $1[1], 0 ) :) );
      add_command( "hurl",
        "<indirect:object:direct-obs'weapon'> at <indirect:living:here> "
          "from <direct:object:me>",
        (: do_hurl( $1[0], $1[1], 1 ) :) );
   }
} /* init() */
