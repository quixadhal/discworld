inherit "std/object";
 
#include "climate.h"
#define FILE_NAME "/save/environ/weather"
#define HOME "/obj/handlers/weather_room"
#define MAX_DIST 100
#define YEAR 168
#define DAY 60
#define SPEED 60
 
/*
 * hmmmmm. ok well cloud cover...
 *                   wind speed...
 * hmmm. what would be a nice way of doing that? and climate...
 *
 * well we could do something like having the cloud cover sort of follow the low
 * or we could change the low scheme. I dont belive in global patterns, ie the
 * whole mud having the same weather. but nearby rooms should have very similar
 * weather.
 *
 * Complaints complaints.  Ok, I might comment some of it.
 */
 
mixed *coldarr,
      *cloudarr,
      *rainarr;

static mapping my_obs;
 
int div,
    year,
    timeofyear,
    mooncycle,
    moonoff,
    timeofday;

mixed * global_climate;   /* some more stuff - anjou@nanvaent */
    
 
int distance(mixed *co_ord1,mixed *co_ord);
int cloud_index(object env);
int rain_index(object env);
int temperature_index(object env);
string cloud_string(object env);
string rain_string(object env);
string temperature_string(object env);
 
void setup() {
  set_name("weather");
  set_short("weather controller extrodinare");
  set_long("The weather controller, at least this one is always right!\n");
/*
 * The default arrays, we use offsets from these to determine the weather
 * The reason for haveing 3 of them is to average the effect, so in effect
 * extremes are possible but more unlikely.  By adding more or taking
 * some of these arrays away you can generate greater or lesser swings
 * of weather in your mud.
 */
  cloudarr = ({ ({ 0,0 }), ({ 50,50 }), ({ -50,-50 }) });
  rainarr = ({ ({ 0,0 }), ({ 50,50 }), ({ -50,-50 }) });
  coldarr = ({ ({ 0,0 }), ({ 50,50 }), ({ -50,-50 }) });
  my_obs = ([ ]);
 
  restore_object(FILE_NAME);

  /* defaults for GLOBAL CLIMATE -> anjou@nanvaent 
   * for C_TEMP, C_CLOUD and C_RAIN. First value is C_OFFSET, then C_WEIGHT
   * [e.g.  global_climate = ({ ({0,10}), ({0,10}), ({0,10}), })]
   *
   * default_weight() is default weight.
   */
  if (!global_climate) global_climate = ({ 0,0,0 });

  /* ***************************************** */
  

  call_out("update_low",SPEED);
  move("bing");
}
 
/*
 * set-up function returns default fluctuation of weather
 * 1 is for wild fluctuation, 10 steady (Anjou)
 */
int default_weight()
{
/* let's put the default here for safe keeping :-> ANJOU */
#define DEFAULT_WEIGHT 10 
  return DEFAULT_WEIGHT;
}


/* have this move here to get it to my weather room ;) (Sleaze) */
move(arg) {
  ::move(HOME);
}

/*
 * The object asks to be told about weather events
 */
void notify_me(object ob) {
  my_obs[ob]++;
}

/*
 * The object tells us it is really bored by these damn stupid weather events/
 */
void unnotify_me(object ob) {
  if (--my_obs[ob] <= 0)
    my_obs = m_delete(my_obs, ob);
}

/* Debug, sort of usefulish */
mixed *query_cold() { return coldarr; }
mixed *query_rain() { return rainarr; }
mixed *query_cloud() { return cloudarr; }

/* Moon cycle and the string we would give if the moon was up */
int query_moon() { return mooncycle; }
string query_moon_string(object env) {
  int bing;
 
  bing = timeofday - (DAY/4) -
         (distance((mixed *)env->query_co_ord(), ({ 0, 0, 0, }))/100);
  if (bing<0) bing += DAY;
  if ((moonoff+(DAY/2)<bing) && (bing<DAY-moonoff))
    return 0; /* the moon is not up... */
  return ({ "full moon",
            "waning three quarter moon",
            "waning half moon",
            "waning quarter moon",
            "waning cresent moon", /* from here */
            "new moon",
            "waxing cresent moon", /* to here you cannot see at night... ! */
            "waxing quarter moon",
            "waxing half moon",
            "waxing three quarter moon" })[mooncycle];
}

