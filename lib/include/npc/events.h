/**
 * This file contains all the different sorts of npc events that can
 * be generated.
 * @author Pinkfish
 * @started Tue Aug  4 19:16:53 PDT 1998
 */
#ifndef __NPC_EVENTS_H__
/** @ignore yes */
#define __NPC_EVENTS_H__

/**
 * This is the event for losing hps.
 */
#define EVENT_LOSE_HPS "lose hps"
/**
 * This is the event for gaining hps.
 */
#define EVENT_GAIN_HPS "gain hps"
/**
 * This is the event for reaching a fully healed state.
 */
#define EVENT_FULLY_HEALED "100% hps"
/**
 * This is the event for crossing the 75% hp barrier.
 */
#define EVENT_75PERCENT_HPS "75% hps"
/**
 * This is the event for crossing the 50% hp barrier.
 */
#define EVENT_50PERCENT_HPS "50% hps"
/**
 * This is the event for crossing the 25% hp barrier.
 */
#define EVENT_25PERCENT_HPS "25% hps"
/**
 * This is the event when the npc has moved.
 */
#define EVENT_MOVED "moved"
/**
 * This is the event for when the item is given.
 */
#define EVENT_GIVEN "given"

/**
 * This is the basis for a finished plan event.
 */
#define EVENT_PLAN_FINISHED_TAG "plan finished : "
/**
 * THis is the macro to create a finished plan event
 */
#define EVENT_PLAN_FINISHED(PLAN) (EVENT_PLAN_FINISHED_TAG + PLAN)

#endif
