#include "mail.h"
/* finger object.... */
/* this object returns finger information about a given person.
 */
 
string title;
string birth_day, where;
int last_log_on, time_on, creator, start_time;
string email, real_name, home_dir, last_on_from;
mapping aliases;
string guild_ob;

string make_string(mixed *al) {
  string str;

  str = (string)"/global/alias"->alias_string(al);
  sscanf(str, "%s $*$", str);
  return str;
} /* make_string() */

string banish_finger(string name) {
  string retval, time, ban_by;
  string *file;

  file = explode(read_file("/banish/"+name+".o"), "\n");
  retval =  sprintf("%35-s%35-s\n", "Login name : "+name, "Real name : "+
                            "Banished");
  retval += "Not real many seconds old.\n";
  retval += "No mail ever.\n";
  retval += "No plan.\nNo future.\n";
  if (this_player()->query_creator())
    if (sizeof(file) == 3)
      retval += implode(file, "\n")+"\n";
    else
      retval += "Banish info not in the correct format.\n";
  return retval;
} /* banish_finger() */

string domain_finger(string name) {
  string ret, master;

  master = "/d/"+name+"/master";
  ret = "The domain of "+name+".\n";
  ret += "The lord of this domain is "+master->query_lord()+".\n";
  ret += "The current members of this domains are "+
         implode((string *)master->query_members(), ", ")+".\n";
  if (master->query_info())
    ret += (string)master->query_info();
  ret += "It hasn't been idle, it hasnt had a lunch break for years and "+
         "it is a frog.\n";
  return ret;
} /* domain_finger() */

string finger_info(string name) {
  string retval, nick, *bing;
  object ob;
  int i;

  if ((ob = find_player(name)))
    ob->save_me();
  if (this_player())
    nick = (string)this_player()->expand_nickname(name);
  if (nick)
    name = nick;
  if (file_size("/banish/"+name+".o") > 0)
    return banish_finger(name);
  seteuid("Root");
  if (file_size("/d/"+name) == -2)
    return domain_finger(name);
  if (!"/secure/login"->test_user(name))
     return 0;
  title = 0;
  birth_day = 0;
  last_log_on = 0;
  time_on = 0;
  email = 0;
  where = 0;
  real_name = 0;
  home_dir = 0;
  guild_ob  = 0;
  last_on_from = 0;
  start_time = 0;
  aliases = ([ ]);
  restore_object("/players/"+name[0..0]+"/"+name);
  retval =  sprintf("%35-s%35-s\n", "Login name : "+name, "Real name : "+
                            (real_name?real_name:"???"));
  if (birth_day)
     retval += sprintf("%35-s", "Birthday : " + birth_day);
  if (email)
    if (email[0] == ':') {
      if ("/secure/master"->valid_read("/players/"+name[0..0]+"/"+name,
                                   geteuid(previous_object())))
        retval += "Email : "+email+"\n";
    } else
      retval += "Email : "+email+"\n";
  else if (birth_day)
    retval += "\n";
  if (home_dir)
    retval += sprintf("%35-s", "Home directory : "+home_dir);
  if (guild_ob)
    retval += "Is a member of the "+guild_ob->query_name()+" guild.\n";
  else if (home_dir)
    retval += "\n";
  if(where)
    retval += sprintf("%35-s", "Location: "+where+"\n");
  if (creator) {
/* Find out which domains they are a member of... */
    bing = get_dir("/d/");
    for (i=0;i<sizeof(bing);i++)
     if (file_size("/d/"+bing[i]) == -2)
        if ((string)("/d/"+bing[i]+"/master")->query_lord() == name) {
          retval += "Is a lord of the "+bing[i]+" domain.\n";
        } else if (!("/d/"+bing[i]+"/master")->query_member(name)) {
          bing = delete(bing, i, 1);
          i--;
        }
    if (!sizeof(bing))
      retval += "Not a member of any domain.\n";
    else
      retval += "A member of the "+implode(bing, ", ")+" domain"+
                (sizeof(bing)>2?"s.\n":".\n");
  }
  if (start_time)
    retval += "First logged on "+ctime(start_time)+".\n";
  time_on = -time_on;
  if (time_on > 86400)
    retval += sprintf("%d days, ", time_on/86400);
  if (time_on > 3600)
    retval += sprintf("%d hours, ", (time_on/3600)%24);
  retval += sprintf("%d minutes and %d seconds old.\n", (time_on/60)%60, time_on%60);
  if ((ob=find_living(name)))
    retval += "On since "+ctime(last_log_on)+"\n";
/*
  else if (ob->query_hidden())
    retval += "Last logged on"+ctime(last_log_on - 14400)+"\n";
*/
  else {
    int tmp_time, sec, min, hour, day;

/* Should be a nice number.... */
    tmp_time = time()-last_log_on;
    if (!tmp_time) {
      sec = min = hour = day = 0;
    }
    if (tmp_time < 60)
      retval += "Last logged on "+tmp_time+" seconds ago.\n";
    else if (tmp_time < 60*60) /* One minute */
      retval += "Last logged on "+(tmp_time/60)+" minute"+(tmp_time<120?"":"s")+
                (tmp_time%60?" and "+(tmp_time%60)+" second"+
                             ((tmp_time%60 < 2)?"":"s"):"")+" ago.\n";
    else if (tmp_time < 24*60*60) /* Hours... */
      retval += "Last logged on "+(hour = tmp_time/(60*60))+" hour"+
                (hour<2?"":"s")+((min = (tmp_time/60)%60)?" and "+min+
                " minute"+(min<2?"":"s"):"")+" ago.\n";
    else /* Days.... */
      retval += "Last logged on "+(day = tmp_time/(24*60*60))+" day"+
                (day<2?"":"s")+((hour = (tmp_time/(60*60))%24)?" and "+hour+
                " hour"+(hour<2?"":"s"):"")+" ago.\n";
/*
    retval += "Last logged on "+ctime(last_log_on)+"\n";
 */
  }
  if (ob)
    if (interactive(ob))
      retval += "Idle for "+(query_idle(ob)/60)+" minutes and "+
                            (query_idle(ob)%60)+" seconds.\n";
    else
      retval += "Net dead.\n";
  if ("/secure/master"->query_lord(previous_object()->query_name()) &&
      last_on_from)
    retval += last_on_from+"\n";
  retval += (string)MAILER->finger_mail(name);
  if (!mappingp(aliases))
    aliases = ([ ]);
  if (aliases[".project"])
    retval += "Project: "+make_string(aliases[".project"])+"\n";
  if (aliases[".plan"])
    return retval+"Plan:\n"+implode(explode(make_string(aliases[".plan"]),
                                     ";"), "\n")+"\n";
  return retval+"No plan.\n";
}
