#define MASTER "/secure/master"
#define PEOPLER "/obj/handlers/peopler"
 
varargs object *wiz_present(string str, object onobj, int nogoout);
string desc_object(mixed o);
string desc_f_object(object o);

static void wiz_commands() {
  add_action("shutdown_game", "shutdown");
  add_action("local_cmds", "loca*lcmd");
  add_action("do_trace", "trace");
  add_action("set_traceprefix", "traceprefix");
  add_action("do_gauge", "ga*uge");
  add_action("do_author", "author");
  add_action("do_domain", "domain");
  add_action("do_status", "status");
  add_action("do_malloc", "malloc");
  add_action("do_fds", "fds");
  add_action("do_sockets", "sockets");
  add_action("do_set", "set");
} /* wiz_commands() */
     
/*
** commands in the app_commands will go to all players with
** level >= 20. This includes players who have opted to
** remain players rather than advancing to wizard. As such
** we must be careful in what we place here
*/
 
static void app_commands() {
  add_action("do_people","pe*ople");
  add_action("quick_people", "qp*eople");
  add_action("do_dirs", "di*rs");
  add_action("do_netstat", "ne*tstat");
  add_action("do_snoopers", "snoope*rs");
  add_action("do_snoop", "snoop");
  add_action("do_snoop", "qsnoop");
  add_action("do_terms", "terms");
  add_action("do_uptime", "uptime");
} /* app_commands() */
 
/* These commands go to ALL players. Note that master.c
** gives only limited read/write access to dirs anyway,
** so the apparent security problems are NOT a problem
*/
 
static void all_commands() {
} /* all_commands() */
 
int do_people(string str) {
  return (int)PEOPLER->do_people(str);
} /* do_people() */

int quick_people(string str) {
  return (int)PEOPLER->do_qpeople(str);
} /* do_people() */

int do_terms(string str) {
  return (int)PEOPLER->do_terms(str);
} /* do_people() */

int do_netstat(string str) {
  return (int)PEOPLER->do_netstat(str);
} /* do_people() */

int do_snoopers(string str) {
  object *obs;
  int i;

  obs = (object *)PEOPLER->get_people(str);
  obs = filter_array(obs, "check_snoop", this_object());
  if (!sizeof(obs)) {
    notify_fail("No one is being snooped by anyone.\n");
    return 0;
  }
  for (i=0;i<sizeof(obs);i++)
    write(obs[i]->query_cap_name()+" is snooping "+
          obs[i]->query_snoopee()->query_cap_name()+".\n");
  return 1;
} /* do_snoopers() */

int check_snoop(object ob) {
  if((object)ob->query_snoopee())
    return 1;
  else
    return 0;
} /* check_snoop() */

static int do_snoop(string str) {
  object targ;

  if (!str) {
    snoop(this_object(), 0);
    write("Ok, snoop cancelled.\n");
    return 1;
  }
  str = (string)this_player()->expand_nickname(str);
  if(!(targ = find_player(str))) {
    notify_fail("Cannot find "+str+".\n");
    return 0;
  }
  if(!snoop(this_object(), targ)) {
    notify_fail("You fail to snoop " + capitalize(str) + ".\n");
    return 0;
  }
  write("Ok, snooping "+str+".\n");
  return 1;
} /* do_snoop() */

static int do_dirs(string str) {
  return (int)PEOPLER->do_dirs(str);
} /* do_dirs() */

static do_set(string str) {
  return (int)PEOPLER->set_var(str);
} /* do_set() */

int review() {
  PEOPLER->review();
  return 1;
} /* review() */

static int shutdown_game(string arg) {
  int tim;
  string str;

  if (this_player(1) != this_object()) return 0;
  if (!arg) {
    write("Argument required.\n");
    return 1;
  }
  str = (string)this_player()->query_name()+" at "+ctime(time());
  log_file("GAME_LOG", "Game shutdown by ");
  log_file("GAME_LOG", str);
/*
  log_file("GAME_LOG", ctime(time()));
 */
  log_file("GAME_LOG", " for\n");
  log_file("GAME_LOG", arg);
  log_file("GAME_LOG", "\n\n");
  write("Ok... shutting down game... \n");
  sscanf(arg, "%d %s", tim, arg);
  if (tim < 10 && !MASTER->query_lord(geteuid(this_object())))
    tim = 10;
  if (str[0..2] == "now" && MASTER->query_lord(geteuid(this_object())))
    tim = 0;
  if (!tim)
    "/obj/shut"->end_it_all();
  else
    "/obj/shut"->shut(tim);
  return 1;
} /* shutdown_game() */
 
static int local_cmds(string str) {
  this_object()->more_string("Currently Defined Commands:\n"+
    sprintf("%#*-s\n", (int)this_player()->query_cols(),
    implode(actions_defined(), "\n")));
  return 1;
} /* local_cmds() */
 
