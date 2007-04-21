/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: autodoc_html.c,v 1.14 2000/06/29 01:08:31 pinkfish Exp $
 * $Log: autodoc_html.c,v $
 * Revision 1.14  2000/06/29 01:08:31  pinkfish
 * Make it use a config define for it's address...
 *
 * Revision 1.13  2000/05/19 22:56:04  pinkfish
 * Fix it up to point to the right directory.
 *
 * Revision 1.12  2000/04/19 18:14:02  pinkfish
 * Fix up an error that was causing the code to runtime.
 *
 * Revision 1.11  1999/10/25 23:07:02  pinkfish
 * Fix up an output issue.
 *
 * Revision 1.10  1999/06/22 02:06:01  pinkfish
 * Update to use less writes when creating a file.
 *
 * Revision 1.9  1999/05/05 00:37:26  pinkfish
 * Fix up how the class names are output.
 *
 * Revision 1.8  1999/02/10 09:15:08  pinkfish
 * Fix up to use classes.
 *
 * Revision 1.7  1998/05/13 15:29:54  pinkfish
 * Fixed up the args printing stuff.
 *
 * Revision 1.6  1998/02/23 06:12:13  pinkfish
 * Put the different index types at the top of the main generated files.
 *
 * Revision 1.5  1998/02/23 05:53:41  pinkfish
 * Fixed up problems with the Change history stuff.
 *
 * Revision 1.4  1998/02/23 05:38:13  pinkfish
 * Make a standard index file...
 *
 * Revision 1.3  1998/02/23 05:30:09  pinkfish
 * Brinke the index up into creatro, effect and main lindexes.
 * Easier to find stuff...
 *
 * Revision 1.2  1998/02/22 13:00:53  pinkfish
 * changes to make the html a bit nicer and some biug fixing.
 *
 * Revision 1.1  1998/01/06 05:04:22  ceres
 * Initial revision
 * 
*/
/**
 * This creates an html document from the generated automatic documentation.
 *
 * @see /obj/handlers/autodoc/autodoc_handler
 * @author Pinkfish
 * @started Tue Oct 28 11:39:47 EST 1997
 */

#include <autodoc.h>
#include <config.h>

private string query_func_def(mixed *stuff, string func);
private string query_methods(string *funcs, function data,
                           int prot);
private string query_method_description(mapping docs);
private string create_href(string str);
private string query_method_summary(string *funcs, object file);
private string query_defines(string *defs, mapping docs);
private string query_class_summary(mapping classes);
private string query_classes(mapping classes);
private string query_class_def(mixed *stuff, string name);
private string query_class_description(mapping docs);

void create() {
   seteuid(getuid());
} /* create() */

/**
 * Writes the data from the autodoc file object out to a file in html
 * format.
 *
 * @param file the autodoc file object
 * @param output the output file to write to
 *
 * @see /obj/handlers/autodoc/autodoc_handler
 */
