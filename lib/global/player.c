/*
 * the wonder discworld player object
 */
#include "library.h"
#include "tune.h"
#include "login_handler.h"
#include "drinks.h"
#include "weather.h"
#include "log.h"

inherit "/global/line_ed";
inherit "/global/auto_load";
inherit "/global/events";
inherit "/global/log";
inherit "/global/spells";
inherit "/global/help";
inherit "/global/more_string";
inherit "/global/finger";
inherit "/global/pweath";
inherit "/std/living/living";
inherit "/std/living/handle";
inherit "/global/stats-rear";
inherit "/global/psoul";
inherit "/global/guild-race";
inherit "/global/drunk";
inherit "/global/last";
inherit "/global/more_file";
inherit "/global/path";

#include "money.h"
#include "mail.h"
#include "player.h"

#define START_POS "/d/am/am/mendeddrum"
#define STD_RACE "/std/races/human"
#define CABBAGE_SHADOW "/obj/cabbage_shadow"

/* 
 * Discworld stuff
 */
string cap_name;
string *auto_load, last_on_from, last_pos;
mixed *money_array;
string title, al_title;
int headache, max_headache, time_on, max_deaths, monitor;
int ntime_last_saved,i;
int invis, *saved_co_ords, ed_setup, start_time;
int creator, app_creator, deaths, level, last_log_on, alignment;
static object snoopee;

void start_player();
void run_away();
void set_title(string str);
void public_commands();
int save_me();
int save();
void set_desc(string str);
string query_title();
int query_creator();
void set_name(string str);
int check_dark(int light);

void create() {
  int *i,j,k;

  living::create();
  events::create();
  pweath::create();
  psoul::create();
  line_ed::create();
  add_property("determinate", "");
  spells = ({ });
  money_array = ({ });
  time_on = time();
  start_time = time();
  seteuid("PLAYER");
  Str = 13;
  Dex = 13;
  Int = 13;
  Con = 13;
  Wis = 13;
  max_social_points = 50;
  max_deaths = 7;
  desc = 0;
  add_attack("hands", 0, 75, 30, 0, 0, "blunt");
  add_attack("feet", 0, 25, 40, 0, 0, "blunt");
  add_ac("bing", "blunt", 15);
  add_ac("bing2", "sharp", 15);
  add_ac("bing3", "pierce", 15);
  add_property("player", 1);
  cols = 79;
  rows = 24;
  verbose = 1;
  last_log_on = time();
  race_ob = RACE_STD;
  sscanf(file_name(this_object()), "%s#", my_file_name);
} /* create() */

string query_start_pos() {
  string start_pos;

  if (!guild_ob)
    start_pos = START_POS;
  else
    catch(start_pos = (string)guild_ob->query_start_pos());
  if (!find_object(start_pos))
    if (catch(call_other(start_pos,"??")))
      start_pos = START_POS;
  return start_pos;
} /* query_start_pos() */

string query_cap_name() { return cap_name; }

void move_player_to_start(string bong, int new, string c_name) {
int tmp;
object money;
string s;
  if (file_name(previous_object())[0..13] != "/secure/login#") {
    notify_fail("You dont have the security clearance to do that.\n");
    return ;
  }
/* some stupid test to make sure that the previous object is /secure/login. */
  seteuid("Root");
  set_name(bong);
  if (query_property("guest"))
    log_file("ENTER", sprintf("Enter : %15-s %s (guest)[%d] [%s]\n",
               name, ctime(time()), time(),
               (query_ip_name()?query_ip_name():query_ip_number())));
  else
    log_file("ENTER", sprintf("Enter : %15-s %s[%d]\n",
               name, ctime(time()), time()));
  restore_object("/players/"+name[0..0]+"/"+name);
  add_property("player", 1);
  cap_name = c_name;
  set_short(cap_name);
  if (!cols) cols = 79;
  add_property("determinate", "");
  if (this_player()->query_creator())
    seteuid(name);
  else
    seteuid("PLAYER");
  write("You last logged in from "+last_on_from+".\n");
  last_on_from = query_ip_name(this_object())+" ("+
                 query_ip_number(this_object())+")";
  bonus_cache = ([ ]);
  level_cache = ([ ]);
  if (time_on < -500*24*60*60)
    time_on += time();
  if (time_on > 0)
    time_on = 0;
  time_on += time();
  guild_joined += time();
  ntime_last_saved += time();
  start_player();
/* ok this moves us to the other player... ie we are already playing. */
  if (query_name() != "tokzic")
    write("Pinkfish shouts: "+cap_name+" is a frog!!!!!!!\n");
  else
    write("Pinkfish shouts: "+cap_name+" is not a frog!!!!!!\n");
  if(!msgin || msgin[0] == '@')
    msgin = msgout = mmsgin = mmsgout = 0;
  if(!msgin)
    msgin = "$N arrive$s from $F.";
  if(!msgout)
    msgout = "$N leave$s $T.";
  if(!mmsgin)
    mmsgin = "$N appear$s out of the ground.";
  if(!mmsgout)
    mmsgout = "$N %vanish% in a puff of smoke.";
  cat("doc/NEWS"); 
  if (!last_pos || catch(call_other(last_pos, "??"))) {
    last_pos = query_start_pos();
    saved_co_ords = 0;
  }
  move(last_pos);
/*
 * Set the old co ord if one does not already exist.  Otherwise don't
 * destroy the already existing nameing scheme.
 */
  if (!last_pos->query_co_ord() && saved_co_ords)
    last_pos->set_co_ord(saved_co_ords);
  event(users(), "inform", query_cap_name() +
    " enters " + (query_property("guest")?"as a guest of ":"") + "Discworld"+
    (new?" (New player)":""), 
    "logon");
  say(cap_name+" enters the game.\n", 0);
  if (verbose)
    command("look");
  else
    command("glance");
  no_time_left();
  last_pos->enter(this_object());
  money = clone_object(MONEY_OBJECT);
  money->set_money_array(money_array);
  money->move(this_object());
  if (query_property(PASSED_OUT_PROP))
    call_out("remove_property", 10+random(30), PASSED_OUT_PROP);
  init_after_save(); /* for effects.  Sorry about the name */
  curses_start();
  write((string)MAILER->new_mail(name));
  if (query_property("dead")) {
    money = clone_object(DEATH_SHADOW);
    money->setup_shadow(this_object());
  }
  if(query_property("noregen"))
    DEATH_CHAR->person_died(query_name());
  exec_alias("login","");
  last_log_on = time();
  set_title((string)"obj/handlers/library"->query_title(query_name()));
  LOGIN_HANDLER->player_logon(bong);
  if (my_file_name != "/global/player")
    if (file_size("/w/"+name+"/"+PLAYER_ERROR_LOG) > 0)
      write("You have ERRORS in /w/"+name+"/"+PLAYER_ERROR_LOG+"\n");
} /* move_player_to_start() */

