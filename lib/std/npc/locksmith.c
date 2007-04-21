/*
 *
 *$Id: locksmith.c,v 1.8 2002/10/11 10:32:25 wirble Exp $
 *
 */

/**
 * -- Inheritable for playerhousing locksmiths --
 * An NPC using this inherit is able to unlock the doors to playerhouses and
 * commercial properties when prompted to do so by the rightful owner.
 *
 * -- How such an NPC works from a player's point of view --
 * - A player can "hire" the npc if he wants a door of his house opened.
 * - He then has to "pay" (*) the amount the npc demands (if that player killed
 *   the npc before, the base price is doubled).  He has 120 seconds to do so.
 * - After that happens, the npc will follow the player until the player
 *   "asks"s him to opens an exit door.
 * - Then player is prompted to enter [y]es [n]o".  If the player confirms and
 *   the handler doesn't return this as well, the npc complains and walks off
 *   (keeping the money, of course).  Otherwise it unlocks the door and then
 *   leaves.
 *
 * (*) Giving the money to the npc does not work, it'll return it.
 *
 * The same procedure applies if the player wants all doors opened.
 *
 * All attempts to hire a locksmith with the result of that action are logged
 * in the file defined in LOG
 *
 * In case of problems you can always try to:
 * 1) look at the log to see if the player in question hired and payed the npc
 * 2) call query_client() on the npc
 * 3) call free_for_hire() on the npc
 * 4) dest and re-load the npc
 *
 *
 * @example
 * inherit "/std/npc/types/locksmith";
 *
 * void setup() {
 *     // basic npc-setup
 *     set_savefile(SAVE + "glodson_killers");
 *     set_price(20000); // 50 $AM
 *     set_currency_region("Ankh-Morpork");
 *     set_domain("/d/am/");
 * }
 *
 * @author Wirble
 * @started Nov. 2001
 */

#include <armoury.h>
#include <money.h>

#define LOG "/log/LOCKSMITH"
#define LOGGING 1

inherit "/obj/monster";


//private functions
private void save_killers();
private void load_killers();
private void remove_killer(string name);
private void clear_false_client();
private void payment_timeout();
private int check_owner();
private void unlock(object door);
private string get_price();
private int calc_price();
private void generic_responses();
private void go_back();
private void unlock_all( string path );
private void delay_opening( string env );
void finish_opening( string path, string env );

// public functions
public void confirmation(string response, object player);
public void free_for_hire();
public string query_client();
public void move_recording();
public string* query_killers();
public string* query_responses();
public string query_domain();
public string query_last_room();

//setup functions
protected int set_savefile(string file);
protected int set_price(int price);
protected int set_currency_region(string region);
protected int set_responses(string *responses);
protected int set_domain(string domain);

// command functions
public int do_hire( int all );
public int do_pay();
public int do_ask(string exit);

// Variables
private string _client;
private string* _killers;
private int _price;
private int _paid;
private int _all_doors;
private object _door;
private string _savefile;
private string _region;
private string* _responses;
private string _domain;
private string _last_room;

// -------------------- basic functions --------------------

/** @ignore */
void create() {
  generic_responses();
  do_setup++;
  ::create();
  do_setup--;  
  if (!do_setup)
    this_object()->setup();
  add_enter_commands("#move_recording");
}


/** @ignore */
void init() {
  ::init();
  if(interactive(this_player())) {
    this_player()->add_command("hire", this_object(), "<direct>",
                                  (: do_hire( 0 ) :) );
    this_player()->add_command("hire", this_object(),
                                   "<direct> to unlock an entire house",
                                   (: do_hire( 1 ) :) );
    this_player()->add_command("pay", this_object(), "<direct>");
    this_player()->add_command("ask", this_object(),
                                "<direct> to {open|unlock} [the] "
                                "<word'exit'> door",
                                (: do_ask($4[2]) :));
    this_player()->add_command("confirm", this_object(), "");

  }
}


/** @ignore */
void reset() {
  clear_false_client();
}

// -------------------- public functions --------------------

/**
 * Use this function if something went wrong and the NPC does not react to the
 * client/new clients anymore.
 */
public void free_for_hire() {
  _client = 0;
  _paid = 0;
  _door = 0;
  _all_doors = 0;
  set_move_after(60, 60);
}


/**
 * This queries the client's name.
 * @return the name of the client or 0 for no client
 */
