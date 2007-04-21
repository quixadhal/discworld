#include <weapon.h>

inherit "/cmds/base";
mapping _weapons;
mapping _items;
mapping _matched;

// Condition modifiers for materials
mapping _conditions = ([ "cloth" : 800, "rubber" : 100, "hide" : 700, "leather" : 600, "wood" : 500, "bone" : 200, "silver" : 300, "copper" : 400, "stone" : 400, "bronze" : 500, "iron" : 700, "steel" : 900, "klatchian steel" : 1100, "octiron" : 1300 ]);

// Damage chance modifiers for materials
mapping _chances = ([ "cloth" : 20, "rubber" : 19, "hide" : 17, "leather" : 15, "wood" : 12, "bone" : 8, "silver" : 15, "copper" : 10, "stone" : 3, "bronze" : 8, "iron" : 6, "steel" : 4, "klatchian steel" : 2, "octiron" : 0 ]);

// Weight modifiers for different materials
mapping _weights = ([ "cloth" : 1, "rubber" : 3, "hide" : 2, "leather" : 2, "wood" : 3, "bone" : 3, "silver" : 6, "copper" : 6, "stone" : 5, "bronze" : 5, "iron" : 5, "steel" : 5, "klatchian steel" : 5, "octiron" : 5 ]);

// Map some weird things to more standard equivalents.
mapping _equivalents = ([ "chocolate" : "rubber",
                          "earthworm" : "octiron" ]);

// The light & heavy weapon specials.
mapping _specials = ([ "bash" : "smash",
                       "hack" : "chop",
                       "stab" : "pierce",
                       "slash" : "slice" ]);
// Standard attack names/types/skills
mapping _types = ([ "strike" : "blunt",
                    "bash" : "blunt",
                    "smash" : "blunt",
                    "hack" : "sharp",
                    "chop" : "sharp",
                    "poke" : "pierce",
                    "stab" : "pierce",
                    "pierce" : "pierce",
                    "cut" : "sharp",
                    "slash" : "sharp",
                    "slice" : "sharp",
                    "spike" : "pierce",
                    "fire" : "any" ]);

nosave string *_attack_types = ({ "blunt", "sharp", "pierce", "fire" });

void create() {
  ::create();
  _weapons = "/obj/handlers/armoury"->make_list("/obj/nweapons/",
                                                ({".c", ".wep"}));
}

string warn(int i) {
  switch(i) {
  case 2:
    return "  %^BOLD%^%^RED%^!%^RESET%^ ";
  case 1:
    return "  %^ORANGE%^o%^RESET%^ ";
  default:
    return "  %^GREEN%^-%^RESET%^ ";
  }
}

int *calc_rating(object weapon) {
    int i, size;
    int ave = 0;
    int max = 0;
    int *damage;
    mixed *data;

    data = (mixed *)weapon->query_attack_data();
    
    if ( !( size = sizeof( data ) ) )
      return ({ 0, 0 });

    for ( i = 0; i < size; i += W_ARRAY_SIZE ) {
      if ( member_array( data[ i + W_SKILL ], _attack_types ) == -1 )
        continue;
      damage = data[ i + W_DAMAGE ];
      ave += ( data[ i + W_CHANCE ] * ( damage[ F_FIXED ] +
            ( damage[ F_NUM ] * ( 1 + damage[ F_DIE ] ) ) / 2 ) ) / 100;
      if(damage[ F_FIXED ] + damage[ F_NUM ] * damage[ F_DIE ] > max)
        max = damage[ F_FIXED ] + damage[ F_NUM ] * damage[ F_DIE ];
    }
    return ({ ave, max });
}

int calc_ave(mixed damage) {
  if(arrayp(damage))
    return damage[F_FIXED] + (damage[F_NUM] * (1 + damage[F_DIE])) / 2;
  
  return damage;
}

int calc_max(mixed damage) {
  if(arrayp(damage))
    return damage[F_FIXED] + (damage[F_NUM] * damage[F_DIE]);
  
  return damage;
}

object *filter(object who) {
  object *tmp;

  if(who->query_creator())
    return ({});
  
  tmp = who->query_weapons();
  switch(sizeof(tmp)) {
  case 2:
    if(tmp[0]->query_property("virtual name")) {
      if(tmp[0]->query_property("virtual name") ==
         tmp[1]->query_property("virtual name"))
        return ({ tmp[0] });
    } else {
      if(base_name(tmp[0]) == base_name(tmp[1]))
        return ({ tmp[0] });
    }
    break;
  default:
  }
  return tmp;
}

