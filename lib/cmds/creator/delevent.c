/**
 * Delevent - deletes an entry from the player info database.
 */
#include <creator.h>
#include <playerinfo.h>
#include <player_handler.h>

inherit "/cmds/base";

// Command entry point
int delete_event(string s1, string s2, int n)
{
    string  who = lower_case(s1);
    string  event = lower_case(s2);

    if(!PLAYER_HANDLER->test_user(who)) {
        write("No such player: "+who+".  \n");
        return 1;
    }

    if(PLAYERINFO_HANDLER->delete_entry(this_player(),who,event,n))
        write("Entry #" + n + " of " + capitalize(who) +
            "'s dossier deleted.  \n");
    else
        write("Cannot remove entry #" + n + " from " +
            capitalize(who) + "'s dossier.  \n");
    return 1;
}

// Answer the command patterns
mixed *query_patterns()
{
    return ({
        "<word'player-name'> <word'event'> <number'entry-number'>",
        (: delete_event($4[0],$4[1],$4[2]) :)
    });
}
