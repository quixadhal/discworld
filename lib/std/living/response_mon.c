/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: response_mon.c,v 1.31 2003/04/01 21:39:38 ceres Exp $
 *
*/
/**
 * This file contains the methods to make the npcs in game respond
 * to soul and says.  Allows for a level of npc interation.
 * <p>
 * Code originaly by nimmox@igor.  Modified for discworld by Pinkfish.
 * <p>
 * @change Olorin Sep 97
 * event_soul does a convert_message on the mess first,
 * to avoid spurious matches caused by a message like eg.
 * "$the_short:/global/lord#64387$ $V$0=smiles,smile$V$ .... in souln
 * @change who knows when
 * changed to stop the responses when the npc is in /room/rubbish
 * @author Pinkfish
 * @see /obj/monster.c
 */
#include <soul.h>
#include <language.h>

#define SPECIAL_DAY "/obj/handlers/special_day"

class response_data {
   object last_ob;
   string last_mess;
   int stop_responding;
   int use_regexp;
   int last_count;
   int last_time;
   int understand_anything;
   object* only_respond_to;
}

mixed *_respond_to_with;
nosave class response_data _response_data;

int match(string str, mixed str2);
void event_soul(object per, string mess, object *avoid, string verb,
                string last, mixed at) ;

void create() {
  _respond_to_with = ({ });
  _response_data = new(class response_data);
  _response_data->only_respond_to = ({ });
} /* create() */

/**
 * This method sets or clears the flag that allows the npc to understand
 * any language at all.  THis is useful for those times when you need an
 * npc that speaks/understands any language.
 * @param flag the new value of the flag
 */
void set_response_mon_understand_anything(int flag) {
   _response_data->understand_anything = flag;
}


/**
 * This method sets or clears the flag to use regular expressions instead of
 * the traditional arrays of words.  In addition, this flag inhibits
 * the usual stripping of non-alphanumerics from the input strings.
 * @param flag the new value of the flag: 1 says to use regular expressions,
 * 0 to not use them
 */
void set_response_mon_regexp(int flag) { _response_data->use_regexp = flag; }

/**
 * This method returns the current state of the flag that indicates whether
 * to use regular expressions instead of the traditional array of words.
 * @return 1 if using regular expressions, 0 if not
 */
int query_response_mon_regexp() { return _response_data->use_regexp; }

/**
 * This method checks to see if the response monster code is in
 * debug mode.
 * @return 1 if in debug mode, 0 if not
 * @see set_response_mon_debug_mode()
 */
int query_response_mon_debug_mode() {
  return this_object()->query_property("respon mon debug");
} /* query_response_mon_debug_mode() */

/**
 * This method sets the current debug flag for the response monster
 * code.
 * @param flag the new value of the flag, 1 on, 0 off
 * @see query_response_mon_debug_mode()
 */
void set_response_mon_debug_mode(int flag) {
  this_object()->add_property("respon mon debug", flag);
} /* set_response_mon_debug_mode() */

/**
 * This method allows the toggling of the responses of the npc.
 * If this is set to 1, then the npc will stop responding to
 * messages.
 * @param i 1 to make the npc not respond, 0 to make it respond again
 * @see query_stop_responding()
 */
void set_stop_responding(int i) { _response_data->stop_responding = i; }
/**
 * This method returns the flag determining the toggling of the responses
 * of the npc.
 * If this is set to 1, then the npc will stop responding to
 * messages.
 * @return 1 to the npc will not respond, 0 it is responding
 * @see query_stop_responding()
 */
int query_stop_responding() { return _response_data->stop_responding; }

/**
 * This method checks to see if the response is allowed for this object.
 * NB: objects to which this_object is not visible are disallowed.
 * @param ob the object to check
 * @param response the response that is matched
 * @return 1 if the object is allowed, 0 if it is not
 */
int query_response_allowed(object ob,
                           string *response) {
  if (!this_object()->query_visible(ob)) {
    return 0;
  }
  return 1;
} /* query_response_allowed() */

/**
 * This method sets the current responses for the npc.
 * This will overwrite the current responses in the npc.
 * Please use add_respond_to_with in your NPC's instead of this function
 * call, as it requires prior knowledge as to the internal structure of
 * the respond_to_with code that add_respond_to_with handles nicely.
 * The array is of the formant:<pre>
 *   ({
 *      trigger1,
 *      response1,
 *      trigger2,
 *      response2,
 *      ...
 *    })
 * </pre>
 * @see add_respond_to_with()
 * @see query_respond_to_with()
 */