int list(string type) {
  mapping weapons;
  object wep;
  mixed *data;
  string *names, *types, str;
  int i;

  weapons = "/obj/handlers/armoury"->query_items("weapons");

  foreach(str in keys(weapons)) {
    if(str == "twoedge" || str == "kring" ||
       strsrch(weapons[str], "ranged") != -1 ||
       strsrch(weapons[str], "misc") != -1)
      continue;
 
    wep = "/obj/handlers/armoury"->request_item(str);
    if(!wep)
      continue;

    types = wep->query_attack_types();
    if(type && types && member_array(type, types) == -1)
      continue;

    data = wep->query_attack_data();
    names = wep->query_attack_names();
    for(i=0; i<sizeof(data); i += W_ARRAY_SIZE) {
      printf("%-30s %8s %3d %3d %3d %6s\n",
             wep->query_short(),
             names[i / W_ARRAY_SIZE],
             data[i + W_CHANCE],
             calc_ave(data[i+W_DAMAGE]),
             calc_max(data[i+W_DAMAGE]),
             data[i+W_SKILL]);
      write_file("/w/ceres/WEAPONS.csv",
                 sprintf("%s,%s,%d,%d,%d,%s\n",
                         wep->query_short(),
                         names[i / W_ARRAY_SIZE],
                         data[i + W_CHANCE],
                         calc_ave(data[i+W_DAMAGE]),
                         calc_max(data[i+W_DAMAGE]),
                         data[i+W_SKILL]));
    }
    wep->dest_me();
  }
  return 1;
}

int sort_weapons(int wep1, int wep2) {
  if(wep1 > wep2)
    return -1;
  if(wep1 < wep2)
    return 1;
  return 0;
}

int players() {
  mapping weapons;
  object wep, *weps;
  mixed *data;
  string str;
  
  data = map(users(), "filter");
  weapons = ([ ]);
  foreach(weps in data) {
    if(weps) {
      foreach(wep in weps) {
        if(wep->query_property("virtual name"))
          str = (clone_object(wep->query_property("virtual name"))->query_short());
        else
          str = base_name(wep)->query_short();
        if(!weapons[str])
          weapons[str] = 1;
        else
          weapons[str] += 1;
      }
    }
  }
  foreach(str in sort_array(keys(weapons),
                            (: sort_weapons($(weapons)[$1],
                                            $(weapons)[$2]) :)))
    if(str && weapons[str])
      printf("%d %s\n", weapons[str], str);
  
  return 1;
}

private string *map_materials(string *materials) {
  string str;
  
  foreach(str in materials) {
    if(_equivalents[str]) {
      materials -= ({ str });
      materials += ({ _equivalents[str] });
    }
  }
  return materials;
}

void total(object pl) {
  write("Total of " + _matched[pl] + " weapons found.\n");
}