/*
 * This is the routine used to caculate the distance between 2 3-d points.
 * Messy...  So, I did it the easy way :)  It isn't a perfect alorithm
 * for this but it suits our purposes. (and is a lot quicker than any
 * other algorithm)
 */
int distance(mixed *co_ord1, mixed *co_ord2) {
  int off;
  if (!pointerp(co_ord1) || !pointerp(co_ord2))
    return 0;
  if (sizeof(co_ord1) < 3)
    co_ord1 = ({ 0, 0, 0})[0..2-sizeof(co_ord1)];
  if (sizeof(co_ord2) < 3)
    co_ord2 += ({ 0, 0, 0})[0..2-sizeof(co_ord2)];
  if (co_ord1[0]>co_ord2[0])
    off = co_ord1[0] - co_ord2[0];
  else
    off = co_ord2[0] - co_ord1[0];
  if (co_ord1[1]>co_ord2[1])
    off += co_ord1[1] - co_ord2[1];
  else
    off += co_ord2[1] - co_ord1[1];
  return off;
}

/* Four seasons of course. */
int query_season() { return timeofyear/(YEAR/4); }

/* Relatively useless ionformation, but hey you might want it! */
int query_time_of_year() { return timeofyear; }
int query_time_of_day() { return timeofday; }

/*
 * Return 0 if it is night or non zero if it is day.   It returns how
 * "dayish" it is.
 */
int query_day(object env) {
  int bing, bit;
 
  if (!env) {
    env = find_object("/d/am/am/mendedrum");
    if (!env)
      call_other("/d/am/am/mendeddrum", "??");
    env = find_object("/d/am/am/mendeddrum");
  }
/* make it change over the mud... but not very quickly... */
  bing = timeofday - (DAY/4) -
         (distance((mixed *)env->query_co_ord(), ({ 0, 0, 0, }))/100);
  if (bing<0) bing += DAY;
/*
 * Create the break point for each day.  Depends on time of year, and length
 * of year.
 */
  bit = (DAY/2)-(timeofyear-(YEAR/2))/(YEAR/10);
  if (bing>bit)
    return 0;
  if (bit < 10)
    return bit;
  return -(bing - bit);
}
 
/*
 * returns a percentage of light.... 100 being full sunlight...
 * down
 */
int query_darkness(object env) {
  int bing, per, i;
 
/*
 * so thats the day.... we should make the light fade towards night
 * though.... Hmmm.
 */
  per = -cloud_index(env);
  if (per <-100)
    per = -100;
  if (per > 100)
    per = 100;
/* This is why query_day returns an obscure number */
  if ((i=query_day(env))) {
    i *= 20;
    if (i>100)
      i = 100;
    return 50+(per+100)*150*i/20000;
  }
  bing = timeofday - (DAY/4) -
         (distance((mixed *)env->query_co_ord(), ({ 0, 0, 0, }))/100);
  if (bing<0) bing += DAY;
  if ((moonoff+(DAY/2)<bing) && (bing<DAY-moonoff))
    return 15+(per+100)*30/200;
  if (timeofday<0) timeofday += DAY;
  i = mooncycle -5;
  if (i<0)
    i = -i;
  return 30+(per+100)*14*i/200; /* lit up by the light of the moon... tell them about it? */
}
 
/* The rain/cloud/temperature weather string is concatenated here */
string weather_string(object env) {
  return temperature_string(env)+" with "+cloud_string(env)+
         rain_string(env);
}

/*
 * Used by the player object to create nicer messages for weather changeing.
 * Returns the current type of rain.  ({ 0, 0 }) is no rain.
 * ({ 1, intensity }) is rain.
 * ({ 2, intensity }) is hail.
 * ({ 3, intensity }) is snow.
 */
