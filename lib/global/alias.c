#include "alias.h"
#include "player.h"
inherit "/global/history";

int convert();

mapping aliases, map_aliases;
static mapping doing_alias;

mixed *compile_alias(string str) {
  mixed *ret;
  int i, space;
  string *frog, *lines, s1, s2;
  int num, tmp, gumby, nodollar;
  mixed *ifargs;

  str = replace(str, ({ "\\;", "$escaped$", ";", "$new_line$", "", "" }));
  str = replace(str, "$escaped$", ";");
  str = "&"+str+"&";
  frog = explode(str, "$");
  if (frog[0] == "&")
    frog = delete(frog, 0, 1);
  else
    frog[0] = frog[0][1..100];
  s1 = frog[sizeof(frog)-1];
  if (s1 == "&")
    frog = delete(frog, sizeof(frog)-1, 1);
  else
    frog[sizeof(frog)-1] = s1[0..strlen(s1)-2];
  ret = ({ });
  ifargs = ({ });
  nodollar = 1;
  for (i=0;i<sizeof(frog);i++)
    if (frog[i] == "new_line") {
      ret += ({ NEW_LINE });
      nodollar = 1;
    } else if (frog[i] == "*") {
      ret += ({ ALL_ARGS });
      gumby = 1;
      nodollar = 1;
/* hack by CPR for andrew so that $!$ substitues current location */
    } else if (frog[i] == "!" && this_object()->query_creator()) {
      ret += ({ CURR_LOC });
      nodollar = 1;
/* end of hack by CPR */
    } else if (frog[i][0..4] == "ifarg") {
      if (sscanf(frog[i], "ifarg%d:%s", tmp, s1) == 2) {
        if (tmp < 0)
          tmp = 0;
        if (tmp > ALIAS_MASK)
          tmp = ALIAS_MASK;
        ret += ({ IFARG_THING+ tmp, 0, "" });
        frog[i--] = s1;
        nodollar = 1;
        ifargs += ({ sizeof(ret)-2 });
        space = 0;
        gumby = 1;
      } else if (frog[i][5] == ':') {
        ret += ({ ALL_IFARG, 0, "" });
        frog[i] = frog[i--][6..100];
        nodollar = 1;
        ifargs += ({ sizeof(ret)-2 });
        space = 0;
        gumby = 1;
      } else
        if (sizeof(ret) && stringp(ret[sizeof(ret)-1]) && !space)
          ret[sizeof(ret)-1] += "$"+frog[i];
        else if (nodollar) {
          ret += ({ frog[i] });
          nodollar = 0;
        } else
          ret += ({ "$"+frog[i] });
    } else if (frog[i][0..2] == "arg") {
      if (sscanf(frog[i], "arg%d:%s", tmp, s1) == 2) {
        if (tmp < 0)
          tmp = 0;
        if (tmp > ALIAS_MASK)
          tmp = ALIAS_MASK;
        ret += ({ ARG_THING+ tmp, s1, "" }); 
        nodollar = 1;
      } else if (frog[i][3] == ':') {
        ret += ({ ALL_ARG, frog[i][4..100], "" });
        nodollar = 1;
      } else
        if (sizeof(ret) && stringp(ret[sizeof(ret)-1]) && !space)
          ret[sizeof(ret)-1] += "$"+frog[i];
        else if (nodollar) {
          ret += ({ frog[i] });
          nodollar = 0;
        } else
          ret += ({ "$"+frog[i] });
      gumby = 1;
      space = 0;
    } else if (frog[i] == "else" && sizeof(ifargs)) {
      ret[ifargs[sizeof(ifargs)-1]] = sizeof(ret)-ifargs[sizeof(ifargs)-1]+1;
      ret += ({ ELSE_THING, 0, "" });
      ifargs[sizeof(ifargs)-1] = sizeof(ret)-2;
      nodollar = 1;
    } else if (strlen(frog[i]) && frog[i][strlen(frog[i])-1] == '*' &&
               sscanf(frog[i], "%d%s*", tmp, s2) == 2 && s1 == "") {
      if (tmp < 0)
        tmp = 0;
      if (tmp > ALIAS_MASK)
        tmp = ALIAS_MASK;
      ret += ({ FROM_ARG + tmp });
      gumby = 1;
      nodollar = 1;
    } else if (strlen(frog[i]) && frog[i][0] == '*' && 
               sscanf(frog[i][1..1000], "%d%s", tmp,s1) == 2 && s1 == "") {
      if (tmp < 0)
        tmp = 0;
      if (tmp > ALIAS_MASK)
        tmp = ALIAS_MASK;
      ret += ({ TO_ARG + tmp });
      gumby = 1;
      nodollar = 1;
    } else if (sscanf(frog[i], "%d%s", tmp, s1) == 2 && s1 == "") {
      if (tmp < 0)
        tmp = 0;
      if (tmp > ALIAS_MASK)
        tmp = ALIAS_MASK;
      ret += ({ ONE_ARG + tmp });
      gumby = 1;
      nodollar = 1;
    } else if ((frog[i] == "~" || frog[i] == "endif") && sizeof(ifargs)) {
      ret += ({ END_IF });
      ret[ifargs[sizeof(ifargs)-1]] = sizeof(ret)-
                                      ifargs[sizeof(ifargs)-1];
      ifargs = delete(ifargs, sizeof(ifargs)-1, 1);
      nodollar = 1;
      space = 1;
    } else {
      if (!nodollar)
        frog[i] = "$"+frog[i];
      nodollar = 0;
      space = 0;
      if (strlen(frog[i]) && frog[i][strlen(frog[i])-1] == '~')
        if (sizeof(ifargs)) {
          if (strlen(frog[i]) == 1)
            frog[i] = "";
          else
            frog[i] = frog[i][0..strlen(frog[i])-2];
/* create an offset */
          ret += ({ END_IF });
          ret[ifargs[sizeof(ifargs)-1]] = sizeof(ret)-
                                           ifargs[sizeof(ifargs)-1];
          ifargs = delete(ifargs, sizeof(ifargs)-1, 1);
          nodollar = 1;
          space = 1;
        }
      if (sizeof(ret) && stringp(ret[sizeof(ret)-1]) && space != 2)
        ret[sizeof(ret)-1] += frog[i];
      else  
        ret += ({ frog[i] });
      if (space)
        space = 2;
    }
  while (sizeof(ifargs)) {
    ret += ({ END_IF });
    ret[ifargs[sizeof(ifargs)-1]] = sizeof(ret)-
                                    ifargs[sizeof(ifargs)-1];
    ifargs = delete(ifargs, sizeof(ifargs)-1, 1);
  }
  if (!gumby) {
    if (sizeof(ret) && !stringp(ret[sizeof(ret)-1]) || space)
      ret += ({ " ", ALL_ARGS });
    else {
      ret[sizeof(ret)-1] += " ";
      ret += ({ ALL_ARGS });
    }
  }
  return ret;
}

