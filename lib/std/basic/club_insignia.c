/**
 * The club insignia inheritable thingy.
 * @author Pinkfish
 * @started Sun Oct  4 04:23:52 EDT 1998
 */
#include <clubs.h>
inherit "/std/basic/club_listing";
inherit "/std/basic/club_discuss";

#include <broadcaster.h>

#define SAVE_START "club_insignia"

#define NOMINATION_STATE 1
#define ELECTION_STATE   2

private nosave string _club;
private nosave int _channel_off;
private nosave int _last_state;
private nosave int _last_club_time;
private nosave int _callout_id;
private nosave string _brief_string;
private nosave int _brief_mode;

private string *query_not_voted_for(string name);
protected int do_recruit(object *players, string club);
protected int club_chat(string str, int emote);
protected int do_history();
protected int do_channel_off(int new_value);
protected int do_resign();
protected int do_resign_position(string name);
protected int do_disbar(string name);
protected int do_brief_mode(int mode);
protected int do_brief_string(string str);
private int query_has_nominated_someone(string name);
private void check_announce(object player);
protected int do_announcement();
string query_club();

private void add_broadcaster() {
   if (query_club()) {
      BROADCASTER->add_object_to_channel(lower_case(query_club()),
                                         this_object());
   }
} /* add_broadcaster() */

/**
 * This method sets the club associated with the badge.
 * @param club the name of the club
 * @see query_club()
 */
void set_club(string club) {
   if (_club) {
      BROADCASTER->remove_object_from_channel(lower_case(_club),
                                              this_object());
   }
   if (club) {
      _club = CLUB_HANDLER->query_club_name(club);
      add_broadcaster();
   } else {
      _club = 0;
   }
} /* set_club() */

/**
 * This method returns the club associated with the badge.
 * @return the club associated with the badge
 * @see set_club()
 */
string query_club() {
   return _club;
} /* query_club() */

/**
 * This method will add the club commands from the player.
 */
protected void add_elected_commands(object per) {
   string tmp_name;
   string *not_voted;
   int announce;

   if (!query_club()) {
      return ;
   }
   tmp_name = replace(lower_case(query_club()), " ", "_");
   per->add_command(tmp_name, this_object(), "announcement",
               (: do_announcement() :));
   per->add_command(tmp_name,
                    this_object(),
                    "resign as <string'position'>",
                    (: do_resign_position($4[0]) :) );
   if (CLUB_HANDLER->is_nomination_in_progress(query_club())) {
      if (_last_state != NOMINATION_STATE ||
           _last_club_time + 24 * 60 * 60 < time()) {
         _last_state = NOMINATION_STATE;
         _last_club_time = time();
         if (!query_has_nominated_someone(this_player()->query_name())) {
            call_out((: tell_object($1,
                "%^YELLOW%^The club '" + query_club() + "' is currently accepting "
                "nominations for positions.%^RESET%^\n") :), 2, this_player());
         }
      }
   } else if (CLUB_HANDLER->is_voting_in_progress(query_club())) {
      if (_last_state != ELECTION_STATE ||
          _last_club_time + 24 * 60 * 60 < time()) {
         _last_state = ELECTION_STATE;
         _last_club_time = time();
         not_voted = query_not_voted_for(this_player()->query_name());
         if (sizeof(not_voted)) {
            call_out((: tell_object($1,
                "%^YELLOW%^You have not voted for " +
                query_multiple_short($(not_voted)) + " in '" + query_club() +
                "'.%^RESET%^\n") :), 2, this_player());
         }
      }
   } else {
      _last_state = 0;
   }

   // Check for an announcement.
   announce = CLUB_HANDLER->query_announcement_time(query_club());
   if (announce &&
       announce != this_player()->query_property(_club + " announce time")) {
      remove_call_out(_callout_id);
      _callout_id = call_out((: check_announce :), 0, this_player());
   }
   // Give the property 4 weeks to time out.
   this_player()->add_property(_club + " announce time", 
              this_player()->query_property(_club + " announce time"),
                                  4 * 7 * (24 * 60 * 60));

   club_discuss::add_elected_commands(per);
} /* add_elected_commands() */

