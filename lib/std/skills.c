/* The skill conroller object....
 */
#include "skills.h"
#define SKILL_BONUS 0
 

mapping stat_bonus; 
mixed *skills,
      *reg_skills;

mixed *flatten(mixed *arr, string bit);
mixed *add_skill_rec(mixed *skill, string *path);
mixed *del_skill_rec(mixed *skill, string *path);
 
void create() {
  reg_skills = flatten(STD_SKILLS, "skill");
  skills = STD_SKILLS;
  stat_bonus = ([
    "fighting" : ({ "S" }),
    "magic" : ({ "I" }),
    "faith" : ({ "W" }),
    "other" : ({ "D" }),
    "fighting.combat.melee" : ({ "SSD", }),
    "fighting.combat.range.bow" : ({ "DDS", }),
    "fighting.combat.range.thrown" : ({ "SD", }),
    "fighting.points" : ({ "C" }),
    "fighting.dodging" : ({ "D" }),
  ]);
}

mixed *query_reg_skills() { return reg_skills; }
mixed *query_skills() { return skills; }
 
/* not sure... do we decied that anyone that querys a skill knows
 * what they are talking about? Or just  assmume they dont...
 * For now I will assume they know
 */
string *query_skill_path(string skill) {
  if (member_array(skill, reg_skills) == -1)
    return 0;
  return explode(skill, ".");
}
 
int register_skill(string skill) {
  if (member_array(skill, reg_skills) != -1)
    return 0;
  reg_skills += ({ skill });
  skills = add_skill_rec(skills, explode(skill, "."));
  return 1;
}
 
int add_skill(string skill) {
  if (member_array(skill, reg_skills) != -1)
    return 0;
  reg_skills += ({ skill });
  skills = add_skill_rec(skills, explode(skill, "."));
  return 1;
}
 
int remove_skill(string skill) {
  int i;
 
  if ((i=member_array(skill, reg_skills)) == -1)
    return 0;
  reg_skills = delete(reg_skills, i, 1);
  skills = del_skill_rec(skills, explode(skill, "."));
  return 1;
}
 
int deregister_skill(string skill) {
  int i;
 
  if ((i=member_array(skill, reg_skills)) == -1)
    return 0;
  reg_skills = delete(reg_skills, i, 1);
  skills = del_skill_rec(skills, explode(skill, "."));
  return 1;
}
 
string query_skill_stat(string skill) {
  int i, j;
  string *bit, s;
 
  bit = explode(skill, ".");
  i = sizeof(bit);
  while (i>=0)
    if (stat_bonus[(s=implode(bit[0..i],"."))])
      return stat_bonus[s][SKILL_BONUS];
    else
      i--;
  return "";
}
 
int add_stat_bonus(string skill, string bonus) {
  if (stat_bonus[skill])
    return 0;
  stat_bonus[skill] = ({ bonus });
  return 1;
}
 
int remove_stat_bonus(string skill) {
  int i;

  if (!stat_bonus[skill])
    return 0;
  stat_bonus = m_delete(stat_bonus, skill); 
  return 1;
}

mixed *flatten(mixed *arr, string bit) {
  int i;
  mixed *ret;

  if (!bit)
    bit = "skill";

  ret = ({ });
  for (i=0;i<sizeof(arr);i+=SKILL_ARR_SIZE)
    if (sizeof(arr[i+SKILL_BIT]) == 0)
      ret += ({ extract(bit+"."+arr[i], 6, 100) });
    else
      ret += ({ (bit+"."+arr[i])[6..100] }) +
              flatten(arr[i+SKILL_BIT], bit+"."+arr[i]);
  return ret;
}

int query_skill_cost_rec(mixed *arr) {
  int i, tmp;

  if (!sizeof(arr))
    return 1;
  for (i=0;i<sizeof(arr);i+=SKILL_ARR_SIZE)
    tmp += query_skill_cost_rec(arr[i+SKILL_BIT]);
  return tmp;
}

int query_skill_cost(string str) {
  int i;
  mixed *arr;
  string *path;

  path = explode(str, ".");
  arr = skills;
  while (sizeof(path))
    if ((i=member_array(path[0], arr)) == -1)
      return -1;
    else {
      path = path[1..sizeof(path)];
      arr = arr[i+SKILL_BIT];
    }
  return query_skill_cost_rec(arr);
}

mixed *add_skill_rec(mixed *arr, string *path) {
  int i;
  if (!sizeof(path))
    return arr;
  if ((i=member_array(path[0],arr)) == -1) {
    i = sizeof(arr);
    arr += ({ path[0], 0, 0, ({ }) });
  }
  arr[i+SKILL_BIT] = add_skill_rec(arr[i+SKILL_BIT], path[1..sizeof(path)]);
  return arr;
}

mixed *del_skill_rec(mixed *arr, string *path) {
  int i;

  if (!sizeof(path))
    return arr;
  if ((i=member_array(path[0], arr)) == -1)
    return arr;
  if (sizeof(path) == 1)
    if (sizeof(arr[i+SKILL_BIT]))
      return arr;
    else
      return delete(arr, i, SKILL_ARR_SIZE);
  arr[i+SKILL_BIT] = del_skill_rec(arr[i+SKILL_BIT], path[1..sizeof(path)]);
  return arr;
}

string query_skill(string *bits) {
  mixed *arr;
  int i;
  string path, s1, s2;

  path = "";
  arr = skills;
  for (i=0;i<sizeof(arr);i+=SKILL_ARR_SIZE) {
    if (sscanf(arr[i], bits[0]+"%s", s1) == 1) {
      path += "."+arr[i];
      arr = arr[i+SKILL_BIT];
      i = -SKILL_ARR_SIZE;
      bits = bits[1..sizeof(bits)];
      if (!sizeof(bits))
        return path[1..strlen(path)];
    }
  }
  if (i<sizeof(arr))
    return path[1..strlen(path)];
  return 0;
}
