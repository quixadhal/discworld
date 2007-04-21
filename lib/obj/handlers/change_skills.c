/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: change_skills.c,v 1.9 2002/08/05 17:14:46 ceres Exp $
 * 
*/
/**
 * Handles updating the skill systems to new levels when the skill tree
 * is changed.
 * @author Deutha
 * @see /std/living/skills.c
 * @change 1/2/97 Pinkfish
 * Updated to include the new language skill tree
 */
#include <login_handler.h>
#include <language.h>
#include <tune.h>
#include <skills.h>
#include <combat.h>

#define ORIGINAL "alpha"
#define VERSION "beta"
#define VERSION_2 "gamma"
#define VERSION_3 "delta"
#define VERSION_4 "epsilon"
#define VERSION_5 "zeta"
#define VERSION_6 "eta"

#define OLD_SKILLS ({ \
      "magic.spells.offensive.area", \
      "magic.spells.offensive.target", \
      "magic.spells.defensive.area", \
      "magic.spells.defensive.self", \
      "magic.spells.defensive.target", \
      "magic.spells.misc.area", \
      "magic.spells.misc.self", \
      "magic.spells.misc.target", \
      "magic.spells.special", \
      "magic.items.held.wand", \
      "magic.items.held.rod", \
      "magic.items.held.staff", \
      "magic.items.held.broom", \
      "magic.items.worn.amulet", \
      "magic.items.worn.ring", \
      "magic.items.scroll", \
      "magic.points", \
      "occult.methods.brewing", \
      "occult.methods.dancing", \
      "occult.methods.chanting", \
      "occult.methods.flying", \
      "occult.spells.cursing", \
      "occult.spells.summoning", \
      "occult.spells.enchanting", \
      "occult.spells.charming", \
      "occult.spells.healing", \
   })

#define OTHER_SKILLS ({ \
      "magic", \
      "magic.spells", \
      "magic.spells.offensive", \
      "magic.spells.defensive", \
      "magic.spells.misc", \
      "magic.items", \
      "magic.items.held", \
      "magic.items.worn", \
      "occult", \
      "occult.methods", \
      "occult.spells", \
   })

#define ETA_CHANGES (["other.tracking" : "crafts.hunting.tracking", \
                      "other.swimming" : "other.movement.swimming", \
                      "other.riding" : "other.movement.riding", \
                      "other.riding.horse" : "other.movement.riding.horse", \
                      "other.riding.camel" : "other.movement.riding.camel", \
                      "other.climbing" : "other.movement.climbing", \
                      "other.climbing.tree" : "other.movement.climbing.tree", \
                      "other.climbing.rock" : "other.movement.climbing.rock", \
                      "other.climbing.rope" : "other.movement.climbing.rope", \
                      "other.valueing" : "other.trading.valueing", \
                      "other.valueing.gems" : "other.trading.valueing.gems", \
                      "other.valueing.jewellery" : "other.trading.valueing.jewellery", \
                      "other.valueing.weapons" : "other.trading.valueing.weapons", \
                      "other.valueing.armour" : "other.trading.valueing.armour"])

#define EFFECTS ({"magic.defensive.floating", "magic.shield.impact", \
                  "magic.defensive.skin.troll", "priest.shield" })

/** @ignore yes */
int query_level_in( mapping skills, string skill ) {
   string *bits;

   if ( !undefinedp( skills[ skill ] ) )
      return skills[ skill ];
   bits = explode( skill, "." );
   if ( sizeof( bits ) == 1 )
      return 0;
   bits = delete( bits, sizeof( bits ) - 1, 1 );
   skill = implode( bits, "." );
   return query_level_in( skills, skill );
} /* query_level_in() */

/**
 * Update the skills from the original skills system to the
 * beta skills system.
 * @param thing the person to update
 */