void start_player() {
  if (app_creator && my_file_name != "/global/player") {
    this_player()->all_commands();
    this_player()->app_commands();
  }
  if (creator && my_file_name != "/global/player") 
    this_player()->wiz_commands();
  reset_get();
  enable_commands();
  public_commands();
  parser_commands();
  handle_commands();
  force_commands();
  race_guild_commands();
  soul_commands();
  event_commands();
  finger_commands();
  communicate_commands();
  living_commands();
  spell_commands();
  logging_commands();
  weather_commands();
  editor_commands();
  set_living_name(name);
  set_no_check(1);
  set_con(Con);
  set_str(Str);
  set_int(Int);
  set_dex(Dex);
  set_wis(Wis);
  reset_all();
  current_path = home_dir;
  call_out("do_load_auto", 0);
  birthday_gifts(); /* check if birthday today and give gifts */
  set_heart_beat(1);
  if (wimpy > 100)
    wimpy = 25;
  if (ntime_last_saved < 0)
    call_out("do_auto_save", TIME_BETWEEN_SAVES);
  else
    call_out("do_update_tmps", TIME_BETWEEN_SAVES);
} /* start_player() */

void init() {
/*
  this_player()->add_command("eat", this_object());
 */
} /* init() */

int do_eat() {
  if (this_object()->query_cabbage_shadow()) {
    this_object()->dest_cabbage_shadow();
    tell_object(this_object(), "You feel a weight lifted off your shoulders.\n");
    return 1;
  }
  return 0;
} /* do_eat() */

int do_load_auto() { 
  load_auto_load(auto_load, this_object());
} /* do_load_auto() */

void public_commands() {
  add_action("rearrange", "rearrange");
  add_action("do_su", "su");
  add_action("help_func","help");
  add_action("last", "last");
  add_action("restart_heart_beat", "restart");
  add_action("look_me","l*ook");
  add_action("glance", "glance");
  add_action("kill","kill");
  add_action("stop","stop");
  add_action("invent","inventory");
  add_action("invent","i");
  add_action("score","score");
  add_action("brief_verbose","brief");
  add_action("brief_verbose", "verbose");
  add_action("save","save");
  add_action("quit","quit");
  add_action("setmin", "setmin");
  add_action("setmout", "setmout");
  add_action("setmmin", "setmmin");
  add_action("setmmout", "setmmout");
  add_action("review", "review");
  add_action("examine","ex*amine");
  add_action("toggle_wimpy", "wimpy");
  add_action("monitor", "mon*itor");
  add_action("consider", "con*sider");
  add_action("do_refresh", "refresh");
  add_action("do_teach", "teach");
  add_action("do_cap", "cap");
} /* public_commands() */

void do_auto_save() {


  save();
  call_out("do_update_tmps", TIME_BETWEEN_SAVES);
  ntime_last_saved = time();
}

void do_update_tmps() {
  update_tmps();
  call_out("do_auto_save", TIME_BETWEEN_SAVES);
  ntime_last_saved = -time();
}

int invent() {
  write(query_living_contents(1));
  return 1;
}

