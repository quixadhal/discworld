/**
 * The base chunk for doing multiplayer board games.
 * @author Pinkfish
 * @started Thu Dec  7 01:27:46 PST 2000
 */
private inherit "/std/basic/auto_load";

#define LOAD_TAG "multiplayer base"

class player_info {
   int number;
   string cap_name;
   string name;
   mixed data;
}

private mapping _players;
private string _winner;
private string* _ids_when_started;
private string _current_player;
private int _minimum_needed;
private int _maximum_allowed;
private int _can_join_as_new;
private int _game_started;

string* query_currently_playing_ids();
void delete_gone_players();
string find_player_id_of_person(object person);

void create() {
   _players = ([ ]);
   _ids_when_started = ({ });
} /* create() */

/**
 * This method should be defined in the top level, it should also
 * call this method to setup stuff as needed when a game starts.
 */
void reset_game() {
//   renumber_players();
} /* reset_game() */

/**
 * This method should be called when a game starts.
 * @return 1 if started, 0 if not enough players
 */
int start_game() {
   delete_gone_players();
   if (sizeof(query_currently_playing_ids()) < _minimum_needed) {
      return 0;
   }
   _winner = 0;
   _game_started = 1;
   _ids_when_started = query_currently_playing_ids();
   //
   // Pick one of these guys to start with.
   //
   _current_player = _ids_when_started[random(sizeof(_ids_when_started))];
   return 1;
} /* start_game() */

/**
 * This method is called when the game ends to set the winner.
 * @param winner the winner
 */
void finish_game(string winner) {
   _winner = winner;
   _game_started = 0;
} /* finish_game() */

/**
 * This method returns the current winner of the game.
 * @return the winner of the game
 */
string query_winner() {
   return _winner;
} /* query_winner() */

/**
 * This method checks to see if the game has started.
 * @return 1 if it has, 0 if it has not
 */
int is_game_started() {
   return _game_started;
} /* is_game_started() */

/**
 * This method checks to see if you can join as a new player, or only
 * occupy the slots from the start of the game.
 * @return 1 if you can join as new
 */
int can_join_as_new_player() {
   return _can_join_as_new;
} /* can_join_as_new_player() */

/**
 * This method sets the flag to allow you to make the game allow players
 * to join after they have started the game.
 * @param join 1 if they can join after the game has started
 */
void set_can_join_as_new_player(int join) {
   _can_join_as_new = join;
} /* set_can_join_as_new_player() */

/**
 * This method checks to see if the person is here.
 * @param person the person to check
 * @return 1 if they are, 0 if not
 */
int is_person_playing(string id) {
   if (_players[id]->name) {
      if (find_player(_players[id]->name) &&
          is_in_me_or_environment(this_object(),
                                  find_player(_players[id]->name)) &&
          interactive(find_player(_players[id]->name))) {
         return 1;
      }
   }
   return 0;
} /* is_person_player() */

/**
 * This method checks to see if the player is player.
 * @return 1 if they are playing, 0 if not
 */
int is_playing(object ob) {
   string id;

   id = find_player_id_of_person(ob);
   return id != 0;
} /* is_playing() */

/**
 * This method checks to see if the object is the current player or not.
 * @return 1 if they are, 0 if not
 */
int is_current_player(object ob) {
   string id;

   id = find_player_id_of_person(ob);
   return id == _current_player;
} /* is_current_player() */

/**
 * This method lets the person join the game.
 * @param id the id of the person to add
 * @param person the person to join
 * @return 1 on success, 0 on failure
 */
int add_person_to_game(string id, object person) {
   class player_info info;

   if (is_person_playing(id)) {
      return 0;
   }
   info = _players[id];
   //info->player_num = _current_player_num++;
   info->name = person->query_name();
   info->cap_name = person->query_cap_name();
   return 1;
} /* add_person_to_game() */

