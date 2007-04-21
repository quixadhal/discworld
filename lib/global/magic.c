/*  -*- LPC -*-  */
/*
 * $Locker: presto $
 * $Id: magic.c,v 1.23 2001/05/13 02:47:41 presto Exp presto $
 * $Log: magic.c,v $
 * Revision 1.23  2001/05/13 02:47:41  presto
 * Changed write()s to printf()s.  Cleaned up formatting.
 *
 * Revision 1.22  2000/07/31 02:47:42  presto
 * New '=' command.  Added file history for 'O' command.  Fixed up some little
 * bugs.  Removed some code dealing with coloring... the effort wasn't worth the
 * reward
 *
 * Revision 1.21  2000/02/06 21:57:53  presto
 * Added customizable setting for colors and tabs
 *
 * Revision 1.20  1999/12/09 01:53:15  presto
 * Fixed up a lot of little buglets
 *
 * Revision 1.19  1999/06/07 01:38:19  presto
 * Added syntax coloring (based on original code by Gototh).  Bug fixes.
 *
 * Revision 1.18  1999/04/16 05:28:46  pinkfish
 * Fix up a minor error in the magic editor.
 *
 * Revision 1.17  1999/03/28 03:24:10  presto
 * can now specify separation string with 'j' command
 *
 * Revision 1.16  1999/03/20 02:28:26  presto
 * Fixed a itsy-bitsy bug with adding text starting with a backslash
 *
 * Revision 1.15  1999/02/07 19:13:00  presto
 * s'a secret
 *
 * Revision 1.14  1999/01/08 01:28:00  presto
 * Fixed a parse error that was making things like 'I @10 ""' not work
 *
 * Revision 1.13  1998/08/22 01:17:44  presto
 * Changed some code that I couldn't remember the purpose of.  :-b
 *
 * Revision 1.12  1998/07/12 17:36:54  presto
 * Since last time: added 'j', '\', 'r' commands
 *
 * Revision 1.11  1998/03/07 02:10:40  presto
 * Some nifty new modes for C and M commands.  New workaround for rm problem.
 *
 * Revision 1.10  1998/02/07 23:23:27  presto
 * Fixed array out of bounds bug reported by Daxa
 *
 * Revision 1.9  1998/01/31 17:49:21  presto
 * Added undo support for 'F' command
 *
 * Revision 1.8  1998/01/31 17:24:42  presto
 * Workaround for damn problem with rm()
 *
 * Revision 1.7  1998/01/31 16:31:11  presto
 * Fixed bug with my_more function.  I think.
 *
 * Revision 1.6  1998/01/19 00:32:56  presto
 * quick fix so it's usable until line_ed gets sorted
 *
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 *
*/
#include <creator.h>
#include <ed.h>
#define HELPPATH "/w/presto/Misc/edit_help/"
#define UPDATE "/secure/cmds/creator/upd_ate"

#define SPACE      ' '
#define COMMA      ','
#define LPAREN     '('
#define RPAREN     ')'
#define LBRACE     '{'
#define RBRACE     '}'
#define PLUS       '+'
#define MINUS      '-'
#define TIMES      '*'
#define DIVIDE     '/'
#define MOD        '%'
#define AND        '&'
#define OR         '|'
#define XOR        '^'
#define NOT        '!'
#define ONESCMP    '~'
#define EQUAL      '='
#define COLON      ':'
#define QUESTION   '?'
#define GREATER    '>'
#define LESS       '<'
#define LSQUARE    '['
#define RSQUARE    ']'
#define SCOLON     ';'
#define DQUOTES    '"'
#define SQUOTES    '''
#define PERIOD     '.'
#define TAB        '\t'

int *SEPARATORS = ({ SPACE, COMMA, SCOLON, TAB });

int *OPERATORS = ({ LPAREN, RPAREN, PLUS, LSQUARE, RSQUARE, MINUS, TIMES,
                    DIVIDE, MOD, AND, OR, XOR, NOT, ONESCMP, EQUAL, COLON,
                    QUESTION, GREATER, LESS, LBRACE, RBRACE, PERIOD });

string *RANGE_CMDS   = ({ "L", "c", "C", "m", "M", "d", "D", "S", "R", "->",
                          "<-", "j", "=", "rot13" });

string *INDEX_CMDS   = ({ "d", "D", "i", "I", "a", "A", ">", "<", "c", "C",
                          "p", "P", "L", "E", "e", "S", "R", "f", "b", "m",
                          "M", "->", "<-", "F", "j", "=", "O", "ed",
                          "rot13" });

string *M_INDEX_CMDS = ({ "d", "D", "c", "C", "L", "S", "R", "f", "b", "m",
                          "M", "->", "<-", "j", "=", "rot13" });

string *MULTI_CMDS   = ({ "L", "C", "c", "m", "M", "D", "d", "S", "R", ">",
                          "<", "f", "b", "+", "-", "U", "->", "<-", "j",
                          "=", "rot13", "O" });

string *CHAR_CMDS    = ({ "d", "c", "m" });

string *STR_CMDS     = ({ ">", "<", "i", "a", "e", "d", "c", "m", "O", "H",
                          "w", "W", "T", "R", "I", "A", "E", "F", "\\", "r",
                          "set", "ed", "help", "#" });

string *CMDS         = ({ "d", "D", "<", ">", "<<", ">>", "|", "^", "V", "I",
                          "i", "A", "a", "E", "e", "C", "c", "M", "m", "P",
                          "p", "O", "L", "f", "b", "S", "R", "T", "+", "++",
                          "-", "W", "w", "N", "Q", "H", "U", "{}", "()", "[]",
                          "->", "<-", "F", "\\", "j", "r", "h", "set", "=",
                          "ed", "help", "rot13", "#" });

string *P_CMDS       = ({ "d", "D", "<", ">", "<<", ">>", "|", "^", "V", "I",
                          "i", "A", "a", "E", "e", "C", "c", "M", "m", "P",
                          "p", "L", "f", "b", "S", "R", "+", "++", "-", "W",
                          "N", "Q", "H", "U", "->", "<-", "F", "\\", "j",
                          "r", "=", "help", "rot13" });
string *AVAIL_CMDS;

string  keyw_color;
string  comm_color;
string  numb_color;
string  stri_color;
string  oper_color;
string  type_color;
string  tab_str;
string *file;
string *clipboard;
string  name;
string  newname;
string *open_history = ({ });
int     chptr;
int     lnptr;
int     tmpch;
int     tmpln;
int     cols;
int     rows;
int     changed;
int     showln = 1;
int     fsize;
int     mode_flag;
int     highlight = 0;
int     in_comment = 0;
int     in_string = 0;
int     tab_replace;
int     num_len;
int     defs_not_done;
mixed  *undo;
mapping defs;

private string *filter_regexp(string *incoming);
private void    init_settings();
private int     set_colors(string *words);
private void    print_line(int show_ptr);
private int     open_file(string filename);
private int     save_file(string cmd, int open_pending);
private string  color_word(string word,
                           int ref if_flag, int ref include_flag,
                           int ref type_flag, int ref class_flag);
private string *boom(string str, int linenum, int last_line);
private string  highlight(string str, int linenum, int numbering,
                          int last_line);
private int     save_history();
public  void    my_more(string input, string *text, int startln,
                        int number, int hilite, int pages, string xtratxt);
public  void    get_new_text(string new_in, string cmd, int count,
                             int cmdline);
public  void    get_command(string action);
public  int     write_permission(string name);
public  int     rm_file(string name);
private string  expand_macros(string ins, string *sorted_defs);
private string  strip_junk(string stuff);

void create(string filename, int flag)  {
   int status;

   if (undefinedp(filename))  return;
   rows = this_player()->query_rows();
   cols = this_player()->query_cols();
   mode_flag = flag;
   seteuid(geteuid(this_player()));
   if (!filename)  filename = "";
   else filename = implode(explode(filename, " "), ""); /* no leading spaces */
   if (mode_flag)
      init_settings();

   status = open_file(filename);
   if (status == 0)  {
      printf("Cannot open file \"%s\".\n", filename);
      open_file("");
   }
   if (!mode_flag)  {
      AVAIL_CMDS = P_CMDS;
      printf("Type '.' on a blank line to stop inserting text.\nType 'H' "
             "at the ':' prompt for help.\n]");
      tmpln = fsize;
      input_to("get_new_text", 0, "I", 0, 0);
   }
   else  {
      AVAIL_CMDS = CMDS;
      if (status != -1)  {
         printf(":");
         input_to("get_command");
      }
   }
   return;
}  /* create() */


