#define NAME 0
#define C_NAME 1
#define STRING 2
#define GENDER 3
#define IP_NUMBER 4
#define IP_NAME 5
#define TYPE 6
#define AGE 7
#define GUILD 8
#define RACE 9
#define LEVEL 10
#define TERMINAL 11
#define ROWS 12
#define COLS 13
#define ROOM 14
#define EUID 15
#define UID 16
#define CFILE 17
#define CUR_DIR 18
#define CENTER 128
#define RIGHT 64
#define MASK (RIGHT+CENTER)
#define ABBREV ({ \
          "n", \
          "N", \
          "", \
          "G", \
          "#", \
          "I", \
          "T", \
          "A", \
          "g", \
          "R", \
          "L", \
          "t", \
          "r", \
          "c", \
          "O", \
          "E", \
          "U", \
          "F", \
          "/", \
        })
#define DE_NAMES ({ \
          "Name", \
          "Name", \
          "", \
          "Gender", \
          "IP Number", \
          "IP Name", \
          "Type", \
          "Age", \
          "Guild", \
          "Race", \
          "Level", \
          "Terminal type", \
          "Rows", \
          "Cols", \
          "Room", \
          "Euid", \
          "Uid", \
          "Creator file", \
          "Current dir", \
        })

/* This is the default people listing thingy. */
#define P_DEFAULT ({ \
          NAME, 12, \
          TYPE|CENTER, 3, \
          GENDER|CENTER, 6, \
          STRING, " ", \
          GUILD|CENTER, 10, \
          STRING, " ", \
          LEVEL|RIGHT, 3, \
          STRING, " ", \
          AGE, 4, \
          ROOM, 30 \
        })
#define QP_DEFAULT ({ \
          AGE|RIGHT, 4, \
          STRING, " ", \
          NAME, 30, \
        })
#define T_DEFAULT ({ \
          NAME, 12, \
          ROWS|CENTER, 5, \
          STRING, " ", \
          COLS|CENTER, 5, \
          STRING, " ", \
          TERMINAL, 20, \
        })
#define N_DEFAULT ({ \
          NAME, 12, \
          STRING, " ", \
          IP_NUMBER, 16, \
          STRING, " ", \
          IP_NAME, 30, \
        })
#define D_DEFAULT ({ \
          NAME, 12, \
          STRING, " ", \
          CUR_DIR, 25, \
          STRING, " ", \
          ROOM, 40, \
        })

string *de_names,
       *abbrev,
       *var_names;

void create() {
  de_names = DE_NAMES;
  abbrev = ABBREV;
  var_names = ({ 
      "dirs",
      "netstat",
      "people",
      "qpeople",
      "terms",
    });
} /* create() */

object *get_people(string str) {
  object *ob;
  int i, cr, hl;
  string s1;

  ob = users();
  cr = (int)previous_object()->query_creator();
  hl = (int)"/secure/master"->high_programmer(geteuid(previous_object()));
  for (i=0; i < sizeof(ob); i++) {
    switch (ob[i]->query_invis()) {
      case 1 :
        if (!cr) {
          ob = delete(ob, i--, 1);
          continue;
        }
        break;
      case 2 :
        if (!hl) {
          ob = delete(ob, i--, 1);
          continue;
        }
        break;
    }
    if (str)
      if(sscanf(ob[i]->query_name(),str+"%s", s1) != 1)
        ob = delete(ob, i--, 1);
  } 
  return ob;
} /* get_people() */

string create_title(mixed *bits) {
  int i, pos;
  string str;

  str = "";
  for (i=0;i<sizeof(bits);i+=2) {
    if ((pos = (bits[i]&~MASK)) == STRING)
      str += bits[i+1];
    else
      if (strlen(de_names[pos]) > bits[i+1])
        str += sprintf("%"+bits[i+1]+"."+bits[i+1]+"s", "");
      else
        switch (bits[i]&MASK) {
          case CENTER :
            str += sprintf("%"+bits[i+1]+"."+bits[i+1]+"|s", de_names[pos]);
            break;
          case RIGHT :
            str += sprintf("%"+bits[i+1]+"."+bits[i+1]+"s", de_names[pos]);
            break;
          default :
            str += sprintf("%"+bits[i+1]+"."+bits[i+1]+"-s", de_names[pos]);
            break;
        }
  }
  return str;
} /* create_title() */

