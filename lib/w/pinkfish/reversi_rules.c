#include "board_master.h"

#ifdef IGOR
#define LET_STR "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#endif

int check_move(mixed *board, int m_no, int x1, int y1, int x2);

mixed *new_board() {
  mixed *bing;
  int i;

  bing = ({ });
  for (i=0;i<8;i++)
    bing += ({ allocate(8) });
  bing[4][4] = 'X';
  bing[3][3] = 'X';
  bing[4][3] = 'O';
  bing[3][4] = 'O';
  return bing;
}

void me_write_board(mixed *board, mixed *info, int show, object tp) {
  int i, j, bl, wh, x, y, me;
  string line;

  x = info[R_LASTX];
  y = info[R_LASTY];
  if (info[R_MOVE_NO]%2)
    me = 'O';
  else
    me = 'X';
  write("x:"+x+" y:"+y+"\n");
  tell_object(tp,"      A   B   C   D   E   F   G  H\n");
  tell_object(tp, "    "+BOARD_COL+"+---+---+---+---+---+---+---+---+"+
                  RESET+"\n");
  for (i=0;i<8;i++) {
    line = " "+(i+1)+": "+BOARD_COL+"|";
    for (j=0;j<8;j++)
      switch (board[i][j]) {
        case 'X' :
          if (i == x && j == y)
            line += " "+BLACK_COL+"L"+END_TILE+" |";
          else
            line += " "+BLACK_COL+"X"+END_TILE+" |";
          bl++;
          break;
        case 'O' :
          if (i == x && j == y)
            line += " "+WHITE_COL+"L"+END_TILE+" |";
          else
            line += " "+WHITE_COL+"O"+END_TILE+" |";
          wh++;
          break;
        case 0 :
          if (show && check_move(board, info[R_MOVE_NO], i, j, -1))
            line += " "+SETUP_COL+"S"+END_TILE+" |";
          else
            line += "   |";
          break;
      }
    switch (i) {
      case 1 :
        tell_object(tp, line+RESET+"    Whelk Reversi\n");
        tell_object(tp, "    "+BOARD_COL+"+---+---+---+---+---+---+---+---+"+
                        RESET+"    by Pinkfish.\n");
        break;
      case 3 :
        tell_object(tp, line+RESET+"    Black (X) "+
                        capitalize(info[R_PLAYER1])+"\n");
        tell_object(tp, "    "+BOARD_COL+"+---+---+---+---+---+---+---+---+"+
                        RESET+"    White (O) "+(info[R_PLAYER2]?
              capitalize(info[R_PLAYER2]):"no one")+"\n");
        break;
      default :
        tell_object(tp, line+RESET+"\n");
        tell_object(tp, "    "+BOARD_COL+"+---+---+---+---+---+---+---+---+"+
                        RESET+"\n");
        break;
    }
  }
  tell_object(tp, "White(O) has "+wh+" pieces and Black(X) has "+bl+
                  " pieces.\n");
  if (info[R_MOVE_NO]%2) {
    if (info[R_PLAYER2])
      tell_object(tp, "White ("+capitalize(info[R_PLAYER2])+") to move.\n");
    else
      tell_object(tp, "White to move.\n");
  } else
    tell_object(tp, "Black ("+capitalize(info[R_PLAYER1])+") to move.\n");
  return ;
}

void write_board(mixed *board, mixed *info, int show) {
  me_write_board(board, info, show, this_player());
}

