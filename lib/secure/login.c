/**
 * The main login object.
 */

#include <data.h>
#include <login_handler.h>
#include <playerinfo.h>
#include <random_names.h>
#include <login.h>
#include <mail.h>
#include <access.h>
#include <playtesters.h>

//#define DEBUG

#define RL_NORMAL 4
#define RL_NONEW 3
#define RL_PT 2
#define RL_CRE 1

#define QUOTE_HANDLER "/obj/handlers/pqf_handler"
#define MULTIPLAYER "/obj/handlers/multiplayer"
#define LIVING "/obj/handlers/livings"
#define CLUB_HANDLER "/obj/handlers/club_handler"
#define BASTARDS "/secure/bastards"

#define FILE_PATH "/doc/login"
#define LOGIN_SCRIPT FILE_PATH + "/login_script.txt"

#define READ_INPUT 1
#define SLEEP 2

// Name length restrictions.
#define MIN_LEN 3
#define MAX_LEN 12

#define ADMIN_EMAIL "trustees@discworld.imaginary.com"
#define FREE_DOMAINS ({"hotmail.com", "yahoo.com"})
#define TIMEOUT_TIME 120
#define THROWOUT_TIME ( 2 * TIMEOUT_TIME )
#define MIN_RESET_TIME 86400

/**
 * This structure holds a single state. A state has a name,
 * an action or write and a set of events. <br>
 * The name is used to identify the state and must be unique within the
 * state table. An action is a function to call while a write is a
 * string or file to be written to the screen.  The events mapping
 * is made of event and new-state. The event is the trigger and
 * new-state is the new state the machine should enter.<p>
 *
 * An event is a string of the form "type-data" where type is either
 * "input" or "return" indicating user input or the return from an
 * action function. Thus "return-ok" would be triggered if the action
 * function in this state returned "ok" while "input-y" would be
 * triggered if the user typed in "y". The special event "input" will
 * trigger on any user input while the event "default" will trigger on
 * any input or return value.
 * 
 */
class state {
  string name;
  string action;
  string write;
  int noecho;
  mapping events;
}

private mapping _states; // This is the state machine.
private nosave string _state; 
private nosave string _last_state;
private nosave string _event;
private nosave string _last_event;
private nosave int _login_start_time;

private nosave int _counter; // A counter used for looping.
private nosave mapping _data; // Login data collected so far.
private nosave int _compiling; // Are we compiling the state machine?

nosave private int _run_level;

// Stuff for terminal type
private nosave string _terminal_name;
private nosave int _rows;
private nosave int _cols;

protected void finish_compiling(string, mapping);
protected void load_file();
void enter_state(string);
void dest_me();


/** @ignore yes */
void create() {
  seteuid("Root");
  _states = ([ ]);
  load_file();
  _data = ([ ]);

  // set the run level based on the mudname.
  _run_level = RL_NORMAL;
  
  
  if(interactive(this_object()))
     resolve(query_ip_number(this_object()), "");

  call_out("time_out", TIMEOUT_TIME);
}

#ifdef DEBUG
void debug_log(string fmt, mixed args ...) {
  log_file("/secure/NLOGIN",
           "%s: " + fmt + "\n", ctime(time())[4..18], args ...);
}
#else
void debug_log(string fmt, mixed args ...) { return; }
#endif

/** @ignore yes
 * This function reads either the saved data or causes the state machines
 * script to be re-parsed.
 */
void load_file() {
  if(_compiling) {
    debug_log("Already compiling, exiting.");
    return;
  }

  if(file_size(base_name(this_object()) + ".o") > -1 &&
     unguarded((: stat(base_name(this_object()) + ".o") :))[1] >=
     unguarded((: stat(LOGIN_SCRIPT) :))[1]) {
    debug_log("loading data.");
    unguarded((: restore_object, base_name(this_object()) :));
    if(!_states)
      _states = ([ ]);
    debug_log("data load complete.");
  } else {
    debug_log("Compiling script");
    debug_printf("Compiling script");
    _compiling = 1;
    DATA_HANDLER->compile_file(LOGIN_SCRIPT, (: finish_compiling :));
  }
}

/** @ignore yes
 * This function is called by the data compiler once it has finished
 * compiling the data into a mapping. This function builds the state
 * machine in _states.
 */
protected void finish_compiling(string fname, mapping data) {
  string name, s, e;
  mixed *bits;
  mapping thing, event;

  debug_printf("Finished compiling");
  
  foreach(name, bits in data) {
    switch(name) {
    case "state":
      foreach(thing in bits) {
        if(_states[thing["name"]])
          debug_log("Error, state %s already exists!", thing["name"]);
        
        _states[thing["name"]] = new(class state,
                                     action: thing["action"],
                                     write: thing["write"],
                                     noecho: thing["no-echo"],
                                     events: ([ ]));
        debug_log("State %s found", thing["name"]);
        if(!arrayp(thing["event"]))
          thing["event"] = ({ thing["event"] });
        
        foreach(event in thing["event"]) {
          if(mapp(event) && stringp(event["data"]) &&
             stringp(event["new-state"])) {
            _states[thing["name"]]->events[event["data"]] = event["new-state"];
            debug_log("Event %s new state %s", event["data"],
                   event["new-state"]);
          } else
            debug_log("Error event %O in state %s", event, thing["name"]);
        }
      }
      break;
    default:
      tell_creator("ceres", "Name: " + name + "\n");
    }
  }

  debug_log("Validating state machine.");
  foreach(s in keys(_states)) {
    if(_states[s]->action && !function_exists(_states[s]->action))
      debug_log("Action function %s doesn't exist", _states[s]->action);
    foreach(e in keys(_states[s]->events)) {
      if(!_states[s]->events[e])
        debug_log("New state %s doesn't exist in %s %s",
                  _states[s]->events[e], _states[s]->name, e);
    }
  }
  debug_log("Done. %d states created.", sizeof(keys(_states)));
  debug_log("Saving data.");
  _compiling = 0;
  unguarded((: save_object, base_name(this_object()) :));
}

/** @ignore yes
 * This function processes a received event. The event can come from an
 * input_to or be generated by the action function for this state.
 * The function the calls enter_state to enter the new state produced
 * by this event.
 */