mixed *query_type_rain(object env) {
  int ind, cloud, rain;

  cloud = cloud_index(env);
  if (cloud <= 0)
    return ({ 0, 0 });
  rain = rain_index(env);
  if (cloud - rain <= 0)
    return ({ 0, 0 });
  ind = temperature_index(env)/(100/7);
  if (ind < 2)
    return ({ 1, cloud-rain });
  if (ind == 2)
    return ({ 2, cloud-rain });
  return ({ 3, cloud-rain });
}

/* So is it raining where you are? */
int query_raining(object env) {
  int cloud,rain;
  if (temperature_index(env)/(100/7)<2) {
    cloud = cloud_index(env);
    if (cloud<=0)
      return 0;
    rain = rain_index(env);
    if (cloud-rain<=0)
      return 0;
    return cloud-rain;
  }
  return 0;
}
 
/* Maybe its hailing instead? */
int query_hailing(object env) {
  int cloud, rain;
 
  if (temperature_index(env)/(100/7)==2) {
    cloud = cloud_index(env);
    if (cloud <=0)
      return 0;
    rain = rain_index(env);
    if (cloud-rain <=0)
      return 0;
    return cloud-rain;
  }
  return 0;
}
 
/* Nahhh, must be snowing */
int query_snowing(object env) {
  int cloud,rain;
  if ((temperature_index(env)/(100/7))>=3) {
    cloud = cloud_index(env);
    if (cloud<=0)
      return 0;
    rain = rain_index(env);
    if (cloud-rain<=0)
      return 0;
    return cloud-rain;
  }
  return 0;
}
 
/*
 * A nice string tells us how heavy and what temperature the rain/hail/snow
 * is.
 */
string rain_string(object env) {
  int cloud, rain, temp;
  string tempstr1,tempstr2;
 
  cloud = cloud_index(env);
  rain = rain_index(env);
  if (cloud<=0) return "";
  if (cloud-rain<=0) return "";
  temp = temperature_index(env)/(100/7);
  if (temp>3) temp = 3;
  if (temp<-3) temp = -3;
  tempstr1 = ({ "very hot ",
                "hot ",
                "warm ",
                "nice ",
                "cold ",
                "",
                "" })[temp+3];
  tempstr2 = ({ " rain",
                " rain",
                " rain",
                " rain",
                " rain",
                " hail",
                " snow" })[temp+3];
  cloud = (cloud-rain) / 20;
  if (cloud>5) cloud = 5;
/* hmmm -> surely u mean FOUR pinkfish? - ok ill add "torrential" instead */
  return "\n"+capitalize(tempstr1 + ({ "very light",
                      "light",
                      "medium",
                      "heavy",
                      "very heavy",
		      "torrential", }) [cloud]+tempstr2);
}
 
/* String for the temperature.  Used for all the temperature thingys. */
string temperature_string(object env) {
  int inten;
  inten = temperature_index(env);
  inten /= 10;
  if (inten>10)
    inten = 10;
  if (inten<-10)
    inten = -10;
  return ({ "Its one of those baking eggs on the pavement days", /* -10 */
            "So hot that the sun feels like its right next door", /* -9 */
            "Damn hot", /* -8 */
            "Very hot", /* -7 */
            "Hot", /* -6 */
            "Hot", /* -5 */
            "Reasonably hot", /* -4 */
            "Very warm", /* -3 */
            "Warm", /* -2 */
            "Pleasantly warm", /* -1 */
            "Average temperature", /* 0 */
            "A little chilly", /* 1 */
            "A slight nip in the air", /* 2 */
            "Chilly", /* 3 */
            "Very chilly", /* 4 */
            "Cold", /* 5 */
            "Cold", /* 6 */
            "Very cold", /* 7 */
            "Damn cold", /* 8 */
            "Incredibly cold", /* 9 */
            "Freezing cold" })[inten+10]; /* 10 */
}

