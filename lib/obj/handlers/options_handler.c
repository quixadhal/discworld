/**
 * This handler deals will the options a player has.  It will return the
 * list of options, set options and control how the options can be
 * set.
 * @author Pinkfish
 * @started Thu Jun  8 17:13:47 PDT 2000
 */
#include <cmds/options.h>
#include <cmds/teach.h>
#include <colour.h>
#include <clubs.h>
#include <ls.h>
#include <obj_parser.h>
#include <player.h>
#include <creator.h>
#include <terrain_map.h>
#include <error_handler.h>

class option {
   mixed type;
   int restriction;
   function set;
   function query;
   mapping suboptions;
   string help;
}

private mapping _options;
private string* _colours;
private mapping _cache_input;

protected int add_option(string path, mixed type, int cre_only,
                         function set_function, function query_function,
                         string help);
private int set_player_brief(object player, string variable, int value);
private int query_player_brief(object player, string variable);
private mapping get_inform_colours(object player);
private mapping get_club_colours(object player);
private int change_bool_property(string prop, object player, int new_value);
private int change_earmuffs(object player, string ear, int new_value);
private int setup_earmuffs(object player, string value);
private int valid_birthday(string str);
private string convert_birthday(string birthday);
private int change_error_reports(object player, string ear, int new_value);

