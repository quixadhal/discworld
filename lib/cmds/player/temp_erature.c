/*  -*- LPC -*-  */
#include <weather.h>
#include <clothing.h>
#include <playtesters.h>

#define COMFORTABLE 20

#define ABS(x) (x > 0? x : -x)

string disp_warmth(int, int);
string disp_total(int i);

int cmd(string str) {
  object where, *clothes, item;
  string zone, type, *zones, tmp;
  mixed types;
  int adjustment, correction, temperature, room_temp, *enums, cwarm, ccool;
  
  where = environment(this_player());
  
  if(!where)
    return notify_fail("You have no environment.\n");
  
  // If they're on a horse or somesuch then use that locations temperature
  // info.
  if(!where->query_property("location") &&
     environment(where) &&
     environment(where)->query_property("location")) {
    where = environment(where);
  }

  // Figure out the temperature where we are.
  switch(where->query_property("location")) {
  case "outside":
    room_temp = (WEATHER->query_temperature(where) - COMFORTABLE);
    break;
  default:
    room_temp = 0;
  }

  temperature = this_player()->query_personal_temp();
                 
  // set the warmth of the room.
  room_temp += where->query_property("warmth");
  room_temp -= where->query_property("cooling");

  write(sprintf("It's %s here.\n",
         WEATHER->temp_string(room_temp + COMFORTABLE)));

  // Calculate how warm (or cool) their clothing is keeping them.
  clothes = filter_array(this_player()->query_wearing(),
                         (: !$1->id("scabbard") &&
                          !$1->id("belt") &&
                          !$1->id("backpack") &&
                          !$1->id("glasses") &&
                          !$1->id("jewellery") :));
  
  zones = ({ });
  foreach(item in clothes) {
    types = item->query_type();
    if(!arrayp(types)) {
      types = ({ item->query_type() });
    }
    
    ccool = 0;
    cwarm = 0;
    
    foreach(type in types) {
      // Find out what zone this clothing type covers.
      // If it doesn't cover a zone (eg. jewellery) it doesn't give any
      // warmth.
      
      foreach(zone in CLOTHING_HANDLER->query_zone_names(type)) {
        if(member_array(zone, zones) == -1)
          zones += ({ zone });
        if(item->query_property("warmth")) {
          adjustment += item->query_property("warmth");
          cwarm += item->query_property("warmth");
        } else {
          adjustment++;
          cwarm++;
        }
        
        // If it's warm here then do adjustments for clothing that
        // cool you.
        if(room_temp > 0 && item->query_property("cooling")) {
          adjustment -= item->query_property("cooling");
          ccool += item->query_property("cooling");
        }
      }
    }
    tmp = item->query_name();
    if(cwarm && ccool) {
      write(sprintf("Your %s provide%s %s and %s.\n", item->query_short(),
             (tmp[<1] == 's'  &&  tmp[<2 .. <1] != "ss") ? "" : "s",
             disp_warmth(cwarm, 1),
             disp_warmth(ccool, 0)));
    } else if(cwarm) {
      write(sprintf("Your %s provide%s %s.\n", item->query_short(),
             (tmp[<1] == 's'  &&  tmp[<2 .. <1] != "ss") ? "" : "s",
             disp_warmth(cwarm, 1)));
    } else if(ccool) {
      write(sprintf("Your %s provide%s %s.\n", item->query_short(),
             (tmp[<1] == 's'  &&  tmp[<2 .. <1] != "ss") ? "" : "s",
             disp_warmth(ccool, 0)));
    }
  }
  
  // You get a warmth bonus or coolness bonus for each zone covered.
  // This means it's better to cover more of your body when it's cold.
  // It also means you can wear light clothing with less temperature
  // penalties since the warmth & cooling cancel out somewhat.
  if(room_temp < 0) {
    adjustment += sizeof(zones);
    if(room_temp + sizeof(zones) > 5)
      adjustment -= (room_temp + sizeof(zones) - 5);
  } else {
    adjustment -= sizeof(zones);
    if(room_temp - sizeof(zones) < -5)
      adjustment -= (room_temp - sizeof(zones) + 5);
  }

  // Wetness makes you cooler -- so sweating isn't necessarily a bad thing.
  ccool = 0;
  enums = (int *)this_player()->effects_matching("body.wetness"); 
  if ( sizeof( enums ) ) {
    adjustment -= sqrt(sqrt((int)this_player()->arg_of( enums[ 0 ] ))) * 2;
    ccool = sqrt(sqrt((int)this_player()->arg_of( enums[ 0 ] ))) * 2;
  }

  // add their personal warmth (from effects or shadows)
  adjustment += this_player()->query_property("warmth");
  adjustment -= this_player()->query_property("cooling");
  
  // this hopefully it does two things.
  // 1. effectively puts a maximum/minimum on temperature
  // 2. accounts for the body working to adjust temperature.
  if(temperature > room_temp && room_temp >= 0 || temperature > 5)
    correction -= (temperature / 5) + 5;
  if(temperature < room_temp && room_temp <= 0 || temperature < -5)
    correction -= (temperature / 5) - 5;

  write(sprintf("On balance, you are %s and %s.\n",
         this_player()->query_temp_str(),
         disp_total(to_int((room_temp+adjustment + correction)))));
  
  return 1;
}

string disp_warmth(int i, int warm) {
  string pos, neg;

  if(warm) {
    pos = " warmth";
    neg = " cooling";
  } else {
    pos = " cooling";
    neg = " warmth";
  }
  
  switch(i) {
  case -1000..-19: return "an enormous amount of"+neg;
  case -9..-18: return "a lot of"+neg;
  case -8..-4: return "quite a lot of"+neg;
  case -3..-2: return "some"+neg;
  case -1: return "a bit of"+neg;
  case 0: return "no"+pos;
  case 1: return "a bit of"+pos;
  case 2..3: return "some"+pos;
  case 4..8: return "quite a lot of"+pos;
  case 9..18: return "a lot of"+pos;
  default: return "an enormous amount of"+pos;
  }
}

string disp_total(int i) {
  switch(i) {
  case -1000..-51: return "are freezing fast";
  case -50..-21: return "are cooling down quickly";
  case -20..-11: return "are cooling down";
  case -10..-6: return "are cooling down slowly";
  case -5..5: return "will stay pretty much as you are";
  case 6..10: return "are warming up slowly";
  case 11..20: return "are warming up";
  case 21..50: return "are warming up quickly";
  case 51..1000: return "are cooking rapidly";
  default:
    return "broken: " + i + "\n";
  }
}

mixed *query_patterns()  {
   return ({ "", (: cmd("") :) });
}
