#include "path.h"
#include <talker.h>
#include <network.h>
#include <term.h>
#include <playtesters.h>
#include <language.h>
#include <cmds/options.h>

// #define DEBUG 1
#define EMOTE_ENABLED 1
#define BEEP sprintf("%c",7)
#define CTRL sprintf("%c", 13)
//#define NOISY

inherit "/std/effect_shadow";

int do_talker_control(mixed *args, string pattern);
int do_chat(string message, string pattern, string verb);

int handle_status_command(string mode, class talker_args args);
int handle_verbose_command(string mode, class talker_args args);
int handle_history_command(string channel, class talker_args args);
int handle_colour_list(mixed *colour_args, class talker_args args);

int handle_colour_command(string channel, string new_colour, class talker_args args);
int handle_colour_on_off_command( string str_status, class talker_args args);
int handle_colour_toggle_command( class talker_args args);

int handle_echo_command( string mode, class talker_args args);

void init() {
  mixed args;
  object where;
  string channel;

  where = environment(player);
#ifdef DEBUG
  tell_creator("taffyd", "%O, %O, %O.\n", player, this_player(), where);
#endif

  // If this isn't here, living objects can't be moved into talkers
  // that are containers by /global/auto_load.  Go figger.
  if( !where )
    return;

  if (!living(where)) {
    player->init();
    return;
  }

  args = arg();

  if (!classp(args)) {
    args = new(class talker_args, status : 0, channels : ({ "one" }),
               verbose : 1, local_echo : 0, colour : 0 );
    set_arg(args);
  }

  where->add_command("talker", player,
                     ({ON_PATTERN,
                         VERBOSE_PATTERN,
                         CHANNEL_PATTERN,
                         DELETE_CHANNEL_PATTERN,
                         HISTORY_PATTERN,
                         HISTORY_CHANNEL_PATTERN,
                         LIST_PATTERN,
                         LIST_CHANNEL_PATTERN,
                         LIST_CHANNEL_PATTERN_BRIEF,
                         SET_COLOUR_PATTERN,
                         COLOUR_ON_OFF_PATTERN,
                         COLOUR_LIST,
                         TOGGLE_COLOUR_PATTERN,
                         NEW_CHANNEL_PATTERN,
                         ECHO_PATTERN,
                         "delete <string'channel'> from <direct:object'talker'>",
                         "help" }),
                     (: do_talker_control($4, $5) :) );

  where->add_command("chat", player,
                     ({ "<string'message'>" }),
                     (: do_chat($4[0], $5, $6) :) );
#ifdef EMOTE_ENABLED
  where->add_command("chat:", player,
                     ({ "<string'message'>" }),
                     (: do_chat(":" + $4[0], $5, $6) :) );
#endif
  /*
   * If they've picked up a talker that's not theirs...
   * nuke the channels.
   */
  if ( !( EFFECTS + "talker" )->valid( args->channels, where ) ) {
    args->channels = ({ "one" });
    set_arg(args);
  }

  foreach (channel in args->channels) {
#ifdef DEBUG
    tell_creator("taffyd", "Adding channel. %O.\n", channel);
#endif
    where->add_command(lower_case(channel), player,
                       "<string'message'>", (: do_chat($4[0], $5, $6) :) );
#ifdef EMOTE_ENABLED
    where->add_command( lower_case(channel) + ":", player,
                        "<string'message'>",
                        (: do_chat(":" + $4[0], $5, $6) :) );
#endif
  }

  player->init();
} /* init() */

object query_player() {
  if(player && environment(player) && living(environment(player)))
    return environment(player);
  return 0;
}

