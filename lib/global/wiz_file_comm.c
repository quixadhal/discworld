/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: wiz_file_comm.c,v 1.34 2002/06/27 15:55:01 drakkos Exp $
 */
/**
 * This file contains all the file related commands and information
 * for the creators.
 * @author Pinkfish
 */
#include <creator.h>
#include <login_handler.h>
#include <ls.h>
#include <playtesters.h>
#include <player_handler.h>
#include <cmds/friends.h>

#ifdef USE_LE
inherit "/global/le";
#endif

inherit "/global/wiz_object_comm";
inherit "/global/playtester";
inherit "/global/wiz_info_comm";
inherit "/global/wiz_channels";
inherit "/global/wiz_inter_comm";

private int invis;
private string *allowed;
private nosave mixed _in_wiz_editor;
private nosave string *dir_list;
private nosave mixed last_location;

private int change_dir(string str);
private int pushd(string str);
private int popd();
protected int set_home_dir(string str);
private int visible();
private int invisible(string level);
private int allow(string word);
private int disallow(string word);

/* Stuck in to help with the auto load stuff change obvject */
void create() {
   playtester::create();
   wiz_object_comm::create();
   allowed = ({ });
} /* create() */

/**
 * This method adds in all the creator commands to the player.  This
 * will be called when the play initialy logs onto the game.
 */ 
protected void wiz_commands() {
   /* Get the commands from the inherited objects. */
   wiz_object_comm::wiz_commands();
   wiz_info_comm::wiz_commands();
   wiz_inter_comm::wiz_commands();
   wiz_channels::wiz_commands();
   
#ifdef USE_LE
   le::le_commands();
#endif
   
   /* Setup our local commands. */
   add_command("cd", this_object(), "<string'directory'>", (: change_dir($4[0]) :));
   add_command("cd", this_object(), "", (: change_dir(0) :));
   add_command("pushd", this_object(), "<string'directory'>", (: pushd($4[0]) :));
   add_command("pushd", this_object(), "", (: pushd(0) :));
   add_command("popd", this_object(), "", (: popd() :));
   add_command("homedir", this_object(), "<string'directory'>", (: set_home_dir($4[0]) :));
   add_command("visible", this_object(), "", (: visible() :));
   add_command("vis", this_object(), "", (: visible() :));
   add_command("invisible", this_object(), "{1|2|3}", (: invisible($4[0]) :));
   add_command("invis", this_object(), "", (: invisible("1") :));
   if (this_player()->query_director()) {
      add_command("invis", this_object(), "{1|2|3}", (: invisible($4[0]) :));
   }
   add_command("allow", this_object(), "<word'name'>", (: allow($4[0]) :));
   add_command("allow", this_object(), "", (: allow(0) :));
   add_command("disallow", this_object(), "<word'name'>", (: disallow($4[0]) :));
   add_command("disallow", this_object(), "all", (: disallow("all") :));
} /* wiz_commands() */

/**
 * This method returns the current invisibility level of the object.
 * This will return 0 if the object is not invisible, 1 for creator
 * invisible, 2 for director invisible and 3 for trustee invisible.
 * @return the current invisibility level
 */
nomask int query_invis() {
  return invis;
} /* query_invis() */

/** @ignore yes */
protected void set_invis(int level) {
   invis = level;
} /* set_invis() */

/**
 * This method returns the current list of people in the allowed array for
 * the creator.
 * @return the current list of allowed people
 */
nomask string *query_allowed() {
  return copy(allowed);
} /* query_allowed() */

/**
 * A bunch of stuff for allows to make it work with friends...
 * @ignore yes
 */
int is_friend(string str) {
   return ::is_friend(str) ||
          member_array(str, allowed) != -1;
} /* is_freiend() */

/** @ignore yes */
string query_friend_tag(string str) {
   string womble;

   if(!userp(previous_object())) {
     womble = ::query_friend_tag(str);
     if (!womble) {
       if (member_array(str, allowed) != -1) {
         return "allowed to reference";
       }
     }
     return womble;
   }
   return "";
} /* query_friend_tag() */

/** @ignore yes */
string* query_friends() {
   if (file_name(previous_object()) == FRIENDS_CMD) {
      return ::query_friends() | allowed;
   }

   return ({ });
} /* query_friends() */

/**
 * This method is called by the visible command to make the creator
 * become visible.
 * @return 1 on success
 */
private int visible() {
   if (GetForced()) {
      return 0;
   }
   if (!query_invis()) {
      return notify_fail("You are already visible.\n");
   }
   if (invis >= 2) {
      invis = 0;
      LOGIN_HANDLER->player_logon(this_player()->query_name());
   } else {
      invis = 0;
   }
   write("You appear.\n");
   return 1;
} /* visible() */

