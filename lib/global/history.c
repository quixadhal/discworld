/*
 * The great and wonderful alias driver by me ;)
 * pinkfish.
 */
static string *history;
static int hoffset, numoff;
static int in_alias_command;
static string ignore_history;

void history_commands() {
  add_action("print_history", "hi*story");
}

string substitute_history(string arg) {
  string s1, s2;

  if (sscanf(arg, "%s^%s", s1, s2) != 2)
    return arg;
  sscanf(s2, "%s^", s2);
  if (hoffset)
    return replace(history[hoffset-1], s1, s2);
  else
    return replace(history[sizeof(history)-1], s1, s2);
}
 
string expand_history(string arg) {
int num, i;
string s1,s2;

  if (arg[0] == '.')
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

void ignore_from_history(string str) {
  ignore_history = str;
}
 
void add_history(string arg) {
int i;

  if (ignore_history) {
    if (ignore_history == arg) {
      ignore_history = 0;
      return ;
    }
    ignore_history = 0;
  }
  if (arg[0] == '.' || arg[0] == '^')
    return;
  if (in_alias_command)
    return;
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
 
  if(this_player()!=this_object()) { /* a call */
    if(!interactive(this_player())) return 0;
    if(!("secure/master"->query_lord(geteuid(this_player())))) return 0;
  }
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
