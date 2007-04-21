/* -*- LPC -*- */
/*
 * $Locker:  $
 * $Id: general_shop.c,v 1.19 2002/08/03 05:09:38 presto Exp $
 *
 */

/**
 * This the base shop.  It buys and sells stuff form players.
 * <p>
 * A storeroom must be set for the shop!
 * <p>
 * Original made who knows when.
 * @author Pinkfish
 * @see set_storeroom()
 * @change bil
 * to make the list a lot nicer.
 * @change Pinkfish
 * to give shops types and make them send out
 * reps to sell/buy things from other shops.
 * @change Ceres
 * to add burglable storerooms.
 * @change 23-11-97, Gototh
 * to add buy, sell, list, browse, value,
 * cost_too_muchy and not_worthy functions.
*/

#include <armoury.h>
#include <money.h>
#include <move_failures.h>
#include <shop.h>
#include <tasks.h>

inherit "/std/room/basic_room";
inherit "/std/shops/inherit/open_shop";

private mixed our_storeroom;
private mixed buy_mess;
private mixed sell_mess;
private mixed list_mess;
private mixed value_mess;
private mixed too_costly_mess;
private mixed not_worthy_mess;
private mixed browse_mess;
private mixed *other_shops;

private int amount_sold;
private int amount_bought;
private int strict_shop;
private int no_steal;
private int sell_stolen;
private int steal_difficulty;
private int min_amount;
private int max_inventory;
private int sell_large;

private mixed shop_type;
private mixed buy_func;
private mixed sell_func;
private mixed value_func;
private mixed too_costly_func;
private mixed cannot_sell_func;
private mixed browse_func;
private mixed list_func;

private nosave string shoplift_handler;

private string original_storeroom;

private int _strict_currency;

private nosave mapping elist = (["north" : "south", "south" : "north", 
  "east" : "west", "west" : "east", "up" : "down", "down" : "up"]);

string shop_list(mixed arr, int detail);
string shop_parse(string str, mixed ob, object client, string money,
  string extra, string which);
string find_free_exit();
int do_buy(object *ob);
int do_list_these(object *obs);
void do_buy_things( object *obs, int cost, object pl );
void do_parse(mixed arr, mixed ob, object client, string money, 
  string extra);
object create_mercenary(object rep);
void event_shoplift(object command_ob, object thief, object victim);
object query_store_room();
int creator_object (object);

void create() {
    buy_mess = ({
      "You buy $ob$ for $money$.\n",
      "$client$ buys $ob$.\n"});
    sell_mess = ({
      "You sell $ob$ for $money$.\n",
      "$client$ sells $ob$.\n"});
    list_mess = "$extra$";
    value_mess = "$ob$ is valued at $money$.\n";
    too_costly_mess = "$ob$ is worth too much to be sold here.\n";
    not_worthy_mess = "$ob$ is not worth enough to be sold here.\n";
    browse_mess = "$ob$ costs $money$, it looks like:\n$extra$";
    other_shops = ({ });
    max_inventory = MAX_INVENTORY;
    min_amount = 50;
    add_help_file("shop");
    _strict_currency = 1;
    ::create();
} /* create() */

/** @ignore yes */
void reset() {
    if(!random(3)) {
        remove_property("inventory_loss"); // This is incremented by shoplifting
    }
    call_out("send_out_reps", 2);
} /* reset() */

/**
 * This method sets the function to call when buying something.  The
 * function will be called with two elements, the first being the
 * player doing the buying and the second being the array
 * of objects being bought.
 * <p>
 * If the function is a string then the function of that name
 * will be called on the shop, if it is a function pointer
 * then the function pointer will be evaluated.
 * @param func the function to call
 * @see set_sell_function()
 * @see set_value_function()
 * @see set_too_costly_function()
 * @see set_cannot_sell_function() 
 * @see set_browse_function()
 * @see set_list_function()
 */
void set_buy_function(mixed func) {
    if (stringp(func) || functionp(func)) {
        buy_func = func;
    }
} /* set_buy_function() */

/**
 * This method sets the function to call when selling something.  The
 * function will be called with two elements, the first being the
 * player doing the selling and the second being the array
 * of objects being sold.
 * <p>
 * If the function is a string then the function of that name
 * will be called on the shop, if it is a function pointer
 * then the function pointer will be evaluated.
 * @param func the function to call
 * @see set_buy_function()
 * @see set_value_function()
 * @see set_too_costly_function()
 * @see set_cannot_sell_function() 
 * @see set_browse_function()
 * @see set_list_function()
 */
void set_sell_function(mixed func) {
    if (stringp(func) || functionp(func)) {
        sell_func = func;
    }
} /* set_sell_function() */

/**
 * This method sets the function to call when an item(*s) are being
 * valued.  The
 * function will be called with three elements, the first being the
 * player doing the valueing and the second being the array
 * of objects being valued and the third being the string
 * value of the objects.
 * <p>
 * If the function is a string then the function of that name
 * will be called on the shop, if it is a function pointer
 * then the function pointer will be evaluated.
 * @param func the function to call
 * @see set_sell_function()
 * @see set_buy_function()
 * @see set_too_costly_function()
 * @see set_cannot_sell_function() 
 * @see set_browse_function()
 * @see set_list_function()
 */
void set_value_function(mixed func) {
    if (stringp(func) || functionp(func)) {
        value_func = func;
    }
} /* set_value_function() */

/**
 * This method sets the function to call when buying something and it
 * costs too much.  The
 * function will be called with two elements, the first being the
 * player doing the buying and the second being the array
 * of objects which cost too much.
 * <p>
 * If the function is a string then the function of that name
 * will be called on the shop, if it is a function pointer
 * then the function pointer will be evaluated.
 * @param func the function to call
 * @see set_sell_function()
 * @see set_value_function()
 * @see set_buy_function()
 * @see set_cannot_sell_function() 
 * @see set_browse_function()
 * @see set_list_function()
 */
void set_too_costly_function(mixed func) {
    if (stringp(func) || functionp(func)) {
        too_costly_func = func;
    }
} /* set_too_costly_function() */

/**
 * This method sets the function to call when selling something
 * which fails for some reason.  The
 * function will be called with two elements, the first being the
 * player doing the selling and the second being the array
 * of objects being sold.
 * <p>
 * If the function is a string then the function of that name
 * will be called on the shop, if it is a function pointer
 * then the function pointer will be evaluated.
 * @param func the function to call
 * @see set_sell_function()
 * @see set_value_function()
 * @see set_too_costly_function()
 * @see set_buy_function() 
 * @see set_browse_function()
 * @see set_list_function()
 */
void set_cannot_sell_function(mixed func) {
    if (stringp(func) || functionp(func)) {
        cannot_sell_func = func;
    }
} /* set_cannot_sell_function() */

/**
 * This method sets the function to call when browsing something.  The
 * function will be called with two elements, the first being the
 * player doing the browsing and the second being the array
 * of objects being browsed.
 * <p>
 * If the function is a string then the function of that name
 * will be called on the shop, if it is a function pointer
 * then the function pointer will be evaluated.
 * @param func the function to call
 * @see set_sell_function()
 * @see set_value_function()
 * @see set_too_costly_function()
 * @see set_cannot_sell_function() 
 * @see set_browse_function()
 * @see set_list_function()
 */
void set_browse_function(mixed func) {
    if (stringp(func) || functionp(func)) {
        browse_func = func;
    }
} /* set_browse_function() */

