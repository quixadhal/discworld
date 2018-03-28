/*
 * $Id: channel.c,v 1.13 2003/03/06 18:31:56 wodan Exp $
 */
/*    /daemon/services/channel.c
 *    from the Nightmare IV LPC Library
 *    handles intermud channels using the Intermud 3 protocols
 *    created by Descartes of Borg 950625
 *    Modified for Discworld by Turrican 1-11-95
 */

#define SERVICE_CHANNEL

#define TALKER "/std/shadows/object/talker"
#define HIST "/obj/handlers/hist_handler"

#include <localtime.h>
#include <term.h>
#include <network.h>
#define SAVE_INTERMUD "/net/save/services"

nosave mapping hour_colors = ([
        0   : "%^BOLD%^BLACK%^",
        1   : "%^BOLD%^BLACK%^",
        2   : "%^BOLD%^BLACK%^",
        3   : "%^BOLD%^BLACK%^",
        4   : "%^RED%^",
        5   : "%^RED%^",
        6   : "%^ORANGE%^",
        7   : "%^ORANGE%^",
        8   : "%^YELLOW%^",
        9   : "%^YELLOW%^",
        10  : "%^GREEN%^",
        11  : "%^GREEN%^",
        12  : "%^BOLD%^GREEN%^",
        13  : "%^BOLD%^GREEN%^",
        14  : "%^BOLD%^WHITE%^",
        15  : "%^BOLD%^WHITE%^",
        16  : "%^BOLD%^CYAN%^",
        17  : "%^BOLD%^CYAN%^",
        18  : "%^CYAN%^",
        19  : "%^CYAN%^",
        20  : "%^BOLD%^BLUE%^",
        21  : "%^BOLD%^BLUE%^",
        22  : "%^BLUE%^",
        23  : "%^BLUE%^",
        ]);

nosave mapping channel_colors = ([
        "admin"             : "%^BOLD%^MAGENTA%^",
        "fluffy"            : "%^BOLD%^MAGENTA%^",
        "playtesters"       : "%^BOLD%^MAGENTA%^",

        "code"              : "%^BOLD%^GREEN%^",
        "cre"               : "%^BOLD%^GREEN%^",
        "liaison"           : "%^BOLD%^GREEN%^",

        "dchat"             : "%^CYAN%^",
        "ds"                : "%^YELLOW%^",
        "free_speech"       : "%^BOLD%^RED%^",
        "skylib"            : "%^BOLD%^MAGENTA%^",

        "dwchat"            : "%^BOLD%^WHITE%^",
        "discworld-chat"    : "%^BOLD%^WHITE%^",
        "dwcre"             : "%^YELLOW%^",
        "discworld-cre"     : "%^YELLOW%^",
        "intergossip"       : "%^GREEN%^",
        "imud_gossip"       : "%^GREEN%^",
        "intercre"          : "%^ORANGE%^",
        "imud_code"         : "%^ORANGE%^",
        "wiley"             : "%^YELLOW%^",
        "wileymud"          : "%^YELLOW%^",

        "url"               : "%^BOLD%^WHITE%^",
        "fluffos"           : "%^YELLOW%^",

        "default"           : "%^BOLD%^BLUE%^",
        ]);

nosave string* chat_colors = ({
        "%^RED%^",
        "%^GREEN%^",
        "%^ORANGE%^",
        "%^BLUE%^",
        "%^MAGENTA%^",
        "%^CYAN%^",
        "%^BOLD%^BLACK%^",
        "%^BOLD%^RED%^",
        "%^BOLD%^GREEN%^",
        "%^YELLOW%^",
        "%^BOLD%^BLUE%^",
        "%^BOLD%^MAGENTA%^",
        "%^BOLD%^CYAN%^",
        "%^BOLD%^WHITE%^",

        "%^B_RED%^BOLD%^WHITE%^",
        "%^B_GREEN%^BOLD%^WHITE%^",
        "%^B_BLUE%^BOLD%^WHITE%^",
        "%^B_MAGENTA%^BOLD%^WHITE%^",

        "%^B_RED%^YELLOW%^",
        "%^B_GREEN%^YELLOW%^",
        "%^B_BLUE%^YELLOW%^",
        "%^B_MAGENTA%^YELLOW%^",

        "%^B_RED%^BLACK%^",
        "%^B_GREEN%^BLACK%^",
        "%^B_MAGENTA%^BLACK%^",
        "%^B_CYAN%^BLACK%^",
        "%^B_ORANGE%^BLACK%^",
        "%^B_WHITE%^BLACK%^",
        });

