/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: peddler.c,v 1.33 2003/04/23 09:36:04 runtime Exp $
 *
 */
/**
 * This is the peddler object, it acts as a walking talking item
 * shop.
 * Added event_enter so to stop food from decaying when the peddler
 * sells it. Siel, October 2001
 *
 */
#include <armoury.h>
#include <money.h>
#include <move_failures.h>
#include <virtual.h>

#define MAX_PROP "max per reset"
#define TYPE "item shop type"
#define NUM_SOLD "number removed this reset"
#define DISPLAY "display in shop"

inherit "/obj/monster";
inherit "/std/shops/inherit/clone_on_demand";

private string _none_left_mess;
private string _out_of_item_mess;
private string _show_item_mess;
private string _missing_punter_mess;
private string _dont_sell_mess;
private string _all_out_mess;
private string _list_mess;
private string _list_item_mess;
private string _no_stock_mess;
private string _too_poor_mess;
private string _too_heavy_mess;
private string _in_combat_mess;
private string _busy_mess;
private int busy;

void create() {

    _none_left_mess      = "I'm afraid I don't have any of those.";
    _out_of_item_mess    = "We are right out of $item$.";
    _show_item_mess      = "$item$ is priced at $cost$.  Let me show it to "
        "you.";
    _missing_punter_mess = "Hey... Where'd the fellow go?  Anyway...";
    _dont_sell_mess      = "I'm afraid I don't have any $item$.";
    _all_out_mess        = "I'm afraid I have no $item$ left.";
    _list_mess           = "I have the following items for sale:";
    _list_item_mess      = "I have $num_left$ $display$ for $cost$.";
    _no_stock_mess       = "I am afraid I have nothing for sale.";
    _too_poor_mess       = "I'm afraid you can't afford to buy $item$.";
    _too_heavy_mess      = "Well, here's $item$, but you can't carry it at "
        "the moment.  I'll put it on the floor.";
    _in_combat_mess      = "Can't you see I'm a bit preoccupied right "
                           "now?";
    _busy_mess           = "Sorry, I can only handle one purchase at a "
                           "time!" ;
    monster::create();

} /* create() */


/** @ignore yes */
void init() {
    object cont = query_cont();

    ::init();
    if( cont ) {
        this_player()->add_command( "browse", this_object(),
                                "<indirect:object:" + file_name(cont) +
                                "> {from|of} <direct:living:here>" );
        this_player()->add_command( "buy", this_object(),
                                "<indirect:object:" + file_name(cont) +
                                "> from <direct:living:here>" );
        this_player()->add_command( "list", this_object(),
                                "goods {from|of} <direct:living:here>" );
    }

} /* init() */


/** @ignore yes */
void check_cont() {
    object cont;
    ::check_cont();
    cont = query_cont();
    cont->set_name( "peddler_store" );
} /* check_cont() */


/**
 * This method returns the cost of the specified item to the buyer.
 * @param thing the thing to cost
 * @param buyer who is buying it
 * @return the cost of the item
 */
int query_cost( object thing, object buyer ) {

   if( thing->query_property("cost here") ) {
        return (int)thing->query_property("cost here");
    }
    else {
       return (int)thing->query_value_at( this_object() );
    }

} /* query_cost() */


/**
 * This method returns the costs of the item as a string.
 * @param thing the thing to buy
 * @param place the money area it is being bought in
 * @param buyer the person who is buying the object
 * @return the string money value
 */
string cost_string( object thing, string place, object buyer ) {
    return (string)MONEY_HAND->money_value_string(
        query_cost( thing, buyer ), place );
} /* cost_string() */


/**
 * The main entrace to the browse for things command.
 * @return 1 on success, 0 on failure
 */
