/*  -*- LPC -*-  */
/*
 * $Locker: ceres $
 * $Id: login_handler.c,v 1.35 2003/03/30 19:23:34 ceres Exp ceres $
 */
/**
 * The login handler lets you have a funct called as a player logs in or out.
 * The funct you setup will be called when the player logs in or out.  The
 * object will also give you a list of all the people who are logged in. This is
 * a by product of its method of operation rather than anything important. 
 * It is quite possibly more useful to call users().
 * <p>
 * The funct which is to be called only needs to be called once as
 * the objects state is saved.  When a player enters, logsout or goes
 * net dead the funct will be called on the specified object.  The
 * funct will have two parameters, the first will be the player
 * name logging in/out and the last will be a define from /include/login.h
 * specifying which sort of event has occured.
 * <p>
 * A new temporary method of adding a call back to the login_handler has been
 * added.  This will allow you to add a call that is not saved,
 * so you can use an object reference instead of a file name.
 * @author Furball
 * @see /secure/login
 * @see efun::users()
 * @see /include/login.h
 */
#include <login.h>

// #define LOGIN_DEBUG 1 

#ifdef LOGIN_DEBUG
#undef MAX_COMPRESSED_PLAYERS
#undef MAX_UNCOMPRESSED_PLAYERS
#undef MAX_PLAYERS

#define LOGIN_MAX_COMPRESSED_PLAYERS 2
#define MAX_UNCOMPRESSED_PLAYERS 1
#define MAX_PLAYERS (MAX_COMPRESSED_PLAYERS + MAX_UNCOMPRESSED_PLAYERS)
#endif

#ifdef LOGIN_DEBUG
#define is_compressed_login(x) (strsrch((x)->query_name(), "test") > -1)
#else
#define is_compressed_login(x) (compressedp(x))
#endif

#define COMPRESSED_INTAKE_COUNT 5
#ifdef USE_RAMDISK
#define SAVE_TIME 300
#else
#define SAVE_TIME 900
#endif

#define NEW_SAVE_TIME 15
#define SAVE_NAME "/save/login_handler"

#define BUFFER_TIME 30

#define LOCKDOWN_TIME 900
#define MAX_FAILURES 3

class login_information {
    /*
     *When saved, this can only be a string.  If not it can be an object of
     * funct pointer.
     */
    mixed ob;
    /* This is not used with a funct pointer */
    string funct;
}

mapping _static_login_calls;

nosave mapping _dynamic_login_calls;
nosave string _best;
nosave string _worst;

nosave string *_players_on;

nosave mapping _compressed_players_on;
nosave mapping _telnet_players_on;

// nosave object *_login_queue;
nosave object *_compressed_login_queue;
nosave object *_telnet_login_queue;
nosave int _compressed_login_count;


nosave string enter_log;
nosave int last_entry;
nosave mapping messages;
nosave mapping lockdowns;

nosave int _creators;
nosave int _players;
nosave int _login_q;
nosave int _next_user_log;
nosave int _title_call;
nosave int _save_call;
nosave int _out_q;
nosave int _q_time;

nosave int _last_queue_time;
nosave int _last_queue_result;

int is_discworld_full(int test_login_queue);
void write_log(string event, string name, int guest, string other);
void save_users();
private void store_user_count();
private void log_user_count();
protected void check_all_titles( string player );
private void do_all_calls(string player, string event);
private void do_call(class login_information login_inf,
  string player, int event);
int is_uncompressed_full( int );

void create() {
    object pl;
    string name;
    
    _static_login_calls = ([ ]);
    _dynamic_login_calls = ([ ]);

    seteuid( "Root" );
    unguarded((: restore_object, SAVE_NAME :));

    foreach(name in keys(_static_login_calls)) {
      if(!sizeof(_static_login_calls[name])) {
        map_delete(_static_login_calls, name);
      }
      catch(unguarded((: save_object, SAVE_NAME :)));
    }

    _compressed_players_on = ([ ]);
    _telnet_players_on = ([ ]);
    _players_on = ({ });

    foreach(pl in users()) {
        if (is_compressed_login(pl)) {
            _compressed_players_on[ pl->query_name() ] = 1;
        }
        else {
            _telnet_players_on[ pl->query_name() ] = 1;
        }

        _players_on += ({ pl->query_name() }); 
    }

    _compressed_login_queue = ({ });
    _telnet_login_queue = ({ }); 
    _compressed_login_count = 1;

    _save_call = call_out( "save_users", NEW_SAVE_TIME );
    _next_user_log = time() + 3601 - (time() % 3600);
    enter_log = "";
    last_entry = time();
    call_out("clean_lockdowns", 3600);
    write_log("Periodic", "", 0, "");
} /* create() */