void create_html_file(object file, string output) {
   int i;
   mapping stuff;
   string *pub_funcs;
   string *pro_funcs;
   string *defs;
   string *bits;
   string name;
   string bing;

   /*
    * Ok, we do the alphabetised list, first a short list with the
    * first sentance of each description, then a longer expanded list..
    */
   bits = explode(file->query_file_name(), "/");
   name = bits[sizeof(bits)-1];
   bing = "<html><head><title>";
   bing += "File " + name +
           "\n</title></head><body bgcolor=\"#ffffff\" TEXT=\"#000030\" LINK=\"#4a529c\" VLINK=\"#b57339\">";
   bing += "[ <a href=\"index.html\">Package Index</a></code> | ";
   bing += "<a href=\"index_std.html\">Mudlib Index</a></code> | ";
   bing += "<a href=\"index_eff.html\">Effect Index</a></code> ]<br>";
   stuff = file->query_main_docs();
   if (stuff["main"]) {
      bing += "<h2>File " + file->query_file_name() +
                         "</h2>";
      bing += implode(stuff["main"], " ");
      if (stuff["see"]) {
         bing += "<p><b>See also:</b><br>" +
                            query_multiple_short(
                            map(stuff["see"], (: create_href($1) :)));
      }
      if (stuff["author"]) {
         bing += "<p>Written by " +
                    query_multiple_short(stuff["author"]);
      }
      if (stuff["started"]) {
         bing += "<p>Started " +
                    query_multiple_short(stuff["started"]);
      }

      if (stuff["example"]) {
         if (sizeof(stuff["example"]) > 1) {
            for (i = 0; i < sizeof(stuff["example"]); i++) {
               bing += "<h3>Example " + (i + 1) + "</h3>\n";
               bing += "<pre>"+stuff["example"][i]+"</pre>\n";
            }
         } else {
            bing +=  "<h3>Example</h3>\n<pre>" + stuff["example"][0] +
                             "</pre>\n";
         }
      }

      if (stuff["changed"]) {
         bing += "<h3>Change history</h3>\n<dl>" +
                            implode(map(stuff["changed"], function (string str) {
                   string start;
                   string rest;

                   if (sscanf(str, "%s\n%s", start, rest) == 2) {
                      return "<dt>" + start + "<dd>" + rest;
                   }
                   return "<dd>" + str;
             } ), "\n") + "</dl>\n";
      }
   }

   if (sizeof(file->query_inherits())) {
      bing += "<hr><h2>Inherits</h2>\n";
      bing += "This class inherits the following classes " +
                         query_multiple_short(
               map(keys(file->query_inherits()), (: create_href :)));
   }

   if (sizeof(file->query_includes())) {
      bing += "<hr><h2>Includes</h2>\n";
      bing += "This class includes the following files " +
                         query_multiple_short(
               map(file->query_includes(), (: create_href :)));
   }

   if (sizeof(file->query_class_docs())) {
       bing += query_class_summary(file->query_class_docs());
   }

   /*
    * Filter out create and setup as they are not real useful functions
    * to document :)
    */
   pub_funcs = keys(file->query_public_functions());
   pro_funcs = keys(file->query_protected_functions());

   /*
    * Create a summary list of the methods at the top of the file.
    * with pointers to a more indepth explantion.
    */
   bing += query_method_summary(pub_funcs + pro_funcs, file);

   /*
    * Create the more indepth explanation.
    */
   if (sizeof(pub_funcs)) {
      bing += "<hr><h2>Public Functions</h2>\n";
      bing += "These are functions that everyone can "
                         "access.<p><dl>\n.";
      bing += query_methods(pub_funcs, (: $(file)->query_public_functions()[$1] :),
                    0);
      bing += "</dl>\n";
   }

   if (sizeof(pro_funcs)) {
      bing += "<hr><h2>Protected Functions</h2>\n";
      bing += "These are functions that only "
                         "objects inheriting the class can access.<p>\n";
      bing += query_methods(pro_funcs, (: $(file)->query_protected_functions()[$1] :),
                    1);
      bing +=  "\n";
   }

   /* If it is an include file.  Do fun stuff with defines. */
   if (sscanf(name, "%*s.h") == 1 &&
       sizeof(file->query_define_docs())) {
      defs = sort_array(keys(file->query_define_docs()),
                        (: strcasecmp($1, $2) :));
      bing +=  "<hr><h2>Defines</h2>\n";
      query_defines(defs, file->query_define_docs());
      bing = "";
   }

   if (sizeof(file->query_class_docs())) {
      bing += "<hr><h2>Classes</h2>\n";
      bing += "These are nice data types for dealing with...  Data!"
                         "<p>\n";
      bing += query_classes(file->query_class_docs());
   }

   write_file(output, bing, 1);
   /* Ignore private functions, because they are less than useful anyway. */
   bing = "/www/footer"->www_function(output);
   write_file(output, bing + "</body></html>");
} /* create_html_file() */

/**
 * Creates a html index file from the input index information.
 * This will contain all the functions starting with a certain letter
 * and the corresponding files they are mentioned in.
 *
 * @param index the values in the index
 * @param letter the letter this is an index of
 * @param output the file to write the output to
 */
