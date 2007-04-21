/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: weather.c,v 1.32 2002/11/15 07:02:35 ceres Exp $
 * 
 */
/**
 * An attempt at a new weather handler for Discworld.
 * Ceres@Discworld 14/4/96
 *
 * Having thought about the way weather works (and Discworlds interesting
 * meteorology) I decided not to try and implement some model of realworld
 * weather. Instead regional variations in weather are handled by
 * regional climate properties. This system will handle diurnal and seasonal
 * variations and that's about it. Hopefully that will make it more
 * understandable, usable and efficient.<p>
 *
 * Basic Operation<br>
 * ---------------<br>
 * The weather is based on weather 'pattern's. The pattern lasts for several
 * hours and is calculated from the 'seasonal' norm and the 'variance'. The
 * 'current' weather will tend towards the pattern weather.<p>
 * 
 * The pattern depends on the climate. The climates are taken from the Koppen
 * Climate Classification system. The possible climates are:<br>
 * tropical<br>
 * - rainforest 'Af' eg. amazon, congo, malaysia<br>
 * - monsoon 'Am' <br>
 * - savanna 'Aw' eg. caribbean, tropical africa, central america<br>
 * desert<br>
 * - arid 'Bw'<br>
 * - semi-arid 'Bs' <br>
 * mediterranean (aka Moist Subtropical Mid-Latitude Climates)<br>
 * - humid subtropical 'Cfa' eg. Souteast US (florida)<br>
 * - mediterranean 'Cs' eg. california, south africa, western/central australia<br>
 * - marine 'Cfb' eg. US west coast, nw europe<br>
 * continental<br>
 * - dry winters 'Dw' <br>
 * - dry summers 'Ds'<br>
 * - wet 'Df'<br>
 * polar<br>
 * - tundra 'Et'<br>
 * - frozen 'Ef'<br>
 * highland 'H' <p>
 * 
 * The default for a room is mediterranean marine (Cfb).<p>
 * 
 * The actual temparture in a room will depend on the current weather for that
 * climate, local modifiers (climate property), diurnal variations and
 * variance.<p>
 *
 * Thus update_patttern() is called once every hour (CYCLE_SPEED) and
 * update_weather() is called every 5 minutes (UPDATE_SPEED).<p>
 *
 * Decisions<br>
 * ---------<br>
 * Calculation of the general weather has been moved into
 * update_weather. Rather than doing it on a room by room basis everytime
 * someone looks! Instead, local climate controls will be added verbatim to
 * the current global weather. This will improve efficiency massively, but will
 * reduce the overall weather variations.<p>
 *
 * Principles<br>
 * ----------<br>
 * Rain/sleet/snow depends on cloud density and temperature.<br>
 * If it's windy and lots of cloud density we might get a thunder storm.
 * Cloud cover should lower temperature as should wind.<p>
 *
 * Usage<br>
 * -----<br>
 * To alter the weather in a given room the easiest approach is to set the
 * climate. Individual room or area variations can be done by adding a
 * climate property.<br>
 * The property should contain three integers, temperature, cloud cover and
 * wind speed. These modify the default values for the room.<p>
 * For example:<br>
 * add_property("climate", ({10, -50, 0});<br>
 * This will increase the temperature by 10C, reduce the cloud cover
 * by 50% and make no change to the wind speed.<p>
 *
 * To completely alter the weather in a room you need to define the function
 * room_weather() as follows:<br>
 *   int *room_weather(int temp, int cloud, int wind, int rain);<br>
 * Your room_weather function will be called with the values the room would
 * ordinarily receive and should return the new temperature, cloud cover,
 * wind speed and rain type values that you desire.<p>
 *
 * If you need to make your room react to changes in the weather you can use
 * the weather_notify() system to be told when the weather changes.<br>
 * This should only be done for a small number of rooms or the lag will be
 * excessive.<p>
 *
 * Changed Jun 3 1996 by Grampa to include weather notifications for rooms.<p>
 * @index weather
 */
#include <climate.h>
#include <am_time.h>
#include <weather.h>

#define FILE_NAME "/save/nweather"
#define UPDATE_SPEED 300
#define CYCLE_SPEED  3600
#define DIY 400

// return the difference between two numbers.
#define DIF(x, y) (x > y) ? (x-y) : (y-x)

// this is the basic state of the weather. The local climate properties are
// then applied to these numbers, followed by the time of day and year to
// give us the actual weather right now.

// temp is the temperature in celcius.
// cloud is the amount of cloud cover. 100 is total cloud cover. >100 the
//          cloud gets denser and denser. Denser cloud makes rain more likely.
// windsp is the wind speed in mph.

// underlying temp etc.
private mapping _pattern;

// the current global values of temp etc.
private mapping _current;

// how much global variance do we want?
private int *_variance;

// Rooms to notify about weather changes.
// Keys are rooms, values are notification values.  Used so there will be
// no duplicates.
private nosave mapping _rooms;

private nosave int _lastupdate;
private nosave int _day;
private nosave int _sunrise;
private nosave int _sunset;
private nosave int _toy;
private int mooncycle;
private int moonupdate;

void set_day();
void update_pattern();
int calc_actual(object env, int type);
int cloud_index(object env);
int query_day(object env);
int query_rain_type(int temp, int rain);
string sun_direction(int which);
int temperature_index(object env);
int rain_index(object env);

/* Interface Functions (ones called from outside) */
/** @ignore yes */
void create() {
  string str;

  _rooms = ([ ]);
  
  unguarded((: restore_object, FILE_NAME :));

  if(!_pattern)
    _pattern = ([ ]);
  if(!_current)
    _current = ([ ]);
  if(!_variance)
    _variance = ({ 10, 75, 20 });
  foreach(str in CLIMATES) {
    if(!_pattern[str])
      _pattern[str] =  ({ 0, 0, 0 });
    if(!_current[str])
      _current[str] =  ({ 0, 0, 0 });
  }

  call_out("update_weather", UPDATE_SPEED);
  set_day();
  update_pattern();

}

