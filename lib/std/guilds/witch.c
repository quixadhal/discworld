#include "skills.h"

#define WITCH_SPELL "/std/spells/witch/"
inherit "/std/guilds/standard";

setup()
{
   set_name("witch");
   set_short("guild of witches");
   set_long("The guild of witches is based around knowledge of the occult.  "+
            "Witches must be adept at making arcane brews, dancing naked around fires "+
            "and generally doing weird things.");
   reset_get();
   remove_spell("missile");
   add_spell("valkyrie jiggle", WITCH_SPELL + "m/b/vjiggle", "start", 0);
   add_spell("orma's stir", WITCH_SPELL + "m/b/ostir", "start", 0);
   add_spell("felstaff mix", WITCH_SPELL + "m/b/fmix", "start", 0);
   add_spell("greater eros deiamind stir", WITCH_SPELL + "m/b/gedstir", "start", 0);
   add_spell("common magical mix", WITCH_SPELL + "m/b/cmmix", "start", 0);
   add_spell("witch wibble", WITCH_SPELL + "m/b/wwibble", "start", 0);
}

query_skill_cost(s)
{
   string *bits;

   bits = explode(s, ".");
   switch(bits[0]) {
   case "fighting":
      return 15;
   case "occult":
      return 3;
   case "magic":
      return 8;
   case "faith":
      return 7;
   default:
      return 10;
   }
}

query_skill_max_level(s)
{
   string *bits;

   bits = explode(s, ".");
   if (bits[0] == "occult") return 300;
   if (bits[0] == "fighting") {
      if (sizeof(bits) > 1 &&  bits[1] == "points") return 10;
      return 5;
   }
   return 10;
}

set_gp(ob)
{
   int max_gp;

   max_gp = ob->query_skill_bonus("occult.points");
   ob->set_max_gp(max_gp + 60);
}

query_title(player)
{
   int lev,fem;

   lev = player->query_skill("occult");
   fem = player->query_gender() == 2;
   if (lev < 3) return "the herb gatherer";
   if (lev < 8) return (fem) ? "the breweress" : "the brewer";
   if (lev < 14) return (fem) ? "the brew mistress" : "the brew master";
   if (lev < 20) return "the fortune teller";
   if (lev < 24) return (fem) ? "the woman of destinies" : "the man of destinies";
   if (lev < 31) return (fem) ? "the maid of the dance" : "the boy of the dance";
   if (lev < 35) return (fem) ? "the lady of the dance" : "the lord of the dance";
   if (lev < 38) return (fem) ? "the wyrd sister" : "the wyrd brother";
   if (lev < 42) return "the devotee of the tree";
   if (lev < 47) return (fem) ? "the mistress of the forest" : "the master of the forest";
   if (lev < 50) return (fem) ? "the junior witch" : "the junior warlock";
   if (lev < 54) return (fem) ? "the witch" : "the warlock";
   if (lev < 56) return (fem) ? "the senior witch" : "the senior warlock";
   if (lev < 61) return (fem) ? "the grand witch" : "the grand warlock";
   return (fem) ? "the granny" : "the grandad";
}

query_level(ob)
{
   return ob->query_skill("occult");
}

set_new_level(arr)
{
   object ob;
   int level;

   ob = arr[0];
   level = arr[1];

   ob->add_skill_level("occult", level);
   ob->add_skill_level("fighting", level);
   ob->add_skill_level("magic", level);
   ob->add_skill_level("faith", level);
   ob->add_skill_level("other", level);
   ob->adjust_str(11);
   ob->adjust_dex(9);
   ob->adjust_int(13);
   ob->adjust_con(13);
   ob->adjust_wis(19);
   ob->set_hp(10000);
   ob->set_gp(10000);
   ob->reset_all();
}

int query_see_octarine()
{
  return 1;
}

start_player()
{
   /* this at the mo is just to mask the function so spells are not auto 
      matically acquired */
}
