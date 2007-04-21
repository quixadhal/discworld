/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: speech.c,v 1.36 2003/04/15 17:46:43 ceres Exp $
*/
/* command, trial out by Turrican for a commands daemon. */
#include <language.h>
#include <player.h>
#include <drinks.h>
#include <cmds/options.h>

#define TP this_player()
#define BEEP sprintf("%c",7)
#define CTRL sprintf("%c", 13)

#define REPLACEMENTS (["ne1":"anyone",\
                       "u":"you",\
                       "r":"are", \
                       "NE1":"anyone",\
                       "U":"you",\
                       "R":"are", \
                       "ur":"you are",\
                       "teh":"the", \
                       "some1":"someone"])

class message {
   string text;
   string emote;
   string language;
   string type;
   string notify_mess;
   int status;
}

varargs string query_word_type(string str, string def) {
  int i;
  
  if (!str || strlen(str) < 1)
    return "";
  
  for (i = strlen(str) - 1; i >= 0 && str[i] == ' '; i--);

  // Make sure the string is not just spaces
  if (i < 0) {
    return "discombobulate";
  }

  switch (str[i]) {
  case '!':
    return "exclaim";
  case '?':
    return "ask";
  default:
    if (def)
      return def;
    else
      return "say";
  }
}

string mangle_tell(string mess, object dest) {
  int i;
  string ret;
  
  if (!TP || !environment(TP))
    return mess;
  
  if (environment(TP)->query_property("reverse_tell") &&
      !dest->query_creator() && !TP->query_creator()) {
    for (i = 0; i < strlen(mess); i++)
      ret = mess[i..i] + ret;
    return ret;
  }
  return mess;
}                               /* mangle_tell() */

string drunk_speech(string str) {
  /* I think I like replace already */
  return replace(str, ({ "S", "sh", "r", "rr", "ing", "in'", "x", "xsh",
                           "S", "SH", "R", "RR" }));
}

string de_eight(string arg) {
  object g;
  string replace_num;
  
  g = (object) TP->query_guild_ob();
  
  if (g && ((string) g->query_name() == "wizards") && !TP->query_creator()) {
    arg = "@ " + arg + "@";
    replace_num = ({ " seven plus one", " nine minus one",
                       " two to the power of three",
                       " four times two" })[random(4)];
    arg = implode(explode(arg, " eight"), replace_num);
    replace_num = ({ "(7+1)", "(9-1)", "(2^3)", "(4*2)" })[random(4)];
    arg = implode(explode(arg, "8"), replace_num);
    arg = arg[2.. < 2];
  }
  return arg;
}

/**
 * Replace "ne1" with "anyone" and suchlike.
 */
string fix_shorthand(string mess) {
  string *bits, name, value;
  mixed *tmp;
  int index, i;
  
  bits = explode(mess, " ");
  foreach(name, value in REPLACEMENTS) {
    tmp = regexp(bits, "(^|[^A-Za-z]+)"+name+"($|[^A-Za-z]+)", 1);
    if(sizeof(tmp)) {
      for(i=0; i<sizeof(tmp); i+=2) {
        index = tmp[i+1]-1;
        bits[index] = replace(bits[index], name, value);
      }
    }
  }
  return implode(bits, " ");
}

/* to properly columnate word_typed things */
void my_mess(string fish, string erk) {
  int bing;
  
  if (!interactive(TP)) {
    return;
  }
  bing = strlen(fish);
  if (bing > 15) {
    bing = 15;
  }
  efun::tell_object(TP, TP->fix_string(sprintf("%s%s\n", fish, erk),
                                       (int) TP->query_cols(), bing));
}

class message build_message(string arg, mixed target, string word_type) {
  string lang;
  string s1;
  string emotion;
  string word;
  string *langs;
  int i;
  class message mess;
  int done;
  
  mess = new (class message);
  
  if (!arg || arg == "" || arg == " ") {
    mess->status = NO_MESSAGE;
    return mess;
  }
  // Look for words starting #, @ or :. Take up to two in either order.
  while (!done && (!lang || !emotion)) {
    switch (arg[0]) {
    case '#':
      if(word_type == "tell") {
        done = 1;
        continue;
      }
      
      if (arg[1] == '\'' && sscanf(arg, "#'%s' %s", lang, arg) == 2)
        continue;

      if (sscanf(arg, "%s %s", lang, arg) == 2) {
        lang = lang[1..];
        // Try and do partial matching.
        langs = LANGUAGE_HAND->query_languages();
        if (member_array(lang, langs) == -1) {
          for (i = 0; i < sizeof(langs); i++) {
            if (TP->query_skill(LANGUAGE_HAND->
                                query_language_spoken_skill(langs[i])) > 1 ||
                TP->query_skill(LANGUAGE_HAND->
                                query_language_written_skill(langs[i])) > 1) {
              if (langs[i][0..strlen(lang)-1] == lang) {
                lang = langs[i];
              }
            }
          }
        }
      } else {
        done = 1;
      }
      break;
    case '@':
      //case ':':
      if (sscanf(arg, "%s %s", emotion, arg) == 2) {
        emotion = emotion[1..] + " ";
      } else {
        done = 1;
      }
      break;
    default:
      done = 1;
    }
  }
  
  if (!emotion)
    emotion = "";

  if(lang == "general" && interactive(TP) && !TP->query_creator()) {
    mess->status = NOT_KNOWN;
    return mess;
  }

