#include <drinks.h>
#include <language.h>
#include <player.h>

#define TP this_player()

inherit "/cmds/base";
inherit "/cmds/speech";

#ifdef USE_SMILEYS
string *two_smileys = ({":)", ":(", ":P", ":p", ":b", ";)", 
  ";(", ";P", ";p", ";b", "=)", "=("});

string *three_smileys = ({":-)", ":-(", ":-P", ":-p", ":-b",
  ";-)", ";-(", ";-P", ";-p", ";-b", ":o)", ":o(", ":oP", 
  ":op", ":ob", ";o)", ";o(", ";oP", ";op", ";ob", "=-)",
  "=-(", "=o)", "=o("});
#endif

int cmd( string arg, mixed thing, int silent ) {
   string word, lang, words, emotion;
#ifdef USE_SMILEYS
   string smiley, smiley_words;
#endif
   string them_mess, temp;
   string me_mess;
   object *obs;
   object *net_dead;
   object *fail;
   mixed busy;

   emotion = "";
   if (pointerp(thing)) {
      words = arg;
   } else if ( !objectp( thing ) ) {
      if ( !arg || sscanf( arg, "%s %s", word, words ) != 2 )
         return 0;
      word = lower_case( word );
      word = (string)TP->expand_nickname( word );
      thing = find_player( word );
      if ( !thing ) {
         if ( sscanf( word, "%*s@%*s" ) == 2 ) {
            "/net/daemon/out_tell"->do_tell( word+" "+words );
            return 1;
         }
         add_failed_mess( capitalize( word ) +" is not logged in.\n" );
         return 0;
      }
   } else {
      words = arg;
   }

   if (!pointerp(thing)) {
      obs = ({ thing });
   } else {
      obs = thing;
   }

   obs -= ({ this_player() });
   if ( !sizeof(obs) ) {
      add_failed_mess( "Talking to yourself again.  I don't know.\n" );
      return 0;
   }

   if (sizeof(obs) > 20) {
      add_failed_mess("You can only tell up to 20 people a message.\n");
      return 0;
   }

   fail = this_player()->query_ignoring(obs);
   if ( sizeof( fail ) )  {
      write( "You are currently ignoring " +
             query_multiple_short( fail ) + ", so " +
             (sizeof(fail) > 1 ? " they" : fail[0]->query_pronoun()) +
             " couldn't reply anyway.\n" );
      obs -= fail;
      if ( !sizeof( obs ) )  {
         return 1;
      }
   }
   lang = (string)TP->query_current_language();
   if ( !LANGUAGE_HAND->query_language_spoken( lang ) ) {
      return notify_fail( capitalize( lang ) +
            " is not a spoken language.\n" );
   }
   if ( !LANGUAGE_HAND->query_language_distance( lang ) ) {
      return notify_fail( capitalize( lang ) +
            " is not able to be spoken at a distance.\n" );
   }
  // This is so small tells don't error.
#ifdef USE_SMILEYS
  if(sizeof(words) >= 3) {
    if(member_array(words[<3..], three_smileys) != -1) {
      smiley = words[<3..];
    } else if(member_array(words[<2..], two_smileys) != -1) {
      smiley = words[<2..];
    }
  } else if(member_array(words, three_smileys) != -1) {
    smiley = words;
  } else if(member_array(words, two_smileys) != -1) {
    smiley = words;
  }
#endif
  word = query_word_type(words, "");
  if(word != "") {
    word = " "+word+"ing";
  }
#ifdef USE_SMILEYS
  if(smiley) {
    // This is so we don't get errors with people telling each
    // other smileys on their own.
    if(sizeof(words) > 3) {
      smiley_words = words[0..<sizeof(smiley) + 1];
    } else {
      smiley_words = words;
    }
    word = query_word_type(smiley_words, "");
    if(word != "") {
      word = " "+word+"ing";
    }
  }
  switch(smiley) {
    case ":)" :
    case ":-)" :
    case ":o)" :
      emotion = " happily";
      break;
    case ":(" :
    case ":-(" :
    case ":o(" :
      emotion = " sadly";
      break;
    case "=)" :
    case "=-)" :
    case "=o)" :
      emotion = " innocently";
      break;
    case "=(" :
    case "=-(" :
    case "=o(" :
      emotion = " tearfully";
      break;
    case ";(" :
    case ";-(" :
    case ";o(" :
      emotion = " angrilly";
      break;
    case ";)" :
    case ";-)" :
    case ";o)" :
      emotion = " mischieviously";
      break;
    case ":P" :
    case ":-P" :
    case ":oP" :
    case ":p" :
    case ":-p" :
    case ":op" :
    case ":b" :
    case ":-b" :
    case ":ob" :
      emotion = " insanely";
      break;
    case "=P" :
    case "=-P" :
    case "=oP" :
    case "=p" :
    case "=-p" :
    case "=op" :
    case "=b" :
    case "=-b" :
    case "=ob" :
      emotion = " cheekily";
      break;
    case ";P" :
    case ";-P" :
    case ";oP" :
    case ";b" :
    case ";-b" :
    case ";ob" :
    case ";p" :
    case ";-p" :
    case ";op" :
      emotion = " evilly";
      break;
    default :
      emotion = "";
  }
#endif

   if ( word != " asking" ) {
      me_mess = "tell";
   } else {
      me_mess = "ask";
      word = "";
   }

   net_dead = ({ });

   foreach (thing in obs) {
      temp = emotion;
      if(thing->query_earmuffs("emoticon")) {
         emotion = "";
      }
      thing->event_person_tell( TP,
            capitalize( (string)TP->query_name() ) +
            " (creator) "+ me_mess + "s "+
            query_multiple_short(obs - ({ thing }) + ({ "you" })) + emotion +
            word + ": ", words, lang );
      if (!interactive(thing) && userp(thing)) {
         net_dead += ({ thing });
      }
   }
   if ( !silent ) {
      if ( lang != "common" ) {
         word += " in "+ lang;
      }
      if (sizeof(obs)) {
         them_mess =
               TP->convert_message(capitalize( query_multiple_short(obs)));
         emotion = temp;
         if(this_player()->query_earmuffs("emoticon")) {
           emotion = "";
         }
         TP->add_tell_history( "(creator) You " + me_mess + " "+ them_mess +
               word + emotion+ ": ", words );
      }
      if ( sizeof(net_dead)) {
          write("Warning: "+query_multiple_short(
                               map(net_dead, (: $1->query_cap_name() :)))+
                " "+ (sizeof(net_dead)>1?"are":"is")+" net dead.\n" );
      }
      emotion = temp;
      if(this_player()->query_earmuffs("emoticon")) {
        emotion = "";
      }
      busy = this_player()->query_busy();
      if (busy == 1) {
         write("Warning! You have your busy flag on.\n");
      } else if (pointerp(busy) && sizeof(obs - busy)) {
         write("Warning! You are currently set as busy with " + 
                  query_multiple_short(busy) + ".\n");
      }
      my_mess("(creator) You "+me_mess+" "+them_mess+
            emotion + word+": ", words);
   }
   TP->adjust_time_left( -5 );
   return 1;
} /* cmd() */

mixed *query_patterns() {
   return ({ "<indirect:player> <string'message'>", (: cmd($4[1], $1, 0) :),
             "<string>", (: cmd($4[0], 0, 0) :) });
} /* query_patterns() */