mapping chatters = ([ ]);
mapping renamed_chatters = ([ ]);
int chat_counter = 0;

// Get the raw data from localtime(), adjusted to be GMT
// if desired.
varargs mixed *getRawTimeBits(int the_time, int use_local) {
    mixed *bits;

    if(undefinedp(the_time))
        the_time = time();

    bits = localtime(the_time);
    if( !use_local ) {
        // Add GMT offset if we aren't using local time
        bits = localtime(the_time + bits[LT_GMTOFF]);
        bits[LT_ZONE] = "GMT";
    }
    return bits;
}

// Get the date and time as an ISO formatted string.
varargs string timestamp(int the_time, int use_local) {
    string ret;
    mixed *bits;

    bits = getRawTimeBits(the_time, use_local);
    ret = sprintf("%04d-%02d-%02d %02d:%02d:%02d %s",
            bits[LT_YEAR], bits[LT_MON]+1, bits[LT_MDAY],
            bits[LT_HOUR], bits[LT_MIN], bits[LT_SEC],
            bits[LT_ZONE]);
    return ret;
}

// Get an hour::minute timestamp
varargs string getDayTime(int the_time, int use_local) {
    string ret;
    mixed *bits;

    bits = getRawTimeBits(the_time, use_local);
    ret = sprintf("%02d:%02d", bits[LT_HOUR], bits[LT_MIN]);
    return ret;
}

// Get an hour:minute timestamp with daytime color.
varargs string getColorDayTime(string prefix, string suffix, int the_time, int use_local) {
    string ret;
    mixed *bits;

    bits = getRawTimeBits(the_time, use_local);
    if(undefinedp(prefix))
        prefix = "";
    if(undefinedp(suffix))
        suffix = "";
    ret = sprintf("%s%s%02d:%02d%s%s", 
            hour_colors[bits[LT_HOUR]], prefix,
            bits[LT_HOUR], bits[LT_MIN],
            suffix, "%^RESET%^"
            );
    return ret;
}

string getChannelColor(string ch) {
    if(member_array(lower_case(ch), keys(channel_colors)) >= 0)
        // If we have a color defined for this channel, use it!
        return channel_colors[lower_case(ch)];
    else
        return channel_colors["default"];
}

varargs string getColorChannelName(string ch, string prefix, string suffix) {
    string the_color;

    if(undefinedp(prefix))
        prefix = "";
    if(undefinedp(suffix))
        suffix = "";
    the_color = getChannelColor(ch);

    return the_color + prefix + ch + suffix + "%^RESET%^";
}

// Sets a particular user to be a particular color.
int setSpeakerColor(string who, string color) {
    string shortwho;

    shortwho = lower_case(explode(who, "@")[0]);
    if(member_array(shortwho, keys(renamed_chatters)) >= 0) {
        shortwho = renamed_chatters[shortwho];
    }
    if(member_array(shortwho,keys(chatters)) >= 0) {
        chatters[shortwho] = color;
        unguarded((: save_object, SAVE_INTERMUD :));
        return 1;
    }
    return 0;
}

// Rename a chatter, putting their old name in a renamed mapping.
int renameChatter(string who, string to, int keep_source_color) {
    string shortwho;
    string shortto;

    shortwho = lower_case(explode(who, "@")[0]);
    shortto = lower_case(explode(to, "@")[0]);
    if (member_array(shortwho,keys(chatters)) < 0) {
        // If the source doesn't exist, there's nothing to do.
        return 0;
    }
    // Copy the source value to the destination and then nuke the source.
    renamed_chatters[shortwho] = shortto;
    if(keep_source_color)
        chatters[shortto] = chatters[shortwho];
    map_delete( chatters, shortwho);
    unguarded((: save_object, SAVE_INTERMUD :));
    return 1;
}

// Figure out what color to make this guy.
string getSpeakerColor(string who) {
    string color;
    string shortwho;

    shortwho = lower_case(explode(who, "@")[0]);
    if(member_array(shortwho, keys(renamed_chatters)) >= 0) {
        shortwho = renamed_chatters[shortwho];
    }
    if (member_array(shortwho,keys(chatters)) >= 0) {
        color = chatters[shortwho];
    } else {
        color = chat_colors[chat_counter % sizeof(chat_colors)];
        chatters[shortwho] = color;
        chat_counter++;
        unguarded((: save_object, SAVE_INTERMUD :));
    }
    return color;
}

