/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: birthdays.c,v 1.3 2003/03/29 03:12:01 presto Exp $
 *
 *
 */

inherit "/cmds/base";

int cmd()  {
   object *peeps;

   peeps = filter(users(), (: $1->query_visible(this_player())  &&
                              $1->query_is_birthday_today() :));
   if (sizeof(peeps) == 0)
      return notify_fail("No one has a birthday today.\n");
   if (sizeof(peeps) == 1)
      printf("%s has a birthday today.\n", peeps[0]->short());
   else
      write(query_multiple_short(peeps) + " have a birthday today.\n"); 
   return 1;
}


mixed *query_patterns()  {
   return ({ "", (: cmd() :) });
}
