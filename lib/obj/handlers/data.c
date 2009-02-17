/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: data.c,v 1.14 2003/03/19 01:01:56 ceres Exp $
 * $Log: data.c,v $
 * Revision 1.14  2003/03/19 01:01:56  ceres
 * Increased compilation speed
 * .
 *
 * Revision 1.13  2002/11/18 01:40:54  ceres
 * modified to support binary true/false
 *
 * Revision 1.12  2002/08/16 01:12:06  pinkfish
 * Make it not call broken fps.
 *
 * Revision 1.11  2002/08/16 01:04:04  pinkfish
 * Make it skip compiling files with dested fps.
 *
 * Revision 1.10  2002/06/15 00:58:52  pinkfish
 * Fix up some issues.
 *
 * Revision 1.9  2002/03/11 03:21:49  pinkfish
 * Add in the file name to the call back.
 *
 * Revision 1.8  2002/03/10 23:52:10  pinkfish
 * Make it more resiliant to runtime errors and parse errors.
 *
 * Revision 1.7  2002/03/10 23:50:36  pinkfish
 * Add in a lisp like data compiler.
 *
 * Revision 1.6  2001/05/14 10:00:54  taffyd
 * Tweaks for better support for arrays.
 *
 * Revision 1.5  1999/11/16 03:35:50  jeremy
 * Fixed it so it breaks up the data into multiple functions,
 * so it should be able to handle much bigger data sets now.
 *
 * Also added the overwrite flag to write_file(); dunno why it
 * didn't bite us every time.
 *
 * Also removed all the #ifdef DEBUG and just #ifdef'd the
 * definition of Error().  I can't read code that has #ifdefs
 * all through it.
 *
 * Revision 1.4  1999/03/06 15:06:40  taffyd
 * Put in debug mode for now, till problem gets sorted out.
 *
 * Revision 1.3  1999/03/05 07:14:26  ceres
 * Made it log only when debugging
 *
 * Revision 1.2  1998/04/27 01:33:00  jeremy
 * Now preserves newlines in source file.  Makes the output easier to
 * debug and avoids "line too long" errors.
 *
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
/**
 * Universal data initialization handler.
 * The data initializer can be used to initialize databases for use in other
 * handlers.  The database is defined in a user-provided text file, with
 * a format similar to the virtual object compiler.  The input file is
 * converted to a temporary object, which allows fairly complicated
 * expressions to be used as data values.  The initializer can handle
 * arrays and mappings (nested to any level (theoretically)), with a base
 * element of any type, including mixed and classes.
 * <p>
 * Added in some code to allow it to compile lisp like files into a
 * mapping as well.
 * <p>
 * To initialize a variable, assign it the value returned by
 * compile_data() in the initializer.  compile_data() takes an array of
 * filenames as its only argument.
 * <p>
 * <b>File format</b>
 * The data file uses the following keywords.  Each keyword is followed
 * by the required data.  The data can be spread over multiple lines,
 * following the same rules as for LPC code, but the keywords must be at
 * the beginning of the line (preceded by optional whitespace).
 * <dl>
 * <dt>::data:: < array | mapping > [ of ] ... < base >
 * <dd>
 * This keyword defines the structure of the data.  The word "of" is
 * optional.  "array" and "mapping" may be repeated as many times as
 * desired.  "base" is the base type of the data.  For classes, it would
 * be of the form "class <classname>".  For types other than classes,
 * the base isn't really
 * used at this time, but something needs to be there to keep the parser
 * in line.  Some examples:
 * <b>
 * <pre>::data:: array of mapping of array of int
 * ::data:: mapping of mapping of mixed
 * ::data:: mapping of array of class myclass</pre>
 * There should only be one ::data:: keyword in the list of files passed
 * to compile_data().  Also, note that classes need to be defined before
 * this statement.  This can be done either with ::quote:: or
 * ::#include::.
 * <dt>::item <index> [,] ... :: [ value ]
 * <dd>This keyword defines the value for one item of the data.  <index> is
 * repeated as often as necessary, given the structure declared in the
 * ::data:: statement.  For mappings, the index can be any valid mapping key.
 * For arrays, the index can be either a number, or the strings i, i++,
 * or ++i, for current index, current index (incrementing afterwards), or
 * next index.  The value can (and probably should) be omitted for
 * classes, with the field values specified with the "->" keyword below.
 * Examples (corresponding to the three ::data:: examples above):
 * <pre>
 * ::item 0, "item 1", 2:: 42
 * ::item "a", "b":: ({ 1, "fred", ({ 2, 3 }) })
 * ::item "x" i++::
 * </pre>
 * <dt> ::-><field>:: <value>
 * <dd>This allows the fields of items of type class to be assigned
 * individually.  In general, the preceding ::item:: keyword should not
 * have been given a value.  The class must have been defined previously,
 * either with an ::#include:: directive, or with the ::quote:: keyword.
 * Examples:
 * <pre>
 * ::Quote::
 * class item_data {
 *     string *season;
 *     mixed  quant;
 *     string ob;
 * }
 * 
 * ::Data:: mapping of class item_data

 * :item "white clover"::
 * ::->season:: ({ "spring" })
 * ::->quant:: ({ 3, 4 })
 * ::->ob:: "/std/plant"
 * </pre>
 * These statements set the season, quant, and ob fields of the mapping
 * <dt>::quote:: <LPC code>
 * <dd>This keyword allows specific LPC statements to be inserted in the
 * object that creates the database.  To use this effectively requires a
 * little understanding of the translation process.  First, all lines
 * associated with a given keyword are folded into one line.  This means
 * that using the "//" comment delimiter in a ::quote:: will cause the
 * remainder of the quoted statements to be ignored.  Second, the
 * prototype of the function that returns the data isn't written until
 * the ::data:: keyword is encountered.  Therefore, any "global"
 * statements (such as class definitions) should be included or quoted
 * before the ::data:: line.  The easiest way to see what's going on is
 * to try a few examples and look at the resulting .c file (which is the
 * first data file name with ".c" stuck on the end).
 * </dl>
 * @example
 * mixed data;
 * data = "/obj/handlers/data"->compile_data(({ "/save/file1.dat",
 *      "/save/file2.dat" }));
 * // This will catenate the two files into one, translate it, and return
 * // the data.  Of course, someone has to create the data files also.
 * @author Jeremy
 */

