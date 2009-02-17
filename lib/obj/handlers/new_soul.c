/*  -*- LPC -*-  */
/*
 * $Locker: presto $
 * $Id: new_soul.c,v 1.30 2003/04/09 01:27:20 presto Exp presto $
 */
/**
 * The soul handler for Discworld.  Handles the soul definitions and
 * the mangling needed to print the output for the players.
 *
 * @author Pinkfish
 */
#include <player.h>
#include <soul.h>
#include <user_parser.h>

#define SAVE_FILE "/save/soul"
#define SOUL_DATA_DIR "/save/soul/data/"

#define POS_SELF   0
#define POS_TARGET 1
#define POS_REST   2

#define PCACHE_MAX_SIZE 30
#define SCACHE_MAX_SIZE 100
#define CLEAN_UP_DELAY 300

/*
 * The soul driver itself...
 */
mixed soul_commands;
mapping soul_command_names;

nosave mapping pattern_cache;
nosave mapping soul_cache;
nosave string *pcache_order,
 *scache_order;
nosave int scache_hits, scache_reads, pcache_hits, pcache_reads, cache_callout;

void load_it();
void save_it();
void print_messages(string verb, mixed obs, string arg, string self,
                    string rest, string target, string force,
                    mapping position);
protected void save_soul_command(string name, mixed *data);

void create() {
  soul_commands = 0;
  soul_command_names = ([ ]);
  pattern_cache = ([ "" : NO_ARGUMENT ]);
  pcache_order = ({ });
  scache_order = ({ });
  soul_cache = ([ ]);
  seteuid("Room");
  load_it();
}


void clean_cache() {
  int i;
  
  for (i = 0; i < (sizeof(pcache_order) - PCACHE_MAX_SIZE); i++) {
    map_delete(pattern_cache, pcache_order[i]);
  }
  pcache_order = pcache_order[i..];
  
  
  for (i = 0; i < (sizeof(scache_order) - SCACHE_MAX_SIZE); i++) {
    map_delete(soul_cache, scache_order[i]);
  }
  scache_order = scache_order[i..];
  cache_callout = 0;
}

/**
 * Adds in a soul command.  Only allows additions from
 * the soul compiler.
 *
 * @see /obj/handlers/soul_compiler.c
 * @param name the name of the soul command
 * @param data the data associated with the soul command
 */
void add_soul_command(string name, mixed data) {
  if (file_name(previous_object()) != SOUL_COMPILER)
    return;
  save_soul_command(name, data);
  map_delete(soul_cache, name);
  save_it();
}

/**
 * Deletes the soul command.  This is used to remove soul commands
 * that are no longer used.
 * 
 * @param name the soul command name to delete
 */
void delete_soul_command(string name) {
  map_delete(soul_cache, name);
  map_delete(soul_command_names, name);
  unguarded((: rm, SOUL_DATA_DIR + name + ".os" :));
  save_it();
}

/**
 * The name of all the soul commands.
 *
 * @return an array containing the names of all the soul commands
 */
string *query_soul_commands() {
  return keys(soul_command_names);
}

/**
 * Used internally to get the soul command data.
 *
 * @param str the soul command to get data for
 * @return the soul command data
 * @see query_soul_command_stuff()
 */
protected mixed *query_soul_command_data(string str) {
  scache_reads++;
  /* This function will load in the rubbish from the disk. */
  if (!soul_cache[str] && soul_command_names[str]) {
    string tmp;
    
    tmp = unguarded((: read_file, SOUL_DATA_DIR + str + ".os" :));
    soul_cache[str] = restore_variable(tmp);
    
    if (!cache_callout && (sizeof(scache_order) > SCACHE_MAX_SIZE))
      cache_callout = call_out("clean_cache", CLEAN_UP_DELAY);
  } else {
    scache_order -= ({ str });
    scache_hits++;
  }
  
  scache_order += ({ str });
  return soul_cache[str];
}

/**
 * Returns the data associated with soul command.
 * Probably not very useful, but useful for debugging.
 *
 * @param str the soul command to get the data for
 * @return the data associated with the soul command
 */
mixed *query_soul_command_stuff(string str) {
  return query_soul_command_data(str) + ({ });
}

/**
 * @ignore yes
 */
protected void save_soul_command(string name, mixed *data) {
  string str;
  
  str = save_variable(data);
  unguarded((: rm, SOUL_DATA_DIR + name + ".os" :));
  unguarded((: write_file, SOUL_DATA_DIR + name + ".os", str :));
  soul_command_names[name] = 1;
}

/**
 * Saves the current state of the soul object.
 */
void save_it() {
  unguarded((: save_object, SAVE_FILE :));
}

/**
 * Loads the previous state of the soul object off the disc.
 */
