inherit "/std/living/handle";
inherit "/std/living/mon_actions";
inherit "/std/living/living";
inherit "/global/communicate";
inherit "/global/drunk";
inherit "/global/guild-race";
inherit "/global/spells";
#include "monster.h"
#define MOVE_DEFAULT ({ 300, 300 })
#define SOUL_OBJECT "/obj/handlers/soul"
 
string cap_name;
mixed *chat_string,
      *achat_string,
      *attack_spells;
mixed move_after,
      *throw_out;
mixed talk_string;
int chat_chance,
    attack_everyone,
    aggressive,
    follow_speed,
    join_fight_type,
    alignment,
    achat_chance;
string race,
       class,
       *move_zones,
       *enter_commands,
       join_fight_mess;
static mixed *doing_story;
static object last_attacked;

void do_move_after(int bing);

void create() {
  reset_get();
  race_ob = "/std/races/unknown";
  /* patch added by [als] 17-oct-92 */ cur_lang = "common";
  follow_speed = 3;
  add_property("determinate", "");
  talk_string = ({ });
  doing_story = ({ ({ }), ({ }) });
  chat_string = ({ 0, ({ }) });
  achat_string = ({ 0, ({ }) });
  move_zones = ({ });
  attack_spells = ({ });
  languages = ({ });
  enter_commands = ({ });
  gr_commands = ([ ]);
  known_commands = ({ });
  enable_commands();
  living_commands();
  handle_commands();
  communicate_commands();
  add_action("soul_commqz", "*");
  mon_actions::create();
  living::create();
  spells::create();
  add_property("npc",1);
  add_action("do_teach", "teach");
  add_action("init_race", "init_race");
  race_guild_commands();
} /* create() */

int query_cols() { return 200; }

int soul_commqz(string str) {
  string verb, bit;

  if (sscanf(str, "%s %s", verb, bit) == 2)
    return (int)SOUL_OBJECT->soul_command(verb, bit);
  return (int)SOUL_OBJECT->soul_command(str, "");
} /* soul_commqz() */

string query_cap_name() {
  return cap_name;
} /* query_cap_name() */

void set_cap_name(string s) { cap_name = s; }

int soul_com_force(string str) {
  string str1,str2;

  if (file_name(previous_object()) != SOUL_OBJECT)
    return 0;
  command(str);
  return 1;
  if (sscanf(str, "%s %s", str1, str2) != 2)
    str1 = str;
  SOUL_OBJECT->soul_command(str1, str2);
} /* soul_com_force() */

int init_race() {
  race_guild_commands();
  return 1;
} /* init_race() */
 
void set_name(string n) {
  if (name && name != "object")
    return;
  name = n;
  cap_name = capitalize(name);
  add_plural(pluralize(name));
  set_short(capitalize(name));
  set_long("You see nothing special.\n");
  set_living_name(n);
} /* set_name() */

string long(string str, int dark) {
  string s;

  s = query_long();
  s += capitalize(query_pronoun())+" "+health_string()+".\n";
  s += calc_extra_look();
  s += query_living_contents(0);
  return s;
} /* long() */

int set_race(string str) {
  race = str;
  return 1;
} /* set_race() */

string query_race() { return race; }
string query_class() { return class; }
string query_guild() { return class; }
string query_profession() { return class; }

int set_class(string str) {
  class = str;
} /* set_class() */

int set_guild(string str) {
  class = str;
} /* set_guild() */

int set_profession(string str) {
  class = str;
} /* set_profession() */

varargs void init_command(string str, int tim) {
  call_out("do_command", tim, str);
} /* init_command() */

void init_equip() {
  call_out("do_equip", 0);
} /* init_equip() */

int do_command(string str) {
  if (stringp(str))
    return command(str);
  else
    printf("Invalid paramater to do_command %O in monster %O, env %O.\n", str,
           this_object(), environment());
} /* do_command() */

