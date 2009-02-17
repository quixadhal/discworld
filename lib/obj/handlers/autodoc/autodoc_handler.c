/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: autodoc_handler.c,v 1.19 1999/10/26 00:57:49 pinkfish Exp $
 * 
*/
/**
 * Documentation system for removing documentation from source files.
 * <p>
 * The documentation will be created in the directory /doc/autodoc.  Extra
 * documentation should be placed into the other help directories and will
 * be accessed via the help system.  The html documentation will be placed
 * into the /www/autodoc directory.  The files in theses directories will
 * be named with the '/'s replaced with '.'s.  ie: /obj/handlers/room_handler.c
 * would be in the file /doc/autodoc/obj.handlers.room_handler.c and
 * /www/autodoc/obj.handlers.room_handler.c.html.  An index of the methods
 * and object names will be generated from the input files and linked into the
 * help system.  An alphabetical index will also be produced that is linked
 * to the html stuff.
 * <p>
 * The first comment which follows the comment format below and occurs right
 * before a function will be used as the documentation for that function.  The
 * comment must start with two '*'s to distinguish it from a normal comment.
 * A special tag at the start of one of the first comments in the file will
 * signify that it is a comment for the class.
 * <p>
 * Only comments before a real function declaration will be used, comments
 * before a predeclaration will be ignored.
 * <p>
 * Any function which is not declared as private and does not have the @ignore
 * tag in its description will be placed into the resulting documentation
 * file.  This will be done even if no comment was found.
 * <p>
 * In the text part of the comment the main section is a free form
 * documentation, it is merely a description of the whole function.  After
 * this several tags can be added to the comment which allows other things
 * to be documented.  All the text after the tag is taken as part of that
 * tags information.  The main tag should never actually be used, it
 * is implied as being the tag associated with the first section of
 * comment.
 * <p>
 * HTML tags will be used in the comments to allow certain things to
 * happen.  The tags 'p', 'b', 'i', 'code', 'ul', 'ol', 'dl', 'dd', 'dt', 'pre'
 * and 'li' will be supported.
 * <p>
 * The sequences '\\/', '\\\\' and '\<' will be processed by the comment
 * handling code so that they do not get placed into the output code.
 * <p>
 * White space is ignored...  Both html and dw nroff format files will be
 * created from the documentation maker.
 * <p>
 * The supported tags are:
 * <dl>
 * <dt>@param
 * <dd>Specify the parameter and what it does.  This should be one short
 *     sentance only.  There needs to be one of these for every
 *     parameter, the first word of the sentance is the name of the 
 *     paramater and the rest is the descrption.  There should be no full
 *     stop on the end of the sentance.
 * <dt>@member
 * <dd>This is used with classes and defines the members of the class.  The
 *     definition is the same as for the @param element above.
 * <dt>@return
 * <dd>Specify what the return value is.  This should be one short
 *     sentance only.  Theres should only be one of these in every
 *     function.  There should be no full stop on the end of the sentance.
 * <dt>@see
 *   <dd>Specify an object or function to see.  The format must be one of:
 *   <ul>
 *   <li> object_name<br><i>@see /global/new_parse</i>
 *   <li> function_name()<br><i>@see frog()</i>
 *   <li> object_name->function_name()<br><i>@see /global/new_parse->add_command()</i>
 *   <li> efun::name()<br><i>@see efun::sort_array()</i>
 *   <li> help::name<br>@see help::effects
 *   </ul>
 * <dt>@ignore
 *   <dd>Do not document this function, the whole comment is ignored.
 * <dt>@main
 *   <dd>The main documentation section.
 * <dt>@classification
 *    <dd>Used in the effects files to classifiy the effect.  So a tree
 *      of the classifications can be derived.
 * <dt>@index
 *        <dd>Adds in a new index refernece on this name.  This should be one
 *            word only.
 * <dt>@change
 *   <dd>Placed in the class documentation to reference a change list.
 *            First line after the tag is the date and the author and
 *            then follows a description of the change.
 * <dt>@example
 *   <dd>If you wish to embed an example in the code.
 * <dt>@started
 *   <dd>When the file was started.
 * <dt>@author
 *   <dd>The author of the file.
 * </dl>
 *
 * The output format will look something like the API documentation for
 * java.  If you have not seen this before, go to http://www.javasoft.com
 * and look in the JDK and then the API section.
 * <p>
 * Comment format:
 * <pre>
 * /\**
 *  *  <text>
 *  *    ..
 *  *  <text>
 *  *
 *  *\/
 * </pre>
 *
 * ie: it starts with a /\** and ends with a *\/ on a line by themselves and
 * every intervening line has a * at the start.  By using a \\ you can
 * make characters escaped.  This means they are not processed by
 * the internal system, so to make a end comment appear inside a
 * the text you go : *\\/
 *
 * @example
 * inherit "/std/object";
 *
 * /\**
 *  *   This class is a nice shade of blue and allows all other shades of
 *  *   green and yellow to mix well.
 *  * @see yellow
 *  * @see blue
 *  * @change 12/3/97 Pinkfish
 *  *    Pushed some of the yellow stuff down a hole and ate a bagel.
 *  *\/
 *
 * /\**
 *  * This method mixes the green in with our blue.
 *  * @param  green   the green object to mix.
 *  *\/
 * void mixGreen(object green) {
 *   /\* Something wild and exciting! *\/
 * } /\* mixGreen() *\/
 *
 * /\**
 *  * This method mixes the yellow in with our blue.
 *  * @param  yellow the yellow object to mix.
 *  * @return returns a TRUE or FALSE depending on if the mix succeeded.
 *  * @example
 *  *   mixYellow(12);
 *  *\/
 * int mixYellow(object yellow) {
 * } /\* mixYellow() *\/
 *
 * @index autodoc
 * @see /obj/handlers/autodoc/autodoc_file
 * @see /obj/handlers/autodoc/autodoc_nroff
 * @see /obj/handlers/autodoc/autodoc_html
 * @author Pinkfish
 * @started Tue Oct 28 13:25:09 EST 1997
 *
 */

