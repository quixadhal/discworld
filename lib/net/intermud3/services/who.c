/*    /daemon/services/who.c
 *    from the Nightmare IV LPC Library
 *    handles the Intermud 3 who service
 *    created by Descartes of Borg 950623
 *    Modified for Discworld by Turrican 1-11-95
 */

#define SERVICE_WHO

int othermud_is_alive;
#define OTHER_MUD "WileyMUD"
//#define OTHER_MUD "Discworld"

void eventReceiveWhoReply(mixed *packet) {
  string *list;
  mixed *who;
  object ob;

  if( file_name(previous_object()) != INTERMUD_D ) return;
  if( !packet[5] ) return;
  if( packet[5] == "URLbot" ) {
      // This means intermud is alive, so no need to nuke our parent.
      if( !othermud_is_alive ) {
          event(users(), "intermud_tell", "URLbot@Disk World", sprintf("%%^GREEN%%^OMG!%%^RESET%%^  %%^YELLOW%%^%s%%^RESET%%^ replied!", OTHER_MUD), "bot");
      }
      othermud_is_alive = 1;
      return;
  } else {
      //event(users(), "intermud_tell", "URLbot@Disk World", "We got a reply for \"" + packet[5] + "\".", "bot");
  }

  if( !(ob = find_player(packet[5])) ) return;
  list = ({ "Remote who information from " + packet[2] + ":" });
  foreach(who in packet[6]) 
    if (who[1] > 0)
      list += ({ who[0] + " (Idle for " + who[1] + " second"+
                   (who[1]>1?"s":"")+"): " + who[2] });
    else
      list += ({ who[0] + " (Not Idle): " + who[2] });
  ob->more_string(implode(list, "\n")+"\n", "Who");
}

string query_status(object ob) {
  switch((string)ob->query_object_type()) {
  case " ":
    return "Player";
  case "C":
    return "Creator";
  case "p":
    return "Playtester";
  case "S":
    return "Senior Creator";
  case "L":
    return "Lord";
  case "X":
    return "Logging in";
  case "H":
    return "High Lord";
  default:
    return "Unknown";
  }
}

void eventReceiveWhoRequest(mixed *packet) {
    mixed *msg;

    if( file_name(previous_object()) != INTERMUD_D ) return;
    msg = map(filter(users(), (: !((int)$1->query_invis()) :)),
        (: ({ (string)$1->query_cap_name(), query_idle($1), 
                    query_status($1) }) :));
    INTERMUD_D->eventExternWrite(({ "who-reply", 5, mud_name(), 0, packet[2],
            packet[3], msg }));
}

void eventSendWhoRequest(string mud, string who) {
    INTERMUD_D->eventExternWrite(({ "who-req", 5, mud_name(), who, mud, 0 }));
}

void kick_othermud();

void reap_the_intermud() {
    if( othermud_is_alive == 1 ) {
        kick_othermud();
    } else {
        event(users(), "intermud_tell", "URLbot@Disk World", sprintf("%%^RED%%^Oh SHIT!%%^RESET%%^  I think the intermud is DEAD... we're all %%^RED%%^DOOOOOMED!!!%%^RESET%%^"), "bot");
        reload_object(find_object(INTERMUD_D));
    }

}

void kick_othermud() {
    othermud_is_alive = 0;
    remove_call_out( "reap_the_intermud" );
    call_out( (: reap_the_intermud :), 30 * 60);
    event(users(), "intermud_tell", "URLbot@Disk World", sprintf("Poking %%^YELLOW%%^%s%%^RESET%%^ with a stick...", OTHER_MUD), "bot");
    eventSendWhoRequest(OTHER_MUD, "URLbot");
}