void init() {
  set_heart_beat(1);
  attack();
  if (aggressive)
    if (aggressive >= 2) {
      if (!this_player()->query_property("no attack"))
        attack_ob(this_player());
    } else if (interactive(this_player()))
      attack_ob(this_player());
} /* init() */
 
int check_anyone_here() {
  object *arr;
  int i;
 
  if (!environment())
    return 1;
  arr = all_inventory(environment());
  for (i=0;i<sizeof(arr);i++)
    if (interactive(arr[i]) && arr[i] != this_object())
      return 1;
  return 0;
 }

void set_attack_everyone(int i) { attack_everyone = i; }
int query_attack_everyone() { return attack_everyone; }

void set_throw_out(int hps, int chance, string their_mess,
                   string everyone_mess) {
  throw_out = ({ hps, chance, their_mess, everyone_mess });
} /* set_throw_out() */

int *query_throw_out() { return throw_out; }

varargs void run_away(int bing) {
  do_move_after(bing+1);
} /* run_away() */

string expand_string(string in_str, object on, object attacker) {
  string *str, ret;
  int i, j, add_dollar;
  object liv, *obs, ob;

  str = explode(in_str, "$");
  ret = "";
  for (i=0;i<sizeof(str);i++)
    switch (str[i][0]) {
      case 'm' :
        ob = this_object();
      case 'l' :
        if (!ob) {
          if (!liv) {
            obs = all_inventory(environment()) - ({ this_object() });
            if (sizeof(obs)) {
              int q;

              j = q = random(sizeof(obs));
              while (!living(obs[q++]) && j != q)
                q = q % sizeof(obs);
              liv = obs[q-1];
            }
          }
          if (!liv)  /* No one here to see us blow up anyway ;) */
            break;
          ob = liv;
        }
      case 'a' :
        if (!ob) {
          if (!sizeof(attacker_list))
            break;
          if (!attacker)
            attacker = attacker_list[random(sizeof(attacker_list))];
          ob = attacker;
        }
      case 'o' :
        if (!ob) {
          if (!on) {
            obs = all_inventory(environment());
            if (sizeof(obs)) {
              j = q = random(sizeof(obs));
              while (living(obs[q++]) && j != q)
                q = q % sizeof(obs);
              on = obs[q-1];
            }
          }
          ob = on;
        }
        switch (str[i][1..1000]) {
          case "name" :
            ret += (string)ob->query_name();
            add_dollar = 0;
            break;
          case "cname" :
            ret += (string)ob->query_cap_name();
            add_dollar = 0;
            break;
          case "gender" :
            ret += (string)ob->query_gender_string();
            add_dollar = 0;
            break;
          case "poss" :
            ret += (string)ob->query_possessive();
            add_dollar = 0;
            break;
          case "obj" :
            ret += (string)ob->query_objective();
            add_dollar = 0;
            break;
          case "gtitle" :
            ret += (string)ob->query_gender_title();
            add_dollar = 0;
            break;
          case "pronoun" :
            ret += (string)ob->query_pronoun();
            add_dollar = 0;
            break;
          default :
            if (add_dollar)
              ret += "$";
            ret += str[i];
            add_dollar = 1;
            break;
        }
        ob = 0;
        break;
      default :
        if (add_dollar)
          ret += "$";
        ret += str[i];
        add_dollar = 1;
        ob = 0;
        break;
    }
  if (ret[strlen(ret)-1] == '$')
    return ret[0..strlen(ret)-2];
  return ret;
}

void expand_mon_string(string str) {
  switch (str[0]) {
    case ':' :
      say(this_player()->query_cap_name()+" "+
            expand_string(str[1..1000],0,0)+"\n");
      break;
    case '\'' :
    case '"' :
      command(expand_string(str,0,0));
      break;
    case '@' :
      command(expand_string(str[1..10000],0,0));
      break;
    default :
      say(expand_string(str,0,0)+"\n");
      break;
  }
}
 
