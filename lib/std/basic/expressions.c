/**
 * This is an inheritable to handle certain sorts of expressions in stuff.
 * These expression can be setup by players (or creators) and then run
 * or evaluated.  It allows controlling the types usable by the expression
 * handlers, strings, arrays, mappings, integers and floats.
 * @author Pinkfish
 * @started Thu May  4 22:44:34 PDT 2000
 */
#include <money.h>
#include <expressions.h>

#define EXPRESSION_AND       -1
#define EXPRESSION_OR        -2
#define EXPRESSION_NOT       -3
#define EXPRESSION_FALSE     -4
#define EXPRESSION_TRUE      -5
#define EXPRESSION_GREATOR_THAN     -6
#define EXPRESSION_LESS_THAN        -7
#define EXPRESSION_EQUAL_TO         -8
#define EXPRESSION_GREATOR_OR_EQUAL -9
#define EXPRESSION_LESS_OR_EQUAL    -10
#define EXPRESSION_PLUS             -11
#define EXPRESSION_MINUS            -12
#define EXPRESSION_MULTIPLY         -13
#define EXPRESSION_DIVIDE           -14
#define EXPRESSION_IF               -15
#define EXPRESSION_NOT_EQUAL_TO     -16
#define EXPRESSION_ARRAY_DEREFERENCE -17
#define EXPRESSION_ARRAY_AGGREGATE   -18
#define EXPRESSION_AND_NEW          -19
#define EXPRESSION_OR_NEW           -20
#define EXPRESSION_IF_NEW           -21

#define EXPRESSION_TREE         0
#define EXPRESSION_PARSE_STRING 1
#define EXPRESSION_TYPE         2

#define EXPRESSION_FUNC_NAME    0
#define EXPRESSION_FUNC_NO_ARGS 1

#define EXPRESSION_FUNC_VAR_NAME 0
#define EXPRESSION_FUNC_VAR_POS  1

class variable_thing {
   int type;
   function value;
}

class func_variable_thing {
   int type;
   int arg_no;
}

class function_thing {
   int type;
   function value;
   int* args;
   int return_pos; // Only used if the return type is an array.
}

class user_function_thing {
   int type;
   class parse_node* expr;
   int* arg_types;
   string* arg_names;
}

private mixed* parse_operator(string str, string token);
string query_expression_string(class parse_node* expr, int brief);
class parse_node evaluate_expression(class parse_node* expr, mixed args ...);
void add_allowed_function(string name, int type, int* args, function value);
mixed query_property(string name);
protected int is_array_type(int type);
int sizeof_function(mixed args ...);
int filter_function(mixed args ...);

private mapping _user_functions;
private nosave mapping _func_variables;
private nosave mapping _variables;
private nosave mapping _functions;
private nosave string _error_string;
private nosave string _warning_string;

void create() {
   _variables = ([ ]);
   _functions = ([ ]);
   if (!_user_functions) {
      _user_functions = ([ ]);
   }
   _func_variables = ([ ]);
   _error_string = "no error";
   add_allowed_function("sizeof", EXPRESSION_TYPE_INTEGER,
                        ({ EXPRESSION_TYPE_ARRAY }), (: sizeof_function :) );
/*  Need to do some other evil things to support this...
   add_allowed_function("filter", EXPRESSION_TYPE_OBJECT +
                                  EXPRESSION_TYPE_ARRAY_OFFSET,
                        ({ EXPRESSION_TYPE_ARRAY_OFFSET + EXPRESSION_TYPE_OBJECT,
                           EXPRESSION_TYPE_BOOLEAN }), (: filter_function :) );
 */
} /* create() */

/**
 * This method returns the last error if there was an error in the parsing.
 * @return the last error
 */
string query_last_expression_error() {
   return _error_string;
} /* query_last_expression_error() */

/**
 * This method returns the last warning if there was an warning in the parsing.
 * @return the last warning
 */
string query_last_expression_warning() {
   return _warning_string;
} /* query_last_expression_warning() */

/**
 * This method adds in an allowed variable and specifies it's type.
 * @param name the name of the variable
 * @param type the type of the variable
 * @param value the value of the variable
 */
void add_allowed_variable(string name, int type, function value) {
   class variable_thing bing;

   bing = new(class variable_thing);
   bing->type = type;
   bing->value = value;
   _variables[name] = bing;
} /* add_allowed_variable() */

/**
 * This method adds in an allowed function and specifies the types it
 * takes.
 * @param name the name of the function
 * @param type the return type of the function
 * @param args the types of the arguements (an array)
 * @param value the function to call to get the value
 */
void add_allowed_function(string name, int type, int* args, function value) {
   class function_thing bing;

   bing = new(class function_thing);
   bing->type = type;
   bing->args = args;
   bing->value = value;
   _functions[name] = bing;
} /* add_allowed_function() */

/**
 * This method returns the type of the function variable.
 * @param name the name of the function variable to check
 * @return the type of the variable, EXPRESSION_TYPE_ERROR if there is no
 * variable
 */
int query_function_variable_type(string name) {
   if (_func_variables[name]) {
      return ((class func_variable_thing)_func_variables[name])->type;
   }
   return EXPRESSION_TYPE_ERROR;
} /* query_function_variable_type() */

/**
 * This method returns the position of the function variable.
 * @param name the name of the function variable to check
 * @return the type of the variable, EXPRESSION_TYPE_ERROR if there is no
 * variable
 */
int query_function_variable_position(string name) {
   if (_func_variables[name]) {
      return ((class func_variable_thing)_func_variables[name])->arg_no;
   }
   return EXPRESSION_TYPE_ERROR;
} /* query_function_variable_position() */

/**
 * This method returns the names of all the variables.
 * @return all the variable names
 */
string *query_variable_names() {
   return keys(_variables);
} /* query_variable_type() */

/**
 * This method returns the type of the variable.
 * @param name the name of the variable to check
 * @return the type of the variable, EXPRESSION_TYPE_ERROR if there is no
 * variable
 */
int query_variable_type(string name) {
   if (_variables[name]) {
      return ((class variable_thing)_variables[name])->type;
   }
   return EXPRESSION_TYPE_ERROR;
} /* query_variable_type() */

/**
 * This method returns the value of the variable.
 * @param name the name of the variable to find
 * @return the value of the variable, 0 if not found
 */
function query_variable_value(string name) {
   if (_variables[name]) {
      return ((class variable_thing)_variables[name])->value;
   }
   return 0;
} /* query_variable_value() */

/**
 * This method returns all the function names defined in this expression
 * inheritable.
 * @return the function names
 */
string* query_function_names() {
   return keys(_functions);
} /* query_function_names() */

/**
 * This method returns the type of the function.
 * @param name the name of the function to check
 * @return the args of the function, null array is not found
 */
int* query_function_args_types(string name) {
   if (_functions[name]) {
      return ((class function_thing)_functions[name])->args;
   }
   return ({ });
} /* query_function_args_types() */

/**
 * This method returns the type of the function.
 * @param name the name of the function to check
 * @return the type of the function, EXPRESSION_TYPE_ERROR if there is no
 * function
 */
int query_function_type(string name) {
   if (_functions[name]) {
      return ((class function_thing)_functions[name])->type;
   }
   return EXPRESSION_TYPE_ERROR;
} /* query_function_type() */

/**
 * This method returns the value of the function.
 * @param name the name of the function to find
 * @return the value of the function, 0 if not found
 */
function query_function_value(string name) {
   if (_functions[name]) {
      return ((class function_thing)_functions[name])->value;
   }
   return 0;
} /* query_function_value() */

/**
 * This method returns the list of user defined functions in the
 * inheritable.
 * @return the list of user defined functions
 */
string* query_user_function_names() {
   return keys(_user_functions);
} /* query_user_function_names() */

/**
 * This method returns the argument names types of the user defined
 * function. 
 * @param name the name of the function to lookup
 * @return information on the arguments, 0 on failure
 */
mixed* query_user_function_arg_types(string name) {
   if (_user_functions[name]) {
      return _user_functions[name]->arg_types;
   }
   return 0;
} /* query_user_function_arg_types() */

/**
 * This method returns the argument names names of the user defined
 * function. 
 * @param name the name of the function to lookup
 * @return information on the arguments, 0 on failure
 */
mixed* query_user_function_arg_names(string name) {
   if (_user_functions[name]) {
      return _user_functions[name]->arg_names;
   }
   return 0;
} /* query_user_function_arg_names() */

/**
 * This method returns the argument names and types of the user defined
 * function.  It returns an array of two elements, the first is an
 * array of names and the second is an array of types.
 * @param args the arguments to return
 * @return information on the arguments, 0 on failure
 */
