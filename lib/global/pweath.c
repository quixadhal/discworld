/* your very own personal weather... */
#include "weather.h"
static int current_thing,
           light;
int wetness;

int query_wetness() { return wetness; }
void add_wetness(int arg) { wetness += arg; }

void create() {
  this_object()->add_alias("weather");
  this_object()->add_alias("moon");
  this_object()->add_alias("sun");
}
 
void weather_commands() {
  add_action("make","make");
  add_action("splash","splash");
}

string weather_extra_look() {
  if (wetness>100)
    return "You could swear that "+this_object()->query_pronoun()+
           " was a pond.\n";
  if (wetness>30)
    return "Is soaked. In fact really not looking that well.\n";
  if (wetness>10)
    return "Looks wet and bedraggled.\n";
  if (wetness>5)
    return "Looks wet.\n";
  if (wetness>0)
    return "Looks slightly wet.\n";
  return "";
}
 
string weather_long(string str) {
  mixed *co_ords;
  object env;
  string i;
 
  env = environment();
    if ((string)environment()->query_property("location") == "inside")
    return "You are not outside.\n";
 
  if (str== "sun")
    if (WEATHER->query_day(env))
      return "Ouch that hurts.\n";
    else
      return "The sun is not up, sorry.\n";
 
  if (str=="moon")
    if (WEATHER->query_day(env))
      return "The moon is not up, try at night.\n";
    else if (WEATHER->query_moon_string(env))
      return WEATHER->query_moon_string(env)+".\n";
    else
      return "The moon is not up at the moment, try again later.\n";
 
  if (str != "weather")
    return "";
  co_ords=(mixed *)environment()->query_co_ord();
  if (pointerp(co_ords))
    return "The weather is currently " + WEATHER->weather_string(env) + ".\n";
  else
    return "sorry but today the weather is on strike, hassle your nearest\n"+
          "neighbourhood wizard.\n";
}

void event_weather() {
  call_out("check_it",0);
}
 
void check_it() {
  mixed *co_ords, *arr;
  object env;
  int bingle, no_co_ord, *type;
  string loc;
 
  if (!environment())
    return ;
  env = environment();
  loc = (string)environment()->query_property("location");
  co_ords = (mixed *)environment()->query_co_ord();
  if (!pointerp(co_ords)) {
    co_ords = ({ 0, 0, 0, "standard" });
    no_co_ord = 1; /* so we dont get wet etc, when we dont know where we are */
  }
  if (!WEATHER->query_day(env)) {
    if (light && loc != "inside")
      tell_object(this_object(), "The sun sets slowly on the horizon.\n");
    light = 0;
  } else {
    if (!light && loc != "inside")
      tell_object(this_object(), "The sun rises above the horizon and greets you for a new day.\n");
    light = 1;
  }
  if (!no_co_ord) {
    type = (int *)WEATHER->query_type_rain(env);
    bingle = type[1];
    if (current_thing != type[0] && loc != "inside")
      if (current_thing && type[0])
        tell_object(this_object(), "It has stopped "+({ "raining", "hailing",
              "snowing" })[current_thing-1] + " and started "+
              ({ "raining", "hailing", "snowing"})[type[0]-1]+".\n");
      else if (current_thing)
        tell_object(this_object(), "It has stopped "+({ "raining", "hailing",
              "snowing" })[current_thing-1]+".\n");
      else if (type[0])
        tell_object(this_object(), "It has started "+({ "raining", "hailing",
               "snowing" })[type[0]-1]+".\n");
    current_thing = type[0];
    if (bingle > 0)
      switch (type[0]) {
        case 1 :
        case 2 :
          if (bingle>0 && loc == "outside") {
/* we get wet ;) */
            arr = all_inventory(this_object());
            arr = filter_array(arr, "check_umbrella", this_object());
            if (!sizeof(arr))
              wetness += bingle; /* strength... my oh my what rain ! */
          }
          break;
        case 3 :
          if (bingle && loc == "outside") {
            arr = all_inventory(this_object());
            arr = filter_array(arr, "check_umbrella", this_object());
            if (!sizeof(arr))
             wetness += bingle /2;
          }
          break;
      }
    else if (wetness>0) {
/* the warmth property is used for fires etc... */
      bingle = WEATHER->temperature_index(env)+
               environment()->query_property("warmth");
      if (bingle<0)
/* we dry very quickly... (oh good) */
        wetness += bingle;
      else
        wetness -= 1; /* we do get dry slowly even if it is cold */
    }
  }
  if (wetness>0 && random(100) < 10)
    tell_object(this_object(),"Squelch.\n");
}
 
int check_umbrella(object ob) {
  return (int)ob->query_property("umbrella");
}
 
int make(string str) {
  object ob, env;
  int *co_ords;
 
  env = environment();
  if ((string)environment()->query_property("location") == "inside") {
    notify_fail("You must be outside to do that.\n");
    return 0;
  }
  co_ords = (mixed *)env->query_co_ord();
  env = environment();
  if (!pointerp(co_ords)) {
    notify_fail("This room has no co-ordinates... Arggghhh.\n");
    return 0;
  }
  if (str == "snowball") {
/* well lets do it then ;) */
    if (!WEATHER->query_snowing(env)) {
      notify_fail("You need snow to make a snowball.\n");
      return 0;
    }
    ob = clone_object("/std/environ/snowball");
    ob->move(this_object());
    write("You make a lovely big snowball. Have fun with it ;)\n");
    return 1;
  }
  if (str == "snowman") {
    if (!WEATHER->query_snowing(env)) {
      notify_fail("You need snow to make a snowman.\n");
      return 0;
    }
    ob = clone_object("/std/environ/snowman");
    ob->move(environment());
    write("You make a snowman to make anyone else weep.\n");
    return 1;
  }
  notify_fail("You can't do that!\n");
  return 0;
}
 
int splash(string str) {
  object *obs, weath,env;
  int *co_ords, i;
 
  if ((string)environment()->query_property("location") == "inside") {
    notify_fail("You must be outside to do that.\n");
    return 0;
  }
  if (!str) {
    notify_fail("You must splash someone.\n");
    return 0;
  }
  env = environment();
  if (!WEATHER->query_raining(env)) {
    notify_fail("It must be raining to splash someone.\n");
    return 0;
  }
  obs = find_match(str,environment());
  if (!sizeof(obs)) {
    notify_fail(str+" does not exist, are you dreaming?\n");
    return 0;
  }
  for (i=0;i<sizeof(obs);i++) {
    obs[i]->add_wetness(2+random(4));
    tell_object(obs[i],(string)this_player()->query_cap_name()+
        " splashes "+query_multiple_short(obs - ({ obs[i] }) + ({ "you" }))+
        ".\n");
  }
  str = query_multiple_short(obs);
  write("You splash "+str+".\n");
  say(this_object()->query_cap_name()+" splashes "+str+".\n", obs);
  return 1;
}
