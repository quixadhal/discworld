/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: player.c,v 1.184 2003/07/15 07:13:27 pinkfish Exp $
 */
/**
 * The main player object.  Does all the player related stuff.
 * @author Pinkfish
 * @started December 1991
 */

#include <config.h>
#include <board.h>
#include <bounty.h>
#ifndef __DISTRIBUTION_LIB__
#include <deity.h>
#endif
#include <drinks.h>
#include <library.h>
#include <living.h>
#include <login.h>
#include <login_handler.h>
#include <log.h>
#include <quest_handler.h>
#include <command.h>
#include <mail.h>
#include <player.h>
#include <skills.h>
#include <top_ten_tables.h>
#include <tune.h>
#include <weather.h>
#include <refresh.h>
#include <nroff.h>
#include <db.h>
#include <error_handler.h>
#include <player_handler.h>
#include <playerinfo.h>
#include <group_handler.h>

// Right now if you move this below the clubs include it will break due to
// mudos class lossage :(
// This class contains all the global, nosaving variables to reduce our
// variable usage.
class player_info {
   int hb_num;
   int level;
   int level_time;
   int save_inhibit;
   int update_tmps_call_out;
   int last_save;
   object snoopee;
   mapping titles;
}
#include <clubs.h>
inherit "/global/line_ed";
inherit "/global/events";
inherit "/global/log";
inherit "/global/spells";
inherit "/global/more_string";
inherit "/global/finger";
inherit "/global/pweath";
inherit "/std/living/living";
inherit "/global/psoul";
inherit "/global/guild-race";
inherit "/global/more_file";
inherit "/global/path";
inherit "/global/start_position";
inherit "/global/family";

#define MIN_TIME_TO_SAVE 1800
#define FORGOT_CO_ORD_TIME (840553267+(8*60*60))
#define LEVEL_CACHE 60

#define ITEM_TRACKER ("/obj/handlers/item_tracker")

/* 
 * Discworld stuff
 */
private nosave class player_info _pinfo;

private int time_on;
private int max_deaths;
private int monitor;
private int _refresh_time;
private int start_time;
private int creator;
private int deaths;
private int last_log_on;
private int no_logins;
private int activity_counter;
private int _flags;
private string cap_name;
private string last_on_from;
private nosave string my_file_name;

void start_player();
void public_commands();
int save();
void set_desc(string str);
void do_load_auto();
void set_name(string str);
int check_dark(int light);
void continue_start_player();
int restart_heart_beat();
int brief_verbose(string,
                  string);
int quit_alt(int verbose);
int review();
int toggle_wimpy(string);
int do_refresh(string);
int do_cap(string);
protected void set_invis(int level);
void set_player_killer(int);
int query_player_killer();

void finished_error_check( object player, string my_name, int status, 
    mixed data );
void check_error_count();
void check_approve_list();

void create()
{
   if (query_name()) {
      return;
   }

   living::create();
   events::create();
   psoul::create();
   line_ed::create();
   more_string::create();
   start_position::create();
   add_property("determinate", "");
   spells::create();
   time_on = time();
   start_time = time();
   seteuid("PLAYER");
   Str = 13;
   Dex = 13;
   Int = 13;
   Con = 13;
   Wis = 13;
   set_max_sp(50);
   set_sp(50);
   wimpy = 20;
   max_deaths = 7;
   set_desc(0);
   add_ac("bing", "blunt", 15);
   add_ac("bing2", "sharp", 15);
   add_ac("bing3", "pierce", 15);
   add_property("player", 1);
   set_cols(79);
   set_rows(24);
   last_log_on = time();
   race_ob = RACE_STD;

   sscanf(file_name(this_object()), "%s#", my_file_name);

   //add_language("common");

   _pinfo = new (class player_info, titles: ([ ]), save_inhibit:1);
}

/**
 * @ignore yes
 */
void dest_me()
{
   if ((query_name() != "object") && objectp(this_player()) &&
       (this_player() != this_object()) &&
       (file_name(this_player()) != "/obj/shut") &&
       (explode(file_name(this_player()), "#")[0] != "/secure/login") &&
       (explode(file_name(this_player()), "#")[0] != "/secure/nlogin")) {
      user_event(this_object(), "inform",
                 (string) this_player()->query_name() +
                 " dests " + query_name(), "dest");
      tell_object(this_object(), "You were destructed by " +
                  capitalize((string) this_player()->query_name()) + ".\n");
   }
   
   if(file_size(PLAYER_HANDLER->query_player_file_name(query_name())+
                ".o.gz") > 0 &&
      PLAYER_HANDLER->query_player_disk_file_name(query_name()) &&
      PLAYER_HANDLER->query_player_file_name(query_name()) !=
      PLAYER_HANDLER->query_player_disk_file_name(query_name())) {
     unguarded((:rename,
                PLAYER_HANDLER->query_player_file_name(query_name())+".o.gz",
                PLAYER_HANDLER->query_player_disk_file_name(query_name())+
                ".o.gz" :));
   }
   ::dest_me();
}                               /* dest_me() */

/**
 * This method stops the player from being able to save.  This is done while
 * the inventory is regenerating to stop problems with inventory loss due
 * to logging off before it has finished.
 * @see allow_save()
 * @see query_save_inhibit()
 */
void disallow_save()
{
   _pinfo->save_inhibit = 1;
}

/**
 * This method allows the player to save again after they have been
 * disallowed.
 * @see disallow_save()
 * @see query_save_inhibit()
 */
void allow_save()
{
   _pinfo->save_inhibit = 0;
}

/**
 * This method returns the value of the disallowing save variable.  If this
 * is a non-zero value then the player must not be saved.
 * @see allow_save()
 * @see disallow_save()
 */
int query_save_inhibit()
{
   return _pinfo->save_inhibit;
}

/**
 * This is the date on which the character was started.  The very first
 * time the player ever logged on.
 * @return the time at which the player first logged on
 */
int query_start_time()
{
   return start_time;
}

/**
 * This method returns the current capitalised name of the player.
 * @return the current capitalised name
 * @see set_cap_name()
 */
nomask string query_cap_name()
{
   return cap_name;
}

/**
 * This method is called from within the login code to start up the
 * new player, set their name and move them into the correct
 * location on the mud.
 * @param bong the name of the player
 * @param new_flag are the a new player?
 * @param c_name their capital name
 * @param ident the ident from the authorisation code
 * @param go_invis go invsible on login
 */
