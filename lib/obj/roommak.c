/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: roommak.c,v 1.1 1998/01/06 04:58:17 ceres Exp $
 * $Log: roommak.c,v $
 * Revision 1.1  1998/01/06 04:58:17  ceres
 * Initial revision
 * 
*/
/*
 * Room maker.  Pinkfish version.
 * Started 2-10-92.
 * Version 0.01
 */
inherit "/std/object";

mixed *exits,
      *items;
string de_long,
       de_short;
int de_light;

void setup() {
  set_name("maker");
  add_adjective("room");
  set_short("Room maker");
  set_long("It is a small wooden rod about 30 centimeters long with deep "+
           "burn marks along the sides.  It has what looks like a small "+
           "lens stuck in the end and three buttons along its length.\n");
}

void init() {
  if (this_player()->query_creator()) {
    add_action("do_new", "new"); /* Creates a new room */
    add_action("do_set", "set");   /* Allows you to seperately set things */
    add_action("do_add", "add");   /* Adds things.  ie doors etc */
    add_action("do_setup", "setup"); /* Sets up a bunch of default things. */
    add_action("do_modify", "modify"); /* Changes things :) */
    add_action("do_map", "map");   /* Draws a nice little map */
  }
}

/*
 * Examples of usage:
 * new bing
 * set short
 * set long
 * add exit
 * modify exit
 */

int do_new(string str) {
  if (!str) {
    notify_fail("Syntax: new <name>\n");
    return 0;
  }
  
}