int check_move(mixed *board, int m_no, int x, int y, int x2) {
  int i, j;
  int not_me, me;

  if (x2 != -1)
    return 0;

  if (x<0 || x>7 || y<0 || y>7)
    return 0;

  if (m_no%2) {
    me = 'O';
    not_me = 'X';
  } else {
    me = 'X';
    not_me = 'O';
  }

  if (board[x][y]) return 0;
/* ok... check diagonals angles. */
  if (x<7 && y<7 && board[x+1][y+1] == not_me)
    for (i=1;x+i<8 && y+i<8;i++)
      if (!board[x+i][y+i])
        break;
      else if (board[x+i][y+i] == me)
        return 1;
  if (y<7 && x>0 && board[x-1][y+1] == not_me)
    for (i=1;x-i>=0 && y+i<8;i++)
      if (!board[x-i][y+i])
        break;
      else if (board[x-i][y+i] == me)
        return 1;
  if (x>0 && y>0 && board[x-1][y-1] == not_me)
    for (i=1;x-i>=0 && y-i>=0;i++)
      if (!board[x-i][y-i])
        break;
      else if (board[x-i][y-i] == me)
        return 1;
  if (x<7 && y>0 && board[x+1][y-1] == not_me)
    for (i=1;x+i<8 && y-i>=0;i++)
      if (!board[x+i][y-i])
        break;
      else if (board[x+i][y-i] == me)
        return 1;
/* Horizontal...  Ohh err. this is a long routine. */
  if (x<7 && board[x+1][y] == not_me)
    for (i=x+1;i<8;i++) {
      if (!board[i][y])
        break;
      else if (board[i][y] == me)
        return 1;
    }
  if (y<7 && board[x][y+1] == not_me)
    for (i=y+1;i<8;i++)
      if (!board[x][i])
        break;
      else if (board[x][i] == me)
        return 1;
  if (x>0 && board[x-1][y] == not_me)
    for (i=x-1;i>=0;i--)
      if (!board[i][y])
        break;
      else if (board[i][y] == me)
        return 1;
  if (y>0 && board[x][y-1] == not_me)
    for (i=y-1;i>=0;i--)
      if (!board[x][i])
        break;
      else if (board[x][i] == me)
        return 1;
}

/* we are assuming in this routine the data is correct */
mixed *do_move(mixed *board, int m_no, int x, int y, int x2) {
  int i, j;
  int not_me, me;

  if (x2 != -1)
    return 0;

  if (m_no%2) {
    me = 'O';
    not_me = 'X';
  } else {
    me = 'X';
    not_me = 'O';
  }

/* ok... check diagonals angles. */
  if (x<7 && y<7 && board[x+1][y+1] == not_me)
    for (i=1;x+i<8 && y+i<8;i++)
      if (!board[x+i][y+i])
        break;
      else if (board[x+i][y+i] == me) {
        for (i=1;1;i++)
          if (board[x+i][y+i] == me) {
            break;
          } else
            board[x+i][y+i] = me;
        break;
      }
  if (y<7 && x>0 && board[x-1][y+1] == not_me)
    for (i=1;x-i>=0 && y+i<8;i++)
      if (!board[x-i][y+i])
        break;
      else if (board[x-i][y+i] == me) {
        for (i=1;1;i++)
          if (board[x-i][y+i] == me) {
            break;
          } else
            board[x-i][y+i] = me;
        break;
      }
  if (x>1 && y>0 && board[x-1][y-1] == not_me)
    for (i=1;x-i>=0 && y-i>=0;i++)
      if (!board[x-i][y-i])
        break;
      else if (board[x-i][y-i] == me) {
        for (i=1;1;i++)
          if (board[x-i][y-i] == me) {
            break;
          } else
            board[x-i][y-i] = me;
        break;
      }
  if (x<7 && y>0 && board[x+1][y-1] == not_me)
    for (i=1;x+i<8 && y-i>=0;i++)
      if (!board[x+i][y-i])
        break;
      else if (board[x+i][y-i] == me) {
        for (i=1;1;i++)
          if (board[x+i][y-i] == me) {
            break;
          } else
            board[x+i][y-i] = me;
        break;
      }
/* Horizontal...  Ohh err. this is a long routine. */
  if (x<7 && board[x+1][y] == not_me)
    for (i=x+1;i<8;i++)
      if (!board[i][y])
        break;
      else if (board[i][y] == me)
        for (i=x+1;i<8;i++)
          if (board[i][y] == me) {
            i = 8;
            break;
          } else
            board[i][y] = me;
  if (y<7 && board[x][y+1] == not_me)
    for (i=y+1;i<8;i++)
      if (!board[x][i])
        break;
      else if (board[x][i] == me)
        for (i=y+1;i<8;i++)
          if (board[x][i] == me) {
            i = 8;
            break;
          } else
            board[x][i] = me;
  if (x>0 && board[x-1][y] == not_me)
    for (i=x-1;i>=0;i--)
      if (!board[i][y])
        break;
      else if (board[i][y] == me)
        for (i=x-1;i>=0;i--)
          if (board[i][y] == me) {
            i = -1;
            break;
          } else
            board[i][y] = me;
  if (y>0 && board[x][y-1] == not_me)
    for (i=y-1;i>=0;i--)
      if (!board[x][i])
        break;
      else if (board[x][i] == me)
        for (i=y-1;i>=0;i--)
          if (board[x][i] == me) {
            i = -1;
            break;
          } else
            board[x][i] = me;
  board[x][y] = me;
  return board;
}

