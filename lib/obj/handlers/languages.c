/*  -*- LPC -*-  */
/*
 * $Locker: ceres $
 * $Id: languages.c,v 1.32 2003/02/19 08:35:25 ceres Exp ceres $
*/
/**
 * The language handler.  This will keep track of the info on languages
 * Each language can be a written language a spoken language and have
 * a garble object.
 *<p>
 * The function garble_say, garble_tell, garble_shout and
 * garble_written should be defined on the garbling object.
 * They should return the entire string to print.  So if the
 * routine returns 0 nothing is printed.
 *
 * @author Pinkfish
 */
 
#include <language.h>

#define L_NEW 512

#define REPLACEMENTS (["no1":"noone",\
                       "ne1":"anyone",\
                       "any1":"anyone",\
                       "u":"you",\
                       "r":"are", \
                       "NE1":"anyone",\
                       "U":"you",\
                       "R":"are", \
                       "ur":"you are",\
                       "teh":"the", \
                       "some1":"someone", \
                       "c":"see", \
                       "cing":"seeing",\
                       "sum1":"someone",\
                       "urs":"yours",\
                       "thier":"their", \
                       "pls":"please",\
                       "PLZ":"please",\
                       "plz":"please",\
                       "b4":"before", \
                       "tnx":"thanks"])

mapping languages;
string search;

void create() {
  languages = ([
  "rabbit" : ({ L_SPOKEN|L_WRITTEN|L_DISTANCE|L_ALWAYS_SPEAK|L_NEW,
                "/std/languages/rabbit", 10 }),
  "general" : ({ L_SPOKEN|L_WRITTEN|L_DISTANCE|L_ALWAYS_SPEAK/*|L_NEW */,
                "/std/languages/general", 10 }),
  //"common" : ({ L_SPOKEN|L_WRITTEN|L_DISTANCE/*|L_NEW */,
  //              "/std/languages/common", 10 }),
  "grunt" : ({ L_SPOKEN | L_ALWAYS_SPEAK, "/std/languages/grunt", 100 }),
  "wommon" : ({ L_WRITTEN|L_SPOKEN|L_DISTANCE|L_MAGIC,
                "/std/languages/wommon", 10 }),
              ]);

  search = "(" + implode(keys(REPLACEMENTS), "|") + ")";
} /* create() */

/**
 * Determine if the language is spoken. 
 *
 * @param str the language name to test
 * @return 1 if the language is spoken, 0 if it is not
 */
int query_language_spoken(string str) {
  if (!languages[str])
    return 0;
  return languages[str][0]&L_SPOKEN;
} /* query_language_spoken() */

/**
 * Determine if the language is written.
 *
 * @param str the language name to test
 * @return 1 if the language is written, 0 if it is not
 */ 
int query_language_written(string str) {
  if (!languages[str])
    return 0;
  return languages[str][0]&L_WRITTEN;
} /* query_language_written() */
 
/**
 * Determine if the language is spoken.
 * Distance languages can be used with tell and shout.
 *
 * @param str the language name to test
 * @return 1 if it is distance language, 0 if it is not
 */      
int query_language_distance(string str) {
  if (!languages[str])
    return 0;
  return languages[str][0]&L_DISTANCE;
} /* query_language_distance() */


/**
 * @ignore yes
 */
int query_language_new(string str) {
  if (!languages[str])
     return 0;
  return languages[str][0]&L_NEW;
} /* query_language_new() */
 
/**
 * Determine if it is a magical language.
 * A magical langage gets a function called on it's object whenever it
 * is read even if the person knows the language.  It is also the language
 * spells are written in.
 *
 * @param str the language to test
 * @return 1 if it is magical, 0 if it is not
 */
int query_language_magic(string str) {
  if (!languages[str])
    return 0;
  return languages[str][0]&L_MAGIC;
} /* query_language_magic() */

/**
 * The size of the language.  Some languages have bigger letters
 * and stuff than others.
 * 
 * @param str the language to get the size of
 * @return the size of each letter 
 */
int query_language_size(string str) {
  if (!languages[str])
    return 0;
  return languages[str][2];
} /* query_language_size() */
 
/**
 * Determine if the language is always able to be spoken. 
 *
 * @param str the language name to test
 * @return 1 if the language is spoken, 0 if it is not
 */
int query_language_always_spoken(string str) {
  if (!languages[str])
    return 0;
  return languages[str][0]&L_ALWAYS_SPEAK;
} /* query_language_spoken() */

/* This is called to resize the text if it doesnt fit... */
/**
 * @ignore yes
 */
mixed squidge_text(string lang, mixed str, int size) {
  if (!languages[lang])
    return 0; /* Don't add it... */
  if (!(languages[lang][0]&L_MAGIC))
    return str[0..(size/languages[lang][2])];
  return (mixed)languages[lang][1]->squidge_text(str, size);
} /* squish_text() */

/* Should not really allow this... 
int add_language(string name, int flags, mixed ob, int size) {
  languages[name] = ({ flags, ob, size });
} * add_language() */

/**
 * Return all the flags for the language.
 *
 * @param name the language to get the flags of
 * @return all the flags
 * @see /include/language.h
 */
int query_flags(string name) {
  if (!languages[name])
    return 0;
  return languages[name][0];
} /* query_flags() */