void process_event(string event, string type) {
  if(type == "input" && _states[_state] && _states[_state]->noecho)
    debug_log("entering process event with state %s and event %s-<hidden>",
              _state, type);
  else
    debug_log("entering process event with state %s and event %s-%s", _state,
              type, event);

  _last_event = _event;
  _event = type + "-" + event;
  
  if(_states[_state]->events[type + "-" + lower_case(event)]) {
    enter_state((_states[_state])->events[type + "-" + lower_case(event)]);
  } else if(_states[_state]->events[type]) {
    enter_state((_states[_state])->events[type]);
  } else if(_states[_state]->events["default"]) {
    enter_state((_states[_state])->events["default"]);
  } else {    
    debug_log("no such event %s in state %s (%s)", type + "-" + 
              lower_case(event), _state,
              query_multiple_short(keys(_states[_state]->events)));
    return;
  }
}

/*
 * @ignore yes
 * This function is called when we enter a new state. It calls the action
 * function if there is one and then does an input_to() or just goes
 * straight on to the event processing function.
 */
void enter_state(string new_state) {
  mixed retval;
  string tmp, field;
  
  _last_state = _state;
  _state = new_state;

  debug_log("entering state %s", _state);

  if(new_state == "finished")
    return;
  
  if(!_states[_state]) {
    debug_log("invalid state %s exiting", _state);
    dest_me();
  }
  
  // Write out files or strings
  if(_states[_state]->write) {
    debug_log("writing %s", _states[_state]->write);
    if(file_size(_states[_state]->write) > 0)
      tmp = LOGIN_HANDLER->get_message(_states[_state]->write);
    else
      tmp = _states[_state]->write;

    // Enable use of $name in strings/files.
    foreach(field in keys(_data))
      if(stringp(_data[field]))
        tmp = replace_string(tmp, "$"+field, _data[field]);
    if(tmp[sizeof(tmp)-1] == '\n')
      tmp = tmp[0..<2];
    write(tmp);
    
  }
  
  // Call the action function.
  if(_states[_state]->action) {
    debug_log("calling %s", _states[_state]->action);
    retval = call_other(this_object(), _states[_state]->action, _event);
  } else
    retval = READ_INPUT;

  if(intp(retval) && retval == READ_INPUT) {
    debug_log("action %s returned read_input in state %s",
              _states[_state]->action, _state);
    input_to("process_event", _states[_state]->noecho, "input");
    return;
  }
  
  if(_states[_state]->action)
    debug_log("action %s returned %s in state %s",
              _states[_state]->action, (string)retval, _state);

  // This is a hack to jump straight from check-login-access to 
  // read-password. It's here because some clients don't do proper
  // login triggers but just blast name & password and a call_out
  // screws that up.
  if(_state == "check-login-access") {
    process_event(retval, "return");
    return;
  }
  call_out("process_event", 0, retval, "return");
}

/** @ignore yes */
string query_state() { return _state; }

/*************************************************************************
 * From here down it's the actual action functions.
 *************************************************************************/
/** @ignore yes */
private int check_valid_name(string str) {
  int i;

  for (i=0;i<strlen(str);i++) {
    if (str[i] < 'a' || str[i] > 'z') {
       return i;
    }
  }
  return -1;
}

/** @ignore yes */
private int check_name(string name, int new_char) {
  if(strlen(name) < MIN_LEN) {
    write("Sorry the player name " + name + " is too short (min " + MIN_LEN +
          " characters).\n");
    return 0;
  }

  if(strlen(name) > MAX_LEN) {
    write("Sorry the player name " + name + " is too long (max " +
          MAX_LEN + " characters).\n");
    return 0;
  }

  if(check_valid_name(name) != -1) {
    write("Invalid characters used in the name.  You can only use letter "
          "characters, from a to z, spaces and any other sort of "
          "punctuation may not be used.\n");
    return 0;
  }
  if(PLAYER_HANDLER->test_banished(name)) {
    write("\nSorry the player name " + name + " has been banished.\n");
    return 0;
  }

  if (!PLAYER_HANDLER->test_valid(name) || MAIL_TRACK->query_list(name)) {
    write("Sorry that name is not allowed.\n");
    return 0;
  }
  if(PLAYER_HANDLER->test_user(name) || LIVING->find_player(name)) {
    write("Sorry that name has already been taken.\n");
    return 0;
  }

  if(CLUB_HANDLER->is_club(name)) {
    write("Sorry that name is already in use for a club or family.\n");
    return 0;
  }

  return 1;
}

/** @ignore yes
 * Generate a random password. This is used when resetting a password or
 * when creating an authorised character.
 */
private string generate_password() {
  int i;
  string pass = "";
  
  for(i=0; i<8; i++)
    switch(random(3)) {
    case 2:
      pass += sprintf("%c", 65 + random(26));
      break;
    case 1:
      pass += sprintf("%c", 97 + random(26));
      break;
    default:
      pass += sprintf("%c", 48 + random(10));
    }
  // This strips out easily confused characters 0oO 1iI
  return replace(pass, ({ "0", "9", "o", "p", "O", "P",
                          "1", "2", "i", "j", "I", "J",
                          "l", "m" }));
}

/** @ignore yes
 * This displays the main menu. It also does some initial checks to make
 * sure that someone will have a chance of logging in.
 */
mixed main_menu(string event) {
  string mess;

  _data = ([ ]);
  _counter = 0;

  write(LOGIN_HANDLER->get_message(FILE_PATH+ "/BANNER"));

  if(LOGIN_HANDLER->site_lockeddown(query_ip_number(this_object()))) {
    write("Sorry, your site has been locked down for excessive failed login "
          "attempts.  If you have forgotten your password please email "+
          ADMIN_EMAIL + ".  If you have not failed to login "
          "please try again later.\n");
    mess = sprintf("attempted login from locked down site: %s.",
                   query_ip_number(this_object()));
    log_file("BAD_PASSWORD", ctime(time())[4..18] + ": " + mess + "\n");
    event(efun::users(), "inform", mess, "bad-password");
    return "quit";
  }

  if(find_object("/obj/shut")) {
    if(find_object("/obj/shut")->query_time_to_crash() < 60) {
      write("With less than one minute to go it is too close to the "
            "shut-down to attempt to log in, please try again in a "
            "few minutes.\n");
      return "quit";
    }
    write("It is too close to the shut-down to attempt to log "
          "in unless you are a creator or net-dead.  Please try again "
          "in a few minutes.\n");
  }
  write(LOGIN_HANDLER->get_message(FILE_PATH+ "/MAIN_MENU"));
  return READ_INPUT;
}

