/*    /daemon/services/error.c
 *    from the Foundation II LPC Library
 *    error handling for the grand Intermud 3 Protocol
 *    created by Descartes of Borg 950715
 *    Modified for Discworld by Turrican 1-11-95
 */

void eventReceiveError(mixed *packet) {
    object ob;
    string error_code, mud, target, msg;

    if( packet[5] ) {
	target = (string)packet[5];
	if( !(ob = find_player(target)) ) return;
    }
    mud = packet[2];
    error_code = packet[6];
    msg = packet[7];
    packet = packet[8];
    switch(error_code) {
	case "unk-dst": case "not-imp": case "unk-src": case "bad-pkt": case "bad-proto":
	log_file("errors/intermud", error_code + ": " + msg + "\n");
        log_file("errors/intermud", "Name of sending mud: "+mud+"\n");
        if (!packet) return;
        log_file("errors/intermud", "Offending packet: "+
            sprintf("%O\n", packet));
	return;
        case "unk-channel":
        log_file("errors/intermud", error_code + ": " + msg + "\n");
        if (!packet) return;
        log_file("errors/intermud", "Channel name: "+packet[6]+"\n");
	case "unk-type":
    	log_file("errors/intermud", 
            error_code + ": " + msg + "\n");
        log_file("errors/intermud", 
            "Name of sending mud: "+mud+"\n");
        if (!packet) return;
        log_file( "errors/intermud", 
            "Type of offending packet: "+packet[0]+"\n");
	return;
	case "unk-user":
	if( !ob ) return;
	tell_object(ob, (msg ? msg : "Unknown user reported from " + mud +
			   "."));
	return;
    }
}