/**
 * This method will add the club commands from the player.
 */
protected void add_club_commands(object per) {
   string tmp_name;

   if (!query_club()) {
      return ;
   }
   tmp_name = replace(lower_case(query_club()), " ", "_");
   per->add_command("recruit",
                    this_object(),
                    "<indirect:living'player'> into <string'club'>",
                  (: do_recruit($1, $4[1]) :) );
   per->add_command(tmp_name,
                    this_object(),
                    "members",
                         (: club_members(query_club(), 0, this_player()->query_name()) :));
   per->add_command(tmp_name,
                    this_object(),
                    "recruit <indirect:living'player'>",
                         (: do_recruit($1, query_club()) :));
   per->add_command(tmp_name,
                    this_object(),
                    "members {online|listening}",
                         (: club_members(query_club(), ($4[0]=="online"?1:2),
                                         this_player()->query_name()) :));
   per->add_command(tmp_name,
                    this_object(),
                    "info",
                         (: club_info(query_club(), 1,
                                      this_player()->query_name()) :));
   per->add_command(tmp_name,
                    this_object(),
                    "talk <string>", (: club_chat($4[0], 0) :));
   per->add_command(tmp_name,
                    this_object(),
                    "emote <string>", (: club_chat($4[0], 1) :));
   per->add_command(tmp_name,
                    this_object(),
                    "history", (: do_history() :));
   per->add_command(tmp_name,
                    this_object(),
                    "resign", (: do_resign() :));
   per->add_command(tmp_name,
                    this_object(),
                    "disbar <string'member'>", (: do_disbar($4[0]) :));
   per->add_command(tmp_name,
                    this_object(),
                    "mute {on|off}",
                    (: do_channel_off($4[0] == "on") :));
   per->add_command(tmp_name,
                    this_object(),
                    "brief {on|off}",
                    (: do_brief_mode($4[0] == "on") :));
   per->add_command(tmp_name,
                    this_object(),
                    "brief name <string'brief name'>",
                    (: do_brief_string($4[0]) :));


   // Add in the discussion commands.
   club_discuss::add_club_commands(per);

   if (CLUB_HANDLER->is_elected_club(query_club())) {
      add_elected_commands(this_player());
   }
} /* add_club_commands() */

/**
 * @ignore yes
 */
void init() {

   if (query_club() &&
       this_player() == environment() &&
       CLUB_HANDLER->is_member_of(query_club(), this_player()->query_name())) {
      this_player()->add_player_club(query_club());
      add_club_commands(this_player());
   } else {
      this_player()->remove_player_club(query_club());
   }
} /* init() */

private void check_announce(object player) {
   int announce;

   if (!query_club()) {
      return ;
   }
   // Every 15 minutes.
   announce = CLUB_HANDLER->query_announcement_time(query_club());
   if (this_player() &&
       announce &&
       announce != this_player()->query_property(_club + " announce time")) {
      remove_call_out(_callout_id);
      call_out((: check_announce :), 15 * 60, player);
      tell_object(player,
                  "\n%^YELLOW%^The club '" +
                  CLUB_HANDLER->query_club_name(query_club()) +
                  "' has a new announcement.\n%^RESET%^\n");
   }
} /* check_announce() */

/**
 * This method checks to see if the person has nomiated anyone for
 * the club.
 */
private int query_has_nominated_someone(string name) {
   string *positions;
   string pos;
   string *people;
   string person;

   positions = CLUB_HANDLER->query_valid_positions(query_club());
   foreach (pos in positions) {
      people = CLUB_HANDLER->query_nominated_for(query_club(), pos);
      foreach (person in people) {
         if (member_array(name,
                 CLUB_HANDLER->query_nominators_for(query_club(), pos, person)) != -1) {
            return 1;
         }
      }
   }
   return 0;
} /* query_has_nominated_someone() */

