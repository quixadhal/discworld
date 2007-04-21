/**
 * Newbie Helpers Handler
 *
 * @author Dasquian
 * @started on 26 March 2002
 *
 */
 
#include <playerinfo.h>
#include <player_handler.h>
#include <mail.h>

#define PI_TYPE "misc"
#define SAVE_FILE "/save/newbiehelpers"
#define TO this_object()
#define TP this_player()

private mapping admin;
private mapping helpers;
private mapping applicants;
private int minimum_apply_age, minimum_vouch_age;

protected void log_message( string message );
protected void load_file();
protected void save_file();

void create() {

    admin = ([ ]);
    helpers = ([ ]);
    applicants = ([ ]);
    minimum_apply_age = 0;
    minimum_vouch_age = 0;
    
    load_file();

} /* create() */


string query_list( object lister ) {
    string list_mess, person;

    if( !sizeof( admin ) ) {
        list_mess = "There are no newbie helpers admin at present.\n";
    }
    else {
        list_mess = "Current newbie helpers admin:\n";
        foreach( person in keys(admin) ) {
            list_mess += "    $C$" + person + "\n";   
        }
    }
    
    list_mess += "\n";

    if( !sizeof( helpers ) ) {
        list_mess += "There are no newbie helpers at present.\n";
    }
    else {
        list_mess += "Current newbie helpers:\n";
        foreach( person in keys(helpers) ) {
            list_mess += "    $C$" + person + "\n";   
        }
    }
    
    // Only admin can see who is applying
    if( admin[lister->query_name()] ) {
    
        list_mess += "\n";
    
        if( !sizeof( applicants ) ) {
            list_mess += "There are no outstanding newbie helper "
                "applications at present.\n";
        }
        else {
            list_mess += "Current applications to newbie helpers:\n";
            foreach( person in keys(applicants) ) {
                list_mess += "    $C$" + person + "\n";   
            }
        }    
    
    }
    
    return list_mess;
    
} /* do_list() */


private string helper_info( string name, object tp ) {
    string helper_mess, *vouches;

    helper_mess = "Details for $C$" + name + ":\n";
    
    helper_mess += "    Hired by $C$" + helpers[name]["employer"] + 
        ", " + ctime(helpers[name]["time"]) + ".\n";

    if( helpers[name] ) {
    
        vouches = helpers[name]["vouches"];
        if( !vouches || !sizeof(vouches) ) {
            helper_mess += "    No vouches.\n";
        }
        else {
            helper_mess += "    Vouches were: " + 
                query_multiple_short( map( vouches, (: "$C$" + $1 :) ) ) +
                ".\n";
        }    

        if( !helpers[name]["guild"] || helpers[name]["guild"] == "unknown" ) {
            helper_mess += "    Guild: Unknown.\n";
        }
        else {
            helper_mess += "    Guild: " + helpers[name]["guild"] + 
                ".\n";
        }  
        
        if( !helpers[name]["nationality"] || 
            helpers[name]["nationality"] == "unknown" ) {
            
            helper_mess += "    Nationality: Unknown.\n";
        }
        else {
            helper_mess += "    Nationality: " + helpers[name]["nationality"] 
                + ".\n";
        }  

          
    }
    
    if( admin[name] ) {    
        helper_mess += "    $C$" + name + " is a newbie helpers admin.\n";
    }
    
    return helper_mess;
    
} /* helper_info() */

private string applicant_info( string name, object tp ) {
    string app_mess, voucher;
    mapping vouches;

    app_mess = "Details for $C$" + name + ":\n";
    
    app_mess += "    Applied: " + 
        ctime(applicants[name]["time"]) + ".\n";
    app_mess += "    Age: " + query_time_string(applicants[name]["age"],-1) +
        ".\n";
    app_mess += "    Guild: " + applicants[name]["guild"] + ".\n";
    app_mess += "    Nationality: " + applicants[name]["nationality"] + ".\n";
    
    vouches = applicants[name]["vouches"];
    if( !vouches || !sizeof(vouches) ) {
        app_mess += "    No vouches.\n";
    }
    else {
        app_mess += "    Vouches:\n";
        foreach( voucher in keys(vouches) ) {
            app_mess += "        $C$" + voucher + " (" + 
                query_time_string( 
                    applicants[name]["vouches"][voucher]["age"], -1 
                ) + " old)\n";
        }
    }    
    
    
    app_mess += "\n";    
    app_mess += "    $C$" + name + "'s application:$I$8=\n\n";
    app_mess += applicants[name]["statement"] + "$I$0=\n\n";
    
    if( !vouches || !sizeof(vouches) ) {
        app_mess += "    No vouches.\n";
    }
    else {
        foreach( voucher in keys(vouches) ) {
            app_mess += "    Vouch by $C$" + voucher + " made " +  
                ctime(applicants[name]["vouches"][voucher]["time"]) +
                ":$I$8=\n\n";
            app_mess += applicants[name]["vouches"][voucher]["statement"] + 
                "$I$0=\n\n";
        }
    }   
     
    if( admin[name] ) {    
        app_mess += "    $C$" + name + " is a newbie helpers admin.\n";
    }
        
    return app_mess;
    
} /* applicant_info() */


