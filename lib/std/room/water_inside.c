/**
 * Standard inside water room inheritable.  This is mainly just a mux
 * for the functions that are defined in both /std/room and
 * /std/room/basic/water.
 * @author Presto, Jeremy, Bakhtosh
 * @see /std/room.c
 * @see /std/room/basic/water.c
 */

inherit "/std/room/basic_room";
inherit "/std/room/basic/water";

private int _setting_water_light = 0;

/**
 * @ignore yes
 */
void init() {
  water::init();
  basic_room::init();
}

/**
 * @ignore yes
 */
int is_allowed_position(string poss) {
  return water::is_allowed_position(poss);
}

/**
 * @ignore yes
 */
mixed *query_default_position(object ob) {
  return water::query_default_position(ob);
}

/**
 * @ignore yes
 */
int add_exit(string direc, mixed dest, string type) {
  int retval = basic_room::add_exit(direc, dest, type);
  water::add_exit(direc, dest, type);
  return retval;
}

/**
 * @ignore yes
 */
int query_light() {
  update_water_light();
  if (query_surface()) {
    return ::query_light();
  }
  return ::query_light()*query_clarity()/100;
}

/**
 * This function is used to set the light in a water room without overriding
 * the use of set_use_surface_light(1).  There should be no reason to use it
 * outside of the water inheritable.
 * @param number the new light level
 * @return the new light level
 * @see set_light
 * @see set_use_surface_light
 * @see update_water_light
 */
int set_water_light(int number) {
  _setting_water_light = 1;
  set_light(number);
  _setting_water_light = 0;
}

/**
 * @ignore yes
 */
int adjust_light(int number) {
  if (!_setting_water_light) {
    set_use_surface_light(0);
  }
  return ::adjust_light(number);
}

/**
 * @ignore yes
 */
mixed query_property(string tag) {
  if ("location" == tag && !query_surface()) {
    return "underwater";
  }
  return ::query_property(tag);
}

/**
 * This method sets the default attack speed for the room.
 * This defaults to 5 for water rooms.
 * @return the default attack speed
 */
int attack_speed() { return 5; }