/** @ignore yes */
mapping query_static_login_calls() {
    return copy(_static_login_calls);
} /* query_static_login_calls() */
/** @ignore yes */
mapping query_dynamic_login_calls() {
    return copy(_dynamic_login_calls);
} /* query_dynamic_login_calls() */

/**
 * This returns the player name with the best alignment.
 * @return player name with the best alignment
 */
string query_best() { return _best; }
/**
 * This returns the player name with the worst alignment.
 * @return player name with the worst alignment
 */
string query_worst() { return _worst; }
/**
 * This method returns a list of the current players who are
 * online.
 * @return the string names of the online players
 */
string *query_players_on() { 
    return keys( _compressed_players_on ) + keys( _telnet_players_on );
}
/**
 * This method returns the current list of people in the login
 * queue.
 * @return the objects in the login queue
 */
object *query_login_queue() { 
    return _compressed_login_queue + _telnet_login_queue; 
} /* query_login_queue() */ 

/**
 * This method adds an object to the login queue.  This is called from
 * inside /secure/login.c
 * @see /secure/login.c
 * @param login_ob the object to add to the queue
 */
void add_to_login_queue(object login_ob) {
    if ( is_compressed_login( login_ob ) ) {
        _compressed_login_queue += ({ login_ob }); 
    }
    else {
        _telnet_login_queue += ({ login_ob });
    }
} /* add_to_login_queue() */

/**
 * This method removes an object from the login queue, if they log out
 * or something.
 * @param ob the object to remove from the queue
 */
void remove_from_login_queue(object ob) {
    _compressed_login_queue -= ({ ob });
    _telnet_login_queue -= ({ ob });
} /* remove_from_login_queue() */

int can_jump_queue( object ob ) {
    return ob->query_creator() || ob->query_net_dead() || 
        ob->query_property( "test character" );
} /* can_jump_queue() */ 
/**
 * This method returns the top element of the login queue and removes
 * them from it.
 * @return the top element of the login queue, 0 if none
 */
object pop_from_login_queue() {
    object ob;
    object *login_queue;

    login_queue = query_login_queue();
    
    if ( !sizeof( login_queue ) ) {
        return 0;
    }
    
    // This keeps track of how many players have recently left, adding
    // them to the total players count so that lots of players don't
    // arrive at the same time causing lag.

    if(time() != _q_time){
      _q_time = time();
      _out_q = 0;
    }

    _out_q++;

    // tell_creator( "taffyd", "Checking for queue jumpers...\n" );
    
    // Look for our queue jumpers in both the compressed login queues
    // and the telnet login queues.
    foreach( ob in _compressed_login_queue ) { 
        if ( can_jump_queue( ob ) ) {
            remove_from_login_queue( ob );
            return ob; 
        }        
    }
    // The is_discworld_full seems to over evaluate :(
    reset_eval_cost();
    if(sizeof(_telnet_login_queue) &&
       !is_uncompressed_full(WITHOUT_LOGIN_QUEUE)) {
      foreach( ob in _telnet_login_queue ) { 
        if ( can_jump_queue( ob ) ) {
          remove_from_login_queue( ob );
          return ob; 
        }        
      }
    }
    
    // If we've taken less than COMPRESSED_INTAKE_COUNT of 
    // compressed logins, then take one from the compressed queue.

    if ( sizeof( _compressed_login_queue) &&
         _compressed_login_count < COMPRESSED_INTAKE_COUNT ) {
        _compressed_login_count++;
        ob = _compressed_login_queue[ 0 ]; 
        _compressed_login_queue = _compressed_login_queue[ 1.. ];
    } else {
        _compressed_login_count = 0;
        ob = _telnet_login_queue[ 0 ]; 
        _telnet_login_queue = _telnet_login_queue[ 1.. ]; 
    }
    
    // tell_creator( "taffyd", "Removed %O from the queue.\n", ob );

    return ob;
} /* pop_from_login_queue() */


