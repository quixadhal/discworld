/*
 * Converts the old skills array to the new skills mapping...
 * Assumes it's #include'd into /std/living/skills.c
 */

#define DIRECT(SKL) skillcpy_con(SKL, SKL)

static mixed *old_skills; /* we don't want to save them :) */

/*
 * Sets the skill pointed to by 'path' to have the 'lvl' and 'exp'
 * values for level and exp respectively.
 */
mapping set_new_skill(mapping sks, string *path, int lvl, int exp) {
  int i;
  string *sks_i;

  if (1 == sizeof(path)) { /* leaf */
    if (!sks[path[0]]) /* new_leaf */
      sks[path[0]] = ({ lvl, exp, ([ ]) });
    else
      sks[path[0]] = ({ lvl, exp, sks[path[0]][2] });
    return sks;
  }
  if (!sks[path[0]]) { /* new skill path */
    sks[path[0]] = ({
      0, 0,
      set_new_skill(([ ]), path[1..sizeof(path)], lvl, exp)
    });
    return sks;
  }
  sks[path[0]] = ({
    sks[path[0]][0], sks[path[0]][1],
    set_new_skill(sks[path[0]][2], path[1..sizeof(path)], lvl, exp)
  });
  return sks;
} /* set_new_skill() */

/*
 * Copies a skill in the old format to a skill in the new format...
 */
int skillcpy_con(string dst, string src) {
  string *bits;
  int i, pos, lvl, exp;
  mixed *old;

  old = old_skills;
  bits = explode(src, ".");
  for (i=0; i<sizeof(bits); i++) {
    if (-1 == (pos = member_array(bits[i], old))) return 0;
    /* The new offsets are 1 less than the old ones */
    lvl = old[i+SKILL_LVL+1];
    exp = old[i+SKILL_EXP+1];
    old = old[i+SKILL_BIT+1];
  }
  skills = set_new_skill(skills, explode(dst, "."), lvl, exp);
  return 1;
} /* skillcpy_con() */

/*
 * Converts the old skills array into the new skills mapping.
 */
int convert_skills() {
  if (mappingp(skills)) return 0; /* assume already done */
  old_skills = skills;
  skills = ([ ]); /* or should this be STD_SKILLS ?? */
  /*
   * Do individual call_out()s for each major skill grouping
   * to avoid any eval problems...
   */
  call_out("convert_fighting", 0);
  call_out("convert_magic", 0);
  call_out("convert_covert", 0);
  call_out("convert_faith", 0);
  call_out("convert_occult", 0);
  call_out("convert_other", 0);
  return 1;
} /* convert_skills() */

void convert_fighting() {
  DIRECT("fighting");
  DIRECT("fighting.combat");
  DIRECT("fighting.combat.melee");
  DIRECT("fighting.combat.melee.sharp");
  DIRECT("fighting.combat.melee.blunt");
  DIRECT("fighting.combat.melee.unarmed");
  DIRECT("fighting.combat.parry");
  DIRECT("fighting.combat.dodging");
  DIRECT("fighting.combat.dodging.range");
  DIRECT("fighting.combat.dodging.melee");
  DIRECT("fighting.combat.special");
  DIRECT("fighting.points");
} /* convert_fighting() */

void convert_magic() {
  DIRECT("magic");
  DIRECT("magic.spells");
  DIRECT("magic.spells.offensive");
  DIRECT("magic.spells.offensive.target");
  DIRECT("magic.spells.offensive.area");
  DIRECT("magic.spells.defensive");
  DIRECT("magic.spells.defensive.area");
  DIRECT("magic.spells.defensive.self");
  DIRECT("magic.spells.defensive.target");
  DIRECT("magic.spells.misc");
  DIRECT("magic.spells.misc.area");
  DIRECT("magic.spells.misc.self");
  DIRECT("magic.spells.misc.target");
  DIRECT("magic.spells.special");
  DIRECT("magic.items");
  DIRECT("magic.items.rod");
  DIRECT("magic.items.staff");
  DIRECT("magic.items.wand");
  DIRECT("magic.items.scroll");
  DIRECT("magic.items.potion");
  DIRECT("magic.points");
} /* convert_magic() */

void convert_faith() {
  DIRECT("faith");
  DIRECT("faith.spells");
  DIRECT("faith.spells.offensive");
  DIRECT("faith.spells.offensive.area");
  DIRECT("faith.spells.offensive.target");
  DIRECT("faith.spells.defensive");
  DIRECT("faith.spells.defensive.area");
  DIRECT("faith.spells.defensive.target");
  DIRECT("faith.spells.defensive.self");
  DIRECT("faith.spells.curing");
  DIRECT("faith.spells.curing.target");
  DIRECT("faith.spells.curing.self");
  DIRECT("faith.spells.misc");
  DIRECT("faith.spells.misc.area");
  DIRECT("faith.spells.misc.target");
  DIRECT("faith.spells.misc.self");
  DIRECT("faith.spells.special");
  DIRECT("faith.items");
  DIRECT("faith.items.rod");
  DIRECT("faith.items.staff");
  DIRECT("faith.items.wand");
  DIRECT("faith.items.scroll");
  DIRECT("faith.items.potion");
  DIRECT("faith.points");
} /* convert_faith() */

void convert_covert() {
  skillcpy_con("covert", "other.covert"); /* hmmm */
  skillcpy_con("covert.urban.stealth", "other.covert.stealth");
  skillcpy_con("covert.urban.hiding", "other.covert.hiding");
  skillcpy_con("covert.lockpick", "other.covert.lockpick");
  skillcpy_con("covert.sleight-of-hand", "other.covert.sleight-of-hand");
} /* convert_covert() */

void convert_other() {
  skillcpy_con("other", "other"); /* extremely dubious */
  skillcpy_con("other.wilderness", "other.woodland"); /* dubious at best */
  skillcpy_con("other.wilderness.tracking", "other.woodland.tracking");
  skillcpy_con("other.wilderness.swiming", "other.woodland.swiming");
  DIRECT("other.smithing");
  skillcpy_con("other.valuing", "other.valueing");
  skillcpy_con("other.valuing.gems", "other.valueing.gems");
  skillcpy_con("other.valuing.jewelery", "other.valueing.jewelery");
  skillcpy_con("other.valuing.weapons", "other.valueing.weapons");
  skillcpy_con("other.valuing.armours", "other.valueing.armours");
  DIRECT("other.evaluating");
  DIRECT("other.evaluating.armour");
  DIRECT("other.evaluating.weapon");
} /* convert_other() */

void convert_occult() {
  DIRECT("occult");
  DIRECT("occult.points");
  DIRECT("occult.methods");
  DIRECT("occult.methods.brewing");
  DIRECT("occult.methods.dancing");
  DIRECT("occult.methods.chanting");
  DIRECT("occult.spells");
  DIRECT("occult.spells.cursing");
  DIRECT("occult.spells.summoning");
  DIRECT("occult.spells.enchanting");
  DIRECT("occult.spells.charming");
  DIRECT("occult.spells.healing");
} /* convert_occult() */