string review_thingy(mixed *bing) {
  int i;
  string str;

  str = "";
  for (i=0;i<sizeof(bing);i+=2) {
    if (bing[i] == STRING) {
      str += bing[i+1];
    } else {
      switch (bing[i]&MASK) {
        case CENTER :
          str += "%|"+bing[i+1]+abbrev[bing[i]&~MASK];
          break;
        case RIGHT :
          str += "%-"+bing[i+1]+abbrev[bing[i]&~MASK];
          break;
        default :
          str += "%-"+bing[i+1]+abbrev[bing[i]&~MASK];
          break;
      }
    }
  }
  return str;
} /* review_thingy() */

/*
 * Ok, the method for doing the format is...
 *    ({ type, width,  ... })
 * With strings the width is the string...
 */
void print_entrys(object *obs, mixed *format) {
  int i, age, j;
  object g;
  string form, str;

  printf("%s\n", create_title(format));
  for (i=0;i<sizeof(obs);i++) {
    str = "";
    for (j=0;j<sizeof(format);j+=2) {
      switch (format[j]&MASK) {
        case RIGHT :
          form = "%"+format[j+1]+"."+format[j+1];
          break;
        case CENTER :
          form = "%|"+format[j+1]+"."+format[j+1];
          break;
        default :
          form = "%-"+format[j+1]+"."+format[j+1];
          break;
      }
      switch (format[j]&~MASK) {
        case STRING :
/* Ignore width for this one... */
          str += format[j+1];
          break;
        case C_NAME :
          str += sprintf(form+"s",
                         obs[i]->query_cap_name());
          break;
        case GENDER :
          str += sprintf(form+"s",
                         obs[i]->query_gender_string());
          break;
        case NAME :
          str += sprintf(form+"s",
                         (obs[i]->query_in_editor()?"*":"")+
                         obs[i]->query_name());
          break;
        case GUILD :
          str += sprintf(form+"s",
                         ((g=(object)obs[i]->query_guild_ob())?
                          g->query_name():"No guild"));
          break;
        case LEVEL :
          str += sprintf(form+"d",
                         obs[i]->query_level());
          break;
        case ROWS :
          str += sprintf(form+"d",
                         obs[i]->query_rows());
          break;
        case COLS :
          str += sprintf(form+"d",
                         obs[i]->query_cols());
          break;
        case TERMINAL :
          str += sprintf(form+"s",
                         obs[i]->query_term_name());
          break;
        case IP_NUMBER :
          str += sprintf(form+"s",
                         query_ip_number(obs[i]));
          break;
        case IP_NAME :
          str += sprintf(form+"s",
                         query_ip_name(obs[i]));
          break;
        case TYPE :
          str += sprintf(form+"s",
                         obs[i]->query_object_type());
          break;
        case AGE :
          age = (int)obs[i]->query_time_on();
          if (age < -86400)
            str += sprintf(form+"s",
                           (age/-86400)+"D");
          else if (age < -3600)
            str += sprintf(form+"s",
                           (age/-3600)+"h");
          else if (age < -60)
            str += sprintf(form+"s",
                           (age/-60)+"m");
          else
            str += sprintf(form+"s",
                           age+"s");
          break;
        case ROOM :
          str += sprintf(form+"s",
                         (environment(obs[i])?
                                       file_name(environment(obs[i]))
                                       :"No environment"));
          break;
        case EUID :
          str += sprintf(form+"s",
                         (environment(obs[i])?geteuid(environment(obs[i]))
                                       :"No environment"));
          break;
        case UID :
          str += sprintf(form+"s",
                         (environment(obs[i])?getuid(environment(obs[i]))
                                       :"No environment"));
          break;
        case CFILE :
          str += sprintf(form+"s",
                         (environment(obs[i])?"/secure/master"->creator_file(
                                            environment(obs[i]))
                                       :"No environment"));
          break;
        case CUR_DIR :
          str += sprintf(form+"s", (obs[i]->query_current_path()?
                          obs[i]->query_current_path():"No dir"));
          break;
      }
    } /* for j... */
    printf("%s\n", str);
  } /* for i... */
} /* print_entrys() */

int do_people(string str) {
  mixed *bing;
  object *obs;

  bing = (mixed *)this_player()->query_property("people list");
  if (!bing)
    bing = P_DEFAULT;
  obs = get_people(str);
  if (!sizeof(obs)) {
    notify_fail("Nobody seems to start with "+str+", sorry.\n");
    return 0;
  }
  print_entrys(obs, bing);
  return 1;
} /* do_people() */

int do_terms(string str) {
  mixed *bing;
  object *obs;

  bing = (mixed *)this_player()->query_property("term list");
  if (!bing)
    bing = T_DEFAULT;
  obs = get_people(str);
  if (!sizeof(obs)) {
    notify_fail("Nobody seems to start with "+str+", sorry.\n");
    return 0;
  }
  print_entrys(obs, bing);
  return 1;
} /* do_terms() */