/* do healing here as well... */
void heart_beat() {
  object ob;
  int i, j;

  if (sizeof(attacker_list)) {
    for (i=0;i<sizeof(attack_spells);i+=3)
      if (random(100) < attack_spells[i])
        call_other(attack_spells[i+2][0], attack_spells[i+2][1]); 
    if (throw_out)
      for (i=0;i<sizeof(attacker_list);i++)
        if (attacker_list[i] && attacker_list[i]->query_hp() < throw_out[0] &&
            environment(attacker_list[i]) == environment() &&
            random(100) < throw_out[1]) {
          if (throw_out[2])
            tell_object(attacker_list[i], expand_string(throw_out[2],
                           0, attacker_list[i]));
          if (throw_out[3])
            say(expand_string(throw_out[3], 0, attacker_list[i]),
                     attacker_list[i]);
          attacker_list[i]->run_away(1);
        }
  } else
    if (hp == max_hp && gp == max_gp && !sizeof(achat_string) && !sizeof(chat_string))
      set_heart_beat(0);
  if (drunk_heart_beat(query_volume(0)))
    attack();
  do_spell_effects(attackee);
  remove_property("done follow");
  if (sizeof(attacker_list) && wimpy && (max_hp*wimpy/100 > hp) )
    run_away();
  if (sizeof(attacker_list)) {
    if ((random(1000) < achat_chance) && sizeof(achat_string[1]))
      if (sizeof(doing_story[1])) {
        expand_mon_string(doing_story[1][0]);
        doing_story[1] = doing_story[1][1..sizeof(doing_story)];
      } else {
        i = random(achat_string[0]+1);
        while ((i-=achat_string[1][j])>0)
          j += 2;
        if (pointerp(achat_string[1][j+1])) {
          doing_story[1] = achat_string[1][j+1];
          expand_mon_string(doing_story[1][0]);
          doing_story[1] = doing_story[1][1..sizeof(doing_story)];
        } else
          expand_mon_string(achat_string[1][j+1]);
      }
    if (attack_everyone) {
      ob = first_inventory(environment());
      while (ob) {
        ob->attack_by(this_object());
        ob = next_inventory(ob);
      }
    }
  } else {
    if (random(1000) < chat_chance && sizeof(chat_string[1]))
      if (sizeof(doing_story[0])) {
        expand_mon_string(doing_story[0][0]);
        doing_story[0] = doing_story[0][1..sizeof(doing_story)];
      } else {
        i = random(chat_string[0]+1);
        while ((i-=chat_string[1][j])>0)
          j += 2;
        if (pointerp(chat_string[1][j+1])) {
          doing_story[0] = chat_string[1][j+1];
          expand_mon_string(doing_story[0][0]);
          doing_story[0] = doing_story[0][1..sizeof(doing_story)];
        } else
          expand_mon_string(chat_string[1][j+1]);
      }
    if (!check_anyone_here())
      set_heart_beat(0);
  }
  catch(RACE_OB->monster_heart_beat(race, class, race_ob, guild_ob));
}

int rand_num(int no, int type) {
  int i, val;

  for (i=0;i<no;i++)
    val = random(type)+1;
  return val;
}

void set_random_stats(int no, int type) {
  set_str(rand_num(no, type));
  set_dex(rand_num(no, type));
  set_int(rand_num(no, type));
  set_con(rand_num(no, type));
  set_wis(rand_num(no, type));
}

void set_level(int i) {
/* sigh... need to some thing here at some point I suppose... */
/* well I did.  totaly boring huh? */
  if (i > 0)
    adjust_xp((i/3)*700);
  else if (!i)
    adjust_xp(700);
  else
    adjust_xp(700/(-i));
  hp = 10000;
  RACE_OB->set_level(i, race, class);
  command("init_race");
} /* set_level() */

int give_money(int base, int rand, string type) {
  if (!type)
    type = "copper";
  return adjust_money(base+random(rand), type);
}