void create_html_index_file(string *index, string letter, string output) {
   int i;
   string name;
   string bing;

   bing = "<html><head><title>\nIndex of " + letter +
                      "\n</title></head><body bgcolor=\"#ffffff\" TEXT=\"#000030\" LINK=\"#4a529c\" VLINK=\"#b57339\">\n";
   bing += "<img align=left src=\"" + CONFIG_EXTERNAL_WEB_ADDRESS +
                       "pics/dw4.gif\">\n"
                      "<p align=right><code><a href=\"index.html\">"
                      "Main Index</a></code></p>\n"
                      "<h2>Index of " + letter + "</h2>\n"
                      "<h3><i>Letters can change the world, remove the l "
                      "from world and you have word</i></h3>\n"
                      "<br clear=both>\n"
                      "<hr><dl>\n";
   index = sort_array(index, (: strcasecmp($1[0], $2[0]) ? strcasecmp($1[0], $2[0]) :
                                        strcasecmp($1[1], $2[1]) :) );
   for (i = 0; i < sizeof(index); i++) {
      name = replace(index[i][AUTO_INDEX_FILE_NAME], "/", ".");
      if (name[0] == '.') {
         name = name[1..];
      }
      if (sscanf(index[i][AUTO_INDEX_NAME], "%*s.%*s") == 2) {
         bing += "<dt><img src=\"images/magenta-ball-small.gif\" "
                            "height=6 width=6 alt=\" . \"><b><a href=\"" +
                            name + ".html\">" + index[i][AUTO_INDEX_NAME] +
                            "</a></b>.\n";
      } else {
         bing += "<dt><img src=\"images/magenta-ball-small.gif\" "
                            "height=6 width=6 alt=\" . \"><b><a href=\"" +
                            name + ".html#" + index[i][AUTO_INDEX_NAME] +
                            "\">" + index[i][AUTO_INDEX_NAME] + "</a></b>" +
                            index[i][AUTO_INDEX_ARGS] +
                            "; <a href=\"" + name + ".html\">" +
                            index[i][AUTO_INDEX_FILE_NAME] + "</a>\n";
      }

      if (index[i][AUTO_INDEX_SUMMARY]) {
         bing += "<dd>"+index[i][AUTO_INDEX_SUMMARY]+"\n";
      }
      reset_eval_cost();
   }
   write_file(output, bing + "</dl>", 1);
   write_file(output, "/www/footer"->www_function(output) + "</body></html>");
} /* create_html_index_file() */

// Move these out to let us use function pointers on them.
private string bing_cre;
private string bing_eff;
private string bing_index;

/**
 * Creates the basic index file which points to all the other index
 * files...  The input letters array is a paired array, the first element
 * is the letter and the second is the file name.
 *
 * @param letters the names of all the other index files
 * @param output_dir the directory to write stuff to
 */