/**
 * This method returns the list of positions that have not been voted for.
 */
private string *query_not_voted_for(string name) {
   string *positions;

   positions = CLUB_HANDLER->query_valid_positions(query_club());
   positions = filter(positions,
         (: !CLUB_HANDLER->has_person_voted(query_club(), $1, $(name)) &&
            sizeof(CLUB_HANDLER->query_nominated_for(query_club(), $1)) > 1 :));
   return positions;
} /* query_not_voted_for() */

/**
 * This method returns the status of the channel off flag.
 * @return the current value of the channel off flag
 * @see club_chat()
 * @see event_channel_message()
 */
int query_channel_off() {
   return _channel_off;
} /* query_channel_off() */

/**
 * This method tells us if the badge is in brief mode or not.
 * @return the current brief mode
 */
int query_brief_mode() {
   return _brief_mode;
} /* query_brief_mode() */

/**
 * This method returns the short name for the channel string.
 * @return the short name for the channel
 */
string query_brief_string() {
   return _brief_string;
} /* query_brief_string() */

/**
 * The says something wonderful and exciting to all the other club members.
 * @param str the exciting comment I know they will say
 */
protected int club_chat(string str, int emote) {
   string player_name;
   string channel_name;
   string colour;

   if (!CLUB_HANDLER->is_member_of(query_club(), this_player()->query_name())) {
      add_failed_mess("You can only chat to clubs you are a member of.\n");
      return 0;
   }

   if (_channel_off) {
      add_failed_mess("You have your club channel turned off.\n");
      return 0;
   }

   player_name = this_player()->query_cap_name();
   if (this_player()->query_family_name()) {
      player_name += " " + this_player()->query_family_name();
   }
   BROADCASTER->broadcast_to_channel(this_player(),
                                     lower_case(query_club()),
                                     ({ str, emote }));
   if (_brief_mode) {
      channel_name = _brief_string;
   } else {
      channel_name = query_club();
   }
   colour = this_player()->colour_event("club_" +
                       CLUB_HANDLER->normalise_name(query_club()), "%^CYAN%^");
   if (emote) {
      write("$I$5=(" + colour + channel_name +
            "%^RESET%^) " + player_name +
            " " + str + "\n");
   } else {
      write("$I$5=(" + colour + channel_name +
            "%^RESET%^) " + player_name +
            ": " + str + "\n");
   }
   if (!this_player()->query_hide_invis()["hiding"] &&
       !this_player()->query_invis()) {
      add_succeeded_mess(({ "",
                            "$N stare$s intently into space for a bit.\n" }));
   }
   return 1;
} /* club_chat() */

/**
 * The method is called when the channel is triggered.
 * @param ob the person who talked
 * @param channel the name of the channel
 * @param message the pmessage top print
 * @see club_chat()
 * @see do_history()
 */
void event_channel_message(object ob, string channel, mixed *data) {
   object our_player;
   int emote;
   string message;
   string player_name;
   string channel_name;
   string colour;

   if (channel != lower_case(query_club())) {
      return ;
   }
   our_player = environment();

   // Found em!
   if (our_player &&
       userp(our_player) &&
       our_player != this_player()) {

      // See if we are ignoreing them.
      if (sizeof(our_player->query_ignoring( ({ ob }) ))) {
          return ;
      }

      emote = data[CLUB_MESSAGE_EMOTE_INDEX];
      message = data[CLUB_MESSAGE_MESSAGE_INDEX];

      // Make sure they really are a member of the club.
      if (CLUB_HANDLER->is_member_of(query_club(), our_player->query_name()) &&
          !_channel_off) {
         player_name = ob->query_cap_name();
         if (ob->query_family_name()) {
            player_name += " " + ob->query_family_name();
         }
         if (_brief_mode) {
            channel_name = _brief_string;
         } else {
            channel_name = query_club();
         }
         colour = our_player->colour_event("club_" +
                       CLUB_HANDLER->normalise_name(query_club()), "%^CYAN%^");
         if (emote) {
            our_player->add_message("$I$5=(" + colour + channel_name +
                                    "%^RESET%^) " +
                                    player_name + " " + message + 
                                    "\n%^RESET%^", ({ }));
         } else {
            our_player->add_message("$I$5=(" + colour + channel_name +
                                    "%^RESET%^) " +
                                    player_name + ": " + message + 
                                    "\n%^RESET%^", ({ }));
         }
      }
   }
} /* event_channel_message() */

