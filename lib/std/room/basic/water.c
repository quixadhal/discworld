/**
 * This is the co-inheritable for rooms containing water.  It handles adding
 * the appropriate effects to objects entering the water, skillchecks to see
 * if they can move around, making their positions appropriate and things like
 * that.  It also takes care of some miscellaneous stuff like salinity,
 * turbidity, currents etc.  Rather than using this co-inheritable directly, it
 * is recommended that you inherit either /std/room/water_inside.c or
 * /std/room/water_outside.c.
 * When using this file, remember the following: if you mask event_enter you
 * should call the inherited function, and unforeseen consequences may result
 * from using modify_exit with the "function", "exit mess" or "enter mess"
 * options.  Using "exit mess" or "enter mess" will only disrupt the tailored
 * swimming/drifting/floating/sinking exit and entry messages, but changing the
 * exit function will allow people through exits without passing the
 * appropriate swimming skillcheck.  For this reason, it is recommended to mask
 * swim_exit instead, and check that the inherited function returns 1 before
 * allowing them to pass.
 * @author Bakhtosh
 * @see /std/room/water_inside
 * @see /std/room/water_outside
 * @see /std/effects/other/immersed
 */

#include <armoury.h>
#include <position.h>
#include <room.h>
#include <tasks.h>

#define ANCHOR_PROP "anchor"
#define GILLS_PROP "gills"
#define FLOATING_PROP "floating"
#define LIVES_IN_WATER_PROP "lives in water"
#define SWIMMING_SKILL "other.movement.swimming"
#define OBJECT_WET_EFFECT "/std/effects/object/wet"
#define LIVING_WET_EFFECT "/std/effects/other/wetness"
#define SWIM_EFFECT "/std/effects/other/water_effect"

mixed query_property(string);
varargs mixed *query_dest_other(string);
varargs int add_property(string, mixed, int);
int modify_exit(mixed, mixed*);
string query_destination(string);
string *query_exits();
int set_water_light(int);
int query_my_light();
varargs string *query_dest_dir(object);
int lives_in_water( object ob );

varargs void soak(object, int);
object get_water();
int water_override(string);
int do_float();
int do_drift();
int do_swim();
int get_swim_enum(object);
int query_bottom();
int query_surface();
string get_exit_mess(object, string);
string get_enter_mess(object, string);
object query_above_room();
void update_water_light();

private string on_bottom = "lying on the bottom",
        non_float = "drifting nearby",
        floating = "floating nearby",
        sinking = "sinking nearby",
        s_in_mess = "$N sink$s $down$ from $F.",
        s_out_mess = "$N sink$s $down$.",
        f_in_mess = "$N float$s $up$ from $F.",
        f_out_mess = "$N float$s $up$.",
        c_in_mess = "$N $V$0=is,are$V$ swept in from $F by the current.",
        c_out_mess = "$N $V$0=is,are$V$ swept $T by the current.",
        up_dir = "up", down_dir = "down";
        
private int bottom = -1, surface = -1, clarity = 90, salinity = 0,
        turbulence = 100, update_light = 1, light_first_queried = 0,
        last_speech_volume = 0;
        
private mapping flows = ([ ]), exit_messes = ([ ]), enter_messes = ([ ]),
        origins = ([ ]);

/**
 * This sets up different search returns for the default 'not there'
 * search result.
 */
string* query_default_search_description() {
   return ({
       "Funnily enough there is nothing interesting in the water.\n",
       "You search around and discover a whole bunch of water.\n",
       "You look up and down, left and right, then up and down again but "
       "all you can find is water.\n"
           });
}

/**
 * @ignore yes
 */
mixed *query_default_position(object ob) {
  return ({SWIMMING, "%^BOLD%^You start to swim.%^RESET%^\n",
           "$C$"+ob->one_short()+" "
           "$V$0=starts,start$V$ to swim.\n"});
} /* query_default_position() */

/**
 * @ignore yes
 */
int is_allowed_position(string poss) {
  if (poss == SWIMMING) {
    return 1;
  }
  return 0;
} /* is_allowed_position() */

/**
 * This function sets the position that non-living items will have when they
 * are lying on the bottom in this room.  The default is "lying on the bottom".
 * @param mess the new position for items lying on the bottom
 * @see query_bottom_mess
 * @example
 *   set_bottom_mess("lying in the seaweed");
 */
void set_bottom_mess(string mess) {
  on_bottom = mess;
}

/**
 * This function returns the position that non-living items will have when they
 * are lying on the bottom in this room.
 * @returns the current position for items lying on the bottom
 * @see set_bottom_mess
 */
string query_bottom_mess() {
  return on_bottom;
}

/**
 * This function sets the position that non-living items will have when they
 * are neither sinking nor floating in this room.  The default is "drifting
 * nearby".
 * @param mess the new position for drifting items
 * @see query_nonfloat_mess
 * @example
 *   set_nonfloat_mess("drifting amid the seaweed");
 */
void set_nonfloat_mess(string mess) {
   non_float = mess;
}

/**
 * This function returns the position that non-living items will have when they
 * are neither sinking nor floating in this room.
 * @return the current position for drifting items
 * @see set_nonfloat_mess
 */
string query_nonfloat_mess() {
  return non_float;
}

/**
 * This function sets the position that non-living items will have when they
 * are floating in this room.  The default is "floating nearby".
 * @param mess the new position for floating items
 * @see query_float_mess
 * @example
 *   set_float_mess("floating above the seaweed");
 */
void set_float_mess(string mess) {
  floating = mess;
}

/**
 * This function returns the position that non-living items will have when they
 * are floating in this room.
 * @return the current position for floating items
 * @see set_float_mess
 */
string query_float_mess() {
  return floating;
}

/**
 * This function sets the position that non-living items will have when they
 * are sinking in this room.  The default is "sinking nearby".
 * @param mess the new position for sinking items
 * @see query_sinking_mess
 * @example
 *   set_sinking_mess("sinking into the seaweed");
 */
