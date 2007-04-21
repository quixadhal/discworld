/*  -*- LPC -*-  */
/*
 * $Id: dry.c,v 1.5 2001/10/07 03:05:35 presto Exp $
 */
inherit "/cmds/base";

mixed cmd(mixed *things)
{
   int pwet;
   int twet;
   int max;
   int *enums;
   object thing;
   object with;

   thing = things[0][0];
   with = things[1][0];
   if (!with->id("towel") && !with->query_property("dryer")) {
      return notify_fail("You can't dry anyone with " +
                         (string) with->the_short() + ".\n");
   }
   if (environment(with) != this_player())  {
      return notify_fail(with->a_short() + " isn't going to do much good "
         "inside " + environment(with)->a_short() + ".\n");
   }
   if (thing->query_property("dead")) {
      return notify_fail((string) thing->the_short() + " is dead; " +
                         (string) thing->query_pronoun() +
                         " has more immediate problems than being wet.\n");
   }
   max = 200 * (int) with->query_weight();
   enums = (int *) with->effects_matching("object.wet");
   if (sizeof(enums)) {
      twet = (int) with->arg_of(enums[0]);
   }
   if (twet > max / 2) {
      return notify_fail((string) with->the_short() +
                         " is already dripping wet.  You can't dry anybody "
                         "with it.\n");
   }

   enums = (int *) thing->effects_matching("body.wetness");
   if (sizeof(enums)) {
      pwet = (int) thing->arg_of(enums[0]);
   }

   if (pwet < 10) {
      if (thing == this_player()) {
         return notify_fail("You don't need drying.\n");
      } else {
         return notify_fail((string) thing->the_short() +
                            " doesn't need drying.\n");
      }
   }
   if (pwet + twet > max) {
      pwet = max - twet;
   }
   pwet -= random(10);
   if (pwet > 0) {
      thing->add_effect("/std/effects/other/wetness", -pwet);
      with->add_effect("/std/effects/object/wet", pwet);
   }
   if (thing == this_player()) {
      write("You dry yourself with " + with->the_short() + ".\n");
      say((string) this_player()->one_short() + " dries " +
          (string) this_player()->query_objective() + "self with " +
          (string) with->poss_short() + ".\n");
   } else {
      write("You dry " + (string) thing->the_short() + " with " +
            (string) with->the_short() + ".\n");
      tell_object(thing, (string) this_player()->one_short() +
                  " dries you with " + (string) with->poss_short() + ".\n");
      say((string) this_player()->one_short() + " dries " +
          (string) thing->the_short() + " with " +
          (string) with->poss_short() + ".\n", thing);
   }
   return 1;
}                               /* cmd() */

mixed *query_patterns()
{
   return ({ "<indirect:living'person'> with <indirect:object:me>",
             (: cmd($1) :),
             "myself with <indirect:object:me>",
             (: cmd(({ ({ this_player() }), $1 })) :) });
}                               /* query_patterns() */
