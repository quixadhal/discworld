/* -*- LPC -*- */
/*
 * $Locker:  $
 * $Id: item_shop_outside.c,v 1.3 2003/07/15 11:17:34 taffyd Exp $
 */
/**
 * This is the default item shop code.  An item shop is a shop that only
 * sells specific items.  This is the outside version of it.
 * @author Pinkfish
 * @started Wed Oct 14 16:42:21 PDT 1998
 * @see /std/shops/inherit/item_shop
 */
#include <armoury.h>
#include <money.h>
#include <move_failures.h>

inherit "/std/room/outside";
inherit "/std/shops/inherit/item_shop";

void create() {
   item_shop::create();
   outside::create();
} /* create() */

/** @ignore yes */
void init() {
   outside::init();
   item_shop::init();
} /* init() */

/** @ignore yes */
void event_theft( object command_ob, object thief, object victim,
  object *stolen ) {

    outside::event_theft( command_ob, thief, victim, stolen );
    item_shop::event_theft( command_ob, thief, victim, stolen ); 
} /* event_theft() */ 

/** @ignore yes */
void dest_me() {
   item_shop::dest_me();
   outside::dest_me();
} /* dest_me() */
