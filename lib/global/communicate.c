#include "drinks.h"
#include "player.h"
int social_points, max_social_points;
static object converse_ob;
static string my_file_name;
string *languages,
       cur_lang;

string drunk_speech(string str);

void communicate_commands() {
  add_action("do_say","sa*y");
  add_action("converse", "converse");
  add_action("do_say","'*");
  add_action("do_loud_say", "lsay");
  add_action("do_loud_say","\"*");
  add_action("do_emote",":*");
  add_action("do_emote","em*ote");
  add_action("do_echo","ec*ho");
  add_action("do_emote_all", "emoteall");
  add_action("do_echo_to", "echoto");
  add_action("do_write", "frimble");
  add_action("do_tell","t*ell");
/* Next two line commented out by GH Tue Jan 19 20:52:34 GMT 1993 */
/* to prevent the code that is killing the driver from running... */
  add_action("do_shout", "sh*out");
  add_action("do_shout", "shout!"); 
  add_action("do_creator_tell", "cre");
  add_action("do_creator_tell", "cre!");
  add_action("do_whisper", "whi*sper");
  add_action("set_language", "sp*eak");
} /* communicate_commands() */

void set_max_social_points(int num) {
  max_social_points = num;
} /* set_max_social_points() */

int query_max_social_points() {
  return max_social_points;
} /* query_max_social_points() */

void set_social_points(int num) {
  social_points = num;
} /* set_social_points() */

int query_social_points() {
  return social_points;
} /* query_social_points() */

int adjust_social_points(int num) {
  int temp;

  temp = social_points + num;
  if(temp >= 0) {
    social_points = temp;
    if(social_points > max_social_points)
      social_points = max_social_points;
    return 1;
  }
  return -1;
} /* adjust_social_points() */

int hard_adjust_social_points(int num) {
  social_points += num;
} /* hard_adjust_social_points() */

varargs string query_word_type(string str, string def) {
  int i;

  for (i=strlen(str)-1;str[i] == ' ';i--);
  switch (str[i]) {
    case '!' : return "exclaim";
    case '?' : return "ask";
    default:   if (def)
                 return def;
               else
                 return "say";
  }
} /* query_word_type() */

string query_shout_word_type(string str) {
  int i;

  for (i=strlen(str)-1;str[i] == ' ';i--);
  switch (str[i]) {
    case '!' : return "yell";
    case '?' : return " asking";
    default:   return "";
    }
} /* query_shout_word_type() */

string query_whisper_word_type(string str) {
  int i;

  for (i=strlen(str)-1;str[i] == ' ';i--);
  switch (str[i]) {
    case '!' : return "urgently ";
    case '?' : return "questioningly ";
    default:   return "";
  }
} /* query_whisper_word_type() */

string de_eight(string arg) {
  object g;

  g=(object)this_object()->query_guild_ob();
  if (g && (string)g->query_name()=="wizards") {
    arg = "@ " + arg + "@";
    arg=implode(explode(arg, " eight"), " seven plus one");
    arg=implode(explode(arg, "8"), "(7+1)");
    arg = arg[2..strlen(arg)-2];
  }
  return arg;
} /* de_eight() */

/* to properly columnate word_typed things */
void my_mess(string fish, string erk) {
  if(!interactive()) return;
  printf("%s%-=*s\n", fish, (int)this_player()->query_cols()-strlen(fish), 
          this_object()->fix_string(erk));
} /* my_mess() */

int do_write(string arg) {
  if (!arg || arg == "") {
    notify_fail("Syntax: "+query_verb()+" <string>\n");
    return 0;
  }
  write(arg+"\n");
  this_player()->adjust_time_left(-DEFAULT_TIME);
  return 1;
} /* do_write() */

int do_loud_say(string arg) {
  string word, s1, s2;
  object g;

  if (!arg) 
    arg = "";
  if (arg == "" || arg == " ") {
    notify_fail("Syntax: lsay <something>\n");
    return 0;
  }
  if (!LANGUAGE_HAND->query_language_spoken(cur_lang)) {
    notify_fail(capitalize(cur_lang)+" is not a spoken language.\n");
    return 0;
  }
  word = query_word_type(arg); 
  if (this_object()->query_volume(D_ALCOHOL))
    arg = drunk_speech(arg);
  arg = de_eight(arg);
  event(environment(), "person_say", this_object()->query_cap_name()+
                       " " + word + "s loudly: ", 
                       arg, cur_lang);
  if (cur_lang != "common") word += " in "+cur_lang;
  my_mess("You " + word + " loudly: ", arg);
  this_player()->adjust_time_left(-5);
  return 1;
} /* do_loud_say() */

