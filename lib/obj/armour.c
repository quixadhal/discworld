/* Ok armour...
 * This will be nasty...
 */
inherit "/std/basic/enchant";
inherit "/std/object";
inherit "/std/armour_logic";
inherit "/std/basic/condition";
#include "money.h"
 
int damage_chance,
    max_ac;
string type;
string *immune_to;
string *wear_remove_func;
/* this is used to determine what attacks dont damage the item...
 * ie magical rings will still be affected by fire etc
 * But would be immune to sharp and blunt attacks...
 */
object worn_by;
 
void create() {
  damage_chance = 100;
  condition::create();
  armour_logic::create();
  immune_to = ({ });
  object::create();
  add_alias("armour");
  add_plural("armours");
} /* create() */
 
/*
 * Called when the armour is worn or removed.  Allow syou to
 * A do mindlessly silly things.  Good huh?
 */
void set_wear_remove_func(string ob, string func) {
  wear_remove_func = ({ ob, func });
} /* set_wear_remove_func() */
 
int query_armour() { return 1; }
 
string long(string s, int dark) {
  return ::long(s, dark) + enchant_string() + cond_string();
} /* long() */
 
/* ok this gets our ac... ;) */
int query_ac(string type, int dam) {
  int val,
      i;
  val = ::query_ac(type, dam);
  if (val > 100)
    val = max_ac;
  else
    val = val*max_ac/100;
  val += (val*enchanted)/100 + enchanted/2;
  if (member_array(type, immune_to) != -1)
    return (val*cond)/max_cond;
  if (!dam || random(100) >= damage_chance)
    return (val*cond)/max_cond;
  adjust_cond(-dam);
  return (val*cond)/max_cond;
} /* query_ac() */
 
void setup_armour(int c) {
  set_cond(c);
  set_max_cond(c);
  set_lowest_cond(c);
} /* setup_armour() */
 
void set_damage_chance(int i) { damage_chance = i; }
 
int set_worn_by(object ob) {
  if (wear_remove_func && !ob != !worn_by)
    if (!ob)
      call_other(wear_remove_func[0], wear_remove_func[1], 0);
    else
      call_other(wear_remove_func[0], wear_remove_func[1], 1);
  worn_by = ob;
  return 1;
} /* set_worn_by() */
 
int query_damage_chance() { return damage_chance; }
object query_worn_by() { return worn_by; }
 
int query_worn() { return worn_by != 0; }
string *query_immune_to() { return immune_to; }
 
void add_immune_to(mixed arr) {
  int i;
 
  if (pointerp(arr))
    for (i=0;i<sizeof(arr);i++)
      add_immune_to(arr[i]);
  else
    if (member_array(arr, immune_to) == -1)
      immune_to += ({ arr });
} /* add_immune_to() */ 
 
void remove_immune_to(mixed arr) {
  int i;
 
  if (pointerp(arr))
    for (i=0;i<sizeof(arr);i++)
      remove_immune_to(arr[i]);
  else
    if ((i=member_array(arr, immune_to)) != -1)
      immune_to = delete(immune_to, i, 1);
} /* remove_immune_to() */
 
move(mixed dest, mixed messout, mixed messin) {
  if (dest != worn_by && worn_by) {
    worn_by->remove_armour(this_object());
    if (wear_remove_func)
      call_other(wear_remove_func[0], wear_remove_func[1], 0);
    worn_by = 0;
  }
  return ::move(dest, messout, messin);
} /* move() */
 
void set_type(string str) {
  if (type)
    remove_alias(type);
  switch (str) {
    case "armour" :
      max_ac = 40;
      break;
    case "shield" :
      max_ac = 20;
      break;
    case "boot" :
      max_ac = 10;
      break;
    case "helmet" :
      max_ac = 10;
      break;
    case "cloak" :
      max_ac = 10;
      break;
    case "glove" :
      max_ac = 10;
      break;
    case "ring" :
      max_ac = 10;
      break;
  }
  add_alias(str);
  type = str;
} /* set_type() */
 
string query_type() { return type; }
 
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
  mixed *ret;
  int i;
 
  ret = ({ });
  for (i=0;i<sizeof(immune_to);i++)
    ret +=  ({ ({ "Immune", immune_to[i] }) }); 
  return ::stats() + ({
     ({ "Condition", cond }),
     ({ "Max cond.", max_cond }),
     ({ "Lowest cond.", lowest_cond }),
     ({ "Damage chance", damage_chance }),
     ({ "Enchant", enchanted }),
  }) + ret + armour_logic::stats();
} /* stats() */
 
void dest_me() {
  if (worn_by) {
    worn_by->remove_armour(this_object());
    if (wear_remove_func)
      call_other(wear_remove_func[0], wear_remove_func[1], 0);
  }
  ::dest_me();
} /* dest_me() */
 
mapping int_query_static_auto_load() {
  mapping tmp;
 
  tmp = ::int_query_static_auto_load();
  return ([ "::" : tmp,
    "ac" : ac, "armour types" : armour_types, "max cond" : max_cond,
    "damage chance" : damage_chance, "type" : type, "immune" :immune_to,
    "wear remove func" : wear_remove_func,
         ]);
} /* int_query_static_auto_load() */

mapping query_dynamic_auto_load() {
  return ([
          "::" : ::query_dynamic_auto_load(),
          "lowest cond" : lowest_cond,
          "enchant" : enchanted,
          "worn" : !(!worn_by),
          "cond" : cond,
          ]);
} /* query_dynamic_auto_load() */

void init_dynamic_arg(mapping map) {
  if (map["::"])
    ::init_dynamic_arg(map["::"]);
  lowest_cond = map["lowest cond"];
  cond = map["cond"];
  enchanted = map["enchant"];
  if (map["worn"])
    call_out("player_wear",0);
} /* init_dynamic_arg() */
 
void init_static_arg(mapping args) {
  if (!mappingp(args)) return ;
  if (args["::"])
    ::init_static_arg(args["::"]);
  if (args["ac"])
    ac = args["ac"];
  if (args["armour types"])
    armour_types = args["armour types"];
  if (!undefinedp(args["max cond"]))
    max_cond = args["max cond"];
  if (!undefinedp(args["damage chance"]))
    damage_chance = args["damage chance"];
  if (args["immune"])
    immune_to = args["immune"];
  if (args["type"])
    set_type(args["type"]);
  if (args["wear remove func"])
    wear_remove_func = args["wear remove func"];
} /* int_init_arg() */
 
void player_wear() {
  environment()->wear_armour(this_object());
/*
(sizeof(adjectives)?implode(adjectives, " ")+" ":"")+
                      name, 1);
 */
} /* player_wear() */
 
mapping query_static_auto_load() {
  if (!name)  return 0;
  if (file_name(this_object())[0..10] == "/obj/armour")
    return int_query_static_auto_load();
  return ([ ]);
} /* query_static_auto_load() */

void init_arg(mixed *bing) {
  if (sizeof(bing) < 7)
    return ;
  ::init_arg(bing[1..bing[1]]);
  bing = bing[bing[0]+1..10000];
  ac = bing[0];
  armour_types = bing[1];
  cond = bing[2];
  max_cond = bing[3];
  lowest_cond = bing[4];
  enchanted = bing[5];
  damage_chance = bing[6];
  type = bing[7];
  immune_to = bing[8];
} /* init_arg() */
