/*  -*- LPC -*-  */

/**
 * The service provider object.  It is designed to be similar to
 * the peddler object, only instead of selling objects, it sells
 * services instead.
 *
 * @author Furcifer
 *
 * March 2000
 **/


#include <armoury.h>
#include <money.h>
#include <move_failures.h>

inherit "/obj/monster";

#define SERVICE  0
#define BROWSE   1
#define FUNCTION 2
#define COST     3
#define ALIASES  4

private int parsing_function( mixed *array , string words );

int _busy;
mixed *_service_array = ({ });

void set_busy()   { _busy = 1; }

void reset_busy() { _busy = 0; }

varargs int check_busy( object player )
{
    if( _busy || sizeof( query_queued_commands() ) )
    {
        if( player ) {
            tell_object( player, query_short() +" is busy at the moment."+
            "  " + capitalize( query_pronoun() )+ " will be with you " +
            "shortly.\n" );
        }
    return 1;
    } else
        return 0;
} /* check_busy */

void create()
{
    do_setup++;
    ::create();
    do_setup--;
    if ( !do_setup ) this_object()->setup();
} /* create() */

/** @ignore yes */
void init()
{
    ::init();
    this_player()->add_command( "inquire", this_object(),
        "[a] <string> {from|of} <direct:living:here>", "do_browse" );
    this_player()->add_command( "browse", this_object(),
        "[a] <string> {from|of} <direct:living:here>" );
    this_player()->add_command( "request", this_object(),
        "[a] <string> from <direct:living:here>" );
    this_player()->add_command( "list", this_object(),
        "services [available] {from|of} <direct:living:here>" );
} /* init() */

/**
 * This method adds a service to the list of services on offer.
 * @param service the name of the service
 * @param cost what it costs
 * @param the function which controls
 * @param *aliases an array of aliases for this service.
 * @return 1 on success, 0 on failure
 */
varargs int add_service( string service , string browse_info ,
    string func, int cost, string *aliases )
{
    string *alias_array;

    /* Does a function exist to define the service */
    if( !service || !browse_info || !func ||
        !function_exists( func , this_object() , 1 ) )
            return 0;

    /* If things get to here, then it obviously does, so... */
    alias_array = !sizeof(aliases) ? ({ }) : aliases ;

    _service_array += ({ ({ service , browse_info , func , cost ,
        alias_array }) });
    return 1;
}

/**
 * This method removes a service.
 * @param string service to be removed
 * @return 1 on successful removal, 0 on failure
 **/
int remove_service( string service )
{
    mixed *things;
    things = filter( _service_array, (: $1[SERVICE]==$(service) :) );
    switch( sizeof(things) ) {
        case 1: _service_array -= things; return 0; break;
        default: return 0; break;
    }
    return 0;
}

/**
 * The main entrance to the browse for things command.
 * @return 1 on success, 0 on failure
 **/
int do_browse( mixed indirect_obs, string dir_match, string indir_match,
    string *words )
{
    string place;
    mixed *things;
    object player;

    /* store the player -- we might need them after a time delay */
    player = this_player();

    /* find out if the provider is busy */
    if ( check_busy( player ) ) return 0;

    /* find out where we are, we'll need that later. */
    place = query_property( "place" );
    if ( !place || ( place == "" ) )
        place = "default";

    things =
        filter( _service_array ,(:parsing_function( $1, $(words)[0]):) );

    if( !sizeof( things ) ) {
        init_command( "say I'm afraid I don't offer that service.", 2 );
    } else {
        init_command("say A " + things[0][SERVICE] + " costs " +
            MONEY_HAND->money_value_string( things[0][COST], place ) +
            ".  " + things[0][1], 2 );
    }
    player->add_succeeded_mess( this_object(), "$N ask$s $D "+
          "about "+ words[ 0 ] +".\n", ({ }) );
    return 1;
} /* do_browse() */

/**
 * The main entrance to the buy things command.
 * @return 1 on success, 0 on failure
 **/
int do_request( mixed indirect_obs, string dir_match, string indir_match,
      string *words )
{
    mixed *things;
    object player;
    int value;
    string place;

    player = this_player();
    place = query_property("place");
    if ( !place || place == "" )
        place = "default";

    if ( check_busy( player ) ) return 0;

    things =
        filter( _service_array , (: parsing_function($1,$(words)[0]) :));

    if ( !sizeof( things ) )
        init_command( "say I'm afraid I don't offer that service.", 2 );
    else {
        value = player->query_value_in("default") +
            ( place == "default" ? 0 : player->query_value_in( place ) );
        if( things[0][COST] > value )
            init_command("say And just how do you propose to pay "+
                "for a "+things[0][SERVICE]+"?" , 2 );
        else {
            init_command("say Certainly, "+
                ( player->query_gender() == 1 ? "Sir" : "Ma'am" )+
                "!");
            tell_object( player , "You hand "+query_short()+" the "+
                "necessary money.\n");
            player->pay_money( MONEY_HAND->
                create_money_array(things[0][COST], place ), place );
            call_out( things[0][FUNCTION] , 4 , player, things[0] );
        }
    }
    player->add_succeeded_mess( this_object(), "$N ask$s $D "+
        "about having a "+ words[ 0 ] +".\n", ({ }) );
   return 1;
} /* do_buy() */

/**
 * The main entrance to the list stuff command.
 * @return 1 on success, 0 on failure
 **/
int do_list()
{
    string place, list;
    int i,number;

    number = sizeof( _service_array );

    place = query_property( "place" );
    if ( !place || ( place == "" ) )
        place = "default";

    list = "I am currently offering the following: ";
    if( number > 3 ) {
        for( i=0 ; i < (number-2) ; i++ ) {
            list += ( "a "+_service_array[i][SERVICE]+" for "+
                MONEY_HAND->money_value_string(
                    _service_array[i][COST], place )+", " );
        }
    }
    if( number > 1 ) {
        list += ( "a " + _service_array[<2][SERVICE] + " for " +
            MONEY_HAND->money_value_string(
                _service_array[<2][COST], place ) +" and " );
    }
    if( number > 0 ) {
        list += ( _service_array[<1][SERVICE] + " for " +
            MONEY_HAND->money_value_string(
                _service_array[<1][COST], place ) +"." );
    }
    if( number == 0 )
        list = "I am not currently offering any services.";

    init_command("say "+ list, 2);

    this_player()->add_succeeded_mess( this_object(), "$N ask$s $D what "+
         "services "+ (string)this_object()->query_pronoun() +" has "+
         "on offer.\n", ({ }) );
   return 1;
} /* do_list() */

/**
 * This function is used in matching strings to elements of the service
 * array.
 **/
private int parsing_function( mixed *array , string word )
{
    /* Does it match the service name? */
    if ( array[SERVICE] == word ) return 1;

    /* Does it match any of the aliases? */
    if ( member_array( word, array[ALIASES] ) != -1 ) return 1;

    /* Well, I guess it doesn't match */
    return 0;
}

