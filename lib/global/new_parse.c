/*   -*- LPC -*-   */
/*
 * $Locker:  $
 * $Id: new_parse.c,v 1.81 2003/04/03 11:33:55 taffyd Exp $
 */
/**
 * This file contains all the code to support and run the text parsing
 * system used by discworld.   This is called 'add_command', please see
 * help on add_command for a more detailed listing.
 * @author Pinkfish
 */
#include <soul.h>
#include <creator.h>
#include <user_parser.h>
#include <command.h>
#include <obj_parser.h>
#include <function.h>

inherit "/global/command";

#define NEW_SOUL

#define MY_MESS_HEADER "#!"
#define OTHER_MESS_HEADER "!#"

class fail_mess_data {
    object* direct;
    object* indirect;
    int weight;
}

private nosave mapping _succ_mess;
private nosave mapping _fail_mess;
private nosave mapping _cur_objects;
private nosave object *_succ_indir;
private nosave object *_succ_mess_dir;
private nosave object *_succ_mess_indir;
private nosave mapping _commands;
private nosave string *_failed_mess;
private nosave string _curpat;

int *pattern_match(string *bits, mixed *pattern);
private int handle_command(string *bits, int *matches, mixed *pattern,
                            mixed *command, string pattern_str);
string get_fail_messages(string verb, object *fail_obs);
void print_special_messages(string verb);
string *query_word_list(string bing);
private void remove_object2(object ob, int flag);
private void remove_object_force(object ob);
varargs string create_message(string *bits, int *matches, mixed *pattern,
                                             object *dir_obs, int flag);
int syntax_messages(string str);
string query_name();

void create() {
   _commands = ([ ]);
   _fail_mess = ([ ]);
   _succ_mess = ([ ]);
   _cur_objects = ([ ]);
   _succ_indir = _succ_mess_dir = _succ_mess_indir = ({ });
   command::create();
} /* create() */

/**
 * This method returns the current internal set of commands.
 * <p>
 * ([ "command_name" :<br>
 * ({ ({ pattern_weight, pattern_str, nn, object, function }) })<br>
 * ])
 * @return the current commands list
 * @see query_p_objects()
 */
mapping query_p_commands() { return copy( _commands ); }
/**
 * This method returns the current mapping between objects and commands.<br>
 * ([ object : ({ "cmd1", "cmd2", ... }), ... ])
 * <p>
 * This mapping is used when the object leaves the environment to make
 * the command updating more efficent.
 * @return the current object/command mapping
 * @see query_p_commands()
 */
mapping query_p_objects() { return copy( _cur_objects ); }

/**
 * This method returns the information associated with the specific
 * command.  This should only be used for debug.
 * @param name the command name to return info on
 * @return the information associated with the command
 */
mixed *query_parse_command(string name) { return _commands[name]; }

/**
 * This method returns the objects associated with the parse command.
 * @param name the command name to return the objects for
 * @return the objects associated with the command
 */
object* query_parse_command_objects(string name) {
   class command_class command;
   class command_data command_data;
   string pattern;
   object* obs;

   command = _commands[name];
   obs = ({ });
   if (command) {
      foreach (pattern, command_data in command->patterns) {
         obs += filter(command_data->calls, (: objectp($1) :));
      }
   }
   return obs;
} /* query_parse_command_objects() */

/**
 * This method returns all the indirect objects used in the success
 * messages.
 * @return the success message
 */
object* query_succ_mess_indir() {
    return _succ_mess_indir;
} /* query_succ_mess_indir() */

/** @ignore yes */
void parser_commands() {
#if efun_defined(add_action)
   add_action("new_parser", "*", -2);
#endif
   add_command("syntax", "<word'verb'>", (:syntax_messages($4[0]):));
} /* parser_commands() */

/**
 * This is called by the object the command is being passed on to find
 * whether or not it succeeded on the objects it was passed... and which
 * ones. This can be passed an object.. or an array of objects.
 * Share and enjoy.
 */
int add_succeeded(mixed ob) {
   int i;

   if (!pointerp(_succ_indir)) _succ_indir = ({ });
   if (objectp(ob)) {
      if (member_array(ob, _succ_indir) == -1) {
         _succ_indir += ({ ob });
      } else {
         return 1;
      }
   } else if (!pointerp(ob)) {
      return 0;
   } else for (i=0;i<sizeof(ob);i++) {
      if (member_array(ob[i], _succ_indir) == -1) {
         _succ_indir += ({ ob[i] });
      }
   }
   return 1;
} /* add_succeeded() */

/* Mess can also be an array, containing two elements. */
int add_succeeded_mess(object dir, mixed incoming_mess, object *in_dir) {
   string my_mess, other_mess;

   if (!pointerp(incoming_mess)) {
      if (stringp(incoming_mess)) {
         my_mess = MY_MESS_HEADER+incoming_mess;
         other_mess = OTHER_MESS_HEADER+incoming_mess;
      } else {
         write("Parameter to add_succeeded_mess() must be a string or "
                  "array.\n");
         return 0;
      }
   } else if (sizeof(incoming_mess) == 2) {
      my_mess = MY_MESS_HEADER+incoming_mess[0];
      other_mess = OTHER_MESS_HEADER+incoming_mess[1];
   } else {
      write("Message array to add_succeeded_mess() must be two long.\n");
      return 0;
   }

   if ( undefinedp( in_dir ) ) {
       in_dir = ({ });
   }

   if (!_succ_mess[my_mess]) {
      _succ_mess[my_mess] = ({ ({ dir }), in_dir });
   } else {
      if (member_array(dir, _succ_mess[my_mess][0]) == -1) {
         _succ_mess[my_mess][0] += ({ dir });
      }
      in_dir = in_dir - _succ_mess[my_mess][1];
      _succ_mess[my_mess][1] += in_dir;
   }

   if (!_succ_mess[other_mess]) {
      _succ_mess[other_mess] = ({ ({ dir }), in_dir });
   } else {
      if (member_array(dir, _succ_mess[other_mess][0]) == -1) {
         _succ_mess[other_mess][0] += ({ dir });
      }
      in_dir = in_dir - _succ_mess[other_mess][1];
      _succ_mess[other_mess][1] += in_dir;
   }
   if (member_array(dir, _succ_mess_dir) == -1) {
      _succ_mess_dir += ({ dir });
   }
   return 1;
} /* add_succeeded_mess() */

void add_failed_mess( object dir, string mess, mixed *in_dir ) {
   int i;
   class fail_mess_data fail;

   if (!stringp(mess)) {
      write("Parameter to add_failed_mess() must be a string.\n");
      return;
   }

   if ( undefinedp( in_dir ) ) {
      in_dir = ({ });
   }
   if (!_fail_mess[mess]) {
      fail = new(class fail_mess_data);
      fail->direct = ({ dir });
      fail->indirect = in_dir;
      _fail_mess[mess] = fail;
   } else {
      if (member_array(dir, _fail_mess[mess]->direct) == -1) {
         _fail_mess[mess]->direct += ({ dir });
      }
      for (i=0;i<sizeof(in_dir);i++) {
         if (member_array(in_dir[i], _fail_mess[mess]->indirect) == -1) {
            _fail_mess[mess]->indirect += in_dir[i..i];
         }
      }
   }
   if (member_array(dir, _succ_mess_indir) == -1) {
      _succ_mess_indir += ({ dir });
   }
} /* add_failed_mess() */

/**
 * This method returns the objects which have success messages already
 * attached for.   This allows you to determine which objects already hace
 * a success message available.   This array is added to by both the
 * add_succeeded_mess and add_failed_mess methods, it disable the
 * autogeneration of these messages.
 * @return the succeeded message objects
 * @see add_succeeded_mess()
 * @see add_failed_mess()
 */
object *query_succ_mess_dir() {
    return _succ_mess_dir;
} /* query_succ_mess_dir() */

/**
 * This method checks to see if the given object has already added a failed
 * message yet or not.   This is checking for a direct object, not an 
 * indirect object.
 * @param dir the object adding the failed message
 * @return 0 if not found, 1 if found
 * @see add_failed_mess()
 */
int query_failed_message_exists(object dir) {
    string mess;
    class fail_mess_data stuff;

    foreach (mess, stuff in _fail_mess) {
         if (member_array(dir, stuff->direct) != -1) {
             return 1;
         }
    }
    return 0;
} /* query_failed_message_exists() */

/**
 * The id is a useful thingy so that things can remember which pattern was 
 * parsed.
 */
