/**
 * The main skills header file.  This contains the skill tree and any bits
 * relating to the skill tree.
 * @author Pinkfish
 * @see /std/skills.c
 * @see /std/living/skills.c
 */
#ifndef __SYS__SKILLS
/** @ignore yes */
#define __SYS__SKILLS

/** The index corresponding to the skill name in the skill array. */
#define SKILL_NAM 0
/** The index corresponding to the skill level in the skill array. */
#define SKILL_LVL 1
/** The index corresponding to the skill exp in the skill array. */
#define SKILL_EXP 2
/** The index corresponding to the array of sub skills in the skill array. */
#define SKILL_BIT 3
/** The size of the skill array. */
#define SKILL_ARR_SIZE 4
/** The skill handling object. */
#define SKILL_OB "/std/skills"
/** The maximum level that you can teach someone else too. */
#define SKILL_MAX_TEACH 600

/*
 * These are used in /std/living/skills.c for the training/learning of skills.
 */
/** @ignore yes */
#define O_SKILL 0
/** @ignore yes */
#define O_NUM 1
/** @ignore yes */
#define O_LVL 2
/** @ignore yes */
#define O_XP 3
/** @ignore yes */
#define O_OTHER_PER 4

/** @ignore yes */
#define STD_SKILLS_COVERT ({ \
        "covert", 0, 0, ({ \
            "stealth", 0, 0, ({ \
                "inside", 0, 0, ({ }), \
                "outside", 0, 0, ({ }), \
                "underwater", 0, 0, ({ }), \
            }), \
            "hiding", 0, 0, ({ \
               "person", 0, 0, ({ }), \
               "object", 0, 0, ({ }), \
            }), \
            "lockpick", 0, 0, ({ \
                "doors", 0, 0, ({ }), \
                "safes", 0, 0, ({ }), \
                "traps", 0, 0, ({ }), \
            }), \
            "manipulation", 0, 0, ({ \
                "palming", 0, 0, ({ }), \
                "passing", 0, 0, ({ }), \
                "sleight-of-hand", 0, 0, ({ }), \
                "stealing", 0, 0, ({ }), \
            }), \
            "casing", 0, 0, ({ \
                "person", 0, 0, ({ }), \
                "place", 0, 0, ({ }), \
            }), \
            "items", 0, 0, ({ \
                "poisons", 0, 0, ({ }), \
                "traps", 0, 0, ({ }), \
                "weapons", 0, 0, ({ }), \
            }), \
            "points", 0, 0, ({ }), \
        }), \
    })

