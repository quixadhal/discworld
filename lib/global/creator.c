/* this is the creator player object */

inherit "/global/wiz_file_comm";

#define NO_SWAP
#define ECHO_ALLOWED

#ifndef NO_SWAP
static string omsgin, omsgout, ommsgin, ommsgout;
static int oxp, omax_deaths=9, odeaths;
static mixed *oskills;
static mapping oprop;
static mapping ospells;

int look_me(string str) {
  if (!str && environment())
    write(file_name(environment()) + "\n");
  return ::look_me(str);
} /* look_me() */

int do_glance(string str) {
  if (!str && environment())
    write(file_name(environment()) + "\n");
  return ::do_glance(str);
} /* do_glance() */

/* This function is used to try to prevent people logged in as a creator from
 * toying with the bits of them that may be useful as a player.  I am sure
 * there are still holes in it, but they can be removed gradually as creators
 * discover them for you.
 */

static void swap_entryexit() {
  string tmin, tmout, tmmin, tmmout;
  int txp, tdeaths, tmax_deaths;
  mixed tskills, tprop, tspells;

  tmin = msgin;
  tmout = msgout;
  tmmin = mmsgin;
  tmmout = mmsgout;
  msgin = omsgin;
  msgout = omsgout;
  mmsgin = ommsgin;
  mmsgout = ommsgout;
  omsgin = tmin;
  omsgout = tmout;
  ommsgin = tmmin;
  ommsgout = tmmout;
  txp = xp;
  xp = oxp;
  oxp = txp;
  tskills = skills;
  skills = oskills;
  oskills = tskills;
/*
  tprop = map_prop;
  map_prop = oprop;
  oprop = tprop;
 */
  tspells = spells;
  spells = ospells;
  ospells = tspells;
  tdeaths = deaths;
  deaths = odeaths;
  odeaths = tdeaths;
  tmax_deaths = max_deaths;
  max_deaths = omax_deaths;
  omax_deaths = tmax_deaths;
}
#else
void swap_entryexit() {
}
#endif


void move_player_to_start(string bong, int new_pl, string c_name) {
  string temp;

  if(!sscanf(file_name(previous_object()), "/secure/login#%s", temp))
    return 0;
  cat("doc/CREATORNEWS");
  ::move_player_to_start(bong, new_pl, c_name);
  if(query_invis())
    tell_object(this_object(), "===> You are currently INVISIBLE! <===\n");
  swap_entryexit();
#ifndef NO_SWAP
  xp = oxp;
#endif
  add_action("visible", "vis");
  add_action("invisible", "invis");
}

string short(int dark) {
  if (query_invis())
    if(!this_player() || this_player()->query_creator())
      return ::short(dark) + " (invis)";
    else
      return 0;
  else
    return ::short(dark);
}

int visible() {
  if(!query_invis()) {
    notify_fail("You are already visible.\n");
    return 0;
  }
  write("You appear.\n");
  invis = 0;
  return 1;
}

int invisible() {
  if(query_invis()) {
    notify_fail("You are already invisible.\n");
    return 0;
  }
  write("You disappear.\n");
  invis = 1;
  return 1;
}

/* removed for now... pending discusion.
int do_emote(string str) {
  write("You are a creator now.\n");
  return 1;
}
*/

#ifndef ECHO_ALLOWED
int do_echo(string str) {
  write("You are a creator now.\n");
  return 1;
}

int do_echo_to(string str) {
  write("You are a creator now.\n");
  return 1;
}

int do_echo_all(string str) {
  write("You are a creator now.\n");
  return 1;
}

int setmin(string str) {
  write("You are a creator now.\n");
  return 1;
}

int setmout(string str) {
  write("You are a creator now.\n");
  return 1;
}

int setmmin(string str) {
  write("You are a creator now.\n");
  return 1;
}

int setmmout(string str) {
  write("You are a creator now.\n");
 return 1;
}
#endif

void save_me() {
  swap_entryexit();
  ::save_me();
  swap_entryexit();
}

int query_creator() { return 1; }
int query_app_creator() { return app_creator; }
int query_creator_playing() { return 0; }

string query_object_type() {
   if ("/d/independent/master"->query_member(geteuid(this_object()))) return "I";
   else return "C";
}