/**
 * Add a room to the list of rooms to notify about weather.  Notifications
 * are an ORed (|) combination of the #defines in /include/climate.h
 * IE: To inform about temperature and rain changes, call
 * weather_notify( room, (NOTIFY_TEMPERATURE | NOTIFY_RAIN) );
 *
 * @param which_room the room to be notified
 * @param notifications a bitmap of notifications
 * @return success or failure (1 or 0)
 */
int weather_notify( object which_room, int notifications ) {
  if(notifications < 0 || notifications >
     (NOTIFY_TEMPERATURE | NOTIFY_CLOUD | NOTIFY_RAIN | NOTIFY_DAY)) {
    return 0;
  }

  _rooms[which_room] = notifications;
  return 1;
}

/** @ignore yes */
void test_notifications() {
  object room;
  string w;

  foreach( room in keys(_rooms)) {
    w = sprintf("%O notified of %s%s%s%s\n", room,
    ((_rooms[room] & NOTIFY_TEMPERATURE) ? "Temperature " : ""),
                ((_rooms[room] & NOTIFY_CLOUD) ? "Clouds " : ""),
                ((_rooms[room] & NOTIFY_RAIN) ? "Rain " : ""),
                ((_rooms[room] & NOTIFY_DAY) ? "Day" : ""));
    write( w );
  }
  write("test_notifications() done\n");
}

/**
 * Find out which day of the Disc (common) year it is.
 * @return the day of the year.
 */
int day_number() {
  return ( time() % AM_SECONDS_PER_HALF_YEAR ) / AM_SECONDS_PER_DAY;
}

/**
 * Find out which minute of the day (0 - 1439) it is.
 * @return the minute.
 */
int minute_number() {
  return (( time() % AM_SECONDS_PER_DAY ) / AM_SECONDS_PER_MINUTE);
}

/** 
 * Get the appropriate moon string. This is almost completely from the old
 * system. I don't understand the moon.
 *
 * @param env the room to check
 * @return the moon string.
 */
string query_moon_string(object env) {
  string str;

  // New, simplified moon string stuff
  if(mooncycle > 10)
    return "It is night and there is no moon.\n";
  
  if(calc_actual(env, CLOUD) > 70)
    str = "It is night and the %s is hidden by the clouds.\n";
  else if(env && env->query_water())
    str = "The water is lit up by the eerie light of the %s.\n";
  else
    str = "The land is lit up by the eerie light of the %s.\n";

  return sprintf(str, ({"waxing crescent moon",
                        "waxing quarter moon",
                        "waxing half moon",
                        "waxing three quarter moon",
                        "waxing gibbous moon",
                        "full moon",
                        "waning gibbous moon",
                        "waning three quarter moon",
                        "waning half moon",
                        "waning quarter moon",
                        "waning crescent moon",
                        "no moon",
                        "no moon",
                        "no moon",
                      })[mooncycle]);
}

/**
 * Query the state of the moon.
 *
 * @return an integer indicating the state of the moon.
 * 0 - no moon, 1 - crescent, 2 - quarter, 3 - half, 4 - three quarter,
 * 5 - gibbous, 6 - full.
 */
int query_moon_state() {
  if(mooncycle < 6)
    return mooncycle+1;
  if(mooncycle < 11)
    return (11 - mooncycle);
  return 0;
}

/**
 * Find out which phase of the moon we're currently in.
 *
 * @return a string for the phase of the moon.
 */
string query_moon_phase() {
  return ({"crescent",
           "quarter",
           "half",
           "three quarter",
           "gibbous",
           "full",
           "gibbous",
           "three quarter",
           "half",
           "quarter",
           "crescent",
           "",
           "",
           "",
         })[mooncycle];
}

/**
 * Return the number of currently visible stars as a percentage.
 * @return A number 0-100, 0 being no stars.
 */
int query_stars( object env ) {
    int clouds, stars ;
    
    clouds = cloud_index(env) ;
    
    // No stars in the daytime, or when it's overcast.
    if ( query_day(env) || clouds > 70 ) {
        return 0 ;
    }
    
    switch( clouds ) {
        case 0:
            stars = 100 ;
            break ;
        case 1..39:
            stars = 80 ;
            break ;
        case 40..59:
            stars = 40 ;
            break ;
        case 60..69:
            stars = 20 ;
            break ;
        default:
            stars = 10 ;
            break ;
    }
    
    return stars ;
}

/**
 * Returns a string describing the current visible stars.
 * @return a string for the visible stars.
 */
string query_star_string( object env ) {
    string str ;
    
    if ( query_day(env) ) {
        return "There are no stars visible during the day.\n" ;
    }
    
    switch( query_stars(env) ) {
        case 1..20:
            str = "one or two bright stars" ;
            break ;
        case 21..40:
            str = "a few stars" ;
            break ;
        case 41..60:
            str = "quite a few stars" ;
            break ;
        case 61..80:
            str = "lots of stars" ;
            break ;
        case 81..100:
            str = "countless stars" ;
            break ;
        default:
            str = "no stars" ;
            break ;
    }
    
    return "Tonight, there are " + str + " visible.\n" ;
}

/**
 * Return which of the 4 seasons we are in.
 * @return spring, summer, autumn or winter.
 */
string query_season() {
  switch(day_number()) {
  case 0..50: return "winter";
  case 51..150: return "spring";
  case 151..250: return "summer";
  case 251..350: return "autumn";
  case 351..400: return "winter";
  }
}

/**
 * Return a string for the period of the day
 *
 * @return night, morning, midday, afternoon or evening.
 */
string query_tod() {
  if((minute_number() < _sunrise) || (minute_number() > _sunset))
    return "night";
  switch(minute_number()) {
  case 0..700:
    return "morning";
  case 701..740:
    return "midday";
    break;
  case 741..1080:
    return "afternoon";
    break;
  default:
    return "evening";
  }
}

/** @ignore yes */
int query_day(object env) {
  return _day;
}

/**
 * return percentage of light
 * should be dependant on the base amount of light, day or night time and
 * cloud cover.
 *
 * @param the room
 * @return a percentage
 */
