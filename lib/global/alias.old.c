/*
 * The great and wonderful alias driver by me ;)
 * pinkfish.
 */
mapping map_aliases, map_nicknames;
static string *doing_alias;
static string *history;
static int hoffset, numoff;
static int in_alias_command;
static string alias_command;
 
string *query_aliaes() { return map_aliases + ({ }); }

/* 
string *query_aliases_data() { return aliases_data+ ({ }); }

mixed *query_alias_data() { return ({ aliases, aliases_data }); }
 */

string *query_nicknames() { return map_nicknames+ ({ }); }
 /*
string *query_nicknames_data() { return nicknames_data+ ({ }); }

mixed *query_nickname_data() { return ({ nicknames, nicknames_data }); }
 */

int add_an_alias(string name,string args) {
  int i;
  if (name == "alias" || name == "unalias") {
    notify_fail("You cant alias that!\n");
    return 0;
  }
  map_aliases[name] = args;
  write("Ok.\n");
  return 1;
}
 
int print_aliases() 
{
  int i,pos1,pos2;
  string str,str1,str2, *tmp;
 
 /* ahh well here goes the clean. you dont want to know what used to
  * be here ;)
  */
  if (!m_sizeof(map_aliases)) {
    notify_fail("None defined.\n");
    return 0;
  }
 
  str1 = "";
  str2 = "";
  tmp = m_indices(map_aliases);
  for (i=0;i<sizeof(tmp);i++) {
    str = tmp[i]+": "+map_aliases[tmp[i]]+"  ";
    if (strlen(str)>39)
      printf(tmp[i]+": %-=*s\n", (int)this_player()->query_cols()-
                                     strlen(tmp[i])-2, map_aliases[tmp[i]]);
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
 
int print_alias(string str) {
  int i;
 
  printf(str+": %-=*s\n", (int)this_player()->query_cols()-
                               strlen(str)-2, map_aliases[str]);
  return 1;
}
 
int delete_alias(string str) {
  int i;
 
 
  if (!str || str == "") {
    notify_fail("Usage: unalias <alias>\n");
    return 0;
  }
  if (!map_aliases[str]) {
    notify_fail("Must delete an existing alias\n");
    return 0;
  }
  map_aliases = m_delete(map_aliases, str);
  write("Deleted "+str+".\n");
  return 1;
}
 
int alias(string str) {
  string name, args;
 
 
  if (str == "" || !str || !stringp(str))
    return print_aliases();
 
  if (sscanf(str,"%s %s",name,args)!=2) {
    if (!map_aliases[str]) {
      notify_fail("You do not have an alias for "+str+".\n");
      return 0;
    }
    return print_alias(str);
  }
  return add_an_alias(name,args);
}
 
string expand_history(string arg) {
int num, i;
string s1,s2;
 
  if (arg[0]=='.')
    if (hoffset)
      return history[hoffset-1]+arg[1..strlen(arg)];
    else
      return history[sizeof(history)-1]+arg[1..strlen(arg)];
  if (sscanf(arg,"%d",num)==1) {
    num = num % sizeof(history);
    if (num<0) {
      num += hoffset;
      if (num<0)
        num += sizeof(history);
    }
    return history[num];
  }
  for (i=hoffset-1;i>=0;i--)
    if (stringp(history[i]))
      if (sscanf(history[i],arg+"%s",s1)==1)
        return history[i];
  for (i=sizeof(history)-1;i>hoffset;i--)
    if (stringp(history[i]))
      if (sscanf(history[i],arg+"%s",s1)==1)
        return history[i];
  return arg;
}

static string *new_array, my_args;

string expand_string(string str, int bong) {
  int i, bing;
  int j;
  string start;
  string s1,s2,s3,s4;

  start = "";
  while (sscanf(str, "%s$%s$%s", s1,s2,s3) ==3)
    if (s2 == "*") {
      start += s1+my_args;
      str = s3;
      bing = 1;
    } else if (sscanf(s2, "%d%s", j, s2) == 2)
      if (s2 == "*") {
        start += s1;
        for (i=0;i<sizeof(new_array);i++)
          start += new_array[i]+" ";
        start += str[0..strlen(str)-2];
        str = s3;
        bing = 1;
      } else {
        start += s1;
        if (j >= 0 && j <sizeof(new_array))
          start += new_array[j];
        str = s3;
        bing = 1;
      }
    else {
      start += s1+"$"+s2;
      str = "$"+s3;
    }
  str  = start + str;
  start = "";
  while (sscanf(str, "%s$%s", s1, s2) == 2) {
    if (s2[0..2] == "arg") {
      s2 = s2[3..strlen(s2)];
      if (s2[0] == ':') {
        i = -1;
        s2 = s2[1..strlen(s2)];
      } else
        sscanf(s2, "%d:%s", i, s2);
      s2 = expand_string(s2,0);
      if (sscanf(s2, "%s$%s", s2, s3) != 2) {
        start += s1+"$";
        str = s2;
        break;
      }
      if (!s3)
        s3 = "";
      if (i == -1)
        if (my_args != "")
          start += s1+my_args;
        else
          start += s1+s2;
      else if (i < sizeof(new_array))
        start += s1 + new_array[i];
      else
        start += s1 + s2;
      str = s3;
      bing = 1;
    } else if (s2[0..4] == "ifarg") {
      s2 = s2[5..strlen(s2)];
      if (s2[0] == ':') {
        i = -1;
        s2 = s2[1..strlen(s2)];
      } else
        sscanf(str, "%d:%s", i, s2);
      s2 = expand_string(s2,0);
      if (sscanf(s2, "%s~$%s", s2, s3) != 2) {
        start += s1+"$";
        str = s2;
        break;
      }
      if (!s3)
        s3 = "";
      sscanf(s2, "%s$else$%s", s2, s4);
      if (!s4)
        s4 = "";
      if (i == -1)
        if (my_args != "")
          start += s1 + s2;
        else
          start += s1 + s4;
      else
        if (i < sizeof(new_array))
          start += s1 + s2;
        else
          start += s1 + s4;
       str = s3;
       bing = 1;
    } else {
      start += s1 + "$";
      str = s2;
    }
  }
  str = start+str;
  while (sscanf(str, "%s\n%d#%s", s4, i, s1) == 2) {
    if (sscanf(s1, "%s\n%s", s2, s3) != 2) {
      s2 = s1;
      s3 = "";
    }
    str = s4+"\n";
    for (j=0;j<i;j++)
      str += s2+"\n";
    str += s3;
  }
  if (!bing && bong)
    str += " "+my_args;
  if (str)
    return str;
  return "";
}
 
string *expand_alias(string str,string args) {
  if (!args)
    args = "";
  my_args = args;
  new_array = explode(args," ");
  str = implode(explode(str, ";"), "\n");
  str = expand_string(str, 1);
  return explode(str,"\n");
}

static void do_command(string *str) {
  int k, i;
  string s1;

  for (i=0;i<sizeof(str);i++) { 
    in_alias_command = 1;
    alias_command = str[i];
    if (str[i])
      command(str[i]);
    in_alias_command = 0;
  }
  if (doing_alias && sizeof(doing_alias))
    doing_alias = delete(doing_alias,sizeof(doing_alias)-1, 1);
}
 
static int exec_command(string str,string args) {
  string *comms;
 
  comms = expand_alias(str,args);
  if (sizeof(comms))
    do_command(comms);
  return 1;
}
 
static int exec_alias(string verb,string str) {
  int i;
  string s1;
 
  if (!map_aliases)
    map_aliases = mkmapping( ({ }), ({ }) );
 
  if (sscanf(verb,".%s",s1)==1) {
    if (str)
      command(expand_history(s1)+str);
    else
      command(expand_history(s1));
    return 1;
  }
  if (!map_aliases[verb])
    return 0;
  if (!doing_alias)
    doing_alias = ({ });
  if (member_array(verb,doing_alias)!=-1) {
    notify_fail("Recursive aliases. Bad boy\n");
    return 0;
  }
  doing_alias += ({ verb });
  exec_command(map_aliases[verb],str);
  return 1;
}
 
/*
int partition(int start,int finish) {
string temp,part;
int upper,lower;
 
  if(start == finish) {
    return start;
  }
  part = aliases[start];
  lower = start;
  upper = finish + 1;
  while(upper != lower) {
    if(aliases[lower] >= part) {
      temp = aliases_data[--upper];
      aliases_data[upper] = aliases_data[lower];
      aliases_data[lower] = temp;
      temp = aliases[upper];
      aliases[upper] = aliases[lower];
      aliases[lower] = temp;
    }
    else {
      lower++;
    }
  }
  temp = aliases_data[finish];
  aliases_data[finish] = aliases_data[lower];
  aliases_data[lower] = temp;
  temp = aliases[finish];
  aliases[finish] = aliases[lower];
  aliases[lower] = temp;
  return lower;
}
 
void sort_aliases(int start,int finish) {
  int part;
 
  part = partition(start,finish);
  if(start != part) {
    sort_aliases(start,part-1);
  }
  if(part != finish) {
    sort_aliases(part+1,finish);
  }
}
*/
 
int add_nickname(string str) {
  string s1,s2;
  int i;
 
  if (sscanf(str,"%s %s",s1,s2)!=2) {
    notify_fail("You are a frog\n");
    return 0;
  }
  if (!map_nicknames)
    map_nicknames = mkmapping( ({ }), ({ }) );
  if (!map_nicknames[s1]) {
    map_nicknames[s1] = s2;
    write("Added nickname "+s1+" as "+s2+".\n");
    return 1;
  }
  write("Changed nickname "+s1+" from "+map_nicknames[s1]+" to "+s2+".\n");
  map_nicknames[s1] = s2;
  return 1;
}
 
string expand_nickname(string str) {
  string *array;
  int i,j;
 
  if (stringp(str))
    array = explode(lower_case(str)," ");
  else
    return str;
  if (!map_nicknames)
    map_nicknames = mkmapping( ({ }), ({ }) );
  for (i=0;i<sizeof(array);i++)
    if (map_nicknames[array[i]])
      array[i] = map_nicknames[array[i]];
  return implode(array," ");
}
 
int delete_nickname(string str) {
  int i;
  if (!map_nicknames)
    map_nicknames = mkmapping( ({ }), ({ }) );
  if (!map_nicknames[str]) {
    notify_fail("That nickname does not exist.\n");
    return 0;
  }
  map_nicknames = m_delete(map_nicknames, str);
  write("Deleted nickname : "+str+"\n");
  return 1;
}
 
int print_nicknames() {
  int pos, i, cols;
  string str, str1, str2, bit, *tmp;
 
  bit = "";
  str1 = "";
  str2 = "";
  if (!map_nicknames)
    map_nicknames = mkmapping( ({ }), ({ }) );
  tmp = m_indices(map_nicknames);
  cols = (int)this_player()->query_cols();
  for (i=0;i<sizeof(tmp);i++) {
    str = tmp[i]+": "+map_nicknames[tmp[i]]+"  ";
    if (strlen(str)>39)
      printf(tmp[i]+": %-=*s\n", cols - strlen(tmp[i]), map_nicknames[tmp[i]]);
    else if (strlen(str) > 19)
      str1 += str+"\n";
    else
      str2 += str+"\n";
  }
  if (strlen(str1))
    printf("%-#*s\n", cols, str1);
  if (strlen(str2))
    printf("%-#*s\n", cols, str2);
  return 1;
}
 
int nickname(string str) {
  string s1,s2;
  int i;
 
  if (!map_nicknames)
    map_nicknames = mkmapping( ({ }), ({ }) );
  if (!str || str == "")
    return print_nicknames();
 
  if (sscanf(str,"%s %s",s1,s2)!=2) {
    if (!map_nicknames[s1]) {
      notify_fail("That nickname does not exist.\n");
      return 0;
    }
    write("Setting for nickname "+str+" is "+map_nicknames[str]+".\n");
    return 1;
  }
  return add_nickname(str);
}
 
void add_history(string arg) {
int i;
  if (arg[0] == '.')
    return;
  if (in_alias_command)
    if (alias_command == arg)
      return;
    else {
      doing_alias = ({ });
      in_alias_command = 0;
    }
  if (!history) {
    history = allocate(40);
    numoff = -40;
  }
  history[hoffset++] = query_verb()+arg;
  if (hoffset>=sizeof(history)) {
    numoff += sizeof(history);
    hoffset = 0;
  }
}
 
int print_history(string arg) {
int i, from, to, num;
 
  from = hoffset;
  num = sizeof(history);
  if (!arg)
    arg = "";
  if (sscanf(arg,"%d,%d",from,to)==2) {
    num = to-from;
    if (num<0) {
      from = from+num;
      num = -num;
    }
  } else if (sscanf(arg,",%d",to)==1) {
    from = hoffset+1+sizeof(history);
    num = from - (to%sizeof(history))+sizeof(history);
  } else if (sscanf(arg,"%d,",from)==1) {
    from = from % sizeof(history);
    num = from - hoffset;
    if (num<0)
      num += sizeof(history);
  } else if (sscanf(arg,"%d",num)!=1)
    num = sizeof(history);
  from = from % sizeof(history);
  if (num>sizeof(history))
    num = sizeof(history);
  for (i=from;i<sizeof(history);i++,num--) {
    if (history[i])
      write((i+numoff)+": "+history[i]+"\n");
    if (!num)
      return 1;
  }
  for (i=0;i<from;i++,num--) {
    if (history[i])
      write((i+numoff+sizeof(history))+": "+history[i]+"\n");
    if (!num)
      return 1;
  }
  return 1;
}
