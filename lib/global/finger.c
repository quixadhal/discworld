string password, email;
static string tmp_password;
string real_name, birth_day, desc, where;

void finger_commands() {
  add_action("chfn", "chfn");
  add_action("set_email","email");
  add_action("finger","finger");
  add_action("describe","describe");
  add_action("change_password","password");
  add_action("change_password","passwd");
  add_action("do_who", "who");
  add_action("do_whoami", "whoami");
} /* finger_commands() */

string who_line(object ob, int cre, int width, int force) {
  string s, tmp, nam;

  if (!(tmp = (string)ob->short()))
    if (!force)
      return 0;
    else
      tmp = (string)ob->query_cap_name();
  s = "";
  nam = tmp;
  if (cre)
    if ((tmp = (string)ob->query_in_editor()))
      s += " (Editing: "+tmp+")";
  if (ob->query_property("guest"))
    s += " guest of Discworld";
  else if (tmp = (string)ob->query_gtitle())
    s += " " + tmp;
  else
    s += " the adventurer";
  if (tmp = (string)ob->query_title())
    s += ", " + tmp;
  if (tmp = (string)ob->query_atitle())
    s += " (" + tmp + ")";
  if (query_idle(ob) > 120)
    s += " (Idle: " + (query_idle(ob)/60) + ")";
  return sprintf(nam+"%*-=s", width - strlen(nam), s)+"%^RESET%^\n";
} /* who_line() */

string who_string(int width, int cre) {
  object *arr;
  int i;
  int number;
  string s, tmp, nam, prt;

  arr = users();
  s = "";

  prt = sprintf("%|*'-'s\n", width, "==========]  Discworld  [===========");
  prt += sprintf("%|*s\n%*'-'s\n", width, ctime(time()), width, "");

  for (i=0;i<sizeof(arr);i++) {
    tmp = who_line(arr[i], cre, width, 0);
    if (!tmp) continue;
    prt += tmp;
    if (!arr[i]->query_hidden())
      number++;
  }
  if(number < 2)
     tmp = "> You are all alone on the Discworld. <";
  else
     tmp = "> There are " + query_num(number, 100) +
        " mudders on the Discworld. <";
  prt += sprintf("%*'-'|s\n", width, tmp);
  return prt;
} /* who_string() */

int do_who() {
  efun::tell_object(this_player(),
 (string)this_object()->fix_string(who_string((int)this_object()->query_cols(),
                                       (int)this_object()->query_creator())));
  return 1;
} /* do_who() */


int do_whoami() {
  efun::tell_object(this_player(),
  (string)this_object()->fix_string(who_line(this_object(),
      (int)this_object()->query_cols(), (int)this_object()->query_creator(),
       1)));
} /* do_whoami() */

int finger(string str) {
  string ret;

  if (!str) {
    object *obs;
    string type;
    int i;

    obs = users();
/*
    obs = (object *)this_object()->get_people(str);
    if (!sizeof(obs)) {
      notify_fail("Sorry no one logged on matches "+str+".\n");
      return 0;
    }
*/
    write(sprintf("%-12.12s    %-20.20s %-20.20s %-20.20s\n",
                  "Name", "Real name", "Where", "Birthday"));
    for (i=0;i<sizeof(obs);i++) {
      string euid;

      if (obs[i]->query_invis() && !this_object()->query_creator())
        continue;
      if ((int)obs[i]->query_invis() == 2 && !this_object()->query_lord())
        continue;
      type = (obs[i]->query_earmuffs() ? "e" : " ");
      euid = geteuid(obs[i]);
      type += obs[i]->query_object_type();
/* above changed to use query_object_type by ember 16-feb-93 ... does
   query_object_type get the 'X' as well? */
/*
      else if ("/secure/master"->god(euid)) type += "G";
      else if ("/secure/master"->high_programmer(euid)) type += "H";
      else if ("/secure/master"->query_lord(euid)) type += "L";
      else if ("/d/independent/master"->query_member(euid)) type += "I";
      else if (obs[i]->query_creator()) type += "C";
      else if (obs[i]->query_app_creator()) type += "A";
      else type += " ";
 */
      printf("%-12.12s %2.2s %-20.20s %-20.20s %-20.20s\n",
      (obs[i]->query_invis()?"("+obs[i]->query_name()+")":(string)obs[i]->query_name()),
                    type,
                    ((ret = (string)obs[i]->query_real_name())?ret:"-"),
                    ((ret = (string)obs[i]->query_where())?ret:"-"),
                    ((ret = (string)obs[i]->query_birthday())?ret:"-"));
    }
    return 1;
  } else if(ret = (string)"/secure/finger"->finger_info(str)) {
    write(ret);
    return 1;
  } else {
    notify_fail("No one going by the name of " + str + " has ever visited " +
      "Discworld.\n");
    return 0;
  }
} /* finger() */
 
