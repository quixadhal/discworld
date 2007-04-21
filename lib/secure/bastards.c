/*  -*- LPC -*-  */
/*
 * $Id: bastards.c,v 1.33 2003/08/12 22:56:19 ceres Exp $
 */
#include <access.h>
#include <mail.h>
#include <player_handler.h>
#include <login.h>
#include <playerinfo.h>
#include <playtesters.h>

// 2419200 == 28 days
#define TIMEOUT_TIME 2419200

/*
 * New player banning changed by Pinkfish June 1996.
 *
 * Changed to make the site access stuff timeout. Pinkfish July 1997.
 */

nosave string *names;

mapping site_access;  /* Site access control */
mapping suspended;
mapping lockedout;
mapping multiuser;
mapping temp_passwords;

int query_access(object ob);
protected void timeout_access();
private void save_me();

void create() {
  seteuid("Root");
  site_access = ([ ]);
  suspended = ([ ]);
  lockedout = ([ ]);
  multiuser = ([ ]);
  temp_passwords = ([ ]);
  
  if(!unguarded((: restore_object, base_name(this_object()) :)))
    write("Failed to restore bastards.\n");
  if (!site_access) {
    write("mmm, no site access\n");
    site_access = ([ ]);
  }
  if(!lockedout)
    lockedout = ([ ]);
  if(!multiuser)
    multiuser = ([ ]);
  if(!temp_passwords) {
    temp_passwords = ([ ]);
  }
  timeout_access();
  
  if (find_object("/room/void"))
    move_object(find_object("/room/void"));
}

string query_player_ob(string name) {
  if("/secure/master"->query_administrator(name) ||
     "/secure/master"->query_lord(name))
    return "/global/lord";
  
  if (PLAYER_HANDLER->test_creator(name))
    return "/global/creator";

  if (PLAYTESTER_HAND->query_playtester(name))
    return "/global/playtester";

  return "/global/player";
}

int query_prevent_shadow() { return 1; }

/*
 * Ok, acces checking stuff...
 */
mapping query_all_access() { return copy(site_access); }

/*
 * Look up the address and find out if it is nice and floppy
 * Adress is of the format ({ "130", "95", "100", "2" })
 */
int query_access(object ob) {
  string tmp;
  
  tmp = query_ip_number(ob);
  while(strlen(tmp)) {
    if(site_access[tmp])
      return site_access[tmp][ACCESS_LEVEL];
    tmp = implode((string *)explode(tmp, ".")[0..<2], ".");
  }
  
  tmp = query_ip_name(ob);
  while(strlen(tmp)) {
    if(site_access[tmp])
      return site_access[tmp][ACCESS_LEVEL];
    tmp = implode(explode(tmp, ".")[1..], ".");
  }

  return DEFAULT;
}

int query_multi(object ob) {
  string tmp;
  
  tmp = query_ip_number(ob);
  while(strlen(tmp)) {
    if(multiuser[tmp])
      return 1;
    tmp = implode((string *)explode(tmp, ".")[0..<2], ".");
  }
  
  tmp = query_ip_name(ob);
  while(strlen(tmp)) {
    if(multiuser[tmp])
      return 1;
    tmp = implode(explode(tmp, ".")[1..], ".");
  }

  return 0;
}

mapping query_all_multi() { return copy(multiuser); }

string query_reason(string address) {
  if(site_access[address])
    return site_access[address][ACCESS_REASON];
  return 0;
}

protected void add_multi(string address, int multi, int timeout) {
  if(!multi)
    map_delete(multiuser, address);
  else
    multiuser[address] = timeout;
}

protected void add_access(string address, int level, string reason,
                          int timeout) {
  site_access[address] = ({ level, reason, timeout });
}

/*
 * Runs through the array nibbling all the timed out bits...
 */
protected void timeout_access() {
  string bit;
  
  foreach(bit in keys(site_access))
    if(site_access[bit][ACCESS_TIMEOUT] < time() ||
       site_access[bit][ACCESS_LEVEL] == ACCESS)
      map_delete(site_access, bit);

  foreach(bit in keys(multiuser))
    if(multiuser[bit] < time())
      map_delete(multiuser, bit);
}

private void save_me() {
  unguarded((: save_object, base_name(this_object()), 2 :));
} 

