/*
 * the wonder paranoia player object
 * real soon now ;)
 */

inherit "failsafe/living.new";
inherit "failsafe/handle.new";
inherit "failsafe/alias";
inherit "failsafe/wiz_cmd_normal";
#include "money.h"

#define MAX_IDLE 1800
#define TIME_BETWEEN_SAVES 450
/*
 * Dave, I put this here so the playr ob would load, you might want to change
 * it...
 */
static string cap_name;
string password, *auto_load, email;
string *news_rc;
string title, al_title;
static string new_password;
int intox, headache, max_headache, stuffed, soaked;
int time_last_saved;
string desc;
int invis;
/* 
 * Diskworld stuff stats, the names could be a little shorter...
 */
/* sec_level has been moved to std/object, have a nice day. */
static int rows, cols;
int creator, app_creator, deaths, our_sec_level, level;

string *create_auto_load();
void start_player();
void public_commands();
void load_auto_load(string *auto_load);
int save_me();
void set_desc(string str);
int query_creator();
void set_name(string str);

void create() {
  int *i,j,k;

  ::create(); 
/*
 * this should change...
 *  seteuid("NONAME"); 
 */
  creator = 1;
  app_creator = 1;
  seteuid("PLAYER");
  msgin = "arrives";
  msgout = "leaves";
  title = "the only slightly paraniod";
  desc = "";
/* he is a player. and gues what? he is not allowed to be cleaned up */
  set_property(({"player"}));
  cols = 79;
  rows = 24;
  verbose = 1;
}

void move_player_to_start(string bong, object bing, int gender, int died) {
int tmp;
object money;
  seteuid("Root");
  set_name(bong);
  cap_name = capitalize(name);
  restore_object("/players/"+extract(name,0,0)+"/"+name);
  start_player();
/* ok this moves us to the other player... ie we are already playing. */
  if (died) {
    move("/d/am/am/mendeddrum");
    exec_alias("throw_out","");
  } else {
    write("Pinkfish shouts: "+capitalize(name)+" is a frog!!!!!!!\n");
    exec_alias("login","");
  }
}

void start_player() {
  if (app_creator) {
    all_commands();
    app_commands();
  }
  if (creator) 
    wiz_commands();
  reset_get();
  enable_commands();
  public_commands();
  set_living_name(name);
  current_path = home_dir;
  load_auto_load(auto_load);
  set_heart_beat(1);
}
 
void public_commands() {
  add_action("help_func","help");
  add_action("take","get");
  add_action("take","take");
  add_action("drop_ob","drop");
  add_action("give","give");
  add_action("give", "put");
  add_action("wield","wield");
  add_action("wear_ob","wear");
  add_action("remove_ob","remove");
  add_action("look_me","look");
  add_action("who","who");
  add_action("invent","inventory");
  add_action("invent","i");
  add_action("score","score");
  add_action("brief_verbose","brief");
  add_action("brief_verbose", "verbose");
  add_action("save","save");
  add_action("quit","quit");
  add_action("describe","describe");
  add_action("do_say","say");
  add_action("do_say","'",1);
  add_action("do_emote",":",1);
  add_action("do_emote","emote");
  add_action("do_echo","echo");
  add_action("do_tell","tell");
  add_action("examine","examine");
  add_action("change_password","password");
  add_action("change_password","passwd");
}

int invent() {
  string con;

  con = query_contents("You are currently carrying:\n");
  if (con == "")
    write("You do not appear to carrying anything.\n");
  write(con);
  return 1;
}

int look_me(string arg) {
  object *ob;
  int i;

  if (!arg || !stringp(arg)) {
    if (environment())
      write((string)environment()->long());
    return 1; /* blame evan */
  }

  if (sscanf(arg,"at %s",arg)!=1) {
     notify_fail("You don't have the security clearance to do that.\n");
     return 0;
  }

  ob = (object *)find_match(arg,this_object());

  if (environment() && !sizeof(ob))
    ob = (object *)find_match(arg,environment());

  if (sizeof(ob)) {
    if (pointerp(ob)) {
      for (i=0;i<sizeof(ob);i++)
        write((string)ob[i]->long(arg));
    } else
      write((string)ob->long(arg));
    return 1;
  }

  if (environment())
    if (environment()->id(arg)) {
      write((string)environment()->long(arg));
      return 1;
    }
  notify_fail("You do not think that the "+arg+" is here.\n");
  return 0;
}

int examine(string arg) {
  return look_me("at "+arg);
}

