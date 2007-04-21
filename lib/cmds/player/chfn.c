/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: chfn.c,v 1.8 2003/07/10 07:10:12 pinkfish Exp $
 * $Log: chfn.c,v $
 * Revision 1.8  2003/07/10 07:10:12  pinkfish
 * Fix up the numvers.
 *
 * Revision 1.7  2001/03/12 04:23:16  pinkfish
 * Change lord only to creator only in the message.
 *
 * Revision 1.6  2001/03/12 00:19:45  ceres
 * Fixed 'creators' to 'lords'.
 *
 * Revision 1.5  1999/04/30 01:11:04  pinkfish
 * Fix this up to strip colours.
 *
 * Revision 1.4  1999/02/09 00:23:41  ceres
 * Increased the max email address length to 50.
 *
 * Revision 1.2  1999/02/03 00:52:50  pinkfish
 * Fix it up so you can only enter certain sized bits of data into the fields.
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";

#define TP this_player()

#define MAX_EMAIL_LEN 50

int cmd(string str) {
  write("Change finger information.\n");
  write("Pressing return at the prompts will take the default.  The default "
        "is the option in []'s.\n");
  write("What real name do you wish to use ["+TP->query_real_name()+"] ? ");
  input_to("real_name");
  return 1;
} /* chfn() */

int real_name(string str) {
  string real_name;

  str = strip_colours(str);
  
  real_name = TP->query_real_name();
  if (str && str != "") {
    if (str == "none") {
      real_name = 0;
    } else {
      real_name = str;
    }
  }

  if (real_name && strlen(real_name) > MAX_EMAIL_LEN) {
    write("Real name is too long, a maximum of " + MAX_EMAIL_LEN + " characters is allowed.\n");
    write("What real name do you wish to use ["+TP->query_real_name()+"] ? ");
    input_to("real_name");
    return 1;
  }

  if (real_name && real_name != "") {
    write("Ok real name set to "+real_name+".\n");
  } else {
    write("Real name cleared.\n");
  }
  TP->set_real_name(real_name);
  
  write("Enter your location (ie Perth, oz, whatever) ["+
        TP->query_where()+"]\n(none for none) : ");
  input_to("get_where");
  return 1;
} /* real_name() */

int get_where(string str) {
  string where;

  str = strip_colours(str);

  where = TP->query_where();
  
  if (str && str != "") {
    if (str == "none") {
      where = 0;
    } else {
      where = str;
    }
  }
  if (where && where != "") {
    write("Ok location set to "+where+".\n");
  } else {
    write("Location cleared.\n");
  }

  if (where && strlen(where) > MAX_EMAIL_LEN) {
     write("Your location is too long, maximum of " + MAX_EMAIL_LEN + " characters allowed.\n");
     write("Enter your location (ie Perth, oz, whatever) ["+
           TP->query_where()+"]\n(none for none) : ");
     input_to("get_where");
     return 1;
  }
  TP->set_where(where);

  if(TP->query_birthday() == "Unknown") {
    write("Enter your birthday (ddmm) ["+TP->query_birthday()+
          "] (none for none) : ");
    input_to("birthday");
  } else {
    write("What email address do you wish to use.  Set to none to clear.\n");
    write("Putting a : in front of it means that only the creators "
          "can read it.\n");
    write("["+TP->query_email()+"] : ");
    input_to("get_email");
  }   
  return 1;
} /* get_where() */

string convert_birthday(string str) {
#undef MONTHS
#define MONTHS ({ "January", "February", "March", "April", "May", "June", \
      "July", "August", "September", "October", "November", "December" })
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
  return retval + " of " + MONTHS[month-1];
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

void birthday(string str) {
  string birth_day;

  birth_day = TP->query_birthday();
  if (str == "") {
    if (birth_day)
      write("Birthday unchanged from "+birth_day+".\n");
    else
      write("Birthday left as blank.\n");
  } else {
    if(birth_day != "Unknown") {
      write("You can't change when you were born! Please ask a Creator or " 
        "a Lord to change it if you made an error.\n");
    } else if(!valid_birthday(str)) {
        write("Invalid Birthday.  Birthday cleared.\n");
        birth_day = 0;
    } else {
      birth_day = convert_birthday(str);
      write("Birthday set to " + birth_day + ".\n");
      TP->birthday_gifts();
    }
  }
  TP->set_birthday(birth_day);

  write("What email address do you wish to use.  Set to none to clear.\n");
  write("Putting a : in front of it means that only the creators and lords "
        "can read it.\n");
  write("["+TP->query_email()+"] : ");
  input_to("get_email");
} /* birthday() */

void get_email(string str) {
  string email;
  
  email = TP->query_email();
  
  str = strip_colours(str);

  if (str == "") {
    if (!email || email == "") {
      write("Email address left blank.\n");
    } else {
      write("Email address left as "+email+".\n");
    }
  } else if (str == "none") {
    email = 0;
    write("Email address cleared.\n");
  } else {
    email = str;
    write("Email address set to "+email+".\n");
  }


  if (email && strlen(email) > MAX_EMAIL_LEN) {
    write("Your email address is too long, maximum of " + MAX_EMAIL_LEN + " characters allowed.\n");
    write("What email address do you wish to use.  Set to none to clear.\n");
    write("Putting a : in front of it means that only the creators and lords "
          "can read it.\n");
    write("["+TP->query_email()+"] : ");
    input_to("get_email");
    return ;
  }

  TP->set_email(email);

  write("Please enter your home page (World Wide Web page address), "
        "type 'none' to clear it.\n");
  if (!TP->query_homepage()) {
    write("[none] : ");
  } else {
    write("["+TP->query_homepage()+"] : ");
  }
  input_to("get_home_page");
} /* get_email() */

void get_home_page(string str) {

  str = strip_colours(str);

  if (str == "") {
    if (!TP->query_homepage()) {
      write("Home page left as blank.\n");
    } else {
      write("Home page left as "+TP->query_homepage()+"\n");
    }
  } else if (str == "none") {
    TP->set_homepage("");
    write("Home page cleared.\n");
  } else if (strlen(str) <= MAX_EMAIL_LEN) {
    TP->set_homepage(str);
    write("Home page set to "+str+"\n");
  } else {
    write("Your home page address is too long, maximum of " + MAX_EMAIL_LEN + " characters allowed.\n");
    write("Please enter your home page (World Wide Web page address), "
          "type 'none' to clear it.\n");
    if (!TP->query_homepage()) {
      write("[none] : ");
    } else {
      write("["+TP->query_homepage()+"] : ");
    }
    input_to("get_home_page");
    return ;
  }
  TP->save_me();
} /* get_home_page() */