int glance(string arg) {
  object *ob;
  int i, dark;
  string ret;
  
  if (!environment()) {
    notify_fail("You are in limbo... sorry you can't look at anything.\n");
    return 0;
  }
  dark = check_dark((int)environment()->query_light());
  if (!arg) {
    if (this_object()->query_creator())
      write(file_name(environment())+"\n");
    if(!dark)
      ret = (string)environment()->query_contents();
    else
      ret = "";
    write((string)environment()->short(dark, 1)+ ".\n" + ret);
    return 1;
  }

  if (!sscanf(arg, "at %s", arg)) {
    notify_fail("Glance at something!\n");
    return 0;
  }

  ob = find_match(arg, ({ this_object(), environment() }) );

  if (sizeof(ob)) {
    for (i=0;i<sizeof(ob);i++)
      write(ob[i]->short(dark)+".\n");
    return 1;
  }

  notify_fail("You do not think that the "+arg+" is here.\n");
  return 0;
}

int look_me(string arg) {
  object *ob;
  int i, dark;
  string ret;

  if (!environment()) {
    notify_fail("You are in limbo... sorry you can't look at anything.\n");
    return 0;
  }
  dark = check_dark((int)environment()->query_light());
  if (!arg || !stringp(arg)) {
    if (this_object()->query_creator())
      write(file_name(environment())+"\n");
    write(environment()->long(0, dark));
    this_player()->adjust_time_left(-5);
    return 1; /* blame evan */
  }

  sscanf(arg, "at %s", arg);

  ob = find_match(arg, ({ this_object(), environment() }) );
  ret = "";
  this_player()->adjust_time_left(-DEFAULT_TIME);
  if (sizeof(ob)) {
    if (pointerp(ob)) {
      for (i=0;i<sizeof(ob);i++) {
        ret += ob[i]->long(arg, dark);
        this_player()->adjust_time_left(2);
      }
    } else {
      ret += ob->long(arg, dark);
      this_player()->adjust_time_left(2);
    }
    more_string(ret, "Look");
    return 1;
  }

  notify_fail("You do not think that the "+arg+" is here.\n");
  return 0;
}

int setmin(string str) {
  if(my_file_name == "/global/player" &&
      !this_player()->query_property("setm")) {
    notify_fail("You are not allowed that power yet.\n");
    return 0;
  }
  str = str + " ";
  if (sizeof(explode("^"+str, "$N")) < 2 || sizeof(explode("^"+str, "$F")) < 2) {
    notify_fail("Must have a $N and a $F in your message in.\n");
    return 0;
  }
  msgin = str;
  write("Ok.\n");
  return 1;
}

int setmout(string str) {
  if(my_file_name == "/global/player" &&
      !this_player()->query_property("setm")) {
    notify_fail("You are not allowed that power yet.\n");
    return 0;
  }
  str = str + " ";
  if (sizeof(explode("^"+str, "$N")) < 2 || sizeof(explode("^"+str, "$T")) < 2) {
    notify_fail("Must have a $N and a $T in your message out.\n");
    return 0;
  }
  msgout = str;
  write("Ok.\n");
  return 1;
}

int setmmin(string str) {
  if(my_file_name == "/global/player" &&
      !this_player()->query_property("setmm")) {
    notify_fail("You are not allowed that power yet.\n");
    return 0;
  }
  str = str + " ";
  if (sizeof(explode("^"+str, "$N")) < 2) {
    notify_fail("Must have a $N in your teleport message in.\n");
    return 0;
  }
  mmsgin = str;
  write("Ok.\n");
  return 1;
}

int setmmout(string str) {
  if(my_file_name == "/global/player" &&
      !this_player()->query_property("setmm")) {
    notify_fail("You are not allowed that power yet.\n");
    return 0;
  }
  str = str + " ";
  if (sizeof(explode("^"+str, "$N")) < 2) {
    notify_fail("Must have a $N in your teleport message out.\n");
    return 0;
  }
  mmsgout = str;
  write("Ok.\n");
  return 1;
}

int review() {
  write("Entry  : " + msgin + "\n");
  write("Exit   : " + msgout + "\n");
  write("MEntry : " + mmsgin + "\n");
  write("MExit  : " + mmsgout + "\n");
  write("Editor : " + editor + "\n");
  return 1;
}

int examine(string arg) {
  return look_me("at "+arg);
}

string short(int dark) {
  string str;

  if (!interactive(this_object()))
    str = "The net dead statue of ";
  else
    str = "";
/*
  if (this_player()->query_creator())
    return str+::short(dark)+(in_editor?" Editing: "+(string)in_editor:"");
 */
  return str+::short(dark);
}

int toggle_wimpy(string str) {
/*
  if (str == "on")
    wimpy = hp;
  else if (str == "off")
    wimpy = 0;
 */
  if (!str) {
    notify_fail("Usage: "+query_verb()+" <num> (caution read help wimpy)\n");
    return 0;
  } else {
    if (sscanf(str, "%d", wimpy) != 1) {
      notify_fail("You must set wimpy to a number.\n");
      return 0;
    }
    if (wimpy < 0 || wimpy > 100) {
      notify_fail("Wimpy must be in the range 0-100, turning wimpy off.\n");
      wimpy = 0;
      return 0;
    }
  }
  if (wimpy)
    write ("You are in wimpy mode, you will run away at "+wimpy+
           "% of your max hps.\n");
  else
    write("You are in brave mode.\n");
  return 1;
}