int do_browse( mixed indirect_obs, string dir_match, string indir_match,
    string *words ) {

    int num_left;
    int i;
    string place;
    object *things;
    string custom_string;

    if( this_object()->query_fighting() ) {
        queue_command( "sayto " + file_name(this_player()) + " " +
            _in_combat_mess, 2 );
        this_player()->add_succeeded_mess( this_object(), "$N ask$s $D "
            "about browsing an item.\n", ({ }) );
        return 1;
    }

    place = query_property( "place" );
    if ( !place || ( place == "" ) ) {
        place = "default";
    }
    check_cont();
    things = indirect_obs;
    if( !sizeof( things ) ) {
        queue_command( "sayto " + file_name(this_player()) + " " +
            _none_left_mess, 2 );
    }
    else {
        for ( i = 0; i < sizeof( things ); i++ ) {
            num_left = things[i]->query_property( MAX_PROP ) -
                things[i]->query_property( NUM_SOLD );
            if (num_left <= 0) {
                custom_string = replace( _out_of_item_mess, ({
                    "$item$", things[i]->one_short()
                }) );
                queue_command( "sayto " + file_name(this_player()) +
                    custom_string);
                continue;
            }

            custom_string = replace( _show_item_mess, ({
                "$item$", "$C$" +
                    strip_colours((string)things[i]->the_short()),
                "$cost$", cost_string( things[i], place,
                    this_player() )
            }) );
            queue_command( "sayto " + file_name(this_player()) + " " +
                custom_string, 4 * i + 2 );

            // Added in by Oaf 11/99 - peddler doesn't have item in
            // inventory, so just tell the player the long() of the item.
            queue_command( "whisper " +
                things[i]->one_short()+":\n"+things[i]->long() + " to " +
                file_name(this_player()) );

            /*
            init_command("show " + things[i]->query_name() + " to " +
                this_player()->query_name(), 4*i+3);
            */
            /*
            call_out("show_it", 4*i+3, evaluate(things[i]->the_short()),
                evaluate(things[ i ]->long()));
            */
        }
    }

    this_player()->add_succeeded_mess( this_object(), "$N ask$s $D "
        "about "+ query_multiple_short(things) +".\n", ({ }) );
    return 1;

} /* do_browse() */


/**
 * This method shows the object in question to the player.
 * @param short the short description
 * @param long the long description
void show_it(string short, string long){
    if(environment(this_player()) != environment(this_object())){
        queue_command("'" + _missing_punter_mess);
        return;
    }
    tell_room(environment(this_object()), capitalize(evaluate(the_short()))+
        " shows "+this_player()->the_short()+" "+short+ ".\n");
    tell_object(this_player(), long);

} * show_it() */

/**
 * The main entrace to the buy things command.
 * @return 1 on success, 0 on failure
 */
int do_buy( mixed indirect_obs, string dir_match, string indir_match,
      string *words ) {

    int i, num_left;
    object *things;
    string custom_string;

    if(busy) {
      do_command("'" + _busy_mess);
      this_player()->add_succeeded_mess( this_object(), "$N confuse$s "
                     "$D, who is busy making business.\n", ({ }));
      return 1;
    }
    busy = 1;

    check_cont();
    things = indirect_obs;

    if ( !sizeof( things ) ) {
        custom_string = replace( _dont_sell_mess, ({
            "$item$", indir_match
        }) );
        queue_command( "sayto " + file_name(this_player()) + " " +
            custom_string);
    }
    else {
        if( this_object()->query_fighting() ) {
           queue_command( "sayto " + file_name(this_player()) + " " +
                          _in_combat_mess, 2 );
           this_player()->add_succeeded_mess( this_object(), "$N ask$s $D "
               "about buying "+ query_multiple_short(things) +".\n", ({ }) );
           busy = 0;
           return 1;
        }

        for ( i = 0; i < sizeof( things ); ++i ) {
            num_left = (int)things[ i ]->query_property( MAX_PROP ) -
                (int)things[ i ]->query_property( NUM_SOLD );
            if ( num_left < 1 ) {
                custom_string = replace( _all_out_mess, ({
                    "$item$", strip_colours((string)things[i]->query_plural())
                }) );
                init_command( "sayto " + file_name(this_player()) + " " +
                    custom_string, 2 * i + 2 );
                busy = 0;
                continue;
            }

            call_out( "sell_thing", 2 * i, this_player(), things[ i ] );
        }
    }

    this_player()->add_succeeded_mess( this_object(), "$N ask$s $D "
        "about buying "+ query_multiple_short(things) +".\n", ({ }) );
    return 1;

} /* do_buy() */


/**
 * The main entrace to the list stuff command.
 * @return 1 on success, 0 on failure
 */