void move_player_to_start(string bong,
                          int new_flag,
                          string c_name,
                          string ident,
                          int go_invis) {
  string lang;
  
   /* Only /secure/login can do this. */
   if (file_name(previous_object())[0..12] != "/secure/login" &&
       file_name(previous_object())[0..13] != "/secure/nlogin") {
      write("You don't have clearance to do that.\n");
      return;
   }
   seteuid("Root");
   set_name(bong);

   if (!new_flag) {
      unguarded((: restore_object,
                   PLAYER_HANDLER->query_player_file_name(query_name()), 1 :));
      if (go_invis) {
        switch(go_invis) {
        case -1:
          set_invis(0);
          break;
        case 2:
          if (this_object()->query_lord()) {
            set_invis(2);
            break;
          }
        case 1:
          set_invis(1);
        }
      }
   }
   disallow_save();             /* don't allow saves until the autoload has started */
   add_property("player", 1);
   cap_name = c_name;
   set_short(cap_name);
   add_property("determinate", "");
   no_logins++;
   if (!query_cols()) {
      set_cols(79);
   }
   remove_property("new player");

   if (this_player()->query_creator()) {
      seteuid(query_name());
      if (file_size("/w/" + query_name() + "/cmds") == -2)
         AddSearchPath(({ "/w/" + query_name() + "/cmds" }));
      AddSearchPath(({ DIR_PLAYER_CMDS, DIR_SECURE_PLAYER_CMDS,
                       DIR_CREATOR_CMDS, DIR_SECURE_CREATOR_CMDS,
                       DIR_PLAYTESTER_CMDS }));
   } else {
      seteuid("PLAYER");
      AddSearchPath(({ DIR_PLAYER_CMDS, DIR_SECURE_PLAYER_CMDS }));
   }
   if (!last_on_from) {
      printf("You have never logged in before.\n");
   } else {
      printf("You last logged in from %s and are currently "
             "logged in from %s (%s).\n", last_on_from,
             query_ip_name(this_object()), query_ip_number(this_object()));
   }
   last_on_from = query_ip_name(this_object()) + " (" +
      query_ip_number(this_object()) + ")";

   //
   // What the heck does this do?
   //
/*
   if ( time_on < -500 * 24 * 60 * 60 ) {
      time_on += time();
   }
 */
   if (time_on > 0) {
      time_on = 0;
   }
   time_on += time();
   if (new_flag) {
      add_property("new player!", 1);
   }
   LOGIN_HANDLER->player_logon(bong, query_property("guest"),
                               (query_ip_name()? query_ip_name() :
                                query_ip_number()));

   if(query_property("guest")) {
     this_player()->set_nationality(DEFAULT_NATIONALITY);
     this_player()->
       set_nationality_region(this_player()->find_region(DEFAULT_NATIONALITY,
                                                         DEFAULT_REGION));
     lang = DEFAULT_NATIONALITY->query_language();
     
     // Setup the default language correctly.
     this_player()->add_language(lang);
     this_player()->set_default_language(lang);
     this_player()->set_language(lang);
   }
   write(LOGIN_HANDLER->get_message("/doc/NEWS"));

   call_out((: continue_start_player :), 0);
   // remove the previously cached entry.
   PLAYER_HANDLER->remove_cache_entry(query_name());
}                               /*  move_player_to_start() */

/** @ignore yes */
protected void move_to_start_pos()
{
   ::move_to_start_pos();
   if (query_verbose("look"))
      command("look");
   else
      command("glance");

   if (!finger_set()) {
      write
         ("%^BOLD%^Please set your finger information with 'chfn'.%^RESET%^\n");
      //command("chfn");
   }

   if(no_logins == 1 && !query_property("guest")) {
     write("\n%^BOLD%^" +
           LOGIN_HANDLER->get_message("/doc/login/NEW_USER_START") +
           "%^RESET%^\n");
   }
   
   no_time_left();
   START_PLAYER->logged_on_times(no_logins);

   // Try resolving hostname one last time.
   if(query_ip_number(this_object()) == query_ip_name(this_object()))
     resolve(query_ip_number(this_object()), "");

}                               /* move_to_start_pos() */

/**
 * @ignore yes
 *
 * This function tracks a players activity to determine whether they're still
 * active or not. Ceres
 */
protected void update_activity(int logon)
{
   int time_on;

   // Lower their activity score if they haven't been on for more than a week.
   if (last_log_on > 0) {
      time_on = last_command - last_log_on;
      if (time_on > 0)
         activity_counter -= (time_on / (3600 * 24 * 7)) * 10;
   }

   if (activity_counter <= -55) {
      activity_counter = -55;
   }
   // when they logon give them a couple more activity points.
   if (logon) {
      activity_counter += 3;
   } else {                     // when they leave give them some more for each hour they were on.
      activity_counter += 2 * ((time() - last_log_on) / 3600);
   }

   // Set minima and maxima to the counter.
   if (activity_counter >= 0)
      activity_counter = 0;
}

/**
 * Return the current value of this players activity_counter
 * (used for debugging)
 */
int query_activity_counter()
{
   return activity_counter;
}

/**
 * THis method prints something if there are bug replies to see.
 */
private void display_bug_replies(int type, class error_replies* replies) {
   if (type == DB_SUCCESS) {
      if (sizeof(replies) > 0) {
         tell_object(this_object(), "\n\nYou have %^YELLOW%^" +
            sizeof(replies) + " NEW%^RESET%^ bug repl" +
            (sizeof(replies) > 1?"ies ":"y ") +
            "(read with 'bug replies').\n\n");
      }
   }
}

private void check_mccp() {
   if(!compressedp(this_object())) {
     write("\n%^BOLD%^%^RED%^You are logged in uncompressed!\n");
     write("Use of an MCCP capable mud client will reduce your load on "
           "the " + mud_name() + " system.%^RESET%^\n\n");
   }
}

/**
 * @ignore yes
 */
void continue_start_player()
{
   string title;
   mapping news_rc;
   mixed *stuff;
   object book;

   // Set the cre flag for setting up commands etc.
   if (this_object()->query_lord()) {
      creator = 1;
   }

   start_player();
   no_time_left();
   /* Remove passed out property, unless the player is
    * in a trance, e.g. when contemplating. */
   if (query_property(PASSED_OUT) && !query_property(TRANCE)) {
      call_out((: remove_property :), 10 + random(30), PASSED_OUT);
   }
   remove_property(UNKNOWN_MOVE);
   remove_property(RUNNING_MOVE);
   init_after_save();           /* for effects */
   printf("%s", fix_string(MAILER->new_mail(query_name())));
   news_rc = BOARD_HAND->query_newsrc(query_name());
   if (!news_rc) {
      news_rc = ([ ]);
   }
   stuff = BOARD_HAND->get_subjects("announcements");
   if (sizeof(stuff)) {
      if (stuff[<1][B_TIME] > news_rc["announcements"]) {
         write("\nThere are %^YELLOW%^NEW%^RESET%^ announcements.  ");
         write("You can read them with \"news\".\n\n");
         //"/cmds/player/news"->do_list();
         //printf("You can read then with \"news\".\n\n");
      }
   }

   call_out((: check_mccp :), 4);
       
   if (query_property("dead")) {
      (clone_object(DEATH_SHADOW))->setup_shadow(this_object());
      // Give them a dead book too.
      book = clone_object(DEAD_BOOK);
      book->move(this_object());
   }
   if (query_property("noregen")) {
      DEATH->person_died(query_name());
   }
   exec_alias("login", "");
   update_activity(1);
   last_log_on = time();
   title = LIBRARY->query_title(query_name());
   if (stringp(title)) {
      _pinfo->titles += ([ "quest" : title ]);
   }
   if (my_file_name != "/global/player") {
      check_error_count();
      if(my_file_name != "/global/playtester") {
        check_approve_list();
      }
   }
   if (query_property("new player!")) {
     // Clear the alt info.
     PLAYERINFO_HANDLER->player_created(query_name());
      user_event(this_object(), "inform",
                 capitalize(query_name()) +
                 " arrives on " + mud_name() + " for the first time!", "new-player");
      remove_property("new player!");
      set_last_pos(CONFIG_NEWBIE_START_LOCATION);
      START_PLAYER->start_player(this_object());
   }
   //add_language("common");
   check_family_name();
   call_out((: move_to_start_pos :), 0);
   call_out((: set_heart_beat(1) :), 1);
   // Tell them about new error replies.
   ERROR_HANDLER->do_error_replies(query_name(), 1, (: display_bug_replies :));

}                               /* continue_start_player() */