void get_command(string action)  {
   string   *bits;
   string    tmp;
   string    tmp2;
   string   *scrap;
   int       i;
   int       j;
   int       amt;
   int       start;
   int       end;
   int       index;
   int       diff;
   int       rexp_flag;
   mixed    *rexp;
   function  search_func;

   bits = explode(action + " ", " ");
   if ((end  = sizeof(bits) - 1) == -1)  {
      printf(":");
      input_to("get_command");
      return;
   }
   for (i = 1, start = diff = 0; i <= end; i++)  {
      if ((j = strlen(bits[i])) == 0  &&  !start)  {
         --i;
         bits = bits[0 .. i] + bits[i + 2 .. ];
         --end;
      }
      else  {
         if (j  &&  (bits[i][0] == '"'  ||  bits[i][0] == ''')  &&  !start)  {
            start = bits[i][0];
            --j;
            bits[i] = j ? bits[i][1 .. ] : "";
            if (i == 1)  {
               diff = 1;
               if (start == ''')  rexp_flag = 1;
            }
            else if (i == 2  &&  start == ''')  rexp_flag = 2;
         }
         if (start  &&  j  &&  bits[i][j - 1] == start)  {
            start = 0;
            --j;
            bits[i] = j ? bits[i][0 .. j - 1] : "";
         }
         if (j  &&  bits[i][j - 1] == '\\')  {
            bits[i] = j > 1 ? bits[i][0 .. j - 2] : "";
         }
         if (start  &&  i != end )  {
            bits[i] += " " + bits[i + 1];
            bits = bits[0 .. i] + bits[i + 2 .. ];
            --i;
            --end;
         }
      }
   }

   /* See if it's a line number */
   if ((i = to_int(bits[0])) != 0  ||  bits[0][0] == '0')  {
      --i;
      if (i > fsize)  i = fsize;
      else if (i <= 0)  i = 0;

      if (highlight)  {
         if (i != lnptr + 1)
            in_string = 0;
         if (i != lnptr + 1)
            in_comment = 0;
      }
      lnptr = i;
      chptr = 0;
      print_line(0);
      printf(":");
      input_to("get_command");
      return;
   }

   /* If it's not a number, see if it's a known command */
   if (member_array(bits[0], AVAIL_CMDS) == -1)  {
      printf("Unknown command.  Enter 'H' for help.\n:");
      input_to("get_command");
      return;
   }
   tmpln = lnptr;
   tmpch = chptr;
   index = 0;
   if (sizeof(bits) > 1)  {
      if (to_int(bits[1]) == 0  &&  bits[1][0] != '0'  &&
          bits[1][0] != '@'  &&  bits[1][0 .. 1] != ".-")
      {
         diff = 1;
      }
      if (!diff)  {
         tmp = tmp2 = "";
         sscanf(bits[1], "%s-%s", tmp, tmp2);
         if (strlen(tmp)  &&  strlen(tmp2))  {
            if (member_array(bits[0], RANGE_CMDS) == -1)  {
               printf("Range mode not valid with command '%s'.\n:", bits[0]);
               input_to("get_command");
               return;
            }
            if ((start = to_int(tmp)) == 0  &&  tmp == ".")
               start = member_array(bits[0], CHAR_CMDS) == -1 ?
                       lnptr + 1 : chptr + 1;
            if ((end = to_int(tmp2)) == 0)  {
               if (tmp2 == ">")
                  end = member_array(bits[0], CHAR_CMDS) == -1 ?
                        fsize : strlen(file[lnptr]);
               else if (tmp2 == ".")
                  end = member_array(bits[0], CHAR_CMDS) == -1 ?
                        lnptr + 1 : chptr + 1;
            }
            if (start > end  ||  start <= 0)  {
               printf("Invalid range.\n:");
               input_to("get_command");
               return;
            }
            else  {
               tmpln = tmpch = start - 1;
               if (tmpch > strlen(file[lnptr]))
                  tmpch = strlen(file[lnptr]);
               if (tmpln > fsize)  tmpln = fsize;
               amt = end - start + 1;
            }
         }
         else if (sscanf(bits[1], "%*s@%d", index))  {
            if (member_array(bits[0], INDEX_CMDS) == -1)  {
               printf("Index mode not valid with command '%s'.\n:", bits[0]);
               input_to("get_command");
               return;
            }
            else if (index <= 0)  {
               printf("Invalid index.\n:");
               input_to("get_command");
               return;
            }
            if (sscanf(bits[1], "%d@%d", amt, index) == 2)  {
               if (member_array(bits[0], M_INDEX_CMDS) == -1)  {
                  printf("Multi-indexing not valid with command '%s'.\n:",
                         bits[0]);
                  input_to("get_command");
                  return;
               }
            }
            else amt = 1;
            tmpln = tmpch = index - 1;
            if (tmpch > strlen(file[lnptr]))
               tmpch = strlen(file[lnptr]);
            if (tmpln > fsize)  tmpln = fsize;
         }
         else if (member_array(bits[0], MULTI_CMDS) == -1)  {
            printf("Amount mode not valid with command '%s'.\n:", bits[0]);
            input_to("get_command");
            return;
         }
         else amt = to_int(bits[1]);
         if (amt <= 0  &&  bits[0] != "O")  {
            printf("Illegal amount.\n:");
            input_to("get_command");
            return;
         }
      }
      else if (member_array(bits[0], STR_CMDS) == -1)  {
         printf("String arguments are not valid with command '%s'.\n:",
                bits[0]);
         input_to("get_command");
         return;
      }
      else amt = 1;
   }
   else amt = 1;

   if (highlight)  {
      if (tmpln != lnptr  &&  tmpln != lnptr + 1)
         in_comment = 0;
      if (tmpln != lnptr + 1)
         in_string = 0;
   }

   switch (bits[0])  {
      case "d":
         if (lnptr == fsize)
            printf("Cannot delete beyond end of file.\n");
         else if (tmpch == strlen(file[lnptr]))
            printf("Cannot delete beyond end of line.\n");
         else  {
            if (rexp_flag == 1)  {
               tmp = catch(scrap = reg_assoc(file[lnptr],
                                             ({ bits[1] }), ({ "1" }))[0]);
               if (tmp)  {
                  printf("REGEXP error: %s", tmp[1 .. ]);
                  break;
               }
               else  {
                  if (sizeof(scrap) != 3)  {
                     sprintf("No match found for regular expression \"%s"
                             "\".\n", bits[1]);
                     break;
                  }
                  else  {
                     tmpch = strlen(scrap[0]);
                     amt = strlen(scrap[1]);
                  }
               }
            }
            else if (diff)  {
               if ((i = strsrch(file[lnptr][tmpch .. ], bits[1])) > -1)  {
                  tmpch += i;
                  amt = strlen(bits[1]);
               }
               else  {
                  if (strlen(bits[1]) == 0)
                     printf("Cannot search for null string.\n");
                  else
                     printf("'%s' not found.\n", bits[1]);
                  break;
               }
            }
            else if (tmpch + amt > strlen(file[lnptr]))
               amt = strlen(file[lnptr]) - tmpch;
            undo = ({ ({ ({ "E", lnptr, ({ file[lnptr] }) }) }) }) + undo;
            changed = 1;
            if (!tmpch)
               file[lnptr] = file[lnptr][amt .. ];
            else
               file[lnptr] = file[lnptr][0 .. tmpch - 1] +
                             file[lnptr][tmpch + amt .. ];
            if (tmpch + amt < chptr)  chptr -= amt;
            else if (chptr >= tmpch)  chptr = tmpch;
            print_line(1);
         }
         break;

      case ">":
         if (lnptr != fsize)  {
            if (index)  chptr = tmpch;
            else if (rexp_flag == 1)  {
               tmp = catch(scrap = reg_assoc(file[lnptr],
                                             ({ bits[1] }), ({ "1" }))[0]);
               if (tmp)  {
                  printf("REGEXP error: %s", tmp[1 .. ]);
                  break;
               }
               else  {
                  if (sizeof(scrap) != 3)  {
                     printf("No match found for regular expression \"%s"
                            "\".\n", bits[1]);
                     break;
                  }
                  else  {
                     chptr = strlen(scrap[0]);
                  }
               }
            }
            else if (diff)  {
               if ((i = strsrch(file[lnptr][chptr + 1 .. ], bits[1])) != -1)
                  chptr += i + 1;
               else if ((i = strsrch(file[lnptr]
                                         [0 .. chptr + strlen(bits[1]) - 1],
                                     bits[1])) != -1)
                  chptr = i;
               else  {
                  if (strlen(bits[1]) == 0)
                     printf("Cannot search for null string.\n");
                  else printf("'%s' not found.\n", bits[1]);
                  break;
               }
            }
            else chptr = (chptr + amt) % (strlen(file[lnptr]) + 1);
            print_line(1);
         }
         else print_line(0);
         break;

      case "<":
         if (lnptr != fsize)  {
            if (index)  chptr = tmpch;
            else if (rexp_flag == 1)  {
               tmp = catch(scrap = reg_assoc(file[lnptr],
                                             ({ bits[1] }), ({ "1" }))[0]);
               if (tmp)  {
                  printf("REGEXP error: %s", tmp[1 .. ]);
                  break;
               }
               else  {
                  if (sizeof(scrap) != 3)  {
                     printf("No match found for regular expression \"%s"
                            "\".\n", bits[1]);
                     break;
                  }
                  else  {
                     chptr = strlen(scrap[0]);
                  }
               }
            }
            else if (diff)  {
               if ((i = strsrch(file[lnptr]
                                    [0 .. chptr + strlen(bits[1]) - 2],
                                bits[1], -1)) != -1)
                  chptr = i;
               else if ((i = strsrch(file[lnptr][chptr .. ],
                                     bits[1], -1)) != -1)
                  chptr += i;
               else  {
                  if (strlen(bits[1]) == 0)
                     printf("Cannot search for null string.\n");
                  else printf("'%s' not found.\n", bits[1]);
                  break;
               }
            }
            else  {
               i = strlen(file[lnptr]);
               amt = i - (amt % (i + 1)) + 1;
               chptr = (chptr + amt) % (i + 1);
            }
            print_line(1);
         }
         else print_line(0);
         break;

      case ">>":
         if (lnptr != fsize)  {
            chptr = strlen(file[lnptr]);
            print_line(1);
         }
         else print_line(0);
         break;

      case "<<":
         if (lnptr != fsize)  {
            chptr = 0;
            print_line(1);
         }
         else print_line(0);
         break;

      case "|":
         if (lnptr != fsize)  {
            chptr = strlen(file[lnptr]) / 2;
            print_line(1);
         }
         else print_line(0);
         break;

      case "^":
         i = showln;
         showln = 1;
         print_line(lnptr != fsize);
         showln = i;
         break;

      case "\\":
         if (lnptr != fsize)  {
            if (rexp_flag == 1)  {
               tmp = catch(scrap = reg_assoc(file[lnptr],
                                             ({ bits[1] }), ({ "1" }))[0]);
               if (tmp)  {
                  printf("REGEXP error: %s", tmp[1 .. ]);
                  break;
               }
               else  {
                  if (sizeof(scrap) != 3)  {
                     printf("No match found for regular expression \"%s"
                            "\".\n", bits[1]);
                     break;
                  }
                  else  {
                     chptr = strlen(scrap[0]);
                  }
               }
            }
            else if (diff)  {
               if ((i = strsrch(file[lnptr][chptr .. ], bits[1])) != -1)
                  chptr += i;
               else  {
                  if (strlen(bits[1]) == 0)
                     printf("Cannot search for null string.\n");
                  else printf("'%s' not found.\n", bits[1]);
                  break;
               }
            }
            if (chptr != strlen(file[lnptr]))  {
               undo = ({ ({ ({ "E", lnptr,  ({ file[lnptr] }) }),
                            ({ "D", lnptr + 1, 1 }) }) }) + undo;
               if (!lnptr)  {
                  file = ({ file[0][0 .. chptr - 1], file[0][chptr .. ]})  +
                         file[1 .. ];
               }
               else  {
                  file = file[0 .. lnptr - 1] +
                         ({ file[lnptr][0 .. chptr - 1],
                            file[lnptr][chptr .. ] }) + file[lnptr + 1.. ];
               }
               ++fsize;
               num_len = strlen(sprintf("%d", fsize + 1));
               changed = 1;
               printf("Line %d split.\n", lnptr + 1);
            }
            else printf("Cannot split beyond end of line.\n");
         }
         else printf("Cannot split beyond end of file.\n");
         break;

      case "j":
         if (tmpln < fsize)  {
            if (amt != 1)  {
               if (tmpln + amt > fsize)  {
                  printf("Joining up to end of file - ");
                  amt = fsize - tmpln;
               }
               undo = ({ ({ ({ "E", tmpln, ({ file[tmpln] }) }),
                            ({ "I", tmpln + 1,
                               file[tmpln + 1 .. tmpln + amt - 1] }) }) }) +
                      undo;
               if (sizeof(bits) > 2)
                  tmp = bits[2];
               else
                  tmp = " ";
               for (i = 1; i < amt; i++)  {
                  file[tmpln] += tmp +
                                 implode(explode(file[tmpln + i], " "),
                                         " ");
               }
               printf("%d lines joined.\n", amt);
               file = file[0 .. tmpln] + file[tmpln + amt .. ];
               fsize -= amt - 1;
               num_len = strlen(sprintf("%d", fsize + 1));
               if (tmpln < lnptr)  lnptr -= amt - 1;
               changed = 1;
            }
            else printf("Joining just 1 line is pointless. :)\n");
         }
         else printf("Cannot join beyond end of file.\n");
         break;

      case "=":
         if (tmpln < fsize)  {
            if (sizeof(bits) > 2)  {
               j = to_int(bits[2]);
               if (j <= 0)  {
                  printf("Line width must be greater than zero.\n");
                  break;
               }
               if (sizeof(bits) > 3)  {
                  j -= strlen(bits[3]);
                  if (j < 0)   {
                     printf("Line width must be greater than the length "
                            "of your indent string.\n");
                     break;
                  }
               }
            }
            else  {
               j = cols - 5;
               if (j <= 0)  j = cols;
            }
            if (tmpln + amt > fsize)  {
               printf("Formatting up to end of file - ");
               amt = fsize - tmpln;
            }
            tmp2 = " \n";
            scrap = ({ });
            i = 0;
            foreach (tmp in file[tmpln .. tmpln + amt - 1])  {
               if (tmp != "")
                  /* strip off leading spaces and append */
                  tmp2 += implode(explode(tmp, " "), " ") + " ";
               else  {
                  scrap += explode(sprintf("%-=*s", j, tmp2[0 .. <2]) +
                                   "\n\n", "\n")[1 .. ];
                  tmp2 = " \n";
               }
            }
            if (strlen(tmp2))
               scrap += explode(sprintf("%-=*s", j,
                                        tmp2[0 .. <2]), "\n")[1 .. ];

            if (sizeof(bits) > 3)
               scrap = map(scrap, (: $(bits[3]) + $1 :));
            /* check for changes */
            if ((i = sizeof(scrap)) != amt)
               diff = 1;
            else for (j = 0; j < amt; j++)  {
               if (file[tmpln + j] != scrap[j])  {
                  diff = 1;
                  break;
               }
            }

            if (diff)  {
               changed = 1;
               undo = ({ ({ ({ "D", tmpln, i }),
                            ({ "I", tmpln,
                               file[tmpln .. tmpln + amt - 1] }) }) }) +
                      undo;
               if (tmpln)  {
                  file = file[0 .. tmpln - 1] + scrap +
                         file[tmpln + amt .. ];
               }
               else  {
                  file = scrap + file[amt .. ];
               }
               printf("Adjustments made.\n");
               fsize += i - amt;
               num_len = strlen(sprintf("%d", fsize + 1));
               if (lnptr >= tmpln + amt)  lnptr += i - amt;
               else if (lnptr >= tmpln)  {
                  lnptr = tmpln + i * (to_float(lnptr - tmpln)) / amt;
                  chptr = 0;
               }
            }
            else printf("No adjustments needed.\n");
         }
         else printf("Cannot adjust beyond end of file.\n");
         break;

      case "->":
         if (tmpln != fsize)  {
            tmp = sizeof(bits) > 2 ? bits[2] : "   ";
            if (strlen(tmp) == 0)
               printf("Why bother indenting by nothing?\n");
            else  {
               if (tmpln + amt > fsize)  {
                  printf("Indenting up to end of file - ");
                  amt = fsize - tmpln;
               }
               undo = ({ ({ ({ "E", tmpln,
                               file[tmpln .. tmpln + amt - 1] }) }) }) +
                      undo;
               changed = 1;
               tmp = sizeof(bits) > 2 ? bits[2] : "   ";
               file[tmpln .. tmpln + amt - 1] =
                  map(file[tmpln .. tmpln + amt - 1], (: $(tmp) + $1 :));
               printf("%d line%s indented.\n", amt, amt == 1 ? "" : "s");
            }
         }
         else printf("Cannot indent beyond end of file.\n");
         break;

      case "<-":
         if (tmpln != fsize)  {
            tmp = sizeof(bits) > 2 ? bits[2] : "   ";
            if ((j = strlen(tmp)) == 0)
               printf("Why bother unindenting by nothing?\n");
            else  {
               if (tmpln + amt > fsize)  {
                  printf("Unindenting up to end of file - ");
                  amt = fsize - tmpln;
               }
               index = diff = i = 0;
               foreach (tmp2 in file[tmpln .. tmpln + amt - 1])  {
                  if (tmp2[0 .. j - 1] == tmp)  {
                     if (!index)  {
                        changed = index = diff = 1;
                        undo = ({ ({ ({ "E", tmpln, ({ tmp2 }) }) }) }) +
                               undo;
                     }
                     else if (!diff)  {
                        undo[0] += ({ ({ "E", tmpln, ({ tmp2 }) }) });
                        diff = 1;
                     }
                     else undo[0][<1][2] += ({ tmp2 });
                     file[tmpln] = tmp2[j .. ];
                     ++i;
                  }
                  else diff = 0;
                  ++tmpln;
               }
               if (i == 0)
                  printf("Couldn't find any lines beginning with \"" + tmp +
                         "\".\n");
               else
                  printf("%d line%s unindented.\n", i, i == 1 ? "" : "s");
            }
         }
         else printf("Cannot unindent beyond end of file.\n");
         break;

      case "[]":
      case "()":
      case "{}":
         tmp = sprintf("%c", bits[0][0]);
         tmp2 = sprintf("%c", bits[0][1]);
         if ((i = file[lnptr][chptr]) != tmp[0]  &&  i != tmp2[0])
            printf("Pointer must be on a '%s' or a '%s'\n:", tmp, tmp2);
         else  {
            if (i == tmp[0])  {
               do  {
                  do  {
                     i = strsrch(file[lnptr][++chptr .. ], tmp);
                     j = strsrch(file[lnptr][chptr .. ], tmp2);
                     if (i > -1  &&  (i < j  ||  j == -1))  {
                        ++amt;
                        chptr += i;
                     }
                     else if (j > -1)  {
                        --amt;
                        chptr += j;
                     }
                  }
                  while ((i > -1  ||  j > -1)  &&  amt);
                  index = chptr;
                  chptr = -1;
               }
               while (amt  &&  ++lnptr < fsize);
               chptr = index;
            }
            else  {
               do  {
                  while ((i > -1  ||  j > -1)  &&  chptr  &&  amt)  {
                     i = strsrch(file[lnptr][0 .. --chptr], tmp2, -1 );
                     j = strsrch(file[lnptr][0 .. chptr], tmp, -1);
                     if (j > i)  {
                        --amt;
                        chptr = j;
                     }
                     else if (i > -1)  {
                        ++amt;
                        chptr = i;
                     }
                  }
                  if (lnptr)  chptr = strlen(file[lnptr - 1]);
                  i = 0;
               }
               while (amt  &&  lnptr--);
               chptr = j;
            }
            if (amt)  printf("No match found.\n");
            else  {
               i = showln;
               showln = 1;
               print_line(1);
               chptr = tmpch;
               lnptr = tmpln;
               showln = i;
            }
         }
         break;

      case "V":
         if (sizeof(clipboard))  {
            my_more("", clipboard, 0, 0, 0, 0, "");
            return;
         }
         else printf("Clipboard is empty.\n");
         break;

      case "D":
         if (tmpln != fsize)  {
            if (tmpln + amt > fsize)  {
               printf("Deleting up to end of file - ");
               amt = fsize - tmpln;
            }
            undo = ({ ({ ({ "I", tmpln,
                            file[tmpln .. tmpln + amt - 1] }) }) }) + undo;
            changed = 1;
            if (!tmpln)  file = file[amt .. ];
            else file = file[0 .. tmpln - 1] + file[tmpln + amt .. ];
            fsize -= amt;
            num_len = strlen(sprintf("%d", fsize + 1));
            if (tmpln + amt < lnptr)  lnptr -= amt;
            else if (lnptr >= tmpln)  {
               lnptr = tmpln;
               chptr = 0;
            }
            printf("%d line%s deleted.\n", amt, amt == 1 ? "" : "s");
         }
         else printf("Cannot delete beyond end of file.\n");
         break;

      /* Old MacDonald had a farm ... */
      case "E":
      case "I":
      case "e":
      case "i":
      case "A":
      case "a":
         if (diff  ||  sizeof(bits) > 2)  {
            get_new_text(bits[2 - diff], bits[0], 1, 1);
            return;
         }
         printf("]");
         input_to("get_new_text", 0, bits[0], 0, 0);
         return;

      case "C":
      case "M":
         if (tmpln != fsize)  {
            index = -1;
            if (sizeof(bits) > 2  &&
                ((index = to_int(bits[2]) - 1) < 0  ||
                 (index >= tmpln  &&  index < tmpln + amt)))  {
               printf("Invalid destination line.\n");
               break;
            }
            if (index > fsize)  index = fsize;
            if (tmpln + amt > fsize)  {
               printf("Copying up to end of file - ");
               amt = fsize - tmpln;
            }
            clipboard = file[tmpln .. tmpln + amt - 1];
            if (bits[0] == "M")  {
               changed = 1;
               undo = ({ ({ ({ "I", tmpln,
                               file[tmpln .. tmpln + amt - 1] }) }) }) +
                      undo;
               if (!tmpln)
                  file = file[amt .. ];
               else
                  file = file[0 .. tmpln - 1] + file[tmpln + amt .. ];
               if (tmpln + amt < lnptr)
                  lnptr -= amt;
               else if (lnptr >= tmpln)  {
                  lnptr = tmpln;
                  chptr = 0;
               }
               if (tmpln < index)  index -= amt;
               if (index >= 0)  {
                  if (!index)
                     file = clipboard + file;
                  else
                     file = file[0 .. index - 1] + clipboard +
                            file[index .. ];
                  if (index <= lnptr)  lnptr += amt;
                  undo[0] = ({ ({ "D", index, amt }) }) + undo[0];
                  printf("%d line%s moved.\n", amt, amt == 1 ? "" : "s");
               }
               else  {
                  fsize -= amt;
                  num_len = strlen(sprintf("%d", fsize + 1));
                  printf("%d line%s copied and deleted.\n",
                         amt, amt == 1 ? "" : "s");
               }
            }
            else  {
               if (index >= 0)  {
                  changed = 1;
                  if (!index)
                     file = clipboard + file;
                  else
                     file = file[0 .. index - 1]+ clipboard +
                            file[index .. ];
                  fsize += amt;
                  num_len = strlen(sprintf("%d", fsize + 1));
                  if (index <= lnptr)  lnptr += amt;
                  undo = ({ ({ ({ "D", index, amt }) }) }) + undo;
                  printf("%d line%s copied and pasted.\n",
                         amt, amt == 1 ? "" : "s");
               }
               else printf("%d line%s copied.\n", amt, amt == 1 ? "" : "s");
            }
         }
         else printf("Cannot copy beyond end of file.\n");
         break;

      case "c":
      case "m":
         if (rexp_flag == 1  &&  bits[0] == "c")  {
            printf("Clipboard may only be initialized with literal "
                   "strings.\n");
            break;
         }
         else if (diff  &&  bits[0] == "c")  {
            if (amt = strlen(bits[1]))
               clipboard = ({ bits[1] });
            else  {
               printf("No point in copying a null string.\n");
               break;
            }
         }
         else  {
            if (lnptr == fsize)  {
               printf("Cannot copy beyond end of file.\n");
               break;
            }
            else if (tmpch == (j = strlen(file[lnptr])))  {
               printf("Cannot copy beyond end of line.\n");
               break;
            }
            if (rexp_flag == 1)  {
               tmp = catch(scrap = reg_assoc(file[lnptr],
                                             ({ bits[1] }), ({ "1" }))[0]);
               if (tmp)  {
                  printf("REGEXP error: %s", tmp[1 .. ]);
                  break;
               }
               else  {
                  if (sizeof(scrap) != 3)  {
                     printf("No match found for regular expression \"%s"
                            "\".\n", bits[1]);
                     break;
                  }
                  else  {
                     tmpch = strlen(scrap[0]);
                     amt = strlen(scrap[1]);
                  }
               }
            }
            else if (diff)  {
               if ((i = strsrch(file[lnptr][tmpch .. ], bits[1])) > -1)  {
                  tmpch += i;
                  amt = strlen(bits[1]);
               }
               else  {
                  if (strlen(bits[1]) == 0)
                     printf("Cannot search for null string.\n");
                  else
                     printf("'%s' not found.\n", bits[1]);
                  break;
               }
            }
            else if ((tmpch + amt - 1) >= j)  {
               printf("Copying up to end of line -  ");
               amt = j - tmpch;
            }
            clipboard = ({ file[lnptr][tmpch .. tmpch + amt - 1] });
         }
         printf("%d character%s copied", amt, amt == 1 ? "" : "s");
         if (bits[0] == "m")  {
            printf(" and deleted.\n");
            undo = ({ ({ ({ "E", lnptr, ({ file[lnptr] }) }) }) }) + undo;
            if (!tmpch)
               file[lnptr] = file[lnptr][amt .. ];
            else
               file[lnptr] = file[lnptr][0 .. tmpch - 1] +
                             file[lnptr][tmpch + amt .. ];
            if (tmpch + amt < chptr)
               chptr -= amt;
            else if (chptr >= tmpch)
               chptr = tmpch;
            changed = 1;
            print_line(1);
         }
         else printf(".\n");
         break;

      case "p":
         if (sizeof(clipboard) == 1)  {
            if (lnptr == fsize)  {
               file = file[0 .. fsize - 1] +
                      ({ clipboard[0] , "End of file." });
               ++fsize;
               num_len = strlen(sprintf("%d", fsize + 1));
               undo = ({ ({ ({ "D", lnptr, 1 }) }) }) + undo;
            }
            else  {
               undo = ({ ({ ({ "E", lnptr, ({ file[lnptr] }) }) }) }) +
                      undo;
               if (!tmpch)
                  file[lnptr] = clipboard[0] + file[lnptr];
               else
                  file[lnptr] = file[lnptr][0 .. tmpch - 1] +
                                clipboard[0] +
                                file[lnptr][tmpch .. ];
            }
            if (tmpch <= chptr)  chptr += strlen(clipboard[0]);
            changed = 1;
            print_line(1);
            break;
         }
         /* No break here */

      case "P":
         if (i = sizeof(clipboard))  {
            undo = ({ ({ ({ "D", tmpln, i }) }) }) + undo;
            changed = 1;
            if (!tmpln)
               file = clipboard + file;
            else
               file = file[0 .. tmpln - 1] + clipboard +
                      file[tmpln ..];
            printf("%d line%s pasted.\n", i, i == 1 ? "" : "s");
            fsize += i;
            num_len = strlen(sprintf("%d", fsize + 1));
            if (tmpln <= lnptr)  lnptr += i;
         }
         else printf("Clipboard is empty.\n");
         break;

      case "O":
      case "ed":
         if (index)  {
            end = -1;
            i = strsrch(file[tmpln], ' ');
            if (i > 0)  {
               if (file[tmpln][0 .. i - 1] == "#define")  {
                  j = strsrch(file[tmpln][i + 1 .. ], ' ');
                  if (j > -1)  {
                     tmp2 = file[tmpln][i + j + 2 .. ];
                     end = i + j + 1;
                  }
                  else
                     tmp2 = file[tmpln];
               }
               else {
                  tmp2 = file[tmpln][i + 1 .. ];
                  end = i;
                  j = strsrch(tmp2, ' ');
                  if (j > 0  &&  tmp2[0 .. j - 1] == "inherit")  {
                     tmp2 = tmp2[j + 1 .. ];
                     end += j + 1;
                  }
               }
            }
            else tmp2 = file[tmpln];
            tmp2 = strip_junk(expand_macros(tmp2, sort_array(keys(defs), -1)));
            if (end > -1)  tmp2 = file[tmpln][0 .. end] + tmp2;
            if (sscanf(tmp2, "#include <%s>", tmp) == 1)
               tmp = "/include/" + tmp;
            else if (sscanf(tmp2, "#include \"%s\"", tmp) == 1  ||
                     sscanf(tmp2, "inherit \"%s\"", tmp) == 1  ||
                     sscanf(tmp2, "nosave inherit \"%s\"", tmp) == 1)
            {
               if (tmp[0] != '/')  {
                  i = strsrch(name, '/', -1);
                  tmp = name[0 .. i] + tmp;
               }
               if (strlen(tmp) > 2  &&  tmp[<2] != '.')  tmp += ".c";
            }
            else  {
               printf("Line %d does not contain a recognizable filename.\n",
                      tmpln + 1);
               break;
            }
         }
         else if (!diff  &&  sizeof(bits) > 1)  {
            if (amt < 0)  amt = sizeof(open_history) + amt;
            if (amt > sizeof(open_history)  ||  amt < 1)  {
               printf("Invalid choice.\n");
               break;
            }
            else tmp = open_history[amt - 1];
         }
         else tmp = sizeof(bits) > 1 ? bits[1] : "";

         if (changed)  {
            printf("Current file has been changed.  Save now? (Y/N/A):]");
            input_to("get_new_text", 0, "O", 0, 0);
            newname = tmp;
            return;
         }
         if (open_file(tmp) == -1)
            return;
         break;

      case "F":
         if (!this_player()->query_creator())
            printf("This command is only available to creators.\n");
         else if ((i = sizeof(bits)) == 1  ||  (i == 2  &&  !diff))
            printf("Must specify a filename.\n");
         else  {
            tmp = this_player()->get_path(bits[2 - diff]);
            if ((amt = file_size(tmp)) == -2)
               printf("'%s' is a directory.\n", tmp);
            else if (amt == -1)
               printf("'%s' does not exist.\n", tmp);
            else  {
               if (!tmpln)
                  file = explode(read_bytes(tmp), "\n") + file;
               else file = file[0 .. tmpln - 1] +
                           explode(read_bytes(tmp), "\n") +
                           file[tmpln .. ];
               amt = fsize;
               fsize = sizeof(file) - 1;
               num_len = strlen(sprintf("%d", fsize + 1));
               printf("File '%s' inserted. (%d line%s)\n",
                      tmp, fsize - amt, (fsize - amt) == 1 ? "" : "s");
               changed = 1;
               undo = ({ ({ ({ "D", tmpln, fsize - amt }) }) }) + undo;
            }
         }
         break;

      case "L":
         my_more("", file[tmpln .. tmpln + amt - 1 + (tmp2 == ">")],
                 tmpln, showln, highlight, 0, "");
         return;

      case "f":
         lnptr = tmpln + amt * rows - amt;
         if (lnptr > fsize)  lnptr = fsize;
         my_more("", file[tmpln .. lnptr], tmpln, showln, highlight,
                 amt, "");
         return;

      case "b":
         lnptr = tmpln - (amt * rows - amt - 1);
         if (lnptr < 0)  {
            diff = lnptr;
            lnptr = 0;
         }
         else diff = 0;
         scrap = file[lnptr .. tmpln];
         i = j = 0;
         if (showln)  {
            foreach (tmp in scrap)  {
               ++i;
               diff += (strlen(sprintf("%'.'*d] %s", num_len, i + lnptr,
                                       tmp)) - 1) / cols;
            }
            while (diff > 0)  {
               diff -= (strlen(sprintf("%'.'*d] %s", num_len, lnptr + j + 1,
                                       scrap[j])) - 1) / cols + 1;
               ++j;
            }
         }
         else  {
            foreach (tmp in scrap)  diff += (strlen(tmp) - 1) / cols;
            while (diff > 0)
               diff -= (strlen(scrap[j++]) - 1) / cols + 1;
         }
         lnptr += j;
         if (lnptr + 1 < in_comment)  in_comment = 0;
         if (lnptr >= tmpln)  {
            my_more("", ({ file[tmpln] }), tmpln, showln, highlight, 1, "");
            lnptr = tmpln;
         }
         else my_more("", scrap[j .. ], lnptr, showln, highlight, 0, "");
         return;

      case "S":
         if (sizeof(bits) < 3)
            printf("Must specify search string.\n");
         else if (strlen(bits[2]) == 0)
            printf("Cannot have a null search string.\n");
         else if (tmpln != fsize)  {
            if (tmpln + amt > fsize)
               printf("Searching up to end of file.\n");
            i = 0;
            scrap = ({ });
            if (rexp_flag)  {
               if (tmp = catch(rexp = regexp(file[tmpln .. tmpln + amt - 1],
                                             bits[2], 1)))
               {
                  printf("REGEXP error: %s", tmp[1 .. ]);
                  break;
               }
               else  {
                  for (i = 0; i < sizeof(rexp); i += 2)  {
                     scrap += ({ (rexp[i + 1] + tmpln) + "] " + rexp[i] });
                  }
               }
            }
            else {
               foreach (tmp in file[tmpln .. tmpln + amt - 1])  {
                  ++i;
                  if (strsrch(tmp, bits[2]) > -1)
                     scrap += ({ (i + tmpln) + "] " + tmp });
               }
            }
            if (sizeof(scrap) == 0)  {
               if (rexp_flag)  {
                  printf("No matches for regular expression \"%s"
                         "\".\n", bits[2]);
               }
               else printf("\"%s\" not found.\n", bits[2]);
            }
            else  {
               my_more("", scrap, 0, 0, 0, 0, "");
               return;
            }
         }
         else printf("Cannot search beyond end of file.\n");
         break;

      case "R":
         if ((i = sizeof(bits)) < 3  ||  (i == 3  &&  !diff))
            printf("Must specify both search and replace strings.\n");
         else if (strlen(bits[2 - diff]) == 0)
            printf("Cannot have a null search string.\n");
         else if (tmpln != fsize)  {
            if (tmpln + amt > fsize)
               printf("Replacing up to end of file.\n");
            if (diff  ||  rexp_flag == 1)  bits = ({ "" }) + bits;
            j = index = 0;
            if (rexp_flag)  {
               tmp = catch(regexp(file[tmpln], bits[2]));
               if (tmp)  {
                  printf("REGEXP error: %s", tmp[1 .. ]);
                  break;
               }
               // You are not expected to understand this.
               search_func =
               (: sizeof($4 = filter_regexp(reg_assoc($1, ({ $2 }),
                                                      ({ "1" }))[0])) > 1 ?
                  ({ sizeof($4) - 1, implode($4, $3) }) : ({ 0 }) :);
            }
            else  {
               replace(bits[2], "\\", "\\\\");
               replace(bits[3], "\\", "\\\\");
               search_func = (: sizeof($4 = explode($5 + $1 + $5, $2)) - 1 ?
                                ({ sizeof($4) - 1,
                                   implode($4, $3)[1 .. <2] }) : ({ 0 }) :);
               /* Get a unique character to tack on */
               tmp = sprintf("%c", ((bits[2][0] + 1) % 127) + 1);
            }

            foreach (tmp2 in file[tmpln .. tmpln + amt - 1])  {
               rexp = evaluate(search_func, tmp2, bits[2], bits[3], scrap,
                               tmp);
               if (rexp[0])  {
                  file[tmpln] = rexp[1];
                  if (tmpln == lnptr)  chptr = 0;
                  if (!index)  {
                     undo = ({ ({ ({ "E", tmpln, ({ tmp2 }) }) }) }) + undo;
                     index = j = 1;
                  }
                  else if (!j)  {
                     undo[0] += ({ ({ "E", tmpln, ({ tmp2 }) }) });
                     j = 1;
                  }
                  else undo[0][<1][2] += ({ tmp2 });
                  printf("%d instance%s replaced on line %d.\n",
                         rexp[0], rexp[0] == 1 ? "" : "s", tmpln + 1);
               }
               else j = 0;
               ++tmpln;
            }
            if (!index)  {
               if (rexp_flag)
                  printf("No match for regular expression \"%s"
                         "\".\n", bits[2]);
               else
                  printf("\"%s\" not found.\n", bits[2]);
            }
            else changed = 1;
         }
         else printf("Cannot replace beyond end of file.\n");
         break;

      case "r":
         if (sizeof(bits) < 3)
            printf("Must specify both search and replace strings.\n");
         else if (strlen(bits[1]) == 0)
            printf("Cannot have a null search string.\n");
         else if (lnptr != fsize)  {
            amt = 0;
            if (rexp_flag == 1)  {
               tmp = catch(scrap = reg_assoc(file[lnptr],
                                             ({ bits[1] }), ({ "1" }))[0]);
               if (tmp)  {
                  printf("REGEXP error: %s", tmp[1 .. ]);
                  break;
               }
               else if (sizeof(scrap) == 3)  {
                  tmpch = strlen(scrap[0]);
                  amt = strlen(scrap[1]);
               }
            }
            else if ((i = strsrch(file[lnptr][tmpch .. ], bits[1])) > -1)  {
               tmpch += i;
               amt = strlen( bits[1] );
            }
            if (amt)  {
               undo = ({ ({ ({ "E", lnptr, ({ file[lnptr] }) }) }) }) +
                      undo;
               if (!tmpch)
                  file[lnptr] = bits[2] + file[lnptr][amt .. ];
               else
                  file[lnptr] = file[lnptr][0 .. tmpch - 1] + bits[2] +
                                file[lnptr][tmpch + amt .. ];
               if (tmpch + amt < chptr)  chptr -= amt;
               else if (chptr >= tmpch)  chptr = tmpch;
               changed = 1;
               print_line(1);
            }
            else  {
               if (rexp_flag)
                  printf("No match found for regular expression \"%s"
                         "\".\n", bits[1]);
               else
                  printf("'%s' not found.\n", bits[1]);
            }
         }
         else  {
            printf("Cannot replace beyond end of file.\n");
         }
         break;

      case "T":
         if (sizeof(bits) == 1  ||  strlen(bits[1]) == 0)  {
            if (fsize == 0)  {
               printf("Congratulations!  Your empty file has no errors! ;)\n");
               break;
            }
            if (strlen(name) > 0)  {
               if (changed)  {
                  tmp = fsize ? implode(file[0 .. fsize - 1], "\n") + "\n"
                              : "";
                  i = strsrch(name, "/", -1);
                  if (i == -1)  tmp2 = "tmp_" + name;
                  else tmp2 = name[0 .. i] + "tmp_" + name[i + 1 .. ];
                  if (!write_file(tmp2, tmp))  {
                     printf("Couldn't write temporary file for test.\n");
                     break;
                  }
               }
               else tmp2 = name;
            }
            else tmp2 = "ArEaLlYdUmBnAmE.c";
         }
         else tmp2 = bits[1];
         catch(UPDATE->cmd(tmp2));
         if (changed  &&  !rm_file(tmp2))
            printf("Couldn't remove temporary file %s\n", tmp2);
         break;

      case "+":
         lnptr = (lnptr + amt) % (fsize + 1);
         if (highlight)  {
            if (lnptr != tmpln + 1)
               in_comment = 0;
         }
         chptr = 0;
         print_line(0);
         break;

      case "++":
         in_comment = 0;
         lnptr = fsize;
         chptr = 0;
         print_line(0);
         break;

      case "-":
         lnptr -= amt;
         while (lnptr < 0)  lnptr += fsize + 1;
         in_comment = 0;
         chptr = 0;
         print_line(0);
         break;

      case "h":
         if (highlight = !highlight)
            printf("Syntax highlighting turned on.\n");
         else printf("Syntax highlighting turned off.\n");

         break;

      case "w":
      case "W":
         if (mode_flag  &&  sizeof(bits) > 1  &&  bits[1] != ""  &&
             name != (newname = this_player()->get_path(bits[1])))
         {
            this_player()->set_in_editor(name = newname);
            changed = 1;
         }
         if (mode_flag  &&  bits[0] == "W")  {
            save_history();
         }
         if (save_file(bits[0], 0)  &&  bits[0] == "W")  return;
         break;

      case "N":
         showln = !showln;
         if (!showln)  printf("Line numbering turned off.\n");
         else printf("Line numbering turned on.\n");
         break;

      case "Q":
         printf("Quitting.\n");
         if (mode_flag)  save_history();
         this_player()->set_in_editor(0);
         this_player()->editor_do_quit(0);
         while (remove_call_out("defines") != -1)
            ;
         return;

      case "H":
      case "help":
         if (sizeof(bits) == 1)  tmp = mode_flag ? "main" : "p_main";
         else tmp = bits[1];
         if (member_array(tmp, AVAIL_CMDS +
                          ({ "main", "p_main", "strings", "modes" })) == -1)
            printf("Unknown command '%s'.\n", tmp);
         else if (tmp2 = read_bytes(HELPPATH + tmp + ".hlp"))  {
            my_more("", explode(tmp2, "\n"), 0, 0, 0, 0, "");
            return;
         }
         else printf("Sorry, no help available on \"%s\".  Go "
                     "holler at Presto.\n", tmp);
         break;

      case "U":
         if (diff = sizeof(undo) - 1)  {
            if (diff < amt)  {
               printf("Not that many commands to undo.  ");
               amt = diff;
            }
            for (i = 0; i < amt; i++)  {
               for (j = 0; j < sizeof(undo[i]); j++)  {
                  if (undo[i][j][0] == "I")  {
                     if (undo[i][j][1])
                        file = file[0 .. undo[i][j][1] - 1] +
                               undo[i][j][2] +
                               file[undo[i][j][1] .. ];
                     else file = undo[i][j][2] + file;
                     fsize += sizeof(undo[i][j][2]);
                     num_len = strlen(sprintf("%d", fsize + 1));
                     if (undo[i][j][1] <= lnptr)
                        lnptr += sizeof(undo[i][j][2]);
                  }
                  else if (undo[i][j][0] == "E")  {
                     if (undo[i][j][1])
                        file = file[0 .. undo[i][j][1] - 1] +
                               undo[i][j][2] +
                               file[undo[i][j][1] +
                                    sizeof(undo[i][j][2]) .. ];
                     else file = undo[i][j][2] +
                                 file[sizeof(undo[i][j][2]) .. ];
                     if (undo[i][j][1] <= lnptr  &&
                          lnptr < undo[i][j][1] + sizeof(undo[i][j][2]))
                        chptr = 0;
                  }
                  else  {
                     if (!undo[i][j][1])
                        file = file[undo[i][j][2] .. ];
                     else file = file[0 .. undo[i][j][1] - 1] +
                                 file[undo[i][j][1] + undo[i][j][2] .. ];
                     fsize -= undo[i][j][2];
                     num_len = strlen(sprintf("%d", fsize + 1));
                     if (undo[i][j][1] + undo[i][j][2] < lnptr)
                        lnptr -= undo[i][j][2];
                     else if (lnptr >= undo[i][j][1])  {
                        lnptr = undo[i][j][1];
                        chptr = 0;
                     }
                  }
               }
            }
            undo = undo[amt .. ];
            if (!changed)  changed = 1;
            else if (diff - amt == undo[<1])  changed = 0;
            printf("%d command%s undone.\n", amt, amt == 1 ? "" : "s");
         }
         else printf("Nothing to undo.\n");
         break;

      case "set":
         if (sizeof(bits) == 1  ||  bits[1] == "save")  {
            tmp = sprintf("tab spacing %d\ntab replacement %s\n"
                          "color keyword %s\ncolor comment %s\n"
                          "color number %s\ncolor operator %s\n"
                          "color string %s\ncolor type %s\n",
                          strlen(tab_str), tab_replace ? "on" : "off",
                          replace(keyw_color, "%^", " ")[0 .. <2],
                          replace(comm_color, "%^", " ")[0 .. <2],
                          replace(numb_color, "%^", " ")[0 .. <2],
                          replace(oper_color, "%^", " ")[0 .. <2],
                          replace(stri_color, "%^", " ")[0 .. <2],
                          replace(type_color, "%^", " ")[0 .. <2]);
            if (sizeof(bits) == 1)
               printf("Current settings:\n%s", tmp);
            else  {
               tmp += sprintf("history %s", implode(open_history[<10 .. ], " "));
               if (write_file("/w/" + this_player()->query_name() +
                              "/.magicrc", tmp, 1))
                  printf("Settings saved.\n");
               else printf("Settings could not be saved.\n");
            }
         }
         else if (sizeof(bits) < 4)
            printf("You must specify the setting to change and its value.  "
                   "See 'H set' for more help.\n");
         else  {
            if (bits[1] == "tab")  {
               if (bits[2] == "replacement")  {
                  if (bits[3] == "on")  tab_replace = 1;
                  else tab_replace = 0;
                  printf("Tab replacement turned %s.\n",
                         tab_replace ? "ON" : "OFF");
               }
               else if (bits[2] == "spacing")  {
                  i = to_int(bits[3]);
                  if (i > 0)  {
                     tab_str = "";
                     for (j = 0; j < i; j++)  {
                        tab_str += " ";
                     }
                     printf("Tab spacing set to %d.\n", i);
                  }
                  else printf("Tab spacing must be greater than 0.\n");
               }
               else printf("Invalid option for \"tab\".\n");
            }
            else if (bits[1] == "color")  {
               if (set_colors(bits[2 .. ]))  {
                  printf("%s color set to %s.\n",
                         capitalize(bits[2]), implode(bits[3 .. ], " "));
               }
               else printf("Invalid option '%s'.\n", bits[2]);
            }
            else printf("Invalid option '%s'.\n", bits[1]);
         }
         break;

      case "rot13":
         if (tmpln != fsize)  {
            for (i = 0; i < amt; i++)  {
               tmp = file[tmpln + i];
               for (j = 0; j < strlen(tmp); j++)  {
                  if (tmp[j] >= 'A'  &&  tmp[j] <= 'Z')
                     tmp[j] = ((13 + tmp[j] - 'A') % 26) + 'A';
                  else if (tmp[j] >= 'a' && tmp[j] <= 'z')
                     tmp[j] = ((13 + tmp[j] - 'a') % 26) + 'a';
                  else if (tmp[j] >= '0' && tmp[j] <= '9')
                     tmp[j] = ((5 + tmp[j] - '0') % 10) + '0';
               }
               file[tmpln + i] = tmp;
            }
            printf("%d line%s rot13ified.\n", amt, amt == 1 ? "" : "s");
         }
         else printf("Cannot rot13 beyond end of file.\n");
         break;

      case "#":
         if (defs_not_done)
            printf("Warning: Macros have not yet been fully processed.\n");
         if (sizeof(bits) > 1)  {
if (bits[1] == "ALL")  printf("%O\n", defs);
            if (undefinedp(defs[bits[1]]))
               printf("No definition for \"%s\".\n", bits[1]);
            else
               printf("%s\n", defs[bits[1]]);
         }
         else printf("Which macro do you want to look up?\n");
         break;

      default:
         printf("Something odd has happened.  Tell Presto.\n");
         break;
   }
   printf(":");
   input_to( "get_command" );
   return;
}  /* get_command() */


void get_new_text(string new_in, string cmd, int count, int cmdline)  {
   string new_in2;
   int len;

   if ((len = strlen(new_in) - 1) > -1  &&  new_in[0] == '\\'  &&  !cmdline)  {
      new_in2 = new_in[1 .. ];
      --len;
   }
   else new_in2 = new_in;

   if (tab_replace)  new_in2 = replace(new_in2, "\t", tab_str);

   switch (cmd)  {
      case "i":
         if (len > -1)  {
            changed = 1;
            if (lnptr < fsize)  {
               undo = ({ ({ ({ "E", lnptr, ({ file[lnptr] }) }) }) }) +
                      undo;
               if (!tmpch)  file[lnptr] = new_in2 + file[lnptr];
               else file[lnptr] = file[lnptr][0 .. tmpch - 1] + new_in2 +
                                  file[lnptr][tmpch .. ];
            }
            else  {
               undo = ({ ({ ({ "D", fsize, 1 }) }) }) + undo;
               if (!fsize)  file = ({ new_in2, "End of file." });
               else file = file[0 .. fsize - 1] +
                           ({ new_in2, "End of file." });
               ++fsize;
            }
            if (tmpch <= chptr)  chptr += len + 1;
            print_line(1);
         }
         break;

      case "I":
         if (new_in != "."  ||  cmdline)  {
            if (new_in == "W"  &&  !mode_flag)  {
               if (this_player()->query_name() == "dogbolter")
                  printf("No more secret message!\n");
               get_new_text("", "W", count, 0);
               return;
            }
            if (new_in == ".;W"  &&  this_player()->query_name() == "warrax")  {
               write("You did that on purpose, didn't you?\n");
               get_new_text("", "W", count, 0);
               return;
            }
            changed = 1;
            if (!tmpln)  file = ({ new_in2 }) + file;
            else file = file[0 .. tmpln - 1] + ({ new_in2 }) +
                        file[tmpln .. ];
            ++tmpln;
            if (!cmdline)  {
               printf("]");
               input_to("get_new_text", 0, "I", count + 1, 0);
               return;
            }
         }
         if (count)  {
            if (tmpln - count <= lnptr)  lnptr += count;
            printf("%d line%s added.\n", count, count == 1 ? "" : "s");
            fsize += count;
            undo = ({ ({ ({ "D", tmpln - count, count }) }) }) + undo;
         }
         break;

      case "a":
         if (len > -1)  {
            changed = 1;
            if (lnptr < fsize)  {
               undo = ({ ({ ({ "E", lnptr, ({ file[lnptr] }) }) }) }) +
                      undo;
               file[lnptr] = file[lnptr][0 .. tmpch] + new_in2 +
                             file[lnptr][tmpch + 1 .. ];
            }
            else  {
               undo = ({ ({ ({ "D", fsize, 1 }) }) }) + undo;
               if (!fsize)  file = ({ new_in2, "End of file." });
               else file = file[0 .. fsize - 1] +
                           ({ new_in2, "End of file." });
               ++fsize;
            }
            print_line(1);
         }
         break;

      case "A":
         if (new_in != "."  ||  cmdline)  {
            changed = 1;
            if (tmpln == fsize)  --tmpln;
            file = file[0 .. tmpln] + ({ new_in2 }) +
                   file[tmpln + 1 .. ];
            ++tmpln;
            ++fsize;
            if (!cmdline)  {
               printf("]");
               input_to("get_new_text", 0, "A", count + 1, 0);
               return;
            }
         }
         if (count)  {
            printf("%d line%s added.\n", count, count == 1 ? "" : "s");
            undo = ({ ({ ({ "D", tmpln - count + 1, count }) }) }) + undo;
         }
         break;

      case "E":
         if (new_in != "."  ||  cmdline)  {
            changed = 1;
            if (tmpln < fsize)  {
               if (!count  ||  cmdline)
                  undo = ({ ({ ({ "E", tmpln, ({ file[tmpln] }) }) }) }) +
                         undo;
               else undo[0][0][2] += ({ file[tmpln] });
            }
            else  {
               if (!count  ||  cmdline)
                  undo = ({ ({ ({ "D", tmpln, 1 }) }) }) + undo;
               else if (undo[0][0][0] == "E")
                  undo[0] = ({ ({ "D", tmpln, 1 }) }) + undo[0];
               else ++undo[0][0][2];
               file += ({ "End of file." });
               ++fsize;
            }
            file[tmpln] = new_in2;
            if (tmpln == lnptr)  chptr = 0;
            ++tmpln;
            if (!cmdline)  {
               printf("]");
               input_to("get_new_text", 0, "E", count + 1, 0);
               return;
            }
         }
         if (count)
            printf("%d line%s changed.\n", count, count == 1 ? "" : "s");

         break;

      case "e":
         if (len > -1)  {
            changed = 1;
            if (lnptr < fsize)  {
               undo = ({ ({ ({ "E", lnptr, ({ file[lnptr] }) }) }) }) +
                      undo;
               if (!tmpch)
                  file[lnptr] = new_in2 + file[lnptr][len + 1 .. ];
               else file[lnptr] = file[lnptr][0 .. tmpch - 1] +
                                  new_in2 +
                                  file[lnptr][tmpch + len + 1 .. ];
            }
            else  {
               if (!fsize)  file = ({ new_in2, "End of file." });
               else file = file[0 .. fsize - 1] +
                           ({ new_in2, "End of file." });
               undo = ({ ({ ({ "D", fsize, 1 }) }) }) + undo;
               ++fsize;
            }
            print_line(1);
         }
         break;

      case "w":
      case "W":
         if (len > -1)  {
            new_in2 = replace(new_in2, ({ " ", "", "\t", "" }));
            name = this_player()->get_path(new_in2);
            this_player()->set_in_editor(name);
            if (save_file(cmd, 0))  {
               if (count)  {
                  if (!open_file(newname)  &&  newname == "...")
                     return;
                  if (member_array(name, open_history) == -1)
                     open_history += ({ name });
               }
               else if (cmd == "W")  return;
            }
         }
         else printf("Save canceled.\n");
         break;

      case "O":
         if (cmdline)  {  
            len = to_int(new_in2);
            new_in2 = "";
            if (len == 0)  {
               printf("Cancelled.\n");
               if (file == 0)  {
                  this_player()->editor_do_quit(0);
                  return;
               }
            }
            else if (len > sizeof(open_history) - count)  {
               printf("Invalid choice.\n");
               if (file == 0)  {
                  this_player()->editor_do_quit(0);
                  return ;
               }
            }
            else new_in2 = open_history[len - 1];
            open_history = open_history[count .. ];
            if (new_in2 != "")  open_file(new_in2);
         }
         else  { 
            if (new_in2 == "y"  ||  new_in2 == "Y")  {
               if (save_file("w", 1))  {
                  if (!changed)  {
                     if (!open_file(newname)  &&  newname == "...")
                        return;
                  }
                  else return;
               }
            }
            else if (new_in2 == "a"  ||  new_in2 == "A")
               printf("Open aborted.\n");
            else if (new_in2 == "n"  ||  new_in2 == "N") {
               printf("OK, you're the boss.\n");
               open_file(newname);
            }
            else  {
               printf("(Y)es, (N)o, or (A)bort?:]");
               input_to("get_new_text", 0, "O", count, 0);
               return;
            }
         }
         break;

      default:
         printf("How in the hell did you get here?\n");
         break;
   }
   printf(":");
   input_to("get_command");
   num_len = strlen(sprintf("%d", fsize + 1));
   return;
}  /* get_new_text() */


private int open_file(string filename)  {
   object *things;
   string  oldname;
   string  tmp;
   string *file_list;
   int     i;
   int     len;

   if (strlen(filename))  {
      if (mode_flag)  {
         if (filename == "...")  {
            if (!sizeof(open_history))  {
               printf("No files in your history.\n:");
               input_to("get_command");
               return -1;
            }
            else  {
               len = strlen(sprintf("%d", sizeof(open_history)));
               for (i = 0; i < sizeof(open_history); i++)  {
                  if (open_history[i] == name)
                     printf("%*d: %s (current file)\n", len, i + 1,
                            open_history[i]);
                  else
                     printf("%*d: %s\n", len, i + 1, open_history[i]);
               }
               printf("Choose file (enter nothing to cancel):] ");
               input_to("get_new_text", 0, "O", 0, 1);
               return -1;
            }
         }
         else if (sizeof(file_list =
                         get_dir(this_player()->get_path(filename))) > 1)  {
            i = strsrch(filename, '/', -1);
            if (i > -1)  filename = filename[0 .. i];
            else filename = "./";
            len = strlen(sprintf("%d", sizeof(file_list)));
            for (i = 0; i < sizeof(file_list); i++)  {
               if (file_list[i] == name)
                  printf("%*d: %s (current file)\n", len, i + 1, file_list[i]);
               else
                  printf("%*d: %s\n", len, i + 1, file_list[i]);
               file_list[i] = filename + file_list[i];
            }
            open_history = file_list + open_history;
            printf("Choose file (enter nothing to cancel):] ");
            input_to("get_new_text", 0, "O", i, 1);
            return -1;
         }
         else if (sizeof(things = WIZ_PRESENT->
                         wiz_present(filename, this_player())) == 1)  {
            oldname = this_player()->get_path(filename);
            filename = file_name(things[0]);
            sscanf(filename, "%s#%*s", filename);
            if (file_size(filename) == -2  ||  filename + ".c" == oldname)
               filename = oldname;
            else if (file_size(filename) == -1)  filename += ".c";
         }

         filename = this_player()->get_path(filename);
         if (file_size(filename) == -2)  {
            printf("\"%s\" is a directory.\n", filename);
            return 0;
         }
      }
      if (file_size(filename) == -1)  {
         if (mode_flag)  {
            if (filename != TMP_FILE)
               printf("Opening new file '%s'.\n", filename);
            else
               printf("Opening new file.\n");
         }
         file = ({ "End of file." });
         fsize = 0;
         this_player()->set_in_editor(filename);
      }
      else  {
         tmp = read_bytes(filename);
         if (stringp(tmp)) {
            file = explode(tmp, "\n") + ({ "End of file." });
            fsize = sizeof(file) - 1;
         }
         else {
            file = ({ "End of file." });
            fsize = 0;
         }
         this_player()->set_in_editor(filename);
         if (mode_flag)  {
            printf("%s opened", filename);
            if (!this_object()->write_permission(filename))
               printf(" (read only)");
            printf(".  (%d line%s, %d character%s)\n",
                   fsize, fsize == 1 ? "" : "s",
                   file_size(filename), file_size(filename) == 1 ? "" : "s");
         }
      }

      if (mode_flag)  {
         i = member_array(filename, open_history);
         if (i == -1)
            open_history += ({ filename });
         else if (i == 0)
            open_history = open_history[1 .. ] + ({ filename });
         else
            open_history = open_history[0 .. i - 1] +
                           open_history[i + 1 .. ] + ({ filename });
      }
   }
   else  {
      printf("Opening new file.\n");
      file = ({ "End of file." });
      fsize = 0;
      this_player()->set_in_editor("New file");
   }
   if (filename == ""  || (strlen(filename) > 2  &&
       (filename[<2 .. ] == ".h"  ||  filename[<2 .. ] == ".c")))
      highlight = 1;
   else
      highlight = 0;
   name = filename;
   lnptr = chptr = tmpln = tmpch = changed = 0;
   undo = ({ 0 });
   num_len = strlen(sprintf("%d", fsize + 1));
   in_string = 0;
   in_comment = 0;
   if (mode_flag)  {
      defs = ([ ]);
      defs_not_done = 1;
      while (remove_call_out("defines") != -1)
         ;
      call_out("defines", 1, name);
   }
   return 1;
}  /* open_file() */


private int save_file(string cmd, int open_pending)  {
   string tmp;
   string ftext;

   if (changed)  {
      if (strlen(name) == 0)  {
         printf("Enter a name for this file (enter nothing to cancel):]");
         input_to("get_new_text", 0, cmd, open_pending, 0);
         return 1;
      }
      ftext = fsize ? implode(file[0 .. fsize - 1], "\n") + "\n" : "";
      if (mode_flag)  {
         tmp = read_bytes(name);
         if (tmp  &&  !this_object()->rm_file(name))  {
            printf("Could not overwrite old file.\n");
            return 0;
         }
         else if (!write_file(name, ftext))  {
            printf("Could not write file '%s'.\n", name);
            if (tmp  &&  !write_file(name, tmp))
               printf("Could not restore the old version either.  Uh-oh.\n");
            return 0;
         }
         else  {
            changed = 0;
            printf("File '%s' saved.\n", name);
            undo[<1] = sizeof(undo) - 1;
         }
      }
   }
   else  {
      printf("No changes made; nothing saved.\n");
      ftext = "";
   }
   if (cmd == "W")  {
      this_player()->set_in_editor(0);
      this_player()->editor_do_quit(mode_flag || strlen(ftext) == 0 ? 0 :
                                    ftext);
   }
   return 1;
}  /* save_file() */


void my_more(string input, string *text, int startln, int number, int hilite,
             int pages, string xtratxt)
{
   int    i;
   int    j;
   int    k;
   int    len;
   int    limit;
   int    lines;
   int    oldlines;
   string junk;

   if (input == "q"  ||  input == "Q")  {
      printf(":");
      input_to("get_command");
      return;
   }
   i = lines = limit = 0;
   j = sizeof(text);
   do  {
      oldlines = lines;
      k = cols;
      if ((len = strlen(xtratxt)) == 0)  {
         junk = replace((number ? sprintf("%'.'*d] %s", num_len, 1 + startln++,
                                          text[i++])
                                : text[i++]), "\t", "\\TAB\\", "%", "%%");
//junk = replace(junk, "%", "%%");
//write("junk == " + junk + "\n");
         len = strlen(junk);

         ++lines;
         while (k < len)  {
            junk = junk[0 .. k - 1] + "\n" + junk[k .. ];
            ++lines;
            ++len;
            k += cols + 1;
         }
         if (hilite)
            junk = highlight(junk, startln - 1, number, lines >= rows);
      }
      else  {
         junk = xtratxt;
         ++lines;
         limit = 0;
         while ((k = strsrch(junk[limit .. ], "\n")) > -1)  {
            limit += k + 1;
            ++lines;
         }
         xtratxt = "";
      }

      if (lines < rows)  {
         if (len == 0)  printf("\n");
         else printf("%s\n", junk);
         len = 0;
      }
      else  {
         limit = 0;
         for (k = 0; k < rows - oldlines - 1; k++)  {
            limit += strsrch(junk[limit .. ], "\n") + 1;
         }
         xtratxt = junk[limit .. ];
         printf("%s", junk[0 .. limit - 1]);
      }
   }
   while (i < j  &&  lines < rows - 1);
   if ((i < j  ||  len)  &&  --pages)  {
      write("%^RESET%^MORE... enter 'q' to stop, anything else to continue:");
      input_to("my_more", 0, text[i .. ], startln, number, hilite, pages,
               xtratxt);
   }
   else  {
      if (i < j  &&  !pages)  lnptr -= j - i;
      write("%^RESET%^:");
      input_to("get_command");
   }
   return;
}  /* my_more() */


private void print_line(int show_ptr)  {
   string tmp;
   int    i;
   int    j;
   int    chr;
   int    tchr;
   int    len;

   chr = chptr;
   if (showln)  {
      tmp = sprintf("%'.'*d] %s", num_len, lnptr + 1, file[lnptr]);
      chr += num_len + 2;
   }
   else tmp = file[lnptr];

   tchr = chr;
   if (chr)  {
      chr += 4 * (chr - strlen(replace(tmp[0 .. chr - 1], "\t", "")));
   }
   if (tchr < strlen(tmp)  &&  tmp[tchr] == '\t')  chr += 2;

   tmp = replace(tmp, "\t", "\\TAB\\", "%", "%%");

   len = strlen(tmp);
   j = cols;
   while (j < len)  {
      tmp = tmp[0 .. j - 1] + "\n" + tmp[j .. ];
      ++len;
      j += cols + 1;
   }
   tmp += "\n";

   if (in_comment == lnptr + 1)
      in_comment = 0;

   if (!show_ptr)  {
      if (highlight)  {
         printf("%s", highlight(tmp, lnptr, showln, 0));
      }
      else  {
         printf("%s", tmp);
      }
   }
   else  {
//      if (this_player()->query_cur_term() == "ansi")  {
//         tmp = this_player()->fix_string(tmp[0 .. chr - 1] + "%^REVERSE%^" + tmp[chr .. chr] +  tmp[chr + 1 .. ]);
//         if (highlight)  tmp = highlight(tmp, lnptr, showln, 1);
//         printf("%s", tmp);
//      }
//      else  {
      if (highlight)  tmp = highlight(tmp, lnptr, showln, 1);
      i = chr / cols + 1;
      tchr = 0;
      for (j = 0; j < i; j++)
         tchr += strsrch(tmp[tchr .. ], "\n") + 1;
      printf("%s", tmp[0 .. tchr - 1]);
      if (chr % cols)  {
          write("                                                           "
                "                                                           "
                "                                          "
                [0 .. chr % cols - 1] + "%^RESET%^^\n");
      }
      else write("%^RESET%^^\n");
      printf("%s", tmp[tchr .. ]);
//      }
   }

   return;
}  /* print_line() */


private string color_word(string word,
                          int ref if_flag, int ref include_flag,
                          int ref type_flag, int ref class_flag)  {
   string newword;
   int    i;

   include_flag = 0;
   type_flag = 0;

   if (strlen(newword = replace(word, "\n", "")) != 0)  {
      if (newword[0] >= '0'  &&  newword[0] <= '9')  {
         word = numb_color + word + "%^RESET%^";
      }
      else switch (newword)  {
         case "#include" :
            include_flag = 1;
         case "#define"  :
         case "#ifdef"   :
         case "#ifndef"  :
         case "#undef"   :
         case "#else"    :
         case "#elif"    :
         case "#endif"   :
         case "#pragma"  :
         case "#echo"    :
         case "varargs"  :
         case "protected":
         case "private"  :
         case "public"   :
         case "nosave"   :
         case "nomask"   :
         case "if"       :
         case "while"    :
         case "for"      :
         case "foreach"  :
         case "in"       :
         case "switch"   :
         case "return"   :
         case "do"       :
         case "else"     :
         case "case"     :
         case "break"    :
         case "default"  :
         case "continue" :
         case "inherit"  :
         case "new"      :
         case "efun"     :
            word = keyw_color + word + "%^RESET%^";
            if (strlen(word) != strlen(newword))  {  /* embedded newline */
               i = strsrch(word, "\n");
               word = word[0 .. i] + keyw_color + word[i + 1 .. ];
            }
            if_flag = 0;
            break;

         /* This is so "#if defined(BLAH)" is colored correctly */
         case "#if"      :
            if_flag = 1;
            word = keyw_color + word + "%^RESET%^";
            if (strlen(word) != strlen(newword))  {  /* embedded newline */
               i = strsrch(word, "\n");
               word = word[0 .. i] + keyw_color + word[i + 1 .. ];
            }
            if_flag = 1;
            break;

         case "defined"  :
            if (if_flag)  {
               word = keyw_color + word + "%^RESET%^";
            }
            if_flag = 0;
            break;

         case "class"    :
            class_flag = 1;
            word = type_color + word + "%^RESET%^";
            if (strlen(word) != strlen(newword))  {  /* embedded newline */
               i = strsrch(word, "\n");
               word = word[0 .. i] + type_color + word[i + 1 .. ];
            }
            break;

         case "int"      :
         case "string"   :
         case "mixed"    :
         case "object"   :
         case "float"    :
         case "mapping"  :
         case "function" :
         case "buffer"   :
         case "ref"      :
            type_flag = 1;
         case "void"     :
            word = type_color + word + "%^RESET%^";
            if (strlen(word) != strlen(newword))  {  /* embedded newline */
               i = strsrch(word, "\n");
               word = word[0 .. i] + type_color + word[i + 1 .. ];
            }

         default         :
            if (class_flag)  {
               word = type_color + word + "%^RESET%^";
               if (strlen(word) != strlen(newword))  {  /* embedded newline */
                  i = strsrch(word, "\n");
                  word = word[0 .. i] + type_color + word[i + 1 .. ];
               }
               type_flag = 1;
               class_flag = 0;
            }
            break;
      }
   }
   return word;
} /* color_word() */


private string *boom(string str, int linenum, int last_line)  {
   int     i;
   int     j;
   int     k;
   int     last;
   int     count;
   int     sep_flag = 0;
   int     op_flag = 0;
   int     split;
   int     if_flag;
   int     include_flag;
   int     type_flag;
   int     class_flag;
   string *shrapnel = ({ });

   last = 0;
   if_flag = 0;
   include_flag = 0;

   for (i = 0; i < strlen(str); i++)  {
      if (in_comment)  {
         split = 0;
         j = strsrch(str[i .. ], "*/");
         if (j == -1)  {
            split = 1;
            j = strsrch(str[i .. ], "*\n/");
         }
         if (j == -1)  {
            shrapnel += ({ comm_color + str[last .. ] + "%^RESET%^" });
            i = strlen(str) - 1;
         }
         else  {
            in_comment = 0;
            shrapnel += ({ comm_color + str[last .. i + j + 1 + split] +
                           "%^RESET%^" });
            i += j + 1 + split;
         }
         if (last_line)  {
            shrapnel[<1] = replace(shrapnel[<1], "\n", "\n" + comm_color);
         }
         sep_flag = op_flag = 0;
         last = i + 1;
      }
      /* Try to detect strings... */
      else if (in_string  ||  str[i] == DQUOTES)  {
         if (i != last)  {
            shrapnel += ({ str[last .. i - 1] });
         }
         last = i;
         while (1)  { /* Don't worry, we'll break out eventually. */
            j = strsrch(str[i + 1 .. ], DQUOTES);
            if (j == -1)  {
               shrapnel += ({ stri_color + str[last .. ] + "%^RESET%^" });
               i = strlen(str) - 1;
               in_string = 1;
               break;
            }
            else  {
               k = i + j;
               count = split = 0;
               while (k > -1  &&  !split)  {
                  if (str[k] == '\\')  {
                     --k;
                     ++count;
                  }
                  else if (str[k] == '\n')  {
                     --k;
                  }
                  else split = 1;
               }
               i += j + 1;
               if (count % 2 == 0)  {
                  shrapnel += ({ stri_color + str[last .. i] +
                                 "%^RESET%^" });
                  in_string = 0;
                  break;
               }
            }
         }
         if (last_line)  {
            shrapnel[<1] = replace(shrapnel[<1], "\n", "\n" + stri_color);
         }
         sep_flag = op_flag = 0;
         last = i + 1;
      }
      else if (str[i] == SQUOTES)  {
         if (i != last)  {
            shrapnel += ({ str[last .. i - 1] });
         }
         last = i;
         while (1)  {
            j = strsrch(str[i + 1 .. ], SQUOTES);
            if (j == -1)  {  /* Must be a syntax error... */
               shrapnel += ({ stri_color + str[last .. ] + "%^RESET%^" });
               i = strlen(str) - 1;
               break;
            }
            else  {
               k = i + j;
               count = split = 0;
               while (k > -1  &&  !split)  {
                  if (str[k] == '\\')  {
                     --k;
                     ++count;
                  }
                  else if (str[k] == '\n')  {
                     --k;
                  }
                  else split = 1;
               }
               i += j + 1;
               if (count % 2 == 0)  {
                  /* special case: ''' */
                  if (i == last + 1  &&  last + 2 < strlen(str)  &&
                      str[last + 2] == SQUOTES)
                  {
                     shrapnel += ({ stri_color + str[last .. last + 2] +
                                    "%^RESET%^" });
                     ++i;
                  }
                  else  {
                     shrapnel += ({ stri_color + str[last .. i] +
                                    "%^RESET%^" });
                  }
                  break;
               }
            }
         }
         if (last_line)  {
            shrapnel[<1] = replace(shrapnel[<1], "\n", "\n" + stri_color);
         }
         sep_flag = op_flag = 0;
         last = i + 1;
      }
      else if (str[i] == LESS  &&  include_flag)  {
         if (i != last)  {
            shrapnel += ({ str[last .. i - 1] });
         }
         j = strsrch(str[i + 1 .. ], GREATER);
         if (j == -1)  {
            shrapnel += ({ stri_color + str[i .. ] + "%^RESET%^" });
            i = strlen(str) - 1;
         }
         else  {
            shrapnel += ({ stri_color + str[i .. i + j + 1] + "%^RESET%^" });
            i += j + 1;
         }
         if (last_line)  {
            shrapnel[<1] = replace(shrapnel[<1], "\n", "\n" + stri_color);
         }
         sep_flag = op_flag = 0;
         last = i + 1;
      }

      /* Try to detect comments... */
      else if (str[i .. i + 1] == "/*"  ||  str[i .. i + 2] == "/\n*")  {
         if (i != last)  {
            shrapnel += ({ str[last .. i - 1] });
         }
         split = (str[i + 1] == '\n');
         j = strsrch(str[i + 2 + split .. ], "*/");
         if (j == -1)  {
            j = strsrch(str[i + 2 + split .. ], "*\n/");
            ++split;
         }

         if (j == -1)  {
            in_comment = linenum + 1;
            shrapnel += ({ comm_color + str[i .. ] + "%^RESET%^" });
            i = strlen(str) - 1;
         }
         else  {
            shrapnel += ({ comm_color + str[i .. i + j + 3 + split] +
                           "%^RESET%^" });
            i += j + 3 + split;
         }
         if (last_line)  {
            shrapnel[<1] = replace(shrapnel[<1], "\n", "\n" + comm_color);
         }
         sep_flag = op_flag = 0;
         last = i + 1;
      }

      else if (str[i .. i + 1] == "//"  ||  str[i .. i + 2] == "/\n/")  {
         if (i != last)  {
            shrapnel += ({ str[last .. i - 1] });
         }
         shrapnel += ({ comm_color + str[i .. ] + "%^RESET%^" });
         i = strlen(str) - 1;
         if (last_line)  {
            shrapnel[<1] = replace(shrapnel[<1], "\n", "\n" + comm_color);
         }
         sep_flag = op_flag = 0;
         last = i + 1;
      }

      else if (member_array(str[i], SEPARATORS) != -1)  {
         if (last <= i - 1)  {
            // Capture the word before the separator
            shrapnel += ({ color_word(str[last .. i - 1],
                                      ref if_flag, ref include_flag,
                                      ref type_flag, ref class_flag) });
         }
         // Capture the separator
         if (sep_flag)  {
            shrapnel[<1] += str[i .. i];
         }
         else  {
            shrapnel += ({ str[i .. i] });
            sep_flag = 1;
         }
         op_flag = 0;
         if (str[i] == SCOLON)
            type_flag = 0;
         last = i + 1;
      }

      else if (member_array(str[i], OPERATORS) != -1)  {
         if (last <= i - 1)  {
            shrapnel += ({ color_word(str[last .. i - 1],
                                      ref if_flag, ref include_flag,
                                      ref type_flag, ref class_flag) });
            class_flag = 0;
         }
         if (str[i] == TIMES  &&  type_flag)  {
            shrapnel += ({ type_color + str[i .. i] + "%^RESET%^" });
            type_flag = 0;
         }
         else if (op_flag)  {
             shrapnel[<1] = shrapnel[<1][0 .. <10] + str[i .. i] +
                            "%^RESET%^";
         }
         else  {
             shrapnel += ({ oper_color + str[i .. i] + "%^RESET%^" });
             op_flag = 1;
         }
         sep_flag = 0;
         last = i + 1;
      }
      else sep_flag = op_flag = 0;
   }
   if (last < i)  {
      shrapnel += ({ color_word(str[last .. ],
                                ref if_flag, ref include_flag,
                                ref type_flag, ref class_flag) });
   }

   return shrapnel;
} /* boom() */


