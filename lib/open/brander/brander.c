/* brander.c: written by Quantum for everyplayer 05/07/91 */

/* you can sometimes find Quantum on the Babylon mud */

/*
   This monster brander lets a player brand a monster that he or she
   is fighting.  This marker will serve as a notice to other players
   that the monster is taken.  Hopefully this will eliminate many of
   those annoying shouts of "SuchAndSuch monster is MINE!".  This
   marker has one special feature:  If the carrying monster dies,
   then the branding player is informed of the identity of the killing
   player (assuming the branding player != killing player).
   The brand self destructs after a period of time.  A player may
   have only one monster branded at a time.  Branding a second monster
   will cause the first brand to be dissolved.
*/

/* added code to create the roombrand */
/* fixed brander so that didn't prevent branding monsters having same
   name as a player */

#define THISFILE "players/truilkan/obj/brander"
#define BRANDFILE "players/truilkan/obj/brand"
#define ROOMBRANDFILE "players/truilkan/obj/roombrand"

#define BRAND "monster_brand"

#define VERSION "2.01"
#define VERSION_DATE "06/04/91"

#include "contains.h"

int active;
object gbrand;

query_value()
{
   return 50;
}

static name(ob)
{
   if (!ob)
      ob = this_player();
   return capitalize(call_other(ob,"query_name"));
}

static real_name(ob)
{
   if (!ob)
      ob = this_player();
   return capitalize(call_other(ob,"query_real_name"));
}

get()
{
   return 1;
}

short()
{
   return "A monster brander";
}

long(arg)
{
   write("A curious looking device that looks like it might be useful\n");
   write("for staking a claim on a monster.\n");
}

init()
{
   if (environment(this_object()) == this_player()) {
      add_action("help"); add_verb("help");
      add_action("brand"); add_verb("brand");
      add_action("unbrand"); add_verb("unbrand");
   }
   return 1;
}

unbrand()
{
   if (!active) {
      write("You have no active brand.\n");
      return 1;
   }
   if (gbrand)
       gbrand->do_destruct(gbrand);
   active = 0;
   return 1;
}

find_monster(theName)
{
    string it;
    object ob;

    if (!theName)
        return 0;
    it = lower_case(theName);
    ob = present(theName, environment(this_player()));
    if (!ob || !living(ob))
        ob = find_living(it);
    if (ob == find_player(theName)) {
        return 0;
    }
    return ob;
}

brand(str)
{
   object monster;
   object brand;
   object roombrand;
   object monsterEnv;

   if (!str) {
      write("You look around for a likely monster to brand.\n");
      return 1;
   }
   monster = find_monster(lower_case(str));
   if (!monster || (environment(monster) != environment(this_player()))) {
       write("That monster isn't in your immediate environment.\n");
       return 1;
   }
   if (contains(BRAND,monster)) {
       write("That monster has already been claimed.\n");
       return 1;
   }
   if (active && gbrand)
      destruct(gbrand);
   gbrand = brand = clone_object(BRANDFILE);
   call_other(brand,"set_brander",this_object());
   move_object(brand,monster);
   call_other(brand,"set_owner",real_name()); /* call after object moved */
   write(name(monster) + " winces as you apply the brand.\n");
   say(capitalize(str) + " winces as " + name() + " applies a brand.\n",
       this_player());
   roombrand = clone_object(ROOMBRANDFILE);
   call_other(brand,"set_roombrand",roombrand);
   call_other(roombrand,"set_owner",real_name());
   call_other(roombrand,"set_monsterName",name(monster));
   call_other(roombrand,"set_monsterObj",monster);
   monsterEnv = environment(monster);
   call_other(roombrand,"set_myEnv",monsterEnv);
   move_object(roombrand,monsterEnv);
   call_other(roombrand,"set_messages",1);
   active = 1;
   return 1;
}

brand_is_dead()
{
   active = 0;
}

help(str)
{
  if (!id(str))
     return 0;
 write("This monster brander may be used to claim dibs on a given\n");
 write("monster.  In addition, this brander has a special property that, in\n");
 write("the event that another player kills the monster having your brand,\n");
 write("compels the spirit of that monster to notify you of the identity\n");
 write("of the offending player.\n\n");
 write("commands: brand <monster name> - mark a monster as taken\n");
 write("          unbrand - dissolve currently active brand.\n\n");
 write("Note that only one brand may be active for a given player\n");
 write("at any one time.  Also, the brands are not permanent and will\n");
 write("dissolve after some period of time has passed.  Even so, if you\n");
 write("brand a monster and then later decide to give up on the monster,\n");
 write("you should use the 'unbrand' command so that other players won't\n");
 write("think you have a claim when you in fact do not.\n\n");
 write("This object coded by Quantum aka Truilkan:\n");
 write("  credit goes to Milamber for the idea.\n");
 write("  version " + VERSION + "\n");
 write("  created on 05/07/91\n");
 write("  last modified on " + VERSION_DATE + "\n");
 return 1;
}

reset(arg)
{
   if (arg)
      return;
   return 1;
}

id(str)
{
    return str == "brander" || str == "monster brander";
}