void set_respond_to_with( mixed map ) {
  mixed *old_keys;
  int i;

  if ( mapp( map ) ) {
#ifdef DEBUG
    log_file("BAD_RESPONSE", "set_respond_to_with() called by %O (%s)\n",
             previous_object(), previous_object()->query_name());
#endif    
    old_keys = keys(map);
    _respond_to_with = allocate(sizeof(old_keys) * 2);
    for (i = sizeof(old_keys); i--; ) {
      _respond_to_with[2*i] = old_keys[i];
      _respond_to_with[2*i+1] = map[old_keys[i]];
    }
    return;
  }
  _respond_to_with = map;
} /* set_respond_to_with() */

/**
 * This method returns the current responding to with array.
 * The array is of the formant:<pre>
 *   ({
 *      trigger1,
 *      response1,
 *      trigger2,
 *      response2,
 *      ...
 *    })
 * </pre>
 * @see add_respond_to_with()
 */
mixed *query_respond_to_with() { return _respond_to_with; }

/**
 * This method adds a respond message to respond to into the
 * current array of responses.
 * <p>
 * If response_mon_regexp is not set, the trigger consists of an
 * array of words to be matched
 * (in order) in the string.  If there is an array instead of
 * a single word at one point then any of the words in the
 * array will be matched.  If response_mon_regexp is set, the
 * trigger is a single string, representing the regular expression
 * to be matched in the string.
 * <p>
 * The response is either an array of things to execute
 * (in which case a random one will be chosen each time) or
 * a function pointer or a string.  If it is a string then
 * the command of that name will be executed, if the
 * string starts with a '#' then the function named after that
 * will be called on the npc.  That was exciting wasn't it?
 * If the string has a $hname$ name in it and it is not a
 * function call (starts with a #) then the $hname$ will be replaced
 * with the name of the triggering object. $hcname$ or $short$ will be
 * replaced with the short of the triggering object.
 * <p>
 * In the case of a function call, two arguments are passed into
 * the function: the first is the person which triggered the effect
 * and the second in the message which triggered the effect.
 * <p>
 * NOTE: The NPC must explicitly know the language being spoken, even
 * if it's "common".  I don't know why.  That's just the way it is.  See
 * add_language().
 * @example
 * // Simple response
 * add_respond_to_with(({ "@say", "bing" }), "'Yes!  Bing bing bing!");
 * @example
 * // respond to someone saying 'frog' or 'toad'
 * add_respond_to_with(({ "@say", ({ "frog", "toad" }) }),
 *                     "'Frogs and toads are nice.");
 * @example
 * // Randomly say something or bing back at them
 * add_respond_to_with(({ "@say", "bing" }),
 *                     ({ "'Yes!  Bing bing bing!", "bing $hname$" }));
 * @example
 * // Call the function 'rabbit' on the npc.
 * add_respond_to_with(({ "@say", "bing" }), "#rabbit");
 * @example
 * // Do something cute with a function pointer
 * add_respond_to_with(({ "@bing" }),
 *                  (: do_command("'something wild for " + $1->a_short()) :));
 * @param trigger the trigger to trigger the action on
 * @param response the response to the action
 * @see query_respond_to_with()
 * @see query_response_allowed()
 * @see set_response_mon_regexp()
 * @see query_response_mon_regexp()
 * @see regexp()
 * @see add_language()
 */
void add_respond_to_with( mixed *trigger, mixed response ) {
  _respond_to_with += ({ trigger, response });
} /* add_response() */

/**
 * This adds an object into the list of objects that we will only respond
 * to.  If this array is empty then we will respond to anyone, if it has
 * something in it then we will only respond to them.
 * @param person the person to add to the response array
 */
void add_only_respond_to(object person) {
  _response_data->only_respond_to += ({ person });
} /* add_only_respond_to() */

/**
 * This method removes an object from the list of people to respond to.
 * @param person the person to remove
 */
void remove_only_respond_to(object person) {
   _response_data->only_respond_to -= ({ person });
} /* remove_only_respond_to() */

/**
 * This method returns the current list of people we are only responding
 * to.
 * @return the array of people we are responding to
 */
object* query_only_respond_to() {
   return _response_data->only_respond_to;
} /* query_only_respond_to() */

/* Check_sub_sequence checks whether the array (words) contains a subsequence
   as specified in the respond_to_with array */
/**
 * This method checks the subsequence of words to see if it matches
 * any of our current response sets.
 * @param words the words which are to be tested
 * @return the response to be executed
 * @see add_respond_to_with()
 */
