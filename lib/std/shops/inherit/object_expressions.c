/**
 * This is the file with all the functions that get information
 * about objects.  All of these methods assume they are called
 * with the following data:  seller, objects
 * @author Pinkfish
 * @started Fri Jun  1 21:37:14 PDT 2001
 */
#include <expressions.h>
#include <player_handler.h>

#define CHARGE_MULT 10 /* This is out of /obj/weapons/maces/cleric_rod.c */

string query_owner();
string* query_allowed();

void add_allowed_function(string name, int type, int* args, function value);
void add_allowed_variable(string name, int type, function value);

private int function_object_base_value(object* obs) {
   int value;
   object ob;
   int tmp;

   value = 1000000000;
   foreach (ob in obs) {
      tmp = ob->query_base_value();
      if (tmp < value) {
         value = tmp;
      }
   }
   return value;
} /* function_object_base_value() */

private int function_object_value(object* obs) {
   int value;
   object ob;
   int tmp;

   value = 1000000000;
   foreach (ob in obs) {
      tmp = ob->query_value();
      if (tmp < value) {
         value = tmp;
      }
   }
   return value;
} /* function_object_value() */

private int function_object_condition(object* obs) {
   int cond;
   object ob;
   int tmp;
   int max;

   cond = 100;
   foreach (ob in obs) {
      tmp = ob->query_cond();
      max = ob->query_max_cond();
      if (max) {
         tmp = tmp * 100 / max;
         if (tmp < cond) {
            cond = tmp;
         }
      }
   }
   return cond;
} /* function_object_condition() */

private int function_object_enchant(object*obs) {
   int enchant;
   object ob;
   int tmp;
   int max;
   string bing;

   if (!sizeof(obs)) {
      return 0;
   }

   //
   // First check to see if any of the owners can actually detect this.
   //
   if (!PLAYER_HANDLER->test_guild(query_owner())->query_see_octarine()) {
      foreach (bing in query_allowed()) {
         if (PLAYER_HANDLER->test_guild(bing)->query_see_octarine()) {
            tmp = 1;
         }
      }
      if (!tmp) {
         return 0;
      }
   }

   enchant = 100;
   foreach (ob in obs) {
      tmp = ob->query_enchant();
      max = ob->query_max_enchant();
      tmp = tmp * 100 / max;
      if (tmp < enchant) {
         enchant = tmp;
      }
   }
   return enchant;
} /* function_object_enchant() */

private string function_object_type(object* obs) {
   string type;
   string old_type;
   object ob;

   if (!sizeof(obs)) {
      return 0;
   }

   //
   // First check to see if any of the owners can actually detect this.
   //
   foreach (ob in obs) {
      if (ob->query_weapon()) {
         type = "weapon";
      } else if (ob->query_armour()) {
         type = "armour";
      } else if (ob->query_clothing()) {
         type = "clothing";
      } else {
         type = "misc";
      }
      if (old_type && old_type != type) {
         type = "mixed";
      }
      old_type = type;
   }

   return type;
} /* function_object_type() */

private object* function_object_stolen(object* obs) {
   obs = filter(obs, (: $1->query_property("stolen") :));
   return obs;
} /* function_object_stolen() */

private object* function_object_warded(object* obs) {
   string classification;

   classification = "/std/effects/object/ward"->query_classification();
   obs = filter(obs, (: sizeof($1->effects_matching($2)) :), classification);
   return obs;
} /* function_object_warded() */

private object* function_object_contains_spell(object* obs, string spell_name) {
   string bing;
   int tmp;

   if (!sizeof(obs)) {
      return 0;
   }

   //
   // First check to see if any of the owners can actually detect this.
   //
   if (!PLAYER_HANDLER->test_guild(query_owner())->query_see_spells()) {
      foreach (bing in query_allowed()) {
         if (PLAYER_HANDLER->test_guild(bing)->query_see_spells()) {
            tmp = 1;
         }
      }
      if (!tmp) {
         return 0;
      }
   }

   obs = filter(obs, (: $1->query_magic_scroll() &&
           ($2 == "any" || lower_case($1->query_spell_name()) == lower_case($2)) :), spell_name);
   return obs;
} /* function_object_contains_spell() */

private object* function_object_contains_imbue(object* obs, string ritual_name) {
   if (!sizeof(obs)) {
      return ({ });
   }


   obs = filter(obs, (: $1->query_faith_imbue() &&
      ($2 == "any" ||
       lower_case($1->query_ritual_name()) == lower_case($2)) :), ritual_name );
   return obs;
} /* function_object_contains_ritual() */