/**
 * This method adds a funct to be called whenever a player logs
 * in.  The funct and object both need to be strings so that
 * they are saved correctly.  This will be saved to the save file
 * and restored over logins.
 * <p>
 * The function will be called with two parameters, the first is the
 * name of the player and the second is the type of the event.  The
 * event types are defined in /include/login.h  The event will be one
 * of RECONNECT, LOGIN, LOGOUT, NETDEATH.
 * @param player specfic player to look for "all" for everyone
 * @param func the funct to call
 * @param obj the object to call the funct on
 * @return 0 on failure and 1 on success
 * @see remove_static_login_call()
 * @see add_dynamic_login_call()
 */
int add_static_login_call(string player, mixed funct, mixed obj) {
  class login_information login_inf;

  if (!stringp(funct) || !stringp(obj) || !stringp(player)) {
    return 0;
  }

  // Don't allow calls to creators own files. They invariably get left
  // behind well after their usefulness is over and cause runtime errors.
  if(obj[0..3] == "/w/")
    return 0;
  
  // Check for dups, we get too many of those.
  if(_static_login_calls[player]) {
    foreach (login_inf in _static_login_calls[player]) {
      if (login_inf->ob == obj &&
    login_inf->funct == funct) {
  return 1;
      }
    }
  }
  
  if (!_static_login_calls[player]) {
    _static_login_calls[player] = ({ });
  }

  login_inf = new (class login_information, funct : funct, ob : obj);
  _static_login_calls[player] += ({ login_inf });
  
  catch(unguarded((: save_object, SAVE_NAME :)));
  return 1;
} /* add_static_login_call() */

/**
 * This method removes a login call.
 * @param player the player specifier
 * @param funct the funct name
 * @param obj the object to call the funct on
 * @return 0 on failure and 1 on success
 * @see add_static_login_call()
 * @see add_dynamic_login_call()
 */
int remove_static_login_call(string player, mixed funct, mixed obj) {
  class login_information login_inf;
  
  if (!stringp(funct) || !stringp(obj) || !stringp(player)) {
    return 0;
  }
  
  if (_static_login_calls[player]) {
    foreach (login_inf in _static_login_calls[player]) {
      if (login_inf->ob == obj &&
          login_inf->funct == funct) {
        _static_login_calls[player] -= ({ login_inf });
        // Clean it out if they don't have any left.
        if(!sizeof(_static_login_calls[player]))
          map_delete(_static_login_calls, player);
        catch(unguarded((: save_object, SAVE_NAME :)));
        return 1;
      }
    }
  }
  
  return 0;
} /* remove_static_login_call() */

/**
 * This method adds a funct to be called whenever a player logs
 * in.  The funct and object are stored only for this reboot
 * so they may be object references or funct pointers.
 * @param player specfic player to look for "all" for everyone
 * @param func the funct to call
 * @param obj the object to call the funct on
 * @return 0 on failure and 1 on success
 * @see remove_dynamic_login_call()
 * @see add_static_login_call()
 */
int add_dynamic_login_call(string player, mixed funct, mixed obj) {
    class login_information login_inf;

    if (stringp(obj) || objectp(obj)) {
        if (!stringp(funct)) {
            return 0;
        }
    } else if (!functionp(funct)) {
        return 0;
    }

    if (!_dynamic_login_calls[player]) {
        _dynamic_login_calls[player] = ({ });
    }
    login_inf = new (class login_information, funct : funct, ob : obj);
    _dynamic_login_calls[player] += ({ login_inf });

    return 1;
} /* add_dynamic_login_call() */

/**
 * This method removes a login call.
 * @param player the player specifier
 * @param funct the funct name
 * @param obj the object to call the funct on
 * @return 0 on failure and 1 on success
 * @see add_dynamic_login_call()
 * @see add_static_login_call()
 */
int remove_dynamic_login_call(string player, mixed funct, mixed obj) {
    class login_information login_inf;

    if (stringp(obj) || objectp(obj)) {
        if (!stringp(funct)) {
            return 0;
        }
    } else if (!functionp(funct)) {
        return 0;
    }

    if (_dynamic_login_calls[player]) {
        foreach (login_inf in _dynamic_login_calls[player]) {

            if (login_inf->ob == obj &&
              login_inf->funct == funct) {
                _dynamic_login_calls[player] -= ({ login_inf });
                return 1;
            }
        }
    }

    return 0;
} /* remove_dynamic_login_call() */