int query_darkness(object env) {
  int result, cloud, day;

  result = 10;
  day = query_day(env);
  
  if(day == 10)
    return 100;

  if(mooncycle <= 10) {
    if(mooncycle < 6)
      result += mooncycle * 10;
    else
      result += (5 - (mooncycle % 6)) * 10;
    }
  
  // now reduce this for cloud cover
  if((cloud = cloud_index(env)) > 0)
    result -= (cloud_index(env) / 15);
  if(result < 0)
    result = 0;

  if(day > 0)
    result += (100 - result) / (10 - day);

  return result;
}

/**
 * return percentage of visibility.
 * It should also depend on things like rain/snow etc, anything which
 * will effect visibility.
 *
 * @param the room
 * @return a percentage
 */
int query_visibility(object env) {
  int result;
  int temp;
  int rain;
  int rt;

  // This has to work in conjunction with darkness.
  result = 100;

  temp = temperature_index(env);
  rain = rain_index(env);
  rt = query_rain_type(temp, rain);

  if (rt ==  WEATHER_SNOW_TYPE) {
     // Snow decreases visibility a lot.
     rain += 50;
  } else if (rt == WEATHER_SLEET_TYPE) {
     // Sleet decreases visbility a bit.
     rain += 20;
  }

  if (rain > 0) {
     if (rain > 100) {
        result = 0;
     } else {
        result = (result * (100 - rain)) / 100;
     }
  }

  return result;
}

/** @ignore yes */
int calc_actual(object env, int type) {
  int *clim;
  string climate;

  if(env) {
    climate = env->query_climate();
    clim = (int *)env->query_property("climate");   
  }
  
  if(!climate)
    climate = DEFAULT_CLIMATE;
  
  if(!clim)
    return _current[climate][type];
  
  return _current[climate][type] + clim[type];
}

/** @ignore yes */
int temperature_index(object env) {
  int temp, tod, j, diurnal;
  string climate;
  temp = calc_actual(env, TEMP);

  if(env)
    climate = env->query_climate();
  
  if(!climate)
    climate = DEFAULT_CLIMATE;
  
  // gives us a number between 0 (midday) and 10 (night)
  tod = 10 - query_day(env);

  // add diurnal variations to temperature
  if(tod) {
    switch(climate) {
    case "Af":
    case "Am":
    case "Aw":
      diurnal = 10;
      break;
    case "Bw":
    case "Bs":
      // The hotter the days, the colder the nights.
      diurnal = 15 + (_pattern[climate][0] / 2);
      break;
    case "Cfa":
    case "Cs":
    case "Cfb":
    case "Dw":
    case "Ds":
    case "Df":
      diurnal = 15;
      break;
    case "Et":
    case "Ef":
    case "H":
      diurnal = 20;
      break;
    }

    // subtract from the temperature dependant on time of day and variance.
    temp -= (diurnal * tod) / 10;
  }

  // lower temperature dependant on cloud cover
  j = sqrt(_current[climate][CLOUD]);
  if(j < 0)
    j = 0;

  // when its cold clouds tend to raise the temperature at night
  // when its hot clouds tend to decrease the temperature during the day
  if(temp < 10 && tod == 10)
    temp += j;
  else if(temp > 30 && !tod)
    temp -= j;

  // lower temperature dependant on wind speed
  temp -= sqrt(_current[climate][WINDSP]);

  return temp;
}

/** @ignore yes */
int cloud_index(object env) {
  int cloud;

  cloud = calc_actual(env, CLOUD);
  if(cloud < 0)
    cloud = 0;
  return cloud;
}

/** @ignore yes */
int rain_index(object env) {
  int rain;

  rain = calc_actual(env, CLOUD) - (calc_actual(env, TEMP) / 2) - 100;
  return (rain > 0)? rain : 0;
}

/**
 * Return a temperature string equating to the temperature passed
 * @param int The temperature in Celcius
 * @return string a temperature string.
 */
string temp_string(int temp) {
  // lets start with the temperature since a lot of stuff falls out from that.
  switch(temp) {
    case 51..1000: return "incredibly, unbelievably hot"; break;
    case 46..50: return "amazingly hot"; break;
    case 41..45: return "damn hot"; break;
    case 36..40: return "very hot"; break;
    case 31..35: return "hot"; break;
    case 26..30: return "reasonably hot"; break;
    case 23..25: return "very warm"; break;
    case 20..22: return "warm"; break;
    case 16..19: return "pleasantly warm"; break;
    case 13..15: return "cool"; break;
    case 10..12: return "slightly chilly"; break;
    case 7..9: return "chilly"; break;
    case 4..6: return "very chilly"; break;
    case 1..3: return "cold"; break;                 
    case -3..0: return "very cold"; break;
    case -10..-4: return "freezing cold"; break;
    case -20..-11: return "damn cold"; break;
    case -30..-21: return "dangerously cold"; break;
    case -50..-31: return "incredibly, unbelievably cold"; break;
  }
}

private string precipitation_string(int rain, int rt, int wind) {
  switch(rain) {
  case -1000..20:
    return "lightly";
    break;
  case 21..40:
    if(wind < 20) {
      return "steadily";
    } else {
      return "hard";
    }
    break;
  case 41..60:
    return "heavily";
    break;
  case 61..500:
    if(rt == WEATHER_RAIN_TYPE) {
      return "torrentially";
    } else {
      return "very heavily";
    }
    break;
  }
}

/**
 * Creates a string showing how heavily it is raining.
 * @return the rain string
 * @param env the room room to check in
 */
string rain_string(object env) {
  int rt, rain, wind, temp;

  temp = temperature_index(env);
  rain = rain_index(env);
  rt = query_rain_type(temp, rain);
  if (rt != WEATHER_RAIN_TYPE) {
    return "There is no rain";
  }
  
  return "It is raining " + precipitation_string(rain, rt, wind);
}

/**
 * Creates a string showing how heavily it is snowing.
 * @return the snow string
 * @param env the room room to check in
 */
string snow_string(object env) {
  int rt, rain, wind, temp;

  temp = temperature_index(env);
  rain = rain_index(env);
  rt = query_rain_type(temp, rain);
  if (rt != WEATHER_SNOW_TYPE) {
    return "There is no snow";
  }
  return "It is snowing " + precipitation_string(rain, rt, wind);
}