private string highlight(string str, int linenum, int numbering,
                         int last_line)
{
   string *words;
   string  num = "";

   if (linenum == fsize)  return str;
   if (numbering)  {
      num = str[0 .. num_len + 1];
      str = str[num_len + 2 .. ];
   }
   words = boom(replace(str, "%^", "%%%^^^"), linenum, last_line);
   words = map(words, "fix_string", this_player());

   return this_player()->fix_string(num + implode(words, ""));

} /* highlight() */


private void init_settings()  {
   string  rc;
   string  line;
   string *sets;
   string *bits;
   int     spaces;
   int     i;

   tab_str = "   ";
   tab_replace = 0;
   keyw_color = "%^CYAN%^";
   comm_color = "%^GREEN%^";
   numb_color = "%^MAGENTA%^";
   oper_color = "%^BLUE%^";
   stri_color = "%^YELLOW%^";
   type_color = "%^BOLD%^%^CYAN%^";

   rc = read_bytes("/w/" + this_player()->query_name() + "/.magicrc");
   if (strlen(rc) == 0)  return;

   sets = explode(rc, "\n");
   foreach (line in sets)  {
      bits = explode(line, " ") - ({ "" });
      if (sizeof(bits) > 2)  {
         if (bits[0] == "tab")  {
            if (bits[1] == "spacing")  {
               if ((spaces = to_int(bits[2])) > 0)  {
                  tab_str = "";
                  for (i = 0; i < spaces; i++)  {
                     tab_str += " ";
                  }
               }
            }
            else if (bits[1] == "replacement")  {
               if (bits[2] == "on")  tab_replace = 1;
               else tab_replace = 0;
            }
         }
         else if (bits[0] == "color")  {
            set_colors(bits[1 .. ]);
         }
         else if (bits[0] == "history")  {
            open_history = bits[1 .. ];
         }
      }
   }
   return;

} /* init_settings() */


