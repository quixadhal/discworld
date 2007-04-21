/**
 * Addevent - adds entries to the player info database.
 */
#include <creator.h>
#include <playerinfo.h>
#include <player_handler.h>

inherit "/cmds/base";

// Update the database with the data the user has entered
void update_database(string comment, string *data) {
    if (comment == 0) {
       return;
    }

    if (PLAYERINFO_HANDLER->add_entry(this_player(),
                                      data[0],
                                      data[1],
                                      comment+"\n")) {
        write(capitalize(data[0])+"'s dossier updated.  \n");
    } else {
        write("Cannot update "+capitalize(data[0])+"'s dossier.  \n");
    }
} /* update_database() */

// Command entry point
int add_event(string player, string event, string comment) {
    string *events;
    string myself;

    player = this_player()->expand_nickname(lower_case(player));
    event = lower_case(event);

    if (!PLAYER_HANDLER->test_user(player)) {
        add_failed_mess("No such player: " + player + ".\n");
        return 0;
    }

    myself = base_name(this_object());
    events = filter(PLAYERINFO_HANDLER->query_events(),
                (: PLAYERINFO_HANDLER->query_source_ok($1, $(myself)) :));
    if (member_array(event, events) == -1) {
       add_failed_mess("Incorrect event type '" + event +
                       "' it must be one of " +
                       query_multiple_short(events) + ".\n");
       return 0;
    }

    if (comment != 0) {
        update_database(comment, ({ player, event }));
        return 1;
    }
    write("Please enter your comments:\n");
    this_player()->do_edit(0, "update_database", this_object(), 0,
                           ({ player, event }));
    return 1;
} /* add_event() */

// Yet Anothr Command Entry Point (YACEP)
int list_events() {
    string result;
    string myself;
    string *list;

    result = "";
    myself = base_name(this_object());
    list = sort_array(
            filter(PLAYERINFO_HANDLER->query_events(),
                (: PLAYERINFO_HANDLER->query_source_ok($1, $(myself)) :)), 1);


    write("The following events can be added using addevent: " +
          query_multiple_short(list) + ". \n");
    return 1;
} /* list_events() */

// Answer the command patterns
mixed *query_patterns()
{
    return ({
        "list",
        (: list_events() :),
        "<word'player-name'> <word'event'>",
        (: add_event($4[0],$4[1],0) :),
        "<word'player-name'> <word'event'> <string'comment'>",
        (: add_event($4[0],$4[1],$4[2]) :)
    });
}