void check(int bad_only, string type, string search, string *weapons,
           object pl) {
  string str, *materials, att, ret;
  string *tstr, *anames;
  object wep;
  int cond, dc, ave, max, weight, tmp, bad, count;
  mixed *data;
  float wf;

  count = 0;
  foreach(str in weapons) {
    bad = 0;
    if(wep)
      wep->dest_me();

    // These don't load.
    if(str == "twoedge" || str == "kring")
      continue;

    // exclude the weird stuff if they'd checking for bad/warning weapons.
    if(bad_only && (strsrch(str, "bow") != -1 ||
                    str == "satin knife boots" || str == "knife boots" ||
                    strsrch(_items[pl][str], "ranged") != -1 ||
                    strsrch(_items[pl][str], "misc") != -1))
      continue;

    // Search for specific weapons.
    if(search != "" && strsrch(str, search) == -1 &&
       strsrch(_items[pl][str], search) == -1)
      continue;
    
    wep = clone_object(_items[pl][str]);
    if(!wep || !wep->query_short())
      continue;

    materials = map_materials(wep->query_materials());
    cond = wep->query_max_cond();
    dc = wep->query_damage_chance();

    ret = "";
    if(search)
      ret = sprintf("\n");
    
    ret += sprintf("%s (%s)\n", capitalize(wep->query_short()),
                   _items[pl][str]);

    tstr = ({ });

    if(!sizeof(materials) || sizeof(materials - keys(_conditions)))
      tstr += ({ "materials" });
    else
      ret += warn(0) + sprintf("is made of %s\n",
                               query_multiple_short(materials));

    if(wep->query_length() == 1)
      tstr += ({ "length" });
    if(wep->query_width() == 1)
      tstr += ({ "width" });
    if(!wep->query_weight())
      tstr += ({ "weight" });
    
    if(sizeof(tstr)) {
      if(sizeof(tstr) == 1 && tstr[0] == "width") {
        ret += warn(1);
        //if(bad < 2)
        //bad = 1;
      } else {
        ret += warn(2);
        bad = 2;
      }
      ret += sprintf("has no %s\n", replace(query_multiple_short(tstr),
                                            "and", "or"));

      if(member_array("materials", tstr) != -1) {
        tell_object(pl, ret);
        continue;
      }
    }

    if(wep->query_length() != 1 && wep->query_width() != 1)
      ret += warn(0) + sprintf("is %s%d\" (%dcm) by %s%d\" (%dcm) "
                               "and weighs %.1f%s (%.1fkg)\n",
                               (wep->query_length() / 12) > 0 ?
                               (wep->query_length() / 12) + "'" : "",
                               wep->query_length() % 12,
                               to_int(wep->query_length() * 2.54),
                               (wep->query_width() / 12) > 0 ?
                               (wep->query_width() / 12) + "'" : "",
                               wep->query_width() % 12,
                               to_int(wep->query_width() * 2.54),
                               wep->query_weight() / 9.0,
                               (wep->query_weight() / 9.0) == 1.0 ? "lb" :
                               "lbs",
                               wep->query_weight() / 20.0);

    // Calculate the ratio of length+width+material to weight.
    wf = 0;
    foreach(tstr in materials)
      wf += _weights[tstr];
    wf /= sizeof(materials);
    
    wf = (wep->query_weight() * (wep->query_weight() / 2)) /
      (wep->query_length() * wep->query_width() * wf);

    if(wf < 0.5 || wf > 3.0) {
      ret += warn(1);
      if(bad < 2)
        bad = 1;
    } else
      ret += warn(0);
    
    ret += sprintf("has weight factor of %.1f [0.5/1.0/3.0] "
                   "(%%^ORANGE%%^Experimental%%^RESET%%^)\n", wf);
      
    switch(type) {
    case "weapons":
      weight = 10 + 2 * sqrt(wep->query_weight());
      break;
    case "armours":
      weight = 5 + 2 * sqrt(wep->query_weight());
      break;
    case "clothes":
      weight = 2 * sqrt(wep->query_weight());
      break;
    }

    tmp = 0;
    foreach(tstr in materials)
      tmp += _conditions[tstr];

    tmp /= sizeof(materials);
    tmp *= weight;
    
    if((cond < tmp - tmp/10) || (cond > tmp + tmp/10)) {
      ret += warn(2) + sprintf("has max cond of %d should be [%d/%d/%d]\n",
                               cond, tmp - tmp/10, tmp, tmp + tmp/10);
      tell_object(pl, ret);
      continue;
    } else if(search)
      ret += warn(0) + sprintf("has max cond of %d [%d/%d/%d]\n",
                               cond, tmp - tmp/10, tmp, tmp + tmp/10);


    tmp = 0;
    foreach(tstr in materials)
      tmp += _chances[tstr];
    tmp /= sizeof(materials);

    if(dc < tmp -1 || dc > tmp+1) {
      ret += warn(2) + sprintf("has damage chance of %d should be "
                               "[%d/%d/%d]\n",
                               dc, tmp-1, tmp, tmp+1);
      bad = 2;
    } else if(search)
      ret += warn(0) + sprintf("has damage chance of %d [%d/%d/%d]\n",
                               dc, tmp-1, tmp, tmp+1);

    // Check the common specials
    anames = wep->query_attack_names();
    if(!sizeof(anames)) {
      ret += warn(2) + sprintf("has no attacks.\n");
      tell_object(pl, ret);
      continue;
    }

    foreach(att in anames) {
      if(!_specials[att] && member_array(att, keys(_types)) == -1) {
        ret += warn(1) + sprintf("has non-standard %s attack\n", att);
        if(bad < 2)
          bad = 1;
      } else if(wep->query_weight() < 30 &&
         member_array(att, values(_specials)) != -1) {
        ret += warn(2) + sprintf("has %s but is too light\n", att);
        bad = 2;
      } else if(wep->query_weight() > 50 && _specials[att] && 
                member_array(_specials[att], anames) == -1) {
        ret += warn(1) + sprintf("has %s but not %s\n", att, _specials[att]);
        if(bad < 2)
          bad = 1;
      }
    }
    
    // Check type & skill.
    data = wep->query_attack_data();
    for(tmp=0; tmp < sizeof(data); tmp += W_ARRAY_SIZE) {
      if(member_array(data[tmp+W_TYPE], _attack_types) == -1) {
        ret += warn(2) + sprintf("has unknown attack type %s\n",
                                 data[tmp+W_TYPE]);
        bad = 2;
      }
      if(member_array(data[tmp+W_SKILL], values(_types)) == -1) {
        ret += warn(2) + sprintf("has unknown attack skill %s\n",
                                 data[tmp+W_SKILL]);
        bad = 2;
      }

      if(_types[anames[tmp/W_ARRAY_SIZE]] &&
         _types[anames[tmp/W_ARRAY_SIZE]] != "any" &&
         _types[anames[tmp/W_ARRAY_SIZE]] != data[tmp+W_SKILL]) {
        ret += warn(2) + sprintf("has attack %s with skill of %s\n",
                                 anames[tmp/W_ARRAY_SIZE], data[tmp+W_SKILL]);
        bad = 2;
      }
      if(data[tmp+W_FUNCTION]) {
        ret += warn(1) + sprintf("has attack function %s on %s attack\n",
                                 data[tmp+W_FUNCTION],
                                 anames[tmp/W_ARRAY_SIZE]);
        bad = 1;
      }
    }
    
    // Check the axe attacks.
    if(strsrch(_items[pl][str], "axe") == -1 &&
       strsrch(wep->query_long(), "axe") == -1) {
      if(member_array("chop", wep->query_attack_names()) != -1) {
        ret += warn(1) + sprintf("has chop attack but is not an axe.\n");
        if(bad < 2)
          bad = 1;
      }
      if(member_array("hack", wep->query_attack_names()) != -1) {
        ret += warn(1) + sprintf("has chop attack but is not an axe.\n");
        if(bad < 2)
          bad = 1;
      }
    } else {
      if(member_array("slash", wep->query_attack_names()) != -1) {
        ret += warn(1) + sprintf("has slash attack but is an axe.\n");
        if(bad < 2)
          bad = 1;
      }
      if(member_array("slice", wep->query_attack_names()) != -1) {
        ret += warn(1) + sprintf("has slice attack but is an axe.\n");
        if(bad < 2)
          bad = 1;
      }
    }
    
    if(cond) {
      data = calc_rating(wep);
      ave = data[0];
      max = data[1];

      if(ave > 140 || max > 300) {
        ret += warn(2);
        bad = 2;
      } else
        ret += warn(0);
      
      ret += sprintf("has damage of %d/%d max permitted is [140/300]\n",
                     ave, max);

      tmp = (100 * ave * max) / cond; // damage factor
      if(tmp < 75 || tmp > 150) {
        ret += warn(1);
        if(bad < 2)
          bad = 1;
      } else
        ret += warn(0);
      
      ret += sprintf("has damage factor of %d [75/150] "
                     "(%%^ORANGE%%^Experimental%%^RESET%%^)\n",
                     tmp);
    }
    
    if(bad >= bad_only) {
      tell_object(pl, ret);
      count++;
    }
  }
  _matched[pl] += count;
  return;
}

