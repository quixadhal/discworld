#include <network.h>

inherit "/cmds/base";

int cmd(string person, string text) {
    string target, mud;
 
    if (sscanf(person, "%s@%s", target, mud) != 2) {
        return 0;
    }

    if ( !strlen( target ) || !strlen( mud ) ) {
        return 0;
    }
    SERVICES_D->eventSendEmote(target, mud, text);
    add_succeeded_mess( ({ sprintf("You emote to %s@%s: %s@%s %s\n",
        capitalize(target), mud, this_player()->query_cap_name(),
        mud_name(), text), "" }) );
    return 1;
} /* cmd() */

mixed *query_patterns() {
    return ({ "<string'person'> <string'message'>",
              (: cmd($4[0], $4[1]) :) });
} /* query_patterns() */