int do_list() {
    int num_left;
    string place;
    object thing;
    int first;
    object cont;
    string display;
    string custom_string;

    if( this_object()->query_fighting() ) {
        queue_command( "sayto " + file_name(this_player()) + " " +
            _in_combat_mess, 2 );
        this_player()->add_succeeded_mess( this_object(), "" );
        return 1;
    }

    place = query_property( "place" );

    if ( !place || ( place == "" ) ) {
        place = "default";
    }

    check_cont();
    cont = query_cont();

    thing = first_inventory( cont );
    first = 1;

    while ( thing ) {
        num_left = (int)thing->query_property( MAX_PROP ) -
            (int)thing->query_property( NUM_SOLD );
        if ( num_left < 1 ) {
            thing = next_inventory( thing );
            continue;
        }
        if (first) {
            queue_command( "sayto " + file_name(this_player()) + " " +
                _list_mess);
            first = 0;
        }

        /* Make it use the display property added with add_object, if it
            exists.  Else use the short.  Pluralize if neccessary */
        display = (string)thing->query_property( DISPLAY );


        if ( !stringp( display )  && thing->short()) {
            if( num_left > 1 ) {
                display = (string)thing->query_plural();
            }
            else {
                display = (string)thing->a_short();
            }
        }
        else {
            if( num_left > 1 ) {
                display = pluralize( display );
            }
        }

        custom_string = replace( _list_item_mess, ({
            "$num_left$", (num_left > 1 ? query_num (num_left) : ""),
//            "$num_left$", query_num (num_left),
            "$display$", display,
            "$cost$", cost_string( thing, place, this_player() ) +
                 (num_left > 1 ? " each" : "" )
        }) );

        queue_command( "sayto " + file_name( this_player() ) +
                    " " + custom_string );
        thing = next_inventory( thing );
    }

    if ( first ) {
        queue_command( "say " + _no_stock_mess);
    }

    this_player()->add_succeeded_mess( this_object(), "" );
    return 1;

} /* do_list() */


/**
 * The main entrace to the sell stuff command.
 * @return 1 on success, 0 on failure
 */
void sell_thing( object player, object thing ) {
    int value;
    string item_name, place;
    object copy;
    string custom_string;

    if ( !player ) {
        busy = 0;
        return;
    }
    if ( environment( player ) != environment( this_object() ) ) {
        busy = 0;
        return;
    }
    if ( !thing ) {
        busy = 0;
        return;
    }

    place = query_property( "place" );
    if ( !place || ( place == "" ) ) {
        place = "default";
    }

    value = (int)player->query_value_in( place );
    if ( place != "default" ) {
        value += (int)player->query_value_in( "default" );
    }

    // Make sure we are making a cost based on the non-cloned object.
    if ( (int)this_object()->query_cost( thing, player ) > value ) {
        custom_string = replace( _too_poor_mess, ({
            "$item$", (string)thing->a_short()
        }) );
        queue_command( "sayto " + file_name(player) + " " +
                       custom_string );
        busy = 0;
        return;
    }

    // So the copy is only made if the item is actually sold.
    copy = (object)this_object()->create_real_object( thing );
    if ( !copy ) {
        switch ( (string)thing->query_property( TYPE ) ) {
            case "armour" :
            case "weapon" :
            case "armoury item":
                copy = ARMOURY->request_item( item_name,
                    80 + random( 20 ) );
                break;
            case "object" :
                copy = clone_object( item_name );
                break;
        }
    }
    if ( !copy ) {
        copy = clone_object( explode( file_name( thing ), "#" )[ 0 ] );
    }
    if ( !copy ) {
        busy = 0;
        return;
    }

    thing->add_property( NUM_SOLD, (int)thing->query_property( NUM_SOLD ) +
        1 );
    player->pay_money( (mixed *)MONEY_HAND->create_money_array(
        (int)this_object()->query_cost( copy, player ), place ), place );
    tell_object( player, "You pay "+ the_short() +" "+
        cost_string( copy, place, player ) +".\n" );
    tell_room( environment(), (string)player->one_short() +
        " gives "+ the_short() +" some money.\n", player );

    if ( (int)copy->move( player ) != MOVE_OK ) {
        custom_string = replace( _too_heavy_mess, ({
            "$item$", (string)copy->the_short()
        }) );
        queue_command( "sayto " + file_name(player) + " " + custom_string );
        copy->move( environment() );
        queue_command( ":puts "+ (string)copy->a_short() +" on the ground." );
    }
    else {
        tell_room( environment(), the_short() +" gives "+
            (string)player->one_short() +" "+ (string)copy->a_short() +
            ".\n" );
    }
    busy = 0;

} /* sell_thing() */

