/**
 * Mancala - an african game of stones.
 * @author Pinkfish
 * @started Mon Dec 11 08:49:30 PST 2000
 */
inherit "/obj/furnitures/misc/games/multiplayer_base";
inherit "/std/room/furniture/basic";

class co_ord {
   int x;
   int y;
}

private int* _board;
private mapping _mancala;

#define LOAD_TAG "mancala"
#define SIDE_LENGTH 6

void create() {
   multiplayer_base::create();
   basic::create();
} /* create() */

void setup() {
   set_short("mancala board");
   add_adjective("mancala");
   add_alias("mancala");
   set_name("board");

   set_long("A small rectangular board with 6 dips on each side and two "
            "bigger dips at each end.  There are a bunch of small multi-"
            "coloured stones in the dips.\n");

   add_player_id_type("green", 0);
   add_player_id_type("red", 0);

   set_minimum_needed(2);

   add_help_file("mancala");

   reset_game();
} /* setup() */

/**
 * This method resets the board to the start position.
 */
void reset_game() {
   _board = allocate(12, (: 4 :));
   _mancala = ([ "red" : 0, "green" : 0 ]);
} /* reset_game() */

/**
 * This method is called to start the game.
 */
int start_game() {
   randomise_player_numbers();
   if (!::start_game()) {
      return 0;
   }

   reset_game();
   return 1;
} /* start_game() */

/**
 * The board itself.
 * @return the board string
 */
string query_board_string() {
   string ret;
   string id;
   int i;

   ret = "";
   foreach (id in query_player_ids()) {
      ret += sprintf("%-25s %s\n",
             query_player_cap_name(id) + " (" + capitalize(id) + ")",
             (is_game_started() && query_current_player() == id?"<-- Their turn":""));
   }

   ret += "                  ------->\n";
   ret += "%^RED%^+--+%^RESET%^   %^GREEN%^";
   for (i = 0; i < SIDE_LENGTH; i++) {
      ret += "+--+ ";
   }
   ret += "   +--+%^RESET%^\n";

   ret += "%^RED%^|  |%^RESET%^   %^GREEN%^";
   for (i = 0; i < SIDE_LENGTH; i++) {
      ret += sprintf("|%2d| ", _board[i]);
   }
   ret += "   |  |%^RESET%^\n";

   ret += "%^RED%^|  |   %^GREEN%^";
   for (i = 0; i < SIDE_LENGTH; i++) {
      ret += "+--+ ";
   }
   ret += "   |  |%^RESET%^\n";

   ret += sprintf("%%^RED%%^|%2d|%%^RESET%%^   ", _mancala["red"]);
   for (i = 0; i < SIDE_LENGTH; i++) {
      ret += "     ";
   }
   ret += sprintf("   %%^GREEN%%^|%2d|%%^RESET%%^\n", _mancala["green"]);

   ret += "%^RED%^|  |   ";
   for (i = 0; i < SIDE_LENGTH; i++) {
      ret += "+--+ ";
   }
   ret += "   %^GREEN%^|  |%^RESET%^\n";

   ret += "%^RED%^|  |   ";
   for (i = 1; i <= SIDE_LENGTH; i++) {
      ret += sprintf("|%2d| ", _board[2 * SIDE_LENGTH - i]);
   }
   ret += "   %^GREEN%^|  |%^RESET%^\n";

   ret += "%^RED%^+--+   ";
   for (i = 0; i < SIDE_LENGTH; i++) {
      ret += "+--+ ";
   }
   ret += "   %^GREEN%^+--+%^RESET%^\n";

   ret += "                 <-------\n";
   ret += "       ";
   for (i = 0; i < SIDE_LENGTH; i++) {
      ret += sprintf("  %c  ", i + 'A');
   }
   ret += "\n";

   if (query_winner()) {
      ret += "The last winner was " + query_winner() + ".\n";
   }

   return ret;
} /* query_board_string() */

string long() {
   return ::long() + query_board_string();
} /* long() */

/**
 * This method checks to see if there is a 4 in a row on the board somewhere.
 */
int check_winner(class co_ord pos) {
   return 0;
} /* check_winner() */

/**
 * This method returns the location as a string.
 * @param loc the location to look at
 * @return the string version of it
 */
string query_location_string(int loc) {
   if (loc < SIDE_LENGTH) {
      return sprintf("green %c", loc + 'A');
   }
   return sprintf("red %c", 2 * SIDE_LENGTH - loc + 'A' - 1);
} /* query_location_string() */

/**
 * The bin on the opposite side.
 */
int query_opposite_side(int loc) {
   if (loc < SIDE_LENGTH) {
      return 2 * SIDE_LENGTH - loc - 1;
   }
   return SIDE_LENGTH - 1 - (loc % (SIDE_LENGTH));
} /* query_opposite_side() */

/**
 * This method checks to see if the game has ended, and ends the game
 * if it has.
 */
