/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: more_file.c,v 1.9 2002/02/26 02:41:22 presto Exp $
 * $Log: more_file.c,v $
 * Revision 1.9  2002/02/26 02:41:22  presto
 * Fixed bug with trying to more a file for which you didn't have read perms.
 * Slight changes for efficiency.
 *
 * Revision 1.8  2001/06/03 20:23:09  pinkfish
 * Change a variable name.
 *
 * Revision 1.7  2001/06/01 21:05:29  ceres
 * Can't remember
 *
 * Revision 1.6  2000/10/20 18:03:57  pinkfish
 * Add in some code to deal with tab stops.
 *
 * Revision 1.5  2000/10/20 17:54:46  ceres
 *  Forcibly unlocked by pinkfish
 *
 * Revision 1.4  2000/04/26 04:53:18  ceres
 * Fixed runtime
 *
 * Revision 1.3  1999/07/24 13:52:30  presto
 * Fix for my fix.  Gerbil was getting sarcastic. :)
 *
 * Revision 1.2  1999/07/24 01:24:54  presto
 * Fixed problem with more-ing files containing blank lines.
 *
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 * 
*/
/*
 * Original by Ember.
 * Improved version by Pinkfish.
 * Debugged by Deutha.
 */

#include <nroff.h>
#include <cmds/options.h>

#define ROWS (this_player()->query_rows() - 1)
#define COLS this_player()->query_cols()

private nosave int fsize, topl, botl, charl, real_rows, search_flag;
private nosave mixed mark_data;
private nosave string _more_file_fname,
              *mark_name,
              *filenames,
              finish_func,
              stat_line,
              last_search;

private int display_file() {
   int i;
   int ncols;
   int nrows;
   string file;
   string *bits;
   string *lines;
   string tstop;
   int tab;

   ncols = COLS;
   nrows = ROWS;
   file = read_file(_more_file_fname, topl, nrows);
   if (file == 0)
      return 0;

   // Work out the tabstops.
   tab = this_player()->query_property(TABSTOP_PROP);

   if (tab)
      tstop = sprintf("%" + tab + "' 's", "");
   else
      tstop = "<TAB>";

   file = replace(file, ({ "\t", tstop,
                           "\r", "^M",
                           sprintf("%c", 13), "^M" }));

   /* We add the extra character and then remove it from the resulting
    * array because this won't work on totally blank lines. :( */
   bits = explode("x\n" + file, "\n")[1 .. ];
   if (charl)
      bits[0] = bits[0][charl .. ];
   real_rows = 0;
   lines = ({ });
   for (i = 0; i < sizeof(bits); i++)  {
      if (strlen(bits[i]) > ncols)  {
         lines = ({ lines..., bits[i][0 .. ncols - 1] });
         bits[i] = bits[i][ncols .. ];
         charl += ncols;
         --i;
      } else {
         lines  = ({ lines..., bits[i] });
         charl = 0;
         ++real_rows;
      }
      if (sizeof(lines) == nrows)
         break;
   }
   printf("%s\n", implode(lines, "\n"));
   botl = topl + real_rows - 1;
   if (botl > fsize)
      botl = fsize;
  return 1;
} /* display_file() */


private int get_next_filename() {
  string str;

  _more_file_fname = filenames[0];
  filenames = filenames[1 .. ];
  fsize = file_length(_more_file_fname);
  if (fsize == 0) {
    printf("Empty file.\n");
    if (sizeof(filenames) > 1)
      return get_next_filename();
    else
      return 0;
  }
  if (fsize == -1) {
    printf("File does not exist.\n");
    if (sizeof(filenames) > 1)
      return get_next_filename();
    else
      return 0;
  }
  if (fsize == -2) {
    printf("You cannot more a directory.\n");
    if (sizeof(filenames) > 1)
      return get_next_filename();
    else
      return 0;
  }
  topl = 1;
  charl = 0;
  printf( "\n" );
  str = read_file(_more_file_fname, 1, 1);
  if (str == 0)
    return 0;
  
  if (str[0] == '.')  {
    string s2;

    /*
     * Nroff file...  do horrible things!  Turn into a mushroom!  Blow up
     * all the hairy crater eating womble doovalacies...
     * In the case of a nroff file we push the file through nroff then shove
     * it through more_string...
     */
    str = "/tmp/nroffed_file";
    if (NROFF_HAND->create_nroff(_more_file_fname, "/tmp/nroffed_file")) {
      s2 = NROFF_HAND->cat_file("/tmp/nroffed_file");
      rm("/tmp/nroffed_file.o");
      if (s2 != 0) {
        this_player()->more_string(s2, _more_file_fname);
        fsize = ROWS - 3;
        return 1;
      }
    }
  }
  return display_file();
} /* get_next_file_name() */


