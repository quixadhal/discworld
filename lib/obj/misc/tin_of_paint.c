/* A tin of paint. */

inherit "/obj/vessel";

#define VOLUME 5000

private nosave string _colour;

varargs int do_paint(string, object);

void setup() {
  set_name( "tin" );
  set_short( "tin of paint" );
  set_main_plural( "tins of paint");
  add_adjective( ({ "tin", "of"}));
  add_alias(({"paint"}));
  set_long( "A tin of paint.  It has a picture of a man with "
            "a big smile on his face painting a wall.\n");
  set_leak_rate( 0 );
  set_max_volume( VOLUME );
  set_value( 30 );
  set_weight( 10 );
  set_max_weight( 30 );
}

void setup_attribs() {
  set_short( "tin of " + _colour + " paint" );
  set_main_plural( "tins of " + _colour + " paint");
  add_adjective( ({ _colour}));
  set_long( "A tin of " + _colour + " paint.  It has a picture of a man with "
            "a big smile on his face painting a wall.\n");
}

void make_tin( string colour ) {
  object liquid;
  liquid = clone_object( "/obj/reagents/generic_liquid.ob" );
  liquid->set_name( "paint" );
  liquid->set_short( colour + " paint" );
  liquid->add_adjective(({colour }));
  liquid->set_amount( VOLUME );
  liquid->set_continuous();
  liquid->set_long( "This is " + colour + " paint, it's thick and gloopy.\n");
  liquid->set_pile_name( "puddle" );
  liquid->add_eat_effect( "/std/effects/ingested/poison", 10 );
  liquid->move( this_object() );

  set_closed();
  _colour = colour;
  setup_attribs();
}

/** @ignore yes */
void init() {
  ::init();
  this_player()->add_command("paint", this_object(),
           "<indirect:object:here'cardinal direction'> with <direct:object'paint'>",
                        (: do_paint( $4[0] ) :));
  this_player()->add_command("paint", this_object(),
                        "<string'part'> on <indirect:object:here> with <direct:object'paint'>",
                        (: do_paint( $4[0], $1[0] ) :));
  this_player()->add_command("paint", this_object(),
            "<indirect:object:here> help",
            (: do_paint( 0, $1[0] ) :));
}

varargs int do_paint(string indirect, object target) {
  object *obs;
  
  if(!indirect && target) {
      if(function_exists("what_to_paint", target))
          printf(target->what_to_paint());
      else
          printf("Help: paint " + target->short() + " with paint\n");
      return 1;
  }

  obs = match_objects_for_existence("paint", this_object());

  if(!query_volume() || !sizeof(obs)) {
    this_player()->add_failed_mess(this_object(), "$D appears to be empty.\n",
                                   ({ }));
    return 0;
  }

  if(query_closed()) {
    this_player()->add_failed_mess(this_object(), "$D is closed.\n", ({ }));
    return 0;
  }

  if(!sizeof(filter(this_player()->query_holding() - ({ 0 }),
                    (: $1->id("paint brush") :)))) {
    this_player()->add_failed_mess(this_object(), "You must be holding a "
                                   "paint brush to paint the walls.\n", ({ }));
    return 0;
  }

  if(!environment(this_player())->test_occupier(this_player()->query_name())) {
    this_player()->add_failed_mess(this_object(),
                                   "You don't own this house.\n", ({ }));
    return 0;
  }
  
  if(target) {
      if(!target->query_short()) return 0;

          if(!function_exists("decorate_part", target)) {
                  this_player()->add_failed_mess(this_object(), "You can't $V the " +
                                indirect + " on the " + target->query_short() + "!\n", ({ }));
                  return 0;
          }
          
          if(!target->decorate_part( indirect, obs[0]->query_short() )) {
                  this_player()->add_failed_mess(this_object(), "You can't $V the " +
                                                                        indirect + ".\n", ({ }));
                  return 0;
          }
          this_player()->add_succeeded_mess(this_object(), "$N $V the " +
                                    indirect + " on the " + target->query_short() + 
                                                                        " with the brush using light, smooth strokes.\n", ({ }));
          return 1;
  }

  if(!function_exists("decorate_surface", environment(this_player()))) {
    this_player()->add_failed_mess(this_object(), "You can't $V the walls "
                                   "here.\n", ({ }));
    return 0;
  }
  
  if(!environment(this_player())->
     decorate_surface(indirect, "covered with " + obs[0]->query_short())) {
    this_player()->add_failed_mess(this_object(), "You can't $V the " +
                                   indirect + ".\n", ({ }));
    return 0;
  }
  
  remove_volume(query_volume());

  this_player()->add_succeeded_mess(this_object(), "$N $V the " +
                                    indirect + " with $D using "
                                    "broad, smooth strokes.\n", ({ }));
  return 1;
}

/** @ignore yes */ 
mapping int_query_static_auto_load() {
  mapping tmp;

  tmp = ::int_query_static_auto_load();
  return ([ "::" : tmp,
          "colour" : _colour,
          ]);
} /* int_query_static_auto_load() */

/** @ignore yes */ 
mapping query_static_auto_load() {
  if ( base_name(this_object()) + ".c" == __FILE__ )
    return int_query_static_auto_load();
  return ([ ]);
} /* query_static_auto_load() */

/** @ignore yes */ 
void init_static_arg(mapping args) {
  if (args["::"])
    ::init_static_arg(args["::"]);
  if (!undefinedp(args["colour"]))
    _colour = args["colour"];
  if(_colour)
    setup_attribs();
} /* init_static_arg() */