void load_chat(int chance, string *c_s) {
  int i;
  for (i=0;i<sizeof(c_s);i+=2) {
    chat_string[1] += ({ c_s[i], c_s[i+1] });
    chat_string[0] += c_s[i];
  }
  chat_chance = chance;
}

void set_chat_chance(int i) { chat_chance = i; }
int query_chat_chance() { return chat_chance; }
void set_chat_string(string *chat) { chat_string = chat; }
string *query_chat_string() { return chat_string; }
void add_chat_string(mixed weight, mixed chat) {
  int i;
 
  if (pointerp(weight))
    for (i=0;i<sizeof(weight);i+=2)
      add_chat_string(weight[i], weight[i+1]);
  else
    if (member_array(chat, chat_string[1]) == -1) {
      chat_string[1] += ({ weight, chat });
      chat_string[0] += weight;
    }
}
 
void remove_chat_string(mixed chat) {
  int i;
 
  if (pointerp(chat))
    for (i=0;i<sizeof(chat);i++)
      remove_chat_string(chat[i]);
  else
    if ((i = member_array(chat, chat_string[1])) != -1) {
      chat_string[0] -= chat_string[1][i-1];
      chat_string[1] = delete(chat_string[1], i-1, 2);
    }
} 

void load_a_chat(int chance, string *c_s) {
  int i;
  for (i=0;i<sizeof(c_s);i+=2) {
    achat_string[1] += ({ c_s[i], c_s[i+1] });
    achat_string[0] += c_s[i];
  }
  achat_chance = chance;
}
 
void set_achat_chance(int i) { achat_chance = i; }
int query_achat_chance() { return achat_chance; }
void set_achat_string(string *chat) { achat_string = chat; }
string *query_achat_string() { return achat_string; }
void add_achat_string(mixed weight, mixed chat) {
  int i;
 
  if (pointerp(weight))
    for (i=0;i<sizeof(weight);i+=2)
      add_achat_string(weight[i], weight[i+1]);
  else
    if (member_array(chat, achat_string[1]) == -1) {
      achat_string[1] += ({ chat });
      achat_string[0] += weight;
    }
}
 
void remove_achat_string(mixed chat) {
  int i;
 
  if (pointerp(chat))
    for (i=0;i<sizeof(chat);i++)
      remove_achat_string(chat[i]);
  else
    if ((i = member_array(chat, achat_string[1])) != -1) {
      achat_string[0] -= achat_string[1][i-1];
      achat_string[1] = delete(achat_string[1], i-1, 1);
    }
}

void set_move_after(int after, int rand);

void add_move_zone(mixed zone) {
  int i;

  if (pointerp(zone))
    for (i=0;i<sizeof(zone);i++)
      add_move_zone(zone[i]);
  else if (member_array(zone, move_zones) != -1)
    return;
  else
    move_zones += ({ zone });
  if (!move_after)
    set_move_after(MOVE_DEFAULT[0], MOVE_DEFAULT[1]);
}

void set_move_zones(string *zones) {
  int i;

  for (i=0;i<sizeof(zones);i++)
    add_move_zone(zones[i]);
}

void remove_move_zone(mixed zone) {
  int i;

  if (pointerp(zone))
    for (i=0;i<sizeof(zone);i++)
      remove_move_zone(zone[i]);
  else {
    if ((i=member_array(zone, move_zones)) == -1)
      return ;
    move_zones = delete(move_zones, i, 1);
  }
}

string *query_move_zones() { return move_zones; } 

void set_move_after(int after, int rand) {
  move_after = ({ after, rand });
  remove_call_out("do_move_after");
  if (after != 0 && rand != 0)
    call_out("do_move_after", after+random(rand));
} /* set_move_after() */

mixed query_move_after() { return move_after; }

int add_enter_commands(mixed str) {
  if (stringp(str))
    enter_commands += ({ str });
  else if (pointerp(str))
    enter_commands += str;
  return 1;
}

string *query_enter_commands() { return enter_commands; }