  if(!lang)
    lang = (string) TP->query_current_language();
  
  mess->language = lang;
  if(!LANGUAGE_HAND->query_language_spoken(lang)) {
    mess->status = NOT_SPOKEN;
    return mess;
  }
  if(word_type == "shout" && !LANGUAGE_HAND->query_language_distance(lang)) {
    mess->status = NOT_DISTANCE;
    return mess;
  }
  if(word_type != "tell" &&
     !TP->query_skill(LANGUAGE_HAND->query_language_spoken_skill(lang)) &&
     !LANGUAGE_HAND->query_language_always_spoken(lang)) {
    mess->status = NOT_KNOWN;
    return mess;
  }

  
  if (!interactive(TP)) {
    arg = TP->convert_message(arg);
    arg = TP->fit_message(arg);
  }
  
  // remove any unwanted stuff from the message.
  arg = replace(arg, ({ BEEP, "", CTRL, "", "[A", "", "[B", "", "[C", "",
                            "[D", "", "%^", " " }));

  if(TP->query_property(SHORTHAND_PROP))
    arg = LANGUAGES->fix_shorthand(arg);

  if(word_type != "tell") {
    if(TP->query_volume(D_ALCOHOL))
      arg = drunk_speech(arg);
    
    // General speech modifications (curses, etc)
    s1 = TP->mangle_speech(arg);
    if (stringp(s1))
      arg = s1;
  
    if(TP && environment(TP) &&
       function_exists("mangle_speech", environment(TP)))
      arg = (string) environment(TP)->mangle_speech(word_type, arg, target);
  
    arg = de_eight(arg);
  }
  
  word = query_word_type(arg, word_type);
  
  mess->text = arg;
  mess->type = word;
  mess->emote = emotion;
  mess->status = MESSAGE_OK;
  if (target)
    mess->notify_mess = "$one_short:" + file_name(TP) + "$ " + emotion +
      "$V$0=" + word + "s," + word + "$V$ $target$: ";
  else
    mess->notify_mess = "$one_short:" + file_name(TP) + "$ " + emotion +
      "$V$0=" + word + "s," + word + "$V$: ";
  
  return mess;
}


int say_it(class message mess) {
  string accent;

  if (!environment(TP))
    write("You are in limbo, noone can hear you.\n");
  
  switch(mess->status) {
  case NO_MESSAGE:
    return notify_fail("Syntax: " + query_verb() + " <something>\n");
  case NOT_SPOKEN:
    return notify_fail(capitalize(mess->language) +
                       " is not a spoken language.\n");
  case NOT_KNOWN:
    return notify_fail("You cannot speak " + capitalize(mess->language) + ".\n");
  }
  
  TP->remove_hide_invis("hiding");
  accent = TP->query_nationality_accent_ob();
  
  TP->comm_event(environment(TP), "person_say",
                 mess->notify_mess, mess->text, mess->language, accent);
  
  if (mess->language != TP->query_default_language() &&
      mess->language != "general") {
    my_mess("You " + mess->emote + mess->type + " in " +
            capitalize(mess->language) + ": ", mess->text);
  } else {
    my_mess("You " + mess->emote + mess->type + ": ", mess->text);
  }
  
  TP->adjust_time_left(-5);
  return 1;
}

int say_it_to(class message mess, mixed targets, int priv, string event) {
  string lstr;
  string extra;
  string accent;
  
  if (!environment(TP)) {
    write("You are in limbo, noone can hear you.\n");
  }

  switch(mess->status) {
  case NO_MESSAGE:
    return notify_fail("Syntax: " + query_verb() + " <something>\n");
  case NOT_SPOKEN:
    return notify_fail(capitalize(mess->language) +
                       " is not a spoken language.\n");
  case NOT_KNOWN:
    return notify_fail("You cannot speak " + capitalize(mess->language) + ".\n");
  }
  
  if (!sizeof(targets)) {
    return 0;
  }
  
  targets -= ({ this_player() });
  TP->remove_hide_invis("hiding");
  this_player()->adjust_time_left(-5);
  accent = TP->query_nationality_accent_ob();
  
  switch (mess->type) {
  case "exclaim":
  case "whisper":
  case "say":
    extra = "to ";
    break;
  default:
    extra = "";
  }
  
  switch (priv) {
  case 2:                     // Only recipients see anything.
    break;
  case 1:                     // Everyone sees it happen but not what's said
    TP->comm_event_to(environment(this_player()), event,
                      (string) this_player()->one_short(1) +
                      " " + mess->emote + mess->type + "s " +
                      extra, mess->text, targets, mess->language,
                      this_player(), accent);
    break;
    
  case 0:                     // Everyone hears it all.
    TP->comm_event(environment(TP), event,
                   "$one_short:" + file_name(TP) + "$ " + mess->emote +
                   "$V$0=" + mess->type + "s," + mess->type + "$V$ " +
                   extra + query_multiple_short(targets, "the") + ": ",
                   mess->text, mess->language, accent);
    break;
  }
  
  if (mess->language != TP->query_default_language() &&
      mess->language != "general") {
    lstr = " in " + cap_words(mess->language);
  } else {
    lstr = "";
  }
  
  write("You " + mess->emote + mess->type + " " + extra +
        query_multiple_short(targets, "the") + lstr + ": " +
        mess->text + "\n");
  return 1;
} /* say_it_to() */