int do_say(string arg, int no_echo) {
  string word, s1, s2;
  object g;

  if (!arg) 
    arg = "";
  if (arg == "" || arg == " ") {
    notify_fail("Syntax: say <something>\n");
    return 0;
  }
  if (!LANGUAGE_HAND->query_language_spoken(cur_lang)) {
    notify_fail(capitalize(cur_lang)+" is not a spoken language.\n");
    return 0;
  }
  word = query_word_type(arg);
  if (this_object()->query_volume(D_ALCOHOL))
    arg = drunk_speech(arg);
  arg = de_eight(arg);
  event(environment(), "person_say", this_object()->query_cap_name()+
                       " " + word + "s: ", arg, cur_lang);
  if (!no_echo) {
    if (cur_lang != "common")
      word += " in "+cur_lang;
    my_mess("You " + word + ": ", arg);
  }
  this_player()->adjust_time_left(-5);
  return 1;
} /* do_say() */

int do_tell(string arg, object ob, int silent) {
  string str, rest, word;

  if ((!arg || arg == "") && !ob) {
    notify_fail("Syntax: tell person <message>\n");
    return 0;
  }
  if (!ob) {
    if (sscanf(arg,"%s %s",str,rest)!=2) {
      notify_fail("Syntax: tell <name> something\n");
      return 0;
    }
  } else
    rest = arg;

  if (!LANGUAGE_HAND->query_language_spoken(cur_lang)) {
    notify_fail(capitalize(cur_lang)+" is not a spoken language.\n");
    return 0;
  }
  if (!LANGUAGE_HAND->query_language_distance(cur_lang)) {
    notify_fail(capitalize(cur_lang)+" is not able to spoken at a "+
                "distance.\n");
    return 0;
  }
  if (!ob) {
    str = lower_case(str);
    str = (string)this_object()->expand_nickname(str);
    ob = find_player(str);
    if (!ob)
      ob = find_living(str);
    if (!ob) {
      notify_fail(capitalize(str) + " is not logged in.\n");
      return 0;
    }
  }
  if (ob == this_player()) {
    notify_fail("Talking to yourself again.  I don't know.\n");
    return 0;
  }
  if (ob->query_property("player") && !interactive(ob)) {
    notify_fail(ob->query_cap_name()+" is net dead.\n");
    return 0;
  }
  if (my_file_name == "/global/player" && 
      adjust_social_points(-TELL_COST) < 0) {
    notify_fail(NO_POWER);
    return 0;
  }
  word = query_word_type(rest, "");
  if (word != "")
    word = " " + word + "ing";
  if (this_object()->query_volume(D_ALCOHOL))
    arg = drunk_speech(arg);
  if (word != " asking") {
    ob->event_person_tell(this_object(), this_object()->query_cap_name()+
                       " tells you" + word + ": ", rest, cur_lang);
    if (cur_lang != "common") word += " in "+cur_lang;
    if (!silent)
      my_mess("You tell " +  ob->query_cap_name() + word + ": ", rest);
  } else {
    ob->event_person_tell(this_object(), this_object()->query_cap_name()+
                       " asks you: ", rest, cur_lang);
    if (cur_lang != "common") word += " in "+cur_lang;
    if (!silent)
      my_mess("You ask " + ob->query_cap_name()+": ", rest);
  }
  this_player()->adjust_time_left(-5);
  return 1;
} /* do_tell() */

