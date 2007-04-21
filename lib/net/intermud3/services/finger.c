/*    /daemon/services/finger.c
 *    from the Nightmare IV LPC Library
 *    Intermud 3 finger service implementation
 *    created by Descartes of Borg 950624
 *    Modified for Discworld by Turrican 1-11-95
 */

#define SERVICE_FINGER

private mapping _finger_ids = ([ ]); 

void eventReceiveFingerRequest(mixed *packet) {
    mixed *ret;
    object demon;

    if( !(ret = 
        (mixed *)"/secure/finger"->remote_finger(lower_case(packet[6]))) ) {
        INTERMUD_D->eventExternWrite(({ "error", 5, mud_name(), 0, packet[2],
                                  packet[3], "unk-user",
                                  "No one going by the name of "+
                                  capitalize(packet[6]) + " has ever visited "+
                                  "Discworld.",
				  packet }));
	return;
    }
    demon = clone_object("/net/daemon/chars/in_finger_demon");
    demon->setup_finger(lower_case(packet[6]));
    ret = ({ "finger-reply", 5, mud_name(), 0, packet[2], packet[3] }) + ret;
    INTERMUD_D->eventExternWrite(ret);
}

void eventReceiveFingerReply(mixed *packet) {
    object ob;
    string fing;
    
    if( file_name(previous_object()) != INTERMUD_D ) return;
    
    if( !( ob = find_player( _finger_ids[ lower_case(packet[5]) ] ) ) ) 
        return;

    map_delete( _finger_ids, lower_case( packet[5] ) ); 

    fing = "Finger information on " + packet[6] + " from " + packet[2] + ":\n";
    if( packet[7] && packet[8] ) fing += packet[7] + " (" + packet[8] + ")\n";
    else if( packet[8] ) fing += packet[6] + " (" + packet[8] + ")\n";
    else if( packet[7] ) fing += packet[7] + "\n";
    else fing += packet[6] + "\n";
    fing += "Email: " + (packet[9] ? packet[9] : "Confidential") + "\n";
    fing += ((packet[11] != -1) ? "On since: " + packet[10]  : 
	     "Last logged in: " + packet[10]);
    if( packet[11] != -1 ) fing += " (idle " + packet[11] + " seconds)\n";
    else fing += "\n";
    fing += "Site: " + (packet[12] ? packet[12] : "Confidential") + "\n";
    fing += (packet[14] ? packet[14] : "");
    tell_object(ob, fing);
}

void eventSendFingerRequest(string who, string where) {
    string pl;
    string crname; 
    
    // This is the person who sent the request. 
    if( !(pl = (string)this_player(1)->query_name()) ) return;
    
    crname = lower_case( crypt( pl, 0 ) ); 

    _finger_ids[ crname ] = pl; 

    INTERMUD_D->eventExternWrite( ({ "finger-req", 5, mud_name(), crname, where, 0, 
			       who }) );
}