varargs int add_command(string cmd, object ob, mixed format, function funct) {
   int i;
   int idx;
   class command_class command;
   class command_data command_data;

   if ((funct && !functionp(funct)) || !objectp(ob)) {
      return 0;
   }

   if (!format) {
      format = "<direct:object>";
   } else if (pointerp(format)) {
      for (i = 0; i < sizeof(format); i++) {
         add_command(cmd, ob, format[i], funct);
      }
      return 1;
   } else if (!stringp(format)) {
      return 0;
   }
   command = _commands[cmd];
   if (!command) {
      command = new(class command_class);
      command->patterns = ([ ]);
      command->order = 0;
      _commands[cmd] = command;
   }
   command_data = command->patterns[format];
   if (command_data) {
      /* Its already in there...   Easy. */
      /* Let's check whether the object is already in there. It shouldn't
          be, but it doesn't hurt :-) */
      idx = member_array(ob, command_data->calls);
      if (idx != -1) {
         command_data->calls[idx + 1] = funct;
      } else {
         command_data->calls += ({ ob, funct });
      }
   } else {
      /*
       * Now. We need to use the weight information to place the
       * pattern into the array.
       */
      command_data = new(class command_data);
      command_data->calls = ({ ob, funct });
      command->order = 0;
      command->patterns[format] = command_data;
   }

   if (_cur_objects[ob]) {
      _cur_objects[ob] |= ({ cmd });
   } else {
      _cur_objects[ob] = ({ cmd });
   }
   return 1;
} /* add_command() */


/*
 * This is the bit that handles object leaving the environment of the
 * player.
 */
int remove_object(mixed ob, int was_env) {
   if (objectp(ob) && !_cur_objects[ob] && !was_env) {
      return 0;
   }
   /* called out for umm, eval reasons. */
   /* Hm... This doesn't really work... */
   remove_object2(ob, was_env);
   return 1;
} /* remove_object() */

/**
 * This method does all the horrible work of removing objects and being
 * evil.
 * The bit that does all the horrible work...
 * We check in here to make sure we havent come back to the same spot.
 * This is so we don't go round deleteing things we shouldn't.
 */
private void remove_object2(object ob, int was_env) {
   object womble;
   object *inv_match;

   /*
    * Either we are in the same room, or the object exists in our inventory
    * or in the inventory of the room we are in...
    */
   if (!ob ||
       (!was_env && (environment() == ob ||
                     environment(ob) == this_object() ||
                     environment(ob) == environment()))) {
      return ;
   }
   if (was_env == 1) {
      remove_object_force(ob);

      inv_match = ob->find_inv_match();

//    This is because not every object supports find_inv_match.
//    Especially teh big monies.
//    foreach (womble in ob->find_inv_match()) {
      if ( arrayp( inv_match ) ) {
        foreach (womble in inv_match) {
           if (objectp(womble) && womble != this_object() &&
               _cur_objects[womble]) {
              remove_object_force(womble);
           }
        }
      }
      return ;
   }
   return ;
} /* remove_object2() */

/**
 * This method will remove a lost object and all zeroed objects for a
 * specific command.
 */
private void remove_from_command(string cmd, object ob) {
   int k;
   string format;
   class command_data command_data;

   if (!_commands[cmd]) {
      return ;
   }
   foreach (format, command_data in _commands[cmd]->patterns) {
      for (k = 0; k < sizeof(command_data->calls); k += 2) {
         if (!command_data->calls[k] || command_data->calls[k] == ob) {
            command_data->calls = command_data->calls[0..k - 1] + command_data->calls[k + 2..];
         }
      }
      if (!sizeof(command_data->calls)) {
         map_delete(_commands[cmd]->patterns, format);
         if (_commands[cmd]->order) {
            _commands[cmd]->order -= ({ format });
         }
      }
   }
   if (!sizeof(_commands[cmd]->patterns)) {
      map_delete(_commands, cmd);
   }
} /* remove_from_command() */

/**
 * This will force the removal of the specified object from the array.
 */
private void remove_object_force(object ob) {
   string ind;

   /* Check to see if our illustious object exists or not. */
   if (!_cur_objects[ob]) {
      return 0;
   }
   foreach (ind in _cur_objects[ob]) {
      remove_from_command(ind, ob);
   }
   /* Delete the object from the object table */
   map_delete(_cur_objects, ob);
} /* remove_object2() */

/* The dest event :) */
/** @ignore yes */
void event_dest_me(object ob) {
   /*
    * Don't call this out as otherwise we end up with a 0 which may mess up
    * the mapping a bit.
    */
   remove_object_force(ob);
} /* event_dest_me() */

/*
 * Handle leaving.   Check to see if it is me leaving.   If it is, then
 * remove all the objects from the inventory.
 */
/** @ignore yes */
void event_exit(object ob, string mess, object dest) {
   if (dest == this_object() || dest == environment()) {
      return ;
   }
   remove_object_force(ob);
} /* event_exit() */

/** @ignore yes */
void me_moveing(object from) {
   /* Use this so as to get the hidden objects as well. */
   if (from) {
      remove_object(from, 1);
   }
} /* me_moveing() */

/**
 * This method does all the real work for add_command parsing.
 */
nomask int new_parser(string str) {
   string *bits;
   string pattern_str;
   string format;
   mixed *wombat;
   mixed *soul_stuff;
   mixed *pattern;
   mixed *command_stuff;
   mixed *stuff;
   int i;
   int j;
   int ret;
   int flag;
   class command_class command;
   class command_data command_data;

   /*
    * First explode the input string into words.   The first word is the
    * command.
    *
    * Test to see if it exists...
    */

   bits = explode(str, " ") - ({ "", 0 });
   command_stuff = cmdPatterns(bits[0]);
   if (this_object()->command_shadowed(bits[0], implode(bits[1..], " "))) {
      return 1;
   }
   command = _commands[bits[0]];
//printf("Cmd: %O %O\n", command, bits[0]);
#ifdef NEW_SOUL
   if (!command && !sizeof(command_stuff)) {
      soul_stuff = SOUL_OBJECT->query_soul_command(bits[0]);
      if (!soul_stuff) {
         return 0;
      }
   }
#else
   if (!_commands[bits[0]] && !sizeof(command_stuff)) {
      return 0;
   }
#endif
   /* Only the word... */
   _failed_mess = ({ "", "" });

  /*
   * This allows other code to find out what verb has been used,
   * since query_verb() will always return "" (verb is *).
  */
    current_verb = bits[ 0 ];

  /*
   * Ok, we have found a command.   There may be one or more commands
   * stored in this array.   Starting from the first one we check to
   * see if our pattern matches.
   */
   if (!soul_stuff) {
      if (command) {
         if (!command->order) {
            // Make sure all the weights are correct.
            foreach (pattern_str, command_data in command->patterns) {
               if (!command_data->weight) {
                  pattern =  PATTERN_OB->query_pattern(pattern_str);
                  if (pattern) {
                     command_data->weight = pattern[0];
                  } else {
                     map_delete(command->patterns, pattern_str);
                  }
               }
            }
            command->order = sort_array(keys(command->patterns),
                                   (: ((class command_data)$3[$2])->weight -
                                      ((class command_data)$3[$1])->weight :),
                                   command->patterns);
         }
         format = command->order[0];
         command_data = command->patterns[format];
      } else {
         command = new(class command_class);
         command->order = ({ });
         command->patterns = ([ ]);
      }
//printf("Blue: %O %O\n", command, command_stuff);
      for (i = 0, j = 0; i < sizeof(command->order) ||
                         j < sizeof(command_stuff); ) {
         if (i < sizeof(command->order)) {
            format = command->order[i];
            command_data = command->patterns[format];
         } else {
            command_data = 0;
         }
         /*
          * Check both patterns groups to see which should go first.
          * It at the same level, the ones setup elsewhere should have
          * precedence
          */
//printf("Womble: %O %O\n", command_stuff, command_data);
         if (j < sizeof(command_stuff) &&
             (!command_data ||
              command_data->weight < command_stuff[j][PATTERN_WEIGHT])) {
            pattern_str = command_stuff[j][PATTERN_STRING];
            stuff = command_stuff[j++][OBJS..];
            flag = 1;
         } else {
            pattern_str = format;
            flag = 0;
            stuff = command_data->calls;
            i++;
         }
         pattern = PATTERN_OB->query_pattern(pattern_str);
         if (!pattern) {
            continue;
         }
         wombat = pattern_match(bits, pattern);
         //
         // If we match check for anything else that matches to see if we
         // should continue to womble.
         //
         if (wombat && (ret = handle_command(bits, wombat, pattern, stuff,
                                             pattern_str))) {
            if (ret == -1) {
               if (flag) {
                  j = 100;
               } else {
                  i = 100;
               }
            } else {
               return 1;
            }
         }
         if (wombat && !sizeof(wombat)) {
            int cont;
            string pat;
            mixed *junk;

            if (_curpat[<2] == '}' || _curpat[<2] == ']') {
               if (_curpat[<2] == '}') {
                  _curpat = _curpat[0..strsrch(_curpat, '{', -1)-2];
               } else {
                  _curpat = _curpat[0..strsrch(_curpat, '[', -1)-2];
               }
            } else {
               _curpat = implode((explode(_curpat, " ") - ({0,""}))[0..<2], " ");
            }
            if (flag) {
               foreach (junk in command_stuff[j..]) {
                  pat = junk[PATTERN_STRING];
                  pat = replace(pat, ({":object", "", ":living", "",
                                                 ":any-living", "", ":distant-living", "",
                                                 ":here", "", ":me>", ">", ":here-me", "",
                                                 ":me-here", "" }));
                  if (_curpat == pat) {
                     cont = 1;
                     break;
                  }
               }
               if (!cont) {
                  j = 100;
               }
            } else {
               foreach (pat in command->order[i..]) {
                  pat = junk[PATTERN_STRING];
                  pat = replace(pat, ({":object", "", ":living", "",
                                                 ":any-living", "", ":distant-living", "",
                                                 ":here", "", ":me>", ">", ":here-me", "",
                                                 ":me-here", "" }));
                  if (_curpat == pat) {
                     cont = 1;
                     break;
                  }
               }
               if (!cont) {
                  i = 100;
               }
            }
         }
      }
   }

#ifdef NEW_SOUL
   // Only check for soul commands if we return 0. 
   if ( !ret ) { 
      if (!soul_stuff) {
         soul_stuff = SOUL_OBJECT->query_soul_command(bits[0]);
      }

      for (i=0;i<sizeof(soul_stuff);i++) {
         pattern = PATTERN_OB->query_pattern(soul_stuff[i][PATTERN_STRING]);
         wombat = pattern_match(bits, pattern);
         if (wombat && handle_command(bits, wombat, pattern, soul_stuff[i][OBJS..],
                                      soul_stuff[i][PATTERN_STRING])) {
            return 1;
         }
      }
   }
#endif
/* This will construct useful error messages. */
   if (_failed_mess[1] == "") {
      if (!query_notify_fail()) {
         if (_failed_mess[0] == "") {
            notify_fail( "See \"syntax "+ bits[ 0 ] +
                  "\" for the input patterns.\n");
         } else {
            notify_fail( _failed_mess[0] );
         }
      }
   } else {
      notify_fail(_failed_mess[1]);
   }
   _fail_mess = ([ ]);
   return 0;
} /* new_parser() */