void create_main_index(mapping chars, string output_dir) {
   int i;
   string output_index;
   string output_eff;
   string output_cre;
   //string output_file;
   string *files;
   mixed *stuff;
   string str;
   string fname;
   string *index;
   mapping summaries;
   function write_all;
   string bing_file;
   string fluff;

   summaries = AUTODOC_HANDLER->query_summary_map();
   output_index = output_dir + "index_std.html";
   output_eff = output_dir + "index_eff.html";
   output_cre = output_dir + "index_cre.html";
   bing_cre = "";
   bing_eff = "";
   bing_file = "";
   bing_index = "";
 
   /* Rm the files so we write new ones... */
   rm (output_eff);
   rm (output_index);
   rm (output_cre);

   write_all = (: bing_eff += $1,
                  bing_cre += $1,
                  bing_index += $1 :);
   evaluate(write_all, "<html><head><title>\n");
   evaluate(write_all, "" + mud_name() + " autodoc index\n</title></head>\n"
                            "<body bgcolor=\"#ffffff\" TEXT=\"#000030\" "
                             "LINK=\"#4a529c\" VLINK=\"#b57339\">\n");
   bing_index += "<img align=left src=\"" + CONFIG_EXTERNAL_WEB_ADDRESS +
                      "/pics/dw4.gif\">\n"
                      "<h2>" + mud_name() + " Documentation</h2>\n"
                      "<h3><i>Where cabbages evaporate in the sun</i></h3>\n"
                      "<br clear=both>\n";
   bing_eff += "<img align=left src=\"" + CONFIG_EXTERNAL_WEB_ADDRESS + 
                      "/pics/dw4.gif\">\n"
                      "<h2>" + mud_name() + " Effect Documentation</h2>\n"
                      "<h3><i>Where your consequences are vanquished</i></h3>\n"
                      "<br clear=both>\n";
   bing_cre += "<img align=left src=\"" + CONFIG_EXTERNAL_WEB_ADDRESS +
                      "pics/dw4.gif\">\n"
                      "<h2>" + mud_name() + " Creator Documentation</h2>\n"
                      "<h3><i>Blue fluff?  No red and scruffy!</i></h3>\n"
                      "<br clear=both>\n";

   evaluate(write_all, "<h2>Function index</h2>\n");
   files = AUTODOC_HANDLER->query_files();
   files = sort_array(files, (: strcasecmp :) );
   index = sort_array(keys(chars), (: strcasecmp :) );
   for (i = 0; i < sizeof(index); i++) {
      if (i > 0) {
         evaluate(write_all, ", ");
      }
      evaluate(write_all, "<a href=\"" + chars[index[i]] + "\">" + index[i] +
                         "</a>\n");
   }

   /* Main index */
   bing_index += ", <a href=\"index_eff.html\">Effects</a>";
   bing_index += ", <a href=\"index_cre.html\">Creator</a>";
   /* Effect index */
   bing_eff += ", <a href=\"index.html\">Main</a>";
   bing_eff += ", <a href=\"index_cre.html\">Creator</a>";
   /* Creator index */
   bing_cre += ", <a href=\"index.html\">Main</a>";
   bing_cre += ", <a href=\"index_eff.html\">Effects</a>";

   evaluate(write_all, "<hr><h2>Class index</h2>\n");
   evaluate(write_all, "<dl>\n");

   stuff = unique_array(files, (: $1[0..strsrch($1, "/", -1)] :));
   for (i = 0; i < sizeof(stuff); i++) {
      str = stuff[i][0][0..strsrch(stuff[i][0], "/", -1)];
      if (str[0..12] == "/std/effects/") {
         bing_eff += "<dt><img src=\"images/cyan-ball.gif\" height=12 "
                            "width=12><b><a href=\"#" + str + "\">"+
                            str + "</a></b>\n";
      } else if (str[0..2] == "/w/") {
         bing_cre += "<dt><img src=\"images/cyan-ball.gif\" "
                            "height=12 "
                            "width=12><b><a href=\"#" + str + "\">"+
                            str + "</a></b>\n";
      } else {
         bing_index += "<dt><img src=\"images/cyan-ball.gif\" height=12 "
                            "width=12><b><a href=\"#" + str + "\">"+
                            str + "</a></b>\n";
      }
   }
   evaluate(write_all, "</dl>\n");
   
   for (i = 0; i < sizeof(stuff); i++) {
      fluff = "";
      str = stuff[i][0][0..strsrch(stuff[i][0], "/", -1)];
      fluff += "<h2><a name=\"" + str + "\">" + str + "</a></h2>\n<dl>\n";
      reset_eval_cost();
      foreach (str in stuff[i]) {
         fname = replace(str, "/", ".")[1..];
         fluff += "<dt><img src=\"images/cyan-ball-small.gif\" "
                            "height=6 width=6><a href=\"" + fname + ".html\">" +
                            str[strsrch(str, "/", -1) + 1..] + "</a>\n";
         if (summaries[str]) {
            fluff += "<dd>" + summaries[str] + "\n";
         }
      }
      fluff += "</dl>\n";
      if (stuff[i][0][0..12] == "/std/effects/") {
         bing_eff += fluff;
      } else if (stuff[i][0][0..2] == "/w/") {
         bing_cre += fluff;
      } else {
         bing_index += fluff;
      }
   }
   //evaluate(write_all, "@@/footer:footer@@</body></html>\n");
   write_file(output_eff, bing_eff, 1);
   write_file(output_eff,
              "/www/footer"->www_function(output_eff) + "</body></html");
   write_file(output_index, bing_index, 1);
   write_file(output_index,
              "/www/footer"->www_function(output_index) + "</body></html");
   write_file(output_cre, bing_cre, 1);
   write_file(output_cre,
              "/www/footer"->www_function(output_cre) + "</body></html");
} /* create_main_index() */

private string query_classes(mapping classes) {
   string name;
   string *names;
   string bing;

   names = sort_array(keys(classes), (: strcasecmp :) );
   bing = "<dl>\n";
   foreach (name in names) {
      reset_eval_cost();
      bing += "<dt><a name=\"class_" + name + "\">\n";
      bing += "<img src=\"images/cyan-ball.gif\" width=12 "
                         "height=12 alt=\" * \">";
      bing += "<b>" + name +
              "</b></a><pre>\n" +
              query_class_def(classes[name], name) + "\n</pre>";
      if (classes[name][AUTO_CLASS_DOCS]["main"]) {
         bing += query_class_description(classes[name][AUTO_CLASS_DOCS]);
      }
      bing += "\n";
   }
   return bing + "</dl>\n";
} /* query_classes() */

