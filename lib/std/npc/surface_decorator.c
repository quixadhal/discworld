/**
 *  This is a inheritable for playerhousing surface decorating npcs.
 *  
 *  With this you can add carpets, wood panelling, beams or anything your 
 *  little heart desires to npcs that will then go out and make the world a    
 *  prettier place.
 *
 *  If you want to add a surface to an npc that he will be able to 
 *  decorate, use eg:
 *  
 *  add_allowed_surface( "floor" );    
 *
 *  If you want to add a type of decoration that the npc can install, use eg:
 *  
 *  add_fitted_surface( "plush blue carpet", "covered with plush "
 *  "blue carpet", <cost> );
 *
 *  @September 3rd, 2002
 *  @authors Una and Dasquian
 *
 **/

#include <money.h>

inherit "/obj/monster";

private mapping fitted_surfaces = ([ ]);
private string *allowed_surfaces = ({ });
private string *allowed_domains = ({ });
private string *responses = ({ 
        "'I don't fit those kinds of surfaces.",
        "'I don't fit that kind of decor.",
        "'You don't own this place, sorry!",
        "'I can't decorate surfaces here!",
        "'Something went horribly wrong!",
        "'All finished!",
        "'You don't have enough money to pay for the $decor$!  It "
        "will cost you $amount$.",
        "'I can fit $allowed$ with any of the following:", 
        "'I don't have any stock right now.",
        "'Ok, I'll get right on with it.", 
        "'What?  Did somebody say something?",
        "'I've already been hired by $employer$.",
        "'Ok, take me to the room you want decorated and ask me to "
        "fit something when we get there.", 
        "'Sorry, I don't go there."
    });
private string under_construction_mess = "covered in dust and dirt" ;
private int work_time = 10;
private object employer;
private string last_ok;

int do_ask( string surface, string decor );


void create() {
 
    do_setup++;
    ::create();
    do_setup--;
    
    if (!do_setup) {
        this_object()->setup();
        this_object()->reset();
    }
    add_help_file("/doc/object/surface_decorator");
    add_enter_commands("#check_domain");
    
} /* create() */


void init() {

    ::init();
    this_player()->add_command( "ask", this_object(), "<direct:object> to "
        "fit the <string'surface'> with <string'decor'>", 
        (: do_ask($4[1], $4[2])  :) );
        
    this_player()->add_command( "list", this_object(), "goods from "
        "<direct:object>" );
        
    this_player()->add_command( "hire", this_object(), "<direct:object>");

} /* init() */


/**
 * Causes the surface decorator to give a list of the surfaces he decorates
 * and what he can decorate them with, or says that he has no stock.
 */
int do_list() {
    int i;
    string thing;
    
    i=1;
    
    if( !sizeof( fitted_surfaces ) ) {
        init_command( responses[8], 1 );
        return 1;
    }
    
    init_command( replace( responses[7], ({
            "$allowed$", query_multiple_short( map( allowed_surfaces, 
                (: pluralize($1) :) ) )
        }) ), i );

    foreach( thing in keys( fitted_surfaces ) ) {
        i++;
        init_command("'" + capitalize(thing) + " for " + 
            MONEY_HAND->money_value_string(fitted_surfaces[thing][1], 
            query_property("place")) + ".", i);
    }
    return 1;
    
} /* do_list() */


/**
 * Sets him to follow the player and marks that player as his current 
 * employer
 */
int do_hire() {
    
    this_player()->add_succeeded_mess( this_object(), "$N ask$s $D about "
        + this_object()->query_possessive() + " services.\n", ({ }) );

    // Make sure he can see you    
    if( !this_player()->query_visible( this_object() ) ) {
        init_command( responses[10], 1 );
        return 1;
    }
    
    // Make sure he isn't already following someone else
    if( employer && 
        environment( employer ) == environment( this_object() ) &&
        this_player() != employer ) {
        
        init_command( replace( responses[11], ({ "$employer$", "$C$" + 
            employer->query_name() }) ), 1 );
        return 1;
    }
    
    // Sets him as following the player
    employer = this_player();
    do_command("unfollow everyone");
    employer->add_follower(this_object());
    
    init_command( responses[12], 1 );
    
    return 1;

} /* do_hire() */


