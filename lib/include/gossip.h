/**
 * The header file for use with the gossip effects.
 * @author Pinkfish
 * @see /obj/handlers/gossip_handler.c
 * @see /std/effects/npc/gossip.c
 */
#ifndef __GOSSIP_H
/** @ignore yes */
#define __GOSSIP_H

/**
 * The path of the gossip handler. 
 * The handler keep tracks of the
 * saved gossip.
 * @see /obj/handlers/gossip_handler.c
 */
#define GOSSIP_HANDLER "/obj/handlers/gossip_handler"

/** The number of gossip message to save. */
#define GOSSIP_KEEP_LEVEL 50
/** The number of rumour messages to save */
#define RUMOUR_KEEP_LEVEL 20


/** Where to save the gossip information. */
#define GOSSIP_SAVE_FILE "/save/gossip"

#endif
