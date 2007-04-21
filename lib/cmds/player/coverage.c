/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: coverage.c,v 1.5 2003/02/21 01:28:10 pinkfish Exp $
 *
 *
 */

inherit "/cmds/base.c";

#define CLOTHING_HANDLER "/obj/handlers/clothing_handler"

private string _zone_patterns;

int cmd(object *items, int inverse, string bits)  {
   object   armor;
   object  *wearing;
   object  *fail = ({ });
   object  *armors;
   object  *covering;
   string  *types;
   string   equiv_type;
   string   type;
   string  *zones;
   string  *all_zones;
   string  *parts;
   string   zone;
   string   verb;
   string   mess;
   mapping  covered;
   mapping  pot_covered;

   all_zones = keys(CLOTHING_HANDLER->query_all_clothing_zones());
   covered = allocate_mapping(sizeof(all_zones));
   pot_covered = allocate_mapping(sizeof(all_zones));
   foreach (zone in all_zones)  {
      covered[zone] = ({ });
      pot_covered[zone] = ({ });
   }

   if (sizeof(items) == 0)
      armors = this_player()->query_armours();
   else
      armors = items;

   fail = filter(armors,
                 (: !$1->query_armour()  &&  !$1->query_clothing() :));
   armors -= fail;
   wearing = this_player()->query_wearing();

   if (bits)  {
      bits = replace(bits, " ", ",");
      bits = replace(bits, ",and,", ",");
      zones = explode(bits, ",") - ({ "" });
      parts = filter(zones, (: member_array($1, $(all_zones)) != -1 :));
      if (sizeof(parts) == 0)  {
         write("Please choose some combination of " +
               query_multiple_short(all_zones) + ".\n");
         return 1;
      }
   }

   foreach (armor in armors)  {
      if (!arrayp(armor->query_type()))
         types = ({ armor->query_type() });
      else
         types = armor->query_type();

      zones = ({ });
      foreach (type in types)  {
         equiv_type = CLOTHING_HANDLER->query_equivilant_type(type);
         if (equiv_type)
            zones += CLOTHING_HANDLER->query_zone_names(equiv_type);
         else
            zones += CLOTHING_HANDLER->query_zone_names(type);
      }

      if (sizeof(zones))  {
         if (member_array(armor, wearing) > -1)
            foreach (zone in zones)
               covered[zone] += ({ armor });
         else
            foreach (zone in zones)
               pot_covered[zone] += ({ armor });
      }
      else fail += ({ armor });
   }

   if (bits)  {
      covered = filter(covered, (: member_array($1, $(parts)) != -1 :));
      pot_covered = filter(pot_covered,
                           (: member_array($1, $(parts)) != -1 :));
   }

   if (inverse)  {
      zones = filter(all_zones, (: sizeof($(covered)[$1]) == 0 :));
      if (sizeof(zones) > 0)  {
         if (sizeof(zones) > 1  ||
             zones[0] == "arms"  ||  zones[0] == "hands"  ||
             zones[0] == "legs"  || zones[0] == "feet")
            verb = " are";
         else
            verb = " is";
         write("Your " + query_multiple_short(zones) + verb +
               " unprotected.\n");
      }
      else write("You are covered from head to foot.\n");
      return 1;
   }

   zones = sort_array(keys(covered) | keys(pot_covered), 1);
   foreach (zone in zones)  {
      if (zone == "arms"  ||  zone == "hands"  ||
          zone == "legs"  ||  zone == "feet")
         verb = " are";
      else
         verb = " is";
      if (sizeof(covering = covered[zone]) > 0)  {
         if (sizeof(covering) == 1)
            mess = "Your " + zone + verb + " protected by " +
                   covering[0]->one_short();
         else
            mess = "Your " + zone + verb + " protected by " +
                   query_num(sizeof(covering)) + " things, " +
                   query_multiple_short(covering, "one");
         if (sizeof(pot_covered[zone]) > 0)  {
            mess += " and could be protected by " +
                    query_multiple_short(pot_covered[zone], "one") + ".\n";
            pot_covered[zone] = ({ });
         }
         else mess += ".\n";
         write(mess);
      }
      else if (sizeof(covering = pot_covered[zone]) > 0)  {
         write("Your " + zone + " could be protected by " +
               query_multiple_short(covering, "one") + ".\n");
      }
      else if (bits) write("Your " + zone + verb + " unprotected.\n");
   }

   if (sizeof(fail) != 0  &&  sizeof(items) != 0)  {
      if (sizeof(fail) == 1  &&  fail[0] == this_player())
         write("You don't offer any protection.  Perhaps you should invest "
               "in some sonkies?\n");
      else
         write(query_multiple_short(fail, "the") +
               (sizeof(fail) == 1 ? " doesn't" : " don't") +
               " offer any protection.\n");
   }
   else if (sizeof(armors - fail) == 0) /* Everything failed! */
      write("You are completely unprotected.  Good luck!\n");

   return 1;
}


mixed *query_patterns()  {
   return ({ "", (: cmd(({ }), 0, 0) :),
             "[by] <indirect:object'armour/clothing'>", (: cmd($1, 0, 0) :),
             "unprotected", (: cmd(({ }), 1, 0) :),
             "of <string'body part(s)'>", (: cmd(({ }), 0, $4[0]) :) });
}