void create() {
   string womble;

   _options = ([ ]);
   _cache_input = ([ ]);
   _colours = ({
    "BOLD",
    "FLASH",
    "BLACK",
    "RED",
    "BLUE",
    "CYAN",
    "MAGENTA",
    "ORANGE",
    "YELLOW",
    "GREEN",
    "WHITE",
    "B_RED",
    "B_ORANGE",
    "B_YELLOW",
    "B_BLACK",
    "B_CYAN",
    "B_WHITE",
    "B_GREEN",
    "B_MAGENTA" });

   //
   // Output brief/verbose.
   //
   add_option("output look", OPTIONS_TYPE_BRIEF, OPTIONS_TYPE_ALL,
              (: $1->set_verbose($2[<1], $3), 1 :),
              (: $1->query_verbose($2[<1]) :),
              "Display room descriptions briefly or in full");
   add_option("output combat", OPTIONS_TYPE_BRIEF, OPTIONS_TYPE_ALL,
              (: $1->set_verbose($2[<1], $3), 1 :),
              (: $1->query_verbose($2[<1]) :),
              "Display all combat messages or only those involving damage");
   add_option("output errors", OPTIONS_TYPE_BRIEF, OPTIONS_TYPE_CRE_ONLY,
              (: $1->set_verbose($2[<1], $3), 1 :),
              (: $1->query_verbose($2[<1]) :),
              "Display errors in the error handler briefly");
   add_option("output score", OPTIONS_TYPE_BRIEF, OPTIONS_TYPE_ALL,
              (: $1->set_verbose($2[<1], $3), 1 :),
              (: $1->query_verbose($2[<1]) :),
              "Amount of detail to be displayed by the 'score' command");
   add_option("output accent", ({ "mangle", "unadulterated" }),
              OPTIONS_TYPE_ALL,
              (: $1->set_mangle_accent($3 == "mangle"), 1 :),
              (: $1->query_mangle_accent() ? "mangle" : "unadulterated" :),
              "Show others speech with or without regional accents");
   add_option("output names", OPTIONS_TYPE_BRIEF, OPTIONS_TYPE_ALL,
              (: $1->set_verbose($2[<1], $3), 1 :),
              (: $1->query_verbose($2[<1]) :),
              "Display player names with or without title and surname");
   add_option("output htell", OPTIONS_TYPE_BRIEF, OPTIONS_TYPE_ALL,
              (: $1->set_verbose($2[<1], $3), 1 :),
              (: $1->query_verbose($2[<1]) :),
              "Cause the 'htell' command to display times for tells or not");
   add_option("output msgout", OPTIONS_TYPE_STRING, OPTIONS_TYPE_CRE_ONLY,
              (: $1->set_msgout($3) :),
              (: $1->query_msgout() :),
              "The message that is displayed when you walk out of a room");
   add_option("output msgin", OPTIONS_TYPE_STRING, OPTIONS_TYPE_CRE_ONLY,
              (: $1->set_msgin($3) :),
              (: $1->query_msgin() :),
              "The message that is displayed when you walk into a room");
   add_option("output mmsgout", OPTIONS_TYPE_STRING, OPTIONS_TYPE_CRE_ONLY,
              (: $1->set_mmsgout($3) :),
              (: $1->query_mmsgout() :),
              "The message that is displayed when you trans out of a room");
   add_option("output mmsgin", OPTIONS_TYPE_STRING, OPTIONS_TYPE_CRE_ONLY,
              (: $1->set_mmsgin($3) :),
              (: $1->query_mmsgin() :),
              "The message that is displayed when you trans into of a room");
   add_option("output usercolour",
              OPTIONS_TYPE_BOOLEAN, 0,
              (: $3?$1->add_property(PLAYER_ALLOW_COLOURED_SOULS, 1):
                    $1->remove_property(PLAYER_ALLOW_COLOURED_SOULS), 1 :),
              (: $1->query_property(PLAYER_ALLOW_COLOURED_SOULS) :),
              "Display user chosen colours in souls");
   add_option("output plainmaps",
              OPTIONS_TYPE_BOOLEAN, 0,
              (: $3?$1->add_property(PLAYER_PLAIN_MAPS, 1):
                    $1->remove_property(PLAYER_PLAIN_MAPS), 1 :),
              (: $1->query_property(PLAYER_PLAIN_MAPS) :),
              "Display terrain maps without colour");
   add_option("output lookmap", OPTIONS_TYPE_BOOLEAN,
              OPTIONS_TYPE_PLAYTESTER_ONLY,
              (: $3?$1->add_property(TERRAIN_MAP_IN_LOOK_PROP, 1):
                    $1->remove_property(TERRAIN_MAP_IN_LOOK_PROP), 1 :),
              (: $1->query_property(TERRAIN_MAP_IN_LOOK_PROP) :),
              "Display room a map in the terrain long or not");

   add_option( "output tabstops", OPTIONS_TYPE_INTEGER, OPTIONS_TYPE_CRE_ONLY,
              (: $1->add_property( TABSTOP_PROP, $3 ) :),
              (: $1->query_property( TABSTOP_PROP ) :),
               "Show tabstops as <TAB> or as spaces");
   add_option( "output shorthand", OPTIONS_TYPE_BOOLEAN, 0,
               (: $1->add_property( SHORTHAND_OUTPUT_PROP, $3 ) :),
               (: $1->query_property( SHORTHAND_OUTPUT_PROP ) :),
               "Convert others shorthand text into long form");
   
   //
   // Colours.
   //
   foreach (womble in USER_COLOUR_LIST) {
      add_option("colour " + womble, OPTIONS_TYPE_COLOUR, 0,
                 (: $1->set_my_colours($2[<1], $3), 1 :),
                 (: $1->colour_event($2[<1], "default") :),
                 "The colour for " + womble + " messages");
   }
   foreach (womble in ADDITIONAL_CHANNELS) {
     add_option("colour " + womble, OPTIONS_TYPE_COLOUR, OPTIONS_TYPE_CRE_ONLY,
                (: $1->set_my_colours($2[<1], $3), 1 :),
                (: $1->colour_event($2[<1], "default") :),
                "The colour for " + womble + " messages");
   }
   foreach (womble in "/secure/master"->query_domains() - NO_CHANNELS) {
     add_option("colour " + womble, OPTIONS_TYPE_COLOUR, OPTIONS_TYPE_CRE_ONLY,
                (: $1->set_my_colours($2[<1], $3), 1 :),
                 (: $1->colour_event($2[<1], "default") :),
                "The colour for " + womble + " messages");
   }
   
   add_option("colour inform", OPTIONS_TYPE_DYNAMIC_GROUP, 0,
              0, (: get_inform_colours :),
              "The colours of various informational messages");
   add_option("colour club", OPTIONS_TYPE_DYNAMIC_GROUP, 0,
              0, (: get_club_colours :),
              "The colour for club messages");

   //
   // Terminal stuff.
   //
   add_option("terminal type", OPTIONS_TYPE_TERMINAL, 0,
              (: $1->set_term_type($3), 1 :),
              (: $1->query_term_name() :),
              "The type of terminal you are using");
   add_option("terminal rows", OPTIONS_TYPE_INTEGER, 0,
              (: $1->set_rows($3), 1 :),
              (: $1->query_rows() :),
              "The number of rows in your terminal");
   add_option("terminal cols", OPTIONS_TYPE_INTEGER, 0,
              (: $1->set_cols($3), 1 :),
              (: $1->query_cols() :),
              "The number of coloumns in your terminal");

   //
   // Combat stuff.  Wimpy etc.
   //
    
    add_option("combat wimpy", OPTIONS_TYPE_INTEGER, 0,
              (: $1->set_wimpy($3) :),
              (: $1->query_wimpy() :),
               "The percentage of your hitpoints at which you will run away");
   add_option("combat monitor", MONITOR_OPTIONS, 0,
              (: $1->set_monitor(member_array($3, MONITOR_OPTIONS)), 1 :),
              (: MONITOR_OPTIONS[$1->query_monitor()] :),
              "The frequency of display of your combat monitor");
   add_option("combat tactics attitude",
              ({ "insane", "offensive", "neutral", "defensive", "wimp" }), 0,
              (: $1->set_combat_attitude($3), 1 :),
              (: $1->query_combat_attitude() :),
              "Your combat attitude (see help tactics)");
   add_option("combat tactics response",
              ({ "dodge", "neutral", "parry" }), 0,
              (: $1->set_combat_response($3), 1 :),
              (: $1->query_combat_response() :),
              "Your combat response (see help tactics)");
   add_option("combat tactics parry",
              ({ "left", "right", "both" }), 0,
              (: $1->set_combat_parry($3), 1 :),
              (: $1->query_combat_parry() :),
              "Which hand you will parry with (see help tactics)");
   add_option("combat tactics unarmed_parry",
              OPTIONS_TYPE_BOOLEAN, 0,
              (: $1->set_unarmed_parry($3), 1 :),
              (: $1->query_unarmed_parry() :),
              "Which hand you will parry with (see help tactics)");
   add_option("combat tactics attack",
              ({ "left", "right", "both" }), 0,
              (: $1->set_combat_attack($3), 1 :),
              (: $1->query_combat_attack() :),
              "Which hand you will attack with (see help tactics)");
#ifdef USE_SURRENDER
   add_option("combat tactics mercy",
              ({ "always", "ask", "never" }), 0,
              (: $1->set_combat_mercy($3), 1 :),
              (: $1->query_combat_mercy() :),
              "Whether or not you will show mercy to opponents");
#endif   
   add_option("combat tactics focus",
              ({"upper body", "lower body", "head", "neck", "chest",
                  "abdomen", "arms", "hands", "legs", "feet", "none"}), 0,
              (: $1->set_combat_focus($3), 1 :),
              (: $1->query_combat_focus() :),
              "Which body part you will focus on in combat (see help tactics)");
#ifdef USE_DISTANCE
   add_option("combat tactics distance",
              ({"long", "medium", "close", "hand-to-hand", "none" }), 0,
              (: $1->set_combat_distance($3), 1 :),
              (: $1->query_combat_distance() :),
              "Your ideal combat distance (see help tactics)");
#endif   
   add_option("combat killer",
              OPTIONS_TYPE_BOOLEAN, 1,
              (: $1->set_player_killer($3) :),
              (: $1->query_player_killer() :),
              "Whether or not you are a registered player killer");
   //
   // Input options.
   //
   add_option("input ambiguous", OPTIONS_TYPE_BOOLEAN, 0,
              (: change_bool_property(OBJ_PARSER_AMBIGUOUS_PROP, $1, !$3) :),
              (: !$1->query_property(OBJ_PARSER_AMBIGUOUS_PROP) :),
              "Should the parser notify you of ambiguities");
   add_option("input andascomma", OPTIONS_TYPE_BOOLEAN, 0,
              (: change_bool_property(OBJ_PARSER_USE_AND_AS_BREAK_PROP, $1, !$3) :),
          (: !$1->query_property(OBJ_PARSER_USE_AND_AS_BREAK_PROP) :),
              "Should 'and' be treated as a comma (an inclusive list)");
   add_option("input editor", ({ "menu", "magic", "command", "ed" }), 0,
              (: $1->set_editor($3) :),
              (: $1->query_editor() :),
              "Your preferred editor");
   add_option( "input shorthand", OPTIONS_TYPE_BOOLEAN, 0,
               (: $1->add_property( SHORTHAND_PROP, $3 ) :),
               (: $1->query_property( SHORTHAND_PROP ) :),
               "Convert your shorthand typing into long form");

   //
   // Earmuffs.
   //
   foreach (womble in ({ "shout", "newbie", "cryer", "remote-soul",
                           "multiple-soul", "multiple-tell",  "teach",
                           "tell", "remote", "multiple-remote" })) {
     add_option("earmuff events "  + womble, OPTIONS_TYPE_BOOLEAN, 0,
                (: change_earmuffs($1, $2[<1], $3) :),
                (: member_array($2[<1], ($1->query_property("earmuffs")?$1->query_property("earmuffs"):({}))) != -1 :),
                "Should you be informed of " + womble + " messages");
   }
   foreach(womble in ADDITIONAL_CHANNELS) {
     add_option("earmuff events "  + womble, OPTIONS_TYPE_BOOLEAN, OPTIONS_TYPE_CRE_ONLY,
                (: change_earmuffs($1, $2[<1], $3) :),
                (: member_array($2[<1], ($1->query_property("earmuffs")?$1->query_property("earmuffs"):({}))) != -1 :),
                "Should you be informed of " + womble + " messages");
   }
   foreach(womble in "/secure/master"->query_domains() - NO_CHANNELS) {
     add_option("earmuff events "  + womble, OPTIONS_TYPE_BOOLEAN, OPTIONS_TYPE_CRE_ONLY,
                (: change_earmuffs($1, $2[<1], $3) :),
                (: member_array($2[<1], ($1->query_property("earmuffs")?$1->query_property("earmuffs"):({}))) != -1 :),
                "Should you be informed of " + womble + " messages");
   }
   
   foreach(womble in ({ "lord", "intermud-all", "intercre",
                          "intergossip", "dwcre", "dwchat", "remote-spam" })) {
     add_option("earmuff events "  + womble, OPTIONS_TYPE_BOOLEAN, OPTIONS_TYPE_CRE_ONLY,
                (: change_earmuffs($1, $2[<1], $3) :),
                (: member_array($2[<1], ($1->query_property("earmuffs")?$1->query_property("earmuffs"):({}))) != -1 :),
                "Should you be informed of " + womble + " messages");
   }
   add_option("earmuff state", 
              ({ "on", "off", "allowfriends" }), 0,
              (: setup_earmuffs($1, $3) :),
              (: $1->query_earmuffs()? ($1->query_earmuffs() == PLAYER_ALLOW_FRIENDS_EARMUFF ? "allowfriends" : "on") : "off" :),
              "Enable or disable earmuffs always or just for friends");

   add_option("earmuff cut-through", ({ "off", "ask", "auto"}), 1,
              (: $1->add_property("cut earmuffed tells",
                    member_array($3, ({ "off", "ask", "auto"}))) :),
              (: ({ "off", "ask", "auto" })[$1->query_property(
                                             "cut earmuffed tells")] :),
              "Cut through a players tell earmuffs");
   //
   // Creator Command options
   //

   add_option("command ls use_nickname", OPTIONS_TYPE_BOOLEAN, OPTIONS_TYPE_CRE_ONLY,
              (: $3 ? change_bool_property( LS_COMMAND_NICKNAME_PROPERTY, $1, 1 ) : change_bool_property( LS_COMMAND_NICKNAME_PROPERTY, $1, 0 ) :),
              (: $1->query_property( LS_COMMAND_NICKNAME_PROPERTY ) :),
              "Should 'ls' check for nicknames");
   foreach(womble in ERROR_TYPE) {
      womble = lower_case(womble);
      add_option("command look error " + womble, OPTIONS_TYPE_BOOLEAN,
              OPTIONS_TYPE_CRE_ONLY,
              (: change_error_reports($1, $2[<1], $3) :),
              (: member_array(upper_case($2[<1]), ($1->query_property(PLAYER_SHOW_ERRORS_PROP)?$1->query_property(PLAYER_SHOW_ERRORS_PROP):({}))) != -1 :),
              "Show the errors of type " + lower_case(womble) +
              " when doing a look.");
   }

   //
   // Player options.
   //
   add_option("personal description", OPTIONS_TYPE_STRING, 0,
              (: strlen($3) > 30?0:($1->set_desc($3 == "none"?0:$3), 1) :),
              (: (($1->query_desc())?($1->query_desc()):"none") :),
              "Ain't you perdy");
   add_option("personal real_name", OPTIONS_TYPE_STRING, 0,
              (: strlen($3) > 30?0:($1->set_real_name($3 == "none"?0:$3), 1) :),
              (: (($1->query_real_name())?($1->query_real_name()):"none") :),
              "The real name displayed in your finger information");
   add_option("personal location", OPTIONS_TYPE_STRING, 0,
              (: strlen($3) > 30?0:($1->set_where($3 == "none"?0:$3), 1) :),
              (: (($1->query_where())?($1->query_where()):"none") :),
              "The location displayed in your finger information");
   add_option("personal home_page", OPTIONS_TYPE_STRING, 0,
              (: strlen($3) > 30?0:($1->set_homepage($3 == "none"?0:$3), 1) :),
              (: (($1->query_homepage())?($1->query_homepage()):"none") :),
              "The url displayed in your finger information");
   add_option("personal email", OPTIONS_TYPE_STRING, 0,
              (: strlen($3) > 30?0:($1->set_email($3 == "none"?0:$3), 1) :),
              (: (($1->query_email())?($1->query_email()):"none") :),
              "The email address displayed in your finger information");
   add_option("personal birthday", OPTIONS_TYPE_STRING, 0,
              (: (!valid_birthday($3) || $1->query_birthday())? 0:($1->set_birthday($3 == "none"?0:convert_birthday($3)), 1) :),
              (: (($1->query_birthday())?($1->query_birthday()):"none") :),
              "Your birthday");
   add_option("personal execinclude", OPTIONS_TYPE_STRING, 0,
              (: ($3=="none"?$1->remove_property(OPTION_EXEC_INCLUDE):$1->add_property(OPTION_EXEC_INCLUDE, $3)), 1 :),
              (: ($1->query_property(OPTION_EXEC_INCLUDE)?$1->query_property(OPTION_EXEC_INCLUDE):"none") :),
              "The path of files to be included in exec commands?");
   add_option("personal auto teach", OPTIONS_TYPE_BOOLEAN, 0,
              (: $3?$1->add_property(TEACH_COMMAND_AUTO_PROPERTY,$3):$1->remove_property(TEACH_COMMAND_AUTO_PROPERTY) :),
              (: $1->query_property(TEACH_COMMAND_AUTO_PROPERTY) :),
              "Are you available to auto-teach");

   add_option("personal travel",
              ({ "walk", "journey" }), 0,
              (: $3 == "walk" ?$1->remove_property(TERRAIN_MAP_LONG_JUMP_PROPERTY):$1->add_property(TERRAIN_MAP_LONG_JUMP_PROPERTY,1), 1 :),
              (: $1->query_property(TERRAIN_MAP_LONG_JUMP_PROPERTY)?"journey":"walk" :),
              "By default should you walk or journey across terrains");

   //
   // Playtester options.
   //
   add_option("playtester protection", OPTIONS_TYPE_BOOLEAN,
              OPTIONS_TYPE_PLAYTESTER_ONLY,
              (: $3 ? $1->enable_personal_pt_protection() : $1->disable_personal_pt_protection() :),
              (: $1->query_pt_protection() :),
              "Enable or disable playtester protection");

   add_option("personal roleplaying", OPTIONS_TYPE_BOOLEAN, OPTIONS_TYPE_PLAYTESTER_ONLY,
              (: $1->set_role_playing($3), 1 :),
              (: $1->query_role_playing() :),
              "Enable or disable roleplaying mode");

} /* create() */