/**
 * This method is called by the invisible command to make the creator
 * turn invisible.
 * @return 1 on success, 0 on failure
 */
private int invisible( string word ) {
   int max, type;
   
   if (GetForced()) {
      return 0;
   }
   if (!word) {
      word = sprintf("%d", query_invis());
      if (word == "0") {
         word = "1";
      }
   }
   max = 1 + (int)master()->query_director(query_name()) +
      (int)master()->query_trustee(query_name());
   if (!sscanf(word, "%d", type) || (type > max) || (type < 1)) {
      return notify_fail("Syntax: invisible [1"+ ( max > 1 ? "|2" : "" ) +
                         ( max > 2 ? "|3" : "" ) + "]\n");
   }
   word = ({ "frog", "creator", "director", "trustee" })[type];
   if (type == query_invis()) {
      return notify_fail("You are already "+ word +" invisible.\n");
   }
   if (type < invis && type < 2 && invis >= 2) {
      invis = type;
      LOGIN_HANDLER->player_login(this_player()->query_name());
   } else {
      invis = type;
   }
   write("You become "+ word +" invisible.\n");
   return 1;
} /* invisible() */

/**
 * This method adds someone to the current allow list of the creator.
 * People on the allow list can see the creator while they are
 * invisible.
 * @param word the person to add to the allow string
 * @return 1 on success, 0 on failure
 */
private int allow(string word) {
    string player, mud; 

   if (!word) {
      if (!sizeof(allowed)) {
         write("You are not allowing anyone to refer to you.\n");
      } else {
         write("You are currently allowing "+
               query_multiple_short( map_array( sort_array(allowed, 0), 
                                                (: capitalize( $1 ) :))) +
               " to refer to you.\n" );
      }
      return 1;
   }
   word = lower_case(word);

   if ( word != "playtesters" && !PLAYER_HANDLER->test_user(word) && 
       sscanf( word, "%s@%s", player, mud ) != 2 ) {
      return notify_fail("There is no user called $C$"+ word +".\n");
   }

   if (member_array(word, allowed) != -1) {
      return notify_fail("You have already allowed $C$"+ word +
                         " to refer to you.\n");
   }
   allowed += ({ word });
   write(word +" is now allowed to refer to you.\n");
   return 1;
} /* allow() */

/**
 * This method removes someone from the current allow list of the
 * creator.  eople on the allow list can see the creator while they are
 * invisible.
 * @param word the person to remove to the allow string
 * @return 1 on success, 0 on failure
 */
private int disallow(string word) {
   if (!word) {
      return notify_fail("Disallow who?\n");
   }
   if (word == "all") {
     allowed = ({ });
     write("Cleared your allow list.\n");
   } else {
     word = lower_case(word);
     allowed -= ({ word });
     write(word +" is no longer allowed to refer to you.\n");
   }
   return 1;
} /* disallow() */

/**
 * This method returns the current visibility status of this creator
 * in respect to the other object.  This does the allow level checking
 * and all sorts of exciting things.
 * <p>
 * It will return 1 for creator invisible, 2 for director invisible, 3 for
 * trustee invisible.
 * @param thing the object to test the visibility against
 * @return 0 if not invisible, non-zero if invisible
 */
int query_visible(object thing) {
   string word;

   if (thing == this_object()) {
      return 1;
   }
   word = (string)thing->query_name();
   if (member_array(word, allowed) != -1) {
      return ::query_visible(thing);
   } else if((member_array("playtesters", allowed) != -1) &&
             PLAYTESTER_HAND->query_playtester(word)) {
     return ::query_visible(thing);
   }
   
   switch (query_invis()) {
   case 3 :
      return (int)master()->query_trustee(word);
   case 2 :
      return (int)master()->query_director(word);
   case 1 :
      return (int)thing->query_creator();
   default :
      return ::query_visible(thing);
   }
} /* query_visible() */

/**
 * This method returns the value of the in editor flag.
 * It will return non-zero if the player is in an editor.
 * @return non-zero in editor, 0 not in an editor
 * @see set_in_editor()
 */
mixed query_in_editor() {
   return _in_wiz_editor || ::query_in_editor();
} /* query_in_editor() */

/**
 * This method sets the current in editor flag.
 * @param what the new value of the in editor flag
 * @see query_in_editor()
 */
void set_in_editor(mixed what) {
   _in_wiz_editor = what;
} /* set_in_editor() */

/**
 * This method is called by the cd command and causes the
 * creator to change their current working directory.
 * @param str the new working directory
 * @return 1 on success, 0 on failure
 */