void update_players_on( string player ) {
    object ob = find_player( player );

    // tell_creator( "taffyd", "update_players_on(): %O, %O\n", player, ob );

    if ( !ob ) {
        return;
    }

    if ( is_compressed_login( ob ) ) {
        if ( undefinedp( _compressed_players_on[ player ] ) ) {
            _compressed_players_on[ player ] = 1;
        }
    }
    else {
        if ( undefinedp( _telnet_players_on[ player ] ) ) {
            _telnet_players_on[ player ] = 1;
        }
    }

    if ( member_array( player, _players_on ) == -1 ) {
        _players_on += ({ player });
    }
}

int tidy_players_on( string player ) {
    int pos; 
    int ret;

    ret = 0; 

    if ( _compressed_players_on[ player ] ) {
        map_delete( _compressed_players_on, player );
        ret = 1;
    }

    if ( _telnet_players_on[ player ] ) {
        map_delete( _telnet_players_on, player );
        ret = 1;
    }

    pos = member_array( player, _players_on );

    if ( pos != -1 ) { 
        //tell_creator( "taffyd", "Deleting %s from the players list.\n",
        //    _players_on[ pos ] );
        _players_on = _players_on[0..pos-1] + _players_on[pos+1..];
        ret = 1;
    }
    
    return ret;
} /* tidy_players_on() */ 



/** @ignore yes */
int player_logon(string player, int guest, string ip) {
    // tell_creator( "taffyd", "player_login(): %O, %O, %O\n", player, guest, ip );

    if(lower_case(player) != (string)previous_object()->query_name()) {
        return 0;
    }

    /* Update the players_on array. */
    
    update_players_on( player );

    /* Figure out if there are any functions to call. */
    do_all_calls(player, LOGIN);

    if (find_call_out(_title_call) == -1) {
        _title_call = call_out("check_all_titles", 5, player );
    }


    write_log("Enter", player, guest, ip);
} /* player_login() */

private void do_all_calls(string player, string event_t) {
  class login_information login_inf;
  
  if (_static_login_calls[player]) {
    foreach (login_inf in _static_login_calls[player]) {
      call_out((: do_call :), 1, login_inf,
               player, event_t );
    }
  }
  if (_static_login_calls["all"]) {
    foreach (login_inf in _static_login_calls["all"]) {
      call_out((: do_call :), 1, login_inf,
               player, event_t );
    }
  }
  if (_dynamic_login_calls[player]) {
    foreach (login_inf in _dynamic_login_calls[player]) {
      call_out((: do_call :), 1, login_inf,
               player, event_t );
    }
  }
  if (_dynamic_login_calls["all"]) {
    foreach (login_inf in _dynamic_login_calls["all"]) {
      call_out((: do_call :), 1, login_inf,
               player, event_t );
    }
  }
} /* do_all_calls() */

/** @ignore yes */
private void do_call(class login_information login_inf,
                     string player, int event_t) {
  object ob;
     
  if (!find_player(player) || find_player(player)->query_invis() < 2) {
    if (!functionp(login_inf->funct)) {
      if(login_inf->ob && login_inf->funct) 
        catch(ob = load_object(login_inf->ob));
      if ( ob ) { 
        catch(call_other(ob, login_inf->funct, player, 
                         event_t));
      } else {
        tell_creator("ceres", "failed to load %s", login_inf->ob);
      }
    } else {
      catch(evaluate(login_inf->funct, player, event_t));
    }
  }
} /* do_call() */

/** @ignore yes */
int player_logout(string player) {
    object ob;

    do_all_calls(player, LOGOUT);

    if ( tidy_players_on( player ) ) {
        if (find_call_out(_title_call) == -1) {
            _title_call = call_out("check_all_titles", 5, player );
        }
    }

    if(find_call_out(_save_call) == -1) {
        _save_call = call_out("save_users", NEW_SAVE_TIME );
    }

    /* always do this bit... */
    
    // tell_creator( "taffyd", "Queue checking...\n" );
    // tell_creator( "taffyd", "is_discworld_full(WITHOUT_LOGIN_QUEUE) = %d",
    //    is_discworld_full(WITHOUT_LOGIN_QUEUE) );

    while (sizeof(query_login_queue()) && 
        !is_discworld_full(WITHOUT_LOGIN_QUEUE) ) {

        // tell_creator( "taffyd", "Doing login queue checks...\n" );

        ob = pop_from_login_queue();

        if (ob) {
            // tell_creator( "Removing %O from the login queue\n" );
            ob->remove_from_login_queue();
        }
    }

    write_log("Exit", player, 0, "");
    return 1;
} /* player_logout() */