void change_skills( object thing ) {
   int i, total;
   mapping skills;

   skills = (mapping)thing->query_skills();
   if ( !m_sizeof( skills ) )
      return;
   for ( i = 0; i < sizeof( OLD_SKILLS ); i++ ) {
      total += query_level_in( skills, OLD_SKILLS[ i ] );
      if ( !undefinedp( skills[ OLD_SKILLS[ i ] ] ) )
         skills = m_delete( skills, OLD_SKILLS[ i ] );
   }
   for ( i = 0; i < sizeof( OTHER_SKILLS ); i++ )
      if ( !undefinedp( skills[ OTHER_SKILLS[ i ] ] ) )
         skills = m_delete( skills, OTHER_SKILLS[ i ] );
   if ( !total )
      return;
   thing->add_property( "magic levels", total );
   thing->set_skills( skills );
   tell_object( thing, "\n\nAn imp that only you can see flashes into "+
         "existence.\n" );
   tell_object( thing, "The imp tells you: The magic and occult skill "+
         "categories are being amalgamated and restructured.\n" );
   tell_object( thing, "The imp tells you: You have a total of "+ total +
         " levels that you can spend on the new magic skills.\n" );
   tell_object( thing, "The imp tells you: Go and find the black "+
         "monolith in the middle of Sator Square which you can use to "+
         "to spend them.\n" );
   tell_object( thing, "The imp flashes back to wherever it came from.\n\n" );
} /* change_skills() */

/**
 * Figure out thge minimum amount of xp that could have been used to
 * get the skill level.  This assumes a leaf skill
 * This appears to be the xp equation.  Using it anyway...
 * @param lvl the level to get to
 * @return the skill cost in xp
 */
int query_minimum_skill_xp(int lvl) {
   int i;
   int xp;

   xp = 0;
   for (i = 0; i < lvl; i++) {
      xp += ( DEFAULT_COST * STD_COST * 100 ) / (( LEVEL_DIV + 1) * 500);
   }
   return xp;
} /* query_minimum_skill_xp() */

/**
 * Changes the skill array up to the new tree involving the
 * languages.
 * @param thing the object to upgrade
 * @param lev the level we are changing up to.
 */
void change_language_skills( object thing, int lev ) {
   int i;
   int total_xp;
   mapping skills;
   string *bits;
   string start;
   string *langs;
   string health;
   
   skills = thing->query_skills();
   if (!sizeof(skills))
      return ;
   bits = keys(skills);

   health = skills["other.health"];
  
   /*
    * Instead of changing the skill levels directly...  We will just
    * call the appropriate functions on the player object to fiddle
    * them.
    */ 
   start = "other.language.";
   total_xp = 0;
   for (i = 0; i < sizeof(bits); i++) {
      if (bits[i][0..strlen(start)-1] == start) {
         /* Ok, now figure out how much xp to give back... */
         total_xp += query_minimum_skill_xp(skills[bits[i]]);
         map_delete(skills, bits[i]);
      }
   }
   
   langs = thing->query_languages();
   for (i = 0; i < sizeof(langs); i++) {
      thing->add_language(langs[i]);
      thing->remove_language( langs[i] );
   }
   if (skills["other.language"]) {
      total_xp += query_minimum_skill_xp(skills["other.language"]);
      map_delete(skills, "other.language");
   }

   /* Everyone gets common... */
   thing->add_language("common");

   if(thing->query_guild_ob() == "/std/guilds/wizard") {
     thing->add_language("wizard spells");
   }
   if(thing->query_guild_ob() == "/std/guilds/thief") {
     thing->add_language("thieves' cant");
   }

   if(skills["other.health"] != health) {
     tell_object(thing, "A fluffy cabbage tells you: Something has gone wrong "
                 "with your language adjustment, please tell Ceres or "
                 "Pinkfish\n");
     return;
   }
     
   /* thing->set_skills( skills ); */
   thing->adjust_xp( total_xp );
   tell_object(thing,
"A fluffy cabbage turns up and stares straight into your eyes.\n"
"You feel that the fluffy cabbage has shuffled around some bits of your mind.\n"
               );

   if (total_xp > 0 && lev < 2) {
      tell_object(thing,
"Your languages have been shifted, you appear to have gained "+total_xp+" xp.\n"
                 );
   }
} /* change_language_skills() */

