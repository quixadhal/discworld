#include <money.h>

inherit "/cmds/base";

#define MAX_COUNT_SIZE 50

int is_not_in_other_player(object thing);

int cmd(object *things, int brief)  {
   string  list;
   string *aliases;
   string  message;
   mixed  *stuff;
   object  thing;
   object  money;
   object  sack;
   object *continuous, *collective;
   object *room_items;
   object *tmp;
   object tmp_ob;
   int     i;
   int     total;

   if (this_player()->
       check_dark(environment(this_player())->query_light()) < 0) {
      add_failed_mess("It's too dark to count anything.\n");
      return 0;
   }
   if(sizeof(things) > MAX_COUNT_SIZE)
     return add_failed_mess("You cannot count that many things!\n");

   things = filter(things, (: !is_not_in_other_player($1) :));
   if (!sizeof(things))  {
      write("You can't count things carried by other people.\n");
      return 1;
   }

   money = 0;
   total = 0;
   list = "";

   foreach (thing in things)  {
      aliases = thing->query_alias();
      if (aliases  &&  member_array(MONEY_ALIAS, aliases) > -1)  {
         if (!money) {
            money = clone_object(MONEY_OBJECT);
         }
         money->adjust_money(thing->query_money_array());
         things -= ({ thing });
      }
   }
   if (money)  {
      total += money->query_number_coins();
      if (sizeof(things))  {
         list = implode(money->half_short(1), ", ");
         if (sizeof(things) == 1) {
            list += " and ";
         } else {
            list += ", ";
         }
      } else {
         list = query_multiple_short(money->half_short(1));
      }
      money->dest_me();
   }

   continuous = filter(things, (: $1->query_continuous() :));
   if (sizeof(continuous))  {
      sack = clone_object("/std/container");
      foreach (thing in continuous) {
         thing->make_medium_clone(thing->query_amount())->move(sack);
      }
      things -= continuous;
      continuous = all_inventory(sack);
      foreach (thing in continuous)  {
         total += thing->query_how_many();
      }
      if (sizeof(things))  {
         list += implode(map(continuous,
                 (: $1->amount_size(1) + " of " + $1->short() :)), ", ");
         if (sizeof(things) == 1) {
            list += " and ";
         } else {
            list += ", ";
         }
      } else {
         list += query_multiple_short(map(continuous,
                 (: $1->amount_size(1) + " of " + $1->short() :)));
      }
      sack->dest_me();
   }
   collective = filter(things, (: $1->query_collective() :));
   if (sizeof(collective))  {
     sack = clone_object("/std/container");
     foreach (thing in collective) {
       if (thing) {
          tmp_ob = thing->make_medium_clone(thing->query_amount());
          if (tmp_ob) {
             tmp_ob->move(sack);
          }
       }
     }
     things -= collective;
     collective = all_inventory(sack);
     foreach (thing in collective) {
       total += thing->query_amount();
     }
      
     if(sizeof(things)) {
       list += implode(map(collective,
                           (: $1->query_amount() + " " +
                            ($1->query_amount() == 1 ? $1->short() :
                             $1->query_plural()) :)),
                       ", ");
       if (sizeof(things) == 1) {
         list += " and ";
       } else {
         list += ", ";
       }
     } else {
       list += query_multiple_short(map(collective,
                                        (: $1->query_amount() + " " +
                                         ($1->query_amount() == 1 ?
                                          $1->short() :
                                          $1->query_plural()) :)));
     }
     sack->dest_me();
   }

   room_items = filter(things, (: strsrch(file_name($1),
                                          "/std/room/basic/item") > -1 :));
   if (sizeof(room_items))  {
      things -= room_items;
      tmp = filter(room_items, (: $1->query_count() :));
      room_items -= tmp;
      if (sizeof(things))  {
         if (sizeof(tmp)) {
            list += implode(map(tmp, (: query_num($1->query_count()) +
                    " " + ($1->query_count() > 1 ? $1->pretty_plural()
                                                 : $1->pretty_short()) :)),
                    ", ");
            foreach (thing in tmp)  {
               total += thing->query_count();
            }
         }
         if (sizeof(room_items)) {
            list += implode(map(room_items, (: "at least one " +
                                               $1->pretty_short() :)), ", ");
            total += sizeof(room_items);
         }
         if (sizeof(things) == 1) {
            list += " and ";
         } else {
            list += ", ";
         }
      } else  {
         if (sizeof(tmp)) {
            list += query_multiple_short(map(tmp,
                    (: query_num($1->query_count()) + " " +
                    ($1->query_count() > 1 ? $1->pretty_plural()
                                           : $1->pretty_short()) :)), ", ");
            total += sizeof(tmp);
         }
         if (sizeof(room_items)) {
            list += query_multiple_short(map(room_items,
                    (: "at least one " + $1->pretty_short() :)));
            total += sizeof(room_items);
         }
      }
   }
   if (sizeof(things))  {
      list += query_multiple_short(things, "one");

if (this_player() == find_player("presto"))
tell_creator("presto", "list == %s\n", list);
      stuff = this_player()->reform_message(list, ({}));
if (this_player() == find_player("presto"))
printf("test message: %O\n", this_player()->evaluate_message(stuff));
      message = stuff[0];
      for (i = 0; i < sizeof(stuff[1]); i++)  {
         message = replace_string(message, "$" + i + "$",
            this_player()->calc_shorts(stuff[1][i], 1));
      }
      total += sizeof(things);
   } else {
      message = list;
   }

   if (brief) {
      write("You count " + total + " items.\n");
   } else {
      write("You count " + message + " with a total of " +
            query_num(total) + (total > 1 ? " items.\n" : " item.\n"));
   }
   return 1;
}

int is_not_in_other_player(object env)  {
   while ((env = environment(env))  &&  !living(env))
      ;

   if (env  &&  env != this_player())
      return 1;
   else
      return 0;
}


mixed *query_patterns()  {
   return ({ "<indirect:object:me-here'things [in <container>]'>",
             (: cmd($1, 0) :),
             "<indirect:object:me'things'> in inventory", (: cmd($1, 0) :),
             "<indirect:object:here'things'> in room", (: cmd($1, 0) :),
             "brief <indirect:object:me-here'things [in <container>]'>",
             (: cmd($1, 1) :),
             "brief <indirect:object:me'things'> in inventory", (: cmd($1, 1) :),
             "brief <indirect:object:here'things'> in room", (: cmd($1, 1) :) });
}