/**
 * This method sets the function to call when listing something.  The
 * function will be called with two elements, the first being the
 * player doing the listing and the second being the array
 * of objects being listed.
 * <p>
 * If the function is a string then the function of that name
 * will be called on the shop, if it is a function pointer
 * then the function pointer will be evaluated.
 * @param func the function to call
 * @see set_sell_function()
 * @see set_value_function()
 * @see set_too_costly_function()
 * @see set_cannot_sell_function() 
 * @see set_browse_function()
 * @see set_list_function()
 */
void set_list_function(mixed func) {
    if (stringp(func) || functionp(func)) {
        list_func = func;
    }
} /* set_list_function() */

/**
 * This sets the message which will be told to the players when
 * they sell something at the shop.  If the parameter is a string
 * then that message is sent to the player and nothing is sent to
 * the other people in the room.  If the message is a two element
 * array, the first element is sent to the player and the second
 * element is sent to the others in the room.  In both these
 * cases the pattersn $ob$ will be replaces with the objects
 * being sold, $client$ will be replaced with the client, $money$
 * will be replaced with the money information, $extra$ will be replaces
 * with any extra information.
 * <p>
 * If the parameter is a function pointer, then this is called with
 * the parameter func(obs, client, money, extra);
 * @param str the message to print
 * @see set_buy_message()
 * @see query_sell_message()
 * @see set_set_value_message()
 * @see set_too_costly_message()
 * @see set_no_worthy_message()
 * @see set_browse_message() 
 * @see set_list_message()
 * @example
 * set_sell_message("You sell $ob$ for $money$.\n");
 * @example
 * set_sell_message( ({ "You sell $ob$ for $money$.\n",
 *                      "$client$ sells $ob$.\n"});
 */
void set_sell_message(mixed str) {
    sell_mess = str;
} /* set_sell_message() */

/**
 * This sets the message which will be told to the players when
 * they buy something at the shop.  If the parameter is a string
 * then that message is sent to the player and nothing is sent to
 * the other people in the room.  If the message is a two element
 * array, the first element is sent to the player and the second
 * element is sent to the others in the room.  In both these
 * cases the pattersn $ob$ will be replaces with the objects
 * being sold, $client$ will be replaced with the client, $money$
 * will be replaced with the money information, $extra$ will be replaces
 * with any extra information.
 * <p>
 * If the parameter is a function pointer, then this is called with
 * the parameter func(obs, client, money, extra);
 * @param str the message to print
 * @see set_sell_message()
 * @see query_buy_message()
 * @see set_set_value_message()
 * @see set_too_costly_message()
 * @see set_no_worthy_message()
 * @see set_browse_message() 
 * @see set_list_message()
 * @example
 * set_buy_message("You buy $ob$ for $money$.\n");
 * @example
 * set_buy_message( ({ "You buy $ob$ for $money$.\n",
 *                      "$client$ buys $ob$.\n"});
 */
void set_buy_message(mixed str) {
    buy_mess = str;
} /* set_buy_message() */

/**
 * This sets the message which will be told to the players when
 * they value something at the shop.  If the parameter is a string
 * then that message is sent to the player and nothing is sent to
 * the other people in the room.  If the message is a two element
 * array, the first element is sent to the player and the second
 * element is sent to the others in the room.  In both these
 * cases the pattersn $ob$ will be replaces with the objects
 * being sold, $client$ will be replaced with the client, $money$
 * will be replaced with the money information, $extra$ will be replaces
 * with any extra information.
 * <p>
 * If the parameter is a function pointer, then this is called with
 * the parameter func(obs, client, money, extra);
 * @param str the message to print
 * @see set_buy_message()
 * @see query_value_message()
 * @see set_set_value_message()
 * @see set_too_costly_message()
 * @see set_no_worthy_message()
 * @see set_browse_message() 
 * @see set_list_message()
 * @example
 * set_value_message("$ob$ is valued at $money$.\n");
 * @example
 * set_sell_message( ({ "$ob$ is valued at $money$.\n",
 *                      "$client$ values $ob$.\n"});
 */
void set_value_message(mixed str) {
    value_mess = str;
} /* set_value_message() */

/**
 * This sets the message which will be told to the players when
 * they buy something at the shop and it costs too much.
 * If the parameter is a string
 * then that message is sent to the player and nothing is sent to
 * the other people in the room.  If the message is a two element
 * array, the first element is sent to the player and the second
 * element is sent to the others in the room.  In both these
 * cases the pattersn $ob$ will be replaces with the objects
 * being sold, $client$ will be replaced with the client, $money$
 * will be replaced with the money information, $extra$ will be replaces
 * with any extra information.
 * <p>
 * If the parameter is a function pointer, then this is called with
 * the parameter func(obs, client, money, extra);
 * @param str the message to print
 * @see set_buy_message()
 * @see query_too_costly_message()
 * @see set_set_value_message()
 * @see set_browse_message()
 * @see set_no_worthy_message()
 * @see set_sell_message() 
 * @see set_list_message()
 * @example
 * set_browse_message("$ob$ is worth too much to be sold here.\n");
 * @example
 * set_browse_message( ({ "$ob$ is worth too much to be sold here.\n",
 *                      "$client$ tries to sell the terribly expensive $ob$.\n"});
 */
void set_too_costly_message(mixed str) {
    too_costly_mess = str;
} /* set_too_costly_message() */

/**
 * This sets the message which will be told to the players when
 * they sell soemthign that is not worth enough at the shop. 
 * If the parameter is a string
 * then that message is sent to the player and nothing is sent to
 * the other people in the room.  If the message is a two element
 * array, the first element is sent to the player and the second
 * element is sent to the others in the room.  In both these
 * cases the pattersn $ob$ will be replaces with the objects
 * being sold, $client$ will be replaced with the client, $money$
 * will be replaced with the money information, $extra$ will be replaces
 * with any extra information.
 * <p>
 * If the parameter is a function pointer, then this is called with
 * the parameter func(obs, client, money, extra);
 * @param str the message to print
 * @see set_buy_message()
 * @see query_not_worthy_message()
 * @see set_set_value_message()
 * @see set_too_costly_message()
 * @see set_browse_message()
 * @see set_sell_message() 
 * @see set_list_message()
 * @example
 * set_not_worthy_message("$ob$ is not worth enough to be sold here.\n");
 * @example
 * set_not_worthy_message( ({ "$ob$ is not worth enough to be sold here.\n",
 *                      "$client$ tries to sell the rubbishy $ob$.\n"});
 */
void set_not_worthy_message(mixed str) {
    not_worthy_mess = str;
} /* set_not_worthy_message() */

/**
 * This sets the message which will be told to the players when
 * they browse something at the shop.  If the parameter is a string
 * then that message is sent to the player and nothing is sent to
 * the other people in the room.  If the message is a two element
 * array, the first element is sent to the player and the second
 * element is sent to the others in the room.  In both these
 * cases the pattersn $ob$ will be replaces with the objects
 * being sold, $client$ will be replaced with the client, $money$
 * will be replaced with the money information, $extra$ will be replaces
 * with any extra information.  In the case oif a browse the
 * extra information is the long description of the object.
 * <p>
 * If the parameter is a function pointer, then this is called with
 * the parameter func(obs, client, money, extra);
 * @param str the message to print
 * @see set_buy_message()
 * @see query_browse_message()
 * @see set_set_value_message()
 * @see set_too_costly_message()
 * @see set_no_worthy_message()
 * @see set_sell_message() 
 * @see set_list_message()
 * @example
 * set_browse_message("$ob$ costs $money$, it looks like:\n$extra$");
 * @example
 * set_browse_message( ({ "$ob$ costs $money$, it looks like:\n$extra$",
 *                      "$client$ browses $ob$.\n"});
 */
