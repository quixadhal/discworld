#include "reversi.h"

#ifdef IGOR
#define LET_STR "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#endif

void save_me();
void load_me();
int check_move(mixed *board, int x, int y, int me);

mixed *games;
mapping ranks;
static mapping doing_move, do_view;
int board_no;

#ifndef IGOR
void create() {
#else
void reset(int arg) {
  if (arg) return ;
#endif
  games = ({ });
  ranks = ([ ]);
  doing_move = ([ ]);
  do_view = ([ ]);
#ifndef IGOR
  seteuid((string)"/secure/master"->creator_file(file_name(this_object())));
#endif
  load_me();
}

int new_board() {
  mixed *bing;
  int i;

  bing = ({ });
  for (i=0;i<8;i++)
    bing += ({ allocate(8) });
  bing[4][4] = 'X';
  bing[3][3] = 'X';
  bing[4][3] = 'O';
  bing[3][4] = 'O';
  BOARD_THING->save_board(bing, board_no);
  return board_no++;
}

void write_board(int b_n, int show) {
  int i, j, bl, wh, x, y, me;
  string line;
  mixed *board;

  board = (mixed *)BOARD_THING->load_board(games[b_n][R_BOARD]);
  if (sizeof(games[b_n]) > R_LASTX) {
    x = games[b_n][R_LASTX];
    y = games[b_n][R_LASTY];
  } else {
    x = -1;
    y = -1;
  }
  if (games[b_n][R_MOVE_NO]%2)
    me = 'O';
  else
    me = 'X';
  write(                    "      A   B   C   D   E   F   G  H\n");
  write("    "+BOARD_COL+"+---+---+---+---+---+---+---+---+"+RESET+"\n");
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
          if (show && check_move(board, i, j, me))
            line += " "+SETUP_COL+"S"+END_TILE+" |";
          else
            line += "   |";
          break;
      }
    switch (i) {
      case 1 :
        write(line+RESET+"    Whelk Reversi\n");
        write("    "+BOARD_COL+"+---+---+---+---+---+---+---+---+"+RESET+
              "    by Pinkfish.\n");
        break;
      case 3 :
        write(line+RESET+"    Black (X) "+capitalize(games[b_n][R_PLAYER1])+
              "\n");
        write("    "+BOARD_COL+"+---+---+---+---+---+---+---+---+"+RESET+
              "    White (O) "+(games[b_n][R_PLAYER2]?
              capitalize(games[b_n][R_PLAYER2]):"no one")+"\n");
        break;
      default :
        write(line+RESET+"\n");
        write("    "+BOARD_COL+"+---+---+---+---+---+---+---+---+"+RESET+"\n");
        break;
    }
  }
  write("White(O) has "+wh+" pieces and Black(X) has "+bl+" pieces.\n");
  if (games[b_n][R_MOVE_NO]%2) {
    if (games[b_n][R_PLAYER2])
      write("White ("+capitalize(games[b_n][R_PLAYER2])+") to move.\n");
  } else
    write("Black ("+capitalize(games[b_n][R_PLAYER1])+") to move.\n");
  return ;
}