/**
 * This method removes a person from the game.
 * @param person the person to remove
 * @return 1 on success, 0 on failure
 */
int remove_person_object_from_game(object person) {
   class player_info info;
   string id;

   foreach (id, info in _players) {
      if (find_player(info->name) == person) {
         info->name = 0;
         info->cap_name = 0;
         return 1;
      }
   }
   return 0;
} /* remove_person_object_from_game() */

/**
 * This method removes a person from the game.
 * @param id the id to remove
 * @return 1 on success, 0 on failure
 */
int remove_person_id_from_game(string id) {
   if (_players[id]->name) {
      _players[id]->name = 0;
      return 1;
   }
   return 0;
} /* remove_person_object_from_game() */

/**
 * This method randomises the numbers  of the players.
 */
void randomise_player_numbers() {
   string* ids;
   int num;
   int pos;

   ids = keys(_players);
   num = 0;
   while (sizeof(ids)) {
      pos = random(sizeof(ids));
      _players[ids[pos]]->number = num;
      ids = ids[0..pos-1] + ids[pos+1..];
      num++;
   }
} /* randomise_player_nambers() */

/**
 * This method adds a type of allowed player.
 * @param id the id to be allowed
 * @param number the start player number
 */
void add_player_id_type(string id, int number) {
   _players[id] = new(class player_info);
   _players[id]->number = number;
} /* add_player_id_type() */

/**
 * This method removes a player id type.
 * @param id the id to remove
 */
void remove_player_id_type(string id) {
   map_delete(_players, id);
} /* remove_player_id_type() */

/**
 * This method figures out the info structure from the number of the player.
 * @param number the number to lookup
 * @return the info structure, 0 if not found
 */
class player_info find_player_info_from_number(int num) {
   class player_info info;
   string id;

   foreach (id, info in _players) {
      if (info->number == num) {
         return info;
      }
   }
   return 0;
} /* find_player_info_from_number() */

/**
 * This method figures out the id from the number of the player.
 * @param number the number to lookup
 * @return the id, 0 if not found
 */
string find_player_id_from_number(int num) {
   class player_info info;
   string id;

   foreach (id, info in _players) {
      if (info->number == num) {
         return id;
      }
   }
   return 0;
} /* find_player_info_from_number() */

/**
 * This method returns all the ids for the players.
 * @return the ids for the player
 */
string* query_player_ids() {
   return keys(_players);
} /* query_player_ids() */

/**
 * This method returns the list of ids who are current playing.
 * @return the list of currently playing ids
 */
string* query_currently_playing_ids() {
   return filter(keys(_players), (: _players[$1]->name :));
} /* query_currently_playing_ids() */

/**
 * This method returns the list of ids of people who started playing the
 * game.
 * @return the list of people who started playing the game
 */
string* query_started_player_ids() {
   return _ids_when_started;
} /* query_started_player_ids() */

/**
 * This method returns the players cap name from the player id.
 * @param id the id to look up from
 * @return the players cap name
 */
string query_player_cap_name(string id) {
   if (_players[id]->name) {
      return _players[id]->cap_name;
   }
   return "No one";
} /* query_player_cap_name() */

/**
 * This method returns the player object from the player id.
 * @param id the id for the player
 * @return the player object
 */
object query_player_object(string id) {
   return find_player(_players[id]->name);
} /* query_player_object() */

/**
 * This method nips through the list of players and checks to see if they
 * are in the room or not.  If they are not it deletes them from the array.
 * Useful for start games or cleanups.
 */
void delete_gone_players() {
   string id;
   class player_info info;

   foreach (id, info in _players) {
      if (info->name) {
         if (!find_player(info->name) ||
             !is_in_me_or_environment(this_object(), find_player(info->name))) {
            info->name = 0;
         }
      }
   }
} /* delete_gone_players() */

/**
 * This method sets the minimum number of players needed to play the
 * game.
 * @param minimum the minimum needed
 */