/**
 * Creates a string showing how heavily it is sleeting.
 * @return the sleet string
 * @param env the room room to check in
 */
string sleet_string(object env) {
  int rt, rain, wind, temp;

  temp = temperature_index(env);
  rain = rain_index(env);
  rt = query_rain_type(temp, rain);
  if (rt != WEATHER_SLEET_TYPE) {
    return "There is no sleet";
  }
  return "It is sleeting " + precipitation_string(rain, rt, wind);
}

/**
 * A string giving a nice description of the cloud types.
 * @param env the environment to find the clouds for
 * @return the current clouds
 */
string cloud_string(object env) {
  int cloud;
  string str;

  cloud = cloud_index(env);
  switch(cloud) {
    case -1000..5:
       str = "a beautifully clear sky";
       break;
    case 6..10:
       str = "a few high level cirrus clouds";
       break;
    case 11..25:
       str = "scattered puffy clouds";
       break;
    case 26..40:
       str = "some puffy clouds";
       break;
    case 41..60:
       str = "many puffy clouds";
       break;
    case 61..80:
       str = "medium cloud cover";
       break;
    case 81..110:
       str = "dense cloud cover";
       break;
    case 111..130:
       str = "packed cloud cover";
       break;
    case 131..160:
       str = "thick black clouds";
       break;
    case 161..1000:
       str = "thick heavy storm clouds";
       break;
  }
  return str;
}

/**
 * Creates the weather string which is put into room descs.
 */
varargs string weather_string(object env, string obscured) {
  int temp, cloud, wind, rain, rt;
  int *warray;
  string str, tstr;

   if ( !env ) 
     return 0;
  temp = temperature_index(env);
  cloud = cloud_index(env);
  wind = calc_actual(env, WINDSP);
  rain = rain_index(env);

  if(function_exists("room_weather", env) &&
     !catch(warray = (int *)env->room_weather(temp, cloud, wind, rain))) {
    temp = warray[0];
    cloud = warray[1];
    wind = warray[2];
    rain = warray[3];
  }
  
  rt = query_rain_type(temp, rain);

  str = "It is ";
  tstr = temp_string(temp);
  switch(tstr[0]) {
  case 'a':
  case 'e':
  case 'i':
  case 'o':
  case 'u':
    str += "an ";
    break;
  default:
    str += "a ";
  }
  str += tstr + " ";
    
  switch((time() % (AM_SECONDS_PER_HALF_YEAR*2)) / AM_SECONDS_PER_DAY) {
  case 51..150:
    str += "spring prime";
    break;
  case 151..250:
    str += "summer prime";
    break;
  case 251..350:
    str += "autumn prime";
    break;
  case 351..450:
    str += "spindlewinter";
    break;
  case 451..550:
    str += "secundus spring";
    break;
  case 551..650:
    str += "secundus summer";
    break;
  case 651..750:
    str += "secundus autumn";
    break;
  default:
    str += "backspindlewinter";
  }

  str += "'s "+ query_tod() +" with ";

  switch(wind) {
    case -1000..5: str += "almost no wind"; break;
    case 6..10: str += "a gentle breeze"; break;
    case 11..15: str += "a steady breeze"; break;
    case 16..20: str += "a strong breeze"; break;
    case 21..30: str += "a steady wind"; break;
    case 31..40: str += "gusty winds"; break;
    case 41..50: str += "strong winds"; break;
    case 51..60: str += "gale-force winds"; break;
    case 61..1000: str += "storm-force winds"; break;
    default: str += "no wind at all"; break;
  }

  if (!obscured) {
    if(rain)
      str += ", ";
    else
      str += " and ";

    switch(cloud) {
      case -1000..5: str += "a beautifully clear sky"; break;
      case 6..10: str += "a few high level cirrus clouds"; break;
      case 11..25: str += "scattered puffy clouds"; break;
      case 26..40: str += "some puffy clouds"; break;
      case 41..60: str += "many puffy clouds"; break;
      case 61..80: str += "medium cloud cover"; break;
      case 81..110: str += "dense cloud cover"; break;
      case 111..130: str += "packed cloud cover"; break;
      case 131..160: str += "thick black clouds"; break;
      case 161..1000: str += "thick heavy storm clouds"; break;
    }
  }

  if(rain) {
    str += " and ";
    switch(rain) {
      case -1000..20: str += "light"; break;
      case 21..40:
        if(wind < 20)
          str += "steady";
        else
          str += "driving";
      break;
      case 41..60: str += "heavy"; break;
      case 61..500:
        if(rt == WEATHER_RAIN_TYPE)
          str += "torrential";
        else
          str += "very heavy";
      break;
    }

    str += " ";

    str += ({"", "snow", "sleet", "rain"})[rt];
  }

  if (obscured) {
    str += ".  " + obscured;
  }
  if((rain > 20) && (wind > 30)) {
    if (obscured)
      str += ".\nPeals of thunder echo overhead";
    else
      str += ".\nPeals of thunder and streaks of lightning rend the sky";
  }

  return str;
}

/** @ignore yes */
int next_update() {
  return _lastupdate + UPDATE_SPEED;
}

/** @ignore yes */
void dest_me() {
  unguarded((: save_object, FILE_NAME :));
}

/** @ignore yes */
int query_rain_type(int temp, int rain) {
  int type;

  if (rain > 0)
    switch(temp) {
    case -1000..-2:
      type = WEATHER_SNOW_TYPE;
      break;
    case -1..3:
      type = WEATHER_SLEET_TYPE;
      break;
    case 4..1000:
      type = WEATHER_RAIN_TYPE;
      break;
    }
  else
    type = WEATHER_NO_RAIN_TYPE;
  return type;
}

/**
 * Find out if its snowing in a given room.
 *
 * @param env the room
 * @return 1 or 0 for true or false.
 */
int query_snowing(object env) {
  int *warray;
  
  if(!env)
    return 0;

  if(function_exists("room_weather", env) &&
     !catch(warray = (int *)env->room_weather(temperature_index(env),
                                              cloud_index(env),
                                              calc_actual(env, WINDSP),
                                              rain_index(env))))
    return (query_rain_type(warray[0], warray[3]) == WEATHER_SNOW_TYPE)? 1 : 0;

  return (query_rain_type(temperature_index(env),
                          rain_index(env)) == WEATHER_SNOW_TYPE) ? 1 : 0;
}

