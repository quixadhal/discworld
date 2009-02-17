/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: master.c,v 1.22 2003/03/25 20:30:29 ceres Exp $
 */
inherit "/secure/master/directory_assignments";

#define ROOT "Root"

#include <log.h>
#include <player_handler.h>

//#define TRUSTEES ([ ROOT : 1, "Admin" : 1, "offler" : 1, \
//                     "pinkfish"  : 1, "ember"     : 1, \
//                     "ceres"     : 1, "turrican"  : 1, \
//                     "brandobas" : 1, "sojan"     : 1, \
//                     "hobbes"    : 1, "wodan"     : 1, \
//                     "taffyd"    : 1,])

#define TRUSTEES ([ ROOT : 1, "cratylus" : 1,])

#define READ_MASK 1
#define WRITE_MASK 2
#define GRANT_MASK 4
#define LOCK_MASK 8

#define SENIOR 4            /* Independents */
#define DIRECTOR 1            /* Lords */
#define TRUSTEE 2     /* High Lords */

private mapping positions, permissions;
private nosave  mapping trustees, checked_master, snoop_list;
private nosave  object unguarded_ob;

protected void create() {
  set_eval_limit(2000000);
  permissions = ([ ]);
  positions   = ([ ]);
  checked_master = ([ ]);
  snoop_list = ([ ]);
  unguarded_ob = 0;
  trustees = TRUSTEES;
  if (!unguarded((: restore_object, "/secure/master" :)))
    if (!unguarded((: restore_object, "/secure/config/master_fallback" :)))
      error("The master object couldn't restore its save file.");
} /* create() */

/** @ignore yes */
string query_name() { return "Root"; }

/**
 * This function is called every time a player connects.
 * input_to() cannot be called from here.
 */
object connect(int port) {
  object ob;

  if (!find_object("/secure/login")) {
    log_file("REBOOT", "Mud rebooted at "+ctime(time())+"["+time()+"]"+"\n");
  }
 
#ifdef __VERSION__
#  define VERSION __VERSION__
#endif

  printf("LPmud version : %s on port %d.", VERSION, port);
  switch (port) {
    case 4243 :
      ob = clone_object("/secure/nlogin");
      if(!ob)
        destruct(this_object());
      break;
    default :
      ob = clone_object("/secure/login");
      break;
  }
  ob->set_login_port(port);
  printf("\n");
  return ob;
} /* connet() */

/**
 * This method checks to see if the person is a trustee or not.
 * @param str the person to check for being a trustee
 * @return 1 if they are, 0 if they are not
 */
int query_trustee(mixed str) {
  if (pointerp(str)) {
    str = filter(str, (: interactive($1) :));

    return sizeof(filter((object *)str,
                         (: geteuid($1) == ROOT ||
                          positions[geteuid($1)] == TRUSTEE ||
                          trustees[geteuid($1)] :))) == sizeof(str);
  }
  return ((str == ROOT) || (positions[str] == TRUSTEE) ||
          (trustees[str]));
}

/** @ignore yes */
int query_administrator(mixed str) {
  return query_trustee(str);
}

/** @ignore yes */
int high_programmer(mixed str) {
  return query_trustee(str);
}

/**
 * This method checks to seee if the person is a director or not.
 * @param arg the person to check
 * @return 1 if they are, 0 if they are not
 */
int query_director( mixed arg ) {
  if ( pointerp( arg ) ) {
    arg = filter(arg, (: interactive($1) :));

    return sizeof( filter( (object *)arg,
                           (: ( positions[ geteuid( $1 ) ] == DIRECTOR ) ||
            query_trustee( geteuid( $1 ) ) :) ) ) == sizeof( arg );
  }
  return ( ( positions[ arg ] == DIRECTOR ) || query_trustee( arg ) );
}

/** @ignore */
int query_leader( mixed arg ) { return query_director( arg ); }
/** @ignore */
int query_lord( mixed arg ) { return query_director( arg ); }