int syntax_messages(string str) {
   string the_mess;
   int i;
   string tmp;
   class command_data fluff;
   mixed *soul_stuff;
   mixed *command_stuff;

   if (!str) {
      notify_fail("Syntax: syntax <verb>\n");
      return 0;
   }
   command_stuff = cmdPatterns(str);
#ifdef NEW_SOUL
   soul_stuff = SOUL_OBJECT->query_soul_command(str);
   if (!_commands[str] && !soul_stuff && !sizeof(command_stuff)) {
      notify_fail("Could not find the verb '"+str+"'.\n");
      return 0;
   }
#else
   if (!_commands[str] && !sizeof(command_stuff)) {
      notify_fail("Could not find the verb '"+str+"'.\n");
      return 0;
   }
#endif

   the_mess = "Forms of syntax available for the command \""+ str +"\":\n";
   if (_commands[str]) {
      foreach (tmp, fluff in _commands[str]->patterns) {
         the_mess += str+" "+PATTERN_OB->query_short_pattern(tmp) + "\n";
      }
   }
   for (i=0;i<sizeof(command_stuff);i++) {
       the_mess += str+" "+PATTERN_OB->query_short_pattern(
                                        command_stuff[i][PATTERN_STRING]) + "\n";
   }
   for (i=0;i<sizeof(soul_stuff);i++) {
       the_mess += str+" "+PATTERN_OB->query_short_pattern(
                                        soul_stuff[i][PATTERN_STRING]) + "\n";
   }
   write(the_mess);
   return 1;
} /* syntax_messages() */

