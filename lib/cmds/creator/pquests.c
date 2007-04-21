#include <quest_handler.h>
#include <library.h>

inherit "/cmds/base";

string quest_text(string quest);

int cmd( string player, int sorted ) {
    string *quests, text;
    int i, size;
    
    if ( !player ) {
        quests = QUEST_HANDLER->query_quest_names();
        text = "$P$Quest list$P$\nTotal quests on " + mud_name() + " :-\n\n";
    }
    else {
        quests = LIBRARY->query_quests(player);    
        
        if (!quests) {
            add_failed_mess( "That player has not done any "
                "quests.\n" );
            return 0;
        }
        
    if ( sorted ) 
        quests = sort_array( quests, 1 );

        text = "$P$Quest list$P$\nQuests done for player " + player + " :-\n\n";
    }
    size = sizeof(quests);

    for ( i = 0; i < size; i++ ) {
        text += sprintf("%d. %s (%s) %s\n", i + 1, quests[i], 
            QUEST_HANDLER->query_quest_title (quests[i]), 
            quest_text( quests[i] ) );
    }
    
    tell_object( this_player(), text );
    return 1;
} /* cmd() */

string quest_text(string quest) {
    switch ( QUEST_HANDLER->query_quest_status(quest) ) {
        case 0:
            return "(inactive)";
            
        case 1:
            return "";
        
        default:
            return "(broken)";
    }
} /* quest_text() */

mixed *query_patterns() {
    return ({ "", (: cmd(0, 0) :),
        "<string'player'>", (: cmd($4[0], 0) :),
        "<string'player'> sorted", (: cmd($4[0], 1) :) 
    });
} /* query_patterns() */

