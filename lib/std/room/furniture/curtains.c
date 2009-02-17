/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: curtains.c,v 1.6 2001/06/01 21:18:40 ceres Exp $
 *
 *
 */
/**
 * This is a furniture item for making all kinds of furniture.
 */
#include <move_failures.h>
#include <position.h>

inherit "/std/room/furniture/basic";

private nosave object _window;
private string _window_id;
private string _window_old_long;

void create() {
  do_setup++;
  ::create();
  add_alias( "furniture" );
  add_plural( "furniture" );
  set_opaque();
  if ( !query_property( "shop type" ) ) {
    add_property( "shop type", "furniture" );
  }
  set_allowed_room_verbs((["hanging" : "hangs" ]));
  do_setup--;
  if ( !do_setup ) {
    this_object()->setup();
  }
}

void init() {
  if(!_window_id) {
    this_player()->add_command("hang", this_object(),
        "<direct:object> [on|at] <indirect:object:here>");
  }
  this_player()->add_command("open", this_object(), "<direct:object>");
  this_player()->add_command("close", this_object(), "<direct:object>");
}

int do_open() {
  if(!_window)
    _window = environment(this_object())->query_door_control(_window_id);

  if(!_window_id || !_window) {
    this_player()->add_failed_mess(this_object(),
        "$D aren't hanging at a window.\n");
    return 0;
  }

  if(_window->query_transparent()) {
    this_player()->add_failed_mess(this_object(),
        "$D are already open.\n");
    return 0;
  }

  _window->set_transparent();
  this_player()->add_succeeded_mess(this_object(), "$N $V $D.\n", ({}));
  return 1;
}

int do_close() {
  if(!_window)
    _window = environment(this_object())->query_door_control(_window_id);

  if(!_window_id || !_window) {
    this_player()->add_failed_mess(this_object(),
        "$D aren't hanging at a window.\n");
    return 0;
  }

  if(!_window->query_transparent()) {
    this_player()->add_failed_mess(this_object(),
        "$D are already closed.\n");
    return 0;
  }

  _window->reset_transparent();
  this_player()->add_succeeded_mess(this_object(), "$N $V $D.\n", ({}));
  
  return 1;
}

int do_hang(mixed *in_dir, string direct, string indirect, mixed *args) {
  if(member_array("window", in_dir[0]->parse_command_id_list()) == -1) {
    this_player()->add_failed_mess(this_object(),
        "You can only $V $D at windows.\n",
        ({ }));
    return 0;
  }

  if(this_object()->move(environment(this_player())) != MOVE_OK) {
    this_player()->add_failed_mess(this_object(),
        "You seem to be unable to $V $D "
        "at $I.\n", ({ in_dir[0] }));
    return 0;
  }
  this_object()->add_property("current room hidden", 1);

  _window_id = in_dir[0]->query_my_id();
  _window_old_long = in_dir[0]->query_long();
  
  in_dir[0]->set_long(in_dir[0]->query_long()+"It has " +
                      this_object()->query_short() +
                      " hanging round it.\n");
  event(environment(), "save", this_object());

  this_player()->add_succeeded_mess(this_object(),
                                   "$N $V $D at $I.\n", ({ in_dir[0] }));
  return 1;
}

/** @ignore yes
 * Reset the windows desc and opacity when the curtains are removed.
 */
void removed() {
  this_object()->remove_property("current room hidden");
  if(!_window)
    _window = environment(this_object())->query_door_control(_window_id);
  _window->set_long(_window_old_long);
  _window->set_transparent();
  _window_id = 0;
}

/**
 * This method will return true if the item is an item of furniture.
 * @return always returns true
 */
int query_furniture() { return 1; }

/** @ignore yes */
mixed query_static_auto_load() {
  if ( base_name(this_object()) + ".c" == __FILE__ )
    return int_query_static_auto_load() +
      ([ "window" : _window_id,
       "window_old_long" : _window_old_long ]);
  return ([ "window" : _window_id,
          "window_old_long" : _window_old_long ]);
}

/** 
 * @ignore yes
 */
void init_static_arg( mapping map ) {
  if ( !mapp( map ) ) {
    return;
  }
  ::init_static_arg(map);

  if ( !undefinedp( map[ "window" ] ) )
    _window_id = map["window"];
  if(!undefinedp(map["window_old_long"]))
    _window_old_long = map["window_old_long"];
}