/**
 * This method adds in an option for the player to be able to set to
 * a mapping.  This method should be used by all the dynamic tree
 * methods to create leaves.
 * @param array the mapping to add the elements to
 * @param path the path to the option eg: ({ "output", "look" })
 * @param type the type of the option eg: OPTIONS_TYPE_BRIEF
 * @param set_function the function to call to set the option
 * @param query_function the function to call to query the option
 * @return 1 if successful, 0 if not
 */
protected void add_option_to_mapping(mapping array, string name, mixed type,
                                     int cre_only, function set_function,
                                     function query_function, string help) {
   class option fluff;

   //
   // Got to the end.
   //
   fluff = new(class option);
   fluff->type = type;
   fluff->set = set_function;
   fluff->query = query_function;
   fluff->restriction = cre_only;
   fluff->suboptions = ([ ]);
   fluff->help = help;
   array[name] = fluff;
} /* add_option() */

/**
 * This method adds in an option for the player to be able to set.
 * The set function will be called with two parameters, the value to
 * set it to and the variable being set.  The set function must return
 * 1 if the value was successfuly set.
 * <p>
 * int set_function(variable, value);
 * @param path the path to the option eg: ({ "output", "look" })
 * @param type the type of the option eg: OPTIONS_TYPE_BRIEF
 * @param set_function the function to call to set the option
 * @param query_function the function to call to query the option
 * @return 1 if successful, 0 if not
 */
