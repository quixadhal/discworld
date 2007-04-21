/*   -*- LPC -*-   */
/*
 * $Locker:  $
 * $Id: soul_compiler.c,v 1.4 2002/12/12 20:33:05 ceres Exp $
 * $Log: soul_compiler.c,v $
 * Revision 1.4  2002/12/12 20:33:05  ceres
 * Fixed
 *
 * Revision 1.3  2001/03/12 05:14:35  pinkfish
 *  Forcibly unlocked by ceres
 *
 * Revision 1.2   2000/06/15 02:24:35   pinkfish
 * Hopefully make it deal with files with a bad syntax.
 *
 * Revision 1.1   1998/01/06 05:03:33   ceres
 * Initial revision
 * 
*/
/**
 * This compiles the soul files into a non human readable format.   The
 * format is far more useful to the soul though :)
 */
#include "soul.h"
#include "user_parser.h"

private nosave string _current_file;
private nosave int _current_line;
private nosave int _file_len;
private nosave int _depth;
private nosave int _look_for;
private nosave int _last_chunk_compile;
private nosave mixed *_to_compile;
private nosave mixed *_arguments;
private nosave object _current_player;

void start_compile();
void parse_chunk(string chunk);
void make_into_soul_commands(mapping commands);

/* Number of lines in a chunk */
#define CHUNK_SIZE 20

#define OPEN_BRACKET 1
#define START_ARGUMENT 2
#define END_BRACKET 3
#define END_STRING 4
#define ARGUMENT_VALUE 5
#define ARGUMENT_NAME 6
#define REST_OF_ARGUMENT 7

void create() {
   _to_compile = ({ });
   seteuid("Root");
} /* create() */

/** @ignore yes */
int test_security(string fname) {
   return 1;
} /* test_security() */

/** @ignore yes */
void notify(string mess) {
   if (_current_player)
      tell_object(_current_player, mess);
} /* notify() */

/**
 * Compiles up the file into the useful soul format.   It also tells
 * the soul about it.
 * <p>
 * See the soul data files in /save/new_soul for details of the format
 * for the soul files.   The file has to come from the soul save
 * directory or the call will not work.
 * @param fname the name of the file to compile
 */
void compile_file(string fname) {
/*
 * First, do we have read access to the file.
 * and is it actually a file?
 */   
   if (file_size(SOUL_DIR+fname) == -1) {
      tell_object(this_player(), "The file "+
                         SOUL_DIR+fname+" does not exist.\n");
      return ;
   }
   if (file_size(SOUL_DIR+fname) == -2) {
      tell_object(this_player(), "The file "+
                         SOUL_DIR+fname+" is a directory exist.\n");
      return ;
   }
   _to_compile += ({ SOUL_DIR+fname, this_player() });
   start_compile();
} /* compile_file() */

/**
 * Compiles up a directory full of files.
 * @see compile_file() 
 * @param fname the directory name to compile
 */
void compile_directory(string fname) {
   string *bits, start;
   int i;

   if (file_size(SOUL_DIR+fname) != -2) {
      tell_object(this_player(), "The file "+
                         fname+" is not a directory exist.\n");
      return ;
   }
   bits = explode(fname, "/");
   start = implode(bits[0..<1], "/");
   if (start != "") {
      start += "/";
   }
   fname = SOUL_DIR+fname;
   if (fname[<1] == '/') {
      fname += "*.s";
   } else {
      fname += "/*.s";
   }
   bits = get_dir(fname);
   for (i=0;i<sizeof(bits);i++) {
      bits[i] = start+bits[i];
      compile_file(bits[i]);
   }
} /* compile_directory() */

/** @ignore yes */
void start_compile() {
/* We are already compiling */
   if ((_current_file || !sizeof(_to_compile)) &&
       (time() - _last_chunk_compile) > 10) {
      return ;
   }
   write("Starting to compile " + _to_compile[0] + "\n");
   _current_file = _to_compile[0];
   _current_player = _to_compile[1];
   _to_compile = _to_compile[2..];
   _current_line = 1;
   _depth = 0;
   _look_for = OPEN_BRACKET;
   _file_len = file_length(_current_file);
   _arguments = ({ 0, ([ ]) });
   call_out("compile_chunk", 2);
} /* start_compile() */

