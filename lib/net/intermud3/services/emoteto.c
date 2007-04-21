/*    /daemon/services/emoteto.c
 *    from the Foundation II Object Library
 *    handles the I3 emoteto service
 *    created by Descartes of Borg 950914
 *    Modified for Discworld by Turrican 1-11-95
 */

#define SERVICE_EMOTETO

void eventReceiveEmote(mixed *packet) {
    object ob;
    string who;

    if( file_name(previous_object()) != INTERMUD_D ) return;
    who = lower_case(packet[5]);
    if( !(ob = find_player(who)) || (int)ob->query_invis() ) {
        INTERMUD_D->eventExternWrite(({ "error", 5, mud_name(), 0, packet[2],
            packet[3], "unk-user", 
            capitalize(packet[5]) + " is nowhere to "
            "be found on " + mud_name() + ".\n",
            packet }));
        return;
    }

    if (!ob->query_creator()) {
        INTERMUD_D->eventExternWrite(({ "error", 5, mud_name(), 0, packet[2],
                                  packet[3], "unk-user",
                                  capitalize(packet[5]) + " is a player "
                                  " and cannot be the target of intermud "
                                  "emotes on " + mud_name() + ".",
                                  packet }));
        return;
    }

    packet[7] = replace_string(packet[7], "$N", packet[6] + "@" + packet[2]);
    ob->event_emote(this_object(), "%^MAGENTA%^"+ packet[7]+ "%^RESET%^\n");
}

void eventSendEmote(string who, string where, string msg) {
    string pl, plc;

    pl = (string)this_player(1)->query_name();
    plc = (string)this_player(1)->query_cap_name();
    where = (string)INTERMUD_D->GetMudName(where);

    if ( !where ) { 
        return;
    }

    INTERMUD_D->eventExternWrite(({ "emoteto", 5, mud_name(), pl, where, 
        lower_case(who), plc, "$N "+msg }));
}
