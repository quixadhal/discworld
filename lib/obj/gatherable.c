/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: gatherable.c,v 1.2 2000/06/23 03:40:56 pinkfish Exp $
 * $Log: gatherable.c,v $
 * Revision 1.2  2000/06/23 03:40:56  pinkfish
 * Fix up the references to find_match.
 *
 * Revision 1.1  1998/01/06 04:58:17  ceres
 * Initial revision
 * 
*/
/*
 * gatherable - a base for adding "gatherable" items to a room.  This object
 *    simply allows the gathering handler to use find_match(), which makes
 *    handling aliases easier.  Generally, these would be hidden
 *    objects in a room, with add_alias() and add_adjective() used to
 *    distinguish between them.  The long() can be changed to something
 *    more descriptive if the item is in plain sight (ie, it doesn't
 *    require skill to find).
 */

#include <parse_command.h>

inherit "/std/object";

string item_name;   // Name in gathering handler
int scarcity;       // percent chance of it being in this room
string get_failure_mess = "Use \"gather\"\n";

void create() {
    ::create();
    reset_get();
}

void set_item_name(string name) { item_name = name; }

string query_item_name() { return item_name; }

void set_scarcity(int s) { scarcity = s; }

int query_scarcity() { return scarcity; }

void set_get_failure_mess(string s) { get_failure_mess = s; }

int do_get() {
    // This is to keep someone from trying to get the hidden gatherable.
    return notify_fail( get_failure_mess );
}

/*
string *parse_command_id_list() {
    // Learned recognition would probably go in here
    return ::parse_command_id_list();
}

object query_parse_id( mixed *args ) {
    //printf("parse_id args = %O\n", args);
    return ::query_parse_id( args );
}
*/