int brief_verbose(string str) {
  if (str == "on")
    verbose = (query_verb() == "verbose");
  else if (str == "off")
    verbose = (query_verb() == "brief");
  else if (str) {
    notify_fail("Usage: "+query_verb()+" <on/off>\n");
    return 0;
  } else
    verbose = !verbose;

  if (verbose)
    write ("You are in verbose mode.\n");
  else
    write("You are in brief mode.\n");
  return 1;
}

int score(string str) {
  int age, i;
  string *st;

  if (str)
    st = explode(str, " ");
  else
    st = ({ });
  if ((verbose && (member_array("brief", st) == -1)) ||
      (member_array("verbose", st) != -1)) {
    write("You are using the new and improved Discworld player object, "+
             "beware of bugs.\n");
    if (member_array("stats", st) != -1) {
      write("Here are some arbitrary numbers for you to look at:\n");
      printf("%-#*s\n", query_cols(), 
             "Strength     : "+stat_string(query_str())+
           "\nIntelligence : "+stat_string(query_int())+
           "\nDexterity    : "+stat_string(query_dex())+
           "\nConsitution  : "+stat_string(query_con())+
           "\nWisdom       : "+stat_string(query_wis())+"\n");
      return 1;
    }
    write("You have "+hp+"("+max_hp+") hit points, "+
          gp+"("+max_gp+") guild points and "+social_points+"("+
          max_social_points+") social points.\n");
    write("Your current experience is "+xp+", and you have died "+deaths+
          " times.  You can die "+(max_deaths-deaths)+" more times before "+
          "you are completely dead.\n");
    write("You are ");
    age = time_on - time();
    age = -age;
    if (age > 86400)
      printf("%d days, ", age/86400);
    if (age > 3600)
      printf("%d hours, ", (age/3600)%24);
    if (age > 60)
      printf("%d minutes and ", (age/60)%60);
    printf("%d seconds old.\n", age % 60);
    if (wimpy)
      write("Wimpy set to "+wimpy+"%\n");
    else
      write("You are in brave mode.\n");
    return 1;
  } else if (member_array("stats", st) != -1) {
    write("Here are some arbitrary numbers for you to look at:\n");
    printf("%-#*s\n", query_cols(), 
           "Str : "+stat_string(query_str())+
           "\nInt : "+stat_string(query_int())+
           "\nDex : "+stat_string(query_dex())+
           "\nCon : "+stat_string(query_con())+
           "\nWis : "+stat_string(query_wis())+"\n");
    return 1;
  }
  write("Hp: "+hp+"("+max_hp+")"+"  Gp: "+gp+"("+max_gp+")"+"  Xp: "+
         xp+"\n");
  return 1;
}

nomask int save() {
  if (my_file_name == "/global/player" || query_verb() == "save")
    tell_object(this_object(), "Saving...\n");
  save_me();
  return 1; 
}

void save_me() {
  object ob;
  mixed old;

  if (query_property("guest")) {
    write("But not saving for guests... sorry.\n");
    return ;
  }
  if ((ob = present("Some Money For Me", this_object())))
    money_array = (mixed *)ob->query_money_array();
  else
    money_array = ({ });
  if (guild_ob)
    guild_ob->player_save();
  if (race_ob)
    race_ob->player_save();
  old = geteuid();
  if (environment())
    last_pos = file_name(environment());
  else
    last_pos = query_start_pos();
  if (last_pos[0..2] == "/w/") {
    if (last_pos[3..strlen(name)+2] != name)
      last_pos = query_start_pos();
  }
  auto_load = create_auto_load(all_inventory());
  saved_co_ords = (int *)last_pos->query_co_ord();
  time_on -= time();
  guild_joined -= time();
  ntime_last_saved -= time();
  seteuid("Root");
  catch(save_object("/players/"+name[0..0]+"/"+name));
  seteuid(old);
  time_on += time();
  guild_joined += time();
  ntime_last_saved += time();
} /* save_me() */

int quit() {
  object *ob, money;
  object frog, frog2;
  int i;

  if (this_player() != this_object()) {
    time_left = 100; /* Make sure it gets done... */
    return command("quit");
  }
  last_log_on = time();
  log_file("ENTER", sprintf("Exit  : %15-s %s[%d]\n", name, 
           ctime(time())+ (query_property("guest")?"(guest)":""), time()));
  catch(editor_check_do_quit());
  write("Thanks for playing see you next time.\n");
  say(cap_name + " left the game.\n");
  event(users(), "inform", query_cap_name() + " left Discworld", "logon");
  LOGIN_HANDLER->player_logout(query_name());
  if (race_ob)
    catch(race_ob->player_quit(this_object()));
  if (guild_ob)
    catch(guild_ob->player_quit(this_object()));
  curses_quit();
  save_me();
/* get rid of the money....
 * we dont want them taking it twice now do we?
 */
  if ((money = present("Some Money For Me", this_object())))
    money->dest_me();
  frog = first_inventory(this_object());
  while (frog) {
    frog2 = next_inventory(frog);
    if (frog->query_auto_load() ||
        frog->query_static_auto_load())
      frog->dest_me();
    frog = frog2;
  }
  transfer_all_to(environment());
  ob = all_inventory(this_object());
  for (i=0;i<sizeof(ob);i++)
    ob[i]->dest_me();
  dest_me();
  return 1;
} /* quit() */
 
