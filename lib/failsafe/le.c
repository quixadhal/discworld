/* This is a very quick and dirty line editor cooked up by */
/* [als] in half an hour on a not-so-stormy night of 10/12/91 */
/* It uses NO input_to's, so ALL commands are prefixed with the command word */
/* it was simply intended for those times when ed goes down the */
/* proverbial gurgler.  If you HAVE to use it, set up aliases :) */
/* commands: r file, w[file], >insert text, /search text, */
/* number goes to line number, +, -, d, N-> clears line editor */
/* Have fun, and win awards */
/* One other possible use for LE is for cute aliases */

static int line;
static string *cfile, cfile_name, last_search;

print_line()
{
   if (line > sizeof(cfile)) line = sizeof(cfile);
   if (!line) { write("No line.\n"); return; }
   write(extract("   ", 0, 3 - strlen(line + "")) + line + ":" + cfile[line-1] + "\n");
}

le(s)
{
   int j;

   if (!pointerp(cfile)) { cfile = ({ }); line = 0; }
   if (!s || s == "") { print_line(); return 1; }

   if (sscanf(s, "%d", j)) { line = j; print_line(); return 1; }
   switch (s[0]) {
   case 'd':
      if (!line) { notify_fail("Not on a line.\n"); return 0; }
      cfile = delete(cfile, line-1, 1);
      print_line();
      return 1;
   case 'N':
      cfile = ({ });
      line = 0;
      cfile_name = 0;
      write("Line editor cleared.\n");
      return 1;
   case '/':
      s = extract(s, 1);
      if (s == "") {
         s = last_search;
      }
      last_search = s;
      {
         int i, s1, s2;
         for (i = line+1; i<= sizeof(cfile); i++) {
            if (sscanf(cfile[i-1], "%s"+s+"%s", s1, s2)) { line = i; print_line(); return 1; }
         }
         write("Search failed.\n");
         return 1;
      }
   case '>':
      s = extract(s, 1);
      {
         string *tmp1, *tmp2;

         tmp1 = (line > 1) ? cfile[0..line-2] : ({ });
         tmp2 = (line <= sizeof(cfile)) ? cfile[line-1..sizeof(cfile)-1] : ({ });
         cfile = tmp1 + ({ s }) + tmp2;
      }
      line++;
      print_line();
      return 1;
   case 'r':
      {
         string sA, sB;
      
         sA = extract(s, 1);
         while (sA[0] == ' ') sA = extract(sA,1);
         sB = this_player()->get_path(sA);
         if (!sB) sB = sA;
         if (file_size(sB) < 0) { notify_fail("File doesn't exist.\n"); return 0; }
         sA = read_file(sB);
         cfile = explode(sA, "\n");
         line = 1;
      }
      write("Read file "  + sB + ": " + sizeof(cfile) + " lines.\n");
      cfile_name = sB;
      print_line();
      return 1;
   case '+':
      if (line < sizeof(cfile)) line++;
      print_line();
      return 1;
   case '-':
      if (line > 1) line --;
      print_line();
      return 1;
   case 'w':
      if (s == "w") {
         if (!cfile_name) { notify_fail("No file name.\n"); return 0; }
      } else {
         s = extract(s,1);
         while (s[0] == ' ') s = extract(s, 1);
         if (this_player()->get_path(s)) s = this_player()->get_path(s);
         cfile_name = s;
      }
      rm(cfile_name);
      write_file(cfile_name, implode(cfile, "\n"));
      write("Tried to write file " + cfile_name + ": " + sizeof(cfile) + " lines.\n");
      return 1;
   case 'z':
      {
         int q;

         q = 20;
         while (q-- && line <= sizeof(cfile)) { print_line(); line++; }
         line --;
      }
      return 1;
   }
}