public string query_client() {
  if(!_client)
    return 0;
  return _client;
}


/**
 * This function gets called after every move of the NPC and records it for a
 * potential return.  It should not be called manually.
 */
public void move_recording() {
  object room = environment(this_object());
  if(!room) {
    if(query_name() != "object")
      call_out("dest_me", 5);
    return;
  }
  if(_client) {
    // check if the NPC is still in the designated domain
    if(_domain && base_name(room)[0..5] == _domain) {
      // record the room if it's not inside housing
      if(!room->query_owner())
        _last_room = base_name(room);
      return;
    }
    do_command("sayto " + _client + " " + _responses[11]);
    go_back();
  }
}


/**
 * This queries the killers of the NPC.
 * @return an array listing the killers' names
 */
public string* query_killers() {
  return _killers;
}


/**
 * This queries the NPC's domain.
 * @return the first 6 chars of the domain.
 */
public string query_domain() {
  return _domain;
}


/**
 * This queries the last valid room the NPC was in.
 * @return the basename of the last room
 */
public string query_last_room() {
  return _last_room;
}


/**
 * This function is used to evaluate the client's response to the "is this the
 * door"-question.
 * @param response the response the player entered
 * @param player the player doing the input
 */
public void confirmation(string response, object player) {
  if(!response || !player)
    return;
  switch(lower_case(response)) {
  case "yes":
  case "y":
    break;
  case "no":
  case "n":
    do_command("nod");
    do_command("follow " + player->query_name());
    _door = 0;
    return;
  default:
    do_command("sayto " + player->query_name() + " " + _responses[0]);
    return;
  }
  if(!_client || player->query_name() != _client) {
    do_command("sayto " + player->query_name() + " " + _responses[1]);
    return;
  }
  // If the destination doesn't belong to the client, abort and move away
  if(!check_owner()) {
    do_command("sayto " + player->query_name() + " " + _responses[2]);
#ifdef LOGGING
    log_file(LOG, "%s: %s tried to get %s to unlock door in %s leading "
              "to %s.\n", ctime(time()), _client, query_short(),
              base_name(environment(this_object())),_door->query_dest());
#endif
  } else if( _all_doors == 0 ) {
    queue_command("emote eyes the lock on the door.", 1);
    queue_command("emote fiddles around on the lock with some wires.", 5);
    queue_command("emote seems satisfied.", 5);
    unlock(_door);

    queue_command("sayto " + player->query_name() + " " + _responses[3], 2);
#ifdef LOGGING
    log_file(LOG,
              "%s: %s successfully got %s to unlock door in %s leading %s.\n",
              ctime(time()), _client, query_short(),
              base_name(environment(this_object())), _door->query_dest());
#endif
  } else {
    queue_command("emote eyes the lock on the door.", 1);
    queue_command("emote fiddles around on the lock with some wires.", 5);
    queue_command("'Stand back and wait for me.", 5 );
    queue_command("emote opens the door and disappears into the house, locking "
                      "the door after " + query_objective() + ".", 2 );
    delay_opening( base_name( environment( this_object() ) ) );
    return;
  }
  free_for_hire();
  go_back();
}


/**
 * This queries the responses of the NPC.
 * @return an array of response-strings
 */
public string* query_responses() {
  return _responses;
}

// -------------------- setup functions --------------------

/**
 * Use this function to set the file where the killers of the NPC are stored,
 * if you want to charge the killers the double price for opening their house.
 * @param file the name of the file to save to
 * @return 1 on success, 0 on failure
 */
protected int set_savefile(string file) {
  if(!file)
    return 0;
  _savefile = file;
  load_killers();
  return 1;
}


/**
 * This function sets the base price for unlocking a door.  If the savefile is
 * set, a killer will be charged the double price.
 * @param price the base-price in money-units
 * @return 1 on success, 0 on failure
 */
protected int set_price(int price) {
  if(!price)
    return 0;
  _price = price;
  return 1;
}


/**
 * This function sets the NPCs region for determining the currency.
 * @param region the region to use
 * @return 1 on success, 0 on failure
 */
protected int set_currency_region(string region) {
  if(!region)
    return 0;
  _region = region;
  return 1;
}


/**
 * This sets the domain the NPC is restricted to.
 * @param domain the first 6 letters of the domain-path, ie "/d/am/"
 * @return 1 on success, 0 on failure
 */
