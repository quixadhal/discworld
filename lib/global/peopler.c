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
#define CENTER 128
#define RIGHT 64
#define MASK (RIGHT+CENTER)
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
          "", \
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

string *de_names;

void create() {
  de_names = DE_NAMES;
} /* create() */

object *get_people(string str) {
  object *ob;
  int i, cr;
  string s1;

  ob = users();
  cr = (int)previous_object()->query_creator();
  for (i=0; i < sizeof(ob); i++) {
    if (ob[i]->query_hidden() || (ob[i]->query_invis() && !cr)) {
      ob = delete(ob, i--, 1);
      continue;
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
                                       virtual_file_name(environment(obs[i]))
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

  bing = (mixed *)this_player()->query_property("people list");
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

int do_qpeople(string str) {
  mixed *bing;
  object *obs;

  bing = (mixed *)this_player()->query_property("people list");
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
