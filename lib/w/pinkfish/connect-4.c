#include "board_master.h"
/*
 * Will do connect-4...
 * Will have to sleeze the co-ordinates though.  But who cares? :)
 * Very simple rule book.  Smile.
 */

void write_board(mixed *board, mixed *info) {
  int i, j;

  write("  A   B   C   D   E   F   G   H\n");
  for (i=0;i<8;i++) {
    write("|");
    for (j=0;j<8;j++)
      if (board[i][j])
        write(" "+board[i][j]+" |");
      else
        write("   |");
    switch (i) {
      case 3 :
        write("      Whelk Connect 4\n");
        break;
      case 4 :
        write("        by Pinkfish\n");
        break;
      default :
        write("\n");
        break;
    }
  }
  write("The game is between "+info[R_PLAYER1]+" (black) and "+
         (info[R_PLAYER2]?info[R_PLAYER2]:"no one")+" (white).\n");
  if (info[R_MOVE_NO]%2)
    write("White to move.\n");
  else
    write("Black to move.\n");
}

mixed *new_board() {
  mixed *board;
  int i;

  board = ({ });
  for (i=0;i<8;i++)
    board += ({ allocate(8) });
  return board;
}

int check_move(mixed *board, int m_no, int x1, int y1, int x2, int y2) {
  if (y2 != -1)
    return 0;
  if (y1 < 0 || y1 > 7)
    return 0;
  return !board[0][y1];
}

mixed *do_move(mixed *board, int m_no, int x1, int y1) {
  int i;

  for (i=0;i<8&&!board[i][y1];i++);
  if (m_no%2)
    board[i-1][y1] = "O";
  else
    board[i-1][y1] = "X";
  return board;
}

int end_condition(mixed *board) {
  int i, j, k;

  for (i=0;i<8;i++)
    for (j=0;j<8;j++)
      if (board[i][j]) {
        if (i < 5)
          for (k=1;k<4;k++)
            if (board[i][j] != board[i+k][j])
              break;
        if (k == 4)
          return (board[i][j] == "O")+1;
        if (j < 5)
          for (k=1;k<4;k++)
            if (board[i][j] != board[i][j+k])
              break;
        if (k == 4)
          return (board[i][j] == "O")+1;
        if (j < 5 && i < 5)
          for (k=1;k<4;k++)
            if (board[i][j] != board[i+k][j+k])
              break;
          if (k == 4)
            return (board[i][j] == "O")+1;
        if (j < 5 && i > 3)
          for (k=1;k<4;k++)
            if (board[i][j] != board[i-k][j+k])
              break;
        if (k == 4)
          return (board[i][j] == "O")+1;
      }
  return 0;
}