/**
 * Find out if its raining in a given room.
 *
 * @param env the room
 * @return 1 or 0 for true or false.
 */
int query_raining(object env) {
  int *warray;
  
  if(!env)
    return 0; 

  if(function_exists("room_weather", env) &&
     !catch(warray = (int *)env->room_weather(temperature_index(env),
                                              cloud_index(env),
                                              calc_actual(env, WINDSP),
                                              rain_index(env))))
  {
    return (query_rain_type(warray[0], warray[3]) > WEATHER_SNOW_TYPE) ? 1 : 0;
  }

  return (query_rain_type(temperature_index(env),
                          rain_index(env)) > WEATHER_SNOW_TYPE) ? 1 : 0;
}

/**
 * Find out the temperature of a given room
 *
 * @param env the room
 * @return a temperature in Celcius
 */
int query_temperature(object env) {
  int *warray;

  if(!env)
    return 0;
  
  if(function_exists("room_weather", env) &&
     !catch(warray = (int *)env->room_weather(temperature_index(env),
                                              cloud_index(env),
                                              calc_actual(env, WINDSP),
                                              rain_index(env))))
    return warray[0];

  return temperature_index(env);
}

/**
 * Find out the cloud cover of a given room
 *
 * @param env the room
 * @return an open ended percentage. Greater than 100 means heavier cover.
 */
int query_cloud(object env) {
  int *warray;

  if(!env)
    return 0;
  
  if(function_exists("room_weather", env) &&
     !catch(warray = (int *)env->room_weather(temperature_index(env),
                                              cloud_index(env),
                                              calc_actual(env, WINDSP),
                                              rain_index(env))))
    return warray[1];

  return cloud_index(env);
}

/**
 * Find out the wind speed in a given room
 *
 * @param env the room
 * @return the wind speed in miles per hour.
 */
int query_windsp(object env) {
  int *warray;
  
  if(!env)
    return 0;

  if(function_exists("room_weather", env) &&
     !catch(warray = (int *)env->room_weather(temperature_index(env),
                                              cloud_index(env),
                                              calc_actual(env, WINDSP),
                                              rain_index(env))))
    return warray[2];

  return calc_actual(env, WINDSP);
}

/** @ignore yes */
int calc_variance(string climate, int type, int seasonal) {
  int diff, ret;

  diff = seasonal - _pattern[climate][type];
  if(diff < 0)
    ret = - random(diff * 2);
  else
    ret = random(diff * 2);

  if(random(2))
    ret += random(_variance[type]);
  else
    ret -= random(_variance[type]);
  
  return ret;
}

// basic system is for a 6am sunrise and 6pm _sunset.
// Days get longer until midsummer and shorter after it.
// I guess it's not really sunrise or sunset it's when it gets dark/light.
/** @ignore yes */
void set_day() {
  int min;
  
  // time of year. Climbs to 200 at mid-summer and then falls to year end
  _toy = (day_number() < (DIY / 2)) ? day_number() :
  (DIY / 2) - (day_number() % (DIY / 2));

  min = minute_number(); // the minute of the day

  _sunrise = 6 * AM_MINUTES_PER_HOUR;  // 6am the base sunrise time
  _sunset = 20 * AM_MINUTES_PER_HOUR;  // 8pm the base sunset time

  _sunrise += (100 - _toy);
  _sunset -= (100 - _toy);

  if((min <= _sunrise) || (min >= _sunset))
    _day = 0;
  else {
    // as it nears sunrise or sunset the light should fail so have the day
    // value drop in the last 100 minutes.
    if((min > _sunrise) && (min < _sunrise + 30))
      _day = (min-_sunrise) / 3;
    else if((min < _sunset) && (min > _sunset - 30))
      _day = (_sunset-min) / 3;
    else
      _day = 10;
  }
}

// change the current weather of type TYPE so that it becomes more like
// the pattern.
/** @ignore yes */
void migrate(string climate, int type) {
  int diff;

  diff = _pattern[climate][type] - _current[climate][type];
  if(diff > (_variance[type] / 2))
    diff = _variance[type] / 2;

  if(diff < 0)
    _current[climate][type] -= random(random(-diff));
  else
    _current[climate][type] += random(random(diff));
}

int check_umbrella(object ob) {
  if ( ob )
    return (int)ob->query_property( "umbrella" );
  return 0;
}