void set_sinking_mess(string mess) {
  sinking = mess;
}

/**
 * This function returns the position that non-living items will have when they
 * are sinking in this room.
 * @return the current position for sinking items
 * @see set_sinking_mess
 */
string query_sinking_mess() {
  return sinking;
}

/**
 * This function sets the message that will be displayed when an object sinks
 * into this room.  The string "$down$" will be replaced by the current down
 * direction for this room (as set by set_down_dir), and the usual other
 * $-expansion for messages will occur, including the replacement of "$F" by
 * the direction from which they are arriving.  The default is "$N sink$s
 * $down$ from $F.".
 * @param mess the new entry message for sinking objects
 * @see query_sink_in_mess
 * @see set_down_dir
 * @see query_down_dir
 * @example
 *   set_sink_in_mess("$N sink$s into the cavern from $F.");
 */
void set_sink_in_mess(string mess) {
  s_in_mess = mess;
}

/**
 * This function returns the message that will be displayed when an object
 * sinks into this room, with the usual $-expansion not done.
 * @return the current entry message for sinking objects
 * @see set_sink_in_mess
 */
string query_sink_in_mess() {
  return replace(s_in_mess, ({"$down$", down_dir}));
}

/**
 * This function sets the message that will be displayed when an object sinks
 * out of this room.  The string "$down$" will be replaced by the current down
 * direction for this room (as set by set_down_dir), and the usual other
 * $-expansion for messages will occur.  The default is "$N sink$s $down$.".
 * @param mess the new exit message for sinking objects
 * @see query_sink_out_mess
 * @see set_down_dir
 * @see query_down_dir
 * @example
 *   set_sink_out_mess("$N sink$s $down$ out of the cavern.");
 */
void set_sink_out_mess(string mess) {
  s_out_mess = mess;
}

/**
 * This function returns the message that will be displayed when an object
 * sinks out of this room, with the usual $-expansion not done.
 * @return the current exit message for sinking objects
 * @see set_sink_out_mess
 */
string query_sink_out_mess() {
  return replace(s_out_mess, ({"$down$", down_dir}));;
}

/**
 * This function sets the message that will be displayed when an object floats
 * into this room.  The string "$up$" will be replaced by the current up
 * direction for this room (as set by set_up_dir), and the usual other
 * $-expansion for messages will occur, including the replacement of "$F" by
 * the direction from which they are arriving.  The default is "$N float$s $up$
 * from $F.".
 * @param mess the new entry message for floating objects
 * @see query_float_in_mess
 * @see set_up_dir
 * @see query_up_dir
 * @example
 *   set_float_in_mess("$N float$s into the cavern from $F.");
 */
void set_float_in_mess(string mess) {
  f_in_mess = mess;
}

/**
 * This function returns the message that will be displayed when an object
 * floats into this room, with the usual $-expansion not done.
 * @return the current entry message for floating objects
 * @see set_float_in_mess
 */
string query_float_in_mess() {
  return replace(f_in_mess, ({"$up$", up_dir}));
}

/**
 * This function sets the message that will be displayed when an object floats
 * out of this room.  The string "$up$" will be replaced by the current up
 * direction for this room (as set by set_up_dir), and the usual other
 * $-expansion for messages will occur.  The default is "$N float$s $up$.".
 * @param mess the new exit message for floating objects
 * @see query_float_out_mess
 * @see set_up_dir
 * @see query_up_dir
 * @example
 *   set_float_out_mess("$N float$s $up$ out of the cavern.");
 */
void set_float_out_mess(string mess) {
  f_out_mess = mess;
}

/**
 * This function returns the message that will be displayed when an object
 * floats out of this room, with the usual $-expansion not done.
 * @return the current entry message for floating objects
 * @see set_float_out_mess
 */
string query_float_out_mess() {
  return replace(f_out_mess, ({"$up$", up_dir}));
}

/**
 * This function sets the message that will be displayed when an object is
 * swept into this room by a current.  The usual $-expansion for messages
 * will occur.  The default is "$N $V$0=is,are$V$ swept in from $F by the
 * current.".
 * @param mess the new entry message for objects swept by a current
 * @see query_sweep_in_mess
 * @example
 *   set_sweep_in_mess("$N $V$0=is,are$V$ washed in from $F by the raging "
 *              "torrent.");
 */
void set_sweep_in_mess(string mess) {
  c_in_mess = mess;
}

/**
 * This function returns the message that will be displayed when an object
 * is swept into this room by a current, with the $-expansion not done.
 * @return the current entry message for objects swept by a current
 * @see set_sweep_in_mess
 */
string query_sweep_in_mess() {
  return c_in_mess;
}

/**
 * This function sets the message that will be displayed when an object is
 * swept out of this room by a current.  The usual $-expansion for messages
 * will occur, including the replacement of "$T" by the direction in which they
 * are moving.  The default is "$N $V$0=is,are$V$ swept $T by the current.".
 * @param mess the new exit message for objects swept by a current
 * @see query_sweep_out_mess
 * @example
 *   set_sweep_out_mess("$N $V$0=is,are$V$ washed away $T by the raging "
 *              "torrent.");
 */
void set_sweep_out_mess(string mess) {
  c_out_mess = mess;
}

/**
 * This function returns the message that will be displayed when an object
 * is swept out of this room by a current, with the $-expansion not done.
 * @return the current exit message for objects swept by a current
 * @see set_sweep_in_mess
 */
string query_sweep_out_mess() {
  return c_out_mess;
}

/**
 * This function sets the direction that is considered to be up by this room.
 * This is used by several exit messages, and by the water effect to decide
 * which way a panicking player will flee to try to reach the surface.  For
 * this reason, it should be set to the name of an exit which leads towards a
 * surface room.  Also, buoyant objects will be inclined to move in this
 * direction.  The default, of course, is "up".
 * @param mess the new direction to be considered to be up
 * @see query_up_dir
 * @see set_float_in_mess
 * @see set_float_out_mess
 * @see flee_drowning
 * @example
 *   set_up_dir("upwest");
 */
