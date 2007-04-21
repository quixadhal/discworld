/**
 * The include file to deal with transport style npcs.
 * @author Pinkfish
 * @started Mon Nov 15 02:07:32 PST 1999
 */
#ifndef __NPC_TRANSPORT_H
/** @ignore yes */
#define __NPC_TRANSPORT_H

/**
 * This means the transport is not currently moving.
 */
#define RIDE_NOT_MOVING   0
/**
 * This means the transport is currently walking.
 * These values are also used to set the maximum speed of the transport.
 */
#define RIDE_WALK   1
/**
 * This means the transporet is currently trotting.
 */
#define RIDE_TROT   2
/**
 * This means the transport is currently cantering.
 */
#define RIDE_CANTER 3
/**
 * This means the transport is currently galloping.
 */
#define RIDE_GALLOP 4

/**
 * This is the object used to repeat messages onto the
 * rest of the world.
 */
#define TRANSPORT_REPEATER "/std/npc/types/helper/repeater"
/**
 * This is the effect used when someone is riding the transport.
 */
#define TRANSPORT_EFFECT   "/std/effects/attached/riding"
/**
 * This is the effect used to setup ownership of the transport.
 */
#define TRANSPORT_OWNER_EFFECT "/std/effects/attached/owned_transport"

/**
 * This is the property used to control who is allowed to use the
 * owner's horses.
 */
#define TRANSPORT_ALLOW_PROP "transport allow"

/**
 * This is the basic inheritable for a transport.
 */
#define TRANSPORT_INHERIT "/std/npc/types/transport"

/**
 * This is the property used on things to make them able to be used
 * as a tether by a horse.
 */
#define TRANSPORT_TETHER_PROP "tether"

/**
 * This is the property used on things to make them able to have horses
 * tethered to them.
 */
#define TRANSPORT_TETHER_POST_PROP "tether post"

#endif
