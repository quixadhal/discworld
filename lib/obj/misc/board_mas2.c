#include "mail.h"
inherit "/std/object";

#include "board.h"

string board_name;

mapping being_written;

void setup() {
  set_name("board");
  set_short("bulletin board");
  add_adjective("boards");
  reset_drop();
  board_name = "fish";
  being_written = ([ ]);
}

string query_plural() {
  mixed *stuff;

  stuff = (mixed *)BOARD_HAND->get_subjects(board_name);
  switch (sizeof(stuff)) {
    case 0:  return pluralize(::short(0))+" [ Empty ]";
    case 1:  return pluralize(::short(0))+" [ 1 note ]";
  }
   return pluralize(::short(0))+" [ "+sizeof(stuff)+" notes ]";
}

string short(int dark) {
  mixed *stuff;

  stuff = (mixed *)BOARD_HAND->get_subjects(board_name);
  switch (sizeof(stuff)) {
    case 0:  return ::short(dark)+" [ Empty ]";
    case 1:  return ::short(dark)+" [ 1 note ]";
    default:
      return ::short(dark)+" [ "+sizeof(stuff)+" notes ]";
  }
}

string the_date(int i) {
  return ctime(i)[4..9];
}

string long(string str, int dark) {
  int i;
  mixed *stuff;
  string ret;
  mapping news_rc;

  stuff = (mixed *)BOARD_HAND->get_subjects(board_name);
  ret = "A bulletin board ("+board_name+").\n";
  ret += sprintf("%#-*s\n\n", this_player()->query_cols(),
                            "read <note number>\npost <subject>\n"+
                            "store <note number> <file>\neat <note number>\n"+
                            "reply <note number>\nfollowup <note number>\n"+
                            "summary [all]\nnext\nprev\nnew\nboard <board name>\n"+
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
}

void init() {
  add_action("read", "r*ead");
  add_action("post", "p*ost");
  add_action("post", "not*e");
  add_action("eat", "eat");
  add_action("followup", "f*ollowup");
  add_action("reply", "rep*ly");
  add_action("save_note", "store");
  add_action("next", "next");
  add_action("prev", "prev");
  add_action("new", "new");
  add_action("board", "board");
  add_action("kfile", "killfile");
  add_action("summary","summary");
}

void string_more(string arg, string prompt) {
  object obj;
/* changed to our more_string

  if (!(obj = (object)MAIL_SERVER->mail_reader(this_player()))) {
    log_file(LOG_FILE, "board: got NULL MAIL_READER from MAIL_SERVER.\n");
    write("MAIL_SERVER error ...\n");
    return;
  }
  obj->string_more(arg, prompt);
*/
  this_player()->set_finish_func(0);
  this_player()->more_string(arg, prompt);
}

int read(string str) {
  int num, i;
  mixed stuff;
  mapping news_rc;

  notify_fail("Syntax: read <note number>\n");
  stuff = (mixed *)BOARD_HAND->get_subjects(board_name);
  news_rc = (mapping)this_player()->query_property(NEWS_RC);
  if (!news_rc)
    news_rc = ([ ]);
  if (!str) {
    i = sizeof(stuff)-1;
    while (i >= 0 && stuff[i][B_TIME] > news_rc[board_name] )
      i--;
    while (i < sizeof(stuff)-1 &&
           this_player()->query_property("news_kill_"+
            lower_case(stuff[i+1][B_NAME]))) {
             i++;
    }
    if (i == sizeof(stuff)-1) {
      notify_fail("No unread messages.\n");
      return 0;
    }
    num = i+2;
  } else if (sscanf(str, "%d", num) != 1)
    return 0;
  if (num < 1 || num > sizeof(stuff)) {
    notify_fail("No note of that number.\n");
    return 0;
  }
  num --;
  if (news_rc[board_name] < stuff[num][B_TIME]) {
    news_rc[board_name] = stuff[num][B_TIME];
    this_player()->add_property(NEWS_RC, news_rc);
  }
  string_more(sprintf("Note #%d by %s posted at %s\nTitle: '%s'\n\n", 
              num+1, capitalize(stuff[num][B_NAME]), ctime(stuff[num][B_TIME]),
              stuff[num][B_SUBJECT][0..(int)this_player()->query_cols()-10])+
              BOARD_HAND->get_message(board_name, num),
                 "[Note "+(num+1)+"]");
/*
  string_more(sprintf("%|*s\n\n", this_player()->query_cols(),
              "Note #"+(num+1)+" on "+stuff[num][B_SUBJECT]+" by "+
              stuff[num][B_NAME])+
              BOARD_HAND->get_message(board_name, num),
                 "[Note "+(num+1)+"]");
 */
  return 1;
}

int post(string str) {
  string body;

  notify_fail("Syntax: post <subject>\n");
  if (!str)
    return 0;
/* ok shove the editing stuff in here.  Lets make it function string_edit
 * sound frogy?
 */
/*
  string_edit("");
  body = string_edit_res;
*/
  being_written[this_player()->query_name()] = str;
  this_player()->do_edit(0,"end_of_thing");
  return 1;
}

void end_of_thing(string body) {
  if (body && body != "" && being_written[this_player()->query_name()])
    if (!BOARD_HAND->add_message(board_name, this_player()->query_name(),
                            being_written[this_player()->query_name()], body))
      write("Error writing message.\n");
    else
      write("Message posted.\n");
  else
    write("Erorr.\n");
  being_written = m_delete(being_written, this_player()->query_name());
  return ;
}

int eat(string str) {
  int num;

  notify_fail("Syntax: read <note number>\n");
  if (!str)
    return 0;
  if (sscanf(str, "%d", num) != 1)
    return 0;
  if (!BOARD_HAND->delete_message(board_name, num-1))
    return 0;
  write("You viciously tear off and eat note "+num+"\n");
  say(this_player()->query_cap_name()+" viciously tears off and eats note "+
         num+".\n");
  return 1;
}

int followup(string str) {
  int num, i;
  mixed stuff;
  string s;

  notify_fail("Syntax: read <note number>\n");
  if (!str)
    return 0;
  if (sscanf(str, "%d", num) != 1)
    return 0;
  stuff = (mixed *)BOARD_HAND->get_subjects(board_name);
  if (num < 1 || num > sizeof(stuff)) {
    notify_fail("No note of that number.\n");
    return 0;
  }
  if (sscanf(stuff[num-1][B_SUBJECT], "Re:#%d %s", i, s) != 2)
    being_written[this_player()->query_name()] = "Re:#1 "+stuff[num-1][B_SUBJECT];
  else
    being_written[this_player()->query_name()] = "Re:#"+i+" "+s;
  this_player()->do_edit(0,"end_of_thing");
  return 1;
}

int reply(string str) {
  int num;
  mixed stuff;

  notify_fail("Syntax: read <note number>\n");
  if (!str)
    return 0;
  if (sscanf(str, "%d", num) != 1)
    return 0;
  stuff = (mixed *)BOARD_HAND->get_subjects(board_name);
  if (num < 1 || num > sizeof(stuff)) {
    notify_fail("No note of that number.\n");
    return 0;
  }
  MAIL_TRACK->mail(stuff[num-1][B_NAME], stuff[num-1][B_SUBJECT]);
  return 1;
}

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
  write_file(file,stuff[which][B_SUBJECT]+"\n\n"+
             BOARD_HAND->get_message(board_name, which));
  write("Ok.\n");
  return 1;
}

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
}

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
}

