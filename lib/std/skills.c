/**
 * This is the skills handler.  It controls all the skills currently in
 * the game and keeps track of their various properties.
 * @author Pinkfish
 */
#include <skills.h>
#include <language.h>

#define SKILL_BONUS 0

private mapping _stat_bonus;
private mixed *_skills;
private mapping _reg_skills;
private mapping _skill_tree;
private mapping _immediate_children;
private mapping _only_leaf;
private mapping _not_allowed_to_teach;
private mapping _only_show_if_non_zero;
private mapping _no_bonus;
private mapping _ignore_bits;

private mapping flatten(mixed *arr, string bit);
mixed *add_skill_rec(mixed *skill, string *path);
mixed *del_skill_rec(mixed *skill, string *path);
int query_skill_cost_int(string str);

void create() {
  string *languages, *lang_tree;
  int i;
  mixed *rabbit;
  string skill_written;
  string skill_spoken;
  string *skill_bits;

  _skills = STD_SKILLS;
  _skill_tree = ([ ]);
  _immediate_children = ([ ]);
  _not_allowed_to_teach = ([ ]);
  _only_show_if_non_zero = ([ ]);
  _no_bonus = ([ ]);
  lang_tree = ({ });
  languages = LANGUAGE_HAND->query_languages();
  for (i=0;i<sizeof(languages);i++) {
     rabbit = ({ });
     if (LANGUAGE_HAND->query_language_spoken(languages[i])) {
        rabbit += ({ SPOKEN_SKILL, 0, 0, ({ }), });
     }
     if (LANGUAGE_HAND->query_language_written(languages[i]) ||
         LANGUAGE_HAND->query_language_magic(languages[i])) {
        rabbit += ({ WRITTEN_SKILL, 0, 0, ({ }) });
     }
     skill_written = LANGUAGE_HAND->query_language_written_skill(languages[i]);
     skill_spoken = LANGUAGE_HAND->query_language_spoken_skill(languages[i]);
     languages[i] = replace(languages[i], " ", "_");
     lang_tree += ({ languages[i], 0, 0, rabbit });
     _not_allowed_to_teach[LANGUAGE_SKILL_START + languages[i]] = 1;
     _not_allowed_to_teach[skill_spoken] = 1;
     _not_allowed_to_teach[skill_written] = 1;
     _only_show_if_non_zero[skill_spoken] = 1;
     _only_show_if_non_zero[skill_written] = 1;
     _no_bonus[skill_spoken] = 1;
     _no_bonus[skill_written] = 1;
  }

  /* We make an assumption here that the skill is only two levels deep...  */
  skill_bits = explode(LANGUAGE_SKILL_START, ".");
  _not_allowed_to_teach["other.language"] = 1;
  i = member_array(skill_bits[0], _skills);
  if (i != -1) {
     _skills[i+SKILL_BIT] += ({ skill_bits[1], 0, 0, lang_tree });
  }
  _only_leaf = ([ "other" : 1 ]);
  _ignore_bits = ([ "crafts" : 1 ]);
  _reg_skills = flatten( _skills, "" );
  _stat_bonus = ([
"covert"                            : ({ "DDDII" }),
  "covert.stealth"                  : ({ "DDDIS" }),
  "covert.hiding"                   : ({ "DDIIS" }),
  "covert.lockpick"                 : ({ "DDDDI" }),
  "covert.manipulation"             : ({ "DDISS" }),
  "covert.casing"                   : ({ "DIIWW" }),
  "covert.items"                    : ({ "DIIII" }),
  "covert.points"                   : ({ "DDIIC" }),

"crafts"                            : ({ "DDIIW" }),
  "crafts.smithing"                 : ({ "DDIIS" }),
  "crafts.mining"                   : ({ "DIISS" }),
  "crafts.hunting"                  : ({ "DDIII" }),
  "crafts.carpentry"                : ({ "DDIIS" }),
  "crafts.pottery"                  : ({ "DDDII" }),
  "crafts.materials"                : ({ "DDIIS" }),
  "crafts.husbandry"                : ({ "IIIWW" }),
  "crafts.culinary"                 : ({ "DDIII" }),
  "crafts.arts"                     : ({ "DIIII" }),
  "crafts.music"                    : ({ "DIIII" }),
  "crafts.points"                   : ({ "DDIIW" }),

"faith"                             : ({ "ISWWW" }),
  "faith.rituals.offensive"         : ({ "ISSWW" }),
  "faith.rituals.defensive"         : ({ "IDDWW" }),
  "faith.rituals.curing"            : ({ "ICCWW" }),
  "faith.rituals.misc"              : ({ "IIWWW" }),
  "faith.items"                     : ({ "IIDWW" }),
  "faith.points"                    : ({ "IICWW" }),

"fighting"                          : ({ "DDSSI" }),
  "fighting.combat.melee"           : ({ "DSSSW" }),
  "fighting.combat.melee.sharp"     : ({ "DDSSS" }),
  "fighting.combat.melee.pierce"    : ({ "DDDSS" }),
  "fighting.combat.melee.blunt"     : ({ "DSSSS" }),
  "fighting.combat.melee.unarmed"   : ({ "DDDSW" }),
  "fighting.combat.range"           : ({ "DDDSS" }),
  "fighting.combat.parry"           : ({ "DDSSW" }),
  "fighting.combat.dodging"         : ({ "DDDSW" }),
  "fighting.combat.special.weapon"  : ({ "SDIII" }),
  "fighting.combat.special.unarmed" : ({ "DDIII" }),
  "fighting.combat.special.tactics" : ({ "WWIII" }),
  "fighting.points"                 : ({ "DSSCC" }),

"magic"                             : ({ "IIIDW" }),
  "magic.spells"                    : ({ "IIDWW" }),
  "magic.spells.offensive"          : ({ "WSSII" }),
  "magic.spells.defensive"          : ({ "WCCII" }),
  "magic.spells.misc"               : ({ "WDDII" }),
  "magic.spells.special"            : ({ "WWWII" }),
  "magic.methods.elemental"         : ({ "IICCC" }),
  "magic.methods.mental"            : ({ "IIIII" }),
  "magic.methods.spiritual"         : ({ "IIWWW" }),
  "magic.methods.physical"          : ({ "IIDDD" }),
  "magic.items"                     : ({ "IIDWW" }),
  "magic.items.held"                : ({ "IIDWW" }),
  "magic.points"                    : ({ "IISWW" }),

"other"                             : ({ "DDISS" }),
   "other.trading"                  : ({ "IIIIW" }),
   "other.movement"                 : ({ "CCDDS" }),
   "other.acrobatics"               : ({ "CDDSS" }),
   "other.evaluating"               : ({ "IIIIW" }),
   "other.perception"               : ({ "IIWWW" }),
   "other.direction"                : ({ "DDIIW" }),
   "other.health"                   : ({ "CCCCS" }),
   "other.points"                   : ({ "CDISW" }),
   "other.language"                 : ({ "IIIWW" }),
   "other.culture"                  : ({ "IIIWW" }),
  ]);
}

