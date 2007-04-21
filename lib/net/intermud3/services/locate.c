/*    /daemon/services/locate.c
 *    from the Nightmare IV LPC Library
 *    performs player lookups
 *    created by Descartes of Borg 950624
 *    Modified for Discworld by Turrican 1-11-95
 *    Modified to implement the new protocol by Turrican 11-1-96
 */

#define SERVICE_LOCATE

void eventReceiveLocateRequest(mixed *packet) {
    object ob;
    string status;

    if( file_name(previous_object()) != INTERMUD_D ) return;
    if( !(ob = find_player(packet[6])) || (int)ob->query_invis() ) return;
    if ( ob->query_in_editor() )
        status = "editing";
    if ( !interactive( ob ) )
        status = "link-dead";
    if ( query_idle(ob) > 5*60 ) {
        if ( status )
            status += ", inactive";
        else
            status = "inactive";
    }
    INTERMUD_D->eventExternWrite( ({ "locate-reply", 5, mud_name(), 0, packet[2], 
			       packet[3], mud_name(), 
			       (string)ob->query_cap_name(),
                               query_idle(ob),
                               status }) );
}

void eventReceiveLocateReply(mixed *packet) {
  object ob;

  if( file_name(previous_object()) != INTERMUD_D ) return;
  if( !stringp(packet[5]) || !(ob = find_player(lower_case(packet[5]))) ) 
    return;
  tell_object(ob, packet[7] + " was just located on " + packet[6] + ".\n");
  if ( sizeof(packet) > 8 ) {
    if (packet[8])
      tell_object(ob, packet[7]+" has been idle for "+packet[8]+" seconds.\n");
    if (packet[9] && packet[9] != "")
      tell_object(ob, packet[7]+" is " + packet[9] + ".\n");
  }
}

void eventSendLocateRequest(string who) {
    string pl;

    if( !(pl = (string)this_player(1)->query_name()) ) return;
    INTERMUD_D->eventExternWrite( ({ "locate-req", 5, mud_name(), pl, 0, 0, who }) );
}
