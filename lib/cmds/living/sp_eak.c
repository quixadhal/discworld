/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: sp_eak.c,v 1.10 2002/11/12 23:35:45 ceres Exp $
 * $Log: sp_eak.c,v $
 * Revision 1.10  2002/11/12 23:35:45  ceres
 * Can't remember
 *
 * Revision 1.8  2002/07/16 20:11:03  pinkfish
 * Add in the default languiage information.
 *
 * Revision 1.7  2001/03/10 20:28:46  ceres
 * Fixed problems from womens day
 *
 * Revision 1.6  2001/03/10 20:27:50  taffyd
 *  Forcibly unlocked by ceres
 *
 * Revision 1.5  2001/03/07 21:52:22  pinkfish
 * Fix up a fail message in speak.
 *
 * Revision 1.4  2000/10/15 01:40:43  presto
 * Removed cap_words function.... now a simul_efun
 *
 * Revision 1.3  1999/05/19 04:31:06  pinkfish
 * Clear something up.
 *
 * Revision 1.2  1999/05/19 04:23:23  pinkfish
 * Capitalise the names.
 *
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";
#include <language.h>

#define TP this_player()

int cmd(string which)
{
   string *langs;
   string skill;
   int i;
   mapping skills;

   skills = TP->query_skills();
   if (!which) {
      langs = LANGUAGE_HAND->query_languages();
      for (i = 0; i < sizeof(langs); i++) {
         if (TP->
             query_skill(LANGUAGE_HAND->
                         query_language_spoken_skill(langs[i])) > 1
             || TP->query_skill(LANGUAGE_HAND->
                                query_language_written_skill(langs[i])) > 1) {
         } else {
            langs = langs[0..i - 1] + langs[i + 1..];
            i--;
         }
      }
      write("You are currently speaking " +
            cap_words(TP->query_current_language()) + " and your default "
            "language is " + cap_words(TP->query_default_language()) + ".\n");
      write("You can speak " +
            query_multiple_short(map(langs, (: cap_words :)) +
                                 ({ "Grunt" })) + ".\n");
      write("Please note that the language you are speaking is used for "
            "writing as well, so some of the languages may be just written.\n");
      return 1;
   }

   which = lower_case(which);
   skill = LANGUAGE_HAND->query_language_spoken_skill(which);

   if (!skill) {
      return notify_fail("You cannot speak that language.\n");
   }

   if((!TP->query_skill(skill) && which != "grunt" && which != "general") ||
      (which == "general" && !TP->query_creator() && interactive(TP)))
      return notify_fail("You don't know that language.\n");

   if (which == this_player()->query_current_language()) {
      add_failed_mess("You are already speaking " + cap_words(which) + ".\n");
      return 0;
   }

   if (!TP->set_language(which)) {
      return notify_fail("Argh... something weird happened.\n");
   }

   write("Now using " + cap_words(which) + " for speaking and writing.\n");
   return 1;
}                               /* cmd() */

mixed *query_patterns()
{
   string *languages;
   string tmp;

   languages = LANGUAGE_HAND->query_languages();

   if ( TP->query_womens_day() ) {
       languages += ({ "wommon" });
   }

   tmp = implode(languages + map(languages, (: cap_words :)), "|");

   return ({ "{" + tmp + "}", (: cmd(implode($4, " ")) :), 
             "", (: cmd(0) :) });
/*    return ({ (TP->query_womens_day()? "wommon" : "{" + tmp + "}"),
             (: cmd(implode($4, " ")) :), "", (: cmd(0) :) }); */ 
} /* query_patterns() */