int reset_enter_commands() {
  enter_commands = ({ });
}

move_player(string dir, string dest, mixed message, object followee,
                mixed enter) {
  int i, j;

  i = ::move_player(dir, dest, message, followee, enter);
  if (i) {
    for (j=0;j<sizeof(enter_commands);j++)
      init_command(enter_commands[j]);
    return i;
  }
  return 0;
}

void do_move_after(int bing) {
  mixed *direcs;
  int i, bong;
  string zone;

  if (!environment() || !bing && sizeof(attacker_list)) {
/* dont run away when someone is fighting you, very rude. */
    call_out("do_move_after", move_after[0]+random(move_after[1]));
    return ;
  } 
  direcs = (string *)environment()->query_dest_dir();
  while (!bong && sizeof(direcs)) {
    i = random(sizeof(direcs)/2)*2;
    bong = 0;
    if (bing > 1)
      catch(bong = (int)direcs[i+1]->query_property("no throw out"));
    catch(zone = (string)direcs[i+1]->query_zone());
    if (sizeof(move_zones) || bong)
      if (bong || (member_array(zone, move_zones) == -1)) {
        direcs = delete(direcs, i, 2);
        continue;
      }
    bong = command(direcs[i]);
    if (!bong)
      direcs = delete(direcs, i, 2);
  }
  if (!bing)
    call_out("do_move_after", move_after[0]+random(move_after[1]));
} /* do_move_after() */
 
void set_talk_string(mixed arr) { talk_string = arr; }
mixed query_talk_string() { return talk_string; }
void add_talk_string(string cat, mixed do_this) {
  talk_string += ({ cat, do_this });
}

void remove_talk_string(string cat) {
  int i;
 
  if ((i=member_array(cat, talk_string)) == -1)
    return ;
  talk_string = delete(talk_string, i, 2);
}

int in_talk;
 
void catch_talk(string str) {
  string s1, s2;
  int i;

  if (in_talk)
    return ;

  in_talk = 1;
  for (i=0;i<sizeof(talk_string);i+=2)
    if (sscanf(str, "%s"+talk_string[i]+"%s", s1, s2))
      if (stringp(talk_string[i+1]))
        call_other(this_object(), talk_string[i+1], str, s1, s2);
      else if (pointerp(talk_string[i+1]))
        call_other(talk_string[i+1][0], talk_string[i+1][1], str, s1, s2);
  in_talk = 0;
}

void event_fight_in_progress(object me, object him) {
  if (!join_fight_mess || !me || !him)
    return ;
  if (him == this_object() || me == this_object())
    return ;
  if (!join_fight_type) {
    if (member_array(me, attacker_list) == -1 &&
        member_array(him, attacker_list) == -1)
      say(join_fight_mess);
    if (!me->query_property("no attack"))
      attack_by(me);
    if (!him->query_property("no attack"))
      attack_by(him);
  }
  if (query_ip_number(me)) {
    if (member_array(me, attacker_list) == -1)
      say(join_fight_mess);
    attack_by(me);
  }
  if (query_ip_number(him)) {
    if (member_array(him, attacker_list) == -1)
      say(join_fight_mess);
    attack_by(him);
  }
}

void set_join_fights(string str) { join_fight_mess = str; }
query_join_fights() { return join_fight_mess; }

void set_join_fight_type(int i) { join_fight_type = i; }
int query_fight_type() { return join_fight_type; }

void set_al(int i) { alignment = i; }
int query_al() { return alignment; }
int query_align() { return alignment; }
void set_align(int i) { alignment = i; }
void set_alignment(int i) { alignment = i; }
int query_alignment() { return alignment; }

