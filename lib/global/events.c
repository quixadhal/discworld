inherit "/global/play_parse_com";
inherit "/global/communicate";
#include "player.h"
#include "term.h"
#include "tune.h"

int earmuffs, cols, rows;
static mapping colour_map; /* this so that changeing the definitions gets fixed
                            * when they log in again.  Also keeps the save
                            * file smaller. */
string term_name = "dumb";
mixed  *tell_history;
static string new_line = "", *dir_array;
static mixed *eemessages;

void create() {
  languages = ({ "common" });
  cur_lang = "common";
  dir_array = ({ "north", "northeast", "east", "southeast",
                 "south", "southwest", "west", "northwest", "north", });
  eemessages = ({ }); 
  ::create();
} /* create() */

string query_term_name() { return term_name; }
int query_earmuffs() { return earmuffs; }

int query_see_octarine() {
  string guild;

  if(!(guild = (string)this_object()->query_guild_ob())) return 0;
  return (int)guild->query_see_octarine();
} /* query_see_octarine() */

string read_message(string str, string type, string lang) {
  mixed bing;

  if (member_array(lang, languages) == -1)
    if ((bing = (mixed)LANGUAGE_HAND->query_garble_object(lang)))
      if (LANGUAGE_HAND->query_language_magic(lang))
        return (string)bing->garble_text(str, previous_object());
      else if ((bing = (mixed)bing->garble_text(str, previous_object())))
        return "You cannot read the writing "+bing+
           (type?" written in "+type:"")+"\n";
      else
        return "You could have sworn there was writing there...\n"; /* Invisible text... */
    else
      return "You cannot read the writing "+(type?" written in "+type:"")+"\n";
/*
 * Magical writing does silly things with the text string.  Need to test
 * for this...
 */
  if (LANGUAGE_HAND->query_language_magic(lang))
    if ((bing = (mixed)LANGUAGE_HAND->query_garble_object(lang)))
      return (string)bing->magical_text(str, previous_object());
    else {
/* ARggh!  No garble object!  Irk.  What now?  Panic!  Yes Panic! */
      return "The mud panics as it finds some magic writting you can read but "+
             "it cannot find the interpreter for,\n";
    }
  if (!type)
    return str+"\n";
  return "'"+str+"' written in "+type+"\n";
} /* read_message() */

string fix_string(string ret) {
  string *st;
  int i, off, oct;

  if (!colour_map)
    colour_map = (mapping)TERM_HANDLER->set_term_type(term_name);
  if (!stringp(ret) || ret == "")
    return ret;
  st = explode(ret, "%^");
  ret = "";
  for (i=0;i<sizeof(st);i++) {
    if(st[i][0..8] == "OCTARINE:")
      if(query_see_octarine()) st[i] = extract(st[i], 9);
      else continue;
    if (colour_map[st[i]])
      ret += colour_map[st[i]];
    else
      ret += st[i];
  }
  return ret+colour_map["RESET"];
} /* fix_string() */

int set_term_type(string str) {
  mapping tmp_col;
  
  if (!str) {
    notify_fail(sprintf("%-=*s", cols,
                "Syntax: "+query_verb()+" <term_type>\n"+
                "Where term type is one of the following: "+
                implode((string *)TERM_HANDLER->query_term_types(), ", ")+
                ".\n"));
    return 0;
  }
  if(str != term_name) {
    if(member_array(str, (string *)TERM_HANDLER->query_term_types()) != -1) {
      colour_map = (mapping)TERM_HANDLER->set_term_type(str);
      term_name = str;
      write("Ok, terminal type set to "+str+".\n");
      return 1;
    } else {
      notify_fail("No such terminal type as " + str  + ".\n");
      return 0;
    }
  } else {
    notify_fail("Terminal type unchanged as " + str + ".\n");
    return 0;
  }
} /* set_term_type() */

