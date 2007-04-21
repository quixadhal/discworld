/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: events.c,v 1.136 2003/10/29 18:25:27 pinkfish Exp $
 */
/**
 * This file contains all the standard event handling code that players
 * need.  This will handle things like informs, shouts, says, tells,
 * whispers.  Everything!   It formats the message correctly and
 * sends it to the player.
 * @author Pinkfish
 */
#include <dirs.h>
#include <living.h>
#include <player.h>
#include <telnet.h>
#include <term.h>
#include <tune.h>
#include <language.h>
#include <broadcaster.h>
#include <cmds/options.h>
#include <newbiehelpers.h>
#include <playerinfo.h>

#define DEBUG
// #define USE_ROLE_PLAYING_SHORTS
#define BROADCASTER "/obj/handlers/broadcaster"
#define SPACES "                                                              "

#define INFORM_COLOURS ([ "default":"WHITE", "logon":"GREEN", "death":"RED", \
                           "cheat":"RED", "multiplayer":"RED",  \
                           "bad-password" : "RED", "link-death":"GREEN", \
                           "new-player" : "BOLD%^%^MAGENTA", \
                           "gag" : "BOLD%^%^RED", "alert" : "GREEN" ])

#ifdef OLD_THINGY
inherit "/global/play_parse_com";
#else
inherit "/global/new_parse";
#endif
inherit "/global/communicate";
inherit "/global/friends";
inherit "/global/options_control";

/*
 * This contains the event related global variables.
 * We are very short on global variables and this reduces our need for them.
 */
class event_info {
  mapping colour_map;
  string cur_term;
  string last_term;
  object where;
  object *had_shorts;
  mixed *eemessages;
  mixed busy;
}

private nosave class event_info _event_info;

private int earmuffs;
private int cols;
private int rows;
private mapping my_colours;

/*
 * This so that changeing the definitions gets fixed
 * when they log in again.  Also keeps the save
 * file smaller.
 */
private string term_name = "network";
private string *inform_types;
private mixed *tell_history;

void set_my_colours(string event_type, string colour);
mapping query_my_colours();

string find_rel(string word, int from);
private int set_our_rows(int num);
private int set_our_cols(int num, int test);
private int do_busy(string str);
private int do_busy_player(object *players);
protected varargs int do_tell_his(string, int);
mixed query_property(string name);
string query_nationality_accent_ob();

void create() {
  friends::create();
  _event_info = new(class event_info,
                    eemessages : ({ }));
                    
  my_colours = ([ ]);
  inform_types = ({ });
#ifdef OLD_THINGY
  play_parse_com::create();
#else
  new_parse::create();
#endif
} /* create() */

#if !efun_defined(has_mxp)
private int has_mxp(object ob) {
   return 0;
}
#endif

/**
 * This method returns the current terminal name.  If this is set to
 * network then the network will be queried for the terminal type
 * (using telnet suboption negotiation).
 * @return the current terminal name
 * @see query_cur_term()
 * @see set_term_type()
 */
string query_term_name() { return term_name; }
/**
 * This method returns the current terminal type that is being used.  This
 * will be 0 if the terminal name is set to network and no response has
 * been gained from the remote site yet.  Otherwise it should be the
 * same value as the terminal name.
 * @return the current terminal type
 * @see query_term_name()
 * @see set_term_type()
 */
string query_cur_term() { return _event_info->cur_term; }
/**
 * This method tells us if the player currently has earmuffs turned on.
 * @return 1 if earmuffs are on, 0 if they are not
 * @see check_earmuffs()
 * @see toggle_earmuffs()
 */
int query_earmuffs() { return earmuffs; }

/**
 * This method returns the list of inform types this player can receive.
 * @return array of inform types
 */
string *query_inform_types() {
  string *types;
  
  types = ({ "logon", "new-player", "birthday", "council", "friend" });
    
  if (this_object()->query_creator()) {
    types += ({ "link-death", "message", "death",
               "guild", "delete", "help", "combat-debug", "skill", "quest",
               "multiplayer", "bad-password", "club", "debug", "gag", 
               "alert" });

    if (master()->query_lord((string)this_object()->query_name())) {
      types += ({ "force", "enter", "dest", "cheat", "xp", "calls",
                    "cpu", "ftp" });
    }
  } else if ( load_object(NEWBIEHELPERS_HANDLER)->query_helper_access(this_object())
                ) {
      types += ({ "guild" });
  }

  if( this_object()->query_guild_ob() == "/std/guilds/witch" ||
      this_object()->query_creator() ) {
    types += ({ "mockery" });
  }

  return types;
}

/* This is pulled back into this object for security reasons... */
/**
 * This is the command to handle the inform stuff.
 * It does all the turning on/off and stuff things for the informs.
 * @param str the informs to listen to
 * @see event_inform()
 */
nomask protected int do_inform(string str) {
  string type, *types, *on, *frog, *off, *failed;
  int i;

  types = query_inform_types();
  foreach(type in inform_types) {
    if(member_array(type, types) == -1) {
      inform_types -= ({ type });
    }
  }

  on = inform_types;
  
  if (!on) {
    on = ({ });
  }
  if (!str) {
    str = "";
  }
  frog = explode(str, " ");
  if (!frog) {
    frog = ({ });
  } else {
    frog -= ({ "", 0 });
  }
  if (!sizeof(frog)) {
    /* show status */
    if (this_object()->query_property("inform repressed")) {
      write("Your informs are currently being repressed.\n");
    }
    if (sizeof(on)) {
      write("$I$5=You will be informed of "+ query_multiple_short(on)+
            " events.\n");
    }
    types -= on;
    if (sizeof(types)) {
      write("$I$5=You are not being informed of "+ query_multiple_short(types)+
            " events.\n");
    }
    return 1;
  }
  if (sizeof(frog) == 1) {
    switch (frog[0]) {
    case "on" :
      this_object()->remove_property("inform repressed");
      write("You are now being informed.  This is true!\n");
      if (sizeof(on)) {
         write("$I$5=You will be informed of "+ query_multiple_short(on)+
               " events.\n");
      } else {
         write("$I$5=Although you are not currently being informed of "
               "any events.\n");
      }
      return 1;
    case "off":
      this_object()->add_property("inform repressed", 1);
      write("Informs are now repressed.\n");
      return 1;
    case "all":
      on = types;
      frog = frog[1..];
      break;
    case "none" :
      on = ({ });
      write("You will not be informed of anything.\n");
      frog = frog[1..];
      break;
    }
  }
  failed = off = ({ });
  for (i=0;i<sizeof(frog);i++) {
    if (member_array(frog[i], types) == -1) {
      failed += ({ frog[i] });
    } else {
      if (sizeof(frog) > i+1) {
        switch (frog[i+1]) {
        case "on" :
          if (member_array(frog[i], on) == -1)
            on += ({ frog[i] });
          i++;
          break;
        case "off" :
          off += ({ frog[i] });
          i++;
          break;
        default :
          if (member_array(frog[i], on) == -1) {
            on += ({ frog[i] });
          } else {
            off += ({ frog[i] });
          }
          break;
        }
      } else {
        if (member_array(frog[i], on) == -1) {
          on += ({ frog[i] });
        } else {
          off += ({ frog[i] });
        }
      }
    }
  }
  on -= off;
  if (sizeof(failed)) {
    write("$I$5=I don't know about "+ query_multiple_short(failed)+
          " events.\n");
  }
  if (sizeof(off)) {
    write("$I$5=You will now not be informed of "+ query_multiple_short(off)+
          " events.\n");
  }
  if (sizeof(on)) {
    write("$I$5=You will now be informed of "+ query_multiple_short(on)+
          " events.\n");
  }
  inform_types = on;
  return 1;
} /* do_inform() */

/**
 * This method tests to see if the player can see octarine objects.
 * @return 1 if they can see octarine, 0 if they cannot
 */
int query_see_octarine() {
  string guild;

  if ( this_object()->query_creator() )
    return 1;
  if(!(guild = (string)this_object()->query_guild_ob())) return 0;
  return (int)guild->query_see_octarine();
} /* query_see_octarine() */

/**
 * This method will do the octarine checking for you on the message.
 * @param str the string to check
 * @see query_see_octarine()
 * @example
 * string query_long_bit() {
 *   return "Glinting and sharp looking, the long sword is an impressive "
 *          "weapon in the hands of someone knowledgeable.\n" +
 *          this_player()->octarine_message("It glows with a fluffy octarine "
 *                                          "aura.\n");
 * } /\* query_long_bit() *\/
 */
string octarine_message(string str) {
  if (query_see_octarine())
    return str;
  return "";
} /* octarine_message() */

/**
 * This function returns the appropriate colour codes for the given event
 * type. It will return the players chosen colour if they've set one or
 * the default if not.
 *
 * @param event_type The type of event
 * @return a string of colour codes.
 */
string colour_event(string event_type, string default_colour) {
  if (my_colours[event_type]) {
    return replace(my_colours[event_type], " ", "");
  }

  if (!default_colour) {
    return "";
  }

  return default_colour;
} /* colour_event() */

/**
 * This method handles the conversersions for the colour mapping which is
 * done on Discworld.  This also does octarine message expansion, if the
 * keyword %\^OCTARINE:message%\^ is embeded into a string then the
 * inside section will be replaced with a blank string if the player
 * cannot see octarine messages.
 * @param str the string to do the conversion on
 * @param width the width of the string
 * @param indent the size of the indent
 * @param args any other arguments
 * @return the fixed up string
 * @see efun::terminal_colour()
 */