#include <autodoc.h>

#define MAIN_FILE 0
#define INDEX_FILE 1

#define SAVE_FILE (SAVE_DIR "main_rubbish")

string *files;
int file_pos;
int something_changed;
int last_created_index;
nosave function *filters;
nosave int call_id;
nosave mapping summary_map;
nosave mixed *extra_indexes;
mapping help_map;

private void create_nroff_file(object ob, int type);
private void create_html_file(object ob, int type);
private void after_thingy(int no_index);
private void start_processing();
private void do_parse_next_file();
private void create_index();
private void add_to_index(object ob);
private void load();
private void save();
string *query_files();
int remove_file(string fname);

void create() {
   seteuid(getuid());
   filters = ({ (: create_nroff_file($1, $2) :),
                (: create_html_file($1, $2) :) });
   file_pos = 0;
   something_changed = 0;
   files = ({ });
   load();
   do_parse_next_file();
   /* For testing... */
   //create_index();
} /* create() */

private void create_nroff_file(mixed ob, int type) {
   string fname;
   mixed *stuff;
   mixed *index_stuff;
   int i;

   if (type == MAIN_FILE) {
      /* Now we create the nroff file... */
      fname = replace(ob->query_file_name(), "/", ".");
      if (fname[0] == '.') {
         fname = fname[1..];
      }
      rm(NROFF_DOC_DIR + fname);
      AUTODOC_NROFF->create_nroff_file(ob, NROFF_DOC_DIR + fname);
   } else if (type == INDEX_FILE) {
      /* Create an index file... */
      /* Divide into letters... */
      index_stuff = ob;
      stuff = map(query_files(), function (string name) {
           string *bits;

           bits = explode(name, "/");
           return ({ bits[sizeof(bits)-1], name });
        } );
      stuff = unique_array(index_stuff + stuff, (: lower_case($1[0])[0] :));
      for (i = 0; i < sizeof(stuff); i++) {
         fname = NROFF_DOC_DIR + "index_" + lower_case(stuff[i][0][0][0..0]);
         AUTODOC_NROFF->create_nroff_index_file(stuff[i], fname);
      }
   }
} /* create_nroff_file() */

