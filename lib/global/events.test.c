inherit "/global/play_parse_com";
#include "player.h"
#include "term.h"

int earmuffs, cols, rows;
static mapping colour_map; /* this so that changeing the definitions gets fixed
                            * when they log in again.  Also keeps the save
                            * file smaller. */
string term_name = "dumb";
static string new_line = "";

void write_prompt(string str);

string query_term_name() { return term_name; }
int query_earmuffs() { return earmuffs; }

int query_see_octarine()
{
  string guild;

  if(!(guild = (string)this_object()->query_guild_ob())) return 0;
  return (int)guild->query_see_octarine();
}

string fix_string(string ret, int window) {
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
      if(query_see_octarine()) st[i] = st[i][9..10000];
      else continue;
    if (colour_map[st[i]])
      ret += colour_map[st[i]];
    else
      ret += st[i];
  }
  if(window)
    return colour_map["WINDOW"]+ret+colour_map["STATUS"]+colour_map["RESET"];
  return ret+colour_map["RESET"];

}

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
      write_prompt(colour_map["ENDTERM"]);
      colour_map = (mapping)TERM_HANDLER->set_term_type(str);
      term_name = str;
      write_prompt(colour_map["INITTERM"]);
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
}

void event_commands(){
     add_action("earmuffs", "earmuffs");
     add_action("set_our_rows", "rows");
     add_action("set_our_cols", "cols");
     add_action("set_term_type", "term");
     add_action("do_new_line","new_line");
     if(this_object()->query_property(NO_LINE_PROP))
       new_line = "";
     else
       new_line = "\n";
      write_prompt(colour_map["INITTERM"]);
}

int earmuffs(string frog){
     if(earmuffs && !earmuffs || frog == "off"){
       write("Ok.  Earmuffs off.\n");
       earmuffs = 0;
       return 1;
     }
     if(!earmuffs && frog == "on"){
       write("Ok.  Earmuffs on.\n");
       earmuffs = 1;
       return 1;
      }
     if(earmuffs && frog == "on"){
       write("Your earmuffs are already on.\n");
       return 1;
     }
     if(!earmuffs && frog == "off"){
       write("Your earmuffs are not on.\n");
       return 1;
     }
     else {
       write("Usage: earmuffs <on/off>\n");
       return 1;
     }
}


void set_rows(int i) { rows = i; }
int query_rows() { return rows; }

int set_our_rows(string str) {
  int val;

  if(!str) {
    notify_fail("Rows currently set to " + rows + ".\nrows <number> to set.\n");
    return 0;
  }
  if(!sscanf(str, "%d", val) || val <= 0)
    val = 24;
  write("Rows set to " + val + ".\n");
  rows = val;
  return 1;
}

int query_cols() { return cols; }
void set_cols(int i) { cols = i; }

int set_our_cols(string str) {
  int val;

  if(!str) {
    notify_fail("Columns currently set to " + cols + ".\ncols <number> to set.\n");
    return 0;
  }
  if(!sscanf(str, "%d", val) || val <= 0)
    val = 79;
  write("Columns set to " + val + ".\n");
  cols = val;
  return 1;
}

int do_new_line(string str) {
  if(!str) {
    if (new_line == "")
      new_line = "\n";
    else
      new_line = "";
  } else if(str == "off")
    new_line = ""; 
  else if(str == "on")
    new_line = "\n"; 
  else {
    notify_fail("Syntax: new_line <on|off>\n");
    return 0;
  }
  if (new_line == "") {
    this_object()->add_property(NO_LINE_PROP, 1);
    write("New lines before everything switched off.\n");
  } else {
    this_object()->remove_property(NO_LINE_PROP);
    write("New lines before everything switched on.\n");
  }
  return 1;
}

void event_enter(object ob, string mess) {
  if (mess)
    efun::tell_object(this_object(), new_line + fix_string(sprintf("%-=*s",
                      cols, mess), 1));
}

void event_exit(object ob, string mess) {
  if (mess)
    efun::tell_object(this_object(), new_line + fix_string(sprintf("%-=*s",
                     cols, mess), 1));
  remove_object(ob);
}

void event_say(object caller, string str, mixed avoid) {
  if (pointerp(avoid)) {
    if (member_array(this_object(), avoid) != -1)
      return ;
  } else if (avoid == this_object())
    return ;
  efun::tell_object(this_object(), new_line + fix_string(sprintf("%-=*s",
                    cols, str), 1));
}

void write_prompt(string str) {
  efun::tell_object(this_object(), fix_string(str, 0));
}

void event_write(object caller, string str) {
  efun::tell_object(this_object(), fix_string(str, 1));
}

void do_efun_write(string str) {
  efun::tell_object(this_object(), fix_string(sprintf("%-=*s",
                    cols, str), 1));
}

void event_soul(object ob, string str, mixed avoid) {
  if (ob != this_object())
    event_say(ob, str, avoid);
  else
    do_efun_write(str);
}

void event_person_say(object ob, string start, string mess) {
  if (ob == this_object()) return;
  efun::tell_object(this_object(), new_line + fix_string(sprintf("%s%-=*s\n",
       start, cols-strlen(start), mess), 1));
}

void event_person_tell(object ob, string start, string mess) {
  efun::tell_object(this_object(), new_line + fix_string(sprintf("%s%-=*s\n",
       start, cols-strlen(start), mess), 1));
}

void event_person_shout(object ob, string start, string mess) {
  if (ob == this_object()) return ;
  if(earmuffs == 1 && (string)ob->query_verb() != "cre!" &&
       (string)ob->query_verb() != "shout!"){ return; }
  efun::tell_object(this_object(), new_line + fix_string(sprintf("%s%-=*s\n",
       start, cols-strlen(start), mess), 1));
}

void event_creator_tell(object ob, string start, string mess) {
  if (ob == this_object() || (earmuffs == 1 && query_verb() != "cre!") ||
      !this_object()->query_creator())
    return;
  efun::tell_object(this_object(), new_line + fix_string(sprintf("%s%-=*s\n",
        start, cols - strlen(start), mess), 1));
}