/**
 * This method returns the all of the skills with their children and the
 * number of children in a nice mapping.  The key of the mapping is the name
 * of the skill, the value is an array of two elements.  The first is an
 * array of all the children and the second is the number of children
 * of the skill.
 * @return the mapping as described above
 */
mapping query_reg_skills() { return _reg_skills; }
/**
 * This method returns the raw unfluffed skill array.  This is the same
 * as the skill array defined in /include/skills.h plus the language skills
 * and other dynamic skills added.
 * @return the skills array
 * @see /include/skills.h
 */
mixed *query_skills() { return _skills; }

/**
 * This method returns the path to skill.  This is essentially the skill
 * name broken up on '.'s and returned as an array.
 * @return the skill path, or 0 if the skill does not exist
 */
string *query_skill_path(string skill) {
   if (skill[0] == '.') {
      skill = skill[1..1090];
   }
   if (!_reg_skills[skill]) {
      return 0;
   }
   return explode(skill, ".");
} /* query_skill_path() */

/**
 * This method returns the skill and all of its parents leading up to
 * it.
 * @example
 * query_skill_tree("magic.methods.physical.binding")
 * Returns: ({
 *  "magic.methods.physical.binding",
 *  "magic.methods.physical",
 *  "magic.methods",
 *  "magic"
 * })
 * @param skill the skill to get the tree of
 * @return the skill tree of the skill
 */
string *query_skill_tree(string skill) {
   return _skill_tree[skill];
} /* query_skill_tree() */