protected int set_domain(string domain) {
  if(sizeof(domain) != 6)
    return 0;
  _domain = domain;
}


/**
 * This function sets the NPC's responses for the various commands.  If not
 * used generic answers will be used.  Mind that you cannot enter only a few
 * responses.  It's all or nothing.  The array should be like this:
 * 1) response to failed confirmation
 * 2) response for not hired by that player
 * 3) response for trying to enter another player's house
 * 4) response for successful opening of the door
 * 5) response for already hired by other player
 * 6) response for not paid yet
 * 7) response for attempt to hire
 * 8) response for already paid
 * 9) response for not enough money to pay
 * 10) response for cannot find specified exit
 * 11) response for successful payment (please include 'ask' hint)
 * 12) response for not leaving the domain
 * 13) response for already hired by that player
 * @param responses an array with the responses, as explained above
 * @return 1 on success, 0 on failure
 */
protected int set_responses(string* responses) {
  if(sizeof(responses) != 13)
    return 0;
  _responses = responses;
}

// -------------------- command functions --------------------

/** @ignore */
public int do_hire( int all ) {
  if(!this_player())
    return 0;
  // If the prospective client is invis to the NPC, abort
  if(!this_player()->query_visible(this_object())) {
    this_player()->add_failed_mess(this_object(),
                                    "$C$" + this_object()->the_short() +
                                   " cannot even see you.\n", ({ }));
    return 0;
  }
  // If the NPC is already hired by someone else, abort
  if(_client && this_player()->query_name() != _client) {
    do_command("sayto " + this_player()->query_name() + " " + _responses[4]);
    this_player()->add_failed_mess(this_object(), "", ({ }));
    return 0;
  }
  // If he is already hired by the player asking, abort
  if(_client && this_player()->query_name() == _client) {
    if(_paid) {
      if(member_array(this_object(), this_player()->query_followers())
          == -1) {
        do_command("nod " + this_player()->query_name());
        do_command("follow " + this_player()->query_name());
        this_player()->add_succeeded_mess(this_object(), "", ({ }));
        return 1;
      }
      do_command("sayto " + this_player()->query_name() + " " +
                  _responses[12]);
      this_player()->add_failed_mess(this_object(), "", ({ }));
      return 0;
    }
    do_command("sayto " + this_player()->query_name() + " " +
                _responses[5]);
    this_player()->add_failed_mess(this_object(), "", ({ }));
    return 0;
  }
  // Otherwise accept
  _client = this_player()->query_name();
  _all_doors = all;
  queue_command("sayto " + this_player()->query_name() + " " +
                 _responses[6], 1);
  if(member_array(_client, _killers) != -1)
    queue_command("sayto " + this_player()->query_name() +
                   " I remember you... for you it'll be " + get_price() +
                   ".  Up front, of course.", 2);
  else
    queue_command("sayto " + this_player()->query_name() + " That'll be "+
                   get_price() + " up front.", 2);
  this_player()->add_succeeded_mess(this_object(), "", ({ }));
  call_out((: payment_timeout() :), 60);
  return 1;
}


/** @ignore */
public int do_pay() {
  if(!this_player())
    return 0;
  // If the player is invis to the NPC, abort
  if(!this_player()->query_visible(this_object())) {
    this_player()->add_failed_mess(this_object(),
                                    "$C$" + this_object()->the_short() +
                                   " cannot even see you.\n", ({ }));
    return 0;
  }
  // Only accept payment from the client
  if(!_client || this_player()->query_name() != _client) {
    do_command("whisper " + this_player()->query_name() + " I appreciate "
                "that you want to give away your cash, but don't you think "
                "should 'hire' me first?");
    this_player()->add_failed_mess(this_object(), "", ({ }));
    return 0;
  }
  // If he has already been paid, abort
  if(_paid) {
    do_command("sayto " + this_player()->query_name() + " " + _responses[7]);
    this_player()->add_failed_mess(this_object(), "", ({ }));
    return 0;
  }
  // In case the client can't pay, abort
  if(this_player()->query_value_in(_region) < _price) {
    do_command("sayto " + this_player()->query_name() + " " +
                _responses[8]);
    free_for_hire();
    this_player()->add_failed_mess(this_object(), "", ({ }));
    return 0;
  }
  this_player()->pay_money(MONEY_HAND->create_money_array(calc_price(),
                                                            _region), _region);
  _paid = 1;
  set_move_after(1200, 0);
  do_command("emote puts the money into a pocket.");
  do_command("whisper " + this_player()->query_name() + " " + _responses[10]);
  init_command("follow " + this_player()->query_name(), 1);
  this_player()->add_succeeded_mess(this_object(), "$N pay$s $D.\n", ({ }));
#ifdef LOGGING
  log_file(LOG, "%s: %s hired %s for %s" +
      ( (_all_doors == 1)?" (all doors)":"" ) + "\n",
      ctime(time()), _client, query_short(), get_price() );
#endif
  remove_killer(_client);
  return 1;
}