int do_talker_control(mixed *args, string pattern) {
  class talker_args t_args;
  int position;
  string full_name;

#ifdef DEBUG
  tell_creator("taffyd", "Args to talker control is %O. Pattern is %O.\n",
               args, pattern);
#endif

  t_args = arg();

  switch (pattern) {
  case ON_PATTERN:
    return handle_status_command(args[0], t_args);
  case VERBOSE_PATTERN:
    return handle_verbose_command(args[0], t_args);
  case CHANNEL_PATTERN:
    if ( !t_args->status ) {
      tell_object(this_player(), "The eyes are asleep.\n");
      return 1;
    }

    tell_object(this_player(), "You may use the following channels:\n" +
                (string)( EFFECTS +"talker" )->channels() +
                "You are on channel(s) " +
                query_multiple_short( t_args->channels ) + ".\n" );
    return 1;
    break;

  case ECHO_PATTERN:
    return handle_echo_command( args[0], t_args );

  case "delete <string'channel'> from <direct:object'talker'>":
    add_succeeded_mess( ({ "", "" }) );

  case DELETE_CHANNEL_PATTERN:
    if ( !t_args->status ) {
      tell_object(this_player(),  "The eyes are asleep.\n");
      return 1;
    }

    full_name = (EFFECTS + "talker")->normalise_name( args[ 0 ] );

    if ( !(EFFECTS +"talker")->valid( full_name ) ) {
      tell_object(this_player(), "The eyes are not aware of that "
                  "channel.\n");
      return 1;
    }

    position = member_array(full_name, t_args->channels);

    if (position == -1) {
      tell_object(this_player(), "The eyes are not listening to "
                  "that channel.\n");
      return 1;
    }

    if (sizeof(t_args->channels) > 1) {
      t_args->channels = t_args->channels[0..position-1] +
        t_args->channels[position+1..];
      tell_object(this_player(), "The eyes sigh with relief as they "
                  "can now ignore what happens on "
                  "channel \"" + full_name + "\".\n");
    }
    else {
      tell_object(this_player(), "The eyes are bored with not listening "
                  "to any channel at all, and add channel \"one\" to their "
                  "list again.\n");
      t_args->channels = ({ "one" });
    }

    set_arg(t_args);
    break;

  case NEW_CHANNEL_PATTERN:
    if ( !t_args->status ) {
      tell_object(this_player(),  "The eyes are asleep.\n");
      return 1;
    }
    full_name = (EFFECTS + "talker")->normalise_name( args[ 0 ] );

    (EFFECTS+"talker")->clear_cache(this_player()->query_name());
    if ( ( EFFECTS +"talker" )->valid( full_name ) ) {
      if ( member_array( full_name, t_args->channels ) != -1 ) {
        tell_object(this_player(), "The eyes frown at you; they are "
                    "already listening to that channel.\n");
        return 1;
      }

      tell_object(this_player(),  "You whisper to " +
                  player->the_short() + " and the eyes add "
                  "channel \""+ full_name +"\" to the list.\n");
      t_args->channels += ({ full_name });
      set_arg(t_args);
      this_object()->init();
    } else {
      tell_object(this_player(), "You whisper to " +
                  player->the_short() + ", but the eyes "
                  "refuse to give you that channel.\n");
      return 1;
    }
    break;

  case SET_COLOUR_PATTERN:
  case SET_COLOUR_FULL_PATTERN:
    args[ 0 ] = (EFFECTS+"talker")->normalise_name( args[0] );
    return handle_colour_command( args[0], args[1], t_args );

  case COLOUR_ON_OFF_PATTERN:
    return handle_colour_on_off_command( args[0], t_args );

  case TOGGLE_COLOUR_PATTERN:
    return handle_colour_toggle_command(t_args);

  case COLOUR_LIST:
    return handle_colour_list( args, t_args );
    break;

  case HISTORY_CHANNEL_PATTERN:
    args[ 0 ] = (EFFECTS+"talker")->normalise_name( args[0] );
    return handle_history_command(args[0], t_args);

  case HISTORY_PATTERN:
    return handle_history_command("one", t_args);

  case LIST_PATTERN:
  case LIST_CHANNEL_PATTERN:
  case LIST_CHANNEL_PATTERN_BRIEF:

    if ( !t_args->status ) {
      tell_object(this_player(),  "The eyes are asleep.\n");
      return 1;
    }

    //            add_succeeded_mess("");

    if (sizeof(args) == 1) {
      args[ 0 ] = (EFFECTS + "talker")->normalise_name( args[ 0 ] );
      tell_object(this_player(), "The following people are using "
                  "channel \""+ args[0] + "\":\n" +
                  ( EFFECTS +"talker" )->list( args[0],
                                 pattern == LIST_CHANNEL_PATTERN_BRIEF) );
    } else
      tell_object(this_player(), "The following people are using "
                  "talkers:\n" + (string)( EFFECTS +"talker" )->list() );

    return 1;
    break;

  case "help":
  default:
    write( "Talker commands:\n\n"
           "help               : give this help file\n"
           "on|off             : switch talker on or off\n"
           "brief              : make talker use shorter messages\n"
           "verbose            : make talker use longer messages\n"
           "channels           : list valid channels\n"
           "<channel>          : switch to <channel> if valid\n"
           "delete <channel>   : switch <channel> off if valid\n"
           "list [channel]     : list users of the talker\n"
           "history [channel]  : show chat history of <channel>\n"
           "Prefix commands with \"talker\" to use, e.g. "
           "\"talker help\".  For a more detailed description type "
           "\"help talker\".\n" );
    break;
  }

  return 1;
} /* do_talker_control() */

