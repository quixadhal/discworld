#include <alias.h>

inherit "/cmds/base";

private nosave string gfilter;
/**
 * Turn the compiled alias array into a string.
 * @param al the alias array
 * @return the string value of the alias array
 * @see compile_alias()
 */
string alias_string(mixed *al) {
   int i;
   int num;
   int *add_thing;
   string str;
   
   str = "";
   add_thing = ({ });
   for (i=0;i<sizeof(al);i++) {
      if (stringp(al[i])) {
         /*
           al[i] = replace(al[i], "@@", "@ @ ");
         */
         str += replace(al[i], ({";", "\\;"}));
      } else {
         num = al[i] & ALIAS_MASK;
         switch (al[i] - num) {
         case NEW_LINE  : 
            str += ";";
            break;
         case ALL_ARGS  : 
            str += "$*$";
            break;
         case ONE_ARG   : 
            str += "$"+num+"$";
            break;
         case TO_ARG    : 
            str += "$*"+num+"$";
            break;
         case FROM_ARG  : 
            str += "$"+num+"*$";
            break;
         case ALL_ARG   : 
            str += "$arg:"+al[++i]+"$";
            break;
         case ARG_THING : 
            str += "$arg"+num+":"+al[++i]+"$";
            break;
         case ELSE_THING :
           str += "$else$";
           /*
             add_thing[sizeof(add_thing)-1] = i+1+al[++i];
           */
           break;
         case ALL_IFARG : 
            str += "$ifarg:";
            /*
              add_thing += ({ i+1+al[++i] });
            */
            break;
         case IFARG_THING :
            str += "$ifarg"+num+":";
            /*
              add_thing += ({ i+1+al[++i] });
            */
            break;
         case CURR_LOC :
            str += "$!$";
            break;
         case END_IF      :
            str += "$endif$";
            break;
         }
      }
      /*
        if (member_array(i, add_thing) != -1)
        str += "~$";
      */
   }
   return str;
} /* alias_string() */

/**
 * Prints out all the aliases on the player object.  This is called by the
 * alias command when no arguments are specified.
 * @return 0 if no aliases are defined, 1 if they are
 * @see alias()
 */
int print_aliases(string filter, int sorted) {
   int i;
   int len;
   string str;
   string str1;
   string str2;
   string *tmp;
   string bing;
   mapping aliases;
   string ret;
 
   aliases = this_player()->query_aliases();
   /* 
    * ahh well here goes the clean. you dont want to know what used to
    * be here ;)
    */
   if (!m_sizeof(aliases)) {
      notify_fail("None defined.\n");
      return 0;
   }
 
   str1 = "";
   str2 = "";

   tmp = m_indices(aliases);
   if (filter) {
      if (regexp(filter, 
           "[\\[\\]\\(\\)\\*\\?\\+][\\[\\]\\(\\)\\*\\?\\+]+") ) {
         add_failed_mess("Bad pattern to alias.\n");
         return 0;
      }
      if (filter[0] == '*' || filter[0] == '+') {
         add_failed_mess("Cannot start a regular expression with a '*' or '+', "
                         "try: '.*' or '.+'.\n");
         return 0;
      }
      gfilter = "^" + filter;
      if (catch(tmp = filter_array(tmp, (: $1 && regexp($1, gfilter) :)))) {
         add_failed_mess("Bad pattern to alias (mismatched brackets?).\n");
         return 0;
      }
   }
    
   tmp = sort_array(tmp, 1);
   ret = "You currently have the following aliases:\n";
   for (i = 0; i < sizeof(tmp); i++) {
      if (!tmp[i]) {
         map_delete(aliases, 0);
         continue;
      }
      bing = alias_string(aliases[tmp[i]]);
      if (!bing) {
         bing = "Error in the alias!";
      }
      if (!tmp[i]) {
         tmp[i] = "icky";
      }
      if (strsrch(bing, "%^") != -1) {
         bing = replace_string(bing, "%^", "% ^");
         tmp[i] += " (colour replace)";
      }
      str = tmp[i] + ": " + bing;
      if (strlen(str) > 39 || sorted) {
         len = this_player()->query_cols() - strlen(tmp[i]) - 2;
         if (len < 0) {
            len = 10;
         }

         /* If it is too long, print it right now. */
         ret += sprintf("%s: %-=*s\n", tmp[i], len, bing);
      } else if (strlen(str) > 19) {
         str1 += str+"\n";
      } else {
         str2 += str+"\n";
      }
   }

   if (strlen(str1)) {
      ret += sprintf("%-#*s\n", this_player()->query_cols(), str1);
   }

   if (strlen(str2)) {
      ret += sprintf("%-#*s\n", this_player()->query_cols(), str2);
   }

   ret += sprintf("A total of %d aliases.\n", sizeof(tmp));
   this_player()->more_string(ret);
   return 1;
} /* print_aliases() */

