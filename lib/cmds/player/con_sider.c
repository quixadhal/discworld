/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: con_sider.c,v 1.24 2003/07/11 00:53:21 pinkfish Exp $
 * 
 */
#define CREATOR "Ceres"
/*
  Replacement consider command. This one takes weapons, skills, armour and hit
  points into account whereas the old one only compared their overall fighting
  skill
 */
#include <weapon.h>
#include <cmr.h>
#include <player.h>

inherit "/cmds/base";

int calc_danger(object ob);
string do_compare(int dif);

mixed cmd(object *obs) {
  object ob;
  int level, ok, dif, total, them;
  string out;

  if(previous_object()->check_dark((int)environment(previous_object())->
    query_light()) < 0) {
     add_failed_mess("It's too dark to consider anyone.\n");
     return 0;
  }
  if(previous_object()->check_dark((int)environment(previous_object())->
    query_light()) > 0) {
     add_failed_mess("It's too bright to consider anyone.\n");
     return 0;
  }

  if (sizeof(obs) > 1) {
     obs -= ({ this_player() });
  }
  if (sizeof(obs) > 5) {
     add_failed_mess("You can only consider five things at once.\n");
     return 0;
  }
  
  if (this_player()->query_property("dead"))  {
    add_failed_mess( "You're dead.  You could just about kill a "
                        "conversation.\n" );
    return 0;
  }

  level = calc_danger(previous_object());
  
  foreach (ob in obs) {
    if (!living(ob)) {
       continue;
    }
    if (ob == previous_object()) {
       if (sizeof(obs) == 1) {
          write("You are exactly equal to yourself, you had better go and "
                "arm yourself with more spiky things and try to not arm "
                "your reflection at the same time.  No problems?\n");
          ok++;
       }
       continue;
    }
    ok++;
    them = calc_danger(ob);
    total += them;
    
    // percentage difference.
    dif = ((level - them) * 100) / level;

    if ( userp( ob ) && this_player()->query_player_killer() &&
          !ob->query_player_killer() ) {
       out = replace( do_compare( dif ), "$who$", (string)ob->the_short() +
             " [not a killer]" );
    } else {
       out = replace( do_compare( dif ), "$who$", (string)ob->the_short() );
    }

    if (userp(ob)) {
       tell_object(ob, this_player()->the_short(1) + " looks you up and down "
                       "carefully checking out your weapons and armour.\n");
    } else {
       ob->event_npc_consider(this_player());
    }
    
    write( out +"\n" );
    
  }
  if(!ok) {
    add_failed_mess("You need to consider someone.\n");
    return 0;
  }
  return 1;
}
  
string do_compare(int dif) {
   string ret;
   int level;

   ret = "$C$$who$";

   // Check and see if they are drunk first.
   level = PLAYER_DRUNK_EFFECT->query_drunk_level(this_player());
   if (level > 4) {
      if (level > 7) {
         return "%^BOLD%^%^GREEN%^Mate!  They looked at you wrong, you "
                "better sort them out before they get more cocky.%^RESET%^";
      }
      return "%^BOLD%^%^GREEN%^You reckon they would be an easy "
             "target.  Why bother with weapons?%^RESET%^";
   }
  
  if(dif > 320) {
    ret = "%^BOLD%^GREEN%^"+ret+" is too weak to bother attacking";
  } else {
    switch(dif) {
    case 161..320:
      ret = "%^BOLD%^%^GREEN%^"+ret+" is someone you look down on";
      break;
    case 81..160:
      ret = "%^GREEN%^"+ret+" would be an easy target";
      break;
    case 41..80:
      ret = "%^GREEN%^"+ret+" is a lot weaker than you";
      break;
    case 21..40:
      ret = "%^GREEN%^"+ret+" is weaker than you";
      break;
    case 11..20:
      ret = "%^CYAN%^"+ret+" is slightly weaker than you";
      break;
    case -10..10:
      ret = "%^CYAN%^"+ret+" is about equal with you";
      break;
    case -20..-11:
      ret = "%^CYAN%^"+ret+" is slightly tougher than you";
      break;
    case -40..-21:
      ret = "%^RED%^"+ret+" is tougher than you";
      break;
    case -80..-41:
      ret = "%^RED%^"+ret+" is a lot tougher than you";
      break;
    case -160..-81:
      ret = "%^RED%^"+ret+" looks pretty nasty";
      break;
    case -320..-161:
      ret = "%^BOLD%^%^RED%^"+ret+" is into the don't-even-think-about-it "
        "region";
      break;
    default:
      ret = "%^BOLD%^%^RED%^You would have to be utterly insane to attack "
        "$who$";
      break;
    }
   }
   return ret + ".%^RESET%^";
}