mapping recalc_parents(mapping skills, string skill) {
  int i, j, lvl;
  string *tree, *bits, this_skill;

  bits = explode(skill, ".");
  for(i=sizeof(bits)-1; i>=0; i--) {
    this_skill = implode(bits[0..i], ".");
    
    tree = (string *)SKILL_OB->query_immediate_children(this_skill);
    lvl = 0;
    for(j=0; j<sizeof(tree); j++) {
      lvl += skills[tree[j]];
    }

    if(sizeof(tree))
      skills[this_skill] = lvl / sizeof(tree);
  }
  return skills;
}

/**
 * Update the skills from the original skills system to the
 * beta skills system.
 * @param thing the person to update
 */
void change_skills_epsilon( object thing ) {
  int lvl, rest;
   mapping skills;
   string *bits;
   
   skills = thing->query_skills();
   if (!sizeof(skills))
      return ;
   bits = keys(skills);

   tell_object(thing, "A squirrel appears and says: I'm just updating your "
               "skill tree, will be just a jiffy.\n");

   // Do sleight-of-hand first.
   lvl = skills["covert.sleight-of-hand"];
   if(lvl > 0) {
     m_delete(skills, "covert.sleight-of-hand");

     if(lvl > 300) { // they get skills up to 300 at 2:1
       rest = lvl - 300;
       lvl = 300;
       if(rest > 600) { // 300-600 at 1:1
         lvl += 300;
         rest -= 600;
         lvl += rest / 5; // >600 at 4:10
       } else
         lvl += rest / 2;
     }
     
     skills["covert.manipulation.stealing"] = lvl;
     skills["covert.manipulation.palming"] = lvl;
     skills["covert.manipulation.passing"] = lvl;
     skills["covert.manipulation.sleight-of-hand"] = lvl;
     skills["covert.manipulation"] = lvl;
     
     recalc_parents(skills, "covert");
   }
   
   // then sharp
   lvl = skills["fighting.combat.melee.sharp"];
   if(lvl > 0) {

     if(lvl > 300) { // they get skills up to 300 at 2:1
       rest = lvl - 300;
       lvl = 300;
       
       if(rest > 600) { // 300-600 at 1:1
         lvl += 300;
         rest -= 600;
         lvl += rest / 5; // >600 at 4:10
       } else
         lvl += rest / 2;
     }
     
     skills["fighting.combat.melee.sharp"] = lvl;
     skills["fighting.combat.melee.pierce"] = lvl;
     skills = recalc_parents(skills, "fighting.combat.melee");
   }

   // then parry
   lvl = skills["fighting.combat.parry.thrown"];
   rest = skills["fighting.combat.parry.fired"];
   if(lvl > 0 || rest > 0) {
     if(rest > lvl)
       lvl = rest + (lvl / 5);
     else
       lvl += (rest / 5);
     
     m_delete(skills, "fighting.combat.parry.thrown");
     m_delete(skills, "fighting.combat.parry.fired");
     skills["fighting.combat.parry.range"] = lvl;
     
     lvl = skills["fighting.combat.parry.held"];
     m_delete(skills, "fighting.combat.parry.held");
     skills["fighting.combat.parry.melee"] = lvl;
     skills = recalc_parents(skills, "fighting.combat.parry");
   }

   // then special
   lvl = skills["fighting.combat.special"];
   if(lvl > 0) {
     
     if(lvl > 300) { // they get skills up to 300 at 2:1
       rest = lvl - 300;
       lvl = 300;
       
       if(rest > 600) { // 300-600 at 1:1
         lvl += 300;
         rest -= 600;
         lvl += rest / 5; // >600 at 4:10
       } else
         lvl += rest / 2;
     }
     skills["fighting.combat.special.weapon"] = lvl;
     skills["fighting.combat.special.unarmed"] = lvl;
     skills["fighting.combat.special.tactics"] = lvl - lvl / 4;
     
     skills = recalc_parents(skills, "fighting.combat.special");
   }
   // do tracking
   lvl = skills["other.tracking.woodland"];
   m_delete(skills, "other.tracking.woodland");
   lvl += skills["other.tracking.desert"];
   m_delete(skills, "other.tracking.desert");
   lvl += skills["other.tracking.mountain"];
   m_delete(skills, "other.tracking.mountain");
   lvl += skills["other.tracking.road"];
   m_delete(skills, "other.tracking.road");
   lvl += skills["other.tracking.field"];
   m_delete(skills, "other.tracking.field");

   if(lvl > 0)
     skills["other.tracking"] = lvl;
   
   thing->set_skills( skills );
   tell_object(thing, "The squirrel says: Ok, all done now.\n");
}