// send weather events to users
/** @ignore yes */
void do_inform(object who, int old_temp, int old_cloud, int old_rain,
               int old_day) {
  string str, where;
  int new_temp, new_cloud, new_wind, new_rain;
  int old_rain_type, new_rain_type;
  int *warray;

  new_temp = temperature_index(environment(who));
  new_rain = rain_index(environment(who));
  new_wind = calc_actual(environment(who), WINDSP);
  new_cloud = cloud_index(environment(who));

  if(function_exists("room_weather", environment(who)) &&
     !catch(warray = (int *)environment(who)->room_weather(new_temp, new_cloud,
                                                   new_wind, new_rain))) {
    new_temp = warray[0];
    new_cloud = warray[1];
    new_wind = warray[2];
    new_rain = warray[3];
  }

  if(who->query_name() == "ceres")
    tell_object(who, sprintf("%s: ot %d, or: %d, oc: %d, "
                             "nt %d, nr: %d, nc: %d, nw: %d",
                             who->query_name(), 
                             old_temp, old_rain, old_cloud, new_temp,
                             new_rain, new_cloud, new_wind));
  
  old_rain_type = query_rain_type(old_temp, old_rain);
  new_rain_type = query_rain_type(new_temp, new_rain);

  str = "";
  
  // sunrise and set messages
  if(_day != old_day) { // this test not needed, but should improve efficiency
    where = sun_direction( 1 ); /* sunset */
    if((old_day == 10)  && (_day < 10))
      str += "The sun starts to set slowly on the "+where+" horizon.\n";
    else if((old_day > 0) && (_day == 0))
      str += "The sun disappears from view below the "+where+" horizon.\n";
    else if((old_day > _day) && (_day < 9) && (_day > 0))
      str += "The sun sinks further below the "+where+" horizon.\n";

    where = sun_direction( 0 ); /* sunrise */
    if((_day > 0) && (old_day == 0))
      str += "The "+where+" sky starts to lighten as the sun peeks over the "+
        "horizon.\n";
    else if((_day == 10) && (old_day < 10))
      str += "The sun rises above the "+where+" horizon and greets you "+
        "for a new day.\n";
    else if((_day > old_day) && (_day < 9) && (_day > 0))
      str += "The sun rises further above the "+where+" horizon lightening "+
        "the sky as morning arrives.\n";
  }
  
  // clouds hiding the sun messages
  // in the 25-60 region the sun comes and goes quite a lot 
  if(_day && (old_cloud != new_cloud)) {
    if((old_cloud < 60) && (new_cloud > 20) && (!(new_cloud % 2)))
      str += "The " + query_tod()+ " sun disappears behind a cloud.\n";
    else if((old_cloud > 20) && (new_cloud < 60) && (new_cloud % 2))
      str += "The " + query_tod() + " sun peeks out from behind a cloud.\n";
  }

  // rain change messages
  if(old_rain_type != new_rain_type) {
    str += ({"", "It has stopped snowing", "It has stopped sleeting",
             "It has stopped raining"})[old_rain_type];
    if(old_rain_type && new_rain_type)
      str += " and started ";
    else if(new_rain_type)
      str += "It has started ";
    str += ({"", "snowing", "sleeting", "raining"})[new_rain_type];
    str += ".\n";
  } else if(new_rain_type > 0)
    str += "The "+({"", "snow", "sleet", "rain"})[new_rain_type]+
      " continues to fall.\n";

  // get them wet
  if(new_rain_type)
    if((!sizeof(filter_array(who->query_holding(), "check_umbrella",
                            this_object())) &&
       !sizeof(filter_array(who->query_wearing(), "check_umbrella",
                          this_object()))) || !random(50))
    {
      who->add_effect("/std/effects/other/wetness",
                      ((new_rain * new_rain_type) / 2) * (UPDATE_SPEED/60));
    }

  // tell them the output
  if(str != "")
    tell_object(who, who->colour_event("weather", "%^ORANGE%^")+str+
                "%^RESET%^");
}

// 0 for sunrise, 1 for sunset. Seasons in comments are for sunrise.
/** @ignore yes */
string sun_direction(int which) {
  switch(((time()+(AM_SECONDS_PER_HALF_YEAR * which)) %
          (AM_SECONDS_PER_HALF_YEAR*2)) / AM_SECONDS_PER_DAY) {
  case 0..174: // backspindle winter, spring prime, summer one
    return "widdershins";
  case 175..225: // summer one - small gods day
    return "hubwards";
  case 226..574: // summer one & two, autumn prime, spindlewinter,
                 //secundus spring
    return "turnwise";
  case 575..625: // summer two - alls falow
    return "rimward";
  default: //summer two, secundus autumn, backspindlewinter
    return "widdershins";
  }
}

// Send notifications to rooms
/** @ignore yes */
void do_room_inform(object what, int old_temp, int old_cloud, int old_rain,
        int old_day, int notifications) {
  int new_cloud, new_rain, new_wind, new_temp;
  int *warray;
  int has_changed = 0;

  new_temp = temperature_index(what);
  new_cloud = cloud_index(what);
  new_wind = calc_actual(what, WINDSP);
  new_rain = rain_index(what);

  if(function_exists("room_weather", what) && 
     !catch(warray = (int *)what->room_weather(new_temp, new_cloud,
                                       new_wind, new_rain))) {
    new_temp = warray[0];
    new_cloud = warray[1];
    new_wind = warray[2];
    new_rain = warray[3];
  }

  // Look at what's changed, and if the room wishes to be notified of those
  // changes.
  if(_day != old_day && (notifications & NOTIFY_DAY)) 
    has_changed += NOTIFY_DAY;
  if(old_temp != new_temp && (notifications & NOTIFY_TEMPERATURE))
    has_changed += NOTIFY_TEMPERATURE;
  if(old_cloud != new_cloud && (notifications & NOTIFY_CLOUD))
    has_changed += NOTIFY_CLOUD;
  if(old_rain != new_rain && (notifications & NOTIFY_RAIN))
    has_changed += NOTIFY_RAIN;

  // Now has_changed carries everything that has changed.  IE: If the clouds
  // and rain have changed,
  // has_changed = NOTIFY_CLOUD + NOTIFY_RAIN = NOTIFY_CLOUD | NOTIFY_RAIN
  // I like bitwise ORs and ANDs :) - Grampa
  if(has_changed) {
    // Using a call out to try to avoid bogging down should there be
    // a large amount of rooms who change a lot on weather notifications.
    call_out( "notify_room", 1, what, has_changed,
      _day, new_temp, new_cloud, new_rain );
  }
}

/** @ignore yes */
void notify_room( object what, int has_changed,
  int day, int temp, int cloud, int rain)
{
  // The room will have to query the changes, but it will know WHAT has
  // changed by ORing has_changed.  IE: If
  // (has_changed | NOTIFY_TEMPERATURE) == 1 then temperature has changed.
  // If it == 0, temperature has not changed.
  if(what)
    what->event_weather( has_changed, day, temp, cloud, rain);
}

/**
 * Find out when sunrise will be on a given day of the year.
 *
 * @param doy Day of the Discworld common year.
 * @return sunrise time (in seconds past midnite).
 * @see minute_number()
 * @see day_number()
 */
int query_sunrise(int doy) {
  int toy;

  toy = (doy < (DIY / 2)) ? doy : (DIY / 2) - (doy % (DIY / 2));
  return (6 * AM_MINUTES_PER_HOUR) + (100 - toy);
}

/**
 * Find out when sunset will be on a given day of the year.
 *
 * @param doy Day of the Discworld common year.
 * @return sunset time (in seconds past midnite).
 */