mixed* query_user_function_args(string name) {
   if (_user_functions[name]) {
      return ({ _user_functions[name]->arg_names,
                _user_functions[name]->arg_types });
   }
   return 0;
} /* query_user_function_args() */

/**
 * This method returns the return type of the user defined function.
 * @return the return type, EXPRESSION_TYPE_ERROR on error
 */
int query_user_function_return_type(string name) {
   if (_user_functions[name]) {
      return ((class user_function_thing)_user_functions[name])->type;
   }
   return EXPRESSION_TYPE_ERROR;
} /* query_user_function_return_type() */

/**
 * This method returns the expression of the user defined function.
 * @param name the name of the function to look up
 * @return the expression of the user defined function
 */
class parse_node* query_user_function_expression(string name) {
   if (_user_functions[name]) {
      return _user_functions[name]->expr;
   }
   return 0;
} /* query_user_function_expression() */

/**
 * This method removes the currently defined user expression.
 * @param name the expression to remove
 */
int remove_user_expression(string name) {
   if (_user_functions[name]) {
      map_delete(_user_functions, name);
      return 1;
   }
   return 0;
} /* remove_user_expression() */

/**
 * This method returns the value of the type.
 * @param type the type to get the integer value of
 * @return the integer value of the type
 */
int query_type_value(string type) {
   switch (lower_case(type)) {
   case "integer" :
   case "int"     :
      return EXPRESSION_TYPE_INTEGER;
   case "string"  :
      return EXPRESSION_TYPE_STRING;
   case "float"   :
      return EXPRESSION_TYPE_FLOAT;
   case "boolean" :
   case "bool"    :
      return EXPRESSION_TYPE_BOOLEAN;
   case "money" :
      return EXPRESSION_TYPE_MONEY;
   case "object" :
      return EXPRESSION_TYPE_OBJECT;
   case "array" :
      return EXPRESSION_TYPE_ARRAY;
   default :
      return EXPRESSION_TYPE_ERROR;
   }
} /* query_type_value() */

/**
 * This method returns the name of the type.
 * @param type the type to get the string name of
 * @return the string name of the type
 */
string query_type_name(int type) {
   if (is_array_type(type)) {
      return "array " + query_type_name(type - EXPRESSION_TYPE_ARRAY_OFFSET);
   }
   switch (type) {
   case EXPRESSION_TYPE_INTEGER  :
      return "integer";
   case EXPRESSION_TYPE_STRING   :
      return "string";
   case EXPRESSION_TYPE_ARRAY    :
      return "array";
   case EXPRESSION_TYPE_ARRAY_NULL :
      return "array null";
   case EXPRESSION_TYPE_MAPPING  :
      return "mapping";
   case EXPRESSION_TYPE_FLOAT    :
      return "float";
   case EXPRESSION_TYPE_BOOLEAN  :
      return "boolean";
   case EXPRESSION_TYPE_MONEY  :
      return "money";
   case EXPRESSION_TYPE_OBJECT :
      return "object";
   default :
      return "error";
   }
} /* query_type_name() */

/**
 * This method returns the string value of the operator name.
 * @param operator the operator name to return
 * @return the string name of the operator
 */
string query_operator_name(int operator) {
   switch (operator) {
   case EXPRESSION_AND_NEW   :
   case EXPRESSION_AND       :
      return "and";
   case EXPRESSION_OR_NEW    :
   case EXPRESSION_OR        :
      return "or";
   case EXPRESSION_NOT       :
      return "not";
   case EXPRESSION_FALSE     :
      return "false";
   case EXPRESSION_TRUE      :
      return "true";
   case EXPRESSION_GREATOR_THAN     :
      return ">";
   case EXPRESSION_LESS_THAN        :
      return "<";
   case EXPRESSION_EQUAL_TO         :
      return "=";
   case EXPRESSION_NOT_EQUAL_TO         :
      return "<>";
   case EXPRESSION_GREATOR_OR_EQUAL :
      return ">=";
   case EXPRESSION_LESS_OR_EQUAL    :
      return "<=";
   case EXPRESSION_PLUS             :
      return "+";
   case EXPRESSION_MINUS            :
      return "-";
   case EXPRESSION_MULTIPLY         :
      return "*";
   case EXPRESSION_DIVIDE           :
      return "/";
   case EXPRESSION_IF               :
      return "if";
   default :
      return "unknown";
   }
} /* query_operator_name() */

private class parse_node make_node(int type, mixed value, int* tree) {
   class parse_node bing;

   bing = new(class parse_node);
   bing->type = type;
   bing->value = value;
   bing->tree = tree;
   return bing;
} /* make_parse_node() */

/**
 * This method returns a null object of the specified type.
 * @param type the type to get the null object of
 * @return the null object
 */
class parse_node query_null_type(int type) {
   if (is_array_type(type)) {
      return make_node(type, ({ }), ({ }));
   }
   switch (type) {
   case EXPRESSION_TYPE_INTEGER  :
      return make_node(type, 0, ({ }));
   case EXPRESSION_TYPE_MONEY  :
      return make_node(type, 0, ({ }));
   case EXPRESSION_TYPE_STRING   :
      return make_node(type, "", ({ }));
   case EXPRESSION_TYPE_ARRAY    :
   case EXPRESSION_TYPE_ARRAY_NULL  :
      return make_node(type, ({ }), ({ }));
   case EXPRESSION_TYPE_MAPPING  :
      return make_node(type, ([ ]), ({ }));
   case EXPRESSION_TYPE_FLOAT    :
      return make_node(type, 0.0, ({ }));
   case EXPRESSION_TYPE_BOOLEAN  :
      return make_node(EXPRESSION_TYPE_OPERATOR, EXPRESSION_FALSE, ({ }));
   case EXPRESSION_TYPE_OBJECT :
      return make_node(type, 0, ({ }));
   default :
      return 0;
   }
} /* query_null_type() */

/**
 * This method checks to see if the passed in character is an alpha
 * or not.
 * @param alpha the character to check
 * @return 1 if it alpha, 0 if not
 */
protected int is_alpha(int alpha) {
   if ((alpha >= 'a' && alpha <= 'z') ||
       (alpha >= 'A' && alpha <= 'Z')) {
      return 1;
   }
   return 0;
} /* isalpha() */

/**
 * This method checks to see if the passed in character is a number
 * or not.
 * @param number the character to check
 * @return 1 if it is a number, 0 if not
 */
protected int is_number(int number) {
   if (number >= '0' && number <= '9') {
      return 1;
   }
   return 0;
} /* is_number() */

/**
 * This method checks to see if the passed in character is a space or
 * a space equivilant.
 * @param space the character to check
 * @return 1 if it is a space, 0 if not
 */
protected int is_space(int space) {
   if (space == ' ' || space == '\t') {
      return 1;
   }
   return 0;
} /* is_space() */

/**
 * This checks to make sure that the type is a number based type.
 * This can be controlled to make sure that any added types are also
 * allowed to be treated as numbers.
 * @param type the type to check
 * @return 1 if it is a number, 0 if not
 */
protected int is_number_type(int type) {
   return type == EXPRESSION_TYPE_INTEGER ||
          type == EXPRESSION_TYPE_FLOAT ||
          type == EXPRESSION_TYPE_MONEY;
} /* is_number_type() */

/**
 * This checks to make sure that the type is an array.
 * @param type the type to check
 * @return 1 if it is an array, 0 if not
 */
protected int is_array_type(int type) {
   return type == EXPRESSION_TYPE_ARRAY ||
          type == EXPRESSION_TYPE_ARRAY_NULL ||
          type >= EXPRESSION_TYPE_ARRAY_OFFSET;
} /* is_number_type() */

/**
 * This checks to make sure that the type is a null array.
 * @param type the type to check
 * @return 1 if it is a null array, 0 if not
 */
protected int is_null_array_type(int type) {
   return type == EXPRESSION_TYPE_ARRAY_NULL;
} /* is_number_type() */

private int is_valid_variable_name(int type) {
   return type == '_' ||
          is_alpha(type) ||
          is_number(type);
} /* is_valid_variable_name() */

/**
 * This method find the next token.  It can be overrideen in higher things
 * to deal with special token types (ie: money).
 * @param str the input string
 * @return ({ token, rest })
 */