// Show who is mapped to a particular color.
mapping mapSpeakerColors() {
    int i;
    mixed *k;
    mixed *v;
    mixed m;
    k = keys(chatters);
    v = values(chatters);
    m = ([]);

    for(i = 0; i < sizeof(k); i++) {
        m[v[i]] = undefinedp(m[v[i]]) ? ({ k[i] }) : m[v[i]] + ({ k[i] });
    }

    return m;
}

// Output a semi-pretty screen of colors mapped to speaker names.
varargs string showSpeakerColors(string who) {
    int i;
    mapping m;
    string s = "";
    mixed *k;
    int t = 0;
    int w;
    string color;
    string t_name = this_player()->query_term_name() || "dumb";
    mapping c_map = (mapping)TERM_HANDLER->set_term_type(t_name);

    if(!undefinedp(who)) {
        string shortwho = lower_case(explode(who, "@")[0]);
        if(member_array(shortwho, keys(renamed_chatters)) >= 0) {
            shortwho = renamed_chatters[shortwho];
        }
        if(member_array(shortwho,keys(chatters)) >= 0) {
            color = chatters[shortwho];
        }
    }
    //w= this_player()->GetScreen()[0] || 80;
    w = 80;
    m = mapSpeakerColors();
    k = sort_array(keys(m), 1);
    for(i = 0; i < sizeof(k); i++) {
        string nk;
        int c;
        string * lines;
        int j;

        if(!undefinedp(color)) {
            if(k[i] != color) {
                continue;
            }
        }
        nk = replace_string(k[i], "%^", "");
        c = sizeof(m[k[i]]);
        if(undefinedp(color)) {
            lines = explode( terminal_colour(implode(sort_array(m[k[i]], 1), ", "), c_map, w - 30, 0), "\n");
            for(j = 1; j < sizeof(lines); j++) {
                lines[j] = sprintf("%%^RESET%%^%29s%s%s", "", k[i], lines[j]);
            }
            s += sprintf("(%4d) %-20s: %s%s%s\n", c, nk, k[i], implode(lines, "\n"), "%^RESET%^");
        } else {
            s += sprintf("(%d) %s: %s%s%s\n", c, nk, k[i], implode(sort_array(m[k[i]], 1), ", "), "%^RESET%^");
        }

        t += c;
    }
    if(undefinedp(color)) {
        s += sprintf("(%4d) Total\n", t);
    }

    return s;
}

// Add the speaker's color to their name.
varargs string getColorSpeakerName(string speaker, string prefix, string suffix) {
    string the_color;

    if(undefinedp(prefix)) prefix = "";
    if(undefinedp(suffix)) suffix = "";
    the_color = getSpeakerColor(speaker);

    return the_color + prefix + speaker + suffix + "%^RESET%^";
}

// original code below here

string GetLocalChannel(string ch);
varargs void eventSendChannel(string who, string ch, string msg, int emote,
                              string target, string targmsg);

void eventReceiveChannelWhoReply(mixed *packet) {
  object ob;

  if (file_name(previous_object()) != INTERMUD_D) {
    return;
  }
  if (!(ob = find_player(packet[5]))) {
    return;
  }
  packet[6] = GetLocalChannel(packet[6]);
  if (!sizeof(packet[7])) {
    tell_object(ob, "No one is listening to " + packet[6] + " at " +
                packet[2] + ".\n");
    return;
  }
  tell_object(ob, "Listening to " + packet[6] + " at " + packet[2] + ": ");
  tell_object(ob, implode(packet[7], " ")+"\n");
}

int filter_listeners(object ob, string chan) {
  if (!ob->query_creator() || ob->query_invis() ||
      ob->check_earmuffs("intermud-all") ||
      ob->check_earmuffs(GetLocalChannel(chan))) {
    return 0;
  }
  return 1;
}

void eventReceiveChannelWhoRequest(mixed *packet) {
  string *who;

  if (file_name(previous_object()) != INTERMUD_D) {
    return;
  }
  if (GetLocalChannel(packet[6])) {
     who = (string *)filter_array(users(), "filter_listeners", this_object(),
                                  packet[6])->query_cap_name();
  } else {
     who = ({ });
  }
  INTERMUD_D->eventExternWrite(({ "chan-who-reply", 5, mud_name(), 0, packet[2],
                            packet[3], packet[6], who }));
}