string query_title() { return title; }
void set_title(string str) { title = str; }

string query_atitle() { return al_title; }
void set_atitle(string str) { al_title = str; }

int query_deaths() { return deaths; }

string query_stime() {
  if (ntime_last_saved>0)
    return ctime(ntime_last_saved);
  return ctime(-ntime_last_saved);
} /* query_stime() */

void set_name(string str) {
  if (name && name != "object")
    return ;
  name = str;
  set_living_name(name);
  set_main_plural(name);
} /* set_name() */

string long(string str, int dark) {
  string s;

  if (str == "soul") {
    return (string)"/obj/handlers/soul"->query_long();
  }
  if (str == "sun" || str == "moon" || str == "weather") {
    return weather_long(str);
  }
/*	Because I like to see myself--what a narcissist!  */
  if (this_player() == this_object())
          write("Looking at yourself again? What a narcissist!\n");
  else
    tell_object(this_object(), this_player()->query_cap_name()+" looks at you.\n");
  s = "You see "+cap_name;
  if (guild_ob)
    s += " "+(string)guild_ob->query_title(this_object());
  if (al_title)
    s += " ("+al_title+"), \n";
  else
    s += ",\n";
  if (race_ob)
    s += (string)race_ob->query_desc(this_object());
  if (desc && desc != "")
    s += query_cap_name() + " " + desc+"\n";
  s += capitalize(query_pronoun())+" "+health_string()+".\n";
  labels = labels - ({ 0 });
  if (sizeof(labels))
    s += "There is "+query_multiple_short(labels)+" stuck on "+
         query_objective()+".\n";
  s += calc_extra_look();
  s += weather_extra_look();
  s += query_living_contents(0);
  return s;
} /* long() */

/* second life routine... handles the player dieing. */
int second_life() {
  string str;
  int i, no_dec;
  object tmp;

  make_corpse()->move(environment());
  str = query_cap_name() + " killed by ";
  if (!sizeof(attacker_list))
    str += this_player()->query_cap_name()+" (with a call)";
  else
    for (i=0;i<sizeof(attacker_list);i++)
      if (attacker_list[i]) {
        str += attacker_list[i]->query_name()+"<"+geteuid(attacker_list[i])+"> ";
        attacker_list[i]->stop_fight(this_object());
        no_dec += interactive(attacker_list[i]);
      }
  log_file("DEATH", ctime(time())+": "+str + "\n");
  event(users(), "inform", str, "death");
  attacker_list = ({ });
  for (i=0;i<sizeof(call_outed);i++)
    call_outed[i]->stop_fight(this_object());
  call_outed = ({ });
  call_out("effects_thru_death", 0); /* this could be longer with no hassle */
  weapon = 0;
  if (!no_dec)
    deaths++;
  if (deaths > max_deaths) {
    write("You have died too many times.  I am sorry, your name will be "+
          "inscribed in the hall of legends.\n");
    shout(this_object()->query_cap_name()+" has just completely died.  "+
          "all mourn his passing.\n");
    LIBRARY->complete_death(query_name());
  }
/* oh dear complete death ;( */
  say(cap_name+" dies(sigh).\n");
  save_me();
  DEATH_CHAR->person_died(query_name());
  hp = 0;
  gp = 0;
  xp = 0;
  if (!no_dec) {
    contmp = -2;
    strtmp = -2;
    dextmp  = -2;
    inttmp = -2;
    wistmp = -2;
  }
  this_object()->add_property("dead",1);
  if (query_special_day("cabbage day"))
    tmp = clone_object(CABBAGE_SHADOW);
  else
    tmp = clone_object(DEATH_SHADOW);
  tmp->setup_shadow(this_object());
  return 1;
} 

void remove_ghost() {
  if (this_object()->query_cabbage_shadow()) {
    write("But they are a cabbage!!!!\n");
    return ;
  }
  if (deaths > max_deaths) {
    if (this_player() != this_object()) {
      tell_object(this_object(), this_player()->query_cap_name()+
                  " tried to raise you, but you are completely dead.\n");
      tell_object(this_player(), query_cap_name()+
                  " is completely dead, you cannot raise him.\n");
    } else
      tell_object(this_object(), "You are completely dead.  You cannot "+
                  "be raised.\n");
    say(query_cap_name()+" struggles to appear in a solid form, but fails.\n");
    return ;
  }
  remove_property("dead");
  tell_object(this_object(), "You reappear in a more solid form.\n");
  say(query_cap_name() + " appears in more solid form.\n");
  this_object()->dest_death_shadow();
} /* remove_ghost() */

static int hb_num;