/** @ignore yes */
void compile_chunk() {
   string chunk;
   int end;

   _last_chunk_compile = time();
   if (_current_line+CHUNK_SIZE > _file_len) {
      end = _file_len+1;
   } else {
      end = _current_line+CHUNK_SIZE;
   }
   chunk = unguarded((: read_file, _current_file, _current_line,
                               end-_current_line :));
   _current_line = end;
   parse_chunk(chunk);
   if (end > _file_len) {
/* finished file */
      call_out("start_compile", 2);
      _current_file = 0;
      make_into_soul_commands(_arguments[1]);
   } else {
      call_out("compile_chunk", 2);
   }
} /* compile_chunk() */

/** @ignore yes */
void parse_chunk(string chunk) {
   /* Now.   What are we looking for? */
   /* Love, a nice place in the world, a happy bag full of groceries. */
   string *bits, s1, s2;
   int pos, chunk_size, start;

   chunk_size = strlen(chunk);
   pos = 0;
   bits = explode(chunk, "(");
   while (pos < chunk_size) {
      switch (_look_for) {
         case OPEN_BRACKET :
            if (sscanf(chunk[pos..], "%s(%s", s1, s2)) {
               chunk = s2;
               chunk_size = strlen(chunk);
               pos = 0;
               _depth++;
               _arguments += ({ 0, 0 });
               _look_for = ARGUMENT_NAME;
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
            _arguments[_depth*2] = chunk[start..pos-1];
            _look_for = ARGUMENT_VALUE;
            break;

         case ARGUMENT_VALUE :
            while (pos < chunk_size && (chunk[pos] == ' ' || 
                      chunk[pos] == '\t' || chunk[pos] == '\n')) {
               pos++;
            }
            if (pos >= chunk_size) {
               break;
            }
            switch (chunk[pos]) {
               case '(' :
                  _arguments[_depth*2+1] = ([ ]);
                  _depth++;
                  _arguments += ({ 0, 0 });
                  _look_for = ARGUMENT_NAME;
                  pos++;
                  break;
               case '"' :
                  _arguments[_depth*2+1] = "";
                  _look_for = END_STRING;
                  pos++;
                  break;
               default :
/* A string, at most one space seperator between them... */
                  start = pos;
                  if (sscanf(chunk[pos..], "%s)%s", s1, s2) == 2) {
                     _arguments[_depth*2+1] = replace(implode(explode(replace(s1, 
                            ({ "\n", " ", "\r", "" })), " ")-({ "" }), " "), ", ", ",");
                     pos = 0;
                     chunk = ")"+s2;
                     _look_for = END_BRACKET;
                  } else {
                     _arguments[_depth*2+1] = chunk[pos..];
                     _look_for = REST_OF_ARGUMENT;
                     pos = 0;
                     chunk = "";
                  }
                  chunk_size = strlen(chunk);
                  break;
            }
            break;

         case REST_OF_ARGUMENT :
            if (sscanf(chunk[pos..], "%s)%s", s1, s2) == 2) {
               _arguments[_depth*2+1] = replace(implode(explode(
                                                    replace(_arguments[_depth*2+1]+s1, "\n", " "),
                                                                         " ") - ({ "" }),
                                                             " "), ", ", ",");
               pos = 0;
               chunk = ")"+s2;
               _look_for = END_BRACKET;
            } else {
               _arguments[_depth*2+1] += chunk[pos..];
               pos = 0;
               chunk = "";
            }
            chunk_size = strlen(chunk);
            break;

         case END_BRACKET :
            if (sscanf(chunk[pos..], "%s)%s", s1, s2)) {
               switch (_depth) {
                  case 2 : 
                  case 3 :
                     if (pointerp(_arguments[_depth*2-1][_arguments[_depth*2]]))
                        _arguments[_depth*2-1][_arguments[_depth*2]] += ({ _arguments[_depth*2+1] });
                     else
                        _arguments[_depth*2-1][_arguments[_depth*2]] = ({ _arguments[_depth*2+1] });
                     break;
                  case 1 :
                  case 4 :
                  case 5 :
                  case 6 :
                  case 7 :
                     _arguments[_depth*2-1][_arguments[_depth*2]] = _arguments[_depth*2+1];
                     break;
               }
               chunk = s2;
               chunk_size = strlen(s2);
               _depth--;
               pos = 0;
               _arguments = _arguments[0.._depth*2+1];
               if (_depth) {
                  _look_for = START_ARGUMENT;
               } else {
                  _look_for = OPEN_BRACKET;
               }
            }
            break;

         case START_ARGUMENT :
            while (pos < chunk_size && (chunk[pos] == ' '
                   || chunk[pos] == '\t' || chunk[pos] == '\n')) {
               pos++;
            }
            if (pos >= chunk_size) {
               break;
            }
            switch (chunk[pos]) {
               case ')' :
                  _look_for = END_BRACKET;
                  break;
               case '(' :
                  _look_for = OPEN_BRACKET;
                  break;
               default :
                  if (!sscanf(chunk[pos..], "%s\n%s", s1, s2)) {
                     s1 = chunk;
                  }
                  printf("Syntax error in file %s, near %s\n", _current_file, s1);
                  _look_for = END_BRACKET;
                  break;
            }
            break;

         case END_STRING :
            if (sscanf(chunk[pos..], "%s\"%s", s1, s2)) {
               if (strlen(s1) > 0 && s1[strlen(s1)-1] == '\\') {
                  _arguments[_depth*2+1] += replace(s1[0..strlen(s1)-2], "\n", "")+"\"";
                  chunk = s2;
                  pos = 0;
                  chunk_size = strlen(s2);
               } else {
                  _arguments[_depth*2+1] += replace(s1, "\n", "");
                  _look_for = END_BRACKET;
                  chunk = s2;
                  pos = 0;
                  chunk_size = strlen(s2);
               }
            } else {
               _arguments[_depth*2+1] += replace(chunk, "\n", "");
               pos = chunk_size;
            }
            break;

         default :
            notify("Horrible error "+_look_for+"\n");
            pos = chunk_size;
            break;
      }
   }
} /* parse_chunk() */

/** @ignore yes */
int check_sort(string pat1, string pat2) {
   int lvl1, lvl2;

   lvl1 = ((mixed *)PATTERN_OB->compile_pattern(pat1))[0];
   lvl2 = ((mixed *)PATTERN_OB->compile_pattern(pat2))[0];
   return lvl2-lvl1;
} /* check_sort() */

/** @ignore yes */
string *sort_patterns(string *inp) {
   if (!pointerp(inp)) {
      printf("%O\n", inp);
      return ({ });
   }
   return sort_array(inp, "check_sort", this_object());
/*
   string *ret;
   int *lvl, i, j, level;

   lvl = allocate(sizeof(inp));
   ret = ({ });
   for (i=0;i<sizeof(inp);i++) {
      level = ((mixed *)PATTERN_OB->compile_pattern(inp[i]))[0];
      for (j=0;j<sizeof(ret);j++) {
         if (lvl[j] < level) {
            if (j) {
               ret = ret[0..j-1]+({ inp[i] })+ret[j..];
               lvl = lvl[0..j-1]+({ level })+lvl[j..];
            } else {
               ret = ({ inp[i] })+ret[j..];
               lvl = ({ level })+lvl[j..];
            }
            break;
         }
      }
      if (j == sizeof(ret)) {
         ret += ({ inp[i] });
         lvl += ({ level });
      }
   }
   return ret;
 */
} /* sort_patterns() */

/** @ignore yes */
void make_into_soul_commands(mapping comms) {
   string *fluff;
   int i, failed, j;
   mapping ret, tmp;

   fluff = keys(comms);
   ret = ([ ]);
   for (i=0;i<sizeof(fluff);i++) {
/* No arguments and arguments... */
      ret[fluff[i]] = ({ comms[fluff[i]]["pattern"], 0, 0 });
/* Ok, now we see if we have a single bit */
      if (comms[fluff[i]]["single"]) {
/* Yes! */
         if (comms[fluff[i]]["single"][0]["no-arguments"]) {
            tmp = comms[fluff[i]]["single"][0]["no-arguments"][0];   
            if (!tmp["self"]) {
               notify("The 'self' type is missing in the no-argument, single for the soul command "+fluff[i]+"\n");
               failed = 1;
            } else if (!tmp["rest"]) {
               notify("The 'rest' type is missing in the no-argument, single for the soul command "+fluff[i]+"\n");
               failed = 1;
            } else {
               ret[fluff[i]][SINGLE] = ({ ({ tmp["self"], tmp["rest"],
                                                            tmp["position"] }),
                                                         0 });
            }
         }
         if (comms[fluff[i]]["single"][0]["arguments"]) {
            j = sizeof(comms[fluff[i]]["single"][0]["arguments"]);
            if (!ret[fluff[i]][SINGLE]) {
               ret[fluff[i]][SINGLE] = ({ 0, allocate(j*SMALL_ARG_SIZE) });
            } else {
               ret[fluff[i]][SINGLE][ARGUMENTS] = allocate(j*SMALL_ARG_SIZE);
            }
            for (j=0;j<sizeof(comms[fluff[i]]["single"][0]["arguments"]);j++) {
               tmp = comms[fluff[i]]["single"][0]["arguments"][j];
               if (!mapp(tmp)) {
                  notify("The soul command "+fluff[i]+" is totaly stuffed.\n");
                  failed = 1;
               } else if (!tmp["self"]) {
                  notify("The 'self' type is missing in the argument, single for the soul command "+fluff[i]+"\n");
                  failed = 1;
               } else if (!tmp["rest"]) {
                  notify("The 'rest' type is missing in the argument, single for the soul command "+fluff[i]+"\n");
                  failed = 1;
               } else if (!tmp["arguments"]) {
                  notify("The 'arguments' type is missing in the argument, single for the soul command "+fluff[i]+"\n");
                  failed = 1;
               } else {
                  ret[fluff[i]][SINGLE][ARGUMENTS][j*SMALL_ARG_SIZE+SELF] = tmp["self"];
                  ret[fluff[i]][SINGLE][ARGUMENTS][j*SMALL_ARG_SIZE+REST] = tmp["rest"];
                  ret[fluff[i]][SINGLE][ARGUMENTS][j*SMALL_ARG_SIZE+ARGS] = explode(tmp["arguments"], ",");
                  ret[fluff[i]][SINGLE][ARGUMENTS][j*SMALL_ARG_SIZE+POSITION_SINGLE] = tmp["position"];
               }
            }
         }
      }
      if (comms[fluff[i]]["targeted"]) {
/* Yes! */
         if (comms[fluff[i]]["targeted"][0]["no-arguments"]) {
            tmp = comms[fluff[i]]["targeted"][0]["no-arguments"][0];   
            if (!tmp["self"]) {
               notify("The 'self' type is missing in the no-argument, target for the soul command "+fluff[i]+"\n");
               failed = 1;
            } else if (!tmp["rest"]) {
               notify("The 'rest' type is missing in the no-argument, target for the soul command "+fluff[i]+"\n");
               failed = 1;
            } else {
               ret[fluff[i]][TARGET] = ({ ({ tmp["self"], tmp["rest"],
                                     tmp["target"], 0, tmp["force"], tmp["position"] }),
                                                         0 });
            }
         }
         if (comms[fluff[i]]["targeted"][0]["arguments"]) {
            j = sizeof(comms[fluff[i]]["targeted"][0]["arguments"]);
            if (!ret[fluff[i]][TARGET]) 
               ret[fluff[i]][TARGET] = ({ 0, allocate(j*ARG_SIZE) });
            else
               ret[fluff[i]][TARGET][ARGUMENTS] = allocate(j*ARG_SIZE);
            for (j=0;j<sizeof(comms[fluff[i]]["targeted"][0]["arguments"]);j++) {
               tmp = comms[fluff[i]]["targeted"][0]["arguments"][j];
               if (!tmp["self"]) {
                  notify("The 'self' type is missing in the argument, target for the soul command "+fluff[i]+"\n");
                  failed = 1;
               } else if (!tmp["rest"]) {
                  notify("The 'rest' type is missing in the argument, target for the soul command "+fluff[i]+"\n");
                  failed = 1;
               } else if (!tmp["arguments"]) {
                  notify("The 'arguments' type is missing in the argument, target for the soul command "+fluff[i]+"\n");
                  failed = 1;
               } else {
                  ret[fluff[i]][TARGET][ARGUMENTS][j*ARG_SIZE+SELF] = tmp["self"];
                  ret[fluff[i]][TARGET][ARGUMENTS][j*ARG_SIZE+REST] = tmp["rest"];
                  ret[fluff[i]][TARGET][ARGUMENTS][j*ARG_SIZE+TARG] = tmp["target"];
                  ret[fluff[i]][TARGET][ARGUMENTS][j*ARG_SIZE+ARGS] = explode(tmp["arguments"], ",");
                  ret[fluff[i]][TARGET][ARGUMENTS][j*ARG_SIZE+FORCE] = tmp["force"];
                  ret[fluff[i]][TARGET][ARGUMENTS][j*ARG_SIZE+POSITION] = tmp["position"];
               }
            }
         }
      }
   }
   fluff = keys(ret);
   for (i=0;i<sizeof(fluff);i++) {
      ret[fluff[i]][PATTERNS] = sort_patterns(ret[fluff[i]][PATTERNS]);
      SOUL_OBJECT->add_soul_command(fluff[i], ret[fluff[i]]);
      notify("Added soul command "+fluff[i]+".\n");
   }
} /* make_into_soul_commands() */

/** @ignore yes */
void blue() {
   printf("%O\n", sort_patterns(({ "[at] <indirect:object>", "<string>" })));
} /* blue() */