protected int add_option(string name, mixed type, int cre_only,
                         function set_function, function query_function,
                         string help) {
   string option;
   mapping stuff;
   string *path;

   path = explode(name, " ");
   stuff = _options;
   foreach (option in path[0..<2]) {
      if (!stuff[option]) {
         //stuff[option] = ([ ]);
         add_option_to_mapping(stuff, option, OPTIONS_TYPE_GROUP, cre_only,
                               0, 0, help);
      } else if (((class option)stuff[option])->type != OPTIONS_TYPE_GROUP) {
         return 0;
      }
      stuff = stuff[option]->suboptions;
   }

   add_option_to_mapping(stuff, path[<1], type, cre_only, set_function,
                         query_function, help);
   return 1;
} /* add_option() */

private mixed query_sub_option(object player, string name, mapping tree) {
   mixed tmp;

   if (mapp(tree[name])) {
      return tree[name];
   }

   if (classp(tree[name])) {
      switch (tree[name]->restriction) {
      case OPTIONS_TYPE_CRE_ONLY:
         if (!player->query_creator()) {
            return 0;
         }
         break;
      case OPTIONS_TYPE_LORD_ONLY:
         if (!player->query_lord()) {
            return 0;
         }
         break;
      case OPTIONS_TYPE_PLAYTESTER_ONLY:
         if (!player->query_playtester()) {
            return 0;
         }
         break;
      }
      if (((class option)tree[name])->type == OPTIONS_TYPE_DYNAMIC_GROUP) {
         tmp = evaluate(tree[name]->query, player, name);
         return tmp;
      }
      if (((class option)tree[name])->type == OPTIONS_TYPE_GROUP) {
         return tree[name]->suboptions;
      }
      return tree[name];
   }
   return 0;
} /* query_sub_option() */