int handle_history_command(string channel, class talker_args args) {
  mixed *history;
  string result;

  if (!args->status) {
    tell_object(this_player(), "The eyes are asleep.\n");
    return 1;
  }

  if (!channel)
    channel = "one";

  if ( !( EFFECTS +"talker" )->valid( channel ) ) {
    tell_object(this_player(), "The eyes are not aware of that "
                "channel.\n");
    return 1;
  }

  //    add_succeeded_mess("");

  tell_object(this_player(), "You whisper to " + player->the_short() +
              ", asking for the last few chats on channel " + channel + ".\n");

  history = HIST->query_chat_history(lower_case( channel ) );

  if ( !arrayp( history ) || !sizeof( history ) ) {
    tell_object(this_player(), "The eyes mutter that nothing has "
                "been said on this channel, or it is not being remembered.\n");
    return 1;
  }

  result = "$P$Channel " + channel + "$P$The eyes mutter about your bad "
    "memory.\n";

  result += implode(map(history, (: "$I$5=$C$" + $1[0] + ($1[1] ? $1[1] : "") +
                                  "%^RESET%^" :)), "\n");
  tell_object(this_player(), result);
  return 1;
} /* handle_history_command() */

string query_colour_status( class talker_args args ) {
  if ( !mapp( args->colour ) ) {
    return "off";
  }

  return args->colour[ "status" ];
} /* query_colour_status() */

int set_colour_status( class talker_args args, string status ) {
  if ( !args->colour ) {
    args->colour = DEFAULT_COLOUR_SETTINGS;
  } else {
    if ( stringp( args->colour ) ) {
      args->colour = DEFAULT_COLOUR_WITH( args->colour );
    }
  }

  args->colour[ "status" ] = status;
  set_arg( args );
  return 1;
} /* set_colour_status() */

int set_channel_colour(class talker_args args, string channel, string colour) {

  if ( !channel || !colour )
    return 0;

  if ( !mapp( args->colour ) ) {
    args->colour = DEFAULT_COLOUR_WITH(colour);
  }

  args->colour[ channel ] = colour;
  set_arg( args );
  return 1;
} /* set_channel_colour() */

varargs string query_channel_colour( class talker_args args, string channel ) {
  string colour;

  if ( !mapp( args ) ) {
    return DEFAULT_COLOUR;
  }

  if ( colour = args->colour[ channel ] ) {
    return colour;
  } else {
    if ( colour = args->colour[ "default" ] ) {
      return colour;
    }
  }

  return DEFAULT_COLOUR;
} /* query_channel_colour() */

