inherit "std/object";

#define MAX_RETRYS 3
#define MIN_LEN 3

int test_add(object ob, int flag) { return 1; }
int test_remove(object ob, int flag) { return 1; }

static object other_copy, new_copy;
string password, title, al_title, cap_name;
int cols;
int Str, Dex, gender, creator, app_creator;
static int no_times;
static string player_ob;

void begin2(int new_pl);
void begin(int new_pl);
void logon3();
void logon4(int bing);
void logon5(int new_pl);
void guest_login();
 
void create() {
  cols = 79;
  ::create();
  set_name("logon");
  gender = -1;
  Str = -1;
  Dex = -1;
  seteuid("Root");
  player_ob = "/global/player";
} /* create() */
 
int query_cols() { return cols; }
string query_cap_name() { return "Logon"; }
string query_title() { return "the title-less"; }
int query_creator() { return creator; }
 
int check_valid_name(string str) {
int i;
  for (i=0;i<strlen(str);i++)
    if (str[i] < 'a' || str[i] > 'z') return i;
  return -1;
} /* check_valid_name() */
 
void logon() {
  int tim;

  call_out("time_out",300);
  if (this_player() == this_object())
    cat("doc/WELCOME");
  if (find_object("/obj/shut")) {
    tim = (int)"/obj/shut"->query_time_to_crash();
    if (tim < 60) {
      write("Mud shutting down in "+tim+" seconds.\n");
    } else { 
      if ((tim/60) > 60) {
        write("Mud shutting down in "+(tim/60/60)+" hours and "+
              ((tim/60)%60)+" minutes.\n");
      } else
        write("Mud shutting down in "+(tim/60)+" minutes.\n");
    }
  }
  write("Enter your name: ");
  input_to("logon1");
} /* logon() */
 
void logon1(string str);

int do_su(string str) {
  object ob, tp;

  ob = this_player();
  tp = this_object();
  exec(tp, ob);
  ob->quit();
  logon1(str);
  return 1;
} /* do_su() */

void time_out() {
  write("Time out.\n\n");
  destruct(this_object());
} /* time_out() */
 
void logon1(string str) {
int tmp, frog;
object ob, me;
  creator = 0;
  app_creator = 0;
  if (str=="") {
    write("Well, come back some other time then!\n");
    destruct(this_object());
    return ;
  }
  name = lower_case(str);
  frog = (name[0] == ':');
  if (name[0] == ':')
    name = name[1..strlen(name)];
  player_ob = (string)"/secure/bastards"->query_player_ob(name);
  if (!player_ob) {
    write("Sorry, that player name has been banished.\n");
    dest_me();
    return ;
  }
  if (strlen(name) > 11) {
    write("Name too long (max 11 chars). Please retry.\n");
    write("Enter your name: ");
    return input_to("logon1");
  }
  if ((tmp = check_valid_name(name)) != -1) {
    write("Invalid character '"+str[tmp]+"'  ("+str+").\n\n");
    write("Enter your name: ");
    input_to("logon1");
    return;
  }
  if (name == "guest")
    return guest_login();
  if (!restore_object("/players/"+name[0..0]+"/"+name)) {
    write("Your name is not in our annals.\n");
    write("Did you enter your name ["+name+"] correctly (y/n) ? ");
    input_to("check_name");
    return ;
  } else if (!password)
    return logon3();
  creator = frog && (creator || app_creator);
  write("Enter password: ");
  input_to("logon2",1);
} /* logon1() */

int check_name(string str) {
  str = implode(explode(str," "),"");
  if (str[0] == 'y') {
    write("New character.\n");
    write("Enter password : ");
    input_to("logon2", 1);
    return 1;
  }
  if (str[0] == 'n') {
    write("Ok, try again... Please enter your name : ");
    input_to("logon1");
    return 1;
  }
  write("No... (y/n)  ? ");
  input_to("check_name");
  return 1;
} /* check_name() */

static void logon2(string str) {
  write("\n");
  if (str == "") {
    write("Come back some other time then!\n");
    destruct(this_object());
    return ;
  }
  str = crypt(str,password);
  if (password && str != password) {
    if (++no_times > MAX_RETRYS) {
      write("Too many retries.\n");
      destruct(this_object());
      return ;
    }
    if (Dex == -1) {
      write("Didn't match up with first password.\n");
      write("Enter password again : ");
      Str = -1;
      input_to("logon2",1);
      return ;
    }
    write("Wrong...\nTry again :");
    input_to("logon2",1);
    return ;
  }
  password = str;
  if (Str == -1) {
    Str = 13;
    write("\nPassword (again): ");
    input_to("logon2", 1);
    return;
  }
  if (Dex == -1)
    Dex = 13;
  logon3();
} /* logon2() */
 
static void logon3() {
  if (gender == -1) {
    write("Are you male or female ? ");
    return input_to("get_sex");
  }
  logon4(0);
} /* logon3() */
 
static void get_sex(string str) {
int i;
  str = lower_case(str);
  if (str != "")
    i = str[0];
  if (i == 'm')
    gender = 1;
  else if (i== 'f')
    gender = 2;
  else {
    write("Thats too wierd for even this game!\n");
    write("Try male or female :");
    return input_to("get_sex");
  }
  if (!query_property("guest"))
    save_object("/players/"+name[0..0]+"/"+name);
  logon4(!query_property("guest"));
} /* get_sex() */

static void logon4(int bing) {
  if (!cap_name || lower_case(cap_name) != name) {
    cap_name = capitalize(name);
    write("How do you want your name capitalized ["+cap_name+"] ? ");
    input_to("get_capitalize", bing);
    return ;
  }
  logon5(bing);
} /* logon5() */

