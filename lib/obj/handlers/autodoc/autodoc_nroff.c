/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: autodoc_nroff.c,v 1.15 2002/07/15 18:50:53 pinkfish Exp $
 * $Log: autodoc_nroff.c,v $
 * Revision 1.15  2002/07/15 18:50:53  pinkfish
 * Fix up the use of the <li> tag.
 *
 * Revision 1.14  2001/08/12 03:57:41  pinkfish
 * Add in some fixes to <dl> mode and add support for underline.
 *
 * Revision 1.13  2001/06/30 10:14:16  pinkfish
 * Fix up the formatting in with the <pre> flags.
 *
 * Revision 1.12  2001/05/10 07:02:45  pinkfish
 * Add in support for center.
 *
 * Revision 1.11  2001/05/10 01:03:35  pinkfish
 * Fix up some autodoc stuff.
 *
 * Revision 1.10  2000/03/03 20:22:18  pinkfish
 * Fix up the autodoc stuff.
 *
 * Revision 1.9  1999/06/30 03:58:56  pinkfish
 * Reverting the changes to the autodoc stuff because the writing out to
 * one file stuff was totaly broken.  It wrote all the normal files into
 * the main file as well.
 *
 * Revision 1.7  1999/05/05 00:35:57  pinkfish
 * Fix it up to print out the name correctly.
 *
 * Revision 1.6  1999/03/22 23:08:31  pinkfish
 * Fix it up to display class data in the help files.
 *
 * Revision 1.5  1999/02/10 10:15:10  pinkfish
 * Add in support for defines and classes.
 *
 * Revision 1.4  1999/02/10 06:11:08  pinkfish
 * Fix up a little.
 *
 * Revision 1.3  1998/11/24 07:50:49  pinkfish
 * Add in stuff to do parameters.
 *
 * Revision 1.2  1998/03/25 13:18:06  pinkfish
 * Added a defined in section so you can see where the function
 * came from
 *
 * Revision 1.1  1998/01/06 05:04:22  ceres
 * Initial revision
 * 
*/
/**
 * This creates an nroff document from the generated automatic documentation.
 * It also writes out the single file descriptions for the methods with
 * documentation attached.
 *
 * @author Pinkfish
 * @started Tue Oct 28 11:39:47 EST 1997
 */

#include <autodoc.h>

private void write_single_description(string func, mixed *stuff,
                                      string output, string fn);
private string query_methods(string *funcs, function data,
                           string base_single, string fn);
private string query_func_def(mixed *stuff, string func);
private string query_method_description(mapping docs);
private string parse_string(string inp);
string convert_html(string text, int depth, int preformat, int edge_width);
private string query_defines(mapping defines);
private string query_class_def(mixed *stuff, string name);
private string query_class_description(mapping docs);
private void write_single_class_description(string func, mixed *stuff,
                                            string output, string fn);
private string query_classes(string base_single,
                           mapping classes,
                           string fn);

void create() {
   seteuid(getuid());
} /* create() */

/**
 * Writes the data from the autodoc file object out to a file in nroff
 * format.
 *
 * @param file the autodoc file object
 * @param output the output file to write to
 *
 * @see /obj/handlers/autodoc/autodoc_handler
 */