int check_move(mixed *board, int x, int y, int me) {
  int i, j;
  int not_me;

  if (me == 'X')
    not_me = 'O';
  else
    not_me = 'X';
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

int *end_of_game(int g_n) {
  int me, not_me, i, j, no_m, bl, wh, x, y;
  mixed *board;

  if (games[g_n][R_MOVE_NO]%2)
    me = 'O';
  else
    me = 'X';
  board = (mixed *)BOARD_THING->load_board(games[g_n][R_BOARD]);
  for (i=0;i<8;i++)
    for (j=0;j<8;j++)
      switch (board[i][j]) {
        case 'X' :
          bl++;
          break;
        case 'O' :
          wh++;
          break;
        default :
         if (check_move(board, i, j, me)) {
           no_m++;
           x = i;
           y = j;
         }
         break;
      }
  return ({ no_m, bl, wh, x, y });
}

void end_game(string win, string lose) {
  int w_r, l_r;
  string nam;

  nam = (string)TP_NAME;
  w_r = (ranks[lose]+50+ranks[win])/2;
  l_r = (ranks[lose]+ranks[win])/2;
  if (w_r > ranks[win])
    ranks[win] = w_r;
  if (l_r < ranks[lose])
    ranks[lose] = l_r;
  if (nam == win) {
    write("The game has finished.  You have won!\n");
    if (find_player(lose))
      tell_object(find_player(lose),
"You get the feeling you just lost a game of reversi.  Bad luck.\n");
  } else {
    write("The game just finished, you have lost.  Better luck next time.\n");
    if (find_player(win))
      tell_object(find_player(win),
"You just won a game of reversi!  Congratulations!\n");
  }
}

static void delete_game(int g_n) {
  string *ind;
  int i;

  ind = m_indices(doing_move);
  for (i=0;i<sizeof(ind);i++)
    if (!pointerp(doing_move[ind[i]]) && doing_move[ind[i]] > g_n)
      doing_move[ind[i]]--;
  BOARD_THING->delete_board(games[g_n][R_BOARD]);
  games = delete(games, g_n, 1);
  save_me();
}

/* we are assuming in this routine the data is correct */
mixed *do_move(int b_n, int x, int y, int me) {
  int i, j;
  int not_me;
  mixed *board;

  board = (mixed *)BOARD_THING->load_board(games[b_n][R_BOARD]);
  if (me == 'X')
    not_me = 'O';
  else
    not_me = 'X';
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
  games[b_n][R_MOVE_NO]++;
  games[b_n][R_LAST] = time();
  if (sizeof(games[b_n]) > R_LASTX) {
    games[b_n][R_LASTX] = x;
    games[b_n][R_LASTY] = y;
  }
  save_me();
  BOARD_THING->save_board(board, games[b_n][R_BOARD]);
}

int input_move(string str) {
  int x, y, b_n, me, frog;
  string nam;
  mixed *board;

  nam = (string)TP_NAME;
  if (pointerp(doing_move[nam])) {
    write("Oh dear, mess up.\n");
    doing_move = m_delete(doing_move, nam);
    return 1;
  }
  if (!(b_n = doing_move[nam])) {
    write("Oh dear.  We have lost your game.\n");
    return 1;
  }
  b_n --;
  board = (mixed *)BOARD_THING->load_board(games[b_n][R_BOARD]);
  str = lower_case(str);
  if (games[b_n][R_PLAYER1] == nam)
    me = 'X';
  else
    me = 'O';
  if (str[0] >= '1' && str[0] <= '8' && str[1] >= 'a' && str[1] <= 'h') {
    if (!check_move(board, str[0]-'1', str[1]-'a', me)) {
      write("Invalid move.\n");
      write("Move: ");
      input_to("input_move");
      return 1;
    }
    x = str[0]-'1';
    y = str[1]-'a';
  } else if (str[1] >= '1' && str[1] <= '8' && str[0] >= 'a' && str[0] <= 'h') {
    if (!check_move(board, str[1]-'1', str[0]-'a', me)) {
      write("Invalid move.\n");
      write("Move: ");
      input_to("input_move");
      return 1;
    }
    x = str[1]-'1';
    y = str[0]-'a';
  } else if (str[0] == 'r') {
    write_board(b_n,0);
    write("Move: ");
    input_to("input_move");
    return 1;
  } else if (str[0] == 's') {
    write_board(b_n, 1);
    write("Move: ");
    input_to("input_move");
    return 1;
  } else if (str[0] == 'q') {
    if (!games[b_n][R_MOVE_NO]) {
      write("Deleteing unstarted game.\n");
      games = delete(games, b_n, 1);
      return 1;
    }
    write("Quiting.\n");
    return 1;
  } else {
    write("Invalid input.\n");
    write("Move: ");
    input_to("input_move");
    return 1;
  }
  do_move(b_n, x, y, me);
  write_board(b_n, 0);
  if (games[b_n][R_PLAYER1] == nam) {
    if (games[b_n][R_PLAYER2] && find_player(games[b_n][R_PLAYER2]))
      tell_object(find_player(games[b_n][R_PLAYER2]),
capitalize(nam)+" just took a move against you in reversi.\n");
  } else if (find_player(games[b_n][R_PLAYER1]))
    tell_object(find_player(games[b_n][R_PLAYER1]),
capitalize(nam)+" just took a move against you in reversi.\n");
  doing_move = m_delete(doing_move, nam);
  do {
    board = end_of_game(b_n);
    frog = 0;
    if (!board[1]) { /* No bits left */
      end_game(games[b_n][R_PLAYER2], games[b_n][R_PLAYER1]);
      delete_game(b_n);
    } else if (!board[2]) {
      end_game(games[b_n][R_PLAYER1], games[b_n][R_PLAYER2]);
      delete_game(b_n);
    } else if (board[1] + board[2] >= 64) {
      if (board[1] > board[2])
        end_game(games[b_n][R_PLAYER1], games[b_n][R_PLAYER2]);
      else
        end_game(games[b_n][R_PLAYER2], games[b_n][R_PLAYER1]);
      delete_game(b_n);
    } else if (!board[0]) {
      write("Other player cannot take a turn skipping.\n");
      if (nam == games[b_n][R_PLAYER1]) {
        if (games[b_n][R_PLAYER2] && find_player(games[b_n][R_PLAYER2]))
          tell_object(find_player(games[b_n][R_PLAYER2]),
"You don't have any moves in reversi game against "+capitalize(nam)+
" skipping.\n");
      } else if (find_player(games[b_n][R_PLAYER1]))
          tell_object(find_player(games[b_n][R_PLAYER1]),
"You don't have any moves in reversi game against "+capitalize(nam)+
" skipping.\n");
      games[b_n][R_MOVE_NO]++;
      save_me();
    } else if (board[0] == 1) {
      write("Other player only has one move, taking it for them.\n");
      frog = 1;
      if (nam == games[b_n][R_PLAYER1]) {
        do_move(b_n, board[3], board[4], 'O');
        if (games[b_n][R_PLAYER2] && find_player(games[b_n][R_PLAYER2]))
          tell_object(find_player(games[b_n][R_PLAYER2]),
#ifndef IGOR
sprintf("You only had one move against "+capitalize(nam)+" takeing move "+
"  %c%c for you.\n",(board[3]+'1'),(board[4]+'a')));
#else
"You only had one move against "+capitalize(nam)+" takeing move "+
"  "+(board[3]+1)+LET_STR[board[4]]+" for you.\n");
#endif
      } else {
        do_move(b_n, board[3], board[4], 'X');
        if (find_player(games[b_n][R_PLAYER1]))
          tell_object(find_player(games[b_n][R_PLAYER1]),
#ifndef IGOR
sprintf("You only had one move against "+capitalize(nam)+" takeing move "+
"  %c%c for you.\n",(board[3]+'1'),(board[4]+'a')));
#else
"You only had one move against "+capitalize(nam)+" takeing move "+
"  "+(board[3]+1)+LET_STR[board[4]]+" for you.\n");
#endif
      }
    }
  } while (frog);
  write("Ok.\n");
  return 1;
}