/** @ignore yes */
#define STD_SKILLS_CRAFTS ({ \
        "crafts", 0, 0, ({ \
            "smithing", 0, 0, ({ \
                "gold", 0, 0, ({ }), \
                "silver", 0, 0, ({ }), \
                "black", 0, 0, ({ \
                    "tools", 0, 0, ({ }), \
                    "weapons", 0, 0, ({ }), \
                    "armour", 0, 0, ({ }), \
                }), \
                "gem", 0, 0, ({ \
                    "cutting", 0, 0, ({ }), \
                    "polishing", 0, 0, ({ }), \
                    "setting", 0, 0, ({ }), \
                }), \
                "locks", 0, 0, ({ }), \
            }), \
            "mining", 0, 0, ({ \
                "gem", 0, 0, ({ }), \
                "ore", 0, 0, ({ \
                    "panning", 0, 0, ({ }), \
                }), \
                "mineral", 0, 0, ({ }), \
            }), \
            "hunting", 0, 0, ({ \
                "tracking", 0, 0, ({ }), \
                "fishing", 0, 0, ({ }), \
                "trapping", 0, 0, ({ }), \
                "foraging", 0, 0, ({ }), \
            }), \
            "carpentry", 0, 0, ({ \
                "furniture", 0, 0, ({ }), \
                "coopering", 0, 0, ({ }), \
                "turning", 0, 0, ({ }), \
                "whittling", 0, 0, ({ }), \
            }), \
            "pottery", 0, 0, ({ \
                "forming", 0, 0, ({ \
                    "throwing", 0, 0, ({ }), \
                    "shaping", 0, 0, ({ }), \
                }), \
                "glazing", 0, 0, ({ }), \
                "staining", 0, 0, ({ }), \
                "firing", 0, 0, ({ }), \
            }), \
            "materials", 0, 0, ({ \
                "tanning", 0, 0, ({ }), \
                "leatherwork", 0, 0, ({ }), \
                "weaving", 0, 0, ({ }), \
                "spinning", 0, 0, ({ }), \
                "needlework", 0, 0, ({ }), \
                "dyeing", 0, 0, ({ }), \
            }), \
            "husbandry", 0, 0, ({ \
                "plant", 0, 0, ({ \
                    "edible", 0, 0, ({ }), \
                    "herbal", 0, 0, ({ }), \
                    "milling", 0, 0, ({ }), \
                    "tree", 0, 0, ({ }), \
                }), \
                "animal", 0, 0, ({ \
                    "grooming", 0, 0, ({ }), \
                    "breeding", 0, 0, ({ }), \
                    "slaughtering", 0, 0, ({ }), \
                }), \
            }), \
            "culinary", 0, 0, ({ \
                "cooking", 0, 0, ({ }), \
                "baking", 0, 0, ({ }), \
                "butchering", 0, 0, ({ }), \
                "preserving", 0, 0, ({ }), \
                "brewing", 0, 0, ({ }), \
                "distilling", 0, 0, ({ }), \
            }), \
            "arts", 0, 0, ({ \
                "design", 0, 0, ({ }), \
                "calligraphy", 0, 0, ({ }), \
                "drawing", 0, 0, ({ }), \
                "painting", 0, 0, ({ }), \
                "printing", 0, 0, ({ }), \
                "sculpture", 0, 0, ({ }), \
            }), \
            "music", 0, 0, ({ \
                "instruments", 0, 0, ({ \
                    "percussion", 0, 0, ({ }), \
                    "wind", 0, 0, ({ }), \
                    "stringed", 0, 0, ({ }), \
                    "keyboard", 0, 0, ({ }), \
                    "vocal", 0, 0, ({ }), \
                }), \
                "special", 0, 0, ({ }), \
                "theory", 0, 0, ({ }), \
                "performance", 0, 0, ({ }), \
            }), \
            "points", 0, 0, ({ }), \
        }), \
    })

/** @ignore yes */
#define STD_SKILLS_FAITH ({ \
        "faith", 0, 0, ({ \
            "rituals", 0, 0, ({ \
                "offensive", 0, 0, ({ \
                    "area", 0, 0, ({ }), \
                    "target", 0, 0, ({ }), \
                }), \
                "defensive", 0, 0, ({ \
                    "area", 0, 0, ({ }), \
                    "self", 0, 0, ({ }), \
                    "target", 0, 0, ({ }), \
                }), \
                "curing", 0, 0, ({ \
                    "self", 0, 0, ({ }), \
                    "target", 0, 0, ({ }), \
                }), \
                "misc", 0, 0, ({ \
                    "area", 0, 0, ({ }), \
                    "self", 0, 0, ({ }), \
                    "target", 0, 0, ({ }), \
                }), \
                "special", 0, 0, ({ }), \
            }), \
            "items", 0, 0, ({ \
                "rod", 0, 0, ({ }), \
                "scroll", 0, 0, ({ }), \
            }), \
            "points", 0, 0, ({ }), \
        }), \
    })
/** @ignore yes */
#define STD_SKILLS_FIGHTING ({ \
        "fighting", 0, 0, ({ \
            "combat", 0, 0, ({ \
                "melee", 0, 0, ({ \
                    "sharp", 0, 0, ({ }), \
                    "pierce", 0, 0, ({ }), \
                    "blunt", 0, 0, ({ }), \
                    "unarmed", 0, 0, ({ }), \
                }), \
                "range", 0, 0, ({ \
                    "thrown", 0, 0, ({ }), \
                    "fired", 0, 0, ({ }), \
                }), \
                "parry", 0, 0, ({ \
                    "melee", 0, 0, ({ }), \
                    "range", 0, 0, ({ }), \
                }), \
                "dodging", 0, 0, ({ \
                    "melee", 0, 0, ({ }), \
                    "range", 0, 0, ({ }), \
                }), \
                "special", 0, 0, ({ \
                    "weapon", 0, 0, ({ }), \
                    "unarmed", 0, 0, ({ }), \
                    "tactics", 0, 0, ({ }), \
              }), \
            }), \
            "points", 0, 0, ({ }), \
        }), \
    })