nomask int set_email(string str) {
    if (!str) {
      write("You current email address is "+email+"\n");
      write("To clear use \"email CLEAR\"\n");
      write("For the email address to be only visible by lords prepend a :\n");
      write("eg email :frog@frogcentral.froguni.swamp\n");
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
} /* set_email() */

void set_desc(string str) { desc = str; }
string query_desc() { return desc; }

int describe(string arg) {
  if (!arg) {
    if(desc)
      notify_fail("Usage: describe <string>\n"+
                  "       describe clear\n"+
                  "description reads : "+this_object()->query_cap_name()+" "+desc+"\n");
    else
      notify_fail("Usage: describe <string>\n"+
                  "       describe clear\n"+
                  "no description set.\n");
    return 0;
  }
  if(arg == "clear") {
    desc = 0;
    write("Description cleared.\n");
    return 1;
  }
  set_desc(arg);
  write("Ok.\n");
  return 1;
} /* describe() */

void set_password(string pass) {
  int i;
  if (sscanf(file_name(previous_object()), "/secure/login#%d", i))
    password = pass;
} /* set_password() */

int change_password2(mixed pass);

static int change_password() {
  if (password) {
    write("Please enter your old password : ");
    input_to("change_password2",1);
    return 1;
  }
  change_password2(0);
  return 1;
} /* change_password() */

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
} /* change_password2() */

static string tmppassword;

static int change_password3(string pass) {
  tmppassword = pass;
  write("\nPlease enter again : ");
  input_to("change_password4",1);
  return 1;
} /* change_password3() */

static int change_password4(string pass) {
  if (tmppassword != pass) {
    write("\nIncorrect.\n");
    return 1;
  }
  password = crypt(pass,password);
  write("\nOk.\n");
  return 1;
} /* change_password4() */

string query_birthday();

int chfn() {
  write("Change finger information.\n");
  write("Pressing return at the prompts will take the default.  The default "+
        "is the option in []'s.\n");
  write("What real name do you wish to use ["+real_name+"] ? ");
  input_to("real_name");
  return 1;
} /* chfn() */

int real_name(string str) {
  if (str && str != "")
    if (str == "none")
      real_name = 0;
    else
      real_name = str;
  if (real_name && real_name != "")
    write("Ok real name set to "+real_name+".\n");
  else
    write("Real name cleared.\n");
  write(
"Enter your location (ie Perth, oz, whatever) ["+where+"]\n"+
"(none for none) : ");
  input_to("get_where");
  return 1;
} /* real_name() */

int get_where(string str) {
  if (str && str != "")
    if (str == "none")
      where = 0;
    else
      where = str;
  if (where && where != "")
    write("Ok location set to "+where+".\n");
  else
    write("Real name cleared.\n");
  write("Enter your birthday (ddmm) ["+query_birthday()+
        "] (none for none) : ");
  input_to("birthday");
  return 1;
} /* get_where() */