int do_emote(string arg) {
  string str;

  if(my_file_name=="/global/player"&&!this_player()->query_property("emote")) {
    notify_fail(NOT_ALLOWED);
    return 0;
  }
  if (!arg)
    arg = "";

  str = query_verb();

  if (arg == "" || arg == " ") {
    notify_fail("Syntax: emote <womble>\n");
    return 0;
  }

  if (my_file_name == "/global/player" && 
      adjust_social_points(-EMOTE_COST) < 0) {
    notify_fail(NO_POWER);
    return 0;
  }
  this_player()->adjust_time_left(-5);
  if (this_object()->query_volume(D_ALCOHOL))
    arg = drunk_speech(arg);
  str = (string)this_object()->query_cap_name()+" "+arg+"%^RESET%^\n";

  say(str);
  write(str);
  return 1;
} /* do_emote() */

/* ok... the shout cost is charged for every 10 letters, plus a bonus
 * one for the start charge..  SO a shout of yes will cost 1 social
 * point... where as a shout of lots a letters will cost lots
 */
int do_shout(string str) {
  string tmp, s1, s2, s;
  object g;

  if(!str || str == "") {
    notify_fail("Syntax : shout <text>\n");
    return 0;
  }
  if (this_object()->check_earmuffs("shout")) {
    notify_fail("Why shout when you can't hear people shout back?\n");
    return 0;
  }
  if (!LANGUAGE_HAND->query_language_spoken(cur_lang)) {
    notify_fail(capitalize(cur_lang)+" is not a spoken language.\n");
    return 0;
  }
  if (!LANGUAGE_HAND->query_language_distance(cur_lang)) {
    notify_fail(capitalize(cur_lang)+" is not able to spoken at a "+
                "distance.\n");
    return 0;
  }
  if (my_file_name == "/global/player" && 
      adjust_social_points(-SHOUT_COST*((strlen(str)/10)+1)) < 0) {    
    notify_fail(NO_POWER);
    return 0;
  }
  s1 = query_shout_word_type(str);
  if (s1 != "yell")
    s = "shouts"+s1;
  else
    s = s1+"s";
  if (this_object()->query_volume(D_ALCOHOL))
    str = drunk_speech(str);
  str = replace(str, "", "");
  if (s1 != "yell") {
    if (cur_lang != "common") s1 += " in "+cur_lang;
    my_mess("You shout" + s1 + ": ", str);
  } else {
    if (cur_lang != "common") s1 += " in "+cur_lang;
    my_mess("You " + s1 + ": ", str);
  }
  s2 =  " " + lower_case(str);
  if (sscanf(s2, "%seight%s", s1, s1)==2 ||
      sscanf(s2, "%s8%s", s1, s1)==2) {
/*
    s2 = " "+lower_case(str);
    s1 = replace(s2, "8", "", " ", "");
*/
    s1 = replace(s2, "8", "", " eight", "eight");
    if (s1 == "" || s1 == "eight") {
    g = (object)this_object()->query_guild_ob();
    write("Oops...\n");
    if (random(100) < 40 || (g && (string)g->query_name() == "wizards"))
      call_out("summon_bel_shamharoth", 3);
    }
  }
  event(users(), "person_shout", this_object()->query_cap_name()+
                 " "+s, str, cur_lang);
  return 1;
} /* do_shout() */

int do_creator_tell(string mess) {
  if (!this_object()->query_creator())
    return 0;
  if (!mess || mess == "") {
    notify_fail("Syntax: cre <text>\n");
    return 0;
  }
  if (this_object()->check_earmuffs("creator-tell")) {
    write("Why use creator-tell when you can't hear a response?\n");
    return 1;
  }
  mess = replace(mess, "", "");
  event(users(), "creator_tell", (string)this_object()->query_cap_name() + ": ",
        mess);
  my_mess("You creator-tell: ", mess);
  this_player()->adjust_time_left(-DEFAULT_TIME);
  return 1;
} /* do_creator_tell() */


int do_echo(string str) {
  if(my_file_name=="/global/player"&&!this_player()->query_property("echo")) {
    notify_fail(NOT_ALLOWED);
    return 0;
  }
  if (!str || str == "") {
    notify_fail("Syntax : echo <text>\n");
    return 0;
  }
  if (my_file_name == "/global/player" && 
      adjust_social_points(-ECHO_COST) < 0) {
    notify_fail(NO_POWER);
    return 0;
  }
  if (this_object()->query_volume(D_ALCOHOL))
    str = drunk_speech(str);
  log_file("ECHOS", ctime(time())+" "+this_player()->query_name()+" echo's: "+
                    str+"\n");
  str += "%^RESET%^";
  my_mess("You echo: ", str);
  event(environment(), "player_echo", str + "\n");
  return 1;
} /* do_echo() */
 
