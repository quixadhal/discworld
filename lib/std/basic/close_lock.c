#include "move_failures.h"

#define MAX_INVENT 40
#define C_CLOSED 1
#define C_TRANS 2
#define C_OPAQUE 1

static int locked, stuck;
static int closed, trans, difficulty;
static string key, trap_open_func, trap_lock_func, unlock_skill;
static object trap_open_ob, trap_lock_ob;

void set_open() { closed &= ~C_CLOSED; }
void set_closed() { closed |= C_CLOSED; }
void set_transperant() { trans = C_TRANS; }
void reset_transperant() { trans = ~C_TRANS; }
int query_closed() { return closed; }
int query_transperant() { return trans; }

void create() {
  difficulty = 200;
  unlock_skill = "other.covert.lockpick";
}

void set_open_trap(object ob, string func) {
  trap_open_func =  func;
  trap_open_ob = ob;
}

void set_unlock_trap(object ob, string func) {
  trap_lock_func = func;
  trap_lock_ob = ob;
}

void set_key(string str) {
  key = str;
}

/* if something is stuck... This means that it cannot open, close, lock
 * or unlock.
 */
void set_stuck(int i) {
  stuck = i;
}

void set_locked() {
  locked = 1;
}

void set_unlocked() {
  locked = 0;
}

string query_open_trap_func() { return trap_open_func; }
object query_open_trap_ob() { return trap_open_ob; }
string query_lock_trap_func() { return trap_lock_func; }
object query_lock_trap_ob() { return trap_lock_ob; }
string query_key() { return key; }
int query_stuck() { return stuck; }
int query_locked() { return locked; }
int query_unlocked() { return !locked; }

void init() {
  this_player()->add_command("open", this_object());
  this_player()->add_command("close", this_object());
  if (key) {
    this_player()->add_command("lock", this_object());
    this_player()->add_command("unlock", this_object());
  }
}

string short_status() {
  return  (locked?" (locked)":"")+(closed?" (closed)":"");
}

/* ok modifyed back to using writes... */
string long_status() {
  if (closed)
    if (locked)
      return "It is closed and locked.\n";
    else
      return "It is closed.\n";
  return "";
}

int test_remove(object ob, int flag) { return !closed; }
int test_add(object ob, int flag) { return !closed; }

int do_unlock() {
  int ok, lvl;
  object *obs;

  if (!locked || stuck || !closed || !key)
    return 0;
  if (sizeof((obs = find_match(key, this_player()))) ||
      sizeof((obs = find_match("master key", this_player())))) {
    obs[0]->use_key(this_object());
    ok = 1;
  } else {
    lvl = (int)this_player()->query_skill_bonus(unlock_skill);
    if (random(lvl) <= lvl-difficulty)
      ok = 1;
    else 
      ok = 0;
  }
  if (trap_lock_func && trap_lock_ob)
    if (!call_other(trap_lock_ob, trap_lock_func, "unlock", ok))
      return 0;
  set_unlocked();
  return 1;
}

int do_lock() {
  int ok, lvl;
  object *obs;

  if (locked || stuck || !closed || !key)
    return 0;
  if (sizeof((obs = find_match(key, this_player()))) ||
      sizeof((obs = find_match("master key", this_player())))) {
    obs[0]->use_key(this_object());
    ok = 1;
  } else {
    lvl = (int)this_player()->query_skill_bonus(unlock_skill);
    if (random(lvl) <= lvl-difficulty)
      ok = 1;
    else 
      ok = 0;
  }
  if (trap_lock_func && trap_lock_ob)
    if (!call_other(trap_lock_ob, trap_lock_func, "lock", ok))
      return 0;
  set_locked();
  return 1;
}

int do_open() {
  if (closed != C_CLOSED || stuck)
    return 0;
  if (locked)
    if (!do_unlock())
      return 0;
  if (trap_open_func && trap_open_ob)
    if (!call_other(trap_open_ob, trap_open_func, "open"))
      return 0;
  set_open();
  return 1;
}

int do_close() {
  if (closed == C_CLOSED || stuck)
    return 1;
  if (trap_open_func && trap_open_ob)
    if (!call_other(trap_open_ob, trap_open_func, "close"))
      return 0;
  set_closed();
  return 1;
}

mixed *stats() {
  return ({
    ({ "closed", query_closed() }),
    ({ "transperant", query_transperant() }),
    ({ "key", query_key() }),
    ({ "locked", query_locked() }),
    ({ "stuck", query_stuck() }),
    ({ "open traped", trap_open_func != 0 }),
    ({ "lock traped", trap_lock_func != 0 }),
  });
}
