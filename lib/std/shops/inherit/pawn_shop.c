
/**
 * This inherit is under development.  Use at your own risk.
 * All bug reports to me, please.
 *
 * This is an inherit for pawn shops, which buy your items from you, but also
 * keep them for a short period of time for you to buy them back, before they
 * go into the shop's general inventory.
 *
 * It is built around the general store inherit /std/shop.
 *
 * @creator Vashti
 * @started 21st September 2002
 */

#include <money.h>
#include <move_failures.h>

#define RECEIPT "/obj/misc/pawn_receipt"
#define TO this_object()
#define TP this_player()

inherit "/std/shop" ;

private int    _pawn_ttl ;
private mixed  _pawn_mess ;
private int    _pawn_markup ;
private string _shop_language;


/**
 * This sets the language that the shop will use for receipts.
 * @param lang The language to write receipts in.
 */
void set_language( string lang ) {
    _shop_language = lang;
} /* set_language */


/**
 * This returns the language that the shop is using for receipts.
 * @return [string] The language receipts are written in.
 */
string query_language() {
    return _shop_language;
} /* query_language */


/**
 * This sets the percentage that the shop will mark reclaim prices up by.
 * The default is 20%.
 * @param percent The percentage to add to reclaim prices.
 */
void set_pawn_markup( int percent ) {
    _pawn_markup = percent ;
} /* set_pawn_markup */


/**
 * This returns the percentage that the shop will mark reclaim prices up by.
 * @return [int] The percentage to add to reclaim prices.
 */
int query_pawn_markup() {
    return _pawn_markup ;
} /* query_pawn_markup */



/**
 * This sets the time to live of pawned items, in seconds.  That is, the
 * length of time between an item being pawned and the shop selling that
 * item to recoup its costs.  The default is one week realtime.
 * @param ttl The time in seconds before the shop will sell a pawned item.
 */
void set_pawn_ttl( int ttl ) {
    _pawn_ttl = ttl ;
} /* set_pawn_ttl */


/**
 * This returns the time to live of pawned items, in seconds.  That is, the
 * length of time between an item's being pawned and the shop selling that
 * item to recoup its costs.
 * @return [int] The time in seconds before the shop will sell a pawned item.
 */
int query_pawn_ttl() {
    return _pawn_ttl ;
} /* query_pawn_ttl */


/**
 * This sets the message given out when somebody pawns something.
 * @param mess The message given when something is successfully pawned.
 */
void set_pawn_mess( mixed mess ) {
    _pawn_mess = mess ;
} /* set_pawn_mess */


/**
 * This returns the message given out when somebody pawns something.
 * @return [string] The message given when something is successfully pawned.
 */
string query_pawn_mess() {
    return _pawn_mess ;
} /* query_pawn_mess */


/** @ignore yes */
void create() {
    do_setup++ ;
    ::create() ;
    do_setup-- ;
    
    add_help_file( "pawn_shop" ) ;
    remove_help_file( "shop" );
  
    _pawn_mess = ({
        "You pawn $ob$ for $money$.\n",
        "$client$ pawns $ob$.\n"
    }) ;

    _pawn_markup = 20 ;             /* 20% */
    _pawn_ttl = 7 * 24 * 60 * 60 ;  /* 1 Roundworld week */
    _shop_language = "morporkian" ;
    set_min_amount( 400 ) ;         /* A$1 ; 4 silver coins */
    add_property( "no steal", 1 ) ; /* You can't just steal your item back */
    
    if ( ! do_setup ) {
        TO->setup() ;
        TO->reset() ;
    }
} /* create */


/** @ignore yes */
void init() {
    ::init() ;
    add_command( "pawn", "<indirect:object:me'items'>" ) ;
} /* init */


/**
 * This handles pawning items to the shop.
 */