int do_echo_to(string str) {
  string who, what;
  object ob;
  if(my_file_name=="/global/player"&&!this_player()->query_property("echoto")) {
    notify_fail(NOT_ALLOWED);
    return 0;
  }
  if(!str || str == "") {
    notify_fail("Syntax : echoto <player> <text>\n");
    return 0;
  }
  if(sscanf(str, "%s %s", who, what) != 2) {
    notify_fail("Syntax : echoto <player> <text>\n");
    return 0;
  }
  who = lower_case(who);
  who = (string)this_object()->expand_nickname(who);
  if (my_file_name == "/global/player" && 
      adjust_social_points(-ECHOTO_COST) < 0) {
    notify_fail(NO_POWER);
    return 0;
  }
  ob = find_player(who);
  if (this_object()->query_volume(D_ALCOHOL))
    what = drunk_speech(what);
  log_file("ECHOS", ctime(time())+" "+this_player()->query_cap_name()+
                    " Echoto's "+who+": " +what+"\n");
  what += "%^RESET%^";
  my_mess("You echo to " + who + ": ", what);
  event(ob, "player_echo_to", what + "\n");
  return 1;
} /* do_echo_to() */

int do_emote_all(string str) {
  if(my_file_name=="/global/player"&&
        !this_player()->query_property("emoteall")) {
    notify_fail(NOT_ALLOWED);
    return 0;
  }
  if(!str || str == "") {
    notify_fail("Syntax : emoteall <string>\n");
    return 0;
  }
  if (my_file_name == "/global/player" && 
      adjust_social_points(-EMOTEALL_COST*((strlen(str)/10)+1)+100) < 0) {
    notify_fail(NO_POWER);
    return 0;
  }
  str = replace(str, "", "");
  if (this_object()->query_volume(D_ALCOHOL))
    str = drunk_speech(str);
  log_file("ECHOS", ctime(time())+" "+this_player()->query_cap_name()+
                    " Emotealls: "+str+"\n");
  str += "%^RESET%^";
  my_mess("You emoteall : ", this_player()->query_cap_name()+" " + str);
  event(users(), "player_emote_all", this_player()->query_cap_name() + " "
                 + str + "\n");
  return 1;
} /* do_emote_all() */

int converse(string str) {
  if (!LANGUAGE_HAND->query_language_spoken(cur_lang)) {
    notify_fail(capitalize(cur_lang)+" is not a spoken language.\n");
    return 0;
  }
  if (str) {
    if (!LANGUAGE_HAND->query_language_distance(cur_lang)) {
      notify_fail(capitalize(cur_lang)+" is not able to spoken at a "+
                  "distance.\n");
      return 0;
    }
    str = lower_case(str);
    str = (string)this_object()->expand_nickname(str);
    converse_ob = find_player(str);
    if (!converse_ob)
      converse_ob = find_living(str);
    if (!converse_ob) {
      notify_fail("Syntax: converse [player]\n");
      return 0;
    }
    if (converse_ob == this_object()) {
      notify_fail("You have a nice conversation with yourself. Gee, what fun.\n");
      return 0;
    }
  }
  write("Give '**' to stop.\n");
  write("] ");
  input_to("do_converse");
  return 1;
} /* converse() */

void do_converse(string str) {
  if(str == "**") {
    write("Ok.\n");
    return;
  }
  if(str)
/*
    if(str[0] == '!')
      command(str[1..strlen(str)-1]);
    else */ if (!converse_ob)
      do_say(str, 1);
    else
      do_tell(str, converse_ob, 1);
  write("] ");
  input_to("do_converse");
} /* do_converse() */