/**
 * @ignore yes
 */
void check_error_count() {
   string my_name; 
    
   my_name = query_name(); 

   DB_HANDLER->make_sql_request( "errors", "atuin", "", 
      "SELECT COUNT(Id) AS idc FROM errors WHERE Directory = '/w/" + 
      my_name + "' AND Status = 'OPEN'", 
       (: finished_error_check, this_object(), my_name :) ); 
} /* check_error_count() */

/**
 * @ignore yes
 */
void finished_error_check( object player, string my_name, int status, 
    mixed data ) {

   int num;
   int ret;

   if ( file_size("/w/" + my_name + "/" + PLAYER_ERROR_LOG) > 0 ) {
       num = file_length("/w/" + my_name + "/" + PLAYER_ERROR_LOG);
       tell_object( player, "%^YELLOW%^You have %^RED%^" + num + 
           "%^YELLOW%^ director" + (num == 1 ? "y" : "ies") + 
           " with %^RED%^ERRORS%^YELLOW%^ in /w/" + my_name + "/" + 
           PLAYER_ERROR_LOG + ".%^RESET%^\n");
   }

   if ( status == DB_SUCCESS && sizeof( data ) > 0 ) { 
      if ( ( ret = data[0]["idc"] ) > 0 ) {
         tell_object( player, "%^YELLOW%^You have %^RED%^" + ret + 
            " ERROR" +  (ret == 1 ? "" : "S") + "%^YELLOW%^ in /w/" + 
            my_name +  ".%^RESET%^\n");
      }
   }
} /* finished_error_check() */ 

/** @ignore yes */
void check_approve_list() {
  string str;
  mapping assignments;
  int count;
  
  assignments = master()->query_directory_assignments();
  foreach(str in keys(assignments)) {
    if(member_array(this_object()->query_name(), assignments[str]) != -1 &&
       file_size(str + "/to_approve")) {
      count = sizeof(get_dir(str + "/to_approve/*")) - 2;
      if(count > 0)
        tell_object(this_object(), "%^YELLOW%^" + str + " has %^RED%^" +
                    count + "%^YELLOW%^ item" + (count > 1 ? "s" : "") +
                    " awaiting approval.%^RESET%^\n");
    }
  }
}

/**
 * @ignore yes
 */
void start_player()
{
   if (this_object()->query_creator() && my_file_name != "/global/player") {
      this_player()->all_commands();
      this_player()->app_commands();
      this_player()->wiz_commands();
   }
   call_out((: do_load_auto :), 1);
   reset_get();
   enable_commands();
   public_commands();
   command_commands();
   parser_commands();
   force_commands();
   race_guild_commands();
   soul_commands();
   event_commands();
   finger_commands();
   communicate_commands();
   living_commands();
   logging_commands();
   editor_commands();
   if (this_player()->query_creator()) {
      set_living_name(query_name());
   }
   set_no_check(1);
   set_con(Con);
   set_dex(Dex);
   set_int(Int);
   set_str(Str);
   set_wis(Wis);
   reset_all();
   set_current_path(query_home_dir());
   if (wimpy > 100) {
      wimpy = 25;
   }
   if (contmp || dextmp || inttmp || strtmp || wistmp) {
      _pinfo->update_tmps_call_out = call_out((: update_tmps :), 900);
   }
}

/* added chrisy 6 june 93 to stop net-dead players getting killed */
/**
 * @ignore yes
 * This is an over ride of something in /std/living
 */
int adjust_hp(int number,
              object attacker,
              object weapon,
              string attack)
{
   if (!interactive(this_object()))
      return query_hp();
   return::adjust_hp(number, attacker, weapon, attack);
}                               /* adjust_hp() */

/**
 * Starts the autoloading process.  This is called just after a player
 * logs in to start up the auto loading process.
 */
void do_load_auto()
{
   load_auto_load_alt(query_auto_load_string(), this_object(), this_object(),
                      (: tell_object(this_object(),
                                     "\n%^CYAN%^Inventory regeneration complete.%^RESET%^\n") :));
   set_auto_load_string(0);
   allow_save();
}                               /* do_load_auto() */

/**
 * @ignore yes 
 */
void public_commands()
{
   add_command("restart", this_object(), "", (: restart_heart_beat() :));
   add_command("save", this_object(), "", (: save() :));
   add_command("quit", this_object(), "", (: quit_alt(-1) :));
   add_command("quit", this_object(), "brief", (: quit_alt(0) :));
   add_command("quit", this_object(), "verbose", (: quit_alt(1) :));
   add_command("review", this_object(), "", (: review() :));
   add_command("wimpy", this_object(), "", (: toggle_wimpy(0) :));
   add_command("wimpy", this_object(), "<word'number'>",
               (: toggle_wimpy($4[0]) :));
   add_command("refresh", this_object(), "<word'totally'>",
               (: do_refresh($4[0]) :));
   add_command("cap", this_object(), "<word'name'>", (: do_cap($4[0]) :));
}                               /* public_commands() */

/**
 * Force the player to glance.  Forces the player to glance in a
 * certain direction.
 * @param str the direction to glance in
 * @return 0 ifg the command failed, non zer if it succeeded
 */
int glance(string str)
{
   return command("glance" + (str ? " " + str : ""));
}                               /* glance() */

/**
 * Force the player to look.  Forces the player to look at something
 * or just around.  If the input is set to "" then they look around.
 * @param str what to look at
 * @return 0 if it failed, non zero on success
 * @see efun::command()
 */
int look_me(string str)
{
   return command(str ? "look" + str : "look");
}                               /* look_me() */

/**
 * @ignore yes
 */
int review()
{
   write("Entry  : " + query_msgin() + "\n");
   write("Exit   : " + query_msgout() + "\n");
   write("MEntry : " + query_mmsgin() + "\n");
   write("MExit  : " + query_mmsgout() + "\n");
   write("Editor : " + query_editor() + "\n");
   return 1;
}                               /* review() */


/**
 * Forces the player to examine something.  This does the same as the look_me
 * function.
 * @see look_me()
 * @return 0 on a failure, non zero on success.
 * @param arg the thing to look at.
 */
int examine(string arg)
{
   return look_me(arg ? " at " + arg : "");
}                               /* examine() */

/**
 * @ignore yes
 */
varargs string short(int dark,
                     int verbose)
{
   string str;
   string family;
   string title;

   if (!interactive(this_object())) {
      str = "the net dead statue of ";
   } else {
      str = "";
   }

   family = query_family_name();
   if (!family) {
      family = "";
   } else {
      family = " " + family;
   }

   if (verbose)
      title = query_player_title();
   if (!title) {
      title = "";
   } else {
      title = title + " ";
   }

   return str + title + living::short(dark) + family;
}                               /* short() */

/**
 * This method changes the current value of the wimpy variables.
 * @param str the new value of the wimpy variable
 * @return 0 on failure and 1 on success
 */
int toggle_wimpy(string str)
{
   int number;
   if (!str) {
      if (this_object()->query_wimpy())
         write("You are in wimpy mode, you will run away at " +
               this_object()->query_wimpy() +
               "% of your max hps.\n");
      else
         write("You are in brave mode.\n");
      write("Usage: wimpy <num> (caution read help wimpy)\n");
      return 1;
   }
   if (sscanf(str, "%d", number) != 1)
      return notify_fail("You must set your wimpy to a number.\n");
   if ((number < 0) || (number > 30))
      return notify_fail("Your wimpy must be between 0 and 30%.\n");
   this_object()->set_wimpy(number);
   printf("Wimpy set to %d%%\n", wimpy);
   return 1;
}                               /* toggle_wimpy() */