private int function_object_percentage_liquid(object* obs, string liquid_name) {
   object ob;
   object* liquids;
   object liq;
   int volume;
   int found;
   int tmp;

   if (!sizeof(obs)) {
      return 0;
   }

   volume = 100;
   foreach (ob in obs) {
      //
      // See what liquids are in the object
      //
      liquids = filter(all_inventory(ob), (: $1->query_liquid() :));
      if (sizeof(liquids))  {
         foreach (liq in liquids) {
            if (liq->query_short() &&
                lower_case(liq->query_short()) == lower_case(liquid_name)) {
               tmp = liq->query_weight() * 100 / ob->query_max_weight();
               if (tmp < volume) {
                  volume = tmp;
                  found = 1;
               }
            }
         }
      }
      // Problem: some things (like healing vials) don't actually have
      // anything in them.  So we have to do something different.
      else if (ob->query_liquid_short() &&
               lower_case(ob->query_liquid_short()) ==
               lower_case(liquid_name))
      {
         tmp = ob->query_water_volume() * 100 / ob->query_max_volume();
         if (tmp < volume)  {
            volume = tmp;
            found = 1;
         }
      }
   }
   if (found) {
      return volume;
   }
   return 0;
} /* function_object_percentage_liquid() */

private int function_object_charges(object* obs) {
   int total = 0;
   int possible = 0;
   int charges;
   mixed info;
   object ob;

   foreach (ob in obs)  {
      charges = ob->query_charges();
      if (charges)  {
         info = ob->query_static_auto_load();
         if (pointerp(info)  &&  sizeof(info) == 2  &&  info[0] == charges)  {
            possible += info[1] * CHARGE_MULT;
            total += charges;
         }
      }
   }
   if (possible)
      return 100 * total / possible;
   else
      return 0;
} /* function_object_charges() */

private object* function_object_matching(object* obs, string match) {
   object ob;
   string *bits;
   string bit;
   object* ret;

   bits = explode(match, ",");
   ret = ({ });
   foreach (ob in obs)  {
      foreach(bit in bits)   {
         if (ob && ob->full_id(bit)) {
            ret += ({ ob });
         }
      }
   }
   return ret;
} /* function_object_matching() */

private object* function_object_short(object* obs, string match) {
   string* bits;
   object* result;

   bits = explode(match, ",");
   result = filter(obs, (: member_array(strip_colours($1->query_short()), $2) != -1 :),
                   bits);
   return result;
} /* function_object_short() */

private object* function_contents(object ob) {
   if (!ob) {
      return ({ });
   }
   return all_inventory(ob);
} /* function_contents() */

/**
 * This method adds all the object base functions.
 */
void create() {
   if (!function_exists("add_allowed_function", this_object())) {
      return ;
   }
   add_allowed_function("objectenchant", EXPRESSION_TYPE_INTEGER,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET }),
                        (: function_object_enchant :) );
   add_allowed_function("objectvalue", EXPRESSION_TYPE_MONEY,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET }),
                        (: function_object_value :) );
   add_allowed_function("objectbasevalue", EXPRESSION_TYPE_MONEY,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET }),
                        (: function_object_base_value :) );
   add_allowed_function("objectcondition", EXPRESSION_TYPE_INTEGER,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET }),
                        (: function_object_condition :) );
   add_allowed_function("objecttype", EXPRESSION_TYPE_STRING,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET }),
                        (: function_object_type :) );
   add_allowed_function("objectwarded", EXPRESSION_TYPE_OBJECT + EXPRESSION_TYPE_ARRAY_OFFSET,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET }),
                        (: function_object_warded :) );
   add_allowed_function("objectstolen", EXPRESSION_TYPE_OBJECT + EXPRESSION_TYPE_ARRAY_OFFSET,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET }),
                        (: function_object_stolen :) );
   add_allowed_function("charges", EXPRESSION_TYPE_INTEGER,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET }),
                        (: function_object_charges :) );
   add_allowed_function("containsspell", EXPRESSION_TYPE_OBJECT + EXPRESSION_TYPE_ARRAY_OFFSET,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET,
                           EXPRESSION_TYPE_STRING }),
                        (: function_object_contains_spell :) );
   add_allowed_function("containsimbue", EXPRESSION_TYPE_OBJECT + EXPRESSION_TYPE_ARRAY_OFFSET,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET,
                           EXPRESSION_TYPE_STRING }),
                        (: function_object_contains_imbue :) );
   add_allowed_function("percentageliquid", EXPRESSION_TYPE_INTEGER,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET,
                           EXPRESSION_TYPE_STRING }),
                        (: function_object_percentage_liquid :) );
   add_allowed_function("objectmatch", EXPRESSION_TYPE_OBJECT + EXPRESSION_TYPE_ARRAY_OFFSET,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET,
                           EXPRESSION_TYPE_STRING }),
                        (: function_object_matching :) );
   add_allowed_function("objectshort", EXPRESSION_TYPE_OBJECT + EXPRESSION_TYPE_ARRAY_OFFSET,
                        ({ EXPRESSION_TYPE_OBJECT +
                           EXPRESSION_TYPE_ARRAY_OFFSET,
                           EXPRESSION_TYPE_STRING }),
                        (: function_object_short :) );
   add_allowed_function("contents", EXPRESSION_TYPE_OBJECT +
                                    EXPRESSION_TYPE_ARRAY_OFFSET,
                        ({ EXPRESSION_TYPE_OBJECT }),
                        (: function_contents :) );
} /* create() */