#include <data.h>
#include <function.h>

#define CALLOUT_DELAY 0

#define DEBUG !
#define DC_DELIM 0
#define DC_ARRAY 1
#define DC_MAPPING 2
#define DC_OF 3
#define DC_CLASS 4
#define DC_STRING 5
#define DC_ITEM 6
#define DC_NUMBER 7
#define DC_GREY 8

// This is the number of characters it reads before starting a new
// function.  I set it to half of the biggest set of files that caused
// me problems.  It can be adjusted as needed.
#define MAX_SUBF_SIZE 16000

#define WHITESPACE(c) (c == 10 || c == 32 || c == '\n')

#ifdef DEBUG
#  define Error(s) write(s); log_file( "DATA_COMPILER", s);
#else
#  define Error(s) ;
#endif

string std_euid;

void create() {
    std_euid = "/secure/master"->creator_file(file_name(this_object()));
    seteuid(std_euid);
    //Error("Note: euid at creation is " + geteuid() + "\n");
}

int tmp_file_no;

private string strip_string( string str ) {
    int     i, j;
 
    if (!str || str == "") return "";
    j = strlen( str ) - 1;
    for( ; WHITESPACE( str[ i ] ) && i <= j; i++ ) ;
    for( ; WHITESPACE( str[ j ] ) && j > i; j-- ) ;
    return str[ i..j ];
} /* strip_space() */

private mixed cleanup_assoc( mixed parse ) {
    int j;

    // Clean out whitespace (seems like there should be an easier
    // way to do this...)
    for (j = 0; j < sizeof(parse[0]); j++) {
        if ((parse[1][j] == DC_DELIM) || (parse[1][j] == DC_OF)) {
            parse[0][j] = 0;
            parse[1][j] = 0;
        }
    }
    parse[0] -= ({ 0 });
    parse[1] -= ({ 0 });
    return parse;
} /* cleanup_assoc */