/** @ignore */
public int do_ask(string exit) {
  object room, door;
  string dest;

  room = environment(this_object());
  if(!this_player() || !exit || !room)
    return 0;
  // If the player is invis to the NPC, abort
  if(!this_player()->query_visible(this_object())) {
    this_player()->add_failed_mess(this_object(),
                                    "$C$" + this_object()->the_short() +
                                   " cannot even see you.\n", ({ }));
    return 0;
  }
  // If the player asking is not the client, abort
  if(!_client || this_player()->query_name() != _client) {
    do_command("sayto " + this_player()->query_name() + " " + _responses[1]);
    this_player()->add_failed_mess(this_object(),
                                   "You are not " + the_short() +
                                   "'s client.\n", ({ }));
    return 0;
  }
  do_command("unfollow " + this_player()->query_name());
  dest = this_player()->find_abs(exit);
  if(room->query_destination(dest) == "/room/void") {
    do_command("sayto " + this_player()->query_name() + " " +
                _responses[9]);
    do_command("follow " + this_player()->query_name());
    this_player()->add_failed_mess(this_object(),
                                    "This exit is not valid.\n", ({ }));
    return 0;
  }
  foreach(door in room->query_hidden_objects())
    if(door->query_dest() == room->query_destination(dest)) {
      _door = door;
      break;
    }
  if( _all_doors == 0 )
    do_command("whisper " + this_player()->query_name() + " Can you confirm "
                "that this is your place?  And that you want the " + exit +
                " door opened?");
  else
    do_command("whisper " + this_player()->query_name() + " So that's your "
                "place on the other side?  Can you confirm that you want the " +
                exit + " door and all the others in your house opened?" );
  this_player()->add_succeeded_mess(this_object(), "", ({ }));
  tell_object(this_player(), "[y]es/[n]o: ");
  input_to("confirmation", 0, this_player());
  return 1;
}

// -------------------- internals --------------------

/** @ignore */
void event_enter(object ob, string message, object from) {
  if(from && environment(ob) == this_object()) {
    if(ob->query_name() == "coin") {
      do_command("sayto " + file_name(from) + " You have to pay me.");
      do_command("give coins to " + file_name(from));
      return;
    }
    do_command("sayto " + file_name(from) + " I don't take donations.");
    do_command("give " + file_name(ob) + " to " + file_name(from));
  }
}


/** @ignore */
private void save_killers() {
  string killerstring, killer;

  if(!_savefile)
    return;
  if(!sizeof(_killers)) {
    unguarded((: write_file($(_savefile), "--- no killers ---", 1) :));
    return;
  }
  killerstring = "";
  foreach(killer in _killers)
    killerstring += killer + " ";
  unguarded((: write_file($(_savefile), $(killerstring), 1) :));
}


/** @ignore */
private void load_killers() {
  string killerstring;

  if(!_savefile)
    return;
  if(file_size(_savefile) == -1) {
    _killers = ({ });
    return;
  }
  killerstring = unguarded((: read_file($(_savefile)) :));
  if( !killerstring || killerstring == "--- no killers ---" ||
      !_killers = explode(killerstring, " "))
    _killers = ({ });
}


/** @ignore */
varargs object do_death(object killer, object weapon, string attack) {
  if(killer && interactive(killer)) {
    if(member_array(killer->query_name(), _killers) == -1) {
      if (!_killers) {
         _killers = ({ });
      }
      _killers += ({ killer->query_name() });
    }
  }
  save_killers();
  if(weapon && attack)
    return ::do_death(killer, weapon, attack);
  if(weapon)
    return ::do_death(killer, weapon);
  if(killer)
    return ::do_death(killer);
  return ::do_death();
}