private void create_html_file(mixed ob, int type) {
   string fname;
   mixed *stuff;
   mixed *index_stuff;
   int i;
   mapping chars;

   if (type == MAIN_FILE) {
      /* Now we create the nroff file... */
      fname = replace(ob->query_file_name(), "/", ".");
      if (fname[0] == '.') {
         fname = fname[1..];
      }
      fname += ".html";
      rm(HTML_DOC_DIR + fname);
      AUTODOC_HTML->create_html_file(ob, HTML_DOC_DIR + fname);
   } else if (type == INDEX_FILE) {
      /* Create an index file... */
      index_stuff = ob;
      stuff = map(query_files(), function (string name) {
           string *bits;

           bits = explode(name, "/");
           return ({ bits[sizeof(bits)-1], name, "", summary_map[name] });
        } );
      stuff = unique_array(index_stuff + stuff, (: lower_case($1[0])[0] :));
      chars = ([ ]);
      for (i = 0; i < sizeof(stuff); i++) {
         fname = "index_" + lower_case(stuff[i][0][0][0..0]) + ".html";
         rm(HTML_DOC_DIR + fname);
         AUTODOC_HTML->create_html_index_file(stuff[i], stuff[i][0][0][0..0],
                                       HTML_DOC_DIR + fname);
         reset_eval_cost();
         chars[capitalize(stuff[i][0][0][0..0])] =  fname;
      }
      /* The differnt sorts of index will be genertated in the html file */
      fname = HTML_DOC_DIR; // + "index.html";
      //rm(fname);
      AUTODOC_HTML->create_main_index(chars, fname);
   }
} /* create_html_file() */

private void after_thingy(int no_index) {
   int i;

   if (no_index) {
      printf("Finished recreating the documentation for %O\n",
              previous_object()->query_file_name(),
             previous_object());
   }

   if (previous_object()->query_changed()) {
      something_changed = 1;
   }

   if (previous_object()->query_num_failed_tries() > 1) {
      /*
       * This means that the file has been deleted...  Argh!  Auto remove
       * it and log it.
       */
      remove_file(files[file_pos - 1]);
      //log_file("AUTODOC", ctime(time()) + ": Removing: " + files[file_pos - 1] + " \n");
   } else if (previous_object()->query_changed() || no_index) {
      //log_file("AUTODOC", ctime(time()) + ": Parsing: " + files[file_pos - 1] + " \n");
      for (i = 0; i < sizeof(filters); i++) {
         call_out((: evaluate($1, $2, $3) :), 0, filters[i],
                  previous_object(), MAIN_FILE);
      }
   }

   call_out((: $1->dest_me() :), 20, previous_object());
   save();
} /* after_thingy() */

private void do_parse_next_file() {
   /* Give it a minute to do this file... */
   call_id = call_out((: start_processing() :), 60);
   if (file_pos >= sizeof(files)) {
      file_pos = 0;
   }
   new(AUTODOC_FILE)->parse_file(files[file_pos++], (: after_thingy(0) :));
} /* do_parse_next_file() */
 
private void start_processing() {
   if (sizeof(files) == 0) {
      return ;
   }

   /* Ok, now we skip onto the next file and zoom it... */
   if (last_created_index + SAVE_INDEX_DELAY < time()) {
      if (something_changed) {
         call_out( (: create_index() :), 2);
      }
      something_changed = 0;
      save();
   }

   call_id = call_out((: do_parse_next_file() :), 360);
} /* start_processing() */

private string query_short_args_def(mixed *args) {
   string ret;
   int i;

   ret = "";
   for (i = 0; i < sizeof(args); i += 2) {
      if (i != 0) {
         ret += ", ";
      }
      ret += implode(args[AUTO_ARGS_TYPE], " ");
   }
   return "(" + ret + ")";
} /* query_short_args_def() */

/*
 * This will add it into the help array stuff too..
 */