void set_browse_message(mixed str) {
    browse_mess = str;
} /* set_browse_message() */

/**
 * This sets the message which will be told to the players when
 * they list something at the shop.  If the parameter is a string
 * then that message is sent to the player and nothing is sent to
 * the other people in the room.  If the message is a two element
 * array, the first element is sent to the player and the second
 * element is sent to the others in the room.  In both these
 * cases the pattersn $ob$ will be replaces with the objects
 * being sold, $client$ will be replaced with the client, $money$
 * will be replaced with the money information, $extra$ will be replaces
 * with any extra information.  In the case of a list,
 * the extra information *is* the list.
 * <p>
 * If the parameter is a function pointer, then this is called with
 * the parameter func(obs, client, money, extra);
 * @param str the message to print
 * @see set_buy_message()
 * @see query_buy_message()
 * @see set_set_value_message()
 * @see set_too_costly_message()
 * @see set_no_worthy_message()
 * @see set_browse_message() 
 * @see set_list_message()
 * @example
 * set_list_message("$extra$.\n");
 * @example
 * set_list_message( ({ "You list $ob$ for $money$.\n",
 *                      "$client$ lists $ob$.\n"});
 */
void set_list_message(mixed str) {
    list_mess = str;
} /* set_list_message() */

/**
 * This sets the conditons which the shop will be open during.  If this
 * is set to an integer the shop will always be in that state, so
 * if you set the open condition to 0, it would always be closed.
 * If it is set to a string, then that function will be called on
 * this object to test to see if it is open.  If it iset to 
 * function pointer, the function pointer will be evaluated.  If it
 * is set to an array, the first element specifies the object and the
 * second specifies the function to call.
 * @see query_open_condition()
 * @see is_open()
 */
void set_open_condition(mixed str) {
   if (functionp(str)) {
      ::set_open_function(str);
   } else if (intp(str)) {
      ::set_open_function( (: $(str) :) );
   } else if (pointerp(str)) {
      ::set_open_function( (: call_other($(str[0]), $(str[1]), $1) :) );
   } if ( stringp(str) ) {
      ::set_open_function( (: call_other( this_object(), $(str), $1 ) :) );
   }
} /* set_open_condition() */

/**
 * This method sets the no steal property.  If a shop is set
 * as no steal, then it cannot be shoplifted.
 * @param i the new value of the no_steal property
 * @see set_steal_difficulty()
 * @see query_no_steal()
 * @see set_sell_stolen()
 */
void set_no_steal(int i) {
    no_steal = i;
} /* set_no_steal() */

/**
 * This method sets the difficulty at which to steal stuff from
 * this shop.
 * @see set_no_steal()
 * @see set_sell_stolen()
 * @see query_steal_difficulty()
 * @param i the new value for the steal difficulty
 */
void set_steal_difficulty(int i) {
    steal_difficulty = i;
} /* set_steal_difficulty() */

/**
 * This method sets the shop as a place which will receive and
 * seel stolen goods.  It automaticly sets the no_steal property
 * to 1.
 * @see set_no_steal()
 * @see set_steal_difficulty()
 * @see query_sell_stolen()
 * @param i the new value for the stolen property
 */
void set_sell_stolen(int i) {
    sell_stolen = i;
    no_steal = 1;
} /* set_sell_stolen() */

/**
 * This method sets the shop to sell very large objects. ie. If a player
 * cannot carry items they are placed in the room for the player rather
 * than not being sold to the player.
 */
void set_sell_large(int i) { sell_large = i; }

/**
 * This method returns the current sell message of the shop.
 * @see set_sell_mess()
 * @return the current sell message of the shop
 */
mixed query_sell_mess() {
    return sell_mess;
} /* query_sell_mess() */

/**
 * This method returns the current list message of the shop.
 * @see set_list_mess()
 * @return the current list message of the shop
 */
mixed query_list_mess() {
    return list_mess;
} /* query_list_mess() */

/**
 * This method returns the current value message of the shop.
 * @see set_value_mess()
 * @return the current value message of the shop
 */
mixed query_value_mess() {
    return value_mess;
} /* query_value_mess() */

/**
 * This method returns the current too costly message of the shop.
 * @see set_too costly_mess()
 * @return the current too costly message of the shop
 */
mixed query_too_costly_mess() {
    return too_costly_mess;
} /* query_too_costly_mess() */

/**
 * This method returns the current not worthy message of the shop.
 * @see set_not worthy_mess()
 * @return the current not worthy message of the shop
 */
mixed query_not_worthy_mess() {
    return not_worthy_mess;
} /* query_not_worthy_mess() */

/**
 * This method returns the current buy message of the shop.
 * @see set_buy_mess()
 * @return the current buy message of the shop
 */
mixed query_buy_mess() {
    return buy_mess;
} /* query_buy_mess() */

/**
 * This method returns the current browse message of the shop.
 * @see set_browse_mess()
 * @return the current browse message of the shop
 */
mixed query_browse_mess() {
    return browse_mess;
} /* query_browse_mess() */

/**
 * This method returns the current no steal property.
 * @see set_no_steal_mess()
 * @return the current no steal properyt of the shop
 */
int query_no_steal() {
    return no_steal;
} /* query_no_steal() */

/**
 * This method returns the current steal difficulty of the shop.
 * @see set_steal_difficulty_mess()
 * @return the current steal difficulty message of the shop
 */
int query_steal_difficulty(int i) {
    return steal_difficulty;
} /* query_steal_difficulty() */

/**
 * This method returns the shop lift response handler for the shop.
 * This allows the shop to respond in someway to someone shop
 * lifting stuff, like the heavys in Ankh-Morpork.
 * @returns the shop lift response handler
 * @see set_shoplift_response_handler()
 * @see set_no_steal()
 * @see set_sell_stolen()
 * @see set_steal_difficulty()
 */
string query_shoplift_response_handler() {
    return shoplift_handler;
} /* query_shoplift_response_handler() */

/**
 * This method tells us if the shop is really a shop or not.
 * @return 1 always
 */
int query_shop() {
    return 1;
} /* query_shop() */

/**
 * This method returns the items which can potentially be shop lifted
 * with the passed in string.
 * @param str the name for the object to attempt to shop lift
 * @return the array of matching objects
 * @see query_steal_difficulty()
 * @see query_shoplift_response_handler()
 */
object *query_shop_lift_items(string str, object player) {
    return match_objects_for_existence(str, ({ query_store_room() }));
} /* query_shop_lift_items() */

/**
 * This method turns the objects into real objects (if that is
 * nessessary, it is not with a normal shop).
 * @param ob the object to turn into a normal object
 */
object shoplift_success(object ob) {
    return ob;
} /* shoplift_success() */

/**
 * This method sets the shop lift response handler for the shop.
 * This allows the shop to respond in someway to shop lifting
 * stuff, like the heavies in Ankh-Morpork.  The function
 * 'handle_shoplift' will be called on the handler when the
 * shop lift is attempted.  It will be passed two arguments
 * the first is the thief, the second is the room being
 * shoplifted.  This can be set to  afucntion pointer
 * which will be evaluated and passed in two arguemtns
 * when  a shoplift occurs.
 * @see query_shoplift_response_handler()
 * @see set_no_steal()
 * @see set_sell_stolen()
 * @see set_steal_difficulty()
 * @param word the new shop lift response handler
 */
void set_shoplift_response_handler(string word) {
    shoplift_handler = word;
} /* set_shoplift_response_handler() */