void create_nroff_file(object file, string output) {
   mapping stuff;
   string *funcs;
   string *bits;
   string name;
   string base_single;
   string *files;
   string ret;
   int pos;

   /*
    * Ok, we do the alphabetised list, first a short list with the
    * first sentance of each description, then a longer expanded list..
    */
   bits = explode(file->query_file_name(), "/");
   name = bits[sizeof(bits)-1];
   ret = "";
   ret += ".DT\n";
   ret += name + "\n" + mud_name() + " autodoc help\n" + name + "\n\n";
   stuff = file->query_main_docs();
   if (stuff["main"]) {
      ret += ".SH Description\n";
      ret += convert_html(implode(stuff["main"], " "), 5, 0, 5);
      ret += ".SP 10 5\n";
      if (stuff["author"]) {
         ret += "\n\nWritten by " +
                    parse_string(query_multiple_short(stuff["author"]));
      }
      if (stuff["started"]) {
         ret += "\n\nStarted " +
                    parse_string(query_multiple_short(stuff["started"]));
      }
      ret += "\n.EP\n\n";
      if (stuff["see"]) {
         ret += "\n\n.SH See also\n.SP 5 5\n" +
                            query_multiple_short(stuff["see"]) + "\n.EP\n";
      }
      if (stuff["changed"]) {
         ret += ".SH Change history\n.SP 5 5\n" +
                            parse_string(implode(stuff["changed"], "\n\n")) +
                            "\n.EP\n\n";
      }
      if (stuff["example"]) {
        ret += ".SH Example\n.SI 5\n" +
               parse_string(implode(stuff["example"], "\n")) + "\n.EI\n";
      }
   }

   if (sizeof(file->query_inherits())) {
      ret += ".SH Inherits\n.SP 5 5\n";
      ret += "This class inherits the following classes " +
                         query_multiple_short(keys(file->query_inherits())) +
                         ".\n.EP\n\n";
   }

   if (sizeof(file->query_includes())) {
      ret += ".SH Includes\n.SP 5 5\n";
      ret += "This class includes the following files " +
                         query_multiple_short(file->query_includes()) +
                         ".\n.EP\n\n";
   }


   /*
    * Filter out create and setup as they are not real useful functions
    * to document :)
    */
   funcs = filter(keys(file->query_public_functions()),
                  (: $1 != "create" && $1 != "setup" &&
                   !$(file)->query_public_functions()[$1][AUTO_DOCS]["ignore"] :));
   base_single = file->query_file_name();
   pos = strsrch(base_single, ".", -1);
   if (pos > 0) {
      base_single = base_single[0..pos-1];
   }
   base_single = NROFF_DOC_SINGLE + base_single + "/";
   files = get_dir(base_single + "*");
   /*
    * Remove all the old files first, in case a function has changed name
    * or something.
    */
   if (pointerp(files)) {
      foreach (name in files) {
         if (file_size(base_single + name) >= 0) {
            rm(base_single + name);
         }
      }
   }

   if (sizeof(file->query_class_docs())) {
      ret += ".SH Classes\n\n";
      ret += query_classes(base_single,
                    file->query_class_docs(),
                    file->query_file_name());
      ret += "\n";
   }

   if (sizeof(funcs)) {
      ret += ".SH Public Functions\n" +
                         ".SP 5 5\nThese are functions that everyone can "
                         "access.\n.EP\n\n";
      ret += query_methods(funcs, 
                    (: $(file)->query_public_functions()[$1] :),
                    base_single, 
                    file->query_file_name());
      ret += "\n";
   }

   /*
    * Filter out create and setup as they are not real useful functions
    * to document :)
    */
   funcs = filter(keys(file->query_protected_functions()),
                  (: !$(file)->query_protected_functions()[$1][AUTO_DOCS]["ignore"] :));
   if (sizeof(funcs)) {
      ret += ".SH Protected Functions\n"
                         ".SP 5 5\nThese are functions that only "
                         "objects inheriting the class can access.\n.EP\n\n";
      ret += query_methods(funcs, (: $(file)->query_protected_functions()[$1] :),
                    base_single, file->query_file_name());
      ret += "\n";
   }

   bits = explode(file->query_file_name(), "/");
   name = bits[sizeof(bits)-1];
   /* Ignore private functions, because they are less than useful anyway. */
   if (sscanf(name, "%*s.h") == 1 &&
       sizeof(file->query_define_docs())) {
      ret += ".SH Defines\n";
      ret += query_defines(file->query_define_docs());
      ret += "";
   }
   write_file(output, ret);
} /* create_nroff() */

private string query_defines(mapping docs) {
   string name;
   string *names;
   string ret;

   names = sort_array(keys(docs), (: strcasecmp :));
   ret = "";
   foreach (name in names) {
      ret += ".SI 3\n* %^BOLD%^" + name + "%^RESET%^\n" +
                         ".EI\n";
      if (docs[name]["main"]) {
         ret += query_method_description(docs[name]);
      }
      ret += "\n";
   }
   return ret;
} /* write_defines() */

