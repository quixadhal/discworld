inherit "std/object";
inherit "/std/weapon_logic";
inherit "/std/basic/enchant";
inherit "/std/basic/condition";
#include "weapon.h"
#include "money.h"

/* ok.... now we start thinking about the strange things...
 *   We have several points we want to implement... 
 *   1)  A much wider range of damages.
 *   2)  A "To hit" and "damage" rolls being seperate things
 *          But are still related.  ie a good hit will do more damage
 *  Implementation...
 *    Ok, here goes.   The being hit players ac is taken based on the
 *    attack type, the ac will be returned as a base (not randomised)
 *    and a randomised amount...  This will be used to calculate the
 *    cute number we need.  Once we have the number we create the
 *    "To hit" role for the weapon.  Subtracting of this from our 
 *    ac number tells us weather or not we hit...  If we have hit
 *    We take the amount we hit by (after subtracting off the
 *    "To hit bonus") and add it too the extra damge that is done
 *    All this is kept in an array, there can be more than one
 *    attack in the array.  They can be connected together in
 *    several ways, 1) follow on after attack one did more that x
 *    points of damage.  2) have a percentage chance of working
 *    each attack.
 *    A standard set of attacks are defined in /std/weapon_handler
 *    please see that file for more details
 */
static object wielder;
string *un_modifyable;
string *wield_func;
int damage_chance;

void create() {
  damage_chance = 5;
  max_cond = 1;
  un_modifyable = ({ });
  weapon_logic::create();
  condition::create();
  object::create();
  add_alias("weapon");
  add_plural("weapons");
} /* create() */

string short(int dark) {
  string str;

  if (enchanted > 0)
    str = " (enchanted)";
  else
    str = "";
  return ::short(dark)+str;
} /* short() */

void set_wield_func(string func, string ob) {
  if (!func)
    wield_func = 0;
  else
    wield_func = ({ func, ob });
} /* set_wield_func() */

int query_weapon() { return 1; }

string long(string s, int dark) {
  return ::long(s, dark) + enchant_string() + cond_string();
} /* long() */
 
void new_weapon(int c) {
  set_cond(c);
  set_max_cond(c);
  set_lowest_cond(c);
} /* new_weapon() */

void set_damage_chance(int i) { damage_chance = i; } 
int query_damage_chance() { return damage_chance; }

int set_wield(object ob) {
  mixed ret;

  ret = 1;
  if (wield_func)
    if (wielder && ob) {
      if (wielder != ob) {
/* Unwield first.... */
        if (call_other(wield_func[1], wield_func[0], 0))
/* Rewield by new person */
          ret = (mixed)call_other(wield_func[1], wield_func[0], ob);
        else
          return 0;
     }
    } else if (wielder != ob)
      if (!ob) {
        ret = (mixed)call_other(wield_func[1], wield_func[0], 0);
      } else {
        ret = (mixed)call_other(wield_func[1], wield_func[0], ob);
      }
  if (!ret)
    return 0;
  wielder = ob;
  return ret;
} /* set_wield() */

object query_wielded() { return wielder; }

int drop() {
  wielder = 0;
  return ::drop();
} /* drop() */

move(ob, str1, str) {
  if (wielder) 
    if (ob != wielder) {
      wielder->unwield_weapon(this_object());
      wielder = 0;
      if (wield_func)
        call_other(wield_func[1], wield_func[0], 0);
    }
  return ::move(ob, str1, str);
} /* move() */

int modify_damage(int val, string name) {
  val += (val*enchanted)/100+enchanted;
  if (member_array(name, un_modifyable) != -1)
    return val;
  val = val*query_cond()/query_max_cond();
  return val;
} /* modify_damage() */

void hit_weapon(int dam, string name) {
  if (member_array(name, un_modifyable) != -1)
    return ;
  if (random(100) >= damage_chance)
    return ;
  adjust_cond(-dam);
} /* hit_weapon() */

/* immune to condtion loss */
int add_immune(string name) {
  if (member_array(name, un_modifyable) != -1)
    return 0;
  un_modifyable += ({ name });
  return 1;
} /* add_immune() */

int remove_immune(string name) {
  int i;
  if ((i = member_array(name, un_modifyable)) == -1)
    return 0;
  un_modifyable = delete(un_modifyable, i, 1);
  return 1;
} /* remove_immune() */

int query_value() {
  int val;

  val = ::query_value();
  return (val*10)/100 + (val*query_cond()*90)/(100*query_max_cond());
} /* query_value() */

mixed *query_money_array() {
  return (mixed *)MONEY_HAND->create_money_array(query_value());
} /* query_money_array() */

int query_money(string type) {
  int i;
  mixed *m_a;

  m_a = (mixed *)MONEY_HAND->create_money_array(query_value());
  if ((i=member_array(type, m_a)) == -1)
    return 0;
  return m_a[i+1];
} /* query_money() */

mixed *stats() {
  return object::stats() + ({
      ({ "condition", cond }),
      ({ "Max_cond", max_cond }),
      ({ "Lowest cond", lowest_cond }),
      ({ "Damage Chance", damage_chance }),
      ({ "Enchant", enchanted }),
      }) + weapon_logic::stats();
} /* stats() */

/* called at condition <= 0 */
void break_me() {
  if(wielder) {
    wielder->unwield_weapon(this_object());
    tell_object(wielder, "Your " + short(0) + " breaks!\n");
    tell_room(environment(wielder), wielder->query_cap_name() + "'s " +
      short(0) + " breaks!\n", wielder);
  }
  ::break_me();
} /* break_me() */

mapping int_query_static_auto_load() {
  mapping tmp;

  tmp = ::int_query_static_auto_load();
  return ([ "::" : tmp,
    "attack name" : attack_name, "attack data" : attack_data, "skill" : skill,
    "max cond" : max_cond,
          ]);
} /* int_query_static_auto_load() */

mapping query_dynamic_auto_load() {
  return ([
     "cond" : cond,
     "enchant" : enchanted,
     "lowest cond" : lowest_cond,
     "wielded" : !(!wielder),
          ]);
} /* query_dynamic_auto_load() */

void init_static_arg(mapping args) {
  if (!mappingp(args)) return ;
  if (args["::"])
    ::init_static_arg(args["::"]);
  if (args["attack name"])
    attack_name = args["attack name"];
  if (args["attack data"])
    attack_data = args["attack data"];
  if (args["skill"])
    skill = args["skill"];
  if (!undefinedp(args["max cond"]))
    max_cond = args["max cond"];
} /* init_static_arg() */

void init_dynamic_arg(mapping map) {
  if (map["::"])
    ::init_dynamic_arg(map["::"]);
  cond = map["cond"];
  lowest_cond = map["lowest cond"];
  enchanted = map["enchant"];
  if (map["wielded"])
   call_out("player_wield",0);
} /* init_dynamic_arg() */

void player_wield() {
  environment()->wield((sizeof(adjectives)?implode(adjectives, " ")+" ":"")
                       +name, 1);
} /* player_wield() */

mapping query_static_auto_load() {
  if (file_name(this_object())[0..10] == "/obj/weapon")
    return int_query_static_auto_load();
  return ([ ]);
} /* query_static_auto_load() */

void init_arg(mixed *bing) {
  if (sizeof(bing) < 10)
    return ;
  ::init_arg(bing[1..bing[0]]);
  bing = bing[bing[0]+1..10000];
  attack_name = bing[0];
  attack_data = bing[1];
  skill = bing[2];
  cond = bing[4];
  max_cond = bing[3];
  lowest_cond = bing[5];
  enchanted = bing[6];
} /* init_arg() */
