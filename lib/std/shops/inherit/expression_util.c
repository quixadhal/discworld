inherit "/std/basic/expressions";
#include <expressions.h>
#include <shops/craft_shop.h>
#include <nroff.h>

int is_allowed(string person);
void event_save(object ob);

private nosave string _type;

/**
 * This sets the thingy type.  It is what is put in front of the
 * control functions so many objects can set up exciting and confusing
 * controls of expressions.
 */
void set_expression_type(string type) {
   _type = type;
} /* set_expression_type() */

/**
 * This returns the expression type.
 * It is what is put in front of the
 * control functions so many objects can set up exciting and confusing
 * controls of expressions.
 * @return the expression type
 */
string query_expression_type() {
   return _type;
} /* qyery_expression_type() */

/**
 * This method allows a function to be added to the base set of user 
 * functions.
 * @param def the name and arguements
 * @param expr the expression itself
 * @return 1 on success, 0  on failure
 */
int do_user_function_add(string def, string expr) {
   string name;

if (this_player()->query_name() != "presto")
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to add functions to this store.\n");
      return 0;
   }

   //
   // Ok, try and parse the arguments and function name first.
   //
   name = parse_user_expression(def, expr);
   if (name) {
      if (strlen(query_last_expression_warning())) {
         write("WARNING!!! Any changes of types could cause expressions "
               "currently using the functions to fail.\n" +
               query_last_expression_warning());
      }
      write("You successfully add:\n" + query_user_function_string(name));
      add_succeeded_mess(({ "",
                         "$N fiddle$s with something in the shop.\n" }) );
      event_save(this_object());
      return 1;
   }

   add_failed_mess(query_last_expression_error() + "\n");
   return 0;
} /* do_user_function_add() */

/**
 * This method allows a function to be added to the base set of user 
 * functions.
 * @param def the name and arguements
 * @param expr the expression itself
 * @return 1 on success, 0  on failure
 */
int do_user_function_remove(string name) {
   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to remove functions from this store.\n");
      return 0;
   }

   //
   // Ok, try and parse the arguments and function name first.
   //
   if (query_user_function_return_type(name) != EXPRESSION_TYPE_ERROR) {
      if (remove_user_expression(name)) {
         add_succeeded_mess(({ "You remove the function " + name + ".\n",
                         "$N fiddle$s with something in the shop.\n" }) );
         return 1;
      }
   }

   add_failed_mess(query_last_expression_error() + "\n");
   return 0;
} /* do_user_function_remove() */

/**
 * This method shows the status of all the user defined functions.
 * These can be used in other exciting functions.
 */
int do_approve_status_internal_functions() {
   string* names;
   string name;
   string ret;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to see the status of this store.\n");
      return 0;
   }

   names = query_function_names();
   ret = "Functions:\n";
   foreach (name in names) {
      ret += query_type_name(query_function_type(name)) + " " + name +
             "(" + implode(map(query_function_args_types(name),
                               (: query_type_name($1) :)), ", ") +
             ")\n";
   }

   ret += "\nVariables:\n";
   names = query_variable_names();
   foreach (name in names) {
      ret += query_type_name(query_variable_type(name)) + " " + name +
             "\n";
   }
   write("$P$Internal functions$P$" + ret);
   add_succeeded_mess(({ "",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_status_functions() */

/**
 * Get help on a specific function.
 * @param name the name of the function to get help on
 */
int do_function_help(string name) {
   string nroff_fn;
   string str;
   string fname;

   fname = CRAFT_SHOP_EXPRESSION_HELP_DIR + name;
   if (file_size(fname) <= 0) {
      add_failed_mess("There is no help for the function '" + name + "'.\n");
      return 0;
   }

   nroff_fn = NROFF_DIR + replace(fname, "/", ".");
   str = NROFF_HAND->cat_file(nroff_fn, 1);
   if (!str) {
      NROFF_HAND->create_nroff(fname, nroff_fn);
      str = NROFF_HAND->cat_file(nroff_fn, 0);
   }

   write("$P$function: " + name + "$P$" + str);
   return 1;
} /* do_function_help() */

/**
 * Get help on a specific function.
 * @param name the name of the function to get help on
 */
int do_function_help_list() {
   string *names;

   names = get_dir(CRAFT_SHOP_EXPRESSION_HELP_DIR);
   if (!sizeof(names)) {
      add_failed_mess("There is no help on any functions.\n");
      return 0;
   }

   names -= ({ ".", "..", "RCS" });

   write(sprintf("%-#*s\n", this_player()->query_cols(), implode(names, "\n")));
   return 1;
} /* do_function_help_list() */

/**
 * This method shows the status of all the user defined functions.
 * These can be used in other exciting functions.
 */
int do_approve_status_functions(int hints) {
   string* names;
   string name;
   string ret;

   if (!is_allowed(this_player()->query_name())) {
      add_failed_mess("You are not allowed to see the status of this store.\n");
      return 0;
   }

   names = query_user_function_names();
   if (!sizeof(names)) {
      add_failed_mess("Could not find any user defined expressions.\n");
      return 0;
   }
   ret = "";
   foreach (name in names) {
      ret += query_user_function_string(name) + "\n";
   }
   if (hints) {
      ret += "$I$0=       Hint: add function <function> as <expression>";
      ret += "$I$0=       Hint: remove function <function>";
   }
   write("$P$User functions$P$" + ret);
   add_succeeded_mess(({ "",
                         "$N fiddle$s with something in the shop.\n" }) );
   return 1;
} /* do_approve_status_functions() */

void init() {
   if (!is_allowed(this_player()->query_name())) {
      return ;
   }

   add_command("add",
               _type + " function <string'function'> as <string'expression'>",
               (: do_user_function_add($4[0], $4[1]) :));
   add_command("remove",
               _type + " function <string'function'>",
               (: do_user_function_remove($4[0]) :));
   add_command("functions", _type + " internal",
               (: do_approve_status_internal_functions() :));
   add_command("functions", _type,
               (: do_approve_status_functions(0) :));
   add_command("functions", _type +" help list",
               (: do_function_help_list() :));
   add_command("functions", _type + " help <string'function name'>",
               (: do_function_help($4[0]) :));
   add_command("functions", _type + " hints",
               (: do_approve_status_functions(1) :));
} /* init() */