private int set_colors(string *words)  {
   string tmp_color;

   tmp_color = implode(map(words[1 .. ],
                           (: "%^" + upper_case($1) + "%^" :)), "");
   switch (words[0])  {
      case "keyword":
         keyw_color = tmp_color;
         return 1;

      case "comment":
         comm_color = tmp_color;
         return 1;

      case "number":
         numb_color = tmp_color;
         return 1;

      case "string":
         stri_color = tmp_color;
         return 1;

      case "operator":
         oper_color = tmp_color;
         return 1;

      case "type":
         type_color = tmp_color;
         return 1;

      default:
         return 0;
   }
} /* set_colors() */


int write_permission(string name)  {
   return write_file(name, "");
}


int rm_file(string name)  {  return rm(name);  }


string *filter_regexp(string *incoming)  {
   string *result;
   int     i;

   result = allocate((sizeof(incoming) + 1) / 2);
   for (i = sizeof(incoming) - 1; i >= 0; i -= 2)
      result[i / 2] = incoming[i];
   return result;

} /* filter_regexp() */


int save_history()  {
   string tmp;
   string *junk;
   int i;
   int res;

   tmp = read_bytes("/w/" + this_player()->query_name() + "/.magicrc");
   if (tmp)  {
      junk = explode(tmp, "\n");
      for (i = 0; i < sizeof(junk); i++)  {
         if (junk[i][0 .. 6] == "history")  {
            junk[i] = sprintf("history %s",
                              implode(open_history[<10 .. ], " "));
            break;
         }
      }
   }
   else junk = ({ sprintf("history %s",
                          implode(open_history[<10 .. ], " ")) });
   res = write_file("/w/" + this_player()->query_name() + "/.magicrc",
                    implode(junk, "\n"), 1);
   return res;

} /* save_history() */