private string *process_stuff(string name,
                              string fname,
                              string fn,
                              mapping docs) {
   string blue;
   int i;
   int end;
   mixed *ret;
   mapping fluff;

   if (name != "create" && name != "setup" && name != "init") {
      if (help_map[name]) {
         help_map[name] += ({ fn });
      } else {
         help_map[name] = ({ fn });
      }
   }
   if (docs[name]) {
      if (arrayp(docs[name])) {
         /* Its a function! */
         fluff = docs[name][AUTO_DOCS];
         ret =  ({ name, fname, query_short_args_def(docs[name][AUTO_ARGS]) });
      } else {
         ret = ({ name, fname, "" });
         fluff = docs[name];
      }
   } else {
      ret = ({ name, fname, "" });
      if (mapp(docs)) {
         fluff = docs;
      } else {
         fluff = ([ ]);
      }
   }
   if (fluff["main"]) {
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
      blue = blue[0..end];
   }
   ret += ({ blue });
   if (fluff["index"]) {
      foreach (blue in fluff["index"]) {
         blue = replace(blue, ({ " ", "", "\n", "" }));
         if (help_map[blue]) {
            help_map[blue] += ({ fn });
         } else {
            help_map[blue] = ({ fn });
         }
         extra_indexes += ({ ({ blue, fname, "", ret[AUTO_INDEX_SUMMARY] }) });
      }
   }
   return ret;
} /* process_stuff() */

private void create_index() {
   int i;
   mixed *index_stuff;
   mixed *rabbit;
   string fname;
   string fn;
   string *bits;
   object parse;

   //log_file("AUTODOC", ctime(time()) + ": Created index.\n");
   /* Build up the list... */
   index_stuff = ({ });
   help_map = ([ ]);
   extra_indexes = ({ });
   summary_map = ([ ]);
   parse = clone_object(AUTODOC_FILE);
   for (i = 0; i < sizeof(files); i++) {
      /* Do not process it...  Merely load it from disk */
      parse->parse_file(files[i], 0, 1);
      fname = parse->query_file_name();
      if (fname) {
         fn = replace(fname, "/", ".");
         if (fn[0] == '.') {
            fn = fn[1..];
         }
         fn = NROFF_DOC_DIR + fn;
         bits = explode(fname[0..strlen(fname)-3], "/");
         rabbit = process_stuff(bits[sizeof(bits) - 1], "", fn,
                                parse->query_main_docs());
         if (rabbit[AUTO_INDEX_SUMMARY]) {
            summary_map[fname] = rabbit[AUTO_INDEX_SUMMARY];
         } else {
            map_delete(summary_map, fname);
         }
         /* We loaded it... */
         index_stuff += map(keys(parse->query_public_functions()),
                            (: process_stuff($1, $(fname), $(fn),
                               $(parse)->query_public_functions()) :) );
         index_stuff += map(keys(parse->query_protected_functions()),
                            (: process_stuff($1, $(fname), $(fn),
                               $(parse)->query_protected_functions()) :) );
         index_stuff += map(keys(parse->query_class_docs()),
                            (: process_stuff($1, $(fname), $(fn),
                               $(parse)->query_class_docs()) :) );
         if (sscanf(fname, "%*s.h") == 1) {
            index_stuff += map(keys(parse->query_define_docs()),
                               (: process_stuff($1, $(fname), $(fn),
                               $(parse)->query_define_docs()) :) );
         }
      }
      reset_eval_cost();
   }
   index_stuff += extra_indexes;
   extra_indexes = ({ });

   /* Don't call this out.  We would be copying huge arrays around.  Eeek. */
   for (i = 0; i < sizeof(filters); i++) {
      reset_eval_cost();
      catch(evaluate(filters[i], index_stuff, INDEX_FILE));
   }
   summary_map = ([ ]);
   last_created_index = time();
   save();
} /* create_index() */

/**
 * Recreate documentation for one fiel immediately.  This does not update
 * the index, it merely creates the base files for this file.  It does it
 * at a random delay of up to 30 seconds.
 *
 * @param fname the file name to update
 */
int recreate_documentation(string fname) {
   object ob;

   if (member_array(fname, files) != -1) {
      ob = clone_object(AUTODOC_FILE);
      ob->parse_file(fname, (: after_thingy(1) :));
      return 1;
   }
   return 0;
} /* recreate_documentation() */