int do_dirs(string str) {
  mixed *bing;
  object *obs;

  bing = (mixed *)this_player()->query_property("dir list");
  if (!bing)
    bing = D_DEFAULT;
  obs = get_people(str);
  if (!sizeof(obs)) {
    notify_fail("Nobody seems to start with "+str+", sorry.\n");
    return 0;
  }
  print_entrys(obs, bing);
  return 1;
} /* do_dirs() */

int do_netstat(string str) {
  mixed *bing;
  object *obs;

  bing = (mixed *)this_player()->query_property("netstat list");
  if (!bing)
    bing = N_DEFAULT;
  obs = get_people(str);
  if (!sizeof(obs)) {
    notify_fail("Nobody seems to start with "+str+", sorry.\n");
    return 0;
  }
  print_entrys(obs, bing);
  return 1;
} /* do_dirs() */

int do_qpeople(string str) {
  mixed *bing;
  object *obs;

  bing = (mixed *)this_player()->query_property("qpeople list");
  if (!bing)
    bing = QP_DEFAULT;
  obs = get_people(str);
  if (!sizeof(obs)) {
    notify_fail("Nobody seems to start with "+str+", sorry.\n");
    return 0;
  }
  print_entrys(obs, bing);
  return 1;
} /* do_qpeople() */

int review() {
  mixed *bing;

  bing = (mixed *)this_player()->query_property("people list");
  if (!bing)
    bing = P_DEFAULT;
  write("People : "+review_thingy(bing)+"\n");
  bing = (mixed *)this_player()->query_property("qpeople list");
  if (!bing)
    bing = QP_DEFAULT;
  write("Qpeople: "+review_thingy(bing)+"\n");
  bing = (mixed *)this_player()->query_property("netstat list");
  if (!bing)
    bing = N_DEFAULT;
  write("Netstat: "+review_thingy(bing)+"\n");
  bing = (mixed *)this_player()->query_property("term list");
  if (!bing)
    bing = T_DEFAULT;
  write("Terms  : "+review_thingy(bing)+"\n");
  bing = (mixed *)this_player()->query_property("dir list");
  if (!bing)
    bing = D_DEFAULT;
  write("Dirs   : "+review_thingy(bing)+"\n");
} /* review() */

mixed *create_review(string str) {
  string *bits, rest;
  int i, bing, width, tmp;
  mixed *ret;

  bits = explode("$"+str, "%");
  bits[0] = bits[0][1..1000];
  if (!strlen(bits[0]))
    ret = ({ });
  else
    ret = ({ STRING, bits[0] });
  for (i=1;i<sizeof(bits);i++) {
    bing = RIGHT;
    rest = bits[i];
    if (rest[0] == '|') {
      bing = CENTER;
      rest = rest[1..1000];
    } else if (rest[0] == '-') {
      bing = 0;
      rest = rest[1..1000];
    }
    if (sscanf(rest, "%d%s", width, rest) == 2) {
      tmp = member_array(rest[0..0], abbrev);
      if (tmp == -1) {
        write("Unknown thingy '"+rest[0..0]+"'\n");
        return 0;
      }
      ret += ({ tmp+bing, width });
      rest = rest[1..1000];
      if (strlen(rest))
        ret += ({ STRING, rest });
    } else {
      ret += ({ STRING, bits[i] });
    }
  }
  return ret;
} /* create_review() */

void list_matches() {
  int i;

  for (i=0;i<sizeof(de_names);i++) {
    if (i == STRING) continue;
    write(abbrev[i]+": "+de_names[i]+"\n");
  }
} /* list_matches() */

int set_var(string str) {
  string name, type;
  mixed *bing;

  if (str == "help") {
    write("The non helpful help.\nThis is the list of things that go after "+
          "the %'s in the value.\n");
    list_matches();
    return 1;
  }
  if (!str || sscanf(str, "%s %s", name, type) != 2) {
    notify_fail("Syntax: "+query_verb()+" <var_name> <value>\n"+
                "        "+query_verb()+" help\n");
    return 0;
  }
  if (member_array(name, var_names) == -1) {
    notify_fail("You cannot set the var "+name+" has to be one of "+
                implode(var_names, ", ")+".\n");
    return 0;
  }
  bing = create_review(type);
  if (!bing)
    return 1;
  previous_object()->add_property(name+" list", bing);
  write("Ok, set var "+name+" to "+type+".\n");
  return 1;
} /* set_var() */