/**
 * This method returns any extra information associated with the score.  This
 * method will mostly be overridden by shadows which wish to place more
 * information into the score command.
 * @return the extra information to place in the score
 */
string extra_score()
{
   return "";
}

/**
 * This method is deprecated.
 *
 * @return always returns 1
 * @see save_me()
 */
nomask int save() {
#ifdef UNUSED
  if(query_verb() == "save") {
    if(_pinfo->last_save > time() - 900) {
      tell_object(this_object(), "You saved only a few minutes ago.\n");
      return 1;
    }
    _pinfo->last_save = time();
  } 
#endif

  /*
   * easier on me - Pinkfish.
   * ..
   */
  if (((time() - time_on) < MIN_TIME_TO_SAVE) &&
      !LIBRARY->query_quest_points(query_name()) &&
      !query_property("authorised player")) {
    if (query_verb() == "save") {
      tell_object(this_object(), "You're too young to save... sorry.\n");
    }
    return 1;
  }
  
  tell_object(this_object(), "Saving...\n");
  this_object()->save_me();
  return 1;
}

/**
 * Saves the player.  Saves the player to disc, doing all the autoload
 * stuff and such like.
 * @see save()
 */
void save_me() {
   mixed old;

#ifdef NOT_SAVEING_PLAYERS
   if (!this_object()->query_creator()) {
      write("Not saving for players currently.\n");
      return;
   }
#endif
   if (query_auto_loading())
      return;

   if (query_property("guest")) {
      if (query_verb() == "save")
         tell_object(this_object(), "But not saving for guests... Sorry.\n");
      return;
   }

   /* If this is called before auto_loading has even started, log and abort
    * Olorin 31-Mar-97
    */
   if (query_save_inhibit()) {
      return;
   }

   if (guild_ob) {
      guild_ob->player_save(this_object());
   }
   if (race_ob) {
      race_ob->player_save(this_object());
   }
   old = geteuid();

   check_last_pos();

   /* for effects */
   effect_freeze();
   effects_saving();
   /* Recreating the autoload stuff. */
   create_auto_load(all_inventory(), 1);
   if (interactive(this_object())) {
      last_on_from = query_ip_name(this_object()) + " (" +
         query_ip_number(this_object()) + ")";
   }
   //
   // Keep the time_on calculation as close to the saving as possible.
   //
   time_on -= time();
   //if (query_creator()) {
      catch(unguarded((: save_object,
                      PLAYER_HANDLER->query_player_file_name(query_name()), 3 :)));
/*
   } else {
      catch(unguarded((: save_object,
                      PLAYER_HANDLER->query_player_file_name(query_name()), 1 :)));
   }
 */
   time_on += time();
   ITEM_TRACKER->save_all_item_states(this_object()->query_name());
   effect_unfreeze();
//   if (this_object()->query_creator()) {
   /* Clear the autoload string after saving to save memroy. */
   set_auto_load_string(0);
//   }
}                               /* save_me() */

void save_with_auto_load(mixed al)
{
   mixed old;

#ifdef NOT_SAVEING_PLAYERS
   if (!this_object()->query_creator()) {
      write("Not saving for players currently.\n");
      return;
   }
#endif
   if (query_property("guest")) {
      write("But not saving for guests... sorry.\n");
      return;
   }
   if (((time() - time_on) < MIN_TIME_TO_SAVE) && query_verb() == "save") {
     tell_object(this_object(), "You're too young to save... sorry.\n");
     return;
   }
   
   /* If this is called before auto_loading has even started, log and abort
    * Olorin 31-Mar-97
    */
   if (query_save_inhibit()) {
      return;
   }
   if (guild_ob) {
      guild_ob->player_save(this_object());
   }
   if (race_ob) {
      race_ob->player_save(this_object());
   }
   old = geteuid();

   /* for effects */
   effect_freeze();
   effects_saving();
   set_auto_load_string(al);
   if (((time() - time_on) >= MIN_TIME_TO_SAVE) ||
       LIBRARY->query_quest_points(query_name())) {
     //
      // Put the time_on calculation as close to the save as possible.
      //
      time_on -= time();
      //if (query_creator()) {
         catch(unguarded((: save_object,
                         PLAYER_HANDLER->query_player_file_name(query_name()),
                         3 :)));
/*
      } else {
         catch(unguarded((: save_object,
                         PLAYER_HANDLER->query_player_file_name(query_name()),
                         1 :)));
      }
 */
      time_on += time();
   }
   effect_unfreeze();
}                               /* save_with_auto_load() */

/**
 * This is the quit code called by the command 'quit'
 * @return 1 if successful, 0 if failed
 * @see quit()
 * @see do_quit()
 */
int quit_alt(int verbose)
{
   object frog;

   if (verbose == -1) {
      verbose = query_verbose("quit");
   }

   if (query_auto_loading()) {
      if (!interactive(this_object())) {
         dest_me();
      } else {
         write
            ("You cannot quit yet: your inventory is still being generated.\n");
      }
      return 1;
   }

   if (sizeof(filter_array((object *) this_object()->query_attacker_list(),
                           (: living($1) :))) &&
       this_player() == this_object() && interactive(this_object())) {
      write
         ("You cannot quit while in combat.  Use 'stop' to stop fighting.\n");
      return 1;
   }

   check_last_pos();

   update_activity(0);
   last_log_on = time();

   // Added to help people tell when the player quits.
   catch(this_object()->event_quit(this_object()));
   catch(environment(this_object())->event_quit(this_object()));

   catch(editor_check_do_quit());
   write("A small gecko takes your hand and rushes you to the departure "
         "lounge.\n");
   say(query_cap_name() + " leaves the game.\n");
   catch(move("/room/departures"));

   user_event(this_object(), "inform",
              query_cap_name() + " leaves " + mud_name() + "", "logon", this_object());

   catch(LOGIN_HANDLER->player_logout(query_name()));
   if (race_ob) {
      catch(race_ob->player_quit(this_object()));
   }
   if (guild_ob) {
      catch(guild_ob->player_quit(this_object()));
   }

   frog = clone_object("/obj/monster/greco");
   frog->move("/room/departures");
   frog->get_rid_of(this_object(), verbose);
   return 1;
}                               /* quit_alt() */

/**
 * quits the player.
 * @return zero if it failed, non zero on success
 * @see quit()
 * @see quit_alt()
 */
int do_quit()
{
   return quit_alt(-1);
}                               /* do_quit() */

/**
 * A forced quit.
 * This is the function called by external objects to quit
 * a player. It avoids checks for things like being in combat.
 *
 * @return 0 if it failed, non zero if it succeeded.
 * @see do_quit()
 * @see quit_alt()
 * @see efun::command()
 */