private string *expand_path(string s) {
  string *s1;

  s1 = this_player()->get_files(s);
  if (s1 == 0)
    return ({ s });
  return s1;
} /* expand_path() */


int set_mark(string s) {
   int i;
   
   if (!mark_name) {
      mark_name = ({ });
      mark_data = ({ });
   }
   if ((i = member_array(s, mark_name)) != -1)
      mark_data[i] = ({ _more_file_fname, topl });
   else {
      mark_name += ({ s });
      mark_data += ({ ({ _more_file_fname, topl }) });
   }
   return 1;
} /* set_mark() */


private void status_line() {
   string *frog;
   string s;
   int i, percentage;
   
   if (!stat_line)
      stat_line = "$N From $T to $B of $S ($%%) - h for help. ";
   s = "";
   frog = explode(stat_line, "$");
   for (i = 0; i < sizeof(frog); i++) {
      if (frog[i] == "") {
         s += "$";
         ++i;
      }
      else switch (frog[i][0]) {
         case 'N':
            s += _more_file_fname + frog[i][1 .. ];
            break;

         case 'T':
            s += topl + frog[i][1 .. ];
            break;

         case 'B':
            if (botl > fsize)
               s += fsize + frog[i][1 .. ];
            else
               s += botl + frog[i][1 .. ];
            break;

         case '%':
            percentage = (botl * 100) / fsize;
            if (percentage > 100) percentage = 100;
            s += percentage + frog[i][1 .. ];
            break;

         case 'S':
            s += fsize + frog[i][1 .. ];
            break;

         default:
            printf("Bad tag (%s) found in format string.\n", frog[i]);
            break;
      }
   }
   printf("%s", s);
   return;
} /* status_line() */