private mixed query_bottom_sub_option(object player, string* path) {
   string option;
   mixed stuff;

   if (!sizeof(path)) {
      return _options;
   }
   stuff = _options;
   foreach (option in path[0..<2]) {
      stuff = query_sub_option(player, option, stuff);
      if (!mapp(stuff)) {
         return 0;
      }
   }
   stuff = query_sub_option(player, path[<1], stuff);
   if (classp(stuff)) {
      switch (stuff->restriction) {
      case OPTIONS_TYPE_CRE_ONLY:
         if (!player->query_creator()) {
            return 0;
         }
         break;
      case OPTIONS_TYPE_LORD_ONLY:
         if (!player->query_lord()) {
            return 0;
         }
         break;
      case OPTIONS_TYPE_PLAYTESTER_ONLY:
         if (!player->query_playtester()) {
            return 0;
         }
         break;
      }
   }
   return stuff;
} /* query_bottom_sub_option() */

/**
 * This method checks to see if the specified option path exists.  This
 * will only return true if it is an actual option, not an option group.
 * @param player the player this is relative to
 * @param name the option path to check
 * @return 1 if it is an option, 0 if not
 */
int is_option(object player, string name) {
   mixed stuff;
   string *path;

   path = explode(name, " ");
   stuff = query_bottom_sub_option(player, path);
   if (!classp(stuff)) {
      return 0;
   }
   return 1;
} /* is_option() */

