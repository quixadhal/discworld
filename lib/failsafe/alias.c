/*
 * The great and wonderful alias driver by me ;)
 * pinkfish.
 */
inherit "failsafe/simul_efun";

string *aliases, *nicknames;
string *aliases_data, *nicknames_data;
static string *doing_alias;
static string *history;
static int hoffset, numoff;
static int in_alias_command;
static string alias_command;

#define align_string(x,len) x+extract("                                                 ",0,len-strlen(x))


string *query_aliases() { return aliases; }

int add_an_alias(string name,string args) {
  int i;
  if (name == "alias" || name == "delete") {
    notify_fail("You cant alias that!\n");
    return 0;
  }
  if ((i=member_array(name,aliases))<0) {
    aliases += ({ name });
    aliases_data += ({ args });
    this_object()->sort_aliases(0,sizeof(aliases_data)-1);
    write("Ok.\n");
    return 1;
  }
  aliases_data[i] = args;
  this_object()->sort_aliases(0,sizeof(aliases_data)-1);
  write("Ok.\n");
  return 1;
}

int print_aliases() 
{
  int i,pos1,pos2;
  string str,str1,str2;

 /* ahh well here goes the clean. you dont want to know what used to
  * be here ;)
  */
  if (sizeof(aliases)==0) {
    notify_fail("None defined.\n");
    return 0;
  }

  str1 = "";
  str2 = "";
  for (i=0;i<sizeof(aliases);i++) {
    str = aliases[i]+": "+aliases_data[i]+"  ";
    if (strlen(str)>39)
      write(str+"\n");
    else if (strlen(str)>19)  {
      str1 += align_string(str,39);
      pos1 = !pos1;
      if (!pos1)
        str1 += "\n";
      else
        str1 += " ";
    } else { 
      str2 += align_string(str,19);
      pos2++;
      if (pos2 >= 4) {
        str2 += "\n";
        pos2 = 0;
     } else
       str2 += " ";
    }
  }
  if (strlen(str1) && pos1)
    write(str1+"\n");
  else
    write(str1);
  if (strlen(str2) && pos2)
    write(str2+"\n");
  else
    write(str2);
  return 1;
}

int print_alias(string str) {
  int i;

  i = member_array(str,aliases);
  write(aliases[i]+": "+aliases_data[i]+"\n");
  return 1;
}

int delete_alias(string str) {
  int i;

  if (!aliases) {
    aliases = ({ });
    aliases_data = ({ });
  }

  if (!str || str == "") {
    notify_fail("Useage: delete <alias>\n");
    return 0;
  }
  i = member_array(str,aliases);
  if (i<0) {
    notify_fail("Must delete an existing alias\n");
    return 0;
  }
  aliases = delete(aliases, i, 1);
  aliases_data = delete(aliases_data, i, 1);
  write("Deleted "+str+".\n");
  return 1;
}