/**
 * Actualy compiles the files down.
 * See the header file for a more detailed explaination
 * @param path the files to parse
 */
mixed compile_data( string *path ) {
    string tmp_name, data = "", file_data = "", s1, tmp_val, base, keyword;
    string *segments, *ind, *val, cur_index;
    int i, j, t, debug_file, class_pending, stat, subfunc_cnt,
        subfunc_char_cnt, data_keyword_found, allocated_data;
    int *index_types;
    mixed parse, index_max;

    // Most of this is blatantly stolen from /global/virtual/compiler.c
    if (!sizeof(path))
      return 0;
    tmp_name = path[0] + "_dc.c";
    if( find_object( tmp_name ) )
        tmp_name->dest_me();
    if (file_size(tmp_name) > 0) {
        if ((stat = seteuid("Root")) == 0) {
            // This always happens.  But everything seems to work,
            // so I'll just take it out.
            // Error("Error: couldn't set euid to Root (" + stat + ", " +
            //       "secure/master"->valid_seteuid(this_object(), "Root") +
            //       ")\n");
        }
        stat = unguarded((: rm, tmp_name :));
        if (!stat) {
            Error("Error: couldn't remove old .c file (" +
                  geteuid(this_object()) + ", " +
                  "secure/master"->valid_seteuid(this_object(), "Root") +
                  ")\n");
            seteuid(std_euid);
            return 0;
        }
        //Error("Note: " + tmp_name + " removed (supposedly).\n");
    }
    seteuid(std_euid);
    for (i = 0; i < sizeof(path); i++) {
        if (file_size(path[i]) <= 0)
          continue;
        data += read_file( path[i] );
    }
    if (!data) {
        Error("Error: file(s) not found.\n");
        return 0;
    }
    /* Lines beginning with a # are a comment... */
    /* Break into segments at comments */
    segments = explode( "$\n" + data, "\n#" );
    if( !segments ) {
        Error( "prop_to_fun() : Nothing but comments?\n" );
        return 0;
    }
    /* Remove dummy $ (?) */
    segments[ 0 ] = segments[ 0 ][ 1..(sizeof(segments[ 0 ]) - 1) ];
    /* Remove comment lines */
    for( i = 1; i < sizeof( segments ); i++ ) {
        if( sscanf( segments[ i ], "%s\n%s", s1, segments[ i ] ) != 2 ) {
            segments[ i ] = "";
        }
    }
    /* Join segments together again */
    data = implode( segments, "\n" );
    
    /* See example file for explanation of syntax. */
 
    segments = explode( strip_string( data ), "::" );
    /* sizeof(segments) can be odd if the last line has no argument */
    if (sizeof( segments ) % 2) {
      segments += ({""});
    }
    ind = allocate( sizeof( segments ) / 2 );
    val = allocate( sizeof( segments ) / 2 );

    allocated_data = 0;
 
    for( i = 0; i < sizeof( ind ); i++ ) {
        ind[ i ] = segments[ i * 2 ];
        //val[ i ] = replace( segments[ i * 2 + 1 ], "\n", " " );
        // This preserves the newlines; it makes it more readable and avoids
        // line length problems.
        val[ i ] = strip_string( segments[ i * 2 + 1 ] );
        /* look for virtual compiler meta char */
        if( ind[ i ][ 0..0 ] == "#" ) {
            ind[ i ] = lower_case( ind[ i ] );
            if( ind[ i ] == "#debug" ) {
                /* debug errent virtual programs, ie, don't rm */
                /* the .c file if debug_file is non-zero */
                sscanf( val[ i ], "%d", debug_file );
            } else if( ind[ i ] == "#include" ) {
                /* include the file in setup(), just before the */
                /* object is cloned.  */
                tmp_val = val[i];
                file_data += "#include " + replace( tmp_val, " ", "" ) + "\n";
            }
        }
    }
    for( i = 0; i < sizeof( ind ); i++ ) {
        keyword = lower_case( ind[ i ] );
        if( keyword[ 0..0 ] == "#" ) {
            /* it's a virtual keyword don't stick it in the .c file */
            continue;
        }
        // Keep track of how big the function is.  The easiest way
        // to judge the amount of code is by counting the characters.
        subfunc_char_cnt += sizeof(ind[i]) + sizeof(val[i]);
        if (keyword == "data") {
            if (data_keyword_found) {
                Error("Error: more than one data keyword found.\n");
                return 0;
            }
            data_keyword_found = 1;
            // This declares the structure of the database
            file_data += "void dest_me() { destruct( this_object() ); }\n\n";
            // Break up code into multiple functions; big files can
            // run into trouble compiling.
            parse = reg_assoc(val[i],
                              ({ "array", "mapping", "of", "class +[^\t ]+",
                                 "[^\t ]+" }),
                              ({ DC_ARRAY, DC_MAPPING, DC_OF, DC_CLASS,
                                 DC_GREY }),
                              DC_DELIM);
            parse = cleanup_assoc( parse );
            //printf("parse = %O\n", parse);
            for (j = 0; (j < sizeof(parse[0])) && !index_max; j++) {
                switch (parse[1][j]) {
                  case DC_ARRAY:
                    break;
                  case DC_MAPPING:
                    break;
                  case DC_CLASS:
                    base = implode(parse[0][j..], " ");
                    file_data += base + " item;\n";
                  case DC_GREY:
                    index_types = parse[1][0..j-1];
                    index_max = allocate(sizeof(index_types));
                    break;
                  default:
                    Error("Error: data parse error 1 (" + parse[0][j] + ")\n");
                    return 0;
                }
            }
            if (index_types[0] == DC_MAPPING)
              file_data += "mapping data = ([ ]);\n\n";
            else
              file_data += "mixed data;\n\n";
            for (j = 0; j < sizeof(index_types); j++) {
                switch (index_types[j]) {
                  case DC_ARRAY:
                    index_max[j] = -1;
                    break;
                  case DC_MAPPING:
                    //index_max[j] = ([ ]);
                    break;
                  default:
                    Error("Error: illegal index type found ("
                          + index_types[j] + ")\n");
                    return 0;
                }
            }
            file_data += "mixed data_return_" + subfunc_cnt + "() {\n";
            subfunc_cnt++;
            continue;
        }
        if (keyword[0..3] == "item") {
            // This is where the actual array values get assigned
            // First take care of pending class assignments
            if (class_pending) {
                file_data += "  data" + cur_index + " = item;\n";
                class_pending = 0;
            }
            // Check to see if we should break to a new function (this
            // is the safest place, since we can't break up an item
            // definition).
            if (subfunc_char_cnt > MAX_SUBF_SIZE) {
                // This could theoretically fail if we somehow got more than
                // MAX_SUBF_SIZE characters before the first "data" keyword.
                // So it goes...
                file_data += "}\n\n";
                file_data += "mixed data_return_" + subfunc_cnt + "() {\n";
                subfunc_cnt++;
                subfunc_char_cnt = 0;
            }
            parse = reg_assoc( ind[i], ({ "\"[^\"]*\"", "item", "[0-9]+",
                                      "[^,\t ]+"}),
                              ({ DC_STRING, DC_ITEM, DC_NUMBER, DC_GREY }));
            parse = cleanup_assoc( parse );
            //printf("parse = %O\nindex_max = %O\n", parse, index_max);
            cur_index = "";
            for (j = 0; j < sizeof(index_types); j++) {
                switch (index_types[j]) {
                  case DC_ARRAY:
                    // Index can be a number, "i", "i++", or "++i"
                    if (parse[1][j+1] != DC_NUMBER) {
                        if (parse[0][j+1] == "i") {
                            parse[0][j+1] = index_max[j] + "";
                        } else if (parse[0][j+1] == "i++") {
                            parse[0][j+1] = index_max[j] + "";
                            index_max[j]++;
                        } else if (parse[0][j+1] == "++i") {
                            index_max[j]++;
                            parse[0][j+1] = index_max[j] + "";
                        } else {
                          Error("Error: illegal index for array (" +
                                  parse[0][j+1] + ")\n");
                            return 0;
                        }
                    }
                    if ((t = to_int(parse[0][j+1])) > index_max[j]) {
                        file_data += "  data" + cur_index
                          + " = allocate(" + (t-index_max[j]) + ");\n";
                        index_max[j] = t;
                        allocated_data = 1; 
                    }
                    break;
                  case DC_MAPPING:
                    if (j) {
                        file_data += "  if (!mapp(data" + cur_index + "))"
                          + " data" + cur_index + " = ([]);\n";
                    }
                    break;
                  default:
                    Error("Error: illegal index type found (" + index_types[j]
                          + ")\n");
                    return 0;
                } /* switch */
                cur_index += "[" + parse[0][j+1] + "]";
            } /* for */
            if (strip_string(val[i]) != "") {
                //printf("val[i] = %O\n", val[i]);
                file_data += "  data" + cur_index + " = " + val[i] + ";\n";
            }
            continue;
        } /* if */
        if (keyword[0..1] == "->") {
            // This is for handling fields of classes
            if (!class_pending) {
                file_data += "  item = new( " + base + " );\n";
                class_pending = 1;
            }
            file_data += "  item->" + ind[i][2..] + " = " + val[i] + ";\n";
            continue;
        }
        if (keyword == "quote") {
            // A backdoor for putting in specific LPC lines
            file_data += val[i] + "\n";
            continue;
        }
    } /* for */
    if (class_pending) {
        file_data += "  data" + cur_index + " = item;\n";
    }
    file_data += "}\n\n";
    file_data += "mixed data_return() {\n";

    if ( !allocated_data ) { 
        for (j = 0; j < sizeof(index_types); j++) {
            switch (index_types[j]) {
                case DC_ARRAY:
                    file_data += "  data = allocate(" + (to_int(index_max[j]) + 1) + ");\n";
                break;
            }
        } 
    }

    for (i = 0; i < subfunc_cnt; i++) {
        file_data += "  data_return_" + i + "();\n";
    }
    file_data += "  return data;\n}\n";
    seteuid("Root");
    unguarded((: write_file, tmp_name, file_data, 1 :));
    seteuid(std_euid);
    return tmp_name->data_return();
} /* compile_data() */