void load_it() {
  string *names;
  int i;
  
  unguarded((: restore_object, SAVE_FILE :));
  if (mapp(soul_commands)) {
    /* Ok, we convert it to the new format... */
    soul_command_names = ([ ]);
    names = keys(soul_commands);
    for (i = 0; i < sizeof(names); i++) {
      reset_eval_cost();
      save_soul_command(names[i], soul_commands[names[i]]);
    }
    soul_commands = 0;
    save_it();
  }
}

private mixed create_pattern_cache(string pattern) {
  mixed *bing, ret;
  string s1, s2;

  bing = explode("#" + pattern, "<indirect:");
  if (sizeof(bing) == 1)
    if (sscanf(bing[0], "%s<word>%s", s1, s2) ||
        sscanf(bing[0], "%s<string>%s", s1, s2))
      ret = ONLY_ARGUMENT;
    else
      ret = NO_ARGUMENT;
  else if (sscanf(bing[0], "%s<word>%s", s1, s2) ||
           sscanf(bing[0], "%s<string>%s", s1, s2))
    ret = ARGUMENT_FIRST;
  else if (sscanf(bing[1], "%s<word>%s", s1, s2) ||
           sscanf(bing[1], "%s<string>%s", s1, s2))
    ret = ARGUMENT_SECOND;
  else
    ret = ONLY_TARGET;
  return ret;
}

/**
 * This returns the arrays that are used by the pattern
 * matcher in the player object. Called from inside add_command interface.
 *
 * @param name the souul command name to find
 * @return 0 if no command found, otherwise an array of patterns
 * @see /global/new_parse->add_command()
 */
mixed *query_soul_command(string name) {
  mixed *data;
  mixed *ret;
  string pat;
  int i;
  
  if (!soul_command_names[name]) {
    return 0;
  }
  data = query_soul_command_data(name);
  if (!data) {
    return 0;
  }
  ret = ({ ({ ({ }), "", 0, this_object(), 0 }) });
  for (i = 0; i < sizeof(data[PATTERNS]); i++) {
    pat = data[PATTERNS][i];
    
    pcache_reads++;
    if (!pattern_cache[pat]) {
      pattern_cache[pat] = create_pattern_cache(pat);
      if (!cache_callout && sizeof(pcache_order) > PCACHE_MAX_SIZE) {
        cache_callout = call_out("clean_cache", CLEAN_UP_DELAY);
      }
    } else {
      pcache_hits++;
      pcache_order -= ({ pat });
    }
    pcache_order += ({ pat });
    
    ret += ({ ({ ((mixed *) PATTERN_OB->query_pattern(pat))[1],
                   pat, 0, this_object(), 0 }) });
  }
  return ret;
}

/**
 * The main soul handling bit.  This is called by the add_command code
 * when a soul command is matched.
 *
 * @param verb the verb matched
 * @param obs the objects to do the soul command on
 * @param in_dir_match the name which was matched for the peoples names
 * @param args the values of the string and stuff
 * @param pattern the pattern which was matched.
 * @return 1 if the command succeeded, 0 if it failed
 */