void event_commands(){
  add_action("earmuffs", "ear*muffs");
  add_action("inform", "inf*orm");
  add_action("set_our_rows", "rows");
  add_action("set_our_cols", "cols");
  add_action("set_term_type", "term");
  add_action("do_new_line","new_*line");
  add_action("do_tell_his", "ht*ell");
  if(this_object()->query_property(NO_LINE_PROP))
    new_line = "";
  else
    new_line = "\n";
} /* event_commands() */

int do_tell_his(string str) {
  int i;

  if (!pointerp(tell_history) || !sizeof(tell_history)) {
    notify_fail("You have not been told anything.\n");
    return 0;
  }
  write("Your tell history is:\n");
  for (i=0;i<sizeof(tell_history);i++) {
    if (sizeof(tell_history[i]) > 2)
      write("** "+ctime(tell_history[i][2])+" **\n");
    efun::tell_object(this_object(), fix_string(sprintf("%s%-=*s\n",
         tell_history[i][0], cols-strlen(tell_history[i][0]),
         tell_history[i][1])));
  }
  return 1;
} /* do_tell_his() */

int earmuffs(string frog){
  string *bits, *types, *on;
  int i;

  types = ({ "shout" });
  if (this_object()->query_creator())
    types += ({ "creator-tell", "multiple-soul", "remote-soul" });
  if (!frog) {
    if (earmuffs)
      write("Your earmuffs are on.\n");
    else
      write("Your earmuffs are off.\n");
    on = (string *)this_object()->query_property("earmuffs");
    if (!on) on = ({ });
    if (!sizeof(on)) {
      write("You have nothing set to be earmuffed.\n");
    } else {
      for (i=0;i<sizeof(on);i++)
        write("You have "+on[i]+" earmuffed.\n");
      on = types - on;
      for (i=0;i<sizeof(on);i++)
        write(on[i]+" events are not currently being earmuffed.\n");
    }
    return 1;
  }
  bits = explode(lower_case(frog), " ");
  bits = bits - ({ "" });
  on = (string *)this_object()->query_property("earmuffs");
  if (!on) on = ({ });
  if (sizeof(bits) == 1)
    switch (bits[0]) {
      case "on" :
        earmuffs = 1;
        write("Ear muffs turned on.\n");
        return 1;
      case "off" :
        earmuffs = 0;
        write("Ear muffs turned off.\n");
        return 1;
      case "all" :
        bits = types;
        break;
      case "none" :
        on = ({ });
        write("Nothing will be ear muffed any more.  (no news is good news?)\n");
      return 1;
    }
  for (i=0;i<sizeof(bits);i++)
    if (member_array(bits[i], types) == -1) {
      write("I cannot earmuff "+bits[i]+" events.\n");
    } else {
      if (sizeof(bits) > i+1) {
        switch (bits[i+1]) {
          case "on" :
            if (member_array(bits[i], on) == -1)
              on += ({ bits[i] });
            write("Your "+bits[i]+" events will now be earmuffed.\n");
            i++;
            break;
          case "off" :
            on = on - ({ bits[i] });
            write("Your "+bits[i]+" events will not be earmuffed.\n");
            i++;
            break;
          default :
            if (member_array(bits[i], on) == -1) {
              write("Your "+bits[i]+" events will be earmuffed.\n");
              on += ({ bits[i] });
            } else {
              write("Your "+bits[i]+" events will not be earmuffed.\n");
              on = on - ({ bits[i] });
            }
            break;
        }
      } else {
        if (member_array(bits[i], on) == -1) {
          write("Your "+bits[i]+" events will be earmuffed.\n");
          on += ({ bits[i] });
        } else {
          write("Your "+bits[i]+" events will not be earmuffed.\n");
          on = on - ({ bits[i] });
        }
      }
    }
  this_player()->add_property("earmuffs", on);
  return 1;
} /* earmuffs() */

int check_earmuffs(string type) {
  string *on;

  if (!earmuffs)
    return 0;
  on = (string *)this_object()->query_property("earmuffs");
  if (!on) return 0;
  if (member_array(type, on) == -1)
    return 0;
  return 1;
} /* check_earmuffs() */