int do_fix(int b_n) { /* this should only be called if a game wombles
                       * itself.  Share and enjoy.
                       */
  mixed *board;
  string nam;

  nam = (string)this_player()->query_name();
  board = end_of_game(b_n);
  if (!board[1]) { /* No bits left */
    end_game(games[b_n][R_PLAYER2], games[b_n][R_PLAYER1]);
    delete_game(b_n);
  } else if (!board[2]) {
    end_game(games[b_n][R_PLAYER1], games[b_n][R_PLAYER2]);
    delete_game(b_n);
  } else if (board[1] + board[2] >= 64) {
    if (board[1] > board[2])
      end_game(games[b_n][R_PLAYER1], games[b_n][R_PLAYER2]);
    else
      end_game(games[b_n][R_PLAYER2], games[b_n][R_PLAYER1]);
    delete_game(b_n);
  } else if (!board[0]) {
    write("Other player cannot take a turn skipping.\n");
    if (nam == games[b_n][R_PLAYER1]) {
      if (games[b_n][R_PLAYER2] && find_player(games[b_n][R_PLAYER2]))
        tell_object(find_player(games[b_n][R_PLAYER2]),
"You don't have any moves in reversi game against "+capitalize(nam)+
" skipping.\n");
    } else if (find_player(games[b_n][R_PLAYER1]))
        tell_object(find_player(games[b_n][R_PLAYER1]),
"You don't have any moves in reversi game against "+capitalize(nam)+
" skipping.\n");
    games[b_n][R_MOVE_NO]++;
    save_me();
  }
  return 1;
}