/** @ignore yes */
int player_net_dead(string player) {
    object ob;
    
    if ( tidy_players_on( player ) ) {
        do_all_calls(player, NETDEATH);

        if (find_call_out(_title_call) == -1) {
            _title_call = call_out("check_all_titles", 5, player );
        }
    }

    /* always do this bit... */
    while ( sizeof(query_login_queue()) && !is_discworld_full(WITHOUT_ANY_LOGINS) ) {
        // tell_creator( "taffyd", "is full netdead check\n" );

        ob = pop_from_login_queue();
        if (ob) {
            ob->remove_from_login_queue();
        }
    }

    write_log("NDead", player, 0, "");
    return 1;
} /* player_net_dead() */

/** @ignore yes */
int player_reconnected(string player) {
    update_players_on(player);
    do_all_calls(player, RECONNECT); 

    if (find_call_out(_title_call) == -1) {
        _title_call = call_out("check_all_titles", 5, player);
    }

    write_log("Recon", player, 0, query_ip_name(find_player(player)));
} /* player_reconnected() */

/** @ignore yes */
protected void check_all_titles( string player ) {
    int al, good, bad;
    object good_thing, bad_thing, thing, *things;

    good = -250;
    bad = 250;
    things = users();
    foreach( thing in things ) {
      if (!thing || thing->query_creator() ) {
        continue;
      }
      al = (int)thing->query_al();
      if ( al > bad ) {
        bad = al;
        bad_thing = thing;
      }
      if ( al < good ) {
        good = al;
        good_thing = thing;
      }
    }
    if ( bad_thing ) {
      if ( stringp( _worst ) && ( _worst != player ) ) {
        thing = find_player( _worst );
      } else {
        thing = 0;
      }
      if ( bad_thing != thing ) {
        if ( thing ) {
          thing->remove_title( "align" );
        }
        switch ( (int)bad_thing->query_gender() ) {
        case 1 :
          bad_thing->set_title( "align", "Lord of Darkness" );
          break;
        case 2 :
          bad_thing->set_title( "align", "Lady of Darkness" );
          break;
        default :
          bad_thing->set_title( "align", "Monster of Darkness" );
        }
        _worst = (string)bad_thing->query_name();
      }
    }
    if ( good_thing ) {
      if ( stringp( _best ) && ( _best != player ) ) {
        thing = find_player( _best );
      } else {
        thing = 0;
      }
      if ( good_thing != thing ) {
        if ( thing ) {
          thing->remove_title( "align" );
        }
        switch ( (int)good_thing->query_gender() ) {
        case 1 :
          good_thing->set_title( "align", "Lord of Light" );
          break;
        case 2 :
          good_thing->set_title( "align", "Lady of Light" );
          break;
        default :
          good_thing->set_title( "align", "Creature of Light" );
        }
        _best = (string)good_thing->query_name();
      }
    }
} /* check_all_titles() */

/** @ignore yes */
void save_users() {
    int number;
    int next_time; 
    string word;
    object thing;

    number = sizeof( _players_on );
    
    if ( !number ) {
//        log_file( "LOGIN_HANDLER", "No players on, resetting...\n" ); 
        _players_on = query_players_on();
        _save_call = call_out("save_users", NEW_SAVE_TIME );
        return;
    }
    
    store_user_count();

    // log the number of players and creators.
    if(_next_user_log < time()) {
        log_user_count();
    }
    
    word = _players_on[ 0 ];

    thing = find_player( word );

    if ( objectp( thing ) && ( number > 1 ) ) {
        if ( environment( thing ) ) {
//        log_file( "LOGIN_HANDLER", "%s: Saving player %s\n", 
//                ctime( time() ), thing->query_name() );
            thing->save_me();
        }
//        log_file( "LOGIN_HANDLER", "%s: Shuffling %s to the back of the queue, "
//            "next player is %s (TP: %O, number: %d, ob: %O)\n", ctime( time() ), 
//                word, _players_on[1],  this_player(), number, thing ); 
        _players_on = _players_on[1..] + ({ word });
        // _players_on[ 0 .. number - 2 ] = _players_on[ 1 .. ];
        //_players_on[ number - 1 ] = word;
    } else {
//        log_file( "LOGIN_HANDLER", "%s: Not saving %s, player doesn't "
//            "exist. (TP: %O, number: %d, ob: %O)\n", ctime( time() ), word, 
//                this_player(), number, find_player( word ) ); 
        _players_on = _players_on[1..] + ({ word });
    } 
    
    /* To try and resolve the weird auto-saving problems... */ 
    next_time = SAVE_TIME / number;
    
    if ( next_time > 10 ) {
        next_time = 10; 
    }

    _save_call = call_out( (: save_users :), next_time );
} /* save_users() */