void set_up_dir(string dir) {
  up_dir = dir;
}

/**
 * This function returns the direction that is currently considered to be up in
 * this room.
 * @return the current direction considered to be up
 * @see set_up_dir
 */
string query_up_dir() {
  return up_dir;
}

/**
 * This function sets the direction that is considered to be down by this room.
 * This is used by several exit messages.  Also, dense objects will be inclined
 * to move in this direction.  The default, of course, is "down".
 * @param mes the new direction to be considered to be down
 * @see query_down_dir
 * @see set_sink_in_mess
 * @see set_sink_out_mess
 * @example
 *   set_down_dir("downeast");
 */
void set_down_dir(string dir) {
  down_dir = dir;
}

/**
 * This function returns the direction that is currently considered to be down
 * in this room.
 * @return the current direction considered to be down
 * @see set_down_dir
 */
string query_down_dir() {
  return down_dir;
}

/**
 * This function sets whether or not this room has a solid surface or bottom in
 * it.  If it does, then items may appear as being on the bottom, and living
 * objects are more able to resist currents (as they have something to hold on
 * to).  The parameter of this function can be 1 to make this room have a
 * bottom, 0 to make it have no bottom, or -1 (the default) to make it decide
 * whether or not it has a bottom by checking to see if it has any exits in the
 * current down direction (as set by set_down_dir).
 * @param val whether or not the room has a bottom
 * @see query_bottom
 * @see set_bottom_mess
 * @see set_down_dir
 */
void set_bottom(int val) {
  bottom = val;
  if (bottom > 1 || bottom < -1) {
    bottom = 1;
  }
}

/**
 * This function sets whether or not this room has an interface with air or
 * surface in it.  If it does, then living objects may breathe here, and
 * turbidity does not effect visibility.  The parameter of this function can be
 * 1 to make this room have a surface, 0 to make it have no surface, or -1 (the
 * default) to make it decide whether or not it has a surface by checking to
 * see if it has any exits in the current up direction (as set by set_up_dir).
 * @param val whether or not the room has a surface
 * @see query_surface
 * @see set_clarity
 * @see set_turbidity
 * @see set_up_dir
 */
void set_surface(int val) {
  surface = val;
  if (surface > 1 || surface < -1) {
    surface = 1;
  }
} /* set_surface() */


/**
 * This function sets the clarity of the water as an integer variable between 1
 * and 100.  The main effect of this is to reduce the light levels of the room
 * to simulate the obscuring effect of turbid water.  The default clarity is
 * 90.
 * @param how_clear the new clarity of the water
 * @see query_clarity
 * @see set_turbidity
 */
void set_clarity(int how_clear) {
  clarity = how_clear;
  if (clarity > 100) {
    clarity = 100;
  }
  else if (clarity < 0) {
    clarity = 0;
  }
  if (!query_property("dark mess")) {
    if (clarity < 20) {
      add_property("dark mess", "The water here is very murky.");
    }
    else if (clarity < 50) {
      add_property("dark mess", "The water here is quite murky.");
    }
    else if (clarity < 80) {
      add_property("dark mess", "The water here is slightly murky.");
    }
  }
}

/**
 * This function returns the current clarity of the water, which is an integer
 * variable between 1 and 100.
 * @return the current clarity of the water
 * @see set_clarity
 * @see query_turbidity
 */
int query_clarity() {
  return clarity;
}

/**
 * This function is an alternative method of setting the clarity of the water.
 * The turbidity is defined as 100 minus the clarity of the water.  The default
 * turbidity is 10.
 * @param how_murky the new turbidity of the water
 * @see set_clarity
 * @see query_turbidity
 */
void set_turbidity(int how_murky) {
  set_clarity(100 - how_murky);
}

/**
 * This function returns the current turbidity of the water, which is an
 * integer variable between 1 and 100.  It will be equal to 100 minus the
 * current clarity of the water.
 * @return the current turbidity of the water
 * @see set_turbidity
 * @see query_clarity
 */
int query_turbidity() {
  return 100 - query_clarity();
}

/**
 * This function sets the salinity of the water as an integer variable between
 * 0 and 100.  Higher salinity will have a small positive effect on the
 * buoyancy of objects in the room.  The default salinity is 0.
 * @param how_salty the new salinity of the water
 * @see query_salinity
 * @see calc_buoyancy
 */
void set_salinity(int how_salty) {
  salinity = how_salty;
  if (salinity > 100) {
    salinity = 100;
  }
  else if (salinity < 0) {
    salinity = 0;
  }
}

/**
 * This function returns the current salinity of the water, which is an integer
 * variable between 0 and 100.
 * @return the current salinity of the water
 * @see set_salinity
 */
int query_salinity() {
  return salinity;
}

/**
 * This function sets the turbulence of the water as a non-negative integer
 * variable.  A random number up to the turbulence is added to the difficulty
 * of all skillchecks to leave the room via a water exit with swim_exit as the
 * exit function, except when the exiting object is not moving of its own
 * accord (such as when it is being swept along by a current).  The default
 * turbulence is 100.
 * @param how_turbulent the new turbulence of the water
 * @see query_turbulence
 * @see swim_exit
 */
void set_turbulence(int how_turbulent) {
  turbulence = how_turbulent;
  if (turbulence < 0) {
    turbulence = 0;
  }
}

/**
 * This function returns the current turbulence of the water, which is a
 * non-negative integer.
 * @return the current turbulence of the water
 * @see set_turbulence
 */
int query_turbulence() {
  return turbulence;
}