int handle_colour_toggle_command( class talker_args args ) {
  if ( query_colour_status( args ) == "on" ) {
    return handle_colour_on_off_command( "off", args );
  }

  return handle_colour_on_off_command( "on", args );
} /* handle_colour_toggle_command() */

int handle_colour_on_off_command( string str_status, class talker_args args ) {
  string status;

  status = query_colour_status( args );

  switch( str_status ) {
  case "on":
    if ( status == "on" ) {
      add_failed_mess( "Talker colours for $D are already on!\n" );
      return -1;
    }
    set_colour_status( args, "on" );
    add_succeeded_mess( ({ "The eyes on $D flash with " +
                             query_channel_colour( args, "default" ) +
                             "colour%^RESET%^.\n", "" }) );
    break;

  case "off":
    if ( status == "off" ) {
      add_failed_mess("Talker colours for $D have already been turned off!\n");
      return -1;
    }
    set_colour_status( args, "off" );
    add_succeeded_mess( ({ "The eyes on $D blink sadly.\n", "" }) );
    break;

  default:
    args->colour = DEFAULT_COLOUR_SETTINGS;
    set_arg( args );
    add_succeeded_mess( ({ "Talker colour settings for $D have been reset.\n",
                             "" }) );
  }

  return 1;
} /* handle_colour_on_off_command() */

mapping create_valid_colours( string my_colours ) {
  string *colours;
  string *valid_colours = TERM_HANDLER->query_colour_codes();

  colours = map( explode( my_colours, " " ),
                 (: upper_case( $1 ) :) );

  return unique_mapping( colours,
                         (: member_array( $1, $(valid_colours) ) > -1 :) );
} /* create_valid_colours() */

int handle_colour_command(string channel, string new_colour,
                          class talker_args args) {
  mapping colours;
  string valid_colour;

  if ( !(EFFECTS + "talker")->valid( channel ) && channel != "default" ) {
    tell_object( this_player(), "You cannot set that "
                 "channel! Use the channel name \"default\" to set "
                 "the default channel colour.\n" );
    return 1;
  }

  colours = create_valid_colours( new_colour );

  switch( sizeof( colours[ 0 ] ) ) {
  case 0:
    valid_colour = implode( map( colours[ 1 ], (: "%^" + $1 + "%^" :) ), "" );

    set_channel_colour( args, channel, valid_colour );
    add_succeeded_mess( ({ "The eyes flash with" + valid_colour +
                             " colour.%^RESET%^\n", "" }) );
    return 1;

  case 1:
    add_failed_mess( "The colour $I is invalid.\n", colours[ 0 ] );
    return -1;

  default:
    add_failed_mess( "The colours $I are invalid.\n", colours[ 0 ] );
    return -1;
  }
} /* handle_colour_command() */

int handle_status_command(string mode, class talker_args args) {
  switch (mode) {
  case "on" :
    if ( args->status ) {
      tell_object(this_player(),  "The eyes are already alert "
                  "and listening to your every word.\n");
    } else {
      tell_object(this_player(), "The eyes open and light up "
                  "eagerly, then dim to a dull glow.\n");
      args->status = 1;
    }
    break;

  case "off" :
    if ( !args->status ) {
      tell_object(this_player(), "You notice that the eyes are "
                  "already closed, so you wake them up again "
                  "in order to tell them to go to sleep.\n");
    } else {
      tell_object(this_player(), "The eyes slowly lose their "
                  "glow as they close.\n");
      args->status = 0;
    }
    break;
  }

  set_arg(args);
  return 1;
} /* handle_status_command() */