/**
 * This method sets the minimum value of items that can be sold here.
 * @see query_min_amount()
 */
void set_min_amount(int i) { min_amount = i; }

/**
 * This method sets the maximum number of inventory items this shop will
 * take before it starts to refuse to purchase items from players and
 * deleting items from its inventory.
 * The default for this is defined as MAX_INVENTORY in shop.h
 */
void set_max_inventory(int i) { max_inventory = i; }

/** @ignore yes */
void init() {
    string room;

    ::init();
    if(stringp(our_storeroom)) {
        room = our_storeroom;
    } else if(our_storeroom) {
        room = file_name(our_storeroom);
    } else {
      tell_room(this_object(),
                "Oh dear, we don't seem to have a storeroom.\n");
    }
    add_command("sell", "<indirect:object:me>");
    add_command("buy", "<indirect:object:"+room+">");
    add_command("list", "[all]");
    add_command("list", "<indirect:object:"+room+">",
      (: do_list_these($1) :));
    add_command("browse", "<indirect:object:"+room+">");
    add_command("value", "<indirect:object:me>");
} /* init() */

/**
 * This method returns the maximum value oif an object that can
 * be sold here.
 * @see query_min_amount()
 * @return the maximum amount
 * @see /obj/handlers/money_handler.c
 */ 
int query_max_amount() {
    return MAX_AMOUNT;
} /* query_max_amount() */


/**
 * This method returns the minimum value oif an object that can
 * be sold here.
 * @see query_max_amount()
 * @return the minimum amount
 * @see /obj/handlers/money_handler.c
 */ 
int query_min_amount() {
    return min_amount;
} /* query_min_amount() */

/**
 * This method returns the value of the object in this shop.
 * @return the value of the object here
 * @param thing the thing to value
 * @param sell are we selling it?
 * @see query_max_amount()
 * @see query_min_amount()
 */
varargs int query_value(object thing, int sell) {

     // Only use the cost here property if we're not selling.
     if ( thing->query_property("cost here") && !sell ) {
         return (int)thing->query_property("cost here");
     }
     else {
        return (int)thing->query_value_at( this_object() );
     }
} /* query_value() */


/**
 * This returns the amount of money you can sell and object for
 * which is less than the amount it will be sold for).
 * @param n the value to scale
 * @return the sell value
 */
int scaled_value(int n) {
    int i, tot, *fish;

    fish = PAY_RATES;
    if(n < fish[0]) {
        return n;
    } else {
        tot = fish[0];
        n -= fish[0];
    }
    i = 0;
    while(n && ((i + 1) < sizeof(fish))) {
        if(n >= fish[i]) {
            tot += fish[i + 1] * fish[i] / 100;
            n -= fish[i];
        } else {
            tot += fish[i + 1] * n / 100;
            n = 0;
        }
        i += 2;
    }
    if(n) {
        tot += (n * fish[sizeof(fish) - 1]) / 100;
    }
    return tot;
} /* scaled_value() */

/**
 * This method does the actual selling.
 * @param in_obs the objects to sell
 * @return 1 on success, 0 on failure
 */
int do_sell(object *in_obs) {
    int i, amt, total_amt;
    string place, filename, *text = ({ });
    object money, *obs, *selling, *cannot, *stolen, storeob, one_item, *cre;
    object thing;
    mixed *m_array;

    if(!is_open(this_player(), 0)) {
        return 0;
    }
    
    in_obs = uniq_array( in_obs );

    obs = filter_array(in_obs, (: !$1->query_keep() :));
    cre = filter (obs, (: creator_object ($1) :));
    
    if (sizeof (cre) && strsrch (file_name (this_object()), 
      "_dev") == -1) {
      
      if (!this_player()->query_creator()) {
        tell_object (this_player(), "Oh dear, you shouldn't have " + 
          query_multiple_short (cre) + "!  They disappear with a flash "
          "of octarine light.\n");
        cre->move ("/room/rubbish");
      }
      else {
        tell_object (this_player(), "Oh dear, you shouldn't be trying to sell " + 
          query_multiple_short (cre) + "!\n");
      }        

      foreach (thing in cre) {
        if ( thing->query_property( "virtual name" ) ) {
          filename = thing->query_property( "virtual name" );
        } else {      
          filename = base_name (thing);          
        }
        
        text += ({filename});
      }
      
      log_file ("ILLEGAL_OBJECT", sprintf ("%s: %s tried to sell %s at "
        "%s.\n\n", ctime (time()), this_player()->query_name(),
        query_multiple_short (text), file_name (this_object())));


      obs -= cre;
   }
   
    if(!sizeof(obs)) {
        this_player()->add_failed_mess(this_object(),
          "You previously decided to keep $I.\n", in_obs);
        return 0;
    }

    in_obs = this_player()->query_holding() + this_player()->query_armours();
    cannot = filter(obs, (: member_array($1, $2) != -1 :), in_obs);
    if (sizeof(cannot)) {
        obs -= cannot;
        tell_object(this_player(), "You decide not to sell " + 
          query_multiple_short(cannot, "the") + ", because "
          "you are wearing or holding $V$0=it,them$V$.\n");
        /*     add_failed_mess("You cannot sell $I since you are wearing or holding "
                             "them.\n",
                             cannot);
             return 0; */
    }

    // If there are more items in the storeroom than max_inventory allows
    // return a failure message.
    if(objectp(our_storeroom)) {
        storeob = our_storeroom;
    } else {
        our_storeroom->rabbit_away();
        storeob = find_object(our_storeroom);
    }
    if(sizeof(all_inventory(storeob)) > max_inventory) {
        this_player()->add_failed_mess(this_object(),
          "Sorry, the shop is full up and isn't buying items!\n", ({ }));
        call_out("tidy_inventory", random(20));
        return 0;
    }

    if(sizeof(obs) > MAX_OBS) {
        write("The shopkeeper can't cope with all those objects.\n");
        obs = obs[0..MAX_OBS - 1];
    }
    selling = cannot = stolen = ({ });
    place = query_property("place");
    if(!place || (place == "")) {
        place = "default";
    }
    for(i = 0; i < sizeof(obs); i++) {
        if(!sell_stolen && obs[i]->query_property("stolen")) {
            stolen += ({obs[i]});
            continue;
        } else {
            obs[i]->remove_property("stolen");
        }
        if((this_object()->query_value(obs[i], 1) > 0) && 
          !obs[i]->do_not_sell() && 
          !this_object()->do_not_buy(obs[i]) && (!strict_shop || 
            shop_type == obs[i]->query_property("shop type")) && 
          environment(obs[i]) == this_player()) {
            if(obs[i]->move(our_storeroom)) {
                if(obs[i]->short()) {
                    cannot += ({obs[i]});
                }
                continue;
            }
            amt = (int)this_object()->query_value(obs[i], 1);
            if(!(obs[i]->query_property("sale_value"))) {
                amt = scaled_value( amt );
            }
            if((amt <= (int)this_object()->query_max_amount()) &&
              (amt >= (int)this_object()->query_min_amount())) {
                if(shop_type != obs[i]->query_property("shop type")) {
                    amt = (amt * 90) / 100;
                }
                total_amt += amt;
                selling += ({obs[i]});
                obs[i]->being_sold();
            } else {
                if(obs[i]->short())
                    cannot += ({obs[i]});
                obs[i]->move(this_player());
            }
        } else {
            if(obs[i]->short()) {
                cannot += ({obs[i]});
            }
        }
    }
    if (!sizeof(selling)) {
        if (sizeof(cannot)) {
            if (stringp(cannot_sell_func)) {
                call_other(this_object(), cannot_sell_func, this_player(), cannot);
            } else if (functionp(cannot_sell_func)) {
                evaluate(cannot_sell_func, this_player(), cannot);
            }
            this_player()->add_failed_mess(this_object(),
              "You cannot sell $I.\n", cannot);
        } else if(sizeof(stolen)) {
            if(sizeof(stolen) > 1) {
                this_player()->add_failed_mess(this_object(),
                  "You cannot sell $I because they're stolen!\n", stolen);
            } else {
                this_player()->add_failed_mess(this_object(),
                  "You cannot sell $I because it's stolen!\n", stolen);
            }
            foreach(one_item in obs) {
                if(one_item->query_property("stolen") == this_player()->query_name()) {
                    event_shoplift(this_object(), this_player(), this_object());
                    break;
                }
            }
        } else {
            this_player()->add_failed_mess(this_object(),
              "You have nothing to sell.\n", ({ }));
        }
        return 0;
    }
    if(this_object()->cannot_afford(total_amt)) {
        selling->move(this_player());
        this_player()->add_failed_mess(this_object(),
          "The shop cannot afford to buy $I from you.\n", selling);
        return 0;
    }
    amount_sold += total_amt;
    m_array = (mixed *)MONEY_HAND->create_money_array(total_amt, place);
    money = clone_object(MONEY_OBJECT);
    money->set_money_array(m_array);
    if(sizeof(cannot)) {
        if(stringp(cannot_sell_func)) {
            call_other(this_object(), cannot_sell_func, this_player(), cannot);
        } else if (functionp(cannot_sell_func)) {
            evaluate(cannot_sell_func, this_player(), cannot);
        }
        write("You cannot sell "+query_multiple_short(cannot)+".\n");
        cannot->move(this_player());
    }
    if(stringp(sell_func)) {
        call_other(this_object(), sell_func, this_player(), obs);
    } else if (functionp(sell_func)) {
        evaluate(sell_func, this_player(), obs);
    }
    do_parse(sell_mess, selling, this_player(), 
      (string)MONEY_HAND->money_string(m_array), "");
    if((int)money->move(this_player()) != MOVE_OK) {
        tell_object(this_player(), 
          "You're too heavily burdened to accept all that money, "
          "so the shopkeeper puts it on the floor.\n");
        money->move(this_object());
    }
    this_object()->made_transaction(-total_amt, selling);
    return 1;
} /* do_sell() */

