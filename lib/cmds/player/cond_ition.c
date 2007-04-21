/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: cond_ition.c,v 1.17 2003/07/11 00:24:45 pinkfish Exp $
 * 
 */
inherit "/cmds/base";

class cond_info {
   object cond_item;
   string cond_string;
   int cond_percent;
}

int compare(float, float);

string cond_colour(int percent)
{
   switch (100 - percent) {
   case 0..50:
      return "";
   case 51..60:
      return "%^CYAN%^";
   case 61..70:
      return "%^GREEN%^";
   case 71..80:
      return "%^YELLOW%^";
   case 81..90:
      return "%^RED%^";
   case 91..100:
      return "%^BOLD%^%^RED%^";
   }
}                               /* cond_colour() */

int is_valid_environment(object ob) {
   object env;

   env = environment(ob);
   if (env == environment(this_player())) {
      return 1;
   }
   while (env) {
      if (env == this_player()) {
         return 1;
      }
      env = environment(env);
   }
   return 0;
}

int cmd(object *things, int dir, int no_excellent)
{
   int i;
   string list;
   object thing;
   class cond_info *info = ({ });
   class cond_info new_info;
   int dark;

   if (!this_player()->query_property("dead")) {
      dark =
         this_player()->check_dark(environment(this_player())->query_light());
   }

   if (dark == 2) {
      add_failed_mess("It is way too bright to see anything at all.\n");
      return 0;
   }
   if (dark == -2) {
      add_failed_mess("It is way too dark to see anything at all.\n");
      return 0;
   }

   things = filter(things, (: is_valid_environment :));
   if (!sizeof(things)) {
      add_failed_mess("Can only check the condition of things you "
                      "have in your inventory or on the ground.\n");
      return 0;
   }

   if (sizeof(things) == 1) {
      list = things[0]->cond_string();
      if (!list || list == "") {
         list = " has no condition.\n";
      } else {
         if (things[0]->query_is_pair())  list = list[4 .. ];
         else list = list[2..];
      }
      write("$C$" + things[0]->the_short() + list);
      return 1;
   }

   foreach(thing in things) {
      list = thing->cond_string();
      if (list && list != "") {
       if (thing->query_is_pair())  list = list[4 .. ];
       else list = list[2 .. ];
       info += ({ new (class cond_info,
                       cond_item: thing,
                       cond_string: list,
                       cond_percent:(thing->query_cond() * 100) /
                         thing-> query_max_cond()
                    )
            });
      }
   }
   if (!sizeof(info)) {
      add_failed_mess("None of those things has a condition.\n");
      return 0;
   } else if (sizeof(info) == 1) {
      write("$C$" + info[0]->cond_item->the_short() + info[0]->cond_string);
   } else {
      if (dir) {
         info = sort_array
            (info, (: $(dir) * compare($1->cond_percent, $2->cond_percent) :));
      }
      i = 0;
      foreach(new_info in info) {
         if (!no_excellent || new_info->cond_percent <= 90) {
            write(cond_colour(new_info->cond_percent) + "$C$" +
                  new_info->cond_item->the_short() + " (" + (i + 1) + ")" +
                  new_info->cond_string + "%^RESET%^");
            ++i;
         }
      }
      if (!i && no_excellent) {
         write("Everything is in excellent condition.\n");
      }
   }

   return 1;
}                               /* cmd() */

mixed *query_patterns()
{
   return ({ "<indirect:object'item(s)'> sorting {up|down}",
             (: cmd($1, $4[1] == "up" ? 1 : -1, 0) :),
             "<indirect:object'item(s)'>",
             (: cmd($1, 0, 0) :),
             "<indirect:object'item(s)'> damaged",
             (: cmd($1, 0, 1) :),
             "<indirect:object'item(s)'> damaged sorting {up|down}",
             (: cmd($1, $4[1] == "up" ? 1 : -1, 1) :), });
}                               /* query_patterns() */


int compare(float first, float second)
{
   if (first > second)
      return 1;
   if (first < second)
      return -1;
   return 0;
}                               /* compare() */
