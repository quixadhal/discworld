/**
 * This file has all the defines for offer/accept npc handlers.
 * @author Pinkfish
 * @started Tue Aug 18 06:47:13 EDT 1998
 */
#ifndef __OFFER_NPC_H__
/** @ignore yes */
#define __OFFER_NPC_H__

/**
 * The class for offering a sale to another player/npc.
 */
class offer_sale {
   /**
    * The value of the sale.
    */
   int value;
   /**
    * The place in which to handle the money.
    */
   string place;
   /**
    * The objects being sold.
    */
   object *sell_obs;
   /**
    * This is the person who does the selling.
    */
   object seller;
   /**
    * This is the person doing the buying.
    */
   object buyer;
}

#endif