void net_dead() {
  if (name == "guest" || name == "root") {
    say(cap_name + " just vanished in a puff of logic.\n");
    quit();
  } else {
    say(cap_name+" goes white, looks very chalky and turns into a "+
        "statue.\n");
    event(users(), "inform", query_cap_name() + " has lost " +
      query_possessive() + " link", "link-death");
    save_me();
    for (i=0;i<sizeof(attacker_list);i++)
      attacker_list[i]->stop_fight(this_object());
  }
  if (time() - last_command > MAX_IDLE) {
    say(cap_name+" has been idle for too long, "+query_pronoun()+
        " vanishes in a puff of boredom.\n");
    quit();
    return ;
  }
} /* net_dead() */

void idle_out() {
  if (((time() - last_command < MAX_IDLE) || sizeof(users()) < 30) &&
     interactive(this_object())) {
    last_command = time();
    return ;
  }
  say(cap_name+" has been idle for too long, "+query_pronoun()+
      " vanishes in a puff of boredom.\n");
  write("You idled out sorry.\n");
  quit();
} /* idle_out() */

/*
 * the heart beat. bounce what does this do? we arent going to tell you
 */
void heart_beat() {
int i, intox;

  flush_queue();
  intox = query_volume(D_ALCOHOL);
  if (drunk_heart_beat(intox) && time_left > 0) {
    if (sizeof(attacker_list)) {
      attack();
      time_left -= (int)environment()->attack_speed();
    } else {
      attack();
      if (sizeof(attacker_list))
        time_left -= (int)environment()->attack_speed();
    }
  }
  do_spell_effects(attackee);
  if (time() - last_command > MAX_IDLE)
    if (!interactive(this_object()) || my_file_name != "/global/lord") {
      call_out("idle_out", 120);
      return ;
    }
/* bing. hit point receival ;) */
  if (guild_ob)
    guild_ob->player_heart_beat(intox);
  else {
    if (hp<max_hp) {
      if (intox)
        hp += 4;
      else
        hp++;
      if (hp>max_hp)
        hp = max_hp;
    } 
    if (gp<max_gp) {
      if (intox)
        gp+=4;
      else
        gp++;
      if (gp>max_gp)
        gp = max_gp;
    }
/* handle intoxication dispersion by our selves...
 * they just handle hp recival and sp recival...
 */
  }
  if (headache)
    if (!--headache) {
      tell_object(this_object(), "Your headache disapears.\n");
      headache = 0;
    }
  if (intox) {
    if (!(intox-1)) {
      headache = 15;
      tell_object(this_object(),"You get a splitting headache, be happy.\n");
      hp -= 50;
      if (hp<1)
        hp = 1;
    }
  }
  if (++hb_num%4) {
    social_points++;
    if(social_points > max_social_points)
      social_points = max_social_points;
    if (monitor && sizeof(attacker_list)) {
      gp--;
      if (gp>0)
        write(sprintf("Hp: %d Gp: %d\n", hp, gp));
      else
        gp = 0;
    }
    if (hb_num > 512) {
      max_social_points++;
      hb_num = 0;
    }
  }
  if (sizeof(attacker_list) && wimpy && hp < max_hp*wimpy/100)
    run_away();
  update_volumes();
} /* heart_beat() */

int monitor(string str) {
  if (!str)
    monitor = !monitor;
  else if (str == "on")
    monitor = 1;
  else if (str == "off")
    monitor = 0;
  else {
    notify_fail("Syntax: monitor <on/off>\n");
    return 0;
  }
  if (monitor)
    write("Your hit point monitor is on.\n");
  else
    write("Your hit point monitor is off.\n");
  return 1;
}

void run_away() {
  mixed *direcs;
  int i, bong;

  direcs = (mixed *)environment()->query_dest_dir();
  while (!bong && sizeof(direcs)) {
    i = random(sizeof(direcs)/2)*2;
    bong = command(direcs[i]);
    if (!bong)
      direcs = delete(direcs, i, 2);
    else
      write("Your feet run away with you.\n");
  }
  if (!bong)
    write("You tried to run away, but no matter how much you scrabbled you "+
          "couldn't find any exits.\n");
}

int query_invis() { return invis; }

int help_func(string str) {
  string rest;
  mixed i;

  if (!str) return do_help(0);
  if (sscanf(str, "spell %s", rest) == 1) {
    i = help_spell(rest);
    if (i) {
      write(i);
      return 1;
    }
    notify_fail("No such spell as '"+rest+"'\n");
    return 0;
  }
  if (sscanf(str, "command %s", rest) == 1) {
    i = help_command(rest);
    if (i) {
      write(i);
      return 1;
    }
    notify_fail("No such command as '"+rest+"'\n");
    return 0;
  }
  i = do_help(str); /* calling /global/help.c */
  if (!i)
    if ((i = help_spell(str)))
      write(i);
  if (!i)
    if ((i = help_command(str)))
      write(i);
  return i;
}


int query_wizard() { return creator; } /* need this fo rthe gamed driver */
int query_app_creator() { return app_creator; }