int *end_of_game(mixed *board, int m_no) {
  int i, j, no_m, bl, wh, x, y;

  for (i=0;i<8;i++)
    for (j=0;j<8;j++)
      switch (board[i][j]) {
        case 'X' :
          bl++;
          break;
        case 'O' :
          wh++;
          break;
/*  Just useing this for end condition checking, not move checking.
        default :
         if (check_move(board, m_no, i, j, -1)) {
           no_m++;
           x = i;
           y = j;
         }
 */
         break;
      }
  return ({ bl, wh, });
}

int end_condition(mixed *board, int m_no) {
  mixed *bing;

  bing = end_of_game(board, m_no);
  if (!bing[0])
    return 2;
  if (!bing[1])
    return 1;
  if (bing[0]+bing[1] == 64)
    if (bing[0] == bing[1])
      return 3;
    else
      return 1+(bing[1]>bing[0]);
  return 0;
}

mixed end_thingy(mixed *board, mixed *info, int m_no) {
  int frog;
  int x, y, i, j, no_m;
  string nam;

  nam = (string)TP_NAME;

  for (i=0;i<8;i++)
    for (j=0;j<8;j++)
      if (!board[i][j])
        if (check_move(board, m_no, i, j, -1))
          if (no_m++ >= 1)
            return 0;
  if (!no_m) {
    write("Other player cannot take a turn skipping.\n");
    if (nam == info[R_PLAYER1]) {
      if (info[R_PLAYER2] && find_player(info[R_PLAYER2]))
        tell_object(find_player(info[R_PLAYER2]),
"You don't have any moves in reversi game against "+capitalize(nam)+
" skipping.\n");
    } else if (find_player(info[R_PLAYER1]))
          tell_object(find_player(info[R_PLAYER1]),
"You don't have any moves in reversi game against "+capitalize(nam)+
" skipping.\n");
    return ({ -1 });
  } else if (no_m == 1) {
    write("Other player only has one move, taking it for them.\n");
    if (nam == info[R_PLAYER1])
      if (find_player(info[R_PLAYER2]))
        tell_object(find_player(info[R_PLAYER2]),
#ifndef IGOR
sprintf("You only had one move against "+capitalize(nam)+" taking move "+
"  %c%c for you.\n", (x+'1'), (y+'a')));
#else
"You only had one move against "+capitalize(nam)+" takeing move "+
"  "+(x+1)+LET_STR[y..y]+" for you.\n");
#endif
     else if (find_player(info[R_PLAYER1]))
       tell_object(find_player(info[R_PLAYER1]),
#ifndef IGOR
sprintf("You only had one move against "+capitalize(nam)+" taking move "+
"  %c%c for you.\n", (x+'1'), (y+'a')));
#else
"You only had one move against "+capitalize(nam)+" takeing move "+
"  "+(x+1)+LET_STR[y..y]+" for you.\n");
#endif
    return ({ x, y });
  }
}