/**
 * This method checks to see if the specified option group path exists.  This
 * will only return true if it is an option group, not an actual option.
 * @param player the player this is relative to
 * @param name the option path to check
 * @return 1 if it is an option group, 0 if not
 */
int is_option_group(object player, string name) {
   mixed stuff;
   string *path;
 
   path = explode(name, " ");
   stuff = query_bottom_sub_option(player, path);
   if (!mapp(stuff)) {
      return 0;
   }
   return 1;
} /* is_option() */

/**
 * This method returns all the sub options and option groups at this
 * level.
 * @param player the player this is relative to
 * @param name the path to return the suboptions of
 * @return the sub options of the path
 */
string *query_sub_options(object player, string name) {
   mapping stuff;
   string *path;
 
   path = explode(name, " ");
   stuff = query_bottom_sub_option(player, path);
   if (mapp(stuff)) {
      return filter(keys(stuff),
                    (: query_bottom_sub_option($2, $3 + ({ $1 })) :),
                    player, path);
   }
   return ({ });
} /* query_sub_options() */

/**
 * This method returns the different values this option can be
 * set to.
 * @param player the player this is relative to
 * @param name the name of the option
 * @return the allowed parameters
 */
string* query_option_values(object player, string name) {
   mixed stuff;
   class option opt;
   string *path;

   path = explode(name, " ");
   stuff = query_bottom_sub_option(player, path);
   if (classp(stuff)) {
      opt = stuff;
      if (intp(opt->type)) {
         switch (opt->type) {
         case OPTIONS_TYPE_BOOLEAN :
            return ({ "on", "off" });
         case OPTIONS_TYPE_BRIEF :
            return ({ "brief", "verbose" });
         case OPTIONS_TYPE_INTEGER :
            return ({ "integer" });
         case OPTIONS_TYPE_STRING :
            return ({ "string" });
         case OPTIONS_TYPE_PERCENTAGE :
            return ({ "0..100" });
         case OPTIONS_TYPE_COLOUR :
            return ({ "none", "default", "colour" });
         }
      } else {
         return opt->type;
      }
   }
   return ({ });
} /* query_option_values() */