/**
 * This function sets whether the room will use the light levels of the
 * surface to determine its own.  If the function is called with a non-zero
 * value, the the current light level in the room will be overridden by a new
 * value based on the light of the room found by query_above_room.  If the room
 * above is an outside room, with light levels that depend on the time of day,
 * then the light in this room will be updated every time it is queried.  A
 * water room will default to using this option, but it will be overridden by
 * any calls to adjust_light (including calls to set_light).  It is possible to
 * call set_water_light to avoid this, but it should not be necessary.  If a
 * series of rooms with vertical exits between them are all set to use surface
 * light, then the effect will be for the light of the room at the top of the
 * stack to filter down through the rest, appropriately attenuated by
 * turbidity.
 * @param val whether the room should use light from the surface
 * @see query_above_room
 * @see query_water_surface_light
 * @see update_water_light
 * @see set_water_light
 * @see set_turbidity
 * @see set_clarity
 */
void set_use_surface_light(int val) {
  int surface_light;
  object above;
  update_light = 0;
  if (!val) {
    return;
  }
  above = query_above_room();
  if (!above) {
    set_water_light(0);
    return;
  }
  if (function_exists("query_day_light", above) ||
           above->water_surface_light_varies()) {
    update_light = 1;
    return;
  }
  if (function_exists("query_water_surface_light", above)) {
    surface_light = above->query_water_surface_light();
  }
  else {
    surface_light = above->query_my_light();
  }
  set_water_light(surface_light);
}


/**
 * This function returns the amount of light that will filter down to rooms
 * below this one that have had set_use_surface_light called in them.  Its
 * default is to return a value based on the light in this room and the clarity
 * of the water.
 * @return the amount of light that filters down to rooms below this one
 * @see set_use_surface_light
 * @see set_turbidity
 * @see set_clarity
 */
int query_water_surface_light() {
  update_water_light();
  return query_my_light()*query_clarity()/100;
}

/**
 * This function returns the amount of light that will filter down to this
 * room from those above it if set_use_surface_light has been called.  It is
 * intended to be used only by the update_water_light function, but has been
 * separated out to allow it to be masked.
 * @return the amount of light that filters down from rooms above this one
 * @see update_water_light
 * @see query_above_room
 * @see query_water_surface_light
 */
int get_water_surface_light() {
  object above = query_above_room();
  if (!above) {
    return 0;
  }
  if (function_exists("query_water_surface_light", above)) {
    return above->query_water_surface_light();
  }
  return above->query_my_light();
}

/**
 * This function updates the amount of light filtering down to this room from
 * the rooms above it, if it is necessary to do so.  It is called by
 * query_light in /std/room/water_inside and /std/room/water_outside.
 * @see set_use_surface_light
 * @see set_light
 * @see query_water_surface_light
 * @see get_water_surface_light
 */
void update_water_light() {
  if (!update_light) {
    return;
  }
  if (!light_first_queried) {
    set_use_surface_light(1);
    light_first_queried = 1;
    update_water_light();
    return;
  }
  set_water_light(get_water_surface_light());
}

/**
 * This function adds a water current flowing through a particular exit, which
 * may sweep objects through it or make it harder for them to swim through.
 * The second argument is the rate of flow.  If this is positive, then water
 * will be flowing from this room into the next one.  If it is positive, then
 * water will be flowing from the next room into this one.  It is up to the
 * coder of the specific rooms to ensure that the currents in different rooms
 * match one another.
 * @param dir the exit through which the current is flowing
 * @param rate the strength of the current
 * @see delete_flow
 * @see query_flow
 * @see query_flows
 * @see drift
 * @see swim_exit
 * @example
 *   // There is a current flowing south.
 *   add_flow("north", -80);
 *   add_flow("south", 80);
 */
void add_flow(string dir, int rate) {
  flows[dir] = rate;
}

/**
 * This function removes a water current through a particular exit.
 * @param dir the exit for which any water current should be removed
 * @see add_flow
 * @see query_flow
 * @see query_flows
 */
void delete_flow(string dir) {
  map_delete(flows, dir);
}

/**
 * This function returns a mapping of all the current flows through exits in
 * this room.  The keys of the mapping are the exits through which the currents
 * flow, and the values are the rates of flow.
 * @return a mapping of the current flows through exits in this room
 * @see add_flow
 * @see delete_flow
 * @see query_flow
 */
mapping query_flows() {
  return copy(flows);
}

/**
 * This function returns the strength of the current flowing through a
 * particular exit, if any.  A positive value represents a current flowing from
 * this room into the next one, and a negative value represents a current
 * flowing from the next room into this one.
 * @param dir the exit which should have its current returned
 * @return the current through this ext
 * @see add_flow
 * @see delete_flow
 * @see query_flows
 */
int query_flow(string dir) {
  return flows[dir];
}

/**
 * This function returns the bonus that objects get to move along the bottom or
 * to resist such movement here.  If this should be anything unusual, such as
 * for a very smooth bottom or one with handles, this function should be masked
 * to return something different.
 * @param thing the object that is moving along the bottom
 * @param buoyancy the object's relative buoyancy
 * @return the traction bonus on the bottom here
 * @see swim_exit
 * @see drift
 * @see calc_buoyancy
 */
int query_water_traction_bonus(object thing, int buoyancy) {
  if (buoyancy < 0) {
    buoyancy = -buoyancy;
    return buoyancy/3 + random(buoyancy/3);
  }
  return 0;
}

/**
 * This function returns the room above this one, in the direction set by
 * set_up_dir.
 * @return the room above this one
 * @see set_up_dir
 * @see query_surface_room
 */
object query_above_room() {
  string destination = query_destination(up_dir);
  if (!destination) {
    return 0;
  }
  return load_object(destination);
}

/**
 * This function returns the room below this one, in the direction set by
 * set_down_dir.
 * @return the room below this one
 * @see set_down_dir
 * @see query_bottom_room
 */
object query_below_room() {
  string destination = query_destination(down_dir);
  if (!destination) {
    return 0;
  }
  return load_object(destination);
}

/**
 * This function returns the top room in a vertical stack of water rooms.  If
 * there is a surface water room, it is returned, else, if the top room is not
 * water, or there is no surface room, 0 is returned.
 * @return the first room above this one to be on the surface
 * @see query_above_room
 * @see query_surface
 */
