#include "mail.h"
inherit "/obj/misc/board";

#include "board.h"

void setup() {
  ::setup();
  action_pri = 3;
  reset_drop();
  set_get();
} /* setup() */

/*
 * Need to redefine this to add all the extra bits and wombles to it.
 */
string long(string str, int dark) {
  int i, irp;
  mixed *stuff;
  string ret;
  mapping news_rc;

  stuff = (mixed *)BOARD_HAND->get_subjects(board_name);
  ret = "A bulletin board ("+board_name+").\n";
  irp = (int)BOARD_HAND->query_minimum(board_name);
  if (irp)
    ret += "Minimum number of messages left by auto magic deletion is "+irp+
           ".\n";
  irp = (int)BOARD_HAND->query_maximum(board_name);
  if (irp)
    ret += "Maximum number of messages on this board is "+irp+".\n";
  irp = (int)BOARD_HAND->query_timeout(board_name);
  if (irp)
    ret += "Messages will automaticaly time out in "+irp+" days.\n";
  irp = (int)BOARD_HAND->query_archive(board_name);
  if (irp)
    ret += "Deleted messages are archived in "+irp+".\n";
  ret += sprintf("%#-*s\n\n", this_player()->query_cols(),
                            "read <note number>\npost <subject>\n"+
                            "boards\n"+
                            "store <note number> <file>\neat <note number>\n"+
                            "reply <note number>\nfollowup <note number>\n"+
                            "summary [all]\nnext\nprev\nnew\n"+
                            "board <board name>\n"+
                            "timeout <time>\nminimum <num>\nmaximum <num>\n"+
                            "archive <name|none>\n"+
                            "security\nadd <name>\nremove <name>\n"+
                            "killfile <board name>");
  if (!sizeof(stuff))
    return ret+"The board is completely empty.\n";
  news_rc = (mapping)this_player()->query_property(NEWS_RC);
  if (!news_rc)
    news_rc = ([ ]);
  for (i=0;i<sizeof(stuff);i++)
    if (news_rc[board_name] < stuff[i][B_TIME])
      ret += sprintf("N %2d: %-=*s\n", i+1, (int)this_player()->query_cols()-6,
             stuff[i][B_SUBJECT]+" ("+
             capitalize(stuff[i][B_NAME])+" "+the_date(stuff[i][B_TIME])+")");
    else
      ret += sprintf("  %2d: %-=*s\n", i+1, (int)this_player()->query_cols()-6,
             stuff[i][B_SUBJECT]+" ("+
             capitalize(stuff[i][B_NAME])+" "+the_date(stuff[i][B_TIME])+")");
  return ret;
} /* long() */

void init() {
  ::init();
  add_action("save_note", "store", action_pri);
  add_action("next", "next", action_pri);
  add_action("prev", "prev", action_pri);
  add_action("new", "new", action_pri);
  add_action("board", "b*oard", action_pri);
  add_action("list_boards", "boards", action_pri);
  add_action("kfile", "killfile", action_pri);
  add_action("do_security", "security", action_pri);
  add_action("do_add", "add");
  add_action("do_remove", "remove");
  add_action("do_timeout", "timeout");
  add_action("do_minimum", "minimum");
  add_action("do_maximum", "maximum");
} /* init() */

int save_note(string arg) {
  int which;
  string file;
  mixed *stuff;

  if (!arg || (sscanf(arg, "%d %s", which, file) != 2)) {
    notify_fail("Syntax: store <note number> <file name>\n");
    return 0;
  }
  stuff = (mixed *)BOARD_HAND->get_subjects(board_name);
  if (which < 1 || which > sizeof(stuff)) {
    notify_fail("Invalid note number.\n");
    return 0;
  }
  which--; /* 1..n */
  file += ".note";
  if (!MASTER_OB->valid_write(file, this_player()->query_name())) {
    notify_fail("The master object appears and prevents you.\n");
    return 0;
  }
  seteuid("Root");
  write_file(file,stuff[which][B_SUBJECT]+"\n\n"+
             BOARD_HAND->get_message(board_name, which));
  write("Ok.\n");
  return 1;
} /* save_note() */

void set_board_name(string str) { board_name = str; }
void set_datafile(string str) { board_name = str; }

int next() {
  int i;
  string *boards;

  boards = (string *)BOARD_HAND->list_of_boards();
  if ((i = member_array(board_name, boards)) == -1)
    i = 0;
  else
    i = (i+1)%sizeof(boards);
  board_name = boards[i];
  write("Gone to board:"+board_name+".\n");
  return 1;
} /* next() */

