/*  -*- LPC -*-  */
/*
 * $Id: rcsf_orce.c,v 1.16 2001/09/11 08:28:17 taffyd Exp $
 */
inherit "/cmds/base";
#include <creator.h>
#include <mail.h>
#include <player_handler.h>

//#define DEBUG 1

#define CMD_NUM 5

class rcsforce_args {
   object player;
   string *cmd;
   string locker;
   string filen;
}

private mapping _globals = ([ ]);
private mapping _ret = ([ ]);
private mapping _lockers = ([ ]);

#define TP _globals[fd]
#define RET _ret[fd]
#define LCK _lockers[fd]

private void do_ci(string text,
                   class rcsforce_args args);

mixed cmd(string arg)
{
   int nfiles = 0;
   string bit;
   string *bits;
   string file;
   string locker;
   string filen;
   string *cmd = allocate(3);

/*
  if (!master()->query_senior(this_player()->query_name())) {
    return 0;
  }
 */

   if (!arg) {
      return notify_fail("rcsforce: No arguments.\n");
   }

   bits = explode(arg, " ");
   arg = "";
   bits -= ({ "", 0 });

   foreach(bit in bits) {
      string *files;
      if (bit[0] == '-') {
         arg += (" " + bit);
         continue;
      }

      files = (string *) this_player()->get_files(bit);
      if (sizeof(files)) {
         filen = file = files[0];
         arg += (" " + file[1..]);
         nfiles++;
      }
   }

   if (!nfiles) {
      return notify_fail("rcsforce: no such file " + arg + ".\n");
   }
   // Now check if the file is locked and who it is locked by!

   bits = explode(file, "/");
   file =
      "/" + (string) implode(bits[0.. < 2], "/") + "/RCS/" + bits[<1] + ",v";

   if (file_size(file) < 0) {
      return notify_fail("That file is not in RCS.\n");
   }

   if (strsrch(read_file(file, 4, 1), "locks; strict:") != -1) {
      return notify_fail("That file is not locked.\n");
   } else {
      bits = explode(read_file(file, 5, 1), ":");
      locker = bits[0][1..];
   }

   // Allow anyone to unlock files by non-creators and anyone above
   // a senior cre can unlock anything.
   if (!master()->query_senior(this_player()->query_name()) &&
       PLAYER_HANDLER->test_creator(locker)) {
      // Check for a domain.
      if (file[0..2] == "/d/") {
         bits = explode(file, "/");
         if (!("/d/" + bits[1] + "/master")->can_rcsforce(file,
                                             this_player()->query_name(),
                                             locker)) {
            return 0;
         }
      } else {
         return 0;
      }
   }

   cmd[0] = "-w" + locker;
   cmd[1] = "-u";
   cmd[2] = "-m Forcibly unlocked by " + this_player()->query_name();
   cmd += explode(arg, " ");

#ifdef DEBUG
   printf("CMD: %O\n", cmd);
#endif

   if (PLAYER_HANDLER->test_creator(locker)) {
      printf("Edit mail? (y/[n]): ");
    input_to("edit_mail", 0, new (class rcsforce_args, player: this_player(), cmd: cmd, locker: locker, filen:filen));
   } else {
      int fd;
      write("Not sending mail to " + locker + " since they are not a creator "
            "any more.\n");

      fd = external_start(CMD_NUM, cmd, "read_call_back", "write_call_back",
                       "close_call_back");

      TP = this_player();
      RET = "";
      LCK = locker;
   }
   return 1;
}

protected void edit_mail(string choice,
                         class rcsforce_args args)
{
   if (!strlen(choice) || lower_case(choice) == "n") {
      printf("No.\n");
      do_ci(0, args);
      return;
   }

   if (lower_case(choice) != "y") {
      printf("Invalid choice. Please answer y or n. (y/[n]): ");
      input_to("edit_mail", 0, args);
   } else {
      printf("Yes. Entering editor.\n");
      args->player->do_edit(0, "do_ci", this_object(), 0, args);
   }
}

/* This canNOT be made private.  /global/line_ed needs to call it */
void do_ci(string text,
           class rcsforce_args args)
{
   int fd;

   AUTO_MAILER->auto_mail(args->locker, args->player->query_name(),
                          "Automatic RCSForce Mail", "",
                          sprintf("Your file: %s, has been forcibly "
                                  "unlocked by: %s.\nEnjoy.\n\n"
                                  "Automatic RCSForce Mailer.\n", args->filen,
                                  args->player->query_name()) +
                          (strlen(text) ? "\n" + text : ""));

   fd =
      external_start(CMD_NUM, args->cmd, "read_call_back", "write_call_back",
                     "close_call_back");

   TP = args->player;
   RET = "";
   LCK = args->locker;
}

// I _think_ this is the function to write the comment string to the ci command
void read_call_back(int fd,
                    mixed mess)
{
   mess = replace(mess, "/home/atuin/lib", "");
   RET += mess;
}

void write_call_back(int fd)
{
   tell_object(TP, "rcsforce: write_call_back() called.\n");
}

void close_call_back(int fd)
{
   string file,
    *file_stuff;
   int i;

   if (RET != "") {
      TP->more_string(RET);
      file_stuff = explode(RET, "\n");
      i = 0;
      while (i + 2 < sizeof(file_stuff)) {
         if (file_stuff[i + 2] == "done") {     /* success! */
            sscanf(file_stuff[i], "%*s  <--  %s", file);
            RCS_HANDLER->remove_lock(LCK, file);
            i += 3;
         } else {
            if (file_stuff[i + 1][0..2] == "ci:") {
               i += 2;
            } else {
               i += 3;
            }
         }
      }
   } else {
      tell_object(TP, "rcsforce completed.\n");
   }

   map_delete(_ret, fd);
   map_delete(_globals, fd);
   map_delete(_lockers, fd);
}