private string query_methods(string *funcs, function data,
                           int prot) {
   int i;
   mixed *stuff;
   string bing;

   funcs = sort_array(funcs, (: strcasecmp($1, $2) :));
   bing = "";
   for (i = 0; i < sizeof(funcs); i++) {
      reset_eval_cost();
      stuff = evaluate(data, funcs[i]);
      bing += "<dt><a name=\"" + funcs[i] + "\">\n";
      if (prot) {
        bing += "<img src=\"images/magenta-ball.gif\" width=12 "
                           "height=12 alt=\" * \">";
      } else {
        bing += "<img src=\"images/cyan-ball.gif\" width=12 "
                           "height=12 alt=\" * \">";
      }
      bing += "<b>" + funcs[i] +
                         "</b></a><pre>\n" +
                         query_func_def(stuff, funcs[i]) +
                         "\n</pre>";
      if (stuff[AUTO_DOCS]["main"]) {
         bing += query_method_description(stuff[AUTO_DOCS]);
      }
      bing += "\n";
   }
   return bing;
} /* query_methods() */

private string query_func_def(mixed *stuff, string func) {
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
} /* query_func_def() */

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

private string query_method_description(mapping docs) {
   int i;
   string name;
   string desc;
   string bing;

   bing = "";
   if (docs["main"]) {
      bing += "<dd>" + implode(docs["main"], " ")
                         +"<p>\n";
   }

   if (docs["param"] || docs["return"] || docs["see"] || docs["example"]) {
      bing += "<dd><dl>\n";
   }

   if (docs["param"]) {
      bing += "<dt><b>Parameters:</b>\n";
      for (i = 0; i < sizeof(docs["param"]); i++) {
         if (sscanf(docs["param"][i], "%s %s", name, desc) == 2) {
            bing += "<dd>" + name + " - " + (desc) + "\n";
         }
      }
   }

   if (docs["return"]) {
      bing += "<dt><b>Returns:</b>\n<dd>" + implode(docs["return"], " ") +
                         "\n";
   }

   if (docs["see"]) {
      bing += "<dt><b>See also:</b>\n<dd>" +
                         query_multiple_short(
                         map(docs["see"], (: create_href($1) :)));
   }

   if (docs["example"]) {
      bing += "<dt><b>Example:</b>\n<dd><pre>" + 
                 implode(docs["example"], "</pre><dd><pre>\n") +
                        "</pre>";
   }

   if (docs["param"] || docs["return"] || docs["see"] || docs["example"]) {
      bing += "</dl>\n";
   }

   return bing;
} /* query_method_description() */

private string query_class_description(mapping docs) {
   int i;
   string name;
   string desc;
   string bing;

   bing = "";
   if (docs["main"]) {
      bing += "<dd>" + implode(docs["main"], " ")
                         +"<p>\n";
   }

   if (docs["member"] || docs["see"]) {
      bing += "<dl>\n";
   }

   if (docs["member"]) {
      bing += "<dt><b>Members:</b>\n";
      for (i = 0; i < sizeof(docs["member"]); i++) {
         if (sscanf(docs["member"][i], "%s %s", name, desc) == 2) {
            bing += "<dd>" + name + " - " +
                               (desc) + "\n";
         }
      }
   }

   if (docs["see"]) {
      bing += "<dt><b>See also:</b>\n<dd>" +
                         query_multiple_short(
                         map(docs["see"], (: create_href($1) :)));
   }

   if (docs["member"] || docs["see"]) {
      bing += "</dl>\n";
   }

   return bing;
} /* query_class_description() */

/*
 * This creates a reference string from the input reference...
 */
private string create_href(string str) {
   string name;
   string ref_name;
   string func;

   if (sscanf(str, "%s#%s", name, func) == 2) {
      ref_name = replace(name, "/", ".");
      if (ref_name[0] == '.') {
         ref_name = ref_name[1..];
      }
      return "<a href=\"" + ref_name + ".c.html#" + func + "\">" +
             name + "->" + func + "()</a>";
   } else if (sscanf(str, "%s->%s", name, func) == 2) {
      sscanf(func, "%s(%*s)", func);
      ref_name = replace(name, "/", ".");
      if (ref_name[0] == '.') {
         ref_name = ref_name[1..];
      }
      return "<a href=\"" + ref_name + ".c.html#" + func + "\">" + str + "</a>";
   } else if (sscanf(str, "%s()", func) == 1) {
      return "<a href=\"#" + func + "\">" + str + "</a>";
   } else if (str[0..4] == "http:") {
      return "<a href=\"" + str + "\">" + str + "</a>";
   } else {
      ref_name = replace(str, "/", ".");
      if (ref_name[0] == '.') {
         ref_name = ref_name[1..];
      }
      if (sscanf(str, "%*s.%*s") != 2) {
         ref_name += ".c";
         str += ".c";
      }
      if (member_array(str, AUTODOC_HANDLER->query_files()) != -1) {
      /* Its an object reference */
         return "<a href=\"" + ref_name + ".html\">" + str + "</a>";
      }
      return str;
   }
} /* create_href() */