int command_control(string verb, object * obs, string, string in_dir_match,
                    string *args, string pattern) {
  int i;
  int j;
  string arg;
  mixed *stuff;
  object *wom;
  object *rem;
  mixed *data;
  
  if (!soul_command_names[verb]) {
    return 0;
  }
  if (pattern != "") {
    stuff = PATTERN_OB->query_pattern(pattern);
    for (i = 1; i < sizeof(stuff); i++) {
      switch (stuff[i]) {
      case INDIRECT_OBJECT:
        /* Ok, in here we check for remote and multiple soul earmuffs... */
        i += 2;
        wom = obs;
        if (previous_object()->query_property("no soul")) {
          previous_object()->add_failed_mess(this_object(),
                                           "You cannot use directed souls.\n",
                                             ({ }));
          return 0;
        }
        
        // check for ignoring people
        if ((sizeof(obs) == 1) &&
            !previous_object()->query_creator() &&
            obs[0]->query_property("ignoring") &&
            member_array((string) previous_object()->query_name(),
                         (string *)obs[0]->query_property("ignoring")) != -1) {
          return 0;
        }
        
        /* prevent multiple souls to people with multiple-soul earmuffed or
         * creators who are invis
         */
        if (sizeof(obs) > 1) {
          obs = filter(obs, (: !$1->check_earmuffs("multiple-soul") :));
          if (!sizeof(obs)) {
            previous_object()->add_failed_mess(this_object(),
               "Everyone seems to have their multiple souls earmuffed.  "
               "I'm depressed, are you depressed?\n",
                                               ({ }));
            return 0;
          }
        }
        
        /*
         * prevent remote souls to npcs, to people you cant see and to
         * people who are role-playing and not in your environment
         */
        rem = filter(obs,
                     (:
                      (!interactive($1) &&
                       environment(previous_object(1)) != environment($1)) ||
                      //!$1->query_visible(previous_object(1)) ||
                      (interactive($1) && $1->query_role_playing() &&
                       environment($1) != environment(previous_object(1)))
                      :));
        obs -= rem;
        obs -= previous_object()->query_ignoring(obs);
        
        /*
         * people who are roleplaying cannot use remote souls or souls to
         * people they don't know.
         */
         if(previous_object()->query_role_playing())
           obs -= filter(obs,
                         (:
                          environment($1) != environment(previous_object(1)) ||
                          (interactive($1) &&
                           !previous_object(1)->is_friend($1->query_name()))
                          :));
         
         if (!sizeof(obs)) {
           return 0;
         }
         if (previous_object()->check_earmuffs("remote-soul")) {
           if (previous_object()->query_earmuffs() ==
               PLAYER_ALLOW_FRIENDS_EARMUFF) {
             rem = filter(obs,
                          (:
                          environment(previous_object(1)) != environment($1) &&
                           !previous_object(1)->is_friend($1->query_name())
                           :));
           } else {
             rem = filter(obs,
                          (:
                           environment(previous_object(1)) != environment($1)
                           :));
           }
           if (sizeof(rem) == sizeof(obs)) {
             previous_object()->add_failed_mess(this_object(),
               "You cannot do a remote soul when you have remote souls "
                                                "earmuffed.\n", ({ }));
             return 0;
           }
           obs = obs - rem;
         } else {
           obs = filter(obs, (: !$1->check_earmuffs("remote-soul") ||
                              (environment(previous_object(1)) ==
                               environment($1)) :));
           if (!sizeof(obs)) {
             previous_object()->add_failed_mess(this_object(),
               "Remote soul earmuffs enabled for $I.\n", wom);
             return 0;
           }
         }
         j++;
         break;
       case STRING:
       case SINGLE_WORD:
       case SHORT_STRING:
         arg = args[j++];
         break;
       case NUMBER:
         j++;
         break;
       case FRACTION:
         j += 2;
         break;
       case OPTIONAL:
       case OPTIONAL_SPACES:
         i++;
         break;
       case WORD_LIST:
       case WORD_LIST_SPACES:
         i++;
         if (pointerp(stuff[i]) && sizeof(stuff[i]) > 1)
           j++;
         break;
       }
     }
   }
   /*
    * Ok.  We have this soul command.  Lets find out what sort of
    * pattern we have
    */
   if (!pattern_cache[pattern]) {
     return 0;
   }
   if (!previous_object()->query_creator()) {
     if ((int) previous_object()->adjust_sp(-SOUL_COST *
                                            (1 + sizeof(obs))) < 0) {
       previous_object()->add_failed_mess(this_object(), NO_POWER, ({ }));
       return 0;
     }
   }
   data = query_soul_command_data(verb);
   switch (pattern_cache[pattern]) {
   case NO_ARGUMENT:
     /*
      * This case.  Means we try the no_argument thing first.  Otherwise
      * we pick the first argument one and use that.
      */
     if (data[SINGLE])
       if (data[SINGLE][NO_ARGUMENTS]) {
         if (sizeof(data[SINGLE][NO_ARGUMENTS]) > POSITION_SINGLE) {
           print_messages(verb, 0, "", data[SINGLE][NO_ARGUMENTS][SELF],
                          data[SINGLE][NO_ARGUMENTS][REST],
                          0, 0,
                          data[SINGLE][NO_ARGUMENTS][POSITION_SINGLE]);
         } else {
           print_messages(verb, 0, "", data[SINGLE][NO_ARGUMENTS][SELF],
                          data[SINGLE][NO_ARGUMENTS][REST], 0, 0, 0);
         }
         return 1;
       } else {
         if (!data[SINGLE][ARGUMENTS])
           return 0;
         arg = data[SINGLE][ARGUMENTS][ARGS][0];
       } else
         return 0;
   case ONLY_ARGUMENT:
     if (arg == "?") {
       /* Find a random arguement... */
       j = 0;
       for (i = 0; i < sizeof(data[SINGLE][ARGUMENTS]); i += SMALL_ARG_SIZE)
         j += sizeof(data[SINGLE][ARGUMENTS][i + ARGS]);
       j = random(j);
       for (i = 0; i < sizeof(data[SINGLE][ARGUMENTS]); i += SMALL_ARG_SIZE)
         if (j < sizeof(data[SINGLE][ARGUMENTS][i + ARGS])) {
           if (data[SINGLE][ARGUMENTS][i + ARGS][j] == "#") {
             if (j > 0)
               j--;
             else {
               j++;
               if (j >= sizeof(data[SINGLE][ARGUMENTS][i + ARGS])) {
                 j -= sizeof(data[SINGLE][ARGUMENTS][i + ARGS]);
                 continue;
               }
             }
           }
           print_messages(verb, 0, data[SINGLE][ARGUMENTS][i + ARGS][j],
                          data[SINGLE][ARGUMENTS][i + SELF],
                          data[SINGLE][ARGUMENTS][i + REST], 0, 0,
                          data[SINGLE][ARGUMENTS][i + POSITION_SINGLE]);
           return 1;
         } else {
           j -= sizeof(data[SINGLE][ARGUMENTS][i + ARGS]);
         }
     }
     /* Ok, now to find the argument... */
     for (i = 0; i < sizeof(data[SINGLE][ARGUMENTS]); i += SMALL_ARG_SIZE) {
       if ((j = member_array(arg, data[SINGLE][ARGUMENTS][i + ARGS], 0, 1))
           != -1) {
         /* Found... */
         print_messages(verb, 0, data[SINGLE][ARGUMENTS][i + ARGS][j],
                        data[SINGLE][ARGUMENTS][i + SELF],
                        data[SINGLE][ARGUMENTS][i + REST],
                        0, 0,
                        data[SINGLE][ARGUMENTS][i + POSITION_SINGLE]);
         return 1;
       }
     }
     /* No argument.  SO we check for a wildcard */
     for (i = 0; i < sizeof(data[SINGLE][ARGUMENTS]); i += SMALL_ARG_SIZE)
       if ((j = member_array("#", data[SINGLE][ARGUMENTS][i + ARGS], 0, 1))
           != -1) {
         /* Found... */
         print_messages(verb, 0, arg,
                        data[SINGLE][ARGUMENTS][i + SELF],
                        data[SINGLE][ARGUMENTS][i + REST], 0, 0,
                        data[SINGLE][ARGUMENTS][i + POSITION_SINGLE]);
         return 1;
       }
     /* No argument found... */
     previous_object()->add_failed_mess(this_object(),
                                        arg +
                                        " is not a valid argument to the soul "
                                        "command \"" + verb + "\".\n",
                                        ({ }));
     return 0;
   case ARGUMENT_FIRST:
   case ARGUMENT_SECOND:
     break;
   case ONLY_TARGET:
     if (data[TARGET][NO_ARGUMENTS]) {
       obs->event_soul_command(this_object(), verb, previous_object(),
                               in_dir_match, 0);
       print_messages(verb, obs, "",
                      data[TARGET][NO_ARGUMENTS][SELF],
                      data[TARGET][NO_ARGUMENTS][REST],
                      data[TARGET][NO_ARGUMENTS][TARGET],
                      data[TARGET][NO_ARGUMENTS][FORCE],
                      data[TARGET][NO_ARGUMENTS][POSITION]);
       return 1;
     }
     /* Ok.  Now, we return 0 if there is no argument one */
     if (!data[TARGET][ARGUMENTS]) {
       return 0;
     }
     arg = data[TARGET][ARGUMENTS][ARGS][0];
     break;
   }
   if (arg == "?") {
     /* Find a random argument... */
     j = 0;
     for (i = 0; i < sizeof(data[TARGET][ARGUMENTS]); i += ARG_SIZE) {
       j += sizeof(data[TARGET][ARGUMENTS][i + ARGS]);
     }
     j = random(j);
     for (i = 0; i < sizeof(data[TARGET][ARGUMENTS]); i += ARG_SIZE) {
       if (j < sizeof(data[TARGET][ARGUMENTS][i + ARGS])) {
         if (data[TARGET][ARGUMENTS][i + ARGS][j] == "#") {
           if (j > 0) {
             j--;
           } else {
             j++;
             if (j >= sizeof(data[TARGET][ARGUMENTS][i + ARGS])) {
               j -= sizeof(data[TARGET][ARGUMENTS][i + ARGS]);
               continue;
             }
           }
         }
         obs->event_soul_command(this_object(), verb, previous_object(),
                                 in_dir_match, arg);
         print_messages(verb, obs, data[TARGET][ARGUMENTS][i + ARGS][j],
                        data[TARGET][ARGUMENTS][i + SELF],
                        data[TARGET][ARGUMENTS][i + REST],
                        data[TARGET][ARGUMENTS][i + TARGET],
                        data[TARGET][ARGUMENTS][i + FORCE],
                        data[TARGET][ARGUMENTS][i + POSITION]);
         return 1;
       } else {
         j -= sizeof(data[TARGET][ARGUMENTS][i + ARGS]);
       }
     }
   }
   for (i = 0; i < sizeof(data[TARGET][ARGUMENTS]); i += ARG_SIZE) {
     if ((j =
          member_array(arg, data[TARGET][ARGUMENTS][i + ARGS], 0,
                       1)) != -1) {
       /* Found... */
       obs->event_soul_command(this_object(), verb, previous_object(),
                               in_dir_match, arg);
       print_messages(verb, obs, data[TARGET][ARGUMENTS][i + ARGS][j],
                      data[TARGET][ARGUMENTS][i + SELF],
                      data[TARGET][ARGUMENTS][i + REST],
                      data[TARGET][ARGUMENTS][i + TARGET],
                      data[TARGET][ARGUMENTS][i + FORCE],
                      data[TARGET][ARGUMENTS][i + POSITION]);
       return 1;
     }
   }
   /* No argument found.  Check for wildcard */
   for (i = 0; i < sizeof(data[TARGET][ARGUMENTS]); i += ARG_SIZE) {
     if ((j =
          member_array("#", data[TARGET][ARGUMENTS][i + ARGS], 0,
                       1)) != -1) {
       /* Found... */
       obs->event_soul_command(this_object(), verb, previous_object(),
                               in_dir_match, arg);
       print_messages(verb, obs, arg,
                      data[TARGET][ARGUMENTS][i + SELF],
                      data[TARGET][ARGUMENTS][i + REST],
                      data[TARGET][ARGUMENTS][i + TARGET],
                      data[TARGET][ARGUMENTS][i + FORCE],
                      data[TARGET][ARGUMENTS][i + POSITION]);
       return 1;
     }
   }
   /* No argument found... */
   return 0;
}