int do_pawn( object *in_obs ) {
    int value, total ;
    mixed *m_array ;
    object ob, money, receipt ;
    object *cheap, *cre, *expensive, *kept, *nobuy, *stolen, *worn ;
    string place, fn ;
    string *text = ({ }) ;
    
    /* Make sure the shop is open for business */
    if ( ! is_open( TP, 0 ) ) {
        return 0 ;
    }
    
    in_obs = uniq_array( in_obs ) ;

    /* Check for cre-only items */
    cre = filter( in_obs, (: creator_object($1) :) ) ;
    if ( sizeof(cre) ) {
        if ( ! TP->query_creator() ) {
            tell_object( TP,
                "Oh dear, you shouldn't have " + query_multiple_short( cre ) +
                "!  "
                + ( sizeof(cre) > 1 ? "They disappear" : "It disappears" ) +
                " with a flash of octarine light.\n" ) ;
            cre->move( "/room/rubbish" ) ;
        } else {
            tell_object( TP,
                "You decide not to pawn " + query_multiple_short( cre ) +
                ", as " + ( sizeof(cre) > 1 ? "they are creator-only items"
                : "it is a creator-only item" ) + ".\n" ) ;
        }

        foreach( ob in cre ) {
            if ( fn = ob->query_property( "virtual name" ) ) {
                fn = ob->query_property( "virtual name" ) ;
            } else {
                fn = base_name( ob ) ;
            }
            text += ({ fn }) ;
        }
        
        log_file( "ILLEGAL_OBJECT", "%s: %s tried to pawn %s at %s.\n\n",
            ctime(time()), TP->query_name(), query_multiple_short( text ),
            file_name( TO ) ) ;
        
        in_obs -= cre ;
    }
    
    /* Check for kept items */
    kept = filter_array( in_obs, (: $1->query_keep() :) ) ;
    if ( sizeof(kept) ) {
        tell_object( TP,
            "You decide not to pawn " + query_multiple_short( kept ) +
            ", as you are keeping " + ( sizeof(kept) > 1 ? "them" : "it" )
            + ".\n" ) ;
        in_obs -= kept ;
    }

    /* Check for held or worn items */
    worn = filter( in_obs, (: $1->query_holder() || $1->query_worn_by() :) ) ;
    if ( sizeof(worn) ) {
        tell_object( TP, "You decide not to pawn "
            + query_multiple_short( worn ) + ", because you are wearing or "
            "holding " + ( sizeof(worn) > 1 ? "them" : "it" ) + ".\n" ) ;
        in_obs -= worn ;
    }

    /* Reject stolen items. */
    stolen = filter( in_obs, (: $1->query_property( "stolen" ) :) ) ;
    if ( sizeof(stolen) ) {
        tell_object( TP,
            "You cannot pawn " + query_multiple_short( stolen ) +
            " because " + ( sizeof(stolen) > 1 ? "they're" : "it's" ) +
            " stolen!\n" ) ;
        in_obs -= stolen ;
    }

    /* Check for items that can't be sold, or that we don't buy */
    nobuy = filter( in_obs, (: $1->do_not_sell() || TO->do_not_buy($1) :) ) ;
    if ( sizeof( nobuy ) ) {
        tell_object( TP,
            "You cannot pawn " + query_multiple_short( nobuy ) + ".\n" ) ;
        in_obs -= nobuy ;
    }
    
    /* Check for items that are too cheap */
    cheap = filter( in_obs, (: $1->query_value() < TO->query_min_amount() :) ) ;
    if ( sizeof(cheap) ) {
        tell_object( TP,
            "You cannot pawn " + query_multiple_short( cheap ) +
            " as " + ( sizeof(cheap) > 1 ? "they are" : "it is" ) +
            " not expensive enough.\n" ) ;
        in_obs -= cheap ;
    }
    
    /* Check for items that are too expensive */
    expensive = filter( in_obs,
        (: $1->query_value() > TO->query_max_amount() :) ) ;
    if ( sizeof(expensive) ) {
        tell_object( TP,
            "You cannot pawn " + query_multiple_short( expensive ) +
            " as " + ( sizeof(expensive) > 1 ? "they are" : "it is" ) +
            " too expensive.\n" ) ;
        in_obs -= expensive ;
    }
    
    /* Make sure we have something left to pawn after all that */
    if ( ! sizeof( in_obs ) ) {
        add_failed_mess( "You have nothing to pawn.\n" ) ;
        return 0 ;
    }

    /* Okay, we've passed all the checks.
     * Let's do some pawning.
     */

    /* Prepare for the sale - tally up how much money we have to give them,
     * give them the receipt(s), then zap the originals.
     */
    foreach( ob in in_obs ) {
        /* To get the value of the item, we use the same procedure as general
         * stores do for items that they don't specialise in.  This is so that
         * it doesn't become more worthwhile to pawn an object than to take it
         * to a general store.
         */
        value = scaled_value( ob->query_value() ) ;
        value = ( value * 90 ) / 100;
        total += value ;
        
        receipt = clone_object( RECEIPT ) ;
        if ( ! receipt ) {
            add_failed_mess(
                "Oh dear, something went very wrong (can't clone receipt object).  "
                "Please bugrep this room.\n" ) ;
            return 0 ;
        }
        receipt->setup_receipt( ob ) ;
        receipt->set_shop_path( TO ) ;
        receipt->set_item_value( value ) ;
        receipt->set_expiry_time( time() + query_pawn_ttl() ) ;
        receipt->setup_read_mess( _shop_language );
        receipt->move( TP ) ;
        ob->move( "/room/rubbish" ) ;
    }
    
    /* Get the currency in use */
    place = query_property( "place" ) ;
    if ( ! place || ( place == "" ) ) {
        place = "default" ;
    }

    /* Transfer some money to the mark^Wpawner */
    m_array = MONEY_HAND->create_money_array(total, place);
    money = clone_object(MONEY_OBJECT);
    if ( ! money ) {
        add_failed_mess(
            "Oh dear, something went very wrong (can't clone money object).  "
            "Please bugrep this room.\n" ) ;
        return 0 ;
    }
    money->set_money_array(m_array);

    if ( money->move( TP ) != MOVE_OK ) {
        tell_object( TP,
            "You're too heavily burdened to accept all that money, so the "
            "shopkeeper puts it on the floor.\n" ) ;
        money->move( TO ) ;
    }
    
    do_parse( _pawn_mess, in_obs, TP,
        MONEY_HAND->money_string( m_array ), "" ) ;
    
    return 1 ;
} /* do_pawn */


/** @ignore yes
 * Pawn shops don't just buy things.
 */
int do_sell( object *in_obs ) {
    add_failed_mess(
        "If you just want to sell items, you'll need to find a general "
        "store.  Pawn shops are for pawning in.\n" ) ;
    return 0 ;
} /* do_buy */