int do_whisper(string str) {
  object *obs;
  string s, s2, *bits, peeps;
  int i;

  notify_fail("Syntax: whisper <string> to <person>\n");
  if (!str)
    return 0;
  if (!LANGUAGE_HAND->query_language_spoken(cur_lang)) {
    notify_fail(capitalize(cur_lang)+" is not a spoken language.\n");
    return 0;
  }
  str = replace(str, " to", "  to");
  bits = explode(str, " to ");
  if (!bits || sizeof(bits) < 2 || (sizeof(bits) == 2 && bits[0] == ""))
    return 0;
  s2 = replace(implode(bits[0..sizeof(bits)-2], " to "), "  to", " to");
  obs = find_match(bits[sizeof(bits)-1], environment());
  if (sizeof(obs) == 1 && obs[0] == this_player()) {
    say(this_player()->query_cap_name()+" whispers to "+
        this_player()->query_objective()+"self.\n");
    write("Whispering to your self!?\n");
    return 1;
  }
  obs = obs - ({ this_player() });
  for (i=0;i<sizeof(obs);i++)
    if (!living(obs[i]))
      obs = delete(obs, i--, 1);
  if (!sizeof(obs)) {
    notify_fail("Could not find anyone to whisper to.\n");
    return 0;
  }
  if (this_object()->query_volume(D_ALCOHOL))
    s2 = drunk_speech(s2);
  s2 += "%^RESET%^";
  s = query_whisper_word_type(s2);
  event(environment(), "whisper", this_object()->query_cap_name() +
        " whispers " + s, s2, obs, cur_lang);
  my_mess("You whisper " + s + "to " + query_multiple_short(obs) + ": ", s2);
  this_player()->adjust_time_left(-5);
  return 1;
} /* do_whisper() */

string drunk_speech(string str) {
/* I think I like replace already */
  return replace(str, ({ "s", "sh", "r", "rr", "ing", "in'", "x", "xsh",
                         "S", "SH", "R", "RR" }));
} /* drunk_speech() */

int summon_bel_shamharoth(int quiet) {
  string mess;
  object *ob;
  int i;
  if ("/secure/master"->high_programmer(geteuid()))
    mess = "The heavens shake with a hideous roar and just as suddenly all "+
           "is quiet.\n" + this_object()->query_cap_name() + " chuckles "+
           "in the distance.\n";
  else
    mess = "A sudden darkness passes over the land as " + 
         this_object()->query_cap_name() + " is carried off screaming to " +
         "the land of Shades.\n";
  if (!quiet) {
    ob = users();
    for (i=0; i < sizeof(ob); i++) {
      if (ob[i]!=this_object() && !ob[i]->query_earmuffs())
        tell_object(ob[i], sprintf("%-=*s", ((int)ob[i]->query_cols()-1), 
                                            mess));
    }
  }
  if ("/secure/master"->high_programmer(geteuid()))
    mess = "Your godhood saves you from a grizzly encounter with the "+
           "Sender of Eight.\n";
  else
    mess = "You think you'd best be careful what you say in the future as " +
         "shadowy tentacles drag you into the earth...\n";
  printf("%-=*s", ((int)this_object()->query_cols()-1), mess);
  if(!"/secure/master"->high_programmer(geteuid()))
    this_object()->quit();
  return 1;
} /* summon_bel_shamharoth() */

void add_language(string lang) {
  if (!LANGUAGE_HAND->test_language(lang))
    return ;
  if (member_array(lang, languages) != -1)
    return ;
  languages += ({ lang });
} /* add_language() */

void remove_language(string lang) {
  int i;

  if ((i = member_array(lang, languages)) == -1)
    return ;
  languages = delete(languages, i, 1);
  if (lang == cur_lang) {
    if (!sizeof(languages))
      cur_lang = "grunt";
    else
      cur_lang = languages[0];
    tell_object(this_object(), "You just forgot the language you were "+
                "speaking.  You are now speaking "+cur_lang+".\n");
  }
} /* remove_language() */

int set_language(string str) {
  if (!str) {
    notify_fail("You are now speaking "+cur_lang+" and can speak any of "+
                query_multiple_short(languages+({ "grunt" }))+".\n");
    return 0;
  }
  if (member_array(str, languages+({ "grunt" })) == -1) {
    notify_fail("You do not know "+str+".\n");
    return 0;
  }
  cur_lang = str;
  write("Now using "+str+" for speaking and writing.\n");
  return 1;
} /* set_language() */

string query_current_language() { return cur_lang; }
 
string *query_languages() { return languages; }

int query_language(string str) {
  if (member_array(str, languages) == -1)
    return 0;
  return 1;
} /* query_language() */