int *pattern_match(string *bits, mixed *pattern) {
   string *elms;
   int pos, *delayed, last, failed, j, opt, i, word_offset, spaces, wcount,
      *matches;
   mixed tmp;

   /*
    * Ok, we need to check to see if the nice pattern we have in
    * comm exists somewhere in bits.
    */
   pos = wcount = 1;
   _curpat = "";
   matches = ({ 0 });
   delayed = ({ });
   for (i = 1; i < sizeof(pattern) && !failed; i++) {
      /* Ok, check matching rules. */
      if (pos >= sizeof(bits))
         failed = 1;
      /* else */
      _curpat += PATTERN[pattern[i]];
      switch (pattern[i]) {
         case DIRECT_OBJECT :
         case INDIRECT_OBJECT :
            i++; /* Move to the type marker. */
            if (pattern[i] == TARGET_PLAYER) {
               if (last) {
                  delayed += ({ SINGLE_WORD });
                  word_offset++;
                  pos++; /* Push the search position forward one. 
                                 It needs to be there at least */
               } else {
                  pos++;
                  matches += ({ pos - 1 });
               }
               failed |= pos > sizeof(bits);
            } else {
               /*
                * We don't know where this one ends, so we don't put a match onto the
                * thingy.
                */
               if (last) {
                  delayed += ({ SINGLE_WORD });
               } else {
                  delayed = ({ STRING });
                  last = FIND_FIRST;
               }
               word_offset++;
               pos++; /* All strings must be at least 1 word long */
            }
            i++; /* Skip the environment marker */
            break;
         case STRING :
            /* This here delayed should not exist! */
            if (last) {
               pos -= word_offset-1;
               for (j=0;j<sizeof(delayed);j++) {
                  switch (delayed[j]) {
                     case STRING :
                        matches += ({ pos - 1 });
                        pos++;
                        break;
                     case OPTIONAL :
                        matches += ({ matches[<1] });
                        break;
                     case SINGLE_WORD :
                        matches += ({ pos - 1 });
                        pos++;
                        break;
                  }
               }
            }
            delayed = ({ STRING });
            word_offset = 1;
            pos++; /* Strings should be at least one word long */
            last = FIND_LAST;
            break;
         case QUOTED_STRING :
            if (last) {
                while (pos < sizeof(bits) &&
                           bits[pos][0] != '"' &&
                           bits[pos][0] != '\'' &&
                           bits[pos][0] != '`') {
                     pos++;
                }
                if (pos < sizeof(bits)) {
                     pos -= word_offset;
                     for (j = 0; j < sizeof(delayed); j++) {
                         switch (delayed[j]) {
                              case STRING :
                                  matches += ({ pos });
                                  pos++;
                                  break;
                              case OPTIONAL :
                                  matches += ({ matches[<1] });
                                  break;
                              case SINGLE_WORD :
                                  matches += ({ pos });
                                  pos++;
                                  break;
                         }
                     }
                     last = FIND_NONE;
                } else {
                     failed = 1;
                }
            }
            if (pos > sizeof(bits)) {
                failed = 1;
            }
            if (!failed) {
                switch (bits[pos][0]) {
                     case '"' :
                     case '\'' :
                     case '`' :
                         /* Ok.   See if we can find the end... */
                         j = pos;
                         while (j < sizeof(bits) &&
                                    bits[j][<1] != bits[pos][0]) {
                              j++;
                         }
                         if (j < sizeof(bits)) {
                              matches += ({ j });
                              pos = j + 1;
                         } else {
                              failed = 1;
                         }
                         break;
                     default :
                         failed = 1;
                         break;
                }
            }
            break;
         case SHORT_STRING :
            if (last) {
               delayed += ({ SINGLE_WORD });
            } else {
               delayed = ({ STRING });
            }
            word_offset++;
            pos++; /* Strings have to be at least one word */
            last = FIND_FIRST;
            break;
         case SINGLE_WORD :
            /* The last bit carrys through here too */
            if (last) {
               /* Some sort of delay */
               /*
                * Hmm, need to keep track of the delayed and single_word status of 
                * each bit we push 
                */
               delayed += ({ SINGLE_WORD });
               word_offset++;
               pos++; /* Push the search position forward one.   It needs to be there 
                              at least */
            } else {
               /*
                * Means the word we are pointing at must be the one we want.
                * Womble!
                */
               matches += ({ pos });
               pos++;
            }
            failed |= pos > sizeof(bits);
            break;
         case NUMBER :
         case FRACTION :
            failed = 1;
            if (last == FIND_LAST) {
               for (j = sizeof(bits) - 1; j >= pos; j--)
                  if ((bits[j][0] >= '0' && bits[j][0] <= '9') ||
                        (pattern[i] != FRACTION && bits[j][0] == '-' &&
                         bits[j][1] >= '0' && bits[j][1] <= '9')) {
                     /* Found number! */          
                     if (pattern[i] != FRACTION || sizeof(explode(bits[j], "/")) > 1) {
                        failed = 0;
                        /* We always point to the next match. */
                        pos = j + 1;
                        break;
                     }
                  }
            } else if (last == FIND_FIRST) {
               for (j = pos; j < sizeof(bits); j++)
                  if ((bits[j][0] >= '0' && bits[j][0] <= '9') ||
                        (pattern[i] != FRACTION && bits[j][0] == '-' &&
                         bits[j][1] >= '0' && bits[j][1] <= '9')) {
                     /* Found number! */          
                     if (pattern[i] != FRACTION || sizeof(explode(bits[j], "/")) > 1) {
                        failed = 0;
                        /* We always point to the next match. */
                        pos = j + 1;
                        break;
                     }
                  }
            } else {
               if (pos < sizeof(bits) &&
                     ((bits[pos][0] >= '0' && bits[pos][0] <= '9') ||
                        (pattern[i] != FRACTION && bits[pos][0] == '-' &&
                         bits[pos][1] >= '0' && bits[pos][1] <= '9'))) {
                  failed = 0;
                  pos++;
               } else {
                  failed = 1;
               }
            }
            if (!failed) {
               if (sizeof(delayed)) {
                  pos -= word_offset; /* Amount which is needed at least to handle the
                                                    pattern */
                  for (j=0;j<sizeof(delayed);j++) {
                     switch (delayed[j]) {
                        case STRING :
                           matches += ({ pos - 1 });
                           pos++;
                           break;
                        case OPTIONAL :
                           matches += ({ matches[<1] });
                           break;
                        case SINGLE_WORD :
                           matches += ({ pos - 1 });
                           pos++;
                           break;
                     }
                  }
                  delayed = ({ });
                  word_offset = 0;
               }
               last = FIND_NONE;
               matches += ({ pos - 1 });
            }
            break;
         case OPTIONAL_SPACES :
         case OPTIONAL :
         case WORD_LIST_SPACES :
            switch (pattern[i]) {
               case OPTIONAL_SPACES :
                  spaces = opt = 1;
                  break;
               case OPTIONAL:
                  opt = 1;
                  break;
               case WORD_LIST_SPACES :
                  spaces = 1;
                  break;
            }
         case WORD_LIST :
            /* Find word list. The next element is the list name */
            /*
             * How do we find a list?   Big question?
             *
             * Currently thought of method.   Use the array subtraction
             * operation to find if there is any intersection.
             */
            /*
             * Method 2:
             * Use member_array a lot.
             */
            if (pointerp(pattern[++i])) {
               string *words;
   
               if (spaces) {
                  tmp = "";
   
                  foreach (words in pattern[i])
                     tmp += implode(words, " ") + "|";
                  tmp = tmp[0..<2];
               }
               _curpat += (sizeof(pattern[i]) > 1?"{":"");
               _curpat += (spaces ? tmp : implode(pattern[i], "|")) +
                  (opt?"] ":(sizeof(pattern[i]) > 1?"} ":" "));
               elms = pattern[i];
            } else {
               _curpat += pattern[i] + (opt?"] ":"} ");
               elms = (string *)master()->query_word_list(pattern[i]);
               if (!elms) {
                  /* Could be a local word list then? */
                  elms = query_word_list(pattern[i]);
               }
            } 
            if (!pointerp(elms) || !sizeof(elms))
               failed = 1;
            else {
               if (!(last || failed || spaces)) {
                  /* Means that the word we are pointing at must be one of em! */
                  tmp = member_array(bits[pos], elms);
                  if (tmp == -1) {
                     failed = 1;
                  }
               } else if (sizeof(elms) == 1 && last == FIND_FIRST && !spaces) {
                  /* Only one word.   Definately quicker to do a member_array */
                  tmp = member_array(elms[0], bits[pos..]);
                  if (tmp != -1) {
                     pos += tmp;
                  } else {
                     failed = 1;
                  }
               } else if (!spaces) {
                  tmp = bits[pos..] - elms;
                  if (sizeof(tmp) < sizeof(bits)-pos) {
                     /* Ok, one exists... */
                     if (last == FIND_FIRST) {
                        for (j = 0; (j+pos) < sizeof(bits) && j < sizeof(tmp) &&
                                  bits[j+pos] == tmp[j]; j++);
                        pos += j;
                     } else {
                        int k;
   
                        for (j = sizeof(tmp)-1, k = sizeof(bits)-1;
                               j >= 0 && bits[k] == tmp[j]; j--, k--);
                        pos = k;
                     }
                  } else {
                     failed = 1;
                  }
               } else {
                  string *elem;
                  int success;
   
                  foreach(elem in elms) {
                     if (!last) {
                        /* Means that the words we are pointing at must be one of em! */
                        if (implode(bits[pos..pos+sizeof(elem)-1], " ") ==
                              implode(elem, " ")) {
                           success = 1;
                           wcount = sizeof(elem);
                           break;
                        }
                        failed = 1;
                     } else {
                        tmp = bits[pos..] - elem;
                        if (sizeof(tmp) <= (sizeof(bits) - pos - sizeof(elem))) {
                           /* Okay, one exists... */
                           success = 1;
                           wcount = sizeof(elem);
                           if (last == FIND_FIRST) {
                              for (j = 0; (j+pos) < sizeof(bits) && j < sizeof(tmp) &&
                                        bits[j+pos] == tmp[j]; j++);
                              pos += j;
                              break;
                           } else {
                              int k;
   
                              for (j = sizeof(tmp)-1, k = sizeof(bits)-1;
                                     j >= 0 && bits[k] == tmp[j]; j--, k--);
                              pos = k - wcount + 1;
                              break;
                           }
                        } else {
                           failed = 1;
                        }
                     }
                  }
                  if (success) {
                     failed = 0;
                  }
               }
            }
            if (opt && failed) {
               failed = 0;
               if (!last)
                  matches += ({ pos - 1 }); // Keep the -1 for the optional stuff..?
               else {
                  delayed += ({ OPTIONAL });
                  opt = 0;
                  spaces = 0;
                  break;
               }
            } else {
               if (!failed) {
                   pos += wcount;
               }
               if (sizeof(delayed) && !failed) {
                  pos -= word_offset; /* Amount which is needed at least to handle the
                                                    pattern */
                  for (j=0;j<sizeof(delayed);j++) {
                     switch (delayed[j]) {
                        case STRING :
                           matches += ({ pos - 1 });
                           pos++;
                           break;
                        case OPTIONAL :
                           matches += ({ matches[<1] });
                           break;
                        case SINGLE_WORD :
                           matches += ({ pos - 1 });
                           pos++;
                           break;
                     }
                  }
                  delayed = ({ });
                  word_offset = 0;
               }
               /* Only matching one word... So the next item must be ours. */
               if (!failed) {
                  last = FIND_NONE;
                  //matches[0] = 1;
                  matches += ({ pos - 1 });
                  //pos += wcount;
               }
               wcount = 1;
               delayed = ({ });
            }
            opt = 0;
            spaces = 0;
            last = FIND_NONE;
            break;
      }
   }
   if (sizeof(delayed)) {
      /* Force to the end of the line... */
      if (/*sizeof(bits)-word_offset >= pos*/ 1) {
         pos = sizeof(bits) + 1;
         pos -= word_offset; /* Amount which is needed at least to handle the 
                                           pattern.   Last pos should be the end thingy. */
         for (j=0;j<sizeof(delayed);j++) {
            switch (delayed[j]) {
               case OPTIONAL :
                  matches += ({ matches[<1] });
                  break;
               case SINGLE_WORD :
               case STRING :
                  matches += ({ pos - 1 });
                  pos++;
                  break;
            }
         }
         delayed = ({ });
         word_offset = 0;
         pos = sizeof(bits);
      } else
         pos = -1;
   }
   matches += ({ sizeof(bits) });
   if (failed || pos != sizeof(bits)) {
//      if (matches[0] == 1)
//         return ({});
      return 0;
   }
   return matches + ({ sizeof(bits)+1 });
} /* pattern_match() */

