/*   -*- LPC -*-   */
/*
 * $Locker:  $
 * $Id: random_names.c,v 1.5 2002/03/29 04:19:07 ceres Exp $
 * $Log: random_names.c,v $
 * Revision 1.5  2002/03/29 04:19:07  ceres
 * Modified to use test_valid() for name checking
 *
 * Revision 1.4  2002/03/29 04:15:03  pinkfish
 *  Forcibly unlocked by ceres
 *
 * Revision 1.3  2000/10/20 18:44:55  pinkfish
 * Fix up an error in the loading.
 *
 * Revision 1.2  2000/10/20 18:41:53  pinkfish
 * Add in a method to have test languages/rules in the system.
 *
 * Revision 1.1   1998/01/06 05:03:33   ceres
 * Initial revision
 * 
*/
/**
 * Generates random names for whatever reason.
 * <p>
 * Werd - a perl script that generates random words according to some
 *            simple rules (inspired by phrase structure trees in Syntax).
 * <p>
 * Based on a program by Mark Rosenfelder (markrose@spss.com)
 *            called psrGrammar
 * <p>
 * Rewritten in perl by Chris Pound (pound@rice.edu)
 * <p>
 * Rewritten in lpc by Pinkfish@Discworld MUD.
 *            10th of January 1996.
 */
#include <random_names.h>
#include <player_handler.h>

private mapping rules;
private string* _test_rules;

void create() {
   seteuid("Room");
   rules = ([ ]);
   _test_rules = ({ });
   unguarded((: restore_object, RANDOM_NAME_SAVE_FILE :));
   if (!_test_rules) {
      _test_rules = ({ });
   }
} /* create() */

private void save_me() {
   unguarded((: save_object, RANDOM_NAME_SAVE_FILE :));
} /* save_me() */

/**
 * Read in a random name grammer from the data dir.   Any line with
 * # at the start is considered a comment, and anything after
 * a # on a line is ignored.   Every line in the file will have
 * 3 fields seperated by a :.   The first field is the name
 * the second is a one letter expansion string, the third is a
 * a space separeted list of things to expand it to.
 * <p>
 * The word starts off as a "W", this is looked up in the expansion
 * first string list.   If it is found then that is expanded to 
 * a random selection of the space seperated data elements.   This
 * process is repeated until there are no bits in the word that can
 * be expanded.
 * @param f the name of the file to read
 */
void open_file(string f) {
   string str, *bits, *stuff, lang;
   int i;

   bits = explode(f, ".");
   if (sizeof(bits) > 1) {
      lang = implode(bits[0..sizeof(bits)-2], ".");
   } else {
      lang = f;
   }
   lang = replace(lang, "_", " ");

   str = unguarded((: read_file, RANDOM_NAME_DATA_DIR+f :));
   bits = explode(str, "\n");
   rules[lang] = ([ ]);
   for (i=0;i<sizeof(bits);i++) {
      sscanf(bits[i], "%s#%*s", bits[i]);
      if (strlen(bits[i])) {
         stuff = explode("G"+bits[i], ":");
         if (sizeof(stuff) >= 3) {
            rules[lang][stuff[1]] = stuff[2];
         } else {
            write("Incorrect file format at line "+i+", less than 3 "
                  ": seporated bits.\n");
         }
      }
   }
   save_me();
} /* open_file() */

/**
 * Generate a random name in the given language.
 * @param lang the language to generate teh name int
 * @return the random name
 */
string random_name(string lang) {
   string word = "W", *bing;
   int some_caps = 1, i;

   do {
      some_caps = 0;
      for (i = 0; i < strlen(word); i++)
         if (word[i] >= 'A' && word[i] <= 'Z') {
            bing = explode(rules[lang][word[i..i]], " ") - ({ "" });
            word = word[0..i-1] + bing[random(sizeof(bing))] + word[i+1..];
            some_caps = 1;
         }
   } while (some_caps);
   return word;
} /* random_name() */

/**
 * The list of all the current languages known.
 * @return an array of the language names.
 */
string *query_languages() { return keys(rules) - _test_rules; }

/**
 * This is the list of test rules in the handler.
 * @return the list of test rules
 */
string *query_test_languages() {
   return _test_rules;
} /* query_test_languages() */

/**
 * This method adds in a language as a test language.
 * @param lang the test language to add
 */
void add_test_language(string lang) {
   _test_rules += ({ lang });
   save_me();
} /* add_test_language() */

/**
 * This method removes a language as a test language.
 * @param lang the test language to remove
 */
void remove_test_language(string lang) {
   _test_rules -= ({ lang });
   save_me();
} /* remove_test_language() */

/**
 * Return a name that is not banished or a player name already.
 * It is > 2 and < 11 chars, it is not banished, a player or
 * in the game.
 * @param lang the language to generat ethe name in
 * @return the generated name
 */
string unique_name(string lang) {
   string str;
   int ok;

   do {
      ok = 1;
      str = random_name(lang);
      if (strlen(str) < 2 || strlen(str) > 11)
         ok = 0;
      if (ok && PLAYER_HANDLER->test_user(str))
         ok = 0;
      if (ok && !PLAYER_HANDLER->test_valid(str))
         ok = 0;
      if (ok && find_living(str))
         ok = 0;
   } while (!ok);
   return str;
} /* unique_name() */

/** @ignore yes */
void dest_me() {
   destruct(this_object());
} /* dest_me() */
