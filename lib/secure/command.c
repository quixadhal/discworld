/*   -*- LPC -*-   */
/*
 * $Locker:  $
 * $Id: command.c,v 1.16 2003/06/09 19:22:31 pinkfish Exp $
 */
/*      /daemon/command.c
 *      from the Nightmare IVr1 Object Library
 *      a new commands daemon, much faster than the old
 *      created by Descartes of Borg 940119
 *      Hacked for Discworld by Turrican 9-11-95
 */

#include <command.h>
#include <user_parser.h>

private nosave mapping Commands, Cache;
private nosave string *Paths, *GRCommands;
private nosave string last_verb, last_args, found_cmd, found_args;

void eventRehash(string *paths);

void create() {
   seteuid(getuid());
   Commands = ([]);
   Cache = ([ ]);
   GRCommands = ({});
   Paths = ({});
   eventRehash( ({ DIR_PLAYER_CMDS, DIR_CREATOR_CMDS, DIR_SECURE_CREATOR_CMDS,
                   DIR_LORD_CMDS, DIR_LIVING_CMDS, DIR_GUILD_CMDS,
                   DIR_PLAYTESTER_CMDS }) );
} /* create() */

void eventGuildRaceRehash() {
   string *paths, path;

   paths = ({ DIR_GUILD_CMDS });
   paths += map(filter(unguarded((: get_dir, DIR_GUILD_CMDS + "/", -1 :)),
                                 (: $1[1] == -2 :)),
                      (: DIR_GUILD_CMDS + "/" + $1[0] :));
   GRCommands = ({ });
   foreach (path in paths) {
      string *files, cmd;

      files = map(unguarded((: get_dir, path + "/*.c" :)),
                        (: $1[0..<3] :));
      GRCommands += files;
      foreach(cmd in files) {
         if (pointerp(Commands[cmd])) {
            Commands[cmd] += ({ path });
         } else {
            Commands[cmd] = ({ path });
         }
      }
      Paths = uniq_array(Paths + ({ path }));
   }
} /* eventGuildRaceRehash() */

void eventRehash(mixed paths) {
   string path;

   // Clear the cache since rehashing the paths may affect the results.
   Cache = ([ ]);
   
   if (stringp(paths)) {
      paths = ({ paths });
   } else if (!pointerp(paths)) {
      return;
   }
   
   foreach(path in paths) {
      string file;

      if  ( path[<1..<1] == "/" ) continue; 
      if (file_size(path) != -2)
         continue;
      if (path == DIR_GUILD_CMDS) {
         eventGuildRaceRehash();
         continue;
      }
      foreach (file in unguarded((: get_dir, path + "/*.c" :))) {
         string cmd;

         cmd = file[0..<3];
         if (pointerp(Commands[cmd])) {
            Commands[cmd] += ({ path });
         } else {
            Commands[cmd] = ({ path });
         }
      }
      Paths = uniq_array(Paths + ({ path }));
   }
} /* eventRehash() */

void HandleCommandLine(class command cmd) {
   string args = cmd->args;
   int i;
   string* bits;

   //args = replace_string(args, "%^", " ");
/*
   if (strsrch("%^", args)) {
      bits = explode("f" + args, "%^");
      for (i = 1; i < sizeof(bits); i += 2) {
         bits[i] += "USER_";
      }
      bits[0] = bits[0][1..];
      args = implode(bits, "%^");
   }
 */
   
   if ((i = strsrch(args, " ")) != -1) {
      cmd->verb = args[0..i-1];
      cmd->args = args[i+1..];
   } else {
      cmd->verb = args;
      cmd->args = (string)0;
   }
} /* HandleCommandLine() */

int strncmp(string s1, string s2, int len) {
   return strcmp(s1[0..len-1], s2[0..len-1]);
} /* strncmp() */

int HandleStars(class command cmd) {
   int i;
   int no_cache;
   string file, *files;

   // No point continuing!
   if(cmd->verb == "END_ALIAS")
     return 0;

   // This function gets called a hell of a lot of times. It gets called
   // 4 times for _each_ command. So, lets remember some common results and 
   // not do all that searching over and over again.

   // exactly the same as last time.
   if(last_verb == cmd->verb && last_args == cmd->args) {
     cmd->verb = found_cmd;
     if(stringp(found_args)) {
       cmd->args = found_args;
     }
     return 1;
   }

   // check common patterns (can't do ones that end in _. 
   if(Cache[cmd->verb]) {
     if(stringp(Cache[cmd->verb][1]))
       cmd->args = Cache[cmd->verb][1];
     cmd->verb = Cache[cmd->verb][0];

     return 1;
   }
   
   if (Commands[cmd->verb]) {
     /* Yippiee ! No searching needed ! */
     return 1;
   }
   
   /* Darn. Need to search for shortcuts. */
   files = keys(Commands);
   files = filter(files, (: (int)$1[0] == $(cmd)->verb[0] :));

   foreach (file in files) {
     string tmpverb, tmpargs = 0, tmpfile = 0;
     int len, length;
     
     if ((i = strsrch(file, "_")) == -1)
       continue;

     tmpfile = (string)delete(file, i, 1);
     len = strlen(tmpfile);
     if (i == len) {
       tmpargs = cmd->verb[i..]+(cmd->args?(strlen(cmd->verb) == i?"":" ") + 
                                 cmd->args:"");
       tmpverb = cmd->verb[0..i-1];
       if (strncmp(tmpfile, tmpverb, len) != 0) {
         continue;
       }
       no_cache = 1;
     } else {       
       length = strlen(cmd->verb);
       if ((length > len) || (length < i))
         continue;
       
       if (strncmp(cmd->verb, tmpfile, length) != 0)
         continue;
     }
     
     /* Yay! Found a match! */
     if (!no_cache) {
        Cache[cmd->verb] = ({ file, tmpargs });
     }

     last_verb = cmd->verb;
     last_args = cmd->args;

     cmd->verb = file;
     if (stringp(tmpargs))
       cmd->args = tmpargs;
     found_cmd = cmd->verb;
     found_args = tmpargs;

     return 1;
   }
   return 0;
} /* HandleStars() */