varargs string fix_string(string str, int width, int indent, int padding, mixed *args ...) {
  string octmess, *bits, bit;
  int i;

  if (!stringp(str) || str == "") {
    return str;
  }

  if(indent > width / 3) {
    indent = 4;
  }

  if (!_event_info->cur_term) {
    if (term_name != "network") {
      _event_info->cur_term = term_name;
    } else {
      _event_info->cur_term = "dumb";
    }
  }
  if (!_event_info->colour_map) {
    _event_info->colour_map = (mapping)TERM_HANDLER->set_term_type(_event_info->cur_term, query_property(PLAYER_ALLOW_COLOURED_SOULS));
  }
  if (sizeof(args)) {
    str = sprintf(str, args ...);
  }
  bits = explode(str, "%^OCTARINE:");
  foreach (bit in bits) {
    i = strsrch(bit, "%^");
    if (i == -1) {
      continue;
    }
    octmess = bit[0..i-1];
    if (query_see_octarine()) {
      str = replace_string(str, sprintf("%%^OCTARINE:%s%%^", octmess),
                           octmess);
    } else {
      str = replace_string(str, sprintf("%%^OCTARINE:%s%%^", octmess), "");
    }
  }
  // If someone has mxp then we can do the formatting in a different
  // way.
  if (is_mxp_enabled()) {
  //if (query_name() == "greenland") {
    str = terminal_colour(str, _event_info->colour_map);
  } else {
    str = terminal_colour(str, _event_info->colour_map, width, indent);
  }
  return str;
} /* fix_string() */

/* Send all the relevant junk down to get the information we want. */
/** @ignore yes */
void player_connected() {
  _event_info->last_term = 0;
  /* Turn on the option */
  printf("%c%c%c", IAC, DO, TELOPT_TTYPE);
  /* Get them to send us their first terminal type */
  printf("%c%c%c%c%c%c", IAC, SB, TELOPT_TTYPE, TELQUAL_SEND, IAC, SE);
  /* Get them to send us their window size */
  printf("%c%c%c", IAC, DO, TELOPT_NAWS);
} /* player_connected() */

/**
 * This method sets the current terminal type for the player.  If the type
 * is network, then the network will be queried for the terminal type
 * and that will be used.  This is the distiction between the current
 * terminal type and the terminal name.  The name will be the value that
 * is set by the player and the current type will be the information
 * garnered from the network (if the name is network) or the same as the
 * name.
 * @param str the new terminal type
 * @return 1 if the terminal was successful set, 0 if not
 * @see set_term()
 * @see query_term_name()
 * @see query_cur_term()
 */ 
