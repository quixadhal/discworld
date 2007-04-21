/**
 * Property tracker.
 * @author Taffyd
 * @started 10/06/99 3:42AM
 * @main
 */

#include <login.h>

// #define LORD_ONLY 1
//#define DEBUG 1 

#define CACHE_PROPERTY "property_tracker"
// #define CACHE_TIME 60*60*24*14
// #define CACHE_TIME 60

nosave inherit "/std/object";

void save_file();

private string *_invalid_properties;
private mapping _property_funcs;

void create() {
    ::create();
    
    seteuid( "Root" );
    
    set_name( "tracker" );
    set_short( "property tracker" );
    
   
    unguarded( (: restore_object, "/save/property_tracker.o" :) );
    
    if ( !_invalid_properties ) {
        _invalid_properties = ({ "team", "ctf_deaths", "ctf_shared", 
            "ctf_solo", 
            "completed feather quest", "ChapelGapp",
            "scythe sharpness", "DJ to ephebe", 
            "emote", "quiting frog", "woeshipper", 
            "arresting", "Junior's friend", "donetd", 
            "Sek_gem", "looky thingy", "riddle_master",
            "help shopkeeper" });
        
        _property_funcs = ([ ]);
    
        save_file();
    }

} /* create() */

/**
 * This method is called by the login handler every time someone logs onto
 * the MUD. It checks through any properties that they have, and removes
 * them if they are marked as invalid.
 * <p>
 * If a property function was set, then this function is called now when the 
 * property is removed. It is passed three parameters, string property,
 * object player, and mixed property_data.
 * <p>
 * @param person the person who is logging on
 * @param type the type of login event. Only LOGIN events are used
 * by this object. 
 * @see /include/login.h
 * @see /obj/handlers/login_handler.c
 */
public void check_properties(mixed person, string type) {
    string property;
    mixed data;
    int result;
    int diff;

    
    if (type != LOGIN)
        return;

    if ( !objectp(person) && !( person = find_player( person ) ) ) {
        return;
    }
#ifdef CACHE_TIME
    diff = CACHE_TIME;
#else
    diff = 1;
#endif
    
    if ( time() > person->query_property( CACHE_PROPERTY ) + diff ) {
#ifdef DEBUG
        tell_creator( "taffyd", "Scanning %s (%O) for properties.\n", 
            person->query_name(), person );
#endif
        foreach( property, data in person->query_properties() ) {
            if ( member_array( property, _invalid_properties ) > -1 ) {
#ifdef DEBUG
                tell_creator( "taffyd", "Checking to property %s.\n",
                    property );
#endif
                if ( _property_funcs[ property ] ) {
                    result = call_other( _property_funcs[ property ][ 1 ],
                        _property_funcs[ property ][ 0 ], property, person, 
                        data );
                    if ( result ) {
                        continue;
                    }
                }
                 
#ifdef DEBUG
                tell_creator( "taffyd", "Removing property %s.\n",
                    property );
#endif                                        
                person->remove_property( property );
            }
        }
#ifdef CACHE_TIME     
        person->add_property( CACHE_PROPERTY, time() );
#else
        person->remove_property( CACHE_PROPERTY );
#endif
    }
    person->add_skill_level( "other.language.common.spoken", -person->query_skill( "other.language.common.spoken" ) );
    person->add_skill_level( "other.language.common.written", -person->query_skill( "other.language.common.written" ) );
} /* check_properties() */


/** 
 * This returns a list of all the properties that are marked as invalid.
 * @return an array of invalid properties
 */
public string query_invalid_properties() {
    return copy( _invalid_properties );
} /* query_invalid_properties() */

/**
 * This function allows a new invalid property to be added to the list
 * of invalid ones.  The last two parameters are used so that you can
 * add a callback if you want to convert a property over to the quest
 * handler or whatever. They are optional.
 * @param property the property name to be added
 * @param func this parameter specifies the name of the function to be 
 * called when the property is removed
 * @param obj this parameter specifies the path of the object on which func
 * is called when the property is removed
 * @return 1 if the property was added as invalid, 0 if it was not.
 * @example
 * add_invalid_property( "womble frog" )
 * @example
 * add_invalid_property( "grave_digger", "convert_property", "/d/sur/Nowhere/chars/isabel" )
 */
varargs public int add_invalid_property( string property, mixed func, mixed obj ) {

#ifdef LORD_ONLY
    if ( !master()->query_lord( previous_object( -1 ) ) ) {
        tell_creator( this_player(1), "Get a Lord to add your property for "
            "you.\n" );
        return 0;
    }
#endif
    
    if ( member_array( property, _invalid_properties ) == -1 ) {

        if ( objectp( func ) ) {
            func = file_name( func );
        }
        
        if ( objectp( obj ) ) {
            obj = file_name( obj );
        }
        
        if ( func && obj ) {
            _property_funcs[ property ] = ({ func, obj });
        }

        _invalid_properties += ({ property });        
        save_file();
        return 1;
    }
    
    return 0;
} /* add_invalid_property() */

int remove_invalid_property( string property ) {

#ifdef LORD_ONLY
    if ( !master()->query_lord( previous_object( -1 ) ) ) {
        tell_creator( this_player(1), "Get a Lord to add your property for "
            "you.\n" );
        return 0;
    }
#endif
    
    if ( _property_funcs[ property ] ) {
        map_delete( _property_funcs, property );
    }

    _invalid_properties -= ({ property });
    save_file();
    return 1;
} /* remove_invalid_property() */

void save_file() {
    unguarded( (: save_object, "/save/property_tracker" :) );
} /* save_file() */


void reset_data() {
    unguarded( (: rm, "/save/property_tracker.o" :) );
} /* reset_data() */


void convert_no_score( string property, object who, mixed data ) {
    if ( property != "no_score" )
        return;

    if ( data ) {
        who->remove_property( property );
        who->add_property( "no score", 1 );
    }
} /* convert_no_score() */

int remove_warmth( string property, object who, mixed data ) {
    if ( property != "warmth" ) {
        return 1;
    }

    if ( !who->query_property_time_left( property ) ) {
        who->remove_property( property );
        return 1;
    }

    return 1;
}