int alias(string str) {
  string name, args;

  if (!aliases) {
    aliases = ({ });
    aliases_data = ({ });
  }

  if (str == "" || !str || !stringp(str))
    return print_aliases();

  if (sscanf(str,"%s %s",name,args)!=2) {
    if (member_array(str,aliases)<0) {
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
      return history[hoffset-1]+extract(arg,1);
    else
      return history[sizeof(history)-1]+extract(arg,1);
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

string *expand_alias(string str,string args) {
  int i, j, k;
  string *new_array, *comms, s1, s3, s4;
  mixed s2;

  if (!args)
    args = "";
  new_array = explode(args," ");
  while (sscanf(str,"%s$*$%s",s1,s2)==2)
    str = s1+args+s2;
  while (sscanf(str,"%s$%d$%s",s1,j,s2)==3)
    if (j>=0 && j<sizeof(new_array))
      str = s1+new_array[j]+s2;
    else
      str = s1+s2;
  while (sscanf(str,"%s$ifarg:%s$else$%s~$%s",s1,s2,s3,s4)==4)
    if (args && args != "")
      str = s1+s2+s4;
    else
      str = s1+s3+s4;
  while (sscanf(str,"%s$ifarg:%s~$%s",s1,s2,s3)==3)
    if (args && args != "")
      str = s1+s2+s3;
    else
      str = s1+s3;
  while (sscanf(str,"%s$arg:%s$%s",s1,s2,s3)==3)
    if (args && args != "")
      str = s1+args+s3;
    else
      str = s1+s2+s3;
  comms = explode(str,";");
  for (i=0;i<sizeof(comms);i++) {
    if (!comms[i])
      comms[i] = "";
    if ((j=member_array(comms[i],
         ({ "n", "s", "e", "w", "ne", "nw", "sw", "se", "u", "d" }) ))>=0)
      comms[i] = ({ "north", "south", "east", "west", "northeast",
                    "northwest", "southwest", "southeast", "up", "down"})[j];
    if (sscanf(comms[i],"%d#%s",j,s1)) {
      s2 = ({ });
      for (k=0;k<j;k++)
        s2 += ({ s1 });
      comms = slice_array(comms,0,i-1) + s2 +
                 slice_array(comms,i+1,sizeof(comms));
    }
  }
  return comms;
}

void do_command(string *str,int i) {
  int k;
  string s1;

  in_alias_command = 1;
  alias_command = str[i];
  command(str[i]);
  in_alias_command = 0;
  i++;
  if (sizeof(str)!=i)
/*    call_out("do_command",0,str); */
    do_command(str,i);
  else
    if (doing_alias && sizeof(doing_alias))
      doing_alias = delete(doing_alias,sizeof(doing_alias)-1, 1);
}

static int exec_command(string str,string args) {
  string *comms;

  comms = expand_alias(str,args);
  if (sizeof(comms))
    do_command(comms,0);
  return 1;
}

static int exec_alias(string verb,string str) {
  int i;
  string s1;

  if (!aliases) {
    aliases = ({ });
    aliases_data = ({ });
  }

  if (sscanf(verb,".%s",s1)==1) {
    if (str)
      command(expand_history(s1)+str);
    else
      command(expand_history(s1));
    return 1;
  }
  if ((i=member_array(verb,aliases))<0)
    return 0;
  if (!doing_alias) {
    doing_alias = ({ });
  }
  if (member_array(verb,doing_alias)!=-1) {
    notify_fail("Recursive aliases. Bad boy\n");
    return 0;
  }
  doing_alias += ({ verb });
  exec_command(aliases_data[i],str);
  return 1;
}

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

int add_nickname(string str) {
  string s1,s2;
  int i;

  if (sscanf(str,"%s %s",s1,s2)!=2) {
    notify_fail("You are a frog\n");
    return 0;
  }
  if (!nicknames) {
    nicknames = ({ });
    nicknames_data = ({ });
  }
  i=member_array(s1,nicknames);
  if (i<0) {
    nicknames += ({ s1 });
    nicknames_data += ({ s2 });
    write("Added nickname "+s1+" as "+s2+".\n");
    return 1;
  }
  write("Changed nickname "+s1+" from "+nicknames_data[i]+" to "+s2+".\n");
  nicknames_data[i] = s2;
  return 1;
}

string expand_nickname(string str) {
  string *array;
  int i,j;

  if (stringp(str))
    array = explode(str," ");
  else
    return str;
  if (!nicknames) {
    nicknames = ({ });
    nicknames_data = ({ });
  }
  for (i=0;i<sizeof(array);i++)
    if ((j=member_array(array[i],nicknames))>=0)
      array[i] = nicknames_data[j];
  return implode(array," ");
}

int delete_nickname(string str) {
  int i;
  if ((i=member_array(str, nicknames)) == -1) {
    notify_fail("That nickname does not exist.\n");
    return 0;
  }
  nicknames = delete(nicknames, i, 1);
  nicknames_data = delete(nicknames_data, i, 1);
  write("Deleted nickname : "+str+"\n");
  return 1;
}

int print_nicknames() {
  int pos, i;
  string str, bit;

  bit = "";
  for (i=0;i<sizeof(nicknames);i++) {
    int tmp;
    str = nicknames[i]+": "+nicknames_data[i]+"  ";
    if ((strlen(str)+pos)>70) {
      bit += "\n";
      pos = 0;
    }
    tmp = pos + strlen(str);
    pos = (((pos + strlen(str))/20)+1)*20;
    bit += str;
    if (pos<=60)
      bit += extract("                          ",pos-tmp);
    pos = pos % 80;
  }
  bit += "\n";
  write(bit);
  return 1;
}

int nickname(string str) {
  string s1,s2;
  int i;

  if (!nicknames) {
    nicknames = ({ });
    nicknames_data = ({ });
  }
  if (!str || str == "")
    return print_nicknames();

  if (sscanf(str,"%s %s",s1,s2)!=2) {
    if ((i=member_array(str,nicknames))<0 || i%2) {
      notify_fail("That nickname does not exist.\n");
      return 0;
    }
    write("Setting for nickname "+str+" is "+nicknames_data[i]+".\n");
    return 1;
  }
  return add_nickname(str);
}

void add_history(string arg) {
int i;
  if (arg)
    arg = " "+arg;
  else
    arg = "";
  if (query_verb()[0] == '.')
    return;
  if (in_alias_command)
    if (alias_command == query_verb()+arg)
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
