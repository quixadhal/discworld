inherit "/obj/clock";

int our_offset,
    their_offset,
    last_time,
    cur_alarm,
    offset_alarm,
    alarm;
mixed *alarms; 
string *alarm_write,
       alarm_name;
object our_player;

varargs int get_text(string line);

void setup() {
  set_name("watch");
  add_adjective("demonic");
  add_alias("watch");
  set_main_plural("demonic watches");
  set_short("demonic watch");
  set_long("A small wrist attackable living quaters for a demon "+
           "the demon looks pretty harried about giveing out the "+
           "time all the time, and the pay is lousy..\n");
  set_value(20000);
  our_offset = 0;
  their_offset = 8;
  last_time = time();
  call_out("check_alarm",60);
  alarms = ({ });
}
/*
string long(string word, int dark) {
  if (our_offset == their_offset)
    return ::long()+".\n"+get_time()+".\n";
  else
    return ::long()+"\n"+get_time()+"\nMud time: "+ctime(time())+".\n";
}

string query_read() {
  if (our_offset == their_offset)
    return get_time()+".\n";
  else
    return get_time()+"\nMud time: "+ctime(time())+".\n";
}
*/

void init() {
  add_command("time", "");
  add_command("gmt", "<word'offset'>", (:this_object()->set_gmt($4[0]):));
  add_command("use", "<direct:object>", (:this_object()->use():));
  this_player()->add_command("read", this_object());
}

int use(string arg) {
  if (!alarms)
    alarms = ({ });
  printf("%-=80s","You look into the watch and fiddle with several buttons, "+
                  "Finally a light lights up in the top corner say you have "+
                  "the security clearance to modify the settings.\n"+
                  "You can \"set\" an alarm \"remove\" an alarm \"list\" the "+
                  "set alarms. You can \"quit\" also.\n");
  write("What do you wish to do ? ");
  input_to("the_command");
  return 1;
}

int the_command(string arg) {
  string s1;
  int i;

  if (arg == "quit") {
    write("Thank you for using this wonderful watch.\n");
    return 1;
  }
  if (sscanf(arg,"set%s",s1)) {
    int hour, min;
    string name;
    if (sscanf(s1," %s %d:%d%s",name,hour,min,s1)!=4)
      write("Usage: set <name> <hour>:<min> [+]\n");
    else {
      if ((i=member_array(name,alarms))<0)
        alarms += ({ name, hour, min, 0 });
      else
        if (i%4)
          write("Oooooopps, you cannot use that name as an alarm\n");
        else {
          alarms[i+1] = hour;
          alarms[i+2] = min;
        }
      if (s1 == " +") {
        if (i==-1)
          i = sizeof(alarms)-4;
        cur_alarm = i;
        get_text();
        return 1;
      }
    }
  }
  if (sscanf(arg,"remove%s",s1)) {
    string frog;
    if (sscanf(s1," %s",frog)!=1)
      write("Usage: remove <name>\n");
    else
      if ((i=member_array(frog,alarms))<0)
        write("You must remove an existing alarm\n");
      else
        if (i%4)
          write("Ohhh dear.\n");
        else {
          alarms = delete(alarms,i);
          alarms = delete(alarms,i);
          alarms = delete(alarms,i);
          alarms = delete(alarms,i);
        }
  }
  if (sscanf(arg,"list%s",s1)) {
    string chicken;
    if (sscanf(s1," %s",chicken)!=1) {
      for (i=0;i<sizeof(alarms);i+=4) {
        write("Alarm "+alarms[i]+" is set for "+alarms[i+1]+":"+alarms[i+2]+"\n");
      }
    } else {
      if ((i=member_array(chicken,alarms))<0)
        write("That alarm does not exist.\n");
      else
        if (i%4)
          write("Bing basicly");
        else
          write("You have set alarm "+chicken+" to go off at "+alarms[i+1]+":"+
                 alarms[i+2]+"\n");
    }
  }
  return 1;
}

varargs int get_text(string line) {
   string text;
  if (!line) {
    text = "";
    write(sprintf("%-=80s","Please enter the text you wish to be displayed "+
          "when the alarm goes off (. to finish)"));
  } else if (line==".") {
    alarms[cur_alarm+3] = text+"\n";
    write("What do you wish to do ? ");
    input_to("the_command");
    return 1;
  } else
    text += line+"\n";
  write("] ");
  input_to("get_text");
  return 1;
}

mixed query_auto_load() {
  mixed ret;

  if (!alarms)
    alarms = ({ });
  ret = ({ their_offset, alarms });
  return ret;
}

void init_arg(mixed *arg) {
  their_offset = arg[0];
  alarms = arg[1];
  if (!alarms)
    alarms = ({ });
}

void check_alarm() {
   int the_time, offset, i;
   int tmp;
   
   the_time = time();
   offset = (the_time/(3600*24))*3600*24 - our_offset*3600;
   for (i=0;i<sizeof(alarms);i+=4) {

      tmp = offset + (alarms[i+1]-our_offset+their_offset)*3600 + alarms[i+2]*60;
      if (the_time>tmp && tmp>last_time) {
         /* we go off. bounce */
         alarm = 6;
         alarm_name = alarms[i];
         if (alarms[i+3])
         alarm_write = explode(alarms[i+3],"\n");
      else
         alarm_write = ({
            "Beep Beep Beep, Your "+alarms[i]+" alarm has gone off.\n",
            "Beep Beep Beep, Your "+alarms[i]+" alarm has gone off.\n",
            "Beep Beep Beep, Your "+alarms[i]+" alarm has gone off.\n",
            "Beep Beep Beep, Your "+alarms[i]+" alarm has gone off.\n",
            "Beep Beep Beep, Your "+alarms[i]+" alarm has gone off.\n",
            "Beep Beep Beep, Your "+alarms[i]+" alarm has gone off.\n",
         });
         offset_alarm = 0;
         set_heart_beat(1);
      }
   }
   last_time = the_time;
   call_out("check_alarm",60);
}

void heart_beat() {
   if (our_player)
     our_player->event_say(this_object(), "The watch goes: "+
                           alarm_write[offset_alarm++]+"\n");
   if (offset_alarm>sizeof(alarm_write))
     set_heart_beat(0);
}

int move(object ob) {
  int i;

  i = ::move(ob);
  our_player = environment();
  return i;
}
