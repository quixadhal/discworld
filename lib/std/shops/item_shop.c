/* -*- LPC -*- */
/*
 * $Locker:  $
 * $Id: item_shop.c,v 1.4 2003/07/15 11:17:48 taffyd Exp $
 */
/**
 * This is the default item shop code.  An item shop is a shop that only
 * sells specific items.
 * @author Pinkfish
 * @started Wed Oct 14 16:42:21 PDT 1998
 * @see /std/shops/inherit/item_shop
 */
#include <armoury.h>
#include <money.h>
#include <move_failures.h>

inherit "/std/room/basic_room";
inherit "/std/shops/inherit/item_shop";

void create() {
  do_setup++;
  basic_room::create();
  item_shop::create();
  do_setup--;

  if ( !do_setup ) {
    this_object()->setup();
    this_object()->reset();
  }
} /* create() */

/** @ignore yes */
void init() {
   basic_room::init();
   item_shop::init();
} /* init() */

/** @ignore yes */
void event_theft( object command_ob, object thief, object victim,
  object *stolen ) {

    basic_room::event_theft( command_ob, thief, victim, stolen );
    item_shop::event_theft( command_ob, thief, victim, stolen ); 
} /* event_theft() */ 

/** @ignore yes */
void dest_me() {
   item_shop::dest_me();
   basic_room::dest_me();
} /* dest_me() */