/**
 * This method does the actual buying.
 * @param obs the objects to buy
 * @return 1 on success, 0 on failure
 */
/**
 * This method determines whether or not a given object is a creator object
 * and should be in this shop or not. 
 * @param ob the object to check
 * @return 1 if the object is not allowed, 0 if it is.
 */
int creator_object( object ob ) {
    string path;

    if ( file_name( ob )[0..2] == "/w/" ) {
        return 1;
    }

    if ( ( path = ob->query_property( "virtual name" ) ) && 
      path[0..2] == "/w/" ) {
        return 1;
    }

    return 0;
} /* creator_object() */

int do_buy(object *obs) {
    int i, amt, ob_amt, total_cost;
    string place;
    object money, *to_buy, *cannot, *too_much;
    object *creator_obs;

    if(!is_open(this_player(), 0)) {
        return 0;
    }
    if(sizeof(obs) > MAX_OBS) {
        write("The shopkeeper can't cope with all those objects.\n");
        obs = obs[0..MAX_OBS-1];
    }
    creator_obs = filter( obs, (: creator_object( $1 ) :) );
    if ( sizeof( creator_obs ) && file_name()[0..2] != "/w/" ) {
        tell_object( this_player(), "You cannot buy " + 
          query_multiple_short( creator_obs, "the" ) + 
          " because they shouldn't be in the game!\n" );
        obs -= creator_obs;
    }

    to_buy = too_much = cannot = ({ });
    place = query_property("place");
    if(!place || (place == "")) {
        place = "default";
    }
    money = present(MONEY_ALIAS, this_player());
    if(!money) {
        if(stringp(too_costly_func)) {
            call_other(this_object(), too_costly_func, this_player(), obs);
        } else if (functionp(too_costly_func)) {
            evaluate(too_costly_func, this_player(), obs);
        }
        this_player()->add_failed_mess(this_object(),
          "You have no money.\n", obs);
        return 0;
    }
    amt = money->query_value_in(place);
    if(place != "default" && !_strict_currency ) {
        amt += money->query_value_in("default");
    }
    while(i < sizeof(obs)) {
        ob_amt = this_object()->query_value(obs[i], 0);
        if(ob_amt > amt) {
            if(obs[i]->short()) {
                too_much += ({obs[i]});
            }
            obs = delete(obs, i, 1);
            continue;
        }
        if(obs[i]->move(this_player())) {
            if(!sell_large) {
                if(obs[i]->short()) {
                    cannot += ({obs[i]});
                }
                i++;
                continue;
            } else {
                obs[i]->move(this_object());
            }
        }
        amt -= ob_amt;
        total_cost += ob_amt;
        to_buy += ({obs[i]});
        i++;
    }
    amount_bought += total_cost;
    if(sizeof(cannot)) {
        this_player()->add_failed_mess(this_object(),
          "You cannot pick up $I.\n", cannot);
    }
    if(sizeof(too_much)) {
        if(stringp(too_costly_func)) {
            call_other(this_object(), too_costly_func, this_player(), cannot);
        } else if (functionp(too_costly_func)) {
            evaluate(too_costly_func, this_player(), cannot);
        }
        this_player()->add_failed_mess(this_object(),
          "$I costs too much.\n", too_much);
    }
    if(!sizeof(to_buy)) {
        return 0;
    }
    do_buy_things(to_buy, total_cost, this_player());
    return 1;
} /* do_buy() */

/** @ignore yes */
void do_buy_things(object *obs, int cost, object pl) {
    int i, j;
    string place;
    object money, change;
    mixed m_array, p_array;

    place = query_property("place");
    if(!place || (place == "")) {
        place = "default";
    }
    money = present(MONEY_ALIAS, pl);
    if(!money) {
        if(stringp(too_costly_func)) {
            call_other(this_object(), too_costly_func, this_player(), obs);
        } else if (functionp(too_costly_func)) {
            evaluate(too_costly_func, this_object(), obs);
        }
        this_player()->add_failed_mess(this_object(),
          "You don't have any money.\n", obs);
        return 0;
    }
    change = clone_object(MONEY_OBJECT);
    m_array = (int)MONEY_HAND->create_money_array(cost, place);
    for(i = 0; i < sizeof(m_array); i += 2) {
        p_array = (mixed *)MONEY_HAND->make_payment(m_array[i],
          m_array[i + 1], money, place);
        if(!pointerp(p_array)) {
            continue;
        }
        for(j = 0; j < sizeof(p_array[0]); j += 2) {
            money->adjust_money(-p_array[0][j + 1], p_array[0][j]);
        }
        change->adjust_money(p_array[1]);
    }
    do_parse(buy_mess, obs, pl, 
      (string)MONEY_HAND->money_string(m_array), "");
    if(stringp(buy_func)) {
        call_other(this_object(), buy_func, pl, obs);
    } else if (functionp(buy_func)) {
        evaluate(buy_func, pl, obs);
    }
    if((int)change->move(pl) != MOVE_OK) {
        tell_object(pl, "You are too heavily burdened to accept "
          "your change, so the shopkeeper puts it on the floor.\n");
        change->move(this_object());
    }
    this_object()->made_transaction(cost, obs);
} /* do_buy() */