private string query_classes(string base_single,
                           mapping classes,
                           string fn) {
   string name;
   string *names;
   string ret;

   names = sort_array(keys(classes), (: strcasecmp :) );
   ret = "";
   foreach (name in names) {
      reset_eval_cost();
      ret += ".SI 3\n* %^BOLD%^" + name + "%^RESET%^\n";
      ret += query_class_def(classes[name], name) + "\n.EI\n";
      if (classes[name][AUTO_CLASS_DOCS]["main"]) {
         ret += query_class_description(classes[name][AUTO_CLASS_DOCS]);
         write_single_class_description(name, 
                                        classes[name], 
                                        base_single + name, 
                                        fn);
      }
      ret += "\n";
   }
   return ret;
} /* write_classes() */

private string query_methods(string *funcs, 
                           function data, 
                           string base_single, 
                           string fn) {
   int i;
   mixed *stuff;
   string ret;

   funcs = sort_array(funcs, (: strcmp($1, $2) :));
   ret = "";
   for (i = 0; i < sizeof(funcs); i++) {
      stuff = evaluate(data, funcs[i]);
      ret += ".SI 3\n* %^BOLD%^" + funcs[i] + "%^RESET%^\n" +
                         ".EI\n.SI 5\n" + query_func_def(stuff, funcs[i]) +
                         "\n.EI\n";
      if (stuff[AUTO_DOCS]["main"]) {
         ret += query_method_description(stuff[AUTO_DOCS]);
         write_single_description(funcs[i], stuff, base_single + funcs[i], fn);
      }
      ret += "\n";
   }
   return ret;
} /* write_methods() */

private string query_class_def(mixed *stuff, string name) {
   string ret;
   int pad;
   int i;

   ret = "class " + name + " {\n";
   pad = strlen(ret);
   for (i = 0; i < sizeof(stuff[AUTO_CLASS_MEMBERS]); i++) {
      ret += sprintf("%-" + pad + "s", "");
      ret += implode(stuff[AUTO_CLASS_MEMBERS][i][AUTO_CLASS_TYPE], " ") +
             " " + stuff[AUTO_CLASS_MEMBERS][i][AUTO_CLASS_NAME] + ";\n";
   }
   ret += "}\n";
   return ret;
} /* query_class_def() */

private string query_func_def(mixed *stuff, string func) {
   string ret;
   int i;

   ret = implode(stuff[AUTO_TYPE], " ") + " " + func+"(";
   for (i = 0; i < sizeof(stuff[AUTO_ARGS]); i += 2) {
      if (i) {
         ret += ", ";
      }
      ret += implode(stuff[AUTO_ARGS][i + AUTO_ARGS_TYPE], " ") + " " +
             stuff[AUTO_ARGS][i + AUTO_ARGS_NAME];
   }
   ret += ")";
   return ret;
} /* query_func_def() */

private string query_long_func_def(mixed *stuff, string func) {
   string ret;
   int i;
   int pad;

   ret = implode(stuff[AUTO_TYPE], " ") + " " + func+"(";
   pad = strlen(ret);
   for (i = 0; i < sizeof(stuff[AUTO_ARGS]); i += 2) {
      if (i) {
         ret += sprintf(",\n%-"+pad+"s", "");
      }
      ret += implode(stuff[AUTO_ARGS][i + AUTO_ARGS_TYPE], " ") + " " +
             stuff[AUTO_ARGS][i + AUTO_ARGS_NAME];
   }
   ret += ")";
   return ret;
} /* query_long_func_def() */

private string query_class_description(mapping docs) {
   int i;
   string name;
   string desc;
   string ret;

   ret = "";

   if (docs["main"]) {
      ret += convert_html(implode(docs["main"], " "), 7, 0, 5);
   }

   if (docs["member"]) {
      ret += ".SI 7\n%^BOLD%^Members:%^RESET%^\n.EI\n";
      for (i = 0; i < sizeof(docs["member"]); i++) {
         if (sscanf(docs["member"][i], "%s %s", name, desc) == 2) {
            ret +=  ".SP 9 5\n" + name + " - " +
                               parse_string(desc) + "\n.EP\n";
         }
      }
   }
   return ret;
} /* query_class_description() */