int do_rank(string str) {
  int *r, i, j;
  string *s;

  r = allocate(40);
  s = m_indices(ranks);
  for (i=0;i<sizeof(s);i++) {
    for (j=(sizeof(r)-1)/2;j>=0 && r[j*2] < ranks[s[i]];j--);
    if (j == (sizeof(r)-1)/2)
      continue;
    r = r[0..(j+1)*2-1] + ({ ranks[s[i]], s[i] }) + r[(j+1)*2..sizeof(r)-3];
  }
  for (i=0;r[i] && i < sizeof(r);i+=2)
    write((i/2)+": "+r[i+1]+" with "+r[i]+"\n");
  return 1;
}

int do_start(string str) {
  mixed *board;
  string nam;
  int no_g, i;

  nam = (string)TP_NAME;
  for (i=0;i<sizeof(games);i++)
    if (games[i][R_PLAYER1] == nam || games[i][R_PLAYER2] == nam)
      no_g++;

  if (no_g > MAX_GAMES) {
    notify_fail("You are involved in too many games.\n");
    return 0;
  }

  games += ({ ({ nam, 0, new_board(), 0, time(), 0, 0 }) });
  doing_move[nam] = sizeof(games);
  write_board(sizeof(games)-1, 0);
  write("Move: ");
  input_to("input_move");
  return 1;
}

int do_take(string str) {
  string nam;
  int b_n, i, *bing;
  mixed *board;

  nam = (string)TP_NAME;
  if (str) {
    if (sscanf(str, "%d", b_n) != 1) {
      notify_fail("Syntax: "+query_verb()+" [game no]\n");
      return 0;
    }
    if (b_n < 0 || b_n >= sizeof(games)) {
      notify_fail("That game does not exist.\n");
      return 0;
    }
    if (games[b_n][R_PLAYER1] !=  nam && games[b_n][R_PLAYER2] != nam) {
      notify_fail("You are not in game "+b_n+".\n");
      return 0;
    }
    if (games[b_n][R_PLAYER1] == nam && (games[b_n][R_MOVE_NO]%2) ||
        games[b_n][R_PLAYER2] == nam && !(games[b_n][R_MOVE_NO]%2)) {
      notify_fail("It is not your turn in "+b_n+".\n");
      return 0;
    }
    doing_move[nam] = b_n+1;
    write_board(b_n, 0);
    write("Move: ");
    input_to("input_move");
    return 1;
  }
  bing = ({ });
  for (i=0;i<sizeof(games);i++)
    if (games[i][R_PLAYER1] == nam && !(games[i][R_MOVE_NO]%2)) {
#ifndef IGOR
      write(sprintf("%c-%2d: %s against %s.\n", sizeof(bing)+'A',
                    i, capitalize(nam), capitalize(games[i][R_PLAYER2])));
#else
      write(LET_STR[i]+"-"+i+": "+captitalize(nam)+" against "+
            capitalize(games[i][R_PLAYER2]+".\n"));
#endif
      bing += ({ i });
    } else if (games[i][R_PLAYER2] == nam && (games[i][R_MOVE_NO]%2)) {
#ifndef IGOR
      write(sprintf("%c-%2d: %s against %s.\n", sizeof(bing)+'A',
                    i, capitalize(nam), capitalize(games[i][R_PLAYER1])));
#else
      write(LET_STR[i]+"-"+i+": "+capitalize(nam)+" against "+
            capitalize(games[i][R_PLAYER1])+".\n");
#endif
      bing += ({ i });
    }
  if (!sizeof(bing)) {
    notify_fail("No games to take a turn in.\n");
    return 0;
  }
  if (sizeof(bing) == 1) {
    write("Only one game to take a turn in.\n");
    doing_move[nam] = bing[0]+1;
    write_board(bing[0], 0);
    write("Move: ");
    input_to("input_move");
    return 1;
  }
  doing_move[nam] = bing;
#ifndef IGOR
  write("Which game [A-"+sprintf("%c", sizeof(bing)-1+'A')+"]? ");
#else
  write("Which game [A-"+LETSTR[sizeof(bing)-1]+"]? ");
#endif
  input_to("get_game_no");
  return 1;
}