int check_living(object ob) {
   return living(ob);
} /* check_living() */

int check_if_creator(object ob) {
   return (int)ob->query_creator() &&
      !(ob == this_player() || ob->query_invis());
} /* check_if_creator() */

int check_if_allowed(object ob) {
   return ob != this_player() && reference_allowed(ob);
} /* check_if_allowed() */

class obj_match my_find_match(string pattern, object *where, int type) {
    object ob;
    class obj_match omatch;

    where = copy(where);
    foreach (ob in where) {
         if (ob && (!ob->query_closed() ||
                        (ob->query_closed() && !ob->query_property("opaque")))) {
             if (ob->query_mirror_room()) {
                  where += ({ ob->query_mirror_room() });
             }
         } else {
             where -= ({ ob });
         }
    }
/*
    where = filter(copy(where), (: $1 && (!$1->query_closed() ||
                                                   ($1->query_closed() &&
                                                    !$1->query_property("opaque"))) :));
 */
    if (!sizeof(where)) {
         omatch = new(class obj_match);
         omatch->text = lower_case(pattern);
         omatch->objects = ({   });
         omatch->result = OBJ_PARSER_BAD_ENVIRONMENT;
         return omatch;
    }
    omatch = (class obj_match)match_objects_in_environments(pattern,
                                          where, type, this_object());
    if (omatch->result != OBJ_PARSER_SUCCESS) {
         ob = find_object(pattern);
         if (ob && member_array(environment(ob), where) != -1) {
             omatch->text = lower_case(pattern);
             omatch->objects = ({ ob });
             omatch->result = OBJ_PARSER_SUCCESS;
         }
    }
    return omatch;
} /* my_find_match() */

private class obj_match match_objects(int type, string pattern, object *env) {
   class obj_match omatch;
   class obj_match new_omatch;
   object* tmp;

   switch (type) {
   case WIZ_PRESENT_TARGET :
      omatch = new(class obj_match);
      omatch->text = pattern;
      omatch->objects = WIZ_PRESENT->wiz_present(pattern, env[0]);
      if (sizeof(omatch->objects)) {
          omatch->result = OBJ_PARSER_SUCCESS;
      } else {
          omatch->result = OBJ_PARSER_NO_MATCH;
      }
      break;
   case ANY_OBJECT :
      omatch = my_find_match(pattern, env, 0);
      break;
   case DISTANT_LIVING :
      omatch = new(class obj_match);
      omatch->text = pattern;
      if ( environment() && !this_object()->query_creator() ) {
         if ( environment()->query_property( "no remote" ) ) {
            omatch->objects = ({ });
            omatch->result = OBJ_PARSER_BAD_ENVIRONMENT;
            break;
         }
      }
      omatch->objects = filter(map(explode(lower_case(pattern), ","),
                  (: find_living( lower_case(this_object()->
            expand_nickname( $1 ))) :) ), (: $1 && check_if_allowed($1) :) );
      omatch->objects = uniq_array(omatch->objects);
      if (sizeof(omatch->objects)) {
          omatch->result = OBJ_PARSER_SUCCESS;
      } else {
          omatch->result = OBJ_PARSER_NO_MATCH;
      }
      break;
   case LIVING :
      omatch = my_find_match(pattern, env, OBJ_PARSER_TYPE_LIVING); 
      tmp = filter(omatch->objects, (: check_living( $1 ) :) ); 
      // See of any of them were not living...
      if (sizeof(omatch->objects) != sizeof(tmp)) {
          if (pattern != "all") {
               omatch->result = OBJ_PARSER_NOT_LIVING;
               omatch->objects -= tmp;
          } else {
               omatch->objects = tmp;
          }
      }
      break;
   case TARGET_PLAYER :
      omatch = new(class obj_match);
      omatch->text = pattern;
      if (environment() && !this_object()->query_creator()) {
         if (environment()->query_property("no remote")) {
            omatch->objects = ({ });
            omatch->result = OBJ_PARSER_BAD_ENVIRONMENT;
            break;
         }
      }

      omatch->objects = filter(map(explode(lower_case(pattern), ","),
                  (: find_player( lower_case(this_object()->
            expand_nickname( $1 ))) :) ), (: $1 && check_if_allowed($1) :) );
      omatch->objects = uniq_array(omatch->objects);

      if (!sizeof(omatch->objects)) {
         if (pattern == "creators" && this_object()->query_creator()) {
            omatch->objects = filter(users(), (: check_if_creator($1) :));
         }
         /*
         if (pattern == "someone") {
            omatch->objects = filter(users(), (: check_if_allowed($1) :));
            if (sizeof(omatch->objects) > 1) {
               omatch->objects = ({ omatch->objects[random(sizeof(omatch->objects))] });
            }
         }
         */
      }
      if (sizeof(omatch->objects)) {
          omatch->result = OBJ_PARSER_SUCCESS;
      } else {
          omatch->result = OBJ_PARSER_NO_MATCH;
      }
      break;
   case ANY_LIVING :
      new_omatch = new(class obj_match);
      new_omatch->text = pattern;
      if ( environment() && !this_object()->query_creator() ) {
         if ( environment()->query_property( "no remote" ) ) {
            new_omatch->objects = ({ });
            new_omatch->result = OBJ_PARSER_BAD_ENVIRONMENT;
            omatch = new_omatch;
         }
      }
      if (new_omatch->result != OBJ_PARSER_BAD_ENVIRONMENT) {
         new_omatch->objects = filter(map(explode(lower_case(pattern), ","),
                                     (: find_player( lower_case(this_object()->
            expand_nickname( $1 ))) :) ), (: $1 && check_if_allowed($1) :) );
         new_omatch->objects = uniq_array(new_omatch->objects);

         if(sizeof(new_omatch->objects)) {
            new_omatch->result = OBJ_PARSER_SUCCESS;
            omatch = new_omatch;
         }
      }
      if(!omatch || omatch->result != OBJ_PARSER_SUCCESS ||
         !sizeof(omatch->objects)) {
         omatch = my_find_match(pattern, env, OBJ_PARSER_TYPE_LIVING);
         tmp = filter(omatch->objects, "check_living", this_object());
         // See of any of them were not living...
         if (sizeof(omatch->objects) != sizeof(tmp)) {
            if (pattern != "all") {
               omatch->result = OBJ_PARSER_NOT_LIVING;
               omatch->objects -= tmp;
            } else {
               omatch->objects = tmp;
            }
         }
      }
      if (omatch->result != OBJ_PARSER_SUCCESS) {
         new_omatch = new(class obj_match);
         new_omatch->text = pattern;
         if (pattern == "creators" && this_object()->query_creator()) {
            new_omatch->objects = filter(users(), (: check_if_creator($1) :));
         }
         if (pattern == "someone") {
            new_omatch->objects = filter(users(), (: check_if_allowed($1) :));
            if (sizeof(new_omatch->objects) > 1) {
               new_omatch->objects = ({ new_omatch->objects[random(sizeof(new_omatch->objects))]});
            }
         }
         if (sizeof(new_omatch->objects)) {
            new_omatch->result = OBJ_PARSER_SUCCESS;
            omatch = new_omatch;
         }
      }
      break;
   }
   if (!omatch) {
      omatch = new(class obj_match);
      omatch->text = pattern;
      omatch->objects = ({ });
      omatch->result = OBJ_PARSER_NO_MATCH;
      return omatch;
   }
   return omatch;
} /* match_objects() */

void setup_failed_mess(class obj_match failed_match) {
    switch (failed_match->result) {
    case OBJ_PARSER_BAD_ENVIRONMENT :
       _failed_mess[0] += "Cannot find \""+ failed_match->text +
                                    "\" here, access is not allowed.\n";
       break;
    case OBJ_PARSER_NOT_LIVING :
       _failed_mess[0] += "The objects \""+
                                    query_multiple_short(failed_match->objects) +
                                    "\" are not living.\n";
       break;
    case OBJ_PARSER_TOO_DARK :
       _failed_mess[0] += "Cannot find \""+ failed_match->text +
                                    "\", it is too dark.\n";
       break;
    default :
       _failed_mess[0] += match_objects_failed_mess(failed_match);
       break;
    }
} /* setup_failed_mess() */