/**
 * This method checks to see if they are only a director.  The
 * query_director method also returns true if they are a trustee.
 * @return 1 if they are only a director
 */
int query_only_director( string word ) {
   return positions[ word ] == DIRECTOR;
} /* query_only_leader() */

/** @ignore yes */
int query_only_leader( string word ) { return query_only_director( word ); }
/** @ignore yes */
int query_only_lord( string word ) { return query_only_director( word ); }

/**
 * This method returns the array of directors for the mud.
 * @return the array of directors
 */
string *query_directors() {
  return filter_array(keys(positions),
                      "query_only_director", this_object());
} /* query_leaders() */

/** @ignore yes */
string *query_leaders() { return query_directors(); }
/** @ignore yes */
string *query_lords() { return query_directors(); }

/** @ignore yes */
int query_player_trustee(string str) {
  return query_trustee(str) && PLAYER_HANDLER->test_user(str);
}

/** @ignore yes */
int query_player_administrator(string str) {
  return query_player_trustee(str);
}
/** @ignore yes */
int query_player_high_lord(string str) {
  return query_player_trustee(str);
}

/** @ignore yes */
string *high_programmers() { return keys( trustees ); }
/** @ignore yes */
string *query_administrators() { return keys( trustees ); }

/**
 * This method returns the current trustees of the mud.
 */
string *query_trustees() { return keys( trustees ); }

/**
 * This method returns all the directors of the mud.
 * @return all the directors
 */
string *query_all_directors() {
  return filter_array(keys(positions), "query_director", this_object() );
}

/** @ignore yes */
string *query_all_leaders() { return query_all_directors(); }
/** @ignore yes */
string *query_all_lords() { return query_all_directors(); }

/**
 * This method checks to see if the specified person is the leader of the
 * given domain or not.
 * @param person the person to check
 * @param domain the domain to check
 * @return 1 if they are the leader, 0 if not
 */
int is_leader_of(string person, string domain) {
  return ("/d/" + domain + "/master")->query_lord() == person;
}

/**
 * This method checks to see if the specified person is a deputy of the
 * given domain or not.
 * @param person the person to check
 * @param domain the domain to check
 * @return 1 if they are a deputy, 0 if not
 */
int is_deputy_of(string person, string domain) {
   return ("/d/" + domain + "/master")->query_deputy(person);
} /* is_deputy_of() */

/**
 * This method checks to see if the person is a liason deputy or not.
 * @param person the person to check
 * @return 1 if they a deputy, 0 if not
 */
int is_liaison_deputy(string person) {
   return "/d/liaison/master"->query_deputy(person);
} /* is_liaison_deputy() */

/**
 * This checks to see if all the objects are either liaison deputies or
 * directors and above.
 * @param arg a person or an array of objects
 * @return 1 if true
 */
int query_liaison_deputy_or_director(mixed arg) {
  if ( pointerp( arg ) ) {
    arg = filter(arg, (: interactive($1) :));

    return sizeof( filter( (object *)arg,
                           (: query_director(geteuid( $1 )) ||
            is_liaison_deputy( geteuid( $1 ) ) :) ) ) == sizeof( arg );
  }
  return ( is_liaison_deputy(arg) || query_director( arg ) );
}

/** @ignore yes */
int query_liaison_deputy_or_lord(mixed arg) {
  return query_liaison_deputy_or_director(arg);
}

/**
 * This method checks to see if the specified person is a senior or not.
 * This returns true if they are also a director as well.
 * @return 1 if they are a senior or higher
 */
int query_senior( mixed arg ) {
   if ( pointerp( arg ) )
      return sizeof(filter( (object *)arg,
            (: ( positions[ geteuid( $1 ) ] == SENIOR ) ||
            query_leader( geteuid( $1 ) ) :) ) ) == sizeof( arg );
   return ( ( positions[ arg ] == SENIOR ) || query_leader( arg ) );
}

