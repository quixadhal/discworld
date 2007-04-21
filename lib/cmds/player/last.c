/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: last.c,v 1.15 2003/03/21 01:53:51 ceres Exp $
 * 
 */
/* last command, trial out by Turrican for a commands daemon. */

#include <player_handler.h>

inherit "/cmds/base";

#define BASENAME "/d/admin/log/ENTER"

string process_file(string fname, string str);
string get_details(string str, string file);
string mtime(string t);
string stime(string t);

nosave int logged_on;
nosave string time;

mixed cmd(string str) {
  string retval;
  int i;

  write("This command is no longer available.\n");
  return 1;
  
  if (!str || str == "") {
    notify_fail("Usage: last <name>\n");
    return 0;
  }

  if(!this_player()->query_creator() &&
     PLAYER_HANDLER->test_creator(str)) {
    return notify_fail("Sorry, you can't do that, "+ str + " is a "
           "creator.\n");
  }
  if("/secure/master"->query_lord(str) &&
     !"/secure/master"->query_lord(this_player()->query_name())) {
    return notify_fail("Sorry, you can't do that, "+ str +" is a "
           "lord\n");
  }

  logged_on = 0;
  retval = "";
  time = "";
  str = (string)this_player()->expand_nickname(str);

  retval = process_file(BASENAME + ".log", str);

  // check old logs _if_ we haven't found them
  //  if(retval == "")
  //retval += process_file(BASENAME+".1", str);
  
  if(logged_on == 1) {
    if (!find_player(str)) {
      retval += sprintf("%15-s %s - Destd (??:??)\n", str, mtime(time));
    } else {
      retval += sprintf("%15-s %s - Not logged off\n", str, mtime(time));
    }
  }
  
  if(retval == "") {
    if(PLAYER_HANDLER->test_user(str)) {
      i = (int)PLAYER_HANDLER->test_last(str);
      if(i) {
        write(sprintf("%15-s %s\n", str, ctime(i)));
        return 1;
      }
    }
    write("No such user " + str + "\n");
  } else {
    write(retval);
  }
  return 1;
}

string process_file(string fname, string str) {
  int size, bits, length, i;
  string retval, file;

  retval = "";

  seteuid("Root");

  // if the file is more than 50k then break it up and read it bit by bit.
  size = file_size(fname);
  if(size > 50000) {
    length = file_length(fname);
    bits = size / 50000;
  } else {
    bits = 1;
  }

  if(bits > 10) {
    i = bits - 10;
  } else {
    i = 0;
  }
  
  for(; i< bits; i++) {
    file = unguarded((: read_file, fname,
          i * (length/bits), length / bits :) );
    if (!file) {
      write("Log file non existant, or bad things are happening.  "
      "(Bad vibes man!)\n");
      return "";
    }

    retval += get_details(str, file);
  }
  seteuid("Room");
  return retval;
}

string get_details(string str, string file) {
  string *sarr;
  string retval;
  string t2;
  string type;
  string time_str;
  int i;
  
  retval = "";
  
  sarr = explode(file, "\n");

  for (i = 0; i < sizeof(sarr); i++) {
    //if (sscanf(sarr[i], "%s [%s] %*s" + str + " ", time_str, type) == 3)  {
    if (sscanf(sarr[i], "%s [%s] " + str + " ", time_str, type) == 2 ||
        sscanf(sarr[i], "%s [%s]  " + str + " ", time_str, type) == 2)  {
      switch(type)  {
        case "Enter":
          time = time_str;
          logged_on = 1;
          break;

        case "Recon":
          time = time_str;
          logged_on = 1;
          break;

        case "Su":
          t2 = time_str;
          logged_on = 0;
          retval += sprintf("%15-s %s - %s [Quit]\n", str, mtime(time),
                            stime(t2));
          break;
   
        case "NDead":
          t2 = time_str;
          logged_on = 1;
          retval += sprintf("%15-s %s - %s [NetDead]\n", str, mtime(time),
                    stime(t2));
          time = "";
          break;

        case "Exit":
          t2 = time_str;
          logged_on = 0;
          retval += sprintf("%15-s %s - %s [Quit]\n", str, mtime(time),
                            stime(t2));
          break;

        default:
          write("Someone has stuffed up... please file a bug report\n");
          break;
      }
    } 
  }
  
  return retval;
}

string mtime(string t) {
  if(t == "")
    return "??? ?? ??:??";
  return t[0..<4];
}

string stime(string t) {
  if(t == "")
    return "??:??";
  return t[7..11];
}