int int_is_discworld_full(int logins) { 
    object *players;
    int compressed;
    int uncompressed;
    int ret;
    int ext;

    players = users();
    
    switch (logins) {
    case WITHOUT_LOGINS_NOT_IN_QUEUE :
        players = filter(players, (: objectp($1) && !$1->query_login_ob() &&
            !$1->query_creator() :)) + query_login_queue();
        break;
    case WITHOUT_LOGIN_QUEUE :
        players -= query_login_queue();
        players = filter(players, (: objectp($1) && !$1->query_creator() :));
        break;
    case WITHOUT_ANY_LOGINS :
        players = filter(players, (: objectp($1) && !$1->query_login_ob() &&
            !$1->query_creator() :));
        ext = _out_q;
        break;
    default :
        players = filter(players, (: objectp($1) && !$1->query_creator() :));
        break;
    }

    if(time() != _q_time){
      ext = 0;
      _out_q = 0;
      _q_time = time();
    }
    
    // Now, determine how many of those players are compressed, 
    // and how many are uncompressed.
    compressed = sizeof( filter( players, (: is_compressed_login($1) :) )  ) + ext;
    uncompressed = sizeof( players ) - compressed;

    ret = DISCWORLD_IS_EMPTY;
    
    // If we're compressed, then we set the COMPRESSED_FULL
    // flag.
    if ( compressed >= MAX_COMPRESSED_PLAYERS ) { 
        ret |= COMPRESSED_FULL;  
    }
    
    // Same for uncompressed.
    if ( uncompressed >= MAX_UNCOMPRESSED_PLAYERS ) { 
        ret |= UNCOMPRESSED_FULL;
    }

    // tell_creator( "taffyd", "%d, %d, %d\n", compressed, uncompressed, ret );
    // tell_creator( "taffyd", "int_is_discworld_full: %d, %O\n", ret, call_stack( 2 ));
    return ret;
} /* int_is_discworld_full() */ 

int is_compressed_full( int logins ) {
    return int_is_discworld_full( logins ) & COMPRESSED_FULL; 
} /* is_compressed_full() */ 

int is_uncompressed_full( int logins ) {
    return int_is_discworld_full( logins ) & UNCOMPRESSED_FULL; 
} /* is_compressed_full() */ 

/**
 * Try and figure out if the mud is full or not.  Counts the number
 * of players and returns a true or false value.
 * @see /include/login.h#WITHOUT_LOGINS_NOT_IN_QUEUE
 * @see /include/login.h#WITHOUT_LOGIN_QUEUE
 * @see /include/login.h#WITHOUT_ANY_LOGINS
 * @param logins a flag
 * @return 1 if the mud is full, 0 if it is not
 */
int is_discworld_full(int logins) {
    int ret;
    
    ret = int_is_discworld_full( logins );

    ret = (ret & COMPRESSED_FULL) && (ret & UNCOMPRESSED_FULL);

    // tell_creator( "taffyd", "is_discworld_full: %d, %O\n", ret, call_stack( 2 ));

    return ret;
} /* is_discworld_full() */

private void store_user_count() {
    object pl;
    int current_players, current_creators, current_login_q;
    string *logins_n;

    foreach(pl in users()) {
      if (pl && !pl->query_login_ob()) {
        if (pl->query_creator()) {
          current_creators++;
        } else {
          current_players++;
        }
      }
    }

    logins_n = map(LOGIN_HANDLER->query_login_queue(),
      (: $1->query_cap_name() :));
    logins_n = sort_array(logins_n, 1);
    current_login_q = sizeof(logins_n);

    if(current_players + current_login_q > _players + _login_q) {
        _players = current_players;
        _creators = current_creators;
        _login_q = current_login_q;
    }
} /* store_user_count() */

private void log_user_count() {
    log_file("USERS", "%s %d %d %d\n" ,ctime(time()), _creators, _players,
      _login_q);

    _creators = 0;
    _players = 0;
    _login_q = 0;
    _next_user_log = time() + 3601 - (time() % 3600);
} /* log_user_count() */

