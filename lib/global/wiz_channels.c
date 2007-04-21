/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: wiz_channels.c,v 1.6 2003/01/30 18:37:02 taffyd Exp $
 *
 * System of intra-mud communications channels.
 * Access to channels is controlled by the noshadow channel_list() function.
 * By default each domain has a channel. Domains not wishing to have a channel
 * should be placed in the NO_CHANNELS array.
 * THE ADDITIONAL_CHANNELS array adds channels that do not have a domain and
 * are accessible to all creators.
 *
 * This has been hacked together from a bunch of things it can probably be
 * simplified considerably.
 */
#include <broadcaster.h>
#include <creator.h>
#include <network.h>
#include <newudp.h>

#define TALKER "/std/shadows/object/talker"

#define BEEP sprintf("%c",7)
#define CHAN_OK 1
#define CHAN_FORCED 2
#define CHAN_EMOTED 4
#define CHAN_QUERY 8

private int channel_cmd(string mess);
private int channel_hist();

nomask string *channel_list() {
  string channel, *tmp, *channels;
  
  tmp = "/secure/master"->query_domains();
  tmp -= NO_CHANNELS;
  channels = ({ });
  
  foreach(channel in tmp)
    if("/secure/master"->query_senior(this_object()->query_name()) ||
       ("/d/"+channel+"/master")->query_member(this_object()->query_name()))
      channels += ({ channel });

  channels += ADDITIONAL_CHANNELS + keys(INTERMUD_MAP);
  
  if(this_object()->query_lord())
    channels += ({ "lord" });
  
  return channels;
}

void wiz_commands() {
  string channel;

  foreach(channel in channel_list()) {
    add_command(channel, "<string'mess'>", (: channel_cmd($4[0]) :));
    add_command(channel+"@", "<string'mess'>", (: channel_cmd("@"+$4[0]) :));
    add_command(channel+"?", "", (: channel_cmd("?") :));

    if(!INTERMUD_MAP[channel]) {
      add_command(channel+"!", "", (: channel_cmd("!") :));
      add_command("h"+channel, "", (: channel_hist() :));
    }
  }
}

/* to properly columnate word_typed things */
private void my_mess(string fish, string erk) {
  int bing;
  
  if (!interactive(this_player())) {
    return;
  }
  bing = strlen(fish);
  if (bing > 15) {
    bing = 15;
  }
  efun::tell_object(this_player(),
                    this_player()->fix_string(sprintf("%s%s\n", fish, erk),
                                              (int)this_player()->query_cols(),
                                              bing));
}

/**
 * This method handles the command line and returns the type of command that
 * is represented by the command line.
 * @param mess the message to parse
 * @return the flags to determine what sort of message it is
 */
protected int handle_command_line(string channel, string mess) {
  int retval;
  function map_func;
  int i;
  object *people, *off_line;

  add_failed_mess("Syntax: "+channel+"[@|?] [text]\n");
  if(!channel || !mess) {
    return 0;
  }

  if(member_array(channel, channel_list()) == -1) {
    add_failed_mess("No such channel " + channel + ".\n");
    return 0;
  }
  
  switch (mess[0]) {
  case '!' :
    retval |= CHAN_FORCED;
    break;
  case '@' :
    retval |= CHAN_EMOTED;
    break;
  case '?' :
    if (mess == "?") {
      // Handle intermud queries.
      if(INTERMUD_MAP[channel]) {
        SERVICES_D->eventSendChannelWhoRequest(INTERMUD_MAP[channel]);
        retval |= CHAN_QUERY;
        return retval;
      }
      
      people = filter(users(), (: $1 && $1->query_creator() &&
                                $1->query_visible(this_object()) :));
      off_line = ({ });
      for (i=0;i<sizeof(people);i++) {
        if(!people[i] || !people[i]->channel_list() ||
           member_array(channel, people[i]->channel_list()) == -1) {
          people = delete(people, i, 1);
          i--;
        } else if(people[i]->check_earmuffs(channel) ||
                  (member_array(channel, keys(INTERMUD_MAP)) != -1 &&
                   people[i]->check_earmuffs("intermud-all"))) {
          off_line += ({ people[i] });
          people = delete(people, i, 1);
          i--;
        }
      }
      people = sort_array(people, (: strcmp($1->query_name(),
                                            $2->query_name()) :) );
      off_line = sort_array(off_line, (: strcmp($1->query_name(),
                                                $2->query_name()) :) );
      map_func = function (object ob) {
        string str;
        
        if (ob->query_invis()) {
          if (ob->query_invis() == 2) {
            str = "({" + $1->query_cap_name() + "})";
          } else {
            str = "(" + $1->query_cap_name() + ")";
          }
        } else {
          str = $1->query_cap_name();
        }
        if (query_idle(ob) > 120) {
          int hours, mins, secs;
          
          secs = query_idle(ob);
          mins = secs / 60;
          secs %= 60;
          hours = mins / 60;
          mins %= 60;
          if ( !hours )  {
            str += sprintf(" [%02dm]", mins);
          } else {
            str += sprintf(" [%02dh:%02dm]", hours, mins);
          }
        }
        return str;
      };
      people = map(people, map_func);
      off_line = map(off_line, map_func);
      if (sizeof(people)) {
        write(sprintf( "On line: %-=" + (this_player()->query_cols() - 10) +
                       "s", query_multiple_short( people ) +".\n"));
      }
      if (sizeof(off_line)) {
        write(sprintf( "Off line: %-=" + (this_player()->query_cols() - 11) +
                       "s", query_multiple_short( off_line ) +".\n"));
      }
      retval |= CHAN_QUERY;
    }
    break;
  default :
    return CHAN_OK;
    break;
  }
  return retval;
}