class data_node {
   int type;
   mixed value;
}

class queue_node {
   string name;
   class data_node data;
}

class compile_data {
   string file_name;
   function call_back;
   int current_line;
   int look_for;
   int last_chunk_compile;
   int file_len;
   class queue_node* depths;
}


private nosave class compile_data* _to_compile = ({ });

void start_compile();
void parse_chunk(class compile_data data, string chunk);

#define DATA_UNKNOWN 0
#define DATA_CHILDREN 1
#define DATA_NUMBER 2
#define DATA_STRING 3
#define DATA_LIST 4


#define OPEN_BRACKET 1
#define START_ARGUMENT 2
#define END_BRACKET 3
#define END_STRING 4
#define START_LIST 5
#define END_NUMBER 6
#define ARGUMENT_VALUE 7
#define ARGUMENT_NAME 8
#define REST_OF_ARGUMENT 9
#define END_STRING_LIST 10
#define END_NUMBER_LIST 11
#define END_LIST 12

#define CHUNK_SIZE 10

/**
 * Compiles up the file into the useful soul format.   It also tells
 * the soul about it.
 * <p>
 * See the soul data files in /save/new_soul for details of the format
 * for the soul files.   The file has to come from the soul save
 * directory or the call will not work.
 * @param fname the name of the file to compile
 */