/**
 * Creates the compiled alias array.  See the alias.h file for the
 * definitions of thevalues in the alias array.
 * @param str the string to compile
 * @return the compiled alias array
 * @see /include/alias.h
 */
mixed *compile_alias(string str) {
   mixed *ret;
   int i;
   int space;
   string *frog;
   string s1;
   int tmp;
   int gumby;
   int nodollar;
   int ending_dollar;
   mixed *ifargs;
   
   str = replace(str, ({ "\\;", "$escaped$", ";", "$new_line$", " ", " " }));
   str = replace(str, "$escaped$", ";");

   if(str[sizeof(str)-1] == '$')
     ending_dollar = 1;
   
   frog = explode("&" + str + "&", "$");
   if (frog[0] == "&") {
      frog[0] = "";
   } else {
      frog[ 0 ] = frog[ 0 ][ 1 .. ];
   }
   s1 = frog[<1];
   if (s1 == "&") {
      frog = frog[0..<2];
   } else {
      frog[<1] = s1[0..<2];
   }
   ret = ({ frog[0] });
   ifargs = ({ });
   nodollar = 1;

   for (i = 1; i < sizeof(frog); i++) {
      switch (frog[i]) {
      case "new_line" :
         ret += ({ NEW_LINE });
         nodollar = 1;
         break;
      case "*" :
         ret += ({ ALL_ARGS });
         gumby = 1;
         nodollar = 1;
         break;
      case "!" :
         /* hack by CPR for andrew so that $!$ substitues current location */
         if (this_object()->query_creator()) {
            ret += ({ CURR_LOC });
            nodollar = 1;
            /* end of hack by CPR */
         }
         break;
      case "else" :
         if (sizeof(ifargs)) {
            ret[ifargs[sizeof(ifargs)-1]] = sizeof(ret) -
               ifargs[sizeof(ifargs)-1]+1;
            ret += ({ ELSE_THING, 0, "" });
            ifargs[sizeof(ifargs)-1] = sizeof(ret)-2;
            nodollar = 1;
         }
         break;
      case "~" :
      case "endif" :
         if (sizeof(ifargs)) {
            ret += ({ END_IF });
            ret[ifargs[sizeof(ifargs)-1]] = sizeof(ret)-
               ifargs[sizeof(ifargs)-1];
            ifargs = delete(ifargs, sizeof(ifargs)-1, 1);
            nodollar = 1;
            space = 1;
         }
         break;
      default :
         if (frog[i][0..4] == "ifarg") {
            if (sscanf(frog[i], "ifarg%d:%s", tmp, s1) == 2) {
               if (tmp < 0) {
                  tmp = 0;
               }
               if (tmp > ALIAS_MASK) {
                  tmp = ALIAS_MASK;
               }
               ret += ({ IFARG_THING+ tmp, 0, "" });
               frog[i--] = s1;
               nodollar = 1;
               ifargs += ({ sizeof(ret)-2 });
               space = 0;
               gumby = 1;
            } else if (frog[i][5] == ':') {
               ret += ({ ALL_IFARG, 0, "" });
               frog[i] = frog[i][6..];
               nodollar = 1;
               ifargs += ({ sizeof(ret)-2 });
               space = 0;
               gumby = 1;
               i--;
            } else {
               if (sizeof(ret) && stringp(ret[sizeof(ret)-1]) && !space) {
                  ret[sizeof(ret)-1] += "$"+frog[i];
               } else if (nodollar) {
                  ret += ({ frog[i] });
                  nodollar = 0;
               } else {
                  ret += ({ "$"+frog[i] });
               }
            }
         } else if (frog[i][0..2] == "arg") {
            if (sscanf(frog[i], "arg%d:%s", tmp, s1) == 2) {
               if (tmp < 0) {
                  tmp = 0;
               }
               if (tmp > ALIAS_MASK) {
                  tmp = ALIAS_MASK;
               }
               ret += ({ ARG_THING+ tmp, s1, "" }); 
               nodollar = 1;
               gumby = 1;
            } else if (frog[i][3] == ':') {
               ret += ({ ALL_ARG, frog[i][4..100], "" });
               nodollar = 1;
               gumby = 1;
            } else {
               if (sizeof(ret) && stringp(ret[sizeof(ret)-1]) && !space) {
                  ret[sizeof(ret)-1] += "$"+frog[i];
               } else if (nodollar) {
                  ret += ({ frog[i] });
                  nodollar = 0;
               } else {
                  ret += ({ "$"+frog[i] });
               }
               gumby = 1;
               space = 0;
            }
         } else if (strlen(frog[i]) && frog[i][<1] == '*' &&
                    sscanf(frog[i], "%d%s*", tmp, s1) == 2 && s1 == "") {
            if (tmp < 0) {
               tmp = 0;
            }
            if (tmp > ALIAS_MASK) {
               tmp = ALIAS_MASK;
            }
            ret += ({ FROM_ARG + tmp });
            gumby = 1;
            nodollar = 1;
         } else if (strlen(frog[i]) && frog[i][0] == '*' && 
                    sscanf(frog[i][1..], "%d%s", tmp,s1) == 2 && s1 == "") {
           if (tmp < 0) {
             tmp = 0;
           }
           if (tmp > ALIAS_MASK) {
             tmp = ALIAS_MASK;
           }
           ret += ({ TO_ARG + tmp });
           gumby = 1;
           nodollar = 1;
         } else if (sscanf(frog[i], "%d%s", tmp, s1) == 2 && s1 == "" &&
                    (i < sizeof(frog)-1 || ending_dollar)) {
           if (tmp < 0) {
             tmp = 0;
           }
           if (tmp > ALIAS_MASK) {
             tmp = ALIAS_MASK;
           }
           ret += ({ ONE_ARG + tmp });
           gumby = 1;
           nodollar = 1;
         } else {
            if (!nodollar) {
               frog[i] = "$"+frog[i];
            }
            nodollar = 0;
            space = 0;
            if (strlen(frog[i]) && frog[i][<1] == '~') {
               if (sizeof(ifargs)) {
                  if (strlen(frog[i]) == 1) {
                     frog[i] = "";
                  } else {
                     frog[i] = frog[i][0..<2];
                  }
                  /* create an offset */
                  ret += ({ END_IF });
                  ret[ifargs[<1]] = sizeof(ret) - ifargs[<1];
                  ifargs = ifargs[0..<2];
                  nodollar = 1;
                  space = 1;
               }
            }
            if (sizeof(ret) && stringp(ret[<1]) && space != 2) {
               ret[<1] += frog[i];
            } else {
               ret += ({ frog[i] });
            }
            if (space) {
               space = 2;
            }
         }
      }

   }

   while (sizeof(ifargs)) {
      ret += ({ END_IF });
      ret[ifargs[sizeof(ifargs)-1]] = sizeof(ret)-
         ifargs[sizeof(ifargs)-1];
      ifargs = delete(ifargs, sizeof(ifargs)-1, 1);
   }

   if (!gumby) {
      if (sizeof(ret) && !stringp(ret[sizeof(ret)-1]) || space) {
         ret += ({ " ", ALL_ARGS });
      } else if(sizeof(ret)) {
         ret[sizeof(ret)-1] += " ";
         ret += ({ ALL_ARGS });
      }
   }

   return ret;
} /* compile_alias() */