/**
 * This method is used to do delayed logging to the log files.  It will
 * store a bunch of entries until it is over a maximum size and then
 * save this to the log.
 * @param event the event generated
 * @param name the name of the player the event is for
 * @param guest if the person in question is a guest
 * @param other other information about the event
 */
void write_log(string event, string name, int guest, string other) {
    switch(event) {
    case "Enter":
        enter_log += sprintf( "%s [Enter] %-15s %s%s\n",
          ctime(time())[4..18], name,
          guest?"(guest) ":"",
          other);
        break;
    case "Su":
        enter_log += sprintf( "%s [Su]    %15-s %15-s\n",
          ctime(time())[4..18], name, other);
        break;
    case "Periodic":
        call_out("write_log", BUFFER_TIME, "Periodic", "", 0, "");
        if (sizeof(enter_log) && time() - last_entry > BUFFER_TIME) {
            unguarded((: write_file, "/d/admin/log/ENTER.log", enter_log :));
            enter_log = "";
            last_entry = time();
        }
        return;
    case "Recon":
        enter_log += sprintf( "%s [Recon] %15-s %s\n",
          ctime(time())[4..18], name, other);
        break;
    default:
        enter_log += sprintf("%s %-7s %15-s\n", ctime(time())[4..18],
          "["+event+"]", name);
        break;
    }

    last_entry = time();
    if(sizeof(enter_log) > 1024) {
        unguarded((: write_file, "/d/admin/log/ENTER.log", enter_log :));
        enter_log = "";
    }
} /* write_log() */

/** @ignore yes */
void dest_me() {
    unguarded((: write_file, "/d/admin/log/ENTER.log", enter_log :));
    enter_log = "";
    destruct(this_object());
} /* dest_me() */

/**
 * This function is used to cache the login messages used by /secure/login
 * it costs a bit in memory but saves us having to re-read the same text
 * file everytime someone logs in.
 * For security reasons this function will only work with files in /doc
 *
 * @param the filename of the file to be returned
 * @return the text of the file.
 */
string get_message(string mess_name) {
    if(!messages)
        messages = ([ ]);

    // Prevent it working on anything except in /doc to avoid any security risks.
    if(mess_name[0..4] != "/doc/")
        return 0;

    if(!messages[mess_name])
        messages[mess_name] = read_file(mess_name);

    return messages[mess_name];
}

void clear_messages() {
    messages = ([ ]);
}

int site_lockeddown(string ip_addr) {
    if(!lockdowns)
        lockdowns = ([ ]);

    if(lockdowns[ip_addr] &&
      (sizeof(lockdowns[ip_addr]) >= MAX_FAILURES) &&
      (lockdowns[ip_addr][0] > time() - LOCKDOWN_TIME))
        return 1;
    return 0;
}

void failed_login(string ip_addr) {
  int i;
  
  if(!lockdowns)
    lockdowns = ([ ]);
  if(!lockdowns[ip_addr])
    lockdowns[ip_addr] = ({ time() });
  else {
    // Trim the last failures
    for(i=0; i<sizeof(lockdowns[ip_addr]); i++)
      if(lockdowns[ip_addr][i] < time() - LOCKDOWN_TIME) {
        lockdowns[ip_addr] -= ({ lockdowns[ip_addr][i] });
        i--;
      }
      
    lockdowns[ip_addr] += ({ time() });
    if(sizeof(lockdowns[ip_addr]) > MAX_FAILURES) {
      lockdowns[ip_addr] =
        lockdowns[ip_addr][sizeof(lockdowns[ip_addr])-MAX_FAILURES..];
    }
  }
}

/*
 * periodically clean out the lockdowns mapping otherwise it can get
 * full of unnecessary entries.
 */
void clean_lockdowns() {
  string ip;
  
  call_out("clean_lockdowns", 1800);
  foreach(ip in keys(lockdowns)) {
    if(lockdowns[ip][sizeof(lockdowns[ip])-1] < time() - LOCKDOWN_TIME) {
      map_delete(lockdowns, ip);
    }
  }
}

void clear_lockdowns(string ip) {
  if(!ip)
    lockdowns = ([ ]);
  else if(lockdowns[ip])
    map_delete(lockdowns, ip);
}

void query_lockdowns() {
    printf("%O\n", lockdowns);
}

void reset() {
    if (find_call_out(_save_call) == -1) { 
        save_users(); 
    }
} /* reset() */

