#define MAX_PAGE ((int)this_player()->query_rows()-3)
#define TP_NAME this_player()->query_name()
/* this will be a handler object... */
#define SAVE_FILE "/w/pinkfish/save/reversi"
#define BOARD_THING "/w/pinkfish/board_getter"

/* used by the board getter */
#define REVERSI_OB "/w/pinkfish/reversi_board"
#define BOARD_PATH "/w/pinkfish/save/reversi/"

/* should be two weeks */
#define MIN_TIME (60*60*24*14)

#undef DEBUG
#undef DEBUG2

#define MAX_GAMES 10

#define R_PLAYER1 0
#define R_PLAYER2 1
/* This is a board number.  A offset to the board save file.
 */
#define R_BOARD 2
#define R_MOVE_NO 3
/* last move.  Saved for timeouts. */
#define R_LAST 4
#define R_LASTX 5
#define R_LASTY 6

#define BOARD_COL "%^B_GREEN%^%^WHITE%^"
#define BLACK_COL "%^B_BLACK%^"
#define WHITE_COL "%^B_WHITE%^"
#define END_TILE  "%^B_GREEN%^"
#define RESET     "%^RESET%^"
#define SETUP_COL "%^B_CYAN%^"