private int handle_command(string *bits, int *matches, mixed *pattern,
                            mixed *command, string pattern_string) {
   int p;
   int failed;
   int i;
   int j;
   int k;
   string dir_match;
   string fail_mess_check;
   string *add_comm_bit;
   string fail_mesg;
   string succ_mesg;
   class obj_match direct_obs;
   object *env;
   object *fail_ob;
   class obj_match failed_match;
   mixed start;
   mixed indirect_obs;
   mixed ret;
   mixed *stuff;
   mixed indir_match;
   mixed bity;
   class obj_match omatch;
   string pattern_str;
   
   /*
    * Ok, we have a match of some sort.   First, do we have any objects
    * to match?   If we do.   Look for them to make sure they
    * really do exist.
    */
   start = 1;
   add_comm_bit = ({ });
   indirect_obs = ({ });
   direct_obs = new(class obj_match);
   direct_obs->objects = ({ });
   if (!sizeof(matches)) {
      return 0;
   }
   for (i=1, p=1;i<sizeof(pattern) && !failed_match;i++,p++) {
      switch (pattern[i]) {
      case DIRECT_OBJECT :
         dir_match = implode(bits[start..matches[p]], " ");
         if (intp(pattern[i+2])) {
             switch (pattern[i+2]) {
                  case ENV_ME :
                     env = ({ this_object() });
                     break;
                  case ENV_HERE :
                     env = ({ environment() });
                     break;
                  case ENV_HERE_ME :
                     env = ({ environment(), this_object() });
                     break;
                  case ENV_ME_HERE :
                     env = ({ this_object(), environment() });
                     break;
             }
         } else if (stringp(pattern[i+2])) {
             ret = find_object(pattern[i+2]);
             if (!ret) {
                  pattern[i+2]->frog_me_a_lot();
                  ret = find_object(pattern[i+2]);
             }
             if (ret) {
                  env = ({ ret });
             }
         }

         direct_obs = match_objects(pattern[++i], dir_match, env);

         i++; /* Skip environment */
         add_comm_bit += ({ dir_match });
         start = matches[p];
         if (direct_obs->result != OBJ_PARSER_SUCCESS) {
            failed_match = direct_obs;
         }
         break;
      case INDIRECT_OBJECT :
         pattern_str = implode(bits[start..matches[p]], " ");
         if (intp(pattern[i+2])) {
             switch (pattern[i+2]) {
             case ENV_ME :
                env = ({ this_object() });
                break;
             case ENV_HERE :
                env = ({ environment() });
                break;
             case ENV_HERE_ME :
                env = ({ environment(), this_object() });
                break;
             case ENV_ME_HERE :
                env = ({ this_object(), environment() });
                break;
             }
         } else if (stringp(pattern[i+2])) {
             ret = find_object(pattern[i+2]);
             if (!ret) {
                  pattern[i+2]->frog_me_a_lot();
                  ret = find_object(pattern[i+2]);
             }
             if (ret) {
                  env = ({ ret });
             }
         }

         if (pattern[i+2] == ENV_DIRECT_OBS) {
            omatch = new(class obj_match);
            omatch->text = pattern_str;
            omatch->objects = ({ ENV_DIRECT_OBS });
            omatch->result = OBJ_PARSER_SUCCESS;
            i++;
         } else {
            omatch = match_objects(pattern[++i], pattern_str, env);
         }
         i++; /* Skip environment specifier */
         add_comm_bit += ({ pattern_str });
         if (omatch->result != OBJ_PARSER_SUCCESS) {
            failed_match = omatch;
         } else {
            if (stringp(indir_match)) {
                if (pointerp(indir_match)) {
                     /* Already an array */
                     indirect_obs += ({ omatch->objects });
                     indir_match += ({ pattern_str });
                } else {
                     // Make an array.
                     indirect_obs = ({ indirect_obs, omatch->objects });
                     indir_match = ({ indir_match, pattern_str });
                }
            } else {
                indirect_obs = omatch->objects;
                indir_match = pattern_str;
            }
         }
         start = matches[p];
         break;
      case SINGLE_WORD :
      case STRING :
      case SHORT_STRING :
         add_comm_bit += ({ implode(bits[start..matches[p]], " ") });
         break;
      case QUOTED_STRING :
         pattern_str = implode(bits[start..matches[p]], " ");
         add_comm_bit += ({ pattern_str[1..<2] });
         break;
      case NUMBER :
         sscanf(implode(bits[start..matches[p]], " "), "%d", j);
         add_comm_bit += ({ j });
         break;
      case FRACTION :
         sscanf(implode(bits[start..matches[p]], " "), "%d/%s", j, k);
         add_comm_bit += ({ j, k });
         break;
      case OPTIONAL_SPACES :
      case OPTIONAL :
         i++; /* skip optional bits */
         break;
      case WORD_LIST_SPACES :
      case WORD_LIST :
         /* These are variable and therefor there must be something associated
             with them */
         if (pointerp(pattern[++i]))
            if (sizeof(pattern[i]) > 1)
               add_comm_bit += ({ implode(bits[start..matches[p]], " ") });
         break;
      }
      start = matches[p]+1;
   }
   if (failed_match) {
      if (query_notify_fail()) {
         _failed_mess[1] += query_notify_fail();
         notify_fail(0);
      } else {
         setup_failed_mess(failed_match);
      }
      return (matches[0]?-1:0);
   }
   _fail_mess = ([ ]);
   _succ_mess = ([ ]);
   _succ_mess_dir = ({ });
   _succ_mess_indir = ({ });
   ret = 0;

   //
   // If this only has a direct-obs method in it, then assume the
   // direct object is the one that defined the method.
   //
   if (!sizeof(direct_obs->objects) &&
         (indirect_obs == ENV_DIRECT_OBS ||
          sizeof(indirect_obs & ({ ENV_DIRECT_OBS }) ) ) ) {
      //
      // Grab all the direct ones.
      //
      direct_obs->objects = ({ });
      for (i = 0; i < sizeof(command); i += 2) {
          direct_obs->objects += ({ command[i] });
      }
   }

   if (!sizeof(direct_obs->objects)) {
      indirect_obs -= ({ ENV_DIRECT_OBS });
      /* Cycle through each object.   Finish when one handles the command */
      for (i = 0; i < sizeof(command) && !ret;i += 2) {
         /* Straight command thingy.   No direct objects to handle at all. */
         /* Ok, what we do in this case is call the function on the set object */
         if (!add_comm_bit) {
            add_comm_bit = ({ });
            start = 1;
            for (j = 0; j < sizeof(matches); j++) {
               add_comm_bit += implode(bits[start..matches[j]], " ");
               start = matches[j]+1;
            }
         }
         if (functionp(command[i+1])) {
            /* If a function pointer exists, check to see that its owner 
             * hasn't been destructed. */
            if ( functionp( command[ i + 1 ] ) & FP_OWNER_DESTED ) {
               /* It has -- so we need to remove it from the player's 
                  command array and then start checking again. */ 
               start = command[i];
               if (!start) {
                  remove_object_force(start);

                  if (start == command[i]) {
                     remove_from_command(bits[0], start);
                  }

                  ret = 0;
                  continue;
               } 
            }
              
            ret = evaluate(command[i+1], indirect_obs, dir_match, indir_match,
                                  add_comm_bit, pattern_string, bits[0]);
         } else {
            start = command[i];
            if (!start) {
               remove_object_force(start);
               if (start == command[i]) {
                  remove_from_command(bits[0], start);
               }
               ret = 0;
               continue;
            }
            do {
               if (function_exists("do_"+bits[0], start)) {
                  ret = call_other(start, "do_"+bits[0], indirect_obs,
                                             dir_match, indir_match, add_comm_bit,
                                             pattern_string);
                  break;
               } else {
                  start = shadow(start, 0);
               }
            } while (start);
            if (!start) {
                ret = call_other(command[i], "command_control", bits[0],
                                          indirect_obs,
                                          dir_match, indir_match, add_comm_bit,
                                          pattern_string);
            }
         } 
      }
      /*
       * Since there are no direct objects.   We don't actually auto generate
       * a message.   That is left up to the object in question.
       */
      if (!ret || ret == -1) {
         if (query_notify_fail()) {
            _failed_mess[1] += query_notify_fail();
            notify_fail(0);
         } else {
            fail_mess_check = get_fail_messages( bits[ 0 ], ({ }) );
            if (_failed_mess[1] == "") {
                _failed_mess[1] += fail_mess_check;
            }
         }
      }
      if (_succ_mess_dir) {
          print_special_messages( bits[ 0 ] );
      }
      _fail_mess = ([ ]);
      return ret;
   }

   /*
    * Ok, now we have some direct objects.   
    * This means we are now working very close to the
    * way that the old add_command did.   Exactly the same actually. 
    */
   /*
    * We go over each direct object and call a function on it.
    * This return value we use to auto generate a success message.
    * However, you can also set your own sucess message.
    */
   fail_ob = ({ });
   bity = ({ });
   failed = 0;
   for (i=0;i<sizeof(direct_obs->objects);i++) {
      /* Check to make sure this object has the command added to it. */
      j = member_array(direct_obs->objects[i], command);
      if (j == -1) {
         fail_ob += ({ direct_obs->objects[i] });
         continue;
      }
      /* Only allow objects with a short. */
      if (!direct_obs->objects[i]->short()) {
         continue;
      }


      if (member_array(ENV_DIRECT_OBS, indirect_obs) != -1) {
         failed_match = my_find_match(indir_match, ({ direct_obs->objects[i] }), 0);
         if (failed_match->result == OBJ_PARSER_SUCCESS) {
             stuff = failed_match->objects;
         } else {
             stuff = ({ });
         }
         debug_printf("Direct Obs Env (%O %O %O)\n", indir_match, stuff, direct_obs->objects[i]);
      } else if (pointerp(indir_match)) {
         failed_match = 0;
         stuff = copy(indirect_obs);
         for (k = 0; k < sizeof(indirect_obs); k++) {
            if (intp(stuff[k]) || ( arrayp(stuff[k]) && sizeof( stuff[k] ) && 
                intp(stuff[k][0]) ) ) {
               failed_match = my_find_match(indir_match[k], ({ direct_obs->objects[i] }), 0);
               if (failed_match->result == OBJ_PARSER_SUCCESS) {
                  stuff[k] = failed_match->objects;
               } else {
                  break;
               }
            }
         }
      } else {
         stuff = indirect_obs;
         failed_match = 0;
      }

      if (failed_match && failed_match->result != OBJ_PARSER_SUCCESS) {
         setup_failed_mess(failed_match);
         failed = 1;
         continue;
      }

      if (functionp(command[j+1])) {
         //
         // See if our function pointer is destructed, and try to fail
         // semi-gracefully.
         //
         if (functionp(command[j+1]) & FP_OWNER_DESTED) {
            start = direct_obs->objects[i];
            if (!start) {
                remove_object_force(start);
                if (start == command[i]) {
                   remove_from_command(bits[0], start);
                }
                ret = 0;
                continue;
            } else {            
                // We still have the start object, but the function pointer
                // has actually been destructed. 
                // We need to remove only this specific command.
                if (start == command[i]) {
                    remove_from_command(bits[0], start);
                }
                ret = 0;
                continue;
            }
         } 
         
         ret = evaluate(command[j+1], stuff, dir_match, indir_match,
                        add_comm_bit, pattern_string, bits[0]);
      } else {
         //
         // See if our command object was dested.
         //
         start = direct_obs->objects[i];
         if (!start) {
             remove_object_force(start);
             if (start == command[i]) {
                remove_from_command(bits[0], start);
             }
             ret = 0;
             continue;
         }

         /* Check to see if the object has the "do_"+verb function. */
         do {
            if (function_exists("do_"+bits[0], start)) {
                ret = call_other(start, "do_"+bits[0], stuff,
                                          dir_match, indir_match, add_comm_bit, 
                                          pattern_string);
                break;
            } else {
                start = shadow(start, 0);
            }
         } while (start);
         if (!start) {
             ret = call_other(direct_obs->objects[i], "command_control", bits[0], stuff,
                                       dir_match, indir_match, add_comm_bit, 
                                       pattern_string);
         }
      }

      //
      // Figure out what to do with the return value.
      //
      if (stringp(ret) || pointerp(ret)) {
         bity += ({ ret });
      } else if (ret && ret != -1) {
         bity += ({ direct_obs->objects[i]});
      } else {
         fail_ob += ({ direct_obs->objects[i] });
         if (ret == -1) {
            failed = 1;
         }
      }
   }
   /*
    * Ok, with this type, we auto generate success and fail messages.
    */
   if (!sizeof(bity)) {
      /* Failure!   We never got any non zero return values */
      fail_mess_check = get_fail_messages( bits[ 0 ], fail_ob );
      if (query_notify_fail()) {
         _failed_mess[1] += query_notify_fail();
         notify_fail(0);
      } else if (_failed_mess[1] == "") {
         /*
          * Only generate a fail message if we do not already have one.
          */
         _failed_mess[1] += fail_mess_check;
         if (_failed_mess[1] == "" && sizeof(fail_ob)) {
            notify_fail(0);
            fail_mesg = create_message(bits, matches, pattern, fail_ob, 1);
            if (!pointerp(indirect_obs)) {
               _failed_mess[1] += "You cannot " + bits[ 0 ] +
                   replace_string( fail_mesg, "$succ_indir$",
                                          ( pointerp( indir_match ) ?
                                             query_multiple_short( indir_match ) :
                                             indir_match) );
            } else {
               if (pointerp(indir_match)) {
                   bity = explode("F" + fail_mesg, "$succ_indir$");
                   _failed_mess[1] += "You cannot "+ bits[ 0 ];
                   bity[0] = bity[0][1..];
                   for (i = 0; i < sizeof(bity) - 1; i++) {
                        if (i >= sizeof(indir_match)) {
                            if (stuff && pointerp(stuff[<1])) {
                                 _failed_mess[1] += bity[i] +
                                                             query_multiple_short( stuff[<1] );
                            } else {
                                 _failed_mess[1] += bity[i] +
                                                             query_multiple_short( indir_match[<1] );
                            }
                        } else {
                            if (stuff && pointerp(stuff[i])) {
                                 _failed_mess[1] += bity[i] +
                                                             query_multiple_short( stuff[i] );
                            } else {
                                 _failed_mess[1] += bity[i] + indir_match[i];
                            }
                        }
                   }
                   _failed_mess[1] += bity[<1];
               } else {
                  if ( member_array( ENV_DIRECT_OBS, indirect_obs ) == -1 ) {
                     stuff = indirect_obs;
                  }
                  if ( member_array( this_player(), stuff ) == -1 ) {
                     _failed_mess[1] += "You cannot "+ bits[ 0 ] +
                           replace_string( fail_mesg, "$succ_indir$",
                                                query_multiple_short( stuff, "a" ) );
                  } else {
                     _failed_mess[1] += "You cannot "+ bits[ 0 ] +
                        replace_string( fail_mesg, "$succ_indir$",
                                                query_multiple_short( stuff - 
                                                                                 ({this_player()}) +
                                                                                 ({ "yourself" }),
                                                                                 "a" ) );
                  }
               }
            }
         }
      }
      _fail_mess = ([ ]);
      if (!failed) {
          return 0;
      }
      return -1;
   }
   /* We succeeded somewhere.   So print our success stuff. */
   if (sizeof(bity) != sizeof(_succ_mess_dir)) {
      /* Auto generated success messages. */
      succ_mesg = create_message( bits, matches, pattern, bity - _succ_mess_dir);
      if ( member_array( this_player(), _succ_indir ) == -1 ) {
         write( "You "+ bits[ 0 ] + replace( succ_mesg, "$succ_indir$",
                                                               query_multiple_short( _succ_indir,
                                                                                                "one" ) ) );
         stuff = _succ_indir;
      } else {
         write("You "+ bits[0] + replace(succ_mesg, "$succ_indir$",
                                                         query_multiple_short(_succ_indir - 
                                                                                        ({ this_player() })
                                                                                        + ({ "yourself" }),
                                                                                        "one") ) );
         stuff = _succ_indir - ({ this_player() });
         stuff = ({ (string)this_player()->query_objective() +"self" }) + stuff;
      }
      say( capitalize( (string)this_player()->the_short() ) +" "+
             pluralize(bits[0]) + replace(succ_mesg, "$succ_indir$",
                                                         query_multiple_short(stuff, "one")),
             _succ_indir );
      for ( i = 0; i < sizeof( _succ_indir ); i++ ) {
         if ( _succ_indir[ i ] != this_player() ) {
            tell_object( _succ_indir[ i ],
                               capitalize( (string)this_player()->the_short() ) +" "+
                               pluralize(bits[0]) + replace(succ_mesg, "$succ_indir$",
                                                                           query_multiple_short(stuff -
                                                                              ({ _succ_indir[i] }) +
                                                                              ({ "you" }), "one" ) ) );
         }
      }
      //
      // And print any special messages too.
      //
      if (sizeof(_succ_mess_dir)) {
          print_special_messages( bits[ 0 ] );
      }
   } else {
      /* Creator generated success messages */
      print_special_messages( bits[ 0 ] );
   }
   _succ_indir = ({ });
   _succ_mess = ([ ]);
   _succ_mess_dir = ({ });
   _succ_mess_dir = ({ });
   return 1;
} /* handle_command() */

