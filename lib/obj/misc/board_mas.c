#include <mail.h>
#include <board.h>

inherit "/obj/misc/board";

int do_store(int num, string str);
int do_storeall(string str);
int do_minimum(int num);
int do_maximum(int num);
int do_timeout(int num);
int do_add(string str);
int do_remove(string str);
int do_board(string str);
int do_archive(string str);
int do_killfile(string str);
int do_unkillfile(string str);
int do_transfer(int num, string str);
int do_summary();
int do_skip(string str, int move_on);

int calc_new(object player, string board);

void setup() {
  ::setup();
  add_alias("board master");
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
    ret += "Minimum number of messages left by automagic deletion is "+ irp +
           ".\n";
  irp = (int)BOARD_HAND->query_maximum(board_name);
  if (irp)
    ret += "Maximum number of messages on this board is "+irp+".\n";
  irp = (int)BOARD_HAND->query_timeout(board_name);
  if (irp)
    ret += "Messages will automagically time out in "+ irp +" days.\n";
  irp = (int)BOARD_HAND->query_archive(board_name);
  if (irp)
    ret += "Deleted messages are archived in "+irp+".\n";
  ret += sprintf("%#-*s\n\n", this_player()->query_cols(),
                 "read <note number>\npost <subject>\n"+
                 "boards\n"
                 "store <note number> <file>\n"
                 "storeall <file> (optional)\n"
                 "eat <note number>\n"
                 "reply <note number>\n"
                 "followup <note number>\n"
                 "summary [all]\n"
                 "next\n"
                 "prev\n"
                 "skip [<board name>]\n"
                 "board <board name>\n"
                 "timeout <time>\n"
                 "minimum <num>\n"
                 "maximum <num>\n"
                 "archive <name|none>\n"
                 "security\n"
                 "sadd <name>\n"
                 "sremove <name>\n"
                 "killfile <board name>\n"
                 "unkillfile <board name>\n"
                 "listkillfile\ntransfer <note> <board>");
  if (!sizeof(stuff))
    return ret+"The board is completely empty.\n";
  news_rc = BOARD_HAND->query_newsrc(this_player()->query_name());
  if (!news_rc)
    news_rc = ([ ]);
  for (i=0;i<sizeof(stuff);i++)
    if (news_rc[board_name] < stuff[i][B_TIME])
      ret += sprintf("N %2d: %-=*s\n", i+1, (int)this_player()->query_cols()-6,
             stuff[i][B_SUBJECT]+" ("+
             stuff[i][B_NAME]+" "+the_date(stuff[i][B_TIME])+")");
    else
      ret += sprintf("  %2d: %-=*s\n", i+1, (int)this_player()->query_cols()-6,
             stuff[i][B_SUBJECT]+" ("+
             stuff[i][B_NAME]+" "+the_date(stuff[i][B_TIME])+")");
  return ret;
} /* long() */

int calc_new(object who, string board) {
  mapping news_rc;
  mixed *stuff;
  int amt;

  news_rc = BOARD_HAND->query_newsrc(who->query_name());
  if ( !news_rc )
    news_rc = ([ ]);
  
  stuff = (mixed *)BOARD_HAND->get_subjects(board);
  if ( !sizeof( stuff ) )
    return 0;
  
  amt = sizeof( filter_array( stuff, (: ( $1[ B_TIME ] > $2 ) &&
              ( lower_case( $1[ B_NAME ] ) != $3 ) :), news_rc[ board ],
                              (string)who->query_name() ) );
  return amt;
}

void init() {
   /* Add the standard board commands... */
   if (this_player()->query_creator()) {
      add_commands();
      add_command("store", "<number> <word>", (: do_store($4[0], $4[1]) :));
      add_command("storeall", "<word>", (: do_storeall($4[0]) :));
      add_command("next", "");
      add_command("prev", "");
      add_command("new", "");
      add_command("board", "<string>", (: do_board($4[0]) :));
      add_command("boards", "");
      add_command("killfile", "<string>", (: do_killfile($4[0]) :));
      add_command("unkillfile", "<string>",(: do_unkillfile($4[0]) :)); 
      add_command("listkillfile", "");
      add_command("security", "");
      add_command("sadd", "<word>", (: do_add($4[0]) :));
      add_command("sremove", "<word>", (: do_remove($4[0]) :));
      add_command("timeout", "<number>", (: do_timeout($4[0]) :));
      add_command("minimum", "<number>", (: do_minimum($4[0]) :));
      add_command("maximum", "<number>", (: do_maximum($4[0]) :));
      add_command("archive", "<word>", (: do_archive($4[0]) :));
      add_command("summary", "");
      add_command("skip", "", (: do_skip(0, 1) :));
      add_command("catchup", "", (: do_skip(0, 0) :));
      add_command("skip", "<string>", (: do_skip($4[0], 1) :));
      add_command("catchup", "<string>", (: do_skip($4[0], 0) :));
      add_command("arts", "");
      add_command("transfer", "<number> <string>",
                     (: do_transfer($4[0], $4[1]) :));
   }
} /* init() */