int check_end() {
   int empty;
   int i;

   //
   // If all of one side is empty, sweep the other side.
   //
   empty = 1;
   for (i = 0; i < SIDE_LENGTH; i++) {
      if (_board[i]) {
         empty = 0;
      }
   }

   if (empty) {
      for (i = 0; i < SIDE_LENGTH; i++) {
         _mancala["red"] += _board[i + SIDE_LENGTH];
         _board[i + SIDE_LENGTH] = 0;;
      }
   } else {
      empty = 1;
      for (i = 0; i < SIDE_LENGTH; i++) {
         if (_board[i + SIDE_LENGTH]) {
            empty = 0;
         }
      }
      if (empty) {
         for (i = 0; i < SIDE_LENGTH; i++) {
            _mancala["green"] += _board[i];
            _board[i] = 0;
         }
      }
   }

   if (empty) {
      if (_mancala["red"] > _mancala["green"]) {
         finish_game(query_player_cap_name("red"));
      } else if (_mancala["red"] < _mancala["green"]) {
         finish_game(query_player_cap_name("green"));
      } else {
         finish_game(query_player_cap_name("green") + " and " +
                     query_player_cap_name("red"));
      }
   }
   return empty;
} /* check_end() */

/**
 * This method makes a move and chooses the next piece for the other player.
 * @param pos the position to play the piece
 * @param next the next piece to choose
 */
int do_move(string pos_str) {
   int i;
   int pos;
   int num;
   int loc;
   int my_mancala;
   int my_cutoff;
   int my_cutoff_bot;
   int last_mancala;

   if (!is_game_started()) {
      add_failed_mess("The game has not started.\n");
      return 0;
   }

   if (!is_current_player(this_player())) {
      add_failed_mess("It is not your go!\n");
      return 0;
   }

   pos = lower_case(pos_str)[0] - 'a';
   if (pos < 0 || pos >= SIDE_LENGTH || strlen(pos_str) > 1) {
      add_failed_mess("The next piece reference " + pos_str +
                      " is not valid on $D.\n");
      return 0;
   }

   if (query_current_player() == "red") {
      pos = SIDE_LENGTH * 2 - pos - 1;
      my_mancala = 0;
      my_cutoff = 12;
      my_cutoff_bot = 6;
   } else {
      my_mancala = SIDE_LENGTH;
      my_cutoff = 6;
      my_cutoff_bot = 0;
   }

   if (!_board[pos]) {
      add_failed_mess("You can only play from a bin that has stones in it.\n");
      return 0;
   }

   num = _board[pos];
   _board[pos] = 0;
   for (i = 1; i <= num; i++) {
      loc = (pos + i) % (2 * SIDE_LENGTH);
      if (!((pos + i) % SIDE_LENGTH) && loc == my_mancala) {
         //
         // See if we stick a stone in the mancala or not.
         //
         _mancala[query_current_player()]++;
         num--;
         last_mancala = 1;
      }
      if (i <= num) {
         _board[loc]++;
         last_mancala = 0;
      }
   }

   if (!last_mancala &&
       _board[loc] == 1 &&
       loc < my_cutoff &&
       loc >= my_cutoff_bot) {
      //
      // We might do a capture!
      //
      i = query_opposite_side(loc);
      if (_board[i]) {
         add_succeeded_mess("$N capture$s " + query_num(_board[i]) +
                            " pieces from " + query_location_string(i) + ".\n");
         _mancala[query_current_player()] += _board[i] + 1;
         _board[i] = 0;
         _board[loc] = 0;
      }
   }


   if (check_end()) {
      add_succeeded_mess("$N end$s the game and " + query_winner() + " wins "
                         "on $D.\n");
   } else {
      add_succeeded_mess("$N start$s from " + query_location_string(pos) +
                         " and goes to " +
                         query_location_string(loc) + ".\n");

      if (!last_mancala) {
         increment_current_player();
         tell_current_player("%^BOLD%^Your turn!%^RESET%^\n" +
                             query_board_string());
      } else {
         tell_current_player("%^BOLD%^Your turn!%^RESET%^\n" +
                             query_board_string());
      }
   }

   return 1;
} /* do_move() */

/**
 * This method starts a new game.
 */
int do_start() {
   if (!is_playing(this_player())) {
      add_failed_mess("You must be playing the game to start it.\n");
      return 0;
   }

   if (!start_game()) {
      add_failed_mess("You need two people to play Quarto.\n");
      return 0;
   }

   add_succeeded_mess("$N $V a game of $D.\n");
   increment_current_player();
   tell_current_player("%^BOLD%^Your turn!%^RESET%^\n" +
                       query_board_string());
   tell_all_players(query_player_cap_name(query_current_player()) +
                    " goes first!\n", ({ query_current_player() }));
   return 1;
} /* do_start() */

void init() {
   ::init();
   add_command("move",
               "<string'position'> on <direct:object>",
               (: do_move($4[0]) :));
   add_command("start", "[new] [game] on <direct:object>", (: do_start() :));
} /* init() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
   mapping map;

   map = basic::query_dynamic_auto_load();
   multiplayer_base::query_dynamic_auto_load(map);
   add_auto_load_value(map, LOAD_TAG, "board", _board);
   add_auto_load_value(map, LOAD_TAG, "mancala", _mancala);
   return map;
} /* query_dynamic_arg() */

/** @ignore yes */
void init_dynamic_arg(mapping map, object player) {
   basic::init_dynamic_arg(map, player);
   multiplayer_base::init_dynamic_arg(map, player);
   _board = query_auto_load_value(map, LOAD_TAG, "board");
   _mancala = query_auto_load_value(map, LOAD_TAG, "mancala");
} /* init_dynamic_arg() */
