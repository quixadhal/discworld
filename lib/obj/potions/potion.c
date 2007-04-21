/*
** potion.c - potion information object
**
** This object will contain the information related to a particular
** potion.  It is intended to be used as a base for virtual objects
** which will be cloned by the potion handler (ie, only one virtual
** object for each potion will exist in the game).  The info on the
** potion will be gotten by querying the potion handler.
**
** Parameters:
**    min_level - minimum aggregate level for success
**    max_level - aggregate level at which success is certain
**    advance_chance - chance of advancing skill(s) when successful
**    backfire_chance - chance of result being other than expected
**    rel_strength - amount of reagent will be
**                   aggregate_sum*rel_strength/min_level
**    skills - array of skills required and weighting
**    reagent_alias - medium alias for resulting reagent (on success)
**    backfire_alias - medium alias for resulting reagent (on backfire)
**    potion_verb - verb expected for this potion (eg, 'mix', 'brew')
**    recipe - the text of the recipe for the potion
**    chant - the chant spoken by the player (optional)
**
** Written by Jeremy, ?/?/95
*/

// I'm wondering if I really need this...
inherit "/std/object";

#include <tasks.h>

#define SKILL_WGT 0
#define SKILL_MAX 1
#define SKILL_LVL 2
#define SKILL_ADV 3
#define REAGENTS "/obj/reagents/"

int min_level, max_level, advance_chance, backfire_chance, rel_strength;
mapping skills = allocate_mapping(1);
string *skill_str, reagent_alias, backfire_alias, potion_verb, recipe;
string chant;

void set_min_level(int l) { min_level = l; }

void set_max_level(int l) { max_level = l; }

int query_min_level() { return min_level; }

int query_max_level() { return max_level; }

void set_advance_chance(int c) { advance_chance = c; }

int query_advance_chance() { return advance_chance; }

void set_backfire_chance(int c) { backfire_chance = c; }

int query_backfire_chance() { return backfire_chance; }

void set_reagent_alias(string s) { reagent_alias = s; }

string query_reagent_alias() { return reagent_alias; }

void set_backfire_alias(string s) { backfire_alias = s; }

string query_backfire_alias() { return backfire_alias; }

void set_rel_strength(int s) { rel_strength = s; }

int query_rel_strength() { return rel_strength; }

void set_potion_verb(string s) { potion_verb = s; }

string query_potion_verb() { return potion_verb; }

void set_recipe(string s) { recipe = s; }

string query_recipe() { return recipe; }

void set_chant(string s) { chant = s; }

string query_chant() { return chant; }

// The final determination of success/failure is a weighted sum of
// one or more skill levels or bonuses.  Each element of 's' is a
// 4-element array: the skill (string), the weight (int), and whether to use
// level or bonus (true=level).  'skills' is a mapping, with the key
// being the skill and the value being a 4-element array: the remaining
// 3 and a flag indicating if the skill should be advanced for this player.

void set_skills(mixed s) {
    int i;

    for (i = 0; i < (sizeof(s)); i++) {
        skills[s[i][0]] = ({s[i][SKILL_WGT+1], s[i][SKILL_MAX+1],
                            s[i][SKILL_LVL+1], 0});
    }
    skill_str = keys(skills);
}

mapping query_skills() { return skills; }

// This is the guts of the potion.  If successful, it'll return an
// object that'll react with the other reagents in the vessel.

object clone_reagent(string given_verb) {
    int result, i, sum, x;
    string s;
    object ob;

    if (potion_verb != given_verb) return 0;
    for (i = 0; i < sizeof(skill_str); i++) {
        s = skill_str[i];
        skills[s][SKILL_ADV] = 0;
        if (skills[s][SKILL_LVL]) {
            if ((x = (int)this_player()->query_skill(s))
                > skills[s][SKILL_MAX]) {
                x = skills[s][SKILL_MAX];
            } else {
                skills[s][SKILL_ADV] = 1;
            }
        } else {
            if ((x = (int)this_player()->query_skill_bonus(s))
                > skills[s][SKILL_MAX]) {
                x = skills[s][SKILL_MAX];
            } else {
                skills[s][SKILL_ADV] = 1;
            }
        }
        sum += x * skills[s][SKILL_WGT];
    }
    //write("sum = " + sum + "\n");
    result = TASKER->attempt_task(min_level, sum, advance_chance,
                          (max_level - min_level));
    if ((result == FAIL) || (result == BARF)) {
        if (random(100) < backfire_chance) {
            // Spell backfired
            ob = clone_object(REAGENTS+"brew_reagent.ob");
            ob->set_medium_alias(backfire_alias);
            ob->set_amount(sum*rel_strength/min_level);
            return ob;
        }           
        return 0;
    }
    if (result == AWARD) {
        for (i = 0; i < sizeof(skill_str); i++) {
            s = skill_str[i];
            if (skills[s][SKILL_ADV]) {
                this_player()->add_skill_level(s, 1, 0);
            }
        }
    }
    ob = clone_object(REAGENTS+"brew_reagent.ob");
    ob->set_medium_alias(reagent_alias);
    ob->set_amount(sum*rel_strength/min_level);
    return ob;
}