/**
 * This method returns a list of all the seniors.
 * @return all of the seniors
 */
string *query_all_seniors() {
   return filter_array( keys( positions ), "query_senior", this_object() );
} /* query_all_seniors() */

/**
 * Return a list of the creator domains.
 *
 * @return a string array of domain names.
 */
string *query_domains() {
  string *domains;
  domains = (get_dir("/d/") - ({ "lost+found", "core" }));
  return filter(domains, (: $1[<4..] != "_dev" :));
}

/**
 * This method checks to see if the specified object is allowed to be
 * loaded.
 * @return 1 if it is
 */
int valid_load(string path, mixed euid, string func) { return 1; }


/**
 * This returns the uid to be used as root.
 * @return the root uid
 */
string get_root_uid() { return ROOT; }
/**
 * This method returns the other defatul uid.
 * @return the bb uid
 */
string get_bb_uid() { return "Room"; }

/**
 * This method returns the include directories to be used in the mud.
 * @return the list of include directories
 */
string *define_include_dirs() {
  return ({ "/include/%s" });
} /* define_include_dirs() */

/**
 * This method returns the status of allowing the trace function to work.
 * @return 1 if it is allowed, 0 if not
 */
int valid_trace() { return 1; }

/**
 * This method shuts down the mud in a specified number of minutes.
 * @param min the number of minutes for the shutdown
 */
void shut(int min) {
  "/obj/shut"->shut(min);
} /* shut() */

/**
 * This removes the master object from the list of checked masters.  This
 * stops the system for going crazy with runtimes if one of the domain masters
 * fails to load.
 * @param name the name to remove
 */
void remove_checked_master(string name) {
  map_delete(checked_master, name);
} /* remove_checked_master() */

/**
 * This method returns the current mapping of checked master objects.
 * @return the mapping of checked master objects
 */
mapping query_checked_master() { return checked_master; }

/* This function comes from the Nightmare master object. */

/**
 * This is used by the unguarded() simul efun to make an unguarded security
 * call.
 * @param f the function to call
 * @param local local to the master or not
 * @return the return value
 */
varargs mixed apply_unguarded(function f, int local) {
  object previous_unguarded;
  string err;
  mixed val;

  if (base_name(previous_object(0)) != "/secure/simul_efun") {
    error("Illegal unguarded apply.");
    return 0;
  }
  previous_unguarded = unguarded_ob;
  if (local)
    unguarded_ob = master();
  else
    unguarded_ob = previous_object(1);
  err = catch(val = (mixed)(*f)());
  unguarded_ob = previous_unguarded;
  if (err) {
    error(err);
  }
  return val;
}

#include "/secure/master/permission.c"
#include "/secure/master/crash.c"
#include "/secure/master/create_dom_creator.c"
#include "/secure/master/creator_file.c"
#include "/secure/master/dest_env.c"
#include "/secure/master/ed_stuff.c"
#include "/secure/master/file_exists.c"
#include "/secure/master/logging.c"
#include "/secure/master/parse_command.c"
#include "/secure/master/preload.c"
#include "/secure/master/query_pl_level.c"
#include "/secure/master/simul_efun.c"
#include "/secure/master/snoop.c"
#include "/secure/master/valid_database.c"
#include "/secure/master/valid_exec.c"
#include "/secure/master/valid_hide.c"
#include "/secure/master/valid_ident.c"
#include "/secure/master/valid_link.c"
#include "/secure/master/valid_override.c"
#include "/secure/master/valid_read.c"
#include "/secure/master/valid_seteuid.c"
#include "/secure/master/valid_shadow.c"
#include "/secure/master/valid_socket.c"
#include "/secure/master/valid_write.c"
#include "/secure/master/valid_copy.c"
#include "/secure/master/virtual_objects.c"
#include "/secure/master/valid_bind.c"
#include "/secure/master/valid_to_c.c"
#include "/secure/master/valid_binary.c"