string query_check_player( object checker, string player ) {

    if( !helpers[player] && !admin[player] && !applicants[player] ) {
        return "$C$" + player + " is not registered with the newbie "
            "helpers handler.\n";
    }
    
    if( !admin[checker->query_name()] ) {
        return "You are not a member of newbiehelpers admin.\n";
    }

    TP->add_succeeded_mess(TO, "", ({ }) );

    if( helpers[player] ) {
        return helper_info( player, TP );
    }
    
    if( applicants[player] ) {
        return applicant_info( player, TP );
    }
    
    return "$C$" + player + " is just an admin.\n";

} /* check_player() */



int add_helper( object adder, string player ) {
    string *player_vouches, player_guild, player_nationality, name;
    
    name = adder->query_name();
    
    if( !admin[name] ) {
        tell_object( TP, "You are not a member of newbiehelpers admin.\n" );
        return 0;    
    }
    
    if( !PLAYER_HANDLER->test_user(player) ) {
        tell_object( TP, "There is no player $C$" + player + "!\n" );
        return 0;
    }

    if( helpers[player] ) {
        tell_object( TP, "$C$" + player + " is already a newbie "
            "helper.\n" );
        return 0;
    }

    if( applicants[player] ) {
        player_vouches = keys( applicants[player]["vouches"] );
        player_guild = applicants[player]["guild"];
        player_nationality = applicants[player]["nationality"];
        map_delete( applicants, player );
            AUTO_MAILER->auto_mail(player, name, "Your newbie helpers "
                "application", "", "Hello,\n\nI am pleased to inform you "
                "that your newbie helpers application was successful, and "
                "that you have now been hired as a newbie helper.  This was "
                "an automatic mail.\n\n$C$" + name, 0, 0);
    }
    else {
        player_vouches = ({ });
        player_guild = "unknown";
        player_nationality = "unknown";
    }


    helpers[player] = ([ ]);
    helpers[player]["time"] = time();
    helpers[player]["employer"] = name;
    helpers[player]["vouches"] = player_vouches;
    helpers[player]["guild"] = player_guild;
    helpers[player]["nationality"] = player_nationality;
    save_file();

    log_message( player + " added as a helper by " + name );
    if ( PLAYERINFO_HANDLER->add_entry( this_object(), player, PI_TYPE,
        "Added as a newbiehelper.", 0 ) ) {
        log_message( player + "'s playerinfo updated" );
    }
    else {
        log_message( player + "'s playerinfo NOT updated (an error "
            "occurred)" );
    }
    
    return 1;

} /* add_helper() */



string remove_helper( object remover, string player, string reason ) {
    string name;
    
    name = remover->query_name();
    
    if( !admin[name] ) {
        tell_object( TP, "You are not a member of newbiehelpers admin.\n" );
        return 0;    
    }

    if( !helpers[player] ) {
        return "$C$" + player + " is not a newbie helper.\n";
    }

    map_delete( helpers, player );
    save_file();
    
    
    log_message( player + " removed as a helper by " + name + ", reason: " + 
        reason );    
    if ( PLAYERINFO_HANDLER->add_entry( this_object(), player, PI_TYPE,
        "Removed as a newbiehelper (" + reason + ").", 0 ) ) {
      
        log_message( player + "'s playerinfo updated" );
    }
    else {
        log_message( player + "'s playerinfo NOT updated (an error "
            "occurred)" );
    }


    return "You remove $C$" + player + "'s helper status.\n";
    
} /* remove_helper() */


void submit_vouch(string statement, mixed args) {
    string name, player;
    object voucher;
    
    voucher = args[0];
    player = args[1];
    name = voucher->query_name();
    
    applicants[player]["vouches"][name] = ([ ]);
    applicants[player]["vouches"][name]["time"] = time();
    applicants[player]["vouches"][name]["age"] = -voucher->query_time_on();
    applicants[player]["vouches"][name]["statement"] = statement;
    save_file();
    
    log_message( name + " submits a vouch for " + player );
    
} /* submit_vouch() */


string no_vouch_reason( object voucher, string applicant ) {
    string name;
    
    name = voucher->query_name();
    
    if( !applicants[applicant] ) {
        return "$C$" + applicant + " isn't applying to be a newbie helper, "
            "so you can't vouch for them.\n";
    }

    if( applicants[applicant]["vouches"][name] ) {
        return "You already have already vouched for $C$" + applicant + " on "
            "this application, you cannot vouch again.\n";
    }  
     
    if( applicant == name ) {
        return "You cannot vouch for yourself!\n";
    }
    
    if( -voucher->query_time_on() < minimum_vouch_age ) {
        return "You cannot vouch for $C$ " + applicant + " as you are too "
            "young to vouch.  You need to be " + 
            query_time_string( minimum_vouch_age, -1 ) + " old before you "
            "can vouch on people's applications to be a newbie helper.\n";
    }

    return 0;

} /* no_vouch_reason() */