string get_name(object ob, int use_name, int type) {
  if(ob == previous_object())
    return ob->query_objective() + "self";
  if(userp(ob) && use_name && environment(ob) != environment(previous_object()))
    return "$mirror_short:" + file_name(ob) + "$";
  return ob->one_short(use_name);
}

private string create_message(mixed targets, string args, string pattern,
                              int type, string verb, string position,
                              string actor_position, mixed me,
                              int use_name) {
  string *bits;
  string *rabbit;
  string singular;
  string plural;
  int i;
  int word_break;

  if(arrayp(targets) && sizeof(targets) == 1)
    targets = targets[0];
  
//if (previous_object() == find_player("presto"))
//tell_creator("presto", "pattern == %s, verb = %s, type == %d, use_name == %d\n", pattern, verb, type, use_name);
  if (stringp(verb) && type && (sizeof(explode(pattern, "$V$")) < 2)) {
    if (pointerp(targets) && sizeof(targets) > 0) {
      pattern = replace_string(pattern, pluralize(verb), "$V$1=" +
                               pluralize(verb) + "," + verb + "$V$");
    } else {
      pattern = replace_string(pattern, pluralize(verb), "$V$0=" +
                               pluralize(verb) + "," + verb + "$V$");
    }
  }
  if(pattern) {
    pattern = replace_string(pattern, "$V$", "VERBFROG");
    bits = explode("%" + replace(pattern, ({ "$arg$", args })), "$");
  }
  if (!me) {
    me = previous_object();
  }
//if (me == find_player("presto"))
//tell_creator("presto", "%O\n", bits);
  for (i = 1; i < sizeof(bits); i += 2) {
    switch (bits[i]) {
    case "article":
      if (vowel(args[0])) {
        bits[i] = "an";
      } else {
        bits[i] = "a";
      }
      break;
    case "hcname":
      if (stringp(targets)) {
        bits[i] = targets;
        break;
      }

      if (objectp(targets)) {
        if ((targets == me)) {
          if (type) {
            bits[i] = (string) targets->query_objective() + "self";
          } else {
            bits[i] = "yourself";
          }
        } else {
          bits[i] = (string) get_name(targets, use_name, type);
        }
        break;
      }
      if(arrayp(targets)) {
        bits[i] = "$M$" + implode(map(targets, (: get_name :), use_name, type), "") +
                  "$M$";
      }
      break;
    case "mhcname":
      /* Stoopid stuff could be revised at some point, if done nicely. */
      if (stringp(targets)) {
        bits[i] = targets + "'s";   /* This will look really stoopid. */
        break;
      }
      if (objectp(targets)) {
        if ((targets == me))
          if (type)
            bits[i] = (string) targets->query_possessive();
          else
            bits[i] = "your";
        else
          bits[i] = (string) targets->the_poss_short();
        break;
      }
      /* This will look really stoopid. */
      bits[i] = "$M$" + implode(map(targets,
        (: objectp($1) ? $1->the_poss_short($(use_name)) : $1 + "'s" :)), "") +
        "$M$";
      break;
    case "hposs":
      if (objectp(targets))
        bits[i] = (string) targets->query_possessive();
      else
        bits[i] = "their";
      break;
    case "hpronoun":
      if (objectp(targets))
        bits[i] = (string) targets->query_pronoun();
      else
        bits[i] = "they";
      break;
    case "hobj":
      if (objectp(targets))
        bits[i] = (string) targets->query_objective();
      else
        bits[i] = "them";
      break;
    case "mcname":
      if(stringp(me))
        bits[i] = me;
      else if (me != previous_object()) {
        /* Must be from the help... */
        bits[i] = (string) me->short(0, 0);
      } else if(userp(me) && use_name) {
        bits[i] = me->short(0, 0);
      } else 
        bits[i] = me->the_short();
      break;
    case "mposs":
      if (type && objectp(me)) {
        bits[i] = (string) me->query_possessive();
      } else {
        bits[i] = "your";
      }
      break;
    case "mpronoun":
      bits[i] = (string) me->query_pronoun();
      break;
    case "mobj":
      if(objectp(me))
        bits[i] = (string) me->query_objective();
      else
        bits[i] = "you";
      break;
    case "position":
      if (position) {
        rabbit = explode("%" + position, "$");
        rabbit[0] = rabbit[0][1..];
        /* Make sure the size is even... */
        if ((sizeof(rabbit) % 2) == 1) {
          rabbit += ({ "" });
        }
        bits = bits[0..i] + rabbit + bits[i + 1..];
      }
      bits[i] = "";
      break;
    case "aposition":
      if (actor_position) {
        rabbit = explode("%" + actor_position, "$");
        rabbit[0] = rabbit[0][1..];
        /* Make sure the size is even... */
        if ((sizeof(rabbit) % 2) == 1) {
          rabbit += ({ "" });
        }
        bits = bits[0..i] + rabbit + bits[i + 1..];
      }
      bits[i] = "";
      break;
    case "s":
      if (i == 0)
        break;

      word_break = strsrch(bits[i - 1], ' ', -1);
      if (word_break == -1)
        break;

      if (me == previous_object())  { /* Not from 'help' */
        if (use_name)
          bits[i] = pluralize(bits[i - 1][word_break + 1 .. ]);
        else  {
          if (pointerp(targets)  &&  sizeof(targets) > 0)
            bits[i] = "$V$1=";
          else
            bits[i] = "$V$0=";
          bits[i] +=  pluralize(bits[i - 1][word_break + 1 .. ]) + "," +
                    bits[i - 1][word_break + 1 .. ] + "$V$";
        }
        bits[i - 1] = bits[i - 1][0 .. word_break];
      }
      break;
    case "verb":
      if (i + 1 < sizeof(bits))  {
        if (sscanf(bits[i + 1], "%s,%s", singular, plural) == 2)
        {
          if (me == previous_object())  {
            if (use_name)
              bits[i] = singular;
            else  {
              if (pointerp(targets)  &&  sizeof(targets) > 0)
                bits[i] = "$V$1=";
              else
                bits[i] = "$V$0=";
              bits[i] +=  singular + "," + plural + "$V$";
            }
          }
          else bits[i] = singular;
          bits = bits[0 .. i] + bits[i + 2 .. ];
        }
      }
      break;
    case "dollar":
      bits[i] = "$";
      break;
    }
  }
  pattern = implode(bits, "")[1..] + "\n";
  pattern = replace_string(pattern, "VERBFROG", "$V$");
//if (me == find_player("presto")) tell_creator("presto", "pattern == %s\n", pattern);
  return pattern;
}