void compile_file(string fname, function call_back) {
   class compile_data data;
/*
 * First, do we have read access to the file.
 * and is it actually a file?
 */   
   if (file_size(fname) == -1) {
      tell_object(this_player(), "The file "+
                         fname+" does not exist.\n");
      return ;
   }
   if (file_size(fname) == -2) {
      tell_object(this_player(), "The file "+
                         fname+" is a directory exist.\n");
      return ;
   }
   data = new(class compile_data);
   data->file_name = fname;
   data->call_back = call_back;
   data->look_for = OPEN_BRACKET;
   _to_compile += ({ data });
   start_compile();
} /* compile_file() */

/** @ignore yes */
void start_compile() {
   class compile_data data;

   /* We are already compiling */
   if (!sizeof(_to_compile) || _to_compile[0]->last_chunk_compile) {
      return ;
   }
   data = _to_compile[0];
   data->last_chunk_compile = time();
   data->current_line = 1;
   data->look_for = OPEN_BRACKET;
   data->file_len = file_length(data->file_name);
//current_result = new(class compile_node, type : 0, children : ([ ]) );
   data->depths = ({ new(class queue_node,
                  data : new(class data_node, type : 0, value : ([ ]) )) });
   call_out("compile_chunk", CALLOUT_DELAY);
} /* start_compile() */