private string query_method_description(mapping docs) {
   int i;
   string name;
   string desc;
   string ret;

   ret = "";
   if (docs["main"]) {
      ret += convert_html(implode(docs["main"], " "), 7, 0, 5);
   }

   if (docs["param"]) {
      ret += ".SI 7\n%^BOLD%^Parameters:%^RESET%^\n.EI\n";
      for (i = 0; i < sizeof(docs["param"]); i++) {
         if (sscanf(docs["param"][i], "%s %s", name, desc) == 2) {
            ret += ".SP 9 5\n" + name + " - " +
                               parse_string(desc) + "\n.EP\n";
         }
      }
   }

   if (docs["return"]) {
      ret +=".SI 7\n%^BOLD%^Returns:%^RESET%^\n.EI\n.SP 9 5\n" +
                         parse_string(implode(docs["return"], " ")) +
                         "\n.EP\n";
   }
   return ret;
} /* query_method_description() */

private void write_single_description(string func, mixed *stuff,
                                      string output, string fn) {
   mapping fluff;
   string blue;
   int i;
   int end;
   string *bits;
   string name;
   string desc;
   string ret;

   reset_eval_cost();
   bits = explode(output, "/");
   blue = "/";
   /* Create the directory if it does not exist... */
   for (i = 0; i < sizeof(bits) - 1; i++) {
      blue += bits[i] + "/";
      if (file_size(blue) != -2) {
         mkdir(blue);
      }
   }

   fluff = stuff[AUTO_DOCS];
   ret = "";
   ret += ".DT\n" + func + "\n" + mud_name() + " creator help\n" +
                      func + "\n\n";
   ret += ".SH Name\n.SP 5 5\n";
   blue = implode(fluff["main"], "\n");
   end = strlen(blue);
   i = strsrch(blue, ".");
   if (i > 0) {
      end = i;
   }
   i = strsrch(blue, "!");
   if (i > 0 && i < end) {
      end = i;
   }
   i = strsrch(blue, "?");
   if (i > 0 && i < end) {
      end = i;
   }
   ret += "" + func + " - " + replace(blue[0..end], "\n", " ") +
                      "\n.EP\n\n";
   ret += ".SH Syntax\n.SI 5\n" + query_long_func_def(stuff, func) +
                      "\n.EI\n\n";
   if (fluff["param"]) {
      ret += ".SH Parameters\n.SI 5\n";
      for (i = 0; i < sizeof(fluff["param"]); i++) {
         if (sscanf(fluff["param"][i], "%s %s", name, desc) == 2) {
            ret += "%^BOLD%^" + name + "%^RESET%^ - " +
                               (desc) + "\n";
         }
      }
      ret += ".EI\n\n";
   }

   if (fluff["return"]) {
      ret += ".SH Returns\n.SI 5\n";
      ret += implode(fluff["return"], " ");
      ret += ".EI\n\n";
   }

   ret += ".SH Defined in\n.SI 5\n" + fn + "\n.EI\n\n";
   ret += ".SH Description\n" + convert_html(blue, 5, 0, 5) + "\n";
   if (fluff["example"]) {
      i = 1;
      foreach (blue in fluff["example"]) {
         ret += ".SH Example " + i + "\n.SI 5\n" +
                            parse_string(blue) + "\n.EI\n\n";
         i++;
      }
   }

   if (fluff["see"]) {
      ret += ".SH See also\n.SP 5 5\n" +
                         implode(fluff["see"], ", ") + "\n.EP\n\n";
   }
   write_file(output, ret);
} /* write_single_description() */