int GetCommand(class command cmd, string *path) {
   string *tmp;
   int i;

   if (!stringp(cmd->args)) {
      return 0;
   }
   tmp = (path & Paths);
   if (sizeof(tmp = path - tmp)) {
      eventRehash(tmp);
   }
   HandleCommandLine(cmd);
   if (HandleStars(cmd) && Commands[cmd->verb] &&
       sizeof(tmp = (path & (string *)Commands[cmd->verb]))) {
      cmd->file = sprintf("%s/%s", tmp[0], cmd->verb);
      cmd->filepart = cmd->verb;
      if ((i = strsrch(cmd->verb, "_")) != -1) {
         cmd->verb = (string)delete(cmd->verb, i, 1);
      }
      if (cmd->args && cmd->args == "") {
         cmd->args = (string)0;
      }
      if ((cmd->file)->query_patterns()) {
         return 0;
      }
      return 1;
   }
   return 0;
} /* GetCommand() */

mixed ReturnPatterns(class command cmd, string *path) {
   mixed *q_patterns;
   mixed *r_patterns;
   mixed *stuff;
   mixed *tmp;
   mixed *ret_patterns;
   int i, j;
   string fname;

   tmp = (path & Paths);
   tmp = path - tmp;
   if (sizeof(tmp)) {
      eventRehash(tmp);
   }
   if (HandleStars(cmd) && Commands[cmd->verb]) {
      tmp = (path & (string *)Commands[cmd->verb]);
      if (tmp) {
         ret_patterns = ({ });
         foreach (fname in tmp) {
            cmd->file = sprintf("%s/%s", fname, cmd->verb);
            //if ((i = strsrch(cmd->verb, "_")) != -1) {
               //cmd->verb = (string)delete(cmd->verb, i, 1);
            //}
            q_patterns = (cmd->file)->query_patterns();
            if (!q_patterns) {
               return 0;
            }
            r_patterns = ({ });
            for (i = 0; i < sizeof(q_patterns); i += 2) {
               if (q_patterns[i+1] && !functionp(q_patterns[i+1])) {
                  continue;
               }
               stuff = (mixed *)PATTERN_OB->query_pattern(q_patterns[i]);
               j = 0;
               while (j < sizeof(r_patterns) && 
                         r_patterns[j][PATTERN_WEIGHT] >= stuff[0]) {
                  j++;
               }
               r_patterns = r_patterns[0..j-1] + 
                  ({ ({ stuff[0], q_patterns[i], 0, find_object(cmd->file),
                           q_patterns[i+1] }) }) + r_patterns[j..];
            }
            ret_patterns += r_patterns;
         }
         return ret_patterns;
      }
   }
   return 0;
} /* ReturnPatterns() */

mixed *GetCommandPatterns(string cmd_name, string *path) {
   class command cmd;
   mixed *stuff;

   cmd = new(class command);
   cmd->verb = cmd_name;
   cmd->args = 0;
   stuff = ReturnPatterns(cmd, path);
   return stuff;
} /* GetCommandPatterns() */

varargs string *GetCommands(string path) {
   string *paths, *tmp;
   string cmd;

   if (!path) {
      return keys(Commands);
   }
   tmp = ({ });
   foreach (cmd, paths in Commands) {
      if (member_array(path, paths) != -1) {
         tmp += ({ cmd });
      }
   }
   return tmp;
} /* GetCommands() */

varargs string *GetPaths(string cmd) {
   if (cmd) {
      string *paths;
      class command tmp;

      if (sizeof(paths = Commands[cmd])) {
         return paths;
      }
      tmp = new(class command, verb : cmd);
      if (HandleStars(tmp)) {
         return Commands[tmp->verb];
      } else {
         return 0;
      }
   }
   return Paths;
} /* GetPaths() */

int IsGRCommand(string cmd) {
   if (member_array(cmd, GRCommands) != -1) {
      return 1;
   } else {
      class command tmp = new(class command, verb : cmd);

      if (HandleStars(tmp)) {
         return (member_array(tmp->verb, GRCommands) != -1);
      }
   }
} /* IsGRCommand() */
