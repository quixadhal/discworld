/*  -*- LPC -*-  */
#include <creator.h>

inherit "/cmds/base";

#define CMD_NUM 5

mapping globals = ([]);
mapping ret = ([]);
mapping cmds = ([]);
mapping comments = ([ ]);
mapping locks = ([ ]);
mapping completed = ([ ]);

#define TP globals[fd]
#define RET ret[fd]
#define CMDS cmds[this_player()]
#define COMMENT comments[TP]

void ask_about_file(object player);
void get_answer(string answer, object player);
void start_input(object player);

int cmd() {
   locks[this_player()] = RCS_HANDLER->query_locks(this_player());
   if (sizeof(locks[this_player()]) == 0)  {
      printf("You don't have any files locked.\n");
      map_delete(locks, this_player());
      return 1;
   }

   ask_about_file(this_player());
   return 1;

} /* cmd() */


void check_complete(object player)  {
   if (!completed[player])
      call_out("check_complete", 1, player);
   else  {
      map_delete(completed, player);
      ask_about_file(player);
   }
}


void ask_about_file(object player)  {
   string tmp;

   tmp = sprintf("Check in %s? (y/N/q)\n", locks[player][0]);
   tell_object(player, tmp);
   input_to("get_answer", 0, player);

} /* ask_about_file() */


void start_input(object player)  {
   unguarded((:input_to((: get_answer :), 0, $(player)) :));
}


void get_answer(string answer, object player)  {
   string  arg;
   string *tmp;

   if (answer == "y"  ||  answer == "Y")  {
      tmp = explode(locks[player][0], "/");
      if (file_size("/" + implode(tmp[0 .. <2], "/") + "/RCS/" + tmp[<1] +
                    ",v") > 0)
      {
         arg = locks[player][0][1 .. ];
         tell_object(player, "Enter a comment.\n");
         CMDS = arg;
         player->do_edit("", "do_ci", this_object(), "", player);
         return;
      }
      else  {
         arg = sprintf("ERROR: No RCS file for %s\n", locks[player][0]);
         tell_object(player, arg);
         if (sizeof(locks[player]) > 1)  {
            locks[player] = locks[player][1 .. ];
            ask_about_file(player);
         }
         else
            map_delete(locks, player);
      }
   }
   else if (answer == "q"  ||  answer == "Q")  {
      map_delete(locks, player);
      return;
   }
   else if (sizeof(locks[player]) > 1)  {
      locks[player] = locks[player][1 .. ];
      ask_about_file(player);
   }
   else
      map_delete(locks, player);

} /* get_answer() */


void do_ci(string comment, object player) {
   int fd;
   string *cmd = allocate(3);

   if (!comment) {
      tell_object(player, "No comment given, skipping.\n");
      if (sizeof(locks[player]) > 1)  {
         locks[player] = locks[player][1 .. ];
         ask_about_file(player);
      }
      else
         map_delete(locks, player);
   }
   else  {
      cmd[0] = "-w" + player->query_name();
      cmd[1] = "-u";
      cmd[2] = "-m" + comment;
      cmd += explode(CMDS, " ");

#ifdef DEBUG  
      tell_object(player, "CMD: %O\n", cmd);
#endif  
      fd = external_start(CMD_NUM, cmd, "read_call_back", "write_call_back",
                          "close_call_back");

      TP = player;
      RET = "";
      COMMENT = comment;

      if (sizeof(locks[player]) > 1)  {
         completed[player] = 0;
         tell_object(player, "Checking in... please wait\n");
         call_out("check_complete", 1, player);
      }
   }

} /* do_ci() */


// I _think_ this is the function to write the comment string to the
// ci command
void read_call_back(int fd, mixed mess) {
  mess = replace(mess, "/home/atuin/lib", "");
  RET += mess;
}


void write_call_back(int fd) {
  tell_object(TP, "rcsin: Write_call_back called.\n");
}


void close_call_back(int fd) {
   string  file;
   string *file_stuff;
   string *bits;
   string  log;
   string  lname;
   int     i;
   object  master;
  
   if (RET != "") {
      TP->more_string(RET);
      file_stuff = explode(RET, "\n");
      i = 0;
      while (i + 2 < sizeof(file_stuff))  {
         if (file_stuff[i + 2] == "done")  {  /* success! */
            sscanf(file_stuff[i], "%*s  <--  %s", file);
            RCS_HANDLER->remove_lock(TP, file);
        
            if (file[0] == 'w')
               lname = "";
            else if (file[0] == 'd') {
               bits = explode(file, "/");
               if (sizeof(bits) >= 2 &&
                   member_array(bits[1],
                                "/secure/master"->query_domains()) != -1)
               {
                  master = find_object("/d/" + bits[1] + "/master");
                  if (!master)
                     TP->tell_object("No master object for domain: " +
                                     bits[1] + ".\n");
                  else
                     lname = master->query_changelog(file);
               }
               if (!lname)
                  lname = "/d/" + explode(file, "/")[1] + "/ChangeLog";
            }
            else
               lname = "/log/ChangeLog";

            if (lname != "") {
               log = TP->fix_string(sprintf(" * %s %s %s\n%s\n",
                                            ctime(time())[4 .. ],
                                            file, TP->query_name(),
                                            COMMENT), 80, 21);
               log_file(lname, log);
            }
            i += 3;
         }
         else if (sscanf(file_stuff[i], "%*sNo such file or directory"))
            ++i;
         else if (file_stuff[i + 1][0 .. 2] == "ci:")
            i += 2;
         else
            i += 3;
      }
   }
   else
      tell_object(TP, "rcsin completed.\n");
  
   if (sizeof(locks[TP]) > 1)  {
      locks[TP] = locks[TP][1 .. ];
      completed[TP] = 1;
   }
   else  {
      map_delete(completed, TP);
      map_delete(locks, TP);
   }
  
   map_delete(comments, TP);
   map_delete(ret, fd);
   map_delete(globals, fd);

} /* close_call_back() */