/** @ignore yes */
mapping query_dynamic_auto_load(mapping map) {
   if (!map) {
      map = ([ ]);
   }
   map[SAVE_START + "club"] =  query_club();
   map[SAVE_START + "channel off"] =  _channel_off;
   map[SAVE_START + "last state"] = _last_state;
   map[SAVE_START + "last time"] = _last_club_time;
   map[SAVE_START + "brief mode"] = _brief_mode;
   map[SAVE_START + "brief string"] = _brief_string;
   return club_discuss::query_dynamic_auto_load(map);
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_dynamic_arg(mapping map) {
   string fluff;

   set_club(map[SAVE_START + "club"]);
   _channel_off = map[SAVE_START + "channel off"];
   _last_state = map[SAVE_START + "last state"];
   _last_club_time = map[SAVE_START + "last time"];
   _brief_mode = map[SAVE_START + "brief mode"];
   _brief_string = map[SAVE_START + "brief string"];

   if (query_club()) {
      fluff = CLUB_HANDLER->query_club_name(query_club());
   }
   if (fluff) {
      _club = fluff;
   }

   club_discuss::init_dynamic_arg(map);
} /* init_dynamic_arg() */

/**
 * This is the method to use when recruiting people.
 * @param obs the people to recruit
 * @return 1 on success, 0 on failure
 * @see /obj/handlers/club_handler.c
 */
protected int do_recruit(object *obs, string club) {
   object ob;
   object *already;
   object *invites;
   object *not_player;

   if (!query_club()) {
      add_failed_mess("The club associated with this badge appears to "
                      "have evaporated.\n");
      return 0;
   }
   if (lower_case(club) != lower_case(query_club())) {
      add_failed_mess("You can only recruit people to club '" + query_club() + "', "
                      "not '" + club  + "'.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_recruiter_of(query_club(), this_player()->query_name())) {
      add_failed_mess("You must be a recruiter to recruit people to "
                      "the club.\n");
      return 0;
   }

   if (CLUB_HANDLER->is_personal_club(query_club()) &&
       sizeof(CLUB_HANDLER->query_members(query_club())) >= CLUB_MAXIMUM_ELECTED_MEMBERS) {
      add_failed_mess("Your club has more than " +
                      query_num(CLUB_MAXIMUM_ELECTED_MEMBERS) + " members, so it "
                      "must change into an elected club before more "
                      "can be added.\n");
      return 0;
   }

   invites = ({ });
   already = ({ });
   not_player = ({ });
   foreach (ob in obs) {
      if (!userp(ob)) {
         not_player += ({ ob });
      } else if (!CLUB_HANDLER->is_member_of(query_club(), ob->query_name())) {
         invites += ({ ob });
         tell_object(ob, this_player()->the_short() + " invites you to "
                         "join the club '" + query_club() + "'.\nUse the command "
                         "'accept invite from " +
                         this_player()->query_name() +
                         " to " + query_club() + "'.\n");
         ob->add_respond_command(CLUB_RESPOND_TYPE, this_player(),
                                 query_club());
      } else {
         already += ({ ob });
      }
   }

   if (!sizeof(invites)) {
      if (sizeof(already)) {
         if (sizeof(already) > 1) {
            add_failed_mess("$I are already members of the club '" + 
                         query_club() + "'.\n", already);
         } else {
            add_failed_mess("$I is already a member of the club '" + 
                         query_club() + "'.\n", already);
         }
      }
      if (sizeof(not_player)) {
         add_failed_mess("You can only recruit players into the club, $I " +
                         (sizeof(not_player) > 1?"are not players":
                                                  "is not a player"),
                         not_player);
      }
   } else {
      add_succeeded_mess(({
        "$N invite $I to join the club '" + query_club() + "'.\n",
        "$N invites $I to join the club '" + query_club() + "'.\n" }),
                         invites);
   }
   return sizeof(invites) > 0;
} /* do_recruit() */

/**
 * This method prints out the history of the channel.
 * @see club_chat()
 * @see event_channel_message()
 */
protected int do_history() {
   mixed *history;
   mixed *data;
   int emote;
   string message;
   string name;
   string colour;
   string tim;

   if (!CLUB_HANDLER->is_member_of(query_club(), this_player()->query_name())) {
      add_failed_mess("You must be a member of the club to get a "
                      "history of its channel.\n");
      return 0;
   }

   history = BROADCASTER->query_channel_history(lower_case(query_club()));
   if (!history ||
       !sizeof(history)) {
      add_failed_mess("No history for this channel.\n");
      return 0;
   }

   colour = this_player()->colour_event("club_" + 
                       CLUB_HANDLER->normalise_name(query_club()), "%^CYAN%^");
   foreach (data in history) {
      name = data[0];
      emote = data[1][CLUB_MESSAGE_EMOTE_INDEX];
      message = data[1][CLUB_MESSAGE_MESSAGE_INDEX];
      if (sizeof(data) > 2) {
         tim = ctime(data[2])[4..15];
      } else {
         tim = "";
      }
      if (emote) {
         this_player()->add_message("$I$5=(" + colour +
                         CLUB_HANDLER->query_club_name(query_club()) + 
                         "%^RESET%^ " + tim + ") " +
                     name + " " + message + "\n", ({ }));
      } else {
         this_player()->add_message("$I$5=(" + colour +
                         CLUB_HANDLER->query_club_name(query_club()) + 
                         "%^RESET%^ " + tim + ") " +
                     name + ": " + message + "\n", ({ }));
      }
   }

   write("History for the '" + query_club() + "' club channel.\n");
   return 1;
} /* do_history() */

/**
 * This method handles setting the flag and printing all the happy success
 * messages.
 * @param flag the new flag
 */
protected int do_channel_off(int flag) {
   if (!CLUB_HANDLER->is_member_of(query_club(), this_player()->query_name())) {
      add_failed_mess("You must be a member of the channel to turn it off.\n");
      return 0;
   }

   if (_channel_off == flag) {
      if (_channel_off) {
         add_failed_mess("The channel is already muted.\n");
      } else {
         add_failed_mess("You are already listening to the channel.\n");
      }
      return 0;
   }

   _channel_off = flag;
   if (_channel_off) {
      add_succeeded_mess(({ "Muting the channel '" + query_club() + "'.\n", "" }));
   } else {
      add_succeeded_mess(({ "Removing the mute from the channel '" + query_club() +
                            "'.\n", "" }));
   }
   return 1;
} /* do_channel_off() */

/**
 * This method sets the brief mode of the badge.
 * @param mode the brief mode]
 * @return 1 on success, 0 on failure
 */
protected int do_brief_mode(int mode) {
   if (!_brief_string) {
      add_failed_mess("You need to set a brief string for '" +
                      query_club() + "' before you can change the brief "
                      "mode.\n");
      return 0;
   }

   _brief_mode = mode;
   if (_brief_mode) {
      write("Set the channel '" + query_club() + "' into brief mode "
            "(using '" + _brief_string + "' as the channel name).\n");
   } else {
      write("Set the channel '" + query_club() + "' into verbose mode,\n");
   }
   return 1;
} /* do_brief_mode() */

/**
 * This method sets the brief message to use for the channel messages.
 * @param str the channel message short
 * @return 1 on success, 0 on failure
 */
protected int do_brief_string(string str) {
   if (strlen(str) > strlen(query_club())) {
      add_failed_mess("Cannot set the channel name brief to longer than it "
                      "already is!\n");
      return 0;
   }

   write("Setting the brief channel name for '" + query_club() +
         "' to '" + str + "'.\n");
   _brief_string = str;
   return 1;
} /* do_brief_string() */

/**
 * This method resigns as a position in the club.
 * @return 1 on succes, 0 on failure
 * @see confirm_resign()
 */
protected int do_resign_position(string position) {
   if (!query_club()) {
      add_failed_mess("This badge is not associated with any clubs!\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_member_of(query_club(),
                                   this_player()->query_name())) {
      add_failed_mess("You need to be a member of a club before you can "
                      "resign from it.\n");
      return 0;
   }

   if (!CLUB_HANDLER->holds_position_of(query_club(),
                                        position,
                                        this_player()->query_name())) {
      add_failed_mess("You need to hold a position before you can "
                      "resign from it.\n");
      return 0;
   }

   write("Are you sure you wish to resign from the position " + position +
         " in the club '" + query_club() + "'? ");
   input_to("confirm_resign", 0, position);
   add_succeeded_mess("");
   return 1;
} /* do_resign() */

/**
 * This method starts the resignation process.
 * @return 1 on succes, 0 on failure
 * @see confirm_resign()
 */
protected int do_resign() {
   if (!query_club()) {
      add_failed_mess("This badge is not associated with any clubs!\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_member_of(query_club(),
                                   this_player()->query_name())) {
      add_failed_mess("You need to be a member of a club before you can "
                      "resign from it.\n");
      return 0;
   }

   write("Are you sure you wish to resign from the club '" +
         query_club() + "'? ");
   input_to("confirm_resign");
   add_succeeded_mess("");
   return 1;
} /* do_resign() */

private void do_fixup_badges(string club_name) {
   object ob;
   string club;

   foreach (ob in all_inventory(this_player())) {
      club = ob->query_club();
      if (club &&
          ob->query_club_insignia()) {
         if (CLUB_HANDLER->query_club_name(club) ==
             CLUB_HANDLER->query_club_name(query_club())) {
            set_club(0);
         }
      }
   }
} /* do_fixup_badges() */

/**
 * This method checks to make sure they really wish to resign.
 * @param str the response
 * @see do_resign()
 */
protected void confirm_resign(string str, string position) {
   string club;

   str = lower_case(str);
   if (!strlen(str) ||
       str[0] == 'n') {
      write("Aborted resignation.\n");
      return 0;
   }

   if (str[0] == 'y') {
      if (!position) {
         if (CLUB_HANDLER->remove_member(query_club(),
                                         this_player()->query_name())) {
            write("You have resigned from the club '" + query_club() + "'.\n");
            club = query_club();
            set_club(0);
            call_out( (: do_fixup_badges :), 0, club);
         } else {
            write("You were unable to resign from the club '" + query_club() +
                  "'.\n");
         }
      } else {
         if (CLUB_HANDLER->set_position_holder(query_club(),
                                              position,
                                              CLUB_UNKNOWN_MEMBER)) {
            write("You have resigned from the position " + position + 
                  " in the club '" + query_club() + "'.\n");
         } else {
            write("You were unable to resign from the position " + position + 
                  " in the club '" + query_club() + "'.\n");
         }
      }
   } else {
      write("You need to answer yes or no.\n");
      if (!position) {
         write("Are you sure you wish to resign from the club '" +
                         query_club() + "'? ");
      } else {
         write("Are you sure you wish to resign from the position " + position +
               " in the club '" + query_club() + "'? ");
      }
      input_to("confirm_resign", 0, position);
   }
} /* confirm_resign() */

/**
 * This method allows the founders and presidents/vice presidents to disbar
 * people from the club.
 * @param name the name of the person to disbar
 */
protected int do_disbar(string name) {
   if (!CLUB_HANDLER->is_member_of(query_club(), this_player()->query_name())) {
      add_failed_mess("You must be a member of the club before you can "
          "disbar people from it.\n");
      return 0;
   }

   if (CLUB_HANDLER->is_elected_club(query_club())) {
      if (!CLUB_HANDLER->is_president_of(query_club(), this_player()->query_name()) &&
          !CLUB_HANDLER->is_vice_president_of(query_club(), this_player()->query_name())) {
         add_failed_mess("You must be the president or the vice president to "
                         "disbar someone.\n");
         return 0;
      }
   } else if (!CLUB_HANDLER->is_founder_of(query_club(), this_player()->query_name())) {
      add_failed_mess("You must be the founder of the club to disbar "
                      "someone.\n");
      return 0;
   }

   name = lower_case(name);
   if (name == this_player()->query_name()) {
      add_failed_mess("You cannot disbar yourself.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_member_of(query_club(), name)) {
      add_failed_mess("You must disbar someone who is a member of the "
                      "club.\n");
      return 0;
   }

   write("Do you want to disbar " + capitalize(name) + " from the club '" +
                   query_club() + "'? ");
   add_succeeded_mess("");
   input_to("confirm_disbar", 0, name);
   return 1;
} /* do_disbar() */

/**
 * THis method confirms the disbar request from the player.
 * @param str the response string
 * @param name the name of the person to disbar
 * @see do_disbar()
 */
protected void confirm_disbar(string str, string name) {
   if (str == "" ||
       str[0] == 'q' ||
       str[0] == 'n') {
      write("Aborting disbar.\n");
      return ;
   }

   if (str[0] == 'y') {
      if (CLUB_HANDLER->remove_member(query_club(), name)) {
         write("Disbarred " + capitalize(name) +
               " from the club '" + query_club() + "'.\n");
      } else {
         write("You were unable to disbar " + capitalize(name) +
               " from the club '" + query_club() + "'.\n");
      }
   } else {
      write("You must type yes or no.\n");
      write("Do you want to disbar " + capitalize(name) + " from the club '" +
                      query_club() + "'? ");
      input_to("confirm_disbar", 0, name);
   }
} /* confirm_disbar() */

/**
 * This method will print out any announcements associated with the club.
 * @return 1 on success
 */
protected int do_announcement() {
   string announce;

   announce = CLUB_HANDLER->query_announcement(query_club());
   if (!strlen(announce)) {
      add_failed_mess("There is no announcement to read.\n");
      return 0;
   }
   write("$P$Announcement$P$Added by the president or secretary at " +
         ctime(CLUB_HANDLER->query_announcement_time(query_club())) + "\n" +
         announce + "\n");
   // Give the property 4 weeks to time out.
   this_player()->add_property(_club + " announce time",
                               CLUB_HANDLER->query_announcement_time(query_club()),
                               4 * 7 * (24 * 60 * 60));
   return 1;
} /* do_announcement() */

/**
 * This is the event generated when the person joins the club.
 * @param person the person doing the joining
 * @param club_name the name of the club being joined
 */
void event_joined_club(object person, string club_name) {
   if (CLUB_HANDLER->query_club_name(query_club()) ==
       CLUB_HANDLER->query_club_name(club_name)) {
      add_club_commands(environment());
   }
} /* event_club_join() */

/**
 * This method is called when the club type is changed.  It will add on
 * the elected club commands, if that is what we have changed too.
 */
void event_club_changed(object person,
                        string club_name) {
   if (CLUB_HANDLER->is_elected_club(club_name)) {
      call_out( (: add_elected_commands(environment()) :), 0);
   }
} /* event_club_changed() */

mixed * stats()
{
  return ({
    ({ "club name", _club })
    });
}
