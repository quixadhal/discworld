/**
 * Bath house inheritable!
 * @author Taffyd
 * @started 30/04/00 12:08
 */
#include <effect.h>

#define SOAP_EFFECT "/std/effects/healing/antiseptic_soap"
#define WET_EFFECT "/std/effects/other/wetness"

/**
 * @ignore yes
 * Function prototypes!
 */
protected int add_wash_effect( string classification, function func );
protected int add_finished_func( function func );
protected void advanced_antiseptic( object ob, string classification, mixed arg, int effect_enum, object effect_ob );
protected void basic_cleaning( object ob, int effect_enum, string message );
protected void apply_soap( object ob, string *applied_effects );
protected void wet_person( object ob, string *applied_effects );

private mapping _wash;
private function *_finished_funcs;


/** @ignore yes */
void create() {
    _wash = ([ ]);
    _finished_funcs = ({ });

    add_wash_effect( "body.smell", (: advanced_antiseptic :) );
    add_wash_effect( "misc.ketchup", (: basic_cleaning( $1, $3,
        "The soap rinses the ketchup off your face." ) :) );
    add_wash_effect( "object.label", (: basic_cleaning( $1, $3,
        "The labels stuck to your body peel off in the water." ) :) );
    add_wash_effect( "body.frog-adhesive", (: basic_cleaning( $1, $3,
        "The frantic ribbiting fades away as the frog adhesive washes "
        "off." ) :) );
    add_finished_func( (: apply_soap :) );
    add_finished_func( (: wet_person :) );

    this_object()->add_setup_wash_command();
} /* create() */

/** @ignore yes */
void init() {
    /* Call this function in this_object() to add any commands */
    this_object()->add_init_wash_command();
} /* init() */


/**
 * This method adds an effect to be removed when washing here.
 * If the effect is matched, then func is evaluated.
 * @param classification the effect classification to match.
 * @param func the function to evaluate when the effect is matched.
 * @return 1 if the effect classification was added, 0 if it could not be
 * (most likely a duplicate effect classification).
 * @example
 * add_wash_effect( "body.smell", (: advanced_antiseptic :) );
 * @example
 * add_wash_effect( "object.label", (: basic_cleaning( $1, $3,
 *    "The labels stuck to your body peel off in the water." ) :) );
 */
protected int add_wash_effect( string classification, function func ) {
    if ( !_wash[ classification ] ) {
        _wash[ classification ] = func;
        return 1;
    }

    return 0;
} /* add_wash_effect() */

/**
 * This adds a function to be evalauted when all of the wash effects
 * have been evaluated.
 * @param func the function to be evaluated
 * @return 1
 */
protected int add_finished_func( function func ) {
    _finished_funcs += ({ func });
    return 1;
} /* add_finish_func() */

/**
 * This returns a list of all the effect classifications that are currently
 * being managed.
 * @return an array of strings.
 */
public string *query_wash_classifications() {
    return keys( _wash );
} /* query_wash_classifications() */

/**
 * This method removes a wash effect.
 * @param the classification of the effect to be removed.
 * @return 1 if the effect was successfully removed, 0 if it wasn't.
 */
protected int remove_wash_effect( string classification ) {
    if ( _wash && _wash[ classification ] ) {
        map_delete( _wash, classification );
        return 1;
    }

    return 0;
} /* remove_wash_effect() */

/**
 * This is a basic cleaning method that can be used to remove a few
 * default things.
 */
protected void advanced_antiseptic( object ob, string classification, int effect_enum, mixed arg, object effect_ob ) {
    tell_object( ob,
        "The antiseptic power of the soap defeats " +
        effect_ob->smell_string( ob, arg ) + ".\n" );
    ob->delete_effect( effect_enum );
} /* advanced_antiseptic() */

/**
 * This is a small cut down cleaning effect that displays a message.
 */
protected void basic_cleaning( object ob, int effect_enum, string message ) {
    tell_object( ob, message + "\n" );
    ob->delete_effect( effect_enum );
} /* basic_cleaning() */

/**
 * This is the default finished func. It adds soap.
 */
protected void apply_soap( object ob, string *applied_effects ) {
    ob->add_effect( SOAP_EFFECT, 10000 );
} /* apply_soap() */

protected void wet_person( object ob, string *applied_effects ) {
    ob->add_effect( WET_EFFECT, 250 );
} /* wet_person() */

/**
 * This method washes a player.  Any wash commands that are defined
 * must ultimately call this function, so that all of the wash
 * functions can be evaluated.
 * <p>
 * When an effect is found on the player for a matching wash effect
 * classification, the function specified is evaluated with the following
 * parameters:
 * <ul>
 *  <li>ob, the player who is being washed
 *  <li>classification, the classification of the effect that was matched.
 *  <li>arg, the arguments to the effect.
 *  <li>effect_enum, the position of the effect in the player's effect list.
 *  <li>effect_ob, the object that the effect is found in.
 * </ul>
 * <p>
 * After this, the finished functions are evaluated with two arguments, the first
 * one being ob, the second one being an array of all the wash effect classifications
 * that were matched correctly.
 * @param ob the object to wash
 * @return 1 if the player was washed successfully, 0 if they weren't.
 */

public int wash_player( object ob ) {
    function func;
    string eff;
    string *cleaned;
    int *enums, eff_enum;

    cleaned = ({ });

    if ( !objectp( ob ) ) {
        return 0; 
    }


    foreach( eff, func in _wash ) {
        enums = ob->effects_matching( eff );

        if ( sizeof( enums ) ) {
            foreach( eff_enum in enums ) {
                evaluate( func, ob, eff, eff_enum, ob->arg_of( eff_enum ),
                    ob->eff_of( eff_enum ) );
            }

            cleaned += ({ eff });
        }
    }

    foreach( func in _finished_funcs ) {
        evaluate( func, ob, cleaned );
    }

    return 1;
} /* wash_player() */

int query_baths() {
    return 1;
} /* query_baths() */