string alias_string(mixed *al) {
  int i, num, *add_thing;
  string str;

  str = "";
  add_thing = ({ });
  for (i=0;i<sizeof(al);i++) {
    if (stringp(al[i]))
      str += replace(al[i], ";", "\\;");
    else {
      num = al[i] & ALIAS_MASK;
      switch (al[i] - num) {
        case NEW_LINE  : str += ";";
                         break;
        case ALL_ARGS  : str += "$*$";
                         break;
        case ONE_ARG   : str += "$"+num+"$";
                         break;
        case TO_ARG    : str += "$*"+num+"$";
                         break;
        case FROM_ARG  : str += "$"+num+"*$";
                         break;
        case ALL_ARG   : str += "$arg:"+al[++i]+"$";
                         break;
        case ARG_THING : str += "$arg"+num+":"+al[++i]+"$";
                         break;
        case ELSE_THING :
                         str += "$else$";
/*
                         add_thing[sizeof(add_thing)-1] = i+1+al[++i];
 */
                         break;
        case ALL_IFARG : str += "$ifarg:";
/*
                         add_thing += ({ i+1+al[++i] });
 */
                         break;
        case IFARG_THING :
                         str += "$ifarg"+num+":";
/*
                         add_thing += ({ i+1+al[++i] });
 */
                         break;
	case CURR_LOC	 :
			 str += "$!$";
			 break;
        case END_IF      :
                         str += "$endif$";
                         break;
      }
    }
/*
    if (member_array(i, add_thing) != -1)
      str += "~$";
 */
  }
  return str;
}

static string editing_alias;

int edit_alias(string str) {
  string al;

  if (editing_alias) {
    notify_fail("You are already editing the '"+editing_alias+"' alias.\n");
    return 0;
  }
  if (!str) {
    notify_fail("Syntax: "+query_verb()+" <alias>\n");
    return 0;
  }
  if (!aliases[str]) {
    notify_fail("No alias '"+str+"' defined.\n");
    return 0;
  }
  editing_alias = str;
  al = replace(alias_string(aliases[str]), ";", "\n");
  this_object()->do_edit(al, "finish_alias_edit", 0, 1);
  return 1;
}

int finish_alias_edit(string str) {
  if (!str) {
    editing_alias = 0;
    return 0;
  }
  str = replace(str, "\n", ";");
  aliases[editing_alias] = compile_alias(str);
  write("Changed alias '"+editing_alias+"'\n");
  editing_alias = 0;
  return 1;
}

void alias_commands() {
  add_action("alias", "al*ias");
  add_action("unalias", "un*alias");
  add_action("edit_alias", "ea*lias");
  add_action("remove_alias_thing", "END_ALIAS");
}