protected string* query_token(string str) {
   int i = 0;
   int j;

   while (strlen(str) > 1 && is_space(str[0])) {
      str = str[1..];
   }

   if (!strlen(str)) {
      return ({ "", str });
   }

   //
   // parse money as money...  Make them enclose money inside two $'s.
   // ie: $4 dollars$
   //
   if (str[0] == '$') {
      i = 1;
      while (strlen(str) > i &&
             (str[i] != '$')) {
             
         i++;
      }
      return ({ str[0..i], str[i+1..] });
   }

   if (str[0] == '-' || is_number(str[0])) {
      i = 0;
      while (strlen(str) > i + 1 &&
             is_number(str[i + 1])) {
         i++;
      }

      return ({ str[0..i], str[i+1..] });
   }

   if (is_alpha(str[0]) || str[0] == '_') {
      //
      // Zip through till we find a non-alpha, non-number.
      //
      while (strlen(str) > i + 1 &&
             is_valid_variable_name(str[i + 1])) {
         i++;
      }
      return ({ str[0..i], str[i+1..] });
   }

   if (str[0] == '"') {
      do {
         j = strsrch(str[i + 1..], "\"");
         if (j == -1) {
            _error_string = "Missing close \"\n";
            return 0;
         }
         i += j + 1;
      } while (str[i - 1] == '\\');
      return ({ str[0..i], str[i+1..] });
   }

   if (str[0] == '\'') {
      do {
         j = strsrch(str[i + 1..], "'");
         if (j == -1) {
            _error_string = "Missing close '\n";
            return 0;
         }
         i += j + 1;
      } while (str[i - 1] == '\\');
      return ({ str[0..i], str[i+1..] });
   }

   if (str[0] == '>' ||
       str[0] == '<') {
      if (strlen(str) > 1 &&
          str[1] == '=') {
         return ({ str[0..1], str[2..] });
      }
      return ({ str[0..0], str[1..] });
   }

   while (strlen(str) > i + 1 &&
          !is_alpha(str[i + 1]) &&
          !is_number(str[i + 1]) && 
          !is_space(str[i + 1]) &&
          member_array(str[i + 1], ({ '>', '<', '=', '!', '-', '$', '\'',
                                      '"', '_', '(', ')', '{', '}', ',' })) == -1)
   {
      i++;
   }

   if (i) {
      return ({ str[0..i], str[i+1..] });
   }

   return ({ str[0..0], str[1..] });
} /* query_token() */

private mixed* parse_node(string str, string token) {
   class parse_node num;
   int type;
   int bing;
   int pos;
   string place;
   string fname;
   string* token_ret;
   mixed *stuff;
   mixed *args;

   while (strlen(str) &&
          str[0] == ' ') {
      str = str[1..];
   }

   if (_functions[token]) {
      fname = token;
      token_ret = query_token(str);
      if (!token_ret) {
         return 0;
      }
      if (token_ret[0] != "(") {
         _error_string = "Expected (, got " + token_ret[0] + " rest: " +
                     token_ret[1];
         return 0;
      }
      str = token_ret[1];

      //
      // Get all the args...
      //
      args = ({ });
      token_ret = query_token(str);
      if (!token_ret) {
         return 0;
      }
      if (token_ret[0] != ")") {
         pos = 0;
         do {
            stuff = parse_operator(token_ret[1], token_ret[0]);
            if (!stuff) {
               return 0;
            }
            pos++;
            if (sizeof(_functions[fname]->args) < pos) {
               _error_string = "To many arguments to " + token + " expected " +
                              sizeof(_functions[fname]->args) + " got " +
                              pos;
               return 0;
            }
            //
            // We allow array to be a type to an internal function.
            // This means it works on all arrays.
            //
            if (_functions[fname]->args[pos - 1] != stuff[EXPRESSION_TYPE] &&
                !(_functions[fname]->args[pos - 1] == EXPRESSION_TYPE_ARRAY &&
                  is_array_type(stuff[EXPRESSION_TYPE])) &&
                !(is_array_type(stuff[EXPRESSION_TYPE]) &&
                  is_null_array_type(stuff[EXPRESSION_TYPE]))) {
               _error_string = "Expected arg " + pos + " to be " +
                        query_type_name(_functions[fname]->args[pos - 1]) +
                        " not " + query_type_name(stuff[EXPRESSION_TYPE]);
               return 0;
            }
            str = stuff[EXPRESSION_PARSE_STRING];
            args += stuff[EXPRESSION_TREE];
            token_ret = query_token(str);
            if (!token_ret) {
               return 0;
            }
            if (token_ret[0] == ",") {
               token_ret = query_token(token_ret[1]);
            }
         } while (token_ret[0] != ")");
         if (token_ret[0] != ")") {
            _error_string = "Expected ')' got " + token_ret[0];
            return 0;
         }
         if (pos != sizeof(_functions[fname]->args)) {
            _error_string = "To few arguments to " + token + " expected " +
                           sizeof(_functions[fname]->args);
            return 0;
         }
         // Jumps are pointless for functions.  We cannot skip anything anyway.
         return ({ args +
                  ({ make_node(EXPRESSION_TYPE_FUNCTION,
                               ({ fname, pos }), ({ })) }),
                  token_ret[1],
                  ((class function_thing)_functions[fname])->type });
      }
   } else if (_user_functions[token]) {
      fname = token;
      token_ret = query_token(str);
      if (!token_ret) {
         return 0;
      }
      if (token_ret[0] != "(") {
         _error_string = "Expected (, got " + token_ret[0] + " rest: " +
                     token_ret[1];
         return 0;
      }
      str = token_ret[1];

      //
      // Get all the args...
      //
      args = ({ });
      token_ret = query_token(str);
      if (!token_ret) {
         return 0;
      }
      if (token_ret[0] != ")") {
         do {
            stuff = parse_operator(token_ret[1], token_ret[0]);
            if (!stuff) {
               return 0;
            }
            pos = sizeof(args);
            if (sizeof(_user_functions[fname]->arg_types) <= pos) {
               _error_string = "To many arguments to " + token + " expected " +
                              sizeof(_user_functions[fname]->arg_types);
               return 0;
            }
            // Allow null arrays to pass as any array type.
            if (_user_functions[fname]->arg_types[pos] != stuff[EXPRESSION_TYPE] &&
                !(is_array_type(stuff[EXPRESSION_TYPE]) &&
                  is_null_array_type(stuff[EXPRESSION_TYPE]))) {
               _error_string = "Expected arg " + (pos + 1) + " to be " +
                        query_type_name(_user_functions[fname]->arg_types[pos]) +
                        " not " + query_type_name(stuff[EXPRESSION_TYPE]);
               return 0;
            }
            str = stuff[EXPRESSION_PARSE_STRING];
            args += stuff[EXPRESSION_TREE];
            token_ret = query_token(str);
            if (!token_ret) {
               return 0;
            }
            if (token_ret[0] != "," && token_ret[0] != ")") {
               _error_string = "Expected ')' or ',' got " + token_ret[0];
               return 0;
            }
            if (token_ret[0] == ",") {
               token_ret = query_token(token_ret[1]);
            }
         } while (token_ret[0] != ")");
         if (token_ret[0] != ")") {
            _error_string = "Expected ')' got " + token_ret[0];
            return 0;
         }
         if (sizeof(args) < sizeof(_user_functions[fname]->arg_types)) {
            _error_string = "To few arguments to " + token + " expected " +
                           sizeof(_user_functions[fname]->arg_types);
            return 0;
         }
         return ({ args +
                  ({ make_node(EXPRESSION_TYPE_USER_FUNCTION,
                               ({ fname, sizeof(args) }), ({ })) }),
                  token_ret[1],
                  ((class user_function_thing)_user_functions[fname])->type });
      }
   } else switch (token) {
   case "true" :
      num = make_node(EXPRESSION_TYPE_OPERATOR, EXPRESSION_TRUE, ({ }));
      type = EXPRESSION_TYPE_BOOLEAN;
      break;
   case "false" :
      num = make_node(EXPRESSION_TYPE_OPERATOR, EXPRESSION_FALSE, ({ }));
      type = EXPRESSION_TYPE_BOOLEAN;
      break;
   case "{" :
      // Array aggregation.
      token_ret = query_token(str);
      args = ({ });
      type = EXPRESSION_TYPE_ARRAY;
      pos = 0;
      while (token_ret[0] != "}") {
         stuff = parse_operator(token_ret[1], token_ret[0]);
         if (!stuff) {
            return 0;
         }
         if (sizeof(args)) {
            if (type != stuff[EXPRESSION_TYPE]) {
               _error_string = "Arrays must be of only one type.  " +
                      query_type_name(type) + " and " +
                      query_type_name(stuff[EXPRESSION_TYPE]) +
                      " do not match.";
               return 0;
            }
         } else {
            type = stuff[EXPRESSION_TYPE];
            if (is_array_type(type)) {
               _error_string = "Cannot have nested arrays.";
               return 0;
            }
         }
         str = stuff[EXPRESSION_PARSE_STRING];
         args += stuff[EXPRESSION_TREE];
         token_ret = query_token(str);
         if (!token_ret) {
            return 0;
         }
         if (token_ret[0] != "," && token_ret[0] != "}") {
            _error_string = "Expected ')' or ',' got " + token_ret[0];
            return 0;
         }
         if (token_ret[0] == ",") {
            token_ret = query_token(token_ret[1]);
         }
         pos++;
      }

      //token_ret = query_token(token_ret[1]);
      if (type != EXPRESSION_TYPE_ARRAY) {
         type += EXPRESSION_TYPE_ARRAY_OFFSET;
         return ({ args +
                  ({ make_node(EXPRESSION_TYPE_INTEGER, pos, ({ })),
                     make_node(EXPRESSION_TYPE_OPERATOR,
                               EXPRESSION_ARRAY_AGGREGATE, ({ })) }),
                  token_ret[1],
                  type });
      } else {
         type = EXPRESSION_TYPE_ARRAY_NULL;
         num = make_node(EXPRESSION_TYPE_ARRAY_NULL, 0, ({ }));
         str = token_ret[1];
      }
      break;
   case "" :
      _error_string = "No token found at: " + token + " " + str;
      return 0;
   default :
      if (token[0] == '-' || is_number(token[0])) {
         sscanf(token, "%d", bing);
         num = make_node(EXPRESSION_TYPE_INTEGER, bing, ({ }));
         type = EXPRESSION_TYPE_INTEGER;
      } else if (token == "$0$") {
         num = make_node(EXPRESSION_TYPE_MONEY, 0, ({ }));
         type = EXPRESSION_TYPE_MONEY;
      } else if (token[0] == '$') {
         place = query_property("place");
         if (!place) {
            place = "default";
         }
         bing = MONEY_HAND->value_from_string(token[1..<2], place);
         if (!bing) {
            bing = MONEY_HAND->value_from_string(token[0..<2], place);
         }
         if (bing) {
            num = make_node(EXPRESSION_TYPE_MONEY, bing, ({ }));
            type = EXPRESSION_TYPE_MONEY;
         } else {
            _error_string = "Money value is invalid: " + token[1..<2] + " and " +
                            token[0..<2];
            return 0;
         }
      } else if (token[0] == '\"' || token[0] == '\'') {
         num = make_node(EXPRESSION_TYPE_STRING, token[1..<2], ({ }));
         type = EXPRESSION_TYPE_STRING;
      } else {
         type = query_variable_type(token);
         if (type == EXPRESSION_TYPE_ERROR) {
            type = query_function_variable_type(token);
            if (type == EXPRESSION_TYPE_ERROR) {
               _error_string = "No variable called '" + token + "' rest: '" +
                               str;
               return 0;
            }
            pos = query_function_variable_position(token);
            num = make_node(EXPRESSION_TYPE_FUNCTION_VARIABLE,
                            ({ token, pos }), ({ }));
         } else {
            num = make_node(EXPRESSION_TYPE_VARIABLE, token, ({ }));
         }
      }
      break;
   }
   return ({ ({ num }), str, type });
} /* parse_node() */

