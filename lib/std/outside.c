#include "weather.h"
inherit "/std/room";
 
/*
 * ok this is the out side room standard, It includes weather and
 * all that jazz
 */

/*
 * The night long is for rooms where you want the describe to change at
 * night.
 */
static string night_long;

/*
 * This is the string it will display instead of it is dark.
 */
static string dark_long;

/*
 * It will call this function on the room when it changes from dark to
 * light.  ie...  changes from day to night.
 */
static string light_change;
static int current_day;
 
void set_dark_long(string s) { dark_long = s; }
string query_dark_long() { return dark_long; }

void set_night_long(string s) { night_long = s; }
string query_night_long() { return night_long; }

void set_light_change(string s) { light_change = s; }
string query_light_change() { return light_change; }
 
void create() {
  dark_long = "It is dark";
  room_size = 50;
  ::create();
  add_property("location", "outside");
} /* create() */
 
string short(int dark) {
  if (dark)
    return query_dark_mess();
  else
    return ::short();
} /* short() */
 
string long(string str, int dark) {
int i;
string s, ret;

  if ((int)WEATHER->query_day(this_object()) != current_day) {
    if (light_change)
      call_other(this_object(), light_change, current_day);
/* The dark level might have changed... */
    dark = (int)this_player()->check_dark(query_light());
  }
  if (dark && !query_night_long())
    return query_dark_long()+".\n";
  if (!str) {
    if (pointerp(co_ord) && !WEATHER->query_day(this_object())) {
/* night... */
      s = (string)WEATHER->query_moon_string(this_object());
      if (!s)
        ret =  "It is night and the moon is not up.\n";
      else if ((int)WEATHER->cloud_index(this_object()) > 20)
        ret = "It is night and the "+s+" is hidden by the clouds.\n";
      else
        ret = "The land is lit up by the eerie light of the "+s+".\n";
      if (!query_night_long())
        ret += query_long();
      else
        ret += query_night_long();
    } else
      ret = query_long();
    s = calc_extra_look();
    if (s && s != "")
      ret += s;
    if (pointerp(co_ord))
      ret += (string)WEATHER->weather_string(this_object())+".\n";
    if (!exit_string)
      s = query_dirs_string();
    else
      s = exit_string;
    ret += s + "\n";
/* Return the long + the contents of the room. */
    return ret+query_contents("");
  }
  str = expand_alias(str);
  return items[str];
} /* long() */
 
/* percentage system */
int query_light() {
  int i;

  i = (int)WEATHER->query_darkness(this_object());
  return ::query_my_light()*i/100 + ::query_int_light();
} /* query_light() */