void event_exit(object me, string mess, mixed ob) {
  mixed *bing;
  int i;
  string zone;

  if (!move_after) /* we dont move :( */
    return ;
/* follow the suckers. */
  if (member_array(me, attacker_list) != -1) {
    bing = (mixed *)environment()->query_dest_dir();
    if ((i = member_array(ob, bing)) == -1) {
      if (!objectp(ob))
        ob = find_object(ob); /* arhghh must have teleported... scums. */
      else
        ob = file_name(ob);
      if ((i = member_array(ob, bing)) == -1)
        return ; /* lost cause */
    }
    zone = (string)ob->query_move_zone();
    if (move_zones && sizeof(move_zones))
      if (member_array(zone, move_zones) == -1) {
        return ; /* If we are suppose to wander in move_zones.. dont go
                  * where we are not supposed to... get stuck */
    }
    remove_call_out("do_follow_move");
    call_out("do_follow_move", 2+random(follow_speed), bing[i-1]);
  }
}

void do_follow_move(string dir) {
  command(dir);
}

int add_attack_spell(int chance, string name, mixed obj, string func) {
  int i;

  if ((i=member_array(name, attack_spells)) == -1)
    attack_spells += ({ chance, name, ({ obj, func }) });
  else {
    attack_spells[i-1] = chance;
    attack_spells[i+1] = ({ obj, func });
  }
  return 1;
}

int remove_attack_spell(string name) {
  int i;

  if ((i=member_array(name, attack_spells)) == -1)
    return 0;
  attack_spells = delete(attack_spells, i-1, 3);
  return 1;
}

mixed query_race_ob() { return race_ob; }
void set_race_ob(mixed r) { race_ob = r; }

mixed query_guild_ob() { return guild_ob; }
void set_guild_ob(mixed g) { guild_ob = g; }
int query_follow_speed() { return follow_speed; }
void set_follow_speed(int f) { follow_speed = f; }

int query_aggressive() { return aggressive; }
void set_aggressive(int a) { aggressive = a; }

int query_level() {
  if (!guild_ob) return 1;
  return (int)guild_ob->query_level(this_object());
}

int do_teach(string str) {
  string bing;
  object *obs;

  notify_fail("Syntax: teach <spell/command> to <person>\n");
  if (!str || str == "")
    return 0;
  if (sscanf(str, "%s to %s", str, bing) != 2)
    return 0;
  obs = find_match(bing, environment());
  if (!sizeof(obs)) {
    notify_fail("You must teach someone.\n");
    return 0;
  }
  if (!command_teach(obs, str) && !spell_teach(obs, str) &&
      !teach_skill(obs, str)) {
    notify_fail("You do not know the spell or command "+str+".\n");
    return 0;
  }
  return 1;
}

mixed *stats() {
  mixed *bing;
  int i;

  bing = ({ });
  for (i=0;i<sizeof(move_zones);i++)
    bing += ({ ({ "move zone "+i, move_zones[i] }) });
   if (!query_move_after())
     return ::stats() + ({
        ({ "guild", query_guild() }),
        ({ "guild ob", query_guild_ob() }),
        ({ "race", query_race() }),
        ({ "race ob", query_race_ob() }),
        ({ "join_fights", query_join_fights() }),
        ({ "follow speed", query_follow_speed() }),
        ({ "level", query_level() }),
        ({ "chat chance", query_chat_chance() }),
        ({ "achat chance", query_achat_chance() }),
        ({ "alignment", query_al() }),
         ({ "aggressive", query_aggressive() }),
    }) + bing;
   return ::stats() + ({
      ({ "race", query_race() }),
      ({ "race ob", query_race_ob() }),
      ({ "guild", query_guild() }),
      ({ "guild ob", query_guild_ob() }),
      ({ "join fights", query_join_fights() }),
      ({ "follow_speed", query_follow_speed() }),
      ({ "level", query_level() }),
      ({ "chat chance", query_chat_chance() }),
      ({ "achat chance", query_achat_chance() }),
      ({ "alignment", query_al() }),
      ({ "aggressive", query_aggressive() }),
      ({ "move after-fix", query_move_after()[0] }),
      ({ "move after-rand", query_move_after()[1] }),
   }) + bing;
}

string expand_nickname(string str) { return str; }