int quit()
{
   if (query_auto_loading() || query_save_inhibit()) {
      tell_object(this_object(),
                  "Forced to quit whilst auto loading.  Not saving.\n");
      ::dest_me();
      return 1;
   }

   check_last_pos();

   update_activity(0);
   last_log_on = time();

   catch(editor_check_do_quit());

   // Added to help people tell when the player quits.
   catch(this_object()->event_quit(this_object()));
   catch(environment(this_object())->event_quit(this_object()));

   tell_object(this_object(), "Thanks for playing.  See you next time.\n");
   say(query_cap_name() + " leaves the game.\n");

   user_event(this_object(), "inform",
              query_cap_name() + " leaves " + mud_name() + "", "logon", this_object());

   catch(LOGIN_HANDLER->player_logout(query_name()));
   if (race_ob) {
      catch(race_ob->player_quit(this_object()));
   }
   if (guild_ob) {
      catch(guild_ob->player_quit(this_object()));
   }
    
   catch(save_me());

   foreach( object inv in all_inventory( this_object() ) ) {
      reset_eval_cost(); 
      catch( inv->set_tracked_item_status_reason("QUIT") );
      catch( inv->dest_me() );
   }

   catch(ITEM_TRACKER->
         save_all_item_states_delete_cache(this_object()->query_name()));
   effects_quiting();
   
   // One more for good luck. 
   reset_eval_cost(); 

   if (catch(dest_me())) {
      destruct(this_object());
   }
   return 1;
}                               /* quit() */

/**
 * This method returns the mapping of all the title associated with this
 * player.
 * @return the current mapping of titles
 * @see query_title()
 * @see set_title()
 * @see remove_title()
 */
mapping query_titles()
{
   return copy(_pinfo->titles);
}

/**
 * This method returns the list of titles associated with this player.
 * @return the list of titles
 * @see query_titles()
 * @see set_title()
 * @see remove_title()
 */
string query_title()
{
   return implode(m_values(_pinfo->titles), ", ");
}

/**
 * This method sets the current title type to be of a certain name.
 * @param type the type of title to set
 * @param title the string to set it to
 * @see query_title()
 * @see set_title()
 * @see remove_title()
 */
void set_title(string type,
               string title)
{
   _pinfo->titles[type] = title;
}

/**
 * This method removes the title of the given type from the title mapping.
 * @param type the type of title to remove
 * @see query_title()
 * @see set_title()
 * @see remove_title()
 */
void remove_title(string type)
{
   map_delete(_pinfo->titles, type);
}

/**
 * This method returns the number of times the player has died.
 * @return the number of times the player has died
 * @see adjust_deaths()
 * @see adjust_max_deaths()
 */
int query_deaths()
{
   return deaths;
}

/**
 * This method changes the number of times the player has died.  This
 * can only be positive.
 * @param i the amount of deaths to adjust by, must be +ve
 * @see query_deaths()
 * @see adjust_max_deaths()
 */
void adjust_deaths(int i)
{
   if (i > 0) {
      deaths += i;
   }
}

/* nomask protected void set_name(string str) { */
/** @ignore yes */
nomask void set_name(string str) {
  if (query_name() && query_name() != "object") {
    return;
  }
  if (file_name(previous_object())[0..12] != "/secure/login" &&
      file_name(previous_object())[0..13] != "/secure/nlogin") {
    return;
  }
  ::set_name(str);
  set_living_name(query_name());
  set_main_plural(query_name());
}                               /* set_name() */

/**
 * @ignore yes
 * No ide what this is doing here?
 */
void init_static_arg(mapping map)
{
   return;
}

/**
 * @ignore yes
 */
void init_dynamic_arg(mapping, object) {
  return;
}

/** @ignore yes */
string long(string str,
            int dark)
{
   string s;
   string tmp;
   string title;
   string nationality;

   title = query_player_title();
   if (!title) {
      title = "";
   } else {
      title = title + " ";
   }

   s = "You see " + title + query_short();
   tmp = query_family_name();
   if (tmp) {
      s += " " + query_family_name();
   }
   if (guild_ob) {
      s += " " + (string) guild_ob->query_title(this_object()) + ".\n";
   } else {
      s += ".\n";
   }
   if (race_ob) {
      s += (string) race_ob->query_desc(this_object());
   }

   nationality = query_nationality_description(this_player());
   if (nationality  &&  nationality != "") {
      s += nationality + "\n";
   }

   if (dark == 2 || dark == -2) {
      s += "It is too ";
      if (dark == -2) {
         s += "dark";
      } else {
         s += "bright";
      }
      s += " to see their description or what " +
         query_pronoun() + " are wearing or carrying.\n";
      s += calc_extra_look();
   } else {
      if (query_desc() && query_desc() != "") {
         s += capitalize(query_pronoun()) + " " + query_desc() +
              query_main_zone_desc(query_wearing()) + "\n";
      }
      s += capitalize(query_pronoun()) + " " +
         this_object()->health_string() + ".\n";
      s +=
         capitalize(query_pronoun()) + " is " + query_position_short() +
         ".\n";
      s += calc_extra_look();
      s += weather_extra_look();
      s += query_living_contents(0);
   }
   return s;
}                               /* long() */

/**
 * This method is called when the player dies.  It creates the corpse
 * and handles all the bits and pieces that should be dealth with
 * when a player dies.
 * <p>
 * If this method returns 0 then the automatic death handling code
 * in the living object is used instead of this.
 * @return always returns 1
 */
mixed second_life()
{
   object corpse;
   string death_mess;

   add_property("dead", time());

   if (!DEATH->someone_died(this_object())) {
      deaths++;
   }

   effects_thru_death();
   corpse = make_corpse();
   reset_protectors();

   if (deaths > max_deaths) {
      death_mess = query_property ("death mess");
      tell_object(this_object(), "You have died your final death.  " +
                "Your name will be inscribed in the Hall of Legends.\n");
      
      if (death_mess && sizeof (death_mess)) {
        death_mess = strip_colours (death_mess);
        death_mess = replace (death_mess, ({"\n", "", "\a", ""}));
        
        shout(capitalize(query_name()) + " has died for the last time.  " +
          death_mess + "\n");
      }
      else {                
        shout(capitalize(query_name()) + " has died for the last time.  " +
             capitalize(query_pronoun()) + " is mourned.\n");
      }
      
      LIBRARY->complete_death(query_name());
      corpse->move(PLAYER_MORGUE);
   } else
      corpse->move(environment());

   call_out((: save_me :), 0);
   if (find_object(OFFLER) && (deaths <= max_deaths)) {
      OFFLER->person_died(query_name());
   } else {
      DEATH->person_died(query_name());
   }
   clone_object(DEATH_SHADOW)->setup_shadow(this_object());
   if (guild_ob) {
      guild_ob->member_died(this_object());
   }
   gp = 0;
   hp = 0;
   set_sp(0);
   set_max_sp(50);
   xp = 0;
   //spells = ([ ]);
   reset_spells();
   reset_processes();
   contmp = dextmp = inttmp = strtmp = wistmp = 0;
   remove_call_out(_pinfo->update_tmps_call_out);
   adjust_tmp_con(-2);
   adjust_tmp_dex(-2);
   adjust_tmp_int(-2);
   adjust_tmp_str(-2);
   adjust_tmp_wis(-2);
   return corpse;
}                               /* second_life() */

/**
 * This method removes the ghost from the player.  This stops them
 * being a ghost and turns them back into a real person.
 * @see second_life()
 */
