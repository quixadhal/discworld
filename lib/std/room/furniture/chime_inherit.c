/*
 * @author Tannah
 * @started April 2k
 *
 * A fnu little wind chime thingie.  You can set its type to whatever
 * you like.  Be sure to also set its value to something appropriate
 * for its type.  The default type is "metal"; the default value is
 * 420.  It also comes with a couple default verbs which can be
 * removed or added to as desired.  It will chime only when hung up
 * in a room that 1) returns a value to query_wind_speed(), 2) is
 * outside, or 3) has the property "windy".  It can be "hung" on 
 * things that return a value to query_hook() or have the property
 * "hook", and then "unhung".
 */

inherit "/std/object";
inherit "/std/room/furniture/inherit/furniture_base";

#include <weather.h>
#define TIME 120 // 2 minutes

string chimes_short();
string chimes_long();
void set_chime_type( string type );
string chime_type();
void jingle();
string chime_verb();
void add_chime_verb( string verb );
void remove_chime_verb( string verb );
string *query_chime_verbs();
void set_hung_up( int up ); 
int hung_up();
void set_hook( object hook );
object hook();

int do_hang( object hook );
int do_unhang();

private string _chime_type = "metal";
private string *_chime_verbs = ({ "jingles", "tinkles" });
private int _hung_up = 0;
private object _hook;

void create() {
   furniture_base::create();
   object::create();
} /* create() */

/** @ignore */
void setup() {
  set_name( "chime" );
  set_short( (: chimes_short() :) );
  set_long( (: chimes_long() :) );
  add_adjective( ({ "wind", chime_type() }) );
  set_value( 420 );
} /* setup() */

/** @ignore */
void init() {
  add_command( "hang", "<direct:object'chime'> {on|from} "
      "<indirect:object'hook'>", (: do_hang( $1[0] ) :) );
  add_command( "unhang", "<direct:object'chime'>" );
} /* init() */

/** @ignore */
string chimes_short() { 
  return chime_type() + " wind chime";
} /* chimes_short() */

/** @ignore */
string chimes_long() {
  return "This little wind chime is nothing more than bits of "
             + chime_type() + " dangling from strings tied to a "
             "ring.\n";
} /* chimes_long() */

/** @ignore */
void set_chime_type( string type ) { 
  remove_adjective( _chime_type );
  add_adjective( type );
  _chime_type = type;
} /* set_chime_type() */

/** @ignore */
string chime_type() { return _chime_type; }

/** @ignore */
void jingle() {
  object env = environment();
  int wind;

  if( !env )
    return;

  if( !hung_up() )
    return;

  if( !inherits( "/std/room", env ) )
    return;

  wind = env->query_wind_speed();

  if( undefinedp( wind ) ) {
    if( env->query_property( "location" ) == "outside" ||
        env->query_property( "windy" ) )
      wind = WEATHER->query_windsp( this_object() );
    else
      return;
  }

  if( random( wind ) ) // the chance is higher for high winds. D'oh.
    tell_room( env, the_short() + chime_verb() + " in the "
        "wind.\n" );
  call_out( "jingle", TIME );
} /* jingle() */

/** @ignore */
string chime_verb() { 
  return " " + _chime_verbs[ random( sizeof( _chime_verbs ) ) ];
} /* chime_verb() */

/** @ignore */
void add_chime_verb( string verb ) {
  _chime_verbs += ({ verb });
} /* add_chime_verb() */

/** @ignore */
void remove_chime_verb( string verb ) {
  _chime_verbs -= ({ verb });
} /* remove_chime_verb() */

/** @ignore */
void set_chime_verbs( string *verbs ) { _chime_verbs = verbs; }

/** @ignore */
string *query_chime_verbs() { return _chime_verbs; }

/** @ignore */
void set_hung_up( int up ) { 
  remove_call_out( "jingle" );
  if( up )
    call_out( "jingle", 30 );
  _hung_up = up;
} /* set_hung_up() */

/** @ignore */
int hung_up() { return _hung_up; }

/** @ignore */
void set_hook( object hook ) { _hook = hook; }

/** @ignore */
object hook() { return _hook; }

/** @ignore */
int jingling() { return find_call_out( "jingle" ); }

/** @ignore */
void init_dynamic_arg( mapping arg ) {
  set_chime_type( arg[ "type" ] );
  set_chime_verbs( arg[ "chime verbs" ] );
  set_hook( arg[ "hook" ] );
  set_hung_up( arg[ "hung up" ] );
  if( undefinedp( hook() ) || !hung_up() ||
      !sizeof( match_objects_for_existence( hook(), ({ environment() }) ) ) ) {
    remove_property( "there" );
    set_hung_up( 0 );
  }
} /* init_dynamic_arg() */

/** @ignore */
mixed query_dynamic_auto_load() {
  return ([ "::" : ::query_dynamic_auto_load(),
            "type" : chime_type(),
            "chime verbs" : query_chime_verbs(),
            "hung up" : hung_up(),
            "hook" : hook() ]);
} /* query_dynamic_auto_load() */

/** @ignore yes */
int do_hang( object hook ) {
  string where = hook->a_short();

  /* Is it already hanging? */
  if( hung_up() ) {
    add_failed_mess( "The chime is already hanging up.\n" );
    return 0;
  }
    
  /* Can the hook actually be used to hang things from? */
  if( !hook->query_hook() && !hook->query_property( "hook" ) )
    return 0;

  /* Can the chime be dropped? */
  if( move( environment( this_player() ) ) )
    return 0;

  /* set its position and make it impossible to get */
  if( where )
    where = "on " + where;
  else
    where = "up";

  add_property( "there", "hanging "+ where );
  reset_get();
  set_hung_up( 1 );
  set_hook( hook );
  this_player()->add_succeeded_mess( this_object(), "$N $V $D on "
      "$I.\n", ({ hook }) );
  return 1;
} /* do_hang() */

/** @ignore yes */
int do_unhang() {
  if( !hung_up() ) {
    add_failed_mess( "The chime isn't hanging from anything.\n" );
    return 0;
  }
  remove_property( "there" );
  set_get();
  set_hung_up( 0 );
  move( this_player() );
  return 1;
} /* do_unhang() */