private int channel_cmd(string mess) {
  int flags;
  string pad = " ";
  string start;
  string channel;
  object *things;
  
  channel = query_verb();
  
  if(channel[<1] == '?' || channel[<1] == '@' || channel[1] == ':') {
    channel = channel[0..<2];
  }
  flags = handle_command_line(channel, mess);
  if (!flags) {
    return 0;
  }
  if (flags & CHAN_QUERY) {
    return 1;
  }
  
  if(mess[0] == '!' || mess[0] == '@' || mess[0] == '?') {
    mess = mess[1..1000];
  }
  if (mess[0] == ' ') {
    mess = mess[1..1000];
  } 
  if (mess == "") {
    return 0;
  }
  
  if (this_object()->check_earmuffs(channel)) {
    write("Why use " + channel + "-tell when you can't hear a response?\n");
    return 1;
  }
  mess = replace(mess, BEEP, "");
  if (mess[0..0] == "'") {
    pad = "";
  }
  
  if (flags & CHAN_EMOTED) {
    start = this_object()->query_cap_name() + pad;
    
    if(INTERMUD_MAP[channel])
      SERVICES_D->eventSendChannel((string)this_player()->query_cap_name(),
                                   INTERMUD_MAP[channel], "$N " + mess,
                                   flags & CHAN_EMOTED);
    else
      my_mess("(" + channel + ") "+
              ((string)this_object()->query_cap_name()) + pad,
              mess);
  } else {
    start = this_object()->query_cap_name() +": ";
    
    if(INTERMUD_MAP[channel])
      SERVICES_D->eventSendChannel((string)this_player()->query_cap_name(),
                                   INTERMUD_MAP[channel], mess,
                                   flags & CHAN_EMOTED);
    else
      my_mess("You " + channel + "-tell: ", mess);
  }
  
  if(INTERMUD_MAP[channel]) {
    user_event(this_object(), "intermud_tell", start, mess, channel);
    
    if(channel == "dwchat") {
      things = children(TALKER);
      things -= ({ find_object(TALKER) });
      if ( flags & CHAN_EMOTED ) { 
        mess = ":" + mess;
      }

      things->receive("intermud", this_object()->query_cap_name(),
                       mess);
    }
  } else {
    user_event(this_object(), "creator_tell", start, mess,
                flags & CHAN_FORCED, channel);
    HIST_HANDLER->add_chat_history(channel, start, mess);
  }
  
  return 1;
}

private int channel_hist() {
  int i;
  mixed *hist;
  string channel;
  
  channel = query_verb()[1..];
  
  hist = HIST_HANDLER->query_chat_history(channel);
  if (!pointerp(hist) || !sizeof(hist)) {
    return notify_fail("Nobody said anything on the " + channel +
                       " channel.\n");
  }
  write("The " + channel + " channel history is:\n");
  for (i=0;i<sizeof(hist);i++) {
    if (sizeof(hist[i]) > 2) {
      efun::tell_object(this_object(), 
                        this_object()->fix_string(sprintf("*%s* %s%-=*s\n",
                ctime(hist[i][2])[11..18], hist[i][0], 
                (int)this_object()->query_cols()-strlen(hist[i][0])-11,
                                                          hist[i][1])));
    } else {
      efun::tell_object(this_object(), 
                        (string)this_object()->fix_string(sprintf("%s%-=*s\n",
               hist[i][0], (int)this_object()->query_cols()-strlen(hist[i][0]),
                hist[i][1])));
    }
  }
  return 1;
}