int get_game_no(string str) {
  string nam;
  int *bing, b_n;
  mixed *board;

  str = lower_case(str);
  nam = (string)TP_NAME;
  if (!pointerp(doing_move[nam])) {
    write("Error!!!  Oh no, quick call a frog!!!\n");
    return 1;
  }
  bing = doing_move[nam];
  if (str[0] == 'q') {
    write("Quiting.\n");
    doing_move = m_delete(doing_move, nam);
    return 1;
  }
  if (sscanf(str, "%d", b_n) == 1) {
    if (member_array(b_n, bing) == -1) {
      if (do_view[nam])
        write("You cannot view that game.\n");
      else
        write("You cannot play that game.\n");
#ifndef IGOR
      write("Game [A-"+sprintf("%c", 'A'+sizeof(bing)-1)+"]? ");
#else
      write("Game [A-"+LET_STR[sizeof(bing)-1]+"]? ");
#endif
      input_to("get_game_no");
      return 1;
    }
    doing_move[nam] = b_n+1;
  } else if (str[0] < 'a' || str[0] >= 'a'+sizeof(bing)) {
#ifndef IGOR
    write("Out of range [A-"+sprintf("%c", 'A'+sizeof(bing)-1)+"]? ");
#else
    write("Out of range [A-"+LET_STR[sizeof(bing)-1]+"]? ");
#endif
    input_to("get_game_no");
    return 1;
  } else if (str[0] == 'q' && do_view[nam]) {
    write("Quiting.\n");
    doing_move = m_delete(doing_move, nam);
    do_view = m_delete(do_view, nam);
    return 1;
  } else
    doing_move[nam] = (b_n = bing[str[0]-'a'])+1;
  write("Game no: "+b_n+".\n");
  write_board(b_n, 0);
  if (do_view[nam]) {
    doing_move = m_delete(doing_move, nam);
    do_view = m_delete(do_view, nam);
    return 1;
  }
  if (!games[doing_move[nam]-1][R_PLAYER2] &&
       games[doing_move[nam]-1][R_MOVE_NO] == 1)
    games[doing_move[nam]-1][R_PLAYER2] = nam;
  write("Move: ");
  input_to("input_move");
  return 1;
}

int do_join(string str) {
  int i, *bing, b_n, no_g;
  string nam;
  mixed *board;

  nam = (string)TP_NAME;
  bing = ({ });
  for (i=0;i<sizeof(games);i++)
    if (!games[i][R_PLAYER2] && games[i][R_PLAYER1] != nam) {
#ifndef IGOR
      write(sprintf("%c-%2d: %s against no one.\n", sizeof(bing)+'A',
                    i, capitalize(games[i][R_PLAYER1])));
#else
      write(LET_STR[i]+"-"+i+": "+capitalize(games[i][R_PLAYER1])+
            " against no one.\n");
#endif
      bing += ({ i });
    } else if (games[i][R_PLAYER1] == nam || games[i][R_PLAYER2] == nam)
      no_g++;

  if (no_g > MAX_GAMES) {
    notify_fail("You are involved in too many games.\n");
    return 0;
  }

  if (!sizeof(bing)) {
    notify_fail("No games to join.\n");
    return 0;
  }

  if (str) {
    if (sscanf(str, "%d", b_n) != 1) {
      notify_fail("Syntax: "+query_verb()+" [game no]\n");
      return 0;
    }
    if (b_n < 0 || b_n >= sizeof(games)) {
      notify_fail("That game does not exist.\n");
      return 0;
    }
    if (games[b_n][R_PLAYER2]) {
      notify_fail("That game has already started.\n");
      return 0;
    }
    doing_move[nam] = b_n+1;
    games[b_n][R_PLAYER2] = nam;
    write_board(b_n, 0);
    write("Move: ");
    input_to("input_move");
    return 1;
  }

  doing_move[nam] = bing;
#ifndef IGOR
  write("Which game [A-"+sprintf("%c", sizeof(bing)-1+'A')+"]? ");
#else
  write("Which game [A-"+LET_STR[sizeof(bing)-1]+"]? ");
#endif
  input_to("get_game_no");
  return 1;
}