void eventReceiveChannelUserRequest(mixed *packet) {
  object ob;
  string visname;
  int gender;

  if (file_name(previous_object()) != INTERMUD_D) {
    return;
  }
  if (!(ob = find_player(packet[6]))) {
    INTERMUD_D->eventExternWrite(({ "error", 5, mud_name(), 0, packet[2], 0,
                              "unk-user", packet[6] + " is not a valid "
                              "player.", packet }) );
    return;
  }
  visname = (string)ob->query_cap_name();
  switch ((string)ob->query_gender_string()) {
  case "male":
    gender = 0;
    break;
  case "female":
    gender = 1;
    break;
  default:
    gender = 2;
    break;
  }
  INTERMUD_D->eventExternWrite( ({ "chan-user-reply", 5, mud_name(), 0, 
                             packet[2], 0, packet[6], visname, gender }));
}

nosave mapping globals = ([]), files = ([]), ret = ([]);
#define CMD_NUM 1
#define TP globals[fd]
#define RET ret[fd]
#define CMDS cmds[this_player()]

void eventReceiveChannelMessage(mixed *packet) {
    object *people, *things;
    //mixed *urls;
    string url_regexp;
    //int i;
    string line;
    string match;
    string *matches;
    int fd;
    string *bits;

    if (file_name(previous_object()) != INTERMUD_D) {
        return;
    }

    // packet[0] == "channel_m"
    // packet[1] == 5
    // packet[2] == originating MUD name (MUD_NAME if local)
    // packet[3] == originating user name
    // packet[4] == 0
    // packet[5] == 0
    // packet[6] == channel name
    // packet[7] == visual name (pretty name) of user
    // packet[8] == message

    //if (packet[2] == mud_name() || !GetLocalChannel(packet[6])) {
    if (!GetLocalChannel(packet[6])) {
        return;
    }

    // get list of people who are listening to the channel
    people = filter(users(), (: $1->check_not_ignored($(packet[2])) && 
                $1->check_not_ignored($(packet[3])) && 
                $1->check_not_ignored($(packet[7])) && 
                $1->check_not_ignored($(packet[3]) + "@" + $(packet[2])) &&
                $1->check_not_ignored($(packet[7]) + "@" + $(packet[2]))
                :));

    // Filter out ANSI codes and BEEP?
    packet[8] = replace(packet[8], ({ sprintf("%c", 7), "!",
                sprintf("%c", 27), "ESC" }));

    // Send the event to listeners
    // event(listeners, "intermud_tell", speaker@mud, message, channel)
    event(people, "intermud_tell", sprintf("%s@%s", packet[7], packet[2]),
            packet[8], GetLocalChannel((string)packet[6]));

    // Scan the message to see if there's a URL in it.
    if( GetLocalChannel((string)packet[6]) != "url" ) {
        //url_regexp = "(https?\\:\\/\\/[\\w.-]+(?:\\.[\\w\\.-]+)+(?:\\:[\\d]+)?[\\w\\-\\.\\~\\:\\/\\?\\#[\\]\\@\\!\\$\\&\\'\\(\\)\\*\\+\\,\\;\\=\\%]+)";
        url_regexp = "(https?://[^ ]+?)(?:[ ]|$)";
        foreach( line in explode(packet[8], "\n") ) {
            matches = pcre_extract(line, url_regexp);
            foreach( match in matches ) {
                //event(people, "intermud_tell", sprintf("%s@%s", packet[7], packet[2]),
                //        sprintf("Found URL: %s", match), "DEBUG__" + GetLocalChannel((string)packet[6]));
                bits = ({ "wiley", match, GetLocalChannel((string)packet[6]), packet[3] });
                fd = external_start(CMD_NUM, bits, "read_call_back", "write_call_back", "close_call_back");
                if( fd < 0 ) {
                    event(people, "intermud_tell", sprintf("%s@%s", packet[7], packet[2]),
                            "Untiny failed to spawn.", "DEBUG__" + GetLocalChannel((string)packet[6]));
                } else {
                    TP = this_player();
                    RET = "";
                    //event(people, "intermud_tell", sprintf("%s@%s", packet[7], packet[2]),
                    //        sprintf("Spawning untiny %s on descriptor %d", implode(bits, " "), fd),
                    //        "DEBUG__" + GetLocalChannel((string)packet[6]));
                }
            }
        }
    }

    // Handle chat channel for mortals
    if ((string)packet[6] == "discworld-chat") {
        if ( strsrch( packet[8], "%^" ) != -1 ) {
           packet[8] = strip_colours( packet[8] );
        }

        things = children(TALKER);
        things -= ({ find_object(TALKER) });
        things->receive("intermud", sprintf("%s@%s", packet[7], packet[2]),
                packet[8]);
        HIST->add_chat_history( "intermud", sprintf( "%s wisped: ",
                    packet[7] + "@" + packet[2] ), packet[8] );   
    }
    /* 
       CHAT_D->eventSendChannel(packet[7] + "@" + packet[2], packet[6],
       packet[8]); */
}