private void write_single_class_description(string func, mixed *stuff,
                                            string output, string fn) {
   mapping fluff;
   string blue;
   int i;
   int end;
   string *bits;
   string name;
   string desc;
   string ret;

   reset_eval_cost();
   bits = explode(output, "/");
   blue = "/";
   /* Create the directory if it does not exist... */
   for (i = 0; i < sizeof(bits) - 1; i++) {
      blue += bits[i] + "/";
      if (file_size(blue) != -2) {
         mkdir(blue);
      }
   }

   fluff = stuff[AUTO_DOCS];
   ret = "";
   ret += ".DT\n" + func + "\n" + mud_name() + " creator help\n" +
                      func + "\n\n";
   ret +=  ".SH Name\n.SP 5 5\n";
   blue = implode(fluff["main"], "\n");
   end = strlen(blue);
   i = strsrch(blue, ".");
   if (i > 0) {
      end = i;
   }
   i = strsrch(blue, "!");
   if (i > 0 && i < end) {
      end = i;
   }
   i = strsrch(blue, "?");
   if (i > 0 && i < end) {
      end = i;
   }
   ret += "" + func + " - " + replace(blue[0..end], "\n", " ") +
                      "\n.EP\n\n";
   ret += ".SH Definition\n.SI 5\n" + 
              query_class_def(stuff, func) +
                      "\n.EI\n\n";
   if (fluff["member"]) {
      ret += ".SH Members\n.SI 5\n";
      for (i = 0; i < sizeof(fluff["member"]); i++) {
         if (sscanf(fluff["member"][i], "%s %s", name, desc) == 2) {
            ret += "%^BOLD%^" + name + "%^RESET%^ - " +
                               (desc) + "\n";
         }
      }
      ret += ".EI\n\n";
   }


   ret += ".SH Defined in\n.SI 5\n" + fn + "\n.EI\n\n";
   ret += ".SH Description\n" + convert_html(blue, 5, 0, 5) + "\n";
   if (fluff["example"]) {
      i = 1;
      foreach (blue in fluff["example"]) {
         ret += ".SH Example " + i + "\n.SI 5\n" +
                            parse_string(blue) + "\n.EI\n\n";
         i++;
      }
   }

   if (fluff["see"]) {
      ret += ".SH See also\n.SP 5 5\n" +
                         implode(fluff["see"], ", ") + "\n.EP\n\n";
   }
   write_file(output, ret);
} /* write_single_class_description() */

/*
 * This handles all the changes needed to parse the string, like
 * emulating the accepted html tags and suchlike.
 */
private string parse_string(string str) {
   return replace(str, ({ "<p>", "\n\n", "<br>", "\n\n",
                          "<b>", "%^BOLD%^", "</b>", "%^RESET%^",
                          "<i>", "%^CYAN%^", "</i>", "%^RESET%^",
                          "<pre>", "\n\n", "</pre>", "\n\n" }));
} /* parse_string() */

/*
 * Tags for the types of list...
 */
#define LIST_NONE 0
#define LIST_DYNAMIC 1
#define LIST_ORDERED 2
#define LIST_UNORDERED 3

/*
 * Tags into the list array...
 */
#define LIST_TYPE 0
#define LIST_DEPTH 1
#define LIST_START 2
#define LIST_ADDITIONAL 3
#define LIST_NUM 4

/**
 * Converts the html tags into nroff format...
 *
 * @param text the text to convert
 * @param depth the current depth we are at
 * @param preformat is this text preformated?
 * @param edge_width the width to use as a buffer on the edge
 * @return a nroff format file string
 */