/**
 * This method will print out one or more aliases.
 * @param str the aliases to print
 */
int print_some_aliases(string str, int every) {
   if (this_player()->is_alias(str) && !every) {
      printf("%s: %-=*s\n", str, 
                (int)this_player()->query_cols() - strlen(str) -2, 
                alias_string(this_player()->query_player_alias(str)));
      return 1;
   }

   return print_aliases(str, 0);
} /* print_some_aliases() */

/**
 * The main alias control function.  Sets up new aliases and prints out
 * the values of single aliases or all the aliases.
 * @param str the command name
 * @return 0 if the alias command failed, 1 if it succeeded
 * @see edit_alias()
 * @see print_aliases()
 */
protected int alias(string name, string value) {
#ifdef DISALLOW_COLOUR 
   if (strsrch(name, "%^") >= 0 || strsrch(value, "%^") >= 0) {
      notify_fail("Cannot add an alias with a colour escape "
                  "sequence (% ^).\n");
      return 0;
   }
#endif
   if (strsrch(value, "END_ALIAS") != -1) {
      add_failed_mess("You cannot use 'END_ALIAS' in an alias.\n");
      return 0;
   }
   
   name = implode(explode(name, " "), "");
   if (name == "unalias" || name == "alias" || name == "ealias")  {
      add_failed_mess("You can't alias the '" + name + "' command, because "
                      "otherwise, there would be Problems.\n");
      return 0;
   }
   if (!this_player()->is_alias(name)) {
      this_player()->add_player_alias(name, compile_alias(value));
      write("Added alias '" + name + "'.\n");
   } else {
      this_player()->add_player_alias(name, compile_alias(value));
      write("Changed alias '" + name + "'.\n");
   }
   return 1;
} /* alias() */

/** @ignore yes */
mixed *query_patterns() {
   return ({ 
         "", (: print_aliases("", 0) :),
         "sorted", (: print_aliases("", 1) :),
         "every <word'alias'>", (: print_some_aliases($4[0], 1) :), 
         "<word'alias'>", (: print_some_aliases($4[0], 0) :), 
         "<word'alias'> <string>", (: alias($4[0], $4[1]) :)
         });
} /* query_patterns() */