/** 
 * Does all the necessary checks to see if he can decorate the requested 
 * surface with the requested decor, handles payment and starts the work. 
 *
 * @param surface the surface to be decorated 
 * @param decor the short name of the decor to be used, eg. green carpet
 * 
 * @see finish_work
 */
int do_ask( string surface, string decor ) {
    string long, allowed, place;
    int fee, can_do;
    object where;
    
    can_do = 0;
    
    // Checks to see if the requested surface contains any of the words 
    // in the list of allowed surfaces, eg. if "wall" is in the allowed
    // surfaces, then "north wall" will be allowed.
    foreach( allowed in allowed_surfaces ) {
        if( strsrch( surface, allowed ) != -1 ) {
            can_do = 1;
        }
    }
    
    if( !can_do ) {
        do_command(responses[0]);
        return 1;
    }   
    
    // Checks that the npc does the requested decor
    if( member_array( decor, keys( fitted_surfaces ) ) == -1 ) {
        do_command(responses[1]);
        return 1;
    }
    
    long = fitted_surfaces[decor][0];
    fee  = fitted_surfaces[decor][1];
    where = environment( this_object() );

    // Checks that the player owns the room
    if(!where->test_occupier( this_player()->query_name() ) ) {
        do_command(responses[2]);
        return 1;        
    }
  
    // Checks that the room can be decorated at all
    if(!function_exists("decorate_surface", where)) {
        do_command(responses[3]);
        return 1;        
    }
  
    place = query_property("place");
  
    // Checks the player has enough money
    if ( this_player()->query_value_in( place ) < fee ) {
        do_command( replace( responses[6], ({
            "$decor$", decor, 
            "$amount$", MONEY_HAND->money_value_string(fee, place)
        }) ) );
        queue_command("stare" + this_player()->query_name());
        return 1;
    }
    
    // Try to set the surface to be under construction and bail if
    // we fail, otherwise set the call_out to finish the work    
    if(!where->decorate_surface(surface,under_construction_mess)) {
        
        do_command(responses[4]);
        return 1;
    }
    call_out( "finish_work", work_time, surface, long, where );
    do_command( responses[9] );
    do_command( "unfollow everyone" );
    
    // Finally, charge the player after we know the work is going to be 
    // done    
    this_player()->pay_money( (mixed *)MONEY_HAND->create_money_array( fee,
            place ), place);
            
    this_player()->add_succeeded_mess( this_object(), "$N pay$s $D some "
        "money and " + this_object()->query_pronoun() + " starts to go "
        "about his work on the " + surface + ".\n", ({ }) );  
    return 1;
        
} /* do_ask() */


/**
 * Called by do_ask(), this finishes decorating the surface
 *
 * @param surface the surface to be decorated
 * @param long the long description of the decorated surface, eg. carpeted   
 * with plush green carpet
 * 
 * @see do_ask
 */
void finish_work( string surface, string long, object where ) {

    do_command(":finishes his work.");
    if( employer ) {
        employer->add_follower(this_object());
    }
    
    // The work is finished.  Set the surface's description to that which
    // was requested!
    if(!where || !where->decorate_surface(surface, long )) {
        do_command(responses[4]);
        return;
    }
    
    else {
        do_command(responses[5]);
        return;
    }

} /* finish_work() */

/**
 * Attempts to move the npc back to its good location.  It first moves it
 * to the rubbish room so that if it hasn't got a last good location or
 * fails to be moved to that location, it stays in the rubbish room.
 */
private void go_back() {
    
    move("/room/rubbish", "$N walk$s in.", "$N walk$s away.");
    
    if( last_ok ) {
        move(last_ok, "$N walk$s in.", "$N walk$s away.");
    }
        
} /* go_back() */