private string query_short_args_def(mixed *args) {
   string ret;
   int i;

   ret = "";
   for (i = 0; i < sizeof(args); i += 2) {
      if (i != 0) {
         ret += ", ";
      }
      ret += implode(args[i + AUTO_ARGS_TYPE], " ");
   }
   return "(" + ret + ")";
} /* query_short_args_def() */

private string query_short_desc(mapping docs) {
   mixed main;
   int pos;
   int tmp_pos;

   main = docs["main"];
   if (!main) {
      return 0;
   }

   /* Look for the first sentance... */
   main = implode(main, " ");
   pos = strlen(main);
   tmp_pos = strsrch(main, ".");
   if (tmp_pos < pos && tmp_pos != -1) {
      pos = tmp_pos;
   }
   tmp_pos = strsrch(main, "?");
   if (tmp_pos < pos && tmp_pos != -1) {
      pos = tmp_pos;
   }
   tmp_pos = strsrch(main, "!");
   if (tmp_pos < pos && tmp_pos != -1) {
      pos = tmp_pos;
   }
   
   return main[0..pos];
} /* query_short_desc() */

private string query_class_summary(mapping classes) {
   string *names;
   string name;
   string short_desc;
   string bing;

   names = sort_array(keys(classes), (: strcasecmp :));
   bing = "<hr><h2>Class Index</h2>\n";
   bing += "<dl>";
   foreach (name in names) {
      bing += "<dt><img src=\"images/cyan-ball-small.gif\" "
                         "height=6 width=6 alt=\" o \">\n";
      
      bing += "<a href=\"#class_" + name + "\"><b>" + name +
                         "</b></a>\n";
      short_desc = query_short_desc(classes[name][AUTO_CLASS_DOCS]);
      if (short_desc) {
         bing += "<dd>" + short_desc + "\n";
      }
   }
   bing += "</dl>";
   return bing;
} /* query_class_summary() */

private string query_method_summary(string *funcs,
                                  object file) {
   int i;
   mixed *stuff;
   string short_desc;
   int prot;
   string bing;

   bing = "";
   if (sizeof(funcs)) {
      bing += "<hr><h2>Method index</h2>\n<dl>";
      funcs = sort_array(funcs, (: strcasecmp :) );
      for (i = 0; i < sizeof(funcs); i++) {
         if (file->query_public_functions()[funcs[i]]) {
            stuff = file->query_public_functions()[funcs[i]];
            prot = 0;
         } else if (file->query_protected_functions()[funcs[i]]) {
            stuff = file->query_protected_functions()[funcs[i]];
            prot = 1;
         }

       
         if (prot) {
           bing += "<dt><img src=\"images/magenta-ball-small.gif\" "
                              "height=6 width=6 alt=\" o \">\n";
         } else {
           bing += "<dt><img src=\"images/cyan-ball-small.gif\" "
                              "height=6 width=6 alt=\" o \">\n";
         }
         bing += "<a href=\"#" + funcs[i] + "\"><b>" + funcs[i] +
                            "</b></a>" + query_short_args_def(stuff[AUTO_ARGS])
;
         short_desc = query_short_desc(stuff[AUTO_DOCS]);
         if (short_desc) {
            bing += "<dd>" + short_desc + "\n";
         }
      }
      bing += "</dl>";
   }
   return bing;
} /* query_method_summary() */

private string query_defines(string *defs, mapping docs) {
   int i;
   string bing;

   bing = ""; 
   for (i = 0; i < sizeof(defs); i++) {
      bing +=  "<dt><a name=\"" + defs[i] + "\">\n";
      bing += "<img src=\"images/red-ball.gif\" width=12 "
                         "height=12 alt=\" * \"><b>" + defs[i] + "</b></a>\n";
      if (docs[defs[i]]["main"]) {
         bing += query_method_description(docs[defs[i]]);
      }
   }
   return bing;
} /* query_defines() */