void read_call_back(int fd, mixed mess) {
    RET += mess;
}

void write_call_back(int fd) {
    tell_object(TP, "untiny: The write_call_back() was called, somehow...\n");
}

void close_call_back(int fd) {
    string result;
    //mixed *debug_arr;

    if(RET != "") {
        result = RET;
        while( result != "" && ( result[<1..<1] == "\n" || result[<1..<1] == "\r" ) ) {
            result = result[0..<2];
        }
        //debug_arr = explode(result, "");
        //tell_object(TP, sprintf("%O", debug_arr));
        eventSendChannel("URLbot", "url", result);
    } else {
        tell_object(TP, "untiny: no result.\n");
    }
    map_delete(ret, fd);
    map_delete(globals, fd);
}

void old_eventReceiveChannelMessage(mixed *packet) {
  object *people, *things;

  if (file_name(previous_object()) != INTERMUD_D) {
    return;
  }
  if (packet[2] == mud_name() ||
      !GetLocalChannel(packet[6])) {
    return;
  }

  // If it is orion and they have colour, tell them to stop using it.
  if (lower_case(packet[7]) == "orion" &&
      strsrch(packet[8], "%^") != -1) {
     packet[8] = strip_colours(packet[8]);
  }

  // Perform any channel based filtering. ie: strip out all colours
  // on the DW chat line, but leave it open for others. 

  switch( packet[6] ) {
     case "discworld-chat":
     case "discworld-cre":
        if ( strsrch( packet[8], "%^" ) != -1 ) {
           packet[8] = strip_colours( packet[8] );
        }
        break;

     default: 
  }

  people = filter(users(), (: $1->check_not_ignored($(packet[2])) && 
                              $1->check_not_ignored($(packet[7])) && 
                              $1->check_not_ignored($(packet[7]) + "@" +
                                                    $(packet[2])) :));
  packet[8] = replace(packet[8], ({ sprintf("%c", 7), "!",
                                    sprintf("%c", 27), "ESC" }));
  event(people, "intermud_tell", sprintf("%s@%s: ", packet[7], packet[2]),
        packet[8], GetLocalChannel((string)packet[6]));
  if ((string)packet[6] == "discworld-chat") {
    things = children(TALKER);
    things -= ({ find_object(TALKER) });
    things->receive("intermud", sprintf("%s@%s", packet[7], packet[2]),
                    packet[8]);
    HIST->add_chat_history( "intermud", sprintf( "%s wisped: ",
        packet[7] + "@" + packet[2] ), packet[8] );   
  }
/* 
    CHAT_D->eventSendChannel(packet[7] + "@" + packet[2], packet[6],
           packet[8]); */
}
string clean_emote_string( string txt ) {
    int i = 0;

    txt = replace_string(txt, "$N", "");
    
    while ( txt[i] == ' ' ) {
        i++;
    }
    
    txt = txt[i..];

    return txt;
} /* clean_emote_string() */ 

void eventReceiveChannelEmote(mixed *packet) {
  object *people, *things;

  if (file_name(previous_object()) != INTERMUD_D) {
    return;
  }
  if (packet[2] == mud_name()) {
    return;
  }
  if (!packet[7]) {
    return;
  }

  // ( "taffyd", "%s: %O\n", ctime(time()), packet );


  switch( packet[6] ) {
     case "discworld-chat":
     case "discworld-cre":
        if ( strsrch( packet[8], "%^" ) != -1 ) {
           packet[8] = strip_colours( packet[8] );
        }
        break;
  }

  people = filter(users(), (: $1->check_not_ignored($(packet[2])) &&
                            $1->check_not_ignored($(packet[7])) &&
                            $1->check_not_ignored($(packet[7]) + "@" +
                                                  $(packet[2])) :));
  packet[8] = replace(packet[8], ({ sprintf("%c", 7), "!",
                                    sprintf("%c", 27), "ESC" }));
  event(people, "intermud_tell", "", 
        replace_string(packet[8], "$N", packet[7]+"@"+packet[2]), 
        GetLocalChannel((string)packet[6]));
  if ((string)packet[6] == "discworld-chat") {
    things = children(TALKER);
    things -= ({ find_object(TALKER) });
    things->receive("intermud", sprintf("%s@%s", packet[7], packet[2]),
                    ":" + clean_emote_string( packet[8] ) );
  }
/*    CHAT_D->eventSendChannel(packet[7] + "@" + packet[2], packet[6],
           packet[8], 1, 0, 0); */
}