/** @ignore yes */
#define STD_SKILLS_MAGIC ({ \
        "magic", 0, 0, ({ \
            "spells", 0, 0, ({ \
                "offensive", 0, 0, ({ }), \
                "defensive", 0, 0, ({ }), \
                "misc", 0, 0, ({ }), \
                "special", 0, 0, ({ }), \
            }), \
            "methods", 0, 0, ({ \
                "elemental", 0, 0, ({ \
                    "air", 0, 0, ({ }), \
                    "earth", 0, 0, ({ }), \
                    "fire", 0, 0, ({ }), \
                    "water", 0, 0, ({ }), \
                }), \
                "mental", 0, 0, ({ \
                    "animating", 0, 0, ({ }), \
                    "channeling", 0, 0, ({ }), \
                    "charming", 0, 0, ({ }), \
                    "convoking", 0, 0, ({ }), \
                    "cursing", 0, 0, ({ }), \
                }), \
                "physical", 0, 0, ({ \
                    "binding", 0, 0, ({ }), \
                    "brewing", 0, 0, ({ }), \
                    "chanting", 0, 0, ({ }), \
                    "dancing", 0, 0, ({ }), \
                    "enchanting", 0, 0, ({ }), \
                    "evoking", 0, 0, ({ }), \
                    "healing", 0, 0, ({ }), \
                    "scrying", 0, 0, ({ }), \
                }), \
                "spiritual", 0, 0, ({ \
                    "abjuring", 0, 0, ({ }), \
                    "banishing", 0, 0, ({ }), \
                    "conjuring", 0, 0, ({ }), \
                    "divining", 0, 0, ({ }), \
                    "summoning", 0, 0, ({ }), \
                }), \
            }), \
            "items", 0, 0, ({ \
                "held", 0, 0, ({ \
                    "wand", 0, 0, ({ }), \
                    "rod", 0, 0, ({ }), \
                    "staff", 0, 0, ({ }), \
                    "broom", 0, 0, ({ }), \
                }), \
                "worn", 0, 0, ({ \
                    "amulet", 0, 0, ({ }), \
                    "ring", 0, 0, ({ }), \
                }), \
                "scroll", 0, 0, ({ }), \
                "talisman", 0, 0, ({ }), \
            }), \
            "points", 0, 0, ({ }), \
        }), \
    })

/** @ignore yes */
#define STD_SKILLS_OTHER ({ \
        "other", 0, 0, ({ \
            "trading", 0, 0, ({ \
                 "buying", 0, 0, ({ }), \
                 "selling", 0, 0, ({ }), \
                 "valueing", 0, 0, ({ \
                    "gems", 0, 0, ({ }), \
                    "jewellery", 0, 0, ({ }), \
                    "weapons", 0, 0, ({ }), \
                    "armour", 0, 0, ({ }), \
                }), \
            }), \
            "movement", 0, 0, ({ \
                 "swimming", 0, 0, ({ }), \
                 "riding", 0, 0, ({ \
                     "horse", 0, 0, ({ }), \
                     "camel", 0, 0, ({ }), \
                 }), \
                 "climbing", 0, 0, ({ \
                     "tree", 0, 0, ({ }), \
                     "rock", 0, 0, ({ }), \
                     "rope", 0, 0, ({ }), \
                 }), \
                 "sailing", 0, 0, ({ }), \
            }), \
            "acrobatics", 0, 0, ({ \
                 "tumbling", 0, 0, ({ }), \
                 "vaulting", 0, 0, ({ }), \
                 "balancing", 0, 0, ({ }), \
            }), \
            "evaluating", 0, 0, ({ \
                "weapons", 0, 0, ({ }), \
                "armour", 0, 0, ({ }), \
            }), \
            "culture", 0, 0, ({ \
                "ankh-morporkian", 0, 0, ({ }), \
                "djelian", 0, 0, ({ }), \
                "ephebian", 0, 0, ({ }), \
                "agatean", 0, 0, ({ }), \
            }), \
            "perception", 0, 0, ({ }), \
            "direction", 0, 0, ({ }), \
            "health", 0, 0, ({ }), \
            "points", 0, 0, ({ }), \
        }), \
    })

/**
 * The complete standard skill tree.  This should be avoided being used
 * if possible.
 * @see /std/skills.c
 * @see /std/living/skills.c
 */
#define STD_SKILLS ( STD_SKILLS_COVERT + STD_SKILLS_CRAFTS + STD_SKILLS_FAITH + STD_SKILLS_FIGHTING + STD_SKILLS_MAGIC + STD_SKILLS_OTHER )
#endif /* __SYS__SKILLS */