/**
 * This method returns the value of the specified option.
 * @param player the player to find the value on
 * @param path the path to the option
 */
string query_option_value(object player, string path) {
   string* bits;
   mixed value;
   class option stuff;

   bits = explode(path, " ");
   stuff = query_bottom_sub_option(player, bits);
   if (classp(stuff)) {
      value = evaluate(stuff->query, player, bits);
      if (intp(stuff->type)) {
         switch (stuff->type) {
         case OPTIONS_TYPE_BRIEF:
            if (value) {
               value = "verbose";
            } else {
               value = "brief";
            }
            break;
         case OPTIONS_TYPE_BOOLEAN :
            if (value) {
               value = "on";
            } else {
               value = "off";
            }
            break;
         case OPTIONS_TYPE_COLOUR :
            if (value == "") {
               value = "[none]";
            } else if (value == "default") {
               value = "[default]";
            } else {
               value = "[" + value + "" + 
                       lower_case(replace(value, ({ "%^%^", " ", "%^", "" }))) +
                       "%^RESET%^]";
            }
            break;
         default :
         }
      }
      return value + "";
   }
} /* query_option_value() */

/**
 * This method returns the help string of the specified option.
 * @param player the player to find the value on
 * @param path the path to the option
 */
string query_option_help(object player, string path) {
  string *bits;
  mixed stuff;
  
  bits = explode(path, " ");
  stuff = query_bottom_sub_option(player, bits);
  if (classp(stuff)) {
    return copy(stuff->help);
  }
  return "";
}

/**
 * This method sets an option value on the player.
 * @param player the player to find the value on
 * @param path the path to the option
 * @param value the value directly from the command line
 */
int set_option_value(object player, string path, string value) {
   string* bits;
   string* bad;
   mixed set_value;
   class option stuff;

   bits = explode(path, " ");
   stuff = query_bottom_sub_option(player, bits);
   if (classp(stuff)) {
      if (pointerp(stuff->type)) {
         if (member_array(value, stuff->type) == -1) {
            add_failed_mess("You must set your type to one of " +
                            query_multiple_short(stuff->type) + ".\n");
            return 0;
         }
         set_value = value;
      } else switch (stuff->type) {
      case OPTIONS_TYPE_BRIEF:
         if (value == "verbose") {
            set_value = 1;
         } else if (value == "brief") {
            set_value = 0;
         } else {
            return 0;
         }
         break;
      case OPTIONS_TYPE_BOOLEAN :
         if (value == "on" || value == "true") {
            set_value = 1;
         } else if (value == "off" || value == "false") {
            set_value = 0;
         } else {
            return 0;
         }
         break;
      case OPTIONS_TYPE_PERCENTAGE :
      case OPTIONS_TYPE_INTEGER :
         if (sscanf(value, "%d", set_value) != 1) {
            return 0;
         }
         if (stuff->type == OPTIONS_TYPE_PERCENTAGE &&
             (set_value < 0 || set_value > 100)) {
            add_failed_mess("Value must be between 0 and 100.\n");
            return 0;
         }
         break;
      case OPTIONS_TYPE_COLOUR :
         if (value == "none" || value == "default") {
            set_value = value;
         } else {
            set_value = map(explode(value, " "),
                         (: upper_case($1) :));
            bad = filter(set_value, (: member_array($1, _colours) == -1 :));
            if (sizeof(bad)) {
               add_failed_mess("Bad colours " + implode(bad, " ") + ".\n");
               return 0;
            }
            set_value = "%^" + implode(set_value, (: $1 + "%^ %^" + $2 :)) + "%^";

         }
         break;
      default :
         set_value = value;
         break;
      }
      return evaluate(stuff->set, player, bits, set_value);
   }
   return 0;
} /* set_option_value() */