protected mixed *check_sub_sequence( mixed *words ) {
  int word_index;
  int format_index; /* what do you know....Meaningful names :-) */
  int format_size;
  int word_size;
  int match_counter;
  int cu_format_size;
  string word_str;
  mixed data;
  string *patterns;
  string pattern;
  string extra;

  format_index = 0;
  word_size = sizeof( words );
  word_str = implode(words, " ");
  format_size = sizeof( _respond_to_with );
  while (format_index < format_size) {
    if ( _response_data->use_regexp ) {
        data = _respond_to_with[ format_index ];

        if ( arrayp( data ) && sizeof( data ) > 1 ) {
            // Excellent. Now, let's add our @say data to the front of
            // the pattern.

            if ( arrayp( data[0] ) ) {
                // Combine the first part into the array, separated
                // by or's, with at least one match.
                extra = "(" + implode( data, "|" ) + ")+";
            }
            else {
                extra = data[0];
            }

            patterns = map( data[1], (: $(extra) + " " + $1 :) );
        }
        else {
            patterns = ({ data });
        }

        //tell_creator( "taffyd", "%O, %O\n", word_str, patterns );

        // Check all of the possible responses.
        foreach( pattern in patterns ) {
            // tell_creator( "taffyd", "%O\n", pattern );
            if ( regexp( word_str, pattern ) ) {
                return _respond_to_with[ format_index + 1 ];
            }
        }
    }
    else {
        word_index = 0;
        match_counter = 0;
        cu_format_size = sizeof(_respond_to_with[format_index]);
        while ((word_index < word_size) && (match_counter < cu_format_size)) {
          match_counter += match(words[word_index],
                                 _respond_to_with[format_index][match_counter]);
          word_index++;
        }
        if (match_counter == (cu_format_size)) {
          return _respond_to_with[format_index + 1];
        }
    }

    format_index += 2;
  }
  return 0;
} /* check_sub_sequence() */

/* match returns 1 if str matches the format.... see also the definitions in
   the example file... */
/** @ignore yes */
protected int match(string str, mixed format) {
  if (pointerp(format)) {
    return (member_array(str, format) != -1);
  }
  if (str == format) {
    return 1;
  }
  return 0;
} /* match() */

/* removing annoying readmarks....  Like .'s and ?'s */
/**
 * This method removes annoying read marks to make the string easier to
 * parse.  Basically it strips puncutation.
 * @param str the string to remove the punctuation from
 * @return the string without any punctuation
 */
protected string remove_read_marks(string str) {
  int blij;
  string result;
  int size;
  string temp;

  size = strlen(str);
  blij = 0;
  result = "";
  while (blij < size) {
    temp = str[blij..blij];
    if (((temp >= "a") && (temp <= "z")) ||
        ((temp >= "0") && (temp <= "9")) ||
        (temp == " ")) {
      result += temp;
    } else {
      result += " ";
    }
    blij++;
  }
  return result;
} /* remove_read_marks() */

/**
 * This method runs the command passed in, doing some substitution.
 * @param str the string to execute
 * @param per the person who triggered the command
 */
protected void senddstr(string str, object per) {
  if ( environment( this_object() ) ) {
    if ( per ) {
      command(replace(str, ({ "$hname$", per->query_name(), "$hcname$",
                                per->query_short(),
                              "$hshort$", per->query_short() })));
    } else {
      command( str );
    }
  }
} /* sendstr() */

/* Take care not to send to other monsters.... Infinite recursion problem...*/

/* Imagine two monsters chatting with each other......*/
/**
 * This method executes the response to the matched string.
 * @param rep the response to execute
 * @param per the person who initiated the event
 * @param mess the message that was matched
 * @see add_respond_to_with()
 */
protected void exec_response(mixed rep, object per, string mess) {
  string *rabbit;

  if (pointerp(rep)) {
    return exec_response(rep[random(sizeof(rep))], per, mess);
  }

  if (functionp(rep)) {
    evaluate(rep, per, mess);
  } else if (stringp(rep)) {
    if (rep[0..0] == "#") {
      rabbit = explode(rep[1..], "#");
      if (sizeof(rabbit) > 1) {
        call_out((: call_other($1, $2, $3, $4) :), 0,
                 rabbit[0], rabbit[1], per, mess);
      } else {
        call_out(rabbit[0], 0, per, mess);
      }
    } else {
      call_out("senddstr", 0, rep, per);
    }
  }
} /* exec_response() */

/** @ignore yes */
private void do_delay_thingy(string *extra, string mess, object per) {
  mixed *response;

  // It does lower case stuff sin...
  mess = lower_case(mess);
  if (!_response_data->use_regexp) {
    mess = remove_read_marks(mess);
  }
  if (query_response_mon_debug_mode()) {
    tell_object(per, "Parsing the text: " + implode(extra, " ") + " " + mess + "\n");
  }
  response = check_sub_sequence(extra + explode(mess, " "));
  if (query_response_mon_debug_mode()) {
    tell_object(per, sprintf("Responding with %O\n", response));
  }

  if (response) {
    if (query_response_allowed(per, response)) {
      exec_response(response, per, mess);
    }
  }
} /* do_delay_thingy() */