/** @ignore */
private void remove_killer(string name) {
  int index;

  index = member_array(name, _killers);
  if(index == -1)
    return;
  if(sizeof(_killers) == 1)
    _killers = ({ });
  if(index == 0)
    _killers = _killers[ 1.. ];
  if(index == sizeof(_killers))
    _killers = _killers[ 0..(sizeof(_killers) -1) ];
  _killers = _killers[ 0..(index - 1) ] + _killers[ (index + 1).. ];
}

/** @ignore */
private void clear_false_client() {
  if(_client && find_living(_client) &&
      environment(find_living(_client)) == environment(this_object()))
    return;
  free_for_hire();
}


/** @ignore */
private void payment_timeout() {
  if(_client && !_paid)
    free_for_hire();
}


/** @ignore */
private int check_owner() {
  string dest = _door->query_dest();

  if(load_object(dest)->query_owner() == _client)
    return 1;
  return 0;
}

/** @ignore */
private void unlock(object door) {
  door->set_unlocked();
  event( load_object( door->query_dest() ), "save");
  event( environment(), "save");
}

/** @ignore */
private string get_price() {
  int price;
  if(member_array(_client, _killers) != -1)
    price = 2 * _price;
  else
    price = _price;
  if( _all_doors == 1 )
    price*= 5;
  return MONEY_HAND->money_string(MONEY_HAND->create_money_array(price,
                                                                 _region));
}


/** @ignore */
private int calc_price() {
  int price;
  if(member_array(_client, _killers) != -1)
    price = 2 * _price;
  price = _price;
  if( _all_doors == 1 )
    price *= 5;
  return price;
}


/** @ignore */
private void go_back() {
  object room, door;
  int i;
  if(!room = environment(this_object()))
    return;
  if(!_last_room || base_name(room) == _last_room) {
    do_command(room->query_exits()[random(sizeof(room->query_exits()))]);
    return;
  }
  i = member_array(_last_room, room->query_dest_dir());
  if(i == -1)
    move(_last_room, "$N walk$s in.", "$N walk$s away.");
  else {
    foreach(door in room->query_hidden_objects())
      if(door->query_dest() == _last_room &&
          door->query_locked()) {
        move(_last_room, "$N walk$s in, locking the door.",
              "$N unlock$s the door and leave$s, locking it "
              "again.");
        return;
      }
    do_command(room->query_dest_dir()[i-1]);
  }
}


/** @ignore */
private void generic_responses() {
  _responses = ({0,0,0,0,0,0,0,0,0,0,0,0,0});
  _responses[0] = "I do not understand.";
  _responses[1] = "You have to hire me.";
  _responses[2] = "This door does not lead to your house.  I cannot help you.";
  _responses[3] = "It has been a pleasure doing business with you.";
  _responses[4] = "I am already hired by someone else.";
  _responses[5] = "You have to 'pay' me.";
  _responses[6] = "You do require my services?";
  _responses[7] = "You already paid me.";
  _responses[8] = "You do not have enough money.";
  _responses[9] = "I cannot find that door.";
  _responses[10] = "Very well.  Please lead me to your house and 'ask' me to "
    "open the door that troubles you.";
  _responses[11] = "I won't leave my beloved home, sorry.  I'll wait there "
    "for a while, if you change your mind and come back.";
  _responses[12] = "You already hired me.";
}

/** @ignore */
private void unlock_all( string path ) {
    string* files = get_dir( path + "*.c" );
    string file;
    object room, ob;

    if( !sizeof( files ) )
        return;
    foreach( file in files ) {
        room = load_object( path + file );
        if( room ) {
            foreach( ob in room->query_hidden_objects() )
                if( ob->query_dest() )
                     ob->set_unlocked();
            event( room, "save" );
        }
    }
}

/** @ignore */
private void delay_opening( string env ) {
    string path;
    string* path_bits;
    path_bits = explode( env, "/" );
    path_bits[ sizeof( path_bits ) -1 ] = "";
    path = "/" + implode( path_bits, "/" );
    move( "/room/void" );
    call_out( "finish_opening", 30, path, env );
}

/** @ignore */
void finish_opening( string path, string env ) {
    unlock_all( path );
#ifdef LOGGING
    log_file(LOG,
              "%s: %s successfully got %s to unlock all doors in %s.\n",
              ctime(time()), _client, query_short(), path );
#endif
    move( env, "$N leaves the house, rubbing " + query_objective() +
           " hands.\n", "" );
    free_for_hire();
    go_back();
}