object query_surface_room() {
  object next;
  if (query_surface()) {
    return this_object();
  }
  next = query_above_room();
  if (next && next->query_water()) {
    return next->query_surface_room();
  }
  return 0;
}

/**
 * This function returns the bottom room in a vertical stack of water rooms.
 * If there is a bottom water room, it is returned, else, if the bottom room is
 * not water, or there is no bottom room, 0 is returned.
 * @return the first room below this one to be on the bottom
 * @see query_below_room
 * @see query_bottom
 */
object query_bottom_room() {
  object next;
  if (query_bottom()) {
    return this_object();
  }
  next = query_below_room();
  if (next && next->query_water()) {
    return next->query_bottom_room();
  }
  return 0;
}

/**
 * @ignore yes
 */
void event_enter(object ob, string mess, object from)  {
  int effnum, buoyancy, *effects, *args;
  effects = ob->effects_matching(SWIM_EFFECT->query_classification());
 


  if (!effects || !sizeof(effects)) {
    if (from && !from->query_water() ) {
      if( !lives_in_water( ob ) ) 
        ob->add_effect(SWIM_EFFECT, 2);
    }
  }

  //Some of the code in this if relies on the water effect, some other code 
  //adds the water effect, either way, we don't want it for things that live in
  //water.
  if ( !lives_in_water( ob ) ) {
      effnum = get_swim_enum(ob);
      args = ob->arg_of(effnum);
      if (living(ob) && query_surface() && !ob->query_property("dead") && from &&
               from->query_water() && !from->query_surface()) {
        if (args[1] < 150 || ob->query_property(GILLS_PROP)) {
          tell_object(ob, "You break the surface.\n");
        }
        else {
          tell_object(ob, "You break the surface and take a deep breath.\n");
        }
        if (args[1] != 0) {
          ob->set_arg_of(effnum, ({args[0], 0}));
        }
      }
  }

  if (!living(ob)) {
    buoyancy = SWIM_EFFECT->calc_buoyancy(ob);
    if (buoyancy < 0 && query_bottom()) {
      ob->add_property("there", on_bottom);
    }
    else if (buoyancy >= 0 && (buoyancy > ob->query_property(ANCHOR_PROP)
           || query_surface())) {
      ob->add_property("there", floating);
    }
    else if (buoyancy < 0 && buoyancy < -ob->query_property(ANCHOR_PROP)) {
      ob->add_property("there", sinking);
    }
    else {
      ob->add_property("there", non_float);
    }
  } else {
    ob->return_to_default_position();
  }
}

/**
 * This function makes things wet.  Anything entering a water room has this
 * function called on it by event_enter, and will have the wetness effect added
 * to it if appropriate, as well as to any appropriate objects inside it if it
 * isn't waterproof.  Open containers will also be filled with water.  The
 * function checks to see that the object is indeed inside the room, unless the
 * optional extra argument is non-zero.
 * @param ob the object to be soaked
 * @param ignore_location whether the object should be soaked wherever it is
 * @see get_water
 */
varargs void soak(object ob, int ignore_location) {
  int wetness = 0, *effects;
  object env, water, *things;
  env = ob;
  if (!ignore_location) {
    while (env && env = environment(env)) {
      if (env == this_object()) {
        break;
      }
      if ((env->query_closed() && env->query_waterproof()) ||
           env->query_dry_cargo()) {
        env = 0;
        break;
      }
    }
  }
  if (!env) {
    return;
  }
  if (ob->query_property(FLOATING_PROP) && query_surface()) {
    return;
  }
  if (living(ob)) {
    if (ob->query_property("dead") || lives_in_water( ob ) ) {
      return;
    }
    effects = ob->effects_matching(LIVING_WET_EFFECT->query_classification());
    if (effects && sizeof(effects)) {
      wetness = ob->arg_of(effects[0]);
    }
    wetness = ob->query_weight() - wetness;
    if (wetness > 0) {
      ob->add_effect(LIVING_WET_EFFECT, wetness);
    }
  }
  else if (ob->id("towel")) {
    effects = ob->effects_matching(OBJECT_WET_EFFECT->query_classification());
    if (effects && sizeof(effects)) {
      wetness = ob->arg_of(effects[0]);
    }
    wetness = 200*ob->query_weight() - wetness;
    if (wetness > 0) {
      ob->add_effect(OBJECT_WET_EFFECT, wetness);
    }
  }
  if (ob->query_max_volume() && !ob->query_closed() &&
           ob->query_max_volume() - ob->query_volume() > 0) {
    water = get_water();
    water->set_amount(ob->query_max_volume() - ob->query_volume());
    water->move(ob);
  }
  if (!(ob->query_closed() && ob->query_waterproof()) &&
           !ob->query_dry_cargo()) {
    things = all_inventory(ob);

    if (things && sizeof(things)) {
      map(things, (: soak($1, 1) :));
    }
  }
}

/**
 * This function returns some appropriate water from the room.  Its appearance
 * will depend on the clarity and salinity set in the room.  The quantity of
 * the water object that it returns is not fixed, and will generally be set by
 * whatever function called it.
 * @return some water from the room
 * @see soak
 * @see set_clarity
 * @see set_turbidity
 * @see set_salinity
 */
object get_water() {
  object water = ARMOURY->request_item("water");
  switch (query_clarity()) {
  case 0..20:
    water->set_short("very muddy water");
    water->add_adjective(({"very", "muddy"}));
    water->set_long("This is $amount_size$ of very muddy water.\n");
    return water;
  case 21..50:
    water->set_short("muddy water");
    water->add_adjective("muddy");
    water->set_long("This is $amount_size$ of muddy water.\n");
    return water;
  case 51..80:
    water->set_short("slightly muddy water");
    water->add_adjective(({"slightly", "muddy"}));
    water->set_long("This is $amount_size$ of slightly muddy water.\n");
  }
  switch (query_salinity()) {
  case 51..100:
    water->set_short("brine");
    water->add_alias("brine");
    water->set_long("This is $amount_size$ of very salty water.\n");
    return water;
  case 11..50:
    water->set_short("salty water");
    water->add_adjective("salty");
    water->set_long("This is $amount_size$ of salty water.\n");
  }
  return water;
}