/* Cloud string....  Makes a nice cloud message for you. */
string cloud_string(object env) {
  int off;
 
  off = cloud_index(env) / 20;
  if (off>5) off = 5;
  if (off<-5) off = -5;
  return ({ "a beautifully clear sky", /* -5 */
            "a few high level sirius clouds", /* -4 */
            "scattered puffy clouds", /* -3 */
            "very thin complete cloud cover", /* -2 */
            "light cloud cover", /* -1 */
            "medium cloud cover", /* 0 */
            "dense cloud cover", /* 1 */
            "packed cloud cover", /* 2 */
            "packed cloud cover", /* 3 */
            "heavy black clouds", /* 4 */
            "thick heavy clouds", /* 5 */
         })[off+5];
}
 
/*
 * Used by the temperature string and other things, returns a nice number
 * which is how HOT it currently is.
 * Returns a number from approximately -100 to 100.  Can get higher or lower
 * But the chance of this is really unlikely....
 */
int temperature_index(object env) {
  int off, i, div;
  mixed clim, *co_ord;
 
  for (i=0;i<sizeof(coldarr);i++)
    off += (distance((mixed *)env->query_co_ord(), coldarr[i]) % (MAX_DIST*2));
  off = off / sizeof(coldarr);
  if (off>MAX_DIST) off = (MAX_DIST*2) - off;
  if ((clim = (mixed *)env->query_property("climate"))) {
    if (!pointerp(clim[C_TEMP])) {
      off += clim[C_TEMP];
      div = default_weight();
    } else {
      off += clim[C_TEMP][C_OFFSET];
      div = clim[C_TEMP][C_WEIGHT];
    }
  } else { /* ANJOU@NANVAENT */
    if (!pointerp(global_climate[C_TEMP])) {
      off += global_climate[C_TEMP];
      div = default_weight();
    } else {
      off += global_climate[C_TEMP][C_OFFSET];
      div = global_climate[C_TEMP][C_WEIGHT];
      if (!div) div = default_weight();
    }
  }
  off -= (MAX_DIST/2);
  off = 0 - off;
  co_ord = (mixed *)env->query_co_ord();
  if (!co_ord)
    co_ord = ({ 0, 0, 0 });
  if (sizeof(co_ord) < 3)
    printf("%O has an incorrect co_ordinate size.\n", env, co_ord);
  return (((off + (timeofyear - (YEAR/2))/((YEAR/60)+1) +
         (timeofday - (DAY/2))/((DAY/30)+1) + co_ord[2]))*10)/div;
}
 
/*
 *  The range is the same as for the rain index.  Returns how cloudy it is.
 */
int cloud_index(object env) {
  int off;
  mixed *clim;
  int i;
 
  for (i=0;i<sizeof(cloudarr);i++)
    off += (distance((mixed *)env->query_co_ord(), cloudarr[i]) % (MAX_DIST*2));
  off = off / sizeof(cloudarr);
  if (off>MAX_DIST) off = (MAX_DIST*2) - off;
  if ((clim = (mixed *)env->query_property("climate"))) {
    if (!pointerp(clim[C_CLOUD])) {
      off += clim[C_CLOUD];
      div = default_weight();
    } else {
      off += clim[C_CLOUD][C_OFFSET];
      div = clim[C_CLOUD][C_WEIGHT];
    }
  } else { /* ANJOU@NANVAENT */
    if (!pointerp(global_climate[C_CLOUD])) {
      off += global_climate[C_CLOUD];
      div = default_weight();
    } else {
      off += global_climate[C_CLOUD][C_OFFSET];
      div = global_climate[C_CLOUD][C_WEIGHT];
      if (!div) div = default_weight();
    }
  }
  off -= (MAX_DIST/2);
  off = 0-off;
  return ((off + (timeofyear - (YEAR/2))/((YEAR/60)+1))*10)/div;
}

/*
 * You guessed it....
 */
