/*  -*- LPC -*-  */
/*
 * $Id: rcsi_n.c,v 1.28 2002/11/18 01:34:20 ceres Exp $
 */
/**
 * Command to perform an RCS "ci -u", ie. to checkin a file and retrieve
 * a read-only version.
 *
 * @author Ceres
 */
inherit "/cmds/base";
#include <creator.h>

#define CMD_NUM 5
//#define DEBUG

mapping globals = ([]), ret = ([]), cmds = ([]), comments = ([ ]);

#define TP globals[fd]
#define RET ret[fd]
#define CMDS cmds[this_player()]
#define COMMENT comments[TP]

mixed cmd(string arg) {
  int nfiles = 0;
  string bit, *bits, *tmp, *notin;
  object *things;
  
  if (!arg) {
    return notify_fail("rcsin: No arguments.\n");
  }

  bits = explode(arg, " ");
  arg = "";
  bits -= ({ "", 0 });
  notin = ({ });
  
  foreach(bit in bits) {
    string *files = ({ }), file;
    if (bit[0] == '-') {
      arg += (" " + bit);
      continue;
    }

    if (sizeof(files = this_player()->get_files(bit))) {
      foreach(file in files) {
        tmp = explode(file, "/");
        if(file_size("/" + implode(tmp[0..<2], "/") + "/RCS/" + tmp[<1]+",v") >
           0) {
          nfiles++;
          arg += (" " + file[1..]);
        } else {
          notin += ({ file });
        }
      }
    } else {
      if(sizeof(things = WIZ_PRESENT->wiz_present(bit, this_player()))) {
        file = file_name(things[0]);
        sscanf(file, "%s#%*d", file);
        if (file_size(file) <= 0)
          file += ".c";

        tmp = explode(file, "/");
        if(file_size("/" + implode(tmp[0..<2], "/") + "/RCS/" + tmp[<1]+",v") >
           0) {
          nfiles++;
          arg += (" " + file[1..]);
        } else {
          notin += ({ file });
        }
      }
    }
  }

  if(!nfiles) {
    if(sizeof(notin))
      return notify_fail("rcsin: file not in RCS " +
                         query_multiple_short(notin) + ".\n");
    else
      return notify_fail("rcsin: no such file "+arg+".\n");
  }

  if(sizeof(notin))
    write("rcsin: file not in RCS " + query_multiple_short(notin) + ".\n");

  printf("Enter a comment.\n");
  CMDS = arg;
  this_player()->do_edit(0, "do_ci");
  return 1;
}

void do_ci(string comment) {
  int fd;
  string *cmd = allocate(3);

  if (!comment) {
    printf("No comment given, aborting.\n");
    return;
  }

  cmd[0] = "-w" + this_player()->query_name();
  cmd[1] = "-u";
  cmd[2] = "-m"+comment;
  cmd += explode(CMDS, " ");


#ifdef DEBUG  
  printf("CMD: %O\n", cmd);
#endif  
  fd = external_start(CMD_NUM, cmd, "read_call_back", "write_call_back",
                      "close_call_back");
    
  TP = this_player();
  RET = "";
  COMMENT = comment;
}

// I _think_ this is the function to write the comment string to the ci command
void read_call_back(int fd, mixed mess) {
  mess = replace(mess, "/home/atuin/lib", "");
  RET += mess;
}

void write_call_back(int fd) {
  tell_object(TP, "rcsin: Write_call_back called.\n");
}

void close_call_back(int fd) {
  string file, *file_stuff, *bits, log, lname;
  int i;
  object master;
  
  if (RET != "") {
    TP->more_string(RET);
    file_stuff = explode(RET, "\n");
    i = 0;
    while (i + 2 < sizeof(file_stuff))  {
      if (file_stuff[i + 2] == "done")  {  /* success! */
        sscanf(file_stuff[i], "%*s  <--  %s", file);
        RCS_HANDLER->remove_lock(TP, file);
        
        if(file[0] == 'w')
          lname = "";
        else if(file[0] == 'd') {
          bits = explode(file, "/");
          if(sizeof(bits) >= 2 &&
             member_array(bits[1], "/secure/master"->query_domains()) != -1) {
            master = find_object("/d/" + bits[1]+"/master");
            if(!master)
              TP->tell_object("No master object for domain: " + bits[1] +
                              ".\n");
            else
              lname = master->query_changelog(file);
          }
          if(!lname)
            lname = "/d/" + explode(file, "/")[1] + "/ChangeLog";
        } else
          lname = "/log/ChangeLog";

        if(lname != "") {
          log = TP->fix_string(sprintf(" * %s %s %s\n%s\n",
                                       ctime(time())[4..],
                                       file, TP->query_name(),
                                       COMMENT), 80, 21);
          log_file(lname, log);
        }
        i += 3;
      } else if (sscanf(file_stuff[i], "%*sNo such file or directory"))  {
        ++i;
      } else if (file_stuff[i + 1][0..2] == "ci:") {
          i += 2;
      } else {
          i += 3;
      }
    }
  } else {
    tell_object(TP, "rcsin completed.\n");
  }
  
  map_delete(ret, fd);
  map_delete(globals, fd);
  map_delete(comments, TP);
}