void remove_ghost()
{
   if (deaths > max_deaths) {
      if (this_player() != this_object()) {
         tell_object(this_object(), this_player()->query_cap_name() +
                     " tried to raise you, but you are completely dead.\n");
         tell_object(this_player(), query_cap_name() +
                     " is completely dead, you cannot raise " +
                     query_objective() + ".\n");
      } else {
         tell_object(this_object(), "You are completely dead.  You cannot " +
                     "be raised.\n");
         say(query_cap_name() +
             " struggles to appear in a solid form, but fails.\n");
         return;
      }
   }
   remove_property("dead");
   tell_object(this_object(), "You reappear in a more solid form.\n");
   if (guild_ob) {
      guild_ob->member_resurrected(this_object());
   }
   say(query_cap_name() + " appears in more solid form.\n");
   if (query_hp() < 0) {
      set_hp(1);
   }
   set_personal_temp(0);

   this_object()->dest_death_shadow();
   save_me();
   all_inventory()->recovery_from_death();
}                               /* remove_ghost() */

/**
 * Called when the player goes net dead.  This is called
 * by the driver when a player goes net dead.  Turns them into
 * a statue and stuff.
 */
void net_dead()
{
   if (!environment() || file_name(environment()) == "/room/departures") {
      // Don't do anything here...
      return;
   }

   if ((query_name() == "guest") || (query_name() == "root")) {
      say(query_name() + " vanishes in a puff of logic.\n");
      quit();
   } else {
      say(query_name() + " goes white, looks very chalky and turns into a " +
          "statue.\n");
      user_event(this_object(), "inform",
                 capitalize(query_name()) + " has lost " +
                 query_possessive() + " link", "link-death", this_object());
      check_last_pos();
      LOGIN_HANDLER->player_net_dead(query_name());
      /* Called to save the player when they go net dead. */
      save_me();
   }
   event(environment(), "net_dead", this_object());
}                               /* net_dead() */

/**
 * Checks to see if they have idled out.  This is called from within
 * the heart beat code.
 * @see heart_beat()
 */
void idle_out()
{
   say(cap_name + " has been idle for too long; " + query_pronoun() +
       " vanishes in a puff of boredom.\n");
   write("You idled out, sorry.\n");
   quit();
}                               /* idle_out() */

/**
 * The main heart beat function.  This is called by the driver
 * every 2 seconds on the player.  Does all the maintence
 *  stuff like fixing up hps and stuff like that.
 */
void heart_beat()
{
   catch {
      int idle_time;

      events::print_messages();
      flush_queue();

      if (!interactive(this_object())) {
         idle_time = time() - last_command;
      } else {
         idle_time = query_idle(this_object());
      }

      if (query_creator()) {
         idle_time = idle_time / 3;
         /* Time out for creators is 3 times that of players... */
      }

      if (idle_time > FULL_IDLE) {
         if ((LOGIN_HANDLER->is_discworld_full() || idle_time > MAX_IDLE) &&
             (!interactive(this_object())
              || my_file_name != "/global/lord")) {
            call_out((: idle_out :), 2);
         }
      }

      if (interactive(this_object())) {

         living::heart_beat();
         adjust_sp(1);
         adjust_xp(3);
         living::update_volumes();

         this_object()->do_spell_effects(0);

         // reset it every hour to stop it rolling over.
         if (_pinfo->hb_num++ > 1800)
            _pinfo->hb_num = 0;

         // Do these checks less frequently.
         if (!(_pinfo->hb_num % 120)) {

            calc_personal_temp();
            adjust_max_sp(1);
         }
      }
   };
}                               /* heart_beat() */

/**
 * Is rthe monitor turned on?  The hit point monitor which is
 * displyed during combat...
 *
 * @return 1 if it is on, 0 if not
 */
int query_monitor()
{
   return monitor;
}

/**
 * Set the hit point minitor.  This sets the value of the hit point
 * monitor flag.  If it is set to 0, the monitor is turned off.  If
 * it is set to 1 the monitor is turned on, higher values cause the monitor
 * to be displayed every N heartbeats.
 *
 * @param i the new value for the hit point monitor flag
 */
void set_monitor(int i) {
  monitor = i;
}

protected void write_prompt()
{
   efun::tell_object(this_object(), "> ");
   return;
}

#ifdef UNUSED
protected void write_prompt()
{
   string prompt;
   int tmp,
     i;
   string stuff;

   if (!(monitor & 2) || !this_object()->query_creator())
      efun::tell_object(this_object(), "> ");
   else {
      prompt = "";
      stuff = this_player()->query_property("prompt");
      for (i = 0; i < strlen(stuff); i++) {
         switch (stuff[i]) {
         case 'h':             // Hp %
            tmp = ((query_hp() * 100) / query_max_hp());
            prompt += "Hp:";
            switch (tmp) {
            case 50..100:
               prompt += "%^GREEN%^";
               break;
            case 20..49:
               prompt += "%^YELLOW%^";
               break;
            default:
               prompt += "%^RED%^";
            }
            prompt += tmp + "%^RESET%^";
            break;

         case 'H':             // Hp %
            tmp = ((query_hp() * 100) / query_max_hp());
            prompt += "Hp:";
            switch (tmp) {
            case 50..100:
               prompt += "%^GREEN%^";
               break;
            case 20..49:
               prompt += "%^YELLOW%^";
               break;
            default:
               prompt += "%^RED%^";
            }
            prompt += query_hp() + "%^RESET%^";
            break;

         case 'g':             // Gp %
            tmp = (query_gp() * 100) / query_max_gp();
            prompt += "Gp:";
            switch (tmp) {
            case 50..100:
               prompt += "%^GREEN%^";
               break;
            case 20..49:
               prompt += "%^YELLOW%^";
               break;
            default:
               prompt += "%^RED%^";
            }
            prompt += tmp + "%^RESET%^";
            break;

         case 'G':             // Gp
            tmp = (query_gp() * 100) / query_max_gp();
            prompt += "Gp:";
            switch (tmp) {
            case 50..100:
               prompt += "%^GREEN%^";
               break;
            case 20..49:
               prompt += "%^YELLOW%^";
               break;
            default:
               prompt += "%^RED%^";
            }
            prompt += query_gp() + "%^RESET%^";
            break;

         case 's':             // Sp %
            tmp = (query_sp() * 100) / query_max_sp();
            prompt += "Sp:";
            switch (tmp) {
            case 50..100:
               prompt += "%^GREEN%^";
               break;
            case 20..49:
               prompt += "%^YELLOW%^";
               break;
            default:
               prompt += "%^RED%^";
            }
            prompt += tmp + "%^RESET%^";
            break;

         case 'S':             // Sp
            tmp = (query_sp() * 100) / query_max_sp();
            prompt += "Sp:";
            switch (tmp) {
            case 50..100:
               prompt += "%^GREEN%^";
               break;
            case 20..49:
               prompt += "%^YELLOW%^";
               break;
            default:
               prompt += "%^RED%^";
            }
            prompt += query_sp() + "%^RESET%^";
            break;

         case 'X':
         case 'x':
            prompt += "Xp:" + query_xp();
            break;

         default:
            if (sizeof(prompt) > 0 && prompt[sizeof(prompt) - 1] != stuff[i])
               prompt += sprintf("%c", stuff[i]);
         }
      }
      prompt += "> ";
      efun::tell_object(this_object(), fix_string(prompt));
   }
}
#endif

/**
 * Called when the player wimpys out of a place.  This does all the run
 * away stuff anfd things like that.
 * @return 1 if succeeded in running away, 0 if not
 */
