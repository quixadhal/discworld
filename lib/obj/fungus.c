/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: fungus.c,v 1.7 2001/09/24 21:56:19 danbala Exp $
 */
#include <move_failures.h>

inherit "/obj/food";

int delay, large_weight, stage;
string adjs, extra, type, *messages;
void next_stage();

void create() {
  do_setup++;
  ::create();
  do_setup--;
  set_name( "fungus" );
  add_plural( "fungi" );
  set_short( "fungus" );
  delay = 600;
  large_weight = 4;
  set_weight_per_bite( 1 );
  extra = "It looks edible.";
  type = "mushroom";
  messages = ({
    "A small $type$ pushes up through the soil.\n",
    "The $type$ seems to be a bit larger now.\n",
    "The $type$ looks nice and large now.\n",
    "The $type$ silently releases a grey dust into the air.\n",
    "The $type$ collapses in on itself and falls apart.\n"
  });
  if ( !do_setup )
    this_object()->setup();
   if ( clonep( this_object() ) )
      call_out( "next_stage", delay / 2 + random( delay ) );
} /* create() */

int query_growing() { return 1; }

int query_delay() { return delay; }

void set_delay( int number ) {
  if ( number < 60 )
    number = 60;
  delay = number;
  if ( remove_call_out( "next_stage" ) > -1 )
    call_out( "next_stage", delay / 2 + random( delay ) );
} /* set_delay() */

int query_large_weight() { return large_weight; }

int query_stage() { return stage; }

void set_stage( int number ) { 
  if(number > 0) {
    stage = number - 1;
  }
  this_object()->next_stage();
}

void set_large_weight( int number ) { large_weight = number; }

string query_adjs() { return adjs; }

void set_adjs( string words ) { adjs = words; }

string query_extra() { return extra; }

void set_extra( string words ) { extra = words; }

string query_type() { return type; }

void set_type( string word ) { type = word; }

string *query_messages() { return messages; }

void set_messages( string *words ) {
  if ( sizeof( words ) != 5 ) {
    write( "WARNING: there must be five messages.\n" );
    return;
  }
  messages = words;
} /* set_messages() */

void init() {
   ::init();
   if ( stage > 5 )
      this_player()->add_command( "pick", this_object() );
} /* init() */

int test_non_spore( object thing ) {
  return ( (int)thing->query_stage() > 5 );
} /* test_non_spore() */

void setup_spore() {
  set_name("pile");
  set_short("small, dusty pile of fungus spores");
  add_alias("dust");
  add_alias("spore");
  add_adjective("fungus");
  add_adjective("spore");
  set_main_plural("small, dusty piles of fungus spores");
  add_plural(({"piles","spores"}));
  add_plural_adjective("small, dusty piles of");
  add_plural_adjective("fungus");
  set_delay( delay );
  set_large_weight( large_weight );
  set_weight_per_bite( query_weight_per_bite() );
  set_adjs( adjs );
  set_extra( extra );
  set_long("This is a small pile of dust, mainly fungus "
           "spores, it seems.\n");
  set_type( type );
  set_messages( messages );
}

object make_spore( object place ) {
  object spore;
  spore = clone_object( explode( file_name( this_object() ), "#" )[0]);
  spore->setup_spore();
  spore->move( place );
  return spore;
} /* make_spore() */

void next_stage() {
  int i, number;
  string words;
  object *things;
  if ( !environment() )
    return;
  if ( adjs )
    words = adjs +" "+ type;
  else
    words = type;
  switch ( stage ) {
    case 0 .. 4 : /* dormant spore */
      this_object()->setup_spore();
      break;
    case 5 : /* small */
      things = match_objects_for_existence( pluralize(words),
                                            ({ environment() }) );
      things = filter_array( things, "test_non_spore", this_object() );
      if ( sizeof( things ) >=
          (int)environment()->query_property( words +" capacity" ) ) {
        move( "/room/rubbish" );
        return;
      }
      tell_room( environment(), replace( messages[ 0 ], "$type$", words ) );
      set_aliases( ({ type }) );
      set_plurals( ({ "fungi", pluralize( type ) }) );
      set_short( "small "+ words );
      if ( adjs )
        set_adjectives( ({ "small" }) + explode( adjs, " " ) );
      else
        set_adjectives( ({ "small" }) );
      set_main_plural( "small "+ pluralize( words ) );
      set_long( "This is a small "+ words +"."+ ( extra ? "  "+ extra : "" ) +
          "\n" );
      set_weight( large_weight / 4 );
      things = all_inventory( environment() );
      for ( i = 0; i < sizeof( things ); i++ )
        if ( living( things[ i ] ) )
          things[ i ]->add_command( "pick", this_object() );
      break;
    case 6 : /* medium */
      tell_room( environment(), replace( messages[ 1 ], "$type$", words ) );
      set_short( "medium "+ words );
      if ( adjs )
        set_adjectives( ({ "medium" }) + explode( adjs, " " ) );
      else
        set_adjectives( ({ "medium" }) );
      set_main_plural( "medium "+ pluralize( words ) );
      set_long( "This is a medium sized "+ words +"."+ ( extra ? "  "+
          extra : "" ) +"\n" );
      set_weight( large_weight / 2 );
      break;
    case 7 : /* large */
      tell_room( environment(), replace( messages[ 2 ], "$type$", words ) );
      set_short( "large "+ words );
      if ( adjs )
        set_adjectives( ({ "large" }) + explode( adjs, " " ) );
      else
        set_adjectives( ({ "large" }) );
      set_main_plural( "large "+ pluralize( words ) );
      set_long( "This is a large "+ words +"."+ ( extra ? "  "+ extra : "" ) +
          "\n" );
      set_weight( large_weight );
      break;
    case 8 .. 10 : /* spore production */
      break;
    case 11 : /* spore release */
      if(!environment() || base_name(environment()) == "/room/rubbish")
        break;
      
      tell_room( environment(), replace( messages[ 3 ], "$type$", words ) );
      number = 2 + random( 3 );
      for ( i = 0; i < number; i++ )
        make_spore( environment() );
      break;
    default : /* death */
      tell_room( environment(), replace( messages[ 4 ], "$type$", words ) );
      move( "/room/rubbish" );
      return;
  }
  stage++;
  call_out( "next_stage", delay / 2 + random( delay ) );
} /* next_stage() */

int do_get() {
  if ( stage != -1 ) {
    write( the_short() +" is currently growing.  You could \"pick\" "+
        "it, though.\n" );
    return 0;
  }
  if ( move( this_player() ) == MOVE_OK )
    return 1;
  return 0;
} /* do_get() */

int do_take() { return do_get(); }

int do_pick() {
  if ( stage < 6 )
    return 0;
  stage = -1;
  remove_call_out( "next_stage" );
  if ( move( this_player() ) != MOVE_OK )
    call_out( "too_heavy_mess", 0, this_player() );
  return 1;
} /* do_pick() */

void too_heavy_mess( object thing ) {
  if ( thing )
    tell_object( thing, "You are carrying too much to lift the "+ type +
        " as well, so you leave it on the ground.\n" );
} /* too_heavy_mess() */

string query_medium_short() {
  if ( adjs )
    return adjs +" "+ type;
  return type;
} /* query_medium_short() */

mapping query_static_auto_load() { return int_query_static_auto_load(); }

void init_dynamic_arg( mapping args, object) {
  ::init_dynamic_arg( args );
  stage = -1;
  remove_call_out( "next_stage" );
} /* init_dynamic_arg() */