private mapping get_inform_colours(object player) {
   string* colours;
   string womble;
   mapping ret;
   string index;

   if (player->query_creator()) {
      index = "colour 1";
   } else if (player->query_lord()) {
      index = "colour 2";
   } else {
      index = "colour 0";
   }

   if (_cache_input[index]) {
      return _cache_input[index];
   }

   colours = player->query_inform_types();
   ret = ([ ]);
   foreach (womble in colours) {
      add_option_to_mapping(ret, womble, OPTIONS_TYPE_COLOUR, 0,
                            (: $1->set_my_colours($2[<1], $3), 1 :),
                            (: $1->colour_event($2[<1], "default") :),
                            "The colour of " + womble + " informs");
   }
   _cache_input[index] = ret;
   return ret;
} /* get_inform_colours() */

private mapping get_club_colours(object player) {
   string* colours;
   string womble;
   mapping ret;
   string index;

   index = "colour " + player->query_name();
   if (_cache_input[index]) {
      return _cache_input[index];
   }

   colours = map(this_player()->query_player_clubs(),
                 (: CLUB_HANDLER->normalise_name($1) :) );
   ret = ([ ]);
   foreach (womble in colours) {
      add_option_to_mapping(ret, womble, OPTIONS_TYPE_COLOUR, 0,
                            (: $1->set_my_colours("club_" + $2[<1], $3), 1 :),
                            (: $1->colour_event("club_" + $2[<1], "default") :),
                            "The colour for " + womble + " club messages");
   }
   _cache_input[index] = ret;
   call_out((: map_delete(_cache_input, $1) :), 5 * 60, index);
   return ret;
} /* get_inform_colours() */

private int change_bool_property(string prop, object player, int new_value) {
   if (new_value) {
      player->add_property(prop, 1);
   } else {
      player->remove_property(prop);
   }
   return 1;
} /* change_bool_property() */

private int setup_earmuffs(object player, string ear) {
   switch (ear) {
   case "on" :
      if (!player->query_earmuffs()) {
         player->toggle_earmuffs();
      } else if (player->query_earmuffs() == PLAYER_ALLOW_FRIENDS_EARMUFF) {
         player->toggle_earmuffs();
         player->toggle_earmuffs();
      }
      break;
   case "off" :
      if (player->query_earmuffs()) {
         player->toggle_earmuffs();
      }
      break;
   case "allowfriends" :
      player->set_allow_friends_earmuffs();
      break;
   }
   return 1;
} /* setup_earmuffs() */

private int change_error_reports(object player, string error, int new_value) {
   string *on;

   on = player->query_property(PLAYER_SHOW_ERRORS_PROP);
   if (!on) {
      on = ({ });
   }
   error = upper_case(error);
   if (new_value) {
      if (member_array(error, on) == -1) {
         on += ({ error });
         player->add_property(PLAYER_SHOW_ERRORS_PROP, on);
      }
   } else {
      on -= ({ error });
      player->add_property(PLAYER_SHOW_ERRORS_PROP, on);
   }
   return 1;
} /* change_earmuffs() */

private int change_earmuffs(object player, string ear, int new_value) {
   string *on;

   on = player->query_property("earmuffs");
   if (!on) {
      on = ({ });
   }
   if (new_value) {
      if (member_array(ear, on) == -1) {
         on += ({ ear });
         player->add_property("earmuffs", on);
      }
   } else {
      on -= ({ ear });
      player->add_property("earmuffs", on);
   }
   return 1;
} /* change_earmuffs() */

private int valid_birthday(string str) {
#define LENGTHS ({ 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 })

  int tot, month, day;

  if(strlen(str) != 4) {
    return 0;
  }
  if(!sscanf(str, "%d", tot)) {
    return 0;
  }
  month = tot % 100;
  day = tot / 100;
  if(month > 12 || month < 1) {
    return 0;
  }
  if(day < 1) {
    return 0;
  }
  return day <= LENGTHS[month];
} /* valid_birthday() */



string convert_birthday(string str) {
#undef MONTHS
#define MONTHS ({ "January", "February", "March", "April", "May", "June", \
      "July", "August", "September", "October", "November", "December" })
  /* we assume it is 4 characters long */

  int day, month, tot;
  string retval;

  sscanf(str, "%d", tot);
  day = tot / 100;
  month = tot % 100;
  switch(day) {
    case 11:
      retval = "11th"; break;
    case 12:
      retval = "12th"; break;
    case 13:
      retval = "13th"; break;
    default:
      switch(day%10) {
        case 1:
          retval = day+"st"; break;
        case 2:
          retval = day+"nd"; break;
        case 3:
          retval = day+"rd"; break;
        default:
          retval = day+"th";
      }
  }
  return retval + " of " + MONTHS[month-1];
} /* convert_birthday() */