int kill(string str) {
  int i;
object *obs,*aobs,*fobs;
 
  if (!str || str == "") {
    notify_fail("Usage: kill <object>\n");
    return 0;
  }
 
  obs = find_match(str, environment());
  if (!sizeof(obs)) {
    notify_fail("Cannot find "+str+"\n");
    return 0;
  }
 /* ok... we add non living things into the list... attack it normaly...
  * I will get people to stop attacking it in the object it self...
  * Perhaps we should hps to objects???
  * Nahhh, weapons and armours, ok... But normal objects?
  * Never.
  */
/* Yes, but it looks ugly to have it just say ok. When
you attack the door or whatever. Lets try this. Might look nicer. */
  aobs = fobs = ({ });
  for (i=0;i<sizeof(obs);i++)
  {
    if (living(obs[i]) && !(userp(obs[i]))) {
      aobs+= ({ obs[i]});
      this_object()->attack_ob(obs[i]);
    } else if(obs[i] != this_object()) fobs += ({ obs[i]});
  }
  write("Ok.\n");
  if(sizeof(aobs)) write("You attack "+query_multiple_short(aobs)+".\n");
  if (sizeof(fobs)) write("You fail to attack "+query_multiple_short(fobs)+".\n");
  return 1;
}
 
int stop(string str) {
  int i;
  object *obs;

  if (!str) {
    obs = attacker_list + call_outed;
    if (!sizeof(obs)) {
      notify_fail("Not fighting anyone.\n");
      return 0;
    }
    for (i=0;i<sizeof(obs);i++)
      stop_fight(obs[i]);
    write("Stopped fighting "+query_multiple_short(obs)+".\n");
    return 1;
  }

  obs = find_match(str, environment());
  if (find_living(str))
   obs += ({ find_living(str) });

  if (!sizeof(obs)) {
    notify_fail("Cannot find "+str+"\n");
    return 0;
  }
  for (i=0;i<sizeof(obs);i++)
    stop_fight(obs[i]);
  write("Ok, stopped fighting "+str+"\n");
  return 1;
}

void set_al(int i) { alignment = i; }
int query_al() { return alignment; }

void reset_align_title() {
/* Use this to come up with cuteoid titles */
  al_title = (string)LOGIN_HANDLER->query_al_string();
  if (al_title) return ;
  if (alignment < 20 && alignment > -20)
    al_title = "neutral";
  else if (alignment < -2000)
    al_title = "so good, you are blinded by the light";
  else if (alignment < -1000)
    al_title = "utter goodness";
  else if (alignment < -500)
    al_title = "really good";
  else if (alignment < -100)
    al_title = "paladin";
  else if (alignment < -80)
    al_title = "good";
  else if (alignment < -60)
    al_title = "goodish";
  else if (alignment < -40)
    al_title = "nice";
  else if (alignment > 2000)
    al_title = "amazingly utterly incredibly evil";
  else if (alignment > 1000)
    al_title = "utterly evil";
  else if (alignment > 500)
    al_title = "really evil";
  else if (alignment > 100)
    al_title = "evil";
  else if (alignment > 80)
    al_title = "nasty";
  else if (alignment > 60)
    al_title = "wicked";
  else if (alignment > 20)
    al_title = "not very nice";
} /* reset_align_title() */

int adjust_align(int i) {
  if (alignment != 0)
    alignment += (i*10)/(alignment<0?-alignment:alignment);
  else
     alignment += i;
  reset_align_title();
  return alignment;
} /* adjust_align() */

int query_time_on() { return time_on - time(); }
int query_last_joined_guild() { return guild_joined - time(); }

int check_dark(int light) {
  int i;

  if (race_ob)
    if (catch(i = (int)race_ob->query_dark(light)))
      race_ob = RACE_STD;
    else
      return i;
  return (int)race_ob->query_dark(light);
}

int query_level() {
  if (guild_ob)
    return (int)guild_ob->query_level(this_object());
  return 0;
}

int restart_heart_beat() {
  set_heart_beat(1);
  write("Ok, heart_beat restarted.\n");
  return 1;
}

void set_snoopee(object ob) { snoopee = ob; }
object query_snoopee() { return snoopee; }

int do_su(string str) {
  if (!str || str == "") {
    notify_fail("Usage: su <name>\n");
    return 0;
  }

  notify_fail("");
  clone_object("/secure/login")->do_su(str);
  return 1;
}


void set_creator(int i) {
  if (file_name(previous_object()) != "/secure/master") {
    write("Illegal attempt to set creator!\n");
    log_file("ILLEGAL", this_player(1)+" Illegal attempt to set_creator "+
             "at "+ctime(time())+" from "+file_name(previous_object())+"\n");
  }
  creator = i;
  app_creator = i;
  home_dir = "/w/"+name;
  save_me();
}

int query_prevent_shadow(object ob) {
  if (function_exists("query_prevent_shadow", ob) ||
      function_exists("query_name", ob) ||
      function_exists("query_creator", ob) ||
      function_exists("query_app_creator", ob) ||
      function_exists("query_hidden", ob) ||
      function_exists("dest_me", ob) ||
      function_exists("save_me",ob))
    return 1;
  return 0;
}

int query_max_deaths() { return max_deaths; }
void set_max_deaths(int i) { max_deaths = i; }
int adjust_max_deaths(int i) { return (max_deaths += i); }

