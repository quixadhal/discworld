inherit "/std/room/furniture/basic";

class co_ord {
   int x;
   int y;
}

private mixed* _board;
private string _player_white;
private string _player_black;
private nosave class co_ord* _offsets;
private class co_ord _last_move;
private int _move_no;

#define BOARD_SIZE 8
#define BOARD_BLACK "X"
#define BOARD_WHITE "O"

#define BOARD_TAG "reversi table"

void reset_board();

void setup() {
   set_name("table");
   set_short("reversi table");
   add_adjective("reversi");
   set_long("A lovely green coloured board table divided into 8 by 8 grid "
            "with small black lines.  There are two curved indentations on "
            "each side of the table filled with bi coloured discs.\n");

   set_weight(100);
   //
   // Setup the offsets for checking.
   //
   _offsets = allocate(8, (: new(class co_ord) :));
   _offsets[0]->x = 1;
   _offsets[0]->y = 1;
   _offsets[1]->x = 0;
   _offsets[1]->y = 1;
   _offsets[2]->x = -1;
   _offsets[2]->y = 1;
   _offsets[3]->x = -1;
   _offsets[3]->y = 0;
   _offsets[4]->x = -1;
   _offsets[4]->y = -1;
   _offsets[5]->x = 0;
   _offsets[5]->y = -1;
   _offsets[6]->x = 1;
   _offsets[6]->y = -1;
   _offsets[7]->x = 1;
   _offsets[7]->y = 0;

   reset_board();
} /* setup() */

/**
 * This method sets up the board.
 */
void reset_board() {
   int i;

   _board = allocate(BOARD_SIZE);
   for (i = 0; i < sizeof(_board); i++) {
      _board[i] = allocate(BOARD_SIZE);
   }
   _board[3][3] = BOARD_BLACK;
   _board[4][4] = BOARD_BLACK;
   _board[3][4] = BOARD_WHITE;
   _board[4][3] = BOARD_WHITE;
   _move_no = 0;
} /* reset_board() */

private string query_line() {
   int i;
   string str;

   str = "   %^B_GREEN%^+";
   for (i = 0; i < BOARD_SIZE; i++) {
      str += "---+";
   }
   return str + "%^RESET%^\n";;
} /* query_line() */

/**
 * This method returns the person who plays the white player.
 * @return the person who plays the white player
 */
string query_white_player() {
   if (_player_white) {
      return _player_white;
   }
   return "unknown";
} /* query_white_player() */

/**
 * This method returns the person who plays the black player.
 * @return the person who plays the black player
 */
string query_black_player() {
   if (_player_black) {
      return _player_black;
   }
   return "unknown";
} /* query_black_player() */

/**
 * This method returns whose move it is.
 */
string query_whose_move() {
   if (_move_no % 2) {
      return BOARD_BLACK;
   } else {
      return BOARD_WHITE;
   }
} /* query_whose_move() */

/**
 * This method returns the name of the player whose move it is.
 */
string query_player_name_move() {
   if (query_whose_move() == BOARD_BLACK) {
      return query_black_player();
   }
   return query_white_player();
} /* query_player_name_move() */

/**
 * This method returns the board stuff.
 */
mixed* query_board() {
   return _board;
} /* query_board() */

/**
 * This method shows the current state of the board.
 */
string query_board_string() {
   int x;
   int y;
   int no_white;
   int no_black;
   string board_str;

   board_str = "";

   if (query_whose_move() == BOARD_BLACK) {
      board_str += "%^BOLD%^Blacks move.%^RESET%^\n";
   } else {
      board_str += "%^BOLD%^Whites move.%^RESET%^\n";
   }

   board_str += "White player (O): " + query_white_player() + "\n";
   board_str += "Black player (X): " + query_black_player() + "\n";

   board_str += "    ";
   for (x = 1; x <= sizeof(_board); x++) {
      board_str += " " + x + "  ";
   }
   board_str += "\n";
   board_str += query_line();

   for (x = 0; x < sizeof(_board); x++) {
      board_str += sprintf("%c  %%^B_GREEN%%^|", x + 'A');
      for (y = 0; y < sizeof(_board[x]); y++) {
         if (!_board[x][y]) {
            board_str += "   |";
         } else {
            if (_board[x][y] == BOARD_BLACK) {
               board_str += " %^B_BLACK%^X%^B_GREEN%^ ";
               no_black++;
            } else {
               board_str += " %^B_WHITE%^O%^B_GREEN%^ ";
               no_white++;
            }
            board_str += "|";
         }
      }
      board_str += "%^RESET%^\n";
      board_str += query_line();
   }

   board_str += "\nWhite has " + no_white +" pieces and black has " +
                no_black + " pieces.\n";

   return board_str;
} /* query_board_string() */