private mixed* parse_bracket(string str, string token) {
   mixed* stuff;
   string* token_ret;
   mixed* ret;

   switch (token) {
   case "(" :
      token_ret = query_token(str);
      if (!token_ret) {
         return 0;
      }
      stuff = parse_operator(token_ret[1], token_ret[0]);
      if (stuff) {
         str = stuff[EXPRESSION_PARSE_STRING];
         token_ret = query_token(str);
         if (!token_ret) {
            return 0;
         }
         if (token_ret[0] !=  ")") {
            _error_string = "Could not find closing bracket at " + str;
            return 0;
         }
         str = token_ret[1];
         ret = ({ stuff[EXPRESSION_TREE], str,
                  stuff[EXPRESSION_TYPE] });
      } else {
         return 0;
      }
      break;
   default :
      stuff = parse_node(str, token);
      if (stuff) {
         ret = ({ stuff[EXPRESSION_TREE],
                   stuff[EXPRESSION_PARSE_STRING],
                   stuff[EXPRESSION_TYPE] });
      } else {
         return 0;
      }
      break;
   }

   //
   // Check for a array de-reference.
   //
   str = ret[EXPRESSION_PARSE_STRING];
   token_ret = query_token(str);
   if (token_ret && token_ret[0] == "[") {
      mixed *stuffy;

      if (!is_array_type(ret[EXPRESSION_TYPE])) {
         _error_string = "Cannot de-reference " +
                         query_type_name(stuff[EXPRESSION_TYPE]);
         return 0;
      }
      if (is_null_array_type(ret[EXPRESSION_TYPE])) {
         _error_string = "Cannot de-reference a null array, it has no "
                         "members!";
         return 0;
      }
      //
      // Get the index.
      //
      token_ret = query_token(token_ret[1]);
      stuffy = parse_operator(token_ret[1], token_ret[0]);
      if (!stuffy) {
         return 0;
      }
      if (stuffy[EXPRESSION_TYPE] != EXPRESSION_TYPE_INTEGER) {
         _error_string = "Can only dereference an array with an integer "
                         "not a " +
                         query_type_name(stuff[EXPRESSION_TYPE]);
         return 0;
      }
      str = stuffy[EXPRESSION_PARSE_STRING];
      token_ret = query_token(str);
      if (token_ret[0] != "]") {
         _error_string = "Could not find closing square bracket at " + str;
         return 0;
      }
      return ({ ret[EXPRESSION_TREE] + stuffy[EXPRESSION_TREE] +
             ({ make_node(EXPRESSION_TYPE_OPERATOR, EXPRESSION_ARRAY_DEREFERENCE, ({ })) }),
                token_ret[1],
                ret[EXPRESSION_TYPE] - EXPRESSION_TYPE_ARRAY_OFFSET });
   }
   return ret;
} /* parse_bracket() */

private mixed* parse_plus(string str, string token) {
   mixed *stuff;
   mixed *stuff2;
   int type;
   string* token_ret;
   string blue;

   stuff = parse_bracket(str, token);
   if (!stuff) {
      return 0;
   }

   str = stuff[EXPRESSION_PARSE_STRING];

   token_ret = query_token(str);
   if (!token_ret) {
      return 0;
   }
 
   switch (token_ret[0]) {
   case "+" :
      type = EXPRESSION_PLUS;
      break;
   case "-" :
      type = EXPRESSION_MINUS;
      break;
   }
   blue = token_ret[0];
   if (type) {
      token_ret = query_token(token_ret[1]);
      if (!token_ret) {
         return 0;
      }
      stuff2 = parse_plus(token_ret[1], token_ret[0]);
      if (!stuff2) {
         return 0;
      }
      if ((!is_number_type(stuff[EXPRESSION_TYPE]) ||
           !is_number_type(stuff2[EXPRESSION_TYPE])) &&
          type != EXPRESSION_PLUS) {
         _error_string = "Invalid types to " + blue +
                     " expected number got: " +
                     query_type_name(stuff[EXPRESSION_TYPE]) + " and " +
                     query_type_name(stuff2[EXPRESSION_TYPE]);
         return 0;
      }
      return ({ stuff[EXPRESSION_TREE] + stuff2[EXPRESSION_TREE] +
                ({ make_node(EXPRESSION_TYPE_OPERATOR, type, ({ })) }),
                stuff2[EXPRESSION_PARSE_STRING], stuff[EXPRESSION_TYPE] });
   } else {
      return stuff;
   }
} /* parse_plus() */

private mixed* parse_multiply(string str, string token) {
   mixed *stuff;
   mixed *stuff2;
   string* token_ret;
   int type;
   string blue;

   stuff = parse_plus(str, token);
   if (!stuff) {
      return 0;
   }

   str = stuff[EXPRESSION_PARSE_STRING];

   token_ret = query_token(str);   
   if (!token_ret) {
      return 0;
   }

   switch (token_ret[0]) {
   case "*" :
      type = EXPRESSION_MULTIPLY;
      break;
   case "/" :
      type = EXPRESSION_DIVIDE;
      break;
   }
   blue = token_ret[0];
   if (type) {
      token_ret = query_token(token_ret[1]);
      if (!token_ret) {
         return 0;
      }
      stuff2 = parse_multiply(token_ret[1], token_ret[0]);
      if (!stuff2) {
         return 0;
      }
      if (!is_number_type(stuff[EXPRESSION_TYPE]) ||
           !is_number_type(stuff2[EXPRESSION_TYPE])) {
         _error_string = "Invalid types to " + blue +
                     " expected number got: " +
                     query_type_name(stuff[EXPRESSION_TYPE]) + " and " +
                     query_type_name(stuff2[EXPRESSION_TYPE]);
         return 0;
      }
      return ({ stuff[EXPRESSION_TREE] + stuff2[EXPRESSION_TREE] +
                ({ make_node(EXPRESSION_TYPE_OPERATOR, type, ({ })) }),
                stuff2[EXPRESSION_PARSE_STRING], stuff[EXPRESSION_TYPE] });
   } else {
      return stuff;
   }
} /* parse_multiply() */

