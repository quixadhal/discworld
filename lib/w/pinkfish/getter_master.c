/* The overly interesting board getter, fun for all the family.
 */
#include "board_master.h"

mixed *board;

void create() {
  seteuid((string)"/secure/master"->creator_file(file_name(this_object())));
}

void save_board(mixed *bd, int game_no) {
  if (file_name(previous_object()) != REVERSI_OB) {
    write("Illegal use of save_board.\n");
    return ;
  }
  board = bd;
  save_object(BOARD_PATH+game_no);
}

void delete_board(int game_no) {
  if (file_name(previous_object()) != REVERSI_OB) {
    write("Illegal use of load_board.\n");
    return ;
  }
  rm(BOARD_PATH+game_no+".o");
}

mixed *load_board(int game_no) {
  if (file_name(previous_object()) != REVERSI_OB) {
    write("Illegal use of load_board.\n");
    return 0;
  }
  restore_object(BOARD_PATH+game_no);
  return board;
}