/** @ignore yes
 * Some new loop detection. Track the last object, message we responded
 * to and when. Don't respond to the same object & message more than twice in
 * a suitable period.
 * Clearly this won't detect three-way, or other complex, loops.
 */
int check_loop(object per, string mess) {

  // We'll assume players can't get into loops. :)
  if(interactive(per)) {
    return 0;
  }

  if (_response_data->last_ob == per &&
      (_response_data->last_mess = mess) &&
      _response_data->last_time > time() - 10)
  {
    if(_response_data->last_count > 1) {
      return 1;
    } else {
      _response_data->last_count++;
    }
    _response_data->last_time = time();
  } else {
    _response_data->last_ob = per;
    _response_data->last_mess = mess;
    _response_data->last_count = 1;
    _response_data->last_time = time();
  }

  return 0;
}

/** @ignore yes */
private void do_response(string *extra, string mess, object per) {
  if (sizeof(_response_data->only_respond_to)) {
     _response_data->only_respond_to -= ({ 0 });
     if (sizeof(_response_data->only_respond_to) &&
         member_array(per, _response_data->only_respond_to) == -1) {
        return ;
     }
  }
  call_out((: do_delay_thingy($1, $2, $3) :), 2, extra, mess, per);
} /* do_response() */

/*
 * respond as defined in respond_to_with array...See also
 * the definitions in the example monster
 */

/**
 * This method is called on the npcs to help in recognising saytos.
 * Beware...  A @say message will also be added, so you need to make sure
 * you respond to the correct message and not to both.
 */
void event_person_sayto(object per, string mess, string lang, object* targets) {
  string skill;

  if(_response_data->stop_responding ||
     !sizeof(_respond_to_with) ||
     environment() == find_object( "/room/rubbish" ) ||
     environment() == find_object( "/room/void" ) ||
     !per || per == this_object()) {
    return ;
  }

  if(check_loop(per, mess)) {
    return;
  }

  skill = LANGUAGES->query_language_spoken_skill(lang);
  if (this_object()->query_skill(skill) < 90 ||
      per->query_skill(skill) < 60) {
    return;
  }

  do_response(({ "@sayto" }), mess, per);
} /* event_person_say() */

/** @ignore yes */
void event_person_say(object per, string start, string mess, string lang) {
  string skill;

  if(_response_data->stop_responding ||
     !sizeof(_respond_to_with) ||
     environment() == find_object( "/room/rubbish" ) ||
     environment() == find_object( "/room/void" ) ||
     !per || per == this_object()) {
    return ;
  }

  if(check_loop(per, mess))
    return;

  if (!_response_data->understand_anything) {
     skill = LANGUAGES->query_language_spoken_skill(lang);
     if (this_object()->query_skill(skill) < 90 ||
         per->query_skill(skill) < 60) {
       return;
     }
  }

  do_response(({ "@say" }), mess, per);
} /* event_person_say() */

/** @ignore yes */
varargs void event_soul(object per, string mess, object *avoid, string verb,
                        string last, mixed at) {
  if (per == find_object(SOUL_OBJECT))
    per = previous_object(2);

  if(!interactive(per) ||
     _response_data->stop_responding ||
     environment() == find_object( "/room/rubbish" ) ||
     environment() == find_object( "/room/void" ) ||
      member_array(this_object(), avoid) != -1 ||
     !sizeof(_respond_to_with)) {
    return ;
  }

  if((!objectp(at) || (objectp(at) && at != this_object())) &&
     environment(per) != environment() ) {
    return;
  }

  mess = this_object()->convert_message( mess );

  if(check_loop(per, mess))
    return;
  if (objectp(at)) {
    do_response(({ "@" + verb, "#" + last, at->query_name() }), mess, per);
  } else {
    do_response(({ "@" + verb, "#" + last }), mess, per);
  }
} /* event_soul() */

/** @ignore yes */
varargs void event_whisper(object per, string mess, object *obs, string lang,
                           object me) {
  string skill;
  
  if (!interactive(per) || _response_data->stop_responding ||
      environment() == find_object( "/room/rubbish" ) ||
      !sizeof(_respond_to_with)) {
    return ;
  }

  if(check_loop(per, mess)) {
    return;
  }

  if (!_response_data->understand_anything) {
    skill = LANGUAGES->query_language_spoken_skill(lang);
    if (this_object()->query_skill(skill) < 90 ||
        per->query_skill(skill) < 60) {
      return;
    }
  }

  // Don't let them see the message unless it is actually directed at them.
  if (member_array(this_object(), obs) == -1) {
    do_response(({ "@whisper", map(obs, (: $1->query_name() :)) }),
                "", per);
  } else {
    mess = this_object()->convert_message( mess );
    do_response(({ "@whisper", map(obs, (: $1->query_name() :)) }),
                mess, per);
  }
}