int set_traceprefix(string str) {
  if (this_player(1) != this_object()) return 0;
  if (!str)
    write("Ok, trace prefix cleared.\n");
  else
    write("Ok trace prefix set to "+str+"\n");
  traceprefix(str);
  return 1;
} /* traceprefix() */

int do_trace(string arg) {
  string *args;
  int i, j, trace_arg;
/* ok lets make this clever...
 */
  if (this_player(1) != this_object()) return 0;
  notify_fail("Usage: trace <off|call|call_other|return|args|exec|heart_beat|" +
    "apply|obj_name|number>\n");
  if (!arg) return 0;

  args = explode(arg, " "); 

  if (!sizeof(args)) return 0;

  for (i=0;i<sizeof(args);i++)
    if ((j=member_array(args[i], ({ "off", "call", "call_other", "return",
                                    "args", "exec", "heart_beat", "apply",
                                     }))) != -1)
      if (!j) {
        trace(0);
        write("Trace off.\n");
        return 1;
      } else
        trace_arg |= ({ 0, 1, 2, 4, 8, 16, 32, 64, 128 })[j];
    else
      if (sscanf(args[i], "%d", j))
        trace_arg |= j;
  trace(trace_arg|128);
  write("Trace level : "+trace_arg+"\n");
  return 1;
} /* do_trace() */

static int do_gauge(string str) {
  int eval_cost;

  if (this_player(1) != this_object()) return 0;
  if(!str) {
    notify_fail("Usage : gauge command.\n");
    return 0;
  }
  eval_cost = command(str);
  write("\nThe command '" + str + "' took: " + eval_cost + " CPU cycles.\n");
  return 1;
} /* do_gauge() */

static mapping blue;

int stats_sort_array(string str, string str2) {
  if (blue[str]["moves"] > blue[str2]["moves"])
    return 1;
  if (blue[str]["moves"] < blue[str2]["moves"])
    return -1;
  return 0;
} /* stats_sort_array() */

int stats_filter_array(string ind) {
  return blue[ind]["moves"]+blue[ind]["heart_beats"]+blue[ind]["worth"]+
         blue[ind]["array_size"]+blue[ind]["errors"]+blue[ind]["objects"];
} /* stats_filter_array() */

string string_stats(mapping map) {
  string *inds, str;
  int i;

  str = sprintf("%20-s %7|s %7|s %7|s %7|s %7|s %7|s\n",
           "name", "moves", "hbs", "worth", "arr", "errors", "objects");
  inds = keys(map);
  blue = map;
  inds = sort_array(filter_array(inds, "stats_filter_array", this_object()),
                    "stats_sort_array", this_object());
  blue = ([ ]);
  for (i=0;i<sizeof(inds);i++) {
    str += sprintf("%20-s %7|d %7|d %7|d %7|d %7|d %7|d\n", inds[i],
                   map[inds[i]]["moves"],
                   map[inds[i]]["heart_beats"], map[inds[i]]["worth"],
                   map[inds[i]]["array_size"], map[inds[i]]["errors"],
                   map[inds[i]]["objects"]);
  }
  return str;
} /* print_stats() */

int do_domain(string str) {
  mapping bit;

  if (str)
    bit = ([ str: domain_stats(str) ]);
  else
    bit = domain_stats();
  this_object()->more_string(string_stats(bit));
  return 1;
} /* do_author() */

int do_author(string str) {
  mapping bit;

  if (str)
    bit = ([ str: author_stats(str) ]);
  else
    bit = author_stats();
  this_object()->more_string(string_stats(bit));
  return 1;
} /* do_author() */

int do_malloc(string str) {
  malloc_status();
  return 1;
} /* do_malloc() */

int do_status(string str) {
  if (str)
    mud_status(1);
  else
    mud_status();
  return 1;
} /* do_status() */

int do_fds() {
  dump_file_descriptors();
  return 1;
} /* do_fds() */

int do_sockets() {
  dump_socket_status();
  return 1;
} /* do_sockets() */

int do_uptime() {
  int tim, num;
  string *bits;

  write("Up for ");
  tim = uptime();
  bits = ({ });
  if (tim > 60*60*24)
    bits += ({ (num=tim/(60*60*24))+" day"+(num==1?"":"s") });
  if (tim > 60*60 && tim%(60*60*24))
    bits += ({ (num=(tim/(60*60))%24)+" hour"+(num==1?"":"s") });
  if (tim > 60 && tim%(60*60))
    bits += ({ (num=(tim/60)%60)+" minute"+(num==1?"":"s") });
  if (tim%60)
    bits += ({ (num=tim%60)+" second"+(num==1?"":"s") });
  if (sizeof(bits) > 1)
    write(implode(bits[0..sizeof(bits)-2], ", ")+" and "+
          bits[sizeof(bits)-1]+".\nMore useless info "+
          query_load_average()+".\n");
  return 1;
} /* do_uptime() */