private mixed* parse_compare(string str, string token) {
   mixed *stuff;
   mixed *stuff2;
   string* token_ret;
   int type;
   string blue;

   stuff = parse_multiply(str, token);
   if (!stuff) {
      return 0;
   }

   str = stuff[EXPRESSION_PARSE_STRING];

   token_ret = query_token(str);   
   if (!token_ret) {
      return 0;
   }

   switch (token_ret[0]) {
   case ">=" :
      type = EXPRESSION_GREATOR_OR_EQUAL;
      break;
   case "<=" :
      type = EXPRESSION_LESS_OR_EQUAL;
      break;
   case ">" :
      type = EXPRESSION_GREATOR_THAN;
      break;
   case "<" :
      type = EXPRESSION_LESS_THAN;
      break;
   case "<>" :
   case "!=" :
      type = EXPRESSION_NOT_EQUAL_TO;
      break;
   case "==" :
   case "=" :
      type = EXPRESSION_EQUAL_TO;
      break;
   }
   blue = token_ret[0];
   if (type) {
      token_ret = query_token(token_ret[1]);
      if (!token_ret) {
         return 0;
      }
      stuff2 = parse_compare(token_ret[1], token_ret[0]);
      if (!stuff2) {
         return 0;
      }
      if ((!is_number_type(stuff[EXPRESSION_TYPE]) ||
           !is_number_type(stuff2[EXPRESSION_TYPE])) &&
          type != EXPRESSION_EQUAL_TO &&
          type != EXPRESSION_NOT_EQUAL_TO)
      {
         _error_string = "Invalid types to " + blue +
                     " expected number got: " +
                     query_type_name(stuff[EXPRESSION_TYPE]) + " and " +
                     query_type_name(stuff2[EXPRESSION_TYPE]);
         return 0;
      }
      return ({ stuff[EXPRESSION_TREE] + stuff2[EXPRESSION_TREE] +
                ({ make_node(EXPRESSION_TYPE_OPERATOR, type, ({ })) }),
                stuff2[EXPRESSION_PARSE_STRING],
                EXPRESSION_TYPE_BOOLEAN });
   } else {
      return stuff;
   }
} /* parse_compare() */

private mixed* parse_not(string str, string token) {
   mixed *stuff;

   if (token == "not") {
      stuff = query_token(str);
      if (!stuff) {
         return 0;
      }
      stuff = parse_not(stuff[1], stuff[0]);
      if (!stuff) {
         return 0;
      }
      if (stuff[EXPRESSION_TYPE] != EXPRESSION_TYPE_BOOLEAN) {
         _error_string = "Invalid type to not expected boolean got: " +
                     query_type_name(stuff[EXPRESSION_TYPE]);
         return 0;
      }
      return ({ stuff[EXPRESSION_TREE] +
                ({ make_node(EXPRESSION_TYPE_OPERATOR, EXPRESSION_NOT, ({ }))  }),
                stuff[EXPRESSION_PARSE_STRING],
                EXPRESSION_TYPE_BOOLEAN });
   } else {
      return parse_compare(str, token);
   }
} /* parse_not() */

private mixed* parse_boolean(string str, string token) {
   mixed *stuff;
   mixed *stuff2;
   string* token_ret;
   string blue;
   int type;

   stuff = parse_not(str, token);
   if (!stuff) {
      return 0;
   }

   str = stuff[EXPRESSION_PARSE_STRING];

   token_ret = query_token(str);
   if (!token_ret) {
      return 0;
   }
   switch (token_ret[0]) {
   case "and" :
      type = EXPRESSION_AND_NEW;
      break;
   case "or" :
      type = EXPRESSION_OR_NEW;
      break;
   }

   blue = token_ret[0];

   if (type) {
      token_ret = query_token(token_ret[1]);
      if (!token_ret) {
         return 0;
      }
      stuff2 = parse_boolean(token_ret[1], token_ret[0]);
      if (!stuff2) {
         return 0;
      }
      if (stuff[EXPRESSION_TYPE] != stuff2[EXPRESSION_TYPE] ||
          stuff[EXPRESSION_TYPE] != EXPRESSION_TYPE_BOOLEAN) {
         _error_string = "Invalid types to " + blue +
                     " expected boolean got: " +
                     query_type_name(stuff[EXPRESSION_TYPE]) + " and " +
                     query_type_name(stuff2[EXPRESSION_TYPE]);
         return 0;
      }
      return ({ ({ make_node(EXPRESSION_TYPE_OPERATOR, type,
                     ({ stuff[EXPRESSION_TREE], stuff2[EXPRESSION_TREE] }) ) }),
                stuff2[EXPRESSION_PARSE_STRING],
                stuff2[EXPRESSION_TYPE] });
   } else {
      return stuff;
   }
} /* parse_boolean() */

private mixed* parse_operator(string str, string token) {
   mixed *stuff;
   mixed *stuff2;
   mixed *stuff3;
   string* token_ret;

      switch (token) {
      case "if" :
         token_ret = query_token(str);
         if (!token_ret) {
            return 0;
         }
         stuff = parse_operator(token_ret[1], token_ret[0]);
         if (!stuff) {
            return 0;
         }

         if (stuff[EXPRESSION_TYPE] != EXPRESSION_TYPE_BOOLEAN) {
            _error_string = "Invalid type to if expected boolean got: " +
                        query_type_name(stuff[EXPRESSION_TYPE]);
            return 0;
         }
         token_ret = query_token(stuff[EXPRESSION_PARSE_STRING]);
         if (!token_ret) {
            return 0;
         }
         if (token_ret[0] != "then") {
            _error_string = "Expected 'then' got " + token_ret[0];
            return 0;
         }

         token_ret = query_token(token_ret[1]);
         if (!token_ret) {
            return 0;
         }
         stuff2 = parse_operator(token_ret[1], token_ret[0]);
         if (!stuff2) {
            return 0;
         }
         token_ret = query_token(stuff2[EXPRESSION_PARSE_STRING]);
         if (!token_ret) {
            return 0;
         }
         if (token_ret[0] == "else") {
            token_ret = query_token(token_ret[1]);
            if (!token_ret) {
               return 0;
            }
            stuff3 = parse_operator(token_ret[1], token_ret[0]);
            if (!stuff3) {
               return 0;
            }
            if (stuff2[EXPRESSION_TYPE] != stuff3[EXPRESSION_TYPE]) {
               // The types are equivilant if one of them is null.
               if (is_array_type(stuff2[EXPRESSION_TYPE]) &&
                   is_array_type(stuff3[EXPRESSION_TYPE]) &&
                   (is_null_array_type(stuff2[EXPRESSION_TYPE]) ||
                    is_null_array_type(stuff3[EXPRESSION_TYPE]))) {
                  //
                  // Move it in here to fix up the type stuff in the
                  // return information.
                  //
                  if (is_null_array_type(stuff2[EXPRESSION_TYPE])) {
                     stuff2[EXPRESSION_TYPE] = stuff3[EXPRESSION_TYPE];
                  }
               } else {
                  _error_string = "Types on both sides of an if must match: " +
                           query_type_name(stuff2[EXPRESSION_TYPE]) + " and " +
                           query_type_name(stuff3[EXPRESSION_TYPE]);
                  return 0;
               }
            }
            token_ret = query_token(stuff3[EXPRESSION_PARSE_STRING]);
            if (!token_ret) {
               return 0;
            }
         } else {
            stuff3 = ({ ({ query_null_type(stuff2[EXPRESSION_TYPE]) }) });
         }
         if (token_ret[0] != "endif") {
            _error_string = "Expected 'endif' got " + token_ret[0] + " (" +
                            token_ret[1];
            return 0;
         }
         // We always evaluate the expression, so leave that on the stack.
         return ({ stuff[EXPRESSION_TREE] +
                   ({ make_node(EXPRESSION_TYPE_OPERATOR, EXPRESSION_IF_NEW,
                                ({ 
                                    stuff2[EXPRESSION_TREE],
                                    stuff3[EXPRESSION_TREE]
                                   }) ) }),
                   token_ret[1],
                   stuff2[EXPRESSION_TYPE] });
         break;
      default :
         stuff = parse_boolean(str, token);
         if (!stuff) {
            return 0;
         }
         return stuff;
      }
} /* parse_operator() */