int handle_verbose_command(string mode, class talker_args args) {

  if (!args->status) {
    tell_object(this_player(), "The eyes are asleep.\n");
    return 1;
  }

  switch (mode) {
  case "brief":
    if ( !args->verbose ) {
      tell_object(this_player(), "The eyes ignore you completely.\n");
    } else {
      tell_object(this_player(), "The eyes seem sad as they dim "
                  "slightly.\n");
      args->verbose = 0;
    }
    break;

  case "verbose":
    if ( args->verbose ) {
      tell_object(this_player(),  "The eyes glitter at you "
                  "angrily.  They already know that they are to "
                  "be verbose.\n");
    } else {
      tell_object(this_player(), "The eyes light up happily for "
                  "a moment.\n");
      args->verbose = 1;
    }
    break;
  }
  set_arg(args);
  return 1;
} /* handle_verbose_command() */

void receive(string channel, string sender, string text, object *receivers) {
  class talker_args args;
  string *ignoring;
  object where;
  string colour;
  string txt;

  if(!player || !environment(player) )
    return;

  where = environment(player);
  args = arg();

  if (args && !classp(args))
    return;

  if (!args || !args->status || !living(where) || !interactive(where))
    return;

  /*
   * If we're not listening to this channel, then don't receive the
   * message.
   */
  if(member_array(channel, map(args->channels, (: lower_case($1) :)) ) == -1) {
    return;
   }

  /*
   * Creators can their talker messages through the dwchat command,
   * they don't need to hear it twice.
   */
  if ( channel == "intermud" && where->query_creator() ) {
    return;
  }


  if ( file_name( previous_object( 1 ) ) == "/secure/cmds/creator/exe_c" ||
       file_name( previous_object( 1 ) ) == "/secure/cmds/creator/ca_ll" ) {
    if ( this_player(1) && this_player(1)->query_creator() &&
         !this_player(1)->query_property( "talker cheat" ) ) {

      txt = this_player(1)->query_name() +
        " made " + sender + " chat \"" + text + "\" on "
        "channel " + channel;
      user_event( "inform", txt, "cheat" );
      log_file( "/d/admin/log/TALKER", txt + "\n" );

      this_player(1)->add_property( "talker cheat", 1, 3 );
    }
  }

  ignoring = where->query_property("ignoring");

  if (ignoring && member_array(lower_case(sender), ignoring) != -1 ) {
    return;

  }
  if(player->query_property(SHORTHAND_OUTPUT_PROP))
    text = LANGUAGES->fix_shorthand(text);

  if ( args->verbose ) {
#ifdef EMOTE_ENABLED
    if ( text[0] == '@' || text[0] == ':' ) {
      text = ( channel == "one" ? "" : "("+ capitalize(channel) +
               ") " ) +
        sender +"'s wispy voice comes from " +
        player->the_short()+", saying that " +
        (this_player()?this_player()->query_pronoun():"they") +
        " " + text[1..];
    } else {
#endif
      text = ( channel == "one" ? "" : "("+ capitalize(channel) +
               ") " ) +
        sender +"'s wispy voice comes from " +
        player->the_short()+", saying: " + text;
#ifdef EMOTE_ENABLED
    }
#endif
  } else { // brief mode.
#ifdef EMOTE_ENABLED
    if ( text[0] == '@' || text[0] == ':' ) {
      text = ( channel == "one" ? "" : "("+ capitalize(channel) +
               ") " ) +
        sender +" wisps that " +
        (this_player()?this_player()->query_pronoun():"they") +
        " " + text[1..];
    } else {
#endif
      text = ( channel == "one" ? "" : "("+ capitalize(channel) +
               ") " ) +
        sender +" wisps: " +text;
#ifdef EMOTE_ENABLED
    }
#endif
  }

  if ( stringp( args->colour ) ) {
    colour = args->colour;
  }

  if(mapp(args->colour) && query_colour_status( args ) == "on") {
    if (!( colour = args->colour[ channel ] ) ) {
      if(!(colour = args->colour[(EFFECTS + "talker")->
                                normalise_name(channel)])) {
        colour = args->colour[ "default" ];
        /* Incase... */
        if ( !colour ) {
          colour = DEFAULT_COLOUR;
        }
      }
    }
  } else {
    colour = "";
  }

  tell_object(where, colour +  "$I$5=$C$"+ text + "%^RESET%^\n" );

#ifdef NOISY
  if(receivers && sizeof(receivers) && !where->query_invis() &&
     environment(where) && base_name(environment(where)) != "/room/rubbish") {

    receivers = map(receivers, (: $1->query_player() :));
    receivers += ({ this_player() });

    tell_room(environment(where),
              "A wispy voice comes from " + player->poss_short(1) + ".\n",
              receivers);
  }
#endif
}