int do_storeall(string file) {
  mixed *stuff;
  int i, j, amt, cnt=0;
  string *boards, their_name;
  mapping news_rc;

  seteuid( geteuid( this_player() ) );
  their_name = this_player()->query_name();
  //if (!str || (sscanf(str, "%s", file) != 1)) 
    //file = "/w/"+their_name+"/boards.txt";

  boards = (string *)BOARD_HAND->list_of_boards();
  /* start from the begining and search for one with new notes. */
  news_rc = BOARD_HAND->query_newsrc(this_player()->query_name());
   if ( !news_rc )
      news_rc = ([ ]);
  if (!write_file(file, "File created on "+ctime(time())+
                        " - Seattle time\n", 1)) {
    add_failed_mess("Unable to save the file "+file+"\n");
    return 0;
  }
  for (i=0;i<sizeof(boards);i++) {
    reset_eval_cost();
    if (!BOARD_HAND->query_killfile(this_player()->query_name(), lower_case(boards[i]))) {
      stuff = (mixed *)BOARD_HAND->get_subjects(boards[i]);
      if (!sizeof(stuff))
        continue;
      for (amt=sizeof(stuff)-1;amt >= 0 &&
                               (stuff[amt][B_TIME] > news_rc[boards[i]] &&
                               lower_case(stuff[amt][B_NAME]) != their_name);
                 amt--);
      if (amt != sizeof(stuff)-1) {
        write_file(file,"\n----> Bulletin Board Name: "+boards[i]+"\n\n\n");
        news_rc[boards[i]] = stuff[sizeof(stuff)-1][B_TIME];
        for(j = amt+1;j<sizeof(stuff);j++) {
          write_file(file,"***** Note #" +(j+1)+ " by " +stuff[j][B_NAME]+
       " posted at " +ctime(stuff[j][B_TIME])+ "\nTitle: "+
       stuff[j][B_SUBJECT][0..(int)this_player()->
                                                 query_cols() - 10]+"\n\n"+
                         BOARD_HAND->get_message(boards[i], j)+"\n\n\n");

          if (cnt == 0)
             printf("Storing ");
          cnt++;
          printf(".");
        }
      }
    }
  }
  write("\n"+cnt+" new messages are now saved in "+file+"\n");
  if (cnt == 0) {
    BOARD_HAND->set_newsrc(this_player()->query_name(), news_rc);
  }
  return 1;
} /* do_storeall() */

int do_store(int which, string file) {
  mixed *stuff;

  stuff = (mixed *)BOARD_HAND->get_subjects(board_name);
  if (which < 1 || which > sizeof(stuff)) {
    add_failed_mess("Invalid note number.\n");
    return 0;
  }
  which--; /* 1..n */
  file += ".note";
     seteuid( geteuid( this_player() ) );
  if (!write_file(file,stuff[which][B_SUBJECT]+"\n\n"+
                  BOARD_HAND->get_message(board_name, which))) {
    add_failed_mess("The master object appears and prevents you.\n");
    return 0;
  }
  write("Ok.\n");
  return 1;
} /* do_store() */

void set_board_name(string str) { board_name = str; }
void set_datafile(string str) { board_name = str; }


int do_next() {
  int start, i, j, k, found;
  string *boards;
  
  if(calc_new(this_player(), board_name))
    return do_read_new();

  boards = (string *)BOARD_HAND->list_of_boards();
  if ((start = member_array(board_name, boards)) == -1)
    start = 0;
  for(i=0; i<sizeof(boards); i++) {
    j = (i + start) % sizeof(boards);
    if(!BOARD_HAND->query_killfile(this_player()->query_name(),
                                     lower_case(boards[j])) &&
       k = calc_new(this_player(), boards[j])) {
      found = 1;
      break;
    }
  }

  if(found) {
    board_name = boards[j];
    write("Okay, changed to board "+board_name+" with " + k +
          " new notes.\n");
    return do_read_new();
  } else
    write("No boards with new messages.\n");
  
  return 1;
} /* next() */

