/*  -*- LPC -*-  */
#include <money.h>

inherit "/cmds/base";

private mapping _money_types;

void create()
{
   string zone;
   string *zones;

   zones = MONEY_HAND->query_all_places();
   _money_types = allocate_mapping(sizeof(zones));
   foreach(zone in zones) {
      _money_types[lower_case(zone)] = zone;
   }
   ::create();
   return;
}                               /* create() */


int cmd(string str,
        int brief,
        object * containers)
{
   object *monies;
   object new_money;
   object *divided_money;
   object money;
   object tmp_money;
   object *inv;
// mixed *tmp_arr;
   mixed *total;
// object env;
   int success;
// string tmp;
// int i;
// int j;

   if (this_player()->
          check_dark(environment(this_player())->query_light()) < 0)
   {
      add_failed_mess("It's too dark to count your money.\n");
      return 0;
   }

   if (str) {
      if (str == "here") {
         if (environment(this_player())->query_bank_name())
            str = environment(this_player())->query_money_place();
         else
            str = environment(this_player())->query_property("place");
         if (!str) {
            add_failed_mess("No idea what type of currency is used here.\n");
            return 0;
         }
      }
      str = lower_case(str);
      if (!_money_types[str]) {
         add_failed_mess("This is not a valid type of money, try one of " +
            query_multiple_short(keys(_money_types) + ({ "here" })) + ".\n");
         return 0;
      }
   }

// if (!containers) {
//    inv = deep_inventory(this_player());
// } else {
//    inv = filter(containers,
//                 (: $1->query_closed()  &&  !$1->query_transparent() :));
//    if (sizeof(inv))  {
//       write(query_multiple_short(inv, "one") +
//             (sizeof(inv) > 1 ? " are " : " is ") + "closed.\n");
//       containers -= inv;
//    }
//    if (sizeof(containers))  {
//       inv = ({ });
//       foreach(env in containers) {
//          inv += deep_inventory(env);
//       }
//    }
//    else return 1;
// }
   inv = all_inventory(this_player());
   monies = filter_array(inv, (: this_object()->find_the_dough($1) :));
   total = ({ });
   if (sizeof(monies)) {
      success = 0;
      divided_money = ({ 0, 0 });
      foreach(money in monies) {
         tmp_money = 0;
         if (!str) {
            tmp_money = money;
         } else if (str && _money_types[str]) {
            new_money = MONEY_VAULT->get_money_ob();
            new_money->set_money_array(money->query_money_array());
            divided_money = MONEY_HAND->
               filter_legal_tender(new_money, _money_types[lower_case(str)]);
            tmp_money = divided_money[0];
            if (divided_money[1]) {
               divided_money[1]->dest_me();
            }
         }
         if (tmp_money) {
//          if (environment(money) == this_player()) {
               if (!brief) {
                  write("Your purse contains " +
                        query_multiple_short(tmp_money->half_short(1)) +
                        ".\n");
               }
               else  {
                  write("Your purse contains " +
                     query_multiple_short(tmp_money->half_symbol_short()) +
                     ".\n");
               }
//            }
//          else if (!brief) {
//             env = environment(money);
//             tmp = env->poss_short();
//             env = environment(env);
//             while (env != this_player()) {
//                tmp += " in " + env->poss_short();
//                env = environment(env);
//             }
//             write(tmp + " contains " +
//                   query_multiple_short(tmp_money->half_short(1)) + ".\n");
//          }
            success++;
//          tmp_arr = tmp_money->query_money_array();
//          for (i = 0; i < sizeof(tmp_arr); i += 2) {
//             j = member_array(tmp_arr[i], total);
//             if (j != -1) {
//                total[j + 1] += tmp_arr[i + 1];
//             } else {
//                total += tmp_arr[i..i + 1];
//             }
//          }
            if (divided_money[0]) {
               divided_money[0]->dest_me();
            }
         }
      }
      if (!success) {
         write("You have no money from the Land of " + capitalize(str) +
               ".\n");
      }
//    else if (success > 1 || brief) {
//       write("You have a total of " +
//             MONEY_HAND->money_string(total) + ".\n");
//    }
// } else if (!containers) {
//    write("You have no money that you can see.\n");
// } else {
//    if (!str) {
//       write("You find no money in " +
//             query_multiple_short(containers, "poss") + ".\n");
//    } else {
//       write("You find no money from " + _money_types[str] +
//             "in " + query_multiple_short(containers, "poss") + ".\n");
//    }
   }
   else write("You are flat broke.\n");
   return 1;

} /* cmd() */


int find_the_dough(object env)
{
   if (member_array(MONEY_ALIAS, env->query_alias()) > -1) {
      while ((env = environment(env)) != this_player()) {
         if (env->query_closed() && !env->query_transparent()) {
            return 0;
         }
      }
      return 1;
   } else
      return 0;

} /* find_the_dough() */


mixed *query_patterns()
{
   return ({ "", (: cmd(0, 0, 0) :),
             "brief", (: cmd(0, 1, 0) :),
             "<string'area name'>", (: cmd($4[0], 0, 0) :),
             "<string'area name'> brief", (: cmd($4[0], 1, 0) :),
//             "<string'area name'> in <indirect:object:me'container'>",
//             (: cmd($4[0], 0, $1) :),
//             "<string'area name'> in <indirect:object:me'container'> brief",
//             (: cmd($4[0], 1, $1) :),
//             "in <indirect:object:me'container'>", (: cmd(0, 0, $1) :),
//             "in <indirect:object:me'container'> brief",
//             (: cmd(0, 1, $1) :
          });
} /* query_patterns() */