int do_chat(string message, string pattern, string verb) {
  class talker_args args;
  object *things, *receivers;
  string sender;
#ifdef DEBUG
  tell_creator("taffyd", "Message is %s, pattern is %s, verb is "
               "%s.\n", message, pattern, verb);
#endif

#ifdef EMOTE_ENABLED
  if ( verb[ <1 ] == ':' ) {
    verb = verb[0..<2];
  }
#endif

  if (pattern == "into <direct:object>") {
    add_succeeded_mess("$N whisper$s lovingly to $D.\n");
    return 1;
  }

  args = arg();
  if (!args->status) {
    tell_object(this_player(), "The eyes are asleep.\n");
    return 1;
  }

  /*
   * Strip colours... makes sure players don't "embed" colour
   * codes.
   */
  while (strsrch(message, "%^") != -1) {
    message = replace(message, ({ "%^", "" }) );
  }

  /*
   * If we're not actually listening to this channel, then stop.
   */

  if(member_array( verb, map(args->channels, (: lower_case($1) :)) ) == -1 &&
     verb != "chat") {
    return 0;
  }

  if(this_player()->query_property("no talker")) {
    add_failed_mess("Sorry, you are not allowed to use a talker.\n");
    return 0;
  }

  if(environment(this_player()) &&
     environment(this_player())->query_property("no talker")) {
    if(stringp(environment(this_player())->query_property("no talker")))
      add_failed_mess(environment(this_player())->query_property("no talker"));
    else
      add_failed_mess("Your talker doesn't seem to work here.\n");
    return 0;
  }

  if (this_player()->query_property("gagged")) {
    add_failed_mess("You have been gagged!  You cannot chat until "
                    "your gag is removed.  Perhaps you should talk to a "
                    "creator about this.\n");
    return 0;
  }

  if (environment(player) != this_player()) {
    add_failed_mess("You're too far away from $D to whisper into it.\n");
    return 0;
  }

  if (verb == "chat") {
    if (member_array("one", args->channels) == -1) {
      verb = args->channels[0];
    }
    else {
      verb = "one";
    }
  }

  sender = this_player()->query_cap_name();

  /* Strip bell characters */
  message = replace(message,
                    ({ BEEP, "", CTRL, "", "[A", "", "[B", "", "[C", "",
                         "[D", "", "%^", " " }));

  if(this_player()->query_property(SHORTHAND_PROP))
    message = LANGUAGES->fix_shorthand(message);

  things = children( PATH + "talker" );

  if ( args->local_echo ) {
    things -= ({ find_object( PATH + "talker" ) });
  } else {
    things -= ({ this_object(), find_object( PATH + "talker" ) });
  }

  tell_object(this_player(), "You whisper into " + player->the_short() +
              ".\n");
  if(!this_player()->query_invis() &&
     base_name(environment(this_player())) != "/room/rubbish" ) {
    receivers = map(things, (: $1->query_player() :));
    receivers += ({ this_player() });
    tell_room(environment(this_player()), this_player()->one_short(1) +
              " whispers into " + this_player()->query_possessive() +
              " " + player->short() + ".\n", receivers);
  }

  things->receive(verb, sender, message, things);

  if( verb == "intermud" ) {
    /*
     * Rudimentary support for Intermud emotes.  Not supported on
     * the receiving end.
     */
    if (message[0] == '@' || message[0] == ':') {
      user_event( "intermud_tell", sender + " ", message[1..], "dwchat" );
      message = "$N " + message[1..];
      SERVICES_D->eventSendChannel(sender, "discworld-chat", message, 1);
    } else {
      /* Intermud "discworld_chat" channel support for players. */
      SERVICES_D->eventSendChannel( sender, "discworld-chat", message, 0 );
      user_event( "intermud_tell", sender +": ", message, "dwchat" );
    }
  }
  if ( verb == "playtesters" ) {
    if ( !this_player()->query_creator() &&
         !PLAYTESTER_HAND->query_playtester(this_player()->query_name()) ) {
      args->channels -= ({ "playtesters" });
      set_arg(args);
      add_failed_mess("You're no longer a playtester!\n");
      return 0;
    }
  }
#ifdef EMOTE_ENABLED
  if ( message[0] == '@' || message[0] == ':' ) {
    HIST->add_chat_history( verb , sender +" wisped that " +
                            this_player()->query_pronoun() + " ",
                            message[1..] );
  } else {
#endif
    HIST->add_chat_history( verb , sender +" wisped: ", message );
#ifdef EMOTE_ENABLED
  }
#endif

  return 1;
} /* do_chat() */