int prev() {
  int i;
  string *boards;

  boards = (string *)BOARD_HAND->list_of_boards();
  if ((i = member_array(board_name, boards)) == -1)
    i = 0;
  else
    i = (i-1+sizeof(boards))%sizeof(boards);
  board_name = boards[i];
  write("Gone to board:"+board_name+".\n");
  return 1;
} /* prev() */

int kfile(string arg) {
  this_player()->add_property("news_kill_"+lower_case(arg),1);
  return 1;
} /* kfile() */

int new() {
  int i;
  string *boards;
  mixed *stuff;
  mixed otherstuff;
  mapping news_rc;

  boards = (string *)BOARD_HAND->list_of_boards();
/* start from the begining and search for one with new notes. */
  news_rc = (mapping)this_player()->query_property(NEWS_RC);
/* John's mark current board as read hack */
  otherstuff = (mixed *)BOARD_HAND->get_subjects(board_name);
  if (sizeof(otherstuff)) {
    news_rc[board_name] = otherstuff[sizeof(otherstuff)-1][B_TIME];
    this_player()->add_property(NEWS_RC, news_rc);
  }
/* */
  for (i=0;i<sizeof(boards);i++) {
    if (!this_player()->query_property("news_kill_"+boards[i])) {
      stuff = (mixed *)BOARD_HAND->get_subjects(boards[i]);
      if (!sizeof(stuff))
        continue;
      if (stuff[sizeof(stuff)-1][B_TIME] > news_rc[boards[i]]) {
        board_name = boards[i];
        write("The "+board_name+" board has new messages.\n");
        return 1;
      }
    }
  }
  notify_fail("No boards with new messages.\n");
  return 0;
} /* new() */

int board(string str) {
  int i;
  string *boards;

  if (!str) {
    notify_fail("Syntax: board <board name>\n");
    return 0;
  }
  boards = (string *)BOARD_HAND->list_of_boards();
  if ((i=member_array(str, boards)) == -1) {
    notify_fail("The board "+str+" does not exist sorry.\n");
    return 0;
  }
  write("Ok.\n");
  board_name = str;
  return 1;
} /* board() */

int list_boards() {
  write("The current boards are '"+
        implode((string *)BOARD_HAND->list_of_boards(), "', '")+"'.\n");
  return 1;
} /* boards() */

int do_security() {
  write("The current people in this boards security array are :\n"+
        implode((string *)BOARD_HAND->query_security(board_name), ", ")+"\n");
  return 1;
} /* do_security() */

int do_add(string name) {
  if (this_player() != this_player(1) ||
      !interactive(previous_object()))
    return 0;
  notify_fail("Syntax: "+query_verb()+" <name>\n");
  return (int)BOARD_HAND->add_allowed(board_name, name);
} /* do_add() */

int do_remove(string name) {
  if (this_player() != this_player(1) ||
      !interactive(previous_object()))
    return 0;
  notify_fail("Syntax: "+query_verb()+" <name>\n");
  return (int)BOARD_HAND->remove_allowed(board_name, name);
} /* do_remove() */

int do_timeout(string name) {
  int i;

  if (this_player() != this_player(1) ||
      !interactive(previous_object()))
    return 0;
  notify_fail("Syntax: "+query_verb()+" <name>\n");
  if (!name || sscanf(name, "%d", i) != 1)
    return 0;
  return (int)BOARD_HAND->set_timeout(board_name, i);
} /* do_timeout() */

int do_minimum(string name) {
  int i;

  if (this_player() != this_player(1) ||
      !interactive(previous_object()))
    return 0;
  notify_fail("Syntax: "+query_verb()+" <name>\n");
  if (!name || sscanf(name, "%d", i) != 1)
    return 0;
  return (int)BOARD_HAND->set_minimum(board_name, i);
} /* do_minimum() */

int do_maximum(string name) {
  int i;

  if (this_player() != this_player(1) ||
      !interactive(previous_object()))
    return 0;
  notify_fail("Syntax: "+query_verb()+" <name>\n");
  if (!name || (sscanf(name, "%d", i) != 1))
    return 0;
  printf("here\n");
  return (int)BOARD_HAND->set_maximum(board_name, i);
} /* do_maximum() */

int do_archive(string name) {
  if (this_player() != this_player(1) ||
      !interactive(previous_object()))
    return 0;
  if (!name)
    return 0;
  if (name != "none")
    return (int)BOARD_HAND->set_archive(board_name, name);
  return (int)BOARD_HAND->set_archive(board_name);
} /* do_archive() */

move(object dest, string s1, string s2) {
  int ret;

  if (!objectp(dest))
    return ::move(dest, s1, s2);
  if (interactive(dest)) {
    ret = ::move(dest, s1, s2);
    if (ret) return ret;
    seteuid(geteuid(dest));
    return 0;
  }
  return ::move(dest, s1, s2);
} /* move() */