int do_view(string str) {
  int i, *bing, b_n, no_g;
  string nam;
  mixed *board;

  nam = (string)TP_NAME;
  bing = ({ });

  if (str) {
    if (sscanf(str, "%d", b_n) != 1) {
      notify_fail("Syntax: "+query_verb()+" [game no]\n");
      return 0;
    }
    if (b_n < 0 || b_n >= sizeof(games)) {
      notify_fail("That game does not exist.\n");
      return 0;
    }
    write_board(b_n, 0);
    return 1;
  }

  bing = ({ });
  for (i=0;i<sizeof(games);i++)
    if (games[i][R_PLAYER2] == nam || games[i][R_PLAYER1] == nam) {
#ifndef IGOR
      write(sprintf("%c-%2d: %s against %s.\n", sizeof(bing)+'A',
                    i, capitalize(games[i][R_PLAYER1]), (games[i][R_PLAYER2]?
                    capitalize(games[i][R_PLAYER2]):"no one")));
#else
      write(LET_STR[i]+"-"i+": "+capitalize(games[i][R_PLAYER1])+" against "+
                    (games[i][R_PLAYER2]?capitalize(games[i][R_PLAYER2]):
                    "no one")+".\n");
#endif
      bing += ({ i });
    }

  do_view[nam] = 1; /* Yes, an obscure global varible.  Like it? */
  doing_move[nam] = bing;
#ifndef IGOR
  write("Which game [A-"+sprintf("%c", sizeof(bing)-1+'A')+"]? ");
#else
  write("Which game [A-"+LET_STR[sizeof(bing)-1]+"]? ");
#endif
  input_to("get_game_no");
  return 1;
}

/* this will show us all the games we are in and any new games. */
int do_list() {
  mixed *new;
  int i;
  string nam;

  nam = (string)TP_NAME;
  new = ({ });
  write("Games you are currently involved in:\n");
  for (i=0;i<sizeof(games);i++)
    if (games[i][R_PLAYER1] == nam) {
      if (!games[i][R_PLAYER2])
#ifndef IGOR
        write(sprintf("%2d: %s against no one.\n", i,
                       capitalize(games[i][R_PLAYER1])));
#else
        write(i+": "+capitalize(games[i][R_PLAYER1])+" against no one.\n");
#endif
      else
#ifndef IGOR
        write(sprintf("%2d: %s against %s.\n", i, capitalize(nam),
                      capitalize(games[i][R_PLAYER2])));
#else
        write(i+": "+capitalize(nam)+" against "+
              capitalize(games[i][R_PLAYER2])+".\n");
#endif
    } else if (games[i][R_PLAYER2] == nam) {
#ifndef IGOR
      write(sprintf("%2d: %s against %s, %s move.\n", i, 
                     capitalize(games[i][R_PLAYER1]), 
                     capitalize(nam), (games[i][R_MOVE_NO]%2?"His":"Your")));
#else
      write(i+": "+capitalize(games[i][R_PLAYER1])+" against "+
            capitalize(games[i][R_PLAYER2])+
            ", "+(games[i][R_MOVE_NO]%2?"His":"Your")+" move.\n");
#endif
    } else if (!games[i][R_PLAYER2])
      new += ({ i });
  if (sizeof(new)) {
    write("\nNew games:\n");
    for (i=0;i<sizeof(new);i++)
#ifndef IGOR
      write(sprintf("%2d: %s.\n", new[i],
                    capitalize(games[new[i]][R_PLAYER1])));
#else
      write(new[i]+": "+capitalize(games[new[i]][R_PLAYER1])+".\n");
#endif
  }
  return 1;
}

int do_show(string str) {
  int b_n, m_p, i;

  if (str) {
    if (sscanf(str, "%d", b_n) != 1) {
      notify_fail("Syntax: "+query_verb()+" [board_number]\n");
      return 0;
    }
    if (b_n < 0 || b_n >= sizeof(games)) {
      notify_fail("Board out of range.\n");
      return 0;
    }
    write_board(b_n, 0);
    return 1;
  }
  m_p = MAX_PAGE;
  for (i=0;i<m_p && i<sizeof(games);i++)
#ifndef IGOR
    write(sprintf("%c-%2d: %s against %s, move %d.\n", i+'A',
                  i, capitalize(games[i][R_PLAYER1]), (games[i][R_PLAYER2]?
               capitalize(games[i][R_PLAYER2]):"no one"), games[i][R_MOVE_NO]));
  write(sprintf("Board to show [A-%c]? ", 'A'+i-1));
#else
    write(LET_STR[i]+"-"+i+": "+capitalize(games[i][R_PLAYER1])+" against "+
                 (games[i][R_PLAYER2]?
                 capitalize(games[i][R_PLAYER2]):"no one")+", move "+
                 games[i][R_MOVE_NO]+".\n");
  write("Board to show? ");
#endif
  doing_move[TP_NAME] = ({ 0, i });
  input_to("input_show");
  return 1;
}

