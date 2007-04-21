/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: pweath.c,v 1.7 2001/09/11 03:38:33 presto Exp $
 * $Log: pweath.c,v $
 * Revision 1.7  2001/09/11 03:38:33  presto
 * Fixed type error I think
 *
 * Revision 1.6  2001/03/13 00:22:08  pinkfish
 * Format it...
 *
 * Revision 1.5  2001/03/13 00:19:35  ceres
 *  Forcibly unlocked by pinkfish
 *
 * Revision 1.4  2000/04/10 05:07:41  ceres
 * Made it public :)
 *
 * Revision 1.3  2000/01/06 07:44:09  terano
 * Changed weather_extra_look to check for null values
 *
 * Revision 1.2  2000/01/06 07:42:16  ceres
 *  Forcibly unlocked by terano
 *
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 * 
*/
/* your very own personal weather... */
#include <weather.h>
#include <clothing.h>
#include <playtesters.h>

#define COMFORTABLE 20

int temperature;
string temperature_str;

int query_wetness()
{
   return 0;
}

void add_wetness(int arg)
{
}

void dry_out()
{
}

void set_personal_temp(int i)
{
   temperature = i;
}
void adjust_personal_temp(int i)
{
   temperature += i;
}

int query_personal_temp()
{
   return temperature;
}

string query_temp_str()
{
   if (!temperature_str || temperature_str == "") {
      return "quite comfortable";
   }
   return temperature_str;
}

/**
 * This method calculates an objects personal temperature index.
 *
 * All temperatures in here are 20C lower so that comfort is at zero to
 * make calculations easier.
 */
int calc_personal_temp()
{
   mixed tmp;
   object where,
    *clothes;
   string item,
     zone,
     type,
    *types,
    *zones;
   int adjustment,
     room_temp,
     correction,
    *enums;

   if (this_object()->query_property("dead")) {
      temperature = 0;
      return 0;
   }
   where = environment(this_object());

   // If they're on a horse or somesuch then use that locations temperature
   // info.
   if (!where->query_property("location") &&
       environment(where) && environment(where)->query_property("location"))
      where = environment(where);

   // Figure out the temperature where they are.
   switch (where->query_property("location")) {
   case "outside":
      room_temp = (WEATHER->query_temperature(where) - COMFORTABLE);
      break;
   default:
      room_temp = 0;
   }

   // add the warmth of the room.
   room_temp += where->query_property("warmth");
   room_temp -= where->query_property("cooling");

   adjustment = room_temp;

   // add their personal warmth (from effects or shadows)
   adjustment += this_object()->query_property("warmth");
   adjustment -= this_object()->query_property("cooling");

   // Calculate how warm (or cool) their clothing is keeping them.
   clothes = filter_array(this_object()->query_wearing(),
                          (: !$1->id("scabbard") &&
                             !$1->id("belt") &&
                             !$1->id("backpack") &&
                             !$1->id("glasses") && !$1->id("jewellery") :));
   zones = ({ });
   foreach(item in clothes) {
      tmp = item->query_type();
      if (arrayp(tmp))  types = tmp;
      else types = ({ tmp });

      foreach(type in types) {
         // Find out what zone this clothing type covers.
         // If it doesn't cover a zone (eg. jewellery) it doesn't give any
         // warmth.
         if (CLOTHING_HANDLER->query_equivilant_type(type))
            type = CLOTHING_HANDLER->query_equivilant_type(type);

         foreach(zone in CLOTHING_HANDLER->query_zone_names(type)) {
            if (member_array(zone, zones) == -1) {
               zones += ({ zone });
            }
            tmp = item->query_property("warmth");
            if (!tmp) {
               adjustment++;
            } else if (intp(tmp) && tmp) {
               adjustment += tmp;
            }
            // If it's warm here then do adjustments for clothing that
            // cool you.
            if (room_temp > 0 && item->query_property("cooling")) {
               adjustment -= item->query_property("cooling");
            }
         }
      }
   }

   // You get a warmth bonus or coolness bonus for each zone covered.
   // This means it's better to cover more of your body when it's cold.
   // It also means you can wear light clothing with less temperature
   // penalties since the warmth & cooling cancel out somewhat.
   if (room_temp < 0) {
      adjustment += sizeof(zones);
      if (room_temp + sizeof(zones) > 5) {
         adjustment -= (room_temp + sizeof(zones) - 5);
      }
   } else {
      adjustment -= sizeof(zones);
      if (room_temp - sizeof(zones) < -5) {
         adjustment -= (room_temp - sizeof(zones) + 5);
      }
   }

   // Wetness makes you cooler -- so sweating isn't necessarily a bad thing.
   enums = (int *) this_object()->effects_matching("body.wetness");
   if (sizeof(enums)) {
      adjustment -= sqrt(sqrt((int) this_object()->arg_of(enums[0]))) * 2;
   }
   // this hopefully does two things.
   // 1. effectively puts a maximum/minimum on temperature
   // 2. accounts for the body working to adjust temperature.
   if (temperature > room_temp && room_temp >= 0 || temperature > 5) {
      correction -= (temperature / 5) + 5;
   }
   if (temperature < room_temp && room_temp <= 0 || temperature < -5) {
      correction -= (temperature / 5) - 5;
   }
#ifdef 0
   /*
    * This factor is not currently used. Depending on how the testing goes
    * it may get put in place.
    */
   // this factor adjusts for when rooms aren't too hot or cold. So if you're
   // not too far off the room temperature you don't adjust quite so fast.
   if ((room_temp < 10 && temperature >= 0) ||
       (room_temp > 10 && temperature <= 0)) {
      factor = (ABS(room_temp - temperature) / 10.0);
      if (factor > 1.0 || factor < 0.0)
         factor = 1.0;
   } else {
      factor = 1.0;
   }
   temperature += to_int((adjustment + correction) * factor);
#endif

   temperature += (adjustment + correction);

   // calculate the temperature string and other effects based on the
   // race of the person.
   temperature_str = (this_object()->query_race_ob())->
      temperature_effects(this_object(), temperature);

   return temperature;
}

string weather_extra_look()
{
   if (stringp(temperature_str) && strlen(temperature_str)) {
      return capitalize((string) this_object()->query_pronoun()) + " looks " +
         temperature_str + ".\n";
   }
   return "";
}