/** @ignore yes */
string long(string str, int dark) {
   if (dark) {
      return ::long() +
             "It is too dark to make out the pieces on the board.\n";
   }
   return ::long() + query_board_string();
} /* long() */

/**
 * This method deciphers some co-ordinates for us.
 */
class co_ord query_co_ords(string str) {
   class co_ord bing;

   str = lower_case(str);
   if (strlen(str) != 2) {
      return 0;
   }

   bing = new(class co_ord);
   if (str[0] >= '1' && str[0] <= '8') {
      bing->y = str[0] - '1';
      if (str[1] >= 'a' && str[1] <= 'h') {
         bing->x = str[1] - 'a';
      } else {
         return 0;
      }
   } else if (str[0] >= 'a' && str[0] <= 'h') {
      bing->x = str[0] - 'a';
      if (str[1] >= '1' && str[1] <= '8') {
         bing->y = str[1] - '1';
      } else {
         return 0;
      }
   }
   return bing;
} /* query_co_ords() */

/**
 * This method checks to see if the specified move is valid for the specified
 * colour.
 * @param co_ord the co-ordinate
 * @param colour the colour they are
 * @return 1 if valid, 0 if not
 */
int is_valid_move(class co_ord bing, string colour) {
   int x;
   int y;
   string other_colour;
   class co_ord off;

   if (_board[bing->x][bing->y]) {
      return 0;
   }

   if (colour == BOARD_BLACK) {
      other_colour = BOARD_WHITE;
   } else {
      other_colour = BOARD_BLACK;
   }

   //
   // Check stuff.
   //
   foreach (off in _offsets) {
      if (bing->x + off->x < BOARD_SIZE && bing->y + off->y < BOARD_SIZE &&
          bing->x + off->x >= 0 && bing->y + off->y >= 0 &&
          _board[bing->x + off->x][bing->y + off->y] == other_colour) {
printf("Checking %O -- %O\n", off, bing);
         for (x = bing->x + off->x * 2, y = bing->y + off->y * 2;
              x < BOARD_SIZE && y < BOARD_SIZE && x >= 0 && y >= 0;
              x += off->x, y += off->y) {
            if (_board[x][y] == colour) {
               return 1;
            } else if (!_board[x][y]) {
               break;
            }
         }
      }
   }

   return 0;
} /* is_valid_move() */

/**
 * This method makes the move.
 * @param co_ord the co-ordinate
 * @param colour the colour they are
 * @return 1 if valid, 0 if not
 */
int make_move(class co_ord bing, string colour) {
   int x;
   int y;
   string other_colour;
   class co_ord off;

   if (!is_valid_move(bing, colour)) {
      return 0;
   }

   if (colour == BOARD_BLACK) {
      other_colour = BOARD_WHITE;
   } else {
      other_colour = BOARD_BLACK;
   }

   //
   // Check stuff.
   //
   _board[bing->x][bing->y] = colour;
   foreach (off in _offsets) {
      if (bing->x + off->x < BOARD_SIZE && bing->y + off->y < BOARD_SIZE &&
          bing->x + off->x >= 0 && bing->y + off->y >= 0 &&
          _board[bing->x + off->x][bing->y + off->y] == other_colour) {
         for (x = bing->x + off->x * 2, y = bing->y + off->y * 2;
              x < BOARD_SIZE && y < BOARD_SIZE && x >= 0 && y >= 0;
              x += off->x, y += off->y) {
            if (_board[x][y] == colour) {
               for (x = bing->x + off->x, y = bing->y + off->y;
                    ;
                    x += off->x, y += off->y) {
                  if (_board[x][y] == colour) {
                     break;
                  }
                  _board[x][y] = colour;
               }
               break;
            }
         }
      }
   }

   _last_move = bing;
   _move_no++;

   //
   // Send the board data to the other person.
   //
   if (query_whose_move() == BOARD_BLACK) {
      if (find_player(query_black_player())) {
         tell_object(find_player(query_black_player()),
                     query_board_string());
      }
   } else {
      if (find_player(query_white_player())) {
         tell_object(find_player(query_white_player()),
                     query_board_string());
      }
   }

   return 1;
} /* make_move() */