void set_minimum_needed(int minimun) {
   _minimum_needed = minimun;
} /* set_minimum_needed() */

/**
 * This method checks to see if the minimum requirements for starting the
 * game have been met.
 * @return 1 on success, 0 on failure
 */
int can_start_game() {
   if (sizeof(query_currently_playing_ids()) > _minimum_needed) {
      return 1;
   }
   return 0;
} /* can_start_game() */

/**
 * This method finds the next player to the specified one.
 * @param id the id of the player to find, 0 means current player
 * @return the next id
 */
string find_next_player(string id) {
   class player_info info;
   int start;
   int cur;
   string new_id;

   if (!id) {
      id = _current_player;
   }

   start = _players[id]->number;
   cur = start;
   do {
      cur = (cur + 1) % sizeof(_players);
      new_id = find_player_id_from_number(cur);
      info = _players[new_id];
   } while (((_can_join_as_new && !info->name) ||
            (!_can_join_as_new &&
                member_array(new_id, _ids_when_started) == -1)) &&
            cur != start);
   return new_id;
} /* find_next_player() */

/**
 * This method finds the previous player to the specified one.
 * @param id the id of the player to find, 0 means current player
 * @return the previous id
 */
string find_previous_player(string id) {
   class player_info info;
   int start;
   string new_id;
   int cur;

   if (!id) {
      id = _current_player;
   }

   start = _players[id]->number;
   cur = start;
   do {
      cur = (cur - 1 + sizeof(_players)) % sizeof(_players);
      new_id = find_player_id_from_number(cur);
      info = _players[new_id];
   } while (((_can_join_as_new && !info->name) ||
            (!_can_join_as_new &&
               member_array(new_id, _ids_when_started) == -1)) &&
            cur != start);
   return new_id;
} /* find_previous_player() */

/**
 * This method increments the player number to the next available player.
 */
void increment_current_player() {
   _current_player = find_next_player(_current_player);
} /* increment_current_player() */

/**
 * This method finds the id of the person from their object.
 * @param person the person to find
 * @return the id of the person
 */
string find_player_id_of_person(object person) {
   class player_info info;
   string id;

   foreach (id, info in _players) {
      if (find_player(info->name) == person) {
         return id;
      }
   }
   return 0;
} /* find_id_of_person() */

/**
 * This method finds the id of the current player.
 * @return the id of the current player
 */
string query_current_player() {
   if (!_current_player) {
      _current_player = find_player_id_from_number(0);
   }
   return _current_player;
} /* query_current_player() */

/**
 * This method finds the specified person and sets the current player
 * number to them.
 * @param person the person to find
 * @return the id the of the person, 0 on failure
 */
string set_current_player(string id) {
   _current_player = id;
} /* set_current_player() */

/**
 * This method tells the specified player the message.
 * @param id the id to tell the message to
 * @param message the message to send
 */
void tell_player(string id, string message) {
   object player;

   player = find_player(_players[id]->name);
   if (player) {
      tell_object(player, message);
   }
} /* tell_player() */

/**
 * THis method tells a message to the current player.
 * @param message the message to send
 */
void tell_current_player(string message) {
   tell_player(_current_player, message);
} /* tell_current_player() */

/**
 * This method tells everyone playing the game something.
 * @param message the message to send to everyone
 * @param exclude the ids to optionaly exclude
 */
varargs void tell_all_players(string message, string* exclude) {
   class player_info info;
   string id;

   if (!exclude) {
      exclude = ({ });
   }
   foreach (id, info in _players) {
      if (member_array(id, exclude) == -1) {
         tell_player(id, message);
      }
   }
} /* tell_all_players() */

/**
 * This method gets the extra data associated with the player id.
 * @param id the id to look up the data for
 * @return the extra data
 */
mixed query_player_data(string id) {
   return _players[id]->data;
} /* query_player_data() */

/**
 * This method sets the extra data associated with the player id.
 * @param id the id to set the data for
 * @param data the data to set
 */
