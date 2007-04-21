/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: language_inherit.c,v 1.9 2003/06/04 23:30:00 pinkfish Exp $
 * 
*/
#include <fixed_random.h>
#include <tasks.h>
#include <living.h>

/*
 * The standard language unit.
 *
 * The level of a language ranges from 0-200.  At 0 everything is
 * garbled, at 200 nothing is.  In between the value is basicly
 * a percentage (with modifications due to length of word).
 */

private string _long;
private string _name;
private string _start_text_bit;

void setup();

void create() {
  setup();
} /* create() */

void set_long(string str) {
   _long = str;
} /* set_long() */

string query_long() {
   return _long;
} /* query_long() */

void set_name(string str) {
   _name = str;
} /* set_name() */

string query_name() {
   return _name;
} /* set_name() */

/*
 * This function should be overwritten higher up the chain.
 */
void setup() {
} /* setup() */

void set_start_text_bit(string str) {_start_text_bit = str; }

/*
 * Default garbler, over-ridden by functions  higher in the
 * inherit chain.
 */
string do_garble(string word, string type) {
  string ret;
  int i;

  ret = "";
  for (i=0;i<strlen(word);i++)
    if (word[i] != ' ')
      ret += "*";
    else
      ret += " ";
  return ret;
} /* do_garble() */

void check_level_increase(object player, int difficulty, int level,
                          string skill, string mess) {
   int chance;


   if(query_verb() == "shout" || player->query_property("dead") ||
      player->query_property(PASSED_OUT) ||
      (userp(player) && !interactive(player)))
     return;

   // If they arent speaking this language then don't use their gps and
   // don't give them a chance of an increase.
   if(player->query_current_language() !=
      lower_case(this_object()->query_name()))
     return;
   
   if (player->query_property("last message:" + skill) == mess ||
       player->query_property("dead")) {
      return ;
   }

   if(player->query_specific_gp("other") < difficulty) {
     return;
   } else {
     player->adjust_gp(-(difficulty*2));
   }
   
   /* 6 minute timeout...  Can get a thingy on it again after 6 minutes */
   player->add_property("last message:" + skill, mess, 360);

   if (difficulty <= level && level < 100) {
      /*
       * This is the base chance, hard at the edges and easier in the
       * middle. 
       */
        // chance = 525 + ((level - 50) * (level - 50)) / 8;
        // tweaked by ceres to give a slightly higher chance at low
        // skill levels.
        chance = 800 + ((level - 40) * (level - 40)) / 20;

      /* How to modify by difficulty? */
      chance -= level - difficulty;

      if (random(1000) > chance) {
          // Taffyd removed the third parameter so that the standard tm 
          // delay checks would still apply. 
        if(player->add_skill_level(skill, 1)) {
          /* Give them a level */
          //
          // Also commented this out, the message is done after the 
          // tm_check_ok() is successfully executed.
          //
          //TASKER->award_made(player->query_name(), file_name(this_object()),
          //                   skill, level);
          tell_object(player, "%^YELLOW%^You feel like the " +
                      capitalize(_name) +
                      " language is less confusing.%^RESET%^\n");
        }
      }
   }
} /* check_level_increase() */

string garble_message(string mess, object player, object speaker,
                      string type, string skill, int no_increase) {
  int i, level, num;
  int max_difficulty, diff, difference;
  string *bits;
  int something_garbled;

  level = player->query_skill(skill);
  if (level > 100) {
    player->add_skill_level(skill, 100 - level, 1);
  }
  num = speaker->query_skill(skill);
  if (num > 100) {
    speaker->add_skill_level(skill, 100 - num, 1);
  }

  difference = speaker->query_skill(skill) - player->query_skill(skill);
  
  if (num < level)
    level = num;
  if (level >= 100) {
    return mess;
  }

  if (level == 0) {
    max_difficulty = 0;
  } else {
    max_difficulty = 1000;
  }

  /* Otherwise it is more complicated */
  /*
   * This will always generate the same garbled string from the
   * same input
   */
  /*
   * Don't remove the seed thingy.  I want the same random sequence each
   * time..
   */
  RANDOM_OBJ->set_seed(crc32(mess));
  bits = explode(mess, " ");
  for (i=0;i<sizeof(bits);i++) {
    if (strlen(mess)) {
      if (level > 0) {
         num = (int)RANDOM_OBJ->random(100);
         if (strlen(mess) < 8) {
           diff = (level*100) / (strlen(mess)*10);
           if (num >= (level*100 / strlen(mess)*10)) {
             if (max_difficulty > diff)
               max_difficulty = diff;
             if (interactive(player)) {
               bits[i] = do_garble(bits[i], type);
               something_garbled = 1;
             } else {
               /* Don't waste time garbling for npcs. */
               bits[i] = "";
             }
           }
         } else {
           diff = (level*100) / 90;
           if (num >= diff) {
             max_difficulty = level*100 / 100;
             if (interactive(player)) {
               bits[i] = do_garble(bits[i], type);
               something_garbled = 1;
             } else {
               /* Don't waste time garbling for npcs. */
               bits[i] = "";
             }
           }
         }
      } else {
         if (interactive(player)) {
           bits[i] = do_garble(bits[i], type);
           something_garbled = 1;
         } else {
           /* Don't waste time garbling for npcs. */
           bits[i] = "";
         }
      }
    }
  }

  /* Max_diffculty is actually a lower bound, not an upper bound... */

  if (something_garbled && (difference > -10) && !no_increase) {
    /* Only get a level increase if you did not understand something */
    check_level_increase(player, max_difficulty, level, skill, mess);
  }
  return implode(bits, " ");
} /* garble_message() */

mixed garble_say(string start, string mess, object player,
                 object from, string type, string skill, int no_increase) {
  return ({ start, garble_message(mess, player, from, "speech", skill,
                                  no_increase) });
} /* garble_say() */

string garble_text( mixed text, object thing, object player, string skill) {
  string bing;

  if (stringp(text)) {
    if (player->query_default_language() != query_name()) {
      return _start_text_bit +
        garble_message(text, player, player, "text", skill, 0);
    } else {
      return garble_message(text, player, player, "text", skill, 0);
    }
  }

  if ( pointerp( text ) ) {
    if ( ( bing = (string)text[ 0 ]->garble_text( text, thing, player ) ) ) {
      if (player->query_default_language() != query_name()) {
        return _start_text_bit + bing;
      } else {
        return bing;
      }
    }
  }

  return "You cannot read the spidery writing.\n";
} /* garble_text() */

void dest_me() {
  destruct(this_object());
} /* dest_me() */