/** @ignore yes */
void compile_chunk() {
   string chunk;
   int end;
   class compile_data data;

   data = _to_compile[0];
   data->last_chunk_compile = time();
   if (data->current_line > data->file_len ||
       functionp(data->call_back) & FP_OWNER_DESTED) {
      call_out("start_compile", CALLOUT_DELAY);
      _to_compile = _to_compile[1..];
      if (!(functionp(data->call_back) & FP_OWNER_DESTED)) {
         evaluate(data->call_back, data->file_name, data->depths[0]->data->value);
      }
      return ;
   }
   if (data->current_line+CHUNK_SIZE > data->file_len) {
      end = data->file_len+1;
   } else {
      end = data->current_line+CHUNK_SIZE;
   }
   chunk = unguarded((: read_file, data->file_name, data->current_line,
                               end-data->current_line :));
   data->current_line = end;
   call_out("compile_chunk", CALLOUT_DELAY);
   parse_chunk(data, chunk);
} /* compile_chunk() */

/**
 * This will return a normal integer, or a dice class.  The dice class
 * allows for things of the form 5d6+10 or 3d11-10.
 * @return the dice class
 */
mixed to_diceint(string str) {
   class data_dice dice;
   string s1;

   if (strsrch(str, "d") != -1) {
      dice = new(class data_dice);
      if (sscanf(str, "%dd%s", dice->number, s1) == 2) {
         if (strsrch(s1, "+")) {
            if (sscanf(s1, "%d+%d", dice->die, dice->modifier) != 2) {
               dice->die = to_int(s1);
            }
         } else if (strsrch(s1, "-")) {
            if (sscanf(s1, "%d-%d", dice->die, dice->modifier) != 2) {
               dice->die = to_int(s1);
            } else {
               dice->modifier = - dice->modifier;
            }
         } else {
            dice->die = to_int(s1);
         }
      }
      return dice;
   }

   return to_int(str);
} /* to_diceint() */

