#define SKILL_NAM 0
#define SKILL_LVL 1
#define SKILL_EXP 2
#define SKILL_BIT 3
#define SKILL_ARR_SIZE 4
#define SKILL_OB "/std/skills"

/*
 * These are used in /std/living/skills.c for the training/learning of skills.
 */
#define O_SKILL 0
#define O_NUM 1
#define O_LVL 2
#define O_XP 3
#define O_OTHER_PER 4

#define STD_SKILLS ({ \
        "fighting", 0, 0, ({ \
            "combat", 0, 0, ({ \
                "melee", 0, 0, ({ \
                    "sharp", 0, 0, ({ }), \
                    "blunt", 0, 0, ({ }), \
                    "unarmed", 0, 0, ({ }), \
                }), \
                "parry", 0, 0, ({ \
/* \
                    "thrown", 0, 0, ({ }), \
                    "bow", 0, 0, ({ }), \
 */ \
                }), \
                "dodging", 0, 0, ({ \
                    "range", 0, 0, ({ }), \
                    "melee", 0, 0, ({ }), \
                }), \
                "special", 0, 0, ({ }), \
            }), \
            "points", 0, 0, ({ }), \
        }), \
        "magic", 0, 0, ({ \
            "spells", 0, 0, ({ \
                "offensive", 0, 0, ({ \
                    "target", 0, 0, ({ }), \
                    "area", 0, 0, ({ }), \
                }), \
                "defensive", 0, 0, ({ \
                    "area", 0, 0, ({ }), \
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
                "staff", 0, 0, ({ }), \
                "wand", 0, 0, ({ }), \
                "scroll", 0, 0, ({ }), \
                "potion", 0, 0, ({ }), \
            }), \
            "points", 0, 0, ({ }), \
        }), \
        "faith", 0, 0, ({ \
            "spells", 0, 0, ({ \
                "offensive", 0, 0, ({ \
                    "area", 0, 0, ({ }), \
                    "target", 0, 0, ({ }), \
                }), \
                "defensive", 0, 0, ({ \
                    "area", 0, 0, ({ }), \
                    "target", 0, 0, ({ }), \
                    "self", 0, 0, ({ }), \
                }), \
                "curing", 0, 0, ({ \
                    "target", 0, 0, ({ }), \
                    "self", 0, 0, ({ }), \
                }), \
                "misc", 0, 0, ({ \
                    "area", 0, 0, ({ }), \
                    "target", 0, 0, ({ }), \
                    "self", 0, 0, ({ }), \
                }), \
                "special", 0, 0, ({ }), \
            }), \
            "items", 0, 0, ({ \
                "rod", 0, 0, ({ }), \
                "staff", 0, 0, ({ }), \
                "wand", 0, 0, ({ }), \
                "scroll", 0, 0, ({ }), \
                "potion", 0, 0, ({ }), \
            }), \
            "points", 0, 0, ({ }), \
        }), \
        "other", 0, 0, ({ \
            "woodland", 0, 0, ({ \
                "tracking", 0, 0, ({ }), \
                "swimming", 0, 0, ({ }), \
            }), \
            "covert", 0, 0, ({ \
                "stealth", 0, 0, ({ }), \
                "hiding", 0, 0, ({ }), \
                "lockpick", 0, 0, ({ }), \
                "sleight-of-hand", 0, 0, ({ }), \
            }), \
            "smithing", 0, 0, ({ }), \
            "valueing", 0, 0, ({ \
                "gems", 0, 0, ({ }), \
                "jewelery", 0, 0, ({ }), \
                "weapons", 0, 0, ({ }), \
                "armours", 0, 0, ({ }), \
            }), \
            "evaluating", 0, 0, ({ \
                "armour", 0, 0, ({ }), \
                "weapon", 0, 0, ({ }), \
            }), \
        }), \
        "occult", 0, 0, ({ \
            "points", 0, 0, ({ }), \
            "methods", 0, 0, ({ \
                "brewing", 0, 0, ({ }), \
                "dancing", 0, 0, ({ }), \
                "chanting", 0, 0, ({ }), \
            }), \
            "spells", 0, 0, ({ \
                "cursing", 0, 0, ({ }), \
                "summoning", 0, 0, ({ }), \
                "enchanting", 0, 0, ({ }), \
                "charming", 0, 0, ({ }), \
                "healing", 0, 0, ({ }), \
            }), \
        }), \
    })