void submit_application(string statement, object player) {
    string name;
    
    name = player->query_name();
    
    applicants[name] = ([ ]);
    applicants[name]["time"] = time();
    applicants[name]["age"] = -player->query_time_on();
    applicants[name]["statement"] = statement;
    applicants[name]["vouches"] = ([ ]);
    applicants[name]["guild"] = 
        (load_object(player->query_guild_ob()))->query_short();
    if( player->query_nationality() ) {
        applicants[name]["nationality"] = "$C$" +
            (load_object(player->query_nationality()))->query_name();
        }
    else {
        applicants[name]["nationality"] = "unknown";
    }
    save_file();

    log_message( name + " submits an application to newbiehelpers" );
    
} /* submit_application() */


string no_apply_reason( object player ) {
    string name;
    
    name = player->query_name();
    
    if( helpers[name] ) {
        return "You are already a newbie helper, why are you applying "
            "again?\n";
    }   
    if( applicants[name] ) {
        return "You already have a newbie helper application open, you "
            "should wait to hear back about it.\n";
    }   
    if( -player->query_time_on() < minimum_apply_age ) {
        return "You cannot apply to be a newbie helper as you are too "
            "young.  You need to be " + 
            query_time_string( minimum_apply_age, -1 ) + " before you can "
            "apply to be a newbie helper.\n";
    }
    
    return 0;

} /* no_apply_reason() */


void reject_application( object rejecter, string player, string reason, 
    string mail ) {
    
    string name;
    
    name = rejecter->query_name();
    
    if( !admin[name] ) {
        return;
    }
    
    if( !applicants[player] ) {
        return;
    }   

    AUTO_MAILER->auto_mail(player, name, "Your newbie helpers "
        "application", "", mail, 0, 0);
    map_delete( applicants, player );
    save_file();

    log_message( name + " rejects " + player + "'s application, reason: " + 
        reason );
    if ( PLAYERINFO_HANDLER->add_entry( this_object(), player, PI_TYPE,
        "Rejected as a newbiehelper (" + reason + ").", 0 ) ) {
        log_message( player + "'s playerinfo updated" );
    }
    else {
        log_message( player + "'s playerinfo NOT updated (an error "
            "occurred)" );
    }

} /* reject_application() */

string no_reject_reason( object rejecter, string player ) {
    string name;
    
    name = rejecter->query_name();
    
    if( !admin[name] ) {
        return "You are not a member of newbiehelpers admin.\n";
    }
    
    if( !applicants[player] ) {
        return "$C$" + player + " isn't applying to be a newbie helper, so "
            "you can't reject them.\n";
    }   

    return 0;

} /* no_reject_reason() */


int query_helper( string name ) {

    if( helpers[name] ) { 
        return 1;
    }
    else { 
        return 0;
    }

} /* query_helper() */


int add_admin( string name ) {

    if( !admin[TP->query_name()] ) {
        return 0;
    }

    if( !admin[name] ) {
        admin[name] = 1;
        save_file();
        log_message( name + " added as admin by " + TP->query_name() );
        return 1;
    }
    else {
        return 0;
    }
    
} /* add_admin() */

int remove_admin( string name ) {

    if( !admin[TP->query_name()] ) {
        return 0;
    }   

    if( !admin[name] ) {
        return 0;
    }
    else {
        map_delete(admin, name);
        save_file();
        log_message( name + " removed from admin by " + TP->query_name() );
        return 1;
    }
    
} /* remove_admin() */

int query_admin( string name ) {

    if( admin[name] ) { 
        return 1;
    }
    else { 
        return 0;
    }

} /* query_admin() */

int query_can_chat( object who ) {
    if( query_helper( who->query_name() ) || 
        query_admin( who->query_name() ) ||
        who->query_creator() ) {
        return 1;
    }
    return 0;
} /* query_can_chat */

string query_name() { return "Newbiehelpers Handler"; } /* query_name() */

// Log a message
protected void log_message( string message ) {
    log_file("/log/NEWBIEHELPERS", ctime(time()) + ": " + message + "\n");
} /* log_message() */

// Save the memory list.
protected void save_file() {
    unguarded( (: save_object, SAVE_FILE, 1 :) );
} /* save_file() */

// Load the memory list.
protected void load_file() {
    if (file_size(SAVE_FILE + ".o") > 0)
        unguarded( (: restore_object, SAVE_FILE, 1 :) );
} /* load_file() */

