/*  -*- LPC -*-  */
/*
 * $Id: peopler.c,v 1.5 2000/09/07 01:04:11 ceres Exp $
 */

/**
 * This prints out nicely formated lists of information.  It ius used
 * by the people, finger etc commands.
 * @author Pinkfish
 */
#include <peopler.h>

#define MULTIPLAYER "/obj/handlers/multiplayer"

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
      "netdups",
    });
} /* create() */

/**
 * This method returns the list of people using the 'str' as a 
 * constraint.  So it only shows the people who match with the
 * str.
 * @param str the constraint string
 * @return the array of people
 */
object *get_people(string str) {
  object *ob;
  int i;
  string s1;

  ob = users();
  
  for(i = 0; i<sizeof(ob); i++) {
    if (str && (sscanf(ob[i]->query_name(),str+"%s", s1) != 1)) {
      ob = ob[0..i - 1] + ob[i+1..];
      i--;
    }
  } 
  return ob;
} /* get_people() */

private string create_title(mixed *bits) {
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

private string review_thingy(mixed *bing) {
  int i;
  string str;

  str = "";
  for (i = 0; i < sizeof(bing); i += 2) {
    if (bing[i] == STRING) {
      str += bing[i+1];
    } else {
      switch (bing[i]&MASK) {
        case CENTER :
          str += "%|"+bing[i+1]+abbrev[bing[i]&~MASK];
          break;
        case RIGHT :
          str += "%"+bing[i+1]+abbrev[bing[i]&~MASK];
          break;
        default :
          str += "%-"+bing[i+1]+abbrev[bing[i]&~MASK];
          break;
      }
    }
  }
  return str;
} /* review_thingy() */

/**
 * This method prints out the entries given the input pattern.
 * Ok, the method for doing the format is...
 *    ({ type, width,  ... })
 * With strings the width is the string...
 */
private void print_entrys(object *obs, mixed *format, int with_dups,
                          string constraint) {
  int age, j;
  mixed g;
  string form, str, mess, *not_allowed;
  object ob, env, *dups;
  mapping per_ip;

  reset_eval_cost();

  if (with_dups) {
    per_ip = unique_mapping(obs, (: query_ip_number($1) :));
  }
  mess = sprintf("%s\n", create_title(format));
  foreach(ob in obs) {
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
          str += sprintf(form+"s", capitalize( (string)ob->query_name() ) );
          break;
        case GENDER :
          str += sprintf(form+"s", ob->query_gender_string());
          break;
        case NAME :
          str += sprintf(form+"s", (ob->query_in_editor()?"*":"")+
              ( ob->query_invis() ? "("+ (string)ob->query_name() +")" :
              (string)ob->query_name() ) );
          break;
        case GUILD :
          if(ob)
            str += sprintf(form+"s",
                           ((g=(object)ob->query_guild_ob())?
                            g->query_name():"No guild"));
          else
            str += sprintf(form+"s", "Broken guild");
          break;
        case LEVEL :
          str += sprintf(form+"s", ""+ ob->query_level());
          break;
        case ROWS :
          str += sprintf(form+"s", ""+ ob->query_rows());
          break;
        case COLS :
          str += sprintf(form+"s", ""+ ob->query_cols());
          break;
        case TERMINAL :
          str += sprintf(form+"s",
                         ((g = ob->query_term_name()) ?
                          g + (g == "network" ? " (" +
                               ob->query_cur_term() + ")" : "") : ""));
          break;
        case IP_NUMBER :
          str += sprintf(form+"s", query_ip_number(ob));
          break;
        case IP_NAME :
          str += sprintf(form+"s",
                           (query_ident(ob)?query_ident(ob)+"@":"")+
                           query_ip_name(ob));
          break;
        case TYPE :
          str += sprintf(form+"s", ob->query_object_type()+"");
          break;
        case AGE :
          age = (int)ob->query_time_on();
          if (age < -86400)
            str += sprintf(form+"s", (age/-86400)+"D");
          else if (age < -3600)
            str += sprintf(form+"s", (age/-3600)+"h");
          else if (age < -60)
            str += sprintf(form+"s", (age/-60)+"m");
          else
            str += sprintf(form+"s", (age/-1)+"s");
          break;
        case ROOM :
          env = environment(ob);
          str += sprintf(form+"s",env?file_name(env):"No environment");
          break;
        case EUID :
          env = environment(ob);
          str += sprintf(form+"s",env?geteuid(env):"No environment");
          break;
        case UID :
          env = environment(ob);
          str += sprintf(form+"s",env?getuid(env):"No environment");
          break;
        case CFILE :
          env = environment(ob);
          str += sprintf(form+"s",env?"/secure/master"->creator_file(env)
                         :"No environment");
          break;
        case CUR_DIR :
          str += sprintf(form+"s", (ob->query_current_path()?
                                    ob->query_current_path():"No dir"));
          break;
        case ND_ALLOWED :
          dups = per_ip[query_ip_number(ob)] - ({ ob });
          not_allowed = MULTIPLAYER->check_allowed(ob, dups);
          str += sprintf(form+"s", (sizeof(not_allowed) ?
                                    query_multiple_short(not_allowed) :
                                    ""));
          break;
      }
    } /* for j... */
    if(!constraint || strsrch(str, constraint) != -1)
      mess += sprintf("%s\n", str);
  } /* for i... */
  this_player()->more_string( mess );
} /* print_entrys() */