int inform(string str) {
  string *types, *on, *frog;
  int i;

  if (this_object()->query_creator())
    types = ({ "logon", "link-death", "message", "call", "death", "guild" });
  else
    types = ({ "logon" });
  if (this_object()->query_lord())
    types += ({ "force", "enter", "dest" });
  on = (string *)this_object()->query_property("inform");
  if (!on)
    on = ({ });
  if (!str) str = "";
  frog = explode(str, " ");
  if (!frog)
    frog = ({ });
  else 
    frog = frog - ({ "", 0 });
  if (!sizeof(frog)) {
/* show status */
    if (this_object()->query_property("inform repressed"))
      write("Your informs are currently being repressed.\n");
    for (i=0;i<sizeof(on);i++)
      write("You will be informed of "+on[i]+" events.\n");
    types = types - on;
    for (i=0;i<sizeof(types);i++)
      write("You are not being informed of "+types[i]+" events.\n");
    return 1;
  }
  if (sizeof(frog) == 1) {
    switch (frog[0]) {
      case "on" :
        this_object()->remove_property("inform repressed");
        write("You are now being informed.  This is true!\n");
        for (i=0;i<sizeof(on);i++)
          write("You will be informed of "+on[i]+" events.\n");
        return 1;
      case "off":
        this_object()->add_property("inform repressed", 1);
        write("Inform is now repressed.\n");
        return 1;
      case "all":
        on = types;
        frog = frog[1..100];
        for (i=0;i<sizeof(on);i++)
          write("You will be informed of "+on[i]+" events.\n");
        break;
      case "none" :
        on = ({ });
        write("You will not be informed of anything.\n");
        frog = frog[1..100];
        break;
    }
  }
  for (i=0;i<sizeof(frog);i++)
    if (member_array(frog[i], types) == -1) {
      write("I don't know about "+frog[i]+" events.\n");
    } else {
      if (sizeof(frog) > i+1) {
        switch (frog[i+1]) {
          case "on" :
            if (member_array(frog[i], on) == -1)
              on += ({ frog[i] });
            write("You will now be informed of "+frog[i]+" events.\n");
            i++;
            break;
          case "off" :
            on = on - ({ frog[i] });
            write("You will now not be informed of "+frog[i]+" events.\n");
            i++;
            break;
          default :
            if (member_array(frog[i], on) == -1) {
              write("You will now not be informed of "+frog[i]+" events.\n");
              on += ({ frog[i] });
            } else {
              write("You will now not be informed of "+frog[i]+" events.\n");
              on = on - ({ frog[i] });
            }
            break;
        }
      } else {
        if (member_array(frog[i], on) == -1) {
          write("You will now be informed of "+frog[i]+" events.\n");
          on += ({ frog[i] });
        } else {
          write("You will now not be informed of "+frog[i]+" events.\n");
          on = on - ({ frog[i] });
        }
      }
    }
  this_object()->add_property("inform", on);
  return 1;
} /* inform() */

void set_rows(int i) { rows = i; }
int query_rows() { return rows; }

int set_our_rows(string str) {
  int val;

  if(!str) {
    notify_fail("Rows currently set to " + rows + ".\nrows <number> to set.\n");
    return 0;
  }
  if(!sscanf(str, "%d", val) || val <= 10) {
    notify_fail("Invalid number of rows.\n");
    return 0;
  }
  write("Rows set to " + val + ".\n");
  rows = val;
  return 1;
} /* set_our_rows() */

int query_cols() { return cols; }
void set_cols(int i) { cols = i; }

int set_our_cols(string str) {
  int val;

  if(!str) {
    notify_fail("Columns currently set to " + cols + ".\ncols <number> to set.\n");
    return 0;
  }
  if(!sscanf(str, "%d", val) || val <= 35) {
    notify_fail("Invalid column size.\n");
    return 0;
  }
  write("Columns set to " + val + ".\n");
  cols = val;
  return 1;
} /* set_our_cols() */