int rain_index(object env) {
  int off;
  mixed *clim;
  int i;
 
  for (i=0;i<sizeof(rainarr);i++)
    off += (distance((mixed *)env->query_co_ord(), rainarr[i]) % (MAX_DIST*2));
  off = off / sizeof(rainarr);
  if (off>MAX_DIST) off = (MAX_DIST*2) - off;
  if ((clim = (mixed *)env->query_property("climate"))) {
    if (!pointerp(clim[C_RAIN])) {
      off += clim[C_RAIN];
      div = default_weight();
    } else {
      off += clim[C_RAIN][C_OFFSET];
      div = clim[C_RAIN][C_WEIGHT];
    }
  } else { /* ANJOU@NANVAENT */
    if (!pointerp(global_climate[C_RAIN])) {
      off += global_climate[C_RAIN];
      div = default_weight();
    } else {
      off += global_climate[C_RAIN][C_OFFSET];
      div = global_climate[C_RAIN][C_WEIGHT];
      if (!div) div = default_weight();
    }
  }
  off -= MAX_DIST/2;
  off = 0-off;
  return (off*10)/div;
}
 
/* Updates the postion of all the arrays used to calculate weather. */
void update_low() {
  int i;
  object *obs;

/* Increase the time of day. */
  timeofday++;
  if (timeofday>DAY) {
/* Greater than time of day, inc time of year and moon cycle */
    timeofday = 0;
    timeofyear++;
    mooncycle++;
    if (mooncycle %2) {
      moonoff += 1;
      moonoff = moonoff % (DAY/2);
    }
    timeofyear = (i=timeofyear) % YEAR;
    if (i != timeofyear) /* New year */
      year++;
    mooncycle = mooncycle % 10;
    save_object(FILE_NAME);
  }
/* Update the rain arrays */
  for (i=0;i<sizeof(rainarr);i++) {
    rainarr[i][0] += random(3)-1;
    rainarr[i][0] = rainarr[i][0] % (MAX_DIST*2);
    rainarr[i][1] += random(3)-1;
    rainarr[i][1] = rainarr[i][1] % (MAX_DIST*2);
  }
/* Update the cloud arrays */
  for (i=0;i<sizeof(cloudarr);i++) {
    cloudarr[i][0] += random(3)-1;
    cloudarr[i][0] = cloudarr[i][0] % (MAX_DIST*2);
    cloudarr[i][1] += random(3)-1;
    cloudarr[i][1] = cloudarr[i][1] % (MAX_DIST*2);
  }
/* Update the temperature arrays */
  for (i=0;i<sizeof(coldarr);i++) {
    coldarr[i][0] += random(3)-1;
    coldarr[i][0] = coldarr[i][0] % (MAX_DIST*2);
    coldarr[i][1] += random(3)-1;
    coldarr[i][1] = coldarr[i][1] % (MAX_DIST*2);
  }
  obs = m_indices(my_obs);
  for (i=0;i<sizeof(obs);i++)
    if (objectp(obs[i]))
      event(obs[i], "weather");
  call_out("update_low",SPEED);
}

void dest_me() {
  save_object(FILE_NAME);
  ::dest_me();
}


/*
 * Some nice functions to deal with global weather offsets & weights.
 * 
 * ANJOU@NANVAENT - please hack these to hell pf!
 *
 */
void save_weather() {  save_object(FILE_NAME); }

mixed * query_global_climate()
{
  return global_climate;
}

mixed * set_global_climate(mixed * clime)
{
  /* need root privs to set global climate */
  if (geteuid(this_player())=="Root") {
    global_climate = clime;
    save_weather();
  }
  return global_climate;
}

status check_valid_type(int type)
{
  if (type < C_TEMP || type > C_RAIN) return 0; else return 1;
}

mixed * adjust_global_offset(int type, int amount)
{
  mixed * temp_climate;
  if (!check_valid_type(type)) return global_climate;
  temp_climate = global_climate;
  temp_climate[type][C_OFFSET] += amount;
  return set_global_climate(temp_climate);
}

mixed * adjust_global_weight(int type, int amount)
{
  mixed * temp_climate;
  if (!check_valid_type(type)) return global_climate;
  temp_climate = global_climate;
  temp_climate[type][C_WEIGHT] += amount;
  return set_global_climate(temp_climate);
}