/* This is exactly the same as the old one. */
/* I can't be bothered making the strings more easily readable... */
/* Someone else can do it. */
void print_special_messages( string verb ) {
    int i, j;
    string words, *messes;
    string type;
    mixed *stuff, *tmp;

    messes = keys( _succ_mess );
    for ( i = 0; i < sizeof( messes ); i++ ) {
         if ( functionp( messes[i] ) ||
                (messes[i][0..1] == MY_MESS_HEADER &&
                  strlen(messes[i]) > 2)) {
            /* 0 as the first arg means, this_player.   Write message */
            if (functionp(messes[i])) {
                words = evaluate(messes[i], 0);
            } else {
                words = messes[i][2..];
            }
            type = "one";
            if (strsrch("$Iposs$", words) != -1) {
                type = "poss";
                words = replace_string(words, "$Iposs$", "");
            }
            if (strsrch("$Ithe$", words) != -1) {
                type = "the";
                words = replace_string(words, "$Ithe$", "");
            }
            if (strsrch("$Ia$", words) != -1) {
                type = "a";
                words = replace_string(words, "$Ia$", "");
            }
            if ( member_array( this_player(), _succ_mess[ messes[ i ] ][ 1 ] ) ==
                     -1 ) {
                words = capitalize( replace( words, ({ "$C$", "$CATFROG",
                         "$N", "you", "$p ", "your ",
                         "$r", "you", "$o", "you", "$V", verb, "$es", "", 
                         "$s", "", "$y", "y",
                         "$I", query_multiple_short( _succ_mess[ messes[ i ] ][ 1 ],
                                                          type ) }) ) );
            } else {
                words = capitalize( replace( words, ({ "$C$", "$CATFROG", 
                         "$N", "you", "$p ", "your ",
                         "$r", "you", "$o", "you", "$V", verb, "$es", "", "$s", "", 
                         "$y", "y", 
                         "$I", query_multiple_short( (mixed *)_succ_mess[ messes[ i ] ][ 1 ] -
                         ({ this_player() }) + ({ "yourself" }), type ) }) ) );
            }
            write( replace( words, ({ "$D",
                                    query_multiple_short( _succ_mess[ messes[ i ] ][ 0 ],
                                                                     "one" ),
                                    "$CATFROG", "$C$" }) ) );
         } else if (messes[i][0..1] == OTHER_MESS_HEADER &&
                         strlen(messes[i]) > 2) {
            if (functionp(messes[i])) {
               words = replace( evaluate(messes[i], 1),
                   ({ "$C$", "$CATFROG",
                        "$N", (string)this_player()->the_short(),
                        "$p ", (string)this_player()->query_possessive() +" ",
                        "$r", (string)this_player()->query_pronoun(),
                        "$o", (string)this_player()->query_objective(),
                        "$V", pluralize( verb ), "$es", "es", "$s", "s", "$y", "ies" }) );
            } else {
               words = replace( messes[i][2..],
                   ({ "$C$", "$CATFROG",
                        "$N", (string)this_player()->the_short(),
                        "$p ", (string)this_player()->query_possessive() +" ",
                        "$r", (string)this_player()->query_pronoun(),
                        "$o", (string)this_player()->query_objective(),
                        "$V", pluralize( verb ), "$es", "es", "$s", "s", "$y", "ies" }) );
            }
            words = replace( words, "$D",
                     query_multiple_short( _succ_mess[ messes[ i ] ][ 0 ], "one" ) );
            if (member_array(this_player(), _succ_mess[ messes[i] ][ 1 ]) == -1) {
               stuff = _succ_mess[messes[i]][1];
            } else {
                stuff = ({ (string)this_player()->query_objective() +"self" });
                stuff += (mixed *)_succ_mess[ messes[ i ] ][ 1 ] - ({ this_player() });
            }

            type = "one";
            if (strsrch("$Iposs$", words) != -1) {
                type = "poss";
                words = replace_string(words, "$Iposs$", "");
            }
            if (strsrch("$Ithe$", words) != -1) {
                type = "the";
                words = replace_string(words, "$Ithe$", "");
            }
            if (strsrch("$Ia$", words) != -1) {
                type = "a";
                words = replace_string(words, "$Ia$", "");
            }

            say( capitalize( replace( words, ({ "$I", query_multiple_short( stuff,
                                                                                                       type),
                                                   "$CATFROG", "$C$" }) ) ),
                     _succ_mess[ messes[ i ] ][ 1 ] );
            for ( j = 0; j < sizeof( _succ_mess[ messes[ i ] ][ 1 ] ); j++ ) {
                if ( _succ_mess[ messes[ i ] ][ 1 ][ j ] != this_player() ) {
                     if (strsrch(words, "$I's") != -1) {
                         tmp = stuff - _succ_mess[ messes[ i ] ][ 1 ][ j..j ];
                         tell_object( _succ_mess[ messes[ i ] ][ 1 ][ j ],
                               capitalize( replace( words, ({ "$I's", 
                                                 query_multiple_short( tmp + ({ "your" }) ),
                                                 "$I", query_multiple_short( tmp + ({ "you" }),
                                                                                           "one"),
                                                 "$CATFROG", "$C$" }) ) ) );
                     } else {
                         tell_object( _succ_mess[ messes[ i ] ][ 1 ][ j ],
                               capitalize( replace( words, ({ "$I",
                                                               query_multiple_short( stuff -
                               ({ _succ_mess[ messes[ i ] ][ 1 ][ j ] }) + ({ "you" }),
                               "one"), "$CATFROG", "$C$" }) ) ) );
                     }
                }
            }
       }
   }
} /* print_special_messages() */