int query_sunset(int doy) {
  int toy;
  
  toy = (doy < (DIY / 2)) ? doy : (DIY / 2) - (doy % (DIY / 2));
  return (20 * AM_MINUTES_PER_HOUR) - (100 - toy);
}

/* Operational Functions (the ones that control everything) */

// set our target underlying temperature, cloudiness etc. for this cycle
// this target is for Ankh-Morpork. Other areas of the Disc set their variance
// from this "norm" by adding a property to the room.
// this function is called once every few hours so it's ok to be longish
/** @ignore yes */
void update_pattern() {
  int toy, temp, wind, cloud, tvar, wvar, cvar;
  string climate;
  
  call_out("update_pattern", CYCLE_SPEED);

  // time of year. Climbs to 200 at mid-summer and then falls to year end
  toy = (day_number() < (DIY / 2)) ? day_number() :
    (DIY / 2) - (day_number() % (DIY / 2));

  foreach(climate in CLIMATES) {
    switch(climate) {
    case "Af": // tropical rainforest
      temp = 30 + (toy / 40); // 30 - 35
      cloud = 50 + random(100);
      wind = random(10);
      break;
    case "Am": // tropical monsoon
      temp = 30 + (toy / 20); // 25 - 35
      cloud = -25 + sqrt(toy) * 10; // -25 - 115, rainy in the summer!
      wind = random(10);
      break;
    case "Aw": // tropical savannah
      temp = 20 + (toy / 10); // 20 - 40
      cloud = toy - 50; // -50 - 150, rainy in the summer!
      wind = random(10);
      break;

    case "Bw": // arid desert
      temp = 30 + (toy / 20); // 30 - 50
      cloud = (100 - toy/2) - 50; // -50 - 50, virtually never rains
      wind = random(10);
      break;
    case "Bs": // semi-arid desert
      temp = 25 + (toy / 20); // 25 - 45
      cloud = ((200 - toy) / 2) - 12; // -13 - 82
      wind = random(10);
      break;
      
    case "Cfa": // humid sub-tropical
      temp = (toy / 8) + 15; // 10 - 35
      cloud = ((225 - toy) / 2) - 25; // 0 - 100
      wind = 10 - (toy / 8);
      break;
      
    case "Cs": // mediterranean
      temp = (toy / 12) + 10; // 10 - 27
      cloud = ((225 - toy) / 2) - 50; // -25 - 75
      wind = 10 - (toy / 8);
      break;
    case "Cfb": // marine west coast (west coast us, 
      temp = (toy / 6) - 5; // -5 & +28 (celcius)
      cloud = ((225 - toy) / 2) - 25; // -12 - 87
      wind = 10 - (toy / 8);
      break;

    case "Dw": // continental dry, cold winters
      temp = (toy / 5) - 10; // gives us a no. between -10 & +30 (celcius)
      cloud = toy / 2; // 100 - 0
      wind = 15 - (toy / 8);
      break;
    case "Ds": // continental wet, cold winters
      temp = (toy / 5) - 10; // gives us a no. between -10 & +30 (celcius)
      cloud = (200 - toy) / 2; // 0 - 100
      wind = 15 - (toy / 8);
      break;
    case "Df": // continental, wet all the time. :)
      temp = (toy / 5) - 10; // gives us a no. between -10 & +30 (celcius)
      cloud = ((200 - toy) / 4) + 50; // 50 - 100
      wind = 15 - (toy / 8);
      break;
      
    case "Et": // tundra (permanently frozen)
      temp = (toy / 20) - 10;      // -10 - +0
      cloud = ((225 - toy) / 2) - 25; // 0 - 100
      wind = 25 - (toy / 8);
      break;
    case "Ef": // polar ice
      temp = (toy / 10) - 30;       // -30 - -10
      cloud = ((225 - toy) / 2) - 25; // 0 - 100
      wind = 25 - (toy / 8);
      break;
      
    case "H":
      temp = (toy / 10) - 10;      // -10 - +10
      cloud = ((225 - toy) / 2); // 20 - 125
      wind = 35 - (toy / 8);
      break;
    }


    tvar = calc_variance(climate, TEMP, temp);
    cvar = calc_variance(climate, CLOUD, cloud);
    wvar = calc_variance(climate, WINDSP, wind);

    _pattern[climate] =  ({ temp + tvar, cloud + cvar, wind + wvar });
  }
  unguarded((: save_object, FILE_NAME :));
}

// go through and update the moon and weather stuff.
// this function is called every UPDATE_SPEED seconds.
/** @ignore yes */
void update_weather() {
  int *warray, notifications;
  mapping list, roomlist, newrooms;
  object user, room;
  string climate;
  
  _lastupdate = time();
  call_out("update_weather", UPDATE_SPEED);

  // before we change it work out the weather where the outside players are
  // and store this information for use later.
  list = ([ ]);
  roomlist = ([ ]);
  foreach(user in filter(users(),
                         (: $1 && environment($1) &&
                          environment($1)->query_property("location") ==
                          "outside" :))) {
    // store the previous weather for a user.
    if(function_exists("room_weather", environment(user)) &&
       !catch(warray = (int *)environment(user)->room_weather(
                                 (int)temperature_index(environment(user)),
                                 (int)cloud_index(environment(user)),
                                 (int)calc_actual(environment(user), WINDSP),
                                 (int)rain_index(environment(user))))) {
      list[user] = ({ warray[0], warray[1], warray[3], _day});
    } else {
      list[user] = ({ (int)temperature_index(environment(user)),
                        (int)cloud_index(environment(user)),
                        (int)rain_index(environment(user)),
                        _day});
    }
  }
  
  newrooms = ([ ]);
  foreach (room, notifications in _rooms)  {
    if (room)
      newrooms[room] = notifications;
  }
  _rooms = copy(newrooms);

  // For rooms
  foreach(room, notifications in _rooms) {
    if(function_exists("room_weather", room) &&
       !catch(warray = (int *)room->room_weather((int)temperature_index(room),
                                                 (int)cloud_index(room),
                                               (int)calc_actual(room, WINDSP),
                                                 (int)rain_index(room) ))) {
      roomlist[room] = ({ warray[0], warray[1], warray[3], _day});
    } else {
      roomlist[room] = ({ (int)temperature_index(room),
                            (int)cloud_index(room),
                            (int)rain_index(room),
                            _day });
    }
  }

  // the mooncycle is updated every other day.
  if((moonupdate + (AM_SECONDS_PER_DAY * 2)) < time()) {
    mooncycle++;         // the moons phase changes each 
    mooncycle %= 14;
    moonupdate = time();
  }
  
  /* all current values will migrate towards their pattern values with some
     randomness thrown in for good measure. */
  
  // calculate the current temperature
  foreach(climate in CLIMATES) {
    migrate(climate, TEMP);
    migrate(climate, CLOUD);
    migrate(climate, WINDSP);
  }
  
  // see if the day has changed
  set_day();
  
  // housekeeping
  unguarded((: save_object, FILE_NAME :));

  // send appropriate messages to players who are outside
  foreach(user, warray in list) {
    do_inform(user, warray[0], warray[1], warray[2], warray[3]);
  }
  
  // send notifications to rooms.
  foreach(room, notifications in _rooms) {
      do_room_inform(room, roomlist[room][0], roomlist[room][1],
                     roomlist[room][2], roomlist[room][3], notifications);
      map_delete(_rooms, room);
  }
}