/**
 * Calulate the new skill value for the skills version zeta
 */
int new_skill_value(int sk) {
  float f = 1000.0;
  float k = 0.3;

  if(sk == 0)
    return 0;
  
  return to_int(f * log( 1.0 + sk/(f + sk * k) ) + 0.5);
}

/**
 * Change everyones skill levels (downwards).
 * @param thing the person to update
 */
void change_skills_zeta( object thing ) {
   mapping skills;
   string *bits, bit;
   
   skills = thing->query_skills();
   if (!sizeof(skills))
      return ;
   bits = keys(skills);

   tell_object(thing, "A small blue frog appears and says: I'm just updating "
         "your skill levels, this won't take a second.\n");

   // go through all the skills one by one
   foreach(bit in bits) {
     // if it's not a language skill & it's a leaf skill recalculate it and
     // then recalculate it's parents.
     if(bit[0..13] != "other.language" &&
        SKILL_OB->query_immediate_children(bit) == ({ })) {

       skills[bit] = new_skill_value(skills[bit]);
       skills = recalc_parents(skills, bit);
     }
   }
   // set these skills as their new skills and we're done.
   thing->set_skills( skills );
   tell_object(thing, "The frog says: Ok, all done now.\n");
}

void fixup_stats(object thing) {
  mapping values;
  mapping changes;
  int stat_total;
  string str;

  changes = ([ ]);
  values = ([
    "con" : thing->query_real_con(),
    "dex" : thing->query_real_dex(),
    "int" : thing->query_real_int(),
    "wis" : thing->query_real_wis(),
    "str" : thing->query_real_str()
            ]);
  stat_total = values["con"] + values["dex"] + values["wis"] + values["str"] +
               values["int"];
  if (stat_total > 65) {
     if (thing->query_real_con() > 8) {
        thing->adjust_con(-1);
        stat_total--;
        values["con"]--;
        changes["con"]++;
     }
     if (stat_total > 65 && thing->query_real_dex() > 8) {
        thing->adjust_dex(-1);
        stat_total--;
        values["dex"]--;
        changes["dex"]++;
     }
     // Now find highest stat and adjust until correct.
     while (stat_total > 65) {
        string highest;
        int value;
        string stat;
        int v;

        value = 0;
        foreach (stat, v in values) {
           if (v > value) {
              value = v;
              highest = stat;
           }
        }
        call_other(thing, "adjust_" + highest, -1);
        values[highest]--;
        changes[highest]++;
        stat_total--;
     }
  }

  str = query_multiple_short(map(keys(changes), (: $1 + " changed by " + $2[$1] :), changes));

  tell_object(thing, "The armadillo says: Stats changed: " + str + "\n");
}