/**
 * @ignore yes
 * Tape did this. This is why items with add_command() stop working
 * when you remove the effect.
 */
void remove_effect_shadow( int i ) {
  if ( i == id ) {
    if ( player && environment( player ) ) {
      // This removes the add_command()s set up by the
      // shadow (this_object()) from the guy who's
      // carrying the talker.  It's a bit hacky, but I
      // can't think of any other way to do it.

//      environment( player )->remove_object2( player, 1 );
      environment( player )->remove_object( player, 1 );
      init();
    }
  }

  ::remove_effect_shadow( i );
} /* remove_effect_shadow() */

/**
 * @ignore yes
 */
protected string format_channel_colour( string colour ) {
  return colour +
    lower_case( replace_string( implode( explode( colour, "%^%^" ), " " ),
                                "%^", "" ) ) + "%^RESET%^";
} /* format_channel_colour() */


int handle_colour_list(mixed *colour_args, class talker_args args) {
  string channel;

  if (!mapp(args->colour) || query_colour_status( args ) == "off" ) {
    add_failed_mess( "You currently have talker colours disabled.  Use "
                     "\"talker colour on\" to turn them on.\n" );
    return 0;
  }

  foreach( channel in args->channels ) {
    if ( args->colour[ channel ] ) {
      tell_object( this_player(),
                   "Channel " + channel + " is " +
                   format_channel_colour( args->colour[ channel ] ) + ".\n");
    }
  }

  if ( args->colour[ "default" ] ) {
    tell_object( this_player(),
                 "The default talker colour is " +
                 format_channel_colour( args->colour[ "default" ] ) + ".\n");
  }

  return 1;
} /* handle_colour_list() */

int handle_echo_command( string mode, class talker_args args) {
  int current;

  current = args->local_echo;

  if ( current ) {
    if ( mode == "on" ) {
      add_failed_mess( "Talker echo is already on for $D!\n" );
      return -1;
    }

    args->local_echo = 0;
    add_succeeded_mess( ({ "The eyes on $D flash slowly.\n", "" }) );
  } else {
    if ( mode == "off" ) {
      add_failed_mess( "Talker echo is already off for $D!\n" );
      return -1;
    }

    args->local_echo = 1;
    add_succeeded_mess( ({ "The eyes on $D flash twice rapidly.\n",
                             "" }) );
  }

  set_arg( args );
  return 1;
} /* handle_echo_command() */