int print_aliases() {
  int i,pos1,pos2;
  string str,str1,str2, *tmp, bing;
 
 /* ahh well here goes the clean. you dont want to know what used to
  * be here ;)
  */
  if (!m_sizeof(aliases)) {
    notify_fail("None defined.\n");
    return 0;
  }
 
  str1 = "";
  str2 = "";
  tmp = m_indices(aliases);
  for (i=0;i<sizeof(tmp);i++) {
    if (!tmp[i]) {
      aliases = m_delete(aliases, 0);
      continue;
    }
    bing = alias_string(aliases[tmp[i]]);
    str = tmp[i]+": "+bing;
    if (strlen(str)>39)
      printf(tmp[i]+": %-=*s\n", (int)this_player()->query_cols()-
                                     strlen(tmp[i])-2, bing);
    else if (strlen(str)>19)
      str1 += str+"\n";
    else
      str2 += str+"\n";
  }
  if (strlen(str1))
    printf("%-#*s\n", this_player()->query_cols(), str1);
  if (strlen(str2))
    printf("%-#*s\n", this_player()->query_cols(), str2);
  return 1;
}

int alias(string str, int bing) {
  string s1, s2;

  if (!mappingp(aliases))
    aliases = ([ ]);
  if (map_aliases && !bing)
    convert();
  if (!str)
    return print_aliases();
  if (sscanf(str, "%s %s", s1, s2) != 2) {
    if (!aliases[str]) {
      notify_fail("No alias '"+str+"' defined.\n");
      return 0;
    }
    printf("%s: %-=*s\n", str, (int)this_player()->query_cols() -
                          strlen(str) -2, alias_string(aliases[str]));
    return 1;
  }
  if (!aliases[s1]) {
    aliases[s1] = compile_alias(s2);
    write("Added alias '"+s1+"'.\n");
  } else {
    aliases[s1] = compile_alias(s2);
    write("Changed alias '"+s1+"'.\n");
  }
  return 1;
}

int unalias(string str) {
  if (!mappingp(aliases))
    aliases = ([ ]);
  if (map_aliases)
    convert();
  if (!str) {
    notify_fail("Syntax: "+query_verb()+" <alias>\n");
    return 0;
  }
  if (!aliases[str]) {
    notify_fail("The alias '"+str+"' does not exist, cannot unalias.\n");
    return 0;
  }
  aliases = m_delete(aliases, str);
  return 1;
}

mapping query_aliases() { return aliases; }

static int exec_alias(string verb, string args) {
  int i, num;
  string *bits, line;
  mixed stuff;

  if (verb[0] == '.') {
    if (args)
      command(expand_history(verb[1..1000])+" "+args);
    else
      command(expand_history(verb[1..1000]));
    return 1;
  }
  if (verb[0] == '^') {
    if (args)
      command(substitute_history(verb[1..1000]+" "+args));
    else
      command(substitute_history(verb[1..1000]));
    return 1;
  }
  if (!mappingp(aliases))
    aliases = ([ ]);
  if (!aliases[verb])
    return 0;
  if (!doing_alias)
    doing_alias = ([ ]);
  if (doing_alias[verb]) {
    notify_fail("Recursive aliases. Bad (insert appropriate gender).\n");
    return 0;
  }
  doing_alias[verb] = 1;
  in_alias_command++;
  stuff = aliases[verb];
  line = "";
  if (!args)
    args = "";
  bits = explode(verb+" "+args, " ");
  for (i=0;i<sizeof(stuff);i++)
    if (stringp(stuff[i]))
      line += stuff[i];
    else {
      num = stuff[i] & ALIAS_MASK;
      switch (stuff[i] - num) {
        case NEW_LINE :
                        command(line);
                        line = "";
                        break;
        case ALL_ARGS : line += args;
                        break;
        case ONE_ARG  : if (num < sizeof(bits))
                          line += bits[num];
                        break;
        case TO_ARG   : line += implode(bits[1..num], " ");
                        break;
        case FROM_ARG : line += implode(bits[num..100], " ");
                        break;
        case ALL_ARG  : i++;
                        if (args == "")
                          line += stuff[i];
                        else
                          line += args;
                        break;
        case ARG_THING : i++;
                         if (num < sizeof(bits))
                           line += bits[num];
                         else
                           line += stuff[i];
                         break;
        case ALL_IFARG : i++;
                         if (args == "")
                           i += stuff[i];
                         break;
        case IFARG_THING :
                         i++;
                         if (num >= sizeof(bits))
                           i += stuff[i];
                         break;
        case ELSE_THING :
                         i++;
                         i += stuff[i];
                         break;
        case CURR_LOC :
                         i++;
                         line += file_name(environment())+".c";
                         break;
        case END_IF :
                         break;
      }
    }
  if (line != "")
    command(line);
  command("END_ALIAS "+verb);
  return 1;
}

int remove_alias_thing(string verb) {
  in_alias_command--;
  doing_alias = m_delete(doing_alias, verb);
  this_player()->adjust_time_left(-DEFAULT_TIME);
  return 1;
}

int convert() {
  int i;
  string *str;

  str = m_indices(map_aliases);
  for (i=0;i<sizeof(str);i++)
    alias(str[i]+" "+map_aliases[str[i]], 1);
  map_aliases = 0;
  return 1;
}