int do_prev() {
  int i;
  string *boards;

  boards = (string *)BOARD_HAND->list_of_boards();
  if ((i = member_array(board_name, boards)) == -1)
    i = 0;
  else
    i = (i-1+sizeof(boards))%sizeof(boards);
  board_name = boards[i];
  write("Okay, changed to board "+board_name+".\n");
  return 1;
} /* prev() */

int do_killfile(string arg) {
    string *bits, board;

    bits = explode( arg, "," );

    foreach ( board in bits ) {        
        if ( sizeof( BOARD_HAND->get_subjects( board ) ) ) {
          BOARD_HAND->set_killfile(this_player()->query_name(),
                lower_case( board ));
            tell_object( this_player(), "Board " + board  + " killed.\n" );
        }
        else { 
            tell_object( this_player(), "There is no such board " + 
                board + ".\n" );  
        }
    }
    
    return 1;
} /* do_kill_file() */



int do_unkillfile(string arg) {
  if (!BOARD_HAND->query_killfile(this_player()->query_name(),
                                  lower_case( arg ) )) {
     add_failed_mess("Board "+arg+" is not kill file anyway.\n");
     return 0;
  } else {
    BOARD_HAND->remove_killfile(this_player()->query_name(),
                                lower_case(arg));
     write("Board "+arg+" unkilled.\n");
  }
  return 1;
} /* ukfile() */

int do_listkillfile() {
   string *boards;

   boards = BOARD_HAND->list_killfile(this_player()->query_name());
   if ( sizeof( boards ) )
     write("You have the following boards killfiled: "+
           query_multiple_short( boards ) +".\n" );
   return 1;
} /* lkfile() */

int do_board(string str) {
  int i;
  string *boards;

  boards = (string *)BOARD_HAND->list_of_boards();
  i = member_array(str, boards);
  if (i == -1) {
    add_failed_mess("The board "+str+" does not exist sorry.\n");
    return 0;
  }
  write("Okay, changed to board "+ str);
  i = calc_new(this_player(), boards[i]);
  if(i)
    write(" with " + i + " new notes.\n");
  else
    write(" with no new notes.\n");

  board_name = str;
  return 1;
} /* board() */

int do_boards() {
   write( "The current boards are:\n\""+
         implode( (string *)BOARD_HAND->list_of_boards(), "\", \"" ) +
         "\".\n" );
  return 1;
} /* boards() */

int do_security() {
   write( "The current people in this boards security array are:\n"+
        implode((string *)BOARD_HAND->query_security(board_name), ", ")+"\n");
  return 1;
} /* do_security() */

int do_add(string name) {
  if (this_player() != this_player(1) ||
      !interactive(previous_object()))
    return 0;
  return (int)BOARD_HAND->add_allowed(board_name, name);
} /* do_add() */

int do_remove(string name) {
  if (this_player() != this_player(1) ||
      !interactive(previous_object()))
    return 0;
  return (int)BOARD_HAND->remove_allowed(board_name, name);
} /* do_remove() */

int do_timeout(int i) {
  if (this_player() != this_player(1) ||
      !interactive(previous_object()))
    return 0;
  return (int)BOARD_HAND->set_timeout(board_name, i);
} /* do_timeout() */

int do_minimum(string i) {
  if (this_player() != this_player(1) ||
      !interactive(previous_object()))
    return 0;
  return (int)BOARD_HAND->set_minimum(board_name, i);
} /* do_minimum() */

int do_maximum(int i) {
  if (this_player() != this_player(1) ||
      !interactive(previous_object()))
    return 0;
  return (int)BOARD_HAND->set_maximum(board_name, i);
} /* do_maximum() */

int do_archive(string name) {
  if (!name)
    return 0;
  if (name != "none")
    return (int)BOARD_HAND->set_archive(board_name, name);
  return (int)BOARD_HAND->set_archive(board_name);
} /* do_archive() */

varargs int move(object dest, string s1, string s2) {
  int ret;

  if (!objectp(dest))
    return ::move(dest, s1, s2);
  if (interactive(dest)) {
    ret = ::move(dest, s1, s2);
    if (ret)
      return ret;
    seteuid(geteuid(dest));
    return 0;
  }
  return ::move(dest, s1, s2);
} /* move() */

mixed query_static_auto_load() {
  return board_name;
} /* query_static_auto_load() */

void init_static_arg(mixed board) {
  if (stringp(board))
    board_name = board;
} /* init_static_auto_load() */

