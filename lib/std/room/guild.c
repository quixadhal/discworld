/**
 * This is the standard guild inheritable.  It makes a room the into
 * a place where players can advance and do all those other wonderful 
 * guild things you would expect in a guild room.
 * @author Pinkfish
 * @revision ceres July 1999
 * Change over to the new guild costing mechanisms.
 */
inherit "/std/room/basic_room";
inherit "/std/room/inherit/guild";

void create() {
  guild::create();
  basic_room::create();
} /* create() */

void init() {
  basic_room::init();
  guild::init();
} /* init() */