/** @ignore yes */
void parse_chunk(class compile_data data, string chunk) {
   /* Now.   What are we looking for? */
   /* Love, a nice place in the world, a happy bag full of groceries. */
   string *bits;
   string s1;
   string s2;
   string s3;
   int pos;
   int chunk_size;
   int start;
   class data_node node;

   // Scan out all the comments.
   while (sscanf(chunk, "%s#%s\n%s", s1, s2, s3) == 3) {
      chunk = s1 + s3;
   }

   chunk_size = strlen(chunk);
   pos = 0;
   bits = explode(chunk, "(");
   while (pos < chunk_size) {
      switch (data->look_for) {
         case OPEN_BRACKET :
            while (pos < chunk_size && (chunk[pos] == ' ' || 
                      chunk[pos] == '\t' || chunk[pos] == '\n')) {
               pos++;
            }
            if (pos == chunk_size) {
               break;
            }
            if (chunk[pos] == ')') {
               data->look_for = END_BRACKET;
               break;
            }
            if (chunk[pos] == '(') {
               chunk = chunk[pos+1..];
               chunk_size = strlen(chunk);
               pos = 0;
               node = new(class data_node, type : 0);
               data->depths += ({ new(class queue_node, data : node) });
               data->look_for = ARGUMENT_NAME;
            } else {
               pos = chunk_size;
            }
            break;

         case ARGUMENT_NAME :
            /* We look for the first non-space, non-tab, non-nl */
            while (pos < chunk_size && (chunk[pos] == ' ' || 
                      chunk[pos] == '\t' || chunk[pos] == '\n')) {
               pos++;
            }
            if (pos == chunk_size) {
               break;
            }
            start = pos;
            /* Ok, now we search for the next one. */
            while (pos < chunk_size && chunk[pos] != ' ' && 
                      chunk[pos] != '\t' && chunk[pos] != '\n') {
               pos++;
            }
            /* Thats it.   Our argument name. */
            data->depths[<1]->name = chunk[start..pos-1];
            data->look_for = ARGUMENT_VALUE;
            break;

         case ARGUMENT_VALUE :
         case START_LIST :
            while (pos < chunk_size && (chunk[pos] == ' ' || 
                      chunk[pos] == '\t' || chunk[pos] == '\n')) {
               pos++;
            }
            if (pos >= chunk_size) {
               break;
            }
            switch (chunk[pos]) {
               case '(' :
                  if (data->look_for == START_LIST) {
                     debug_printf("Error, found a bracket inside a list.\n");
                     pos = chunk_size;
                     break;
                  }
                  // Change it to look for a bracket.
                  data->look_for = OPEN_BRACKET;
                  data->depths[<1]->data->type = DATA_CHILDREN;
                  data->depths[<1]->data->value = ([ ]);
                  break;
               case '"' :
                  if (data->look_for == START_LIST) {
                     data->look_for = END_STRING_LIST;
                     data->depths[<1]->data->value += ({ "" });
                  } else {
                     data->look_for = END_STRING;
                     data->depths[<1]->data->value = "";
                  }
                  pos++;
                  data->depths[<1]->data->type = DATA_STRING;
                  break;
               case '0'..'9' :
                  // Number.
                  if (data->look_for == START_LIST) {
                     data->look_for = END_NUMBER_LIST;
                     data->depths[<1]->data->value += ({ "" });
                  } else {
                     data->look_for = END_NUMBER;
                     data->depths[<1]->data->value = "";
                  }
                  data->depths[<1]->data->type = DATA_NUMBER;
                  break;
               case '}' :
                  if (data->look_for == START_LIST) {
                     data->look_for = END_BRACKET;
                     pos++;
                     data->depths[<1]->data->type = DATA_LIST;
                  } else {
                     debug_printf("End of list without a start of list.\n");
                     pos = chunk_size;
                  }
                  break;
               case '{' :
                  if (data->look_for == START_LIST) {
                     debug_printf("Cannot have nested lists.\n");
                     pos = chunk_size;
                  } else {
                     data->look_for = START_LIST;
                  }
                  data->depths[<1]->data->value = ({ });
                  data->depths[<1]->data->type = DATA_LIST;
                  pos++;
                  break;
               default :
                 if(chunk[pos] = 't')
                  if(chunk[pos..pos+3] == "true" || chunk[pos..pos+2] == "yes") {
                    if(chunk[pos..pos+3] == "true")
                      pos += 3;
                    else
                      pos += 2;
                    
                    data->depths[<1]->data->value = 1;
                    data->depths[<1]->data->type = DATA_NUMBER;
                    data->look_for = END_BRACKET;
                    break;
                  }
                  debug_printf("Unknown data type %s in %s\n",
                               chunk[pos..pos+5], chunk);
                  pos = chunk_size;
                   
                  break;
            }
            break;

         case END_LIST :
            while (pos < chunk_size && (chunk[pos] == ' ' || 
                      chunk[pos] == '\t' || chunk[pos] == '\n')) {
               pos++;
            }
            if (pos >= chunk_size) {
               break;
            }
            switch (chunk[pos]) {
               case ',' :
                  pos++;
                  data->look_for = START_LIST;
                  break;
               case '}' :
                  data->look_for = START_LIST;
                  break;
               default :
                  debug_printf("Expected , or } not %s\n", chunk[pos..pos+5]);
                  pos = chunk_size;
                  break;
            }
            break;

         case END_BRACKET :
            if (sscanf(chunk[pos..], "%s)%s", s1, s2)) {
               if (arrayp(data->depths[<2]->data->value[data->depths[<1]->name])) {
                  data->depths[<2]->data->value[data->depths[<1]->name] += ({ 
                       data->depths[<1]->data->value });
               } else if (sizeof(data->depths) == 2) {
                  data->depths[<2]->data->value[data->depths[<1]->name] = ({ 
                       data->depths[<1]->data->value });
               } else if (!undefinedp(data->depths[<2]->data->value[data->depths[<1]->name])) {
                  data->depths[<2]->data->value[data->depths[<1]->name] = ({ 
                       data->depths[<2]->data->value[data->depths[<1]->name],
                       data->depths[<1]->data->value });
               } else {
                  data->depths[<2]->data->value[data->depths[<1]->name] = data->depths[<1]->data->value;
               }
               data->depths = data->depths[0..<2];
               chunk = s2;
               chunk_size = strlen(s2);
               pos = 0;
               data->look_for = OPEN_BRACKET;
            }
            break;

         case END_NUMBER_LIST :
         case END_NUMBER :
            // Look for the first non-number.
            start = pos;
            while (pos < chunk_size && ((chunk[pos] >= '0' && 
                      chunk[pos] <= '9') ||
                      chunk[pos] == 'd' || chunk[pos] == '+' ||
                      chunk[pos] == '-')) {
               pos++;
            }
            if (data->look_for == END_NUMBER) {
               data->depths[<1]->data->value += chunk[start..pos - 1];
            } else {
               data->depths[<1]->data->value[<1] += chunk[start..pos - 1];
            }
            if (pos < chunk_size) {
               if (data->look_for == END_NUMBER) {
                  data->depths[<1]->data->value = to_diceint(data->depths[<1]->data->value);
                  data->look_for = END_BRACKET;
               } else {
                  data->depths[<1]->data->value[<1] = to_diceint(data->depths[<1]->data->value[<1]);
                  data->look_for = END_LIST;
               }
            }
            break;

         case END_STRING_LIST :
         case END_STRING :
            if (sscanf(chunk[pos..], "%s\"%s", s1, s2)) {
               if (strlen(s1) > 0 && s1[strlen(s1)-1] == '\\') {
                  if (data->look_for == END_STRING) {
                     data->depths[<1]->data->value += replace(s1[0..strlen(s1)-2], "\n", "")+"\"";
                  } else {
                     data->depths[<1]->data->value[<1] += replace(s1[0..strlen(s1)-2], "\n", "")+"\"";
                  }
                  chunk = s2;
                  pos = 0;
                  chunk_size = strlen(s2);
               } else {
                  if (data->look_for == END_STRING) {
                     data->depths[<1]->data->value += replace(s1, "\n", "");
                     data->look_for = END_BRACKET;
                  } else {
                     data->depths[<1]->data->value[<1] += replace(s1, "\n", "")+"\"";
                     data->look_for = END_LIST;
                  }
                  chunk = s2;
                  pos = 0;
                  chunk_size = strlen(s2);
               }
            } else {
               if (data->look_for == END_STRING) {
                  data->depths[<1]->data->value += replace(chunk, "\n", "");
               } else {
                  data->depths[<1]->data->value[<1] += chunk;
               }
               pos = chunk_size;
            }
            break;

         default :
            debug_printf("Horrible error "+data->look_for+" (" +
                         data->file_name + ") " + data->current_line + " " +
                         chunk[pos..] + "\n");
            pos = chunk_size;
            break;
      }
   }

   //debug_printf("%O\n", data);
} /* parse_chunk() */