int do_summary() {
   int i, no, amt;
   string *boards;

   boards = (string *)BOARD_HAND->list_of_boards();
   for ( i = sizeof( boards ) - 1; i > -1; i-- )
      if ( !BOARD_HAND->query_killfile(this_player()->query_name(),
                                      lower_case( boards[ i ] ) ) ) {
        amt = calc_new(this_player(), boards[i]);
         if ( amt ) {
            printf( "The %s board has %d new message%s.\n", boards[ i ],
                  amt, amt > 1 ? "s" : "" );
            no++;
         }
      }
   if ( !no )
      add_failed_mess( "No boards with new messages.\n" );
   return no;
} /* do_summary() */

int do_new() {
   int i, amt;
   string *boards;
   boards = (string *)BOARD_HAND->list_of_boards();
   for ( i = sizeof( boards ) - 1; i > -1; i-- )
     if ( !BOARD_HAND->query_killfile(this_player()->query_name(),
                                      lower_case( boards[ i ] ) ) ) {
        amt = calc_new(this_player(), boards[i]);
         if ( amt ) {
            board_name = boards[ i ];
            printf( "The %s board has %d new message%s.\n", boards[ i ],
                  amt, amt > 1 ? "s" : "" );
            return 1;
         }
      }
   add_failed_mess( "No boards with new messages.\n" );
   return 0;
} /* do_new() */

/* Skip does the same as a new, but marks the board as read. */
int do_skip(string str, int move_on) {
  mapping news_rc;
  mixed *otherstuff;
  int i, j, k, start, found;
  string *boards;
  
  if(!str) {
    str = board_name;
  }

  boards = (string *)BOARD_HAND->list_of_boards();
  if(member_array(str, boards) == -1)
    return notify_fail("No such board " + str + ".\n");

  news_rc = BOARD_HAND->query_newsrc(this_player()->query_name());
   if ( !news_rc )
      news_rc = ([ ]);
  otherstuff = (mixed *)BOARD_HAND->get_subjects(str);
  if (sizeof(otherstuff)) {
    news_rc[str] = otherstuff[sizeof(otherstuff)-1][B_TIME];
    BOARD_HAND->set_newsrc(this_player()->query_name(), news_rc);
  }
  printf("Marked all of "+str+" as read.\n");

  if(move_on) {
    // Needed coz skipping a large number of messages can be a little costly.
    reset_eval_cost();
    if ((start = member_array(board_name, boards)) == -1)
      start = 0;
    for(i=0; i<sizeof(boards); i++) {
      j = (i + start) % sizeof(boards);
      if(!BOARD_HAND->query_killfile(this_player()->query_name(),
                                     lower_case(boards[j])) &&
         k = calc_new(this_player(), boards[j])) {
        found = 1;
        break;
      }
    }
    
    if(found) {
      board_name = boards[j];
      write("Okay, changed to board "+board_name+" with " + k +
            " new notes.\n");
    } else
      write("No boards with new messages.\n");
  }
  return 1;
} /* do_skip() */

int do_arts() {
  int i;
  mixed *otherstuff;

  otherstuff = (mixed *)BOARD_HAND->get_subjects(board_name);
  for (i=0;i<sizeof(otherstuff);i++)
    printf("%2d# %-20s %d\n", i, otherstuff[i][B_NAME], otherstuff[i][B_NUM]);
  return 1;
} /* do_arts() */

int do_transfer(int num, string str) {
  string mess;
  string *boards;
  mixed *otherstuff;

  boards = (string *)BOARD_HAND->query_boards();
  otherstuff = (mixed *)BOARD_HAND->get_subjects(board_name);
  if (member_array(str, boards) == -1) {
    add_failed_mess("Cannot find the board '"+str+"'.\n");
    return 0;
  }
  if (num < 1 || num > sizeof(otherstuff)) {
    add_failed_mess("There is no note '"+num+"' on the board '"+
                        board_name+".\n");
    return 0;
  }
  mess = BOARD_HAND->get_message(board_name, num-1);
  if (mess && BOARD_HAND->add_message(str, otherstuff[num-1][B_NAME],
                              otherstuff[num-1][B_SUBJECT],
                              mess)) {
    write("Transfered note "+num+" to board '"+str+
                   "', local copy retained.\n");
    return 1;
  }
  add_failed_mess("Unable to transfer note "+num+" to the board '"+
                      board_name+", no security privilages?\n");
  return 0;
} /* do_transfer() */