int set_term_type(string str) {
  if (!str) {
    notify_fail(sprintf("%-=*s", cols,
                "Syntax: "+query_verb()+" <term_type>\n"+
                "Where term type is one of the following: "+
                implode((string *)TERM_HANDLER->query_term_types(), ", ")+
                ".\nOr set it to \"network\", and the mud will try to "
                "figure it out itself.\n"));
    return 0;
  }
  if(str != term_name) {
    if (member_array(str, (string *)TERM_HANDLER->query_term_types()) != -1 ||
        str == "network") {
      if (str != "network") {
        _event_info->colour_map = (mapping)TERM_HANDLER->set_term_type(str, query_property(PLAYER_ALLOW_COLOURED_SOULS));
      } else { /* Get em to send us our terminal type */
        player_connected();
      }
      term_name = str;
      _event_info->cur_term = 0;
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

/** @ignore yes */
void set_term(string name) {
  term_name = name;
  _event_info->cur_term = 0;
} /* set_term() */

/**
 * This method handles the responses from the remote site informing
 * us of their terminal type.
 * @param name the terminal type gathered from the remote site
 * @return 0 if the type was not used, 1 if it was
 * @see set_term_type()
 * @see query_term_name()
 * @see query_cur_term()
 */
int set_network_terminal_type(string name) {
  if (term_name != "network") return 0;
  if (!name) {
    _event_info->cur_term = "dumb";
    _event_info->colour_map = 0;
  }
  if (_event_info->colour_map =
      (mapping)TERM_HANDLER->set_network_term_type(name, query_property(PLAYER_ALLOW_COLOURED_SOULS))) {
    _event_info->cur_term = name;
    return 1;
  }
} /* set_network_terminal_type() */

/**
 * This method resets the colour map information on the player.  THis is
 * called when some change is made to the terminal data and it needs to
 * be updated.
 */
void reset_colour_map() {
   _event_info->colour_map = 0;
}

/*
mapping query_debug_colour_map() {
   return _event_info->colour_map;
}
 */

/** 
 * This method adds all the event commands onto the player.
 * This will be called in the player startup sequence.
 */
void event_commands(){
  add_command("rows", this_object(), "", (: set_our_rows(0) :));
  add_command("cols", this_object(), "", (: set_our_cols(0, 0) :));
  add_command("rows", this_object(), "<number>", (: set_our_rows($4[0]) :));
  add_command("cols", this_object(), "<number>", (: set_our_cols($4[0], 0) :));
  add_command("cols", this_object(), "test <number>", (: set_our_cols($4[0], 1) :));
  add_command("term", this_object(), "{" + 
                implode((string *)TERM_HANDLER->query_term_types() +
                ({ "network" }), "|")+"}", (: set_term_type($4[0]) :));
  add_command("term", this_object(), "", (: set_term_type(0) :));
  add_command("inform", this_object(), "", (: do_inform(0) :));
  add_command("inform", this_object(), "<string>", (: do_inform($4[0]) :));
  
  /* If a liaison or a lord... */
  if (this_object()->query_creator()) {
    add_command("busy", this_object(), "{on|off}", (: do_busy($4[0]) :));
    add_command("busy", this_object(), "<indirect:player>",
                (: do_busy_player($1) :));

  }
}

/**
 * Return function pointer for do_tell_his, for htell only.
 */

function get_htell_func(){
  if(file_name(previous_object()) != "/cmds/player/ht_ell")
    return 0;
  return (:do_tell_his:);
}

/**
 * This method is the command used to print a players tell history.
 */
protected varargs int do_tell_his(string str, int brief) {
  string ret;
  mixed bit, *filter_history;

  if (str != "")  {
    filter_history = filter(tell_history,
                            (: strsrch(lower_case($1[0]), $(str)) > -1 :));
  }
  else filter_history = tell_history;
  if (!pointerp(filter_history) || !sizeof(filter_history)) {
    return notify_fail("You have not been told anything.\n");
  }
  if (undefinedp(brief))  brief = 0;
  ret = "$P$Tell History$P$Your tell history is:\n";
  foreach (bit in filter_history) {
    if (!brief) {
      ret += "** "+ctime(bit[2])+" **\n";
    }
    ret += fix_string("%s%s\n", cols, strlen(bit[0]), 0, bit[0], bit[1]);
  }
  write(ret);
  return 1;
} /* do_tell_his() */

/**
 * This method handles setting the busy flag.  The busy flag can only
 * be set by liaisons and lords, why lords?  Just because :)  This
 * command was inspired by moonchild.
 * @param str the on or off string
 * @return 1 on success, 0 on failure
 */
int do_busy(string str) {
   if (str == "on") {
      _event_info->busy = 1;
   } else {
      _event_info->busy = 0;
   }
   write("Busy set to " + str + ".\n");
   return 1;
} /* do_busy() */

/**
 * This method allows creators to set the player for whom they are currently
 * busy too.
 * @param obs the player to be busy with
 * @return 1 on success, 0 on failure
 */
int do_busy_player(object *obs) {
   write("Ok, setting you as busy with " + query_multiple_short(obs) + ".\n");
   _event_info->busy = obs;
   return 1;
} /* do_busy_player() */

/**
 * This method tells us if the player/creator/lord is currently in busy
 * mode.  This will be 1 if the creatopr is generaly busy, or
 * it will return the array of players they are busy with.
 * @return the busy mode flag
 */
mixed query_busy() {
   if (pointerp(_event_info->busy)) {
      _event_info->busy -= ({ 0 });
   }
   if (pointerp(_event_info->busy) && !sizeof(_event_info->busy)) {
      _event_info->busy = 0;
   }
   return _event_info->busy;
} /* query_busy() */
 
/**
 * This method will change the current value of the earmuffs on the player.
 * @see check_earmuffs()
 * @see query_earmuffs()
 */
void toggle_earmuffs() {
  earmuffs = !earmuffs;
} /* toggle_earmuffs() */

/**
 * This method will set the system to allow friends to punch through
 * earmuffs.
 * @see check_earmuffs()
 * @see query_earmuffs()
 */
void set_allow_friends_earmuffs() {
  earmuffs = PLAYER_ALLOW_FRIENDS_EARMUFF;
} /* toggle_earmuffs() */

/**
 * This method checks to see if a particular event is earmuffed.
 * @param type the type of event to check
 * @param person the person who is being checked for the earmuffing of
 * @return 1 if the event is earmuffed and 0 if it is not
 * @see toggle_earmuffs()
 * @see query_earmuffs()
 * @example
 * obs = users();
 * 
 * obs = filter(obs, (: $1->check_earmuffs("shout", this_player()) :) );
 * // Do the shout
 */
int check_earmuffs(string type, object person) {
  string *on;

  if (!earmuffs) {
    return 0;
  }
  on = (string *)this_object()->query_property(PLAYER_EARMUFF_PROP);
  if (!on) {
     return 0;
  }
  if (member_array(type, on) == -1) {
    return 0;
  }
  // If they are a friend they punch through
  if (!person) {
    person = this_player();
  }
  if (person && earmuffs == PLAYER_ALLOW_FRIENDS_EARMUFF) {
    if (is_friend(person->query_name())) {
      return 0;
    }
  }
  return 1;
} /* check_earmuffs() */

/**
 * This method sets the number of rows on the players screen.
 * @param i the new number of rows
 * @see query_rows()
 */
void set_rows(int i) {
  if (i < 5) {
     return ;
  }
  rows = i;
} /* set_rows() */

/**
 * This method returns the current number of rows the player has
 * set on their screen.
 * @return the number of rows on the screen
 * @see set_rows()
 */
int query_rows() { return rows; }

private int set_our_rows(int val) {
  if (!val) {
    notify_fail("Rows currently set to " + rows + ".\nrows <number> to set.\n");
    return 0;
  }
  if (val <= 10) {
    notify_fail("Invalid number of rows.\n");
    return 0;
  }
  write("Rows set to " + val + ".\n");
  rows = val;
  return 1;
} /* set_our_rows() */

/**
 * This method returns the current number of columns the player has
 * set on their screen.
 * @return the number of columns on the screen
 * @see set_cols()
 */
int query_cols() { return cols; }

/**
 * This method sets the current number of columns the player has set on
 * their screen.
 * @param i the new number of columns
 * @see query_cols()
 */
void set_cols(int i) {
  if (i <= 10 || i > 999)
     return ;
   cols = i;
} /* set_cols() */

private int set_our_cols(int val, int test) {
  int cur;

  if(!val) {
    notify_fail("Columns currently set to " + cols + ".\ncols <number> to set.\n");
    return 0;
  }
  if(val <= 35 || val > 999) {
    notify_fail("Invalid column size.\n");
    return 0;
  }

  if (test)  {
    cur = cols;
    cols = val;
    /* Watch this.... */
    write(implode(allocate(val, (: sprintf("%d", ($1 + 1) % 10) :)), "") + "\n");
    call_out((: cols = $(cur) :), 3);
    return 1;    
  }
  write("Columns set to " + val + ".\n");
  cols = val;
  return 1;
} /* set_our_cols() */

/**
 * set the colour codes for a given event
 */
void set_my_colours(string event_type, string colour) {
  if(colour == "default") {
    map_delete(my_colours, event_type);
  } else if(colour == "none") {
    my_colours[event_type] = "";
  } else {
    my_colours[event_type] = colour;
  }
}

/**
 * Return a players list of custom colours.
 * @return a mapping of the players custom colours for different events.
 */
mapping query_my_colours() {
  return my_colours;
}

void set_looked( object thing ) { _event_info->where = thing; }

/**
 * This method does all those terrible things with messages and $'s
 * turning them into real strings.  It is a neat function if somewhat
 * complicated :)  It was written by Deutha.
 * <p>
 * The return array has two elements, the first being the reformed
 * message and the second being the reformed things array.
 * @param message the message to reform
 * @param things some bonus things to reform it with
 * @return an array consisting of two elements
 */
mixed *reform_message( string message, mixed *things ) {
   int last, number;
   string before, middle, after, info;

   last = -1;
   if ( !things ) {
      things = ({ });
   } else {
      last = sizeof(things) - 1;
   }
   if ( !_event_info->where ) {
      _event_info->where = environment();
   }
   if (strsrch(message, "$L$") != -1) {
      while ( sscanf( message, "%s$L$%s$L$%s", before, middle, after ) == 3 ) {
         if (sscanf( middle, "[%s]%s", info, middle) == 2) {
            if (info[0..4] == "read:") {
               middle = LANGUAGE_HAND->garble_text(info[5..], middle, 0, this_object());
            } else {
               middle = LANGUAGE_HAND->garble_text_say(middle, info, this_object());
            }
         }
         message = sprintf("%s%s%s", before, middle, after);
      }
   }

   if (strsrch(message, "$R$") != -1) {
      while ( sscanf( message, "%s$R$%s$R$%s", before, middle, after ) == 3 ) {
         if ( sscanf( middle, "[%s]%s", info, middle ) != 2 ) {
            info = "";
         }
         switch ( middle[ 0 ] ) {
            case '-' :
               number = 0;
               middle = middle[ 1 .. ];
               break;
            case '+' :
               number = 1;
               middle = middle[ 1 .. ];
               break;
            default :
               number = 2;
         }
         if ( ( number == 2 ) || _event_info->where->query_relative( middle ) ) {
            if ( this_object()->query_creator() ) {
               message = sprintf("%s%s (%s)%s", before, find_rel(middle, number),
                                 middle, after);
            } else {
               message = sprintf("%s%s%s", before, find_rel(middle, number),
                                 after);
            }
         } else {
            message = sprintf("%s%s%s%s", before, info, middle, after);
         }
      }
   }
   if (strsrch(message, "$r$") != -1) {
      while ( sscanf( message, "%s$r$%s$r$%s", before, middle, after ) == 3 ) {
         if ( sscanf( middle, "[%s]%s", info, middle ) != 2 ) {
            info = "";
         }
         switch ( middle[ 0 ] ) {
            case '-' :
               number = 0;
               middle = middle[ 1 .. ];
               break;
            case '+' :
               number = 1;
               middle = middle[ 1 .. ];
               break;
            default :
               number = 2;
         }
         if ( ( number == 2 ) || _event_info->where->query_relative( LENGTHEN[ middle ] ) ) {
            if ( this_object()->query_creator() ) {
               message = sprintf("%s%s (%s)%s", before,
                                 SHORTEN[find_rel(LENGTHEN[middle], number)],
                                 middle, after);
            } else {
               message = sprintf("%s%s%s", before,
                                 SHORTEN[find_rel(LENGTHEN[middle], number)],
                                 after);
            }
         } else {
            message = sprintf("%s%s%s%s", before, info, middle, after);
         }
      }
   }
#ifdef DEBUG
   if ( this_object()->query_property( "see eemessages" ) )
      efun::tell_object( this_object(), "Looking at "+ message +".\n" );
#endif   
   if (strsrch(message, "$M$") != -1) {
      while ( sscanf( message, "%s$M$%s$M$%s", before, middle, after ) == 3 ) {
#ifdef DEBUG     
         if ( this_object()->query_property( "see eemessages" ) )
            efun::tell_object( this_object(), "Expanding "+ middle +".\n" );
#endif      
         last++;
         message = sprintf("%s$%d$%s", before, last, after);
         things += ({ ({ }) });
         if (strsrch(middle, "$") == -1) {
            things[ last ] += ({ middle });
            middle = 0;
         } else {
            while ( sscanf( middle, "$%s$%s", info, middle ) == 2 ) {
#ifdef DEBUG        
               if ( this_object()->query_property( "see eemessages" ) ) {
                  efun::tell_object( this_object(), "Found "+ info +".\n" );
               }
#endif      
               things[ last ] += ({ "my_"+ info });
            }
         }
      }
   }
   if (strsrch(message, "$") != -1) {
      after = message;
      message = "";
      while ( sscanf( after, "%s$%s$%s", before, middle, after ) == 3 ) {
#ifdef DEBUG     
         if ( this_object()->query_property( "see eemessages" ) )
            efun::tell_object( this_object(), "Trying "+ middle +".\n" );
#endif      
         if ( sscanf( middle, "%s_short:%s", middle, info ) != 2 ) {
            message = sprintf("%s%s$%s", message, before, middle);
            after = sprintf("$%s", after);
            continue;
         }
         last++;
         message = sprintf("%s%s$%d$", message, before, last);
         things += ({ ({ sprintf("my_%s_short:%s", middle, info) }) });
      }
      message += after;
   }
   return ({ message, things });
} /* reform_message() */

/**
 * This adds a message into the current list of printable messages.  This
 * will be squided up together and printed out slightly later, this handles
 * the concatenating of enter messages, and soul messages.  Etc.
 * @param message the message to add
 * @param things the objects which are involved with the message
 * @see reform_message()
 */
void add_message( string message, mixed *things ) {
   int last;
   mixed *stuff;

   if ( !interactive( this_object() ) ) {
      return;
   }
   if ( strsrch( message, "$" ) == -1 ) {
      stuff = ({ message, things });
   } else  {
      stuff = reform_message( message, things );
   }
   _event_info->where = 0;
   if ( !sizeof( _event_info->eemessages ) ) {
      _event_info->eemessages = stuff;
   } else {
      last = sizeof( _event_info->eemessages ) - 2;
      if ( ( stuff[ 0 ] == _event_info->eemessages[ last ] ) &&
            ( sizeof( stuff[ 1 ] ) == 1 ) ) {
         if ( sizeof( filter( stuff[ 1 ][ 0 ], (: member_array($1, $2) != -1 :),
               _event_info->eemessages[ last + 1 ][ 0 ] ) ) ) {
            _event_info->eemessages += stuff;
         } else {
            _event_info->eemessages[ last + 1 ][ 0 ] += stuff[ 1 ][ 0 ]; 
         }
      } else {
         _event_info->eemessages += stuff;
      }
   }
#ifdef DEBUG
   if ( this_object()->query_property( "see eemessages" ) )
      efun::tell_object( this_object(), sprintf( "%O\n", _event_info->eemessages ) );
#endif   
} /* add_message() */

/**
 * @ignore yes 
 * This method is used for the reform_mesage stuff
 */
string get_pretty_short( object thing, int dark ) {
  if (dark && environment(thing) != this_object()) {
    if (living(thing) && thing->query_race_ob() &&
        (thing->query_race_ob())->query_humanoid()) {
      return "someone";
    } else {
      return "something";
    }
  }

  if ( !thing->query_visible( this_object() ) ) {
    if (living(thing) && thing->query_race_ob() &&
        (thing->query_race_ob())->query_humanoid()) {
      return "someone";
    } else {
      return "something";
    }
  }

#ifdef USE_ROLE_PLAYING_SHORTS
   if(interactive(thing) &&
      this_object()->query_role_playing() &&
      !this_object()->is_friend(thing->query_name())) {
     return thing->query_short_desc();
   }
#endif
   
   return (string)thing->pretty_short( this_object() ) +
         (string)thing->hide_invis_string();
} /* get_pretty_short() */

/**
 * @ignore yes
 * This method is used for the reform_mesage stuff
 */
string get_pretty_plural( object thing, int dark ) {
   if (dark && environment(thing) != this_object()) {
      if (living(thing)) {
         return "people";
      } else {
         return "things";
      }
   }

   if ( !thing->query_visible( this_object() ) ) {
      return "things";
   }

#ifdef USE_ROLE_PLAYING_SHORTS
   if(interactive(thing) &&
      this_object()->query_role_playing() &&
      !this_object()->is_friend(thing->query_name())) {
     return thing->query_plural_desc();
   }
#endif

   return (string)thing->pretty_plural( this_object() ) +
         (string)thing->hide_invis_string();
} /* get_pretty_plural() */

/**
 * This is a special function for use with the reform message, it allows
 * the string to be echoed into the list instead of using the object's
 * value itself.  This is done specifically to handle objects with
 * variable shorts, so we get the short description correct at the
 * moment it is queried.
 */
string my_mirror_short( object thing, string arg) {
   if (objectp(thing)  &&  userp(thing))
      return thing->short(0, 0);
   return arg;
} /* my_mirror_short() */

/**
 * @ignore yes
 * This method is used for the reform_mesage stuff
 */
string my_a_short( object thing, string arg, int dark) {
   string article;

   if (!thing) {
      return "something";
   }
   if (dark && environment(thing) != this_object()) {
      // Allow remote shorts to work.
     if (living(thing) && thing->query_race_ob() &&
         (thing->query_race_ob())->query_humanoid()) {
       if (environment(thing) != environment()) {
         return "someone";
       }
     } else {
       return "something";
     }
   }

   if ( !objectp( thing ) ) {
     return "an unknown object";
   }

   article = (string)thing->query_determinate( this_object() );
   if ( !article ) {
      return add_a( (string)this_object()->get_pretty_short( thing, dark ) );
   }
   return article + (string)this_object()->get_pretty_short( thing, dark );
} /* my_a_short() */

/**
 * @ignore yes
 * This method is used for the reform_mesage stuff
 */
string my_the_short( object thing, string arg, int dark ) {
   string article;

   if (dark && environment(thing) != this_object()) {
      // Allow remote shorts to work.
     if (living(thing) && thing->query_race_ob() &&
         (thing->query_race_ob())->query_humanoid()) {
         if (environment(thing) == environment()) {
            return "someone";
         }
      } else {
         return "something";
      }
   }

   if ( !objectp( thing ) ) {
      return "the unknown object";
   }

   article = (string)thing->query_determinate( this_object() );
   if ( !article || ( article == "a " ) || ( article == "an " ) )
      return "the "+ (string)this_object()->get_pretty_short( thing, dark );
   return article + (string)this_object()->get_pretty_short( thing, dark );
} /* my_the_short() */

int some_more( string word ) {
    return sizeof( filter( all_inventory( _event_info->where ),
        (: $1->query_plural() == $(word) &&
           $1->query_visible(this_object()) :))) > 1;
} /* some_more() */

/**
 * @ignore yes
 * This method is used for the reform_mesage stuff
 */
string my_one_short( object thing, string arg, int dark ) {
   string article, its_plural;

   if (dark && environment(thing) != this_object()) {
      // Allow remote shorts to work.
     if (living(thing) && thing->query_race_ob() &&
         (thing->query_race_ob())->query_humanoid()) {
         if (environment(thing) == environment()) {
            return "someone";
         }
      } else {
         return "something";
      }
   }

   if ( !objectp( thing ) ) {
      return "one of the unknown objects";
   }
   if ( environment( thing ) != environment() ) {
      _event_info->where = this_object();
   } else {
      _event_info->where = environment();
   }

   /* return my_a_short( thing ); */
   article = (string)thing->query_determinate( this_object() );
   if ( !article || ( article == "a " ) || ( article == "an " ) ) {
      its_plural = (string)this_object()->get_pretty_plural( thing, dark );
      if ( some_more( its_plural ) ) {
         return "one of the "+ its_plural;
      }
      return "the "+ (string)this_object()->get_pretty_short( thing, dark );
   }
   return article + (string)this_object()->get_pretty_short( thing, dark );
} /* my_one_short() */

/**
 * @ignore yes
 * This method is used for the reform_mesage stuff
 */
string my_poss_short(object thing, string arg, int dark) {
   string its_plural;
   string article;
   string of_whom;

   if (dark && environment(thing) != this_object()) {
      // Allow remote shorts to work.
     if (living(thing) && thing->query_race_ob() &&
         (thing->query_race_ob())->query_humanoid()) {
         if (environment(thing) == environment()) {
            return "someone";
         }
      } else {
         return "something";
      }
   }

   if ( !objectp( thing ) ) {
      return "an unknown object";
   }
   if(living(thing)) {
     if (thing == this_object()) {
       return "your";
     } else {
       of_whom = this_object()->get_pretty_short( thing, dark );
       if(of_whom[<1] == 's')
         of_whom = of_whom + "'";
       else
         of_whom = of_whom +"'s";
       return of_whom;
     }
   }
   _event_info->where = environment( thing );
   if ( !_event_info->where ) {
      return my_a_short( thing, arg, dark );
   }
   if ( !living( _event_info->where ) && !_event_info->where->query_corpse() ) {
      return my_a_short( thing, arg, dark );
   }
#ifdef DEBUG
   if ( this_object()->query_property( "see eemessages" ) )
      efun::tell_object( this_object(), sprintf( "%O\n", _event_info->had_shorts ) );
#endif   
   if ( _event_info->where == this_object() ) {
      of_whom = "your ";
   } else {
      if ( member_array( _event_info->where, _event_info->had_shorts ) != -1 ) {
         of_whom = (string)_event_info->where->query_possessive() +" ";
      } else {
        of_whom = my_the_short( _event_info->where, arg, dark );
        if(of_whom[<1] == 's') {
          of_whom = of_whom + "' ";
        } else {
          of_whom = of_whom +"'s ";
        }
      }
   }
   if ( !article || ( article == "a " ) || ( article == "an " ) ) {
      its_plural = (string)this_object()->get_pretty_plural( thing, dark );
      if ( some_more( its_plural ) ) {
         return "one of "+ of_whom + its_plural;
      }
   }
   return of_whom + (string)this_object()->get_pretty_short( thing, dark );
}

/**
 * @ignore yes
 * This method is used for the reform_mesage stuff
 */
string my_the_poss_short(object thing, string arg, int dark) {
   string its_plural;
   string article;
   string of_whom;

   if (dark && environment(thing) != this_object()) {
      // Allow remote shorts to work.
      if (living(thing)) {
         if (environment(thing) == environment()) {
            return "someone's";
         }
      } else {
         return "something's";
      }
   }

   if ( !objectp( thing ) ) {
      return "an unknown object's";
   }

   article = (string)thing->query_determinate(this_object());
   if(!article || (article == "a " || article == "an "))
     article = "the ";

   if(living(thing)) {
     if (thing == this_object()) {
       return "your";
     } else {
       of_whom = this_object()->get_pretty_short( thing, dark );
       if(of_whom[<1] == 's')
         of_whom = of_whom + "'";
       else
         of_whom = of_whom +"'s";
       return article + of_whom;
     }
   }
   _event_info->where = environment( thing );
   if ( !_event_info->where ) {
      return my_the_short( thing, arg, dark );
   }
   if ( !living( _event_info->where ) && !_event_info->where->query_corpse()) {
      return my_the_short(thing, arg, dark);
   }
#ifdef DEBUG
   if ( this_object()->query_property( "see eemessages" ) )
      efun::tell_object(this_object(), sprintf("%O\n",
                                               _event_info->had_shorts));
#endif   
   if ( _event_info->where == this_object() ) {
     of_whom = "your ";
   } else {
     if(member_array( _event_info->where, _event_info->had_shorts ) != -1) {
       debug_printf("Had shorts");
       of_whom = (string)_event_info->where->query_possessive() +" ";
     } else {
       of_whom = my_the_short(_event_info->where, arg, dark);
       if(of_whom[<1] == 's') {
         of_whom = of_whom + "' ";
       } else {
         of_whom = of_whom +"'s ";
       }
     }
   }
   
   its_plural = (string)this_object()->get_pretty_plural( thing, dark );
   if(some_more(its_plural))
     return "one of "+ article + of_whom + its_plural;
   
   return of_whom + article + this_object()->get_pretty_short(thing, dark);
}

/** @ignore yes */
protected void clear_event_info_had_shorts() { _event_info->had_shorts = ({ }); }

/**
 * @ignore yes
 * This method is used for the reform_mesage stuff
 */
varargs string calc_shorts( string *short_list, int exact ) {
   int i;
   int dark;
   string list;
   string str;
   string desc;
   object ob;
   object *things;
   //mapping shorts;
   // These next two are tied.
   string *descs_str;
   mixed *descs_ob;
   mixed *parts;
   function number;

   descs_str = ({ });
   descs_ob  = ({ });
   if (!_event_info->had_shorts)  clear_event_info_had_shorts();
   if (exact) {
      number = (: query_num($1) :);
   } else {
      number = (: query_num($1, 20) :);
   }
   if (environment()) {
      dark = this_object()->check_dark(environment()->query_light());
   }
   foreach (str in short_list) {
      parts = explode( str, ":" );
      if (sizeof(parts) == 2) {
         ob = find_object( parts[ 1 ] );
      } else if (sizeof(parts) > 2) {
         parts[1] = implode(parts[ 1.. ], ":");
         ob = find_object( parts[1] );
      } else {
         ob = 0;
         parts += ({ "" });
      }
#ifdef DEBUG
      if ( this_object()->query_property( "see eemessages" ) )
         efun::tell_object( this_object(), sprintf( "%O\n", ob ) );
#endif      
      if (ob) {
         _event_info->had_shorts = ({ ob }) + _event_info->had_shorts;
      }
      if ( ob == this_object() ) {
         if (parts[0] == "my_poss_short" || parts[0] == "my_the_poss_short") {
            desc = "your";
         } else {
            desc = "you";
         }
      } else if (sizeof(parts) >= 2) {
#ifdef DISBALED
        // We were crashing a lot in this call so the code below is a
        // workaround attempt to see if we can prevent the crashing.
         desc = (string)call_other( this_object(), parts[0], ob, parts[1], dark);
#else
         switch(parts[0]) {
         case "my_mirror_short":
           desc = my_mirror_short(ob, parts[1]);
           break;
         case "my_a_short":
           desc = my_a_short(ob, parts[1], dark);
           break;
         case "my_the_short":
           desc = my_the_short(ob, parts[1], dark);
           break;
         case "my_one_short":
           desc = my_one_short(ob, parts[1], dark);
           break;
         case "my_poss_short":
           desc = my_poss_short(ob, parts[1], dark);
           break;
         case "my_the_poss_short":
           desc = my_the_poss_short(ob, parts[1], dark);
           break;
         }
#endif
      } else {
         if (sizeof(parts[0])) {
            desc = parts[0];
         } else {
            desc = "something";
         }
      }
      i = member_array(desc, descs_str);
      if (i == -1) {
         descs_str += ({ desc });
         descs_ob += ({ ({ ob }) });
      } else {
         descs_ob[i] += ({ ob });
      }
   }
   i = member_array( "you", descs_str );
   if ( i != -1 && i != sizeof(descs_str) - 1) {
      descs_str = descs_str[0..i-1] + descs_str[i+1..] + ({ "you" });
      descs_ob = descs_ob[0..i-1] +  descs_ob[i+1..] + descs_ob[i..i];
   }
   list = "";
/*
   if (dark) {
     if(sizeof(short_list) > 1) {
        list = "somethings";
     } else {
        if(descs_ob[0] && living(descs_ob[0])) {
           list = "someone";
        } else {
           list = "something";
        }
     }
   } else */ {
      for ( i = 0; i < sizeof( descs_str ); i++ ) {
         things = descs_ob[ i ];
         if ( sizeof( things ) == 1 ) {
            if (exact)  {
               if (things[0]) {
                  list += "one " + things[0]->short();
               } else {
                  list += descs_str[i];
               }
            } else {
               list += descs_str[ i ];
            }
         } else {
            things -= ({ 0 });
            if ( sizeof(things) ) {
               list += evaluate(number, sizeof(things)) + " " +
                     this_object()->get_pretty_plural( things[ 0 ], dark );
            } else {
               list += evaluate(number, sizeof(things)) + " unknown objects";
            }
         }
         if ( i == sizeof( descs_str ) - 1 ) {
            continue;
         }
         if ( i == sizeof( descs_str ) - 2 ) {
            list += " and ";
         } else {
            list += ", ";
         }
      }
   }
   return list;
} /* calc_shorts() */

/**
 * This method handles doing the indenting of the input string, fitting it
 * correctly to the width of the column.
 * @param part the column to do stuff with.
 */
string indent_column(string column, int width, int pad) {
   int left;
   int right;
   int space;
   string *parts;
   string part;
   mixed stuff;
   string ret;

   ret = "";
   if ( column[ 0 .. 2 ] != "$I$" ) {
      column = "$I$0=$C$"+ column;
   }
   parts = explode( column, "$C$" );
   parts = parts[0..0] + map(parts[1..], (: capitalize($1) :));
   parts = explode( implode(parts, ""), "$I$" );
   foreach ( part in parts) {
      sscanf( part, "%s=%s", stuff, part );
      if ( stuff[ 0 ] == ' ' ) {
         space = !space;
      }
      stuff = explode( stuff, "," );
      if ( stuff[ 0 ] != "" ) {
         switch ( stuff[ 0 ][ 0 ] ) {
            case '+' :
               if ( space && ( left > 0 ) && ( part != "" ) ) {
                  part = SPACES[ 0 .. left - 1 ] + part;
               }
               left += to_int( stuff[ 0 ][ 1 .. ] );
               break;
            case '-' :
               left -= to_int( stuff[ 0 ][ 1 .. ] );
               if ( left < 0 ) {
                  left = 0;
               }
               if ( space && ( left > 0 ) && ( part != "" ) ) {
                  part = SPACES[ 0 .. left - 1 ] + part;
               }
               break;
            default :
               left = to_int( stuff[ 0 ] );
         }
      } else {
         left = 0;
      }
      if ( sizeof( stuff ) > 1 ) {
         switch ( stuff[ 1 ][ 0 ] ) {
            case '+' :
               right += to_int( stuff[ 1 ][ 1 .. ] );
               break;
            case '-' :
               right -= to_int( stuff[ 1 ][ 1 .. ] );
               if ( right < 0 )
                  right = 0;
               break;
            default :
               right = to_int( stuff[ 1 ] );
         }
      } else {
         right = 0;
      }
#ifdef DEBUG
      if ( this_object()->query_property( "see eemessages" ) )
         efun::tell_object( this_object(), part +"\n"+
               left +", "+ right +", "+ cols +"\n" );
#endif      
      // Need to change the terminal_colour efun to allow padding.
      if ( left > 0 ) {
         ret += fix_string( part, width - right, left, pad);
      } else {
         ret += fix_string( part, width - right, 0, pad );
      }
   }
   return ret;
}

/**
 * This method fits a message into a the current players screen size.  This
 * does all sorts of other evil stuff too, like handling indenting and
 * all sorts of things!  Most of which I do not understand so I won't
 * say anything about here.  The $C$ does capitalization, $I$<num>=
 * does indenting of all the following lines of <num> spaces.  The
 * message automatically has $I$0=$C$ prepended to the message if it
 * does not start with $I$.  SO if you start a message with $I$ then it
 * will not capitalize the first letter of the message.
 * @param message the message to fit in
 * @return the fitted message
 */
string fit_message( string message ) {
   int i;
   int j;
   int size;
   int width;
   int ok;
   string* columns;
   string num;
   mixed* stuff;
   string ret;

   if ( strlen( message ) < 6 ) {
      return message;
   }
   // The way the columns work, there is a <num>= after the column
   // definition that handles setting how wide the column is, the
   // last column may have the <num> bit blank.
   columns = explode( message, "$COLUMN$" );
   if (sizeof(columns) > 1) {
      stuff = allocate(sizeof(columns), ({ }));
      width = cols;
      for (i = 0; i < sizeof(columns) - 1; i++) {
         if (sscanf(columns[i], "%s=%s", num, columns[i]) == 2) {
            size = to_int(num);
         } else {
            size = 10;
            columns[i] += "\nMust specify a column size.\n";
         }
         width -= size;
         stuff[i] = ({ size }) + explode(indent_column(columns[i], size, 1), "\n");
      }
      stuff[<1] = ({ width }) + explode(indent_column(columns[<1], width, 0), "\n");
      // Now merge them together.
      j = 1;
      ret = "";
      do {
         // Keep going till the bits run out.
         ok = 0;
         for (i = 0; i < sizeof(stuff); i++) {
            if (sizeof(stuff[i]) > j) {
               // Check the colours.
               if (sizeof(stuff[i]) > j + 1) {
                  ok = 1;
               }
               ret += stuff[i][j];
            } else if (i < sizeof(stuff) -1) {
               // Need to pad it out.
               ret += sprintf("%*s ", stuff[i][0] - 1, "");
            }
         }
         j++;
         ret += "\n";
         ret = fix_string(ret);
      } while (ok);
   } else {
      ret = indent_column(columns[0], cols, 0);
   }
   return ret;
} /* fit_message() */

/**
 * This method does any extra mxp processing on the output string
 * so it will work with the client correctly.
 * @param input the input string
 * @return the output string with mangling
 */
string fix_for_mxp(string input) {
    return replace(input, ({
                          "&", "&amp;",
                          "<", "&lt;", ">", "&gt;", 
                          "\n", mxp_next_secure(this_object()) + "<BR>",
                          "MXP&lt;", "<", "MXP&gt;", ">" }));
}

/**
 * This method shows the message to the player.
 * @param message the message to show
 * @see fit_message()
 */
void show_message( string message ) {
   string bit;

   if ( sscanf( message, "$P$%s$P$%s", bit, message ) == 2 ) {
      this_object()->more_string( fit_message( message ), bit, 1 );
   } else {
      // In here we do some mxp processing if need be.
      if (has_mxp(this_object())) {
         efun::tell_object( this_object(), fix_for_mxp( fit_message( message ) ));
      } else {
         efun::tell_object( this_object(), fit_message( message ) );
      }
   }
} /* show_message() */

/**
 * This is the main evaluation routine.  This is the one that
 * co-ordinates the works...  It is used by the print_messages
 * routine to create the message to print out.
 * <p>
 * The input parameter contains two elements, the first is the
 * message and the second is the things array.  This corresponds to
 * the values returned by the fix_message method.
 * @param stuff the message to evaulate
 * @return the nice printed out string
 * @see print_messages()
 */
string evaluate_message( mixed *stuff ) {
   int i;
   string message, start, finish, verb_sing, verb_plur;

   message = stuff[ 0 ];
   clear_event_info_had_shorts();
   for ( i = 0; i < sizeof( stuff[ 1 ] ); i++ ) {
/* Things assume this ordering, so do not change. */
      message = replace_string( message, "$"+ i +"$",
            calc_shorts( stuff[ 1 ][ i ] ) );
      while ( sscanf( message, "%s$V$"+ i +"=%s,%s$V$%s", start, verb_sing,
            verb_plur, finish ) == 4 ) {
         if ( ( sizeof( stuff[ 1 ][ i ] ) == 1 ) &&
               sizeof(_event_info->had_shorts) &&
               objectp( _event_info->had_shorts[ 0 ] ) ) {
            if ( _event_info->had_shorts[ 0 ]->query_property( "group object" ) ||
                  _event_info->had_shorts[ 0 ]->group_object() ) {
               message = sprintf("%s%s%s", start, verb_plur, finish);
            } else {
               message = sprintf("%s%s%s", start, verb_sing, finish);
            }
         } else {
            message = sprintf("%s%s%s", start, verb_plur, finish);
         }
      }
   }
   return message;
} /* evaluate_message() */

/**
 * This prints out the messages after the delay, printing out the
 * messages in a nice cute way.  It still retains the order of the
 * messages though.  This can be forced to occur by a message
 * occuring on the player object which requires something to
 * be printed.
 * @see evaluate_message()
 * @see show_message()
 */
void print_messages() {
   int i;
   string message;
   mixed *messages;

   messages = _event_info->eemessages;
   _event_info->eemessages = ({ });
   for ( i = 0; i < sizeof( messages ); i += 2 ) {
      message = messages[ i ];
      if ( sizeof( messages[ i + 1 ] ) ) {
         message = evaluate_message( ({ message, messages[ i + 1 ] }) );
      }
      show_message( message );
   }
   _event_info->where = 0;
} /* print_messages() */

/**
 * This method will handle doing exciting things to messages and
 * returning them as a usable format.
 * @param message the message to evaluate
 * @return the message in a printable (to the player) format
 * @see evaluate_message()
 */
string convert_message( string message ) {
  if(message)
    message = evaluate_message( reform_message( message, ({ }) ) );
  _event_info->where = 0;
  return message;
} /* convert_message() */

/**
 * This method is called when an inform event is called.
 * @param mess the message to print
 * @param which the type of inform
 * @parm thing the thing associated with the inform
 */
nomask varargs void event_inform( object, mixed mess, string which,
                           object thing ) {
  string *on, inform_col;
  int add_friend_later;

  on = inform_types;
  if (!on) {
     on = ({ });
  }

  if(this_object()->query_property( "inform repressed" ) || !sizeof( on))
      return;
  if(thing && thing->query_creator() && !thing->query_visible(this_object()))
    return;

  if (which == "logon" && is_friend(thing->query_name()) &&
      member_array("friend", on) != -1 ) {
    which = "friend";
  } else if ( ( which == "logon" || which == "link-death" ) &&
      PLAYERINFO_HANDLER->query_alerts_for(thing->query_name()) &&
      member_array("alert", on) != -1 ) {
    which = "alert";
  } else if(member_array(which, on) == -1 ) {
    return;
  }

  if(INFORM_COLOURS[which]) {
    inform_col = colour_event(which, INFORM_COLOURS[which]);
  } else {
    inform_col = colour_event(which, INFORM_COLOURS["default"]);
  }
  
  if(which == "friend") {
    // check for a friend.
    add_friend_later = 0; 
    
    if(!mess) {
      mess = " -- " + query_friend_tag(thing->query_name());
    } else if ( stringp( mess ) ) { 
      mess += " -- " + query_friend_tag(thing->query_name());
    } else { 
      add_friend_later = 1;
    }
    
    if (INFORM_COLOURS["friend"]) {
      inform_col = colour_event(which, INFORM_COLOURS["friend"]);
    }
  }

  if (functionp(mess)) {
    mess = evaluate(mess, this_object());
    
    if ( stringp( mess ) && add_friend_later ) { 
      mess += "%^RESET%^%^" + inform_col + "%^ -- " + 
        query_friend_tag(thing->query_name());
    }
  }

  if(which == "logon" && this_object()->query_lord() &&
     !check_earmuffs("verbose-logon", this_object()) &&
     strsrch(mess, "enters") != -1)
    mess +=  " %^RESET%^%^" + inform_col + "%^" + query_ip_name(thing);

  this_object()->add_message( "[%^"+ inform_col +"%^"+ mess +"%^RESET%^]\n",                              ({ }) );
} /* event_inform() */

/**
 * This method is called when an object leaves or arrives in
 * the room.  It prints out
 * the message, well adds it to the queue of printable messages.
 * @param mess the message to print
 * @param thing the thing which is leaving/arriving
 * @param going if it is going
 */
protected void enter_exit_mess( string mess, object thing, int going ) {
   int i;
   string part, verb, *words;

   mess = replace_string( mess, "$N", "$0$" );
   words = explode( mess, " " );
   for ( i = sizeof( words ) - 1; i > -1; i-- ) {
     /*
      * If $s ends a word, it represents verb conjugation.
      * The test of the following letter is to check that it does end the word.
      */
      if ( ( sscanf( words[ i ], "%s$s%s", verb, part ) == 2 ) &&
            ( ( part[ 0 ] < 97 ) || ( part[ 0 ] > 122 ) ) ) {
         words[ i ] = "$V$0="+ pluralize( verb ) +","+ verb +"$V$"+ part;
      }
      /*
       * If we have %<verb>%, it represents verb conjugation.
       * This is unlikely to get confused with anything else...
       */
      if ( sscanf( words[ i ], "%%%s%%%s", verb, part ) == 3 ) {
         words[ i ] = "$V$0="+ pluralize( verb ) +","+ verb +"$V$"+ part;
      }
   }
   if ( going ) {
      this_object()->add_message( implode( words, " " ) +"\n",
            ({ ({ "my_the_short:"+ file_name( thing ) }) }) );
   } else {
      this_object()->add_message( implode( words, " " ) +"\n",
            ({ ({ "my_a_short:"+ file_name( thing ) }) }) );
   }
} /* enter_exit_mess() */

/**
 * This method is called when an object enters the room.
 * @param thing the thing entering
 * @param mess the message to print on entering
 */
void event_enter( object thing, string mess, object ) {
  if ( !stringp( mess ) )
    return;

  thing->sneak_task(this_object());
  this_object()->perception_task(thing);

  if ( !thing->query_visible( this_object() ) ) {
    return;
  }

  enter_exit_mess( mess, thing, 0 );
  
} /* event_enter() */

/**
 * This method is printed when an object exits the room.
 * @param thing the object exiting
 * @param mess the message to print
 * @param to where the object is going to
 */
void event_exit( object thing, string mess, object to ) {
   ::event_exit( thing, mess, to );
   if ( !stringp( mess ) )
      return;
   if ( !thing->query_visible( this_object() ) )
      return;
   enter_exit_mess( mess, thing, 1 );
} /* event_exit() */

/** @ignore yes */
void event_death( object thing, object *, object killer,
      string room_mess, string killer_mess ) {
   if ( this_object() == thing ) {
      return;
   }

   // Take dead people out of the attacker list
   //this_object()->remove_attacker_list(thing);
   
   if ( ( this_object() == killer ) && stringp( killer_mess ) ) {
      this_object()->add_message( replace( killer_mess, "$D",
            (string)thing->the_short() ), ({ }) );
      print_messages();
      return;
   }
   if ( stringp( room_mess ) ) {
      if ( objectp( killer ) ) {
         this_object()->add_message( replace( room_mess,
               ({ "$D", (string)thing->the_short(),
               "$K", (string)killer->the_short() }) ), ({ }) );
      } else {
         this_object()->add_message( replace( room_mess, "$D",
               (string)thing->the_short() ), ({ }) );
      }
      print_messages();
   }
} /* event_death() */

/**
 * This method is called when the 'say' and 'tell_room' simul_efuns is used.
 * @param caller the object doing the say
 * @param str the message to print
 * @param avoid the people to avoid in the say
 */
void event_say(object caller, string str, mixed avoid, string language) {
  if (pointerp(avoid)) {
    if (member_array(this_object(), avoid) != -1) {
      return ;
    }
  } else if (avoid == this_object()) {
    return ;
  }
  if (language) {
      this_object()->add_message( LANGUAGE_HAND->garble_text_say(str,
                                  language, this_object()), ({ }) );
   } else {
     this_object()->add_message( str, ({ }) );
   }
} /* event_say() */

void event_see( object caller, string words, object thing, mixed avoid ) {
   if ( !thing->query_visible( this_object() ) )
      return;
   event_say( caller, words, avoid, 0 );
} /* event_see() */

/**
 * This method is called by the simul_efun 'write'.
 * @param caller the calling object
 * @param str the string to write, can be a pointer to two strings with a language
 * @param language the language to write with
 */
void event_write(object caller, string str, string language) {
   if (language) {
     this_object()->add_message( LANGUAGE_HAND->garble_text_say(str,
                                    language, this_object()), ({ }) );
   } else {
      this_object()->add_message( str, ({ }) );
   }
   print_messages();
} /* event_write() */

/** @ignore yes */
void do_efun_write( string str, string language ) {
   event_write( 0, str, language );
}

/**
 * This method is called by the soul to print out the soul messages.
 * @param ob the object doing the soul
 * @param str the string to print
 * @param avoid the people not to print the message to
 */
varargs void event_soul(object ob, string str, mixed avoid) {
  //str = replace(str, "%^", " ");
  if (ob != this_object()) {
    if (strsrch(str, "%^") != -1) {
      if (!query_property(PLAYER_ALLOW_COLOURED_SOULS)) {
         str = strip_colours(str);
      }
    }

    if (sizeof(avoid) > 1 && check_earmuffs("multiple-soul", ob)) {
      return ;
    }
    event_say(ob, colour_event("soul", "") + "$C$" + str + "%^RESET%^", avoid, 0);
  } else {
    event_write( ob, str + "%^RESET%^", 0 );
  }
} /* event_soul() */

/**
 * This method is called when someone does an emote.
 * @param thing the object doing the emote
 * @param mess the emote to print
 */
void event_emote( object thing, string mess ) {
   if ( thing == this_object() )
      return;
   this_object()->add_message( colour_event("emote", "%^CYAN%^") + mess + "%^RESET%^", ({ }) );
} /* event_emote() */

/**
 * This method is called when someone says something.
 * @param ob the object doing the say
 * @param start the start message
 * @param mess the message to say
 * @param lang the language it is printed in
 * @param accent the accent object to use
 */
void event_person_say(object ob, string start, string mess, string lang,
                      string accent) {
   string *args, astr, tstr;
   int mangle_accent;

   if(ob == this_object() )
      return;

   if (accent) {
      if (query_nationality_accent_ob() != accent) {
         if (query_mangle_accent()) {
            mangle_accent = 2;
         } else {
            mangle_accent = 1;
         }
      }
   }

   if((lang != query_default_language() && lang != "general") ||
       (accent && mangle_accent == 1)) {

     tstr = start[0..<3];
     if(lang != query_default_language() && lang != "general")
       tstr += " in " + cap_words(lang);

     if(accent && mangle_accent == 1) {
       astr = accent->query_accent_name();
       if(astr[0] == 'a' || astr[0] == 'e' || astr[0] == 'i' ||
          astr[0] == 'o' || astr[0] == 'u')
         astr = "an " + astr;
       else
         astr = "a " + astr;
       
       tstr += " with " + astr + " accent";
     }
     start = tstr + ": ";
   }

   if(this_object()->query_property(SHORTHAND_OUTPUT_PROP)) {
     mess = LANGUAGES->fix_shorthand(mess);
   }

   args = LANGUAGE_HAND->garble_say(lang, start, mess, this_object(),
                                    ob, SAY_TYPE);
   start = args[ 0 ];
   mess = args[ 1 ];

   if (accent && mangle_accent == 2) {
      mess = accent->garble_say(mess, this_object(), ob, SAY_TYPE,
                                query_default_language());
   }
   
   this_object()->add_message( "$I$5=" + colour_event("say", "%^CYAN%^") +
                               "$C$"+ start +
                               mess +"%^RESET%^\n",
             ({ }) );
} /* event_person_say() */

/**
 * @ignore yes
 * Use this on pain of death :)
 */
void add_tell_history(string start, string mess, int remote) {
  if (!pointerp(tell_history)) {
    tell_history = ({ });
  }
  tell_history +=  ({ ({ start, mess, time(), remote }) });
  if (sizeof(tell_history) > MAX_TELL_HIS) {
    tell_history = tell_history[1..];
  }
} /* add_tell_history() */

/* Modification by Pinkfish 8th of August 1995
 * on Grampa's request to remove the nomask.
 */
/**
 * This method is called when a person is told something.
 * @param ob the object doing the tell
 * @param start the start bit of the message
 * @param mess the message itself
 * @param lang the language it was spoken in
 */
/* nomask */ void event_person_tell( object ob, string start, string mess) {
   int id;
   string *args;
   string tmp;
   
   if ( this_object()->query_property( "ignoring" ) ) {
      if ( member_array( (string)ob->query_name(), 
           (string *)this_object()->query_property( "ignoring" ) ) != -1 &&
           !ob->query_creator()) {
         tell_object(ob,this_object()->query_name()+" is ignoring you and "
                     "will not have heard what you said.\n");
         return;
      }
   }

   if(this_object()->query_property(SHORTHAND_OUTPUT_PROP))
     mess = LANGUAGES->fix_shorthand(mess);

   if (_event_info->busy && (!pointerp(_event_info->busy) ||
                             member_array(ob, _event_info->busy) == -1)) {
     tell_object(ob, this_object()->query_cap_name() +
                  " is currently busy with "
                  "someone else, " + this_object()->query_pronoun() + 
                  " has heard this message and will get back to "
                  "you shortly.\n");
      start = "[BUSY] " + start;
   }

   add_tell_history(start, mess, 0);
   tmp = fix_string(replace(colour_event("tell", "%^YELLOW%^"),
                                         "%^", "%%^") +
                                 "%s%s%%^RESET%%^\n",
                                 cols, 5, 0, capitalize(start), mess);
   if (has_mxp(this_object())) {
      efun::tell_object(this_object(), fix_for_mxp(tmp));
   } else {
      efun::tell_object(this_object(), tmp);
   }
   if (userp(this_object()) &&  interactive( this_object() ) &&
      (id = query_idle(this_object())) > TELL_WARN_TIME) {
     args = ({ });
     if (id/(60*60))
        args += ({ (id/(60*60))+" hours" });
     if ((id/60)%60)
        args += ({ ((id/60)%60)+" minutes" });
     if (id%60)
        args += ({ (id%60)+" seconds" });
     write(this_object()->query_name()+" has been idle for "+
           query_multiple_short(args)+".\n");
   }

   if ( this_object()->query_property( PASSED_OUT ) ) {
      write( "Something tells you that $C$"+
            (string)this_object()->query_name() +
            " will be unable to reply to you just yet.\n" );
   }
} /* event_person_tell() */

/**
 * This method is called when the whisper even is generated.
 * @param ob the object whispering
 * @param start the start of the whisper message
 * @param mess the message to print
 * @param obs the objects to tell the message to
 * @param lang the lanaguage the whisper is in
 * @param me the object doing the whispering
 */
void event_whisper(object ob, string start, string mess,
                   object *obs, string lang, object me) {
   string blue;
   string *args;

   blue = "";
   if (me == this_object())
      return;

   if (lang != query_default_language() && lang != "general")
     blue = " in " + cap_words(lang);
   
   if(this_object()->query_property(SHORTHAND_OUTPUT_PROP))
     mess = LANGUAGES->fix_shorthand(mess);

   if ( member_array( this_object(), obs ) == -1)  {
      if(ob->query_visible(this_object()))
         this_object()->add_message( "$I$5=" +
                                   colour_event("whisper", "%^CYAN%^") + "$C$"+
                                   start +
                                   query_multiple_short( obs ) +
                                   ".%^RESET%^\n", ({ }) );
   } else {
      args = LANGUAGE_HAND->garble_say(lang, start, mess, this_object(),
                                    ob, WHISPER_TYPE);
      start = args[ 0 ];
      mess = args[ 1 ];

      if ( sizeof( obs ) == 1 ) {
         this_object()->add_message( "$I$5=" +
                                     colour_event("whisper", "%^CYAN%^") +
                                     "$C$"+ start +"you"+
                                     blue +": "+ mess +"%^RESET%^\n", ({ }) );
      } else {
         this_object()->add_message( "$I$5=" +
                                     colour_event("whisper", "%^CYAN%^") +
                                     "$C$"+ start +
                                     query_multiple_short( obs ) + blue +": " +
                                     mess +"%^RESET%^\n",
                                     ({ }) );
      }
   }
} /* event_whisper() */

/**
 * This method is generated when a shout is done by a player.
 * @param thing the thing doing the shout
 * @param start the start string to print
 * @param mess the message to print
 * @param lang the language the shout is in
 * @param co_ord the co-ordinate of the shouter
 * @param range the range of the shout
 */
void event_person_shout( object thing, string start, string mess, string lang,
      int *co_ord, int range ) {
   /*string garbler; */
   mixed *args;

   if ( this_object() == this_player() ) {
      return;
   }
   if ( check_earmuffs( "shout", thing ) ) {
      return;
   }
   if ( !environment() ) {
      return;
   }
   if ( environment()->query_property( "shout zone" ) !=
        environment( this_player() )->query_property( "shout zone" ) ) {
      return;
   }

   if (lang != query_default_language() && lang != "general") {
      start += " in " + capitalize(lang);
   }

   //
   // Put in a check so you don't see shouts from people you are
   // ignoring.
   //
   if (sizeof(query_ignoring(({ this_player() }))) ) {
      return ;
   }

   args = LANGUAGE_HAND->garble_say(lang, start, mess, this_object(),
                                    thing, SHOUT_TYPE);
   start = args[ 0 ];
   mess = args[ 1 ];
   if(this_object()->query_property(SHORTHAND_OUTPUT_PROP))
     mess = LANGUAGES->fix_shorthand(mess);

   if ( environment() == environment( this_player() ) ) {
      this_object()->add_message( "$I$5=" + colour_event("shout", "") + "$C$"+
                                  start +": "+ mess +"%^RESET%^\n", ({ }) );
      return;
   }
   if ( sizeof( co_ord ) != 3 ) {
      return;
   }
   BROADCASTER->broadcast_event( ({ this_object() }), co_ord, start +
      ": "+ mess, range, 1, 0 );
} /* event_person_shout() */

/**
 * This event is generated when a newbie chat event is done.
 * @param thing the thing generateing the newbie chat
 * @param message the message the newbie said
 */
void event_newbie(object thing, string message) {
  string tmp;

  if((this_player() == this_object() ) ||
     check_earmuffs("newbie", this_player())) {
    return;
  }
  
  if (sizeof(query_ignoring(({ this_player() }))) ) {
    return ;
  }
  
  if(this_object()->query_property(SHORTHAND_OUTPUT_PROP))
    message = LANGUAGES->fix_shorthand(message);
   
  tmp = fix_string(replace(colour_event("newbie", "%^MAGENTA%^"),
                                       "%^", "%%^") +
                               "(newbie)%%^RESET%%^ %s\n",
                               cols, 5, 0, message);
  if (has_mxp(this_object())) {
     efun::tell_object(this_object(), fix_for_mxp(tmp));
  } else {
     efun::tell_object(this_object(), tmp);
  }
}

/**
 * This event is generated when a newbiehelpers chat event is done.
 * @param thing the thing generateing the newbiehelpers chat
 * @param message the message the newbiehelper said
 */
void event_newbiehelpers(object thing, string message) {
  string tmp;

  if((this_player() == this_object() ) ||
     check_earmuffs("newbiehelpers", this_player()) ||
     !NEWBIEHELPERS_HANDLER->query_helper_access(this_object())) {
    return 0;
  }
  
  if (sizeof(query_ignoring(({ this_player() }))) ) {
    return 0;
  }
  
  if(this_object()->query_property(SHORTHAND_OUTPUT_PROP))
    message = LANGUAGES->fix_shorthand(message);
  
  tmp = fix_string(replace(colour_event("newbiehelpers", 
                                  "%^YELLOW%^"), "%^", "%%^") +
                               "(newbiehelpers)%%^RESET%%^ %s\n",
                               cols, 5, 0, message);
  if (has_mxp(this_object())) {
     efun::tell_object(this_object(), fix_for_mxp(tmp));
  } else {
     efun::tell_object(this_object(), tmp);
  }
}

/**
 * This event is generated when a combat message occurs.
 * @param thing the thing generateing the event.
 * @param message the message to be displayed.
 * @param exclude objects not to show this message to.
 * @param verbose is the message considered verbose?
 */
void event_combat(object thing, string message, object *exclude, int verbose) {
  if(member_array(this_object(), exclude) != -1)
    return;

  if(verbose && !this_object()->query_verbose("combat"))
    return;
  
  this_object()->add_message(colour_event("combat", "%^WHITE%^") + "$C$" + 
                             message + "%^RESET%^", ({ }));
  this_object()->print_messages();
}

/**
 * This event is generated when a creator tell is done.
 * @param ob the object generating the event
 * @param start the start bit
 * @param mess the message bit
 * @param forced if it is forced to occur
 * @param channel the creator channel to talk on
 */
nomask void event_creator_tell(object ob, string start, string mess,
                               int forced, string channel) {
  string tmp;
  
  if (!channel)
    channel = "cre";
  
  if (ob == this_object() ||
      ((sizeof(this_object()->query_ignoring( ({ ob }) ) ) || 
        check_earmuffs( channel, ob )) && !forced) ||
      !this_object()->query_creator()) {
    return;
  }
  // Permissions checks.
  if(member_array(channel, this_object()->channel_list()) == -1)
    return;
       
  if(this_object()->query_property(SHORTHAND_OUTPUT_PROP))
    mess = LANGUAGES->fix_shorthand(mess);
 
  tmp = fix_string(replace(colour_event(channel, "%^CYAN%^"),
                                       "%^", "%%^") +
                               "(" + channel + ") %s%s%%^RESET%%^\n",
                               cols, 5, 0, start, mess);
  if (has_mxp(this_object())) {
     efun::tell_object(this_object(), fix_for_mxp(tmp));
  } else {
     efun::tell_object(this_object(), tmp);
  }
}

/**
 * This event is generated when a lord tell is done.
 * @param ob the object generating the event
 * @param start the start bit
 * @param mess the message bit
 * @param forced if it is forced to occur
 */
nomask void event_lord_tell( object ob, string start, string mess,
      int forced ) {
  string tmp;

  if ( this_player() == this_object() ) {
      HIST_HANDLER->add_lord_history(start, mess);
      return;
   }
  if (ob == this_object()
     || (check_earmuffs("lord", ob) && !forced) ||
      !master()->query_lord((string)this_object()->query_name()))
    return;

  if(this_object()->query_property(SHORTHAND_OUTPUT_PROP))
     mess = LANGUAGES->fix_shorthand(mess);

  tmp = fix_string(replace(colour_event("lord", "%^YELLOW%^"),
                                       "%^", "%%^") +
                               "(lord) %s%s%%^RESET%%^\n",
                               cols, 5, 0, start, mess);
  if (has_mxp(this_object())) {
     efun::tell_object(this_object(), fix_for_mxp(tmp));
  } else {
     efun::tell_object(this_object(), tmp);
  }
} /* event_lord_tell() */

/**
 * This event is generated when a intermud creator tell is done.
 * @param ob the object generating the event
 * @param mname the name of the mud
 * @param pname the name of the player
 * @param mess the message
 * @param ig the ignore object
 * @param emote if it is an emote
 */
void event_inter_creator_tell(object ob, string mname, string pname, 
                              string mess, object ig, int emote) {
   string tmp;

  if (!this_object()->query_creator() || check_earmuffs("inter-creator-tell", 0)
      || this_object() == ig)
    return ;
   tmp = fix_string("%s@%s%s%s\n", cols, strlen(mname) + strlen(pname) + 3,
                      0, pname, mname, (emote?" ":": "), mess);
  if (has_mxp(this_object())) {
     efun::tell_object(this_object(), fix_for_mxp(tmp));
  } else {
     efun::tell_object(this_object(), tmp);
  }
} /* event_inter_creator_tell() */

/**
 * This event is generated when a creator tell is done.
 * @param ob the object generating the event
 * @param mname the name of the mud
 * @param pname the name of the player
 * @param mess the message
 * @param ig the ignore object
 * @param emote if it is an emote
 */
void event_intermud_tell(object ob, string start, string mess,
                         string channel, object ig) {
  string tmp;

  if (!this_object()->query_creator() || check_earmuffs("intermud-all", 0)
      || check_earmuffs(channel, 0) || this_object() == ig) {
    return ;
  }
  tmp = fix_string("%%^MAGENTA%%^(%s)%%^RESET%%^ %s%s\n", cols,
                      strlen(start)+ strlen(channel)+ 3, 0, channel, start,
                      mess);
  if (has_mxp(this_object())) {
     efun::tell_object(this_object(), fix_for_mxp(tmp));
  } else {
     efun::tell_object(this_object(), tmp);
  }
} /* event_intermud_tell() */

/**
 * This method is called when an echo to is generated by the player.
 * @param ob the object doing the echo to
 * @param mess the message being printed
 * @param me the person generating the echo to
 */
void event_player_echo_to(object ob, string mess, object me) {
  if ( master()->query_lord( (string)this_object()->query_name() ) )
     this_object()->add_message( "$0$ echos to you:\n", ({ ({ "my_the_short:"+
            file_name( me ) }) }) );
   this_object()->add_message( colour_event("echo", "") + mess + "%^RESET%^",
                               ({ }) );
} /* event_player_echo_to() */

/**
 * This method is called when an emoteall is generated by a player.
 * @param ob the object doing the emoteall
 * @param mess the message being printed
 */
void event_player_emote_all(object ob, string mess) {
  if (ob == this_object()) {
    return;
  }

  if ( master()->query_lord( (string)this_object()->query_name() ) ) {
     this_object()->add_message( "$0$ emotes to all:\n", ({ ({ "my_the_short:"+
            file_name( ob ) }) }) );
  }

  mess = this_object()->colour_event( "emoteall" ) + mess + "%^RESET%^"; 
  
  this_object()->add_message( "$I$5=$C$"+ replace_string( mess, "$N", "$0$" ),
    ({ ({ "my_the_short:"+ file_name( ob ) }) }) );
} /* event_player_emote_all() */

/**
 * This method is called when an echo is generated by a player.
 * @param ob the object doing the echo
 * @param mess the message being printed
 */
void event_player_echo(object ob, string mess) {
   if (ob == this_object()) {
      return;
   }
   if ( master()->query_lord( (string)this_object()->query_name() ) ) {
      this_object()->add_message( "$0$ echos:\n", ({ ({ "my_the_short:"+
            file_name( ob ) }) }) );
   }
   this_object()->add_message( mess, ({ }) );
} /* event_player_echo() */

/**
 * This method is generated internally by the driver when this player is
 * snooping someone else.
 * @param mess the snoop message
 */
void receive_snoop(string mess) {
  tell_object(this_object(), "] "+mess);
} /* receive_snoop() */

/**
 * This is a call back generated by the driver internally to tell us about
 * terminal types.
 * @param type the terminal type returned
 */
void terminal_type(string type) {
  if (set_network_terminal_type(type)) {
    tell_object(this_object(), "Setting your network terminal type to \""+
                type+"\".\n");
  } else {
    if (term_name == "network") {
      if (!_event_info->last_term || _event_info->last_term != type) {
        _event_info->last_term = type;
/* Keep going until they repeat twice.  This is the end of the list. */
        printf("%c%c%c%c%c%c", IAC, SB, TELOPT_TTYPE, TELQUAL_SEND, IAC, SE);
      } else
        tell_object(this_object(), "Unknown terminal type \""+type+"\".\n");
    }
  }
} /* terminal_type() */

/**
 * This is a call back generated by the driver internally to tell us about
 * the window size of the remote machine.  This information in this is
 * only used if the terminal is a network type.
 * @param width the number of columns
 * @param height the number of rows
 */
void window_size(int width, int height) {
  if (term_name == "network") {
    if (width > 10 && width < 256) {
      set_cols(width);
   }
   if (height > 5 && height < 256) {
      set_rows(height);
   }
   tell_object(this_object(), "Your machine told our machine that your "
               "terminal has "+height+" rows and "+width+" columns.\n");
  }
} /* window_size() */

/**
 * This is a callback generated by the driver to deal with mxp related
 * stuff.
 */
void mxp_enable() {
   // Send down some code to turn on the suboption
   efun::tell_object(this_object(),
               sprintf("%c%c%c%c%c%c", IAC, SB, TELOPT_MXP, IAC, SE, 0) +
               sprintf("%c[1z", 27) +
               replace_string(read_file("/doc/login/mxp_setup"), "\n", "") + "\n");
}