int do_new_line(string str) {
  switch(str) {
    case 0:
     if (new_line == "")
       new_line = "\n";
     else
       new_line = "";
     break;
    case "off":
      new_line = "";
      break;
    case "on":
      new_line = "\n";
      break;
    default:
      notify_fail("Syntax: new_line <on|off>\n");
      return 0;
  }
  if (new_line == "") {
    this_object()->add_property(NO_LINE_PROP, 1);
    write("New lines after everything switched off.\n");
  } else {
    this_object()->remove_property(NO_LINE_PROP);
    write("New lines after everything switched on.\n");
  }
  return 1;
} /* do_new_line() */

void add_message(object ob, string mess, int mangle) {
  if (!sizeof(eemessages)) {
    call_out("print_messages", 0);
    eemessages = ({ mess, ({ mangle, ob }) });
    return ;
  }
  if (eemessages[sizeof(eemessages)-2] == mess 
      && eemessages[sizeof(eemessages)-1])
    eemessages[sizeof(eemessages)-1] += ({ ob });
  else
    eemessages += ({ mess, ({ mangle, ob }) });
} /* add_message() */

void event_inform(object ob, string mess, string type) {
  string *on;

  on = (string *)this_object()->query_property("inform");
  if (!on) on = ({ });
  if (this_object()->query_property("inform repressed")
      || (ob->query_invis() && !this_object()->query_creator())
        || ((int)ob->query_invis() == 2 && !this_object()->query_lord())
      || !sizeof(on))
    return ;
  if (member_array(type, on) == -1) return ;
  add_message(ob, "["+sprintf("%-=*s", cols-2, mess)+"]\n", 0);
} /* event_inform() */

void event_enter(object ob, string mess, object from) {
  int i;

  if (stringp(mess))
    add_message(ob, mess+"\n", 1);
} /* event_enter() */

void event_exit(object ob, string mess, object to) {
  int i;

  ::event_exit(ob, mess, to);
  if (mess)
    add_message(ob, mess+"\n", 1);
} /* event_exit() */

void print_messages() {
  mixed *bing;
  string *bits;
  int i, j;

  bing = eemessages;
  eemessages = ({ });
  tell_object(this_object(), new_line);
  for (i=0;i<sizeof(bing);i+=2) {
/* Do squidging of two wide patterns. */
    while (i+6 < sizeof(bing) && bing[i] == bing[i+4] 
           && bing[i+2] == bing[i+6] && bing[i+1][0] && bing[i+3][0]
           && bing[i+5][0] && bing[i+7][0]) {
/* Squidge...  */
      bing[i+1] += bing[i+5][1..10900];
      bing[i+3] += bing[i+7][1..10000];
      bing = delete(bing, i+4, 4);
    }
/* Three wide patterns. */
    while (i+10 < sizeof(bing) && bing[i] == bing[i+6] 
           && bing[i+2] == bing[i+8] && bing[i+4] == bing[i+10]
           && bing[i+1][0] && bing[i+3][0] && bing[i+9][0] && bing[i+11][0]
           && bing[i+5][0] && bing[i+7][0]) {
/* Squidge...  */
      bing[i+1] += bing[i+7][1..10900];
      bing[i+3] += bing[i+9][1..10000];
      bing[i+5] += bing[i+11][1..10000];
      bing = delete(bing, i+6, 6);
    }
    if (sizeof(bing[i+1]) > 2)
      tell_object(this_object(), fix_string(sprintf("%-=*s", cols,
                  process_string(
                 (bing[i+1][0]?replace(bing[i], ({ "$s", "", "%", "", "$N",
                              query_multiple_short(bing[i+1][1..10000], 1)})):
                              bing[i])))));
    else if (bing[i+1][0] && bing[i+1][1]) {
      bits = explode("!"+bing[i]+"!", "%");
      if (sizeof(bits) > 1) {
        for (j=1;j<sizeof(bits);j+=2)
          bits[j] = pluralize(bits[j]);
        bing[i] = implode(bits, "");
        bing[i] = bing[i][1..strlen(bing[i])-2];
      }
      tell_object(this_object(), fix_string(sprintf("%-=*s", cols,
                  process_string(replace(bing[i], ({ "$s", "s", "$N",
                              bing[i+1][1]->short() }))))));
    } else {
      tell_object(this_object(), fix_string(sprintf("%-=*s", cols, bing[i])));
    }
  }
} /* print_messages() */

