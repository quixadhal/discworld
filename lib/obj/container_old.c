#include "move_failures.h"
inherit "/std/liquid";
inherit "/std/basic/close_lock";

#define MAX_INVENT 40
#define C_CLOSED 1
#define C_TRANS 2
#define C_OPAQUE 1

static int full_weight, leak_rate, hb_count;

void set_full_weight(int i) { full_weight = i; }
void set_leak_rate(int i) { leak_rate = i; }
int query_full_weight() { return full_weight; }
int query_leak_rate() { return leak_rate; }

void set_max_weight(int i) {
  if (!max_volume && i)
    max_volume = i*10;
  max_weight = i;
}

void set_max_volume(int i) {
  if (!max_weight && i)
    max_weight = i/10;
  max_volume = i;
}

void create() {
  liquid::create();
  close_lock::create();
  leak_rate = 1000;
}

void init() {
  liquid::init();
  close_lock::init();
}

int query_weight() {
  if (!full_weight)
    return ::query_weight();
  return ::query_weight() + ((loc_weight+(max_weight*volume)/max_volume)
                             *full_weight)/max_weight;
}

int query_weight_left() {
  if (!max_volume)
    return max_weight - loc_weight;
  return max_weight - loc_weight - (max_weight*volume)/max_volume;
}

int query_volume_left() {
  if (!max_weight)
    return max_volume - volume;
  return max_volume - volume - (max_volume*loc_weight)/max_weight;
}

int add_weight(int n) {
  int old, new, amt;

  if (query_weight_left() < n)
    return 0;
  if (!full_weight)
    return ::add_weight(n);
  old = query_weight();
  loc_weight += n;
  new = query_weight();
  if (new != old)
    if (environment() && !environment()->add_weight(new-old)) {
      loc_weight -= n;
      return 0;
    }
  return 1;
}

string pretty_short(int dark) {
  string s;
  if (trans == C_TRANS && closed != C_CLOSED) {
    s = query_multiple_short(all_inventory());
    return this_object()->short(dark)+short_status()+(s!=""?", it contains "+s:s);
  }
  return (string)this_object()->short(dark)+short_status();
}

string pretty_plural() {
  string s;
  if (trans != C_TRANS && closed != C_CLOSED)
    return ::pretty_plural();
  s = query_multiple_short(all_inventory());
  return ::pretty_plural()+short_status()+(s!=""?", they contain "+s:s);
}

int transfer_all_to(object dest) {
  object *ob;
  int i;
 
  ob = all_inventory(this_object());
  for (i=0;i<sizeof(ob);i++)
    ob[i]->move(dest);
  if (first_inventory(this_object())) return 0;
  return 1;
}

/* ok modifyed back to using writes... */
string long(string str, int dark) {
  string ret;
  int bing;

  ret = query_long();
  ret += calc_extra_look();
  if (trans != C_OPAQUE && closed != C_CLOSED)
    ret += query_contents(short(dark)+" contains:\n");
  ret += long_status();

/* the long for the water inside of it */
  if (volume) {
    bing = (volume*8 + max_volume/16)/max_volume;
    if (bing <= 0)
      return ret + "It is slightly wet from "+liquid_name+".\n";
    if (bing >= 8)
      return ret + "It is full to the brim with " + liquid_name+".\n";
    return ret + "It is " + ({ "an eighth", "a quarter",
              "three eighths", "half", "five eighths", "three quarters",
              "seven eighths" })[bing-1]+" full of "+liquid_name+".\n";
  }
  return ret;
}

mixed stats() {
  return liquid::stats() + close_lock::stats() + ({
    ({ "full_weight", query_full_weight(), }),
    ({ "leak_rate", query_leak_rate() }),
  });
}

void heart_beat() {
  int lost, off;

  if (hb_count--) return ;
  hb_count = 10;
  lost = leak_rate;
  if (lost > volume)
    lost = volume;
  off = lost/100;
  if (off > 10)
    off = 10;
  tell_room(environment(),
            query_liquid_name()+({ " dribbles slowly out ",
                               " dribbles out ",
                               " drips out ",
                               " trickles slow out ",
                               " trickles out ",
                               " quickly trickles out ",
                               " slowly pours out ",
                               " pours out ",
                               " streams out ",
                               " gushes out ",
                               " fountains out ", })[off]+"of the "+
                                                          short(1)+".\n");
  volume -= lost;
  if (!volume) {
    set_heart_beat(0);
    set_liquid_name(0);
  }
}

int mix_up(int amt, object ob) {
  object env;

  if (locked) {
    write("Locked!\n");
    return 0;
  }
  if (closed) /* has to be closed */
    if (do_open())
      write("You open the "+short(1)+" when you pour the "+
             ob->query_liquid_name()+" in.\n");
    else {
      write("Closed!\n");
      return 0;
    }
  if (max_volume && full_weight) {
    if (!add_weight((max_weight*volume)/max_volume)) {
      if (!(env = environment())) {
        write("Too heavy!  No environment!\n");
        return 0;
      }
      if (!(env = environment(env))) {
        write("Too heavy!  No environment! (2)\n");
        return 0;
      }
      if (move(env)) {
        tell_room(env, environment()->query_cap_name()+" drops "+
                       short(1)+" when it gets too heavy.\n");
        return mix_up(amt, ob);
      }
      write("Too heavy!  Couldn't drop it! (2)"+
            ((max_weight*volume)/max_volume)+"\n");
      return 0;
    }
    loc_weight -= (max_weight*volume)/max_volume;
  }
  if (leak_rate)
    set_heart_beat(1);
  return ::mix_up(amt, ob);
}

mixed *int_query_auto_load() {
  mixed *tmp;

  tmp = ::int_query_auto_load();
  return ({ sizeof(tmp) }) + tmp + ({
            leak_rate, full_weight,
            locked, stuck, closed, trans, difficulty,
            key, trap_open_func, trap_lock_func, unlock_skill,
            trap_open_ob, trap_lock_ob,
         });
}

void int_init_arg(mixed *args) {
  ::int_init_arg(args[1..args[0]+1]);
  args = args[args[0]+1..1000];
  leak_rate = args[0];
  full_weight = args[1];
  locked = args[2];
  stuck = args[3];
  closed = args[4];
  trans = args[5];
  difficulty = args[6];
  key = args[7];
  trap_open_func = args[8];
  trap_lock_func =args[9];
  unlock_skill = args[10];
  trap_open_ob = args[11];
  trap_lock_ob = args[12];
}

/*
 * Do it all the time for containers...  They could have things in them.
 * too hard to andle rhe special cases you know...
 */
mixed query_auto_load() {
  return int_query_auto_load();
}

void init_arg(mixed *args) {
  int_init_arg(args);
}