/*
 * Don't know if I will need this.   I guess I do though.   Oh well,
 * so it explodes.
 */
string get_fail_messages( string verb, object *fail_obs ) {
    string whole;
    string words;
    string mess;
    string *str;
    object ob;
    class fail_mess_data data;

    whole = "";
    foreach (mess, data in _fail_mess) {
         if ( !stringp( mess ) ) {
             continue;
         }

         str = ({ });
         foreach(ob in data->direct)
            str += ({ "one_short:" + sprintf("%O", ob) });
         
         words = this_player()->evaluate_message( ({ mess, ({ str }) }) );
         words = replace( words,
                                  ({ "$D", query_multiple_short( data->direct, "one" ),
                                       "$V", verb }) );

         if ( member_array( this_player(), data->indirect) == -1 )
             words = replace( words, "$I",
                      query_multiple_short( data->indirect, "one" ) );
         else
             words = replace( words, "$I",
                      query_multiple_short( data->indirect -
                      ({ this_player() }) + ({ "yourself" }), "one" ) );
         whole += capitalize( words );
    }

    return whole;
} /* get_fail_messages() */

string *query_word_list(string list) {
   return 0;
} /* query_word_list() */

varargs string create_message(string *bits, int *matches, mixed *pattern, 
                                             object *dir, int flag) {
   string ret;
   int i, pos;
   
   ret = " ";
   if (matches[0])
      matches[0] = 0;
   for (i=1;i<sizeof(pattern);i++, pos++) {
      switch (pattern[i]) {
      case DIRECT_OBJECT :
         if (member_array(this_player(), dir) != -1)
            ret += query_multiple_short(dir - ({ this_player() }) +
                                                      ({ "yourself" }), (flag?"a":"one"));
         else
            ret += query_multiple_short(dir, (flag?"a":"one"));
         i += 2;
         break;
      case INDIRECT_OBJECT :
         ret += "$succ_indir$";
         i += 2;
         break;
      case SHORT_STRING :
      case STRING :
         ret += implode(bits[matches[pos]+1..matches[pos+1]], " ");
         break;
      case WORD_LIST_SPACES :
         pos += matches[pos+1]-1;
      case WORD_LIST :
         ret += implode(bits[matches[pos]+1..matches[pos+1]], " ");
         i++;
         break;
      case NUMBER :
      case FRACTION :
      case SINGLE_WORD :
/* Can only be one word...   Must be this one. */
         ret += bits[matches[pos]+1];
         break;
      case OPTIONAL_SPACES :
         if (!matches[pos] == matches[pos+1])
            pos += matches[pos+1]-1;
      case OPTIONAL :
         if (matches[pos] == matches[pos+1]) {
            if (pointerp(pattern[i+1][0]))
               ret += implode(pattern[i+1][0], " ");
            else
               ret += pattern[i+1][0];
         } else {
            ret += implode(bits[matches[pos]+1..matches[pos+1]], " ");
         }
         i++;
         break;
      }
      if (i+1 < sizeof(pattern))
         ret += " ";
   }
   return ret+".\n";
} /* create_mesage() */