/**
 * A very small recursive decent parser which must return a boolean
 * value.
 * @param str the input string
 * @return the array control structure
 */
class parse_node* parse_boolean_string(string str) {
   mixed* stuff;
   string* token_ret;

   _warning_string = "";
   _func_variables = ([ ]);
   token_ret = query_token(lower_case(str));
   if (!token_ret) {
      return 0;
   }
   stuff = parse_operator(token_ret[1], token_ret[0]);

   if (!stuff) {
      return ({ });
   }

   if (strlen(stuff[EXPRESSION_PARSE_STRING])) {
      _error_string = "Unable to parse the rest of: " +
                      stuff[EXPRESSION_PARSE_STRING];
      return ({ });
   }

   if (stuff[EXPRESSION_TYPE] != EXPRESSION_TYPE_BOOLEAN) {
      _error_string = "Invalid return type, expected boolean got " +
                     query_type_name(stuff[EXPRESSION_TYPE]);
      return ({ });
   }

   return stuff[EXPRESSION_TREE];
} /* parse_boolean_string() */

/**
 * A very small recursive decent parser which must return a integer
 * value.
 * @param str the input string
 * @return the array control structure
 */
class parse_node* parse_integer_string(string str) {
   mixed* stuff;
   string* token_ret;

   _warning_string = "";
   _func_variables = ([ ]);
   token_ret = query_token(lower_case(str));
   if (!token_ret) {
      return 0;
   }
   stuff = parse_operator(token_ret[1], token_ret[0]);


   if (!stuff) {
     //printf("%O", _error_string);
      return ({ });
   }

   if (strlen(stuff[EXPRESSION_PARSE_STRING])) {
      _error_string = "Unable to parse the rest of: " +
                      stuff[EXPRESSION_PARSE_STRING];
     //printf("%O", _error_string);
      return ({ });
   }

   if (stuff[EXPRESSION_TYPE] != EXPRESSION_TYPE_INTEGER) {
      _error_string = "Invalid return type, expected integer got " +
                     query_type_name(stuff[EXPRESSION_TYPE]);
//printf("%O", query_expression_string(stuff[EXPRESSION_TREE], 0));
//printf("%O", evaluate_expression(stuff[EXPRESSION_TREE]));
      return ({ });
   }

//printf("%O", query_expression_string(stuff[EXPRESSION_TREE], 0));
//printf("%O", evaluate_expression(stuff[EXPRESSION_TREE]));

   return stuff[EXPRESSION_TREE];
} /* parse_integer_string() */

/**
 * A very small recursive decent parser which must return a money
 * value.
 * @param str the input string
 * @return the array control structure
 */
class parse_node* parse_money_string(string str) {
   mixed* stuff;
   string* token_ret;

   _warning_string = "";
   _func_variables = ([ ]);
   token_ret = query_token(lower_case(str));
   if (!token_ret) {
      return 0;
   }
   stuff = parse_operator(token_ret[1], token_ret[0]);


   if (!stuff) {
     //printf("%O", _error_string);
      return ({ });
   }

   if (strlen(stuff[EXPRESSION_PARSE_STRING])) {
      _error_string = "Unable to parse the rest of: " +
                      stuff[EXPRESSION_PARSE_STRING];
     //printf("%O", _error_string);
      return ({ });
   }

   if (stuff[EXPRESSION_TYPE] != EXPRESSION_TYPE_MONEY) {
      _error_string = "Invalid return type, expected money got " +
                     query_type_name(stuff[EXPRESSION_TYPE]);
//printf("%O", query_expression_string(stuff[EXPRESSION_TREE], 0));
//printf("%O", evaluate_expression(stuff[EXPRESSION_TREE]));
      return ({ });
   }

//printf("%O", stuff[EXPRESSION_TREE]);
//printf("%O", query_expression_string(stuff[EXPRESSION_TREE], 0));
//printf("%O", evaluate_expression(stuff[EXPRESSION_TREE]));

   return stuff[EXPRESSION_TREE];
} /* parse_integer_string() */

/**
 * This will pass the arguments to a user defined function and return
 * an array of three elements.  The first is the name of the function,
 * the second is the array of arguments types and the third is the array of
 * argument names.
 */
mixed* parse_user_expression_definition(string def) {
   mixed* token;
   string name;
   int* arg_types;
   int type;
   string* arg_names;

   token = query_token(def);
   name = token[0];
   def = token[1];
   if (!strlen(def)) {
      //
      // No arguments.
      //
      return ({ name, ({ }), ({ }) });
   }

   token = query_token(def);
   if (token[0] == "(") {
      //
      // Parse some arguments.
      //
      token = query_token(token[1]);
      arg_names = ({ });
      arg_types = ({ });
      while (token[0] != ")") {
         type = query_type_value(token[0]);
         if (type == EXPRESSION_TYPE_ARRAY) {
            // Try the next token for the type.
            token = query_token(token[1]);
            type = query_type_value(token[0]);
            if (type != EXPRESSION_TYPE_ERROR) {
               type += EXPRESSION_TYPE_ARRAY_OFFSET;
            }
         }

         if (type == EXPRESSION_TYPE_ERROR) {
            _error_string = "Invalid type " + token[0];
            return 0;
         } else {
            token = query_token(token[1]);
            if (member_array(token[0], arg_names) != -1) {
               _error_string = "Duplicate argument name " + token[0];
               return 0;
            }
            arg_types += ({ type });
            arg_names += ({ token[0] });
            token = query_token(token[1]);
            if (token[0] != "," && token[0] != ")") {
               _error_string = "Expected , or ) after " + arg_names[<1];
               return 0;
            }
            if (token[0] == ",") {
               token = query_token(token[1]);
            }
         }
      }
      if (strlen(token[1])) {
         _error_string = "Unable to parse the rest of " + token[1];
         return 0;
      }
      return ({ name, arg_types, arg_names });
   } else {
      _error_string = "Unable to parse rest of: " + def;
   }
} /* parse_user_expression_definition() */

/**
 * This method attempts to parse a user expression.
 * @param name the name of the function
 * @param args the name and types of the arguments
 * @param str the input string
 * @return the name of the function, 0 on failure
 */
string parse_user_expression(
   string def,
   string str)
{
   mixed* stuff;
   int i;
   string* token_ret;
   class user_function_thing old_fluff;
   class user_function_thing fluff;
   class func_variable_thing womble;
   int* arg_types;
   string* arg_names;
   string name;

   _warning_string = "";
   stuff = parse_user_expression_definition(def);
   if (!stuff) {
      return 0;
   }

   //
   // Pull the useful bits out of the return value.
   //
   name = stuff[0];
   arg_types = stuff[1];
   arg_names = stuff[2];

   //
   // Setup the function variables.
   //
   _func_variables = ([ ]);
   for (i = 0; i < sizeof(arg_types); i++) {
      womble = new(class func_variable_thing);
      womble->type = arg_types[i];
      womble->arg_no = i;
      _func_variables[arg_names[i]] = womble;
   }

   token_ret = query_token(lower_case(str));
   if (!token_ret) {
      return 0;
   }
   stuff = parse_operator(token_ret[1], token_ret[0]);

   //
   // Zap them back to the default.
   //
   _func_variables = ([ ]);

   if (!stuff) {
     //printf("%O", _error_string);
      return 0;
   }

   if (strlen(stuff[EXPRESSION_PARSE_STRING])) {
      _error_string = "Unable to parse the rest of: " +
                      stuff[EXPRESSION_PARSE_STRING];
     //printf("%O", _error_string);
      return 0;
   }

   fluff = new(class user_function_thing);
   fluff->type = stuff[EXPRESSION_TYPE];
   fluff->expr = stuff[EXPRESSION_TREE];
   fluff->arg_names = arg_names;
   fluff->arg_types = arg_types;

   //
   // Let us check to see if one exists current and if we have changed
   // any types.
   //
   old_fluff = _user_functions[name];
   if (old_fluff) {
      if (old_fluff->type != fluff->type) {
         _warning_string += "Warning: Return types have changed " +
                    query_type_name(old_fluff->type) + " to " +
                    query_type_name(fluff->type) + ".\n";
      }
      if (sizeof(old_fluff->arg_types) != sizeof(fluff->arg_types)) {
         _warning_string += "Warning: Number of arguments has changed.\n";
      }
      for (i = 0; i < sizeof(old_fluff->arg_types) && i < sizeof(fluff->arg_types); i++) {
         if (old_fluff->arg_types[i] != fluff->arg_types[i]) {
            _warning_string += "Warning: Argument number " + i +
                               " differs in type " +
                               query_type_name(old_fluff->arg_types[i]) +
                               " to " + query_type_name(fluff->arg_types[i]) +
                               ".\n";
         }
      }
   }
   _user_functions[name] = fluff;

   return name;
} /* parse_user_expression() */

