/*    /daemon/services/tell.c
 *    from the Nightmare IV LPC Library
 *    handles the IMP tell service
 *    created by Descartes of Borg 950507
 *    Modified for Discworld by Turrican 1-11-95
 */

#define SERVICE_TELL

void eventReceiveTell(mixed *packet) {
    object ob;
    string who;
    string from;

    if( file_name(previous_object()) != INTERMUD_D ) return;

    who = lower_case(packet[5]);
    from = packet[6]+"@"+packet[2];

    if( !(ob = find_player(who)) || ( (int)ob->query_invis() && 
        member_array( lower_case( from ), ob->query_allowed() ) == -1 ) ) {
        INTERMUD_D->eventExternWrite(({ "error", 5, mud_name(), 0, packet[2],
                      packet[3], "unk-user", 
                      capitalize(packet[5]) + " is nowhere to "
                        "be found on " + mud_name() + ".",
                      packet }));
    	return;
    }
    if (!ob->query_creator()) {
        INTERMUD_D->eventExternWrite(({ "error", 5, mud_name(), 0, packet[2],
                                  packet[3], "unk-user",
                                  capitalize(packet[5]) + " is a player "
                                  " and cannot be the target of intermud "
                                  "tell s on " + mud_name() + ".",
                                  packet }));
        return;
    }
    ob->event_person_tell(this_object(), from +
                           " tells you: ", packet[7], "common");
    /*ob->SetProperty("reply", packet[6] + "@" + packet[2]); */
} /* eventReceiveTell() */

void eventSendTell(string who, string where, string msg) {
    string pl, plc;
    
    pl = (string)this_player(1)->query_name();
    plc = (string)this_player(1)->query_cap_name();
    where = (string)INTERMUD_D->GetMudName(where);
    INTERMUD_D->eventExternWrite(({ "tell", 5, mud_name(), pl, where, 
			      lower_case(who), plc, msg }));
} /* eventSendTell() */