int run_away()
{
   int number;
   become_flummoxed();
   number =::run_away();
   if (number)
      tell_object(this_object(), "Your feet run away with you!\n");
   else
      tell_object(this_object(), "You try to run away, but no matter how "
                  "you scrabble, you can't find any way out.\n");
   return number;
}                               /* run_away() */

/**
 * The amount of time on line.  This is the total amount of time online
 * in seconds from when they first started playing.  The return from
 * this method is *negative*, you will need to make it positive to use it
 * most likely.
 * @return the total log on time in seconds (the return value is *negative*)
 */
nomask int query_time_on()
{
   return time_on - time();
}

/**
 * This method determins if the player is mature or not yet.  It is based
 *  on the amount of time on.
 * @return 1 if the player is mature, 0 if they are not
 */
int query_mature()
{
   return (MATURITY * 60 * 60 + time_on < time());
}

/**
 * Can the player see?  Checks to see if the player can see at the passed
 * in light level.
 * @return 1 if they can see, 0 if they cannot
 */
int check_dark(int light)
{
   int i;

   if (race_ob)
      if (catch(i = (int) race_ob->query_dark(light)))
         race_ob = RACE_STD;
      else
         return i;
   return (int) RACE_STD->query_dark(light);
}                               /* check_dark() */

/**
 * Returns the players level.  Uses their current guild to determine their
 * guild level.
 * @return their guild level
 */
int query_level()
{
   string tmp;

   if (_pinfo->level_time < (time() - LEVEL_CACHE)) {
      if (guild_ob) {
         _pinfo->level = (int) guild_ob->query_level(this_object());
      } else {
         /*
          * If they do not have a guild object, see if they have a backup
          * set.  Otherwise use the warriors guild as the level setter.
          */
         tmp = query_property("backup guild");
         if (tmp) {
            _pinfo->level = tmp->query_level(this_object());
         } else
            _pinfo->level = CONFIG_DEFAULT_GUILD->query_level(this_object());
      }
      _pinfo->level_time = time();
   }

   return _pinfo->level;
}                               /* query_level() */

/**
 * Restarts their heartbeat if it has got turned off for some reason.
 * @return always returns 1
 */
int restart_heart_beat()
{
   set_heart_beat(1);
   write("Ok, heart_beat restarted.\n");
   return 1;
}

/**
 * @ignore yes
 */
nomask void set_snoopee(object ob)
{
   _pinfo->snoopee = ob;
}

/**
 * Returns the object snooping us.  If someone is snooping the player it
 * returns the object doing the snooping.  If no one is snooping the player
 * it returns 0.
 * @return the object snooping the player
 * @see efun::snoop()
 */
nomask object query_snoopee()
{
   return _pinfo->snoopee;
}

/**
 * @ignore yes
 */
void set_creator(int i)
{
   if (previous_object() != master()) {
      write("Illegal attempt to set creator!\n");
      log_file("ILLEGAL", this_player(1)->query_name() + " (" +
               file_name(this_player(1)) +
               ") Illegal attempt to set_creator " + "at " + ctime(time()) +
               " from " + file_name(previous_object()) + "\n");
      return;                   /* THIS WAS MISSING !!!  Turrican 4-11-95 */
   }
   creator = i;
   set_home_dir("/w/" + query_name());
   save_me();
}                               /* set_creator() */

/**
 * Prevent the object from shadowing us?  This checks to see if we should
 * prevent this object from shadowing the player.
 * @return 1 if we are prventing the shadow
 */
int query_prevent_shadow(object ob)
{
   if (function_exists("query_prevent_shadow", ob) ||
       function_exists("query_name", ob) ||
       function_exists("query_creator", ob) ||
       function_exists("query_lord", ob) ||
       function_exists("query_hidden", ob) ||
       function_exists("dest_me", ob) ||
       function_exists("query_object_type", ob) ||
       function_exists("query_lord", ob) || function_exists("save_me", ob))
/* added by sojan to fix security holes */
      return 1;
   return 0;
}

/**
 * Returns the maxium number of deaths.  This is the maxium number of times they
 * can die before they are totaly dead.
 * @return the maxiumm number fo deaths
 */
int query_max_deaths()
{
   return max_deaths;
}

/**
 * Sets the maximum number of deaths.  This sets the maximum number of times a player can
 * die.
 * @param i the nukber of deaths to set it to
 * @see adjust_max_deaths()
 */
protected void set_max_deaths(int i)
{
   max_deaths = i;
}

/**
 * Changes the current number of maxium deaths.  This adjust
 * the number of times a player can die before they are totaly dead.
 * @return the current total maximum deaths
 * @param i the amount of deaths to adjust by, must be +ve
 */
int adjust_max_deaths(int i)
{
   if (i > 0) {
      return (max_deaths += i);
   }
}

/**
 * @ignore yes
 */
varargs int move(mixed dest,
                 string msgin,
                 string msgout)
{
   int i;
   object env = environment();

   i = living::move(dest, msgin, msgout);
   if (!i) {
      me_moveing(env);
   }
   return i;
}                               /* move() */

/**
 * @ignore yes
 */
nomask protected int do_refresh(string str) {
   if (query_property("guest")) {
      return notify_fail("Guests cannot refresh, sorry.\n");
   }
   if (!str || str != "totally") {
      return notify_fail("Please read the docs before using this command.\n");
   }
   write("%^BOLD%^WARNING!  This will restart your character.%^RESET%^\n\n" +
         "Are you sure you wish to refresh yourself totally? ");
   input_to("refresh_docs");
   return 1;
}

/**
 * @ignore yes
 */
protected int refresh_docs(string str) {
   string helpfile = "";
   str = lower_case(str);
   if (str[0] != 'y') {
      write("Ok, not refreshing.\n");
      return 1;
   }
   NROFF_HAND->create_nroff( "/doc/helpdir/refresh", helpfile );
   helpfile = NROFF_HAND->cat_file( helpfile );
   write ( helpfile[0..670] );
   write("\n\n%^BOLD%^Having read the documentation, are you still sure you "
         "wish to refresh totally?%^RESET%^\n");
   input_to("refresh3");
   return 1;
}

/**
 * @ignore yes
 */
