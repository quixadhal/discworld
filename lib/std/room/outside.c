#include <weather.h>

inherit "/std/room/basic_room";

#define FAST_CLEAN_UP 15
//#define SLOW_CLEAN_UP 180
//#define CLEAN_UP_LOG "clean_up"

nosave int current_day;
nosave object sunlight;
nosave int day_light;
nosave string _climate;
nosave int next_weather_update;
nosave string weather_str;
private nosave string _weather_handler;

// this is for the new call_out system, and the function stuff...
// it incorporates a much faster 'handle' to grab call_outs with.
nosave int callout_handle;
nosave int room_stabilize;
private nosave int room_create_time;
private nosave int _fast_clean_up;

int clean_up_room( int flag );
int query_fast_clean_up();
void set_fast_clean_up(int fast_clean_up);

/**
 * This sets the weather handler that the room will use.
 * @param the handler to use.
 */
void set_weather_handler( string handler ) {
    _weather_handler = handler ;
} /* set_weather_handler */

/**
 * This returns the path to the weather handler currently being used.
 */
string query_weather_handler() {
    return _weather_handler ;
} /* query_weather_handler */

/**
 * This method sets the climate for this room from the standard
 * climate types.
 * @param a climate type string
 * @see /obj/handlers/weather
 */
int set_climate(string climate) {
  if(member_array(climate, CLIMATES) == -1)
    return 0;
  
  _climate = climate;
  return 1;
}

/**
 * This method returns the climate type for this room
 * @return climate string
 * @see /obj/handlers/weather
 */
string query_climate() {
  if(!_climate)
    return DEFAULT_CLIMATE;
  
  return _climate;
}

void set_light_change(string s) { 
    add_property("light change", s); 
} /* set_light_change() */

string query_light_change() { 
    return query_property("light change"); 
} /* query_light_change() */

/**
 * Returns the snow look for the snow item.
 */
string snow_look() {
   return _weather_handler->snow_string(this_object()) + ".";
} /* snow_look() */

/**
 * Returns the rain look for the rain item.
 */
string rain_look() {
   return _weather_handler->rain_string(this_object()) + ".";
} /* rain_look() */

/**
 * Returns the sleet look for the sleet item.
 */
string sleet_look() {
   return _weather_handler->sleet_string(this_object()) + ".";
} /* sleet_look() */

/**
 * Returns the cloud look for the cloud item.
 */
string cloud_look() {
   return _weather_handler->cloud_string(this_object()) + ".";
} /* cloud_look() */

void create() {
  set_fast_clean_up(1);
  do_setup++;
  ::create();
  do_setup--;
  set_room_size( 50 );
  add_property( "location", "outside" );
  add_property( "here", "on the ground" );
  _weather_handler = WEATHER ;

  /*
  add_item("snow", ({ this_object(), "snow_look" }));
  add_item("rain", ({ this_object(), "rain_look" }));
  add_item("sleet", ({ this_object(), "sleet_look" }));
  add_item("cloud", ({ this_object(), "cloud_look" }));
  */
  
  if ( !do_setup ) {
    this_object()->setup();
    this_object()->reset();
  }
  
  room_create_time = time();
  room_stabilize = 0;
} /* create() */

/** @ignore yes */
int query_light() {
  int new_light;

  new_light = ( day_light * (int)_weather_handler->query_darkness
    ( this_object() ) ) / 100;
  if ( new_light != query_my_light() ) {
    ::set_light(new_light);
  }
  return ::query_light();
} /* query_light() */

/** @ingore yes */
int query_visibility() {
   return _weather_handler->query_visibility(this_object());
}

/**
 * This method returns the current level of day light in the room.  The
 * day light is the light level the room is set to if there was full sun
 * and no darkness at all.  Mostly a percentage of this is calculated
 * in the query_light() method.
 * @return the current daylight level
 */
int query_day_light() { return day_light; }

/** @ignore yes */
int set_light( int number ) {
  ::set_light(number);
  day_light = number;
} /* set_light() */

/** @ignore yes */
string long( string word, int dark ) {
   int day_time;
   string ret;
   string obscure;
   mixed bit;

/* No point reinventing the wheel... */
   ret = ::long( word, dark );
   day_time = (int)_weather_handler->query_day( this_object() );
   if ( day_time != current_day ) {
      if ( query_light_change() ) {
         call_other( this_object(), query_light_change(), current_day );
      }
   }
   obscure = this_object()->query_weather_obscured(day_time);
   if ( !day_time )
   {
      bit = query_night_long();
      if (functionp(bit)) {
         bit = evaluate(bit, word, dark);
      }
      ret = replace(ret, "$long$", bit);
      if (!obscure) {
         ret = (string)_weather_handler->query_moon_string() + ret;
      }
   } else {
      bit = query_long();
      if (functionp(bit)) {
         bit = evaluate(bit, word, dark);
      }
      ret = replace( ret, "$long$", bit);
   }
   if ( time() > next_weather_update ) {
      weather_str = _weather_handler->weather_string(this_object(), obscure) +".\n";
      next_weather_update = (int)_weather_handler->next_update();
   }
   return replace(ret, "$weather$",
                  this_player()->colour_event("weather", "%^ORANGE%^")+
                  weather_str+ "%^RESET%^");
} /* long() */