/**
 * This is an exit function set for any exits in a water room.  It finds the
 * difficulty of swimming through the exit, on the basis of the current through
 * that exit, the buoyancy of the object and the extra difficulty that it has
 * in swimming.  The swimming/immersion effect calculates the latter two with
 * calc_buoyancy and swim_difficulty respectively.  The object is subjected to
 * a skillcheck in other.movement.swimming to see if it can move through the
 * exit.
 * There is a guildpoint cost equal to 1/20th of the difficulty of the
 * skillcheck.
 * @param dir the direction in which the object is leaving
 * @param ob the object that is trying to leave
 * @param mess a weird extra string that isn't relevant here
 * @return whether or not the object can move through the exit
 * @see modify_exit
 * @see exit_function
 * @see calc_buoyancy
 * @see swim_difficulty
 * @see query_water_traction_bonus
 * @see perform_task
 */
int swim_exit(string dir, object ob, string mess) {
  int difficulty, buoyancy = 0, place, this_turb, gp_cost, effnum, *arg;
  string *places_to_go, *tm_messes;
  object destination;

  if (!living(ob) || ob->query_property("dead") || lives_in_water( ob ) ) {
    return 1;
  }

  effnum = get_swim_enum(ob);
  arg = ob->arg_of(effnum);
  if (-2 == arg[0]) {
    // If you're not moving of your own accord, assume that it's already been
    // checked that you can pass through the exit.
    return 1;
  }
  difficulty = -flows[dir];
  buoyancy = SWIM_EFFECT->calc_buoyancy(ob);
  if (ob->query_weight()) {
    buoyancy /= ob->query_weight();
  }
  places_to_go = query_dest_dir();
  place = member_array(dir, places_to_go);
  if (-1 == place || !(destination = load_object(places_to_go[place+1]))) {
    return 0;
  }
  if (dir == up_dir) {
    // It's harder to go up if you're not buoyant.
    difficulty -= buoyancy + 50;
  } else if (dir == down_dir) {
    // It's harder to go down if you're buoyant.
    difficulty += buoyancy - 50;
  } else if (buoyancy < 0) {
    // It's harder to swim if you keep sinking.
    difficulty -= buoyancy/5;
  } else {
    // It's harder to swim if you keep floating.
    difficulty += buoyancy/5;
  }
  if (query_bottom() && (!destination->query_water() ||
           destination->query_bottom())) {
    // It's easier to walk along the bottom if you're not buoyant.
    difficulty -= query_water_traction_bonus(ob, buoyancy);
  } 
  this_turb = random(query_turbulence());
  difficulty += this_turb;
  if (arg[0] == -1) {
    // You get a bonus if you're desperately trying to reach the surface.
    difficulty -= 50;
  }
  if (!destination->query_water()) {
    // It's easier if you can pull yourself ashore.
    difficulty -= 50;
  }
  difficulty *= SWIM_EFFECT->swim_difficulty(ob);
  difficulty /= 100;
  if (difficulty <= 0) {
    return 1;
  }
  if (arg[0] != -1) {
    gp_cost = difficulty/20;
    if (gp_cost > 50) {
      gp_cost = 50;
    }
    else if (gp_cost < 1) {
      gp_cost = 1;
    }
    if (ob->query_specific_gp("other") < gp_cost) {
      tell_object(ob, "You're too "+({"fatigued", "tired", "weary",
           "exhausted"})[random(4)]+" to swim "+dir+" at the moment.\n");
      notify_fail("");
      return 0;
    }
  }
  ob->adjust_gp(-gp_cost);
  switch (TASKER->perform_task(ob, SWIMMING_SKILL, difficulty,
           TM_CONTINUOUS)) {
  case AWARD:
    tm_messes = ({"You move more surely as you glide through the water.",
                  "You discover a more efficient stroke.",
                  "You find a better way to streamline your body.",
                  "You find a more efficient swimming rhythm.",
                  "You begin to move more confidently through the water."});
    tell_object(ob, "%^YELLOW%^"+tm_messes[random(sizeof(tm_messes))]+
           "%^RESET%^\n");
  case SUCCEED:
    return 1;
  }
  notify_fail("");
  if (dir == up_dir && buoyancy < -50) {
    tell_object(ob, "You struggle to leave "+up_dir+" but, with the load "
           "you're carrying, you can't make any headway.\n");
    tell_room(this_object(), "$C$"+ob->one_short()+" "
           "$V$0=struggles,struggle$V$ to leave "+up_dir+", but can't make "
           "any headway.\n", ob);
    return 0;
  }
  if (dir == down_dir && buoyancy > 50) {
    tell_object(ob, "You struggle to leave "+down_dir+" but, with your "
           "buoyancy, you can't make any headway.\n");
    tell_room(this_object(), "$C$"+ob->one_short()+" "
           "$V$0=struggles,struggle$V$ to leave "+down_dir+", but can't make "
           "any headway.\n", ob);
    return 0;
  }
  if (flows[dir] < 0) {
    tell_object(ob, "You struggle to leave "+dir+", but you can't make any "
           "headway against the current.\n");
    tell_room(this_object(), "$C$"+ob->one_short()+" "
           "$V$0=struggles,struggle$V$ to leave "+dir+", but can't make any "
           "headway against the current.\n", ob);
    return 0;
  }
  if (this_turb > 0) {
    tell_object(ob, "You struggle to leave "+dir+", but you can't make any "
           "headway in the turbulent waters.\n");
    tell_room(this_object(), "$C$"+ob->one_short()+" "
           "$V$0=struggles,struggle$V$ to leave "+dir+", but can't make any "
           "headway.\n", ob);
    return 0;
  }
  tell_object(ob, "You struggle to leave "+dir+", but you can't make any "
           "headway.\n");
  tell_room(this_object(), "$C$"+ob->one_short()+" $V$0=struggles,struggle$V$ "
           "to leave "+dir+", but can't make any headway.\n", ob);
  return 0;
}