/**
 * This method lists all the objects in stock.
 * @return 1 on succes, 0 on failure
 * @param do_list_these()
 */
int do_list() {
    object ob;

    if(!is_open(this_player(), 0)) {
        return 0;
    }
    if(objectp(our_storeroom)) {
        ob = our_storeroom;
    } else {
        if ( original_storeroom ) {
            our_storeroom = load_object( original_storeroom );
            ob = our_storeroom;
        }
        else {
            add_failed_mess( "Please notify a creator: the storeroom for "
                "this shop cannot load or has gone missing.\n" );
            return 0;
        }
    }
    if(stringp(list_func)) {
        call_other(this_object(), list_func, this_player());
    } else if (functionp(list_func)) {
        evaluate(list_func, this_player());
    }
    do_parse(list_mess, ({ this_object() }), this_player(), "",
      shop_list(all_inventory(ob), 0));
    return 1;
} /* do_list() */

/**
 * This method lists only the specified objects
 * @return 1 on success, 0 on failure
 * @see do_list()
 */
int do_list_these(object *obs) {
    if(!is_open(this_player(), 0)) {
        return 0;
    }

    do_parse(list_mess, ({this_object()}), 
      this_player(), "", shop_list(obs, 1));
    return 1;
} /* do_list_these() */

/**
 * This method is called when the player is browseing stuff.
 * @param obs the objects to browse
 * @return 1 on success, 0 on failure
 */
int do_browse(object *obs) {
    int i, value;
    string place;

    if (!is_open(this_player(), 0)) {
        return 0;
    }
    place = query_property("place");
    if(!place || (place == "")) {
        place = "default";
    }
    if(stringp(browse_func)) {
        call_other(this_object(), browse_func, this_player(), obs);
    } else if (functionp(browse_func)) {
        evaluate(browse_func, this_player(), obs);
    }
    for(i = 0; i < sizeof(obs); i++) {
        value = (int)this_object()->query_value(obs[i], 0);
        do_parse(browse_mess, obs[i..i], this_player(),
          (string)MONEY_HAND->money_value_string(value, place),
          (string)obs[i]->long());
    }
    return 1;
} /* do_browse() */

/**
 * This method is called when the player is valueing stuff.
 * @param obs the objects to value
 * @return 1 on success, 0 on failure
 */
int do_value(object *obs) {
    int i;
    int val;
    int total;
    string place;
    int obnum;

    if(!is_open(this_player(), 0)) {
        return 0;
    }
    place = query_property("place");
    if(!place || (place == "")) {
        place = "default";
    }
    for(i = 0; i < sizeof(obs); i++) {
        if(obs[i]->do_not_sell() || this_object()->do_not_buy(obs[i]) ||
          (environment(obs[i] ) != this_player()) ||
          (strict_shop &&
            (shop_type != (string)obs[i]->query_property("shop type")))) {
            val = 0;
        } else {
            val = (int)this_object()->query_value(obs[i], 1);
            if (shop_type != (string)obs[i]->query_property("shop type")) {
                val = (val * 90) / 100;
            }
        }
        val = scaled_value(val);
        total += val;
        if(val > (int)this_object()->query_max_amount()) {
            do_parse(too_costly_mess, obs[i..i], this_player(), "",
              (string)obs[i]->do_not_sell());
            total -= val;
        } else if(val < (int)this_object()->query_min_amount()) {
            do_parse(not_worthy_mess, obs[i..i], this_player(), "",
              (string)obs[i]->do_not_sell());
            total -= val;
        } else {
            do_parse(value_mess, obs[i..i], this_player(),
              (string)MONEY_HAND->money_value_string(val, place),
              (string)obs[i]->do_not_sell());
            if(stringp(value_func)) {
                call_other(this_object(), value_func, this_player(), obs,
                  MONEY_HAND->money_string(MONEY_HAND->create_money_array(val,
                      place)));
            } else if (functionp(value_func)) {
                evaluate(value_func, this_player(), obs,
                  MONEY_HAND->money_string(MONEY_HAND->create_money_array(val,
                      place)));
            }
        obnum++;
        }
    }

    if (obnum > 1) {
        write("This gives you a total value of " +
          MONEY_HAND->money_value_string(total, place) +
          ".\n");
        return 1;
    }
    this_player()->add_failed_mess( this_object(), "None of your items are "
        "suitable for sale here!\n", obs );
    return obnum;
} /* do_value() */

/**
 * This method creates the list  for the shop.
 * @param arr the array of objecsts to list
 * @param detail display them in detail?
 * @return the string list
 */
string shop_list(mixed arr, int detail) {
    int i, j, value, num;
    string s, mon, place, *shorts, *vals;
    object *list;
    mapping inv, costs;
    mixed ind;

    if(pointerp(arr)) {
        list = arr;
    } else {
        list = all_inventory(this_object());
    }
    // Only keep track of things with shorts.
    inv = ([ ]);
    for(i = 0; i < sizeof(list); i++) {
        s = (string)list[i]->short();
        if(!s || !this_object()->query_value(list[i], 1)) {
            continue;
        }
        if(!stringp(s)) {
            s = "get a creator for this one!";
        }
        if(inv[s]) {
            inv[s] += ({list[i]});
        } else {
            inv[s] = ({list[i]});
        }
    }
    // Okay, print it.
    s = "";
    shorts = m_indices(inv);
    if(!sizeof(shorts)) {
        if(detail) {
            return "The shop is all out of what you wanted.\n";
        } else {
            return "The shop is totally out of stock.\n";
        }
    }
    s = "You find on offer:\n";
    place = query_property("place");
    if(!place || (place == "")) {
        place = "default";
    }
    for(i = 0; i < sizeof(shorts); i++) {
        ind = inv[shorts[i]];
	num = sizeof(ind);
	if(num == 1 && ind[0]->query_collective())
	  num = ind[0]->query_amount();

        switch(num) {
        case 1:
            s += "Our very last " + shorts[i];
            break;
        case 2..5 :
            s += capitalize(query_num(num, 0)+" "+
			    (string)ind[0]->query_plural());
            break;
        default:
            if(detail) {
                s += capitalize(query_num(num, 0) + " " +
                  (string)ind[0]->query_plural());
            } else {
                s += "A large selection of "+
                (string)ind[0]->query_plural();
            }
        }
        if(detail) {
            costs = ([ ]);
            for(j = 0; j < sizeof(ind); j++) {
                value = (int)this_object()->query_value(ind[j], 0);
                mon = (string)MONEY_HAND->money_value_string(value, place);
                if(!costs[mon]) {
                    costs[mon] = ({""+(j + 1)});
                } else {
                    costs[mon] += ({""+(j + 1)});
                }
            }
            if(m_sizeof(costs) == 1) {
                s += " for "+m_indices(costs)[0];
                if(sizeof(m_values(costs)[0]) > 1) {
                    s += " each.\n";
                } else {
                    s += ".\n";
                }
            } else {
                s += ":-\n";
                vals = m_indices(costs);
                for(j = 0; j < sizeof(vals); j++) {
                    s += "  [#"+implode(costs[vals[j]], ",")+"] for "
                    +vals[j]+".\n";
                }
            }
        } else {
            s += ".\n";
        }
    }
    return s;
} /* shop_list() */

/**
 * This method sets the current store room associated with
 * the shop.  This is important!  A shop needs a storeroom.
 * @param ob the storeroom to set
 * @example
 * set_store_room(PATH+"store_room");
 */