/**
 * This method is the main access point to the peopler.  It prints out
 * the commands.  The optional sort function allows you to sort on 
 * somethign other than the name of the player.
 * <p>
 * The format of the pattern string is an array with every second element
 * being the type and the other element being the width of the printable
 * string.
 * <pre>
 *    ({ type, width,  ... })
 * </pre>
 * With strings the width is the string...
 * @param pattern the pattern to print with
 * @param constraint All the names should start with this, 0 fo rnone
 * @param sort_func the function to sort with (optional)
 * @param only_duplicates only print entries whicxh are the same (using the
 *         sort_func
 * @return 0 on failure, 1 on success
 * @see help::people
 * @see help::netstat
 * @see help::snetstat
 * @see help::netdups
 * @see help::terms
 * @see help::dirs
 */
int do_command(mixed *pattern, string constraint, function sort_func,
               int only_duplicates) {
   object *obs;
   object *tmpobs;
   int i;

   obs = get_people("");
   if (!sizeof(obs)) {
      notify_fail("Nobody seems to start with '" + constraint + "'.\n");
      return 0;
   }
   if (!sort_func) {
      sort_func = (: strcmp($1->query_name(), $2->query_name()) :);
   }

   obs = sort_array(obs, sort_func);

   if (only_duplicates) {
      tmpobs = ({ });
      for (i = 1; i <sizeof(obs); i++) {
         if (!evaluate(sort_func, obs[i - 1], obs[i])) {
            if (member_array(obs[i - 1], tmpobs) == -1) {
               tmpobs += obs[i - 1..i];
            } else {
               tmpobs += ({ obs[i] });
            }
         }
      }
      if (!sizeof(tmpobs)) {
         notify_fail("Unable to find any duplicates.\n");
         return 0;
      }
      obs = tmpobs;
   }
   print_entrys(obs, pattern, only_duplicates, constraint);
   return 1;
} /* do_command() */

/**
 * The review command.  Prints out the current settings for
 * the commands.
 * @param str the player name restriction string
 * @return 0 if it failed, 1 if it succeeded
 * @see help::review
 */
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
  bing = (mixed *)this_player()->query_property("netdup list");
  if (!bing)
    bing = ND_DEFAULT;
  write("Netdups: "+review_thingy(bing)+"\n");
} /* review() */

private mixed *create_review(string str) {
  string *bits, rest;
  int i, bing, width, tmp;
  mixed *ret;

  bits = explode("$"+str, "%");
  bits[0] = bits[0][1..];
  if (!strlen(bits[0]))
    ret = ({ });
  else
    ret = ({ STRING, bits[0] });
  for (i=1;i<sizeof(bits);i++) {
    bing = RIGHT;
    rest = bits[i];
    if (rest[0] == '|') {
      bing = CENTER;
      rest = rest[1..];
    } else if (rest[0] == '-') {
      bing = 0;
      rest = rest[1..];
    }
    if (sscanf(rest, "%d%s", width, rest) == 2) {
      tmp = member_array(rest[0..0], abbrev);
      if (tmp == -1) {
        write("Unknown thingy '"+rest[0..0]+"'\n");
        return 0;
      }
      ret += ({ tmp+bing, width });
      rest = rest[1..];
      if (strlen(rest))
        ret += ({ STRING, rest });
    } else {
      ret += ({ STRING, bits[i] });
    }
  }
  return ret;
} /* create_review() */

private void list_matches() {
  int i;

  for (i=0;i<sizeof(de_names);i++) {
    if (i == STRING) continue;
    printf("%s: %s\n", abbrev[i], de_names[i]);
  }
} /* list_matches() */

/**
 * Sets a variable.  Sets the variable to the specified value.
 * @param the name and arg of the varible
 * @return 0 on failure, 1 on success
 */
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
    notify_fail("You cannot set the var "+name+", it has to be one of "+
                implode(var_names[0..<2], ", ")+" or " + var_names[<1] +
                ".\n");
    return 0;
  }
  bing = create_review(type);
  if (!bing)
    return 1;
  this_player( 1 )->add_property( name +" list", bing );
  write("Ok, set var "+name+" to "+type+".\n");
  return 1;
} /* set_var() */
