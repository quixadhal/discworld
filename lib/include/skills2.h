#undef SKILL_NAM 0
#define SKILL_LVL 0
#define SKILL_EXP 1
#define SKILL_BIT 2
#define SKILL_ARR_SIZE 3
#define SKILL_OB "/std/skills"
#define O_SKILL 0
#define O_NUM 1
#define O_LVL 2
#define O_XP 3
#define SKILLS_FIGHTING ({ 0, 0, ([ \
  "combat" : ({ 0, 0, ([ \
    "melee" : ({ 0, 0, ([ \
      "sharp" : ({ 0, 0, ([ ]), }), \
      "blunt" : ({ 0, 0, ([ ]), }), \
      "unarmed" : ({ 0, 0, ([ ]), }), \
    ]), }), \
    "parry" : ({ 0, 0, ([ \
/* \
      "thrown" : ({ 0, 0, ([ ]), }), \
      "bow" : ({ 0, 0, ([ ]), }), \
 */ \
    ]), }), \
    "dodging" : ({ 0, 0, ([ \
      "range" : ({ 0, 0, ([ ]), }), \
      "melee" : ({ 0, 0, ([ ]), }), \
    ]), }), \
    "special" : ({ 0, 0, ([ ]), }), \
  ]), }), \
  "points" : ({ 0, 0, ([ ]), }), \
]), })

#define SKILLS_MAGIC ({ 0, 0, ([ \
  "spells" : ({ 0, 0, ([ \
    "offensive" : ({ 0, 0, ([ \
      "target" : ({ 0, 0, ([ ]), }), \
      "area" : ({ 0, 0, ([ ]), }), \
    ]), }), \
    "defensive" : ({ 0, 0, ([ \
      "area" : ({ 0, 0, ([ ]), }), \
      "self" : ({ 0, 0, ([ ]), }), \
      "target" : ({ 0, 0, ([ ]), }), \
    ]), }), \
    "misc" : ({ 0, 0, ([ \
      "area" : ({ 0, 0, ([ ]), }), \
      "self" : ({ 0, 0, ([ ]), }), \
      "target" : ({ 0, 0, ([ ]), }), \
    ]), }), \
    "special" : ({ 0, 0, ([ ]), }), \
  ]), }), \
  "items" : ({ 0, 0, ([ \
    "rod" : ({ 0, 0, ([ ]), }), \
    "staff" : ({ 0, 0, ([ ]), }), \
    "wand" : ({ 0, 0, ([ ]), }), \
    "scroll" : ({ 0, 0, ([ ]), }), \
    "potion" : ({ 0, 0, ([ ]), }), \
  ]), }), \
  "points" : ({ 0, 0, ([ ]), }), \
]), })

#define SKILLS_FAITH ({ 0, 0, ([ \
  "spells" : ({ 0, 0, ([ \
    "offensive" : ({ 0, 0, ([ \
      "area" : ({ 0, 0, ([ ]), }), \
      "target" : ({ 0, 0, ([ ]), }), \
    ]), }), \
    "defensive" : ({ 0, 0, ([ \
      "area" : ({ 0, 0, ([ ]), }), \
      "target" : ({ 0, 0, ([ ]), }), \
      "self" : ({ 0, 0, ([ ]), }), \
    ]), }), \
    "curing" : ({ 0, 0, ([ \
      "target" : ({ 0, 0, ([ ]), }), \
      "self" : ({ 0, 0, ([ ]), }), \
    ]), }), \
    "misc" : ({ 0, 0, ([ \
      "area" : ({ 0, 0, ([ ]), }), \
      "target" : ({ 0, 0, ([ ]), }), \
      "self" : ({ 0, 0, ([ ]), }), \
    ]), }), \
    "special" : ({ 0, 0, ([ ]), }), \
  ]), }), \
  "items" : ({ 0, 0, ([ \
    "rod" : ({ 0, 0, ([ ]), }), \
    "staff" : ({ 0, 0, ([ ]), }), \
    "wand" : ({ 0, 0, ([ ]), }), \
    "scroll" : ({ 0, 0, ([ ]), }), \
    "potion" : ({ 0, 0, ([ ]), }), \
  ]), }), \
  "points" : ({ 0, 0, ([ ]), }), \
]), })

