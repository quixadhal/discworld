
/* 
 *   WEATHERDIAL WITH ANJOU'S WEATHER FUNCTIONS...
 */

inherit "/std/object";

#include "climate.h"
#define WEATHER "/obj/handlers/weather"


void setup()
{
  set_name("barometer");
  add_adjective("weather");
  add_adjective("wooden");
  add_alias("barom");
  add_alias("needle");
  set_short("barometer");
  set_long(
"An oblong box, made of the finest mahogany. It has an ornate metal needle "+
"which point at intricate pictures of the sun, clouds and rain. This "+
"highly magical device performs the eldritch and magickal task "+
"of telling you what the weather is like outside...@@show_weather:"+
file_name(this_object())+"@@");
  set_read_mess("Made in Anjolopolis, NANVAENT.\n");
  set_weight(100);
  set_value(2000);
}

void init()
{
  add_action("adjust_weather","off*set");
  add_action("adjust_weather","wei*ght");  
  add_action("tap","tap");
  add_action("tap","move");
  ::init();
}

string weather_string()
{
  string weather;
  object env;
  if ((env=environment(this_player())) && pointerp(env->query_co_ord()))
    weather = (string)WEATHER->weather_string(env);
  if (weather)
    return " According to the barmoter the weather outside is:\n"
      +weather+".\n";
  else return "changable"; /* :-) */
}  

string show_offsets()
{
  mixed w;
  int d;
    w = (mixed *)WEATHER->query_global_climate();
  d = (int)WEATHER->default_weight();
  /* hmmm -> make sure our array is in the correct format */
  if (!pointerp(w[C_TEMP]))  w[C_TEMP] = ({ w[C_TEMP],  d });
  if (!pointerp(w[C_CLOUD])) w[C_CLOUD] = ({ w[C_CLOUD], d });
  if (!pointerp(w[C_RAIN]))  w[C_RAIN] = ({ w[C_RAIN],  d });
   
  /* now we can get down to some serious game internals :=> */
  return sprintf("\nGLOBAL CLIMATE\t\tOFFSET\tWEIGHT\n"+
	 "Temp:\t\t\t%4d\t%4d\n"+
	 "Cloud:\t\t\t%4d\t%4d\n"+
	 "Rain:\t\t\t%4d\t%4d\n\n",
	 w[C_TEMP][C_OFFSET],  w[C_TEMP][C_WEIGHT],
	 w[C_CLOUD][C_OFFSET], w[C_CLOUD][C_WEIGHT],
	 w[C_RAIN][C_OFFSET],  w[C_RAIN][C_WEIGHT]);
}

	

string show_weather()
{
  mixed * w;
  int d;
  
  if (!this_player()->query_creator()) 
    return weather_string();
  
  w = (mixed *)WEATHER->query_global_climate();
  d = (int)WEATHER->default_weight();
  /* hmmm -> make sure our array is in the correct format */
  if (!pointerp(w[C_TEMP]))  w[C_TEMP] = ({ w[C_TEMP],  d });
  if (!pointerp(w[C_CLOUD])) w[C_CLOUD] = ({ w[C_CLOUD], d });
  if (!pointerp(w[C_RAIN]))  w[C_RAIN] = ({ w[C_RAIN],  d });
   
  /* now we can get down to some serious game internals :=> */
  return weather_string() + show_offsets();
  

	 

  
}

status tap(string s)
{
  if (!s || !id(s))
    return 0;
  write("You tap away at the barometer. The needle slowly shudders, then moves back to show that the weather outside is. "+show_weather());
  say(this_player()->query_cap_name()+" taps at the barometer.\n");
  return 1;
}


status adjust_weather(string s)
{
  mixed * weather;
  status var, value;
  string type;
  int amount;
  if (!s || sscanf(s,"%s %d", type, amount)<2) {
    write("Usage: off*set | wei*ght [t]emp [c]loud [r]ain <amount>\n");
    return 1;
  }
  type = lower_case(extract(type,0,0));
  if (type=="t") var = C_TEMP;
  else if (type=="c") var = C_CLOUD;
  else if (type=="r") var = C_RAIN;
  if (query_verb()=="weight")
    value=C_WEIGHT;
  else
    value=C_OFFSET;
  weather = (mixed *)WEATHER->query_global_climate();
  weather[var][value] += amount;
  
  /* hmmm oh well ill check for max values here as well i suppose */
/*
  if (weather[var][value]<-100) weather[var][value]=100;
  else if (weather[var][value]>100) weather[var][value]=100;
*/  

  WEATHER->set_global_climate(weather);
  return tap(query_name());
}