string convert_birthday(string str) {
  /* we assume it is 4 characters long */

  int day, month, tot;
  string retval;

  sscanf(str, "%d", tot);
  day = tot / 100;
  month = tot % 100;
  switch(day) {
    case 11:
      retval = "11th"; break;
    case 12:
      retval = "12th"; break;
    case 13:
      retval = "13th"; break;
    default:
      switch(day%10) {
        case 1:
          retval = day+"st"; break;
        case 2:
          retval = day+"nd"; break;
        case 3:
          retval = day+"rd"; break;
        default:
          retval = day+"th";
      }
  }
  retval += " of ";
  switch(month) {
    case 1:
      return retval + "January";
    case 2:
      return retval + "February";
    case 3:
      return retval + "March";
    case 4:
      return retval + "April";
    case 5:
      return retval + "May";
    case 6:
      return retval + "June";
    case 7:
      return retval + "July";
    case 8:
      return retval + "August";
    case 9:
      return retval + "September";
    case 10:
      return retval + "October";
    case 11:
      return retval + "November";
    case 12:
      return retval + "December";
  }
  return retval;
} /* convert_birthday() */

int valid_birthday(string str) {
#define LENGTHS ({ 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 })

  int tot, month, day;

  if(strlen(str) != 4) {
    return 0;
  }
  if(!sscanf(str, "%d", tot)) {
    return 0;
  }
  month = tot % 100;
  day = tot / 100;
  if(month > 12 || month < 1) {
    return 0;
  }
  if(day < 1) {
    return 0;
  }
  return day <= LENGTHS[month];
} /* valid_birthday() */

string query_birthday() {
  if(birth_day) {
    return birth_day;
  }
  return "Unknown";
} /* query_birthday() */

int set_birthday(string str)
{ /* added so wizzes can set player's birthdays if they foul up */
  if(valid_birthday(str)) {
     birth_day = convert_birthday(str);
     this_object()->save_me();
     return 1;
  }
  return 0;
} /* set_birthday() */

void birthday(string str) {
  if (str == "") {
    if (birth_day)
      write("Birthday unchanged from "+query_birthday()+".\n");
    else
      write("Birthday left as blank.\n");
  } else {
    if(query_birthday() != "Unknown") {
      write("You can't change when you were born! Please ask a Creator or " +
        "a Lord to change it if you made an error.\n");
    } else if(!valid_birthday(str)) {
        write("Invalid Birthday.  Birthday cleared.\n");
        birth_day = 0;
    } else {
      birth_day = convert_birthday(str);
      write("Birthday set to " + query_birthday() + ".\n");
      this_object()->birthday_gifts();
    }
  }
  this_object()->save_me();
  write("What email address do you wish to use.  Set to none to clear.\n");
  write("Putting a : in front of it means that only the creators and lords "+
        "can read it.\n");
  write("["+email+"] : ");
  input_to("get_email");
} /* birthday() */

void get_email(string str) {
  if (str == "")
    if (!email || email == "")
      write("Email address left blank.\n");
    else
      write("Email address left as "+email+".\n");
  else if (str == "none") {
    email = 0;
    write("Email address cleared.\n");
  } else {
    email = str;
    write("Email address set to "+email+".\n");
  }
  this_object()->save_me();
} /* get_email() */

int query_is_birthday_today() {
    string cmonth, dummy, bmonth;
    int cdate, bdate;

    if(sscanf(ctime(time()), "%s %s %d %s", dummy, cmonth, cdate, dummy)!=4)
        return 0; /* error in date */
    if(sscanf(query_birthday(), "%d%s of %s", bdate, dummy, bmonth) !=3)
        return 0; /* no bday set */
    if(cmonth == bmonth[0..2] && cdate == bdate)
        return 1;
} /* query_is_birthday_today() */

string query_real_name() { return real_name; }
string query_where() { return where; }
