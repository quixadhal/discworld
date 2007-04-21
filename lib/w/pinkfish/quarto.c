/**
 * Quarto, a nifty game of connect-4.
 * @author Pinkfish
 * @started Mon Dec 11 08:49:30 PST 2000
 */
inherit "/obj/furnitures/misc/games/multiplayer_base";
inherit "/std/room/furniture/basic";

private mixed* _board;
private int _next_piece;
private int* _pool;

#define FLAG_SIZE   1
#define FLAG_CENTER 2
#define FLAG_COLOUR 4
#define FLAG_SHAPE  8

void create() {
   multiplayer_base::create();
   basic::create();
} /* create() */

void setup() {
   set_short("quarto board");
   add_adjective("quarto");
   add_alias("quarto");
   set_name("board");

   set_long("A small board with 4 x 4 circles draw on the surface.  There "
            "are 16 pieces of varying colours and sizes placed in little "
            "sockets around the board.\n");

   add_player_id_type("green", 0);
   add_player_id_type("red", 0);

   set_minimum_needed(2);

   reset_game();
} /* setup() */

/**
 * This method resets the board to the start position.
 */
void reset_game() {
   int i;

   _board = allocate(4, (: allocate(4, -1) :));
   _pool = allocate(16);
   for (i = 0; i < sizeof(_pool); i++) {
      _pool[i] = i;
   }
} /* reset_game() */

/**
 * This method is called to start the game.
 */
void start_game() {
   string id;

   randomise_player_numbers();
   if (!::start_game()) {
      return 0;
   }
} /* start_game() */

/**
 * This method returns the type of the piece as an array.
 * @param id the id of the piece
 * @return the piece type
 */
string* query_piece_type_string(int id) {
   string outside;
   string center;

   if (id == -1) {
      return ({ "    ",
                "    ",
                "    ",
                "    " });
   }

/*
 *     +
 *    +X+
 *     +
 *
 *    +++
 *    + +
 *    +++
 *
 *    ++++      ++
 *    +XX+     +  +
 *    +XX+     +  +
 *    ++++      ++
 */
   if (id & FLAG_CENTER) {
      center = "X";
   } else {
      center = " ";
   }

   if (id & FLAG_COLOUR) {
      outside = "*";
   } else {
      outside = "+";
   }

   if (id & FLAG_SIZE) {
      if (id & FLAG_SHAPE) {
         return ({ outside + outside + outside + outside,
                   outside + center + center + outside,
                   outside + center + center + outside,
                   outside + outside + outside + outside });
      } else {
         return ({ " " + outside + outside + " ",
                   outside + center + center + outside,
                   outside + center + center + outside,
                   " " + outside + outside + " " });
      }
   } else {
      if (id & FLAG_SHAPE) {
         return ({ "    ",
                   " " + outside + outside + outside,
                   " " + outside + center + outside,
                   " " + outside + outside + outside });
      } else {
         return ({ "    ",
                   "  " + outside + " ",
                   " " + outside + center + outside,
                   "   " + outside + " " });
      }
   }
} /* query_piece_type() */

/**
 * The board itself.
 * @return the board string
 */
string query_board_string() {
   string ret;
   string* lines;
   string* bit;
   int x;
   int y;
   int i;

   ret = "";
   for (y = 0; y < sizeof(_board[0]); y++) {
      ret += ".....";
   }
   ret += ".\n";
   for (x = 0; x < sizeof(_board); x++) {
      lines = allocate(4, "");
      for (y = 0; y < sizeof(_board[x]); y++) {
         bit = query_piece_type_string(_board[x][y]);
         for (i = 0; i < sizeof(bit); i++) {
            lines[i] += "." + bit[i];
         }
      }
      for (i = 0; i < sizeof(bit); i++) {
         ret += lines[i] + ".\n";
      }
      for (y = 0; y < sizeof(_board[x]); y++) {
         ret += ".....";
      }
      ret += ".\n";
   }

   return ret;
} /* query_board_string() */

string long() {
   return ::long() + query_board_string();
} /* long() */

void init() {
   ::init();
} /* init() */