private int change_dir(string str) {
   string *filenames;
   object *obs;
   
   if (GetForced()) {
      return 0;
   }
   if (!str) {
      if (!query_home_dir()) {
         add_failed_mess(this_object(), 
                         "No homedir.  Use homedir to set it.\n", ({ }));
         return 0;
      }
      str = query_home_dir();
   }
   else { 
      if ( this_object()->query_property( LS_COMMAND_NICKNAME_PROPERTY ) ) {
         str = this_object()->expand_nickname( str );
      }
   } 

   filenames = get_files(str);
   if (sizeof(filenames) > 1) {
      add_failed_mess(this_object(),
                      "Ambiguous directory.\n", ({ }));
      return 0;
   }
   if (!sizeof(filenames)) {
      if (!sizeof(obs = WIZ_PRESENT->wiz_present(str, this_object()))) {
         add_failed_mess(this_object(),
                         "No such dir.\n", ({ }));
         return 0;
      }
      if (sizeof(obs) > 1) {
         add_failed_mess(this_object(),
                         "Ambiguous directory.\n", ({ }));
         return 0;
      }
      filenames =
         map(obs,
             (: sprintf("/%s",implode(explode(file_name($1), "/")[0..<2],
                                      "/")) :));
   }
   str = filenames[0];
   if (file_size(str) != -2) {
      printf("cd: %s: Not a directory.\n", str);
   } else {
      set_current_path(str);
   }
   printf("%s\n", query_current_path());
   return 1;
} /* change_dir() */

/**
 * This method is called by the pushd command.
 * @return 1 on success, 0 on failure
 * @param str the new working directory
 */
private int pushd(string str) {
   if (!dir_list) {
      dir_list = ({ });
   }
   dir_list += ({ query_current_path() });
   return change_dir(str);
} /* pushd() */

/**
 * This method is called by the popd command.
 * @return 1 on success, 0 on failure
 */
private int popd() {
   string dest;
  
   if (!sizeof(dir_list)) {
      return 0;
   }
   dest = dir_list[sizeof(dir_list)-1];
   dir_list = delete(dir_list, sizeof(dir_list)-1, 1);
   return change_dir(dest);
} /* popd() */

/**
 * This method returns the creators current working directory.
 * @return the current working directory
 */
string query_path() {
   return query_current_path();
} /* query_path() */

/**
 * This method sets the home directory of the player.  It is called
 * by the homedir command.
 * @param str the new home directory
 * @return 0 on failure and 1 on success
 */
private int set_home_dir(string str) {
   if (GetForced()) {
      return 0;
   }
   if (str) {
      ::set_home_dir(get_path(str));
   }
   printf("Home directory set to %s.\n", query_home_dir());
   return 1;
} /* set_home_dir() */

/**
 * This method returns the saved setup for the inbuild ed command.  THis
 * allows certain flag settings to be saved between sessions.
 * @return the current ed setup flags
 * @see set_ed_setup()
 */ 
int query_ed_setup() {
   return query_property("ed_setup");
} /* query_ed_setup() */

/**
 * This method sets the current flags for the inbuild ed command.  THis
 * allows certain flag settings to be saved between sessions.
 * @param i the new flags for the ed command
 * @see query_ed_setup()
 */
void set_ed_setup(int i) {
   add_property("ed_setup", i);
} /* set_ed_setup() */

/**
 * This method prints out any interesting bits of reviewable information
 * available on the creator.  This is used by the review command.
 * @return always returns 1
 */
int review() {
   playtester::review();
   wiz_info_comm::review();
   return 1;
} /* review() */

/**
 * This method stores the last location of the creator for use by goback.
 * @param string location
 */
void set_last_location(mixed loc) { last_location = loc; }

/**
 * This method returns the last location of the creator for use by goback.
 * @return string last location.
 */
mixed query_last_location() { return last_location; }

protected string process_input(string inp){
  int start;

  start = strsrch(inp, '@');
  if(start > 0){
    int space = strsrch(inp, ' ');
    if(!(space > 0 && ((space == start+1) || space < start))){
      inp = inp[0..start] + " " + inp[start+1..];
    }
  }
  start = strsrch(inp, '`');
  if(start > 0){
    int end;
    end = strsrch(inp, '`', -1);
    if(end != start){
      string lpc = inp[start+1..end-1];
      string err;
      mixed ret;
      ret = "/secure/cmds/creator/exe_c"->do_exec("return "+lpc, ref err);
      if(!err){
        if(intp(ret))
          ret = ""+ret;

        if(objectp(ret))
           ret = file_name(ret);

        if(arrayp(ret)){
          ret = filter(ret, (:stringp($1) || intp($1) || objectp($1):));
          ret = map(ret, (:intp($1)?""+$1:(objectp($1)?file_name($1):$1):));
          if(sizeof(ret))
            ret = implode(ret, ",");
        }

        if(stringp(ret)){
          inp = inp[0..start-1] + ret + inp[end+1..];
        }
      }
    }
  }
  return ::process_input(inp);
}

protected mixed command( string txt ) {
    return ::command( txt );
}