string convert_html(string text, int depth, int preformat, int edge_width) {
   string ret;
   string *bits;
   mixed *lists;
   string* before_centers;
   string start;
   string new_start;
   string additional;
   string add;
   int i;
   string tag;
   string rest;

   ret = "";
   //text = replace_string(text, "\n", " ");
   //text = replace_string(text, "\n.", "\n .");
   if (text[0] == '.') {
      text = " " + text;
   }
   bits = explode("#" + text, "<");
   bits[0] = bits[0][1..];
   start = ".SP " + depth + " " + edge_width;
   ret = start + "\n" + bits[0];
   additional = "";
   lists = ({ ({ LIST_NONE, depth, start, additional }) });
   before_centers = ({ });
   for (i = 1; i < sizeof(bits); i++) {
         if (sscanf(bits[i], "%s>%s", tag, rest) == 2) {
            new_start = start;
            add = 0;
            switch (lower_case(tag)) {
               case "p" :
                  ret += "\n\n" + additional;
                  break;
               case "br" :
                  ret += "\n.E" + start[2..2] + "\n" + start + "\n" + additional;
                  break;
               case "dl" :
                  /* Start a thingy list... */
                  lists += ({ ({ LIST_DYNAMIC, depth, start, additional }) });
                  break;
               case "/dl" :
                  if (lists[<1][LIST_TYPE] == LIST_DYNAMIC) {
                     lists = lists[0..<2];
                     new_start = lists[<1][LIST_START];
                     additional = lists[<1][LIST_ADDITIONAL];
                  }
                  break;
               case "u" :
                  ret += "_";
                  break;
               case "/u" :
                  ret += "_";
                  break;
               case "center" :
                  before_centers += ({ start });
                  new_start = ".SC";
                  break;
               case "/center" :
                  new_start = before_centers[<1];
                  before_centers = before_centers[0..<2];
                  break;
               case "ol" :
                  /* Start a thingy list... */
                  lists += ({ ({ LIST_ORDERED, depth, start, additional, 1 }) });
                  break;
               case "/ol" :
                  if (lists[<1][LIST_TYPE] == LIST_ORDERED) {
                     lists = lists[0..<2];
                     new_start = lists[<1][LIST_START];
                     additional = lists[<1][LIST_ADDITIONAL];
                  }
                  break;
               case "ul" :
                  /* Start a thingy list... */
                  lists += ({ ({ LIST_UNORDERED, depth, start, additional }) });
                  break;
               case "/ul" :
                  if (lists[<1][LIST_TYPE] == LIST_UNORDERED) {
                     lists = lists[0..<2];
                     new_start = lists[<1][LIST_START];
                     additional = lists[<1][LIST_ADDITIONAL];
                  }
                  break;
               case "dd" :
                  if (lists[<1][LIST_TYPE] == LIST_DYNAMIC) {
                     new_start = ".SP " + (lists[<1][LIST_DEPTH] + 3) + " " +
                                 edge_width;
                     depth = lists[<1][LIST_DEPTH] + 3;
                     start = start[0..3]; // Evil hack!  Don't look!      
                     additional = "";
                  }
                  break;
               case "dt" :
                  if (lists[<1][LIST_TYPE] == LIST_DYNAMIC) {
                     new_start = ".SP " + (lists[<1][LIST_DEPTH] + 0) + " " +
                                 edge_width;
                     depth = lists[<1][LIST_DEPTH] + 0;
                     start = start[0..3]; // Evil hack!  Don't look!
                     additional = "";
                  } 
                  break;
               case "li" :
                  if (lists[<1][LIST_TYPE] != LIST_DYNAMIC) {
                     switch (lists[<1][LIST_TYPE]) {
                        case LIST_ORDERED :
                           new_start = ".SO " + (lists[<1][LIST_DEPTH] + 1) +
                                 " 3 -" + (6 + lists[<1][LIST_DEPTH]);
                           add = "\n\t" + (lists[<1][LIST_NUM]++) + ". \t";
                           additional = "\t\t";
                           depth = (lists[<1][LIST_DEPTH] + 3);
                           break;
                        case LIST_UNORDERED :
                           new_start = ".SO " + (lists[<1][LIST_DEPTH] + 1) +
                                 " 2 -" + (5 + lists[<1][LIST_DEPTH]);
                           add = "\n\t*\t";
                           additional = "\t\t";
                           depth = (lists[<1][LIST_DEPTH] + 3);
                           break;
                     }
                  }
                  break;
               case "pre" :
                  preformat++;
                  break;
               case "b" :
                  ret += "%^BOLD%^";
                  break;
               case "i" :
                  ret += "%^CYAN%^";
                  break;
               case "/b" :
               case "/i" :
                  ret += "%^RESET%^";
                  break;
               case "/pre" :
                  preformat--;
                  break;
               case "code" :
               case "/code" :
                  break;
               default :
                  rest = "<" + tag + ">" + rest;
                  break;
            }
            if (new_start != start) {
               ret += "\n.E" + start[2..2] + "\n" + new_start + "\n";
               start = new_start;
            }
            if (add) {
               ret += add;
            }
            if (preformat) {
               ret += replace(rest, "\n", "\n.E" + start[2..2] + "\n" +
                                    start + "\n" + additional);
            } else {
               rest = replace(rest, "\n", " ");
               while (strlen(rest) && rest[0] == ' ') {
                  rest = rest[1..];
               }
               while (strsrch(rest, "  ") != -1) {
                  rest = replace_string(rest, "  ", " ");
               }
               ret += rest;
            }
         } else {
            ret += ">" + bits[i];
         }
   }
   ret += "\n.E" + start[2..2] + "\n";
   return ret;
} /* convert_html() */