int calc_weapon_damage(object weapon) {
  mixed *data;
  int wdam, *damage, i;
  
  data = (mixed *)weapon->query_attack_data();
  if(sizeof(data)) {
    wdam = 0;
    for ( i = 0; i < sizeof( data ); i += W_ARRAY_SIZE ) {
      if (member_array( data[ i + W_TYPE ], ({ "blunt", "unarmed",
                                               "sharp", "pierce" }) ) == -1 )
        continue;
      damage = data[ i + W_DAMAGE ];
      wdam += ( data[ i + W_CHANCE ] * ( damage[ F_FIXED ] +
                        ( damage[ F_NUM ] * damage[ F_DIE ] ) / 2 ) ) / 100;
      wdam += damage[ F_FIXED ] + damage[ F_NUM ] * damage[ F_DIE ];
    }
  }
  return wdam;
}

int calc_value(mixed arr) {
  if (intp(arr))
    return arr;
  if (!pointerp(arr))
    return 0;
  if (sizeof(arr) == 1)
    return arr[F_FIXED];
  if (sizeof(arr) == 2)
    return arr[F_FIXED]+arr[F_NUM];
  return arr[F_FIXED]+(arr[F_NUM] * arr[F_DIE]);
}

int calc_armour_protection(object armour) {
  int ac, i;
  mapping data;
  mixed *stuff;

  if(!armour->query_armour())
    return 0;

  data = armour->query_armour_class();
  if(!sizeof(data))
     return 0;
  stuff = keys(data);
  for(i=0; i<sizeof(stuff); i++)
    ac += calc_value(data[stuff[i]][0]);
  return ac;
}

//  calculate how dangerous ob is. Factors used include: weapon damage, weapon
// skill, defensive skill, armour worn and hit points.
int calc_danger(object ob) {
   object weapon;
   object *weapons;
   object armour;
   object rider;
   int wdam;
   int wskill;
   int dskill;
   int ac;
   int hp;
   int trans;

   if (ob->query_transport()) {
      foreach (rider in ob->query_riders()) {
         trans += calc_danger(rider);
      }
   }

  // decide which weapon to use for the consider
  weapons = ob->query_holding();

  if(weapons && sizeof(weapons) > 0 && (weapons[0] != 0) && (weapons[0]->
    query_weapon())) {
    weapon = weapons[0];
    dskill = ob->query_skill_bonus("fighting.combat.parry.melee");
  } else if(weapons && sizeof(weapons) > 1 && (weapons[1] != 0) && 
      (weapons[1]->query_weapon())) {
    weapon = weapons[1];
    dskill = ob->query_skill_bonus("fighting.combat.parry.melee");
  } else {
    weapon = ob;
    dskill = ob->query_skill_bonus("fighting.combat.parry.melee");
  }

  // count parry *and* dodge skill because they might change tactics
  // partway through any combat.
  dskill += ob->query_skill_bonus("fighting.combat.dodging.melee");
  
  wdam = calc_weapon_damage(weapon);
  if(!sizeof(weapon->query_attack_data())) {
    wskill += ob->query_skill_bonus("fighting.combat.melee.unarmed");
    wskill += ob->query_skill_bonus("fighting.combat.special.unarmed") / 2;
  } else {
    wskill += ob->query_skill_bonus("fighting.combat.melee."+
              (string)weapon->query_attack_data()[W_SKILL]);
    wskill += ob->query_skill_bonus("fighting.combat.special.weapon") / 2;
  }

  wskill += ob->query_skill_bonus("fighting.combat.special.tactics");
  
  wskill += ob->query_specific_gp("fighting");
  hp = ob->query_hp();

  if(ob->query_wearing())
    foreach(armour in ob->query_wearing())
      if(armour)
        ac += calc_armour_protection(armour);

  // weighting of the different statistics
  wdam /= 3;
  hp /= 3;
  dskill /= 2;
  ac /= 6;
  
  return trans + wdam + wskill + dskill + ac + hp;
}

mixed* query_patterns() {
  return ({ "<indirect:living:here>", (: cmd($1) :) });
} /* query_patterns() */
