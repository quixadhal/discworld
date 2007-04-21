#include <terrain_map.h>

inherit "/std/object";

#define WORLD_MAP TERRAIN_MAP_WORLD_MAP

private nosave int *_area;
private nosave int _detail;
private nosave string *_features;
private nosave mapping _locations;

private nosave object _env, _pl;
private nosave int *_co_ords;
private nosave int _fudge;

/** @ignore yes */
void create() {
  do_setup++;
  ::create();
  do_setup--;

  add_alias("map");
  add_plural("maps");
  _locations = ([ ]);
  add_extra_look(this_object());
  
  if(!do_setup) {
    this_object()->setup();
  }
}

/** @ignore yes */
string extra_look() {
  return "Marked on the map are " +
    query_multiple_short(_features->query_base_description() +
                         keys(_locations)) + ".";
}

/** @ignore yes
 * Function to remove features that should not be shown on maps with
 * this level of detail.
 */
int filter_features(string feature) {
#ifdef DEBUG
  debug_printf("%s %d %d", feature,
               feature->query_max_range() / TERRAIN_MAP_ONE_MILE,
               _detail / TERRAIN_MAP_ONE_MILE);
#endif
  return (!feature->query_max_range() ||
          feature->query_max_range() >= _detail);
}

/**
 * Function to setup the details of the map.
 * The first two params give the coordinates of the top left corner
 * of the map and the second two the coordinates of the lower right corner.
 * The detail indicates the level of detail. This is given in terms of the
 * distance from which objects are visible. For example a low-detail map
 * might show objects that are visible from 200 miles or more whereas a
 * more detailed map may show items down to 50 miles visibility.
 *
 * @param x1 x part of the top left corner
 * @param y1 y part of the top left corner
 * @param x2 x part of the lower right corner
 * @param y2 y part of the lower right corner
 */
void setup_map(int x1, int y1, int x2, int y2, int detail) {
  _area = ({ x1, y1, x2, y2 });
  _detail = detail;
  _features = WORLD_MAP->query_features_in_region(x1, y1, x2, y2);
  _features = filter(_features, "filter_features", this_object());
}

/**
 * Function to set the map to one of the standard areas.  If you wish to
 * have the map cover a custom area use setup_map.
 *
 * @param area the area of Discworld the map should cover.
 */
void set_map(string area) {
  int width, height, detail, x1, y1, x2, y2;
  
  switch(area) {
  case "sur":
    width = (TERRAIN_MAP_ONE_MILE * 300);
    height = (TERRAIN_MAP_ONE_MILE * 200);
    x1 = -150575040 - width;
    y1 = -2173248 - height;
    x2 = -150575040 + width;
    y2 = -2173248 + height;
    detail = (TERRAIN_MAP_ONE_MILE * 50);
    break;
    
  case "world":
    x1 = TERRAIN_MAP_WORLD_LOW_X / 10;
    y1 = TERRAIN_MAP_WORLD_LOW_Y / 10;
    x2 = TERRAIN_MAP_WORLD_HIGH_X / 10;
    y2 = TERRAIN_MAP_WORLD_HIGH_Y / 10;
    detail = (TERRAIN_MAP_ONE_MILE * 200);
    break;
  }

  setup_map(x1, y1, x2, y2, detail);
}

/** @ignore yes
 * Recalc our current position.
 */
void recalc_coords() {
  int accuracy;
  
  if(!_co_ords || this_player() != _pl || environment(this_player()) != _env) {
    _pl = this_player();
    _env = environment(_pl);
    _co_ords = _env->query_co_ord();

    accuracy = 500 - _pl->query_skill_bonus("other.direction");
    if(accuracy < 1)
      accuracy = 1;

    _fudge = TERRAIN_MAP_ONE_MILE * accuracy;
    
    // Where we _think_ we are.
    _co_ords[0] += -(_fudge/2) + random(_fudge);
    _co_ords[1] += -(_fudge/2) + random(_fudge);
    _co_ords[2] += -(_fudge/2) + random(_fudge);
  }
}

/** @ignore yes */
int do_consult(string find) {
  string feature, *res;
  object ob;
  mapping direcs;
  
  if(this_player()->check_dark(environment(this_player())->query_light()))
    return notify_fail("Sorry the light levels are not conducive to "
                       "reading a map.\n");
  recalc_coords();
  
  if(_co_ords[0] < _area[0] || _co_ords[1] < _area[1] ||
     _co_ords[0] > _area[2] || _co_ords[1] > _area[3])
    return notify_fail("Sorry, you do not appear to be anywhere on this "
                       "map.\n");

  res = ({ });
  foreach(feature in _features) {
    ob = feature->query_region_ob();
    if(ob) {
#ifdef DEBUG
      debug_printf("%O max range %d", feature,
                   feature->query_max_range()/TERRAIN_MAP_ONE_MILE);
#endif
      direcs = ob->query_feature_desc_from(_co_ords[0], _co_ords[1],
                                           _co_ords[2], 1);

      if(direcs) {
        if(find == "" || strsrch(lower_case(feature->query_base_description()),
                   lower_case(find)) != -1)
          res += ({ feature->calc_map_feature_desc(direcs, _fudge) });
      }
    }
  }

  if(res == ({ })) {
    if(find != "")
      return notify_fail("You cannot find " + find + " on your map.\n");
    else
      return notify_fail("You cannot find anything on your map.\n");
  }
  write("You consult your map and estimate that " +
        query_multiple_short(res) + "\n");
  this_player()->add_succeeded_mess(this_object(), "");
  return 1;
}

/** @ignore yes */
int do_add(string location) {
  if(member_array(lower_case(location), keys(_locations)) != -1)
    return notify_fail("A location with the name " + location +
                       " already exists on this map.\n");
  _locations[lower_case(location)] = _co_ords;
  this_player()->add_succeeded_mess(this_object(), "$N $V a new location "
                                    "to $D.\n");
  return 1;
}

/** @ignore yes */
void init() {
  this_player()->add_command("consult", this_object(), "<direct:object>",
                             (: do_consult("") :));
  this_player()->add_command("find", this_object(),
                             "<string'place'> on <direct:object>",
                             (: do_consult($4[0]) :));
  this_player()->add_command("add", this_object(),
                             "<string'description'> to <direct:object>",
                             (: do_add($4[0]) :));
}

/** @ignore yes */
mapping int_query_static_auto_load() {
  return ([
    "::": ::int_query_static_auto_load(),
    "features": _features,
    "detail": _detail,
    "area": _area,
  ]);
}

/** @ignore yes */
mixed query_static_auto_load() {
  if((file_name(this_object()))[0..7] != "/std/map" )
    return 0;
  return int_query_static_auto_load();
}

/** @ignore yes */
void init_static_arg(mapping args) {
  if(args["::"])
    ::init_static_arg(args["::"]);
  if(args["features"])
    _features = args["features"];
  if(args["detail"])
    _detail = args["detail"];
  if(args["area"])
    _area = args["area"];
}