/* Includeding new hack for parse_command ;) */
move(object dest, string msgin, string msgout) {
  int i;
  object env;

  if ((env = environment()))
    WEATHER->unnotify_me(environment());
  i = ::move(dest, msgin, msgout);
  if (environment())
    WEATHER->notify_me(environment());
  if (!i)
    me_moveing(env);
  return i;
}

int consider(string str) {
  object *obs;
  int level, i, ok, dif;

  if (!str) {
    notify_fail("Syntax: consider <monster>\n");
    return 0;
  }
  obs = find_match(str, environment());
  if (!sizeof(obs)) {
    notify_fail("You need to consider someone who exists.\n");
    return 0;
  }
  level = query_level();
  for (i=0;i<sizeof(obs);i++) {
    if (!living(obs[i]))
      continue;
    ok = 1;
    dif = (int)obs[i]->query_level() - level;
    if (dif > 100) {
      write("You would have to be utterly insane to attack "+
            obs[i]->short()+".\n");
      continue;
    }
    if (level > 5) {
      dif /= 5;
      if (dif > 2 || dif < -2)
        dif /= 2;
      if (dif > 5)
        dif = 5;
      if (dif < -5)
        dif = -5;
      dif += 5;
      write(obs[i]->short()+({ " is too weak to bother attacking.\n",
                               " is some one you look down on.\n",
                               " is a lot weaker than you are.\n",
                               " is weaker than you are.\n",
                               " is slightly weaker than you are.\n",
                               " is about equal with you.\n",
                               " is about equal with you.\n",
                               " is about equal with you.\n",
                               " is slightly tougher than you are.\n",
                               " is tougher than you are.\n",
                               " is a lot tougher than you are.\n",
                               " looks pretty nasty.\n",
                               " is into don't think about it region.\n"
                              })[dif]);
    } else if (dif < 0)
        write(obs[i]->short()+" would be an easy target.\n");
      else {
        dif /= 2;
        if (dif > 5)
          dif = 5;
        write(obs[i]->short()+({ " is about equal with you.\n",
                                 " is slightly tougher than you are.\n",
                                 " is tougher than you are.\n",
                                 " is a lot tougher than you are.\n",
                                 " looks pretty nasty.\n",
                                 " is into don't think about it region.\n"
                               })[dif]);
      }
  }
  if (!ok) {
    notify_fail("It helps if you consider attacking something that is "+
                "alive.\n");
    return 0;
  }
  return 1;
}

static int do_refresh(string str) {
  if (!str || str != "me") {
    notify_fail("Please read the docs before using this command.\n");
    return 0;
  }
  write("WARNING!  Make sure you have read the docs before doing this!\n\n"+
        "Are you sure you wish to refresh yourself? ");
  input_to("refresh2");
  return 1;
}

static int refresh2(string str) {
  str = lower_case(str);
  if (str[0] != 'n' && str[0] != 'y') {
    write("Pardon?  I do not understand.  Do you want to refresh yourself? ");
    input_to("refresh2");
    return 1;
  }
  if (str[0] == 'n') {
    write("Ok, not refreshing.\n");
    return 1;
  }
  write("Doing refresh.\n");
  Str = 13;
  Con = 13;
  Int = 13;
  Wis = 13;
  Dex = 13;
  inttmp = dextmp = wistmp = strtmp = contmp = 0;
  race_ob = STD_RACE;
  guild_ob = 0;
  skills = ({ });
  spells = ({ });
  guild_ob = 0;
  xp = 0;
  deaths = 0;
  max_deaths = 7;
  level_cache = ([ ]);
  bonus_cache = ([ ]);
  gr_commands = ([ ]);
  race_guild_commands();
  stat_cache = ([ ]);
  remove_property("stats_rearranged");
  write("Done.\n");
  map_prop = ([ ]);
  reset_all();
  say(cap_name+" refreshes "+query_objective()+"self.\n");
  return 1;
}

/* for creators who are playing as players */
int query_creator_playing() { return creator; }

int do_teach(string str) {
  string bing;
  object *obs;

  notify_fail("Syntax: teach <spell/command> to <person>\n");
  if (!str || str == "")
    return 0;
  if (sscanf(str, "%s to %s", str, bing) != 2)
    return 0;
  obs = find_match(bing, environment());
  if (!sizeof(obs)) {
    notify_fail("You must teach someone.\n");
    return 0;
  }
  if (!command_teach(obs, str) && !spell_teach(obs, str) &&
      !teach_skill(obs, str)) {
    notify_fail("You do not know the spell or command "+str+".\n");
    return 0;
  }
  return 1;
}

string query_object_type() {
  if (creator)
    return "A";
  return " ";
} /* query_object_type() */

int do_cap(string str) {
  if (!str) {
    notify_fail("Syntax: "+query_verb()+" <cap_name>\n");
    return 0;
  }
  if (lower_case(str) != name) {
    notify_fail("You must have the same letters in your capitalized name.\n");
    return 0;
  }
  cap_name = str;
  write("Set capitalised name to "+cap_name+".\n");
  return 1;
} /* do_cap() */