/**
 * This method returns the stats that effect the skill.
 * @param skill the skill to check
 * @return the stats that effect the skill
 */
string query_skill_stat(string skill) {
   int i;
   string *bit, s;

   bit = explode(skill, ".");
   i = sizeof(bit);
   while (i >= 0) {
      s = implode(bit[0..i], ".");
      if (_stat_bonus[s])
         return _stat_bonus[s][SKILL_BONUS];
      else
         i--;
   }
   return "";
} /* query_skill_stat() */

private int add_stat_bonus(string skill, string bonus) {
   if (_stat_bonus[skill]) {
      return 0;
   }
   _stat_bonus[skill] = ({ bonus });
   return 1;
} /* add_stat_bonus() */

private int remove_stat_bonus(string skill) {
   if (!_stat_bonus[skill]) {
      return 0;
   }
   _stat_bonus = m_delete(_stat_bonus, skill);
   return 1;
} /* remove_stat_bonus() */

/**
 * This method returns the children of the specified skill chunk.
 * You need to pass into this array a skill chunk as taken from the
 * skill array.  It is recursive and returns all the internal children
 * as well.
 * @param arr the skill chunk
 * @param path the skill bit leading up to this section
 * @return the children of the skill chunk
 */
string *query_children(mixed *arr, string path) {
   string *ret;
   int i;

   ret = ({ });
   for (i = 0; i < sizeof(arr); i += SKILL_ARR_SIZE) {
      ret += ({ path+arr[i] }) +
           query_children(arr[i+SKILL_BIT], path+arr[i]+".");
   }
   return ret;
} /* query_children() */

/**
 * This method returns the immediate children of the specified skill.
 * @param skill the skill to get the children of
 * @return the immediate children of the skill
 */
string *query_immediate_children(string skill) {
   return _immediate_children[skill];
} /* query_immediate_childen() */

/**
 * This method returns all the children of the specified skill.
 * This does a deep children finding exercise, returing all the children
 * as it goes lower and lower into the array.
 * @param skill the skill to get the children of
 * @return all the children
 * @see query_related_skills()
 */
string *query_all_children(string skill) {
   if (_reg_skills[skill]) {
      return _reg_skills[skill][0];
   }
   return 0;
} /* query_all_children() */

private string *query_imm_children(mixed *arr, string path) {
   string *ret;
   int i;

   ret = ({ });
   for (i = 0; i < sizeof(arr); i+= SKILL_ARR_SIZE) {
      ret += ({ path+arr[i] });
   }
   return ret;
} /* query_imm_children() */

private string *create_skill_tree(string skill) {
   string *ret, *bits;
   int i;

   ret = ({ });
   bits = explode(skill, ".");
   /* For the only leaf heirachy, only count the upper level... */
   if (_only_leaf[bits[0]]) {
      return ({ skill });
   }
   for (i = sizeof(bits) - 1; i >= 0; i--) {
      ret += ({ implode(bits[0..i], ".") });
   }
   return ret;
} /* create_skill_tree() */

private mapping flatten(mixed *arr, string bit) {
   int i;
   mapping ret;

   if (!bit) {
      bit = "";
   }
   ret = ([ ]);
   for (i = 0; i < sizeof(arr); i += SKILL_ARR_SIZE) {
      reset_eval_cost();
      if (sizeof(arr[i+SKILL_BIT]) == 0) {
         ret[(bit+arr[i])] = ({ ({ bit+arr[i] }), 1 });
      } else {
         ret[bit + arr[i]] = ({ query_children(arr[i+SKILL_BIT], bit+arr[i]+".") +
                              ({ bit + arr[i] }),
                              query_skill_cost_int(bit+arr[i]) });
         ret += flatten(arr[i+SKILL_BIT], bit+arr[i]+".");
      }
      _skill_tree[(bit+arr[i])] = create_skill_tree(bit+arr[i]);
      _immediate_children[(bit+arr[i])] = query_imm_children(arr[i+SKILL_BIT],bit+arr[i]+".");
   }
   return ret;
} /* flatten() */


private int query_skill_cost_rec(mixed *arr) {
   int i, tmp;

   if (!sizeof(arr)) {
      return 1;
   }
   for (i=0;i<sizeof(arr);i+=SKILL_ARR_SIZE) {
      tmp += query_skill_cost_rec(arr[i+SKILL_BIT]);
   }
   return tmp;
} /* query_skill_cost_rec() */