/* Debugging functions */
/** @ignore yes */
void print_stats(object here) {
  string climate, *weather;
  
  if(here)
    climate = here->query_climate();
  weather = ({ temperature_index(here), cloud_index(here),
                 calc_actual(here, WINDSP) });
  printf("Climate: %s toy: %d tod: %s [%d]\n"
         "here: %O\ncurrent: %O\npattern: %O\nvariance: %O\n",
         climate, _toy, query_tod(), 10 - query_day(here), weather,
         _current[climate], _pattern[climate], _variance);
  printf("Sunrise at: %d:%02d, sunset at: %d:%02d\n",
         _sunrise/60, _sunrise%60, _sunset/60, _sunset%60);
}

/** @ignore yes */
int *query_mooncycle() {
  return ({ mooncycle, 0});
}

/** @ignore yes */
void set_variance(int temp, int cloud, int wind) {
  _variance = ({temp, cloud, wind});
  unguarded((: save_object, FILE_NAME :));
}

/** @ignore yes */
// Reset the weather in case it's messed up.
void reset_weather() {
  int toy, temp, wind, cloud;
  string climate;

  // time of year. Climbs to 200 at mid-summer and then falls to year end
  toy = (day_number() < (DIY / 2)) ? day_number() :
    (DIY / 2) - (day_number() % (DIY / 2));

  foreach(climate in CLIMATES) {
    switch(climate) {
    case "Af": // tropical rainforest
      temp = 25 + (toy / 40);
      cloud = 50 + random(100);
      wind = random(10);
      break;
    case "Am": // tropical monsoon
      temp = 20 + (toy / 20); // 20 - 30
      cloud = -25 + sqrt(toy) * 10; // -25 - 115, rainy in the summer!
      wind = random(10);
      break;
    case "Aw": // tropical savannah
      temp = 20 + (toy / 20); // 20 - 30
      cloud = toy - 50; // -50 - 150, rainy in the summer!
      wind = random(10);
      break;

    case "Bw": // arid desert
      temp = 30 + (toy / 20); // 30 - 50
      cloud = (100 - toy/2) - 50; // -50 - 50, virtually never rains
      wind = random(10);
      break;
    case "Bs": // semi-arid desert
      temp = 25 + (toy / 20); // 25 - 45
      cloud = ((200 - toy) / 2) - 12; // -13 - 82
      wind = random(10);
      break;
      
    case "Cfa": // humid sub-tropical
      temp = (toy / 8) + 15; // 10 - 35
      cloud = ((225 - toy) / 2) - 25; // 0 - 100
      wind = 10 - (toy / 8);
      break;
      
    case "Cs": // mediterranean
      temp = (toy / 12) + 10; // 10 - 27
      cloud = ((225 - toy) / 2) - 50; // -25 - 75
      wind = 10 - (toy / 8);
      break;
    case "Cfb": // marine west coast (west coast us, 
      temp = (toy / 6) - 5; // -5 & +28 (celcius)
      cloud = ((225 - toy) / 2) - 25; // -12 - 87
      wind = 10 - (toy / 8);
      break;

    case "Dw": // continental dry, cold winters
      temp = (toy / 5) - 10; // gives us a no. between -10 & +30 (celcius)
      cloud = toy / 2; // 100 - 0
      wind = 15 - (toy / 8);
      break;
    case "Ds": // continental wet, cold winters
      temp = (toy / 5) - 10; // gives us a no. between -10 & +30 (celcius)
      cloud = (200 - toy) / 2; // 0 - 100
      wind = 15 - (toy / 8);
      break;
    case "Df": // continental, wet all the time. :)
      temp = (toy / 5) - 10; // gives us a no. between -10 & +30 (celcius)
      cloud = ((200 - toy) / 4) + 50; // 50 - 100
      wind = 15 - (toy / 8);
      break;
      
    case "Et": // tundra (permanently frozen)
      temp = (toy / 20) - 10;      // -10 - +0
      cloud = ((225 - toy) / 2) - 25; // 0 - 100
      wind = 25 - (toy / 8);
      break;
    case "Ef": // polar ice
      temp = (toy / 10) - 30;       // -30 - -10
      cloud = ((225 - toy) / 2) - 25; // 0 - 100
      wind = 25 - (toy / 8);
      break;
      
    case "H":
      temp = (toy / 10) - 10;      // -10 - +10
      cloud = ((225 - toy) / 2); // 20 - 125
      wind = 35 - (toy / 8);
      break;
    }
    
    _pattern[climate] =  ({ temp, cloud, wind });
    _current[climate] = ({ temp, cloud, wind });
  }
  unguarded((: save_object, FILE_NAME :));
}

/** @ignore yes */
mapping query_dynamic_auto_load() {
   return ([ "rooms" : _rooms ]);
}

/** @ignore yes */
void init_dynamic_arg(mapping map, object) {
   _rooms = map["rooms"];
   if (!_rooms) {
      _rooms = ([ ]);
   }
}