/**
 * Regenerate the index files.
 */
void recreate_indexes() {
   unguarded( (: create_index() :));
} /* recreate_indexes() */

/**
 * Adds a file into the list of files to process for autodocumentation.
 * This file will be processed every time around in the autodocumentation
 * loop to check to see if it has been changed and the results will be
 * placed into the index.  It automaticly calls recreate_documentation
 * in random(60) seconds.
 *
 * @see recreate_documentation()
 *
 * @param fname the name of the file to add
 *
 * @return 1 if the add succeeds, 0 otherwise.
 */
int add_file(string fname) {
   fname = "/" + implode(explode(fname, "/") - ({ "" }), "/");
   if (member_array(fname, files) == -1) {
      if (sizeof(unguarded( (: stat($(fname)) :) ))) {
         files += ({ fname });
         save();
         if (sizeof(files) == 1) {
            start_processing();
         } else {
            call_out((: recreate_documentation($1) :), random(60), fname);
         }
         return 1;
      }
   }
   return 0;
} /* add_file() */

/**
 * Removes a file from the list of files to be processed for autodocumentation.
 *
 * @param fname the name of the file to remove
 *
 * @return 1 if the remove succeeds, 0 otherwise.
 */
int remove_file(string fname) {
   string fn;
   string dfn;
   string *our_files;
 
   if (member_array(fname, files) != -1) {
      files -= ({ fname });
      file_pos = 0;

      /* Delete the files associated with it, start with the html files. */
      fn = HTML_DOC_DIR + replace(fname, "/", ".");
      unguarded((: rm($(fn)) :));
      /* Delete the nroff files. */
      fn = NROFF_DOC_DIR + replace(fname, "/", ".");
      unguarded((: rm($(fn)) :));
      /* Delete the nroff single function files. */
      fn = NROFF_DOC_SINGLE + fname[0..<3] + "/";
      our_files = get_dir(fn);
      if (sizeof(our_files)) {
         foreach (dfn in our_files) {
            dfn =  fn + dfn;
            unguarded( (: rm($(dfn)) :) );
         }
      }
      /* Remove the directory if it exists. */
      if (file_size(fn[0..<2]) != -1) {
         unguarded((: rm($(fn[0..<2])) :));
      }

      save();
      if (!sizeof(files)) {
         remove_call_out(call_id);
      }
      return 1;
   }
   return 0;
} /* remove_file() */

private void load() {
   unguarded( (: restore_object( SAVE_FILE ) :) );
} /* load() */

private void save() {
   unguarded( (: save_object( SAVE_FILE ) :) );
} /* save() */

/**
 * Returns the list of files we are current processing.
 *
 * @return an array of strings being the file names
 */
string *query_files() { return files; }

/**
 * Returns the help mapping.  This is the mapping from function names to
 * files.  Each element in the mapping referes to an array of file
 * names which contain the function or define.
 *
 * @return a mapping of arrays of files
 */
mapping query_help_map() { return help_map; }

/**
 * This method returns the file name of the help for the specified
 * function in the specified file.
 * @param file the file name to find the help in
 * @param func the function to look for help on in the file
 * @return the full path to the help file, 0 if it does not exist
 */
string query_help_on(string file, string func) {
   if (file[0] == '/') {
      file = file[1..];
   }
   if (sscanf(file, "%*s.c") == 1) {
      file = file[0..<3];
   }
   file = NROFF_DOC_SINGLE + file + "/" + func;
   if (file_size(file) > 0) {
      return file;
   }
   return 0;
} /* query_help_on() */

/**
 * Retuirns the mapping of file names to a summary.  This is only
 * valid during the index creation cycle of the documentation generation
 * system.  It is used to create a more useful index page.
 * @return the mapping of file names to summarys
 */
mapping query_summary_map() { return summary_map; }

/**
 * This method tells us if the file is currently in the autodoc set.
 * @return 1 if the file is found, 0 if not
 */
int is_autodoc_file(string name) {
   return member_array(name, files) != -1;
} /* is_autodoc_file() */