string get_inc_path(string fname, string last_file)  {
   string  ret;
   string  tmp;
   string *bits;
   int     count;

   /* Strip off any leading spaces */
   while (fname[0] == ' ')
      fname = fname[1 .. ];

   if (sscanf(fname, "<%s>", tmp) == 1)  {
      ret = "/include/";
      fname = tmp;
   }
   else if (sscanf(fname, "\"%s\"", tmp) == 1)  {
      if (tmp[0] == '/')
         ret = "";
      else  {
         bits = explode(last_file, "/");
         count = 2;
         while (tmp[0 .. 2] == "../")  {
           ++count;
           tmp = tmp[3 .. ];
         }
         ret = "/" + implode(bits[0 .. <count], "/") + "/";
      }
      fname = tmp;
   }
   else return "";

   if (file_size(ret + fname) < 0)
      return "";
   return ret + fname;

} /* get_inc_path() */


string expand_macros(string ins, string *sorted_defs)  {
   int     changed;
   int    *allowed = ({' ', '\t', '+', '-', ',', '(', '\"', '[' });
   int     off;
   string  def;

   do {
      changed = 0;
      foreach (def in sorted_defs)  {
         if ((off = strsrch(ins, def)) != -1)  {
            if (off == 0  ||  member_array(ins[off - 1], allowed) >= 0)  {
               ins = replace_string(ins, def, defs[def]);
               changed = 1;
            }
         }
      }
   }
   while(changed);

   return ins;

} /* expand_macros() */


