/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: find.c,v 1.12 2002/03/25 20:33:37 wodan Exp $
*/

#include <creator.h>

#define WIZ WIZ_PRESENT 
#define EXTRACT_CODE 1
#define LINE_NUMBERS 2
#define FIND_ALL 4
 
inherit "/cmds/base";

int file_exists(string str) { return (file_size(str) > -1); }
 
mixed cmd(string str) {
  string func, thing, s, ping;
  object *obs, fish, user = this_player();
  object *list;
  mixed *fnd;
  int i, flags;
  int k;
 
  notify_fail("Usage: find [-a] [-d] [-n] function() <object(s)>\n");
 
  if (!str)
     return 0;
  
  while (sscanf(str, "-%s %s", s, str) == 2)
    switch (s) {
    case "d":
        flags |= EXTRACT_CODE;
        flags &= ~FIND_ALL;
        break;
    case "n":
        flags |= (LINE_NUMBERS | EXTRACT_CODE);
        flags &= ~FIND_ALL;
        break;
    case "a":
        flags = FIND_ALL;
        break;
      default:
        return notify_fail("Unknown option '"+ s +"'.\n");
    }
 
  if (sscanf(str, "%s() %s", func, thing) != 2)
     if(sscanf(str, "%s %s", func, thing) != 2)
        return 0;
 
  obs = WIZ -> wiz_present(thing, user);
 
  if (!sizeof(obs))
    return notify_fail("Can't find " + thing + ".\n");
 
  s = "";
  fnd = ({ });
 
  for (i = 0; i < sizeof(obs); i++) {
     if (flags & FIND_ALL)
        list = ({ obs[i] }) + map(deep_inherit_list(obs[i]),
               (: find_object($1) :));
     else list = ({ obs[i] });
     for (k = 0; k < sizeof(list); k++)  {
        if ((ping = function_exists(func, list[k], 1))  &&
            member_array(ping, fnd) == -1)
        {
           s += "*** " + WIZ->desc_object(list[k]) + ": " + func +
                "() found in " + ping + "\n";
           fnd += ({ func, ping });
        } else if (!(flags & FIND_ALL))
           s += "*** " + WIZ->desc_object(list[k]) + ": " + func +
                "() not found.\n";
    
        fish = list[k];
    
        while (fish = shadow(fish, 0))
           if (function_exists(func, fish, 1)) {
              s += "      Shadowed by " + file_name(fish) + "\n";
              fnd += ({ func, base_name(fish) });
           }
     }
  }
 
  if ((flags & FIND_ALL)  &&  !sizeof(fnd))
     s += "*** " + func + "() not found.\n";

  /* If we are going to extract a function, there are many different possible
     formats it might be declared in:
 
     void foo*(*)*{
 
     Take * as indicating where whitespace might split the header, whitespace
     possibly being a space, a tab or a new line. */
 
  if ((flags & EXTRACT_CODE) && sizeof(fnd)) {
    if (sizeof(fnd) > 2)
      s += "Can only extract code from one object at a time.\n";
    else {
      mixed tmp;
      int j = 0;
      int startline;
      ping = fnd[1] +".c";
      func = fnd[0];
      i = 0;
 
      if (!file_exists(ping))
        return notify_fail("The file for "+ thing +" doesn't exist.\n");

      tmp = read_file(ping);

      if(!strlen(tmp))
        return notify_fail("Could not read file.\n");
      
      tmp = reg_assoc(tmp,  ({"\n[a-z_ \t]*[\\* ][ \t]*" + func +
                    "[ \t]*\\([a-zA-Z0-9, _\\*\n\t]*(...)?[ \t]*\\)[ \t\n]*{"
                                , "\n}"})
                      , ({1, 2}));
      
      
      while(tmp[1][j] != 1)
        j++;

      if(flags & LINE_NUMBERS){
        startline = sizeof(explode(implode(tmp[0][0..j], ""), "\n"));
      }
      
      j++;
      while(tmp[0][j][i] != '\n'){
        if(tmp[0][j][i] == '}'){
          tmp = tmp[0][j - 1][1..] + tmp[0][j][0..i];
          if(flags & LINE_NUMBERS)
            tmp = implode(explode(tmp, "\n"),
                           (:({$1[0]+1, $1[1] + "\n" + $1[0] + " " + $2}):),
                           ({startline, ""}))[1];
          user->more_string(sprintf("%s%s", s, tmp), 0, 1);
          return 1;
        }
        i++;
      }

      tmp = implode(tmp[0][j-1..j+1], "");
      if(flags & LINE_NUMBERS)
        tmp = implode(explode(tmp, "\n"),
                      (:({$1[0]+1, $1[1] + "\n" + $1[0] + " " + $2}):),
                      ({startline, ""}))[1];
      
      s += tmp;
    }
  }
 
  user->more_string(s, 0, 1);
 
  return 1;
}
