/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: voting_room.c,v 1.2 2001/02/20 03:25:41 pinkfish Exp $
 *
 */

#include <board.h>

#define DEFAULT_VOTE_DURATION 2600*24*7

inherit "/std/room/basic_room";
inherit "/std/room/inherit/voting_room";

/**
 * Generalised voting room intended to be inherited into rooms in the mud that
 * will setup the parameters and settings for votes.
 * <p>
 * The room does not inherantly support player initiated votes, however it is
 * a fairly simple matter to add this to your own room and have your room add
 * votes when players initiate them.
 *
 * @author Ceres
 */

/**
 * @ignore
 */
void create() {
  voting_room::create();
  do_setup++;
  add_help_file("voting_room");
  basic_room::create();
  do_setup--;
  if ( !do_setup ) {
    this_object()->setup();
    this_object()->reset();
  }
  seteuid("Room");
}

/**
 * @ignore
 */
void init() {
   basic_room::init();
   voting_room::init();
} /* init() */