/** @ignore yes
 * Check that the person trying to delete themselves exists and isn't
 * already marked for deletion
 */
mixed check_delete_name(string event) {
  _data["name"] = lower_case(replace_string(event, "input-", ""));

  if(!PLAYER_HANDLER->test_user(_data["name"])) {
    write("There is no such player.\n\nPress enter to continue ");
    return READ_INPUT;
  }

  if(PLAYER_HANDLER->test_creator(_data["name"])) {
    write("Creators cannot be deleted this way.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }

  if(file_size(PLAYER_HANDLER->query_delete_player_file_name(_data["name"]) +
               ".o") > 0 ||
     file_size(PLAYER_HANDLER->query_delete_player_file_name(_data["name"]) +
               ".o.gz") > 0) {
    write("That character is already marked for deletion.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }

  return "ok";
}

/** @ignore yes
 * Verify the password of someone wanting to delete themselves.
 */
mixed check_delete_password(string event) {
  _data["password"] = replace_string(event, "input-", "");

  if(!PLAYER_HANDLER->test_password(_data["name"], _data["password"])) {
    write("\nPassword incorrect.\n");
    return "invalid";
  }
  write("This is will PERMANENTLY delete your character, are you sure? "
        "[y/n] "); 
  return READ_INPUT;
}

/** @ignore yes
 * Mark a character for deletion
 */
mixed delete_character(string event) {
  string extension;
  
  if(PLAYER_HANDLER->test_creator(_data["name"])) {
    write("Creators cannot be deleted this way.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }

  if(unguarded((: file_size,
                PLAYER_HANDLER->query_player_file_name(_data["name"]) + ".o"
                :)) != -1) {
    extension = ".o";
  } else {
    extension = ".o.gz";
  }

  if(unguarded((: cp, PLAYER_HANDLER->query_player_file_name(_data["name"]) +
                extension,
                PLAYER_HANDLER->query_delete_player_file_name(_data["name"]) +
                extension :))){
    unguarded((: rm, PLAYER_HANDLER->query_player_file_name(_data["name"]) +
               extension :));
    unguarded((: rm,
               PLAYER_HANDLER->query_player_disk_file_name(_data["name"]) +
               extension :));
    write("\n\nPLEASE READ:\nCharacter deleted.  You have 10 days "
          "cooling off time, during "
          "which you may reinstate your character.  To reinstate it "
          "simply log on again and it will reinstate it for you.\n\n");
  } else {
    write("WARNING! Unable to delete your character.\n\n");
  }
  write("Press enter to continue ");
  return READ_INPUT;
}

/** @ignore yes
 * Check if new players are allowed from this site. It also checks the
 * time to shutdown. The function returns "ok", "auth" or READ_INPUT. 
 */
mixed check_new_access(string event) {
  mixed *tmp;
  
  _data["new_player"] = 1;

  if(_run_level != RL_NORMAL) {
    write("Sorry this site is not open to new players.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }

  if(find_object("/obj/shut")) {
    write("It is too close to the shut-down to attempt to create a new "
          "character.  Please try again in a few minutes.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }
  
  switch(BASTARDS->query_access(this_object())) {
  case NO_NEW:
  case NO_ACCESS:
    write("\nNew player logins are disabled from this site.  If you wish "
          "to create a character, please email " + ADMIN_EMAIL + 
          " with the player name you "
          "would like to use, the name(s) of your other characters and "
          "the name of your ISP.  Note that requests from free email "
          "accounts (hotmail, yahoo etc.) may not be acceptable.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  case AUTH_NEW:
    write("\nNew player logins from this site are disabled.  In order to "
          "create a new character you will be asked for a character "
          "name and an email address, a password will then be emailed "
          "to you at that address.  Note that the address must not "
          "include free accounts (hotmail, yahoo etc.)\n\n");
    return "auth";
  }

  if(!BASTARDS->query_multi(this_object())) {
    tmp = filter(users(), 
                 (: interactive($1) &&
                  !$1->query_creator() &&
                  !$1->query_property("test character") &&
                  !strcmp(query_ip_number($1), $2) :), 
                 query_ip_number(this_object())) - ({ this_object() });
    if(sizeof(MULTIPLAYER->check_allowed(this_object(), tmp))) {
      write(LOGIN_HANDLER->get_message(FILE_PATH+ "/MULTIPLAYERS") +
            "\n\n"
            "Press enter to continue ");
      return READ_INPUT;
    }
  }
  
  return "ok";

  // Must not be nonew, or noaccess. Must be unique IP unless site is marked
  // for MP.
}

/** @ignore yes
 * Check if guests are allowed to login from this site.
 */
mixed check_guest_access(string event) {
  _data["guest"] = 1;
  _data["new_player"] = 1;

  if(_run_level != RL_NORMAL) {
    write("Sorry this site is not open to guests.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }

  if(find_object("/obj/shut")) {
    write("It is too close to the shut-down to attempt to create a new "
          "character.  Please try again in a few minutes.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }

  if(BASTARDS->query_access(this_object()) != DEFAULT) {
    write("\nGuest logins are disabled from this site.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }

  return "ok";
}

/** @ignore yes
 * Show the finger information for a specific user.
 */
mixed finger_player(string event) {
  string finger_info;
  string player;
  player = replace_string(event, "input-", "");
  if(player)
    player = lower_case(player);

  if (stringp(player) && strlen(player) > 2 && check_valid_name(player)) {
    finger_info = "/secure/finger"->finger_info(player, 1);
    if (!finger_info) {
      write("I am sorry, there is no such player.\n\n");
    } else {
      write(strip_colours(finger_info) + "\n\n");
    }
  } else {
    write("Invalid name, returning to the login menu.\n\n");
  }
  write("Press enter to continue ");
  return READ_INPUT;
}

/** @ignore yes
 * Provide the who is online list.
 */
mixed list_players(string event) {
  write("Here is a list of the people currently playing Discworld:\n" +
        implode(sort_array(map(filter(users(), (: $1->query_cap_name() &&
                                                !$1->query_invis() &&
                                                $1->query_name() != "logon":)),
                               (: $1->query_cap_name() :)),
                           (: strcmp :)), ", ") + "\n");
  write( "\nPress enter to continue ");
  return READ_INPUT;
}

/** @ignore yes
 * This resets the counter for such things as password retries.
 */
mixed reset_counter(string event) {
  _counter = 0;
  return READ_INPUT;
}

/** @ignore yes
 * Check if an email address is valid and not in our disallowed list.
 */
mixed check_email(string event) {
  string name, domain, address;

  address = replace_string(event, "input-", "");
  if(!regexp(address,
           "^[a-zA-Z0-9\\+\\.\\_\\-]+@[a-zA-Z0-9\\_\\-].[a-zA-Z0-9\\_\\-]")){
    write("Sorry, that address is invalid.  Please try again or "
          "enter 'q' to quit.\n");
    return "invalid";
  }
  
  sscanf(address, "%s@%s", name, domain);
  if(member_array(domain, FREE_DOMAINS) != -1) {
    write("Sorry, that is a free account and not allowed.  Please try "
          "again or enter 'q' to quit.  If you do not have a non-free "
          "address email " + ADMIN_EMAIL + " for a new character.\n");
    return "invalid";
  }

  _data["email"] = address;
  return "ok";
}

/** @ignore yes
 * This function validates a new character name. It checks that it's OK
 * for a new character.
 */
mixed check_new_name(string event) {
  _data["name"] = lower_case(replace_string(event, "input-", ""));

  if(!_data["name"] || _data["name"] == "" || _data["name"] == "q")
    return "failed";

  _counter++;
  if(check_name(_data["name"], 1)) {
    _data["cap_name"] = capitalize(_data["name"]);
    return "ok";
  } else if(_counter < 3) {
    return "invalid";
  } else {
    return "failed";
  }
}

/** @ignore yes
 * This validates random names. Returns "ok" or "invalid".
 */
mixed check_random_name(string event) {
  string tmp;
  
  if(!event || event == "")
    return "invalid";
  
  tmp = replace_string(event, "input-", "");
  
  if((tmp[0] - '1') < 0 || (tmp[0] - '1') >= sizeof(_data["random_names"]))
    return "invalid";
  
  _data["name"] = lower_case(_data["random_names"][tmp[0] - '1']);

  if(check_name(_data["name"], 1)) {
    return "ok";
  } else 
    return "invalid";
}

mixed main_random_name_menu(string event) {
  string *langs;
  int i;

  i = 1;
  langs = RANDOM_NAME_GENERATOR->query_languages();
  for (i=0;i<sizeof(langs);i++)
    langs[i] = sprintf("%c - %s (eg. %s)", (i+'1'), capitalize(langs[i]),
                       RANDOM_NAME_GENERATOR->unique_name(langs[i]));

  write("You are choosing a name for the first time on Discworld.\n"
        "You can choose a name which is generated to sound something "
        "like:\n"+
        implode(langs, "\n") +
        "\nM - Main Menu\n"
        "Q - Quit\n\n"
        "Your choice? ");
  return READ_INPUT;
}

/** @ignore yes
 * This function is called to choose a language type for the random name
 * generator.
 */
mixed random_lang_choice(string event) {
  string choice;
  int len;
  
  choice = replace_string(event, "input-", "");
  len = sizeof(RANDOM_NAME_GENERATOR->query_languages());
  
  if(strlen(choice) == 1 && choice[0] >= '1' && choice[0] <= ('1'+len-1))
    return RANDOM_NAME_GENERATOR->query_languages()[choice[0]-'1'];

  write("Incorrect choice.\n");
  return "invalid";
}

/** @ignore yes
 * List a selection of random names in the chosen language type.
 */
mixed show_random_names(string event) {
  int i;
  string lang;

  lang = replace_string(event, "return-", "");
  write("Here is a list of 9 random names.  Please choose one:\n");
  _data["random_names"] = allocate(9);
  for (i=0; i<sizeof(_data["random_names"]); i++) {
    _data["random_names"][i] = RANDOM_NAME_GENERATOR->unique_name(lang);
    write(sprintf("%c - %s\n", i+'1', _data["random_names"][i]));
  }
  write("M - Main Menu\n"
        "N - Name Menu\n"
        "G - Generate a new set of names\n"
        "Q - Quit\n"
        "Or, type in your name of choice\n\n"
        "Your choice? ");

  return READ_INPUT;
}

/** @ignore yes
 * Do some simple checks on a new players password. Would be nice to have
 * more sophisticated checks here!
 */
mixed check_new_password(string event) {
  
  _data["password"] = replace_string(event, "input-", "");
  write("\n");
  
  if(!_data["password"] || _data["password"] == "")
    return "failed";

  if(strlen(_data["password"]) < 6) {
    write("Password is too short, must be at least 6 characters.\n");
    return "invalid";
  }

  if(_data["password"] == _data["name"]) {
    write("Password is the same as your name, please try again.\n");
    return "invalid";
  }

  return "ok";
}

/** @ignore
 * Check that the password is the same when re-entered.
 */
mixed verify_password(string event) {
  string tmp;

  write("\n");

  tmp = replace_string(event, "input-", "");

  if(tmp == _data["password"])
    return "ok";
  else {
    write("Passwords do not match.\n");
    return "invalid";
  }
}

/** @ignore yes
 * Offer a capitalization option for their name.
 */
mixed get_capitalization(string event) {
  if(!_data["cap_name"])
    _data["cap_name"] = capitalize(_data["name"]);
  
  write("\nHow would you like your name capitalised? [" +
        _data["cap_name"] + "] ");
  return READ_INPUT;
}

/** @ignore yes
 * Check that their chosen capitalization matches their character name.
 */
mixed check_capitalization(string event) {
  string tmp;
  
  tmp = replace_string(event, "input-", "");
  
  if(!tmp || tmp == "")
    return "ok";

  if(lower_case(tmp) != _data["name"]) {
    write("Sorry it has to be the same as your name.");
    return "invalid";
  }

  _data["cap_name"] = tmp;
  return "ok";
}

/** @ignore yes
 * Generate a temporary password for a user and mail it to them.
 */
mixed request_password(string event) {
  string name, email, pass, mess;
  mixed tmp;
  
  name = replace_string(event, "input-", "");
  if(name)
    name = lower_case(name);
  
  if(!PLAYER_HANDLER->test_user(name)) {
    write("There is no such player.\n\nPress enter to continue ");
    return READ_INPUT;
  }

  if(PLAYER_HANDLER->test_creator(name)) {
    write("Creator passwords cannot be reset this way mail " + 
          ADMIN_EMAIL + ".\n\nPress enter to continue ");
    return READ_INPUT;
  }

  if(!PLAYER_HANDLER->test_ip_allowed(name, query_ip_number(this_object()))) {
    write("Sorry that characters password cannot be reset from your IP "
          "address.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }
  
  email = PLAYER_HANDLER->test_email(name);
  if(sizeof(email) > 0 && email[0] == ':')
    email = email[1..];
  
  while(email != "" && sizeof(email) && email[0] == ' ')
    email = email[1..];
  
  if(!email || email == "" ||
     !regexp(email, "^[A-z0-9_+\\.]+@[A-z0-9_+]+\\.[A-z0-9_+\\.]+$")) {
    write("Sorry, that player does not have a valid email address set.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }

  tmp = BASTARDS->query_temp_password(name);
  if(tmp && tmp[0] > time() - MIN_RESET_TIME) {
    write("Sorry that character has had a temporary passwords set "
          "within the last " + number_as_string(MIN_RESET_TIME / 3600) +
          " hours, please try again later.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }

  pass = generate_password();
  BASTARDS->set_temp_password(name, crypt(pass, 0));
  
  MAILER->do_mail_message(email, "admin", "Your temporary password", 0,
                          "The temporary password for " + name + " is " + 
                          pass + "\n\nYou must use this password within "
                          "the next 7 days.  This is a one use\npassword.  "
                          "After using it to login you should immediately "
                          "use the\n'password' command to set a new "
                          "password.\n\n");
  mess = sprintf("password requested for %s by user at site  %s.", name, 
                  query_ip_number(this_object()));

  log_file("BAD_PASSWORD", ctime(time())[4..18] + ": " + mess + "\n");
  event(efun::users(), "inform", mess, "bad-password");

  write("An email is on its way to you " +
        "with a temporary password for this character.\n\n"
        "Press enter to continue ");
  return READ_INPUT;
}

/** @ignore yes
 *
 * This function checks to see if the user is allowed to login at this
 * time. It returns "ok" if they can or READ_INPUT if they cannot.
 */
mixed check_login_access(string event) {
  mixed *tmp;
  string *alts;
  
  if(event && strsrch(event, "input-") != -1)
    _data["name"] = lower_case(replace_string(event, "input-", ""));

  // Deal with cres using -<name> to go vis, :<name> to be go invis or
  // #<name> to be invis 2.
  if(_data["name"][0] == '-') {
    _data["name"] = _data["name"][1..];
    _data["go_invis"] = -1;
  } else if(_data["name"][0] == ':') {
    _data["name"] = _data["name"][1..];
    _data["go_invis"] = 1;
  } else if(_data["name"][0] == '#') {
    _data["name"] = _data["name"][1..];
    _data["go_invis"] = 2;
  }
  
  // Check which run level we're at and allow logins accordingly.
  switch(_run_level) {
  case RL_CRE:
    if(!PLAYER_HANDLER->test_creator(_data["name"]) &&
       !PLAYER_HANDLER->test_property(_data["name"], "test character")) {
      write("Sorry this site is only open to creators.\n\n"
            "Press enter to continue ");
      return READ_INPUT;
    }
    break;
  case RL_PT:
    if(!PLAYER_HANDLER->test_creator(_data["name"]) &&
       !PLAYER_HANDLER->test_property(_data["name"], "test character") &&
       !PLAYTESTER_HAND->query_playtester(_data["name"])) {
    write("Sorry this site is only open to creators and playtesters.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
    }
    break;
  default:
    break;
  }

  // Check how close we are to a shutdown. Allow creators & reconnects
  // during a reboot.
  if(find_object("/obj/shut") &&
     !PLAYER_HANDLER->test_creator(_data["name"]) &&
     !LIVING->find_player(_data["name"])) {
    write("It is too close to the shut-down to attempt to login.  "
          "Please try again in a few minutes.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }

  // This is probably unnecessary, but not player, no login.
  if(!PLAYER_HANDLER->test_user(_data["name"])) {
    write("There is no player by that name.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }

  // Check IP allow/deny lists.
  if(!PLAYER_HANDLER->test_ip_allowed(_data["name"],
                                      query_ip_number(this_object()))) {
    write("Sorry that character's access control list does not permit "
          "login from your IP address.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }

  // Are they awaiting deletion?
  if(PLAYER_HANDLER->test_appealing(_data["name"])) {
    write("Sorry that character has been marked for deletion pending "
          "appeal.  If you wish to appeal please contact the trustees at "
          ADMIN_EMAIL + ".\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }

  // The tests below don't apply to creators to creator test chars.
  if(PLAYER_HANDLER->test_creator(_data["name"]) ||
     PLAYER_HANDLER->test_property(_data["name"], "test character"))
    return "ok";


  // Player lockouts.
  tmp = BASTARDS->query_lockedout(_data["name"]);
  if(tmp) {
    write("Sorry that character has chosen to be locked out until\n" +
          ctime(tmp[SUSPEND_TIME]) + " for " + tmp[SUSPEND_REASON] + ".\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }

  // No alts permitted regardless of which site they're connecting from.
  if(PLAYERINFO_HANDLER->query_alt_of(_data["name"]))
    alts = PLAYERINFO_HANDLER->query_alts(PLAYERINFO_HANDLER->
                                          query_alt_of(_data["name"])) +
      ({ PLAYERINFO_HANDLER->query_alt_of(_data["name"]) });
  else
    alts = PLAYERINFO_HANDLER->query_alts(_data["name"]);
  if(sizeof(alts))
    alts -= ({ _data["name"] });
  if(sizeof(alts)) {
    tmp = filter(alts, (: LIVING->find_player($1) :));
    if(sizeof(tmp)) {
      write("Sorry, you cannot login while one of your alts is logged in.\n"
            "Press enter to continue ");
      return READ_INPUT;
    }
  }
  
  if(tmp = BASTARDS->query_suspended(_data["name"])) {
    write("You are suspended until " + ctime(tmp[SUSPEND_TIME]) +
          ", enter your password to see why.\n");
  }

  // Authorised? Then allow them in.
  if(PLAYER_HANDLER->test_property(_data["name"], "authorised player"))
    return "ok";

  // Check site (IP/DNS) access.
  switch(BASTARDS->query_access(this_object())) {
  case NO_ACCESS:
    write("Player logins are disabled from this site.  Please contact "
          "a creator to have your character authorised to login.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
    break;
  case NO_NEW:
  case AUTH_NEW:
    if(PLAYER_HANDLER->test_age(_data["name"]) > -(2 * 24 * 60 * 60)) {
      write("\nNew player logins are disabled from this site and your character "
            "is too young.  Please contact a creator to have your character "
            " authorised to login.\n\n"
            "Press enter to continue ");
      return READ_INPUT;
    }
    break;
  case ERROR:
    write("Sorry your site has invalid access permissions set.  Please "
          "contact a creator to have this corrected.\n\n"
            "Press enter to continue ");
    return READ_INPUT;
  default:
  }

  // Check multiplayer access.
  if(!BASTARDS->query_multi(this_object())) {
    tmp = filter(users(), 
                 (: interactive($1) &&
                  !$1->query_creator() &&
                  !$1->query_property("test character") &&
                  $1->query_name() != "logon" &&
                  $1->query_name() != $3 &&
                  !strcmp(query_ip_number($1), $2)
                  :), 
                 query_ip_number(this_object()), _data["name"]) -
      ({ this_object() });
    
    if(sizeof(MULTIPLAYER->check_allowed(this_object(), tmp))) {
      write("Sorry, someone else is already logged in from the same computer\n"
            "address as you and your site is not authorised for multiple "
            "players.\n\n"
            "Press enter to continue ");
      return READ_INPUT;
    }
  }
 
  return "ok";
}

/** @ignore yes
 * Verify that the password entered is correct for this user.
 */
mixed check_password(string event) {
  string mess;
  mixed *tmp;
  
  _data["password"] = replace_string(event, "input-", "");
  _counter++;
  
  if(_data["password"] == "")
    return "failed";

  // Check their password
  if(!PLAYER_HANDLER->test_password(_data["name"], _data["password"])) {

    // Check their temporary password (if they have one).
    tmp = BASTARDS->query_temp_password(_data["name"]);
    if(!tmp || crypt(_data["password"], tmp[1]) != tmp[1]) {
      if(_counter < 3) {
        write("\nPassword incorrect.\n");
        return "invalid";
      }
      
      // record a failed login from this IP.
      LOGIN_HANDLER->failed_login(query_ip_number(this_object()));
      write("\nToo many retries.\n\nPress enter to continue ");
      mess = capitalize(_data["name"]) + " failed to login, " +
        (query_ip_name(this_object()) != query_ip_number(this_object())
         ? query_ip_name(this_object()) + " (" +
         query_ip_number(this_object()) + ")"
         : query_ip_number(this_object()));
      event(users(), "inform", mess, "bad-password");
      log_file("BAD_PASSWORD", ctime(time())[4..18] + ": " + mess + "\n");
      return READ_INPUT;
    }
    "/cmds/lord/resetpw"->do_reset(_data["name"], _data["password"]);
    write("\n\nYour temporary password has now been used!  It is essential "
          "that you use the password command to set a new password.\n\n");
  }
  
  BASTARDS->clear_temp_password(_data["name"]);

  // Are they suspended?
  if(tmp = BASTARDS->query_suspended(_data["name"])) {
    write("\nSorry, you are suspended until " + 
          ctime(tmp[SUSPEND_TIME]) + " for " +
          tmp[SUSPEND_REASON] + ".\n\nPress enter to continue ");
    return READ_INPUT;
  }

  _data["password-check"] = "ok";

  // This is a character that has been pre-authorised and so doesn't
  // yet have a gender so send them off to receive one.
  if(!PLAYER_HANDLER->test_gender(_data["name"]) &&
     PLAYER_HANDLER->test_property(_data["name"], "new player"))
    return "new";
  
  // Ok now we have to actually log them in!
  return "ok";
}

/** @ignore yes
 * Check & set the gender for a new char
 */
mixed set_gender(string event) {
  string tmp;
  tmp = lower_case(replace_string(event, "input-", ""));

  if (tmp == "m") {
    tmp = "male";
  }
  if (tmp == "f") {
    tmp = "female";
  }

  if(tmp == "male" || tmp == "female") {
    _data["gender"] = tmp;
    return "ok";
  }
  write("That's too weird even for this game!\nTry male or female ");
  return "invalid";
}

/** @ignore yes
 * Check if someone is reconnecting when netdead.
 */
mixed check_reconnecting(string event) {
  if(_data["password-check"] != "ok") {
    write("Error\n");
    dest_me();
  }
  if(LIVING->find_player(_data["name"])) {
    write("You are already playing.\n"+
          "Throw the other copy out? (y/n/restart) ");
    return READ_INPUT;
  }
  return "no";
}

/** @ignore yes
 * Perform a restart for people reconnecting.
 */
mixed restart(string event) {
  object pl;
  
  pl = LIVING->find_player(_data["name"]);
  if(!pl) {
    write("Your other copy has been eaten.  Logging in normally.\n");
    return "ok";
  }

  tell_object(pl, "Disconnected by someone from " +
              query_ip_name(this_object()) + ".\n");
  if(catch(pl->quit()) && catch(pl->dest_me()))
    destruct(pl);
  if(objectp(pl))
    pl->dest_me();
  return "ok";
}

/** @ignore yes
 * Perform a reconnect for people reconnecting.
 */
mixed reconnect(string event) {
  object pl, ob;

  pl = LIVING->find_player(_data["name"]);
  if(!pl) {
    write("Your other copy has been eaten.  Logging in normally.\n");
    return "login";
  }

  if(interactive(pl)) {
    tell_object(pl, "Disconnected by someone from " +
                query_ip_name(this_object()) + ".\n");
    ob = clone_object("/std/object");
    exec(ob, pl);
    ob->dest_me();
  }
  exec(pl, this_object());
  pl->look_me();

  LOGIN_HANDLER->player_reconnected(pl->query_name());
  
  tell_room(environment(pl), pl->query_cap_name()+
            " has reconnected.\n", ({ pl }) );

  if (function_exists("inform_reconnect_game", pl))
    pl->inform_reconnect_game();

  if(query_ip_number(pl) == query_ip_name(pl))
     resolve(query_ip_number(pl), "");

  return "ok";
}

/** @ignore yes
 * Check if we're full.
 */
mixed check_full(string event) {
  int ret;

  //  if(_data["name"] == "ctest")
  //return "full";
  
  if(PLAYER_HANDLER->test_creator(_data["name"]) ||
     PLAYER_HANDLER->test_property(_data["name"], "test character"))
    return "ok";
  
  ret = LOGIN_HANDLER->int_is_discworld_full(WITHOUT_LOGINS_NOT_IN_QUEUE);
  if((ret & UNCOMPRESSED_FULL) && 
     ((ret & COMPRESSED_FULL) || !compressedp(this_object()))) {
    write("Sorry, there are no player slots available.\n\n");
    return "full";
  }

  return "ok";
}

/** @ignore yes
 * Add this person to the login queue.
 */
mixed add_to_queue(string event) {
  object *obs, pl, ob;
  
  if(member_array(_data["name"], map(LOGIN_HANDLER->query_login_queue(),
                                     (: $1->query_name() :) )) != -1 ) {
    write("You are already in the login queue.\n");
    obs = filter(LOGIN_HANDLER->query_login_queue(),
                 (: $1->query_name() == $2 :), _data["name"]);
    
    pl = obs[0];
    if (pl && pl->query_login_ob()) {
      write("Reconnecting you to the login queue.\n");
      
      if (interactive(pl)) {
        tell_object(pl, "Disconnected by someone from " +
                    query_ip_name(this_object()) + ".\n");
        ob = clone_object("/std/object");
        exec(ob, pl);
        ob->dest_me();
      }
      ob = this_object();
      exec(pl, ob);
      return "ok";
    }
  }

  LOGIN_HANDLER->add_to_login_queue(this_object());
  pl = LIVING->find_player(_data["name"]);
  if(pl) {
    /*
     * Hack!  Hack!  Warning!  No idea why reconnection to net
     * deads is not working.  So force them to quit instead.
     */
    //pl->quit();
    write("You were net dead when you left, shuffling you to the "
          "start of the queue...\n");
  } else {
    write("Placing you in the login queue: you have position "+
          sizeof(LOGIN_HANDLER->query_login_queue()) + 
          ".\nPlease wait.  Type \"quit\" to escape.\n" );
  }
  remove_call_out("time_out");
  call_out("check_status", 15);
  return "ok";
}

/** @ignore yes
 * Show the Terms and Conditions and setup the callout waiting for them
 * to agree.
 */
mixed display_terms(string event) {
  write(LOGIN_HANDLER->get_message(FILE_PATH+ "/TERMS"));
  call_out("terms_delayed", 30);
  return "ok";
}

/** @ignore yes
 * Ask the player to agree to the terms.
 */
void terms_delayed() {
  _counter = 0;
  write("\nEnter 'yes' if you agree to the terms or 'no' if you cannot.\n"
        "By typing 'yes' you signify that you have read, understand and "
        "agree to by bound by these terms and conditions: [yes/no] ");
  enter_state("agree-terms");
}

/** @ignore yes
 * See if they accept the terms or not.
 */
mixed check_terms(string event) {
  if(event == "input-yes")
    return "yes";
  if(event == "input-no" || _counter++ > 3)
    return "no";
  return "error";
}

/** @ignore yes
 * Send a password to a new player connecting from a site that only
 * allows authenticated logins.
 */
mixed new_player_auth(string event) {
  object pl;
  string pass;
  
  if(catch(pl = clone_object(BASTARDS->query_player_ob(_data["name"])))) {
    write("Oh no, someone has broken the player object!  "+
            "Come back later.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }
  
  if(!pl) {
    write("Something bad happened.\n\n"
          "Press enter to continue ");
    return READ_INPUT;
  }

  pl->set_name(_data["name"]);
  pass = generate_password(); 
  pl->set_password(crypt(pass, 0));
  pl->set_email(_data["email"]);
  pl->add_property("authorised player", 1);
  pl->add_property("new player", 1);
  pl->add_property("authorised email", _data["email"]);
  pl->allow_save();
  pl->save_me();
  pl->dest_me();
  MAILER->do_mail_message(_data["email"], "admin", "Your password", 0,
                          "The password for " + _data["name"] + " is " + 
                          pass + "\n\n");
  write("An email is on its way to you with a password for this "
        "character.\n\n"
        "Press enter to continue ");
  return READ_INPUT;
}

/** @ignore yes
 * This actually logs the new player in. It uses exec() to pass control
 * from this object to the newly cloned player object.
 */
mixed new_player_login(string event) {
  object pl, tp;

  if(catch(pl = clone_object(BASTARDS->query_player_ob(_data["name"])))) {
    write("Oh no, someone has broken the player object!  "+
            "Come back later.\n" );
    return "error";
  }

  if(!pl) {
    write("Something bad happened.\n");
    return "error";
  }

  // Have to copy these across if they've been authorised already.
  if(PLAYER_HANDLER->test_property(_data["name"], "authorised player")) {
    pl->add_property("authorised player", 1);
    pl->add_property("authorised email",
                     PLAYER_HANDLER->test_property(_data["name"],
                                                   "authorised email"));
    pl->set_email(PLAYER_HANDLER->test_property(_data["name"],
                                                "authorised email"));
  }

  pl->set_name(_data["name"]);
  if(_data["password"])
    pl->set_password(crypt(_data["password"], 0));

  if(_data["guest"]) {
    pl->add_property("guest", 1);
    pl->set_title("guest of Discworld");
  }
  pl->set_gender(_data["gender"]);
  pl->set_language("general");
  pl->set_default_language("general");

  tp = this_object();
  if(!exec(pl, tp)) {
    write("Oh dear, something went wrong.\n");
    return "error";
  }
  
  if(tp != this_object())
    tp->quit();

  write("\n");
  pl->move_player_to_start(_data["name"], !_data["guest"], _data["cap_name"],
                           "", _data["go_invis"]);

  if(query_ip_number(pl) == query_ip_name(pl))
    resolve(query_ip_number(pl), "");

  return "ok";
}

/** @ignore yes
 * This actually logs the player in. It uses exec() to pass control from this
 * object to the newly cloned player object.
 */
mixed player_login(string event) {
  object pl, tp;
  string cap_name;
  
  if(!interactive(this_object())) {
    debug_log("This ob not interactive. %s %s %s %s", _state, _last_state, 
              _event, _last_event);
    dest_me();
  }
  cap_name = PLAYER_HANDLER->query_cap_name(_data["name"]);

  if(file_size(PLAYER_HANDLER->query_delete_player_file_name(_data["name"]) +
               ".o") > 0 &&
     unguarded((: rename,
                PLAYER_HANDLER->query_delete_player_file_name(_data["name"]) +
                ".o",
                PLAYER_HANDLER->query_player_file_name(_data["name"]) +
                ".o" :)))
    write("Removing player from delete queue.\n");
  else if(file_size(PLAYER_HANDLER->
                    query_delete_player_file_name(_data["name"]) + ".o.gz") > 0
          &&
          unguarded((: rename,
                     PLAYER_HANDLER->query_delete_player_file_name(_data["name"]) +
                     ".o.gz",
                    PLAYER_HANDLER->query_player_file_name(_data["name"]) +
                     ".o.gz" :)))
    write("Removing player from delete queue.\n");
  
  if(catch(pl = clone_object(BASTARDS->query_player_ob(_data["name"])))) {
    write("Oh no, someone has broken the player object!  "+
            "Come back later.\n" );
    return "error";
  }

  if(!pl) {
    write("Something bad happened.\n");
    return "error";
  }

  tp = this_object();
  if(!exec(pl, tp)) {
    write("Oh dear, something went wrong.\n");
    return "error";
  }
  
  if(tp != this_object())
    tp->quit();

  write("\n");
  pl->move_player_to_start(_data["name"], 0, cap_name, 0, _data["go_invis"]);
  if(_terminal_name)
    pl->terminal_type(_terminal_name);

  if(_cols && _rows)
    pl->window_size(_cols, _rows);

  if(query_ip_number(pl) == query_ip_name(pl))
     resolve(query_ip_number(pl), "");

  return "ok";
}

/** @ignore yes */
mixed exit_queue(string event) {
  call_out("time_out", TIMEOUT_TIME);
  if(LIVING->find_player(_data["name"]))
    return "reconnect";
  else
    return "login";
}

/** @ignore yes
 * This is called by the login handler to remove the person from the queue.
 */
void remove_from_login_queue() {
  LOGIN_HANDLER->remove_from_login_queue(this_object());
  tell_object(this_object(), "\n\nYou have exited the login queue!\n\n"
              "Please press enter to continue ");

  enter_state("leaving-queue");
}


mixed quit(string event) {
  write("Come back soon!\n");
  dest_me();
}

/** @ignore yes */
mixed time_out() {
  if(!interactive(this_object()))
    return dest_me();
  
  /* time them out if they are not in the queue and either idle or
   * not idle but have been sitting around too long
   */
  if((query_idle(this_object()) > TIMEOUT_TIME ||
      (time() > _login_start_time + THROWOUT_TIME)) &&
     member_array(this_object(), LOGIN_HANDLER->query_login_queue()) == -1) {
    write("\nTime out.\n\n");
    return dest_me();
  }

  call_out( "time_out", TIMEOUT_TIME);
  return;
}

/** @ignore yes */
void check_status() {
  int pos;

  if(_data["ok_to_login"])
    return;
  
  pos = member_array(this_object(), LOGIN_HANDLER->query_login_queue())+1;
  if(!pos) {
    /* Oh dear, we got deleted...  Put ourselves back in */
    LOGIN_HANDLER->add_to_login_queue(this_object());
    pos = member_array(this_object(), LOGIN_HANDLER->query_login_queue())+1;
  }

  if(pos && pos != _data["login_pos"])
    write("You now have position " + pos + " in the queue.\n");

  _data["login_pos"] = pos;
  write(sprintf("%-=*s\n", 79,
                  implode(QUOTE_HANDLER->query_random_quote(), "\n")));
  call_out("check_status", 30);
}

/** @ignore yes */
int do_su(string str) {
  object ob, tp;

  ob = this_player();
  tp = this_object();

  _terminal_name = ob->query_cur_term();
  _cols = ob->query_cols();
  _rows = ob->query_rows();

  exec(tp, ob);
  ob->quit();

  _data["name"] = lower_case(str);
  debug_log("Suing to %s", str);
  enter_state("check-login-access");

  return 1;
}

/** @ignore yes
 * Used by the creator upgrade command. This goes directly to player-login
 * after destructing the old object.
 */
int do_upgrade(object old) {
  old->save();
  _data["name"] = old->query_name();
  exec(this_object(), old);
  destruct(old);
  enter_state("player-login");
}

/** @ignore yes
 * This function is called when someone connects to the mud, it's what starts
 * it all off.
 */
void logon() {
  debug_log("Connection received.");
  if(find_call_out("time_out") == -1)
    call_out("time_out", TIMEOUT_TIME);
  _login_start_time = time();
  
  enter_state("main-menu");
}

/** @ignore yes */
int query_prevent_shadow() { return 1; }

/** @ignore yes */
void do_efun_write(string str) {
  efun::tell_object(this_object(), sprintf("%-=*s", 79, str));
}

/** @ignore yes */
string query_object_type() { return "X"; }

/** @ignore yes */
string query_gender_string() { return "blue"; }

/** @ignore yes */
void terminal_type(string type) {
  _terminal_name = type;
} /* terminal_type() */

/** @ignore yes */
void window_size(int width, int height) {
  _cols = width;
  _rows = height;
} /* window_size() */

/** @ignore yes */
void net_dead() {
  LOGIN_HANDLER->remove_from_login_queue(this_object());
}

/** @ignore yes */
void dest_me() {
  LOGIN_HANDLER->remove_from_login_queue(this_object());
  destruct(this_object());
}

/**
 * This method determines if the object is a login object.
 * @return always returns 1
 */
int query_login_ob() { return 1; }

/**
 * This method returns the path to the player file.
 * This is the player files actual normal location, it was written to
 * allow moving the player files aruond easier.
 * @param name the name of hte player whose file to find
 * @see test_user()
 * @see query_delete_player_file_name()
 */
string query_player_file_name(string name) {
  return PLAYER_HANDLER->query_player_file_name(name);
}

/**
 * This method returns the path to the deleted player file.
 * This is the deleted player files actual normal location, it was written to
 * allow moving the player files aruond easier.
 * @param name the name of hte player whose file to find
 * @see test_user()
 * @see query_delete_player_file_name()
 */
string query_delete_player_file_name(string name) {
   return PLAYER_HANDLER->query_delete_player_file_name(name);
}

/** @ignore yes */
string query_name() {
  if(_data && _data["name"])
    return _data["name"];
  return "logon";
}

/** @ignore yes */
string query_cap_name() {
  if(_data && _data["cap_name"])
    return _data["cap_name"];
  return "Logon";
}

/** @ignore yes */
protected void write_prompt() { return; }