private string position_command(mixed bing) {
  if (stringp(bing)) {
    return bing;
  }
  if (mapp(bing)) {
    return bing["cmd"];
  }
  return 0;
}

private void do_position_stuff(object ob, mapping position) {
  string cur_pos;
  string new_pos;
  
  /* Ok, the position stuff... */
  if (position) {
    cur_pos = ob->query_position();
    if (position[cur_pos]) {
      /* Ok, do it... */
      new_pos = position_command(position[cur_pos]);
    } else if (position["default"]) {
      new_pos = position_command(position["default"]);
    }
    if (new_pos && new_pos != "ignore" &&
        !ob->query_cannot_change_position() &&
        ("/cmds/living/" + new_pos)->query_position_command()) {
      ("/cmds/living/" + new_pos)->position(ob, position["silent"]);
    }
  }
}

private string *position_string(object ob, mapping position, int) {
  string *str;
  string cur_pos;
  mixed new_pos;

  str = ({ 0, 0, 0 });
  if (position) {
    cur_pos = ob->query_position();
    if (position[cur_pos]) {
      new_pos = position[cur_pos];
    } else if (position["default"]) {
      new_pos = position["default"];
    }
    if (mapp(new_pos)) {
      return ({ new_pos["self"], new_pos["target"], new_pos["rest"] });
    }
    if (new_pos && new_pos != "ignore" &&
        !ob->query_cannot_change_position() &&
        ("/cmds/living/" + new_pos)->query_position_command()) {
      str = ({ " making you " + new_pos + " " +
                 ("/cmds/living/" + new_pos)->query_up_down(),
                 " making $hobj$ " + new_pos + " " +
                 ("/cmds/living/" + new_pos)->query_up_down(),
                 " making $hobj$ " + new_pos + " " +
                 ("/cmds/living/" + new_pos)->query_up_down()
                 });
    }
  }
  return str;
}

