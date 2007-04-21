/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: general_hypothermia.c,v 1.2 2000/01/07 22:18:49 ceres Exp $
 * $Log: general_hypothermia.c,v $
 * Revision 1.2  2000/01/07 22:18:49  ceres
 * bing
 *
 * Revision 1.1  1998/01/06 04:08:27  ceres
 * Initial revision
 * 
*/
/**
 * This effect has a classification of "disease.hypothermia".
 * <p>
 * Describe the arguments in here.
 * @classification disease.hypothermia
 * @see help::effects
 */
#include <effect.h>
#include <weather.h>
#include <clothing.h>

#define DEBUG 1
#define COMFORTABLE 20
#define DEATH -500

void shiver(object player, int temperatureess, int id);
int calc_personal_temp(object player);

/** @ignore yes */
string query_classification() { return "disease.hypothermia"; }

/** @ignore yes */
int beginning( object player, int temperature, int id ) {
  if(temperature + calc_personal_temp(player) < COMFORTABLE) {
    tell_object(player, "You feel a bit chilly.\n");
    
    player->submit_ee( "shiver", ({ 20, 20 }), EE_CONTINUOUS );
    player->submit_ee( "stat_adjusts", ({ 240, 240 }), EE_CONTINUOUS );
    player->add_extra_look( this_object() );

  } else {
    player->submit_ee(0, 0, EE_REMOVE );
  }
  return temperature + calc_personal_temp(player) - COMFORTABLE;
}

/** @ignore yes */
int merge_effect( object player, int new_temperature, int old_temperature,
                  int id ) {
  return old_temperature + (new_temperature + calc_personal_temp(player) -
                            COMFORTABLE);
}

void restart(object player) {
  player->add_extra_look( this_object() );
}

/** @ignore yes */
void end( object player ) {
  tell_object( player, "You don't feel cold anymore.\n" );
  player->remove_extra_look( this_object() );
}

void shiver(object player, int temperature, int id) {
  int *enums, pwet, personal;
  int room_temp, min;
  
  if (!environment(player))
    return;

  // Update the temperature factor so they warm up or cool down as
  // appropriate.
  room_temp = WEATHER->temperature_index(environment(player));
  personal = calc_personal_temp(player);
  if(room_temp + personal > COMFORTABLE)
    temperature += (room_temp + personal - COMFORTABLE);

  // Set a lower limit on the coldness factor. This stops people dying of
  // hypothermia at 5C
  min = 100 * (personal + room_temp - COMFORTABLE);
  if(min > COMFORTABLE)
    min = personal + room_temp - COMFORTABLE;
  
  if(temperature < min)
    temperature = min;

  if(temperature >= COMFORTABLE)
    player->submit_ee(0, 0, EE_REMOVE );

  // They die if their temperature factor gets too low.
  if ( temperature < DEATH ) {
    player->dest_hide_shadow();
    tell_object( player, "The cold is just too much for you.\n" );
    tell_room( environment( player ), "Frost riming "+
              (string)player->query_possessive() +" skin in a thick crust, "+
              (string)player->the_short() +" expires.\n", player );
    player->do_death();
    return;
  }

  // Display some messages to them about how cold they are.
  switch(random(4)) {
  case 0 :
    player->new_parser("shiver");
    tell_object(player, "You feel cold to your bones.\n");
    break;
  case 1 :
    player->dest_hide_shadow();
    tell_object( player, "You rub your arms briskly in an attempt to "+
                "keep warm.\n" );
    tell_room( environment( player ),
              capitalize( (string)player->short() ) +" rubs "+
              (string)player->query_possessive() +" arms briskly.\n",
              player );
    break;
  case 2 :
    player->new_parser("shiver");
    enums = (int *)player->effects_matching("body.wetness"); 
    if ( sizeof( enums ) )
      pwet = (int)player->arg_of( enums[ 0 ] );
         
    if ( (int)pwet > 25 ) {
      tell_object( player, "You dislodge a lump of ice, which falls "+
                  "to the ground and shatters.\n" );
      tell_room( environment( player ),
                capitalize( (string)player->short() ) +" dislodges a lump "+
                "of ices, which falls to the ground and shatters.\n",
                player );
      player->add_effect("/std/effects/other/wetness", -25 );
    }
    break;
  default :
    player->new_parser("shiver");
    tell_object(player, "You feel really cold.\n" );
  }

  player->set_arg_of( (int)player->sid_to_enum( id ), temperature );
  return;
}

int calc_personal_temp(object player) {
  object where;
  string item, zone, type, *types, *zones;
  int count;

  zones = ({ });
  
  where = environment(player);
  count = 0;
  
  if(where->query_property("location") == "inside")
    count += 20;
  
  count += where->query_property("warmth");

  // Add up the number of zones covered by the clothing. The more zones the
  // warmer they'll be.
  foreach(item in player->query_wearing()) {
    types = item->query_type();
    if(!arrayp(types))
      types = ({ item->query_type() });
    
    foreach(type in types) {
      // Find out what zone this clothing type covers.
      // If it doesn't cover a zone (eg. jewellery) it doesn't give any
      // warmth.
      foreach(zone in CLOTHING_HANDLER->query_zone_names(type)) {
        if(member_array(zone, zones) == -1)
          zones += ({ zone });
        if(item->query_property("warmth"))
          count += item->query_property("warmth");
        else
          count ++;
      }
    }
  }

  count += sizeof(zones);
  count += environment(player)->query_property("warmth");

  return count;
}

void stat_adjusts(object player, int temperature, int id ) {
  if(WEATHER->temperature_index(environment(player)) > 0 ||
     temperature > -100)
    return;

  switch(player->query_race()) {
  case "troll":
    tell_object( player, "You feel a tingling between your ears and "+
                 "new and strangely clever thoughts race along the "+
                 "superconducting neural pathways of your brain.\n" );
    player->adjust_int( 1 );
    break;
  default:
    tell_object( player, "You feel your strength being drained by the "+
                 "freezing temperatures.\n" );
    player->adjust_tmp_str( -1 );
    player->adjust_tmp_con( -1 );
    break;
  }
}

string extra_look( object player ) {
  string desc;
  int temperature;
  int *enums;

  enums = (int *)player->effects_matching( "disease.hypothermia" );
  if ( !sizeof( enums ) )
    return "";

  temperature = (int)player->arg_of(enums[0]);

  switch(temperature/5) {
    case -5 .. 100 :
      desc = "looks a bit cold.\n";
      break;
    case -10 .. -6 :
      desc = "looks rather cold.\n";
      break;
    case -30 .. -11 :
      desc = "looks very cold.\n";
      break;
    case -80 .. -31 :
      desc = "appears to be suffering from extreme hypothermia.\n";
      break;
    case -100 .. -81 :
      desc = "looks close to death from hypothermia.\n";
      break;
    default :
      return "";
  }
  return capitalize( (string)player->query_pronoun() ) +" "+ desc;
}