varargs void eventSendChannel(string who, string ch, string msg, int emote,
                              string target, string targmsg) {
  mixed *packet;
  string targpl, where;

  if (emote) {
    if (target && targmsg) {
      if (sscanf(target, "%s@%s", targpl, where) != 2) {
        targpl = target;
      }
      where = mud_name();
    } else {
      targpl = where = targmsg = 0;
    }
    packet = ({ "channel-e", 5, mud_name(), lower_case(who), 0, 0, ch, 
                who, msg });
  } else {
    packet = ({ "channel-m", 5, mud_name(), lower_case(who), 0, 0, ch, 
                who, msg });
  }
  INTERMUD_D->eventExternWrite(packet);
}

void eventSendChannelWhoRequest(string channel, string mud) {
  string pl;

  pl = (string)this_player(1)->query_name();
  INTERMUD_D->eventExternWrite(({ "chan-who-req", 5, mud_name(), pl, mud, 0,
                            channel }));
}

string oldGetLocalChannel(string channel) {
  switch (channel) {
  case "imud_code":
    return "intercre";
  case "imud_gossip":
    return "intergossip";
  case "discworld-cre":
    return "dwcre";
  case "discworld-chat":
    return "dwchat";
  }
  return 0;
}

#include "creator.h"

string GetLocalChannel(string channel) {
    return allocate_mapping(values(INTERMUD_MAP), keys(INTERMUD_MAP))[channel] || 0;
}

void eventRegisterChannels(mapping list) {
  mixed *val;
  string channel, ns, local;

  if (file_name(previous_object()) != INTERMUD_D) {
    return;
  }
  ns = (string)INTERMUD_D->GetNameserver();
  foreach (channel, val in list) {
    if (!val) {
      continue;
    }
    
    local = GetLocalChannel(channel);
    //
    // Don't try registering on channels we don't care about anyway.
    //
    if (!local) {
      //INTERMUD_D->eventExternWrite(({ "channel-listen", 5, mud_name(), 0, ns, 
                                //0, channel, 0 }));
//      log_file("channels", "New channel: " + local + " recognized " +
//         ctime(time()) + "\nValue: " + sprintf("%O", val) + "\n\n");
    } else {
      INTERMUD_D->eventExternWrite(({ "channel-listen", 5, mud_name(), 0, ns,
                                0, channel, 1 }));
    }
  }
}

int eventAdministerChannel(string channel, string *additions, string *subs) {
  if (!((int)master()->high_programmer(geteuid(this_player(1))))) {
    return 0;
  }
  if (member_array(channel, (string *)INTERMUD_D->GetChannels()) == -1) {
    return 0;
  }
  INTERMUD_D->eventExternWrite(({ "channel-admin", 5, mud_name(),
                            (string)this_player(1)->query_cap_name(),
                            (string)INTERMUD_D->GetNameserver(),
                            0, channel, additions, subs }));
  return 1;
}

int AddChannel(string channel, int privee) {
  if (!((int)master()->high_programmer(geteuid(this_player(1))))) {
    return 0;
  }
  if (member_array(channel, (string *)INTERMUD_D->GetChannels()) != -1) {
    return 0;
  }
  INTERMUD_D->eventExternWrite(({ "channel-add", 5, mud_name(),
                            (string)this_player(1)->query_cap_name(),
                            (string)INTERMUD_D->GetNameserver(), 0,
                            channel, privee }));
  return 1;
}

int RemoveChannel(string channel) {
  if (!((int)master()->high_programmer(geteuid(this_player(1))))) {
    return 0;
  }
  if (member_array(channel, (string *)INTERMUD_D->GetChannels()) == -1) {
    return 0;
  }
  INTERMUD_D->eventExternWrite(({ "channel-remove", 5, mud_name(),
                            (string)this_player(1)->query_cap_name(),
                            (string)INTERMUD_D->GetNameserver(), 0,
                            channel }));
  return 1;
}