int input_show(string str) {
  int i, b_n, m_p, low;
  string nam;

  nam = (string)TP_NAME;
  if (sscanf(str, "%d", b_n) == 1) {
    if (b_n < 0 || b_n >= sizeof(games)) {
      write("Board out of range.\nBoard to show? ");
      input_to("input_show");
      return 1;
    }
    write_board(b_n, 0);
    doing_move = m_delete(doing_move, nam);
    return 1;
  }
  if (str != "") {
    str = lower_case(str);
    if (str[0] == 'q') {
      write("Quiting.\n");
      doing_move = m_delete(doing_move, nam);
      return 1;
    }
    b_n = str[0]-'a'+doing_move[nam][0];
    if (b_n < doing_move[nam][0] || b_n > doing_move[nam][1]) {
      write("Out of range.\n");
#ifndef IGOR
      write(sprintf("Board to show [A-%c]? ", 'A'+doing_move[nam][1]-1));
#else
      write("Board to show [A-"+LET_STR[doing_move[nam][1]-1]+"]? ");
#endif
      input_to("input_show");
      return 1;
    }
    b_n += doing_move[nam][0];
    write_board(b_n, 0);
    doing_move = m_delete(doing_move, nam);
    return 1;
  }
  if (i >= sizeof(games)) {
    write("End of list.\n");
    doing_move = m_delete(doing_move, nam);
    return 1;
  }
  low = doing_move[nam][1];
  m_p = doing_move[nam][1]+MAX_PAGE;
  for (i=low;i<m_p && i<sizeof(games);i++)
#ifndef IGOR
    write(sprintf("%c-%2d: %s against %s, move %d.\n", i+'A'-low,
                  i, capitalize(games[i][R_PLAYER1]), (games[i][R_PLAYER2]?
               capitalize(games[i][R_PLAYER2]):"no one"), games[i][R_MOVE_NO]));
  write(sprintf("Board to show [A-%c]? ", 'A'+i-1));
#else
    write(LET_STR[i]+"-"+i+": "+capitalize(games[i][R_PLAYER1])+" against "+
                 (games[i][R_PLAYER2]?
                 capitalize(games[i][R_PLAYER2]):"no one")+", move "+
                 games[i][R_MOVE_NO]+".\n");
  write("Board to show [A-"+LET_STR[doing_move[nam][1]-1]+"]? ");
#endif
  doing_move[nam] = ({ low, i });
  input_to("input_show");
  return 1;
}

int do_delete(string str) {
  string nam;
  int b_n;

  nam = (string)TP_NAME;
  if (str) {
    if (sscanf(str, "%d", b_n) != 1) {
      notify_fail("Syntax: "+query_verb()+" <game no>\n");
      return 0;
    }
    if (b_n < 0 || b_n >= sizeof(games)) {
      notify_fail("That game does not exist.\n");
      return 0;
    }
    if (games[b_n][R_PLAYER1] != nam && games[b_n][R_PLAYER2] != nam) {
      notify_fail("You cannot delete other peoples games!\n");
      return 0;
    }
    if (games[b_n][R_LAST] < MIN_TIME) {
      notify_fail("That game is too young to delete.\n");
      return 0;
    }
    delete_game(b_n);
    write("Ok, game deleted.\n");
    return 1;
  }
  notify_fail("Syntax: "+query_verb()+" <game no>\n");
  return 0;
}

void save_me() {
#ifndef NO_SAVE
  save_object(SAVE_FILE);
#endif
}

void load_me() {
#ifndef NO_SAVE
  restore_object(SAVE_FILE);
#endif
}