void set_store_room(mixed ob) {
    if(stringp(ob)) {
        original_storeroom = ob;
        our_storeroom = find_object(ob);
        if(!our_storeroom) {
            our_storeroom = load_object(ob);
        }
    }
    else our_storeroom = ob;  
} /* set_store_room() */

/** @ignore yes */
void guards(object tp) {
    object ob;

    if(environment(tp) != this_object() && environment(tp) != our_storeroom) {
        return;
    }
    while(!random(6)) {
        ob = create_mercenary(0);
        ob->move(environment(tp));
        ob->attack_ob(tp);
    }
} /* guards() */

/**
 * This method returns the storeroom associated with the shop.
 * @see set_store_room()
 * @return the current store room
 */
object query_store_room() {
    return our_storeroom;
} /* query_store_room() */

/**
 * This method is the major message processing function for
 * the buye messages, sell messages etc.  It handles calling
 * the functions and setting the results back onto the
 * player.
 * @param arr the value of the message
 * @param ob the objects to process
 * @param money the money string
 * @param extra the extra string
 */
void do_parse(mixed arr, object *ob, object client,
  string money, string extra) {
    if(stringp(arr)) {
        this_player()->show_message("$P$List$P$"+
          this_player()->convert_message(replace(arr, 
              ({"$ob$", query_multiple_short(ob),
                "$client$", this_player()->short(),
                "$money$", money, "$extra$", extra}))));
        this_player()->add_succeeded_mess(this_object(), "", ob);
    } else if (functionp(arr)) {
        evaluate(arr, ob, client, money, extra);
    } else {
        this_player()->show_message("$P$List$P$"+
          this_player()->convert_message(replace(arr[0],
              ({"$ob$", query_multiple_short(ob),
                "$client$", this_player()->short(),
                "$money$", money, "$extra$", extra}))));
        this_player()->add_succeeded_mess(this_object(),
          ({"", replace(arr[1], ({"$ob$", "$I", "$client$",
                "$N", "$money$", money, "$extra$", extra}))}), ob);
    }
} /* do_parse() */

/** 
 * This is a minature version of do_parse(), which can be used when you
 * set the money array to a function pointer in order to make message
 * processing go smoother. 
 * @param str the value of the message
 * @param ob the objects to process
 * @param money the money string
 * @param extra the extra string
 * @return a parsed string
 */
string shop_parse(string str, mixed ob, object client, string money,
  string extra, string which) {
    if(sizeof(ob)) {
        str = replace(str, "$ob$", query_multiple_short(ob, which));
    } else {
        str = replace(str, "$ob$", call_other(ob, which +"_short"));
    }
    if(client) {
        str = replace(str, "$client$", client->the_short());
    }
    str = replace(str, ({"$money$", money, "$extra$", extra}));
    return str;
} /* shop_parse() */

/**
 * This method adds a shop with shich we will exchange inventories
 * of certain types.  So the main shop can sell off its swords and
 * stuff to the sword shop.
 * @param shop the other shop
 */
void add_other_shop(mixed shop) {
    // Should give a nice string telling us the other room.
    other_shops += ({shop});
} /* add_other_shop() */

/**
 * This method returns the type of the shop.  This conttrols what sort
 * of merchandise the shop will buy and sell.
 * <p>The types of allowed shops are:
 * <ul>
 * <li>  jewelery
 * <li>  armoury
 * <li>  clothes
 * <li>  magic
 * <li>  none set (ie: 0, general type)
 * </ul>
 * @return the shop type
 * @see set_shop_type()
 * @see set_strict_shop()
 */
string query_shop_type() {
    return shop_type;
} /* query_shop_type() */

/**
 * This method sets the type of the shop.  This controls what sort of
 * mechandise the shop will buy and sell.
 * <p>The types of allowed shops are:
 * <ul>
 * <li>  jewelery
 * <li>  armoury
 * <li>  clothes
 * <li>  magic
 * <li>  none set (ie: 0, general type)
 * </ul>
 * @see query_shop_type()
 * @see set_strict_shop()
 * @param ty the type of the shop
 */
void set_shop_type(string ty) {
    shop_type = ty;
} /* set_shop_type() */

/**
 * This method sets the strictness of the shop, if the shop is strict
 * it will not deal in items of other types at all.
 * @param i the new strict value
 * @see set_shop_type()
 * @see query_strict_shop()
 */
void set_strict_shop(int i) {
    strict_shop = i;
} /* set_strict_shop() */

/**
 * This method returns the structness of the shop, if the shop is strict
 * it will not deal in items of other types at all.
 * @return the current strict value
 * @see set_shop_type()
 * @see set_strict_shop()
 */
int query_strict_shop() {
    return strict_shop;
} /* query_strict_shop() */

/**
 * This method creates the sales representative which is sent off
 * to exchange goods with other shops.
 * @return the sales representative
 * @see add_other_shop()
 * @see send_out_reps()
 */
object create_rep() {
    object ob;

    ob = clone_object("/obj/monster");
    ob->set_name("rep");
    ob->set_short("sales rep");
    ob->add_adjective("sales");
    ob->set_long("This is tall strong looking sales rep.  He "
      "stares at you with bright piercing eyes.\n");
    ob->add_alias("Sales rep alias");
    ob->set_guild("fighter");
    ob->set_race("human");
    ob->adjust_bon_str(15);
    ob->set_level(60);
    ARMOURY->request_weapon("dagger", 100)->move(ob);
    ARMOURY->request_armour("cloth robe", 100)->move(ob);
    ob->init_equip();
    ob->add_property("rep type", shop_type);
    all_inventory(ob)->add_property("mine", 1);
    return ob;
} /* create_rep() */

// Send out the reps.

/**
 * This method checks to see if there are any other shops
 * associated with ours and sends out representatives to them
 * to exchange goods.
 * @see create_rep()
 * @see add_other_shop()
 */
void send_out_reps() {
    int i;
    object ob;

    for(i = 0; i < sizeof(other_shops); i++) {
        ob = (object)this_object()->create_rep();
        ob->add_property("goto destination", other_shops[i]);
        ob->add_property("goto property", "shop");
        ob->move(this_object(),
          "$N stride$s determinedly into the room.");
        ob->add_triggered_action("froggy", "goto_destination",
          file_name(this_object()), "rep_made_it");
    }
} /* send_out_reps() */

/**
 * This method is called onces the representative
 * reaches its destination.
 * @param bing we mkde it ok
 */
void rep_made_it(int bing) {
    object *obs, rep;
    int i, cost;

    if(!bing) {
        previous_object()->init_command("'Oh no!  I am utterly lost!");
        previous_object()->init_command("sigh");
        call_out("set_up_return", 5, previous_object());
        return ;
    }
    rep = present("Sales rep alias",
      (object)previous_object()->query_current_room());
    obs = (previous_object()->query_current_room())->query_stock(shop_type);
    if(!obs) {
        obs = ({ });
    }
    if(!sizeof(obs)) {
        this_object()->none_to_sell();
        call_out("set_up_return", 5, previous_object());
        return ;
    }
    for(i = 0; i < sizeof(obs); i++) {
        if(obs[i]) {
            cost += (int)this_object()->query_value( obs[ i ], 1 ) * 2 / 3;
        }
    }
    call_out("do_rep_buy", 5, ({previous_object(), obs, cost}));
    cost += (int)this_object()->query_value(obs[i], 1) * 2 / 3;
    call_out("do_rep_buy", 5, ({previous_object(), obs, cost}));
    previous_object()->adjust_value(cost);
} /* rep_made_it() */