int do_check(int bad_only, string type) {
  string *tmp, search;
  int i;

  if(!_items)
    _items = ([ ]);
  if(!_matched)
    _matched = ([ ]);
  
  if(type == "armours" || type == "clothes") {
    _items[this_player()] = "/obj/handlers/armoury"->query_items(type);
    search = "";
  } else if(type == "weapons") {
    _items[this_player()] = _weapons;
    search = "";
  } else {
    search = type;
    type = "weapons";
    _items[this_player()] = _weapons;
  }

  _matched[this_player()] = 0;
  
  tmp = keys(_items[this_player()]);

  if(!sizeof(tmp))
    return notify_fail("No items matched.\n");

  for(i=0; i<sizeof(tmp); i+= 50) {
    if(i+50 < sizeof(tmp)) {
      call_out("check", 0, bad_only, type, search, tmp[i..i+49],
               this_player());
    } else
      call_out("check", 0, bad_only, type, search, tmp[i..], this_player());
  }
  call_out("total", 1, this_player());
  return 1;
}

mixed *query_patterns() {
  return ({
    //"list", (: list("") :),
              //              "list <string'type'>", (: list($4[0]) :), 
              "players", (: players() :),
              "check", (: do_check(0, "") :),
              "check <string'type'>", (: do_check(0, $4[0]) :),
              "check bad", (: do_check(2, "") :),
                "check bad <string'type'>", (: do_check(2, $4[0]) :),
              "check warn", (: do_check(1, "") :),
              "check warn <string'type'>", (: do_check(1, $4[0]) :)
                });
}