static void get_capitalize(string str, int bing) {
  if (str && str != "") {
    if (lower_case(str) != name) {
      write("I am sorry, it has to be the same as your name.\n");
      write("How would you like your name capitalised ["+cap_name+"] ? ");
      input_to("get_capitalize", bing);
      return ;
    }
    cap_name = str;
  }
  logon5(bing);
} /* get_sex() */
 
static void logon5(int new_pl) {
  if (name != "root" && name != "guest")
    other_copy = find_player(name);
  if (other_copy == this_player()) 
    return begin(new_pl);
  if (other_copy) {
    write("You are already playing. Throw the other copy out (y/n/restart) ? ");
    return input_to("try_throw_out");
  }
  begin(new_pl);
} /* logon4() */

void guest_login() {
  write("Use what name for your visit ? ");
  input_to("guest_login2");
  return ;
}  /* guest_login() */

void guest_login2(string str) {
  int tmp;

  if (!str || str == "") {
    write("Ok see you some other time then.\n");
    write("Have fun.\n");
    destruct(this_object());
    return ;
  }
  if (strlen(str) < MIN_LEN) {
    write("That name is too short.\n");
    write("Try again with a longer name : ");
    input_to("guest_login2");
    return ;
  }
  str = lower_case(str);
  str = str[0..11];
  if(str == "guest") {
    write("Yes, we know you are a guest.\n" +
      "Please enter a name for this visit : ");
    input_to("guest_login2");
    return;
  }
  if ((tmp = check_valid_name(str)) != -1) {
    printf("Invalid character '%d'-%c\n", str[tmp], str[tmp]);
    printf("Please enter your name again : ");
    input_to("guest_login2");
    return ;
  }
  if (file_size("/players/"+str[0..0]+"/"+str+".o") > 0 ||
      file_size("/banish/"+str+".o") > 0) {
    write("Sorry that name is already used, or banished.\n");
    write("Please try another name : ");
    input_to("guest_login2");
    return ;
  }
  title = "guest of discworld";
  name = str;
  add_property("guest", 1);
  printf("What sex do you want to be (M/F) ? ");
  input_to("get_sex");
} /* guest_login2() */

static void try_throw_out(string str) {
object tmp, ob, ob1;
  if (str == "restart") {
    if (catch(other_copy->quit()))
      if (catch(other_copy->dest_me()))
        destruct(other_copy);
    return begin(0);
  }
  if (str == "" || (str[0] != 'y' && str[0] != 'Y')) {
    write("Ok, come back later on then!\n");
    destruct(this_object());
    return;
  }
  if (interactive(other_copy)) {
    ob = clone_object("/std/object");
    tmp = other_copy;
    exec(ob, other_copy);
    ob->dest_me();
  }
  tmp = other_copy;
  ob = this_object();
  exec(other_copy, ob);
/* force a look ;) */
  other_copy->look_me();
  tell_room(environment(other_copy), other_copy->query_cap_name()+" has reconnected.\n", ({ other_copy }) );
/* Getting error on dested object here...  Which could not be considered
 * good.  Hopefully the - ({ 0 }) will fix this.   Though I think I will patch
 * event to avoid it altogether...
 */
  event(users() - ({ 0 }), "inform",
        other_copy->query_cap_name() + " reconnected", "link-death");
  destruct(this_object());
} /* try_throw_out() */

void begin(int new_pl) {
  player_ob = (string)"/secure/bastards"->query_player_ob(name);
  if (!find_object(player_ob)) {
    write("Please wait... Loading player object.\n");
    call_out("begin2", 0, new_pl);
  } else
    begin2(new_pl);
} /* begin() */

void begin2(int new_player) {
object ob, ob1, tp;
/* clone the player object */
  new_copy = clone_object(player_ob);
  if (!new_copy) {
    write("Arrggghhhh something bad happened.\n");
    destruct(this_object());
    return 0;
  }
  new_copy -> set_name(name);
  new_copy -> set_password(password);
  tp = this_object();
  exec(new_copy, tp);
  if (tp != this_object())
    tp -> quit();
  if (query_property("guest")) {
    new_copy->add_property("guest",1);
    new_copy->set_title(title);
  }
  new_copy -> move_player_to_start(name, new_player, cap_name);
  new_copy -> set_gender(gender);
  destruct(this_object());
} /* begin2() */
 
int query_level() { return 1; }
int query_gender() { return gender; }

/*
 * used by the mailer to determine if bing is a real man
 */
int test_user(string str) {
  string tmp;

  if (!stringp(str) || !str || sscanf(str, "%s ",tmp))
    return 0;
  return restore_object("/players/"+str[0..0]+"/"+str);
} /* test_user() */

int test_creator(string str) {
  if (!stringp(str) || strlen(str) < 2)
    return 0;
  if (!restore_object("/players/"+str[0..0]+"/"+str))
    return 0;
  return creator;
} /* test_creator() */

int new_player(object old) {
  object new, tp;
  string name;

  name = (string)old->query_name();
  cap_name = (string)old->query_cap_name();
  tp = this_object();
  exec(tp, old);
  creator = (int)old->query_creator();
  old->quit();
  creator = (int)this_player()->query_creator();
  new = clone_object((string)"/secure/bastards"->query_player_ob(name));
  exec(new, tp);
  new->move_player_to_start(name, 0, cap_name);
  destruct(this_object());
} /* new_player() */

int query_prevent_shadow() { return 1; }

/*
 * Needed so the object actually writes things to the screen.  Mildly
 * embarasing otherwise.
 */
void do_efun_write(string str) {
  efun::tell_object(this_object(), str);
} /* do_efun_write() */

string query_object_type() { return "X"; }

string query_gender_string() { return "blue"; }