/**
 * This method does the actual move.
 * @param str the co-ordinate
 * @return 1 on success, 0 failure
 */
int do_move(string str) {
   class co_ord bing;

   if (query_player_name_move() != this_player()->query_name()) {
      add_failed_mess("It is not your move!\n");
      return 0;
   }

   bing = query_co_ords(str);
   if (!bing) {
      add_failed_mess("Sorry, " + str + " is not a valid move.\n");
      return 0;
   }

   if (!is_valid_move(bing, query_whose_move())) {
      add_failed_mess("Sorry, " + str + " is not a valid move.\n");
      return 0;
   }

   make_move(bing, query_whose_move());
   add_succeeded_mess("$N make$s an exciting move on $I.\n",
                      ({ this_object() }));
   return 1;
} /* do_move() */

/**
 * This method allows you to join the game.
 * @param colour the colour to join as
 * @return 1 on success, 0 failure
 */
int do_join(string colour) {
   string play;
   string name;

   if (colour == BOARD_BLACK) {
      play = query_black_player();
   } else {
      play = query_white_player();
   }

   if (find_player(play) && find_player(play) == environment()) {
      add_failed_mess("Someone is already playing that colour.\n");
      return 0;
   }

   if (colour == BOARD_BLACK) {
      _player_black = this_player()->query_name();
      name = "black";
   } else {
      _player_white = this_player()->query_name();
      name = "white";
   }

   add_succeeded_mess("$N join$s the game on $I as " + name + ".\n",
                      ({ this_object() }));
   return 1;
} /* do_join() */

/**
 * This method starts a game.
 * @return 1 on success, 0 failure
 */
int do_start() {
   if (query_black_player() == this_player()->query_name() ||
       query_white_player() == this_player()->query_name()) {
      reset_board();
      add_succeeded_mess("$N start$s a new game on $I.\n",
                         ({ this_object() }));
      return 0;
   }
   add_failed_mess("Only the players of the game can start a new one.\n");
   return 0;
} /* do_start() */

/**
 * This method allows you to resign from the game.
 * @return 1 on success, 0 on failure
 */
int do_resign() {
   if (query_black_player() == this_player()->query_name()) {
      _player_black = 0;
      add_succeeded_mess("$N resign$s as the black player on $I.\n",
                        ({ this_object() }));
      return 1;
   }
   if (query_white_player() == this_player()->query_name()) {
      _player_white = 0;
      add_succeeded_mess("$N resign$s as the white player on $I.\n",
                        ({ this_object() }));
      return 1;
   }
   return 0;
} /* do_resign() */

/**
 * This method views the current state of the board.
 */
int do_view() {
   int dark;

   dark = this_player()->check_dark(environment()->query_light());
   if (dark) {
      add_failed_mess("It is too dark to see the board.\n");
      return 0;
   }

   write(query_board_string());
   return 1;
} /* do_view() */

/** @ignore yes */
void init() {
   ::init();
   add_command("join", "game as {black|white}",
               (: do_join($4[0] == "black"?BOARD_BLACK:BOARD_WHITE) :));
   add_command("resign", "from game",
               (: do_resign() :));
   add_command("move", "<string'pos'>", (: do_move($4[0]) :) );
   add_command("start", "new game", (: do_start() :) );
   add_command("view", "", (: do_view() :));
} /* init() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
   mapping map;

   map = ::query_dynamic_auto_load();
   add_auto_load_value(map, BOARD_TAG, "board", _board);
   add_auto_load_value(map, BOARD_TAG, "move no", _move_no);
   add_auto_load_value(map, BOARD_TAG, "white", _player_white);
   add_auto_load_value(map, BOARD_TAG, "black", _player_black);
   add_auto_load_value(map, BOARD_TAG, "last move", _last_move);
   return map;
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_dynamic_arg(mapping map, object player) {
   ::init_dynamic_arg(map, player);

   _board = query_auto_load_value(map, BOARD_TAG, "board");
   _move_no = query_auto_load_value(map, BOARD_TAG, "move no");
   _player_white = query_auto_load_value(map, BOARD_TAG, "white");
   _player_black = query_auto_load_value(map, BOARD_TAG, "black");
   _last_move = query_auto_load_value(map, BOARD_TAG, "last move");
   if (!_board) {
      reset_board();
   }
} /* init_dynamic_arg() */
