/**
 * This is the standard guild inheritable for outside rooms. 
 * It makes a room the into
 * a place where players can advance and do all those other wonderful
 * guild things you would expect in a guild room.
 * @author No idea
 * @revision ceres July 1999
 * Change over to the new guild costing mechanisms.
 */
inherit "/std/room/outside";
inherit "/std/room/inherit/guild";

void create() {
  guild::create();
  outside::create();
} /* create() */

void init() {
  outside::init();
  guild::init();
} /* init() */