void event_enter(object ob, string mess, object from) {
    if (ob->query_food_object()) { ob->set_decay_speed(0); }
} /* event_enter() */


void event_exit(object ob, string message, object to) {
    if (ob->query_food_object()) { ob->set_decay_speed(8000); }
} /* event_exit() */


void dest_me() {
  if(query_cont())
    log_file("PEDDLER", "Cont: %s\n", file_name(query_cont()));
    clone_on_demand::dest_me();
    monster::dest_me();
} /* dest_me() */


/* CHAT OVERRIDE FUNCTIONS, Dasquian 8/7/02 */


/**
 * Overrides the default reply to having none left
 * @param s the chat to replace the default with
 */
void set_none_left_mess(string s) {
    _none_left_mess = s;
} /* set_none_left_mess() */

/**
 * Overrides the default chat of being out of an item that is browsed
 * @param s the chat to replace the default with.  Must include $item$, which
 * will be replaced by the item short.
 */
void set_out_of_item_mess(string s) {
    _out_of_item_mess = s;
} /* set_out_of_item_mess() */

/**
 * Overrides the default chat of showing an item to a player
 * @param s the chat to replace the default with.  Must include $item$ and
 * $cost$, which will be replaced by the item short and the cost of the item
 * respectively.
 */
void set_show_item_mess(string s) {
    _show_item_mess = s;
} /* set_show_item_mess() */

/**
 * Overrides the default chat to the player having wandered off
 * @param s the chat to replace the default with
 */
void set_missing_punter_mess(string s) {
    _missing_punter_mess = s;
} /* set_missing_punter_mess() */

/**
 * Overrides the default chat of the peddler not selling an item
 * @param s the chat to replace the default with.  Must include $item$, which
 * will be replaced by the item short.
 */
void set_dont_sell_mess(string s) {
    _dont_sell_mess = s;
} /* set_dont_sell_mess() */

/**
 * Overrides the default chat of the item being out of stock when a player
 * tries to buy it
 * @param s the chat to replace the default with.  Must include $item$, which
 * will be replaced by the item short.
 */
void set_all_out_mess(string s) {
    _all_out_mess = s;
} /* set_all_out_mess() */

/**
 * Overrides the default chat to announcing the stock list
 * @param s the chat to replace the default with
 */
void set_list_mess(string s) {
    _list_mess = s;
} /* set_list_mess() */

/**
 * Overrides the default chat of listing a stock item's price and stock
 * @param s the chat to replace the default with.  Must include $num_left$,
 * $display$ and $cost$, which will be replaced by the appropriate
 * strings.
 */
void set_list_item_mess(string s) {
    _list_item_mess = s;
} /* set_list_item_mess() */

/**
 * Overrides the default reply to having no listable stock
 * @param s the chat to replace the default with
 */
void set_no_stock_mess(string s) {
    _no_stock_mess = s;
} /* set_no_stock_mess() */

/**
 * Overrides the default chat of the player being too poor to buy an item
 * @param s the chat to replace the default with.  Must include $item$, which
 * will be replaced by the item short.
 */
void set_too_poor_mess(string s) {
    _too_poor_mess = s;
} /* set_too_poor_mess() */

/**
 * Overrides the default chat of the player being too burdened to carry an
 * item
 * @param s the chat to replace the default with.  Must include $item$, which
 * will be replaced by the item short.
 */
void set_too_heavy_mess(string s) {
    _too_heavy_mess = s;
} /* set_too_heavy_mess() */

/**
 * Overrides the default chat of the peddler being in combat and hence unable
 * to sell his wares!
 * @param s the chat to replace the default with
 */
void set_in_combat_mess(string s) {
    _in_combat_mess = s;
} /* set_in_combat_mess() */

/**
 * Overrides the default chat of the peddler being busy selling
 * something already.
 * @param s the chat to replace the default with
 */
void set_busy_mess(string s) {
    _busy_mess = s;
} /* set_busy_mess() */
