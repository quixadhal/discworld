/*
    Warrior's Guild Blackboard
    For The Guildmaster's Shop
    
    Suggested by Lanfear.
    Wrytten by Drakkos Thee Creator
    16/06/2000
*/

#include <language.h>

#define DEFAULT_LANGUAGE "morporkian"
#define BOARD_TAG "blackboard"

inherit "/std/room/furniture/commercial";

int do_write( string language );

void setup() {
    set_name ("blackboard");
    set_short ("blackboard");
    add_alias (({"board", "sign"}));
    set_long ("The residue of faded chalk-dust and long forgotten "
        "doodles lies heavily on the blackboard.  The grain of the "
        "board is worn and slightly mottled with age, hinting at "
        "a long and dignified history of classrooms and university "
        "lecterns.  Or perhaps just hinting at cheap wood and "
        "bad upkeep.\n");
    set_allowed_room_verbs( (["standing" : "stands",
                            "hanging"  : "hangs"]) );
    set_commercial_size(1);
    set_weight(2000);
    set_value(240000);
}

void init() {
    add_command ("write", "[on] <direct:object'blackboard'> in <string'language'>", 
        (: do_write($4[1]) :) );
    add_command ("clean", "<direct:object>");
}    

string query_readable_message(object player, int ignore_labels) {
    if (!sizeof(query_read_mess())) {
        return "A few random letters that escaped cleaning can be seen on "
            "the blackboard";
    }
    
    return ::query_readable_message(player, ignore_labels);    
} /* query_readable_message() */ 


int do_clean() {
    if (!is_allowed (this_player()->query_name())) {
        this_player()->add_failed_mess (this_object(), "You cannot clean " 
            +  capitalize (query_owner()) + "'s blackboard!\n", ({ }));
        return 0;
    }
    
    /* Clear the read mess */ 
    set_read_mess( 0 );
    
    return 1;
}

string find_words( string language ) {
    string words;
    
    words = implode( map( filter( query_read_mess(), 
        (: $1[READ_LANG] == $(language) :) ), (: $1[READ_MESS] :) ), "\n" );
    
    return words;
} /* find_words() */

int do_write(string language)
{
    string existing_text;
    
    if (!is_allowed (this_player()->query_name())) {
        this_player()->add_failed_mess (this_object(), "You cannot write on " 
            +  capitalize (query_owner()) + "'s blackboard!\n", ({ }));
        return 0;
    }
    
    language = lower_case( language ); 
    
    if ( !this_player()->query_skill(
        LANGUAGE_HAND->query_language_written_skill(language)) ) {
        this_player()->add_failed_mess(this_object(), "You can't write in " +
           language + ".\n" );
        return -1;
    }
        
    /* Find all the existing words in the language. */ 
    
    existing_text = find_words( language ); 
                    
    this_player()->do_edit(existing_text, "blackboard_text", 
        this_object(), 0, language);
    
    this_player()->add_succeeded_mess (this_object(), "$N find$s a handy "
        "piece of chalk and start$s to write on the blackboard.\n", ({ }));
    return 1;

}

void blackboard_text(string text, string language)
{

    if (!text)
    {
        tell_room (environment (this_object()), this_player()->one_short()
            + " steps back from the blackboard without making any "
            "changes.\n", this_player());
        tell_object (this_player(), "Okay, nothing has been changed.\n");
        return;
    }
    
    /* Remove the existing text. */ 
    
    set_read_mess( filter( query_read_mess(), 
        (: $1[READ_LANG] != $(language) :) ) );
    
    add_read_mess( text, "chalk", language ); 
    
    tell_room(environment (this_object()), this_player()->one_short() 
            + " finishes writing on the blackboard.\n", this_player());
}

void init_dynamic_arg(mapping map) {
   string my_words;
   
   commercial::init_dynamic_arg(map, player);
   
   my_words = query_auto_load_value(map, BOARD_TAG, "words");
   
   if ( stringp(my_words) ) {
       add_read_mess(my_words, "chalk", DEFAULT_LANGUAGE);
   }
     set_read_mess( filter( query_read_mess(), (: $1[READ_LANG] != "common" :) ) );
}
