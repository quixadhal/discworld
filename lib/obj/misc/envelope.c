/**
 * Paper envelopes that can be written on, closed, sealed and unsealed.  Use
 * a call to "make_envelope()" to set the size and description of the
 * envelope.
 * @author Lemming
 * @started 7/1/2000
 */

#define SMALL   3
#define MEDIUM  5
#define LARGE   8

#define UNSEALED  1
#define SEALED    2
#define OPENED    3

inherit "/obj/baggage";

nosave int status;

/**
 * This is the method used to define the envelope.  It works just like the
 * "make_bottle()" call on "/obj/bottle.c", and must be done whenever one is
 * created.  The description should be short and should not include the size
 * of the envelope - this will be inferred from the size specified.  A size
 * of 3 or less will be "small", 4 to 5 will be "medium", 6 to 8 is "large"
 * and anything over that will be a "large packet".  The default and minimum
 * size is 2.
 * @param description a brief description of the envelope
 * @param size the maximum weight of the envelope plus contents
 * @example
 * // Make "a small pink envelope".
 * make_envelope( "pink", 3 );
 * @example
 * // Make "a large frog flavoured packet"
 * make_envelope( "frog flavoured", 10 );
 * @see query_envelope_status()
 */
void make_envelope( string description, int size ) {
   string adjective, noun;

   if( size < 2 )
      size = 2;

   switch( size ) {
      case 0..SMALL:
         adjective = "small";
         noun = "envelope";
         break;
      case SMALL + 1..MEDIUM:
         adjective = "medium sized";
         noun = "envelope";
         break;
      case MEDIUM + 1..LARGE:
         adjective = "large";
         noun = "envelope";
         break;
      default:
         adjective = "large";
         noun = "packet";
         add_alias("packet");
   }

   set_name( "envelope" );
   set_short( adjective + " " + description + " " + noun );
   add_adjective( adjective );
   add_adjective( explode( description, " " ) );
   set_long( "This is a " + adjective + " " + description + " envelope.  " );
   set_weight( 1 );
   set_value( 300 + size * 20 );
   set_max_weight( size );
   set_material( "paper" );
   add_property( "writeable", 1 );
   add_extra_look( this_object() );

   status = UNSEALED;
} /* setup() */

/** @ignore */
void init() {
   ::init();
   this_player()->add_command( "seal", this_object(),
      "<direct:object:me>" );
   this_player()->add_command( "unseal", this_object(),
      "<direct:object:me>" );
} /* init() */

/** @ignore */
string long( string str, int dark ) {
   string desc;

   switch( status ) {
   case UNSEALED :
      desc = "It has a sticky flap at one end with which you could "
         "probably seal it if you wanted to, though it might be wise to put "
         "something into it first or you're going to feel rather silly.\n";
      break;
   case SEALED :
      desc = "The sticky flap seems to have been sealed down.\n";
      break;
   case OPENED :
      desc = "It looks like it has been sealed and then opened up.\n";
      break;
   default :
      desc = "It is completely broken - you'd better tell a liaison.\n";
   }

   return ::long( str, dark ) + desc;
} /* long() */

/** @ignore */
int do_seal() {
   if( status != UNSEALED ) {
      return notify_fail( this_object()->the_short() + " has already been "
         "sealed.\n" );
   }

   do_close();
   set_stuck( 1 );
   status = SEALED;
   this_player()->add_succeeded_mess( this_object(), "$N lick$s the flap on "
      "$D and seal$s it down.\n", ({ }) );
   return 1;
} /* do_seal() */

/** @ignore */
int do_unseal() {
   if( status != SEALED ) {
      return notify_fail( this_object()->the_short() + " has not been "
         "sealed.\n" );
   }

   set_stuck( 0 );
   do_open();
   status = OPENED;
   this_player()->add_succeeded_mess( this_object(), "$N tear$s back the "
      "flap on $D and open$s it.\n", ({ }) );
   return 1;
} /* do_seal() */

/**
 * This is the method used to query what status the envelope is in.
 * @return 1 for unsealed, 2 for sealed, 3 for sealed and then torn open
 * @see make_envelope()
 */
int query_envelope_status() {
   return status;
} /* query_envelope_status() */

/** @ignore */
mixed *stats() {
   return ::stats() + ({
      ({ "status", status, }),
   });
} /* stats() */

/** @ignore */
mapping int_query_static_auto_load() {
   return ([
      "::" : ::int_query_static_auto_load(),
      "status" : status,
   ]);
} /* int_query_static_auto_load() */

/** @ignore */
void init_static_arg( mapping map ) {
   if( map["::"] )
      ::init_static_arg( map["::"] );
   if( !undefinedp( map["status"] ) )
      status = map["status"];
} /* init_static_arg() */

/** @ignore */
mixed query_static_auto_load() {
   if( base_name( this_object() ) + ".c" == __FILE__ )
      return int_query_static_auto_load();
   return ([ ]);
} /* query_static_auto_load() */