int kfile(string arg) {
  this_player()->add_property("news_kill_"+lower_case(arg),1);
  return 1;
}

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
}

int summary(string str) {
  int all,i,j,unred,numdone;
  string *boards;
  mixed *stuff;
  mapping news_rc;

  all=0;
  if (str) {
    if (str=="all") {
      all=1;
    } else {
      write("Usage: summary [all]\n");
      return 1;
    }
  }
  boards = (string *)BOARD_HAND->list_of_boards();
/* start from the begining and start listing them. */
  news_rc = (mapping)this_player()->query_property(NEWS_RC);
  numdone = 0;
  for (i=0;i<sizeof(boards);i++) {
    if (boards[i] && !this_player()->query_property("news_kill_"+boards[i])) {
      unred = 0;
      stuff = (mixed *)BOARD_HAND->get_subjects(boards[i]);
      for (j=sizeof(stuff);j && stuff[--j][B_TIME]>news_rc[boards[i]];unred++);
      /*stuff[sizeof(stuff)-1][B_TIME];*/
      if (unred || all) {
        printf("%-*s %d\n",(int)this_player()->query_cols()/2,boards[i],unred);
        numdone++;
      }
    }
  }
  if (!numdone) write("No unread messages anywhere.\n");
  return 1;
}

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
}