string strip_junk(string stuff){
   stuff = replace(stuff, ({ " ", "", "+", "", "(", "", ")", "" }));
   return replace_string(stuff, "\"\"", "");
}


void defines(string fname)  {
   string *lines;
   string  line;
   string  tmp;
   string  tmp2;
   int     off;
   string *sorted_defs;
   int     i;
   int     j;
   int     nest;

   if (fname != name)  {
      tmp = read_file(fname);
      if (tmp)
         lines = regexp(explode(tmp, "\n"), "^#[ ]*(include|define)[ \t]+");
      else
         lines = ({ });
   }
   else lines = regexp(file, "^#[ ]*(include|define)[ \t]+");

   foreach (line in lines)  {
      if ((off = strsrch(line, "include")) != -1)  {
         tmp = line[off + 8 .. ];
         tmp2 = get_inc_path(tmp, fname);
         call_out("defines", defs_not_done, tmp2);
         ++defs_not_done;
      }
      else {
         i = 0;
         nest = 0;

         off = strsrch(line, "define");
         tmp = line[off + 7 .. ];
         while (tmp[i] == ' ')
            ++i;
         tmp = tmp[i .. ];
         i = 0;
         while (i < strlen(tmp)  &&  (tmp[i] != ' '  ||  nest))  {
            if (tmp[i] == '(')  {
               ++nest;
            }
            else if (tmp[i] == ')')
               --nest;
            ++i;
         }
         j = i;
         while (j < strlen(tmp)  &&  tmp[j] != ' ')
            ++j;

         tmp2 = tmp[j .. ];
         tmp = tmp[0 .. i - 1];

         defs[tmp] = tmp2;
      }
   }
   --defs_not_done;

   if (!defs_not_done)  {
      sorted_defs = sort_array(keys(defs), -1);
      foreach (tmp, tmp2 in copy(defs))  {
         if (strsrch(tmp, '(') == -1)
            defs[tmp] = strip_junk(expand_macros(tmp2, sorted_defs));
      }
   }
} /* defines() */