protected int refresh2(string str, int noevent) {
   object ob;
   string group;

   str = lower_case(str);
   if (str[0] != 'n' && str[0] != 'y') {
      write
         ("Pardon?  I do not understand.  Do you want to refresh yourself? ");
      input_to("refresh2");
      return 1;
   }
   if (str[0] == 'n') {
      write("Ok, not refreshing.\n");
      return 1;
   }
   write("Doing refresh.\n");
   Str = 13;
   Con = 13;
   Int = 13;
   Wis = 13;
   Dex = 13;
   inttmp = dextmp = wistmp = strtmp = contmp = 0;
   race_ob = CONFIG_DEFAULT_RACE;
   if (guild_ob)
      catch(guild_ob->delete_member(query_name()));

   if (!noevent) {
      REFRESH_HANDLER->player_refreshed(this_object(), PARTIAL_REFRESH);
   }
   guild_ob = 0;
   set_al(0);
   if (query_deity()) {
#ifndef __DISTRIBUTION_LIB__
      catch(DEITY->delete_worshipper(query_deity(), query_name()));
      catch(DEITY->delete_priest(query_deity(), query_name()));
#endif
   }
   set_deity(0);
   set_skills(0);

   /* Remove them as a playtester. */
   PLAYTESTER_HAND->remove_playtester(query_name());

   //
   // If they have a family, remove them from that too.
   //
   if (query_family_name()) {
      CLUB_HANDLER->remove_member(query_family_name(), query_name());
      set_family_name(0);
   }

   reset_effects();
   reset_spells();
   reset_processes();
   setup_default_titles();
   guild_ob = 0;
   set_guild_data(0);
   xp = 0;
   deaths = 0;
   max_deaths = 7;
   _refresh_time = time();
   totaly_zap_bonus_cache();
   known_commands = ({ "skills", "rearrange", "gp", "newbie" });
   reset_starts();
   race_guild_commands();
   totaly_zap_stat_cache();
   drink_info = allocate(D_SIZEOF);
   map_prop = ([ "determinate" : "",
                 "player" : 1,
                 "skills version" :
                 (string) "/obj/handlers/change_skills"->query_version() ]);
   this_object()->remove_ghost();
   set_nationality(0);
   set_nationality_region(0);
   set_language("general");
   set_default_language("general");
   reset_all();

   group = this_player()->query_group();
   if (group) {
      GROUP->remove_member(group, this_player(), " refreshed themselves.");
   }

   set_last_pos(CONFIG_NEWBIE_START_LOCATION);
   START_PLAYER->give_junk(this_object());

   // Remove people from the hunting list.
   foreach (ob in query_attacker_list()) {
      catch(ob->stop_fight(this_object()));
      catch(stop_fight(ob));
   }
   
   call_out((: move_to_start_pos :), 0);

   if (!noevent) {
      write("Done refreshing.\n");
      say(cap_name + " refreshes " + query_objective() + "self.\n");
      save_me();
   }
   return 1;
}                               /* refresh2() */

/**
 * @ignore yes
 */
protected int refresh3(string str)
{
   object *items,
    *ritems,
     item;

   str = lower_case(str);
   if (str[0] != 'n' && str[0] != 'y') {
      write("Pardon?  I do not understand.  Do you want to refresh " +
            "yourself totally? ");
      input_to("refresh3");
      return 1;
   }
   if (str[0] == 'n') {
      write("Ok, not refreshing.\n");
      return 1;
   }
   write("Ok, refreshing totally.\n");
   ritems = filter(all_inventory(environment()),
                   (: $1->query_property("dropped") &&
                      $1->query_property("dropped")[0] == query_name() :));
   move(CONFIG_NEWBIE_START_LOCATION);
   refresh2("y", 1);
   set_player_killer(0);
   _pinfo->titles = ([ ]);
   REFRESH_HANDLER->player_refreshed(this_object(), TOTAL_REFRESH);
   "/secure/related_files"->delete_related_files(query_name(),
                                                 0, TOTAL_REFRESH);

   items = ritems + all_inventory();
   foreach(item in items) {
      item->dest_me();
   }

   foreach(item in children("/obj/corpse")) {
      if (item->query_owner() == this_object()->query_name()) {
        item->move("/room/rubbish");
      }
   }

   // Remove from clubs and families.
   foreach (string club in query_player_clubs()) {
      catch(CLUB_HANDLER->remove_member(club, this_player()->query_name()));
   }
   if (query_family_name()) {
      catch(CLUB_HANDLER->remove_member(query_family_name(),
                                  this_player()->query_name()));
      call_out("check_family_name", 2);
   }

   write("Done refreshing.\n");
   say(cap_name + " refreshes " + query_objective() + "self.\n");
   save_me();
   return 1;
}

/**
 * This method returns the one letter object type which is used in the
 * finger command to display the type of object.
 * @return the one letter object type
 */
string query_object_type(object player)
{
   if ("/obj/handlers/playtesters"->query_playtester(query_name())) {
      return "p";
   }
   return " ";
}                               /* query_object_type() */

/**
 * @ignore yes
 * Sets the cap name, part of a command..
 */
int do_cap(string str)
{
   if (!str)
      return notify_fail("Syntax: " + query_verb() + " <cap_name>\n");
   if (lower_case(str) != query_name())
      return
         notify_fail
         ("You must have the same letters in your capitalized name.\n");
   cap_name = str;
   write("Capitalized name set to " + cap_name + ".\n");
   return 1;
}                               /* do_cap() */

/** @ignore yes */
void set_my_ident(string str)
{
   return;
}

/** @ignore yes */
string query_my_ident()
{
   return 0;
}

/**
 * @ignore yes
 */
void event_enter(object thing,
                 string mess,
                 object from)
{
   events::event_enter(thing, mess, from);
   living::event_enter(thing, mess, from);
}                               /* event_enter() */

/**
 * @ignore yes
 */
void event_exit(object thing,
                string mess,
                object to)
{
   events::event_exit(thing, mess, to);
   living::event_exit(thing, mess, to);
}                               /* event_exit() */

/**
 * @ignore yes
 */
string *parse_command_id_list()
{
   if (query_player_killer()) {
      return ({ "killer" }) +
             living::parse_command_id_list() + family::parse_command_id_list();
   } else {
      return living::parse_command_id_list() + family::parse_command_id_list();
   }
}                               /* parse_command_id_list() */

/**
 * @ignore yes
 */
string *parse_command_plural_id_list()
{
   if (query_player_killer()) {
      return ({ "killers" }) + living::parse_command_plural_id_list() +
         family::parse_command_plural_id_list();
   } else {
      return living::parse_command_plural_id_list() +
         family::parse_command_plural_id_list();
   }
}                               /* parse_command_plural_id_list() */

/**
 * @ignore yes
 */
string *parse_command_adjectiv_id_list()
{
   return living::parse_command_adjectiv_id_list() +
         family::parse_command_adjectiv_id_list();
}                               /* parse_command_adjectiv_id_list() */

/**
 * This method returns the time at which the player last logged on.
 * @return the last logged on time
 */
int query_last_log_on()
{
   return last_log_on;
}

/* This is the time since the last refresh... */
/**
 * This method returns the time at which the player last refreshed.
 * @return the last refresh time
 */
int query_refresh_time()
{
   return _refresh_time;
}

/**
 * This method returns the number of times the player has logged onto
 * Discworld.
 * @return the number of logins
 */
int query_no_logins()
{
   return no_logins;
}                               /* query_no_logins() */

/**
 * This method returns if the object is a creator or not.
 * @return 1 if the object is a creator, 0 if not
 */
int query_creator()
{
   return 0;
}

/**
 * @ignore yes
 */
int _living(){return 1;}

/**
 * Used to set the invisibility level upon login.
 * @param level the invisiblity level
 */
protected void set_invis(int level) {
} /* set_invis() */

/**
 * This method checks to see if the person is a player killer or not.
 * @return 1 if they are a player killer, 0 if not
 */
int query_player_killer() {
   return _flags & PLAYER_KILLER_FLAG;
} /* query_player_killer() */

/**
 * This method sets the player killer flag on the player.
 * @param flag 1 for pker, 0 for not
 */
void set_player_killer(int flag) {
   if (flag) {
      _flags |= PLAYER_KILLER_FLAG;
   } else {
      _flags &= ~PLAYER_KILLER_FLAG;
   }
}

/**
 * This method puts the person into or out of role playing mode.
 * @param i 1 sets role playing mode, 0 unsets it.
 */
void set_role_playing(int i) {
  if(i) 
    add_property(RPMODE, 1);
  else
    remove_property(RPMODE);
}

/**
 * This method checks to see if the person is in role playing mode or not.
 * @return 1 if they are in role playing mode, 0 if not.
 */
int query_role_playing() {
  return query_property(RPMODE);
}