/**
 * Does a sizeof an array.  Nifty...
 * @param arr the array to get the sizeof
 * @param args the rest of the arguments
 * @return the sizeof the array
 */
int sizeof_function(mixed* arr, mixed args ...) {
   return sizeof(arr);
} /* sizeof_function() */

/**
 * This method returns the expresion as a string.
 * @param expr the expression
 * @param brief don't expand the variable names
 * @return the expression as a string
 */
string query_expression_string(class parse_node* expr, int brief) {
   string str;
   int i;
   string* stack;
   int* thing;
   string tmp;
   string place;
   int value;
   string indent;

   if (!sizeof(expr)) {
      return "No expression set.";
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   indent = "";
   str = "";
   stack = ({ });
   thing = ({ });
   for (i = 0; i < sizeof(expr); i++) {
      if (!classp(expr[i])) {
         stack += ({ "Broken element" });
         thing += ({ 0 });
         continue;
      }

      switch (expr[i]->type) {
      case EXPRESSION_TYPE_OPERATOR :
         value = expr[i]->value;
         switch (expr[i]->value) {
         case EXPRESSION_NOT :
            str = " " + query_operator_name(expr[i]->value) + " ";
            if (thing[<1] && thing[<1] != expr[i]->value) {
               str += "(" + stack[<1] + ")";
            } else {
               str += stack[<1];
            }
            stack = stack[0..<2];
            thing = thing[0..<2];
            break;
         case EXPRESSION_AND_NEW :
         case EXPRESSION_OR_NEW :
            str += query_expression_string(expr[i]->tree[0], brief);
            str += " " + query_operator_name(expr[i]->value) + " ";
            str += query_expression_string(expr[i]->tree[1], brief);
            break;
         case EXPRESSION_AND :
         case EXPRESSION_OR :
         case EXPRESSION_PLUS :
         case EXPRESSION_MINUS :
         case EXPRESSION_DIVIDE :
         case EXPRESSION_MULTIPLY :
         case EXPRESSION_GREATOR_THAN     :
         case EXPRESSION_LESS_THAN        :
         case EXPRESSION_EQUAL_TO         :
         case EXPRESSION_GREATOR_OR_EQUAL :
         case EXPRESSION_LESS_OR_EQUAL    :
         case EXPRESSION_NOT_EQUAL_TO     :
            tmp = stack[<2];
            if (thing[<2] && thing[<2] != expr[i]->value) {
               str = "(" + stack[<2] + ")";
            } else {
               str = stack[<2];
            }
            str += " " + query_operator_name(expr[i]->value) + " ";
            if (thing[<1] && thing[<1] != expr[i]->value) {
               str += "(" + stack[<1] + ")";
            } else {
               str += stack[<1];
            }
            stack = stack[0..<3];
            thing = thing[0..<3];
            break;
         case EXPRESSION_TRUE :
            str = "true";
            value = 0;
            break;
         case EXPRESSION_FALSE :
            str = "false";
            value = 0;
            break;
         case EXPRESSION_IF_NEW :
            if (brief) {
               str = "if " + stack[<1] + " then " + 
                     query_expression_string(expr[i]->tree[0], brief) +
                     " else " +
                     query_expression_string(expr[i]->tree[1], brief) +
                     " endif";
            } else {
               str = "if " + stack[<1] + " then\n" +
                     "   " + replace_string(
                        query_expression_string(expr[i]->tree[0], brief),
                                     "\n", "\n   ") +
                     "\nelse\n" +
                     "   " + replace_string(
                        query_expression_string(expr[i]->tree[1], brief),
                                     "\n", "\n   ") +
                     "\nendif";
            }
            stack = stack[0..<2];
            thing = thing[0..<2];
            value = 0;
            break;
         case EXPRESSION_IF :
            if (brief) {
               str = "if " + stack[<3] + " then " + stack[<2] + " else " +
                     stack[<1] + " endif";
            } else {
               str = "if " + stack[<3] + " then\n" +
                     "   " + replace_string(stack[<2], "\n", "\n   ") +
                     "\nelse\n" +
                     "   " + replace_string(stack[<1], "\n", "\n   ") +
                     "\nendif";
            }
            stack = stack[0..<4];
            thing = thing[0..<4];
            value = 0;
            break;
         case EXPRESSION_ARRAY_DEREFERENCE :
            str = stack[<2] + "[" + stack[<1] + "]";
            stack = stack[0..<3];
            thing = thing[0..<3];
            value = 0;
            break;
         case EXPRESSION_ARRAY_AGGREGATE :
            str = "{ " +
                   implode(stack[<(expr[i - 1]->value+1)..<2],
                        ", ") + " }";
            thing = thing[0..<expr[i - 1]->value + 2];
            stack = stack[0..<expr[i - 1]->value + 2];
            value = 0;
            break;
         default :
            str = "Error!";
            break;
         }
         stack += ({ str });
         thing += ({ value });
         break;
      case EXPRESSION_TYPE_ARRAY_NULL :
         stack += ({ "{}" });
         thing += ({ 0 });
         break;
      case EXPRESSION_TYPE_MONEY :
         stack += ({ MONEY_HAND->money_value_string(expr[i]->value, place) });
         thing += ({ 0 });
         break;
      case EXPRESSION_TYPE_INTEGER :
         stack += ({ "" + expr[i]->value });
         thing += ({ 0 });
         break;
      case EXPRESSION_TYPE_STRING :
         stack += ({ "\"" + expr[i]->value + "\"" });
         thing += ({ 0 });
         break;
      case EXPRESSION_TYPE_VARIABLE :
         stack += ({ expr[i]->value });
         thing += ({ 0 });
         break;
      case EXPRESSION_TYPE_FUNCTION_VARIABLE :
         stack += ({ expr[i]->value[EXPRESSION_FUNC_VAR_NAME] });
         thing += ({ 0 });
         break;
      case EXPRESSION_TYPE_FUNCTION :
      case EXPRESSION_TYPE_USER_FUNCTION :
         str = expr[i]->value[EXPRESSION_FUNC_NAME] + "(";
         str += implode(stack[<expr[i]->value[EXPRESSION_FUNC_NO_ARGS]..],
                        ", ");
         str += ")";
         stack = stack[0..<expr[i]->value[EXPRESSION_FUNC_NO_ARGS] + 1];
         thing = thing[0..<expr[i]->value[EXPRESSION_FUNC_NO_ARGS] + 1];
         stack += ({ str });
         thing += ({ 0 });
         break;
      default :
         stack += ({ "Unknown: " + expr[i]->type + " (" + expr[i]->value + ")" });
         thing += ({ 0 });
         break;
      }
   }
   return stack[<1];
} /* query_expression_string() */

/**
 * This method makes a nice string representation of the user function.
 * @param func the name of the user function to get a string of
 * @return the user function string
 */
string query_user_function_string(string func) {
   class user_function_thing thing;
   int i;
   string ret;

   thing = _user_functions[func];
   ret = "";
   ret += query_type_name(thing->type) + " " + func + "(";
   for (i = 0; i < sizeof(thing->arg_types); i++) {
      ret += query_type_name(thing->arg_types[i]) + " " + thing->arg_names[i];
      if (i < sizeof(thing->arg_types) - 1) {
         ret += ", ";
      }
   }
   ret += ") {\n   ";
   ret += replace_string(query_expression_string(thing->expr, 0),
                         "\n", "\n   ") + "\n}\n";
   return ret;
} /* query_user_function_string() */

/**
 * This method evaluates the expression and creates a nice end result
 * thingy.
 * @param expr the exrpession to evaluate
 * @param args the optional args parsed into the various function calls
 * @return the type and value of the expression, 0 if failed
 */
class parse_node evaluate_expression(class parse_node* expr, mixed args ...) {
   class parse_node bing;
   class parse_node new_node;
   class parse_node* stack;
   string fname;
   mixed* fargs;
   int i;

   if (!sizeof(expr)) {
      return make_node(EXPRESSION_TYPE_BOOLEAN, 0, ({ }));
   }

   stack = ({ });
   foreach (bing in expr) {
      if (!classp(bing)) {
         continue;
      }
      switch (bing->type) {
      case EXPRESSION_TYPE_OPERATOR :
         switch (bing->value) {
         case EXPRESSION_NOT :
            stack[<1] = make_node(EXPRESSION_TYPE_BOOLEAN, !stack[<1]->value, 0);
            break;

         case EXPRESSION_AND_NEW :
            // First we evaluate one bit, then the other bit.
            new_node = evaluate_expression(bing->tree[0],
                   args + ({ sizeof(args) }) ...);
            if (!new_node->value) {
               stack += ({ make_node(EXPRESSION_TYPE_BOOLEAN, 0, 0) });
            } else {
               new_node = evaluate_expression(bing->tree[1],
                       args ...);
               stack += ({ make_node(EXPRESSION_TYPE_BOOLEAN, new_node->value, 0) });
            }
            //stack = stack[0..<2];
            break;
         case EXPRESSION_OR_NEW :
            // First we evaluate one bit, then the other bit.
            new_node = evaluate_expression(bing->tree[0],
                   args...);
            if (new_node->value) {
               stack += ({ make_node(EXPRESSION_TYPE_BOOLEAN, 1, 0) });
            } else {
               new_node = evaluate_expression(bing->tree[1],
                       args ...);
               stack += ({ make_node(EXPRESSION_TYPE_BOOLEAN, new_node->value, 0) });
            }
            //stack = stack[0..<2];
            break;
         case EXPRESSION_AND :
            stack[<2] = make_node(EXPRESSION_TYPE_BOOLEAN, stack[<1]->value &&
                                                           stack[<2]->value, 0);
            stack = stack[0..<2];
            break;
         case EXPRESSION_OR :
            stack[<2] = make_node(EXPRESSION_TYPE_BOOLEAN, stack[<1]->value ||
                                                           stack[<2]->value, 0);
            stack = stack[0..<2];
            break;
         case EXPRESSION_PLUS :
            stack[<2] = make_node(stack[<1]->type, stack[<2]->value +
                                                   stack[<1]->value, 0);
            stack = stack[0..<2];
            break;
         case EXPRESSION_MINUS :
            stack[<2] = make_node(stack[<1]->type, stack[<2]->value -
                                                   stack[<1]->value, 0);
            stack = stack[0..<2];
            break;
         case EXPRESSION_DIVIDE :
            stack[<2] = make_node(stack[<1]->type, stack[<2]->value /
                                                   stack[<1]->value, 0);
            stack = stack[0..<2];
            break;
         case EXPRESSION_MULTIPLY :
            stack[<2] = make_node(stack[<1]->type, stack[<2]->value *
                                                   stack[<1]->value, 0);
            stack = stack[0..<2];
            break;
         case EXPRESSION_GREATOR_THAN     :
            stack[<2] = make_node(EXPRESSION_TYPE_BOOLEAN, stack[<2]->value >
                                                           stack[<1]->value, 0);
            stack = stack[0..<2];
            break;
         case EXPRESSION_LESS_THAN        :
            stack[<2] = make_node(EXPRESSION_TYPE_BOOLEAN, stack[<2]->value <
                                                           stack[<1]->value, 0);
            stack = stack[0..<2];
            break;
         case EXPRESSION_EQUAL_TO         :
            stack[<2] = make_node(EXPRESSION_TYPE_BOOLEAN, stack[<2]->value ==
                                                           stack[<1]->value, 0);
            stack = stack[0..<2];
            break;
         case EXPRESSION_GREATOR_OR_EQUAL :
            stack[<2] = make_node(EXPRESSION_TYPE_BOOLEAN, stack[<2]->value >=
                                                           stack[<1]->value, 0);
            stack = stack[0..<2];
            break;
         case EXPRESSION_LESS_OR_EQUAL    :
            stack[<2] = make_node(EXPRESSION_TYPE_BOOLEAN, stack[<2]->value <=
                                                           stack[<1]->value, 0);
            stack = stack[0..<2];
            break;
         case EXPRESSION_NOT_EQUAL_TO     :
            stack[<2] = make_node(EXPRESSION_TYPE_BOOLEAN, stack[<2]->value !=
                                                           stack[<1]->value, 0);
            stack = stack[0..<2];
            break;
         case EXPRESSION_IF_NEW               :
            if (stack[<1]->value) {
               new_node = evaluate_expression(bing->tree[0],
                   args ...);
            } else {
               new_node = evaluate_expression(bing->tree[1],
                   args ...);
            }
            stack = stack[0..<2] + ({ new_node });
            break;
         case EXPRESSION_IF               :
            if (stack[<3]->value) {
               new_node = stack[<2];
            } else {
               new_node = stack[<1];
            }
            stack = stack[0..<4] + ({ new_node });
            break;
         case EXPRESSION_ARRAY_DEREFERENCE :
            if (stack[<1]->value < 0 ||
                stack[<1]->value >= sizeof(stack[<2]->value)) {
               printf("Warning!  Array out of bounds (" + 
                      query_expression_string(expr, 1) + ")\n");
               stack = stack[0..<3] + ({ query_null_type(EXPRESSION_TYPE_INTEGER) });
            } else {
               stack = stack[0..<3] +
                     ({ make_node(stack[<1]->type - EXPRESSION_TYPE_ARRAY_OFFSET,
                                  stack[<2]->value[stack[<1]->value], 0) });
            }
            break;
         case EXPRESSION_ARRAY_AGGREGATE :
            fargs = ({ });
            for (i = 0; i < stack[<1]->value; i++) {
               fargs += ({ stack[<i + 2]->value });
            }
            new_node = new(class parse_node,
                           type : stack[<2]->type + EXPRESSION_TYPE_ARRAY_OFFSET,
                           value : fargs);
            stack = stack[0..<stack[<1]->value+2] + ({ new_node });
            break;
         case EXPRESSION_TRUE             :
            stack += ({ make_node(EXPRESSION_TYPE_BOOLEAN, 1, 0) });
            break;
         case EXPRESSION_FALSE            :
            stack += ({ make_node(EXPRESSION_TYPE_BOOLEAN, 0, 0) });
            break;
         default :
            printf("Unknown operator %O\n", bing);
            break;
         }
         break;
      case EXPRESSION_TYPE_ARRAY_NULL :
         stack += ({ new(class parse_node, type : EXPRESSION_TYPE_ARRAY_NULL,
                         value : ({ }) ) });
         break;
      case EXPRESSION_TYPE_INTEGER :
         stack += ({ copy(bing) });
         break;
      case EXPRESSION_TYPE_STRING :
         stack += ({ copy(bing) });
         break;
      case EXPRESSION_TYPE_MONEY :
         stack += ({ copy(bing) });
         break;
      case EXPRESSION_TYPE_VARIABLE :
         stack += ({ make_node(query_variable_type(bing->value),
                               evaluate(query_variable_value(bing->value),
                                        args ...), 0) });
         break;
      case EXPRESSION_TYPE_FUNCTION_VARIABLE:
         stack += ({ args[bing->value[EXPRESSION_FUNC_VAR_POS] + args[<1]] });
         break;
      case EXPRESSION_TYPE_USER_FUNCTION:
         fname = bing->value[EXPRESSION_FUNC_NAME];
         fargs = stack[<bing->value[EXPRESSION_FUNC_NO_ARGS]..];
         stack = stack[0..<bing->value[EXPRESSION_FUNC_NO_ARGS] + 1];
         if (!_user_functions[fname]) {
            printf("Unknown function %O\n", fname);
            break;
         }
         bing = evaluate_expression(_user_functions[fname]->expr,
                   args + fargs + ({ sizeof(args) }) ...);
         if (!bing) {
            printf("Broken return stuff.\n");
            break;
         }
         stack += ({ bing });
         break;
      case EXPRESSION_TYPE_FUNCTION :
         fname = bing->value[EXPRESSION_FUNC_NAME];
         fargs = stack[<bing->value[EXPRESSION_FUNC_NO_ARGS]..];
         stack = stack[0..<bing->value[EXPRESSION_FUNC_NO_ARGS] + 1];
         stack += ({ make_node(query_function_type(fname),
                               evaluate(query_function_value(fname),
                                        map(fargs, (: $1->value :)) +
                                        args ...), 0) });
         break;
      default :
         printf("Unknown type %O\n", bing);
         break;
      }
//printf("%O: %O\n", bing, stack);
   }

   return stack[<1];
} /* evaluate_expression() */