private string position_of(object ob, mapping position) {
  string tmp;

  tmp = ob->query_position();
  if (position[tmp]) {
    tmp = position_command(position[tmp]);
    if (tmp) {
      return tmp;
    }
  }
  if (position["default"]) {
    tmp = position_command(position["default"]);
    if (tmp) {
      return tmp;
    }
  }
  return "";
}

private string env_position_of(object ob, mapping position) {
  return file_name(ob->query_current_room()) + position_of(ob, position);
}

private void print_messages(string verb, mixed obs, string arg, string self,
                            string rest, string target, string force,
                            mapping position) {

  object *here, *targ;
  string *pos_stuff, *actor_pos_stuff;

  if(!obs)
    obs = ({ });
  
  if(!position)
    position = ([ ]);
  
  switch(sizeof(obs)) {
  case 0:
    pos_stuff = position_string(previous_object(), position, 0);
    actor_pos_stuff = ({ "", "", ""});

    // Change positions.
    do_position_stuff(previous_object(), position);
    
    break;
  case 1:
    pos_stuff = position_string(obs[0], position, 0);
    actor_pos_stuff = position_string(previous_object(), position["actor"], 0);

    // Change positions
    do_position_stuff(obs[0], position);
    if (position && position["actor"])
      do_position_stuff(previous_object(), position["actor"]);

    break;
  default:
    actor_pos_stuff = position_string(previous_object(), position["actor"], 0);
    pos_stuff = ({ "", "", "" });
    
    // Change positions.
    if (position && position["actor"])
      do_position_stuff(previous_object(), position["actor"]);
  }

  here = filter(obs, (: environment(previous_object(1)) == environment($1) :));
  targ = obs - here - ({ previous_object() });
  
//if (previous_object() == find_player("presto")) tell_creator("presto", "targ == %O, p_o == %O\n", targ, previous_object(1)->short());
  if(!sizeof(obs) || sizeof(here))
    previous_object()->remove_hide_invis("hiding");

  // Souler. This is used if the souler isn't a target.
  if(member_array(previous_object(), obs) == -1) {
    previous_object()->event_soul(previous_object(),
                                 create_message(obs, arg, self, 0, 0,
                                                 pos_stuff[POS_TARGET],
                                                 actor_pos_stuff[POS_TARGET],
                                                 0, 1), ({ }), verb, arg, 0);
  } else {
    // Remove ourselves if this is a multiple soul, otherwise keep ourselves
    // in it.
    if(sizeof(obs) > 1)
      obs -= ({ previous_object() }); 

    previous_object()->event_soul(previous_object(),
                                  create_message(obs, arg, self, 0, 0,
                                                 pos_stuff[POS_SELF],
                                                 actor_pos_stuff[POS_SELF],
                                                 0, 1), ({ }), verb, arg, 0);
  }
  
  // Convert arg to 3rd person.
  if(arg)
    arg = replace(arg,
                  ({ "yourself", previous_object()->query_objective() + "self",
                       "your", previous_object()->query_possessive() }));

  // Remote targets
  if(sizeof(targ)) {
    /*
     * This is so that souls to multiple remote targets will show up as
     * "Womble bings happily at you and Cabbage".
     */
    target = replace(target, ({ " you.", " $hcname$.",
                                " you ", " $hcname$ ",
                                " you!", " $hcname$!",
                                " your ", " $mhcname$ " }));
    targ->event_soul(previous_object(),
                     create_message(obs, arg, target, 1, 0,
                                    pos_stuff[POS_SELF],
                                    actor_pos_stuff[POS_TARGET], 0, 1), 
                     ({ }), verb, arg, 0);
  }
  
  // Strip the souler.
  obs -= ({ previous_object() });


  // Spectators and local targets (except the souler)
  if(!sizeof(obs) || sizeof(here)) {
    if(sizeof(here) > 1)
      here -= ({ previous_object() });
    if(environment(previous_object()))
      event(environment(previous_object()), "soul",
            create_message(here, arg, rest, 1, 0, pos_stuff[POS_REST],
                           actor_pos_stuff[POS_REST], 0, 0),
            ({ previous_object() }) + targ, verb, arg,
            sizeof(here)>=1?here[0]:0);
  }
  
#ifdef DISABLED
  // Do soul forces.
  if(sizeof(obs) && stringp(force)) {
    force = replace(force, "$mcname$", previous_object()->query_name());
    foreach(ob in obs)
      call_out("do_force", 2, ({ ob, force }));
  }
#endif
}