/**
 * Figure out the garble object.  This returns the garble object for the
 * language.  The garble object is used to modify the text so that
 * player does not see plain text if they do not know the language.
 *
 * @param name the language to get the garble object for
 * @return the garble object
 */ 
string query_garble_object(string name) {
  if (!languages[name])
    return 0;
  return languages[name][1];
} /* query_garble_object() */

/**
 * Names of all the languages.
 * @return the names of all the languages
 */ 
string *query_languages() {
  return m_indices(languages);
} /* query_languages() */

/**
 * Determine if this is a language.
 * @return 1 if it is a language, 0 if it s not
 * @param str the language to check
 */ 
int test_language(string str) {
  return pointerp(languages[str]);
} /* test_language() */

/**
 * The skill for the spoken language.  This gets the skill name in the skill
 * tree for the spoken part of the language.
 * @param lang the language to get the skill of
 * @return the skill name
 */
string query_language_spoken_skill(string lang) {
  return LANGUAGE_SKILL_START + replace(lang, " ", "_") + "." + SPOKEN_SKILL;
} /* query_language_spoken_skill() */

/**
 * The skill for the written language.  This gets the skill name in the skill
 * tree for the written part of the language.
 * @param lang the language to get the skill of
 * @return the skill name
 */
string query_language_written_skill(string lang) {
  return LANGUAGE_SKILL_START + replace(lang, " ", "_") + "." + WRITTEN_SKILL;
} /* query_language_written_skill() */

/**
 * This garbles a say.  Called from the events class to garble the text
 * that players see.  This looks up the garble object and calls the
 * correct function on it.  The returned array contains the
 * garbled start and message.
 *
 * @param lang the language spoken in
 * @param start the start bit
 * @param mess the message itself
 * @param player the player hearing
 * @param from the player speaking
 * @param type 'say', 'tell', 'shout' etc
 * @return an array <code>({ start, mess })</code>
 */
mixed garble_say(string lang, string start, string mess,
                 object player, object from, string type) {
   string garble_ob;

   if (query_language_new(lang)) {
      garble_ob = query_garble_object(lang);
      if (living (from) && garble_ob) {
         return garble_ob->garble_say(start, mess, player, from, type,
                                      query_language_spoken_skill(lang),
                                      type == SHOUT_TYPE);
      }
   } else {
      if (player->query_skill(query_language_spoken_skill(lang)) < 99) {
         garble_ob = query_garble_object(lang);
         if(garble_ob)
           return garble_ob->garble_say(start, mess, player, type);
         else
           debug_printf("Error, %s has no garble object.", lang);
      }
   }
   
   return ({ start, mess });
} /* garble_say() */

/**
 * Garble written text.  This is the same as garble_say, but it works on
 * written text.
 *
 * @param lang the language written in
 * @param text the text which has been written
 * @param thing the thing which is written on
 * @param player the player doing the reading
 * @return the garbled text as a string
 */
string garble_text(string lang, mixed text, object thing, object player ) {
   string garble_ob;

   if (functionp(text)) {
      text = evaluate(text);
   }

   if (query_language_new(lang)) {
      garble_ob = query_garble_object(lang);
      if (garble_ob) {
         return garble_ob->garble_text(text, thing, player,
                                       query_language_written_skill(lang));
      }
   } else {
      if (player->query_skill(query_language_written_skill(lang)) < 99) {
         garble_ob = query_garble_object(lang);
         if( !garble_ob )
            return "You do not recognise the language.\n";
         else
            return garble_ob->garble_text(text, thing, player);
      }
      if (query_language_magic(lang)) {
         garble_ob = query_garble_object(lang);
         return garble_ob->magical_text(text, thing, player);
      }
      return text;
   }

   if (query_language_magic(lang))
      return 0;

   return "Text written in " + lang + " could not have been written.  "
          "There are "
          "broken bits of letters scattered over the page, looks like "
          "someone had a bad game of scrabble.\n";
} /* garble_text() */

/**
 * Remove "ne1" and suchlike
 */

string do_replacements(string mess) {
  string *bits, name, value;
  mixed *tmp;
  int index, i, add_space;

  // This one's a little different so it's done on its own.
  mess = replace(mess, ({"ne1 no ", "anyone know "}));
  
  // First check to see if the sentance contains a possible replacement.
  if(!regexp(mess, search))
    return mess;

  /*
   * We have a possible so explode the line into words and check each
   * replacement against each word. If there's a match then replace.
   */
  if(mess[<1] == ' ')
    add_space = 1;
  bits = explode(mess, " ");
  foreach(name, value in REPLACEMENTS) {
    tmp = regexp(bits, "(^|[^A-Za-z0-9\\\\./_&-]+)"+name+"($|[^A-Za-z0-9\\./&_-]+)", 1);
    if(sizeof(tmp)) {
      for(i=0; i<sizeof(tmp); i+=2) {
        index = tmp[i+1]-1;
        bits[index] = replace(bits[index], name, value);
      }
    }
  }
  return implode(bits, " ") + (add_space ? " " : "");
}

string fix_shorthand(string mess) {
  string pre, mid, post, res;

  if (!stringp(mess)) {
     return "";
  }
  res = "";
  while(sscanf(mess, "%s'%s'%s", pre, mid, post) == 3) {
    res += do_replacements(pre) + "'" + mid;
    mess = "'" + post;
  }
    
  res += do_replacements(mess);
  
  return res;
}