#define SKILLS_OCCULT ({ 0, 0, ([ \
  "points" : ({ 0, 0, ([ ]), }), \
  "methods" : ({ 0, 0, ([ \
    "brewing" : ({ 0, 0, ([ ]), }), \
    "dancing" : ({ 0, 0, ([ ]), }), \
    "chanting" : ({ 0, 0, ([ ]), }), \
  ]), }), \
  "spells" : ({ 0, 0, ([ \
    "cursing" : ({ 0, 0, ([ ]), }), \
    "summoning" : ({ 0, 0, ([ ]), }), \
    "enchanting" : ({ 0, 0, ([ ]), }), \
    "charming" : ({ 0, 0, ([ ]), }), \
    "healing" : ({ 0, 0, ([ ]), }), \
  ]), }), \
]), })

#define SKILLS_COVERT ({ 0, 0, ([ \
  "sleight-of-hand" : ({ 0, 0, ([ ]), }), \
  "lockpick" : ({ 0, 0, ([ ]), }), \
  "forgery" : ({ 0, 0, ([ ]), }), \
  "disguise" : ({ 0, 0, ([ ]), }), \
  "urban" : ({ 0, 0, ([ \
    "stealth" : ({ 0, 0, ([ ]), }), \
    "hiding" : ({ 0, 0, ([ ]), }), \
    "signaling" : ({ 0, 0, ([ ]), }), \
  ]), }), \
  "wilderness" :  ({ 0, 0, ([ \
    "stealth" : ({ 0, 0, ([ ]), }), \
    "hiding" : ({ 0, 0, ([ ]), }), \
    "signaling" : ({ 0, 0, ([ ]), }), \
  ]), }), \
  "manuevering" : ({ 0, 0, ([ \
    "powered-armour" : ({ 0, 0, ([ ]), }), \
    "sherman-tanks" : ({ 0, 0, ([ ]), }), \
  ]), }), \
]), })

#define SKILLS_OTHER ({ 0, 0, ([ \
  "wilderness" : ({ 0, 0, ([ \
    "tracking" : ({ 0, 0, ([ ]), }), \
    "swimming" : ({ 0, 0, ([ ]), }), \
    "fishing" : ({ 0, 0, ([ ]), }), \
    "riding" : ({ 0, 0, ([ \
      "horse" : ({ 0, 0, ([ ]), }), \
      "chicken" : ({ 0, 0, ([ ]), }), \
      "guppy" : ({ 0, 0, ([ ]), }), \
    ]), }), \
  ]), }), \
  "first-aid" : ({ 0, 0, ([ ]), }), \
  "smithing" : ({ 0, 0, ([ ]), }), \
  "valuing" : ({ 0, 0, ([ \
    "gems" : ({ 0, 0, ([ ]), }), \
    "jewelery" : ({ 0, 0, ([ ]), }), \
    "weapons" : ({ 0, 0, ([ ]), }), \
    "armours" : ({ 0, 0, ([ ]), }), \
  ]), }), \
  "evaluating" : ({ 0, 0, ([ \
    "armour" : ({ 0, 0, ([ ]), }), \
    "weapon" : ({ 0, 0, ([ ]), }), \
  ]), }), \
  "drug-resistance" : ({ 0, 0, ([ \
    "alcohol" : ({ 0, 0, ([ ]), }), \
  ]), }), \
]), })

#define STD_SKILLS ([ \
  "fighting" : SKILLS_FIGHTING, \
  "magic" : SKILLS_MAGIC, \
  "faith" : SKILLS_FAITH, \
  "covert" : SKILLS_COVERT, \
  "occult" : SKILLS_OCCULT, \
  "other" : SKILLS_OTHER, \
])
