/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: random_name.c,v 1.2 2000/10/20 18:45:32 pinkfish Exp $
 * $Log: random_name.c,v $
 * Revision 1.2  2000/10/20 18:45:32  pinkfish
 * Add in a method to generate names from test languages.
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
#include <random_names.h>
inherit "/cmds/base";

string lang_list;

void create() {
  lang_list = implode(RANDOM_NAME_GENERATOR->query_languages() +
                      RANDOM_NAME_GENERATOR->query_test_languages(), "|");
} /* create() */

mixed cmd(string pattern, string my_lang) {
  string *langs;
  int i;

  if (pattern == "") {
    write("Generating one word from each language.  To generate more "
          "words in a particular language, specify the language on the "
          "command line.\n");
    langs = (string *)RANDOM_NAME_GENERATOR->query_languages() +
            RANDOM_NAME_GENERATOR->query_test_languages();
    for (i=0;i<sizeof(langs);i++) {
      write(capitalize(langs[i])+": "+
            (string)RANDOM_NAME_GENERATOR->random_name(langs[i])+"\n");
    }
    return 1;
  }
  write("Generateing 10 names in "+my_lang+"\n\n");
  for (i=0;i<10;i++) {
    write( (string)RANDOM_NAME_GENERATOR->random_name(my_lang)+"\n");
  }
  return 1;
} /* cmd() */

mixed *query_patterns() {
  return ({ "", (: cmd($5, "") :),
            "{"+lang_list+"}", (: cmd($5, $4[0]) :) });
} /* query_patterns() */
