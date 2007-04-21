/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: rcsc_reate.c,v 1.9 2000/03/04 00:07:55 turrican Exp $
 * $Log: rcsc_reate.c,v $
 * Revision 1.9  2000/03/04 00:07:55  turrican
 * Modified the order of things so you can use rcs commands on multiple .c files
 *
 * Revision 1.8  1998/04/02 05:11:45  ceres
 * Removed the log adding bit. It's somewhat unnecessary since rcslog will give the same info.
 *
 * Revision 1.7  1998/03/11 16:43:32  pinkfish
 * Figuredout what it was supposed to be for and set it up
 * to do that.  (cmd_str).
 *
 * Revision 1.6  1998/03/11 16:36:53  pinkfish
 * No idea whct cmd_str was supposed to do.  It is certainly
 * not set to anything right now...
 *
 * Revision 1.5  1998/03/09 15:57:34  turrican
 * Now uses the editor to enter the comment.
 *
 * Revision 1.4  1998/02/24 17:18:00  turrican
 * REALLY fix up the dollar sign stuff :)
 *
 * Revision 1.3  1998/02/15 17:19:36  pinkfish
 * Really fix up the dollars sign stuff :)
 *
 * Revision 1.2  1998/02/15 17:17:25  pinkfish
 * Fix up the comment stuff so that it does not get manged when checked in/out.
 *
 * Revision 1.1  1998/02/15 17:02:58  pinkfish
 * Initial revision
 *
 * Revision 1.1  1998/01/06 05:10:49  ceres
 * Initial revision
 * 
 */
inherit "/cmds/base";
#include <creator.h>

#define CMD_NUM 5

mapping globals = ([]), ret = ([]);
string cmd_str;

#define TP globals[fd]
#define RET ret[fd]

mixed cmd(string arg) {
  string *files;
  int nfiles = 0;
  string bit, *bits;
  int add_comments;
  string tmp;
  
  if (!arg) {
    return notify_fail("rcscreate: No arguments.\n");
  }

  notify_fail("rcscreate: no such file "+arg+".\n");
  
  bits = explode(arg, " ");
  arg = "";
  bits -= ({""});

  foreach (bit in bits) {
    string  file;
    if (bit[0] == '-') {
      if (bit[1] == 'c') {
        add_comments = 1;
      } else {
        arg += (" " + bit);
      }
      continue;
    }
    
    files = this_player()->get_files(bit);
    foreach (file in files) {
      if (master()->valid_write(file, geteuid(this_player()), "cmd")) {
        string dir, *tmpbits;

        // Make sure there is an RCS directory here. If not make one.
        tmpbits = explode(file, "/");
        /* Do not let the rcs create create rcs entries for these files. */
        if (tmpbits[<1] == "RCS" ||
            tmpbits[<1] == "." ||
            tmpbits[<1] == ".." ||
            tmpbits[<1] == "ERROR_REPORTS") {
          continue;
        }
        if (sizeof(tmpbits) > 1) {
          dir = "/" + implode(tmpbits[0..<2], "/") + "/RCS";
        } else {
          dir = "/RCS";
        }
        
        if (file_size(dir) == -1) {
          write("No directory "+dir+", creating one.\n");
          mkdir(dir);
        }
        
        // insert the standard comments at the top of the file
        if (add_comments) {
          tmp = read_file(file);
          if ( file[ strsrch( file, ".", -1 ) .. ] == ".c"  ||
               file[ strsrch( file, ".", -1 ) .. ] == ".h" )  {
            tmp = replace_string(tmp, "/*  -*- LPC -*-  */\n", "");
            tmp = replace_string(tmp, "/* -*- LPC -*- */\n", "");
  
            // Split this write over multiple lines to avoid the rcs auto
            // replacement stuff for these strings.
            write_file(file,
                       "/*  -*- LPC -*-  */\n"
                       "/*\n"
                       " * $", 1);
            write_file(file,
                       "Locker$\n"
                       " * $");
            write_file(file,
                       "Id$\n"
                       " *\n"
                       " *\n"
            //           " * $");
            //          write_file(file,
            //           "Log$\n"
                       " */\n\n" + tmp);
          }
          else  {
            tmp = replace_string( tmp, "#  -*- LPC -*-  #\n", "" );
            tmp = replace_string( tmp, "# -*- LPC -*- #\n", "" );
            write_file( file,
                        "#  -*- LPC -*- #\n"
                        "#\n"
                        "# $", 1 );
            write_file( file,
                        "Locker$\n"
                        "# $");
            write_file(file, "Id$\n"
                        "#\n"
                        "#\n"
                        "#\n\n" + tmp );
          }
        }
        arg += (" " + file[1..]);
        nfiles++;
      } else {
        notify_fail("You do not have write access to "+file+"\n");
      }
    }
  }

  if (!nfiles) {
    return 0;
  }

  printf("Enter a comment.\n");
  cmd_str = arg;
  this_player()->do_edit(0, "do_ci");
  return 1;
}

void do_ci(string comment) {
  int fd;
  string *cmd = allocate(4);

  if (!comment) {
    printf("No comment given, aborting.\n");
    return;
  }

  cmd[0] = "-w" + this_player()->query_name();
  cmd[1] = "-i";
  cmd[2] = "-u";
  cmd[3] = "-t-" + comment;
  cmd += explode(cmd_str, " ");
#ifdef DEBUG  
  printf("CMD: %O\n", cmd);
#endif  
  fd = external_start(CMD_NUM, cmd, "read_call_back", "write_call_back",
                      "close_call_back");
    
  TP = this_player();
  RET = "";
}

// I _think_ this is the function to write the comment string to the ci command
void read_call_back(int fd, mixed mess) {
  mess = replace_string(mess, "/home/atuin/lib", "");
  RET += mess;
}

void write_call_back(int fd) {
  tell_object(TP, "rcscreate: Write_call_back called.\n");
}

void close_call_back(int fd) {

  if (RET != "") {
    TP->more_string(RET);
  } else {
    tell_object(TP, "rcscreate completed.\n");
  }
  map_delete(ret, fd);
  map_delete(globals, fd);
}