/**
 * This method creates a mercenaries to wander along with the
 * sales rep to get the stuff to the destination.
 * @param rep the representative to protect
 * @return the new mercenary
 */
object create_mercenary(object rep) {
    object ob;
    string nam;

    if(rep) {
        nam = implode(rep->query_adjectives(), " ")+" "+rep->query_name();
    }
    ob = clone_object("/obj/monster");
    ob->set_name("mercenary");
    ob->add_alias("troll");
    ob->add_adjective("troll");
    ob->set_short("troll mercenary");
    ob->set_main_plural("troll mercenaries");
    ob->set_race("troll");
    ob->set_class("fighter");
    ob->set_level(200 + random(200));
    ob->set_long("This is a large, hulking troll.  He looks "
      "quite competent and capable of mashing you with or "
      "without a weapon.\n");
    if(rep) {
        ob->move(environment(rep));
        ob->do_command("follow "+nam);
        ob->do_command("protect "+nam);
        ob->add_property("merchant", rep);
    } else {
        ARMOURY->request_weapon("spiked club", 100)->move(ob);
    }
    ob->set_natural(1);
    ob->init_equip();
    ob->set_join_fights("Troll yells something incomprehensible.\n");
    ob->set_join_fight_type(0); // So they only beat up players.
    return ob;
} /* create_mercenry() */

/* The shop types are:
 *   jewelery
 *   armoury
 *   clothes
 *   magic
 *   none set (ie: 0, general type)
 */

/**
 * This method returns alkl the stock in the shop of various
 * types.  The types are:
 * <ul>
 * <li>  jewelery
 * <li>  armoury
 * <li>  clothes
 * <li>  magic
 * <li>  none set (ie: 0, general type)
 * </ul>
 * @param type the type of stock to return
 * @return the array of objects of the type
 */
object *query_stock(string type) {
    mapping blue;

    blue = (mapping)our_storeroom->query_shop_type_mapping();
    if(!blue[type]) {
        return ({ });
    }
    return blue[type];
} /* query_stock() */

/**
 * This method is called when the rep tries to buy stuff.
 * @param bing the stuff to buy
 */
void do_rep_buy(mixed *bing) {
    object rep, *obs;
    int cost;

    rep = bing[0];
    obs = bing[1];
    cost = bing[2];
    rep->adjust_money(cost, "brass");
    rep->query_current_room()->do_buy(obs, cost, rep);
    obs->move(present("Sales rep alias", rep->query_current_room()));
    call_out("set_up_return", 5, rep);
} /* do_rep_buy() */

/**
 * This method sets up the rep to return home.
 * @param rep the rep to return home
 */
void set_up_return(object rep) {
    rep->add_property("goto destination", file_name(this_object()));
    rep->add_triggered_action("froggy", "goto_destination",
      this_object(), "rep_came_back");
} /* set_up_return() */

/**
 * This method is called when the rep gets back home.
 */
void rep_came_back() {
    int i;
    object *obs, *obs2, rep;

    obs = previous_object()->find_inv_match("all", previous_object());
    obs2 = ({ });
    for(i = 0; i < sizeof(obs); i++) {
        if(obs[i]->query_property("mine")) {
            continue;
        }
        if(obs[i]->query_property("money")) {
            continue;
        }
        obs[i]->move(our_storeroom);
        obs2 += ({obs[i]});
    }
    if(sizeof(obs2)) {
        tell_room(this_object(), previous_object()->short()+" puts "+
          query_multiple_short(obs2)+" into the stock.\n");
    }
    rep = present("Sales rep alias",
      (object)previous_object()->query_current_room());
    obs = all_inventory(this_object());
    obs2 = ({ });
    for(i = 0; i < sizeof(obs); i++) {
        if((object)obs[i]->query_property("merchant") == rep) {
            obs2 += ({ obs[i] });
        }
    }
    if(sizeof(obs2)) {
        tell_room(this_object(), 
          query_multiple_short(obs2+({previous_object()}), "one")+" go away.\n");
    } else {
        tell_room(this_object(), previous_object()->short()+" goes away.\n");
    }
    obs2->dest_me();
    previous_object()->dest_me();
} /* rep_came_back() */

// Used to create guards to protect the shop.

/**
 * This method is yused to handle guards to protect the shop.
 * @param tp the object which is shop lifting
 * @see event_shoplift()
 * @see set_shoplift_handler()
 */
void summon_guards(object tp) {
    object ob;
    int i;

    if(environment(tp) != this_object()) {
        return;
    }
    ob = environment(tp)->create_mercenary(0);
    if(!ob) {
        return;
    }
    ob->move(this_object(),
      "$N charge$s in to protect the shop!");
    ob->attack_ob(tp);
    for(i = 0; i < random(5); i++) {
        ob = create_mercenary(0);
        ob->move(this_object(),
          "$N charge$s in to protect the shop!");
        ob->attack_ob(tp);
    }
} /* summon_guards() */

/**
 * This method controls whether or not a shop will accept currency
 * from the "default" area.
 * @param new_strict set this to 1 if the shop should only accept currency
 * from the current area it is in (determined by the "place" property).
 */
void set_strict_currency( int new_strict ) {
    _strict_currency = new_strict;
} /* set_strict_currency() */

/** 
 * This returns the value of the strict currency variable.
 * @return 0 if default currency is accepted, 1 if it is not.
 */
int query_strict_currency() {
    return _strict_currency;
} /* query_strict_currency() */
    

/**
 * This method is called when a shop lift is done on the shop.
 * @param command_ob the command object
 * @param thief the theif doing the shop lifting
 * @param victim the victim of the shoplifiting, us I guess :)
 * @see event_shoplift()
 * @see set_shoplift_handler()
 */
void event_shoplift(object command_ob, object thief, object victim) {
    if(stringp(shoplift_handler)) {
        if(shoplift_handler != "none") {
            shoplift_handler->handle_shoplift(thief, victim);
        }
    } else if (functionp(shoplift_handler)) {
        evaluate(shoplift_handler, thief, victim);
    } else {
        "/obj/handlers/theft_handler"->handle_shoplift(thief, victim);
    }
} /* event_shoplift() */

/**
 * This method is used by the shop to tidyup its inventory.  It does this by
 * desting objects at random until it has reduced the inventory to 3/4 of its
 * maximum.
 */
void tidy_inventory() {
    object storeob;
    object *inventory;
    int i, inv_to_leave;
    int count;

    inv_to_leave = max_inventory - (max_inventory / 4);

    if(objectp(our_storeroom)) {
        storeob = our_storeroom;
    } else {
        our_storeroom->rabbit_away();
        storeob = find_object(our_storeroom);
    }

    if(sizeof(all_inventory(storeob)) < inv_to_leave)
        return;

    inventory = all_inventory(storeob);

    while(sizeof(inventory) > inv_to_leave) {
        i = random(sizeof(inventory));
        if(inventory[i])
            inventory[i]->dest_me();

        // safety code.
        if(count++ > 500)
            break;

        inventory = all_inventory(storeob);    
    }

}


/** @ignore yes */
void dest_me() {
    if(our_storeroom)
        our_storeroom->dest_me();
    ::dest_me();
}

/** @ignore yes */
mixed *stats() {
    return ::stats()+({
      ({"total sold", amount_sold}),
      ({"total bought", amount_bought}),
      ({"shop type", shop_type}),
      ({"shoplift handler", shoplift_handler }),
      ({"strict shop", strict_shop})});
} /* stats() */