void event_say(object caller, string str, mixed avoid) {
  if (pointerp(avoid)) {
    if (member_array(this_object(), avoid) != -1)
      return ;
  } else if (avoid == this_object())
    return ;
  add_message(caller, process_string(str), 0);
} /* event_say() */

void event_write(object caller, string str) {
  if (sizeof(eemessages))
    print_messages();
  efun::tell_object(this_object(), fix_string(process_string(str)));
} /* event_write() */

void do_efun_write(string str) {
  if (sizeof(eemessages))
    print_messages();
  efun::tell_object(this_object(), fix_string(sprintf("%-=*s",
                    cols, process_string(str))));
} /* do_efun_write() */

void event_soul(object ob, string str, mixed avoid) {
  if (ob != this_object()) {
    if (sizeof(avoid) > 1 && check_earmuffs("multiple-soul"))
      return ;
    event_say(ob, str, avoid);
  } else
    do_efun_write(str);
} /* event_soul() */

void event_person_say(object ob, string start, string mess, string lang) {
  if (member_array(lang, languages) == -1) {
    mixed str;

    if ((str = (mixed)LANGUAGE_HAND->query_garble_object(lang)))
      if ((str = (mixed)str->garble_say(start, mess))) {
        start = str[0];
        mess = str[1];
      } else
        return ;
    else
      return ;
  } else if (lang != "common")
    start = start[0..strlen(start)-3]+" in "+lang+": ";
  if (ob == this_object()) return;
  add_message(ob, sprintf("%s%-=*s\n", start, cols-strlen(start), mess), 0);
} /* event_person_say() */

void event_person_tell(object ob, string start, string mess, string lang) {
  string s;

  if (member_array(lang, languages) == -1) {
    mixed str;

    if (str = (mixed)LANGUAGE_HAND->query_garble_object(lang))
      if (str = (mixed)str->garble_say(start, mess)) {
        start = str[0];
        mess = str[1];
      } else
        return ;
    else
      return ;
  } else if (lang != "common")
      start = start[0..strlen(start)-3]+" in "+lang+": ";
  if (!pointerp(tell_history))
    tell_history = ({ });
   tell_history +=  ({ ({ start, mess, time() }) });
  if (sizeof(tell_history) > MAX_TELL_HIS)
    tell_history = tell_history[1..1000];
  add_message(ob, sprintf("%s%-=*s\n", start, cols-strlen(start), mess), 0);
} /* event_person_tell() */

void event_whisper(object ob, string start, string mess, object *obs,
                   string lang) {
  string blue;
  mixed str;

  blue = "";
  if (member_array(lang, languages) == -1) {
    if (str = (mixed)LANGUAGE_HAND->query_garble_object(lang))
      if (str = (mixed)str->garble_whisper(start, mess)) {
        start = str[0];
        mess = str[1];
        blue = ": ";
      } else
        return ;
    else
      return ;
  } else if (lang != "common")
      blue = " in "+lang;
  else
    if (!stringp(blue)) blue = "";
  if (member_array(this_object(), obs) == -1)
    add_message(ob, start+"to "+query_multiple_short(obs) + blue + "\n", 0);
  else if (sizeof(obs) == 1)
    add_message(ob, sprintf("%s%-=*s\n",
                      start + "to you " + blue+": ", cols - strlen(start) - 10,
                      mess), 0);
  else
    add_message(ob, sprintf("%s%-=*s\n",
                      start + "to ", cols - strlen(start) - 10,
                      query_multiple_short(obs+ ({ "you" })) + blue +
                      ": " + mess), 0);
} /* event_whisper() */