void change_skills_eta(object thing) {
  mapping skills;
  string bit, effect;
  class tactics tactics;
  int eff;

  // Initialise tactics since we have some new fields.
  tactics = new(class tactics,
                attitude : "neutral",
                response : "neutral",
                parry : "both",
                parry_unarmed : 0,
                mercy : "ask",
                focus_zone : 0,
                ideal_distance : 0);
  thing->set_tactics(tactics);
  
  skills = thing->query_skills();
  if(!sizeof(skills))
    return;

  tell_object(thing, "An armadillo appears out of nowhere and says: "
              "I'm just updating your skills, this won't take a second.\n");
  
  foreach(bit in keys(ETA_CHANGES)) {
    tell_object(thing, "Mapping " + bit + " to " + ETA_CHANGES[bit] + ".\n");
    skills[ETA_CHANGES[bit]] = skills[bit];
    map_delete(skills, bit);
  }

  tell_object(thing, "Adding new skills.\n");
  foreach(bit in
          "/std/skills"->query_immediate_children("crafts")) {
    if(!skills[bit]) {
      skills[bit] = 0;
    }
  }
  recalc_parents(skills, "crafts.hunting");
  recalc_parents(skills, "crafts");
  recalc_parents(skills, "other.movement.riding");
  recalc_parents(skills, "other.movement.climbing");
  recalc_parents(skills, "other.movement");
  recalc_parents(skills, "other.trading.valueing");
  recalc_parents(skills, "other.trading");
  recalc_parents(skills, "other");
  thing->set_skills( skills );

  foreach(effect in EFFECTS)
    foreach(eff in thing->effects_matching(effect))
      catch(thing->delete_effect(eff));

  tell_object(thing, "Fixing stats.\n");
  fixup_stats(thing);

  tell_object(thing, "Granting rearrange.\n");
   thing->add_known_command("rearrange");

  if ((thing->query_guild_ob() == "/std/guilds/assassin" &&
       thing->query_property("assassin") > 1) ||
      (thing->query_guild_ob() == "/std/guilds/wizard" &&
       "/obj/handlers/guild_things/wizard_orders"->query_order() == "last")) {
    tell_object(thing, "Returning PK flag.\n");
    thing->set_player_killer(1);
  }
  thing->remove_property(PKER);
  thing->remove_property(PKER_MISSPELL);
  thing->add_property("not real newbie", 1);

  tell_object(thing, "Removing old combat commands.\n");
  if(thing->query_known_command("strike"))
    thing->remove_known_command("strike");

  if(thing->query_guild_ob() != "/std/guilds/warrior") {
    thing->remove_known_command("impale");
    thing->remove_known_command("crush");
    thing->remove_known_command("behead");
  }
  
  thing->remove_known_command("punch");
  thing->remove_known_command("kick");

  tell_object(thing, "The armadillo says: Ok, all done now.\n");
  "/d/liaison/NEWBIE/choose_land"->fixup_player(this_player());
}

/**
 * Called by the login handler, checks to see if an upgrade
 * is neede.
 * @param name the name of the player logging in
 * @param type the type of the login
 * @see /obj/handlers/login_handler.c
 */
void entering( string name, string type ) {
   object thing;
   int lvl;

   if ( type != LOGIN )
      return;
   if ( !( thing = find_player( name ) ) )
      return;

   switch((string)thing->query_property( "skills version" )) {
   case VERSION:
     change_skills( thing );
   case VERSION_2:
     change_language_skills( thing, lvl );
   case VERSION_3:
     change_skills_epsilon( thing );
   case VERSION_4:
     change_skills_zeta(thing);
   case VERSION_5:
     change_skills_eta(thing);
   default:
     // If they don't have a skill version then they're a new player and
     // don't need any modification to their skill tree.
     thing->add_property("skills version", VERSION_6);
     return;
   }
} /* entering() */

/**
 * The current version of the skill tree.
 * @return the version of the skill system
 */
string query_version() { return VERSION_5; }