private int query_skill_cost_int(string str) {
   int i;
   mixed *arr;
   string *path;

   path = explode(str, ".");
   arr = _skills;
   while (sizeof(path)) {
      i = member_array(path[0], arr);
      if (i == -1) {
         return -1;
      } else {
         path = path[1..sizeof(path)];
         arr = arr[i+SKILL_BIT];
      }
   }
   return query_skill_cost_rec(arr);
} /* query_skill_cost_int() */

/**
 * This method also returns all the children of a skill.  It preforms
 * exactly the same action as query_all_children()
 * @param skill the skill to get the children of
 * @return the children of the skill
 */
string *query_related_skills(string skill) {
   if (_reg_skills[skill]) {
      return _reg_skills[skill][0];
   }
   return 0;
} /* query_related_skills() */

/**
 * This method returns the cost of the specified skill.  This is the
 * the number of children it has.
 * @param skill the skill to check the cost of
 * @return the costof the skill
 */
int query_skill_cost(string skill) {
   if (_reg_skills[skill]) {
      return _reg_skills[skill][1];
   }
   return -1;
} /* query_skill_cost() */

/**
 * This method attempts to figure out the path of a skill from an array
 * of bits.  It verifies that the skill actually exists on the way.
 * @param bits the skill to check
 * @return the path of the skill
 */
string query_skill(string *bits) {
   mixed *arr;
   int i;
   string path, s1;

   if( !bits || !sizeof(bits) ) {
      return 0;
   }

   path = "";
   arr = _skills;
   for (i = 0; i < sizeof(arr); i += SKILL_ARR_SIZE) {
      if (sscanf(arr[i], bits[0]+"%s", s1) == 1) {
         path += "."+arr[i];
         arr = arr[i+SKILL_BIT];
         i = -SKILL_ARR_SIZE;
         bits = bits[1..sizeof(bits)];
         if (!sizeof(bits))
            return path[1..strlen(path)];
      }
   }
   if (i<sizeof(arr)) {
      return path[1..strlen(path)];
   }
   return 0;
} /* query_skill() */

/**
 * This method determines if only leaf skills can be increased in this
 * skill tree.  This is used for the other skill tree, to avoid increasing
 * all the other skills at the same time.
 * @param str the skill tree to check
 * @return 1 if the tree is only leaf, 0 if not
 */
int query_only_leaf(string str) {
   string *bits;

   bits = explode(str, ".");
   return _only_leaf[bits[0]];
} /* query_only_leaf() */

/**
 * This method checks to see if this particular skill is allowed to be
 * taught to people.
 * @param str the skill to check
 * @return 1 if they are not allowed to teach it, 0 if they are
 */
int query_allowed_to_teach(string str) {
   return !_not_allowed_to_teach[str];
} /* query_allowed_to_teach() */

/**
 * This method checks to see if the skill should only be shown on the
 * skill list if it is non-zero.
 * @param str the skill to check
 * @return 1 if should only show non-zero skills, 0 if show any skill
 */
int query_only_show_if_non_zero(string str) {
   return _only_show_if_non_zero[str];
} /* query_only_show_if_non_zero() */

/**
 * This method checks to see if the specified skill has no bonus associated
 * with it.
 * @param str the skill to check
 * @return 1 if there is no bonus, 0 if there is a bonus
 */
int query_no_bonus(string str) {
   return _no_bonus[str] || _ignore_bits[str];
} /* query_no_bonus() */

/**
 * This method returns 1 if the skill being asked about should be ignored.
 * An ignored skill means that up to this point the fact it is this deep
 * is ignored.  For example,if 'crafts' is ignored then 'crafts.smithing'
 * would count as if it was a top level skill.
 * @param skill the skill to check
 */
int is_skill_ignored(string skill) {
   return _ignore_bits[skill];
}

/**
 * This method returns the skill depth for teaching calculations.
 * @param skill the skill to get the depth of
 * @return the skill depth
 */
int query_skill_depth(string* skill) {
   int depth;
   int i;

   depth = 0;
   for (i = 0; i < sizeof(skill) - 1; i++) {
      if (!is_skill_ignored(implode(skill[0..i], "."))) {
         depth++;
      }
   }
   return depth;
}
