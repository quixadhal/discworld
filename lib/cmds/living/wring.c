inherit "/cmds/base";

int cmd(object *things)  {
   object *towels;
   int    *enums;
   int     wetness;
   int     success;
   string  str;

   towels = filter(things,
                   (: $1->id("towel")  ||  $1->query_property("dryer") :));
   things -= towels;

   str = "";

   if (sizeof(towels) == 1)  {
      enums = towels[0]->effects_matching("object.wet");
      if (sizeof(enums)) {
         wetness = towels[0]->arg_of(enums[0]);
      }
      if (wetness < 36 - this_player()->query_str())  {
         str = "You can't wring any more moisture out of " +
               towels[0]->one_short();
         success = 0;
      } else  {
         towels[0]->add_effect("/std/effects/object/wet",
            -wetness / ((36 - this_player()->query_str()) / 4));
         str = "You wring some moisture out of " +
               towels[0]->one_short();
         success = 1;
      }
   } else if (sizeof(towels))  {
      str = "You can only wring out one thing at a time";
      success = 0;
   }

   if (sizeof(things))  {
      if (success) {
         str += ", but you can't wring out " +
                query_multiple_short(things) + " at all.\n";
      } else {
         str += ", and you can't wring out " +
                query_multiple_short(things) + " at all.\n";
      }
   } else {
      str += ".\n";
   }

   write(str);

   return 1;
}

mixed *query_patterns()  {
   return ({ "[out] <indirect:object:me'towel'>", (: cmd($1) :) });
}