int who() {
  object *arr;
  int i;

  arr = users();
  for (i=0;i<sizeof(arr);i++)
    write(arr[i]->short()+"\n");
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

int do_echo(string arg) {
  if (!arg || arg == "") {
    notify_fail("Useage: echo <string>\n");
    return 0;
  }
  write(arg+".\n");
  return 1;
}

int do_say(string arg) {
  if (!arg) 
    arg = "";
  if (query_verb() != "say") {
    arg = extract(query_verb(),1)+" "+arg;
  }
  if (arg == "" || arg == " ") {
    notify_fail("Usage: say <something>\n");
    return 0;
  }
  say(cap_name+" says: "+arg+"\n");
  write("You say: "+arg+"\n");
  return 1;
}

int do_tell(string arg) {
  object ob;
  string str,rest;

  if (sscanf(arg,"%s %s",str,rest)!=2) {
    notify_fail("Useage: tell <name> something\n");
    return 0;
  }

  ob = find_living(str);
  if (!ob || ob==this_player()) {
    notify_fail("You don't have the security clearance to talk to him.\n");
    return 0;
  }

  tell_object(ob,cap_name+" tells you: "+rest+"\n");
  write("You tell "+ob->query_cap_name()+": "+rest+"\n");
  return 1;
}

int do_emote(string arg) {
  string str;

  if (!arg)
    arg = "";

  str = query_verb();

  if (str != "emote")
    arg = extract(str,1,strlen(str))+" "+arg;

  if (arg == "" || arg == " ") {
    notify_fail("Usage: emote <womble>\n");
    return 0;
  }

  str = cap_name+" "+arg+"\n";

  say(str);
  write(str);
  return 1;
}

int score(string str) {
  int i;

  if ((verbose && (str != "brief")) || (str == "verbose")) {
    write("You are using the new and improved Discworld player object, ");
    write("beware of bugs.\n");
    write("Here are some arbitrary numbers for you to look at\n");
    write("Your current monetary status "+coins+" coins.\n");
    return 1;
  }
  return 1;
}

nomask int save() {
    write("Saving...\n");
    save_me();
    return 1; 
}

void save_me() {
  object ob;

}
 
nomask int set_email(string str) {
    if (!str) {
      write("You current email address is "+email+"\n");
      write("To clear use \"email CLEAR\"\n");
      return 1;
    }
    if (str == "CLEAR")
      email = "";
    else
      email = str;
    if (email != "")
      write("Email address set to "+str+".\n");
    else
      write("Email address is null.\n");
    return 1;
}
 
int quit() {
  object *ob, money;
  int i;

  write("You go into deep freeze. See you next time.\n");
  say(cap_name + " left the game.\n");
  auto_load = create_auto_load();
  save_me();
/* get rid of the money....
 * we dont want them taking it twice now do we?
 */
  if ((money = present("Some Money For Me", this_object())))
    money->dest_me();
  transfer_all_to(environment());
  ob = all_inventory(this_object());
  for (i=0;i<sizeof(ob);i++)
    ob[i]->dest_me();
  dest_me();
  return 1;
}
 
string query_title() { return title; }
void set_title(string str) { title = str; }

string query_atitle() { return al_title; }
void set_atitle(string str) { al_title = str; }


int query_deaths() { return deaths; }
string query_stime() {
  if (time_last_saved>0)
    return ctime(time_last_saved);
  return ctime(-time_last_saved);
}

void set_name(string str) {
  if (name)
    return ;
  name = str;
  cap_name = capitalize(name);
  set_short(cap_name);
  set_living_name(name);
  set_main_plural(name);
}

string long(string str) {
  string s;

  if (str == "soul") {
    return (string)"global/soul"->query_long();
  }
  s = "You see "+cap_name+" "+query_title()+", ";
  if (desc)
    s += desc+"\n";
  s += query_contents("");
  return s;
}

int describe(string arg) {
  if (!arg) {
    notify_fail("Useage: describe <string> (setting description to null)\n");
    desc = 0;
    return 0;
  }
  set_desc(arg);
  write("Ok.\n");
  return 1;
}

void set_desc(string s) { desc = s; }

string query_desc() { return desc; }

/* second life routine... handles the player dieing. */
int second_life() {
  int i;
  object tmp;

  deaths++;
/* oh dear complete death ;( */
  tell_object(this_object(),"You come around... but every things seems hazy\n");
  tell_object(this_object(),"There is someone in a Yellow uniform bringing you\n");
  tell_object(this_object(),"around. Perhaps you have just died?\n");
  say(cap_name+" dies(sigh).\n");
  save_me();
  move("room/alpha/clone_revival");
  say(short()+" next clone is revived.\n");
  exec_alias("death","");
  return 1;
} 

/*
 * the heart beat. bounce what does this do? we arent going to tell you
 */
void heart_beat() {
int i;
  if (!interactive(this_object())) {
    if (name == "guest" || name == "root") {
      say(cap_name + " just vanished.\n");
      quit();
    } else {
      say(cap_name+" has just gone net dead.\n");
      save_me();
    }
  }
  if (query_idle(this_object()) > MAX_IDLE) {
    say(cap_name+" has been idle for too long, he vanishes in a puff of smoke.\n");
    write("You idled out... Sorry.\n");
/* so it does a save as well... ;) */
    quit();
    return ;
  }
  if (time_last_saved > 0) {
    if (time()-time_last_saved > TIME_BETWEEN_SAVES) {
      save();
      time_last_saved = -time();
    }
  } else if (time_last_saved+time() > TIME_BETWEEN_SAVES) {
    time_last_saved = time();
  }
/* bing. hit point receival ;) */
  if (headache)
    if (intox || --headache) {
      tell_object(this_object(), "Your headache disapears ;)\n");
      headache = 0;
    }
  if (intox) {
    intox --;
    if (!intox) {
      headache = 5;
      tell_object(this_object(),"You get a spliting headache, be happy.\n");
    }
  }
}

/* does the invis stuff ;) */
int invisible() {
  invis = 1;
  write("Ok.\n");
  return 1;
}

int visible() {
  invis = 0;
  write("Ok.\n");
  return 1;
}

int query_invis() { return invis; }

void set_password(string pass) {
  int i;
  if (sscanf(file_name(previous_object()), "/secure/login#%d", i))
    password = pass;
}

int change_password2(mixed pass);

static int change_password() {
  if (password) {
    write("Please enter your old password : ");
    input_to("change_password2",1);
    return 1;
  }
  change_password2(0);
  return 1;
}

static int change_password2(mixed pass) {
  string str;
  if (password) {
    str = crypt(pass,password);
    if (str != password) {
      write("\nIncorrect.\n");
      return 1;
    }
  }
  write("\nEnter new Password : ");
  input_to("change_password3",1);
  return 1;
}

static string tmppassword;

static int change_password3(string pass) {
  tmppassword = pass;
  write("\nPlease enter again : ");
  input_to("change_password4",1);
  return 1;
}

static int change_password4(string pass) {
  if (tmppassword != pass) {
    write("\nIncorrect.\n");
    return 1;
  }
  password = crypt(pass,password);
  write("\nOk.\n");
  return 1;
}

string *create_auto_load() {
  object ob;
  string s, *auto_string;

  auto_string = ({ });
  ob = first_inventory(this_object());
  while (ob) {
    s=(string)ob->query_auto_load();
    if (s)
      auto_string += ({ s });
    ob = next_inventory(ob);
  }
  return auto_string;
}

void load_auto_load(string *auto_string) {
  object ob;
  string name, args;
  int i;

  if (stringp(auto_string))
    return ;
  if (!auto_string || !sizeof(auto_string))
    return ;
  for (i=0;i<sizeof(auto_string);i++)
    if (stringp(auto_string[i]))
      if (sscanf(auto_string[i],"%s^([%s",name,args)==2) {
        if (!catch(name->load_up_and_frog())) {
          ob = clone_object(name);
          ob->init_arg(args);
          ob->move(this_object());
        }
      } else {
        if (!catch(name->load_up_and_frog())) {
          name->init_arg("");
          name->move(this_object());
        }
      }
}

int query_rows() { return rows; }
int query_cols() { return cols; }

int help_func(string str) {
  object helper;
  int i;

  helper = clone_object("global/help");
  i = (int)helper->help(str);
  return i;
}

mixed *query_news_rc() { return news_rc; }
void set_news_rc(mixed *bing) { news_rc = bing; }

int query_creator() { return creator; }
int query_wizard() { return creator; } /* need this fo rthe gamed driver */
int query_app_creator() { return app_creator; }

/* ok here is all the money handleing stuff */
/* shoould this be in living.c? */
mixed *query_money_array() {
  object ob;

  if (!(ob=present("Some Money For Me", this_object())))
    return ({ });
  return (mixed *)ob->query_money_array();
}

int pay_money(mixed *m_array) {
  int i, j;
  mixed *m_a;
  object ob;

  ob = present("Some Money For Me", this_object());
  if (!ob) {
    ob = clone_object(MONEY_OBJECT);
    ob->move(this_object());
  }
  for (i=0;i<sizeof(m_array);i+=2) {
    m_a = (mixed *)MONEY_HAND->make_payment(m_array[i], m_array[i+1], this_object());
    for (j=0;j<sizeof(m_a[0]);j+=2)
      ob->adjust_money(-m_a[0][j+1], m_a[0][j]);
    for (j=0;j<sizeof(m_a[1]);j+=2)
      ob->adjust_money(m_a[1][j+1], m_a[1][j]);
  }
}

int adjust_money(int amt, string type) {
  object ob;

  ob = present("Some Money For Me", this_object());
  if (!ob) {
    ob = clone_object(MONEY_OBJECT);
    ob->move(this_object());
  }
  return (int)ob->adjust_money(amt, type);
}