void event_person_shout(object ob, string start, string mess, string lang) {
  mixed *co_ord1, *co_ord2;
  int d, dx, dy, tan, off;
  string dir;
  mixed str;

  if (ob == this_object()) return ;
  if (check_earmuffs("shout")) {
    return ;
  }
  if (member_array(lang, languages) == -1) {
    if (str = (mixed)LANGUAGE_HAND->query_garble_object(lang))
      if (str = (mixed)str->garble_say(start, mess)) {
        start = str[0];
        mess = str[1];
      } else
        return ;
    else
      return ;
  } else if (lang != "common")
    start = start+" in "+lang;
  if (environment(ob) && environment()) {
    co_ord1 = (mixed *)environment(ob)->query_co_ord();
    co_ord2 = (mixed *)environment()->query_co_ord();
    if (!co_ord1 || !co_ord2)
      return ;
    if (co_ord1[0] > co_ord2[0])
      d = dx = co_ord1[0] - co_ord2[0];
    else
      d = dx = co_ord2[0] - co_ord1[0];
    if (co_ord1[1] > co_ord2[1])
      d += (dy = co_ord1[1] - co_ord2[1]);
    else
      d += (dy = co_ord2[1] - co_ord1[1]);
    if (co_ord1[2] > co_ord2[2])
      d += co_ord1[2] - co_ord2[2];
    else
      d += co_ord2[2] - co_ord1[2];
    if (d > SHOUT_DISTANCE*5/4)
      return ;
    if (dy || dx) {
      if (!dy)
        off = 2;
      else if ((tan = 1000*dx/dy) < 414)
        off = 0;
      else if (tan > 2414)
        off = 2;
      else
        off = 1;
      if (co_ord1[0] > co_ord2[0]) {
        if (co_ord1[1] > co_ord2[1])
          dir = dir_array[6-off];
        else
          dir = dir_array[6+off];
      } else {
        if (co_ord1[1] > co_ord2[1])
          dir = dir_array[2-off];
        else
          dir = dir_array[2+off];
      }
      if (d > SHOUT_DISTANCE) {
        str = "In the distance to the "+dir+" "+start+
              " something you cannot make out.";
      } else {
        if (d < SHOUT_DISTANCE/4) {
          str = "Nearby to the "+dir+" "+start+": "+mess;
        } else if (d > SHOUT_DISTANCE/2) {
          str = "In the distance to the "+dir+" "+start+": "+mess;
        } else
          str = start+": "+mess;
      }
    } else {
      str = start+": "+mess;
    }
  } else 
    str = start + ": "+mess;
  if (strlen(str) > 20)
    add_message(ob, sprintf("%-s%-=*s\n", str[0..19], cols-20, str[20..10000]),
                    0);
  else
    add_message(ob, str+"\n", 0);
} /* event_person_shout() */

void event_creator_tell(object ob, string start, string mess) {
  if (ob == this_object()
     || (check_earmuffs("creator-tell") && query_verb() != "cre!") ||
      !this_object()->query_creator())
    return;
  efun::tell_object(this_object(), new_line + fix_string(sprintf("%s%-=*s\n",
        start, cols - strlen(start), mess)));
} /* event_creator_tell() */

void event_player_echo_to(object ob, string mess) {
  if (this_object()->query_lord())
    efun::tell_object(this_object(), ob->query_cap_name()+" echo to's:\n");
  efun::tell_object(this_object(), new_line + fix_string(sprintf("%-=*s\n",
        cols, mess)));
} /* event_player_echo_to() */

void event_player_emote_all(object ob, string mess) {
  if (ob == this_object())
    return;
  if (this_object()->query_lord())
    add_message(ob, ob->query_cap_name()+" emote all's:\n", 0);
  add_message(ob, sprintf("%-=*s\n", cols, mess), 0);
} /* event_player_echoall() */

void event_player_echo(object ob, string mess) {
  if (ob == this_object())
    return;
  if (this_object()->query_lord())
    add_message(ob, ob->query_cap_name()+" emote all's:\n", 0);
  add_message(ob, sprintf("%-=*s\n", cols, mess), 0);
} /* event_player_echo() */