/** 
 * Called every time the npc moves.  This function checks that the file name 
 * of its new environment contains at least one of the allowed domains.  If
 * it does, it will store the current room as being its last good location.  
 * Otherwise, it will move it to its last good location.
 *
 * @see go_back
 */
void check_domain() {
    string allowed, here;
    int ok;
    
    here = file_name( environment( this_object() ) );
    ok = 0;
    
    foreach( allowed in allowed_domains ) {
        if( strsrch( here, allowed ) != -1 ) {
            ok = 1;
        }
    }   
    
    if( ok ) {
        last_ok = here;
        return;
    }
    else {
        do_command("unfollow everyone");
        do_command( responses[13] );
        go_back();
        return;
    }
    
} /* check_domain() */


/**
 * Adds a new decoration that the npc can use to his stock list
 *
 * @param short the short description used in the stock list and for ordering
 * @param long the long description actually used for the new decoration in 
 * the room
 * @param cost the cost of installing this decoration
 */
void add_fitted_surface( string short, string long, int cost ) {
    fitted_surfaces[ short ] = ({ long, cost });
} /* add_fitted_surface() */

mapping query_fitted_surfaces() { return fitted_surfaces; }


/**
 * Removes a decoration whose short matches the argument given
 *
 * @param short the short description of the surface to remove
 */
void remove_fitted_surface( string short ) {
    map_delete( fitted_surfaces, short );
} /* remove_fitted_surface() */


/**
 * Removes all decorations from the npc's stock list
 */
void remove_all_fitted_surfaces() {
    fitted_surfaces = ([ ]);
} /* remove_all_fitted_surfaces() */


/**
 * Allows the npc to work on a specific surface in a room, eg. floor, ceiling
 * If the argument passed is "wall", they will be able to work on all walls,
 * eg. north wall, east wall, south wall, etc.
 *
 * @param surface the name of the surface to be added
 */ 
void add_allowed_surface( string surface ) {
    if( member_array( surface, allowed_surfaces ) == -1 ) {
        allowed_surfaces += ({ surface });
    }
} /* add_allowed_surface() */

string *query_allowed_surfaces() { return allowed_surfaces; }


/**
 * Adds a string to the list of domains to be checked by the npc when it 
 * moves to see if it's allowed to be in that room, eg. "/d/guilds" or 
 * "Bes_Pelargic"
 *
 * @param domain the name of the domain to be added
 */ 
void add_allowed_domain( string domain ) {
    if( member_array( domain, allowed_domains ) == -1 ) {
        allowed_domains += ({ domain });
    }
} /* add_allowed_domain() */

string *query_allowed_domains() { return allowed_domains; }


/**
 * Sets custom responses.  The array size must match the existing one or the  
 * call will be disregarded.
 *
 * @param the_responses the array of responses to be used
 */
void set_responses( string *the_responses ) {

    if( sizeof(the_responses) != sizeof(responses) ) {
        return 0;
    }
    else {
        responses = the_responses;
    }
    
} /* set_responses() */

string *query_responses() { return responses; } /* query_responses() */


/** 
 * Sets what a surface looks like while the npc is working on it.
 *
 * @param mess the description to replace the surface's appearance in the 
 * room while it is being worked on
 */
void set_under_construction_mess( string mess ) {
    under_construction_mess = mess;    
} /* set_under_construction_mess() */

string query_under_construction_mess() { return under_construction_mess; }
    

/**
 * Sets the time it takes to finish the work.
 *
 * @param time the time (in seconds) taken to complete the work
 */
void set_work_time( int time ) {
    work_time = time;
} /* set_work_time() */

int query_work_time() { return work_time; }


/**
 * Sets the current player employer
 *
 * @param player the player object currently using the npc
 */
void set_employer( object player ) {
    employer = player;
} /* set_employer() */

object query_employer() { return employer; } /* query_employer() */