int change_access(string address, int level, string reason, int timeout) {
  if(!master()->query_lord(previous_object(-1)) &&
     base_name(previous_object()) != "/cmds/creator/ban")
    return 0;
  
  if(!stringp(address) || !reason)
    return notify_fail("Invalid parameters.\n");
  
  if (!timeout)
    timeout = time() + 100 * 24 * 60 * 60;

  add_access(address, level, reason, timeout);
  save_me();
  unguarded((: write_file, "/log/ACCESS", ctime(time())[4..16] + " " + 
             address+" set to " + PERM_NAMES[level] + " for " + reason +
             " until " + ctime(timeout) + " by "+
             this_player()->query_name()+".\n" :));
  return 1;
}

int change_multi(string address, int multi, int timeout) {
  if(!master()->query_lord(previous_object(-1)) &&
     base_name(previous_object()) != "/cmds/creator/multipl_ayer")
    return 0;
  
  if(!stringp(address))
    return notify_fail("Invalid parameters.\n");
  
  if (!timeout)
    timeout = time() + 100 * 24 * 60 * 60;

  add_multi(address, multi, timeout);
  save_me();
  
  unguarded((: write_file, "/log/ACCESS", ctime(time())[4..16] + " " + 
             address+" set to " + (multi?"allow":"disallow") + 
             " multiple users until " + ctime(timeout) + " by "+
             this_player()->query_name()+".\n" :));
  return 1;
}

int suspend_person(string str, int tim, string reason) {
  if (!master()->query_lord(previous_object(-1)) &&
      base_name(previous_object()) != "/cmds/creator/suspend") {
    return 0;
  }
  if (!PLAYER_HANDLER->test_user(str)) {
    return 0;
  }
  
  if (!reason) {
    reason = "you have been bad";
  }

  suspended[str] = ({ time() + tim, reason });
  save_me();
  unguarded((: write_file, "/log/SUSPEND", ctime(time())[4..16] + " " +
             str+" suspended until "+ ctime(time()+tim)+" by "+
             this_player()->query_name()+" because "+ reason + ".\n" :));
  PLAYERINFO_HANDLER->add_entry(this_player(), str, "suspend",
                                "Suspended until " + ctime(time() + tim) +
                                " for " + reason);
  return 1;
}

int unsuspend_person(string str) {
  if (!"/secure/master"->query_lord(previous_object(-1)) &&
      base_name(previous_object()) !="/cmds/creator/unsuspend") {
    return 0;
  }
  map_delete(suspended, str);
  unguarded((: save_object, base_name(this_object()), 2 :));
  unguarded((: write_file, "/log/SUSPEND",
             ctime(time())[4..16] + " " + str+" unsuspended.\n" :));
  PLAYERINFO_HANDLER->add_entry(this_player(), str, "suspend", "Unsuspended.");
  return 1;
}

mixed *query_suspended(string name) {
  if (suspended[name]) {
    if (suspended[name][SUSPEND_TIME] > time()) {
      return copy(suspended[name]);
    }
    map_delete(suspended, name);
    save_me();
  }
  return suspended[name];
}

int lockout_person(string str, int tim, string reason) {
  if (!master()->query_lord(previous_object(-1)) &&
      base_name(previous_object()) != "/cmds/player/lockout") {
    return 0;
  }
  if (!PLAYER_HANDLER->test_user(str)) {
    return 0;
  }
  
  if (!reason) {
    reason = "you have been bad";
  }

  lockedout[str] = ({ time() + tim, reason });
  save_me();
  unguarded((: write_file, "/log/LOCKOUT",
             ctime(time())[4..16] + " " + str+" lockedout until "+
             ctime(time()+tim)+" by "+this_player()->query_name()+" because "+
             reason + ".\n" :));
  PLAYERINFO_HANDLER->add_entry(this_player(), str, "lockedout",
                                "Locked out until " + ctime(time() + tim) +
                                " for " + reason);
  return 1;
}

mixed *query_lockedout(string name) {
  if (lockedout[name]) {
    if (lockedout[name][SUSPEND_TIME] > time()) {
      return copy(lockedout[name]);
    }
    map_delete(lockedout, name);
    save_me();
  }
  return lockedout[name];
}

int set_temp_password(string name, string pass) {
  temp_passwords[name] = ({ time(), pass });
  save_me();
  return 1;
}

mixed *query_temp_password(string name) {
  int found;
  string tmp;
  
  foreach(tmp in keys(temp_passwords)) {
    if(temp_passwords[tmp][0] < (time() - TIMEOUT_TIME)) {
      map_delete(temp_passwords, tmp);
      found = 1;
    }
  }
  if(found)
    save_me();
  
  return copy(temp_passwords[name]);
}

void clear_temp_password(string name) {
  if(temp_passwords[name]) {
    map_delete(temp_passwords, name);
    save_me();
  }
}