void next_page(string str) {
   int num, noargs, i, j, k, redraw;
   string s1, *s3;
   
   if (str == 0)
      str = "";
   
   if (sscanf(str, "%d%s", num, str) != 2)
      noargs = 1;
   s1 = extract(str, 1);
   /* case statements WEEEEEE */
   switch (str[0 .. 0]) {
      case "":
      case " ":
         topl += real_rows;
         redraw = 1;
         break;

      case "f":
         /* go on a number of pages... */
         if (noargs)
            num = 1;
         topl += ROWS * num;
         redraw = 1;
         break;

      case "q" :
         printf("OK.\n");
         return;

      case "/":
         /* sigh */
         search_flag = 0;
         j = topl + 4;
         if (s1[0] == '!') {
            search_flag = 1;
            s1 = extract(s1, 1);
         }
         set_mark("'");
         if (s1=="" || !s1)
            s1 = last_search;
         if (s1 == "") {
            printf("No previous search string.\n");
            break;
         }
         do {
            i = j;
            j = i + 900;
            if (j > fsize)
               j = fsize;
            s3 = explode(read_file(_more_file_fname, i, j), "\n");
            for (k = 0; k < sizeof(s3); k++)
               if (!search_flag) {
                  if (sscanf(s3[k], "%*s" + s1 + "%*s") == 2)
                     if (num-- <= 0)
                        break;
               } else
                  if (sscanf(s3[k], "%*s" + s1 + "%*s") != 2)
                     if (num-- <= 0)
                     break;
         } while (j < fsize && k == sizeof(s3));
         if (k == sizeof(s3))
            printf("Sorry " + s1 + " not found.\n");
         else {
            topl = i + k - 3;
            redraw = 1;
         }
         last_search = s1;
         break;

      case "?":
         i = topl;
         if (s1[0] == '!') {
            s1 = extract(s1, 1);
            search_flag = 3;
         } else
            search_flag = 2;
         set_mark("'");
         if (s1=="" || !s1)
            s1 = last_search;
         if (s1 == "") {
            printf("No previous search string.\n");
            break;
         }
         do {
            j = i - 900;
            if (j < 0)
               j = 0;
            s3 = explode(read_file(_more_file_fname, j, i), "\n");
            for (k = 0; k < sizeof(s3); k++)
               if (search_flag == 2) {
                  if (sscanf(s3[k], "%*s" + s1 + "%*s") == 2)
                     if (num-- <= 0)
                        break;
               } else
                  if (sscanf(s3[k], "%*s" + s1 + "%*s") != 2)
                     if (num-- <= 0)
                        break;
            i = j;
         } while (j > 0 && k == sizeof(s3));
         if (k == sizeof(s3))
            printf("Sorry "+ s1 +" not found.\n");
         else {
            topl = k + i - 2;
            redraw = 1;
         }
         last_search = s1;
         break;

      case "n":
         switch (search_flag) {
            case 0:
               next_page(num + "/");
               break;

            case 1:
               next_page(num + "/!");
               break;

            case 2:
               next_page(num + "?");
               break;

            case 3:
               next_page(num + "?!");
               break;
         }
         return;

      case "b":
         if (noargs)
            num = 1;
         if (topl > 0) {
            topl -= ROWS * num;
            redraw = 1;
            if (topl < 0)
               topl = 0;
         }
         break;

      case "G":
         if (noargs)
            num = fsize - ROWS + 1;
  
      case "g":
         set_mark("'");
         topl = num;
         if (topl >= fsize)
         topl = fsize - 2;
         redraw = 1;
         break;

      case "P":
         if (noargs)
            num = 100;

      case "p":
      case "%":
         redraw = 1;
         set_mark("'");
         topl = (num * fsize) / 100;
         if (topl + ROWS - 1 > fsize)
            topl -= ROWS - 1;
         break;

      case "d":
         if (noargs)
            num = ROWS / 2;
         topl += num;
         redraw = 1;
         break;

      case "u":
         if (noargs)
            num = ROWS / 2;
         topl -= num;
         redraw = 1;
         break;

      case "r":
         redraw = 1;
         break;

      case "m":
         if (s1 == "") {
            printf("Sorry, you must specify a name.\n");
            break;
         }
         set_mark(s1);
         printf("OK, mark " + s1 + " set.\n");
         break;

      case "'":
         if (!mark_name) {
            printf("Sorry, must go to a mark.\n");
            break;
         }
         if ((i = member_array(s1, mark_name)) != -1) {
            if (_more_file_fname != mark_data[i][0]) {
               _more_file_fname = mark_data[i][0];
               redraw = 1;
            }
            if (topl != mark_data[i][1]) {
               topl = mark_data[i][1];
               redraw = 1;
            }
         } else
            printf("Mark " + s1 + " not set.\n");
         break;

      case "F":
         filenames = expand_path(str);
         if (sizeof(filenames) == 0) {
            printf("No matching files.\n");
            break;
         }
         last_search = "";
         redraw = get_next_filename();
         break;

      case "h":
         cat("/doc/helpdir/more");
         break;
   }
   if (redraw)
      display_file();
   if (botl < fsize || charl) {
      status_line();
      input_to("next_page");
   } else
      if (sizeof(filenames) > 0) {
         if (get_next_filename())
            input_to("next_page");
         else if (finish_func)
            call_other(this_player(), finish_func);
      } else {
         if (finish_func)
            call_other(this_player(), finish_func);
      }
} /* next_page() */


int more_file(string str) {
   filenames = expand_path(str);
   if (!sizeof(filenames)) {
      notify_fail("No matching files.\n");
      return 0;
   }
   last_search = "";
   if (!get_next_filename())  {
      notify_fail("Could not read " + _more_file_fname + ".\n");
      return 0;
   }
   if (botl < fsize || charl) {
      status_line();
      input_to("next_page");
   } else
      if (finish_func)
         call_other(this_player(), finish_func);
   return 1;
} /* more_file() */