/**
 * With this function here, creators will be able to point and laugh when
 * someone dies by drowning in a water room because they'll see the death
 * reason in an inform.  It will also show up in the death log.  It would be a
 * good idea to mask this function to return a reason more specific to the
 * place where the drowning may occur (such as "drowning in the Djel"), or at
 * least a humorous one (such as "failing to realise that scuba gear hasn't
 * been invented yet") so that creators will have something to laugh at.
 * @return the reason for death
 * @see do_death
 */
string query_death_reason() {
  return "drowning";
}

/**
 * @ignore yes
 */
int add_exit(string direc, mixed dest, string type) {
    mixed *messy = query_dest_other(direc);
  
    if (messy && arrayp(messy[ROOM_ENTER]) && sizeof(messy[ROOM_ENTER]) == 2){
        origins[direc] = messy[ROOM_ENTER][1];
    }
    modify_exit(direc, ({"function", "swim_exit",
                       "exit mess", (: get_exit_mess($1, $(direc)) :),
                       "enter mess", (: get_enter_mess($1, $(direc)) :)}));

    // We can't bury objects here if they can sink
    if( direc == "down" ) {
        add_property("no burial", 1);
    }

}

/**
 * This function returns the opposite to the direction of a particular exit.
 * This information is normally only accessible within the room handler, but it
 * is cached here in the origins mapping by the add_exit in this file.  If no
 * value is found, "elsewhere" will be returned.  This value is used in exit
 * messages to correctly display where they are coming from.
 * @param dir the exit for which the opposite direction should be found
 * @return the opposite of the specified direction
 * @see add_exit
 * @see room_handler
 */
string query_origin(string dir) {
  if (origins[dir]) {
    return origins[dir];
  }
  return "elsewhere";
}

/**
 * This function sets the next exit message from this room for a particular
 * object.  It is used automagically by the swim_exit function, but can be used
 * for other purposes if you feel like it.
 * @param ob the object for which the next exit message should be set
 * @param mess the next exit message for the object
 * @see get_exit_mess
 * @see add_enter_mess
 * @see get_enter_mess
 * @see swim_exit
 */
void add_exit_mess(object ob, string mess) {
  exit_messes[file_name(ob)] = mess;
}

/**
 * This function sets the next entry message from this room for a particular
 * object.  It is used automagically by the swim_exit function, but can be used
 * for other purposes if you feel like it.
 * @param ob the object for which the next entry message should be set
 * @param mess the next entry message for the object
 * @see get_enter_mess
 * @see add_exit_mess
 * @see get_exit_mess
 * @see swim_exit
 */
void add_enter_mess(object ob, string mess) {
  enter_messes[file_name(ob)] = mess;
}

/**
 * This function returns the appropriate exit message for the specified object
 * in the specified direction.  If a value has been set by add_exit_mess then
 * it is returned.  It is used automagically by the swim_exit function, but can
 * be overridden if you feel like it.
 * @param ob the object for which the exit message should be found
 * @param direc the direction in which the object is exiting
 * @return the exit message for this object
 * @see add_exit_mess
 * @see add_enter_mess
 * @see get_enter_mess
 */
string get_exit_mess(object ob, string direc) {
  string retval;
  if (retval = exit_messes[file_name(ob)]) {
    map_delete(exit_messes, file_name(ob));
    return retval;
  }
  return "$N $V$0=swims,swim$V$ $T.";
}

/**
 * This function returns the appropriate entry message for the specified object
 * in the specified direction.  If a value has been set by add_enter_mess then
 * it is returned.  The query_origin function is used to find a replacement for
 * the "$F" token.  It is used automagically by the swim_exit function, but can
 * be overridden if you feel like it.
 * @param ob the object for which the entry message should be found
 * @param direc the direction in which the object is exiting
 * @return the entry message for this object
 * @see add_enter_mess
 * @see add_exit_mess
 * @see get_exit_mess
 * @see query_origin
 */
string get_enter_mess(object ob, string direc) {
  string retval;
  if (retval = enter_messes[file_name(ob)]) {
    map_delete(enter_messes, file_name(ob));
  }
  else if (direc == up_dir) {
    retval = "$N $V$0=swims,swim$V$ up from $F.";
  }
  else if (direc == down_dir) {
    retval = "$N $V$0=swims,swim$V$ down from $F.";
  }
  else {
    retval = "$N $V$0=swims,swim$V$ in from $F.";
  }
  retval = replace(retval, "$F", query_origin(direc));
  return retval;
}

/**
 * @ignore yes
 */
void init()  {
  add_command("float", "", (: do_float() :));
  add_command("drift", "", (: do_drift() :));
  add_command("swim", "", (: do_swim() :));
}

/**
 * @ignore yes
 */
string mangle_speech(string type, string words, mixed target) {
  int drown;
  string garbled = "";
  if (query_surface() || !this_player() ||
           this_player()->query_property("dead")) {
    return words;
  }
  switch (type) {
  case "whisper":
    drown = 20 + random(20);
    break;
  case "lsay":
  case "mock":
    drown = 80 + random(80);
    break;
  case "shout":
    drown = 120 + random(120);
    break;
  default:
    drown = 40 + random(40);
  }
  for (int inc = (strlen(words) / 10) + 1;inc > 0;--inc) {
    garbled += ({"blub", "glub", "gloog", "arrrble"})[random(4)]+" ... ";
  }
  if (drown > 70) {
    garbled = garbled[0..(strlen(garbled) - 6)]+"!";
  } else {
    garbled = garbled[0..(strlen(garbled) - 2)];
  }
  garbled = capitalize(garbled);
  last_speech_volume = drown;
  return garbled;
}

/**
 * @ignore yes
 */