/**
 * @ignore yes
 */
string add_start(string pat, string verb) {
  return verb + " " + pat;
}

/**
 * The list of soul comands in the look at soul function.
 *
 * @return the list of all the soul commands formated for the screen
 */
string help_list() {
  return "$P$Soul$P$The commands available in this soul currently number " +
    sizeof(soul_command_names) +
    ".\nHere is a nice list of them.\nGood luck!\n" +
    sprintf("%-#*s\n\n", (int) this_player()->query_cols(),
            implode(sort_array(keys(soul_command_names), 1), "\n"));
}

/**
 * Returns the help string for the soul ocmmand.  Creates a nice helkp
 * message for the passed soul command.
 *
 * @param verb the soul command to get help on
 * @return the soul command help string
 */
string help_string(string verb) {
  string ret, arg;
  int i;
  mixed target;
  mixed *data;
  
  if (!soul_command_names[verb])
    return 0;
  data = query_soul_command_data(verb);
  ret = sprintf("Allowed command patterns:\n%-#*s\n\n",
                (int) this_player()->query_cols(),
                implode(map_array(data[PATTERNS],
                                  "add_start", this_object(), verb), "\n"));
  if (data[SINGLE]) {
    if (data[SINGLE][NO_ARGUMENTS]) {
      ret += "Has a no arguments mode.\nSelf:   " +
        create_message(0, "", data[SINGLE][NO_ARGUMENTS][SELF], 0, 0,
                       0, 0, this_player(), 0) +
        "Others: " +
        create_message(0, "", data[SINGLE][NO_ARGUMENTS][REST], 1, 0,
                       0, 0, this_player(), 0) + "\n";
    }
    if (data[SINGLE][ARGUMENTS]) {
      for (i = 0; i < sizeof(data[SINGLE][ARGUMENTS]); i += SMALL_ARG_SIZE) {
        arg = data[SINGLE][ARGUMENTS][i + ARGS][0];
        ret += sprintf("For the arguments: [%-=*s",
                       (int) this_player()->query_cols() - 20,
                       implode(data[SINGLE][ARGUMENTS][i + ARGS],
                               ", ") + "]") + "\nSelf:   " +
          create_message(0, arg, data[SINGLE][ARGUMENTS][i + SELF], 0, 0,
                         0, 0, this_player(), 0)
          + "Others: " + create_message(0, arg,
                                        data[SINGLE][ARGUMENTS][i + REST], 1,
                                        0, 0, 0, this_player(), 0)
          + "\n";
      }
    }
  }
  if (data[TARGET]) {
    target = query_multiple_short(({ "Womble", "Cabbage" }));
    if (data[TARGET][NO_ARGUMENTS]) {
      ret += "Has a no arguments, targeted mode.\nSelf:   " +
        create_message(target, "", data[TARGET][NO_ARGUMENTS][SELF], 0, 0,
                       0, 0,
                       this_player(), 0) + "Target: " +
        (data[TARGET][NO_ARGUMENTS][TARGET] ?
         create_message(target, "", data[TARGET][NO_ARGUMENTS][TARGET], 2,
                        0, 0, 0, this_player(), 0) :
         create_message(target, "", data[TARGET][NO_ARGUMENTS][REST],
                        1, 0, 0, 0,this_player(), 0)) +
        "Others: " + create_message(target, "",
                                    data[TARGET][NO_ARGUMENTS][REST], 1,
                                    0, 0, 0, this_player(), 0) + "\n";
    }
    if (data[TARGET][ARGUMENTS]) {
      for (i = 0; i < sizeof(data[TARGET][ARGUMENTS]); i += ARG_SIZE) {
        arg = data[TARGET][ARGUMENTS][i + ARGS][0];
        ret += sprintf("For the arguments: [%-=*s",
                       (int) this_player()->query_cols() - 20,
                       implode(data[TARGET][ARGUMENTS][i + ARGS],
                               ", ") + "]") + "\nSelf:   " +
          create_message(target, arg, data[TARGET][ARGUMENTS][i + SELF],
                         0, 0, 0, 0,
                         this_player(), 0) + "Target: " +
          (data[TARGET][ARGUMENTS][TARGET + i] ?
           create_message(target, arg,
                          data[TARGET][ARGUMENTS][i + TARGET], 2, 0, 0,
                          0, this_player(), 0) :
           create_message(target, arg,data[TARGET][ARGUMENTS][i + REST],
                          1, 0, 0, 0,this_player(), 0)) +
          "Others: " + create_message(target, arg,
                                      data[TARGET][ARGUMENTS][i + REST],
                                      1, 0, 0, 0, this_player(), 0);
      }
    }
  }
  return this_player()->convert_message(ret);
}

/**
 * Attempts to do the force on the player.
 * @param arr the args used to force
 */
void do_force(mixed *arr) {
  string cmd;
  
  cmd = explode(arr[1], " ")[0];
  /* Only allow them to use soul commands on the force... */
  if (soul_command_names[cmd] &&
      !arr[0]->query_property("dead") &&
      userp(arr[0]))
    arr[0]->eventForce(arr[1]);
}

int query_pcache_size() {
  return sizeof(keys(pattern_cache));
}

int query_scache_size() {
  return sizeof(keys(soul_cache));
}

mixed *stats() {
  return ({ ({ "souls read", scache_reads, }),
              ({ "soul cache hit percent",
                   (scache_hits * 100) / scache_reads, }),
              ({ "souls in cache", sizeof(keys(soul_cache)), }),
              ({ "patterns read", pcache_reads, }),
              ({ "pattern cache hit percent",
                   (pcache_hits * 100) / pcache_reads, }),
              ({ "patterns in cache", sizeof(keys(pattern_cache)) - 1, }), });
}