/** @ignore yes */
mixed *stats() {
  return ::stats() + ({
    ({ "day light", day_light, }),
  });
} /* stats() */

/* added by Funtime to catch daylight searches */
/* 31/Aug/1995 */
/* the returned objects should be an array, hence sunlight[0] */
/** @ignore yes */
object *find_inv_match( string word, object looker ) {
  int daylight;

  if((word == "sunlight") || (word == "daylight"))
  {
    daylight = this_object()->query_light();
    /* nighttime - kill the light object as well */
    if(daylight == 0)
    {
      if(sunlight)
      {
        remove_hidden_object(sunlight);
        sunlight->dest_me();
        sunlight = 0;
      }
      return ::find_inv_match( word, looker );
    }

    /* any other strength of light we should have an object of some kind */
    /* see if we have a sunlight object already - if not, create */
    if(!sunlight)
    {
      sunlight = clone_object("/std/object");
      sunlight->set_name("sunlight");
      sunlight->set_weight(0);
      sunlight->reset_get();
/*       sunlight->move(this_object()); */
      sunlight->add_alias("daylight");
      add_hidden_object(sunlight);
    }

    /* see if the daylight strength is high enough */
    if(daylight < 25)
    {
      sunlight->set_long("The light here is currently quite dim.\n");
    } else {
      sunlight->set_long("What do you want to look at the light for?  "+
        "It's... well, light really.\n");
    }
  }

  /* object has been added to the inv of the room, so  */
  /* search the room as normal */

  return ::find_inv_match( word, looker );
} /* find_inv_match() */

/** @ignore yes */
void dest_me() {
  if(sunlight) {
    sunlight->dest_me();
  }
  ::dest_me();
} /* dest_me() */


#ifdef FAST_CLEAN_UP
/*
 * The following code simply detects runthroughs and dests the room
 * if that happens. ie. if the room is empty within a few seconds of being
 * created it will dest itself.
 *
 * Ceres July 1997
 */
/** @ignore yes */
void event_exit(object ob, string message, object to) {
  if(!room_stabilize) {
    remove_call_out(callout_handle);
    callout_handle = call_out("clean_up_room", FAST_CLEAN_UP, 0);
  }
}

/** @ignore yes */
string *query_zones() {
  // monsters call this to move, but may not actually come here,
  // so potential clean_up_room {Laggard}
  if(!room_stabilize)
    callout_handle = call_out("clean_up_room", FAST_CLEAN_UP, 0) ;

  return ::query_zones();
} /* query_zones() */

// use call_out to attempt faster clean up {Laggard}
// this is almost but not quite the same as the preceeding function.
// the differences are not subtle.
/** @ignore yes */
int clean_up_room(int flag) {
  object *obs, ob;
  int elapsed_time = time() - room_create_time;
  
  if(room_stabilize || !query_fast_clean_up() ||
     this_object()->query_keep_room_loaded())
    return 0;
   
  callout_handle = 0 ;

  // We don't cleanup if this room contains any: users, non-living objects,
  // npcs that are not transient, or transients unable to move.
  obs = filter(all_inventory(this_object()),
               (: !living($1) || (userp($1)) ||
                !$1->query_property("transient") ||
                sizeof($1->query_attacker_list()) ||
                $1->cannot_walk() :));

  if(sizeof(all_inventory(this_object())) ||
     elapsed_time > (FAST_CLEAN_UP * 3)) {
    room_stabilize = 1;
    return 1;
  }

  // Move transients back to their hospital for recycling.
  obs = filter(all_inventory(this_object()),
               (: $1->query_property("transient") :));
  foreach(ob in obs)
    if(ob->query_property("hospital"))
      ob->move(ob->query_property("hospital"), "$N wander$s in.",
               "$N wander$s out.");
  
  dest_me();
  return 0;
} /* clean_up_room() */

/**
 * This function allows you to disable the fast clean-up of outside
 * rooms.
 *
 * @see clean_up_room
 * @param number 1 to allow fast clean-up, 0 to disable it
 */
void set_fast_clean_up(int number) {
   _fast_clean_up = number;
} /* set_fast_cleanup() */

/**
 * This function allows you to check whether or not fast clean-up
 * is currently allowed.
 *
 * @return 0 if it is disabled, 1 if it is allowed
 */
int query_fast_clean_up() {
   return _fast_clean_up;
} /* query_fast_cleanup() */
#endif