void event_person_say(object ob, string start, string mess, string lang,
           string accent) {
  int effnum, *args;
  if (!ob || ob->query_property("dead") || ob->query_property(GILLS_PROP) ||
      lives_in_water( ob ) || environment(ob) != this_object() || query_surface()) {
    last_speech_volume = 0;
    return;
  }

  if (!last_speech_volume) {
    last_speech_volume = 40 + random(40);
  }
  effnum = get_swim_enum(ob);
  args = ob->arg_of(effnum);
  args[1] += last_speech_volume;
  ob->set_arg_of(effnum, args);
  ob->adjust_tmp_con(-random((last_speech_volume / 50) + 1));
  switch (last_speech_volume) {
  case 0..30:
    tell_object(ob, "You inhale a bit of water.\n");
    break;
  case 31..80:
    tell_object(ob, "You inhale some water.\n");
    break;
  case 81..150:
    tell_object(ob, "You inhale a fair amount of water.\n");
    break;
  default:
    tell_object(ob, "You inhale about a lungful of water.\n");
  }
  last_speech_volume = 0;
}

/**
 * This function is for the float command, which will allow someone to start
 * floating freely rather than swimming.  It is identical to the drift command,
 * except for the messages.
 * @return whether the command succeeded
 * @see init
 * @see do_drift
 * @see do_swim
 */
int do_float()  {
  int *args, effnum = get_swim_enum(this_player());
  args = this_player()->arg_of(effnum);
  if (!args[0]) {
    if (sizeof(filter(query_flows(), (: $2 :)))) {
      add_failed_mess("You are already floating on the current.\n");
    }
    else {
      add_failed_mess("You are already floating freely.\n");
    }
    return 0;
  }
  this_player()->set_arg_of(effnum, ({0, args[1]}));
  if (sizeof(filter(query_flows(), (: $2 :)))) {
    add_succeeded_mess(({"You begin to float freely on the current.\n", ""}));
  }
  else {
    add_succeeded_mess(({"You begin to float freely.\n", ""}));
  }
  return 1;
}

/**
 * This function is for the drift command, which will allow someone to start
 * drifting freely rather than swimming.  It is identical to the float command,
 * except for the messages.
 * @return whether the command succeeded
 * @see init
 * @see do_float
 * @see do_swim
 */
int do_drift()  {
  int *args, effnum = get_swim_enum(this_player());
  args = this_player()->arg_of(effnum);
  if (!args[0]) {
    if (sizeof(filter(query_flows(), (: $2 :)))) {
      add_failed_mess("You are already drifting with the current.\n");
    }
    else {
      add_failed_mess("You are already drifting freely.\n");
    }
    return 0;
  }
  this_player()->set_arg_of(effnum, ({0, args[1]}));
  if (sizeof(filter(query_flows(), (: $2 :)))) {
    add_succeeded_mess(({"You begin to drift freely with the current.\n",
           ""}));
  }
  else {
    add_succeeded_mess(({"You begin to drift freely.\n", ""}));
  }
  return 1;
}

/**
 * This function is for the swim command, which will allow someone to stop
 * drifting freely and start swimming and resisting anything that tries to move
 * them.
 * @return whether the command succeeded
 * @see init
 * @see do_float
 * @see do_drift
 */
int do_swim()  {
  int *args, effnum = get_swim_enum(this_player());
  args = this_player()->arg_of(effnum);
  if (args[0]) {
    if (sizeof(filter(query_flows(), (: $2 :)))) {
      add_failed_mess("You are already swimming against the current.\n");
    }
    else {
      add_failed_mess("You are already swimming.\n");
    }
    return 0;
  }
  this_player()->set_arg_of(effnum, ({1, args[1]}));
  if (sizeof(filter(query_flows(), (: $2 :)))) {
    add_succeeded_mess(({"You begin to swim against the current.\n", ""}));
  }
  else {
    add_succeeded_mess(({"You begin to swim.\n", ""}));
  }
  return 1;
}

/**
 * This function returns the effect number of the swimming/immersion effect on
 * the object specified.  If there isn't one, it adds the effect.
 * @param thing the object for which the swimming effect number should be found
 * @return the swimming effect number
 * @see effects_matching
 * @see query_classification
 */
int get_swim_enum(object thing) {
  int *effects = thing->effects_matching(SWIM_EFFECT->query_classification());
  if (!effects || !sizeof(effects)) {
    thing->add_effect(SWIM_EFFECT, 1);
    effects = thing->effects_matching(SWIM_EFFECT->query_classification());
    call_out("soak", 1, thing);
  } else if (!random(50)) {
    call_out("soak", 1, thing);
  }
  return effects[0];
}

/**
 * This function returns 1 if this is a bottom room, and 0 otherwise.
 * @return whether this is a bottom room
 * @see set_bottom
 */
int query_bottom()  {
  if (bottom == -1) {
    return member_array(down_dir, query_exits()) == -1;
  }
  return bottom;
}

/**
 * This function returns 1 if this is a surface room, and 0 otherwise.
 * @return whether this is a surface room
 * @see set_surface
 * @see query_underwater
 */
int query_surface() {
    if (surface == -1) {
        return member_array(up_dir, query_exits()) == -1;
    }
    return surface;
}

/**
 * This function returns 1 if this room is underwater (that is, it is not a
 * surface room), and 0 otherwise.
 * @return whether this is an underwater room
 * @see query_surface
 * @see set_surface
 */
int query_underwater() {
  return !query_surface();
}

/**
 * This function returns 1 to indicate that this is a water room.  It fulfills
 * the same purpose as the inherits efun in this case, but may be slightly
 * easier to use.
 * @return 1
 * @see inherits
 */
int query_water()  {
  return 1;
}

int lives_in_water( object ob ) {
  string race_ob = ob->query_race_ob();

  //Corpses don't live in water.
  if ( !living( ob ) ) {
    return 0;
  }

  if(ob->query_property(LIVES_IN_WATER_PROP) ||
     ob->lives_in_water() ||
     (race_ob && race_ob->lives_in_water()))
    return 1;
  return 0;
}

// The colour this room should use for the terrain map.
string query_terrain_map_colour() { return "%^CYAN%^"; }