void set_player_data(string id, mixed data) {
   _players[id]->data = data;
} /* set_player_data() */

/**
 * This method is called when an id joins the game.  This method should 
 * be overridden in higher up objects to handle special events.
 * @param id the id of the person joining
 */
void multiplayer_someone_joins(string id) {
} /* multiplayer_someone_joins() */

/**
 * This method is called when an id resigns from the game.  This method should 
 * be overridden in higher up objects to handle special events.
 * @param id the id of the person joining
 */
void multiplayer_someone_resigns(string id) {
} /* multiplayer_someone_resigns() */

/**
 * This method scrambles the array.
 * @return the array shuffled
 */
mixed* shuffle_array(mixed *arr) {
   int i;
   int pos;
   mixed* new_arr;

   for (i = 0; i < 2; i++) {
      new_arr = ({ });
      while (sizeof(arr)) {
         pos = random(sizeof(arr));
         new_arr += arr[pos..pos];
         arr = arr[0..pos - 1] + arr[pos + 1..];
      }
      arr = new_arr;
   }
   return arr;
} /* shuffle_array() */

/**
 * This method is the one that does the joining.
 * @param id the id they wish to join as
 * @return 1 on success, 0 on failure
 */
int do_join(string id) {
   if (is_person_playing(id)) {
      add_failed_mess("Someone is already playing " + id + " on $D.\n");
      return 0;
   }

   if (is_playing(this_player())) {
      add_failed_mess("You are already playing on $D.\n");
      return 0;
   }

   if (is_game_started() &&
       !can_join_as_new_player() &&
       member_array(id, query_started_player_ids()) == -1) {
      add_failed_mess("You can only take over one of the spots vacated by "
                      "someone else.\n");
      return 0;
   }

   if (add_person_to_game(id, this_player())) {
      add_succeeded_mess("$N $V as " + id + " on $D.\n");
      multiplayer_someone_joins(id);
      return 1;
   }

   add_failed_mess("Some weird error joining game on $D.\n");
   return 0;
} /* do_join() */

/**
 * This method is the one that does the resignation from the game.
 * @return 1 on success, 0 on failure
 */
int do_resign() {
   string id;

   id = find_player_id_of_person(this_player());
   if (remove_person_object_from_game(this_player())) {
      add_succeeded_mess("$N $V from game on $D.\n");
      multiplayer_someone_resigns(id);
      return 1;
   }
   add_failed_mess("You are not playing on $D to resign.\n");
   return 0;
} /* do_resign() */

/** @ignore yes */
void init() {
   string ids;

   ids = implode(keys(_players), "|");
   add_command("join", "[game] [as] {" + ids + "} on <direct:object>",
                (: do_join($4[0]) :));
   add_command("resign", "[from] [game] on <direct:object>",
               (: do_resign() :));
} /* init() */

/** @ignore yes */
mapping query_dynamic_auto_load(mapping map) {
   if (!map) {
      return 0;
   }
   add_auto_load_value(map, LOAD_TAG, "players", _players);
   add_auto_load_value(map, LOAD_TAG, "current player", _current_player);
   add_auto_load_value(map, LOAD_TAG, "started ids", _ids_when_started);
   add_auto_load_value(map, LOAD_TAG, "winner", _winner);
   add_auto_load_value(map, LOAD_TAG, "game started", _game_started);
   return map;
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_dynamic_arg(mapping map, object player) {
   if (!map) {
      return ;
   }
   _players = query_auto_load_value(map, LOAD_TAG, "players");
   _current_player = query_auto_load_value(map, LOAD_TAG, "current player");
   _ids_when_started = query_auto_load_value(map, LOAD_TAG, "started ids");
   _winner = query_auto_load_value(map, LOAD_TAG, "winner");
   _game_started = query_auto_load_value(map, LOAD_TAG, "game started");
} /* init_dynamic_arg() */
