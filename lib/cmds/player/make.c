/*
 * $Locker:  $
 * $Id: make.c,v 1.14 2003/03/01 01:35:41 pinkfish Exp $
 * $Log: make.c,v $
 * Revision 1.14  2003/03/01 01:35:41  pinkfish
 * Add in snow player killer.
 *
 * Revision 1.13  2002/08/29 18:45:54  taffyd
 * Fixed space error
 *
 * Revision 1.12  2001/11/08 08:41:07  pinkfish
 * Add in a few extra thingies.
 *
 * Revision 1.11  2001/11/08 08:04:31  pinkfish
 * Add in a snowcabbage.
 *
 * Revision 1.10  2001/07/16 23:03:22  ceres
 * Added snowangel
 *
 * Revision 1.8  2001/03/13 02:50:33  pinkfish
 * Fix a typo.
 *
 * Revision 1.7  2000/09/03 05:38:40  ceres
 * Added some more options.
 *
 * Revision 1.6  2000/06/16 02:02:24  pinkfish
 * Fix it up to work on the distribution lib.
 *
 * Revision 1.5  1999/05/08 01:15:04  pinkfish
 * Fixed some bugs.
 *
 * Revision 1.4  1999/01/13 01:35:26  pinkfish
 * Fix up the make stuff.
 * *8
 *
 * Revision 1.3  1999/01/11 06:28:37  pinkfish
 * Add in the cream pie stuff.
 *
 * Revision 1.2  1998/04/14 02:35:44  pinkfish
 * Changed to use add_command.
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
/* make command for making snowballs and snowmen. Moved from /global/pweath.c
 * by Ceres 24/3/96
 */

#include <config.h>
#include <weather.h>
#include <move_failures.h>
#ifndef __DISTRIBUTION_LIB__
#include <special_day.h>
#endif

inherit  "/cmds/base";

#define TP this_player()

private string *_snow_creatures = ({ 
    "snowman",
    "snowwoman", 
    "snowboy",
    "snowgirl",
    "snowbunny",
    "snowfrog",
    "snowball",
    "snowwomble",
    "snowangel",
    "snowcabbage",
    "snowwitch",
    "snowwizard",
    "snowwarrior",
    "snowthief",
    "snowpriest",
    "snowassassin",
    "snowfamily",
    "snowfox",
    "snowdwarf",
    "snowplayerkiller",
    "snowtroll"
}); 

private string _make_pattern_cache; 

void create() {
    ::create();

    _snow_creatures = sort_array( _snow_creatures, 1 );
} /* create() */ 

int is_snowing(object player, string str) {
  object env;
  int *co_ords;

  env = environment(player);
  if ((string)env->query_property("location") == "inside") {
    add_failed_mess("You must be outside to make a " + str + ".\n");
    return 0;
  }

  co_ords = (mixed *)env->query_co_ord();

  if (!WEATHER->query_snowing(env)) {
     add_failed_mess("You need snow to make a " + str + ".\n");
     return 0;
  }

  return 1;
} /* is_snowing() */

mixed cmd(string str) {
  object ob;
  object env;
 
  env = environment(TP);
  switch (str) {
  case "snowball" :
    if (is_snowing(TP, str)) {
      ob = clone_object("/std/environ/snowball");
      if (ob->move(TP) != MOVE_OK) {
        add_succeeded_mess(({ "$N try to make a snowball, but you cannot "
                              "pick it up so it disintergrates in "
                              "disinterest.\n",
                              "$N tries to make a snowball, but they cannot "
                                "pick it up.\n" }));
      } else {
        add_succeeded_mess(({
          "$N $V a lovely big snowball.  Have fun with it ;)\n",
            "$N $V a lovely big snowball.\n" }));
      }
      return 1;
    }
    break;
#ifndef __DISTRIBUTION_LIB__
  case "cream pie":
    if (SPECIAL_DAY_HANDLER->query_special_day() == CREAM_PIE_DAY) {
      if (TP->query_property("made cream pie")) {
        add_failed_mess("You made a cream pie less than 30 seconds ago.  "
                        "Don't be greedy now.\n");
      } else {
        ob = clone_object("/obj/misc/cream_pie");
        if (ob->move(TP) != MOVE_OK) {
          add_succeeded_mess(({ "You try to magicaly conjure a cream pie up "
                                  "out of nowhere, but you cannot pick it up.\n",
                                  "$N tries to conjure up a cream pie, but "
                                  "just looks silly.\n" }));
          ob->dest_me();
          return 1;
        } else {
          /* One pie every 30 seconds... */
          TP->add_property("made cream pie", 1, 30);
          add_succeeded_mess("$N conjure$s up $I.\n", ({ ob }));
          return 1;
        }
      }
    } else {
       add_failed_mess("You can only create cream pies on cream pie day!\n");
    }
    break;

  default:
    if ( member_array( str, _snow_creatures ) > -1 ) { 
        if (is_snowing(TP, str)) {
          ob = clone_object("/std/environ/snowman");
          ob->set_type(str);
          ob->move(env);
          add_succeeded_mess("$N $V a beautiful " + str + ".\n");
          return 1;
        }
    }
    break;

#endif
  }
  return 0;
} /* cmd() */

mixed *query_patterns() {
    if ( !_make_pattern_cache ) { 
        _make_pattern_cache = "{" + implode( _snow_creatures + 
            ({ "cream pie" }), "|" ) + "}";
    }

   return ({ _make_pattern_cache, (: cmd($4[0]) :) });
} /* query_patterns() */
